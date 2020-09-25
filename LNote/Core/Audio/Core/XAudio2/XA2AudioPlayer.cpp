//=============================================================================
//【 XA2AudioPlayer 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../AudioUtil.h"
#include "XA2AudioPlayer.h"

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
// ■ OnMemory クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	OnMemory::OnMemory( AudioDevice* manager_ )
		: mManager          ( manager_ )
        , mAudioSource      ( NULL )
		, mSourceVoice      ( NULL )
		, mEmitterState     ( NULL )
		, mVolume           ( 100.f )
		, mPitch            ( 100.f )
		, mLoopBegin        ( 0 )
		, mLoopLength       ( 0 )
		, mIsPlaying        ( false )
		, mIsPausing        ( false )
		, mIsLoop           ( false )
	{
        LN_SAFE_ADDREF( mManager );
    }

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	OnMemory::~OnMemory()
	{
		stop();

        SAFE_DELETE( mEmitterState );
		LN_SAFE_RELEASE( mAudioSource );

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
        LN_SETERR_R_S_( ( !mAudioSource ), LN_ERR_ASSERT, "< OnMemory::setAudioSource >\nmAudioSource is not NULL" );

		mAudioSource = audio_source_;
		mAudioSource->addRef();

		// オンメモリ再生用に内部に持ってるバッファを埋める
		mAudioSource->fillBufferSafe();

		// 3D 音源として作成している場合はその情報を格納する構造体を作る
		if ( enable_3d_ )
		{
            mEmitterState = LN_NEW EmitterState( mAudioSource->getWaveFormat()->Channels );
		}

        // ソースからループ位置を取得
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
        mVolume = Math::limit( mVolume, 0.0f, 100.0f );
        if ( mSourceVoice )
        {
		    mSourceVoice->SetVolume( mVolume * 0.01f );
        }
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
        mPitch = Math::limit( mPitch, 50.0f, 200.0f );
        if ( mSourceVoice )
        {
		    mSourceVoice->SetFrequencyRatio( mPitch * 0.01f );
        }
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
    u64 OnMemory::getPlayedSamples() const
    {
        if ( mSourceVoice )
        {
            XAUDIO2_VOICE_STATE voice_state;
		    mSourceVoice->GetState( &voice_state );
		    return voice_state.SamplesPlayed;
        }
        return 0;
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
        stop();

        // XAudio のソースボイス作成
        HRESULT hr;
        const LNWaveFormat* ln_format = mAudioSource->getWaveFormat();
		WAVEFORMATEX format;
        AudioUtil::convLNWaveFormatToWAVEFORMATEX( *ln_format, &format );
        LN_COMCALL( mManager->getXAudio2()->CreateSourceVoice( &mSourceVoice, &format ) );
		LN_COMCALL( mSourceVoice->FlushSourceBuffers() );
        
        // SourceVoiceに送信するデータ
        XAUDIO2_BUFFER submit = { 0 };
		submit.AudioBytes = mAudioSource->getOnmemoryPCMBufferSize();	// バッファのバイト数
		submit.pAudioData = (u8*)mAudioSource->getOnmemoryPCMBuffer();	// バッファの先頭アドレス
		submit.Flags = XAUDIO2_END_OF_STREAM;

		// ループ再生する場合
		if ( mIsLoop )
		{
			submit.LoopCount	= XAUDIO2_LOOP_INFINITE;	// 無限ループ
			submit.LoopBegin	= mLoopBegin;
			submit.LoopLength	= mLoopLength;
		}
		// ループ再生しない場合
		else
		{
			submit.LoopBegin = XAUDIO2_NO_LOOP_REGION;
		}

		// ボイスキューにデータを送る
		LN_COMCALL( mSourceVoice->SubmitSourceBuffer( &submit ) );

        LN_COMCALL( mSourceVoice->SetVolume( mVolume * 0.01f ) );
        LN_COMCALL( mSourceVoice->SetFrequencyRatio( mPitch * 0.01f ) );

		// 再生開始
		LN_COMCALL( mSourceVoice->Start() );

		mIsPlaying = true;
		mIsPausing = false;

		polling();	// 3D オーディオの計算を行うため

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● 停止
	//---------------------------------------------------------------------
	LNRESULT OnMemory::stop()
	{
        if ( mSourceVoice )
		{
			mSourceVoice->Stop();
			mSourceVoice->FlushSourceBuffers();
			mSourceVoice->DestroyVoice();
			mSourceVoice = NULL;
		}

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
		if ( mSourceVoice && mIsPlaying )
		{
			// 一時停止する場合で、現在一時停止していない場合
			if ( is_pause_ && !mIsPausing )
			{
				mSourceVoice->Stop( 0 );
				mIsPausing = true;
			}
			// 一時停止を解除する場合で、現在一時停止している場合
			else if ( !is_pause_ && mIsPausing )
			{
				mSourceVoice->Start( 0 );
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
		if ( !mSourceVoice || !mIsPlaying || mIsPausing )
		{
			return false;
		}

		// ループ再生ではない場合
		if ( !mIsLoop )
		{
			XAUDIO2_VOICE_STATE state;
			mSourceVoice->GetState( &state );

			// ボイスキューが空になった場合
			if ( state.BuffersQueued == 0 )
			{
				stop();
				return false;
			}
		}

        // 3D オーディオの更新
        if ( mEmitterState )
        {
            mManager->calcEmitterState( mEmitterState );

		    mSourceVoice->SetFrequencyRatio( mEmitterState->DSPSettings.DopplerFactor * ( mPitch * 0.01f ) );

		    mSourceVoice->SetOutputMatrix(
			    mManager->getMasteringVoice(),   // NULL でもいいみたいだけど一応
			    mEmitterState->DSPSettings.SrcChannelCount,
			    mEmitterState->DSPSettings.DstChannelCount,
			    mEmitterState->DSPSettings.pMatrixCoefficients );
        }

        return true;
    }

    //---------------------------------------------------------------------
	// ● 3D 音源としての位置を設定する
    //---------------------------------------------------------------------
    LNRESULT OnMemory::setPosition( const LVector3& pos_ )
    {
        if ( mEmitterState )
        {
            mEmitterState->Position = pos_;
        }
        return LN_OK;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void OnMemory::setVelocity( const LVector3& v )
    {
        if ( mEmitterState )
        {
            mEmitterState->Velocity = v;
        }
    }


//=============================================================================
// ■ Streaming クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	Streaming::Streaming( AudioDevice* manager_ )
		: mManager          ( manager_ )
        , mAudioSource( NULL )
		, mSourceVoice( NULL )
		, mEmitterState( NULL )
		, mVolume( 100.f )
		, mPitch( 100.f )
		, mLoopBegin( 0 )
		, mLoopLength( 0 )
        //, mEndSample        ( 0 )
		, mPrimaryAudioData( NULL )
		, mSecondaryAudioData( NULL )
		, mAudioDataBufferSize( 0 )
		, mReadCursor( 0 )
        , mLoopBeginBlockOffset ( 0 )
        , mCursorBeginPos       ( 0 )
        , mCursorEndPos         ( 0 )
        , mWriteSampleNum       ( 0 )
		, mIsPlaying( false )
		, mIsPausing( false )
		, mIsLoop( false )
		, mEOF( false )
	{
        LN_SAFE_ADDREF( mManager );
	}

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	Streaming::~Streaming()
	{
        stop();

		LN_SAFE_RELEASE( mAudioSource );
		SAFE_DELETE( mEmitterState );

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
        LN_SETERR_R_S_( ( !mAudioSource ), LN_ERR_ASSERT, "< Streaming::setAudioSource >\nmAudioSource is not NULL" );

		mAudioSource = audio_source_;
		mAudioSource->addRef();

		// 3D 音源として作成している場合はその情報を格納するクラスを作る
		if ( enable_3d_ )
		{
            mEmitterState = LN_NEW EmitterState( mAudioSource->getWaveFormat()->Channels );
		}

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
        mVolume = Math::limit( mVolume, 0.0f, 100.0f );
        if ( mSourceVoice )
        {
	        mSourceVoice->SetVolume( mVolume * 0.01f );
        }
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
        mPitch = Math::limit( mPitch, 50.0f, 200.0f );
        if ( mSourceVoice )
        {
		    mSourceVoice->SetFrequencyRatio( mPitch * 0.01f );
        }
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 再生したサンプル数の取得
    //---------------------------------------------------------------------
    u64 Streaming::getPlayedSamples() const
    {
        if ( mSourceVoice )
        {
            XAUDIO2_VOICE_STATE voice_state;
		    mSourceVoice->GetState( &voice_state );
		    return voice_state.SamplesPlayed;
        }
        return 0;
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
        stop();

        // ソースボイス作成
        HRESULT hr;
        const LNWaveFormat* ln_format = mAudioSource->getWaveFormat();
		WAVEFORMATEX format;
        AudioUtil::convLNWaveFormatToWAVEFORMATEX( *ln_format, &format );
        LN_COMCALL( mManager->getXAudio2()->CreateSourceVoice( &mSourceVoice, &format ) );
        mSourceVoice->FlushSourceBuffers();

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

        mSourceVoice->SetVolume( mVolume * 0.01f );
        mSourceVoice->SetFrequencyRatio( mPitch * 0.01f );

		// 再生開始
		LN_DXCALL_R( mSourceVoice->Start() );

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
		if ( mSourceVoice )
		{
			mSourceVoice->Stop();
			mSourceVoice->FlushSourceBuffers();
			mSourceVoice->DestroyVoice();
			mSourceVoice = NULL;
		}

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
		if ( mSourceVoice && mIsPlaying )
		{
			// 一時停止する場合で、現在一時停止していない場合
			if ( is_pause_ && !mIsPausing )
			{
				mSourceVoice->Stop( 0 );
				mIsPausing = true;
			}
			// 一時停止を解除する場合で、現在一時停止している場合
			else if ( !is_pause_ && mIsPausing )
			{
				mSourceVoice->Start( 0 );
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
		if ( !mSourceVoice || !mIsPlaying || mIsPausing )
		{
			return false;
		}

		// 状態の取得
		XAUDIO2_VOICE_STATE voice_state;
		mSourceVoice->GetState( &voice_state );

		// 再生が終了しておらず、キューにデータがふたつたまっていない場合
		if ( !mEOF && voice_state.BuffersQueued < 2 )
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
			// キューが完全になくなった場合、停止とする
			if ( voice_state.BuffersQueued == 0 )
			{
				stop();
				return false;
			}
		}

        // 3D オーディオの更新
        if ( mEmitterState )
        {
            mManager->calcEmitterState( mEmitterState );

		    mSourceVoice->SetFrequencyRatio( mEmitterState->DSPSettings.DopplerFactor * ( mPitch * 0.01f ) );

		    mSourceVoice->SetOutputMatrix(
			    mManager->getMasteringVoice(),   // NULL でもいいみたいだけど一応
			    mEmitterState->DSPSettings.SrcChannelCount,
			    mEmitterState->DSPSettings.DstChannelCount,
			    mEmitterState->DSPSettings.pMatrixCoefficients );
        }

		return true;
	}


    //---------------------------------------------------------------------
	// ● 3D 音源としての位置を設定する
    //---------------------------------------------------------------------
    LNRESULT Streaming::setPosition( const LVector3& pos_ )
    {
        if ( mEmitterState )
        {
            mEmitterState->Position = pos_;
        }
        return LN_OK;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Streaming::setVelocity( const LVector3& v )
    {
        if ( mEmitterState )
        {
            mEmitterState->Velocity = v;
        }
    }

	//---------------------------------------------------------------------
	// ● セカンダリに新しいデータを書き込んで、再生キューに追加する
	//---------------------------------------------------------------------
	LNRESULT Streaming::_addNextBuffer()
	{
        HRESULT hr;

#if 0
        // 読み込み位置に移動
		mAudioSource->seek( mReadCursor );

		// データ読み込み
		u32 read_size;		// 呼んだサイズ
		u32 write_size;		// デコードして書き込んだサイズ
		mAudioSource->read( mSecondaryAudioData, mAudioDataBufferSize, &read_size, &write_size );
#endif

		// データ読み込み
		u32 read_size;		// 呼んだサイズ
		u32 write_size;		// デコードして書き込んだサイズ
		mAudioSource->readSafe( mSecondaryAudioData, mAudioDataBufferSize, mReadCursor, &read_size, &write_size );

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

        if ( mIsLoop )
        {
            if ( mLoopBegin == 0 && mLoopLength == 0 )
            {
                mCursorBeginPos = 0;
                mCursorEndPos = mAudioSource->getOnmemoryPCMBufferSize();
            }
            else
            {
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
            }
        }
        
		// SourceVoice に送信するデータの設定
		XAUDIO2_BUFFER buffer = { 0 };

		// 現在位置がファイルの終端に達している場合
		if ( !mIsLoop && mReadCursor >= mCursorEndPos )//mAudioSource->getSourceDataSize() )
		{
			// フラグ追加 ( 最後まで演奏した時、GetState で取得する演奏時間をリセットする )
			buffer.Flags = XAUDIO2_END_OF_STREAM;
		}

		buffer.AudioBytes = write_size;
		buffer.pAudioData = data_buffer;

		// SourceVoice にデータを送る
        LN_DXCALL_R( mSourceVoice->SubmitSourceBuffer( &buffer ) );

		// primary と secondary の入れ替え
		u8* temp = mPrimaryAudioData;
		mPrimaryAudioData = mSecondaryAudioData;
		mSecondaryAudioData = temp;

        return LN_OK;
	}


//=============================================================================
// ■ MidiPlayer クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	MidiPlayer::MidiPlayer( AudioDevice* manager_ )
		: mManager          ( manager_ )
        , mAudioSource      ( NULL )
        , mSegment          ( NULL )
		, mVolume           ( 100.0f )
		, mPitch            ( 100.0f )
		, mLoopBegin        ( 0 )
		, mLoopLength       ( 0 )
        , mPauseTime        ( 0 )
		, mIsPlaying        ( false )
		, mIsPausing        ( false )
		, mIsLoop           ( false )
	{
        LN_SAFE_ADDREF( mManager );
	}

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	MidiPlayer::~MidiPlayer()
	{
        SAFE_DELETE( mSegment );
		LN_SAFE_RELEASE( mAudioSource );

        if ( mManager )
        {
            mManager->removeAudioPlayer( this );
            LN_SAFE_RELEASE( mManager );
        }
	}

	//---------------------------------------------------------------------
	// ● オーディオソースの設定
	//---------------------------------------------------------------------
	LNRESULT MidiPlayer::initialize( IAudioSource* audio_source_, bool enable_3d_ )
	{
        LN_SETERR_R_S_( ( !mAudioSource ), LN_ERR_ASSERT, "< MidiPlayer::setAudioSource >\nmAudioSource is not NULL" );

        mAudioSource = dynamic_cast< Midi* >( audio_source_ );
		mAudioSource->addRef();

		// オンメモリ再生用に内部に持ってるバッファを埋める
		mAudioSource->fillBufferSafe();


        if ( DirectMusicManager::getInstance()->isInitialized() && !mSegment )
        {
            mSegment = LN_NEW DirectMusicSegment(
                DirectMusicManager::getInstance()->createDMPerformance(),
                dynamic_cast< Midi* >( mAudioSource ) );
        }

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● 音量の取得
	//---------------------------------------------------------------------
	int MidiPlayer::getVolume() const
	{
		return static_cast< int >( mVolume );
	}

	//---------------------------------------------------------------------
	// ● 音量の設定
	//---------------------------------------------------------------------
	LNRESULT MidiPlayer::setVolume( int volume_ )
	{
		mVolume = static_cast< float >( volume_ );
		if ( mVolume < 0.f ) { 
			mVolume = 0.f; }
		if ( mVolume > 100.f ) { 
			mVolume = 100.f; }

        if ( mSegment )
        {
		    mSegment->setVolume( mVolume );
        }

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● ピッチの取得
	//---------------------------------------------------------------------
	int MidiPlayer::getPitch() const
	{
		return static_cast< int >( mPitch );
	}

	//---------------------------------------------------------------------
	// ● ピッチの設定
	//---------------------------------------------------------------------
	LNRESULT MidiPlayer::setPitch( int pitch_ )
	{
		 mPitch = static_cast< float >( pitch_ );
		if ( mPitch < 50.f )  { 
			mPitch = 50.f; }
		if ( mPitch > 200.f ) { 
			mPitch = 200.f; }
		float p = mPitch * 0.01f;
        if ( mSegment )
        {
            mSegment->setPitch( mPitch );
        }

        return LN_OK;
	}

    //---------------------------------------------------------------------
	// ● 再生したサンプル数の取得
    //---------------------------------------------------------------------
    u64 MidiPlayer::getPlayedSamples() const
    {
        if ( mSegment )
        {
		    return mSegment->getPlayPosition();
        }
        return 0;
    }

	//---------------------------------------------------------------------
	// ● 再生中かを判定する
	//---------------------------------------------------------------------
	bool MidiPlayer::isPlaying() const
	{
		return mIsPlaying;
	}

	//---------------------------------------------------------------------
	// ● 一時停止中かを判定する
	//---------------------------------------------------------------------
	bool MidiPlayer::isPausing() const
	{
		return mIsPausing;
	}

	//---------------------------------------------------------------------
	// ● ループ情報の設定
	//---------------------------------------------------------------------
	LNRESULT MidiPlayer::setLoopState( u32 loop_begin_, u32 loop_length_ )
	{
		mLoopBegin = loop_begin_;
		mLoopLength = loop_length_;

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● 再生
	//---------------------------------------------------------------------
	LNRESULT MidiPlayer::play()
	{
        if ( !DirectMusicManager::getInstance()->isInitialized() )
        {
            // 同じものは追加されないのでこのままで OK
            DirectMusicManager::getInstance()->addPlayRequest( this );
            return LN_OK;
        }



        // 初期化完了前にユーザーによってループ位置が設定されていなければ
        // CC111 を目印とした位置に設定する
        if ( mLoopBegin == 0 && mLoopLength == 0 )
        {
            u32 cc111time;
            u32 base_time;
            mAudioSource->getLoopState( &cc111time, &base_time );

            mLoopBegin  = cc111time * LN_MUSICTIMEBASE / base_time;
            mLoopLength = 0;
        }

		// 再生開始
		return _play();
	}

	//---------------------------------------------------------------------
	// ● 停止
	//---------------------------------------------------------------------
	LNRESULT MidiPlayer::stop()
	{
		if ( mSegment )
		{
            mSegment->stop();
		}
		mIsPlaying = false;
		mIsPausing = false;

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● 一時停止
	//---------------------------------------------------------------------
	LNRESULT MidiPlayer::pause( bool is_pause_ )
	{
		// 再生中の場合
		if ( mIsPlaying )
		{
			// 一時停止する場合で、現在一時停止していない場合
			if ( is_pause_ && !mIsPausing )
			{
				// 現在の演奏位置を記憶して停止
				mPauseTime = mSegment->getPlayPosition();
				mSegment->stop();
				
				mIsPausing = true;
			}
			else if ( !is_pause_ && mIsPausing )
			{
				// 一時停止した時の再生位置に移動
				mSegment->setPlayPosition( mPauseTime );

				mSegment->play();

				mIsPausing = false;
			}
		}
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● ポーリング更新
	//---------------------------------------------------------------------
	bool MidiPlayer::polling()
	{
		// 再生中ではない場合は中断
		if ( !mIsPlaying || mIsPausing )
		{
			return false;
		}

		// ループ再生ではない場合
		if ( !mIsLoop )
		{
            if ( mSegment )
            {
                // 再生開始直後は IDirectMusicPerformance8::IsPlaying() は false になってしまい、
                // TotalTime() も実際に音が鳴る長さよりも若干短くなってしまっている。
                // (DirectMusic の仕様？というか、バグな気もする…)
                // そのため、音が鳴っている事と、再生位置による二重判定を行う。
                if ( !mSegment->isPlaying() && mSegment->getPlayPosition() >= mSegment->getTotalTime() )
			    {
                    stop();
				    return false;
			    }
		    }
        }

        return true;
	}



    // 臨時用。全体の再生時間の取得
    u32 MidiPlayer::getTotalTime() const
    {
        if ( mSegment ) // 未初期化対策
        {
            return mSegment->getTotalTime();
        }
        return 0;
    }

    //---------------------------------------------------------------------
	// ● 再生要求リストに追加されている状態で DirectMusic の初期化が完了したときに呼ばれる
    //---------------------------------------------------------------------
    void MidiPlayer::onFinishDMInit( IDirectMusicPerformance8* dm_performance_ )
    {
        mSegment = LN_NEW DirectMusicSegment( dm_performance_, dynamic_cast< Midi* >( mAudioSource ) );

        // 初期化中に設定されたパラメータを再設定する
        setVolume( static_cast< int >( mVolume ) );
        setPitch( static_cast< int >( mPitch ) );

        // 初期化完了前にユーザーによってループ位置が設定されていなければ
        // CC111 を目印とした位置に設定する
        if ( mLoopBegin == 0 && mLoopLength == 0 )
        {
            u32 cc111time;
            u32 base_time;
            mAudioSource->getLoopState( &cc111time, &base_time );

            mLoopBegin  = cc111time * LN_MUSICTIMEBASE / base_time;
            mLoopLength = 0;

        }

        _play();
    }







    //---------------------------------------------------------------------
	// ● 実際に再生する
	//---------------------------------------------------------------------
	LNRESULT MidiPlayer::_play()
	{
        // とりあえず停止
        stop();


        if ( !mSegment )
        {
            mSegment = LN_NEW DirectMusicSegment(
                DirectMusicManager::getInstance()->createDMPerformance(),
                dynamic_cast< Midi* >( mAudioSource ) );

            // ちなみに setAudioSource() で作成するのはダメ。
            // DirectMusic の初期化中に setAudioSource() が呼ばれた場合はもちろん mSegment = NULL だけど、
            // そのあと play() が呼ばれたときにも当然 NULL のまま。
            // 初期化中に一度でも play() を呼んで要求リストに追加していれば onFinishDMInit() が呼ばれるけど、
            // そうでない場合はずっと NULL のままになってしまう。
        }
		


		// ループ再生する場合
		if ( mIsLoop )
		{
			mSegment->setLoopState( true, mLoopBegin, mLoopLength );
		}
		// ループ再生しない場合
		else
		{
			mSegment->setLoopState( false, 0, 0 );
		}

        mSegment->play();

        mIsPlaying = true;
		mIsPausing = false;

        return LN_OK;
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