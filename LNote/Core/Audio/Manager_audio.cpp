//=============================================================================
//【 Manager 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../File/Manager.h"
#include "../File/InFile.h"
#if defined(LNOTE_DIRECTX)
    #include "Core/XAudio2/XA2AudioDevice.h"
    #include "Core/XAudio2/DirectMusic.h"
#elif defined(LNOTE_ANDROID)
    #include "Core/OpenSLES/SLESAudioDevice.h"
#endif
#include "Core/AudioUtil.h"
#include "Interface.h"
#include "Sound.h"
#include "Manager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Audio
{

//=============================================================================
// ■ Manager クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	Manager::Manager()
        : mLogFile              ( NULL )
        , mAudioDevice          ( NULL )
        , mResourceManager      ( NULL )
        , mGameAudio            ( NULL )
        , mFileIOManager        ( NULL )
        , mThread               ( NULL )
        , mEndRequest           ( NULL )
        , mLock                 ( NULL )
        , mOnMemoryLimitSize    ( 100000 )
	{
		}

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	Manager::~Manager()
	{
        LN_LOG_FIN_BEGIN;

        

        finalize();

        LN_SAFE_RELEASE( mEndRequest );
        LN_SAFE_RELEASE( mLock );

        

        // mSoundList に何か残っていれば全て解放
        //_p(mSoundList.size());
        mSoundList.clear();
        /*
        ISound* sound = NULL;
        SoundList::iterator it  = mSoundList.begin();
        SoundList::iterator end = mSoundList.end();
        for ( ; it != end; )
        {
            sound = ( *it );
			it = mSoundList.erase( it );
            if ( ( sound->getRefCount() == 0 ) &&
				 ( sound->getPlayerFlags() & LN_AUDIOPLAYER_AFTERRELEASE ) )
			{
                SAFE_DELETE( sound );
            }
            else
            {
                LN_SAFE_RELEASE( sound );
            }
        }
        */

		
        LN_SAFE_RELEASE( mResourceManager );
        LN_SAFE_RELEASE( mAudioDevice );

        LN_SAFE_RELEASE( mFileIOManager );

        LN_LOG_FIN_END;
	}

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    LNRESULT Manager::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        LNRESULT lr;

        mFileIOManager = init_data_.FileIOManager;
        LN_SAFE_ADDREF( mFileIOManager );

        // AudioDevice
#if defined(LNOTE_DIRECTX)
        XAudio2::AudioDevice::Initdata data;
        data.LogFile            = mLogFile;
        data.EnableDebugFlag    = false;
        data.DMInitMode         = init_data_.DMInitMode;
        data.Window             = init_data_.Window;
        XAudio2::AudioDevice* device = LN_NEW XAudio2::AudioDevice();
        LN_CALL_R( device->initialize( data ) );
        mAudioDevice = device;
#elif defined(LNOTE_ANDROID)
        SLES::AudioDevice::Initdata data;
        data.LogFile            = mLogFile;
        SLES::AudioDevice* device = LN_NEW SLES::AudioDevice();
        LN_CALL_R( device->initialize( data ) );
        mAudioDevice = device;
#endif


        // ResourceManager
        ResourceManager::InitData rm_data;
        rm_data.LogFile                 = mLogFile;
        rm_data.AudioSourceCacheSize    = init_data_.AudioSourceCacheSize;
        mResourceManager = LN_NEW ResourceManager();
        LN_CALL_R( mResourceManager->initialize( rm_data ) );


        // GameAudio
        mGameAudio = LN_NEW GameAudio( this );
        

        // スレッド関係
        mEndRequest = LN_NEW Thread::EventFlag( false );
        mLock = LN_NEW Thread::Mutex();
		mThread = LN_NEW PollingThread( this );


		// スレッド開始
		mThread->start();


        LN_LOG_INIT_END;
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 終了処理 ( GameAudio の解放 )
    //---------------------------------------------------------------------
    void Manager::finalize()
    {
        if ( mEndRequest )
        {
		    // 終了要求フラグを ON
		    mEndRequest->setTrue();

		    // スレッド終了
		    SAFE_DELETE( mThread );
        }

        if ( mGameAudio )
        {
            // GameAudio のデストラクタでは ISound の stop() を呼んでいる。
            // そのため、単純にミューテックスで囲んで delete するとデッドロックする。

            GameAudio* a = mGameAudio;

            mLock->lock();
            mGameAudio = NULL;
            mLock->unlock();
            
            SAFE_DELETE( a );
        }

        if ( mResourceManager )
        {
            mResourceManager->finalize();
        }
    }
    
    //---------------------------------------------------------------------
	// ● ISound の作成
    //---------------------------------------------------------------------
    LNRESULT Manager::createSound( ISound** obj_, File::IInStream* stream_, LNSoundPlayType type_, bool enable_3d_, LSharingKey key_ )
    {
        LNRESULT lr = LN_OK;
        *obj_ = NULL;
        LNSoundPlayType player_type;// = LN_SOUNDPLAYTYPE_UNKNOWN;

        mLock->lock();

        //-----------------------------------------------------
        // IAudioSource

        IAudioSource* source;
        mResourceManager->findAudioSource( &source, key_ );
        if ( !source )
        {
            if ( stream_ )
            {
                LN_CALL_GOTO( ERR_EXIT, mResourceManager->createAudioSource( &source, stream_, key_ ) );

                // 正しい種類をチェック
                player_type = AudioUtil::checkAudioPlayType( type_, source, mOnMemoryLimitSize );

                // ストリーミング以外はキャッシュ登録
                //if ( player_type != LN_SOUNDPLAYTYPE_STREAMING )
                //{
                //    mResourceManager->registerCachedObject( key_, source );
                //}
            }
            else
            {
                lr = LN_FALSE;
                goto ERR_EXIT;
            }
        }
        else
        {
            if ( source->getSourceFormat() == LN_AUDIOSOURCE_MIDI )
            {
                player_type = LN_SOUNDPLAYTYPE_MIDI;
            }
            else
            {
                player_type = LN_SOUNDPLAYTYPE_ONMEMORY;
            }
        }

        

        

        

        //-----------------------------------------------------
        // IAudioPlayer

        

        IAudioPlayer* player;
        lr = mAudioDevice->createAudioPlayer( &player, source, enable_3d_, player_type );
        if ( LN_FAILED( lr ) )
        {
            LN_SAFE_RELEASE( source );
            goto ERR_EXIT;
        }

        // g++ goto 対応の { }
        {
            Sound* sound = LN_NEW Sound( this, player );
            mSoundList.push_back( sound );

            *obj_ = sound;
        }

ERR_EXIT:

        LN_SAFE_RELEASE( source );
        mLock->unlock();
        return lr;
    }

    //---------------------------------------------------------------------
	// ● ISound の作成 ( ファイル名指定 )
    //---------------------------------------------------------------------
    LNRESULT Manager::createSound( ISound** obj_, const char* filename_, LNSoundPlayType type_, bool enable_3d_ )
    {
        LNRESULT lr;

        *obj_ = NULL;

        ISound* sound;

        // 既に作成されているものを検索
        createSound( &sound, NULL, type_, enable_3d_, LSharingKey( filename_ ) );


        // 無かった場合は新しく作る
        if ( !sound )
        {
            File::IInFile* stream;
            LN_CALL_R( mFileIOManager->createInFile( &stream, filename_ ) );

            lr = createSound( &sound, stream, type_, enable_3d_, LSharingKey( filename_ ) );

            

            if ( LN_FAILED( lr ) || !stream )
            {
                LN_SAFE_RELEASE( stream );
                //LN_SETERR_R_S( LN_ERR_FILENOTFOUND, "< Manager::createSound >\n\n音声ファイルが見つかりませんでした。\nfile : \"%s\"", filename_ );
            }

            LN_SAFE_RELEASE( stream );
        }
        else
        {
            //printf( "共有:%s\n", filename_ ); 
        }
   
        *obj_ = sound;

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● ISound の作成 ( IAudioSource 指定 )
    //---------------------------------------------------------------------
    LNRESULT Manager::createSound( ISound** sound_, IAudioSource* source_, LNSoundPlayType type_, bool enable_3d_ )
    {
        LNRESULT lr = LN_OK;
        *sound_ = NULL;

        mLock->lock();

        // IAudioPlayer
        IAudioPlayer* player;
        LN_CALL_GOTO( ERR_EXIT, mAudioDevice->createAudioPlayer( &player, source_, enable_3d_, type_ ) );

        // g++ goto 対応の { }
        {
            // 今のところ player は Sound 側で addref してないのでこのままでOK
            Sound* sound = LN_NEW Sound( this, player );
            mSoundList.push_back( sound );

            *sound_ = sound;
        }

ERR_EXIT:

        mLock->unlock();
        return lr;
    }

 //   //---------------------------------------------------------------------
	//// ● ISound の削除 ( Sound の onDelete() から呼ばれる )
 //   //---------------------------------------------------------------------
 //   LNRESULT Manager::deleteSound( ISound* sound_ )
 //   {
 // //      // 再生終了まで解放を待つ場合
 // //      if ( sound_->getPlayerFlags() & LN_AUDIOPLAYER_AFTERRELEASE )
	//	//{
 // //          
 // //      }
 // //      // 待たない場合はすぐ解放
 // //      else
 //       {
 //           mLock->lock();
 //           mSoundList.remove( sound_ );
 //           
 //           SAFE_DELETE( sound_ );

 //           mLock->unlock();
 //       }
 //       return LN_OK;
 //   }

    //---------------------------------------------------------------------
	// ● Sound のデストラクタから呼ばれる
    //---------------------------------------------------------------------
    void Manager::removeSound( Sound* sound_ )
    {
        mLock->lock();
        mSoundList.remove( sound_ );
        mLock->unlock();
    }

    //---------------------------------------------------------------------
	// ● 指定したグループに属するオーディオプレイヤーの停止
	//---------------------------------------------------------------------
	void Manager::stopGroup( u32 group_ )
	{
		mLock->lock();

        SoundList::iterator it  = mSoundList.begin();
        SoundList::iterator end = mSoundList.end();
        for ( ; it != end; ++it )
        {
            if ( (*it)->getGroupFlag() & group_ )
            {
                (*it)->stop();
            }
        }

		mLock->unlock();
	}

    //---------------------------------------------------------------------
	// ● フレーム更新
    //---------------------------------------------------------------------
    void Manager::update( float elapsed_time_ )
    {
        mLock->lock();

        mAudioDevice->update();

        Sound* sound = NULL;
        bool incr = false;
        SoundList::iterator it  = mSoundList.begin();
        SoundList::iterator end = mSoundList.end();
        for ( ; it != end; )
        {
            sound = ( *it );
            incr = true;
  
            // 更新して、再生が終了した場合 ( ループ再生の場合は常に true )
			if ( !sound->update( elapsed_time_ ) )
			{
                /*
                // リストにだけ残っていて、再生終了後に解放する場合
				if ( ( sound->getRefCount() == 0 ) &&
					 ( sound->getPlayerFlags() & LN_AUDIOPLAYER_AFTERRELEASE ) )
				{
                    //printf( "停止・解放\n" );
					it = mSoundList.erase( it );
                    //LN_SAFE_RELEASE( sound );
					SAFE_DELETE( sound );

                    printf( "旧 再生終了検出\n" );

                    incr = false;
				}
                */
            }

            if ( incr )
            {
                ++it;
            }
        }

#if defined(LNOTE_DIRECTX)
        if ( XAudio2::DirectMusicManager::getInstance() )
        {
            XAudio2::DirectMusicManager::getInstance()->polling();
        }
#endif

        mLock->unlock();

        // finalize() が呼ばれた後もスレッド側から呼ばれる可能性があるので NULL チェック
        if ( mGameAudio )
        {
            mGameAudio->polling();
        }
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Audio
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================