//=============================================================================
//�y GameAudio �z
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
// �� GameAudio �N���X
//=============================================================================

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
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
	// �� �f�X�g���N�^
	//---------------------------------------------------------------------
	GameAudio::~GameAudio()
	{
        stopBGM( 0 );
        stopBGS( 0 );
        stopME();
		stopSE();
		_p(mReleaseAtPlayEndList.size());
        // �Đ��I����̉�����X�g�ɓ����Ă���T�E���h�����
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
	// �� BGM �̉��t���J�n����
	//---------------------------------------------------------------------
	LNRESULT GameAudio::playBGM( const lnChar* filename_, int volume_, int pitch_, int fade_time_ )
	{
        // ���t�ĊJ�`�F�b�N
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

        // �t�@�C�����L��
        mBGMName = filename_;
        
    ERR_EXIT:
        LN_SAFE_RELEASE( sound );
        return lr;
	}

    //---------------------------------------------------------------------
	// �� �w�肳�ꂽ ISound �� BGM �Ƃ��ĉ��t����
    //---------------------------------------------------------------------
	LNRESULT GameAudio::playBGMFromSound( ISound* sound_, int volume_, int pitch_, int fade_time_ )
    {
		
        // ���t�ĊJ�`�F�b�N
		// TODO:�t�F�[�h�A�E�g���ɍĊJ����Ɩ����ɂȂ�
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


        // ME ���Đ����ł͂Ȃ����𒲂ׂ�
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

		// ME �Đ����ł͂Ȃ��ꍇ
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
        // ME �Đ����̏ꍇ�́A��x�Đ����邯�ǂ����Ɉꎞ��~���� ( ME �I����ɍĊJ )
		else
		{
			mBGM->play();
			mBGM->pause( true );
		}

        // �t�F�[�h���Ԃ�����ꍇ
        if ( fade_time_ > 0 )
		{
			if ( prev_bgm )
			{
				// �ЂƂO��BGM�́Afade_time_ ��ɒ�~�A�������悤�ɂ���
				prev_bgm->loop( false );
				prev_bgm->fadeVolume( 0, fade_time_, LN_SOUNDFADE_STOP_RESET );
                _pushReleaseAtPlayEndList( prev_bgm );
			}
		}
        // �t�F�[�h���Ԃ��Ȃ��ꍇ�͂����ɒ�~
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
	// �� BGM �̉��t���~����
	//---------------------------------------------------------------------
	void GameAudio::stopBGM( int fade_time_ )
	{
		if ( mBGM )
		{
			if ( fade_time_ > 0 )
		    {
                // �t�F�[�h�I����ɒ�~���āA���ʓ������ɖ߂�
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
	// �� BGS ( ���� ) �̉��t���J�n����
	//---------------------------------------------------------------------
	LNRESULT GameAudio::playBGS( const lnChar* filename_, int volume_, int pitch_, int fade_time_ )
    {
        // ���t�ĊJ�`�F�b�N
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
        
        // �t�@�C�����L��
        mBGSName = filename_;

    ERR_EXIT:
        LN_SAFE_RELEASE( sound );
        return lr;
    }

    //---------------------------------------------------------------------
	// �� �w�肳�ꂽ ISound �� BGS �Ƃ��ĉ��t����
    //---------------------------------------------------------------------
	LNRESULT GameAudio::playBGSFromSound( ISound* sound_, int volume_, int pitch_, int fade_time_ )
    {
        // ���t�ĊJ�`�F�b�N
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

        // �t�F�[�h���Ԃ�����ꍇ
        if ( fade_time_ > 0 )
		{
			if ( prev_bgs )
			{
				// �ЂƂO��BGS�́Afade_time_ ��ɒ�~�A�������悤�ɂ���
				prev_bgs->loop( false );
				prev_bgs->fadeVolume( 0, fade_time_, LN_SOUNDFADE_STOP_RESET );
                _pushReleaseAtPlayEndList( prev_bgs );
			}
		}
        // �t�F�[�h���Ԃ��Ȃ��ꍇ�͂����ɒ�~
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
	// �� BGS �̉��t���~����
	//---------------------------------------------------------------------
	void GameAudio::stopBGS( int fade_time_ )
	{
		if ( mBGS )
		{
			if ( fade_time_ > 0 )
		    {
                // �t�F�[�h�I����ɒ�~���āA���ʓ������ɖ߂�
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
	// �� ME (���ʉ��y) �̉��t���J�n����
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
	// �� �w�肳�ꂽ ISound �� ME �Ƃ��ĉ��t����
    //---------------------------------------------------------------------
	LNRESULT GameAudio::playMEFromSound( ISound* sound_, int volume_, int pitch_ )
    {
        stopME();

        mME = sound_;
        LN_SAFE_ADDREF( mME );

        mME->setPitch( pitch_ );
		mME->loop( true );
        mME->setGroupFlag( 0 );

        bool flag = false;  // BGM ������A�Đ�����Ă��邩�������t���O

		// BGM ������ꍇ
		if ( mBGM )
		{
            LNAudioPlayState state;
            mBGM->getState( &state );

			// �Đ�����Ă���ꍇ
            if ( state.SoundPlayState == LN_SOUNDPLAYSTATE_PLAYING )
			{
				flag = true;
			}
		}

        // BGM ���Đ����̏ꍇ
        if ( flag )
		{
			// �t�F�[�h�A�E�g���Ԃ�����ꍇ
			if ( mBGMFadeOutTime > 0 )
			{
				// �t�F�[�h�A�E�g��A�ꎞ��~����
				mBGM->fadeVolume( 0, mBGMFadeOutTime, LN_SOUNDFADE_PAUSE );
			}
			// �t�F�[�h�A�E�g���Ԃ��Ȃ��ꍇ
			else
			{
				// �����Ɉꎞ��~
				mBGM->pause( true );
			}
		}
        // BGM ���Ȃ��ꍇ�͂������t�J�n
		else
		{
			mME->play();
			mMEPlaying = true;
		}

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� ME �̉��t���~����
	//---------------------------------------------------------------------
	void GameAudio::stopME()
	{
		if ( mME )
		{
			mME->stop();
            LN_SAFE_RELEASE( mME );
			
			// BGM �������āA�ꎞ��~���̏ꍇ�͍ĊJ
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
	// �� SE �̉��t���J�n����
	//---------------------------------------------------------------------
	LNRESULT GameAudio::playSE( const lnChar* filename_, int volume_, int pitch_ )
	{
        LNRESULT lr;

        // �T�E���h�쐬
        ISound* sound;
        LN_CALL_R( mManager->createSound( &sound, filename_, LN_SOUNDPLAYTYPE_ONMEMORY, false ) );

        // �{�����[���E�s�b�`�ݒ�
        sound->setVolume( volume_ );
        sound->setPitch( pitch_ );

        // �Đ��r���ŉ������悤�Ƃ��Ă��Đ��I���܂ł͉������Ȃ� & SE �Ƃ��čĐ�����
        //sound->setPlayerFlags( LN_AUDIOPLAYER_AFTERRELEASE );
        _pushReleaseAtPlayEndList( sound );

        sound->setGroupFlag( LN_AUDIOPLAYER_GROUP_SE );

        // �Đ�
		sound->loop( false );
		sound->play();

        LN_SAFE_RELEASE( sound );
        return LN_OK;
	}

    //---------------------------------------------------------------------
	// �� SE �� 3D ��ԏ�ōĐ�����
    //---------------------------------------------------------------------
    LNRESULT GameAudio::playSE( const lnChar* filename_, const LVector3& position_, float distance, int volume_, int pitch_ )
    {
        LNRESULT lr;

        // �T�E���h�쐬
        ISound* sound;
        LN_CALL_R( mManager->createSound( &sound, filename_, LN_SOUNDPLAYTYPE_ONMEMORY, true ) );

        // �ʒu�E�{�����[���E�s�b�`�ݒ�
        sound->setPosition( position_ );
        sound->setMaxDistance( distance );
        sound->setVolume( volume_ );
        sound->setPitch( pitch_ );

        // �Đ��r���ŉ������悤�Ƃ��Ă��Đ��I���܂ł͉������Ȃ� & SE �Ƃ��čĐ�����
        sound->setGroupFlag( LN_AUDIOPLAYER_GROUP_SE );
        _pushReleaseAtPlayEndList( sound );
        
        // �Đ�
		sound->loop( false );
		sound->play();

        LN_SAFE_RELEASE( sound );
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� �w�肳�ꂽ ISound �� SE �Ƃ��ĉ��t����
    //---------------------------------------------------------------------
	LNRESULT GameAudio::playSEFromSound( ISound* sound_, int volume_, int pitch_ )
    {
        LNRESULT lr;
        
        // �󂯎���� ISound �������Ă���\�[�X�����ƂɐV���� ISound ���쐬
        ISound* sound;
        LN_CALL_R( mManager->createSound(
            &sound,
            sound_->getAudioPlayer()->getAudioSource(),
            LN_SOUNDPLAYTYPE_ONMEMORY,
            sound_->is3DSound() ) );

        // �ʒu�E�{�����[���E�s�b�`�ݒ�
        sound->setVolume( volume_ );
        sound->setPitch( pitch_ );
        if ( sound_->is3DSound() )
        {
            sound->setPosition( sound_->getPosition() );
        }

        // �Đ��r���ŉ������悤�Ƃ��Ă��Đ��I���܂ł͉������Ȃ� & SE �Ƃ��čĐ�����
        sound->setGroupFlag( LN_AUDIOPLAYER_GROUP_SE );
        _pushReleaseAtPlayEndList( sound );
        
        // �Đ�
		sound->loop( false );
		sound->play();

        LN_SAFE_RELEASE( sound );
        return LN_OK;
    }

	//---------------------------------------------------------------------
	// �� �S�Ă� SE �̉��t���~����
	//---------------------------------------------------------------------
	void GameAudio::stopSE()
	{
        mManager->stopGroup( LN_AUDIOPLAYER_GROUP_SE );
	}

    //---------------------------------------------------------------------
    // �� ME ���t���� BGM �̃t�F�[�h���Ԃ�ݒ肷��
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
	// �� �X�V
	//---------------------------------------------------------------------
	LNRESULT GameAudio::polling()
	{
        LNRESULT lr = LN_OK;

        mLock->lock();

		// ���t���� ME ������ꍇ
		if ( mME )
		{
            LNAudioPlayState mestate;
            mME->getState( &mestate );

			// BGM ������ꍇ
			if ( mBGM )
			{
                LNAudioPlayState bgmstate;
                mBGM->getState( &bgmstate );

				// BGM�̃t�F�[�h�A�E�g���I����Ĉꎞ��~��ԂɂȂ��Ă���ꍇ
                if ( bgmstate.SoundPlayState == LN_SOUNDPLAYSTATE_PAUSING )
				{
                    // ME �Đ��J�n
					if ( !mMEPlaying )
					{
						mME->loop( false );
						mME->play();
						mMEPlaying = true;
					}
					// ME �̍Đ����I�������ꍇ
                    else if ( mestate.SoundPlayState != LN_SOUNDPLAYSTATE_PLAYING )
					{
						// ME �Đ����� BGM ���X�g�b�v�����Ƃ��ŉ������Ă���ꍇ�͂Ȃɂ����Ȃ�
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
			// BGM ���Ȃ��ꍇ
			else
			{
				// ME ���I�������ꍇ
                if ( mestate.SoundPlayState != LN_SOUNDPLAYSTATE_PLAYING )
				{
					LN_SAFE_RELEASE( mME );
                    mMEPlaying = false;
				}
			}
		}

        /// �Đ��I�����Ă��鉹���̓��X�g����O���ĉ������
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
	// �� �Đ��I����������X�g�ɒǉ�����
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