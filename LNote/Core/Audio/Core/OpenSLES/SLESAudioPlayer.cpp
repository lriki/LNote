//=============================================================================
//【 SLESAudioPlayer 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../../Math/Math.h"
#include "../AudioUtil.h"
#include "SLESAudioPlayer.h"

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

/// サンプリングレート -> SL_SAMPLINGRATE への変換 (非対応は 0)
SLuint32 convertSamplingRateValue( lnU32 sr_ )
{
    switch ( sr_ )
    {
        case 8000: 
        	return SL_SAMPLINGRATE_8;
        case 11025: 
        	return SL_SAMPLINGRATE_11_025;
        case 16000: 
        	return SL_SAMPLINGRATE_16;
        case 22050: 
        	return SL_SAMPLINGRATE_22_05;
        case 24000: 
        	return SL_SAMPLINGRATE_24;
        case 32000: 
        	return SL_SAMPLINGRATE_32;
        case 44100: 
        	return SL_SAMPLINGRATE_44_1;
        case 64000: 
        	return SL_SAMPLINGRATE_64;
        case 88200: 
        	return SL_SAMPLINGRATE_88_2;
        case 96000: 
        	return SL_SAMPLINGRATE_96;
        case 192000: 
        	return SL_SAMPLINGRATE_192;
        default: 
        	return 0;
    }
}

/// サンプリングあたりのビット数 -> SL_PCMSAMPLEFORMAT への変換 (非対応は 0)
SLuint32 convertBitsPerSampleValue( lnU16 bps_ )
{
    switch ( bps_ )
    {
        case 8:  
        	return SL_PCMSAMPLEFORMAT_FIXED_8;
        case 16: 
        	return SL_PCMSAMPLEFORMAT_FIXED_16;
        return 0;
    }
}

/// 音量（0.0～1.0）をミリベルへ変換する
SLint16 convertVolume2Milibell( float volume_ )
{
  const float min_limit = 0.1f;
  const float vol = LMath::limit< float >( 1.f - volume_, 0.f, 1.f );
  SLint16 volume_level = SL_MILLIBEL_MIN;
  if ( volume_ > min_limit )
  {
    const static float base2 = log( 2 );
    const float antilogarithm = log( 1.f / vol );
    const float bell = base2 / antilogarithm;
    volume_level = static_cast< SLint16 >( bell * -1000.f );
  }
  return volume_level;
}

