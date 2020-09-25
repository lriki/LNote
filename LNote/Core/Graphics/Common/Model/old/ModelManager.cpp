//=============================================================================
//【 ModelManager 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "ModelCore.h"
#include "Motion.h"
#include "ModelManager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Graphics
{

//=============================================================================
// ■ ModelManager クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    ModelManager::ModelManager()
        : mLogFile          ( NULL )
        , mGraphicsDevice   ( NULL )
        , mFileManager      ( NULL )
        , mPhysicsManager   ( NULL )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    ModelManager::~ModelManager()
    {
    }

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    LNRESULT ModelManager::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        mGraphicsDevice = init_data_.GraphicsDevice;
        mFileManager    = init_data_.FileManager;
        mPhysicsManager = init_data_.PhysicsManager;

        ModelCoreCache::initialize( init_data_.ModelCoreCacheSize );

        LN_LOG_INIT_END;
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 終了処理
    //---------------------------------------------------------------------
    void ModelManager::finalize()
    {
        LN_LOG_FIN_BEGIN;

        ModelCoreCache::finalize();

        LN_LOG_FIN_END;
    }

    //---------------------------------------------------------------------
	// ● モデルファイルを読み込む
    //---------------------------------------------------------------------
    LNRESULT ModelManager::createModel( Model** model_, void* data_, lnU32 size_, LSharingKey key_ )
    {
        LNRESULT lr;

        ModelCore* core = LN_NEW ModelCore( this );
        LN_CALL_R( core->loadPMD( data_, size_, true, model_ ) );

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● モデルファイルを読み込む
    //---------------------------------------------------------------------
    LNRESULT ModelManager::createModel( Model** model_, File::IInStream* stream_, LSharingKey key_ )
    {
        lnU32 size = stream_->getSize();
        lnByte* buf = mTempBuffer.lock( size );

        stream_->read( buf, size );

        LNRESULT lr = createModel( model_, buf, size, key_ );

        mTempBuffer.unlock();
        return lr;
    }

    //---------------------------------------------------------------------
	// ● モデルファイルを読み込む (ファイル名指定)
    //---------------------------------------------------------------------
    LNRESULT ModelManager::createModel( Model** model_, const lnChar* filename_, LSharingKey key_ )
    {
        lnChar dir[ LN_MAX_PATH ];
        lnChar filename[ LN_MAX_PATH ];
        File::getDirPathAndFilePath( filename_, dir, filename );

        // ディレクトリ移動
        mFileManager->lock();
        mFileManager->moveCurrentPath( dir );

        LNRESULT lr = LN_OK;
        File::IInFile* file;
        LN_CALL_R( mFileManager->createInFile( &file, filename_ ) );

        lr = createModel( model_, file, key_ );
        
        // ディレクトリ復帰
        mFileManager->returnCurrentPath();
        mFileManager->lock();

        LN_SAFE_RELEASE( file );
        return lr;
    }

    //---------------------------------------------------------------------
	// ● モーションファイルを読み込む
    //---------------------------------------------------------------------
    LNRESULT ModelManager::createMotion( Motion** motion_, void* data_, lnU32 size_, LSharingKey key_ )
    {
        LNRESULT lr;

        Motion* motion = LN_NEW Motion();
        LN_CALL_R( motion->initializeVMD( data_, size_ ) );

        *motion_ = motion;
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● モーションファイルを読み込む
    //---------------------------------------------------------------------
    LNRESULT ModelManager::createMotion( Motion** motion_, File::IInStream* stream_, LSharingKey key_ )
    {
        lnU32 size = stream_->getSize();
        lnByte* buf = mTempBuffer.lock( size );

        stream_->read( buf, size );

        LNRESULT lr = createMotion( motion_, buf, size, key_ );

        mTempBuffer.unlock();
        return lr;
    }

    //---------------------------------------------------------------------
	// ● モーションファイルを読み込む (ファイル名指定)
    //---------------------------------------------------------------------
    LNRESULT ModelManager::createMotion( Motion** motion_, const lnChar* filename_, LSharingKey key_ )
    {
        LNRESULT lr = LN_OK;
        File::IInFile* file;
        LN_CALL_R( mFileManager->createInFile( &file, filename_ ) );

        lr = createMotion( motion_, file, key_ );

        LN_SAFE_RELEASE( file );
        return lr;
    }

    //---------------------------------------------------------------------
	// ● テクスチャ作成
    //---------------------------------------------------------------------
    LNRESULT ModelManager::createTexture( ITexture** texture_, const char* filename_ )
    {
        LNRESULT lr = LN_OK;
        LRefTString filename( filename_ );
        File::IInFile* file;
        LN_CALL_R( mFileManager->createInFile( &file, filename.c_str() ) );

        lr = mGraphicsDevice->createTexture(
            texture_, file, 0, 1, LN_FMT_A8R8G8B8, LSharingKey( filename ) );

        LN_SAFE_RELEASE( file );
        return lr;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Lumine

//=============================================================================
//								end of file
//=============================================================================