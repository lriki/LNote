//=============================================================================
//【 Sprite3DEmitter 】
//-----------------------------------------------------------------------------
///**
//  @file       Sprite3DEmitter.h
//  @brief      Sprite3DEmitter
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Common/SpriteRenderer.h"
#include "EmitterBase.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Effect
{

#if 0

//=============================================================================
// ■ Sprite3DEmitterCore クラス
//-----------------------------------------------------------------------------
///**
//  @brief		3D スプライトエミッタ
//*/
//=============================================================================
class Sprite3DEmitterCore
    : public EmitterCoreBase
{
public:

    Sprite3DEmitterCore( Manager* manager_ );

    virtual ~Sprite3DEmitterCore();

public:

    /// エミッタインスタンスクラスの生成
    virtual EmitterInstanceBase* createInstance();

    Graphics::SpriteRenderer* getSprite3DRenderer() { return mSprite3DRenderer; }

    void            setSize( const LVector2& size_ ) { mSize = size_; }
    const LVector2& getSize() const { return mSize; }

    void            setCenter( const LVector3& center_ ) { mCenter = center_; }
    const LVector3& getCenter() const { return mCenter; }

	void					setSrcRect( const Geometry::Rect& src_rect_ ) { mSrcRect = src_rect_; }
    const Geometry::Rect&	getSrcRect() const { return mSrcRect; }

    void            setTextureFilename( LRefTString filename_ ) { mTextureFilename = filename_; this->mModified = true; }
    LRefTString     getTextureFilename() { return mTextureFilename; }

private:

    Graphics::SpriteRenderer*   mSprite3DRenderer;
    LVector2		            mSize;
	LVector3		            mCenter;
    Geometry::Rect				mSrcRect;
    //Graphics::ITexture*         mTexture;
    LRefTString                 mTextureFilename;
    LSamplerState               mSamplerState;
    LNBillboardType             mBillboardType;
};

//=============================================================================
// ■ Sprite3DEmitterInstance クラス
//-----------------------------------------------------------------------------
///**
//  @brief		3D スプライトエミッタインスタンス
//*/
//=============================================================================
class Sprite3DEmitterInstance
    : public EmitterInstanceBase
{
public:

    Sprite3DEmitterInstance( Sprite3DEmitterCore* core_ );

    virtual ~Sprite3DEmitterInstance() {}

public:

    /// リソース再構築
    virtual void refresh();

    /// 描画
    virtual void draw( const LMatrix& world_mat_ );

protected:

    Sprite3DEmitterCore*    mEmitterCore;
    Graphics::ITexture*     mTexture;
};

#endif

#if 0
class Sprite3DEmitterSubset;

//=============================================================================
// ■ Sprite3DEmitter クラス
//-----------------------------------------------------------------------------
///**
//  @brief		スプライトエミッタ
//*/
//=============================================================================
class Sprite3DEmitter
    : public EmitterBase
{
public:

    Sprite3DEmitter( EffectNode* node_ );

    virtual ~Sprite3DEmitter();

public:

    /// エミッタ描画クラスの生成
    virtual EmitterSubsetBase* createSubset();

    /// 描画
    virtual void draw();

private:

    Graphics::SpriteRenderer*   mSprite3DRenderer;
    LVector2		            mSize;
	LVector3		            mCenter;
    LRect			            mSrcRect;
    Graphics::ITexture*         mTexture;
    LSamplerState               mSamplerState;
    LNBillboardType             mBillboardType;

    friend class Sprite3DEmitterSubset;
};

//=============================================================================
// ■ Sprite3DEmitter クラス
//-----------------------------------------------------------------------------
///**
//  @brief		スプライトエミッタ描画本体
//*/
//=============================================================================
class Sprite3DEmitterSubset
    : public EmitterSubsetBase
{
public:

    Sprite3DEmitterSubset( Sprite3DEmitter* parent_emitter_ );

public:

    /// 描画
    virtual void draw();
    
private:

    Sprite3DEmitter*    mEmitter;
};
#endif
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================