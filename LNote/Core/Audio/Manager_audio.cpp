//=============================================================================
//�y Manager �z
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
// �� Manager �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
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
	// �� �f�X�g���N�^
	//---------------------------------------------------------------------
	Manager::~Manager()
	{
        LN_LOG_FIN_BEGIN;

        

        finalize();

        LN_SAFE_RELEASE( mEndRequest );
        LN_SAFE_RELEASE( mLock );

        

        // mSoundList �ɉ����c���Ă���ΑS�ĉ��
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
	// �� ������
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
        

        // �X���b�h�֌W
        mEndRequest = LN_NEW Thread::EventFlag( false );
        mLock = LN_NEW Thread::Mutex();
		mThread = LN_NEW PollingThread( this );


		// �X���b�h�J�n
		mThread->start();


        LN_LOG_INIT_END;
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� �I������ ( GameAudio �̉�� )
    //---------------------------------------------------------------------
    void Manager::finalize()
    {
        if ( mEndRequest )
        {
		    // �I���v���t���O�� ON
		    mEndRequest->setTrue();

		    // �X���b�h�I��
		    SAFE_DELETE( mThread );
        }

        if ( mGameAudio )
        {
            // GameAudio �̃f�X�g���N�^�ł� ISound �� stop() ���Ă�ł���B
            // ���̂��߁A�P���Ƀ~���[�e�b�N�X�ň͂�� delete ����ƃf�b�h���b�N����B

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
	// �� ISound �̍쐬
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

                // ��������ނ��`�F�b�N
                player_type = AudioUtil::checkAudioPlayType( type_, source, mOnMemoryLimitSize );

                // �X�g���[�~���O�ȊO�̓L���b�V���o�^
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

        // g++ goto �Ή��� { }
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
	// �� ISound �̍쐬 ( �t�@�C�����w�� )
    //---------------------------------------------------------------------
    LNRESULT Manager::createSound( ISound** obj_, const char* filename_, LNSoundPlayType type_, bool enable_3d_ )
    {
        LNRESULT lr;

        *obj_ = NULL;

        ISound* sound;

        // ���ɍ쐬����Ă�����̂�����
        createSound( &sound, NULL, type_, enable_3d_, LSharingKey( filename_ ) );


        // ���������ꍇ�͐V�������
        if ( !sound )
        {
            File::IInFile* stream;
            LN_CALL_R( mFileIOManager->createInFile( &stream, filename_ ) );

            lr = createSound( &sound, stream, type_, enable_3d_, LSharingKey( filename_ ) );

            

            if ( LN_FAILED( lr ) || !stream )
            {
                LN_SAFE_RELEASE( stream );
                //LN_SETERR_R_S( LN_ERR_FILENOTFOUND, "< Manager::createSound >\n\n�����t�@�C����������܂���ł����B\nfile : \"%s\"", filename_ );
            }

            LN_SAFE_RELEASE( stream );
        }
        else
        {
            //printf( "���L:%s\n", filename_ ); 
        }
   
        *obj_ = sound;

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� ISound �̍쐬 ( IAudioSource �w�� )
    //---------------------------------------------------------------------
    LNRESULT Manager::createSound( ISound** sound_, IAudioSource* source_, LNSoundPlayType type_, bool enable_3d_ )
    {
        LNRESULT lr = LN_OK;
        *sound_ = NULL;

        mLock->lock();

        // IAudioPlayer
        IAudioPlayer* player;
        LN_CALL_GOTO( ERR_EXIT, mAudioDevice->createAudioPlayer( &player, source_, enable_3d_, type_ ) );

        // g++ goto �Ή��� { }
        {
            // ���̂Ƃ��� player �� Sound ���� addref ���ĂȂ��̂ł��̂܂܂�OK
            Sound* sound = LN_NEW Sound( this, player );
            mSoundList.push_back( sound );

            *sound_ = sound;
        }

ERR_EXIT:

        mLock->unlock();
        return lr;
    }

 //   //---------------------------------------------------------------------
	//// �� ISound �̍폜 ( Sound �� onDelete() ����Ă΂�� )
 //   //---------------------------------------------------------------------
 //   LNRESULT Manager::deleteSound( ISound* sound_ )
 //   {
 // //      // �Đ��I���܂ŉ����҂ꍇ
 // //      if ( sound_->getPlayerFlags() & LN_AUDIOPLAYER_AFTERRELEASE )
	//	//{
 // //          
 // //      }
 // //      // �҂��Ȃ��ꍇ�͂������
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
	// �� Sound �̃f�X�g���N�^����Ă΂��
    //---------------------------------------------------------------------
    void Manager::removeSound( Sound* sound_ )
    {
        mLock->lock();
        mSoundList.remove( sound_ );
        mLock->unlock();
    }

    //---------------------------------------------------------------------
	// �� �w�肵���O���[�v�ɑ�����I�[�f�B�I�v���C���[�̒�~
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
	// �� �t���[���X�V
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
  
            // �X�V���āA�Đ����I�������ꍇ ( ���[�v�Đ��̏ꍇ�͏�� true )
			if ( !sound->update( elapsed_time_ ) )
			{
                /*
                // ���X�g�ɂ����c���Ă��āA�Đ��I����ɉ������ꍇ
				if ( ( sound->getRefCount() == 0 ) &&
					 ( sound->getPlayerFlags() & LN_AUDIOPLAYER_AFTERRELEASE ) )
				{
                    //printf( "��~�E���\n" );
					it = mSoundList.erase( it );
                    //LN_SAFE_RELEASE( sound );
					SAFE_DELETE( sound );

                    printf( "�� �Đ��I�����o\n" );

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

        // finalize() ���Ă΂ꂽ����X���b�h������Ă΂��\��������̂� NULL �`�F�b�N
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