//=============================================================================
//【 LBatchPanel 】
//-----------------------------------------------------------------------------
///**
//  @file       LBatchPanel.h
//  @brief      LBatchPanel
//  @version    1.0
//  @date       2010/10/06
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Core/Scene/Manager.h"
#include "../../Core/Scene/SceneNode/BatchSprite.h"
#include "LSceneObject.h"
#include "LTexture.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// ■ LBatchPanel クラス
//-----------------------------------------------------------------------------
///**
//  @brief      バッチ処理されるパネルオブジェクトのクラス
//*/
//=============================================================================
class LBatchPanel
    : public LRefClass< LBatchPanel, Core::Scene::BatchSprite3D >
    , public LSceneObject3D
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      パネルオブジェクトを作成する
    //
    //  @param[in]  width_   : 横方向のサイズ
    //  @param[in]  height_  : 縦方向のサイズ
    //  @param[in]  dir_     : 初期状態の面の方向
    //
    //  @par
    //              dir_ を省略した場合は、四角形の中心を原点として
    //              Z- 方向(手前)を正面とするパネルオブジェクトを作成します。
    //
    //*/
    //---------------------------------------------------------------------
    static LBatchPanel create( lnfloat width_, lnfloat height_, LNAxisAlignedDir dir_ = LN_AADIR_RZ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      パネルオブジェクトを作成する
    //
    //  @param[in]  width_   : 横方向のサイズ
    //  @param[in]  height_  : 縦方向のサイズ
    //  @param[in]  texture_ : セットするテクスチャ
    //  @param[in]  dir_     : 初期状態の面の方向
    //*/
    //---------------------------------------------------------------------
    static LBatchPanel create( lnfloat width_, lnfloat height_, LTexture texture_, LNAxisAlignedDir dir_ = LN_AADIR_RZ );

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      大きさの設定
    //
    //  @param[in]  pos_ : 設定する大きさ
    //*/
    //---------------------------------------------------------------------
    virtual void setSize( const LVector2& size_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      大きさの設定
    //
    //  @param[in]  width_  : 横方向のサイズ
    //  @param[in]  height_ : 縦方向のサイズ
    //*/
    //---------------------------------------------------------------------
    virtual void setSize( lnfloat width_, lnfloat height_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      大きさの取得
    //*/
    //---------------------------------------------------------------------
    virtual const LVector2& getSize() const;

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


    LN_UTIL_REFCLASS_METHODS( LBatchPanel, Core::Scene::BatchSprite3D );
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================