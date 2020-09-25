//=============================================================================
// �� BasicLayer
//-----------------------------------------------------------------------------
//  ���C���[��`�悷�邽�߂̃V�F�[�_�v���O�����B
//=============================================================================

//-------------------------------------------------------------------------
// �萔���W�X�^
//-------------------------------------------------------------------------
float4          gColor;             // �J���[�X�P�[���ƕs�����x��Z�ς�
float4          gBlendColor;        // �u�����h�J���[
float4          gTone;              // �F��

float           gBlurPower;         // �u���[�̋���
float4          gBlurColor;         // �u���[�̐F
float4x4        gBlurMatrix;        // �u���[�C���[�W�̍��W�ϊ��s��

float           gTransitionTime;    // �g�����W�V�����̎��� (-gTransitionVague�`1.0)
float           gTransitionVague;   // �g�����W�V�������E�̂����܂��� (�F�͈̔́B0.0�`1.0)

float2          gViewportSize;      // �`���� RT �T�C�Y
static float2   gViewportOffset = ( float2( 0.5, 0.5 ) / gViewportSize );   // 0.5 �s�N�Z���̃I�t�Z�b�g

//-------------------------------------------------------------------------
// �e�X�N�`��
//-------------------------------------------------------------------------
// LN_LAYERTYPE_BUFFERINGDRAW �t���O���w�肳��Ă���Ƃ��ɍ쐬�������� RT
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

// �u���[�p�� 1�t���[���O�̃C���[�W
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

// �g�����W�V�����p�̃t���[�Y���ꂽ�X�N���[���̃C���[�W
texture gTransitionFreezedTexture;
sampler gTransitionFreezedSampler = sampler_state
{
    Texture = < gTransitionFreezedTexture >;
    MinFilter = None;
    MagFilter = None;
};

// �g�����W�V�����p�}�X�N�e�N�X�`��
texture gTransitionMaskTexture;		
sampler gTransitionMaskSampler = sampler_state
{
    Texture = < gTransitionMaskTexture >;
    MinFilter = None;
    MagFilter = None;
};

//-------------------------------------------------------------------------
// �o�͍\����
//-------------------------------------------------------------------------
struct VS_OUTPUT 
{
    float4 Pos      : POSITION;
    float2 TexUV    : TEXCOORD0;
};

//-------------------------------------------------------------------------
// ���y ���_�V�F�[�_ �z
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
// ���y �s�N�Z���V�F�[�_ �z
//-------------------------------------------------------------------------
float4 psBasic(
    float2 in_uv_   : TEXCOORD0  ) : COLOR
{
    float4 out_color = tex2D( gTexSampler, in_uv_ ) * gColor;
    
    // �u�����h�̌v�Z
    //out_color.rgb = ( gBlendColor.rgb * gBlendColor.a ) + ( out_color.rgb * ( 1.0 - gBlendColor.a ) );
    out_color.rgb = lerp( out_color.rgb, gBlendColor.rgb, gBlendColor.a );
    
    // �F���̌v�Z�B(NTSC �n���d���ϖ@�Ƃ����炵���B�΂������Ȃ�ƂȂ����邭������Ƃ���h��)
	float y = ( 0.208012 * out_color.r + 0.586611 * out_color.g + 0.114478 * out_color.b ) * gTone.w;
    out_color.rgb = ( out_color.rgb * ( 1.0 - gTone.w ) ) + y + gTone.rgb;

	return out_color;
}

//-------------------------------------------------------------------------
// ���y �s�N�Z���V�F�[�_ �z�g�����W�V����
//-------------------------------------------------------------------------
float4 psTransiton(
    float2 		 in_uv_   : TEXCOORD0,
	uniform bool use_mask_textre_ ) : COLOR
{
	float4 out_color = psBasic( in_uv_ );
	
	// �}�X�N�e�N�X�`��������ꍇ
	if ( use_mask_textre_ )
	{
		float alpha = tex2D( gTransitionMaskSampler, in_uv_ ).r;
		
		// �ڂ�ς��O�̃C���[�W�̃A���t�@�l
		alpha = ( alpha - gTransitionTime ) / gTransitionVague;
		
		// �ڂ�ς��O�ƌ���Ԃ��Ċ���
		out_color.rgb = lerp( out_color.rgb, tex2D( gTransitionFreezedSampler, in_uv_ ).rgb, clamp( alpha, 0.0, 1.0 ) );
	}
	// �}�X�N�e�N�X�`�����Ȃ��ꍇ�͑S�̂�P���ɑJ�ڂ�����
	else
	{
		out_color.rgb = lerp( out_color.rgb, tex2D( gTransitionFreezedSampler, in_uv_ ).rgb, (1.0 - gTransitionTime) );
	}
	
	return out_color;
}

