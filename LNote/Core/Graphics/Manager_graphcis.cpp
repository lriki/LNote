//=============================================================================
//【 Manager 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../System/Interface.h"
#if defined(LNOTE_DIRECTX)
    #include "DirectX9/GraphicsDevice.h"
#endif
#if defined(LNOTE_WINDOWS)
    #include "Font/Win32/Win32FontManager.h"
#elif defined(LNOTE_ANDROID)
    #include "Font/Android/AndroidFontManager.h"
#endif
#include "OpenGL/GLGraphicsDevice.h"
#include "Common/SpriteRenderer.h"
#include "Common/Model/ModelManager.h"
#include "Common/DebugText.h"
#include "Manager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

//=============================================================================
// ■ Manager クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    Manager::Manager()
        : mLogFile          ( NULL )
        , mFileManager      ( NULL )
        , mTaskManager      ( NULL )
        , mGraphicsDevice   ( NULL )
        , mGraphicsAPI      ( LN_GRAPHICSAPI_DIRECTX9 )
        , mFontManager      ( NULL )
        , mGeometryRenderer ( NULL )
        , mSprite3DRenderer ( NULL )
        , mSprite2DRenderer ( NULL )
        , mModelManager     ( NULL )
        , mDebugText        ( NULL )
        , mDummyTexture     ( NULL )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    Manager::~Manager()
    {
        LN_SAFE_RELEASE( mFileManager );
        LN_SAFE_RELEASE( mDebugText );
    }

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    LNRESULT Manager::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        LNRESULT lr;

        mGraphicsAPI = init_data_.GraphicsAPI;
        mFileManager = init_data_.FileManager;
        mTaskManager = init_data_.TaskManager;
        LN_SAFE_ADDREF( mFileManager );

#if !defined(LNOTE_WINDOWS)
        mGraphicsAPI = LN_GRAPHICSAPI_OPENGL;
#endif
        // DirectX 9
        if ( mGraphicsAPI == LN_GRAPHICSAPI_DIRECTX9 )
        {
#if defined(LNOTE_DIRECTX)
            // フルスクリーンは、基本的には単純にウィンドウを最大化することで行う。
            // そのため、フルスクリーンと垂直同期は OFF。
            DirectX9::GraphicsDevice::InitData init_data;
            init_data.LogFile           = mLogFile;
            init_data.Manager           = this;    
            init_data.SystemManager     = init_data_.SystemManager;
            init_data.FileManager       = mFileManager;
            init_data.CanvasSize        = init_data_.CanvasSize;
            init_data.FullScreen        = false;
            init_data.EnableVSyncWait   = true;
            init_data.EnableFPUPreserve = init_data_.EnableFPUPreserve;
            init_data.TextureCacheSize  = 32;
            init_data.ShaderCacheSize   = 32;
            DirectX9::GraphicsDevice* device = LN_NEW DirectX9::GraphicsDevice();
            LN_CALL_R( device->initialize( init_data ) );

            mGraphicsDevice = device;
#endif
        }
        // DirectX11
        else if ( mGraphicsAPI == LN_GRAPHICSAPI_DIRECTX11 )
        {

        }
        // OpenGL
        else //if ( init_data_.GraphicsAPI == LN_GRAPHICSAPI_OPENGL )
        {
            OpenGL::GraphicsDevice::InitData init_data;
            init_data.LogFile           = mLogFile;
            init_data.Manager           = this;    
            init_data.SystemManager     = init_data_.SystemManager;
            init_data.FileManager       = mFileManager;
            init_data.CanvasSize        = init_data_.CanvasSize;
            init_data.EnableVSyncWait   = true;
            init_data.TextureCacheSize  = 32;
            init_data.ShaderCacheSize   = 32;
            OpenGL::GraphicsDevice* device = LN_NEW OpenGL::GraphicsDevice();
            device->initialize( init_data );
            mGraphicsDevice = device;
        }

#pragma message( __FUNCTION__" ◆ フォント管理とか形状描画とか一時的に無効にしてある" ) 


        //-----------------------------------------------------
        // 
        
        // フォント管理
#if defined(LNOTE_WINDOWS)
        Win32::Win32FontManager* fm = LN_NEW Win32::Win32FontManager();
        fm->initialize( mFileManager );
        mFontManager = fm;
#elif defined(LNOTE_ANDROID)
        Android::AndroidFontManager* fm = LN_NEW Android::AndroidFontManager();
        fm->initialize( mFileManager );
        mFontManager = fm;
