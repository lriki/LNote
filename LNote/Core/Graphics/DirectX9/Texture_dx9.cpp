//=============================================================================
//【 Texture 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Common/BitmapTextRenderer.h"
#include "../Common/GraphicsUtil.h"
#include "../Manager.h"
#include "DX9Module.h"
#include "GraphicsDevice.h"
#include "Texture.h"

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
// ■ Texture クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
    Texture::Texture( GraphicsDevice* device_ )
        : mGraphicsDevice   ( device_ )
        , mDxTexture    ( NULL )
        , mDxSurface    ( NULL )
        , mFont         ( NULL )
	{
        LN_SAFE_ADDREF( mGraphicsDevice );
    }

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
    Texture::~Texture()
	{
		SAFE_RELEASE( mDxSurface );
		SAFE_RELEASE( mDxTexture );
        LN_SAFE_RELEASE( mFont );
        LN_SAFE_RELEASE( mGraphicsDevice );
	}

    //---------------------------------------------------------------------
    // ● テクスチャ作成
	//---------------------------------------------------------------------
	LNRESULT Texture::initialize( u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ )
    {
        HRESULT hr;

        D3DFORMAT dx_fmt = DX9Module::convertLNFormatToDxFormat( format_ );

        // サイズ格納
        UINT w = width_;
        UINT h = height_;
        UINT levels = levels_;
        mSize.x = static_cast< lnfloat >( width_ );
        mSize.y = static_cast< lnfloat >( height_ );

        // 実際に作成されるべきテクスチャの情報を取得する
        LN_DXCALL_R(
            DX9Module::getInstance()->D3DXCheckTextureRequirements(
				mGraphicsDevice->getDX9Device(),
                &w, &h,
				&levels,        // 0 の場合は最大数が格納される
                0,
                &dx_fmt,    
                D3DPOOL_MANAGED ) );

        mRealSize.x = static_cast< lnfloat >( w );
        mRealSize.y = static_cast< lnfloat >( h );
        mRealSizeInv.x = 1.0f / mRealSize.x;
        mRealSizeInv.y = 1.0f / mRealSize.y;

		// テクスチャ作成
		// 3つめの引数 ( ミップマップ ) は、使わない場合は 1 にしておく( 0 にすると可能な限り全部作られる )
		LN_DXCALL_R(
            mGraphicsDevice->getDX9Device()->CreateTexture(
				w, h, levels, 0,
				dx_fmt, D3DPOOL_MANAGED, &mDxTexture, NULL ) );

		// テクスチャのサーフェイスを取得しておく
		LN_DXCALL_R( mDxTexture->GetSurfaceLevel( 0, &mDxSurface ) );

        // デフォルトのフォントをセットしておく
        setFont( mGraphicsDevice->getManager()->getDefaultFont() );

        return LN_OK;
    }


	//---------------------------------------------------------------------
	// ● テクスチャ作成 ( メモリに展開された画像データから )
	//---------------------------------------------------------------------
	LNRESULT Texture::initialize( const void* data_, u32 size_, u32 color_key_, u32 levels_, LNSurfaceFormat format_ )
	{
		// メモリから読み込む
		D3DXIMAGE_INFO image_info;
        HRESULT hr;

        LN_DXCALL_R( DX9Module::getInstance()->D3DXGetImageInfoFromFileInMemory( data_, size_, &image_info ) );

        // テクスチャのフォーマットを決める
        D3DFORMAT format = ( format_ == LN_FMT_UNKNOWN ) ? image_info.Format : DX9Module::getInstance()->convertLNFormatToDxFormat( format_ );
        
        LN_DXCALL_R(
            DX9Module::getInstance()->D3DXCreateTextureFromFileInMemoryEx(
			    mGraphicsDevice->getDX9Device(), 
                data_, size_,
                //512, 512, // ノート OK
			    //D3DX_DEFAULT_NONPOW2, // ノート NG  デスクトップ OK
                //D3DX_DEFAULT_NONPOW2,
                //D3DX_DEFAULT, // ノート OK  デスクトップ NG
                //D3DX_DEFAULT,
                //0, 0,       // ノート OK  デスクトップ NG
                //300, 300,       // デスクトップ OK
                image_info.Width,
                image_info.Height,
			    levels_,
                0,
			    format,
			    D3DPOOL_MANAGED,
                D3DX_FILTER_NONE,
                D3DX_DEFAULT,
			    color_key_,
			    NULL,
			    NULL,
			    &mDxTexture ) );

		// ここの時点で mDxTexture の参照カウントは「 1 」

		// ファイルのイメージの幅と高さを記憶
		mSize.x = static_cast< lnfloat >( image_info.Width );
        mSize.y = static_cast< lnfloat >( image_info.Height );

        UINT levels = levels_;  // 64 bit 対応
        
		// 実際のテクスチャの大きさを取得
		LN_DXCALL_R(
            DX9Module::getInstance()->D3DXCheckTextureRequirements(
			    mGraphicsDevice->getDX9Device(),
                &image_info.Width, &image_info.Height,
			    &levels, 0, NULL, D3DPOOL_MANAGED ) );

        mRealSize.x = static_cast< lnfloat >( image_info.Width );
        mRealSize.y = static_cast< lnfloat >( image_info.Height );
        mRealSizeInv.x = 1.0f / mRealSize.x;
        mRealSizeInv.y = 1.0f / mRealSize.y;

       
		// テクスチャのサーフェイスを取得する
		// ( ここでテクスチャのインターフェイスの参照カウントもひとつ増えてるみたい )
		LN_DXCALL_R( mDxTexture->GetSurfaceLevel( 0, &mDxSurface ) );

    
		// ここの時点で mDxSurface の参照カウントは「 3 」
		// ここの時点で mDxTexture の参照カウントは「 2 」

        // デフォルトのフォントをセットしておく
        setFont( mGraphicsDevice->getManager()->getDefaultFont() );

		return LN_OK;
	}

    
	//---------------------------------------------------------------------
	// ● クリア
	//---------------------------------------------------------------------
	void Texture::clear( const LColor& color_ )
	{
		D3DLOCKED_RECT locked_rect;
		HRESULT hr = mDxTexture->LockRect( 0, &locked_rect, NULL, 0 );
		u32* buf = (u32*)locked_rect.pBits;

        u32 c = color_.to32Bit( LN_GRAPHICSAPI_DIRECTX9 );

        int m = static_cast< int >( mRealSize.x * mRealSize.y );
        for ( int i = 0; i < m; ++i )
        {
            buf[ i ] = c;
        }

		mDxTexture->UnlockRect( 0 );
	}

	//---------------------------------------------------------------------
	// ● ロック
	//---------------------------------------------------------------------
	u32* Texture::lock()
	{
		D3DLOCKED_RECT locked_rect;
		HRESULT hr = mDxTexture->LockRect( 0, &locked_rect, NULL, 0 );
		return (u32*)locked_rect.pBits;
	}

	//---------------------------------------------------------------------
	// ● アンロック
	//---------------------------------------------------------------------
	void Texture::unlock()
	{
		mDxTexture->UnlockRect( 0 );
	}

    //---------------------------------------------------------------------
	// ● 文字列の描画
    //---------------------------------------------------------------------
    LNRESULT Texture::drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_, int len_ )
    {
        LNRESULT lr;

        Graphics::BitmapTextRenderer tr( LN_GRAPHICSAPI_DIRECTX9 );

        if ( mFont )
        {
            tr.setFont( mFont );
        }
        else
        {
            tr.setFont( mGraphicsDevice->getManager()->getDefaultFont() );
        }

        tr.setTargetBuffer(
            lock(), 
            static_cast< u32 >( mRealSize.x ),
            static_cast< u32 >( mRealSize.y ) );

        lr = tr.drawText( text_, rect_, align_, len_ );
        
         unlock();

        return lr;
    }

    //---------------------------------------------------------------------
	// ● テクスチャのブロック転送
    //---------------------------------------------------------------------
    LNRESULT Texture::blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ )
    {
        // フォーマットチェックが必要


        u32* dest = lock();
        u32* src  = src_texture_->lock();

        const LVector2& src_size = src_texture_->getRealSize();

        Util::bitblt(
            x_, y_,
            static_cast< int >( mRealSize.x ),
            static_cast< int >( mRealSize.y ),
            dest,
            static_cast< int >( mRealSize.x ),
            static_cast< int >( mRealSize.y ),
            src_rect_.x, src_rect_.y,
            src_rect_.w, src_rect_.h,
            src,
            static_cast< int >( src_size.x ),
            static_cast< int >( src_size.y ),
            true );

        unlock();
        src_texture_->unlock();
        return LN_OK;
    }

