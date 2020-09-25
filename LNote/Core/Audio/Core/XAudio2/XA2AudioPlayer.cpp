//=============================================================================
//�y XA2AudioPlayer �z
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
// �� OnMemory �N���X
//=============================================================================

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
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
	// �� �f�X�g���N�^
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
	// �� ������
	//---------------------------------------------------------------------
    LNRESULT OnMemory::initialize( IAudioSource* audio_source_, bool enable_3d_ )
	{
        LN_SETERR_R_S_( ( !mAudioSource ), LN_ERR_ASSERT, "< OnMemory::setAudioSource >\nmAudioSource is not NULL" );

		mAudioSource = audio_source_;
		mAudioSource->addRef();

		// �I���������Đ��p�ɓ����Ɏ����Ă�o�b�t�@�𖄂߂�
		mAudioSource->fillBufferSafe();

		// 3D �����Ƃ��č쐬���Ă���ꍇ�͂��̏����i�[����\���̂����
		if ( enable_3d_ )
		{
            mEmitterState = LN_NEW EmitterState( mAudioSource->getWaveFormat()->Channels );
		}

        // �\�[�X���烋�[�v�ʒu���擾
        u32 loop_begin, loop_length;
        mAudioSource->getLoopState( &loop_begin, &loop_length );
        mLoopBegin  = loop_begin;
		mLoopLength = loop_length;

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� ���ʂ̎擾
	//---------------------------------------------------------------------
	int OnMemory::getVolume() const
	{
		return static_cast< int >( mVolume );
	}

	//---------------------------------------------------------------------
	// �� ���ʂ̐ݒ�
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
	// �� �s�b�`�̎擾
	//---------------------------------------------------------------------
	int OnMemory::getPitch() const
	{
		return static_cast< int >( mPitch );
	}

	//---------------------------------------------------------------------
	// �� �s�b�`�̐ݒ�
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
	// �� �Đ�������
	//---------------------------------------------------------------------
	bool OnMemory::isPlaying() const
	{
		return mIsPlaying;
	}

	//---------------------------------------------------------------------
	// �� �ꎞ��~������
	//---------------------------------------------------------------------
	bool OnMemory::isPausing() const
	{
		return mIsPausing;
	}

    //---------------------------------------------------------------------
	// �� �Đ������T���v�����̎擾
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
	// �� ���[�v���̐ݒ�
	//---------------------------------------------------------------------
	LNRESULT OnMemory::setLoopState( u32 loop_begin_, u32 loop_length_ )
	{
        if ( loop_begin_ == 0 && loop_length_ == 0 )
        {
            // �\�[�X���烋�[�v�ʒu���擾���Đݒ肷��
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
	// �� �Đ�
	//---------------------------------------------------------------------
	LNRESULT OnMemory::play()
	{
        stop();

        // XAudio �̃\�[�X�{�C�X�쐬
        HRESULT hr;
        const LNWaveFormat* ln_format = mAudioSource->getWaveFormat();
		WAVEFORMATEX format;
        AudioUtil::convLNWaveFormatToWAVEFORMATEX( *ln_format, &format );
        LN_COMCALL( mManager->getXAudio2()->CreateSourceVoice( &mSourceVoice, &format ) );
		LN_COMCALL( mSourceVoice->FlushSourceBuffers() );
        
        // SourceVoice�ɑ��M����f�[�^
        XAUDIO2_BUFFER submit = { 0 };
		submit.AudioBytes = mAudioSource->getOnmemoryPCMBufferSize();	// �o�b�t�@�̃o�C�g��
		submit.pAudioData = (u8*)mAudioSource->getOnmemoryPCMBuffer();	// �o�b�t�@�̐擪�A�h���X
		submit.Flags = XAUDIO2_END_OF_STREAM;

		// ���[�v�Đ�����ꍇ
		if ( mIsLoop )
		{
			submit.LoopCount	= XAUDIO2_LOOP_INFINITE;	// �������[�v
			submit.LoopBegin	= mLoopBegin;
			submit.LoopLength	= mLoopLength;
		}
		// ���[�v�Đ����Ȃ��ꍇ
		else
		{
			submit.LoopBegin = XAUDIO2_NO_LOOP_REGION;
		}

		// �{�C�X�L���[�Ƀf�[�^�𑗂�
		LN_COMCALL( mSourceVoice->SubmitSourceBuffer( &submit ) );

        LN_COMCALL( mSourceVoice->SetVolume( mVolume * 0.01f ) );
        LN_COMCALL( mSourceVoice->SetFrequencyRatio( mPitch * 0.01f ) );

		// �Đ��J�n
		LN_COMCALL( mSourceVoice->Start() );

		mIsPlaying = true;
		mIsPausing = false;

		polling();	// 3D �I�[�f�B�I�̌v�Z���s������

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� ��~
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
	// �� �ꎞ��~
	//---------------------------------------------------------------------
	LNRESULT OnMemory::pause( bool is_pause_ )
	{
		// �Đ����̏ꍇ
		if ( mSourceVoice && mIsPlaying )
		{
			// �ꎞ��~����ꍇ�ŁA���݈ꎞ��~���Ă��Ȃ��ꍇ
			if ( is_pause_ && !mIsPausing )
			{
				mSourceVoice->Stop( 0 );
				mIsPausing = true;
			}
			// �ꎞ��~����������ꍇ�ŁA���݈ꎞ��~���Ă���ꍇ
			else if ( !is_pause_ && mIsPausing )
			{
				mSourceVoice->Start( 0 );
				mIsPausing = false;
			}
		}
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� �|�[�����O�X�V
	//---------------------------------------------------------------------
	bool OnMemory::polling()
	{
		// �Đ����ł͂Ȃ��ꍇ�͒��f
		if ( !mSourceVoice || !mIsPlaying || mIsPausing )
		{
			return false;
		}

		// ���[�v�Đ��ł͂Ȃ��ꍇ
		if ( !mIsLoop )
		{
			XAUDIO2_VOICE_STATE state;
			mSourceVoice->GetState( &state );

			// �{�C�X�L���[����ɂȂ����ꍇ
			if ( state.BuffersQueued == 0 )
			{
				stop();
				return false;
			}
		}

        // 3D �I�[�f�B�I�̍X�V
        if ( mEmitterState )
        {
            mManager->calcEmitterState( mEmitterState );

		    mSourceVoice->SetFrequencyRatio( mEmitterState->DSPSettings.DopplerFactor * ( mPitch * 0.01f ) );

		    mSourceVoice->SetOutputMatrix(
			    mManager->getMasteringVoice(),   // NULL �ł������݂��������ǈꉞ
			    mEmitterState->DSPSettings.SrcChannelCount,
			    mEmitterState->DSPSettings.DstChannelCount,
			    mEmitterState->DSPSettings.pMatrixCoefficients );
        }

        return true;
    }

    //---------------------------------------------------------------------
	// �� 3D �����Ƃ��Ă̈ʒu��ݒ肷��
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
// �� Streaming �N���X
//=============================================================================

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
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
	// �� �f�X�g���N�^
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
	// �� ������
	//---------------------------------------------------------------------
    LNRESULT Streaming::initialize( IAudioSource* audio_source_, bool enable_3d_)
	{
        LN_SETERR_R_S_( ( !mAudioSource ), LN_ERR_ASSERT, "< Streaming::setAudioSource >\nmAudioSource is not NULL" );

		mAudioSource = audio_source_;
		mAudioSource->addRef();

		// 3D �����Ƃ��č쐬���Ă���ꍇ�͂��̏����i�[����N���X�����
		if ( enable_3d_ )
		{
            mEmitterState = LN_NEW EmitterState( mAudioSource->getWaveFormat()->Channels );
		}

		// 1 �b���̃o�b�t�@�T�C�Y�擾
		mAudioDataBufferSize = mAudioSource->getBytesPerSec();

		// 1 �b�̃o�b�t�@���ӂ��p��
		mPrimaryAudioData	= LN_NEW u8[ mAudioDataBufferSize ];
		mSecondaryAudioData = LN_NEW u8[ mAudioDataBufferSize ];

        // �\�[�X���烋�[�v�ʒu���擾���Đݒ肷��
        u32 loop_begin, loop_length;
        mAudioSource->getLoopState( &loop_begin, &loop_length );
        mLoopBegin  = loop_begin;
		mLoopLength = loop_length;

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� ���ʂ̎擾
	//---------------------------------------------------------------------
	int Streaming::getVolume() const
	{
		return static_cast< int >( mVolume );
	}

	//---------------------------------------------------------------------
	// �� ���ʂ̐ݒ�
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
	// �� �s�b�`�̎擾
	//---------------------------------------------------------------------
	int Streaming::getPitch() const
	{
		return static_cast< int >( mPitch );
	}

	//---------------------------------------------------------------------
	// �� �s�b�`�̐ݒ�
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
	// �� �Đ������T���v�����̎擾
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
	// �� �Đ������𔻒肷��
	//---------------------------------------------------------------------
	bool Streaming::isPlaying() const
	{
		return mIsPlaying;
	}

	//---------------------------------------------------------------------
	// �� �ꎞ��~�����𔻒肷��
	//---------------------------------------------------------------------
	bool Streaming::isPausing() const
	{
		return mIsPausing;
	}

	//---------------------------------------------------------------------
	// �� ���[�v���̐ݒ�
	//---------------------------------------------------------------------
	LNRESULT Streaming::setLoopState( u32 loop_begin_, u32 loop_length_ )
	{
        if ( loop_begin_ == 0 && loop_length_ == 0 )
        {
            // �\�[�X���烋�[�v�ʒu���擾���Đݒ肷��
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
	// �� �Đ�
	//---------------------------------------------------------------------
	LNRESULT Streaming::play()
	{
        stop();

        // �\�[�X�{�C�X�쐬
        HRESULT hr;
        const LNWaveFormat* ln_format = mAudioSource->getWaveFormat();
		WAVEFORMATEX format;
        AudioUtil::convLNWaveFormatToWAVEFORMATEX( *ln_format, &format );
        LN_COMCALL( mManager->getXAudio2()->CreateSourceVoice( &mSourceVoice, &format ) );
        mSourceVoice->FlushSourceBuffers();

		// �f�R�[�h��Ԃ����Z�b�g���� ( MP3 �p )
		mAudioSource->reset();

		// �X�g���[�~���O�ǂݍ��݈ʒu���Z�b�g
		mReadCursor = 0;

        // �Ƃ肠�����\�[�X�f�[�^���J�[�\���̏I�[�ɂ���B
        // �t�@�C��������ǂނ̂� _addNextBuffer() ��
        mCursorEndPos = mAudioSource->getSourceDataSize();
        
        mWriteSampleNum = 0;
        mCursorBeginPos = 0;
        mLoopBeginBlockOffset = 0;

		// �ŏ��̃f�[�^���L���[�ɒǉ�����
		_addNextBuffer();

        mSourceVoice->SetVolume( mVolume * 0.01f );
        mSourceVoice->SetFrequencyRatio( mPitch * 0.01f );

		// �Đ��J�n
		LN_DXCALL_R( mSourceVoice->Start() );

		mIsPlaying = true;
		mIsPausing = false;

		polling();	// 3D �I�[�f�B�I�̌v�Z���s������

        return LN_OK;

	}

	//---------------------------------------------------------------------
	// �� ��~
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
	// �� �ꎞ��~
	//---------------------------------------------------------------------
	LNRESULT Streaming::pause( bool is_pause_ )
	{
		// �Đ����̏ꍇ
		if ( mSourceVoice && mIsPlaying )
		{
			// �ꎞ��~����ꍇ�ŁA���݈ꎞ��~���Ă��Ȃ��ꍇ
			if ( is_pause_ && !mIsPausing )
			{
				mSourceVoice->Stop( 0 );
				mIsPausing = true;
			}
			// �ꎞ��~����������ꍇ�ŁA���݈ꎞ��~���Ă���ꍇ
			else if ( !is_pause_ && mIsPausing )
			{
				mSourceVoice->Start( 0 );
				mIsPausing = false;
			}
		}

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� �|�[�����O�X�V
	//---------------------------------------------------------------------
	bool Streaming::polling()
	{
		// �Đ����ł͂Ȃ��ꍇ�͒��f
		if ( !mSourceVoice || !mIsPlaying || mIsPausing )
		{
			return false;
		}

		// ��Ԃ̎擾
		XAUDIO2_VOICE_STATE voice_state;
		mSourceVoice->GetState( &voice_state );

		// �Đ����I�����Ă��炸�A�L���[�Ƀf�[�^���ӂ����܂��Ă��Ȃ��ꍇ
		if ( !mEOF && voice_state.BuffersQueued < 2 )
		{
			// �ǂݍ��݃J�[�\�����I�[�𒴂��Ă���ꍇ
			if ( mReadCursor >= mCursorEndPos )
			{
				// ���[�v�Đ��̏ꍇ�A���ɃL���[�ɒǉ�����f�[�^�̓��[�v�̈�̐擪�ł��邽�߁A�ǂݍ��݃J�[�\���ʒu��擪�ɖ߂�
				if ( mIsLoop )
				{
					mReadCursor = mCursorBeginPos;
                    mCursorEndPos = mAudioSource->getSourceDataSize();
				}
				// ���[�v�Đ����Ȃ��ꍇ�̓t�@�C���̍Ō�܂œǂݐ؂������Ƃ������t���O�� ON
				else
				{
					mEOF = true;
				}
			}

			// �t�@�C���̍Ō�܂œǂݐ؂��Ă��Ȃ��ꍇ
			if ( !mEOF )
			{
				// ���̃f�[�^���Đ��L���[�ɒǉ�
				_addNextBuffer();
			}
		}

		// �񃋁[�v�Đ��ŁA�t�@�C���̍Ō�܂œǂݐ؂��Ă���ꍇ ( �Đ��I�����Ă���ꍇ )
		if ( !mIsLoop && mEOF )
		{
			// �L���[�����S�ɂȂ��Ȃ����ꍇ�A��~�Ƃ���
			if ( voice_state.BuffersQueued == 0 )
			{
				stop();
				return false;
			}
		}

        // 3D �I�[�f�B�I�̍X�V
        if ( mEmitterState )
        {
            mManager->calcEmitterState( mEmitterState );

		    mSourceVoice->SetFrequencyRatio( mEmitterState->DSPSettings.DopplerFactor * ( mPitch * 0.01f ) );

		    mSourceVoice->SetOutputMatrix(
			    mManager->getMasteringVoice(),   // NULL �ł������݂��������ǈꉞ
			    mEmitterState->DSPSettings.SrcChannelCount,
			    mEmitterState->DSPSettings.DstChannelCount,
			    mEmitterState->DSPSettings.pMatrixCoefficients );
        }

		return true;
	}


    //---------------------------------------------------------------------
	// �� 3D �����Ƃ��Ă̈ʒu��ݒ肷��
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
	// �� �Z�J���_���ɐV�����f�[�^����������ŁA�Đ��L���[�ɒǉ�����
	//---------------------------------------------------------------------
	LNRESULT Streaming::_addNextBuffer()
	{
        HRESULT hr;

#if 0
        // �ǂݍ��݈ʒu�Ɉړ�
		mAudioSource->seek( mReadCursor );

		// �f�[�^�ǂݍ���
		u32 read_size;		// �Ă񂾃T�C�Y
		u32 write_size;		// �f�R�[�h���ď������񂾃T�C�Y
		mAudioSource->read( mSecondaryAudioData, mAudioDataBufferSize, &read_size, &write_size );
#endif

		// �f�[�^�ǂݍ���
		u32 read_size;		// �Ă񂾃T�C�Y
		u32 write_size;		// �f�R�[�h���ď������񂾃T�C�Y
		mAudioSource->readSafe( mSecondaryAudioData, mAudioDataBufferSize, mReadCursor, &read_size, &write_size );

		// ���ۂɌ��f�[�^����ǂ񂾃f�[�^���A�J�[�\����i�߂�
		mReadCursor += read_size;

        // �������񂾃T�C�Y�����Z
        mWriteSampleNum += write_size;

        u8* data_buffer = mSecondaryAudioData;

        // ���[�v��ň����T���v�����́A�`�����l�����֌W�����ɂȂ��Ă�B
        // ����𐳂����l�ɕϊ����邽�߁A���ۂ� 1 �T���v��������̃o�C�g�����v�Z����
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
                // �ǂ񂾃f�[�^���Ƀ��[�v�̈�̊J�n�ʒu���܂܂�Ă���ꍇ
                if ( mCursorBeginPos > 0 && mReadCursor - read_size == mCursorBeginPos )
                {
                    data_buffer = ( data_buffer + mLoopBeginBlockOffset );
                    write_size -= mLoopBeginBlockOffset; 
                }
                // ���[�v�̈�̊J�n�ʒu�����ݒ�ŁA���[�v�̈�̊J�n�T���v���𒴂����ꍇ
                else if ( mLoopBegin > 0 && mCursorBeginPos == 0 && mWriteSampleNum > ( mLoopBegin * smp_size ) )
                {
                    // �J�[�\���̓u���b�N�P�ʁB���O�� read() �̂ЂƂO���w��
                    mCursorBeginPos = mReadCursor - read_size;

                    // �u���b�N�擪����̃T���v����
                    mLoopBeginBlockOffset = write_size - ( mWriteSampleNum - ( mLoopBegin * smp_size ) );
                }

                //printf( "%d %d %d\n", mLoopBegin, mLoopLength, ( mLoopBegin + mLoopLength ) );
                //printf( "b:%d l:%d n:%d a:%d\n", mLoopBegin, mLoopLength, mWriteSampleNum, ( mLoopBegin + mLoopLength ) * smp_size );

                // �I�[�T���v�����𒴂����ꍇ�A�������I�[
                if ( mLoopLength > 0 && mWriteSampleNum > ( mLoopBegin + mLoopLength ) * smp_size )
                {
                    // �I�[�Bpolling() �ł����o����
                    mCursorEndPos = mReadCursor;

                    write_size -= ( mWriteSampleNum - ( mLoopBegin + mLoopLength ) * smp_size );
                  
                    mWriteSampleNum = mCursorBeginPos;
                }
            }
        }
        
		// SourceVoice �ɑ��M����f�[�^�̐ݒ�
		XAUDIO2_BUFFER buffer = { 0 };

		// ���݈ʒu���t�@�C���̏I�[�ɒB���Ă���ꍇ
		if ( !mIsLoop && mReadCursor >= mCursorEndPos )//mAudioSource->getSourceDataSize() )
		{
			// �t���O�ǉ� ( �Ō�܂ŉ��t�������AGetState �Ŏ擾���鉉�t���Ԃ����Z�b�g���� )
			buffer.Flags = XAUDIO2_END_OF_STREAM;
		}

		buffer.AudioBytes = write_size;
		buffer.pAudioData = data_buffer;

		// SourceVoice �Ƀf�[�^�𑗂�
        LN_DXCALL_R( mSourceVoice->SubmitSourceBuffer( &buffer ) );

		// primary �� secondary �̓���ւ�
		u8* temp = mPrimaryAudioData;
		mPrimaryAudioData = mSecondaryAudioData;
		mSecondaryAudioData = temp;

        return LN_OK;
	}


//=============================================================================
// �� MidiPlayer �N���X
//=============================================================================

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
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
	// �� �f�X�g���N�^
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
	// �� �I�[�f�B�I�\�[�X�̐ݒ�
	//---------------------------------------------------------------------
	LNRESULT MidiPlayer::initialize( IAudioSource* audio_source_, bool enable_3d_ )
	{
        LN_SETERR_R_S_( ( !mAudioSource ), LN_ERR_ASSERT, "< MidiPlayer::setAudioSource >\nmAudioSource is not NULL" );

        mAudioSource = dynamic_cast< Midi* >( audio_source_ );
		mAudioSource->addRef();

		// �I���������Đ��p�ɓ����Ɏ����Ă�o�b�t�@�𖄂߂�
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
	// �� ���ʂ̎擾
	//---------------------------------------------------------------------
	int MidiPlayer::getVolume() const
	{
		return static_cast< int >( mVolume );
	}

	//---------------------------------------------------------------------
	// �� ���ʂ̐ݒ�
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
	// �� �s�b�`�̎擾
	//---------------------------------------------------------------------
	int MidiPlayer::getPitch() const
	{
		return static_cast< int >( mPitch );
	}

	//---------------------------------------------------------------------
	// �� �s�b�`�̐ݒ�
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
	// �� �Đ������T���v�����̎擾
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
	// �� �Đ������𔻒肷��
	//---------------------------------------------------------------------
	bool MidiPlayer::isPlaying() const
	{
		return mIsPlaying;
	}

	//---------------------------------------------------------------------
	// �� �ꎞ��~�����𔻒肷��
	//---------------------------------------------------------------------
	bool MidiPlayer::isPausing() const
	{
		return mIsPausing;
	}

	//---------------------------------------------------------------------
	// �� ���[�v���̐ݒ�
	//---------------------------------------------------------------------
	LNRESULT MidiPlayer::setLoopState( u32 loop_begin_, u32 loop_length_ )
	{
		mLoopBegin = loop_begin_;
		mLoopLength = loop_length_;

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� �Đ�
	//---------------------------------------------------------------------
	LNRESULT MidiPlayer::play()
	{
        if ( !DirectMusicManager::getInstance()->isInitialized() )
        {
            // �������̂͒ǉ�����Ȃ��̂ł��̂܂܂� OK
            DirectMusicManager::getInstance()->addPlayRequest( this );
            return LN_OK;
        }



        // �����������O�Ƀ��[�U�[�ɂ���ă��[�v�ʒu���ݒ肳��Ă��Ȃ����
        // CC111 ��ڈ�Ƃ����ʒu�ɐݒ肷��
        if ( mLoopBegin == 0 && mLoopLength == 0 )
        {
            u32 cc111time;
            u32 base_time;
            mAudioSource->getLoopState( &cc111time, &base_time );

            mLoopBegin  = cc111time * LN_MUSICTIMEBASE / base_time;
            mLoopLength = 0;
        }

		// �Đ��J�n
		return _play();
	}

	//---------------------------------------------------------------------
	// �� ��~
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
	// �� �ꎞ��~
	//---------------------------------------------------------------------
	LNRESULT MidiPlayer::pause( bool is_pause_ )
	{
		// �Đ����̏ꍇ
		if ( mIsPlaying )
		{
			// �ꎞ��~����ꍇ�ŁA���݈ꎞ��~���Ă��Ȃ��ꍇ
			if ( is_pause_ && !mIsPausing )
			{
				// ���݂̉��t�ʒu���L�����Ē�~
				mPauseTime = mSegment->getPlayPosition();
				mSegment->stop();
				
				mIsPausing = true;
			}
			else if ( !is_pause_ && mIsPausing )
			{
				// �ꎞ��~�������̍Đ��ʒu�Ɉړ�
				mSegment->setPlayPosition( mPauseTime );

				mSegment->play();

				mIsPausing = false;
			}
		}
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� �|�[�����O�X�V
	//---------------------------------------------------------------------
	bool MidiPlayer::polling()
	{
		// �Đ����ł͂Ȃ��ꍇ�͒��f
		if ( !mIsPlaying || mIsPausing )
		{
			return false;
		}

		// ���[�v�Đ��ł͂Ȃ��ꍇ
		if ( !mIsLoop )
		{
            if ( mSegment )
            {
                // �Đ��J�n����� IDirectMusicPerformance8::IsPlaying() �� false �ɂȂ��Ă��܂��A
                // TotalTime() �����ۂɉ����钷�������኱�Z���Ȃ��Ă��܂��Ă���B
                // (DirectMusic �̎d�l�H�Ƃ������A�o�O�ȋC������c)
                // ���̂��߁A�������Ă��鎖�ƁA�Đ��ʒu�ɂ���d������s���B
                if ( !mSegment->isPlaying() && mSegment->getPlayPosition() >= mSegment->getTotalTime() )
			    {
                    stop();
				    return false;
			    }
		    }
        }

        return true;
	}



    // �Վ��p�B�S�̂̍Đ����Ԃ̎擾
    u32 MidiPlayer::getTotalTime() const
    {
        if ( mSegment ) // ���������΍�
        {
            return mSegment->getTotalTime();
        }
        return 0;
    }

    //---------------------------------------------------------------------
	// �� �Đ��v�����X�g�ɒǉ�����Ă����Ԃ� DirectMusic �̏����������������Ƃ��ɌĂ΂��
    //---------------------------------------------------------------------
    void MidiPlayer::onFinishDMInit( IDirectMusicPerformance8* dm_performance_ )
    {
        mSegment = LN_NEW DirectMusicSegment( dm_performance_, dynamic_cast< Midi* >( mAudioSource ) );

        // ���������ɐݒ肳�ꂽ�p�����[�^���Đݒ肷��
        setVolume( static_cast< int >( mVolume ) );
        setPitch( static_cast< int >( mPitch ) );

        // �����������O�Ƀ��[�U�[�ɂ���ă��[�v�ʒu���ݒ肳��Ă��Ȃ����
        // CC111 ��ڈ�Ƃ����ʒu�ɐݒ肷��
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
	// �� ���ۂɍĐ�����
	//---------------------------------------------------------------------
	LNRESULT MidiPlayer::_play()
	{
        // �Ƃ肠������~
        stop();


        if ( !mSegment )
        {
            mSegment = LN_NEW DirectMusicSegment(
                DirectMusicManager::getInstance()->createDMPerformance(),
                dynamic_cast< Midi* >( mAudioSource ) );

            // ���Ȃ݂� setAudioSource() �ō쐬����̂̓_���B
            // DirectMusic �̏��������� setAudioSource() ���Ă΂ꂽ�ꍇ�͂������ mSegment = NULL �����ǁA
            // ���̂��� play() ���Ă΂ꂽ�Ƃ��ɂ����R NULL �̂܂܁B
            // ���������Ɉ�x�ł� play() ���Ă�ŗv�����X�g�ɒǉ����Ă���� onFinishDMInit() ���Ă΂�邯�ǁA
            // �����łȂ��ꍇ�͂����� NULL �̂܂܂ɂȂ��Ă��܂��B
        }
		


		// ���[�v�Đ�����ꍇ
		if ( mIsLoop )
		{
			mSegment->setLoopState( true, mLoopBegin, mLoopLength );
		}
		// ���[�v�Đ����Ȃ��ꍇ
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