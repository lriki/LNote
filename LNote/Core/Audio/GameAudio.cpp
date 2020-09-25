//=============================================================================
//【 GameAudio 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Manager.h"
#include "GameAudio.h"

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
// ■ GameAudio クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	GameAudio::GameAudio( Manager* mamager_ )
		: mManager	        ( mamager_ )
        , mLock             ( NULL )
		, mBGM		        ( NULL )
        , mBGS              ( NULL )
        , mME               ( NULL )
        , mBGMVolume        ( 0 )
        , mBGMPitch         ( 0 )
        , mBGSVolume        ( 0 )
        , mBGSPitch         ( 0 )
        , mBGMFadeOutTime   ( 500 )
        , mBGMFadeInTime    ( 1000 )
        , mMEPlaying        ( false )
        , mBGMRestart       ( false )
        , mBGSRestart       ( false )
	{
        mLock = LN_NEW Thread::Mutex();
	}

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	GameAudio::~GameAudio()
	{
        stopBGM( 0 );
        stopBGS( 0 );
        stopME();
		stopSE();
		_p(mReleaseAtPlayEndList.size());
        // 再生終了後の解放リストに入っているサウンドを解放
        ReleaseAtPlayEndList::iterator itr = mReleaseAtPlayEndList.begin();
        ReleaseAtPlayEndList::iterator end = mReleaseAtPlayEndList.end();
        for ( ; itr != end; ++itr )
        {
            _p((*itr)->release());
        }
        mReleaseAtPlayEndList.clear();

		LN_SAFE_RELEASE( mBGM );
        LN_SAFE_RELEASE( mBGS );
        LN_SAFE_RELEASE( mME );
        LN_SAFE_RELEASE( mLock );
	}

	//---------------------------------------------------------------------
	// ● BGM の演奏を開始する
	//---------------------------------------------------------------------
	LNRESULT GameAudio::playBGM( const lnChar* filename_, int volume_, int pitch_, int fade_time_ )
	{
        // 演奏再開チェック
        if ( !mBGMRestart && mBGM )
        {
            if ( mBGMName == filename_ )
            {
                return LN_OK;
            }
        }

        LNRESULT lr = LN_OK;
        ISound* sound;

        LN_CALL_GOTO( ERR_EXIT, mManager->createSound( &sound, filename_, LN_SOUNDPLAYTYPE_STREAMING, false ) );
        LN_CALL_GOTO( ERR_EXIT, playBGMFromSound( sound, volume_, pitch_, fade_time_ ) );

        // ファイル名記憶
        mBGMName = filename_;
        
    ERR_EXIT:
        LN_SAFE_RELEASE( sound );
        return lr;
	}

    //---------------------------------------------------------------------
	// ● 指定された ISound を BGM として演奏する
    //---------------------------------------------------------------------
	LNRESULT GameAudio::playBGMFromSound( ISound* sound_, int volume_, int pitch_, int fade_time_ )
    {
		
        // 演奏再開チェック
		// TODO:フェードアウト中に再開すると無音になる
        //if ( !mBGMRestart && mBGM )
        //{
        //    if ( sound_ == mBGM && volume_ == mBGMVolume && pitch_ == mBGMPitch )
        //    {
        //        return LN_OK;
        //    }
        //}

        ISound* prev_bgm = mBGM;

        mBGM = sound_;
        LN_SAFE_ADDREF( mBGM );

        mBGM->setPitch( pitch_ );
        mBGM->loop( true );
        mBGM->setGroupFlag( LN_AUDIOPLAYER_GROUP_BGM );

        mBGMVolume = volume_;
        mBGMPitch  = pitch_;


        // ME が再生中ではないかを調べる
        bool me_not_play = true;    
		if ( mME )
		{
            LNAudioPlayState state;
            mME->getState( &state );
            if ( state.SoundPlayState == LN_SOUNDPLAYSTATE_PLAYING )
			{
				me_not_play = false;
			}
		}

		// ME 再生中ではない場合
		if ( me_not_play )
		{
			if ( fade_time_ > 0 )
			{
                mBGM->setVolume( 0 );
                mBGM->play();
                mBGM->fadeVolume( volume_, fade_time_, LN_SOUNDFADE_CONTINUE );
			}
			else
			{
				mBGM->setVolume( volume_ );
		        mBGM->play();
			}
		}
        // ME 再生中の場合は、一度再生するけどすぐに一時停止する ( ME 終了後に再開 )
		else
		{
			mBGM->play();
			mBGM->pause( true );
		}

        // フェード時間がある場合
        if ( fade_time_ > 0 )
		{
			if ( prev_bgm )
			{
				// ひとつ前のBGMは、fade_time_ 後に停止、解放するようにする
				prev_bgm->loop( false );
				prev_bgm->fadeVolume( 0, fade_time_, LN_SOUNDFADE_STOP_RESET );
                _pushReleaseAtPlayEndList( prev_bgm );
			}
		}
        // フェード時間がない場合はすぐに停止
		else
		{
			if ( prev_bgm )
            {
                prev_bgm->stop();
            }
		}

        LN_SAFE_RELEASE( prev_bgm );

        return LN_OK;
    }

	//---------------------------------------------------------------------
	// ● BGM の演奏を停止する
	//---------------------------------------------------------------------
	void GameAudio::stopBGM( int fade_time_ )
	{
		if ( mBGM )
		{
			if ( fade_time_ > 0 )
		    {
                // フェード終了後に停止して、音量等を元に戻す
			    mBGM->fadeVolume( 0, fade_time_, LN_SOUNDFADE_STOP_RESET );
		    }
		    else
		    {
			    mBGM->stop();
                LN_SAFE_RELEASE( mBGM );
		    }

			mBGMName = _T("");
		}
	}

    //---------------------------------------------------------------------
	// ● BGS ( 環境音 ) の演奏を開始する
	//---------------------------------------------------------------------
	LNRESULT GameAudio::playBGS( const lnChar* filename_, int volume_, int pitch_, int fade_time_ )
    {
        // 演奏再開チェック
        if ( !mBGSRestart && mBGS )
        {
            if ( mBGSName == filename_ )
            {
                return LN_OK;
            }
        }

        LNRESULT lr = LN_OK;
        ISound* sound;

        LN_CALL_GOTO( ERR_EXIT, mManager->createSound( &sound, filename_, LN_SOUNDPLAYTYPE_STREAMING, false ) );
        LN_CALL_GOTO( ERR_EXIT, playBGSFromSound( sound, volume_, pitch_, fade_time_ ) );
        
        // ファイル名記憶
        mBGSName = filename_;

    ERR_EXIT:
        LN_SAFE_RELEASE( sound );
        return lr;
    }

    //---------------------------------------------------------------------
	// ● 指定された ISound を BGS として演奏する
    //---------------------------------------------------------------------
	LNRESULT GameAudio::playBGSFromSound( ISound* sound_, int volume_, int pitch_, int fade_time_ )
    {
        // 演奏再開チェック
        //if ( !mBGSRestart && mBGS )
        //{
        //    if ( sound_ == mBGS && volume_ == mBGSVolume && pitch_ == mBGSPitch )
        //    {
        //        return LN_OK;
        //    }
        //}

        LNRESULT lr = LN_OK;

        ISound* prev_bgs = mBGS;

        mBGS = sound_;
        LN_SAFE_ADDREF( mBGS );

        mBGS->setPitch( pitch_ );
		mBGS->loop( true );
        mBGS->setGroupFlag( LN_AUDIOPLAYER_GROUP_BGS );

        mBGMVolume = volume_;
        mBGMPitch  = pitch_;

        if ( fade_time_ > 0 )
		{
            mBGS->setVolume( 0 );
            mBGS->play();
            mBGS->fadeVolume( volume_, fade_time_, LN_SOUNDFADE_CONTINUE );
		}
		else
		{
            mBGS->setVolume( volume_ );
		    mBGS->play();
		}

        // フェード時間がある場合
        if ( fade_time_ > 0 )
		{
			if ( prev_bgs )
			{
				// ひとつ前のBGSは、fade_time_ 後に停止、解放するようにする
				prev_bgs->loop( false );
				prev_bgs->fadeVolume( 0, fade_time_, LN_SOUNDFADE_STOP_RESET );
                _pushReleaseAtPlayEndList( prev_bgs );
			}
		}
        // フェード時間がない場合はすぐに停止
		else
		{
			if ( prev_bgs )
            {
                prev_bgs->stop();
            }
		}

        LN_SAFE_RELEASE( prev_bgs );
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● BGS の演奏を停止する
	//---------------------------------------------------------------------
	void GameAudio::stopBGS( int fade_time_ )
	{
		if ( mBGS )
		{
			if ( fade_time_ > 0 )
		    {
                // フェード終了後に停止して、音量等を元に戻す
			    mBGS->fadeVolume( 0, fade_time_, LN_SOUNDFADE_STOP_RESET );
		    }
		    else
		    {
			    mBGS->stop();
		    }

			mBGSName = _T("");
		}
	}

    //---------------------------------------------------------------------
	// ● ME (効果音楽) の演奏を開始する
	//---------------------------------------------------------------------
	LNRESULT GameAudio::playME( const lnChar* filename_, int volume_, int pitch_ )
    {       
        LNRESULT lr = LN_OK;
        ISound* sound;

        LN_CALL_GOTO( ERR_EXIT, mManager->createSound( &sound, filename_, LN_SOUNDPLAYTYPE_AUTO, false ) );
        LN_CALL_GOTO( ERR_EXIT, playMEFromSound( sound, volume_, pitch_ ) );
        
    ERR_EXIT:
        LN_SAFE_RELEASE( sound );
        return lr;
    }

    //---------------------------------------------------------------------
	// ● 指定された ISound を ME として演奏する
    //---------------------------------------------------------------------
	LNRESULT GameAudio::playMEFromSound( ISound* sound_, int volume_, int pitch_ )
    {
        stopME();

        mME = sound_;
        LN_SAFE_ADDREF( mME );

        mME->setPitch( pitch_ );
		mME->loop( true );
        mME->setGroupFlag( 0 );

        bool flag = false;  // BGM があり、再生されているかを示すフラグ

		// BGM がある場合
		if ( mBGM )
		{
            LNAudioPlayState state;
            mBGM->getState( &state );

			// 再生されている場合
            if ( state.SoundPlayState == LN_SOUNDPLAYSTATE_PLAYING )
			{
				flag = true;
			}
		}

        // BGM が再生中の場合
        if ( flag )
		{
			// フェードアウト時間がある場合
			if ( mBGMFadeOutTime > 0 )
			{
				// フェードアウト後、一時停止する
				mBGM->fadeVolume( 0, mBGMFadeOutTime, LN_SOUNDFADE_PAUSE );
			}
			// フェードアウト時間がない場合
			else
			{
				// すぐに一時停止
				mBGM->pause( true );
			}
		}
        // BGM がない場合はすぐ演奏開始
		else
		{
			mME->play();
			mMEPlaying = true;
		}

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● ME の演奏を停止する
	//---------------------------------------------------------------------
	void GameAudio::stopME()
	{
		if ( mME )
		{
			mME->stop();
            LN_SAFE_RELEASE( mME );
			
			// BGM があって、一時停止中の場合は再開
			if ( mBGM )
			{
                LNAudioPlayState state;
                mBGM->getState( &state );

                if ( state.SoundPlayState == LN_SOUNDPLAYSTATE_PAUSING )
				{
					mBGM->fadeVolume( mBGMVolume, mBGMFadeInTime, LN_SOUNDFADE_CONTINUE );
                    mBGM->pause( false );
				}
			}

			mMEPlaying = false;
		}
	}

	//---------------------------------------------------------------------
	// ● SE の演奏を開始する
	//---------------------------------------------------------------------
	LNRESULT GameAudio::playSE( const lnChar* filename_, int volume_, int pitch_ )
	{
        LNRESULT lr;

        // サウンド作成
        ISound* sound;
        LN_CALL_R( mManager->createSound( &sound, filename_, LN_SOUNDPLAYTYPE_ONMEMORY, false ) );

        // ボリューム・ピッチ設定
        sound->setVolume( volume_ );
        sound->setPitch( pitch_ );

        // 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
        //sound->setPlayerFlags( LN_AUDIOPLAYER_AFTERRELEASE );
        _pushReleaseAtPlayEndList( sound );

        sound->setGroupFlag( LN_AUDIOPLAYER_GROUP_SE );

        // 再生
		sound->loop( false );
		sound->play();

        LN_SAFE_RELEASE( sound );
        return LN_OK;
	}

    //---------------------------------------------------------------------
	// ● SE を 3D 空間上で再生する
    //---------------------------------------------------------------------
    LNRESULT GameAudio::playSE( const lnChar* filename_, const LVector3& position_, float distance, int volume_, int pitch_ )
    {
        LNRESULT lr;

        // サウンド作成
        ISound* sound;
        LN_CALL_R( mManager->createSound( &sound, filename_, LN_SOUNDPLAYTYPE_ONMEMORY, true ) );

        // 位置・ボリューム・ピッチ設定
        sound->setPosition( position_ );
        sound->setMaxDistance( distance );
        sound->setVolume( volume_ );
        sound->setPitch( pitch_ );

        // 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
        sound->setGroupFlag( LN_AUDIOPLAYER_GROUP_SE );
        _pushReleaseAtPlayEndList( sound );
        
        // 再生
		sound->loop( false );
		sound->play();

        LN_SAFE_RELEASE( sound );
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 指定された ISound を SE として演奏する
    //---------------------------------------------------------------------
	LNRESULT GameAudio::playSEFromSound( ISound* sound_, int volume_, int pitch_ )
    {
        LNRESULT lr;
        
        // 受け取った ISound が持っているソースをもとに新しい ISound を作成
        ISound* sound;
        LN_CALL_R( mManager->createSound(
            &sound,
            sound_->getAudioPlayer()->getAudioSource(),
            LN_SOUNDPLAYTYPE_ONMEMORY,
            sound_->is3DSound() ) );

        // 位置・ボリューム・ピッチ設定
        sound->setVolume( volume_ );
        sound->setPitch( pitch_ );
        if ( sound_->is3DSound() )
        {
            sound->setPosition( sound_->getPosition() );
        }

        // 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
        sound->setGroupFlag( LN_AUDIOPLAYER_GROUP_SE );
        _pushReleaseAtPlayEndList( sound );
        
        // 再生
		sound->loop( false );
		sound->play();

        LN_SAFE_RELEASE( sound );
        return LN_OK;
    }

	//---------------------------------------------------------------------
	// ● 全ての SE の演奏を停止する
	//---------------------------------------------------------------------
	void GameAudio::stopSE()
	{
        mManager->stopGroup( LN_AUDIOPLAYER_GROUP_SE );
	}

    //---------------------------------------------------------------------
    // ● ME 演奏時の BGM のフェード時間を設定する
	//---------------------------------------------------------------------
    void GameAudio::setMEFadeState( int begin_, int end_ )
    {
        if ( begin_ >= 0 )
        {
            mBGMFadeOutTime = begin_;
        }
        if ( end_ >= 0 )
        {
            mBGMFadeInTime = end_;
        }

    }

    //---------------------------------------------------------------------
	// ● 更新
	//---------------------------------------------------------------------
	LNRESULT GameAudio::polling()
	{
        LNRESULT lr = LN_OK;

        mLock->lock();

		// 演奏する ME がある場合
		if ( mME )
		{
            LNAudioPlayState mestate;
            mME->getState( &mestate );

			// BGM がある場合
			if ( mBGM )
			{
                LNAudioPlayState bgmstate;
                mBGM->getState( &bgmstate );

				// BGMのフェードアウトが終わって一時停止状態になっている場合
                if ( bgmstate.SoundPlayState == LN_SOUNDPLAYSTATE_PAUSING )
				{
                    // ME 再生開始
					if ( !mMEPlaying )
					{
						mME->loop( false );
						mME->play();
						mMEPlaying = true;
					}
					// ME の再生が終了した場合
                    else if ( mestate.SoundPlayState != LN_SOUNDPLAYSTATE_PLAYING )
					{
						// ME 再生中に BGM がストップしたとかで解放されている場合はなにもしない
						if ( mBGM )
						{
							mBGM->fadeVolume( mBGMVolume, mBGMFadeInTime, LN_SOUNDFADE_CONTINUE );
                            mBGM->pause( false );
						}
						LN_SAFE_RELEASE( mME );
						mMEPlaying = false;
					}
				}
			}
			// BGM がない場合
			else
			{
				// ME が終了した場合
                if ( mestate.SoundPlayState != LN_SOUNDPLAYSTATE_PLAYING )
				{
					LN_SAFE_RELEASE( mME );
                    mMEPlaying = false;
				}
			}
		}

        /// 再生終了している音声はリストから外して解放する
        LNAudioPlayState state;
        ReleaseAtPlayEndList::iterator itr = mReleaseAtPlayEndList.begin();
        ReleaseAtPlayEndList::iterator end = mReleaseAtPlayEndList.end();
        for ( ; itr != end;  )
        {
            (*itr)->getState( &state );
            if ( state.SoundPlayState != LN_SOUNDPLAYSTATE_PLAYING )
            {
                (*itr)->release();
                itr = mReleaseAtPlayEndList.erase( itr );
                end = mReleaseAtPlayEndList.end();
            }
            else
            {
                ++itr;
            }
        }

        mLock->unlock();
        return lr;
	}

    //---------------------------------------------------------------------
	// ● 再生終了時解放リストに追加する
    //---------------------------------------------------------------------
    void GameAudio::_pushReleaseAtPlayEndList( ISound* sound_ )
    {
        if ( sound_ )
        {
            mLock->lock();
            mReleaseAtPlayEndList.push_back( sound_ );
            LN_SAFE_ADDREF( sound_ );
            mLock->unlock();
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