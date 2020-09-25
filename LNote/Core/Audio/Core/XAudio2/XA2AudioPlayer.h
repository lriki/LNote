//=============================================================================
//�y XA2AudioPlayer �z
//-----------------------------------------------------------------------------
///**
//  @file       XA2AudioPlayer.h
//  @brief      XA2AudioPlayer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "DirectMusic.h"
#include "XA2AudioDevice.h"
#include "../../Interface.h"

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
// �� AudioPlayerBase �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �����̍Đ��������N���X�̊��
//*/
//=============================================================================
class AudioPlayerBase
    : public IAudioPlayer
    , public Base::ReferenceObject
    , public Base::NodeList< AudioPlayerBase >::Node
{
public:

    /// IXAudio2SourceVoice �̎擾
    virtual IXAudio2SourceVoice* getXAudio2SourceVoice() const = 0;

    /// 3D �����̏��̎擾
    virtual EmitterState* getEmitterState() const = 0;

    LN_REFOBJ_METHODS();
};

//=============================================================================
// �� OnMemory �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �I���������f�[�^�̍Đ��������N���X
//*/
//=============================================================================
class OnMemory
    : public AudioPlayerBase
{
public:

	/// �R���X�g���N�^
	OnMemory( AudioDevice* manager_ );

protected:

	/// �f�X�g���N�^
	virtual ~OnMemory();

public:

	/// ������
	virtual LNRESULT initialize( IAudioSource* audio_source_, bool enable_3d_ = false );

    /// �ݒ肳��Ă���I�[�f�B�I�\�[�X�̎擾 ( ���̂Ƃ���Q�ƃJ�E���g�͑��₳�Ȃ� )
    virtual IAudioSource* getAudioSource() const { return mAudioSource; }

	/// ���ʂ̎擾
	virtual int getVolume() const;

	/// ���ʂ̐ݒ�
	LNRESULT setVolume( int volume_ );

	/// �s�b�`�̎擾
	virtual int getPitch() const;

	/// �s�b�`�̐ݒ�
	LNRESULT setPitch( int pitch_ );

	/// �Đ�������
	virtual bool isPlaying() const;

	/// �ꎞ��~������
	virtual bool isPausing() const;

    /// �Đ���Ԃ̎擾
    virtual LNSoundPlayState getPlayState() const { return (mIsPausing) ? LN_SOUNDPLAYSTATE_PAUSING : ( (mIsPlaying) ? LN_SOUNDPLAYSTATE_PLAYING : LN_SOUNDPLAYSTATE_STOP ); }

    /// �Đ������T���v�����̎擾
    virtual u64 getPlayedSamples() const;

    /// ���[�v�Đ��̗L���ݒ�
    virtual void loop( bool loop_ ) { mIsLoop = loop_; }

    /// ���[�v�Đ����L�����𔻒肷��
    virtual bool isLoop() { return mIsLoop; }

	/// ���[�v���̐ݒ�
	virtual LNRESULT setLoopState( u32 loop_begin_, u32 loop_length_ );

	/// �Đ�
	virtual LNRESULT play();

	/// ��~
	virtual LNRESULT stop();

	/// �ꎞ��~
	virtual LNRESULT pause( bool is_pause_ );

	/// �|�[�����O�X�V
	virtual bool polling();

    /// 3D �������𔻒肷��
    virtual bool is3DSound() { return ( mEmitterState != NULL ); }

    /// 3D �����Ƃ��Ă̈ʒu��ݒ肷��
    virtual LNRESULT setPosition( const LVector3& pos_ );

    /// 3D �����Ƃ��Ă̈ʒu���擾����
    virtual const LVector3& getPosition() { return mEmitterState->Position; }

    /// ���x�̐ݒ�
    virtual void setVelocity( const LVector3& pos_ );

    /// 3D �����̉e���͈� (���̓͂�����) �̐ݒ�
    virtual void setEmitterDistance( lnfloat distance_ ) { mEmitterState->setDistance( distance_ ); }

    /// 3D �����̉e���͈� (���̓͂�����) �̎擾
    virtual lnfloat getEmitterDistance() const { return mEmitterState->getDistance(); }

public:

    /// IXAudio2SourceVoice �̎擾
    virtual IXAudio2SourceVoice* getXAudio2SourceVoice() const { return mSourceVoice; }

    /// 3D �����̏��̎擾
    virtual EmitterState* getEmitterState() const { return mEmitterState; }

private:

    AudioDevice*                mManager;       ///< �Ǘ��N���X
    IAudioSource*		        mAudioSource;
	IXAudio2SourceVoice*	    mSourceVoice;
    EmitterState*               mEmitterState;  ///< 3D �����̏�� (3D�����Ƃ��ď��������ꂽ�ꍇ�ɍ쐬�����)

	
	float					    mVolume;		///< ���� ( 0 �` 100 )
	float					    mPitch;			///< �s�b�` ( 50 �` 200 )

	u32						    mLoopBegin;		///< ���[�v�����̈�̍ŏ��̃T���v��
	u32						    mLoopLength;	///< ���[�v�̈�̒��� (�T���v�����P��)

	bool					    mIsPlaying;
	bool					    mIsPausing;
	bool					    mIsLoop;
};