//-------------------------------------------------------------------------
// �� VS �_�u���o�b�t�@�����O�u���[
//-------------------------------------------------------------------------
VS_OUTPUT vsBufferingBulr(
    float3 in_pos_  : POSITION,
    float2 in_uv_   : TEXCOORD0 )
{
    VS_OUTPUT o;
    o.Pos = mul( float4( in_pos_, 1.0f ), gBlurMatrix );
    
    // �������s�N�Z���I�t�Z�b�g����Ă����Ȃ��ƁA�g�嗦 1.0 �ł̃u���[�̍āA
    // �u���[�� 0.5 �s�N�Z�����E���Ɉړ����Ă����Ă��܂�
    o.TexUV = in_uv_ + gViewportOffset;
    return o;
}

//-------------------------------------------------------------------------
// �� PS �_�u���o�b�t�@�����O�u���[
//-------------------------------------------------------------------------
float4 psBufferingBulr(
    float2 		 in_uv_   : TEXCOORD0 ) : COLOR
{
	float4 out_color = tex2D( gSecondarySampler, in_uv_ );
	out_color.a *= gBlurPower;
    
    
    // �u���[����ۂ�����
    //out_color.rgb *= float3(0, 0, 1);
    
    // �O���قǓ����ɂȂ�悤�ɂ���
    //float len = length( in_uv_ - 0.5 );
    //color.a *= 1.0 - len;
    
    return out_color;
}

//-------------------------------------------------------------------------
// �� �e�N�j�b�N�錾
//-------------------------------------------------------------------------
// LN_LAYERTYPE_BUFFERINGDRAW �t���O���w�肳��Ă���Ƃ��̓��� RT �̕`�� (�g�����W�V�������̃G�t�F�N�g����)
technique MainDraw
{
    pass P0
    {
        VertexShader = compile vs_2_0 vsBasic();
        PixelShader	 = compile ps_2_0 psBasic();
    }
}

// �}�X�N�Ȃ��g�����W�V����
technique TransitonNotMask
{
    pass P0
    {
        VertexShader = compile vs_2_0 vsBasic();
        PixelShader	 = compile ps_2_0 psTransiton( false );
    }
}

// �}�X�N�t���g�����W�V����
technique TransitonWithMask
{
    pass P0
    {
        VertexShader = compile vs_2_0 vsBasic();
        PixelShader	 = compile ps_2_0 psTransiton( true );
    }
}

// �u���[
technique BufferingBlur
{
    pass P0
    {
        VertexShader = compile vs_2_0 vsBufferingBulr();
        PixelShader	 = compile ps_2_0 psBufferingBulr();


        ZENABLE = FALSE;
        ZWRITEENABLE = FALSE;

        // �����_�����O�^�[�Q�b�g���A���t�@�t���ł��A�`���ɒ��r���[�ȃA���t�@�l�ɂȂ�Ȃ��悤�ɂ���ݒ�
        
        COLORWRITEENABLE = ALPHA | BLUE | GREEN | RED;
        BLENDOP = ADD;
        SRCBLEND = SRCALPHA;
        DESTBLEND = INVSRCALPHA;
        SEPARATEALPHABLENDENABLE = TRUE;
        SRCBLENDALPHA = SRCALPHA;
        DESTBLENDALPHA = DESTALPHA;
        

        // ���ʂ̃A���t�@�u�����h
        /*
        ALPHABLENDENABLE = TRUE;
        SRCBLEND = SRCALPHA;
        DESTBLEND = INVSRCALPHA;
        */

        // ���Z�u�����h
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