//=============================================================================
//【 Sprite 】
//-----------------------------------------------------------------------------
///**
//  @file       Sprite.h
//  @brief      Sprite
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "SceneNode.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

//=============================================================================
// ■ Sprite クラス
//-----------------------------------------------------------------------------
///**
//  @brief		四角形オブジェクト
//*/
//=============================================================================
class Sprite
    : public SceneNode
    //, public ISprite3D
{
public:

    /// コンストラクタ
    Sprite( SceneGraph* scene_ );

    /// デストラクタ
    virtual ~Sprite();

public:

    /// 初期化
    LNRESULT initialize();

    ///// 大きさの設定
    //void setSize( const LVector2& size_ );

    ///// 大きさの取得
    //const LVector2& getSize() const { return mSize; }

    /// テクスチャの設定
    void setTexture( Graphics::ITexture* texture_ );

    /// テクスチャの取得
    Graphics::ITexture* getTexture();

    /// テクスチャの転送矩形の設定
    void setSrcRect( const Geometry::Rect& rect_ );

    /// テクスチャの転送矩形の取得
    const Geometry::Rect& getSrcRect() const { return mSrcRect; }

    /// 反転方法の設定
    void setFlipFlag( lnU32 flags_ ) { mFlipFlags = flags_; _updateTexUV(); }

    /// 反転方法の取得
    lnU32 getFlipFlag() const { return mFlipFlags; }

public:

    /// ノードの種類の取得
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_SPRITE3D; }

    /// サブセットを描画する
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    /// テクスチャ座標の更新
    void _updateTexUV();

private:

    Graphics::GeometryRenderer* mRenderer;
    //LNAxisAlignedDir            mAADir;
    //LVector2                    mSize;
    Geometry::Rect				mSrcRect;
    lnU32                       mFlipFlags;

    LVector3                    mUpperLeft;
    LVector3                    mLowerRight;
    LVector2                    mUVUpperLeft;
    LVector2                    mUVLowerRight;

    bool                        mCalledSetSrcRect;
    
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================