//=============================================================================
// �� Streaming �N���X
//-----------------------------------------------------------------------------
/**
//  @brief      �X�g���[�~���O�ł̍Đ��������N���X
//*/
//=============================================================================
class Streaming
    : public AudioPlayerBase
{
public:

	/// �R���X�g���N�^
	Streaming( AudioDevice* manager_ );

protected:

	/// �f�X�g���N�^
	virtual ~Streaming();

public:

    /// ������
	virtual LNRESULT initialize( IAudioSource* audio_source_, bool enable_3d_ = false );

    /// �ݒ肳��Ă���I�[�f�B�I�\�[�X�̎擾 ( ���̂Ƃ���Q�ƃJ�E���g�͑��₳�Ȃ� )
    virtual IAudioSource* getAudioSource() const { return mAudioSource; }

	/// ���ʂ̎擾
	virtual int getVolume() const;

	/// ���ʂ̐ݒ�
	virtual LNRESULT setVolume( int volume_ );

	/// �s�b�`�̎擾
	virtual int getPitch() const;

	/// �s�b�`�̐ݒ�
	virtual LNRESULT setPitch( int pitch_ );

	/// �Đ������𔻒肷��
	virtual bool isPlaying() const;

	/// �ꎞ��~�����𔻒肷��
	virtual bool isPausing() const;

        /// �Đ���Ԃ̎擾
    virtual LNSoundPlayState getPlayState() const { return (mIsPausing) ? LN_SOUNDPLAYSTATE_PAUSING : ( (mIsPlaying) ? LN_SOUNDPLAYSTATE_PLAYING : LN_SOUNDPLAYSTATE_STOP ); }

    /// �Đ������T���v�����̎擾
    virtual u64 getPlayedSamples() const;

    /// ���[�v�Đ��̗L���ݒ�
    virtual void loop( bool loop_ ) { mIsLoop = loop_; }

    /// ���[�v�Đ����L�����𔻒肷��
    virtual bool isLoop() { return mIsLoop; }

	/// ���[�v���̐ݒ�
	virtual LNRESULT setLoopState( u32 loop_begin_, u32 loop_length_ );

	/// �Đ�
	virtual LNRESULT play();

	/// ��~
	virtual LNRESULT stop();

	/// �ꎞ��~
	virtual LNRESULT pause( bool is_pause_ );

	/// �|�[�����O�X�V
	virtual bool polling();

    /// 3D �������𔻒肷��
    virtual bool is3DSound() { return ( mEmitterState != NULL ); }

    /// 3D �����Ƃ��Ă̈ʒu��ݒ肷��
    virtual LNRESULT setPosition( const LVector3& pos_ );

    /// 3D �����Ƃ��Ă̈ʒu���擾����
    virtual const LVector3& getPosition() { return mEmitterState->Position; }

    /// ���x�̐ݒ�
    virtual void setVelocity( const LVector3& pos_ );
    
    /// 3D �����̉e���͈� (���̓͂�����) �̐ݒ�
    virtual void setEmitterDistance( lnfloat distance_ ) { mEmitterState->setDistance( distance_ ); }

    /// 3D �����̉e���͈� (���̓͂�����) �̎擾
    virtual lnfloat getEmitterDistance() const { return mEmitterState->getDistance(); }

public:

    /// IXAudio2SourceVoice �̎擾
    virtual IXAudio2SourceVoice* getXAudio2SourceVoice() const { return mSourceVoice; }

    /// 3D �����̏��̎擾
    virtual EmitterState* getEmitterState() const { return mEmitterState; }

private:

	/// �Z�J���_���ɐV�����f�[�^����������ŁA�Đ��L���[�ɒǉ�����
	LNRESULT _addNextBuffer();

private:

    AudioDevice*                   mManager;       ///< �Ǘ��N���X
    Audio::IAudioSource*		mAudioSource;
	IXAudio2SourceVoice*	    mSourceVoice;
    EmitterState*  mEmitterState;

	float					mVolume;		///< ���� ( 0 �` 100 )
	float					mPitch;			///< �s�b�` ( 50 �` 200 )

	u32						mLoopBegin;		///< ���[�v�����̈�̍ŏ��̃T���v��
	u32						mLoopLength;	///< ���[�v�̈�̒��� (�T���v�����P��)
    //u32                     mEndSample;     ///< �I�[�T���v�� ( ���[�v�I�[ )

	u8*						mPrimaryAudioData;		///< �_�u���o�b�t�@�����O�p�v���C�}��
	u8*						mSecondaryAudioData;	///< �_�u���o�b�t�@�����O�p�Z�J���_��
	u32						mAudioDataBufferSize;	///< �v���C�}���o�b�t�@�ƃZ�����_���o�b�t�@�̃T�C�Y ( 1 �b�� )
	u32						mReadCursor;

    u32                     mLoopBeginBlockOffset;
    u32                     mCursorBeginPos;
    u32                     mCursorEndPos;

    u32                     mWriteSampleNum;

	bool					mIsPlaying;
	bool					mIsPausing;
	bool					mIsLoop;
	bool					mEOF;			///< �t�@�C���̏I�[�܂ōĐ������t���O
};