//=============================================================================
// ■ OnMemory クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	OnMemory::OnMemory( AudioDevice* manager_ )
		: mManager              ( manager_ )
        , mAudioSource          ( NULL )
        , mSLOutputMixObject    ( NULL )
        , mSLPlayerObject       ( NULL )
        , mSLPlayerPlay         ( NULL )
        , mSLPlayerBufferQueue  ( NULL )
        , mSLPlayerVolume       ( NULL )
        , mSLPlaybackRate       ( NULL )
		, mEmitterState         ( NULL )
		, mVolume               ( 100.f )
		, mPitch                ( 100.f )
		, mLoopBegin            ( 0 )
		, mLoopLength           ( 0 )
		, mIsPlaying            ( false )
		, mIsPausing            ( false )
		, mIsLoop               ( false )
        , mPlayEnd              ( false )
	{
        LN_SAFE_ADDREF( mManager );
    }

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	OnMemory::~OnMemory()
	{
	    if ( mSLPlayerObject )
        {
            (*mSLPlayerObject)->Destroy( mSLPlayerObject );
            mSLPlayerObject = NULL;
        }
	    if ( mSLOutputMixObject )
        {
            (*mSLOutputMixObject)->Destroy( mSLOutputMixObject );
            mSLOutputMixObject = NULL;
        }
	    
        if ( mManager )
        {
            mManager->removeAudioPlayer( this );
            LN_SAFE_RELEASE( mManager );
        }
	}

	//---------------------------------------------------------------------
	// ● 初期化
	//---------------------------------------------------------------------
    LNRESULT OnMemory::initialize( IAudioSource* audio_source_, bool enable_3d_ )
	{
		mAudioSource = audio_source_;
		mAudioSource->addRef();

	    // オンメモリ再生用に内部に持ってるバッファを埋める
		mAudioSource->fillBufferAndReleaseStream();

	    SLresult result;
	    
	    //-----------------------------------------------------
        // OutputMix
        
        // create output mix, with environmental reverb specified as a non-required interface
        const SLInterfaceID mix_ids[1] = { SL_IID_VOLUME };//, SL_IID_RATEPITCH, SL_IID_DYNAMICINTERFACEMANAGEMENT };//SL_IID_ENVIRONMENTALREVERB };    // リバーブっぽい, SL_IID_PITCH
        const SLboolean mix_req[1]     = { SL_BOOLEAN_FALSE };//, SL_BOOLEAN_FALSE, SL_BOOLEAN_FALSE };//, SL_BOOLEAN_FALSE
        result = (*mManager->getSLEngineEngine())->CreateOutputMix( mManager->getSLEngineEngine(), &mSLOutputMixObject, 1, mix_ids, mix_req );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

        // realize the output mix
        result = (*mSLOutputMixObject)->Realize( mSLOutputMixObject, SL_BOOLEAN_FALSE );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
    
        //-----------------------------------------------------
        // Player
        
	    // waveフォーマット取得
	    const LNWaveFormat* ln_format = mAudioSource->getWaveFormat();
	    SLuint32 sr  = convertSamplingRateValue( ln_format->SamplesPerSec );
	    SLuint16 bps = convertBitsPerSampleValue( ln_format->BitsPerSample );
	    int speakers = ( ln_format->Channels == 1 ) ? ( SL_SPEAKER_FRONT_CENTER ) : ( SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT );
	    
	    // waveフォーマット作成
        SLDataFormat_PCM format_pcm = {
            SL_DATAFORMAT_PCM,              // この構造体がSLDataFormat_PCMであることを指定
            ln_format->Channels,            // チャンネル数
            sr,                             // サンプリングレート
            bps,                            // サンプルフォーマット
            bps,                            // サンプル格納フォーマット
            speakers,                       // チャンネルマスク
            SL_BYTEORDER_LITTLEENDIAN };    // エンディアン

	    // オーディオデータのソース設定を作成 (入力元の情報)
        // 種類はバッファーキュー
	    // バッファキューの個数は8個に設定 (あるところでは 2 個だったけど…)
        SLDataLocator_AndroidSimpleBufferQueue loc_bufq = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2 };
        SLDataSource audio_src = { &loc_bufq, &format_pcm };

	    // オーディオデータの出力先の情報を作成
        SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX, mSLOutputMixObject };
        SLDataSink audioSnk = { &loc_outmix, NULL };

	    // 要求するインターフェイス
        const SLInterfaceID ids[4] = { SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_PLAYBACKRATE, SL_IID_PLAY, SL_IID_VOLUME };//, SL_IID_SEEK  SL_IID_RATEPITCH, SL_IID_EFFECTSEND, SL_IID_VOLUME };
        const SLboolean req[4] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };//, SL_BOOLEAN_TRUE SL_BOOLEAN_TRUE };

	    // プレイヤーを作成
        result = (*mManager->getSLEngineEngine())->CreateAudioPlayer(
            mManager->getSLEngineEngine(),        /* エンジンインターフェイス */
            &mSLPlayerObject,    /* 新規に作るプレイヤ */
            &audio_src,             /* オーディオデータの入力元 */
            &audioSnk,              /* オーディオデータの出力先 */
            4,                      /* 要求するインターフェイスの数 */
            ids,                    /* 要求するインターフェイスの種類 */
            req );                  /* 要求するインターフェイスのためのフラグ */
	    LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

	    // プレイヤーのリソースを確保
        result = (*mSLPlayerObject)->Realize( mSLPlayerObject, SL_BOOLEAN_FALSE );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

	    // playインターフェイスを取得
        result = (*mSLPlayerObject)->GetInterface( mSLPlayerObject, SL_IID_PLAY, &mSLPlayerPlay );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

	    // BufferQueueインターフェイスを取得
        result = (*mSLPlayerObject)->GetInterface(
                mSLPlayerObject,
                SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
                &mSLPlayerBufferQueue );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
        
        // 音量変更インターフェイス
        result = (*mSLPlayerObject)->GetInterface( mSLPlayerObject, SL_IID_VOLUME, &mSLPlayerVolume );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

        // 再生速度変更インターフェイス
	    result = (*mSLPlayerObject)->GetInterface( mSLPlayerObject, SL_IID_PLAYBACKRATE, &mSLPlaybackRate );
	    LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

        // イベントコールバック設定
	    (*mSLPlayerPlay)->RegisterCallback( mSLPlayerPlay, _slPlayCallback, this );
        
        // SL_PLAYEVENT_HEADATEND イベント(再生中の音声が末尾に達した)を受け取るようにする
	    (*mSLPlayerPlay)->SetCallbackEventsMask( mSLPlayerPlay, SL_PLAYEVENT_HEADATEND );
    
        /*
	    SLPlaybackRateItf mPBack = NULL;
	    result = (*mSLPlayerObject)->GetInterface( mSLPlayerObject, SL_IID_PLAYBACKRATE, &mPBack );
	    _p(mPBack);
	    LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
	    
        
        SLpermille rate;
        result = (*mPBack)->SetRate( mPBack, 1200 );
	    LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
      
        SLpermille MinRate;
		SLpermille MaxRate;
		SLpermille StepSize;
		*/
    
    /*
        result = (*mPBack)->GetRateRange(mPBack, 0, &MinRate, &MaxRate, &StepSize, NULL);
    _p(MinRate);
	    _p(MaxRate);
        _p(StepSize);
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
    */

    
	    /*
	    SLRatePitchItf mSLPitch = NULL;
	    result = (*mSLPlayerObject)->GetInterface( mSLPlayerObject, SL_IID_RATEPITCH, &mSLPitch );
	    _p(mSLPitch);
	    LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
	    
	    SLpermille MinPitch;
		SLpermille MaxPitch;
	    (*mSLPitch)->GetRatePitchCapabilities( mSLPitch, &MinPitch, &MaxPitch );
	    LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
	    _p(MinPitch);
	    _p(MaxPitch);
	    
	    SLpermille Pitch;
	    (*mSLPitch)->GetRate( mSLPitch, &Pitch );
	    LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
	    _p(Pitch);
    */
	    /*
	    SLPitchItf mSLPitch = NULL;
	    result = (*mSLPlayerObject)->GetInterface( mSLPlayerObject, SL_IID_PITCH, &mSLPitch );
	    _p(mSLPitch);
	    LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
	    
	    SLpermille MinPitch;
		SLpermille MaxPitch;
	    (*mSLPitch)->GetPitchCapabilities( mSLPitch, &MinPitch, &MaxPitch );
	    LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
	    _p(MinPitch);
	    _p(MaxPitch);
	    
	    SLpermille Pitch;
	    (*mSLPitch)->GetPitch( mSLPitch, &Pitch );
	    LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
	    _p(Pitch);
	    */
	    
	    


	    
        // ソースからループ位置を取得して設定する
        u32 loop_begin, loop_length;
        mAudioSource->getLoopState( &loop_begin, &loop_length );
        mLoopBegin  = loop_begin;
		mLoopLength = loop_length;

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● 音量の取得
	//---------------------------------------------------------------------
	int OnMemory::getVolume() const
	{
		return static_cast< int >( mVolume );
	}

	//---------------------------------------------------------------------
	// ● 音量の設定
	//---------------------------------------------------------------------
	LNRESULT OnMemory::setVolume( int volume_ )
	{
        mVolume = static_cast< float >( volume_ );
		if ( mVolume < 0 ) { 
			mVolume = 0; }
		if ( mVolume > 100 ) { 
			mVolume = 100; }
		float v = mVolume * 0.01f;

        SLresult result = (*mSLPlayerVolume)->SetVolumeLevel( mSLPlayerVolume, convertVolume2Milibell( v ) );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● ピッチの取得
	//---------------------------------------------------------------------
	int OnMemory::getPitch() const
	{
		return static_cast< int >( mPitch );
	}

	//---------------------------------------------------------------------
	// ● ピッチの設定
	//---------------------------------------------------------------------
	LNRESULT OnMemory::setPitch( int pitch_ )
	{
		mPitch = static_cast< float >( pitch_ );
		if ( mPitch < 50 ) { 
			mPitch = 50; }
		if ( mPitch > 200 ) { 
			mPitch = 200; }
		float p = mPitch * 10;
	    
        SLresult result = (*mSLPlaybackRate)->SetRate( mSLPlaybackRate, p );
	    LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● 再生中判定
	//---------------------------------------------------------------------
	bool OnMemory::isPlaying() const
	{
		return mIsPlaying;
	}

	//---------------------------------------------------------------------
	// ● 一時停止中判定
	//---------------------------------------------------------------------
	bool OnMemory::isPausing() const
	{
		return mIsPausing;
	}

    //---------------------------------------------------------------------
	// ● 再生したサンプル数の取得
    //---------------------------------------------------------------------
    lnU64 OnMemory::getPlayedSamples() const
    {
        SLmillisecond ms;
	    (*mSLPlayerPlay)->GetPosition( mSLPlayerPlay, &ms );
        
        return mAudioSource->getWaveFormat()->SamplesPerSec * ms / 1000;
    }

	//---------------------------------------------------------------------
	// ● ループ情報の設定
	//---------------------------------------------------------------------
	LNRESULT OnMemory::setLoopState( u32 loop_begin_, u32 loop_length_ )
	{
        if ( loop_begin_ == 0 && loop_length_ == 0 )
        {
            // ソースからループ位置を取得して設定する
            u32 loop_begin, loop_length;
            mAudioSource->getLoopState( &loop_begin, &loop_length );
            mLoopBegin  = loop_begin;
		    mLoopLength = loop_length;
        }
        else
        {
            mLoopBegin = loop_begin_;
		    mLoopLength = loop_length_;
        }

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● 再生
	//---------------------------------------------------------------------
	LNRESULT OnMemory::play()
	{
	    // とりあえず停止
	    stop();
	    
		// 再生状態を再生に設定
        SLresult result = (*mSLPlayerPlay)->SetPlayState( mSLPlayerPlay, SL_PLAYSTATE_PLAYING );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
	    
	    // バッファキューをプレイヤーへ登録する
        result = (*mSLPlayerBufferQueue)->Enqueue(
            mSLPlayerBufferQueue,
            mAudioSource->getOnmemoryPCMBuffer(),
            mAudioSource->getOnmemoryPCMBufferSize() );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
	    


		mIsPlaying = true;
		mIsPausing = false;
	    mPlayEnd = false;

		polling();	// 3D オーディオの計算を行うため

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● 停止
	//---------------------------------------------------------------------
	LNRESULT OnMemory::stop()
	{
	    SLresult result = (*mSLPlayerPlay)->SetPlayState( mSLPlayerPlay, SL_PLAYSTATE_STOPPED );
	    
	    (*mSLPlayerBufferQueue)->Clear( mSLPlayerBufferQueue );

		mIsPlaying = false;
		mIsPausing = false;
	    
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● 一時停止
	//---------------------------------------------------------------------
	LNRESULT OnMemory::pause( bool is_pause_ )
	{
	    // 再生中の場合
		if ( mIsPlaying )
		{
			// 一時停止する場合で、現在一時停止していない場合
			if ( is_pause_ && !mIsPausing )
			{
			    (*mSLPlayerPlay)->SetPlayState( mSLPlayerPlay, SL_PLAYSTATE_PAUSED );
				mIsPausing = true;
			}
			// 一時停止を解除する場合で、現在一時停止している場合
			else if ( !is_pause_ && mIsPausing )
			{
				SLresult result = (*mSLPlayerPlay)->SetPlayState( mSLPlayerPlay, SL_PLAYSTATE_PLAYING );
				mIsPausing = false;
			}
		}
	    
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● ポーリング更新
	//---------------------------------------------------------------------
	bool OnMemory::polling()
	{
		// 再生中ではない場合は中断
		if ( !mIsPlaying || mIsPausing )
		{
			return false;
		}

		// ループ再生ではない場合
		if ( !mIsLoop )
		{
		    if ( mPlayEnd )
		    {
		        stop();
		        return false;
		    }
		}
        return true;
    }

    //---------------------------------------------------------------------
	// ● 3D 音源としての位置を設定する
    //---------------------------------------------------------------------
    LNRESULT OnMemory::setPosition( const LVector3& pos_ )
    {
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● Play イベント処理
    //---------------------------------------------------------------------
    void OnMemory::_slPlayCallback( SLPlayItf caller_, void* context_, SLuint32 event_ )
    {
        OnMemory* player = (OnMemory*)context_;
        if ( event_ == SL_PLAYEVENT_HEADATEND )
        {
            if ( player->mIsLoop )
            {
                (*player->mSLPlayerBufferQueue)->Enqueue(
                    player->mSLPlayerBufferQueue,
                    player->mAudioSource->getOnmemoryPCMBuffer(),
                    player->mAudioSource->getOnmemoryPCMBufferSize() );
            }
            else
            {
                player->mPlayEnd = true;
            }
        }
    }


//=============================================================================
// ■ Streaming クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	Streaming::Streaming( AudioDevice* manager_ )
		: mManager              ( manager_ )
	    , mSLOutputMixObject    ( NULL )
        , mSLPlayerObject       ( NULL )
        , mSLPlayerPlay         ( NULL )
        , mSLPlayerBufferQueue  ( NULL )
	    , mSLPlayerVolume       ( NULL )
	    , mSLPlaybackRate       ( NULL )
        , mAudioSource          ( NULL )
		, mVolume               ( 100.f )
		, mPitch                ( 100.f )
		, mLoopBegin            ( 0 )
		, mLoopLength           ( 0 )
		, mPrimaryAudioData     ( NULL )
		, mSecondaryAudioData   ( NULL )
		, mAudioDataBufferSize  ( 0 )
		, mReadCursor           ( 0 )
        , mLoopBeginBlockOffset ( 0 )
        , mCursorBeginPos       ( 0 )
        , mCursorEndPos         ( 0 )
        , mWriteSampleNum       ( 0 )
		, mIsPlaying            ( false )
		, mIsPausing            ( false )
		, mIsLoop               ( false )
		, mEOF                  ( false )
	    , mPlayEnd              ( false )
	{
        LN_SAFE_ADDREF( mManager );
	}

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	Streaming::~Streaming()
	{
	    if ( mSLPlayerObject )
        {
            (*mSLPlayerObject)->Destroy( mSLPlayerObject );
            mSLPlayerObject = NULL;
        }
	    if ( mSLOutputMixObject )
        {
            (*mSLOutputMixObject)->Destroy( mSLOutputMixObject );
            mSLOutputMixObject = NULL;
        }

		LN_SAFE_RELEASE( mAudioSource );

		SAFE_DELETE_ARRAY( mPrimaryAudioData );
		SAFE_DELETE_ARRAY( mSecondaryAudioData );

        if ( mManager )
        {
            mManager->removeAudioPlayer( this );
            LN_SAFE_RELEASE( mManager );
        }
	}

	//---------------------------------------------------------------------
	// ● 初期化
	//---------------------------------------------------------------------
    LNRESULT Streaming::initialize( IAudioSource* audio_source_, bool enable_3d_)
	{
		mAudioSource = audio_source_;
		mAudioSource->addRef();
	    
	    SLresult result;
	    
	    //-----------------------------------------------------
        // OutputMix
        
        // create output mix, with environmental reverb specified as a non-required interface
        const SLInterfaceID mix_ids[1] = { SL_IID_VOLUME };//, SL_IID_RATEPITCH, SL_IID_DYNAMICINTERFACEMANAGEMENT };//SL_IID_ENVIRONMENTALREVERB };    // リバーブっぽい, SL_IID_PITCH
        const SLboolean mix_req[1]     = { SL_BOOLEAN_FALSE };//, SL_BOOLEAN_FALSE, SL_BOOLEAN_FALSE };//, SL_BOOLEAN_FALSE
        result = (*mManager->getSLEngineEngine())->CreateOutputMix( mManager->getSLEngineEngine(), &mSLOutputMixObject, 1, mix_ids, mix_req );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

        // realize the output mix
        result = (*mSLOutputMixObject)->Realize( mSLOutputMixObject, SL_BOOLEAN_FALSE );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
    
        //-----------------------------------------------------
        // Player
        
	    // waveフォーマット取得
	    const LNWaveFormat* ln_format = mAudioSource->getWaveFormat();
	    SLuint32 sr  = convertSamplingRateValue( ln_format->SamplesPerSec );
	    SLuint16 bps = convertBitsPerSampleValue( ln_format->BitsPerSample );
	    int speakers = ( ln_format->Channels == 1 ) ? ( SL_SPEAKER_FRONT_CENTER ) : ( SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT );
	    
	    // waveフォーマット作成
        SLDataFormat_PCM format_pcm = {
            SL_DATAFORMAT_PCM,              // この構造体がSLDataFormat_PCMであることを指定
            ln_format->Channels,            // チャンネル数
            sr,                             // サンプリングレート
            bps,                            // サンプルフォーマット
            bps,                            // サンプル格納フォーマット
            speakers,                       // チャンネルマスク
            SL_BYTEORDER_LITTLEENDIAN };    // エンディアン

	    // オーディオデータのソース設定を作成 (入力元の情報)
        // 種類はバッファーキュー
	    // バッファキューの個数は8個に設定 (あるところでは 2 個だったけど…)
        SLDataLocator_AndroidSimpleBufferQueue loc_bufq = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 4 };
        SLDataSource audio_src = { &loc_bufq, &format_pcm };

	    // オーディオデータの出力先の情報を作成
        SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX, mSLOutputMixObject };
        SLDataSink audioSnk = { &loc_outmix, NULL };

	    // 要求するインターフェイス
        const SLInterfaceID ids[4] = { SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_PLAYBACKRATE, SL_IID_PLAY, SL_IID_VOLUME };//, SL_IID_SEEK SL_IID_RATEPITCH, SL_IID_EFFECTSEND, SL_IID_VOLUME };
        const SLboolean req[4] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };//SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };

	    // プレイヤーを作成
        result = (*mManager->getSLEngineEngine())->CreateAudioPlayer(
            mManager->getSLEngineEngine(),        /* エンジンインターフェイス */
            &mSLPlayerObject,    /* 新規に作るプレイヤ */
            &audio_src,             /* オーディオデータの入力元 */
            &audioSnk,              /* オーディオデータの出力先 */
            4,                      /* 要求するインターフェイスの数 */
            ids,                    /* 要求するインターフェイスの種類 */
            req );                  /* 要求するインターフェイスのためのフラグ */
	    LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

	    // プレイヤーのリソースを確保
        result = (*mSLPlayerObject)->Realize( mSLPlayerObject, SL_BOOLEAN_FALSE );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

	    // playインターフェイスを取得
        result = (*mSLPlayerObject)->GetInterface( mSLPlayerObject, SL_IID_PLAY, &mSLPlayerPlay );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

        // イベントコールバック設定
        (*mSLPlayerPlay)->RegisterCallback( mSLPlayerPlay, _slPlayCallback, this );
        
        // SL_PLAYEVENT_HEADATEND イベントを受け取るようにする
	    (*mSLPlayerPlay)->SetCallbackEventsMask( mSLPlayerPlay, SL_PLAYEVENT_HEADATEND );

        // BufferQueueインターフェイスを取得
        result = (*mSLPlayerObject)->GetInterface(
                mSLPlayerObject,
                SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
                &mSLPlayerBufferQueue );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
        
        // 音量変更インターフェイス
        result = (*mSLPlayerObject)->GetInterface( mSLPlayerObject, SL_IID_VOLUME, &mSLPlayerVolume );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

        // 再生速度変更インターフェイス
	    result = (*mSLPlayerObject)->GetInterface( mSLPlayerObject, SL_IID_PLAYBACKRATE, &mSLPlaybackRate );
	    LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
	    
        //-----------------------------------------------------
        // Memory Buffer

		// 1 秒分のバッファサイズ取得
		mAudioDataBufferSize = mAudioSource->getBytesPerSec();

		// 1 秒のバッファをふたつ用意
		mPrimaryAudioData	= LN_NEW u8[ mAudioDataBufferSize ];
		mSecondaryAudioData = LN_NEW u8[ mAudioDataBufferSize ];

        // ソースからループ位置を取得して設定する
        u32 loop_begin, loop_length;
        mAudioSource->getLoopState( &loop_begin, &loop_length );
        mLoopBegin  = loop_begin;
		mLoopLength = loop_length;

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● 音量の取得
	//---------------------------------------------------------------------
	int Streaming::getVolume() const
	{
		return static_cast< int >( mVolume );
	}

	//---------------------------------------------------------------------
	// ● 音量の設定
	//---------------------------------------------------------------------
	LNRESULT Streaming::setVolume( int volume_ )
	{
        mVolume = static_cast< float >( volume_ );
		if ( mVolume < 0 ) { 
			mVolume = 0; }
		if ( mVolume > 100 ) { 
			mVolume = 100; }
		float v = mVolume * 0.01f;

        SLresult result = (*mSLPlayerVolume)->SetVolumeLevel( mSLPlayerVolume, convertVolume2Milibell( v ) );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● ピッチの取得
	//---------------------------------------------------------------------
	int Streaming::getPitch() const
	{
		return static_cast< int >( mPitch );
	}

	//---------------------------------------------------------------------
	// ● ピッチの設定
	//---------------------------------------------------------------------
	LNRESULT Streaming::setPitch( int pitch_ )
	{
		mPitch = static_cast< float >( pitch_ );
		if ( mPitch < 50 ) { 
			mPitch = 50; }
		if ( mPitch > 200 ) { 
			mPitch = 200; }
		float p = mPitch * 10;
	    
        SLresult result = (*mSLPlaybackRate)->SetRate( mSLPlaybackRate, p );
	    LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 再生したサンプル数の取得
    //---------------------------------------------------------------------
    lnU64 Streaming::getPlayedSamples() const
    {
	    SLmillisecond ms;
	    (*mSLPlayerPlay)->GetPosition( mSLPlayerPlay, &ms );
        
        return mAudioSource->getWaveFormat()->SamplesPerSec * ms / 1000;
    }

	//---------------------------------------------------------------------
	// ● 再生中かを判定する
	//---------------------------------------------------------------------
	bool Streaming::isPlaying() const
	{
		return mIsPlaying;
	}

	//---------------------------------------------------------------------
	// ● 一時停止中かを判定する
	//---------------------------------------------------------------------
	bool Streaming::isPausing() const
	{
		return mIsPausing;
	}

	//---------------------------------------------------------------------
	// ● ループ情報の設定
	//---------------------------------------------------------------------
	LNRESULT Streaming::setLoopState( u32 loop_begin_, u32 loop_length_ )
	{
        if ( loop_begin_ == 0 && loop_length_ == 0 )
        {
            // ソースからループ位置を取得して設定する
            u32 loop_begin, loop_length;
            mAudioSource->getLoopState( &loop_begin, &loop_length );
            mLoopBegin  = loop_begin;
		    mLoopLength = loop_length;
        }
        else
        {
            mLoopBegin = loop_begin_;
		    mLoopLength = loop_length_;
        }
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● 再生
	//---------------------------------------------------------------------
	LNRESULT Streaming::play()
	{
	    // とりあえず停止
	    stop();

		// デコード状態をリセットする ( MP3 用 )
		mAudioSource->reset();

		// ストリーミング読み込み位置リセット
		mReadCursor = 0;

        // とりあえずソースデータをカーソルの終端にする。
        // ファイル内から読むのは _addNextBuffer() で
        mCursorEndPos = mAudioSource->getSourceDataSize();
        
        mWriteSampleNum = 0;
        mCursorBeginPos = 0;
        mLoopBeginBlockOffset = 0;

		// 最初のデータをキューに追加する
	    
		_addNextBuffer();

	    // 再生状態を再生に設定
        SLresult result = (*mSLPlayerPlay)->SetPlayState( mSLPlayerPlay, SL_PLAYSTATE_PLAYING );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );

		mIsPlaying = true;
		mIsPausing = false;

		polling();	// 3D オーディオの計算を行うため

        return LN_OK;

	}

	//---------------------------------------------------------------------
	// ● 停止
	//---------------------------------------------------------------------
	LNRESULT Streaming::stop()
	{	    
	    SLresult result = (*mSLPlayerPlay)->SetPlayState( mSLPlayerPlay, SL_PLAYSTATE_STOPPED );
	    
	    (*mSLPlayerBufferQueue)->Clear( mSLPlayerBufferQueue );
	    
		mIsPlaying = false;
		mIsPausing = false;

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● 一時停止
	//---------------------------------------------------------------------
	LNRESULT Streaming::pause( bool is_pause_ )
	{
		// 再生中の場合
		if ( mIsPlaying )
		{
			// 一時停止する場合で、現在一時停止していない場合
			if ( is_pause_ && !mIsPausing )
			{
			    (*mSLPlayerPlay)->SetPlayState( mSLPlayerPlay, SL_PLAYSTATE_PAUSED );
				mIsPausing = true;
			}
			// 一時停止を解除する場合で、現在一時停止している場合
			else if ( !is_pause_ && mIsPausing )
			{
				SLresult result = (*mSLPlayerPlay)->SetPlayState( mSLPlayerPlay, SL_PLAYSTATE_PLAYING );
				mIsPausing = false;
			}
		}

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● ポーリング更新
	//---------------------------------------------------------------------
	bool Streaming::polling()
	{	    
		// 再生中ではない場合は中断
		if ( !mIsPlaying || mIsPausing )
		{
			return false;
		}

	    // state.count がキュー内のバッファの数
	    // state.index はバッファを追加するたびにインクリメントされていく
	    SLAndroidSimpleBufferQueueState state;
        (*mSLPlayerBufferQueue)->GetState(mSLPlayerBufferQueue, &state);

		if ( !mEOF && state.count < 2 )
		{
			// 読み込みカーソルが終端を超えている場合
			if ( mReadCursor >= mCursorEndPos )
			{
				// ループ再生の場合、次にキューに追加するデータはループ領域の先頭であるため、読み込みカーソル位置を先頭に戻す
				if ( mIsLoop )
				{
					mReadCursor = mCursorBeginPos;
                    mCursorEndPos = mAudioSource->getSourceDataSize();
				}
				// ループ再生しない場合はファイルの最後まで読み切ったことを示すフラグを ON
				else
				{
					mEOF = true;
				}
			}

			// ファイルの最後まで読み切っていない場合
			if ( !mEOF )
			{
				// 次のデータを再生キューに追加
				_addNextBuffer();
			}
		}

		// 非ループ再生で、ファイルの最後まで読み切っている場合 ( 再生終了している場合 )
		if ( !mIsLoop && mEOF )
		{
		    if ( mPlayEnd )//( voice_state.BuffersQueued == 0 )
			{
				stop();
				return false;
			}
		}

		return true;
	}

    //---------------------------------------------------------------------
	// ● 3D 音源としての位置を設定する
    //---------------------------------------------------------------------
    LNRESULT Streaming::setPosition( const LVector3& pos_ )
    {
        return LN_OK;
    }

	//---------------------------------------------------------------------
	// ● セカンダリに新しいデータを書き込んで、再生キューに追加する
	//---------------------------------------------------------------------
	LNRESULT Streaming::_addNextBuffer()
	{
	    SLresult result;
	    
		// 読み込み位置に移動
		mAudioSource->seek( mReadCursor );


		// データ読み込み
		u32 read_size;		// 呼んだサイズ
		u32 write_size;		// デコードして書き込んだサイズ
		mAudioSource->read( mSecondaryAudioData, mAudioDataBufferSize, &read_size, &write_size );

		// 実際に元データから読んだデータ分、カーソルを進める
		mReadCursor += read_size;


        // 書きこんだサイズを加算
        mWriteSampleNum += write_size;

        u8* data_buffer = mSecondaryAudioData;

        // ループ上で扱うサンプル数は、チャンネル数関係無しになってる。
        // それを正しい値に変換するため、実際の 1 サンプルあたりのバイト数を計算する
        u32 smp_size =
            mAudioSource->getWaveFormat()->Channels *
            mAudioSource->getWaveFormat()->BitsPerSample / 8;

        // 読んだデータ内にループ領域の開始位置が含まれている場合
        if ( mCursorBeginPos > 0 && mReadCursor - read_size == mCursorBeginPos )
        {
            data_buffer = ( data_buffer + mLoopBeginBlockOffset );
            write_size -= mLoopBeginBlockOffset; 
        }
        // ループ領域の開始位置が未設定で、ループ領域の開始サンプルを超えた場合
        else if ( mLoopBegin > 0 && mCursorBeginPos == 0 && mWriteSampleNum > ( mLoopBegin * smp_size ) )
        {
            // カーソルはブロック単位。直前の read() のひとつ前を指す
            mCursorBeginPos = mReadCursor - read_size;

            // ブロック先頭からのサンプル数
            mLoopBeginBlockOffset = write_size - ( mWriteSampleNum - ( mLoopBegin * smp_size ) );
        }

        //printf( "%d %d %d\n", mLoopBegin, mLoopLength, ( mLoopBegin + mLoopLength ) );
        //printf( "b:%d l:%d n:%d a:%d\n", mLoopBegin, mLoopLength, mWriteSampleNum, ( mLoopBegin + mLoopLength ) * smp_size );

        // 終端サンプル数を超えた場合、そこが終端
        if ( mLoopLength > 0 && mWriteSampleNum > ( mLoopBegin + mLoopLength ) * smp_size )
        {
            // 終端。polling() でも検出する
            mCursorEndPos = mReadCursor;

            write_size -= ( mWriteSampleNum - ( mLoopBegin + mLoopLength ) * smp_size );
          
            mWriteSampleNum = mCursorBeginPos;
        }
        
	    
	    
        // バッファキューをプレイヤーへ登録する
        result = (*mSLPlayerBufferQueue)->Enqueue(
            mSLPlayerBufferQueue,
            data_buffer,
            write_size );
        LN_THROW_SystemCall( result == SL_RESULT_SUCCESS, "result %x", result );
        

		// primary と secondary の入れ替え
		u8* temp = mPrimaryAudioData;
		mPrimaryAudioData = mSecondaryAudioData;
		mSecondaryAudioData = temp;

        return LN_OK;
	}
	
	//---------------------------------------------------------------------
	// ● Play イベント処理
    //---------------------------------------------------------------------
    void Streaming::_slPlayCallback( SLPlayItf caller_, void* context_, SLuint32 event_ )
    {
        Streaming* player = reinterpret_cast< Streaming* >( context_ );
        if ( event_ == SL_PLAYEVENT_HEADATEND )
        {
            
            player->mPlayEnd = true;
        }
    }


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