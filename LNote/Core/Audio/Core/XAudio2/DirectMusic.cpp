//=============================================================================
//�y DirectMusic �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <algorithm>
#include <cguid.h>
#include "DirectMusic.h"

#define LN_OUT_LOG  // �O�̂��߃��O���o���Ă���

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Audio
{
namespace XAudio2
{

//=============================================================================
// �� DirectMusicSegment �N���X
//=============================================================================

#define LN_DMUS_TYPE DMUS_APATH_DYNAMIC_STEREO  // DMUS_APATH_SHARED_STEREOPLUSREVERB �ɂ���ƃf�t�H�Ń��o�[�u�t�����ǁA�s�b�`�ύX���ł��Ȃ��Ȃ�݂���

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    DirectMusicSegment::DirectMusicSegment( IDirectMusicPerformance8* dm_performance_, const Midi* audio_source_ )
        : mDMPerformance    ( dm_performance_ )
        , mDMAudioPath      ( NULL )
        , mDMSegment        ( NULL )
        , mDMSegmentState   ( NULL )
        , mDSSoundBuffer    ( NULL )
    {
        HRESULT hr = S_OK;
        
        mDMPerformance = dm_performance_;

        if ( mDMPerformance )
        {
            // DirectSound �̃o�b�t�@���擾���邽�߁A�I�[�f�B�I�p�X���擾����
		    hr = mDMPerformance->CreateStandardAudioPath(
			     LN_DMUS_TYPE,
			     64, FALSE, &mDMAudioPath );
            
            // DirectSound �o�b�t�@�̎擾
		    hr = mDMAudioPath->GetObjectInPath( 
                DMUS_PCHANNEL_ALL,
                DMUS_PATH_BUFFER,
                0, GUID_NULL, 0,
                IID_IDirectSoundBuffer8, 
                (void**)&mDSSoundBuffer );


            // �o�b�t�@�ɃG�t�F�N�g��ݒ肷��
		    //		GUID_DSFX_STANDARD_CHORUS
		    //		GUID_DSFX_STANDARD_COMPRESSOR
		    //		GUID_DSFX_STANDARD_DISTORTION
		    //		GUID_DSFX_STANDARD_ECHO �G�R�[���肷��
		    //		GUID_DSFX_STANDARD_FLANGER
		    //		GUID_DSFX_STANDARD_GARGLE �Ԃ�Ԃ�
		    //		GUID_DSFX_STANDARD_I3DL2REVERB ����������
		    //		GUID_DSFX_STANDARD_PARAMEQ
		    //		GUID_DSFX_WAVES_REVERB
		    DSEFFECTDESC ds_desc;
		    ZeroMemory( &ds_desc, sizeof( DSEFFECTDESC ) );
		    ds_desc.dwSize        = sizeof( DSEFFECTDESC );
		    ds_desc.dwFlags       = 0;//DSFX_LOCSOFTWARE;
		    ds_desc.guidDSFXClass = GUID_DSFX_WAVES_REVERB;  // �G�t�F�N�g���ʎq ( �E�F�[�u���o�[�u )
		    ds_desc.dwReserved1   = 0;	// �\��ς�
		    ds_desc.dwReserved2   = 0;	// �\��ς�
		    hr = mDSSoundBuffer->SetFX( 1, &ds_desc, NULL );
		    

            // ���o�[�u�̃p�����[�^��ݒ肷�邽�߂̃C���^�[�t�F�[�X���擾����
		    // ( ���ʂ͕K�v�Ȃ����ǁA�ł��邾�� RGSS �ɋߕt�������̂� )
		    IDirectSoundFXWavesReverb8* waves_reverb = NULL;
		    hr = mDSSoundBuffer->GetObjectInPath( GUID_All_Objects, 0, IID_IDirectSoundFXWavesReverb8, (void**)&waves_reverb );
		    
		    // �p�����[�^��ݒ肵�� �o�b�t�@�ɃZ�b�g
		    DSFXWavesReverb rv;
		    rv.fInGain			= 0.f;//DSFX_WAVESREVERB_INGAIN_MIN ;//DSFX_WAVESREVERB_INGAIN_DEFAULT;
		    rv.fReverbMix		= 0.f;//DSFX_WAVESREVERB_REVERBMIX_MIN  DSFX_WAVESREVERB_REVERBMIX_DEFAULT;
		    rv.fReverbTime		= 600.f;//DSFX_WAVESREVERB_REVERBTIME_DEFAULT;DSFX_WAVESREVERB_REVERBTIME_MIN 
		    rv.fHighFreqRTRatio = DSFX_WAVESREVERB_HIGHFREQRTRATIO_DEFAULT;//DSFX_WAVESREVERB_HIGHFREQRTRATIO_MIN; //DSFX_WAVESREVERB_HIGHFREQRTRATIO_DEFAULT;DSFX_WAVESREVERB_HIGHFREQRTRATIO_MAX
		    hr = waves_reverb->SetAllParameters( &rv );
		    
            


            IDirectMusicLoader8* loader = NULL;

		    //���[�_�[�̍쐬
		    HRESULT hr = CoCreateInstance( CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, IID_IDirectMusicLoader8, (void**)&loader );
    		
		    // �L���b�V������ ( �L���ɂ���ꍇ�Ahttp://www.nomuraz.com/denpa/prog001.htm#IDPROG0009 �̃o�O�ɒ��� )
		    hr = loader->EnableCache( GUID_DirectMusicAllTypes, FALSE );
		    //hr = loader->EnableCache( CLSID_DirectMusicSegment, FALSE );

		    // �Z�O�����g�쐬�̂��߂̏���ݒ肷��
		    DMUS_OBJECTDESC desc;
		    ZeroMemory( &desc, sizeof( DMUS_OBJECTDESC ) );
		    desc.dwSize      = sizeof( DMUS_OBJECTDESC );
		    desc.dwValidData = DMUS_OBJ_MEMORY | DMUS_OBJ_CLASS;
		    desc.guidClass   = CLSID_DirectMusicSegment;
		    desc.llMemLength = (LONGLONG)audio_source_->getSourceDataSize();	// �o�b�t�@�̃T�C�Y
		    desc.pbMemData   = (LPBYTE)audio_source_->getSourceData();	    // �f�[�^�̓����Ă���o�b�t�@

		    // �Z�O�����g�쐬
		    hr = loader->GetObject( &desc, IID_IDirectMusicSegment8, (void**)&mDMSegment );

            // SetParam �� GUID_StandardMIDIFile ��ݒ肷��O�ɌĂ΂Ȃ���
            // �� 1 �g���b�N���Đ�����Ȃ��Ȃ�
            hr = mDMSegment->Download( mDMPerformance );

		    // SMF �Ƃ��čĐ����邱�Ƃ�`����
		    hr = mDMSegment->SetParam( GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL );
    		
		    //loader->ClearCache( CLSID_DirectMusicSegment );

		    

            
		    //LN_DXASSERT_GOTO_S( hr, EXIT, "< DirectMusicSegment::onCreatePerformance >" );

            // ���[�_�[�͂����g��Ȃ��̂ŉ��
		    SAFE_RELEASE( loader );
        }
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    DirectMusicSegment::~DirectMusicSegment()
    {
        stop();
        
        if ( mDMSegment )
		{
            mDMSegment->Unload( mDMPerformance );
		}
		if ( mDMPerformance )
		{
			mDMPerformance->CloseDown();
		}

        SAFE_RELEASE( mDMSegmentState );
        SAFE_RELEASE( mDMSegment );
		
        SAFE_RELEASE( mDSSoundBuffer );
        SAFE_RELEASE( mDMAudioPath );
        SAFE_RELEASE( mDMPerformance );
    }

    

    //---------------------------------------------------------------------
	// �� ���ʂ̐ݒ� ( 100.0 �` 0.0 )
    //---------------------------------------------------------------------
    void DirectMusicSegment::setVolume( float volume_ )
    {
		// 10db �` -100db �����p�͈́B�n���l�� 1000 �` -10000
		// �f�t�H���g�� 1db ( 100 ) �ŁA����𒴂���Ƒ�̉����ꂷ��B(���o�[�u�G�t�F�N�g)
		long volume = 70 * ( 100 - static_cast< long >( volume_ ) ) / 100;
		volume *= volume;
		volume = -volume + 100;
		if ( volume > 100 )
		{
			volume = 100;
		}

		mDMPerformance->SetGlobalParam( GUID_PerfMasterVolume, &volume, sizeof( long ) );
    }

    //---------------------------------------------------------------------
	// �� �s�b�`�̐ݒ�
    //---------------------------------------------------------------------
    void DirectMusicSegment::setPitch( float pitch_ )
    {
		DWORD p = 22050 * static_cast< DWORD >( pitch_ ) / 100;	
		HRESULT hr = mDSSoundBuffer->SetFrequency( p );
    }

    //---------------------------------------------------------------------
	// �� �S�̂̍Đ����Ԃ̎擾 ( �~���[�W�b�N�^�C���P�� )
    //---------------------------------------------------------------------
    u32 DirectMusicSegment::getTotalTime() const
    {
        MUSIC_TIME segment_length = 0;
		mDMSegment->GetLength( &segment_length );
        return segment_length;
    }

    //---------------------------------------------------------------------
	// �� �Đ��ʒu�̎擾 ( �~���[�W�b�N�^�C���P�� )
    //---------------------------------------------------------------------
    u32 DirectMusicSegment::getPlayPosition() const
    {
        MUSIC_TIME time = 0;
        if ( mDMSegmentState )
        {
	        mDMSegmentState->GetSeek( &time );
		    return time;
        }
        return 0;
    }

    //---------------------------------------------------------------------
	// �� �Đ��ʒu�̐ݒ� ( �~���[�W�b�N�^�C���P�� )
    //---------------------------------------------------------------------
    void DirectMusicSegment::setPlayPosition( u32 time_ )
    {
        if ( mDMSegment )
		{
			mDMSegment->SetStartPoint( time_ );
		}
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    bool DirectMusicSegment::isPlaying() const
    {
        return mDMPerformance->IsPlaying( mDMSegment, mDMSegmentState ) == S_OK;
        //DWORD flags = 0;
        //mDSSoundBuffer->GetStatus( &flags );
        //_p(flags & DSBSTATUS_PLAYING);
        //return (flags & DSBSTATUS_PLAYING) != 0;
    }

    //---------------------------------------------------------------------
	// �� ���[�v���̐ݒ�
	//---------------------------------------------------------------------
	void DirectMusicSegment::setLoopState( bool is_loop_, u32 begin_, u32 length_ )
	{
        HRESULT hr;

		// ���[�v�Đ�����ꍇ
		if ( is_loop_ )
		{
			if ( length_ <= 0 )
			{
				// �Z�O�����g�S�̂̒����擾
				MUSIC_TIME time_length;
				mDMSegment->GetLength( &time_length );
				length_ = time_length;

                hr = mDMSegment->SetLoopPoints( begin_, length_ );
			}
            else
            {
			    mDMSegment->SetLoopPoints( begin_, begin_ + length_ );
            }
				
			// �������[�v
			mDMSegment->SetRepeats( DMUS_SEG_REPEAT_INFINITE );
		}
		// ���[�v�Đ����Ȃ��ꍇ
		else
		{
            mDMSegment->SetLoopPoints( 0, 0 );
			mDMSegment->SetRepeats( 0 );
		}
	}

    //---------------------------------------------------------------------
    // �� �Đ�
    //---------------------------------------------------------------------
    void DirectMusicSegment::play()
    {
        stop();

        SAFE_RELEASE( mDMSegmentState );

        mDMAudioPath->Activate( TRUE );


        IDirectMusicSegmentState* state = NULL;
		mDMPerformance->PlaySegmentEx( mDMSegment, NULL, NULL, 0, 0, &state, NULL, mDMAudioPath );
		state->QueryInterface( IID_IDirectMusicSegmentState8, (LPVOID*)&mDMSegmentState );
		state->Release();
      

        // �񓯊��ŏ��������Ă��ꍇ�A�{�����[���Ȃǂ́A�Đ��J�n���O�ɐݒ肷��K�v������
    }

    //---------------------------------------------------------------------
    // �� ��~
    //---------------------------------------------------------------------
    void DirectMusicSegment::stop()
    {
        if ( mDMAudioPath )
		{
			mDMAudioPath->Activate( FALSE );
		}
        if ( mDMPerformance )
		{
			mDMPerformance->Stop( mDMSegment, NULL, 0, 0 );
		}
    }


//=============================================================================
// �� DirectMusicManager �N���X
//=============================================================================

    // PlayerObject �N���X�̃f�X�g���N�^
    DirectMusicManager::PlayerObject::~PlayerObject()
    {
        DirectMusicManager::getInstance()->removePlayRequest( this );
    }


    DirectMusicManager* DirectMusicManager::sInstance = NULL;

    //---------------------------------------------------------------------
    // �� ������
	//---------------------------------------------------------------------
    LNRESULT DirectMusicManager::initialize( const InitData& init_data_ )
    {
        if ( !sInstance && init_data_.DMInitMode != LN_DMUSINGMODE_NOTUSE )
        {
            sInstance = LN_NEW DirectMusicManager();
            return sInstance->_initialize( init_data_ );
        }
        return LN_OK;
    } 

    //---------------------------------------------------------------------
	// �� ���
	//---------------------------------------------------------------------
    void DirectMusicManager::finalize()
    {
        SAFE_DELETE( sInstance );
    }

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    DirectMusicManager::DirectMusicManager()
        : mWindowHandle         ( NULL )
        , mInitMode             ( LN_DMUSINGMODE_THREAD_WAIT )
        , mDirectSound          ( NULL )
        , mDirectMusic          ( NULL )
        , mFirstPerformance     ( NULL )
        , mInitThread           ( NULL )
        , mPerformanceInited    ( NULL )
        , mLock                 ( NULL )
        , mListLock             ( NULL )
        , mErrorState           ( 1 )       // �ŏ��̓G���[�ɂ��Ă����B����ɏ������ł����� 0
        , mLogFile              ( NULL )
    {
    	}

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    DirectMusicManager::~DirectMusicManager()
    {
        LN_LOG_FIN_BEGIN;
        // �X���b�h�̏I����҂��ĉ��
		if ( mInitThread )
		{
			mInitThread->wait();
			SAFE_DELETE( mInitThread );
		}

        LN_SAFE_RELEASE( mListLock );
        LN_SAFE_RELEASE( mLock );
        LN_SAFE_RELEASE( mPerformanceInited );

        SAFE_RELEASE( mFirstPerformance );
        SAFE_RELEASE( mDirectSound );
	    SAFE_RELEASE( mDirectMusic );

        //::CoUninitialize();

        LN_LOG_FIN_END;
    }

    //---------------------------------------------------------------------
	// �� �G���[��Ԃ̎擾
	//---------------------------------------------------------------------
    u32 DirectMusicManager::getErrorState() const
    {
        u32 state;
        mLock->lock();
        state = mErrorState;
        mLock->unlock();
        return state;
    }

    //---------------------------------------------------------------------
	// �� �Đ��v���̒ǉ�
	//---------------------------------------------------------------------
	void DirectMusicManager::addPlayRequest( PlayerObject* obj_ )
    {
        mListLock->lock();

        // �Đ��v�����X�g�ɒǉ�����Ă��Ȃ��ꍇ�͒ǉ����Ă���
        PlayRequestList::iterator pos;
        pos = std::find( mPlayRequestList.begin(), mPlayRequestList.end(), obj_ );
        if ( pos == mPlayRequestList.end() )
        {
            mPlayRequestList.push_back( obj_ );
        }

        mListLock->unlock();
    }

    //---------------------------------------------------------------------
	// �� �Đ��v���̍폜
	//---------------------------------------------------------------------
	void DirectMusicManager::removePlayRequest( PlayerObject* obj_ )
    {
        mListLock->lock();
        mPlayRequestList.remove( obj_ );
        mListLock->unlock();
    }

    //---------------------------------------------------------------------
	// �� IDirectMusicPerformance8 �̍쐬
	//---------------------------------------------------------------------
    IDirectMusicPerformance8* DirectMusicManager::createDMPerformance()
    {
        // �ꉞ�������ς݂����`�F�b�N
        if ( !isInitialized() ) { 
	return NULL; }

        mLock->lock();
        IDirectMusicPerformance8* performance = mFirstPerformance;
        mLock->unlock();

        // ��ԍŏ��ɏ������������̂�����ꍇ�͂����Ԃ�
        if ( performance )
        {
            mLock->lock();
            mFirstPerformance = NULL;
            mLock->unlock();
            return performance;
        }

        // �p�t�H�[�}���X�̍쐬
        HRESULT hr = CoCreateInstance(
		    CLSID_DirectMusicPerformance,
		    NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8,
		    (void**)&performance );
        if ( FAILED( hr ) ) return NULL;

        // �p�t�H�[�}���X�̏�����
		//hr = mPerformance->Init( (IDirectMusic**)&direct_music, (IDirectSound*)direct_sound, LDirectMusic::instance()->windowHandle() );
		hr = performance->InitAudio(
			(IDirectMusic**)&mDirectMusic, (IDirectSound**)&mDirectSound,
			mWindowHandle,
			LN_DMUS_TYPE,	
			64, DMUS_AUDIOF_ALL, NULL );
		if ( FAILED( hr ) ) return NULL;

        return performance;
    }

    //---------------------------------------------------------------------
	// �� �Đ��v�����X�g���`�F�b�N���āA�\�Ȃ�Đ�����
	//---------------------------------------------------------------------
    void DirectMusicManager::polling()
    {
        if ( mPerformanceInited->isTrue() )
        {
            mListLock->lock();

            if ( mPlayRequestList.size() > 0 )
            {
                PlayRequestList::iterator it  = mPlayRequestList.begin();
                PlayRequestList::iterator end = mPlayRequestList.end();
                for ( ; it != end; ++it )
                {
                    (*it)->onFinishDMInit( createDMPerformance() );
                }
                mPlayRequestList.clear();
            }

            mListLock->unlock();
        }
    }

    //---------------------------------------------------------------------
    // �� ������
    //---------------------------------------------------------------------
    LNRESULT DirectMusicManager::_initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        HRESULT hr;

        mWindowHandle = init_data_.WindowHandle;
        mInitMode     = init_data_.DMInitMode;

        // COM ������
        //HRESULT hr = ::CoInitializeEx( NULL, COINIT_MULTITHREADED );
        //LN_SET_DXERR_R_S( hr, "< DirectMusicManager::_initialize >\nCoInitializeEx" );

        // DirectSound �̍쐬
        LN_DXCALL_R(
            ::CoCreateInstance(
				    CLSID_DirectSound8,
				    NULL, 
				    CLSCTX_ALL,
				    IID_IDirectSound8,
				    (void**)&mDirectSound ) );

        // DirectSound �̏�����
        mDirectSound->Initialize( NULL );


	    // �������x���̐ݒ�
	    LN_DXCALL_R(mDirectSound->SetCooperativeLevel( mWindowHandle, DSSCL_PRIORITY ) );

	    // DirectMusic �쐬
	    LN_DXCALL_R(CoCreateInstance( CLSID_DirectMusic, NULL, CLSCTX_INPROC, IID_IDirectMusic, (void**)&mDirectMusic ) );

	    // �쐬���� DirectMusic �� DirectSound ��ݒ�
	    LN_DXCALL_R(mDirectMusic->SetDirectSound( mDirectSound, mWindowHandle ) );
    

        mPerformanceInited = LN_NEW Thread::EventFlag( false );
        mLock = LN_NEW Thread::Mutex();
        mListLock = LN_NEW Thread::Mutex();

        if ( mInitMode == LN_DMUSINGMODE_THREAD_WAIT || mInitMode == LN_DMUSINGMODE_THREAD_REQUEST )
		{
			mInitThread = LN_NEW PerformanceInitThread();
			mInitThread->start();
		}
		else
		{
			_initPerformance();
		}

        LN_LOG_INIT_END;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �ЂƂ߂̃p�t�H�[�}���X�̏�����
    //---------------------------------------------------------------------
	void DirectMusicManager::_initPerformance()
    {
#ifdef LN_OUT_LOG
        LN_LOG_WRITE( "IDirectMusicPerformance8 �̏��������J�n���܂�..." );
#endif

        IDirectMusicPerformance8* performance = NULL;
        u32 error = 0;
        
        // �p�t�H�[�}���X�̍쐬
        HRESULT hr = CoCreateInstance(
		    CLSID_DirectMusicPerformance,
		    NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8,
		    (void**)&performance );
        if ( FAILED( hr ) ) error = 1; 

        if ( SUCCEEDED( hr ) )
        {
            // �p�t�H�[�}���X�̏�����
		    //hr = mPerformance->Init( (IDirectMusic**)&direct_music, (IDirectSound*)direct_sound, LDirectMusic::instance()->windowHandle() );
		    hr = performance->InitAudio(
			    (IDirectMusic**)&mDirectMusic, (IDirectSound**)&mDirectSound,
			    mWindowHandle,
			    LN_DMUS_TYPE,
			    64, DMUS_AUDIOF_ALL, NULL );
		    if ( FAILED( hr ) ) error = 2;
        }

#ifdef LN_OUT_LOG
        LN_LOG_WRITE( "IDirectMusicPerformance8 �̏�������̏������ׁ݂̈A���b�N���܂��B" );
#endif

        mLock->lock();
        mFirstPerformance = performance;
        mErrorState = error;
        mLock->unlock();

        // ����������
        mPerformanceInited->setTrue();

#ifdef LN_OUT_LOG
        LN_LOG_WRITE( "IDirectMusicPerformance8 �̏��������������܂����B" );
#endif
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace XAudio2
} // namespace Audio
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================
