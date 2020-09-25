//=============================================================================
//【 RenderState 】
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
// ■ LRenderState クラス
//-----------------------------------------------------------------------------
///**
//  @brief      レンダリングステート
//*/
//=============================================================================
class LRenderState
{
public:

	LNBlendMode         BlendMode;
	LNCullingMode	    Culling;
    LNFillMode          FillMode;       ///< (Opengl ES では無効)
	bool        	    AlphaTest;      ///< (Opengl ES では無効)
	bool        	    DepthTest;
	bool        	    DepthWrite;
    bool                PointSprite;

    //Geometry::Rect             ScissorRect;
    //bool	            ScissorTest;

public:

    static const LRenderState BUFFER_COPY_STATE;    ///< 単純に画面全体を別の RT にコピーする目的で使うステート
    static const LRenderState SMOOTH_ALPHA;         ///< 通常のアルファブレンド。bool フラグはすべて false

public:

    /// デフォルト値
    LRenderState()
        : BlendMode     ( LN_BLEND_ALPHA )
        , Culling       ( LN_CULL_FRONT )
        , FillMode      ( LN_FILL_SOLID )
        , AlphaTest     ( true )
        , DepthTest     ( true )
        , DepthWrite    ( true )
        , PointSprite   ( false )
    {}

    /// 値を指定して作成
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
// ■ LSamplerState クラス
//-----------------------------------------------------------------------------
///**
//  @brief      サンプラステート
//*/
//=============================================================================
class LSamplerState
{
public:

    LNTextureFilter     MagFilter;  ///< LN_TEXFILTER_POINT, LN_TEXFILTER_LINEAR のいずれか
    LNTextureFilter     MinFilter;  ///< LN_TEXFILTER_POINT, LN_TEXFILTER_LINEAR のいずれか
    LNTextureFilter     MipFilter;  ///< LN_TEXFILTER_NONE, LN_TEXFILTER_LINEAR のいずれか
    LNTextureAddress    AddressU;
    LNTextureAddress    AddressV;

public:

    /// デフォルト値
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