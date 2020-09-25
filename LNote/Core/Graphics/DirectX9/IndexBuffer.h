//=============================================================================
//【 IndexBuffer 】
//-----------------------------------------------------------------------------
///**
//  @file       IndexBuffer.h
//  @brief      IndexBuffer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace DirectX9
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
    : public Base::ReferenceObject 
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
    LNRESULT initialize( GraphicsDevice* device_, int index_num_, const u16* data_, bool is_dynamic_, bool is_16bit_ );

    /// インデックス数の取得
    virtual u32 getIndexNum() const { return mIndexNum; }

    /// インデックスひとつ分のサイズの取得 (2 または 4)
    virtual lnU32 getIndexStride() const { return mIndexStride; }

	/// 動的なインデックスバッファかを判定する	
	virtual bool isDynamic() const { return ( !mIndexBuffer ); }

	/// ロック
	virtual void* lock();

	/// アンロック
	virtual void unlock();

	/// IDirect3DIndexBuffer9 へのポインタ取得
	IDirect3DIndexBuffer9* getDxIndexBuffer() const { return mIndexBuffer; }

private:

	GraphicsDevice*	        mGraphicsDevice;
	IDirect3DIndexBuffer9*	mIndexBuffer;	    ///< IDirect3DIndexBuffer9 へのポインタ
	void*	                mIndices;			///< 動的なインデックスを作る場合はここに作る
	u32		                mIndexNum;			///< インデックスの数
    lnU32                   mIndexStride;       ///< 2 または 4
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace DirectX9
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================