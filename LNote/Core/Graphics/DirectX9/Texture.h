//=============================================================================
//【 Texture 】
//-----------------------------------------------------------------------------
///**
//  @file       Texture.h
//  @brief      Texture
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"
#include "GraphicsDevice.h"

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

//=============================================================================
// ■ DX9TextureBase クラス
//-----------------------------------------------------------------------------
///**
//  @brief		DirectX9 テクスチャの基底
//*/
//=============================================================================
class DX9TextureBase
    : public Base::ReferenceObject
    , public ITexture
{
public:

    /// コンストラクタ
    DX9TextureBase() {}

    /// デストラクタ
    virtual ~DX9TextureBase() {}

public:

    /// ファイルから作成したときのファイル名
    virtual const lnChar* getFilename() const { return NULL; }

    virtual void* getIDirect3DTexture9() { return getDxTexture(); }
    virtual void* getIDirect3DSurface9() { return getDxSurface(); }

public:

    /// IDirect3DTexture9 ポインタの取得
    virtual IDirect3DTexture9* getDxTexture() = 0;

	/// テクスチャサーフェイスへのポインタ取得
	virtual IDirect3DSurface9* getDxSurface() = 0;

    /// デバイスリセット直前のコールバック
    virtual LNRESULT onLostDevice() = 0;

	/// デバイスリセット直後のコールバック
    virtual LNRESULT onResetDevice() = 0;
};

//=============================================================================
// ■ Texture クラス
//-----------------------------------------------------------------------------
///**
//  @brief		普通のテクスチャのクラス
//*/
//=============================================================================
class Texture
    : public DX9TextureBase
    , public TextureCache::CachedObject
{
public:
    
    LN_CACHED_REFOBJ_METHODS;

	/// コンストラクタ
	Texture( GraphicsDevice* device_ );

	/// デストラクタ
	virtual ~Texture();

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief		テクスチャ作成
    //
    //  @param[in]  width_   : テクスチャの幅
    //  @param[in]  height_  : テクスチャの高さ
    //  @param[in]  level_   : ミップマップレベル ( 0 で全て作成 )
    //  @param[in]  format_  : サーフェイスフォーマット
    //*/
    //---------------------------------------------------------------------
	LNRESULT initialize( u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief		テクスチャ作成 ( メモリに展開された画像データから )
    //  
    //  @param[in]  data_      : テクスチャデータが格納されているバッファへのアドレス
    //  @param[in]  size_      : テクスチャデータのサイズ (バイト単位)
    //  @param[in]  color_key_ : 透明化する色
    //  @param[in]  level_     : ミップマップレベル ( 0 で全て作成 )
    //  @param[in]  format_    : サーフェイスフォーマット         
    //*/
    //---------------------------------------------------------------------
	LNRESULT initialize( const void* data_, u32 size_, u32 color_key_ = 0x00000000, u32 levels_ = 1, LNSurfaceFormat format_ = LN_FMT_A8R8G8B8 );

    

	/// 種類の取得
	virtual LNTextureType getType() const { return LN_TEXTURETYPE_NORMAL; }

	/// サイズの取得
    virtual const LVector2& getSize() const { return mSize; }

	/// 実際のサイズの取得
    virtual const LVector2& getRealSize( bool is_reciprocal_ ) const { return ( is_reciprocal_ ) ? mRealSizeInv : mRealSize; }

    /// クリア
	virtual void clear( const LColor& color_ );

    /// ロック
	virtual u32* lock();

    /// アンロック
	virtual void unlock();

    /// drawText() での文字列描画に使用するフォントの設定
    virtual void setFont( IFont* font_ ) { LN_SAFE_RELEASE( mFont ); mFont = font_; LN_SAFE_ADDREF( mFont ); }

    /// drawText() での文字列描画に使用するフォントの取得
    virtual IFont* getFont() const { return mFont; }

    /// 文字列の描画
    virtual LNRESULT drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_, int len_ );

    /// テクスチャのブロック転送 (現在、普通のテクスチャ (RT、深度バッファではない) にのみ有効)
    virtual LNRESULT blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ );

    /// ファイルから作成したときのファイル名
    virtual const lnChar* getFilename() const { return mFilename.c_str(); }


	/// IDirect3DTexture9 ポインタの取得
	virtual IDirect3DTexture9* getDxTexture() { return mDxTexture; }

	/// テクスチャサーフェイスへのポインタ取得
	virtual IDirect3DSurface9* getDxSurface() { return mDxSurface; }

    /// デバイスリセット直前のコールバック
    virtual LNRESULT onLostDevice() { return LN_OK; }

	/// デバイスリセット直後のコールバック
	virtual LNRESULT onResetDevice() { return LN_OK; }

