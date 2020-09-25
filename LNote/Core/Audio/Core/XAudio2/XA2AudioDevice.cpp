//=============================================================================
//�y XA2AudioDevice �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../../Math/Math.h"
#include "../AudioUtil.h"
#include "XA2AudioPlayer.h"
#include "XA2AudioDevice.h"

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
// �� AudioDevice �N���X
//=============================================================================

    // �w�ʂ̉��������������R�[���̐ݒ�
	static const X3DAUDIO_CONE sListener_DirectionalCone = {
        X3DAUDIO_PI*5.0f/6.0f,
        X3DAUDIO_PI*11.0f/6.0f,
        1.0f, 1.75f,
        0.0f, 0.25f,
        0.708f, 1.0f };

    // �������S�ɕ������Ȃ��Ȃ鋗�� (XACT �g���΂���Ȃ̂���Ȃ����ۂ����ǁA�Ȃ񂩖ʓ|�Ȃ̂�)
    static const float OuterDistance    = 14.0f;


    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    AudioDevice::AudioDevice()
        : mXAudio                   ( NULL )
        , mMasteringVoice           ( NULL )
        , mDeviceChannels           ( 0 )
        //, mOnMemoryLimitSize        ( 100000 )
        , mX3DAudioModule           ( NULL )
        , mMD_X3DAudioCalculate     ( NULL )
        , mListenableDistance       ( 10.0f )
        , mListenableDistanceInv    ( OuterDistance / 10.0f )
        , mMetreUnitDistanceInv     ( 1.0f )
        , mLogFile                  ( NULL )
        , mCoInited                 ( false )
    {
        memset( mX3DInstance, 0, sizeof( mX3DInstance ) );
    }


    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    AudioDevice::~AudioDevice()
    {
        LN_LOG_FIN_BEGIN;

        if ( DirectMusicManager::getInstance() )
        {
            DirectMusicManager::getInstance()->finalize();
        }

        if ( mMasteringVoice )
		{
			mMasteringVoice->DestroyVoice();
			mMasteringVoice = NULL;
		}

        if ( mXAudio )
        {
            mXAudio->StopEngine();
		    SAFE_RELEASE( mXAudio );
        }

        if ( mCoInited )
        {
            ::CoUninitialize();
            mCoInited = false;
        }

        if ( mX3DAudioModule )
        {
            ::FreeLibrary( mX3DAudioModule );
            mX3DAudioModule = NULL;
        }

        SAFE_DELETE( mAudioPlayerList );
        

        LN_LOG_FIN_END;
    }

    //---------------------------------------------------------------------
	// �� ������
    //---------------------------------------------------------------------
    LNRESULT AudioDevice::initialize( const Initdata& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        LNRESULT lr;
        HRESULT hr;

        mAudioPlayerList = LN_NEW AudioPlayerList();

        if ( SUCCEEDED( ::CoInitializeEx( NULL, COINIT_MULTITHREADED ) ) )
        {
            mCoInited = true;
        }
        

        mX3DAudioModule = ::LoadLibrary( _T( "X3DAudio1_7.dll" ) );
        LN_SETERR_R_S_( ( mX3DAudioModule ), LN_ERR_SYSTEM, "< AudioDevice::initialize >\nLoadLibrary" );

        UINT32 flags = 0;

	#ifdef LNOTE_DEBUG

        if ( init_data_.EnableDebugFlag )
        {
		    flags |= XAUDIO2_DEBUG_ENGINE;
        }

	#endif

        // XAudio2 �쐬
        LN_DXCALL_R( XAudio2Create( &mXAudio, flags ) );

		// �}�X�^�[�{�C�X�쐬
		LN_DXCALL_R( mXAudio->CreateMasteringVoice( &mMasteringVoice ) );

        LN_LOG_WRITE( "�I�[�f�B�I�f�o�C�X�̏����擾���܂�..." );

		// �f�o�C�X�ڍ׏��̊m�F
		XAUDIO2_DEVICE_DETAILS details;
		hr = mXAudio->GetDeviceDetails( 0, &details );
        if ( SUCCEEDED( hr ) )
        {
            LN_LOG_WRITE( L"    DeviceID    : %s", details.DeviceID );
            LN_LOG_WRITE( L"    DisplayName : %s", details.DisplayName );
            LN_LOG_WRITE( L"    Role        : %d", details.Role );
            LN_LOG_WRITE( L"    OutputFormat ( WAVEFORMATEX )" );
            LN_LOG_WRITE( "        �t�H�[�}�b�gID           : %hu", details.OutputFormat.Format.wFormatTag );
	        LN_LOG_WRITE( "        �`�����l����             : %hu", details.OutputFormat.Format.nChannels );
	        LN_LOG_WRITE( "        �T���v�����O���[�g       : %lu", details.OutputFormat.Format.nSamplesPerSec );
	        LN_LOG_WRITE( "        �f�[�^���x(Byte/sec)     : %lu", details.OutputFormat.Format.nAvgBytesPerSec );
	        LN_LOG_WRITE( "        �u���b�N�T�C�Y           : %hu", details.OutputFormat.Format.nBlockAlign );
	        LN_LOG_WRITE( "        �T���v��������̃r�b�g�� : %hu", details.OutputFormat.Format.wBitsPerSample );
	        LN_LOG_WRITE( "        �g�������̃T�C�Y         : %hu", details.OutputFormat.Format.cbSize );
        }
        else
        {
            LN_LOG_WRITE( "�I�[�f�B�I�f�o�C�X�̏����擾�Ɏ��s���܂����B" );
        }

        // �`�����l�����L��
        mDeviceChannels = details.OutputFormat.Format.nChannels;
		
        // �֐��A�h���X���擾���� 3D �I�[�f�B�I�������� ( �O���[�o�����̍쐬 )
        typedef void ( STDAPIVCALLTYPE *DEF_X3DAudioInitialize ) ( UINT32 , FLOAT32 , __out X3DAUDIO_HANDLE );
        DEF_X3DAudioInitialize md_X3DAudioInitialize = (DEF_X3DAudioInitialize)GetProcAddress( mX3DAudioModule, "X3DAudioInitialize" );

	    md_X3DAudioInitialize(
			details.OutputFormat.dwChannelMask,		// Xbox 360 �ł� SPEAKER_XBOX
			X3DAUDIO_SPEED_OF_SOUND, mX3DInstance );
		    //SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER |
            //SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT

        // ���X�i�[�̐ݒ�
        mListenerState.OrientFront	= D3DXVECTOR3( 0, 0, 1 );	// �O�����̌���
		mListenerState.OrientTop	= D3DXVECTOR3( 0, 1, 0 );	// ������̌��� ( OrientFront �Ɛ��K�����ł��邱�� )
		mListenerState.Position		= D3DXVECTOR3( 0, 0, 0 );
		mListenerState.Velocity		= D3DXVECTOR3( 0, 0, 0 );
		mListenerState.pCone		= NULL;//(X3DAUDIO_CONE*)&sListener_DirectionalCone;

        // X3DAudioCalculate() �̊֐��A�h���X
        mMD_X3DAudioCalculate = (DEF_X3DAudioCalculate)GetProcAddress( mX3DAudioModule, "X3DAudioCalculate" );

        // DirectMusic ������������ꍇ
        if ( init_data_.DMInitMode != LN_DMUSINGMODE_NOTUSE && init_data_.Window )
        {
            DirectMusicManager::InitData dm_data;
            dm_data.LogFile         = mLogFile;
            dm_data.DMInitMode      = init_data_.DMInitMode;
            dm_data.WindowHandle    = init_data_.Window->getWindowHandle();
            LN_CALL_R( DirectMusicManager::initialize( dm_data ) );
        }

        LN_LOG_INIT_END;
        return LN_OK;
    }


    //---------------------------------------------------------------------
	// �� 3D �����̌v�Z������
    //---------------------------------------------------------------------
    LNRESULT AudioDevice::calcEmitterState( EmitterState* emitter_ )
    {
        if ( emitter_ )
        {
            static const DWORD calc_flags =
				X3DAUDIO_CALCULATE_MATRIX       |
                X3DAUDIO_CALCULATE_LPF_DIRECT   |
                X3DAUDIO_CALCULATE_LPF_REVERB   |
                X3DAUDIO_CALCULATE_REVERB       |
                X3DAUDIO_CALCULATE_DOPPLER;
                //X3DAUDIO_CALCULATE_EMITTER_ANGLE;
            //if (g_audioState.fUseRedirectToLFE)
		    //{
		    //	// On devices with an LFE channel, allow the mono source data
		    //	// to be routed to the LFE destination channel.
		    //	calc_flags |= X3DAUDIO_CALCULATE_REDIRECT_TO_LFE;
		    //}

            emitter_->updateXAudioEmitter( mMetreUnitDistanceInv );
            emitter_->DSPSettings.DstChannelCount = mDeviceChannels;

            mMD_X3DAudioCalculate(
                mX3DInstance,
                &mListenerState,
                &emitter_->Emitter,
                calc_flags,
                &emitter_->DSPSettings );
        }

        return LN_OK;
    }

    

    //---------------------------------------------------------------------
	// �� IAudioPlayer ���쐬����
    //---------------------------------------------------------------------
    LNRESULT AudioDevice::createAudioPlayer( IAudioPlayer** obj_, IAudioSource* source_, bool enable_3d_, LNSoundPlayType type_ )
    {
        AudioPlayerBase* audio_player = NULL;
        *obj_ = NULL;

        

        // ��ނɉ����ăv���C���[���쐬����
		switch ( type_ )
		{
			// �I���������Đ�
			case LN_SOUNDPLAYTYPE_ONMEMORY:
            {
                OnMemory* player = LN_NEW OnMemory( this );
                player->initialize( source_, enable_3d_ );
                audio_player = player;
				break;
            }
			// �X�g���[�~���O�Đ�
			case LN_SOUNDPLAYTYPE_STREAMING:
            {
                Streaming* player = LN_NEW Streaming( this );
				player->initialize( source_, enable_3d_ );
                audio_player = player;
				break;
            }
			// SMF
			case LN_SOUNDPLAYTYPE_MIDI:
            {
                if ( !DirectMusicManager::getInstance() )
                {
                    LN_SETERR_R_S( LN_ERR_NOINITIALIZE, "DirectMusic ������������Ă��܂���B" );
                }
 
                MidiPlayer* player = LN_NEW MidiPlayer( this );
				player->initialize( source_, enable_3d_ );
                audio_player = player;
				break;
            }
		}

        mAudioPlayerList->push_back( audio_player );

        *obj_ = audio_player;

        return LN_OK;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void AudioDevice::update()
    {
        mListenerState.OrientFront.x = m3DSoundListener.Direction.x;
        mListenerState.OrientFront.y = m3DSoundListener.Direction.y;
        mListenerState.OrientFront.z = m3DSoundListener.Direction.z;
        mListenerState.OrientTop.x = m3DSoundListener.UpDirection.x;
        mListenerState.OrientTop.y = m3DSoundListener.UpDirection.y;
        mListenerState.OrientTop.z = m3DSoundListener.UpDirection.z;
        mListenerState.Position.x = m3DSoundListener.Position.x * mMetreUnitDistanceInv;
        mListenerState.Position.y = m3DSoundListener.Position.y * mMetreUnitDistanceInv;
        mListenerState.Position.z = m3DSoundListener.Position.z * mMetreUnitDistanceInv;
        mListenerState.Velocity.x = m3DSoundListener.Velocity.x * mMetreUnitDistanceInv;
        mListenerState.Velocity.y = m3DSoundListener.Velocity.y * mMetreUnitDistanceInv;
        mListenerState.Velocity.z = m3DSoundListener.Velocity.z * mMetreUnitDistanceInv;
    }

    //---------------------------------------------------------------------
	// �� 3D �����̃��X�i�[����ݒ肷��
    //---------------------------------------------------------------------
    LNRESULT AudioDevice::setListenerState( const LVector3& position_, const LVector3& front_ )
    {
        LN_SETERR_R_( ( !front_.isZero() ), LN_ERR_ASSERT, "< AudioDevice::setListenerState >\nfront_ length is 0" );

        // �O�̂��߂ɐ��K�����Ă���ݒ肷��
        LVector3 n;
        LVector3::normalize(  &n, front_ );

        mListenerState.OrientFront	= n.toD3DXVECTOR3();
		mListenerState.Position		= position_.toD3DXVECTOR3();

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� 3D �����̉������̐ݒ�
    //---------------------------------------------------------------------
    //void AudioDevice::setListenableDistance( lnfloat length_ )
    //{
    //    mListenableDistance = length_;
    //    mListenableDistanceInv = OuterDistance / mListenableDistance;

    //    AudioPlayerList::iterator it = mAudioPlayerList->begin();
    //    AudioPlayerList::iterator end = mAudioPlayerList->end();
    //    for ( ; it != end; ++it )
    //    {
    //        (*it)->getEmitterState()->updatePosition( mListenableDistanceInv );
    //    }
    //}

    //---------------------------------------------------------------------
	// �� IAudioPlayer �����X�g����O�� ( �e AudioPlayer �̃f�X�g���N�^����Ă΂�� )
    //---------------------------------------------------------------------
    void AudioDevice::removeAudioPlayer( IAudioPlayer* player_ )
    {
        mAudioPlayerList->remove( static_cast< AudioPlayerBase* >( player_ ) );

        
    }

  

//=============================================================================
// �� EmitterState
//=============================================================================

    static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[ 3 ]		= { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
	static const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve				= { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };
	static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[ 3 ]	= { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
	static const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve				= { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    EmitterState::EmitterState( u32 input_channels_ )//, u32 output_channels_ )
    {
        
        EmitterAzimuths = LN_NEW FLOAT32[ input_channels_ ];
		MatrixCoefficients = LN_NEW FLOAT32[ input_channels_ * OUTPUTCHANNELS ];

		for ( u32 i = 0; i < input_channels_; ++i )
		{
			EmitterAzimuths[ i ] = 0;
		}

		// �T�E���h�R�[���̐ݒ�
		EmitterCone.InnerAngle		= 0.0f;		// �����R�[���̊p�x ( ���W�A���P�� 0.0f �` X3DAUDIO_2PI )
		EmitterCone.OuterAngle		= 0.0f;		// �O���R�[���̊p�x ( ���W�A���P�� 0.0f �` X3DAUDIO_2PI )
		EmitterCone.InnerVolume	= 0.0f;		// �����R�[����/���̃{�����[���X�P�[�� ( 0.0f �` 2.0f )
		EmitterCone.OuterVolume	= 1.0f;		// �O���R�[����/�ȍ~�̃{�����[���X�P�[�� ( 0.0f �` 2.0f )
		EmitterCone.InnerLPF		= 0.0f;		// �����R�[����/���� LPF �_�C���N�g�p�X�܂��̓��o�[�u�p�X�̌W���X�P�[�� ( 0.0f �` 1.0f )
		EmitterCone.OuterLPF		= 1.0f;		// �O���R�[����/�ȍ~�� LPF �_�C���N�g�p�X�܂��̓��o�[�u�p�X�̌W���X�P�[�� ( 0.0f �` 1.0f )
		EmitterCone.InnerReverb	= 0.0f;		// �����R�[����/���̃��o�[�u �Z���h���x���X�P�[�� ( 0.0f �` 2.0f )
		EmitterCone.OuterReverb	= 1.0f;		// �O���R�[����/�ȍ~�̃��o�[�u �Z���h���x���X�P�[�� ( 0.0f �` 2.0f )
	
		// �G�~�b�^�̐ݒ�
		Emitter.pCone			= &EmitterCone;			// �T�E���h �R�[���ւ̃|�C���^�܂��� NULL ( NULL �͑S������ )
		Emitter.OrientFront	    = D3DXVECTOR3( 0, 0, 1 );	// �O�����̌��� ( OrientTop �Ɛ��K���� )
		Emitter.OrientTop		= D3DXVECTOR3( 0, 1, 0 );	// ������̌��� ( OrientFront �Ɛ��K���� )
		Emitter.Position		= D3DXVECTOR3( 0, 0, 0 );	// ���[���h�ʒu
		Emitter.Velocity		= D3DXVECTOR3( 0, 0, 0 );	// ���[�U�[��`�̃��[���h�P��/�b�̑��x

		Emitter.ChannelCount		= input_channels_;		// X3DAUDIO_EMITTER �\���̂ɂ���Ē�`�����G�~�b�^�̐�
		Emitter.ChannelRadius		= 1.0f;					// ChannelCount �� 1 ���傫���ꍇ�ɃG�~�b�^���z�u����� Position ����̋��� ( 0.0f �ȏ� )
		Emitter.pChannelAzimuths	= EmitterAzimuths;		// ���W�A���P�ʂ̕��ʊp�ŕ\�����`�����l���ʒu�e�[�u�� ( FLOAT32 �̔z��B�e�v�f 0.0f �` X3DAUDIO_2PI )
		Emitter.InnerRadius		    = 2.0f;					// �������a�̌v�Z�Ɏg�p�����l ( 0.0f �` MAX_FLT )
		Emitter.InnerRadiusAngle	= X3DAUDIO_PI / 4.0f;	// �����p�x�̌v�Z�Ɏg�p�����l ( 0.0f �` X3DAUDIO_PI/4.0 )

		Emitter.pVolumeCurve		= (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;	// �{�����[�� ���x�������J�[�u ( NULL �Ńf�t�H���g�l���g�� )
		Emitter.pLFECurve			= (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;				// LFE ���[���I�t�����J�[�u ( NULL �Ńf�t�H���g�l���g�� )
		Emitter.pLPFDirectCurve		= NULL;														// ���[�p�X�t�B���^�[ (LPF) �_�C���N�g�p�X�W�������J�[�u ( NULL �Ńf�t�H���g�l���g�� )
		Emitter.pLPFReverbCurve		= NULL;														// LPF ���o�[�u�p�X�W�������J�[�u ( NULL �Ńf�t�H���g�l���g�� )
		Emitter.pReverbCurve		= (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;			// ���o�[�u �Z���h ���x�������J�[�u ( NULL �Ńf�t�H���g�l���g�� )
		Emitter.CurveDistanceScaler	= 10.0f;													// ���K�����ꂽ�����J�[�u���X�P�[�����O���邽�߂Ɏg�p����J�[�u�����X�P�[�� ( FLT_MIN �` FLT_MAX )
		Emitter.DopplerScaler		= 1.0f;													// �h�b�v���[�Έڌ��ʂ��������邽�߂Ɏg�p����h�b�v���[�ΈڃX�P�[���[ ( 0.0f �` FLT_MAX )
	
		//  X3DAudioCalculate �̌Ăяo�����ʂ��󂯎��\���̂̏�����
		memset( &DSPSettings, 0, sizeof( DSPSettings ) );
		memset( MatrixCoefficients, 0, sizeof( FLOAT32 ) * input_channels_ * OUTPUTCHANNELS );
		DSPSettings.SrcChannelCount		= input_channels_;
		DSPSettings.DstChannelCount		= 0;//output_channels_;     // calc �ŃZ�b�g���Ă���
		DSPSettings.pMatrixCoefficients	= MatrixCoefficients;
    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    EmitterState::~EmitterState()
    {
        SAFE_DELETE_ARRAY( EmitterAzimuths );
		SAFE_DELETE_ARRAY( MatrixCoefficients );
    }

    //---------------------------------------------------------------------
	// �� ���W�̐ݒ�
    //---------------------------------------------------------------------
    //void EmitterState::setPosition( const LVector3& pos_, lnfloat ld_inv_ )
    //{
    //    Position = pos_;
    //    updatePosition( ld_inv_ );
    //}

    //---------------------------------------------------------------------
	// �� X3DAUDIO_EMITTER �ɐ��������W��ݒ肷��
    //---------------------------------------------------------------------
    void EmitterState::updateXAudioEmitter( lnfloat scale )
    {
        Emitter.Position.x = Position.x * scale;
        Emitter.Position.y = Position.y * scale;
        Emitter.Position.z = Position.z * scale;
        Emitter.Velocity.x = Velocity.x * scale;
        Emitter.Velocity.y = Velocity.y * scale;
        Emitter.Velocity.z = Velocity.z * scale;
        Emitter.CurveDistanceScaler = Distance * scale;
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