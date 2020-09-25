//=============================================================================
//【 VertexBuffer 】
//-----------------------------------------------------------------------------
///**
//  @file       VertexBuffer.h
//  @brief      VertexBuffer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"
#include "GLUtil.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace OpenGL
{
class GraphicsDevice;

//=============================================================================
// ■ VertexBuffer クラス
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
class VertexBuffer
    : public GLResource//: public Base::ReferenceObject
    , public IVertexBuffer
{
public:

    /// コンストラクタ
    VertexBuffer();

    /// デストラクタ
    virtual ~VertexBuffer();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    LNRESULT initialize( GraphicsDevice* device_, LNVertexElemenst* elements_, u32 vertex_num_, const void* data_, bool is_dynamic_ );

    /// 頂点数の取得
	virtual u32 getVertexNum() const { return mVertexNum; }

	/// 頂点ひとつ分のサイズの取得
	virtual u32 getVertexStride() const { return mVertexStride; }

	/// 動的な頂点かを判定する	
	virtual bool isDynamic() const { return mIsDynamic; }

	/// ロック
	virtual void* lock();

	/// アンロック
	virtual void unlock();

public:

    GLuint getGLVertexBuffer() { return mGLVertexBuffer; }

    LNGLVertexAttribPointerData* getGLVertexAttribPointerData() { return mLNGLAttribData; }

    virtual void onCommitGLResource();
    virtual void onDisposeGLResource();
    void onLostDevice() { GLResource::onLostDevice(); }
    void onResetDevice() { GLResource::onResetDevice(); }

private:

	GraphicsDevice*	                mGraphicsDevice;
    GLuint                          mGLVertexBuffer;
	LNGLVertexAttribPointerData*	mLNGLAttribData;    ///< 頂点宣言
    lnByte*                         mBufferData;        ///< CPU 側で管理されるバッファデータ (静的バッファの時に作成される)
	u32		                        mVertexNum;		    ///< 頂点の数
	u32		                        mVertexStride;	    ///< 頂点ひとつ分のサイズ
    GLenum                          mUsage;
    bool                            mIsDynamic;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace OpenGL
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================