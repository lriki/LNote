//=============================================================================
// ■
//-----------------------------------------------------------------------------
//  
//=============================================================================

//-------------------------------------------------------------------------
// 定数レジスタ
//-------------------------------------------------------------------------

float4x4 gWorldViewProjMatrix   : WORLDVIEWPROJECTION;
float4x4 gWorldViewMatrix       : WORLDVIEW;

//-------------------------------------------------------------------------
// テスクチャ
//-------------------------------------------------------------------------
texture gTexture    : MATERIALTEXTURE;
sampler gTexSampler = sampler_state
{
    texture = < gTexture >;
    MINFILTER = NONE;
    MAGFILTER = NONE;
};

texture gToonTexture    : TOONTEXTURE;
sampler gToonSampler = sampler_state
{
    texture = < gToonTexture >;
    MINFILTER = NONE;
    MAGFILTER = NONE;
};

//-------------------------------------------------------------------------
// 出力構造体
//-------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos          : POSITION;
   
    float4 Color        : COLOR0;
    float2 TexUV        : TEXCOORD0;
    float2 ToonTexUV    : TEXCOORD1;
};

//-------------------------------------------------------------------------
// VS
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(
    float4 in_pos_      : POSITION,
    float3 in_normal_   : NORMAL,
    float4 in_color_    : COLOR0,
    float2 in_uv_       : TEXCOORD0 )
{
    VS_OUTPUT o;

    o.Pos   = mul( in_pos_, gWorldViewProjMatrix );
    o.Color = in_color_;
    o.TexUV = in_uv_;
    o.ToonTexUV.y = 0.0f;
    
    float3 wv_pos=normalize(mul(in_pos_, gWorldViewMatrix).xyz);
	float3 wv_normal=normalize(mul(in_normal_, (float3x3)gWorldViewMatrix));
    //o.ToonTexUV.y=abs( dot(wv_pos, wv_normal) );
    o.ToonTexUV.x= dot(wv_pos, wv_normal)*0.5+0.5;
    
    //if ( o.ToonTexUV.y > 0.1 ) { o.ToonTexUV.y = 1.0; }

    return o;
}

//-------------------------------------------------------------------------
// PS
//-------------------------------------------------------------------------
float4 psBasic(
    float4 in_color_    : COLOR0,
    float2 in_uv_       : TEXCOORD0,
    float2 in_toon_uv_  : TEXCOORD1 ) : COLOR0
{
    /*
    /if ( abs(in_toon_uv_.y) > 0.1 ) { in_toon_uv_.y = 1.0; }
    
    float4 out_color = tex2D( gTexSampler, in_uv_ );
    
    out_color.rgb *= in_toon_uv_.y;
    */
    
    float4 out_color = tex2D(gTexSampler, in_uv_)*tex2D(gToonSampler, in_toon_uv_);//*Diffuse;
    
    return out_color;// * in_color_;
}

//-------------------------------------------------------------------------
// テクニック宣言
//-------------------------------------------------------------------------
technique MainDraw
{
    pass P0
    {
        VertexShader = compile vs_3_0 vsBasic();
        PixelShader	 = compile ps_2_0 psBasic();
    }
}

//=============================================================================
//                              end of file
//=============================================================================