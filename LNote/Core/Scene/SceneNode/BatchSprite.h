//=============================================================================
//【 BatchSprite 】
//-----------------------------------------------------------------------------
///**
//  @file       BatchSprite.h
//  @brief      BatchSprite
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
// ■ BatchSprite3D
//-----------------------------------------------------------------------------
///**
//  @brief		バッチ描画3Dスプライト (※シェーダ使用不可)
//*/
//=============================================================================
class BatchSprite3D
    : public SceneNode
{
public:

    /// コンストラクタ
    BatchSprite3D( SceneGraph* scene_ );

    /// デストラクタ
    virtual ~BatchSprite3D();

public:

    /// 初期化
    void initialize( lnfloat w_size_, lnfloat h_size_, LNAxisAlignedDir dir_ = LN_AADIR_RZ );

    /// 大きさの設定
    void setSize( const LVector2& size_ ) { mSize = size_; }

    /// 大きさの取得
    const LVector2& getSize() const { return mSize; }

    /// テクスチャの設定
    void setTexture( Graphics::ITexture* texture_ );

    /// テクスチャの取得
    Graphics::ITexture* getTexture() { return mTexture; }

    /// テクスチャの転送矩形の設定
    void setSrcRect( const Geometry::Rect& rect_ );

    /// テクスチャの転送矩形の取得
    const Geometry::Rect& getSrcRect() const { return mSrcRect; }

    /// 反転方法の設定
    void setFlipFlag( lnU8 flags_ ) { mFlipFlags = flags_; }

    /// 反転方法の取得
    lnU8 getFlipFlag() const { return mFlipFlags; }

public:

    /// ノードの種類の取得
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_BATCHSPRITE3D; }

    /// サブセットを描画する
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    BatchSpriteProxy*   mBatchSpriteProxy;
    LNAxisAlignedDir    mAADir;
    LVector2            mSize;
    Graphics::ITexture* mTexture;
    Geometry::Rect		mSrcRect;
    lnU8                mFlipFlags;
    bool                mCalledSetSrcRect;

    class BatchSprite3DContext;
    friend class BatchSprite3DContext;
};

//=============================================================================
// ■ BatchSprite
//-----------------------------------------------------------------------------
///**
//  @brief		バッチ描画2Dスプライト (※シェーダ使用不可)
//*/
//=============================================================================
class BatchSprite
    : public SceneNode
{
public:

    /// コンストラクタ
    BatchSprite( SceneGraph* scene_ );

    /// デストラクタ
    virtual ~BatchSprite();

public:

    /// 初期化
    void initialize();

    /// テクスチャの設定
    void setTexture( Graphics::ITexture* texture_ );

    /// テクスチャの取得
    Graphics::ITexture* getTexture() { return mTexture; }

    /// テクスチャの転送矩形の設定
    void setSrcRect( const Geometry::Rect& rect_ );

    /// テクスチャの転送矩形の取得
    const Geometry::Rect& getSrcRect() const { return mSrcRect; }

    /// 反転方法の設定
    void setFlipFlag( lnU8 flags_ ) { mFlipFlags = flags_; }

    /// 反転方法の取得
    lnU8 getFlipFlag() const { return mFlipFlags; }

public:

    /// ノードの種類の取得
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_BATCHSPRITE; }

    /// サブセットを描画する
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    BatchSpriteProxy*   mBatchSpriteProxy;
    Graphics::ITexture* mTexture;
    Geometry::Rect		mSrcRect;
    lnU8                mFlipFlags;
    bool                mCalledSetSrcRect;

    class BatchSpriteContext;
    friend class BatchSpriteContext;
};

#if 0
//=============================================================================
// ■ BatchSprite
//-----------------------------------------------------------------------------
///**
//  @brief		バッチ描画スプライト (※シェーダ使用不可)
//*/
//=============================================================================
class BatchSprite
    : public SceneNode
{
public:

    BatchSprite( SceneGraph* scene_ );

    virtual ~BatchSprite();

public:

    /// 初期化
    LNRESULT initialize();

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
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_BATCHSPRITE; }

    /// サブセットを描画する
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    /// テクスチャ座標の更新
    void _updateTexUV();

private:

    Engine::Graphics::ISpriteRenderer*  mSpriteRenderer;
    LN3DFront                           mPanelDir;
    LVector2                            mSize;
    Core::Graphics::ITexture*           mTexture;
    LRect                               mSrcRect;
    u32                                 mFlipFlags;
    bool                                mCalledSetSrcRect;  ///< 一度でも setSrcRect() が呼ばれたフラグ


    Graphics::GeometryRenderer* mRenderer;
    //LNAxisAlignedDir            mAADir;
    //LVector2                    mSize;
    LRect                       mSrcRect;
    lnU32                       mFlipFlags;

    LVector3                    mUpperLeft;
    LVector3                    mLowerRight;
    LVector2                    mUVUpperLeft;
    LVector2                    mUVLowerRight;

    bool                        mCalledSetSrcRect;
    
};
#endif
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================