//=============================================================================
// �� MidiPlayer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      Midi �f�[�^�̍Đ��������N���X
//
//  @par
//              DirectMusic ��ʃX���b�h�ŏ���������ꍇ�A�t�F�[�h�C���E�A�E�g���Ԃ�
//              ���܂�����Ȃ��Ȃ�Ƃ������Ƃ�����܂��B
//              ����́uDirectMusic ���������͉���炷���Ƃ��ł��Ȃ����߁A
//              �Đ����ł͂Ȃ��v�Ƃ������肪����A��~���̓t�F�[�h����������Ȃ����߂ɋN����܂��B
//              <br>
//              ������C�����AMidi �̉��t���J�n����鎞�Ԃɍ��킹�Đ��������ʂ�
//              �ݒ肷��Ƃ������͉\�ł������A���ۂɂ��̕s��ɑ�������悤�Ȏ��ԂƂ����̂�
//              ���[�U�[�̕����u�Q�[���J�n������t�F�[�h�C���Đ��������v�Ƃ������̂��قƂ�ǂł��B
//              ������C�������ꍇ�A�J�n���ɒ��r���[�ȉ��ʂ��牉�t���J�n���邱�ƂɂȂ�A
//              ���o�Ƃ��Ă��܂�ǂ��Ȃ��ƍl�������߁A���i�K�ł͂���͎d�l�Ƃ��܂��B
//*/
//=============================================================================
class MidiPlayer
    : public AudioPlayerBase
    , public DirectMusicManager::PlayerObject
{
public:

	/// �R���X�g���N�^
	MidiPlayer( AudioDevice* manager_ );

protected:

	/// �f�X�g���N�^
	virtual ~MidiPlayer();

public:

    /// ������ ( xaudio2_ �͖����B���ʂ� NULL ��n���Ă��� )
	virtual LNRESULT initialize( IAudioSource* audio_source_, bool enable_3d_ = false );

    /// �ݒ肳��Ă���I�[�f�B�I�\�[�X�̎擾 ( ���̂Ƃ���Q�ƃJ�E���g�͑��₳�Ȃ� )
    virtual IAudioSource* getAudioSource() const { return mAudioSource; }

	/// ���ʂ̎擾
	virtual int getVolume() const;

	/// ���ʂ̐ݒ�
	virtual LNRESULT setVolume( int volume_ );

	/// �s�b�`�̎擾
	virtual int getPitch() const;

	/// �s�b�`�̐ݒ�
	virtual LNRESULT setPitch( int pitch_ );

	/// �Đ������𔻒肷��
	virtual bool isPlaying() const;

	/// �ꎞ��~�����𔻒肷��
	virtual bool isPausing() const;

    /// �Đ���Ԃ̎擾
    virtual LNSoundPlayState getPlayState() const { return (mIsPausing) ? LN_SOUNDPLAYSTATE_PAUSING : ( (mIsPlaying) ? LN_SOUNDPLAYSTATE_PLAYING : LN_SOUNDPLAYSTATE_STOP ); }

    /// �Đ������T���v�����̎擾 ( Midi �̏ꍇ�̓~���[�W�b�N�^�C�� )
    virtual u64 getPlayedSamples() const;

    /// ���[�v�Đ��̗L���ݒ�
    virtual void loop( bool loop_ ) { mIsLoop = loop_; }

    /// ���[�v�Đ����L�����𔻒肷��
    virtual bool isLoop() { return mIsLoop; }

	/// ���[�v���̐ݒ�
	virtual LNRESULT setLoopState( u32 loop_begin_, u32 loop_length_ );

	/// �Đ�
	virtual LNRESULT play();

	/// ��~
	virtual LNRESULT stop();

	/// �ꎞ��~
	virtual LNRESULT pause( bool is_pause_ );

	/// �|�[�����O�X�V
	virtual bool polling();

    /// �Đ��v�����X�g�ɒǉ�����Ă����Ԃ� DirectMusic �̏����������������Ƃ��ɌĂ΂��
    virtual void onFinishDMInit( IDirectMusicPerformance8* dm_performance_ );

    /// 3D �������𔻒肷��
    virtual bool is3DSound() { return false; }

    /// 3D �����Ƃ��Ă̈ʒu��ݒ肷��
    virtual LNRESULT setPosition( const LVector3& pos_ ) { return LN_OK; }

    /// 3D �����Ƃ��Ă̈ʒu���擾����
    virtual const LVector3& getPosition() { static const LVector3 pos( 0, 0, 0 ); return pos; }

    /// ���x�̐ݒ�
    virtual void setVelocity( const LVector3& v ) {}

    /// 3D �����̉e���͈� (���̓͂�����) �̐ݒ�
    virtual void setEmitterDistance( lnfloat distance_ ) {}

    /// 3D �����̉e���͈� (���̓͂�����) �̎擾
    virtual lnfloat getEmitterDistance() const { return 0; }

    /// �Վ��p�B�S�̂̍Đ����Ԃ̎擾
    u32 getTotalTime() const;

public:

    /// IXAudio2SourceVoice �̎擾
    virtual IXAudio2SourceVoice* getXAudio2SourceVoice() const { return NULL; }

    /// 3D �����̏��̎擾
    virtual EmitterState* getEmitterState() const { return NULL; }

private:

    /// ���ۂɍĐ�����
    LNRESULT _play();

private:

    AudioDevice*                   mManager;       ///< �Ǘ��N���X
    Midi*		                    mAudioSource;   ///< Midi �f�[�^
    DirectMusicSegment*             mSegment;       ///< DirectMusic �̍Đ��Ǘ�
	
	float					        mVolume;		///< ���� ( 0 �` 100 )
	float					        mPitch;			///< �s�b�` ( 50 �` 200 )

	u32						        mLoopBegin;		///< ���[�v�����̈�̐擪 ( �~���[�W�b�N�^�C���P�� )
	u32						        mLoopLength;	///< ���[�v�̈�̒��� ( �~���[�W�b�N�^�C���P�� )
    u32                             mPauseTime;

	bool					        mIsPlaying;
	bool					        mIsPausing;
	bool					        mIsLoop;
};

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