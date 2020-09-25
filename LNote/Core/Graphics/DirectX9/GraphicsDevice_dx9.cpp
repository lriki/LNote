//=============================================================================
//【 GraphicsDevice 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../File/Manager.h"
#include "DX9Module.h"
#include "Renderer.h"
#include "Canvas.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"
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
// ■ GraphicsDevice クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    GraphicsDevice::GraphicsDevice()
        : mLogFile                  ( NULL )
        , mRenderer                 ( NULL )
        , mDirect3D                 ( NULL )
        , mDxDevice                 ( NULL )
        , mMainWindow               ( NULL )
        , mFileManager              ( NULL )
        , mCurrentMinFilter         ( D3DTEXF_POINT )
        , mCurrentMagFilter         ( D3DTEXF_POINT )
        , mCurrentMipFilter         ( D3DTEXF_NONE )
        , mBestMinFilter            ( D3DTEXF_POINT )
        , mBestMagFilter            ( D3DTEXF_POINT )
        , mBestMipFilter            ( D3DTEXF_NONE )
        , mMSAAQualityWindowed      ( 0 )
        , mMSAAQualityFullScreen    ( 0 )
        , mDeviceType               ( D3DDEVTYPE_HAL )
        , mDefaultCanvas            ( NULL )
        , mIsDeviceLost             ( true )
        , mIsFullScreen             ( false )
        , mEnableFPUPreserve        ( false )
        , mDeviceLostEventSemaphore ( 1, 1 )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    GraphicsDevice::~GraphicsDevice()
    {
        SAFE_RELEASE( mDxDevice );
        SAFE_RELEASE( mDirect3D );
        LN_SAFE_RELEASE( mMainWindow );
        LN_SAFE_RELEASE( mSystemManager );
    }

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        LNRESULT lr;

        mManager            = init_data_.Manager;
        mEnableFPUPreserve  = init_data_.EnableFPUPreserve;
        mSystemManager      = init_data_.SystemManager;
        mMainWindow         = init_data_.SystemManager->getWindow();
        mFileManager        = init_data_.FileManager;
        LN_SAFE_ADDREF( mSystemManager );
        LN_SAFE_ADDREF( mMainWindow );

        // DLL 読み込み
        LN_CALL_R( DX9Module::getInstance()->initialize() );

        // Direct3D 作成
        mDirect3D = DX9Module::getInstance()->Direct3DCreate9( D3D_SDK_VERSION );
	    LN_SETERR_R_S_( ( mDirect3D ), LN_ERR_SYSTEM,
            "#Direct3D の初期化に失敗しました。\n"
            "DirectX が正しくインストールされていないか、\n"
            "お使いのビデオカードのドライバーが対応していない可能性があります。" );

        // デバイスの性能チェック   
        LN_CALL_R( _checkDeviceInformation() );

        // 基本的に変化のないプレゼンテーションパラメータの設定
		// ( PresentationInterval は D3DPRESENT_INTERVAL_IMMEDIATE 以外の場合、
		//   DirectShow を使って動画再生した時に画面が表示されなくなることがある )
		ZeroMemory( &mPresentParameters, sizeof( D3DPRESENT_PARAMETERS ) );
		mPresentParameters.BackBufferCount				= 1;
		mPresentParameters.EnableAutoDepthStencil		= FALSE;//TRUE;//					// これはいらない。不具合出たらもどしておく
		mPresentParameters.AutoDepthStencilFormat		= D3DFMT_D24S8;//D3DFMT_UNKNOWN;//
		mPresentParameters.SwapEffect					= D3DSWAPEFFECT_DISCARD;	// マルチサンプリングするにはコレらしい
		mPresentParameters.PresentationInterval			= ( init_data_.EnableVSyncWait ) ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;//D3DPRESENT_INTERVAL_IMMEDIATE(アダプタの更新間隔無視); // 	//D3DPRESENT_INTERVAL_DEFAULT(アダプタの更新間隔に合わせる);	// 画面の更新間隔
		mPresentParameters.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
        mPresentParameters.hDeviceWindow				= mMainWindow->getWindowHandle();
		//gPresentParameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

        // デバイス作成
        LN_CALL_R( resetDevice( init_data_.FullScreen, mMainWindow->getSize() ) );

        

        // キャッシュサイズを設定
        TextureCache::initialize( init_data_.TextureCacheSize );
        ShaderCache::initialize( init_data_.ShaderCacheSize ); 

        // デフォルトのキャンバス (コンストラクタでリストに入れられる)
        mDefaultCanvas = LN_NEW DefaultCanvas( this );
        LN_CALL_R( mDefaultCanvas->initialize( mMainWindow->getSize() ) );

        // 描画クラスの作成
        Renderer::InitData rend_data;
        rend_data.LogFile           = mLogFile;
        rend_data.GraphicsDevice    = this;
        rend_data.DxCaps            = &mDxCaps;
        mRenderer = LN_NEW Renderer();
        LN_CALL_R( mRenderer->initialize( rend_data ) );

        // Renderer::setRenderTarget() で mDefaultCanvas を使ってるとかの都合で、
        // 上記二つのクラスの初期化が終わった後に設定しておく必要がある
        LN_CALL_R( mDefaultCanvas->activate() );

        LN_LOG_INIT_END;
        return LN_OK;
    }


    //---------------------------------------------------------------------
	// ● 終了処理
    //---------------------------------------------------------------------
    void GraphicsDevice::finalize()
    {
        LN_LOG_FIN_BEGIN;

        LN_SAFE_RELEASE( mDefaultCanvas );
        LN_SAFE_RELEASE( mRenderer );

		

        ShaderCache::finalize();
        TextureCache::finalize();

		
        
        LN_LOG_FIN_END;
    }

    //---------------------------------------------------------------------
    // ● 描画クラスの取得
    //---------------------------------------------------------------------
    IRenderer* GraphicsDevice::getRenderer()
    {
        return mRenderer;
    }

    //---------------------------------------------------------------------
    // ● デフォルトのバックバッファを示すキャンバスの取得
    //---------------------------------------------------------------------
    ICanvas* GraphicsDevice::getDefaultCanvas()
    {
        return mDefaultCanvas;
    }

    //---------------------------------------------------------------------
    // ● 頂点バッファの作成
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createVertexBuffer( IVertexBuffer** obj_, LNVertexElemenst* elements_, u32 vertex_num_, const void* data_, bool is_dynamic_ )
    {
        LNRESULT lr;
        *obj_ = NULL;
            
        VertexBuffer* vb = LN_NEW VertexBuffer();
        LN_CALL_R( vb->initialize( this, elements_, vertex_num_, data_, is_dynamic_ ) );

        *obj_ = vb;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● インデックスバッファの作成
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createIndexBuffer( IIndexBuffer** obj_, u32 index_num_, const u16* data_, bool is_dynamic_, bool is_16bit_ )
    {
        LNRESULT lr;
        *obj_ = NULL;

        IndexBuffer* ib = LN_NEW IndexBuffer();
        LN_CALL_R( ib->initialize( this, index_num_, data_, is_dynamic_, is_16bit_ ) );

        *obj_ = ib;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● テクスチャの作成
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createTexture( ITexture** obj_, u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ )
    {
        LNRESULT lr;
        *obj_ = NULL;

        Texture* tex = LN_NEW Texture( this );
        LN_CALL_R( tex->initialize( width_, height_, levels_, format_ ) );

        *obj_ = tex;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● テクスチャの作成 ( メモリに展開された画像データから )
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createTexture( ITexture** obj_, const void* data_, u32 size_, u32 color_key_, u32 levels_, LNSurfaceFormat format_, LSharingKey key_ )
    {
        LNRESULT lr;
        *obj_ = NULL;

        // キャッシュを検索
        *obj_ = TextureCache::findCacheAddRef( key_ );
        if ( *obj_ )
        {
            return LN_OK;
        }
        
        // 新しく作る
        Texture* texture = LN_NEW Texture( this );
        LN_CALL_R( texture->initialize( data_, size_, color_key_, levels_, format_ ) );

        // キャッシュ管理クラスに追加
        TextureCache::registerCachedObject( key_, texture );
        
        *obj_ = texture;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● テクスチャの作成 (入力ストリームから)
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createTexture( ITexture** obj_, File::IInStream* stream_, u32 color_key_, u32 levels_, LNSurfaceFormat format_, LSharingKey key_ )
    {
        u32 size = stream_->getSize();
        *obj_ = NULL;

        // TODO:TempBufferLock

        lnByte* buf = mTempBuffer.lock( size );
        stream_->read( buf, size );

        LNRESULT lr = createTexture( obj_, buf, size, color_key_, levels_, format_, key_ );

        mTempBuffer.unlock();
        return lr;
    }

    //---------------------------------------------------------------------
    // ● テクスチャの作成 (ファイルから)
    //---------------------------------------------------------------------
    void GraphicsDevice::createTexture( ITexture** obj_, const lnChar* filename_, u32 color_key_, u32 levels_, LNSurfaceFormat format_, LSharingKey key_ )
    {
        // ファイル作成
        File::IInFile* file = NULL;
        mFileManager->createInFile( &file, filename_ );
        Base::FinallyReleasePtr< File::IInFile > file_ptr( file );

        createTexture( obj_, file, color_key_, levels_, format_, key_ );
    }

    //---------------------------------------------------------------------
    // ● レンダーターゲットテクスチャの作成
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createRenderTarget( ITexture** obj_, u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ )
    {
        LNRESULT lr;
        *obj_ = NULL;

        RenderTargetTexture* tex = LN_NEW RenderTargetTexture( this );
        LN_CALL_R( tex->initialize( width_, height_, levels_, format_ ) );

        mRenderTargetList.push_back( tex );

        *obj_ = tex;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 深度バッファの作成
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createDepthBuffer( ITexture** obj_, u32 width_, u32 height_, LNSurfaceFormat format_ )
    {
        LNRESULT lr;
        *obj_ = NULL;

        DepthBuffer* tex = LN_NEW DepthBuffer( this );
        LN_CALL_R( tex->initialize( width_, height_, format_ ) );

        mDepthBufferList.push_back( tex );

        *obj_ = tex;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● シェーダの作成
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createShader( IShader** obj_, const void* data_, u32 size_, LSharingKey key_ )
    {
#if 0
        LNRESULT lr;
        *obj_ = NULL;

        // キャッシュを検索
        *obj_ = ShaderCache::findCacheAddRef( key_ );
        if ( *obj_ )
        {
            return LN_OK;
        }
        
        // 新しく作る
        Shader* shader = LN_NEW Shader( this );
        LN_CALL_R( shader->initialize( static_cast< const char* >( data_ ) ) );

        // キャッシュ管理クラスに追加
        ShaderCache::registerCachedObject( key_, shader );

        mShaderList.push_back( shader );
        
        *obj_ = shader;
#endif
        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● シェーダの作成 ( 入力ストリームから )
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createShader( IShader** obj_, File::IInStream* stream_, LSharingKey key_ )
    {
#if 0
        u32 size = stream_->getSize();
        *obj_ = NULL;

        lnByte* buf = mTempBuffer.lock( size + 1 );

        // mTempBuffer に全部読み込む
        stream_->read( buf, size );

        // Cg に渡すときは終端 NULL が必要
        buf[ size ] = '\0';

        LNRESULT lr = createShader( obj_, buf, size, key_ );

        mTempBuffer.unlock();
#endif
        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }
	
    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GraphicsDevice::createShader( IShader** obj_, const lnChar* filename_, LSharingKey key_ )
    {
        // キャッシュを検索
        Shader* shader = ShaderCache::findCacheAddRef( key_ );
        if ( shader )
        {
            *obj_ = shader;
            return;
            //return shader;
        }

        // ファイル作成
        File::IInFile* file = NULL;
        mFileManager->createInFile( &file, filename_ );
        Base::FinallyReleasePtr< File::IInFile > file_ptr( file );

        // mTempBuffer に全部読み込む
        lnU32 size = file->getSize();

        Base::TempBufferLock tlock( mTempBuffer, size + 1 );
        char* buf = (char*)tlock.getBuffer();
        file->read( buf, size );
        buf[ size ] = '\0';

        // Shader 作成
        Base::FinallyReleasePtr< Shader > shader_ptr( LN_NEW Shader( this ) );
        shader_ptr->initialize( buf );
        if ( shader_ptr->getCompileResult() == LN_SHADERRESULT_ERROR )
        {
			ln_std_tstring str = filename_;
			str += _T("\n");
			str += shader_ptr->getCompileErrors();
			LN_THROW_InvalidCall( 0, str.c_str() );
        }
        

        // キャッシュ管理に追加
        ShaderCache::registerCachedObject( key_, shader_ptr );

        mShaderList.push_back( shader_ptr );

        *obj_ = shader_ptr.returnObject();
        return;
        //return shader_ptr.returnObject();

        //LN_PRINT_NOT_IMPL_FUNCTION;
        //*obj_ = mShaderManager->createShader( filename_, key_ );
    }

    //---------------------------------------------------------------------
    // ● キャッシュのクリア
    //---------------------------------------------------------------------
    void GraphicsDevice::clearCache( bool texture_, bool shader_ )
    {
        if ( texture_ ) TextureCache::clear();
        if ( shader_  ) ShaderCache::clear();
    }

    //---------------------------------------------------------------------
    // ● デバイスリセット前処理
    //---------------------------------------------------------------------
    void GraphicsDevice::onLostDevice()
    {
        LN_PCALL( mRenderer->onLostDevice() );
        
        ln_foreach( Shader* t, mShaderList )
        {
            LN_PCALL( t->onLostDevice() );
        }

        ln_foreach( DepthBuffer* t, mDepthBufferList )
        {
            LN_PCALL( t->onLostDevice() );
        }

        ln_foreach( RenderTargetTexture* t, mRenderTargetList )
        {
            LN_PCALL( t->onLostDevice() );
        }

        ln_foreach( CanvasBase* t, mCanvasList )
        {
            LN_PCALL( t->onLostDevice() );
        }
    }

    //---------------------------------------------------------------------
    // ● デバイスリセット後処理
    //---------------------------------------------------------------------
    void GraphicsDevice::onResetDevice()
    {
        ln_foreach( CanvasBase* t, mCanvasList )
        {
            LN_PCALL( t->onResetDevice() );
        }
        
        ln_foreach( RenderTargetTexture* t, mRenderTargetList )
        {
            LN_PCALL( t->onResetDevice() );
        }
       
        ln_foreach( DepthBuffer* t, mDepthBufferList )
        {
            LN_PCALL( t->onResetDevice() );
        }
        
        ln_foreach( Shader* t, mShaderList )
        {
            LN_PCALL( t->onResetDevice() );
        }

        for ( int i = 0; i < LN_MAX_SAMPLER_STAGE_INDEX; ++i )
        {
            setSamplerState( i, mSamplerStatus[i], true );
        }

        // ビューポートサイズのリセットにバックバッファサイズを使っているため、
        // デフォルトキャンバスの onResetDevice() の後でないと
        // バックバッファサイズが古いままになってしまう
        LN_PCALL( mRenderer->onResetDevice() );
    }

    //---------------------------------------------------------------------
    // ● デバイスのリセット
    //---------------------------------------------------------------------
    void GraphicsDevice::resetDevice()
    {
        LN_PCALL(
            resetDevice( mIsFullScreen, mDefaultCanvas->getSize() ));
    }

    //---------------------------------------------------------------------
    // ● デバイスのリセット
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::resetDevice( bool fullscreen_, const Geometry::Size& window_bb_size_ )
    {
        HRESULT hr;
        LNRESULT lr;

        // プレゼンテーションパラメータ設定
        mIsFullScreen = ( canFullScreen() ) ? fullscreen_ : false;
		_setPresentParameters( window_bb_size_ );

        // まだデバイスが作成されていなければ新規作成
        if ( !mDxDevice )
        {
            // D3Dデバイスの生成
            DWORD fpu_precision = ( mEnableFPUPreserve ) ? D3DCREATE_FPU_PRESERVE : 0;
            LN_DXCALL_R(
                mDirect3D->CreateDevice(
				    D3DADAPTER_DEFAULT,
				    mDeviceType,
				    mPresentParameters.hDeviceWindow,
				    fpu_precision | D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,//D3DCREATE_SOFTWARE_VERTEXPROCESSING, | D3DCREATE_MULTITHREADED
				    &mPresentParameters,
				    &mDxDevice ) );
        }
        // 既に作成されている場合はリセット
        else
        {
            onLostDevice();

            // デバイスのリセット
            LN_DXCALL_R( mDxDevice->Reset( &mPresentParameters ) );

        //    IDirect3DSurface9* rp;
        //mDxDevice->GetRenderTarget(0, &rp);
        //_p(rp->Release());

        //D3DSURFACE_DESC rrd;
        //rp->GetDesc(&rrd);

            onResetDevice();
        }

        mIsDeviceLost.setFalse();

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // 
    //---------------------------------------------------------------------
    void GraphicsDevice::postDeviceLostEvent()
    {
        if ( !mIsDeviceLost.isTrue() && mDeviceLostEventSemaphore.decrease( 1 ) )
        {
            mIsDeviceLost.setTrue();

            //System::Event* ev = LN_NEW System::Event();
            //ev->Type = LNEV_DEVICE_LOST;
            //mSystemManager->postEvent( ev );

            mDeviceLostEventSemaphore.increase();
        }
    }

    //---------------------------------------------------------------------
    // 
    //---------------------------------------------------------------------
    void GraphicsDevice::setSamplerState( int index_, const LSamplerState& state_, bool reset_ )
    {
        const DWORD filter_table[] = 
        {
            D3DTEXF_NONE,
            D3DTEXF_POINT,
            D3DTEXF_LINEAR,
        };

        const DWORD address_table[] = 
        {
            D3DTADDRESS_WRAP,
            D3DTADDRESS_CLAMP,
        };

        // MagFilter
        if ( state_.MagFilter != mSamplerStatus[index_].MagFilter || reset_ )
        {
            mDxDevice->SetSamplerState( index_, D3DSAMP_MAGFILTER, filter_table[state_.MagFilter] );
            mSamplerStatus[index_].MagFilter = state_.MagFilter;
        }
        // MinFilter
        if ( state_.MinFilter != mSamplerStatus[index_].MinFilter || reset_ )
        {
            mDxDevice->SetSamplerState( index_, D3DSAMP_MINFILTER, filter_table[state_.MinFilter] );
            mSamplerStatus[index_].MinFilter = state_.MinFilter;
        }
        // MipFilter
        if ( state_.MipFilter!= mSamplerStatus[index_].MipFilter || reset_ )
        {
            mDxDevice->SetSamplerState( index_, D3DSAMP_MIPFILTER, filter_table[state_.MipFilter] );
            mSamplerStatus[index_].MipFilter = state_.MipFilter;
        }

        // AddressU
        if ( state_.AddressU!= mSamplerStatus[index_].AddressU || reset_ )
        {
            mDxDevice->SetSamplerState( index_, D3DSAMP_ADDRESSU, address_table[state_.AddressU] );
            mSamplerStatus[index_].AddressU = state_.AddressU;
        }
        // AddressV
        if ( state_.AddressV!= mSamplerStatus[index_].AddressV || reset_ )
        {
            mDxDevice->SetSamplerState( index_, D3DSAMP_ADDRESSV, address_table[state_.AddressV] );
            mSamplerStatus[index_].AddressV = state_.AddressV;
        }
    }

    //---------------------------------------------------------------------
    // ● デバイスの性能チェック
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::_checkDeviceInformation()
    {
        // 使えるデバイスの種類を調べる
		HRESULT hr = mDirect3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &mDxCaps );
        if ( SUCCEEDED( hr ) )
		{
			mDeviceType = D3DDEVTYPE_HAL;
		}
		else
		{
			hr = mDirect3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, &mDxCaps );
			if ( SUCCEEDED( hr ) )
			{
				mDeviceType = D3DDEVTYPE_REF;
			}
			else
			{
				hr = mDirect3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_SW, &mDxCaps );
				if ( SUCCEEDED( hr ) )
				{
					mDeviceType = D3DDEVTYPE_SW;
				}
				else
				{
					LN_SETDXERR_R_S( hr, "#使用できるグラフィックデバイスが見つかりませんでした。" );
				}
			}
		}

        // ディスプレイの情報を取得
		LN_DXCALL_R( mDirect3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &mDxDisplayMode ) );

        // アンチエイリアスする場合の品質
		// ウィンドウモードのときは？
		hr = mDirect3D->CheckDeviceMultiSampleType(
			D3DADAPTER_DEFAULT,  mDeviceType, 
            mDxDisplayMode.Format, TRUE,
			D3DMULTISAMPLE_NONMASKABLE,
			&mMSAAQualityWindowed );
		if ( FAILED( hr ) )
		{
			mMSAAQualityWindowed = 0;
		}
		// フルスクリーンのときは？
		hr = mDirect3D->CheckDeviceMultiSampleType(
			D3DADAPTER_DEFAULT,  mDeviceType, 
			mDxDisplayMode.Format, FALSE,
			D3DMULTISAMPLE_NONMASKABLE,
			&mMSAAQualityFullScreen );
		if ( FAILED( hr ) )
		{
			mMSAAQualityFullScreen = 0;
		}

        // 一番いいっぽいテクスチャフィルタ取得 ( あとでもっと追加しておく )
		DWORD tfcaps = mDxCaps.TextureFilterCaps;
		// 縮小
		if ( tfcaps & D3DPTFILTERCAPS_MIPFLINEAR )
		{
			mBestMipFilter = D3DTEXF_LINEAR;
		}
		else if ( tfcaps & D3DPTFILTERCAPS_MIPFPOINT )
		{
			mBestMipFilter = D3DTEXF_POINT;
		}
		// 拡大
		if ( tfcaps & D3DPTFILTERCAPS_MAGFANISOTROPIC )
		{
			mBestMagFilter = D3DTEXF_ANISOTROPIC;
		}
		else if ( tfcaps & D3DPTFILTERCAPS_MAGFLINEAR )
		{
			mBestMagFilter = D3DTEXF_LINEAR;
		}
		// ミップマップ
		if ( tfcaps & D3DPTFILTERCAPS_MINFANISOTROPIC )
		{
			mBestMinFilter = D3DTEXF_ANISOTROPIC;
		}
		else if ( tfcaps & D3DPTFILTERCAPS_MINFLINEAR )
		{
			mBestMinFilter = D3DTEXF_LINEAR;
		}

        LN_LOG_WRITE( "グラフィックデバイスの情報を取得します..." );
        LN_LOG_WRITE( "    スクリーンの幅             : %u", mDxDisplayMode.Width );
        LN_LOG_WRITE( "    スクリーンの高さ           : %u", mDxDisplayMode.Height );
        LN_LOG_WRITE( "    リフレッシュレート         : %u", mDxDisplayMode.RefreshRate );
        LN_LOG_WRITE( "    ディスプレイフォーマット   : %s", DX9Module::getDxFormatString( mDxDisplayMode.Format ) );
        LN_LOG_WRITE( "    テクスチャ" );
        LN_LOG_WRITE( "        最大横幅               : %u", mDxCaps.MaxTextureWidth );
        LN_LOG_WRITE( "        最大縦幅               : %u", mDxCaps.MaxTextureHeight );
        LN_LOG_WRITE( "        2 の累乗制限           : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_POW2 ? "あり" : "なし" );
        LN_LOG_WRITE( "        2 の累乗制限の限定解除 : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL ? "あり" : "なし" );
        LN_LOG_WRITE( "        正方形制限             : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY ? "あり" : "なし" );
        LN_LOG_WRITE( "        動的テクスチャ         : %s", mDxCaps.Caps2 & D3DCAPS2_DYNAMICTEXTURES ? "○" : "×" );
        LN_LOG_WRITE( "    キューブテクスチャ         : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP ? "○" : "×" );
        LN_LOG_WRITE( "        2 の累乗制限           : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP_POW2 ? "あり" : "なし" );
        LN_LOG_WRITE( "    ボリュームテクスチャ       : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP ? "○" : "×" );
        LN_LOG_WRITE( "        2 の累乗制限           : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP_POW2 ? "あり" : "なし" );
        LN_LOG_WRITE( "        ﾃﾞｨﾒﾝｼﾞｮﾝ最大値        : %u", mDxCaps.MaxVolumeExtent );
        LN_LOG_WRITE( "    異方性の次数               : %u", mDxCaps.MaxAnisotropy );
        LN_LOG_WRITE( "    異方性フィルタ拡大         : %s", mDxCaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC ? "○" : "×" );
        LN_LOG_WRITE( "    異方性フィルタ縮小         : %s", mDxCaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC ? "○" : "×" );
        LN_LOG_WRITE( "    最大プリミティブ数         : %u", mDxCaps.MaxPrimitiveCount );
        LN_LOG_WRITE( "    最大インデックス数         : %u", mDxCaps.MaxVertexIndex );
        LN_LOG_WRITE( "    頂点シェーダバージョン     : %u.%u", D3DSHADER_VERSION_MAJOR(mDxCaps.VertexShaderVersion), D3DSHADER_VERSION_MINOR(mDxCaps.VertexShaderVersion) );
        LN_LOG_WRITE( "    ピクセルシェーダバージョン : %u.%u", D3DSHADER_VERSION_MAJOR(mDxCaps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(mDxCaps.PixelShaderVersion) );
        LN_LOG_WRITE( "    マルチレンダーターゲット数 : %u", mDxCaps.NumSimultaneousRTs );
        LN_LOG_WRITE( "    MSAA レベル(Window)        : %u", mMSAAQualityWindowed );
        LN_LOG_WRITE( "    MSAA レベル(FullScreen)    : %u", mMSAAQualityFullScreen );

        // シェーダのバージョンチェック
		LN_SETERR_R_S_( ( mDxCaps.VertexShaderVersion >= D3DVS_VERSION( 2, 0 ) ), LN_ERR_FATAL, "#お使いの環境は頂点シェーダ2.0に対応していません。" );
		LN_SETERR_R_S_( ( mDxCaps.PixelShaderVersion  >= D3DPS_VERSION( 2, 0 ) ), LN_ERR_FATAL, "#お使いの環境はピクセルシェーダ2.0に対応していません。" );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● スクリーンモード ( mIsFullScreen ) に応じて mPresentParameters 設定
	//---------------------------------------------------------------------
	void GraphicsDevice::_setPresentParameters( const Geometry::Size& window_bb_size_ )
	{
		// フルスクリーンモードの場合
		if ( mIsFullScreen == true )
		{
            mPresentParameters.Windowed = TRUE;
			mPresentParameters.BackBufferWidth  = mDxDisplayMode.Width; 
			mPresentParameters.BackBufferHeight = mDxDisplayMode.Height;
			mPresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;//D3DFMT_X8B8G8R8;//
			if ( false )//( mMSAAQualityWindowed > 0 )
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
				mPresentParameters.MultiSampleQuality = mMSAAQualityWindowed - 1;
			}
			else
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
				mPresentParameters.MultiSampleQuality = 0;
			}
            /*
			mPresentParameters.Windowed = FALSE;
			mPresentParameters.BackBufferWidth  = mDxDisplayMode.Width; 
            mPresentParameters.BackBufferHeight = mDxDisplayMode.Height;
            mPresentParameters.BackBufferFormat = mDxDisplayMode.Format;
			// 品質レベルが 0 より大きい場合
			if ( false )//( mMSAAQualityFullScreen > 0 )
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
				mPresentParameters.MultiSampleQuality = mMSAAQualityFullScreen - 1;
			}
			else
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
				mPresentParameters.MultiSampleQuality = 0;
			}
            */
		}
		// ウィンドウモードの場合
		else
		{
			mPresentParameters.Windowed = TRUE;
            mPresentParameters.BackBufferWidth  = window_bb_size_.w; 
            mPresentParameters.BackBufferHeight = window_bb_size_.h;
			mPresentParameters.BackBufferFormat = D3DFMT_X8R8G8B8;//D3DFMT_UNKNOWN;//D3DFMT_X8R8G8B8;//
			if ( false )//( mMSAAQualityWindowed > 0 )
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
				mPresentParameters.MultiSampleQuality = mMSAAQualityWindowed - 1;
			}
			else
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
				mPresentParameters.MultiSampleQuality = 0;
			}
		}
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