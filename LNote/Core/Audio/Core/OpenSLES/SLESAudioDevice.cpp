//=============================================================================
//【 SLESAudioDevice 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../../Math/Math.h"
#include "../AudioUtil.h"
#include "SLESAudioPlayer.h"
#include "SLESAudioDevice.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Audio
{
namespace SLES
{

//=============================================================================
// ■ AudioDevice クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    AudioDevice::AudioDevice()
        : mSLEngineObject       ( NULL )
        , mSLEngineEngine       ( NULL )
        , mLogFile                  ( NULL )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    AudioDevice::~AudioDevice()
    {
        LN_LOG_FIN_BEGIN;

        if ( mSLEngineObject )
        {
            (*mSLEngineObject)->Destroy( mSLEngineObject );
            mSLEngineObject = NULL;
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

        mAudioPlayerList = LN_NEW AudioPlayerList();

        SLresult result;

        //-----------------------------------------------------
        // Engine のサポートしているインターフェイスを調べる
        
        // Nbr of engine interfaces
        SLuint32 supported_interface_num = 0;
        slQueryNumSupportedEngineInterfaces( &supported_interface_num );
        LN_LOG_WRITE( _T( "  OpenSL supported engine interfaces: %d" ), supported_interface_num );

        for ( SLuint32 i = 0; i< supported_interface_num; ++i )
        {
            SLInterfaceID  interface_id;
            slQuerySupportedEngineInterfaces( i, &interface_id );
            
            if ( interface_id == SL_IID_OBJECT )                            { 
            	LN_LOG_WRITE( _T( "    SL_IID_OBJECT" ) ); }
            else if ( interface_id == SL_IID_DYNAMICINTERFACEMANAGEMENT )   { 
            	LN_LOG_WRITE( _T( "    SL_IID_DYNAMICINTERFACEMANAGEMENT" ) ); }
            else if ( interface_id == SL_IID_ENGINE )                       { 
            	LN_LOG_WRITE( _T( "    SL_IID_ENGINE" ) ); }
            else if ( interface_id == SL_IID_ENGINECAPABILITIES )           { 
            	LN_LOG_WRITE( _T( "    SL_IID_ENGINECAPABILITIES" ) ); }
            else if ( interface_id == SL_IID_THREADSYNC )                   { 
            	LN_LOG_WRITE( _T( "    SL_IID_THREADSYNC" ) ); }
            else if ( interface_id == SL_IID_AUDIOIODEVICECAPABILITIES )    { 
            	LN_LOG_WRITE( _T( "    SL_IID_AUDIOIODEVICECAPABILITIES" ) ); }
            else if ( interface_id == SL_IID_AUDIOENCODERCAPABILITIES )     { 
            	LN_LOG_WRITE( _T( "    SL_IID_AUDIOENCODERCAPABILITIES" ) ); }
            else if ( interface_id == SL_IID_AUDIODECODERCAPABILITIES )     { 
            	LN_LOG_WRITE( _T( "    SL_IID_AUDIODECODERCAPABILITIES" ) ); }
            else if ( interface_id == SL_IID_3DCOMMIT )                     { 
            	LN_LOG_WRITE( _T( "    SL_IID_3DCOMMIT" ) ); }
            else if ( interface_id == SL_IID_DEVICEVOLUME )                 { 
            	LN_LOG_WRITE( _T( "    SL_IID_DEVICEVOLUME" ) ); }
            else if ( interface_id == SL_IID_ANDROIDEFFECTCAPABILITIES )    { 
            	LN_LOG_WRITE( _T( "    SL_IID_ANDROIDEFFECTCAPABILITIES" ) ); }
            else                                                            { 
            	LN_LOG_WRITE( _T( "    UNKNOWN %x" ), interface_id->time_low ); }
        }
        
        //-----------------------------------------------------
        // SLES エンジンの作成
        
        // Android では SL_ENGINEOPTION_THREADSAFE、SL_ENGINEOPTION_LOSSOFCONTROL を指定しても無視される
        SLEngineOption engine_option[1];
        engine_option[0].feature = SL_ENGINEOPTION_THREADSAFE;
        engine_option[0].data    = SL_BOOLEAN_TRUE;
        //engine_option[1].feature = SL_ENGINEOPTION_MAJORVERSION;
        //engine_option[1].data    = 1;
        //engine_option[2].feature = SL_ENGINEOPTION_MINORVERSION;
        //engine_option[2].data    = 1;
        result = slCreateEngine( &mSLEngineObject, 1, engine_option, 0, NULL, NULL );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
        
        result = (*mSLEngineObject)->Realize( mSLEngineObject, SL_BOOLEAN_FALSE );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
        
        // エンジンインターフェイスの取得。ほかのいろんなオブジェクトを作るのに必要
        result = (*mSLEngineObject)->GetInterface( mSLEngineObject, SL_IID_ENGINE, &mSLEngineEngine );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

        //-----------------------------------------------------
        // デバイス情報の取得
        //      Android では SL_IID_ENGINECAPABILITIES がサポートされていないみたいなので取得不可
#if 0

        SLEngineCapabilitiesItf cap;
        result = (*mSLEngineObject)->GetInterface( mSLEngineObject, SL_IID_ENGINECAPABILITIES, &cap );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

        // Query API version
        SLint16 vMajor = 0, vMinor = 0, vStep = 0;
        result = (*cap)->QueryAPIVersion(
            cap,
            &vMajor,
            &vMinor,
            &vStep );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

        LN_LOG_WRITE( _T( "  Audio API Version : %d.%d.%d" ), vMajor, vMinor, vStep );

        // Query profile support
        SLuint16 profiles = 0;
        result = (*cap)->QuerySupportedProfiles( cap, &profiles );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
        
        bool is_game_profile_supported = ( profiles & SL_PROFILES_GAME );
        
        LN_LOG_WRITE( _T( "  SupportedProfiles :" ) );
        LN_LOG_WRITE( _T( "    SL_PROFILES_PHONE ... %s" ), ( profiles & SL_PROFILES_PHONE ) ? _T( "ok" ) : _T( "no" ) );
        LN_LOG_WRITE( _T( "    SL_PROFILES_MUSIC ... %s" ), ( profiles & SL_PROFILES_MUSIC ) ? _T( "ok" ) : _T( "no" ) );
        LN_LOG_WRITE( _T( "    SL_PROFILES_GAME  ... %s" ), ( profiles & SL_PROFILES_GAME  ) ? _T( "ok" ) : _T( "no" ) );
        
        // Query available voices for 2D audio
        SLint16 numMax2DVoices = 0;
        SLboolean isAbsoluteMax2D = SL_BOOLEAN_FALSE;
        SLint16 numFree2DVoices = 0;
        result = (*cap)->QueryAvailableVoices(
            cap, 
            SL_VOICETYPE_2D_AUDIO,
            &numMax2DVoices, 
            &isAbsoluteMax2D,
            &numFree2DVoices );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
        
        LN_LOG_WRITE( _T( "  2D Audio :" ) );
        LN_LOG_WRITE( _T( "    MaxVoices   : %hd" ), numMax2DVoices );
        LN_LOG_WRITE( _T( "    AbsoluteMax : %s" ), ( isAbsoluteMax2D ) ? _T( "true" ) : _T( "false" ) );
        LN_LOG_WRITE( _T( "    FreeVoices  : %hd" ), numFree2DVoices );

        // Query available voices for MIDI. (MIDI is mandated only in the PHONE and GAME profiles.
        SLint16 numMaxMIDIVoices = 0, numFreeMIDIVoices = 0;
        SLboolean isAbsoluteMaxMIDI = SL_BOOLEAN_FALSE; 
        result = (*cap)->QueryAvailableVoices( 
            cap,
            SL_VOICETYPE_MIDI,
            &numMaxMIDIVoices, 
            &isAbsoluteMaxMIDI,
            &numFreeMIDIVoices );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
        
        LN_LOG_WRITE( _T( "  MIDI :" ) );
        LN_LOG_WRITE( _T( "    MaxVoices   : %hd" ), numMaxMIDIVoices );
        LN_LOG_WRITE( _T( "    AbsoluteMax : %s" ), ( isAbsoluteMaxMIDI ) ? _T( "true" ) : _T( "false" ) );
        LN_LOG_WRITE( _T( "    FreeVoices  : %hd" ), numFreeMIDIVoices );

        // 3D audio functionality is mandated only in the game profile, so might want to query for 3D voice types only if GAME profile is supported 
        if ( is_game_profile_supported )
        { 
            // 3D Audio
            SLint16 numMax3DVoices = 0, numFree3DVoices = 0;
            SLboolean isAbsoluteMax3D = SL_BOOLEAN_FALSE;
            result = (*cap)->QueryAvailableVoices( 
                cap,
                SL_VOICETYPE_3D_AUDIO,
                &numMax3DVoices, 
                &isAbsoluteMax3D,
                &numFree3DVoices );
            LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
            
            LN_LOG_WRITE( _T( "  3D Audio :" ) );
            LN_LOG_WRITE( _T( "    MaxVoices   : %hd" ), numMax3DVoices );
            LN_LOG_WRITE( _T( "    AbsoluteMax : %s" ), ( isAbsoluteMax3D ) ? _T( "true" ) : _T( "false" ) );
            LN_LOG_WRITE( _T( "    FreeVoices  : %hd" ), numFree3DVoices );
            
            // 3D MIDI
            SLint16 numMax3DMidiVoices = 0, numFree3DMidiVoices = 0; 
            SLboolean isAbsoluteMax3DMidi = SL_BOOLEAN_FALSE;
            result = (*cap)->QueryAvailableVoices(
                cap,
                SL_VOICETYPE_3D_MIDIOUTPUT,
                &numMax3DMidiVoices, 
                &isAbsoluteMax3DMidi,
                &numFree3DMidiVoices );
            LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
            
            LN_LOG_WRITE( _T( "  3D MIDI :" ) );
            LN_LOG_WRITE( _T( "    MaxVoices   : %hd" ), numMax3DMidiVoices );
            LN_LOG_WRITE( _T( "    AbsoluteMax : %s" ), ( isAbsoluteMax3DMidi ) ? _T( "true" ) : _T( "false" ) );
            LN_LOG_WRITE( _T( "    FreeVoices  : %hd" ), numFree3DMidiVoices );
        } 
        
        // Determine if implementation is thread safe */
        SLboolean isThreadSafe = SL_BOOLEAN_FALSE; 
        result = (*cap)->IsThreadSafe( cap, &isThreadSafe );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
        LN_LOG_WRITE( _T( "    ThreadSafe : %s" ), ( isThreadSafe ) ? _T( "true" ) : _T( "false" ) );

#endif 

        LN_LOG_INIT_END;
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● IAudioPlayer を作成する
    //---------------------------------------------------------------------
    LNRESULT AudioDevice::createAudioPlayer( IAudioPlayer** obj_, IAudioSource* source_, bool enable_3d_, LNAudioPlayType type_ )
    {
        AudioPlayerBase* audio_player = NULL;
        *obj_ = NULL;

        // 種類に応じてプレイヤーを作成する
		switch ( type_ )
		{
			// オンメモリ再生
			case LN_AUDIOPLAYTYPE_ONMEMORY:
            {
                OnMemory* player = LN_NEW OnMemory( this );
                player->initialize( source_, enable_3d_ );
                audio_player = player;
				break;
            }
			// ストリーミング再生
			case LN_AUDIOPLAYTYPE_STREAMING:
            {
                Streaming* player = LN_NEW Streaming( this );
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
	// ● 3D 音源のリスナー情報を設定する
    //---------------------------------------------------------------------
    LNRESULT AudioDevice::setListenerState( const LVector3& position_, const LVector3& front_ )
    {
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 3D 音源の可聴距離の設定
    //---------------------------------------------------------------------
    void AudioDevice::setListenableDistance( lnfloat length_ )
    {
    }

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

    /*
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
		Emitter.CurveDistanceScaler	= 14.0f;													// 正規化された距離カーブをスケーリングするために使用するカーブ距離スケーラ ( FLT_MIN ～ FLT_MAX )
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
    void EmitterState::setPosition( const LVector3& pos_, lnfloat ld_inv_ )
    {
        Position = pos_;
        updatePosition( ld_inv_ );
    }

    //---------------------------------------------------------------------
	// ● X3DAUDIO_EMITTER に正しい座標を設定する
    //---------------------------------------------------------------------
    void EmitterState::updatePosition( lnfloat ld_inv_ )
    {
        Emitter.Position.x = Position.x * ld_inv_;
        Emitter.Position.y = Position.y * ld_inv_;
        Emitter.Position.z = Position.z * ld_inv_;
    }
    */

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace SLES
} // namespace Audio
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================