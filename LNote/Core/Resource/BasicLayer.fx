//=============================================================================
// ■ BasicLayer
//-----------------------------------------------------------------------------
//  レイヤーを描画するためのシェーダプログラム。
//=============================================================================

//-------------------------------------------------------------------------
// 定数レジスタ
//-------------------------------------------------------------------------
float4          gColor;             // カラースケールと不透明度乗算済み
float4          gBlendColor;        // ブレンドカラー
float4          gTone;              // 色調

float           gBlurPower;         // ブラーの強さ
float4          gBlurColor;         // ブラーの色
float4x4        gBlurMatrix;        // ブラーイメージの座標変換行列

float           gTransitionTime;    // トランジションの時間 (-gTransitionVague～1.0)
float           gTransitionVague;   // トランジション境界のあいまいさ (色の範囲。0.0～1.0)

float2          gViewportSize;      // 描画先の RT サイズ
static float2   gViewportOffset = ( float2( 0.5, 0.5 ) / gViewportSize );   // 0.5 ピクセルのオフセット

//-------------------------------------------------------------------------
// テスクチャ
//-------------------------------------------------------------------------
// LN_LAYERTYPE_BUFFERINGDRAW フラグが指定されているときに作成される内部 RT
texture gTexture;
sampler gTexSampler = sampler_state
{
    texture = < gTexture >;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
    AddressU  = CLAMP;
    AddressV  = CLAMP;
};

// ブラー用の 1フレーム前のイメージ
texture gSecondaryTexture;
sampler gSecondarySampler = sampler_state
{
    Texture = < gSecondaryTexture >;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
    AddressU  = CLAMP;
    AddressV  = CLAMP;
};

// トランジション用のフリーズされたスクリーンのイメージ
texture gTransitionFreezedTexture;
sampler gTransitionFreezedSampler = sampler_state
{
    Texture = < gTransitionFreezedTexture >;
    MinFilter = None;
    MagFilter = None;
};

// トランジション用マスクテクスチャ
texture gTransitionMaskTexture;		
sampler gTransitionMaskSampler = sampler_state
{
    Texture = < gTransitionMaskTexture >;
    MinFilter = None;
    MagFilter = None;
};

//-------------------------------------------------------------------------
// 出力構造体
//-------------------------------------------------------------------------
struct VS_OUTPUT 
{
    float4 Pos      : POSITION;
    float2 TexUV    : TEXCOORD0;
};

//-------------------------------------------------------------------------
// ●【 頂点シェーダ 】
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(
    float3 in_pos_  : POSITION,
    float2 in_uv_   : TEXCOORD0 )
{
    VS_OUTPUT o;
    
    o.Pos = float4( in_pos_, 1.0f );
    o.TexUV = in_uv_ + gViewportOffset;

    return o;
}

//-------------------------------------------------------------------------
// ●【 ピクセルシェーダ 】
//-------------------------------------------------------------------------
float4 psBasic(
    float2 in_uv_   : TEXCOORD0  ) : COLOR
{
    float4 out_color = tex2D( gTexSampler, in_uv_ ) * gColor;
    
    // ブレンドの計算
    //out_color.rgb = ( gBlendColor.rgb * gBlendColor.a ) + ( out_color.rgb * ( 1.0 - gBlendColor.a ) );
    out_color.rgb = lerp( out_color.rgb, gBlendColor.rgb, gBlendColor.a );
    
    // 色調の計算。(NTSC 系加重平均法というらしい。緑だけがなんとなく明るく感じるとかを防ぐ)
	float y = ( 0.208012 * out_color.r + 0.586611 * out_color.g + 0.114478 * out_color.b ) * gTone.w;
    out_color.rgb = ( out_color.rgb * ( 1.0 - gTone.w ) ) + y + gTone.rgb;

	return out_color;
}

