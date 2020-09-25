//=============================================================================
//�y Audio::Sound �z
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
// �� Sound �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
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
    // �� �f�X�g���N�^
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
    // �� ���ʂ̎擾
    //---------------------------------------------------------------------
    int Sound::getVolume()
    {
        mLock->lock();
        int r = mAudioPlayer->getVolume();
        mLock->unlock();
        return r;
    }

	//---------------------------------------------------------------------
    // �� ���ʂ̐ݒ�
    //---------------------------------------------------------------------
    void Sound::setVolume( int volume_ )
    {
        mLock->lock();
        mAudioPlayer->setVolume( volume_ );
        mLock->unlock();
    }

	//---------------------------------------------------------------------
    // �� �s�b�`�̎擾
    //---------------------------------------------------------------------
    int Sound::getPitch()
    {
        mLock->lock();
        int r = mAudioPlayer->getPitch();
        mLock->unlock();
        return r;
    }

	//---------------------------------------------------------------------
    // �� �s�b�`�̐ݒ�
    //---------------------------------------------------------------------
    void Sound::setPitch( int pitch_ )
    {
        mLock->lock();
        mAudioPlayer->setPitch( pitch_ );
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    // �� �T�E���h�̏�Ԃ��擾����
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
 //   // �� �Đ������𔻒肷��
 //   //---------------------------------------------------------------------
 //   bool Sound::isPlaying()
 //   {
 //       mLock->lock();
 //       bool r = 
 //       mLock->unlock();
 //       return r;
 //   }

	////---------------------------------------------------------------------
 //   // �� �ꎞ��~�����𔻒肷��
 //   //---------------------------------------------------------------------
 //   bool Sound::isPausing()
 //   {
 //       mLock->lock();
 //       bool r = mAudioPlayer->isPausing();
 //       mLock->unlock();
 //       return r;
 //   }

    //---------------------------------------------------------------------
    // �� ���[�v�Đ��̗L���ݒ�
    //---------------------------------------------------------------------
    void Sound::loop( bool loop_ )
    {
        mAudioPlayer->loop( loop_ );
    }

    //---------------------------------------------------------------------
    // �� ���[�v�Đ����L�����𔻒肷��
    //---------------------------------------------------------------------
    bool Sound::isLoop()
    {
        return mAudioPlayer->isLoop();
    }

    //---------------------------------------------------------------------
    // �� ���[�v���̐ݒ�
    //---------------------------------------------------------------------
    void Sound::setLoopState( u32 begin_, u32 length_ )
    {
        mLock->lock();
        mAudioPlayer->setLoopState( begin_, length_ );
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    // �� �Đ�
    //---------------------------------------------------------------------
    LNRESULT Sound::play()
    {
        mLock->lock();
        LNRESULT lr = mAudioPlayer->play();
        mLock->unlock();
        return lr;
    }

    //---------------------------------------------------------------------
    // �� ��~
    //---------------------------------------------------------------------
    void Sound::stop()
    {
        mLock->lock();
        mAudioPlayer->stop();
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    // �� �ꎞ��~
    //---------------------------------------------------------------------
    void Sound::pause( bool is_pause_ )
    {
        mLock->lock();
        mAudioPlayer->pause( is_pause_ );
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    // �� ���ʂ̃t�F�[�h
    //---------------------------------------------------------------------
    LNRESULT Sound::fadeVolume( int target_volume_, int time_, LNSoundFadeState state_ )
    {
        mLock->lock();

        mFadeValue.start( static_cast< int >( mAudioPlayer->getVolume() ), target_volume_, static_cast< double >( time_ ) * 0.001 );	// ���݂̉��ʂ��� volume_ �ւ̃t�F�[�h
		mFadeState = state_;
		mIsFading = true;

		mLock->unlock();

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� 3D �������𔻒肷��
    //---------------------------------------------------------------------
    bool Sound::is3DSound()
    {
        return mAudioPlayer->is3DSound();
    }

    //---------------------------------------------------------------------
    // �� 3D �����Ƃ��Ă̈ʒu��ݒ肷��
    //---------------------------------------------------------------------
    LNRESULT Sound::setPosition( const LVector3& pos_ )
    {
        mLock->lock();
        LNRESULT lr = mAudioPlayer->setPosition( pos_ );
        mLock->unlock();
        return lr;
    }

    //---------------------------------------------------------------------
    // �� 3D �����Ƃ��Ă̈ʒu���擾����
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

        // �t�F�[�h���̏ꍇ
		if ( mIsFading )
		{
			mFadeValue.advanceTime( elapsed_time_ );
			
			mAudioPlayer->setVolume( mFadeValue.getValue() );

			// �t�F�[�h����
			if ( mFadeValue.isFinished() )
			{
				mIsFading = false; 

				// �t�F�[�h�I�����̓���ɂ���ĕ���
				switch ( mFadeState )
				{
					// ��~����ꍇ
					case LN_SOUNDFADE_STOP:
						mAudioPlayer->stop();
						break;
					// �ꎞ��~����ꍇ
					case LN_SOUNDFADE_PAUSE:
						mAudioPlayer->pause( true );
						break;
					// ��~���ĉ��ʂ�߂��ꍇ
					case LN_SOUNDFADE_STOP_RESET:
						mAudioPlayer->stop();
						mAudioPlayer->setVolume( mFadeValue.getStartValue() );
						break;
					// �ꎞ��~���ĉ��ʂ�߂��ꍇ
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
// �� Sound �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
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
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    Sound::~Sound()
    {
        // mManager ���c���Ă�����o�O


        LN_SAFE_RELEASE( mAudioPlayer );

        LN_SAFE_RELEASE( mLock );
    }

	//---------------------------------------------------------------------
    // �� ���ʂ̎擾
    //---------------------------------------------------------------------
    int Sound::getVolume()
    {
        return mAudioPlayer->getVolume();
    }

	//---------------------------------------------------------------------
    // �� ���ʂ̐ݒ�
    //---------------------------------------------------------------------
    void Sound::setVolume( int volume_ )
    {
        mAudioPlayer->setVolume( volume_ );
    }

	//---------------------------------------------------------------------
    // �� �s�b�`�̎擾
    //---------------------------------------------------------------------
    int Sound::getPitch()
    {
        return mAudioPlayer->getPitch();
    }

	//---------------------------------------------------------------------
    // �� �s�b�`�̐ݒ�
    //---------------------------------------------------------------------
    void Sound::setPitch( int pitch_ )
    {
        mAudioPlayer->setPitch( pitch_ );
    }

	//---------------------------------------------------------------------
    // �� �Đ������𔻒肷��
    //---------------------------------------------------------------------
    bool Sound::isPlaying()
    {
        return mAudioPlayer->isPlaying();
    }

	//---------------------------------------------------------------------
    // �� �ꎞ��~�����𔻒肷��
    //---------------------------------------------------------------------
    bool Sound::isPausing()
    {
        return mAudioPlayer->isPausing();
    }


    //---------------------------------------------------------------------
    // �� ���[�v���̐ݒ�
    //---------------------------------------------------------------------
    void Sound::setLoopState( bool loop_, int begin_, int length_ )
    {
        mLock->lock();
        mAudioPlayer->setLoopState( loop_, begin_, length_ );
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    // �� �Đ�
    //---------------------------------------------------------------------
    void Sound::play()
    {
        mLock->lock();
        mAudioPlayer->play();
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    // �� ��~
    //---------------------------------------------------------------------
    void Sound::stop()
    {
        mLock->lock();
        mAudioPlayer->stop();
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    // �� �ꎞ��~
    //---------------------------------------------------------------------
    void Sound::pause( bool is_pause_ )
    {
        mLock->lock();
        mAudioPlayer->pause( is_pause_ );
        mLock->unlock();
    }

    //---------------------------------------------------------------------
    // �� ���ʂ̃t�F�[�h
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