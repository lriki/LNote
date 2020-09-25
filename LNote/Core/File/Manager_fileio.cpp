//=============================================================================
//�y Manager �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Resource/LNResource.h"
#include "FileUtil.h"
#include "Archive.h"
#include "ASyncProcessObject.h"
#include "InFile.h"
#include "OutFile.h"
#include "Manager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File
{

//=============================================================================
// �� Manager �N���X
//=============================================================================


    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    Manager::Manager()
        : mLogFile          ( NULL )
        , mArchives         ( NULL )
        , mArchiveNum       ( 0 )
        , mLoadThreadLock   ( NULL )
        , mEndFlag          ( NULL )
        , mLoadThread       ( NULL )
        , mLock             ( NULL )
    {
    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    Manager::~Manager()
    {
        
		LN_SAFE_RELEASE( mEndFlag );
		LN_SAFE_RELEASE( mLoadThreadLock );

        lnU32 n = mArchiveList.size();
        for ( lnU32 i = 0; i < n; ++i )
        {
            SAFE_DELETE( mArchiveList[i] );
        }
        mArchiveList.clear();

        SAFE_DELETE_ARRAY( mArchives );
        LN_SAFE_RELEASE( mLock );
    }

    //---------------------------------------------------------------------
	// �� ������
    //---------------------------------------------------------------------
    void Manager::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        //-----------------------------------------------------
        // �A�[�J�C�u���`�F�b�N

        int archive_num = init_data_.ArchiveList.size();
		LNFileAccessMode mode = init_data_.AccessMode;

		// �A�[�J�C�u or �f�B���N�g���D��
		if ( mode == LN_FILEACCESS_ARCHIVE_FIRST || mode == LN_FILEACCESS_DIRECTORY_FIRST )
		{
			mArchiveNum = archive_num + 1;
		}
		// �A�[�J�C�u�̂�
		else
		{
			mArchiveNum = archive_num;
		}

        LN_THROW_InvalidCall( mArchiveNum > 0 );

        //-----------------------------------------------------
        // �A�[�J�C�u�쐬
		
        mArchives = LN_NEW Archive[ mArchiveNum ];

		// �f�B���N�g���D��̏ꍇ
		if ( mode == LN_FILEACCESS_DIRECTORY_FIRST )
		{
			// [ 0 ] �̓_�~�[�ō��
			mArchives[ 0 ].initialize( NULL, NULL, "" );

			int i = 1;
            LNArchiveOpenDataList::const_iterator it = init_data_.ArchiveList.begin();
			for ( ; it != init_data_.ArchiveList.end(); )
			{
				u32 err = mArchives[ i ].initialize(
                    it->Directory.c_str(), it->ArchiveName.c_str(), it->ArchiveKey.c_str() );
                switch ( err )
                {
                    case Archive::ERR_NOT_OPEN: 
                    	LN_THROW_InvalidCall( 0, Resource::String::ERR_FileArchive1 ); break;
                    case Archive::ERR_FORMAT:   
                    	LN_THROW_InvalidCall( 0, Resource::String::ERR_FileArchive2 ); break;
                    case Archive::ERR_KEY:      
                    	LN_THROW_InvalidCall( 0, Resource::String::ERR_FileArchive3 ); break;
                }
				++it;
				++i;
			}
		}
        // �A�[�J�C�u�̂� or �A�[�J�C�u�D��̏ꍇ
		else
		{
			int i = 0;
			LNArchiveOpenDataList::const_iterator it = init_data_.ArchiveList.begin();
			for ( ; it != init_data_.ArchiveList.end(); )
			{
                u32 err = mArchives[ i ].initialize(
                    it->Directory.c_str(), it->ArchiveName.c_str(), it->ArchiveKey.c_str() );
				switch ( err )
                {
                    case Archive::ERR_NOT_OPEN: 
                    	LN_THROW_InvalidCall( 0, Resource::String::ERR_FileArchive1 ); break;
                    case Archive::ERR_FORMAT:   
                    	LN_THROW_InvalidCall( 0, Resource::String::ERR_FileArchive2 ); break;
                    case Archive::ERR_KEY:      
                    	LN_THROW_InvalidCall( 0, Resource::String::ERR_FileArchive3 ); break;
                }
				++it;
				++i;
			}
			// �A�[�J�C�u�D��̏ꍇ�A�Ō�Ƀ_�~�[�����
			if ( mode == LN_FILEACCESS_ARCHIVE_FIRST )
			{
				mArchives[ i ].initialize( NULL, NULL, "" );
			}
		}

        //mDefaultStreamCreator = LN_NEW DefaultStreamCreator( this );

        mArchiveList.push_back( LN_NEW Resource::HardCodeResourceArchive() );
        

        //-----------------------------------------------------
        // ���[�f�B���O�X���b�h�֌W

        mLock = LN_NEW Thread::Mutex();

        mLoadThreadLock = LN_NEW Thread::Mutex();
		mEndFlag	    = LN_NEW Thread::EventFlag( false );
		mLoadThread     = LN_NEW LoadThread( this );
		mLoadThread->start();

        LN_LOG_INIT_END;
    }

    //---------------------------------------------------------------------
	// �� �I������
    //---------------------------------------------------------------------
    void Manager::finalize()
    {
        LN_LOG_FIN_BEGIN;

        if ( mEndFlag )
        {
            mEndFlag->setTrue();		    // �I���t���O�𗧂Ă���A
        }
        SAFE_DELETE( mLoadThread );         // �X���b�h�̏I����҂�

        LN_LOG_FIN_END;
    }

    //---------------------------------------------------------------------
	// �� �ǂݍ��ݗp�J�����g�p�X�̐ݒ�
    //---------------------------------------------------------------------
    void Manager::setCurrentPath( const lnChar* directory_ )
    {
        
        //mLock->lock();

        if ( directory_ == NULL )
        {
            mCurrentPath = "";
        }
        else if ( _tcslen( directory_ ) == 0 )
        {
        }
        else
        {
            ln_std_tstring tmp;

            // / �� \\ �ɕϊ����� ".\\" ����菜��
            File::normalizePathName( &tmp, directory_ );

            tmp = mCurrentPath + tmp;

            //_p( tmp.c_str() );

            // �����Ŏg����t�@�C�����ɕϊ�
            File::makeFileName( &mCurrentPath, tmp.c_str() );

            // �I�[�� '\\' ��t��
            if ( mCurrentPath.size() > 0 )
            {
                if ( mCurrentPath[ mCurrentPath.size() - 1 ] != _T( '\\' ) )
                {
                    mCurrentPath += _T( "\\" );
                }
            }
        }

        //_p( mCurrentPath.c_str() );

        //mLock->unlock();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    LRefTString Manager::getAbsolutePath( const lnChar* filename_ )
    {
        if ( filename_[ 0 ] == '$' ) return LRefTString( filename_ );

        LRefTString tmp = (mCurrentPath + filename_).c_str();
        File::normalizePathName( &tmp );

        ln_std_tstring path;
        File::makeFileName( &path, tmp.c_str() );

        LRefTString t( _T( "$" ) );
        t += path.c_str();
        
        return t;
    }

    //---------------------------------------------------------------------
	// �� InFile �̍쐬
    //---------------------------------------------------------------------
    LNRESULT Manager::createInFile( IInFile** file_, const lnChar* filename_, LNFileOpenMode mode_ )
    {
        LNRESULT lr = LN_OK;
        *file_ = NULL;

        //mLock->lock();

        ln_std_tstring path;

		if ( File::isFullPath( filename_ ) )
		{
			path = filename_;
		}
        else if ( filename_[0] == _T( '$' ) )
        {
            path = filename_ + 1;
        }
        else
        {
            LRefTString tmp = (mCurrentPath + filename_).c_str();
            File::normalizePathName( &tmp );
            File::makeFileName( &path, tmp.c_str() );
        }

        IInStream* stream = NULL;
        for ( int i = 0; i < mArchiveNum; ++i )
		{
            mArchives[ i ].createStream( &stream, path.c_str(), mode_ );
			if ( stream )
			{
				break;
			}
		}


        if ( !stream )
        {
            ln_foreach( IArchive* ac, mArchiveList )
            {
                stream = ac->createStream( path.c_str() );
                if ( stream ) break;
            }
        }

        
        LN_THROW_FileNotFound( stream, path.c_str() );
        //LN_SETERR_S( LN_ERR_FILENOTFOUND, "< File::Manager::createInFile >\n\n%s", path.c_str() );
        //lr = LN_ERR_FILENOTFOUND;
        //goto ERR_EXIT;
        

        *file_ = LN_NEW InFile( this, stream );

    ERR_EXIT:
        //mLock->unlock();
        return lr;
    }

    //---------------------------------------------------------------------
	// �� OutFile �̍쐬
    //---------------------------------------------------------------------
    LNRESULT Manager::createOutFile( IOutFile** file_, const lnChar* filename_, const void* buffer_, int size_ )
    {
        *file_ = LN_NEW OutFile( this, filename_, buffer_, size_ );
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� �ǂݍ��ݗp�J�����g�p�X�̈ړ�
    //---------------------------------------------------------------------
    void Manager::moveCurrentPath( const lnChar* directory_ )
    {
        //mLock->lock();

        mPathStack.push( mCurrentPath );

        setCurrentPath( directory_ );

        //printf( "move:%s\n", mCurrentPath.c_str() );

        //mLock->unlock();
    }

    //---------------------------------------------------------------------
	// �� �ǂݍ��ݗp�J�����g�p�X�̕��A
    //---------------------------------------------------------------------
    void Manager::returnCurrentPath()
    {
        //mLock->lock();

        mCurrentPath = mPathStack.top();
        mPathStack.pop();

        //printf( "ret:%s\n", mCurrentPath.c_str() );

        //mLock->unlock();
    }

    //---------------------------------------------------------------------
	// �� �t�@�C���A�N�Z�X�̃��b�N
    //---------------------------------------------------------------------
    void Manager::lock()
    {
        mLock->lock();
    }

    //---------------------------------------------------------------------
	// �� �t�@�C���A�N�Z�X�̃A�����b�N
    //---------------------------------------------------------------------
    void Manager::unlock()
    {
        mLock->unlock();
    }

    //---------------------------------------------------------------------
	// �� �񓯊������̃��N�G�X�g
    //---------------------------------------------------------------------
    void Manager::requestASyncProcess( ASyncProcessObject* obj_ )
    {
        // �I���v�������Ă���ꍇ�͒ǉ����Ȃ�
        if ( isEndRequested() )
        {
            return;
        }

        // ���s�҂�
        obj_->_m.State = LN_ASYNCSTATE_READY;

		mLoadThreadLock->lock();
		mASyncProcessRequests.push_back( obj_ );
		mLoadThreadLock->unlock();
    }

    //---------------------------------------------------------------------
	// �� �L���[�ɂ���񓯊������̃��N�G�X�g���O��
    //---------------------------------------------------------------------
    void Manager::removeASyncProcess( ASyncProcessObject* obj_ )
    {
        mLoadThreadLock->lock();
        mASyncProcessRequests.remove( obj_ );
		mLoadThreadLock->unlock();
    }

    //---------------------------------------------------------------------
	// �� �|�[�����O (LoadThread �N���X����Ă΂��)
    //---------------------------------------------------------------------
	void Manager::polling()
    {
        mLoadThreadLock->lock();
		size_t in_size = mASyncProcessRequests.size();
		mLoadThreadLock->unlock();

		if ( in_size == 0 )
		{
			// �����Ȃ��ꍇ�͂�����Ƒ҂�
			// ( �������Ă����Ȃ��Ɩ������[�v�ȃX���b�h�ɂȂ�̂ŕ��ׂ��������o�Ă��� )
            Thread::sleep( 10 );
			return;
		}


        // �v�����o��
		mLoadThreadLock->lock();

		if ( mASyncProcessRequests.size() == 0 )
		{
			mLoadThreadLock->unlock();
			return;
		}
		ASyncProcessObject* obj = mASyncProcessRequests.front();
		mASyncProcessRequests.pop_front();

		mLoadThreadLock->unlock();

        // ������
        obj->_m.State = LN_ASYNCSTATE_PROCESSING;
       
        // ���s
		if ( !( *( obj ) )() )
        {
            obj->_m.Failed->setTrue();  // �G���[
        }

        // �����t���O�� ON
		obj->_m.Finished->setTrue();
        obj->_m.State = LN_ASYNCSTATE_FINISH;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace File
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================