//-------------------------------------------------------------------------
// ●【 ピクセルシェーダ 】トランジション
//-------------------------------------------------------------------------
float4 psTransiton(
    float2 		 in_uv_   : TEXCOORD0,
	uniform bool use_mask_textre_ ) : COLOR
{
	float4 out_color = psBasic( in_uv_ );
	
	// マスクテクスチャがある場合
	if ( use_mask_textre_ )
	{
		float alpha = tex2D( gTransitionMaskSampler, in_uv_ ).r;
		
		// 移り変わる前のイメージのアルファ値
		alpha = ( alpha - gTransitionTime ) / gTransitionVague;
		
		// 移り変わる前と後を補間して完了
		out_color.rgb = lerp( out_color.rgb, tex2D( gTransitionFreezedSampler, in_uv_ ).rgb, clamp( alpha, 0.0, 1.0 ) );
	}
	// マスクテクスチャがない場合は全体を単純に遷移させる
	else
	{
		out_color.rgb = lerp( out_color.rgb, tex2D( gTransitionFreezedSampler, in_uv_ ).rgb, (1.0 - gTransitionTime) );
	}
	
	return out_color;
}

//-------------------------------------------------------------------------
// ● VS ダブルバッファリングブラー
//-------------------------------------------------------------------------
VS_OUTPUT vsBufferingBulr(
    float3 in_pos_  : POSITION,
    float2 in_uv_   : TEXCOORD0 )
{
    VS_OUTPUT o;
    o.Pos = mul( float4( in_pos_, 1.0f ), gBlurMatrix );
    
    // ここもピクセルオフセット入れておかないと、拡大率 1.0 でのブラーの再、
    // ブラーが 0.5 ピクセル分右↓に移動していってしまう
    o.TexUV = in_uv_ + gViewportOffset;
    return o;
}

//-------------------------------------------------------------------------
// ● PS ダブルバッファリングブラー
//-------------------------------------------------------------------------
float4 psBufferingBulr(
    float2 		 in_uv_   : TEXCOORD0 ) : COLOR
{
	float4 out_color = tex2D( gSecondarySampler, in_uv_ );
	out_color.a *= gBlurPower;
    
    
    // ブラーを青っぽくする
    //out_color.rgb *= float3(0, 0, 1);
    
    // 外側ほど透明になるようにする
    //float len = length( in_uv_ - 0.5 );
    //color.a *= 1.0 - len;
    
    return out_color;
}

//-------------------------------------------------------------------------
// ■ テクニック宣言
//-------------------------------------------------------------------------
// LN_LAYERTYPE_BUFFERINGDRAW フラグが指定されているときの内部 RT の描画 (トランジション等のエフェクト無し)
technique MainDraw
{
    pass P0
    {
        VertexShader = compile vs_2_0 vsBasic();
        PixelShader	 = compile ps_2_0 psBasic();
    }
}

// マスクなしトランジション
technique TransitonNotMask
{
    pass P0
    {
        VertexShader = compile vs_2_0 vsBasic();
        PixelShader	 = compile ps_2_0 psTransiton( false );
    }
}

// マスク付きトランジション
technique TransitonWithMask
{
    pass P0
    {
        VertexShader = compile vs_2_0 vsBasic();
        PixelShader	 = compile ps_2_0 psTransiton( true );
    }
}

// ブラー
technique BufferingBlur
{
    pass P0
    {
        VertexShader = compile vs_2_0 vsBufferingBulr();
        PixelShader	 = compile ps_2_0 psBufferingBulr();


        ZENABLE = FALSE;
        ZWRITEENABLE = FALSE;

        // レンダリングターゲットがアルファ付きでも、描画後に中途半端なアルファ値にならないようにする設定
        
        COLORWRITEENABLE = ALPHA | BLUE | GREEN | RED;
        BLENDOP = ADD;
        SRCBLEND = SRCALPHA;
        DESTBLEND = INVSRCALPHA;
        SEPARATEALPHABLENDENABLE = TRUE;
        SRCBLENDALPHA = SRCALPHA;
        DESTBLENDALPHA = DESTALPHA;
        

        // 普通のアルファブレンド
        /*
        ALPHABLENDENABLE = TRUE;
        SRCBLEND = SRCALPHA;
        DESTBLEND = INVSRCALPHA;
        */

        // 加算ブレンド
        /*
        ALPHABLENDENABLE = TRUE;
        SRCBLEND = SRCALPHA;
        DESTBLEND = ONE;
        */
        
    }
}

//=============================================================================
//                              end of file
//=============================================================================