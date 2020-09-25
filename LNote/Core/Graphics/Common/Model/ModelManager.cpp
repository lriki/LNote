//=============================================================================
//�y ModelManager �z
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
namespace LNote
{
namespace Core
{
namespace Graphics
{

//=============================================================================
// �� ModelManager �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    ModelManager::ModelManager()
        : mLogFile          ( NULL )
        , mGraphicsDevice   ( NULL )
        , mFileManager      ( NULL )
        , mPhysicsManager   ( NULL )
    {
    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    ModelManager::~ModelManager()
    {
    }

    //---------------------------------------------------------------------
	// �� ������
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
	// �� �I������
    //---------------------------------------------------------------------
    void ModelManager::finalize()
    {
        LN_LOG_FIN_BEGIN;

        ModelCoreCache::finalize();

        LN_LOG_FIN_END;
    }

    //---------------------------------------------------------------------
	// �� ���f���t�@�C����ǂݍ���
    //---------------------------------------------------------------------
    LNRESULT ModelManager::createModel( Model** model_, void* data_, lnU32 size_, lnU32 flags_, LSharingKey key_ )
    {
        LNRESULT lr;

        ModelCore* core = LN_NEW ModelCore( this );

        // PMD
        lr = core->loadPMD( data_, size_, flags_, model_ );
        if ( LN_SUCCEEDED( lr ) ) return LN_OK;

#if defined(LNOTE_DIRECTX)
        // X
        core->loadXFile( data_, size_, flags_, model_ );

#endif

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� ���f���t�@�C����ǂݍ���
    //---------------------------------------------------------------------
    LNRESULT ModelManager::createModel( Model** model_, File::IInStream* stream_, lnU32 flags_, LSharingKey key_ )
    {
        lnU32 size = stream_->getSize();
        lnByte* buf = mTempBuffer.lock( size );

        stream_->read( buf, size );

        LNRESULT lr = createModel( model_, buf, size, flags_, key_ );

        mTempBuffer.unlock();
        return lr;
    }

    //---------------------------------------------------------------------
	// �� ���f���t�@�C����ǂݍ��� (�t�@�C�����w��)
    //---------------------------------------------------------------------
    LNRESULT ModelManager::createModel( Model** model_, const lnChar* filename_, lnU32 flags_, LSharingKey key_ )
    {
        lnChar dir[ LN_MAX_PATH ];
        lnChar filename[ LN_MAX_PATH ];
        File::getDirPathAndFilePath( filename_, dir, filename );

        // �f�B���N�g���ړ�
        mFileManager->lock();
        mFileManager->moveCurrentPath( dir );

        LNRESULT lr = LN_OK;
        File::IInFile* file;
        LN_CALL_R( mFileManager->createInFile( &file, filename ) );

        lr = createModel( model_, file, flags_, key_ );
        
        // �f�B���N�g�����A
        mFileManager->returnCurrentPath();
        mFileManager->unlock();

        LN_SAFE_RELEASE( file );
        return lr;
    }

    //---------------------------------------------------------------------
	// �� ���[�V�����t�@�C����ǂݍ���
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
	// �� ���[�V�����t�@�C����ǂݍ���
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
	// �� ���[�V�����t�@�C����ǂݍ��� (�t�@�C�����w��)
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
	// �� �e�N�X�`���쐬
    //---------------------------------------------------------------------
    LNRESULT ModelManager::createTexture( ITexture** texture_, const char* filename_ )
    {
        LRefTString filename( filename_ );

        mGraphicsDevice->createTexture(
            texture_, filename.c_str(), 0, 1, LN_FMT_A8R8G8B8, LSharingKey( filename ) );

        return LN_OK;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================