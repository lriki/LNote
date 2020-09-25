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
// ■ VertexBuffer クラス
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
class VertexBuffer
    : public Base::ReferenceObject
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
    LNRESULT initialize( GraphicsDevice* device_, LNVertexElemenst* elements_, u32 vertex_num_, const void* data_, bool is_dynamic_ = false );

    /// 初期化 (渡された IDirect3DVertexBuffer9 と D3DVERTEXELEMENT9 を参照する)
    LNRESULT initialize( GraphicsDevice* device_, IDirect3DVertexBuffer9* device_obj_, D3DVERTEXELEMENT9* layout_ );

public:

    /// 頂点数の取得
	virtual u32 getVertexNum() const { return mVertexNum; }

	/// 頂点ひとつ分のサイズの取得
	virtual u32 getVertexStride() const { return mVertexStride; }

	/// 動的な頂点かを判定する	
	virtual bool isDynamic() const { return ( !mVertexBuffer ); }

	/// ロック
	virtual void* lock();

	/// アンロック
	virtual void unlock();

public:

	/// IDirect3DVertexBuffer9 へのポインタ取得
	IDirect3DVertexBuffer9* getDxVertexBuffer() { return mVertexBuffer; }

    /// IDirect3DVertexDeclaration9 へのポインタ取得
	IDirect3DVertexDeclaration9* getDxVertexDeclaration() { return mVertexDecl; }

private:

	GraphicsDevice*	                mGraphicsDevice;
    IDirect3DVertexBuffer9*	        mVertexBuffer;	    ///< 頂点バッファ
	void*					        mVertices;		    ///< 動的な頂点バッファとして作った場合はこっちに配列が作られる ( union でもいいかな・・・ )
	IDirect3DVertexDeclaration9*	mVertexDecl;	    ///< 頂点宣言
	u32		                        mVertexNum;		    ///< 頂点の数
	u32		                        mVertexStride;	    ///< 頂点ひとつ分のサイズ
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