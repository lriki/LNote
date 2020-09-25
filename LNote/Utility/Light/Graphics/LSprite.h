//=============================================================================
//【 LSprite 】
//-----------------------------------------------------------------------------
///**
//  @file       LSprite.h
//  @brief      LSprite
//  @version    1.0
//  @date       2010/10/10
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Core/Scene/Manager.h"
#include "../../Core/Scene/SceneNode/Sprite.h"
#include "LSceneObject.h"
#include "LTexture.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// ■ LSprite クラス
//-----------------------------------------------------------------------------
///**
//  @brief      スプライトのクラス
//*/
//=============================================================================
class LSprite
    : public LRefClass< LSprite, Core::Scene::Sprite >
    , public LSceneObject2D
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      スプライトを作成する
    //*/
    //---------------------------------------------------------------------
    static LSprite create();

    //---------------------------------------------------------------------
    ///**
    //  @brief      テクスチャを指定してスプライトを作成する
    //
    //  @par
    //              最初からテクスチャのセットされているスプライトを
    //              作成します。create() の後に setTexture() で
    //              テクスチャを設定することと全く同じです。
    //*/
    //---------------------------------------------------------------------
    static LSprite create( LTexture texture_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      テクスチャの設定
    //*/
    //---------------------------------------------------------------------
    void setTexture( LTexture texture_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      テクスチャの取得
    //*/
    //---------------------------------------------------------------------
    LTexture getTexture() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      テクスチャの転送矩形の設定
    //*/
    //---------------------------------------------------------------------
    void setSrcRect( const LRect& rect_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      テクスチャの転送矩形の設定
    //
    //  @param[in]  x_      : 四角形の左上隅の X 座標
    //  @param[in]  x_      : 四角形の左上隅の Y 座標
    //  @param[in]  width_  : 四角形の幅
    //  @param[in]  height_ : 四角形の高さ
    //*/
    //---------------------------------------------------------------------
    void setSrcRect( int x_, int y_, int width_, int height_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      テクスチャの転送矩形の取得
    //*/
    //---------------------------------------------------------------------
    const LRect& getSrcRect() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      反転方法の設定
    //
    //  @param[in]  flags_ : LNFlipFlag のフラグ
    //
    //  @par
    //              表示内容を反転させる方法の設定です。
    //              <br>
    //              縦と横を同時に反転させたい場合は setFlipFlag( LN_FLIP_V | LN_FLIP_H )
    //              と設定します。
    //*/
    //---------------------------------------------------------------------
    void setFlipFlag( u32 flags_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      反転方法の取得
    //*/
    //---------------------------------------------------------------------
    u32 getFlipFlag();


    LN_UTIL_REFCLASS_METHODS( LSprite, Core::Scene::Sprite );
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================