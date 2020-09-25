//=============================================================================
//【 GLIndexBuffer 】
//-----------------------------------------------------------------------------
///**
//  @file       GLIndexBuffer.h
//  @brief      GLIndexBuffer
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
// ■ IndexBuffer クラス
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
class IndexBuffer
    : public GLResource//Base::ReferenceObject 
    , public IIndexBuffer
{
public:

    /// コンストラクタ
    IndexBuffer();

    /// デストラクタ
    virtual ~IndexBuffer();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    LNRESULT initialize( GraphicsDevice* device_, int index_num_, const void* data_, bool is_dynamic_, bool is_16bit_ );

    /// インデックス数の取得
    virtual u32 getIndexNum() const { return mIndexNum; }

    /// インデックスひとつ分のサイズの取得 (2 または 4)
    virtual lnU32 getIndexStride() const { return mIndexStride; }

	/// 動的なインデックスバッファかを判定する	
	virtual bool isDynamic() const { return mIsDynamic; }

	/// ロック
	virtual void* lock();

	/// アンロック
	virtual void unlock();

public:

	GLuint getGLIndexBuffer() { return mGLIndexBuffer; }


    virtual void onCommitGLResource();
    virtual void onDisposeGLResource();
    void onLostDevice() { GLResource::onLostDevice(); }
    void onResetDevice() { GLResource::onResetDevice(); }

private:

	GraphicsDevice*     mGraphicsDevice;
    GLuint              mGLIndexBuffer;
    lnByte*             mBufferData;        ///< CPU 側で管理されるバッファデータ (静的バッファの時に作成される)
	lnU32		        mIndexNum;			///< インデックスの数
    lnU32               mIndexStride;       ///< 2 または 4
    GLenum              mUsage;
    bool                mIsDynamic;
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