//=============================================================================
// ■ RenderTargetTexture クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    RenderTargetTexture::RenderTargetTexture( GraphicsDevice* device_ )
        : mGraphicsDevice   ( device_ )
        , mDxTexture        ( NULL )
	    , mDxSurface        ( NULL )
	    , mDxSystemSurface  ( NULL )
        , mMipLevel         ( 1 )
        , mFormat           ( LN_FMT_A8R8G8B8 )
    {
        LN_SAFE_ADDREF( mGraphicsDevice );
    }

	//---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
	RenderTargetTexture::~RenderTargetTexture()
    {
        _dispose();

        if ( mGraphicsDevice )
        {
            mGraphicsDevice->removeRenderTarget( this );
        }
        LN_SAFE_RELEASE( mGraphicsDevice );
    }

    //---------------------------------------------------------------------
    // ● レンダーターゲットテクスチャの作成
    //---------------------------------------------------------------------
	LNRESULT RenderTargetTexture::initialize( u32 width_, u32 height_, u32 level_, LNSurfaceFormat format_ )
    {
        mSize.set( static_cast< lnfloat >( width_ ), static_cast< lnfloat >( height_ ) );
	    mMipLevel = level_;
        mFormat = format_;
	
        return _create();
    }

    //---------------------------------------------------------------------
    // ● ロック
    //---------------------------------------------------------------------
	u32* RenderTargetTexture::lock()
    {
        IDirect3DDevice9*   dx_device = mGraphicsDevice->getDX9Device();
        D3DSURFACE_DESC     desc;
		D3DLOCKED_RECT	    locked_rect;

		// レンダーテクスチャのフォーマットを取得
		HRESULT hr = mDxTexture->GetLevelDesc( 0, &desc );
		//LDXASSERT_S( hr, "< LTextureImplRenderTarget::lock >\nIDirect3DTexture9::GetLevelDesc" );

		// レンダーテクスチャと同じフォーマットのサーフェイスをシステムメモリに確保
		hr = dx_device->CreateOffscreenPlainSurface(
			desc.Width, desc.Height, desc.Format,
			D3DPOOL_SYSTEMMEM, &mDxSystemSurface, NULL );
		//LDXASSERT_S( hr, "< LTextureImplRenderTarget::lock >\nIDirect3DDevice9::CreateOffscreenPlainSurface" );

		// レンダータゲットのサーフェイスをシステムメモリに転送
		hr = dx_device->GetRenderTargetData( mDxSurface, mDxSystemSurface );
		mDxSystemSurface->GetDesc( &desc );
		//LDXASSERT_S( hr, "< LTextureImplRenderTarget::lock >\nIDirect3DDevice9::GetRenderTargetData" );

		hr = mDxSystemSurface->LockRect( &locked_rect, NULL, 0 );
		if ( FAILED( hr ) )
		{
			SAFE_RELEASE( mDxSystemSurface );
			//LDXASSERT_S( hr, "< LTextureImplRenderTarget::lock >\nIDirect3DSurface9::LockRect" );
			return NULL;
		}

		return (u32*)locked_rect.pBits;
    }

    //---------------------------------------------------------------------
    // ● アンロック
    //---------------------------------------------------------------------
	void RenderTargetTexture::unlock()
    {
        if ( mDxSystemSurface )
		{
            IDirect3DDevice9* dx_device = mGraphicsDevice->getDX9Device();

			mDxSystemSurface->UnlockRect();
			dx_device->UpdateSurface( mDxSystemSurface, NULL, mDxSurface, NULL );
			SAFE_RELEASE( mDxSystemSurface );
		}
    }

    //---------------------------------------------------------------------
    // ● デバイスリセット直前のコールバック
    //---------------------------------------------------------------------
    LNRESULT RenderTargetTexture::onLostDevice()
    {
        
        _dispose();
        return LN_OK;
    }

	//---------------------------------------------------------------------
    // ● デバイスリセット直後のコールバック
    //---------------------------------------------------------------------
    LNRESULT RenderTargetTexture::onResetDevice()
    {
        onLostDevice();
        return _create();
    }

    //---------------------------------------------------------------------
    // ● 実際に作成する
    //---------------------------------------------------------------------
    LNRESULT RenderTargetTexture::_create()
    {
		HRESULT hr;
        IDirect3DDevice9* dx_device = mGraphicsDevice->getDX9Device();

        D3DFORMAT dx_fmt = DX9Module::convertLNFormatToDxFormat( mFormat );
        switch ( dx_fmt )
        {
            case D3DFMT_A8B8G8R8: dx_fmt = D3DFMT_A8R8G8B8; break;
            case D3DFMT_X8B8G8R8: dx_fmt = D3DFMT_X8R8G8B8; break;
        }

        // サイズ格納
        UINT w = static_cast< UINT >( mSize.x );
        UINT h = static_cast< UINT >( mSize.y );
        UINT levels = mMipLevel;
       /*
        // 実際に作成されるべきテクスチャの情報を取得する
        LN_DXCALL_R(
            GraphicsUtil::D3DXCheckTextureRequirements(
				dx_device,
                &w, &h,
				&levels,        // 0 の場合は最大数が格納される
                D3DUSAGE_RENDERTARGET,
                &dx_fmt,    
                D3DPOOL_MANAGED ),
            "Texture::initialize" );
            

        mSize.dump();

        // 実際の値格納
        mRealSize.x = static_cast< lnfloat >( w );
        mRealSize.y = static_cast< lnfloat >( h );
        mMipLevel = levels;
        mFormat = GraphicsUtil::convertFormatDxToLN( dx_fmt );

        mRealSize.dump();
        */

        LN_DXCALLF_R(
            dx_device->CreateTexture(
				w, h,
				levels,
				D3DUSAGE_RENDERTARGET,	// レンダリングターゲットとして作成
                dx_fmt,
				D3DPOOL_DEFAULT,		// レンダリングターゲットは D3DPOOL_DEFAULT
				&mDxTexture,
				NULL ),
            "w=%d h=%d levels=%d dx_fmt=%d\n\nミップマップレベルが 1 以外でテクスチャサイズが 2 の累乗ではない場合、このエラーが発生することがあります。",
            w, h, levels, dx_fmt );

        // サイズチェック
        D3DSURFACE_DESC desc;
        mDxTexture->GetLevelDesc( 0, &desc );
        //_p( desc.Width );
        //_p( desc.Height );
        mRealSize.x = static_cast< lnfloat >( desc.Width );
        mRealSize.y = static_cast< lnfloat >( desc.Height );
        mRealSizeInv.x = 1.0f / mRealSize.x;
        mRealSizeInv.y = 1.0f / mRealSize.y;

		// レンダリングターゲットのサーフェイスを取得する
		LN_DXCALL_R( mDxTexture->GetSurfaceLevel( 0, &mDxSurface ) );

		// ちなみに D3DFMT_R8G8B8 はレンダリングターゲットには使えない
     
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 解放処理
    //---------------------------------------------------------------------
    void RenderTargetTexture::_dispose()
    {
        SAFE_RELEASE( mDxSurface );
		SAFE_RELEASE( mDxSystemSurface );
		SAFE_RELEASE( mDxTexture );
    }

//=============================================================================
// ■ DepthBuffer クラス
//=============================================================================

	//---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    DepthBuffer::DepthBuffer( GraphicsDevice* device_ )
        : mGraphicsDevice   ( device_ )
        , mDxSurface        ( NULL )
        , mFormat           ( LN_FMT_D24S8 )
    {
        LN_SAFE_ADDREF( mGraphicsDevice );
    }

	//---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    DepthBuffer::~DepthBuffer()
    {
        _dispose();

        if ( mGraphicsDevice )
        {
            mGraphicsDevice->removeDepthBuffer( this );
        }
        LN_SAFE_RELEASE( mGraphicsDevice );
    }

    //---------------------------------------------------------------------
    // ● 深度バッファの作成
    //---------------------------------------------------------------------
	LNRESULT DepthBuffer::initialize( u32 width_, u32 height_, LNSurfaceFormat format_ )
    {
        mSize.set( static_cast< lnfloat >( width_ ), static_cast< lnfloat >( height_ ) );
        mFormat = format_;
        return _create();
    }

    //---------------------------------------------------------------------
    // ● デバイスリセット直前のコールバック
    //---------------------------------------------------------------------
    LNRESULT DepthBuffer::onLostDevice()
    {
        _dispose();
        return LN_OK;
    }

	//---------------------------------------------------------------------
    // ● デバイスリセット直後のコールバック
    //---------------------------------------------------------------------
    LNRESULT DepthBuffer::onResetDevice()
    {
        onLostDevice();
        return _create();
    }

    //---------------------------------------------------------------------
    // ● 実際に作成する
    //---------------------------------------------------------------------
    LNRESULT DepthBuffer::_create()
    {
        HRESULT hr;
        IDirect3DDevice9* dx_device = mGraphicsDevice->getDX9Device();

        LN_DXCALL_R(
		    dx_device->CreateDepthStencilSurface(
                static_cast< UINT >( mSize.x ),
				static_cast< UINT >( mSize.y ),
				DX9Module::convertLNFormatToDxFormat( mFormat ),
				D3DMULTISAMPLE_NONE,	// 高度な補間方法の設定
				0,						// 画像の品質レベルの設定
				TRUE,					// 深度バッファの制御方法の指定
				&mDxSurface,
				NULL ) );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 解放処理
    //---------------------------------------------------------------------
    void DepthBuffer::_dispose()
    {
        SAFE_RELEASE( mDxSurface );
    }

//=============================================================================
// ■ SystemCreatedTexture クラス
//=============================================================================

	//---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
	SystemCreatedTexture::SystemCreatedTexture()
        : mDxTexture        ( NULL )
        , mDxSurface        ( NULL )
        //, mFont             ( NULL )
    {
    }

	//---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
	SystemCreatedTexture::~SystemCreatedTexture()
    {
        SAFE_RELEASE( mDxTexture );
        SAFE_RELEASE( mDxSurface );
        //SAFE_RELEASE( mDxSystemSurface );
    }

    //---------------------------------------------------------------------
    // ● テクスチャ、サーフェイスのセット
    //---------------------------------------------------------------------
    LNRESULT SystemCreatedTexture::setTextureAndSurface( IDirect3DTexture9* texture_, IDirect3DSurface9* surface_ )
    {
        SAFE_ADDREF( texture_ );
        SAFE_ADDREF( surface_ );

        SAFE_RELEASE( mDxTexture );
        SAFE_RELEASE( mDxSurface );

        mDxTexture = texture_;
        mDxSurface = surface_;

        

        if ( mDxSurface )
        {
            D3DSURFACE_DESC desc;
            mDxSurface->GetDesc( &desc );

            //DX9Module::convertFormatDxToLN

            mRealSize.x = mSize.x = static_cast< lnfloat >( desc.Width );
            mRealSize.y = mSize.y = static_cast< lnfloat >( desc.Height );
            mRealSizeInv.x = 1.0f / mRealSize.x;
            mRealSizeInv.y = 1.0f / mRealSize.y;
        }

        return LN_OK;
    }

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