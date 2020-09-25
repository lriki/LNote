//=============================================================================
//【 LGameAudio 】
//=============================================================================

#include "stdafx.h"
#include "../../Core/Framework/Framework.h"
#include "../../Core/Audio/Manager.h"
#include "../../Core/Audio/GameAudio.h"
#include "LGameAudio.h"
#include "LSound.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// ■ LGameAudio クラス
//=============================================================================

    // インスタンスの取得
    LGameAudio LGameAudio::getInstance()
    {
        LGameAudio o;
        if ( gFramework->getAudioManager() )
        {
		    o._impl = gFramework->getAudioManager()->getGameAudio();
        }
		return o;
    }

    // BGM の演奏を開始する
    void LGameAudio::playBGM( const char* filename_, int volume_, int pitch_, int fade_time_ )
    {
        _impl->playBGM( filename_, volume_, pitch_, fade_time_ );
    }

    // BGM の演奏を停止する
	void LGameAudio::stopBGM( int fade_time_ )
    {
        _impl->stopBGM( fade_time_ );
    }

    // BGS ( 環境音 ) の演奏を開始する
	void LGameAudio::playBGS( const char* filename_, int volume_, int pitch_, int fade_time_ )
    {
        _impl->playBGS( filename_, volume_, pitch_, fade_time_ );
    }

    // BGS の演奏を停止する
	void LGameAudio::stopBGS( int fade_time_ )
    {
        _impl->stopBGS( fade_time_ );
    }

    // ME ( 効果音楽 ) の演奏を開始する
	void LGameAudio::playME( const char* filename_, int volume_, int pitch_ )
    {
        _impl->playME( filename_, volume_, pitch_ );
    }

    // ME の演奏を停止する
	void LGameAudio::stopME()
    {
        _impl->stopME();
    }

    // SE の演奏を開始する
	void LGameAudio::playSE( const char* filename_, int volume_, int pitch_ )
    {
        _impl->playSE( filename_, volume_, pitch_ );
    }

    // SE を 3D 空間上で再生する
    void LGameAudio::playSE( const char* filename_, const LVector3& position_, int volume_, int pitch_ )
    {
        _impl->playSE( filename_, position_, volume_, pitch_ );
    }

    // 全ての SE の演奏を停止する
	void LGameAudio::stopSE()
    {
        _impl->stopSE();
    }

    // ME 演奏時の BGM のフェード時間を設定する
    void LGameAudio::setMEFadeState( int begin_, int end_ )
    {
        _impl->setMEFadeState( begin_, end_ );
    }

    // 3D 音源のリスナー情報を設定する
    //void LGameAudio::setListenerState( const LVector3& position_, const LVector3& front_ )
    //{
    //    gFramework->getAudioManager()->getAudioDevice()->setListenerState( position_, front_ );
    //}

    // 3D 音源の可聴距離の設定
    //void LGameAudio::setListenableDistance( lnfloat length_ )
    //{
    //    gFramework->getAudioManager()->getAudioDevice()->setListenableDistance( length_ );
    //}

//=============================================================================
// ■ LSound クラス
//=============================================================================


    // 音声を作成する
    LSound LSound::create( const char* filename_, LNSoundPlayType type_ )
    {
        LSound o;
        LN_PCALL( gFramework->getAudioManager()->createSound( &o._impl, filename_, type_, false ) );
        return o;
    }

    // 3D 音源として音声を作成する
    LSound LSound::create3D( const char* filename_, LNSoundPlayType type_ )
    {
        LSound o;
        LN_PCALL( gFramework->getAudioManager()->createSound( &o._impl, filename_, type_, true ) );
        return o;
    }

    // 音量の取得 (0～100)
	int LSound::getVolume()
    {
        return _impl->getVolume();
    }

	// 音量の設定 (0～100)
	void LSound::setVolume( int volume_ )
    {
        _impl->setVolume( volume_ );
    }

	// ピッチの取得 (50～200)
	int LSound::getPitch()
    {
        return _impl->getPitch();
    }

	// ピッチの設定 (50～200)
	void LSound::setPitch( int pitch_ )
    {
        _impl->setPitch( pitch_ );
    }

    // サウンドの状態を取得する
    void LSound::getState( LNAudioPlayState* state_ ) const
    {
        _impl->getState( state_ );
    }

    // ループ再生の有効設定
    void LSound::loop( bool loop_ )
    {
        _impl->loop( loop_ );
    }

    // ループ再生が有効かを判定する
    bool LSound::isLoop()
    {
        return _impl->isLoop();
    }

    // ループ情報の設定
	void LSound::setLoopState( u32 begin_, u32 length_ )
    {
        _impl->setLoopState( begin_, length_ );
    }

    // 再生
	void LSound::play()
    {
        _impl->play();
    }

    // 停止
	void LSound::stop()
    {
        _impl->stop();
    }

    // 一時停止
	void LSound::pause( bool is_pause_ )
    {
        _impl->pause( is_pause_ );
    }

    // 音量のフェード
	void LSound::fadeVolume( int target_volume_, int time_, LNSoundFadeState state_ )
    {
        _impl->fadeVolume( target_volume_, time_, state_ );
    }

    // 3D 音源としての位置を設定する
    void LSound::setPosition( const LVector3& pos_ )
    {
        _impl->setPosition( pos_ );
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================