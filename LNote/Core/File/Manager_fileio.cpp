//=============================================================================
//【 Manager 】
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
// ■ Manager クラス
//=============================================================================


    //---------------------------------------------------------------------
	// ● コンストラクタ
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
	// ● デストラクタ
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
	// ● 初期化
    //---------------------------------------------------------------------
    void Manager::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        //-----------------------------------------------------
        // アーカイブ数チェック

        int archive_num = init_data_.ArchiveList.size();
		LNFileAccessMode mode = init_data_.AccessMode;

		// アーカイブ or ディレクトリ優先
		if ( mode == LN_FILEACCESS_ARCHIVE_FIRST || mode == LN_FILEACCESS_DIRECTORY_FIRST )
		{
			mArchiveNum = archive_num + 1;
		}
		// アーカイブのみ
		else
		{
			mArchiveNum = archive_num;
		}

        LN_THROW_InvalidCall( mArchiveNum > 0 );

        //-----------------------------------------------------
        // アーカイブ作成
		
        mArchives = LN_NEW Archive[ mArchiveNum ];

		// ディレクトリ優先の場合
		if ( mode == LN_FILEACCESS_DIRECTORY_FIRST )
		{
			// [ 0 ] はダミーで作る
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
        // アーカイブのみ or アーカイブ優先の場合
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
			// アーカイブ優先の場合、最後にダミーを作る
			if ( mode == LN_FILEACCESS_ARCHIVE_FIRST )
			{
				mArchives[ i ].initialize( NULL, NULL, "" );
			}
		}

        //mDefaultStreamCreator = LN_NEW DefaultStreamCreator( this );

        mArchiveList.push_back( LN_NEW Resource::HardCodeResourceArchive() );
        

        //-----------------------------------------------------
        // ローディングスレッド関係

        mLock = LN_NEW Thread::Mutex();

        mLoadThreadLock = LN_NEW Thread::Mutex();
		mEndFlag	    = LN_NEW Thread::EventFlag( false );
		mLoadThread     = LN_NEW LoadThread( this );
		mLoadThread->start();

        LN_LOG_INIT_END;
    }

    //---------------------------------------------------------------------
	// ● 終了処理
    //---------------------------------------------------------------------
    void Manager::finalize()
    {
        LN_LOG_FIN_BEGIN;

        if ( mEndFlag )
        {
            mEndFlag->setTrue();		    // 終了フラグを立てた後、
        }
        SAFE_DELETE( mLoadThread );         // スレッドの終了を待つ

        LN_LOG_FIN_END;
    }

    //---------------------------------------------------------------------
	// ● 読み込み用カレントパスの設定
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

            // / を \\ に変換して ".\\" を取り除く
            File::normalizePathName( &tmp, directory_ );

            tmp = mCurrentPath + tmp;

            //_p( tmp.c_str() );

            // 検索で使えるファイル名に変換
            File::makeFileName( &mCurrentPath, tmp.c_str() );

            // 終端に '\\' を付加
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
	// ● InFile の作成
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
	// ● OutFile の作成
    //---------------------------------------------------------------------
    LNRESULT Manager::createOutFile( IOutFile** file_, const lnChar* filename_, const void* buffer_, int size_ )
    {
        *file_ = LN_NEW OutFile( this, filename_, buffer_, size_ );
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 読み込み用カレントパスの移動
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
	// ● 読み込み用カレントパスの復帰
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
	// ● ファイルアクセスのロック
    //---------------------------------------------------------------------
    void Manager::lock()
    {
        mLock->lock();
    }

    //---------------------------------------------------------------------
	// ● ファイルアクセスのアンロック
    //---------------------------------------------------------------------
    void Manager::unlock()
    {
        mLock->unlock();
    }

    //---------------------------------------------------------------------
	// ● 非同期処理のリクエスト
    //---------------------------------------------------------------------
    void Manager::requestASyncProcess( ASyncProcessObject* obj_ )
    {
        // 終了要求が来ている場合は追加しない
        if ( isEndRequested() )
        {
            return;
        }

        // 実行待ち
        obj_->_m.State = LN_ASYNCSTATE_READY;

		mLoadThreadLock->lock();
		mASyncProcessRequests.push_back( obj_ );
		mLoadThreadLock->unlock();
    }

    //---------------------------------------------------------------------
	// ● キューにある非同期処理のリクエストを外す
    //---------------------------------------------------------------------
    void Manager::removeASyncProcess( ASyncProcessObject* obj_ )
    {
        mLoadThreadLock->lock();
        mASyncProcessRequests.remove( obj_ );
		mLoadThreadLock->unlock();
    }

    //---------------------------------------------------------------------
	// ● ポーリング (LoadThread クラスから呼ばれる)
    //---------------------------------------------------------------------
	void Manager::polling()
    {
        mLoadThreadLock->lock();
		size_t in_size = mASyncProcessRequests.size();
		mLoadThreadLock->unlock();

		if ( in_size == 0 )
		{
			// 何もない場合はちょっと待つ
			// ( こうしておかないと無限ループなスレッドになるので負荷がすごく出てくる )
            Thread::sleep( 10 );
			return;
		}


        // 要求取り出し
		mLoadThreadLock->lock();

		if ( mASyncProcessRequests.size() == 0 )
		{
			mLoadThreadLock->unlock();
			return;
		}
		ASyncProcessObject* obj = mASyncProcessRequests.front();
		mASyncProcessRequests.pop_front();

		mLoadThreadLock->unlock();

        // 処理中
        obj->_m.State = LN_ASYNCSTATE_PROCESSING;
       
        // 実行
		if ( !( *( obj ) )() )
        {
            obj->_m.Failed->setTrue();  // エラー
        }

        // 完了フラグを ON
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