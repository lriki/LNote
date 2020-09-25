//=============================================================================
//【 Audio::Sound 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#if defined(LNOTE_DIRECTX)
#include "Core/XAudio2/XA2AudioPlayer.h"    // for get midi state
#endif
#include "Manager.h"
#include "Sound.h"

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
// ■ Sound クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    Sound::Sound( Manager* manager_, IAudioPlayer* player_ )
        : mManager          ( manager_ )
        , mAudioPlayer      ( player_ )
        , mOrginalVolume    ( 100 )
        , mFadeState        ( LN_SOUNDFADE_CONTINUE )
        , mFlags            ( 0 )
        , mGroupFlags       ( 0 )
        , mIsFading         ( false )
    {
        LN_SAFE_ADDREF( mManager );

        mLock = LN_NEW Thread::Mutex();
    }

	//---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    Sound::~Sound()
    {
        if ( mManager )
        {
            mManager->removeSound( this );
        }
        
        LN_SAFE_RELEASE( mAudioPlayer );
        LN_SAFE_RELEASE( mLock );
        LN_SAFE_RELEASE( mManager );
    }

	//---------------------------------------------------------------------
    // ● 音量の取得
    //---------------------------------------------------------------------
    int Sound::getVolume()
    {
        mLock->lock();
        int r = mAudioPlayer->getVolume();
        mLock->unlock();
        return r;
    }

	//---------------------------------------------------------------------
    // ● 音量の設定
    //---------------------------------------------------------------------
    void Sound::setVolume( int volume_ )
    {
        mLock->lock();
        mAudioPlayer->setVolume( volume_ );
        mLock->unlock();
    }

	//---------------------------------------------------------------------
    // ● ピッチの取得
    //---------------------------------------------------------------------
    int Sound::getPitch()
    {
        mLock->lock();
        int r = mAudioPlayer->getPitch();
        mLock->unlock();
        return r;
    }

	//---------------------------------------------------------------------
    // ● ピッチの設定
    //---------------------------------------------------------------------
    void Sound::setPitch( int pitch_ )
    {
        mLock->lock();
        mAudioPlayer->setPitch( pitch_ );
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    // ● サウンドの状態を取得する
    //---------------------------------------------------------------------
    void Sound::getState( LNAudioPlayState* state_ )
    {
        mLock->lock();

        IAudioSource* source = mAudioPlayer->getAudioSource();
        if ( source->getSourceFormat() == LN_AUDIOSOURCE_MIDI )
        {
#if defined(LNOTE_DIRECTX)
            state_->TotalUnits    = static_cast< XAudio2::MidiPlayer* >( mAudioPlayer )->getTotalTime();
            state_->SamplesPerSec = LN_MUSICTIMEBASE;
#endif
        }
        else
        {
            state_->TotalUnits    = mAudioPlayer->getAudioSource()->getTotalUnits();
            state_->SamplesPerSec = mAudioPlayer->getAudioSource()->getWaveFormat()->SamplesPerSec;
        }
        state_->CurrentUnits = static_cast< u32 >( mAudioPlayer->getPlayedSamples() );
        state_->SoundPlayState = mAudioPlayer->getPlayState();

        mLock->unlock();
    }

	////---------------------------------------------------------------------
 //   // ● 再生中かを判定する
 //   //---------------------------------------------------------------------
 //   bool Sound::isPlaying()
 //   {
 //       mLock->lock();
 //       bool r = 
 //       mLock->unlock();
 //       return r;
 //   }

	////---------------------------------------------------------------------
 //   // ● 一時停止中かを判定する
 //   //---------------------------------------------------------------------
 //   bool Sound::isPausing()
 //   {
 //       mLock->lock();
 //       bool r = mAudioPlayer->isPausing();
 //       mLock->unlock();
 //       return r;
 //   }

    //---------------------------------------------------------------------
    // ● ループ再生の有効設定
    //---------------------------------------------------------------------
    void Sound::loop( bool loop_ )
    {
        mAudioPlayer->loop( loop_ );
    }

    //---------------------------------------------------------------------
    // ● ループ再生が有効かを判定する
    //---------------------------------------------------------------------
    bool Sound::isLoop()
    {
        return mAudioPlayer->isLoop();
    }

    //---------------------------------------------------------------------
    // ● ループ情報の設定
    //---------------------------------------------------------------------
    void Sound::setLoopState( u32 begin_, u32 length_ )
    {
        mLock->lock();
        mAudioPlayer->setLoopState( begin_, length_ );
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    // ● 再生
    //---------------------------------------------------------------------
    LNRESULT Sound::play()
    {
        mLock->lock();
        LNRESULT lr = mAudioPlayer->play();
        mLock->unlock();
        return lr;
    }

    //---------------------------------------------------------------------
    // ● 停止
    //---------------------------------------------------------------------
    void Sound::stop()
    {
        mLock->lock();
        mAudioPlayer->stop();
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    // ● 一時停止
    //---------------------------------------------------------------------
    void Sound::pause( bool is_pause_ )
    {
        mLock->lock();
        mAudioPlayer->pause( is_pause_ );
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    // ● 音量のフェード
    //---------------------------------------------------------------------
    LNRESULT Sound::fadeVolume( int target_volume_, int time_, LNSoundFadeState state_ )
    {
        mLock->lock();

        mFadeValue.start( static_cast< int >( mAudioPlayer->getVolume() ), target_volume_, static_cast< double >( time_ ) * 0.001 );	// 現在の音量から volume_ へのフェード
		mFadeState = state_;
		mIsFading = true;

		mLock->unlock();

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 3D 音源かを判定する
    //---------------------------------------------------------------------
    bool Sound::is3DSound()
    {
        return mAudioPlayer->is3DSound();
    }

    //---------------------------------------------------------------------
    // ● 3D 音源としての位置を設定する
    //---------------------------------------------------------------------
    LNRESULT Sound::setPosition( const LVector3& pos_ )
    {
        mLock->lock();
        LNRESULT lr = mAudioPlayer->setPosition( pos_ );
        mLock->unlock();
        return lr;
    }

    //---------------------------------------------------------------------
    // ● 3D 音源としての位置を取得する
    //---------------------------------------------------------------------
    const LVector3& Sound::getPosition()
    {
        return mAudioPlayer->getPosition();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Sound::setVelocity( const LVector3& v )
    {
        mLock->lock();
        mAudioPlayer->setVelocity( v );
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Sound::setMaxDistance( lnfloat distance )
    {
        mAudioPlayer->setEmitterDistance( distance );
    }

    void Sound::setGroupFlag( u32 flag_ )
    {
        mLock->lock();
        mGroupFlags = flag_;
        mLock->unlock();
    }

    u32 Sound::getGroupFlag()
    {
        mLock->lock();
        u32 f = mGroupFlags;
        mLock->unlock();
        return f;
    }

    bool Sound::update( float elapsed_time_ )
    {
        mLock->lock();
        bool r = mAudioPlayer->polling();

        // フェード中の場合
		if ( mIsFading )
		{
			mFadeValue.advanceTime( elapsed_time_ );
			
			mAudioPlayer->setVolume( mFadeValue.getValue() );

			// フェード完了
			if ( mFadeValue.isFinished() )
			{
				mIsFading = false; 

				// フェード終了時の動作によって分岐
				switch ( mFadeState )
				{
					// 停止する場合
					case LN_SOUNDFADE_STOP:
						mAudioPlayer->stop();
						break;
					// 一時停止する場合
					case LN_SOUNDFADE_PAUSE:
						mAudioPlayer->pause( true );
						break;
					// 停止して音量を戻す場合
					case LN_SOUNDFADE_STOP_RESET:
						mAudioPlayer->stop();
						mAudioPlayer->setVolume( mFadeValue.getStartValue() );
						break;
					// 一時停止して音量を戻す場合
					case LN_SOUNDFADE_PAUSE_RESET:
						mAudioPlayer->pause( true );
						mAudioPlayer->setVolume( mFadeValue.getStartValue() );
						break;
				}
			}
		}

        mLock->unlock();
        return r;
    }

    /*
    void Sound::onDelete()
    {
        Manager* m = mManager;
        mManager->deleteSound( this );

        LN_SAFE_RELEASE( m );
    }
    */
    

#if 0

//=============================================================================
// ■ Sound クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    Sound::Sound( Manager* manager_, Audio::IAudioPlayer* player_ )
        : mManager      ( manager_ )
        , mAudioPlayer  ( player_ )
        , mFlags        ( 0 )
        , mLock         ( NULL )
    {
        LN_SAFE_ADDREF( mManager );

        mLock = LN_NEW Thread::Mutex();
    }

	//---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    Sound::~Sound()
    {
        // mManager が残っていたらバグ


        LN_SAFE_RELEASE( mAudioPlayer );

        LN_SAFE_RELEASE( mLock );
    }

	//---------------------------------------------------------------------
    // ● 音量の取得
    //---------------------------------------------------------------------
    int Sound::getVolume()
    {
        return mAudioPlayer->getVolume();
    }

	//---------------------------------------------------------------------
    // ● 音量の設定
    //---------------------------------------------------------------------
    void Sound::setVolume( int volume_ )
    {
        mAudioPlayer->setVolume( volume_ );
    }

	//---------------------------------------------------------------------
    // ● ピッチの取得
    //---------------------------------------------------------------------
    int Sound::getPitch()
    {
        return mAudioPlayer->getPitch();
    }

	//---------------------------------------------------------------------
    // ● ピッチの設定
    //---------------------------------------------------------------------
    void Sound::setPitch( int pitch_ )
    {
        mAudioPlayer->setPitch( pitch_ );
    }

	//---------------------------------------------------------------------
    // ● 再生中かを判定する
    //---------------------------------------------------------------------
    bool Sound::isPlaying()
    {
        return mAudioPlayer->isPlaying();
    }

	//---------------------------------------------------------------------
    // ● 一時停止中かを判定する
    //---------------------------------------------------------------------
    bool Sound::isPausing()
    {
        return mAudioPlayer->isPausing();
    }


    //---------------------------------------------------------------------
    // ● ループ情報の設定
    //---------------------------------------------------------------------
    void Sound::setLoopState( bool loop_, int begin_, int length_ )
    {
        mLock->lock();
        mAudioPlayer->setLoopState( loop_, begin_, length_ );
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    // ● 再生
    //---------------------------------------------------------------------
    void Sound::play()
    {
        mLock->lock();
        mAudioPlayer->play();
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    // ● 停止
    //---------------------------------------------------------------------
    void Sound::stop()
    {
        mLock->lock();
        mAudioPlayer->stop();
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    // ● 一時停止
    //---------------------------------------------------------------------
    void Sound::pause( bool is_pause_ )
    {
        mLock->lock();
        mAudioPlayer->pause( is_pause_ );
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    // ● 音量のフェード
    //---------------------------------------------------------------------
    void Sound::fadeVolume( int target_volume_, int time_, LNSoundFadeState state_ )
    {
    }

    void Sound::setGroupFlag( u32 flag_ )
    {
    }

    bool Sound::update( float elapsed_time_ )
    {
        bool r = mAudioPlayer->polling();
        return r;
    }

    void Sound::onDelete()
    {
        LN_SAFE_RELEASE( mManager );
        delete this;
    }

#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Audio
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================