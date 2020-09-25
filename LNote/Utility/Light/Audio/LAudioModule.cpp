//=============================================================================
//�y LGameAudio �z
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
// �� LGameAudio �N���X
//=============================================================================

    // �C���X�^���X�̎擾
    LGameAudio LGameAudio::getInstance()
    {
        LGameAudio o;
        if ( gFramework->getAudioManager() )
        {
		    o._impl = gFramework->getAudioManager()->getGameAudio();
        }
		return o;
    }

    // BGM �̉��t���J�n����
    void LGameAudio::playBGM( const char* filename_, int volume_, int pitch_, int fade_time_ )
    {
        _impl->playBGM( filename_, volume_, pitch_, fade_time_ );
    }

    // BGM �̉��t���~����
	void LGameAudio::stopBGM( int fade_time_ )
    {
        _impl->stopBGM( fade_time_ );
    }

    // BGS ( ���� ) �̉��t���J�n����
	void LGameAudio::playBGS( const char* filename_, int volume_, int pitch_, int fade_time_ )
    {
        _impl->playBGS( filename_, volume_, pitch_, fade_time_ );
    }

    // BGS �̉��t���~����
	void LGameAudio::stopBGS( int fade_time_ )
    {
        _impl->stopBGS( fade_time_ );
    }

    // ME ( ���ʉ��y ) �̉��t���J�n����
	void LGameAudio::playME( const char* filename_, int volume_, int pitch_ )
    {
        _impl->playME( filename_, volume_, pitch_ );
    }

    // ME �̉��t���~����
	void LGameAudio::stopME()
    {
        _impl->stopME();
    }

    // SE �̉��t���J�n����
	void LGameAudio::playSE( const char* filename_, int volume_, int pitch_ )
    {
        _impl->playSE( filename_, volume_, pitch_ );
    }

    // SE �� 3D ��ԏ�ōĐ�����
    void LGameAudio::playSE( const char* filename_, const LVector3& position_, int volume_, int pitch_ )
    {
        _impl->playSE( filename_, position_, volume_, pitch_ );
    }

    // �S�Ă� SE �̉��t���~����
	void LGameAudio::stopSE()
    {
        _impl->stopSE();
    }

    // ME ���t���� BGM �̃t�F�[�h���Ԃ�ݒ肷��
    void LGameAudio::setMEFadeState( int begin_, int end_ )
    {
        _impl->setMEFadeState( begin_, end_ );
    }

    // 3D �����̃��X�i�[����ݒ肷��
    //void LGameAudio::setListenerState( const LVector3& position_, const LVector3& front_ )
    //{
    //    gFramework->getAudioManager()->getAudioDevice()->setListenerState( position_, front_ );
    //}

    // 3D �����̉������̐ݒ�
    //void LGameAudio::setListenableDistance( lnfloat length_ )
    //{
    //    gFramework->getAudioManager()->getAudioDevice()->setListenableDistance( length_ );
    //}

//=============================================================================
// �� LSound �N���X
//=============================================================================


    // �������쐬����
    LSound LSound::create( const char* filename_, LNSoundPlayType type_ )
    {
        LSound o;
        LN_PCALL( gFramework->getAudioManager()->createSound( &o._impl, filename_, type_, false ) );
        return o;
    }

    // 3D �����Ƃ��ĉ������쐬����
    LSound LSound::create3D( const char* filename_, LNSoundPlayType type_ )
    {
        LSound o;
        LN_PCALL( gFramework->getAudioManager()->createSound( &o._impl, filename_, type_, true ) );
        return o;
    }

    // ���ʂ̎擾 (0�`100)
	int LSound::getVolume()
    {
        return _impl->getVolume();
    }

	// ���ʂ̐ݒ� (0�`100)
	void LSound::setVolume( int volume_ )
    {
        _impl->setVolume( volume_ );
    }

	// �s�b�`�̎擾 (50�`200)
	int LSound::getPitch()
    {
        return _impl->getPitch();
    }

	// �s�b�`�̐ݒ� (50�`200)
	void LSound::setPitch( int pitch_ )
    {
        _impl->setPitch( pitch_ );
    }

    // �T�E���h�̏�Ԃ��擾����
    void LSound::getState( LNAudioPlayState* state_ ) const
    {
        _impl->getState( state_ );
    }

    // ���[�v�Đ��̗L���ݒ�
    void LSound::loop( bool loop_ )
    {
        _impl->loop( loop_ );
    }

    // ���[�v�Đ����L�����𔻒肷��
    bool LSound::isLoop()
    {
        return _impl->isLoop();
    }

    // ���[�v���̐ݒ�
	void LSound::setLoopState( u32 begin_, u32 length_ )
    {
        _impl->setLoopState( begin_, length_ );
    }

    // �Đ�
	void LSound::play()
    {
        _impl->play();
    }

    // ��~
	void LSound::stop()
    {
        _impl->stop();
    }

    // �ꎞ��~
	void LSound::pause( bool is_pause_ )
    {
        _impl->pause( is_pause_ );
    }

    // ���ʂ̃t�F�[�h
	void LSound::fadeVolume( int target_volume_, int time_, LNSoundFadeState state_ )
    {
        _impl->fadeVolume( target_volume_, time_, state_ );
    }

    // 3D �����Ƃ��Ă̈ʒu��ݒ肷��
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