private:

    GraphicsDevice*     mGraphicsDevice;    ///< 管理クラス
	IDirect3DTexture9*	mDxTexture;		    ///< Direct3DTexture 本体
	IDirect3DSurface9*	mDxSurface;		    ///< テクスチャのサーフェイス

    LVector2            mSize;                   ///< 作成時に要求したサイズ
    LVector2            mRealSize;               ///< 実際のテクスチャのサイズ
    LVector2            mRealSizeInv;            ///< 実際のテクスチャサイズの逆数

    LRefTString         mFilename;
	IFont*              mFont;

    LSharingKey mSharingKey;
};

//=============================================================================
// ■ RenderTargetTexture クラス
//-----------------------------------------------------------------------------
///**
//  @brief		レンダーターゲットテクスチャのクラス
//*/
//=============================================================================
class RenderTargetTexture
    : public DX9TextureBase
{
public:

	/// コンストラクタ
	RenderTargetTexture( GraphicsDevice* device_ );

	/// デストラクタ
	virtual ~RenderTargetTexture();

    LN_REFOBJ_METHODS;

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief		レンダーターゲットテクスチャの作成
    //
    //  @param[in]  width_   : テクスチャの幅
    //  @param[in]  height_  : テクスチャの高さ
    //  @param[in]  level_   : ミップマップレベル ( 0 で全て作成 )
    //  @param[in]  format_  : サーフェイスフォーマット
    //*/
    //---------------------------------------------------------------------
	LNRESULT initialize( u32 width_, u32 height_, u32 level_, LNSurfaceFormat format_ );

	/// 種類の取得
	virtual LNTextureType getType() const { return LN_TEXTURETYPE_RENDERTARGET; }

	/// サイズの取得
    virtual const LVector2& getSize() const { return mSize; }

	/// 実際のサイズの取得
	virtual const LVector2& getRealSize( bool is_reciprocal_ ) const { return ( is_reciprocal_ ) ? mRealSizeInv : mRealSize; }

    /// クリア ( レンダリングターゲットではサポートしてません )
    void clear( const LColor& color_ ) { printf( "レンダーターゲットテクスチャの clear() はサポートしていません。\n" ); }

    /// ロック
	u32* lock();

    /// アンロック
	void unlock();

    /// drawText() での文字列描画に使用するフォントの設定
    virtual void setFont( IFont* font_ ) { }

    /// drawText() での文字列描画に使用するフォントの取得
    virtual IFont* getFont() const { return NULL; }

    /// 文字列の描画
    virtual LNRESULT drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_, int len_ ) { return LN_ERR_INVALIDCALL; }

    /// テクスチャのブロック転送
    virtual LNRESULT blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ ) { return LN_ERR_INVALIDCALL; }

public:

	/// IDirect3DTexture9 ポインタの取得
	virtual IDirect3DTexture9* getDxTexture() { return mDxTexture; }

	/// テクスチャサーフェイスへのポインタ取得
	virtual IDirect3DSurface9* getDxSurface() { return mDxSurface; }

    /// デバイスリセット直前のコールバック
	virtual LNRESULT onLostDevice();

	/// デバイスリセット直後のコールバック
	virtual LNRESULT onResetDevice();

private:

    /// 実際に作成する
    LNRESULT _create();

    /// 解放処理
    void _dispose();

private:

    GraphicsDevice*     mGraphicsDevice;    ///< 管理クラス

	IDirect3DTexture9*	mDxTexture;			///< Direct3DTexture 本体
	IDirect3DSurface9*	mDxSurface;			///< テクスチャのサーフェイス
	IDirect3DSurface9*	mDxSystemSurface;	///< レンダリングターゲットをロックする時に使う、システムメモリに配置されたサーフェイス

    LVector2            mSize;              ///< 作成時に要求したサイズ
    LVector2            mRealSize;          ///< 実際のテクスチャのサイズ
    LVector2            mRealSizeInv;            ///< 実際のテクスチャサイズの逆数
    u32                 mMipLevel;			///< ミップマップレベル
	LNSurfaceFormat	    mFormat;			///< サーフェイスフォーマット

    //friend class ResourceManager;
};

