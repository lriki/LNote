//=============================================================================
//�y RenderState �z
//-----------------------------------------------------------------------------
///**
//  @file       RenderState.h
//  @brief      RenderState
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Types.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{

//=============================================================================
// �� LRenderState �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �����_�����O�X�e�[�g
//*/
//=============================================================================
class LRenderState
{
public:

	LNBlendMode         BlendMode;
	LNCullingMode	    Culling;
    LNFillMode          FillMode;       ///< (Opengl ES �ł͖���)
	bool        	    AlphaTest;      ///< (Opengl ES �ł͖���)
	bool        	    DepthTest;
	bool        	    DepthWrite;
    bool                PointSprite;

    //Geometry::Rect             ScissorRect;
    //bool	            ScissorTest;

public:

    static const LRenderState BUFFER_COPY_STATE;    ///< �P���ɉ�ʑS�̂�ʂ� RT �ɃR�s�[����ړI�Ŏg���X�e�[�g
    static const LRenderState SMOOTH_ALPHA;         ///< �ʏ�̃A���t�@�u�����h�Bbool �t���O�͂��ׂ� false

public:

    /// �f�t�H���g�l
    LRenderState()
        : BlendMode     ( LN_BLEND_ALPHA )
        , Culling       ( LN_CULL_FRONT )
        , FillMode      ( LN_FILL_SOLID )
        , AlphaTest     ( true )
        , DepthTest     ( true )
        , DepthWrite    ( true )
        , PointSprite   ( false )
    {}

    /// �l���w�肵�č쐬
    LRenderState( LNBlendMode blend_mode_, LNCullingMode culling_, LNFillMode fill_mode_, bool alpha_test_, bool depth_test_, bool depth_write_, bool point_sprite_ )
        : BlendMode     ( blend_mode_ )
        , Culling       ( culling_ )
        , FillMode      ( fill_mode_ )
        , AlphaTest     ( alpha_test_ )
        , DepthTest     ( depth_test_ )
        , DepthWrite    ( depth_write_ )
    {}

    

};


//=============================================================================
// �� LSamplerState �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �T���v���X�e�[�g
//*/
//=============================================================================
class LSamplerState
{
public:

    LNTextureFilter     MagFilter;  ///< LN_TEXFILTER_POINT, LN_TEXFILTER_LINEAR �̂����ꂩ
    LNTextureFilter     MinFilter;  ///< LN_TEXFILTER_POINT, LN_TEXFILTER_LINEAR �̂����ꂩ
    LNTextureFilter     MipFilter;  ///< LN_TEXFILTER_NONE, LN_TEXFILTER_LINEAR �̂����ꂩ
    LNTextureAddress    AddressU;
    LNTextureAddress    AddressV;

public:

    /// �f�t�H���g�l
    LSamplerState()
        : MagFilter ( LN_TEXFILTER_LINEAR )
        , MinFilter ( LN_TEXFILTER_LINEAR )
        , MipFilter ( LN_TEXFILTER_NONE )
        , AddressU  ( LN_TEXFILTER_REPEAT )
        , AddressV  ( LN_TEXFILTER_REPEAT )
    {}
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================