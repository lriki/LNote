//=============================================================================
//【 XA2AudioDevice 】
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
// ■ AudioDevice クラス
//=============================================================================

    // 背面の音源が減衰されるコーンの設定
	static const X3DAUDIO_CONE sListener_DirectionalCone = {
        X3DAUDIO_PI*5.0f/6.0f,
        X3DAUDIO_PI*11.0f/6.0f,
        1.0f, 1.75f,
        0.0f, 0.25f,
        0.708f, 1.0f };

    // 音が完全に聞こえなくなる距離 (XACT 使えばこんなのいらないっぽいけど、なんか面倒なので)
    static const float OuterDistance    = 14.0f;


    //---------------------------------------------------------------------
	// ● コンストラクタ
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
	// ● デストラクタ
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
	// ● 初期化
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

        // XAudio2 作成
        LN_DXCALL_R( XAudio2Create( &mXAudio, flags ) );

		// マスターボイス作成
		LN_DXCALL_R( mXAudio->CreateMasteringVoice( &mMasteringVoice ) );

        LN_LOG_WRITE( "オーディオデバイスの情報を取得します..." );

		// デバイス詳細情報の確認
		XAUDIO2_DEVICE_DETAILS details;
		hr = mXAudio->GetDeviceDetails( 0, &details );
        if ( SUCCEEDED( hr ) )
        {
            LN_LOG_WRITE( L"    DeviceID    : %s", details.DeviceID );
            LN_LOG_WRITE( L"    DisplayName : %s", details.DisplayName );
            LN_LOG_WRITE( L"    Role        : %d", details.Role );
            LN_LOG_WRITE( L"    OutputFormat ( WAVEFORMATEX )" );
            LN_LOG_WRITE( "        フォーマットID           : %hu", details.OutputFormat.Format.wFormatTag );
	        LN_LOG_WRITE( "        チャンネル数             : %hu", details.OutputFormat.Format.nChannels );
	        LN_LOG_WRITE( "        サンプリングレート       : %lu", details.OutputFormat.Format.nSamplesPerSec );
	        LN_LOG_WRITE( "        データ速度(Byte/sec)     : %lu", details.OutputFormat.Format.nAvgBytesPerSec );
	        LN_LOG_WRITE( "        ブロックサイズ           : %hu", details.OutputFormat.Format.nBlockAlign );
	        LN_LOG_WRITE( "        サンプルあたりのビット数 : %hu", details.OutputFormat.Format.wBitsPerSample );
	        LN_LOG_WRITE( "        拡張部分のサイズ         : %hu", details.OutputFormat.Format.cbSize );
        }
        else
        {
            LN_LOG_WRITE( "オーディオデバイスの情報を取得に失敗しました。" );
        }

        // チャンネル数記憶
        mDeviceChannels = details.OutputFormat.Format.nChannels;
		
        // 関数アドレスを取得して 3D オーディオを初期化 ( グローバル情報の作成 )
        typedef void ( STDAPIVCALLTYPE *DEF_X3DAudioInitialize ) ( UINT32 , FLOAT32 , __out X3DAUDIO_HANDLE );
        DEF_X3DAudioInitialize md_X3DAudioInitialize = (DEF_X3DAudioInitialize)GetProcAddress( mX3DAudioModule, "X3DAudioInitialize" );

	    md_X3DAudioInitialize(
			details.OutputFormat.dwChannelMask,		// Xbox 360 では SPEAKER_XBOX
			X3DAUDIO_SPEED_OF_SOUND, mX3DInstance );
		    //SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER |
            //SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT

        // リスナーの設定
        mListenerState.OrientFront	= D3DXVECTOR3( 0, 0, 1 );	// 前方向の向き
		mListenerState.OrientTop	= D3DXVECTOR3( 0, 1, 0 );	// 上方向の向き ( OrientFront と正規直交であること )
		mListenerState.Position		= D3DXVECTOR3( 0, 0, 0 );
		mListenerState.Velocity		= D3DXVECTOR3( 0, 0, 0 );
		mListenerState.pCone		= NULL;//(X3DAUDIO_CONE*)&sListener_DirectionalCone;

        // X3DAudioCalculate() の関数アドレス
        mMD_X3DAudioCalculate = (DEF_X3DAudioCalculate)GetProcAddress( mX3DAudioModule, "X3DAudioCalculate" );

        // DirectMusic を初期化する場合
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
	// ● 3D 音源の計算をする
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
	// ● IAudioPlayer を作成する
    //---------------------------------------------------------------------
    LNRESULT AudioDevice::createAudioPlayer( IAudioPlayer** obj_, IAudioSource* source_, bool enable_3d_, LNSoundPlayType type_ )
    {
        AudioPlayerBase* audio_player = NULL;
        *obj_ = NULL;

        

        // 種類に応じてプレイヤーを作成する
		switch ( type_ )
		{
			// オンメモリ再生
			case LN_SOUNDPLAYTYPE_ONMEMORY:
            {
                OnMemory* player = LN_NEW OnMemory( this );
                player->initialize( source_, enable_3d_ );
                audio_player = player;
				break;
            }
			// ストリーミング再生
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
                    LN_SETERR_R_S( LN_ERR_NOINITIALIZE, "DirectMusic が初期化されていません。" );
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
	// ● 3D 音源のリスナー情報を設定する
    //---------------------------------------------------------------------
    LNRESULT AudioDevice::setListenerState( const LVector3& position_, const LVector3& front_ )
    {
        LN_SETERR_R_( ( !front_.isZero() ), LN_ERR_ASSERT, "< AudioDevice::setListenerState >\nfront_ length is 0" );

        // 念のために正規化してから設定する
        LVector3 n;
        LVector3::normalize(  &n, front_ );

        mListenerState.OrientFront	= n.toD3DXVECTOR3();
		mListenerState.Position		= position_.toD3DXVECTOR3();

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 3D 音源の可聴距離の設定
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
	// ● IAudioPlayer をリストから外す ( 各 AudioPlayer のデストラクタから呼ばれる )
    //---------------------------------------------------------------------
    void AudioDevice::removeAudioPlayer( IAudioPlayer* player_ )
    {
        mAudioPlayerList->remove( static_cast< AudioPlayerBase* >( player_ ) );

        
    }

  

//=============================================================================
// ■ EmitterState
//=============================================================================

    static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[ 3 ]		= { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
	static const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve				= { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };
	static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[ 3 ]	= { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
	static const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve				= { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    EmitterState::EmitterState( u32 input_channels_ )//, u32 output_channels_ )
    {
        
        EmitterAzimuths = LN_NEW FLOAT32[ input_channels_ ];
		MatrixCoefficients = LN_NEW FLOAT32[ input_channels_ * OUTPUTCHANNELS ];

		for ( u32 i = 0; i < input_channels_; ++i )
		{
			EmitterAzimuths[ i ] = 0;
		}

		// サウンドコーンの設定
		EmitterCone.InnerAngle		= 0.0f;		// 内部コーンの角度 ( ラジアン単位 0.0f ～ X3DAUDIO_2PI )
		EmitterCone.OuterAngle		= 0.0f;		// 外部コーンの角度 ( ラジアン単位 0.0f ～ X3DAUDIO_2PI )
		EmitterCone.InnerVolume	= 0.0f;		// 内部コーン上/内のボリュームスケーラ ( 0.0f ～ 2.0f )
		EmitterCone.OuterVolume	= 1.0f;		// 外部コーン上/以降のボリュームスケーラ ( 0.0f ～ 2.0f )
		EmitterCone.InnerLPF		= 0.0f;		// 内部コーン上/内の LPF ダイレクトパスまたはリバーブパスの係数スケーラ ( 0.0f ～ 1.0f )
		EmitterCone.OuterLPF		= 1.0f;		// 外部コーン上/以降の LPF ダイレクトパスまたはリバーブパスの係数スケーラ ( 0.0f ～ 1.0f )
		EmitterCone.InnerReverb	= 0.0f;		// 内部コーン上/内のリバーブ センドレベルスケーラ ( 0.0f ～ 2.0f )
		EmitterCone.OuterReverb	= 1.0f;		// 外部コーン上/以降のリバーブ センドレベルスケーラ ( 0.0f ～ 2.0f )
	
		// エミッタの設定
		Emitter.pCone			= &EmitterCone;			// サウンド コーンへのポインタまたは NULL ( NULL は全方向性 )
		Emitter.OrientFront	    = D3DXVECTOR3( 0, 0, 1 );	// 前方向の向き ( OrientTop と正規直交 )
		Emitter.OrientTop		= D3DXVECTOR3( 0, 1, 0 );	// 上方向の向き ( OrientFront と正規直交 )
		Emitter.Position		= D3DXVECTOR3( 0, 0, 0 );	// ワールド位置
		Emitter.Velocity		= D3DXVECTOR3( 0, 0, 0 );	// ユーザー定義のワールド単位/秒の速度

		Emitter.ChannelCount		= input_channels_;		// X3DAUDIO_EMITTER 構造体によって定義されるエミッタの数
		Emitter.ChannelRadius		= 1.0f;					// ChannelCount が 1 より大きい場合にエミッタが配置される Position からの距離 ( 0.0f 以上 )
		Emitter.pChannelAzimuths	= EmitterAzimuths;		// ラジアン単位の方位角で表したチャンネル位置テーブル ( FLOAT32 の配列。各要素 0.0f ～ X3DAUDIO_2PI )
		Emitter.InnerRadius		    = 2.0f;					// 内部半径の計算に使用される値 ( 0.0f ～ MAX_FLT )
		Emitter.InnerRadiusAngle	= X3DAUDIO_PI / 4.0f;	// 内部角度の計算に使用される値 ( 0.0f ～ X3DAUDIO_PI/4.0 )

		Emitter.pVolumeCurve		= (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;	// ボリューム レベル距離カーブ ( NULL でデフォルト値を使う )
		Emitter.pLFECurve			= (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;				// LFE ロールオフ距離カーブ ( NULL でデフォルト値を使う )
		Emitter.pLPFDirectCurve		= NULL;														// ローパスフィルター (LPF) ダイレクトパス係数距離カーブ ( NULL でデフォルト値を使う )
		Emitter.pLPFReverbCurve		= NULL;														// LPF リバーブパス係数距離カーブ ( NULL でデフォルト値を使う )
		Emitter.pReverbCurve		= (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;			// リバーブ センド レベル距離カーブ ( NULL でデフォルト値を使う )
		Emitter.CurveDistanceScaler	= 10.0f;													// 正規化された距離カーブをスケーリングするために使用するカーブ距離スケーラ ( FLT_MIN ～ FLT_MAX )
		Emitter.DopplerScaler		= 1.0f;													// ドップラー偏移効果を強調するために使用するドップラー偏移スケーラー ( 0.0f ～ FLT_MAX )
	
		//  X3DAudioCalculate の呼び出し結果を受け取る構造体の初期化
		memset( &DSPSettings, 0, sizeof( DSPSettings ) );
		memset( MatrixCoefficients, 0, sizeof( FLOAT32 ) * input_channels_ * OUTPUTCHANNELS );
		DSPSettings.SrcChannelCount		= input_channels_;
		DSPSettings.DstChannelCount		= 0;//output_channels_;     // calc でセットしておく
		DSPSettings.pMatrixCoefficients	= MatrixCoefficients;
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    EmitterState::~EmitterState()
    {
        SAFE_DELETE_ARRAY( EmitterAzimuths );
		SAFE_DELETE_ARRAY( MatrixCoefficients );
    }

    //---------------------------------------------------------------------
	// ● 座標の設定
    //---------------------------------------------------------------------
    //void EmitterState::setPosition( const LVector3& pos_, lnfloat ld_inv_ )
    //{
    //    Position = pos_;
    //    updatePosition( ld_inv_ );
    //}

    //---------------------------------------------------------------------
	// ● X3DAUDIO_EMITTER に正しい座標を設定する
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