#endif
        LNFontNameList::const_iterator itr = init_data_.FontFilenameList.begin();
        LNFontNameList::const_iterator end = init_data_.FontFilenameList.end();
        for ( ; itr != end; ++itr )
        {
            mFontManager->addFontFile( itr->c_str() );
        }

        // 形状描画クラス
        GeometryRenderer::InitData gr_data;
        gr_data.LogFile         = mLogFile;
        gr_data.GraphicsDevice  = mGraphicsDevice;
        mGeometryRenderer = LN_NEW GeometryRenderer();
        LN_CALL_R( mGeometryRenderer->initialize( gr_data ) );

        // 3Dスプライト描画クラス
        mSprite3DRenderer = LN_NEW SpriteRenderer( this );
        mSprite3DRenderer->initialize( mGraphicsDevice, 512, true );

        // 2Dスプライト描画クラス
        mSprite2DRenderer = LN_NEW SpriteRenderer( this );
        mSprite2DRenderer->initialize( mGraphicsDevice, 512, false );

        // モデル管理クラス
        ModelManager::InitData mo_data;
        mo_data.LogFile             = mLogFile;
        mo_data.GraphicsDevice      = mGraphicsDevice;
        mo_data.FileManager         = mFileManager;
        mo_data.PhysicsManager      = init_data_.PhysicsManager;
        mo_data.ModelCoreCacheSize  = 32;
        mModelManager = LN_NEW ModelManager();
        LN_CALL_R( mModelManager->initialize( mo_data ) );

        // デバッグ文字描画クラス
        mDebugText = LN_NEW DebugText();
        mDebugText->initialize( mGraphicsDevice, mSprite2DRenderer );

        // ダミーテクスチャ
        LN_CALL_R( mGraphicsDevice->createTexture( &mDummyTexture, 32, 32, 1, LN_FMT_A8R8G8B8 ) );
        mDummyTexture->clear( LColor( 1, 1, 1, 1 ) );

        LN_LOG_INIT_END;
        return LN_OK;
    }


    //---------------------------------------------------------------------
	// ● 終了処理
    //---------------------------------------------------------------------
    void Manager::finalize()
    {
        LN_LOG_FIN_BEGIN;

        LN_SAFE_RELEASE( mDummyTexture );

        if ( mModelManager )
        {
            mModelManager->finalize();
            LN_SAFE_RELEASE( mModelManager );
        }

        LN_SAFE_RELEASE( mSprite2DRenderer );
        LN_SAFE_RELEASE( mSprite3DRenderer );
        LN_SAFE_RELEASE( mGeometryRenderer );

        if ( mFontManager )
        {
            mFontManager->finalize();
            LN_SAFE_RELEASE( mFontManager );
        }

        if ( mGraphicsDevice )
        {
            switch ( mGraphicsAPI )
            {
#if defined(LNOTE_DIRECTX)
            case LN_GRAPHICSAPI_DIRECTX9:
                dynamic_cast< DirectX9::GraphicsDevice* >( mGraphicsDevice )->finalize();
                break;
            case LN_GRAPHICSAPI_DIRECTX11:
                break;
#endif
            case LN_GRAPHICSAPI_OPENGL:
                dynamic_cast< OpenGL::GraphicsDevice* >( mGraphicsDevice )->finalize();
                break;
            }
            LN_SAFE_RELEASE( mGraphicsDevice );
        }

        LN_LOG_FIN_END;
    }

    //---------------------------------------------------------------------
    // ● IGraphicsDevice の取得
    //---------------------------------------------------------------------
    IGraphicsDevice* Manager::getGraphicsDevice()
    {
        return mGraphicsDevice;
    }

    //---------------------------------------------------------------------
    // ● フォントの作成
    //---------------------------------------------------------------------
    LNRESULT Manager::createFont( IFont** obj_, const lnChar* fontname_ )
    {
        *obj_ = mFontManager->createFont( fontname_ );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● デフォルトのフォントを取得する
    //---------------------------------------------------------------------
    IFont* Manager::getDefaultFont()
    {
        return mFontManager->getDefaultFont();
    }

    //---------------------------------------------------------------------
    // ● テクスチャをファイルから作成する
    //---------------------------------------------------------------------
//    LNRESULT Manager::createTextureFromFile( ITexture** tex_, const lnChar* filename_, u32 color_key_, u32 levels_, LNSurfaceFormat format_, LSharingKey key_ )
//    {
//        printf("Manager::createTextureFromFile 削除予定\n");
//#if !LNOTE_ANDROID
//        LNRESULT lr;
//        *tex_ = NULL;
//
//        File::IInFile* file;
//        LN_CALL_R( mFileManager->createInFile( &file, filename_ ) );
//
//        lr = mGraphicsDevice->createTexture( tex_,  file, color_key_, levels_, format_, key_ );
//
//        LN_SAFE_RELEASE( file );
//        return lr;
//#endif
//        LN_PRINT_NOT_IMPL_FUNCTION;
//        return LN_OK;
//    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================