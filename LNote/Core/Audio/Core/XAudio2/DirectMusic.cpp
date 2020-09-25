//=============================================================================
//【 DirectMusic 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <algorithm>
#include <cguid.h>
#include "DirectMusic.h"

#define LN_OUT_LOG  // 念のためログを出しておく

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
// ■ DirectMusicSegment クラス
//=============================================================================

#define LN_DMUS_TYPE DMUS_APATH_DYNAMIC_STEREO  // DMUS_APATH_SHARED_STEREOPLUSREVERB にするとデフォでリバーブ付くけど、ピッチ変更ができなくなるみたい

    //---------------------------------------------------------------------
    // ● コンストラクタ
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
            // DirectSound のバッファを取得するため、オーディオパスを取得する
		    hr = mDMPerformance->CreateStandardAudioPath(
			     LN_DMUS_TYPE,
			     64, FALSE, &mDMAudioPath );
            
            // DirectSound バッファの取得
		    hr = mDMAudioPath->GetObjectInPath( 
                DMUS_PCHANNEL_ALL,
                DMUS_PATH_BUFFER,
                0, GUID_NULL, 0,
                IID_IDirectSoundBuffer8, 
                (void**)&mDSSoundBuffer );


            // バッファにエフェクトを設定する
		    //		GUID_DSFX_STANDARD_CHORUS
		    //		GUID_DSFX_STANDARD_COMPRESSOR
		    //		GUID_DSFX_STANDARD_DISTORTION
		    //		GUID_DSFX_STANDARD_ECHO エコーありすぎ
		    //		GUID_DSFX_STANDARD_FLANGER
		    //		GUID_DSFX_STANDARD_GARGLE ぶるぶる
		    //		GUID_DSFX_STANDARD_I3DL2REVERB 音ちいさい
		    //		GUID_DSFX_STANDARD_PARAMEQ
		    //		GUID_DSFX_WAVES_REVERB
		    DSEFFECTDESC ds_desc;
		    ZeroMemory( &ds_desc, sizeof( DSEFFECTDESC ) );
		    ds_desc.dwSize        = sizeof( DSEFFECTDESC );
		    ds_desc.dwFlags       = 0;//DSFX_LOCSOFTWARE;
		    ds_desc.guidDSFXClass = GUID_DSFX_WAVES_REVERB;  // エフェクト識別子 ( ウェーブリバーブ )
		    ds_desc.dwReserved1   = 0;	// 予約済み
		    ds_desc.dwReserved2   = 0;	// 予約済み
		    hr = mDSSoundBuffer->SetFX( 1, &ds_desc, NULL );
		    

            // リバーブのパラメータを設定するためのインターフェースを取得する
		    // ( 普通は必要ないけど、できるだけ RGSS に近付けたいので )
		    IDirectSoundFXWavesReverb8* waves_reverb = NULL;
		    hr = mDSSoundBuffer->GetObjectInPath( GUID_All_Objects, 0, IID_IDirectSoundFXWavesReverb8, (void**)&waves_reverb );
		    
		    // パラメータを設定して バッファにセット
		    DSFXWavesReverb rv;
		    rv.fInGain			= 0.f;//DSFX_WAVESREVERB_INGAIN_MIN ;//DSFX_WAVESREVERB_INGAIN_DEFAULT;
		    rv.fReverbMix		= 0.f;//DSFX_WAVESREVERB_REVERBMIX_MIN  DSFX_WAVESREVERB_REVERBMIX_DEFAULT;
		    rv.fReverbTime		= 600.f;//DSFX_WAVESREVERB_REVERBTIME_DEFAULT;DSFX_WAVESREVERB_REVERBTIME_MIN 
		    rv.fHighFreqRTRatio = DSFX_WAVESREVERB_HIGHFREQRTRATIO_DEFAULT;//DSFX_WAVESREVERB_HIGHFREQRTRATIO_MIN; //DSFX_WAVESREVERB_HIGHFREQRTRATIO_DEFAULT;DSFX_WAVESREVERB_HIGHFREQRTRATIO_MAX
		    hr = waves_reverb->SetAllParameters( &rv );
		    
            


            IDirectMusicLoader8* loader = NULL;

		    //ローダーの作成
		    HRESULT hr = CoCreateInstance( CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, IID_IDirectMusicLoader8, (void**)&loader );
    		
		    // キャッシュ無効 ( 有効にする場合、http://www.nomuraz.com/denpa/prog001.htm#IDPROG0009 のバグに注意 )
		    hr = loader->EnableCache( GUID_DirectMusicAllTypes, FALSE );
		    //hr = loader->EnableCache( CLSID_DirectMusicSegment, FALSE );

		    // セグメント作成のための情報を設定する
		    DMUS_OBJECTDESC desc;
		    ZeroMemory( &desc, sizeof( DMUS_OBJECTDESC ) );
		    desc.dwSize      = sizeof( DMUS_OBJECTDESC );
		    desc.dwValidData = DMUS_OBJ_MEMORY | DMUS_OBJ_CLASS;
		    desc.guidClass   = CLSID_DirectMusicSegment;
		    desc.llMemLength = (LONGLONG)audio_source_->getSourceDataSize();	// バッファのサイズ
		    desc.pbMemData   = (LPBYTE)audio_source_->getSourceData();	    // データの入っているバッファ

		    // セグメント作成
		    hr = loader->GetObject( &desc, IID_IDirectMusicSegment8, (void**)&mDMSegment );

            // SetParam で GUID_StandardMIDIFile を設定する前に呼ばないと
            // 第 1 トラックが再生されなくなる
            hr = mDMSegment->Download( mDMPerformance );

		    // SMF として再生することを伝える
		    hr = mDMSegment->SetParam( GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL );
    		
		    //loader->ClearCache( CLSID_DirectMusicSegment );

		    

            
		    //LN_DXASSERT_GOTO_S( hr, EXIT, "< DirectMusicSegment::onCreatePerformance >" );

            // ローダーはもう使わないので解放
		    SAFE_RELEASE( loader );
        }
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
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
	// ● 音量の設定 ( 100.0 ～ 0.0 )
    //---------------------------------------------------------------------
    void DirectMusicSegment::setVolume( float volume_ )
    {
		// 10db ～ -100db が実用範囲。渡す値は 1000 ～ -10000
		// デフォルトは 1db ( 100 ) で、これを超えると大体音割れする。(リバーブエフェクト)
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
	// ● ピッチの設定
    //---------------------------------------------------------------------
    void DirectMusicSegment::setPitch( float pitch_ )
    {
		DWORD p = 22050 * static_cast< DWORD >( pitch_ ) / 100;	
		HRESULT hr = mDSSoundBuffer->SetFrequency( p );
    }

    //---------------------------------------------------------------------
	// ● 全体の再生時間の取得 ( ミュージックタイム単位 )
    //---------------------------------------------------------------------
    u32 DirectMusicSegment::getTotalTime() const
    {
        MUSIC_TIME segment_length = 0;
		mDMSegment->GetLength( &segment_length );
        return segment_length;
    }

    //---------------------------------------------------------------------
	// ● 再生位置の取得 ( ミュージックタイム単位 )
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
	// ● 再生位置の設定 ( ミュージックタイム単位 )
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
	// ● ループ情報の設定
	//---------------------------------------------------------------------
	void DirectMusicSegment::setLoopState( bool is_loop_, u32 begin_, u32 length_ )
	{
        HRESULT hr;

		// ループ再生する場合
		if ( is_loop_ )
		{
			if ( length_ <= 0 )
			{
				// セグメント全体の長さ取得
				MUSIC_TIME time_length;
				mDMSegment->GetLength( &time_length );
				length_ = time_length;

                hr = mDMSegment->SetLoopPoints( begin_, length_ );
			}
            else
            {
			    mDMSegment->SetLoopPoints( begin_, begin_ + length_ );
            }
				
			// 無限ループ
			mDMSegment->SetRepeats( DMUS_SEG_REPEAT_INFINITE );
		}
		// ループ再生しない場合
		else
		{
            mDMSegment->SetLoopPoints( 0, 0 );
			mDMSegment->SetRepeats( 0 );
		}
	}

    //---------------------------------------------------------------------
    // ● 再生
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
      

        // 非同期で初期化してた場合、ボリュームなどは、再生開始直前に設定する必要がある
    }

    //---------------------------------------------------------------------
    // ● 停止
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
// ■ DirectMusicManager クラス
//=============================================================================

    // PlayerObject クラスのデストラクタ
    DirectMusicManager::PlayerObject::~PlayerObject()
    {
        DirectMusicManager::getInstance()->removePlayRequest( this );
    }


    DirectMusicManager* DirectMusicManager::sInstance = NULL;

    //---------------------------------------------------------------------
    // ● 初期化
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
	// ● 解放
	//---------------------------------------------------------------------
    void DirectMusicManager::finalize()
    {
        SAFE_DELETE( sInstance );
    }

    //---------------------------------------------------------------------
    // ● コンストラクタ
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
        , mErrorState           ( 1 )       // 最初はエラーにしておく。正常に初期化できたら 0
        , mLogFile              ( NULL )
    {
    	}

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    DirectMusicManager::~DirectMusicManager()
    {
        LN_LOG_FIN_BEGIN;
        // スレッドの終了を待って解放
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
	// ● エラー状態の取得
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
	// ● 再生要求の追加
	//---------------------------------------------------------------------
	void DirectMusicManager::addPlayRequest( PlayerObject* obj_ )
    {
        mListLock->lock();

        // 再生要求リストに追加されていない場合は追加しておく
        PlayRequestList::iterator pos;
        pos = std::find( mPlayRequestList.begin(), mPlayRequestList.end(), obj_ );
        if ( pos == mPlayRequestList.end() )
        {
            mPlayRequestList.push_back( obj_ );
        }

        mListLock->unlock();
    }

    //---------------------------------------------------------------------
	// ● 再生要求の削除
	//---------------------------------------------------------------------
	void DirectMusicManager::removePlayRequest( PlayerObject* obj_ )
    {
        mListLock->lock();
        mPlayRequestList.remove( obj_ );
        mListLock->unlock();
    }

    //---------------------------------------------------------------------
	// ● IDirectMusicPerformance8 の作成
	//---------------------------------------------------------------------
    IDirectMusicPerformance8* DirectMusicManager::createDMPerformance()
    {
        // 一応初期化済みかをチェック
        if ( !isInitialized() ) { 
	return NULL; }

        mLock->lock();
        IDirectMusicPerformance8* performance = mFirstPerformance;
        mLock->unlock();

        // 一番最初に初期化したものがある場合はそれを返す
        if ( performance )
        {
            mLock->lock();
            mFirstPerformance = NULL;
            mLock->unlock();
            return performance;
        }

        // パフォーマンスの作成
        HRESULT hr = CoCreateInstance(
		    CLSID_DirectMusicPerformance,
		    NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8,
		    (void**)&performance );
        if ( FAILED( hr ) ) return NULL;

        // パフォーマンスの初期化
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
	// ● 再生要求リストをチェックして、可能なら再生する
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
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT DirectMusicManager::_initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        HRESULT hr;

        mWindowHandle = init_data_.WindowHandle;
        mInitMode     = init_data_.DMInitMode;

        // COM 初期化
        //HRESULT hr = ::CoInitializeEx( NULL, COINIT_MULTITHREADED );
        //LN_SET_DXERR_R_S( hr, "< DirectMusicManager::_initialize >\nCoInitializeEx" );

        // DirectSound の作成
        LN_DXCALL_R(
            ::CoCreateInstance(
				    CLSID_DirectSound8,
				    NULL, 
				    CLSCTX_ALL,
				    IID_IDirectSound8,
				    (void**)&mDirectSound ) );

        // DirectSound の初期化
        mDirectSound->Initialize( NULL );


	    // 協調レベルの設定
	    LN_DXCALL_R(mDirectSound->SetCooperativeLevel( mWindowHandle, DSSCL_PRIORITY ) );

	    // DirectMusic 作成
	    LN_DXCALL_R(CoCreateInstance( CLSID_DirectMusic, NULL, CLSCTX_INPROC, IID_IDirectMusic, (void**)&mDirectMusic ) );

	    // 作成した DirectMusic に DirectSound を設定
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
    // ● ひとつめのパフォーマンスの初期化
    //---------------------------------------------------------------------
	void DirectMusicManager::_initPerformance()
    {
#ifdef LN_OUT_LOG
        LN_LOG_WRITE( "IDirectMusicPerformance8 の初期化を開始します..." );
#endif

        IDirectMusicPerformance8* performance = NULL;
        u32 error = 0;
        
        // パフォーマンスの作成
        HRESULT hr = CoCreateInstance(
		    CLSID_DirectMusicPerformance,
		    NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8,
		    (void**)&performance );
        if ( FAILED( hr ) ) error = 1; 

        if ( SUCCEEDED( hr ) )
        {
            // パフォーマンスの初期化
		    //hr = mPerformance->Init( (IDirectMusic**)&direct_music, (IDirectSound*)direct_sound, LDirectMusic::instance()->windowHandle() );
		    hr = performance->InitAudio(
			    (IDirectMusic**)&mDirectMusic, (IDirectSound**)&mDirectSound,
			    mWindowHandle,
			    LN_DMUS_TYPE,
			    64, DMUS_AUDIOF_ALL, NULL );
		    if ( FAILED( hr ) ) error = 2;
        }

#ifdef LN_OUT_LOG
        LN_LOG_WRITE( "IDirectMusicPerformance8 の初期化後の書き込みの為、ロックします。" );
#endif

        mLock->lock();
        mFirstPerformance = performance;
        mErrorState = error;
        mLock->unlock();

        // 初期化完了
        mPerformanceInited->setTrue();

#ifdef LN_OUT_LOG
        LN_LOG_WRITE( "IDirectMusicPerformance8 の初期化が完了しました。" );
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