//=============================================================================
// ■ DepthBuffer クラス
//-----------------------------------------------------------------------------
///**
//  @brief		深度バッファのクラス
//*/
//=============================================================================
class DepthBuffer
    : public DX9TextureBase
{
public:

	/// コンストラクタ
	DepthBuffer( GraphicsDevice* device_ );

	/// デストラクタ
	virtual ~DepthBuffer();

    LN_REFOBJ_METHODS;

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief		深度バッファの作成
    //
    //  @param[in]  width_   : テクスチャの幅
    //  @param[in]  height_  : テクスチャの高さ
    //  @param[in]  format_  : サーフェイスフォーマット
    //*/
    //---------------------------------------------------------------------
	LNRESULT initialize( u32 width_, u32 height_, LNSurfaceFormat format_ );

	/// 種類の取得
	virtual LNTextureType getType() const { return LN_TEXTURETYPE_DEPTH; }

	/// サイズの取得
    virtual const LVector2& getSize() const { return mSize; }

	/// 実際のサイズの取得
	virtual const LVector2& getRealSize( bool is_reciprocal_ ) const { return ( is_reciprocal_ ) ? mRealSizeInv : mRealSize; }

    /// クリア ( 深度バッファではサポートしてません )
    void clear( const LColor& color_ ) { printf( "深度バッファの clear() はサポートしていません。\n" ); }

    /// ロック
    u32* lock() { return NULL; }

    /// アンロック
    void unlock() { }

    /// drawText() での文字列描画に使用するフォントの設定
    virtual void setFont( IFont* font_ ) { }

    /// drawText() での文字列描画に使用するフォントの取得
    virtual IFont* getFont() const { return NULL; }

    /// 文字列の描画
    virtual LNRESULT drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_, int len_ ) { return LN_ERR_INVALIDCALL; }

    /// テクスチャのブロック転送
    virtual LNRESULT blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ ) { return LN_ERR_INVALIDCALL; }


	/// IDirect3DTexture9 ポインタの取得
	virtual IDirect3DTexture9* getDxTexture() { return NULL; }

	/// テクスチャサーフェイスへのポインタ取得
	virtual IDirect3DSurface9* getDxSurface() { return mDxSurface; }

    /// デバイスリセット直前のコールバック
	virtual LNRESULT onLostDevice();

	/// デバイスリセット直後のコールバック
	virtual LNRESULT onResetDevice();

private:

    /// 実際に作成する
    LNRESULT _create();

    /// 解放処理
    void _dispose();

private:

    GraphicsDevice*     mGraphicsDevice;    ///< 管理クラス
	IDirect3DSurface9*	mDxSurface;			///< 深度バッファサーフェイス
    LVector2            mSize;              ///< 作成時に要求したサイズ
    LVector2            mRealSize;          ///< 実際のテクスチャのサイズ
    LVector2            mRealSizeInv;       ///< 実際のテクスチャサイズの逆数
	LNSurfaceFormat	    mFormat;			///< サーフェイスフォーマット

    friend class ResourceManager;
};

//=============================================================================
// ■ SystemCreatedTexture クラス
//-----------------------------------------------------------------------------
///**
//  @brief		DirectX 側で自動的に作成されたテクスチャを扱うクラス
//
//  @note
//              とりあえずデフォルトのスワップチェインの処理で使ってる。
//*/
//=============================================================================
class SystemCreatedTexture
    : public DX9TextureBase
{
public:

	/// コンストラクタ
	SystemCreatedTexture();

	/// デストラクタ
	virtual ~SystemCreatedTexture();

    LN_REFOBJ_METHODS;

public:

    /// テクスチャ、サーフェイスのセット (texture_ は NULL でもOK。surface_ は必ず。デバイスリセット時は両方 NULL で参照カウントをデクリメント)
	LNRESULT setTextureAndSurface( IDirect3DTexture9* texture_, IDirect3DSurface9* surface_ );

	/// 種類の取得
	virtual LNTextureType getType() const { return LN_TEXTURETYPE_RENDERTARGET; }

    /// サイズの取得
    virtual const LVector2& getSize() const { return mSize; }

	/// 実際のサイズの取得
	virtual const LVector2& getRealSize( bool is_reciprocal_ ) const { return ( is_reciprocal_ ) ? mRealSizeInv : mRealSize; }

    /// クリア
    void clear( const LColor& color_ ) { printf( "レンダーターゲットテクスチャの clear() はサポートしていません。\n" ); }

    /// drawText() での文字列描画に使用するフォントの設定
    virtual void setFont( IFont* font_ ) { }

    /// drawText() での文字列描画に使用するフォントの取得
    virtual IFont* getFont() const { return NULL; }

    /// 文字列の描画
    virtual LNRESULT drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_, int len_ ) { return LN_ERR_INVALIDCALL; }

    /// テクスチャのブロック転送
    virtual LNRESULT blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ ) { return LN_ERR_INVALIDCALL; }

    /// ロック
    virtual u32* lock() { return NULL; }

    /// アンロック
    virtual void unlock() {}

public:

    /// IDirect3DTexture9 ポインタの取得
    virtual IDirect3DTexture9* getDxTexture() { return mDxTexture; }

	/// テクスチャサーフェイスへのポインタ取得
	virtual IDirect3DSurface9* getDxSurface() { return mDxSurface; }

    /// デバイスリセット直前のコールバック
    virtual LNRESULT onLostDevice() { return LN_OK; }

	/// デバイスリセット直後のコールバック
    virtual LNRESULT onResetDevice() { return LN_OK; }

private:

    IDirect3DTexture9*  mDxTexture;
	IDirect3DSurface9*	mDxSurface;			///< テクスチャのサーフェイス

    LVector2            mSize;              ///< 作成時に要求したサイズ
    LVector2            mRealSize;          ///< 実際のテクスチャのサイズ
    LVector2            mRealSizeInv;       ///< 実際のテクスチャサイズの逆数
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