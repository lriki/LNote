//=============================================================================
//�y Sound �z
//-----------------------------------------------------------------------------
///**
//  @file       Sound.h
//  @brief      Sound
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Base/Container/NodeList.h"
#include "../Geometry/Vector3.h"
#include "../Animation/FadeValue.h"
#include "Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Audio
{
class Manager;

//=============================================================================
// �� Sound �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �����Ƃ��̍Đ��������N���X
//*/
//=============================================================================
class Sound
    : public Base::ReferenceObject
    , public ISound
    , public Base::NodeList< Sound >::Node
{
public:

    /// �R���X�g���N�^ ( IAudioPlayer �� Manager �ł̍쐬�シ���ɐݒ肷�邱�Ƃ�z�肵�Ă��邽�߁A�Q�ƃJ�E���g�͑��₳�Ȃ��B�f�X�g���N�^�� release() )
    Sound( Manager* manager_, IAudioPlayer* player_ );

	/// �f�X�g���N�^
	virtual ~Sound();

    LN_REFOBJ_METHODS;

public:

	/// ���ʂ̎擾
	virtual int getVolume();

	/// ���ʂ̐ݒ�
	virtual void setVolume( int volume_ );

	/// �s�b�`�̎擾
	virtual int getPitch();

	/// �s�b�`�̐ݒ�
	virtual void setPitch( int pitch_ );

    /// �T�E���h�̏�Ԃ��擾����
    virtual void getState( LNAudioPlayState* state_ );

    /// ���[�v�Đ��̗L���ݒ�
    virtual void loop( bool loop_ );

    /// ���[�v�Đ����L�����𔻒肷��
    virtual bool isLoop();

    /// ���[�v���̐ݒ�
	virtual void setLoopState( u32 begin_, u32 length_ );

    /// �Đ�
	virtual LNRESULT play();

    /// ��~
	virtual void stop();

    /// �ꎞ��~
	virtual void pause( bool is_pause_ );

    /// ���ʂ̃t�F�[�h
	virtual LNRESULT fadeVolume( int target_volume_, int time_, LNSoundFadeState state_ );

    /// 3D �������𔻒肷��
    virtual bool is3DSound();

    /// 3D �����Ƃ��Ă̈ʒu��ݒ肷��
    virtual LNRESULT setPosition( const LVector3& pos_ );

    /// 3D �����Ƃ��Ă̈ʒu���擾����
    virtual const LVector3& getPosition();

    /// ���x�̐ݒ�
    virtual void setVelocity( const LVector3& v );

    /// ���������̐ݒ�
    virtual void setMaxDistance( lnfloat distance );

	virtual void setGroupFlag( u32 flag_ );

    virtual u32 getGroupFlag();

    /// �����Ɏ����Ă��鉹���Đ��N���X�̎擾
    virtual IAudioPlayer* getAudioPlayer() { return mAudioPlayer; }

    virtual bool update( float elapsed_time_ );

    //virtual u32 getPlayerFlags() const { return mFlags; }

    ///// LNAudioPlayerFlag �̒l
    //virtual void setPlayerFlags( u32 flags_ ) { mFlags = flags_; }


private:

    Manager*                        mManager;
    IAudioPlayer*	                mAudioPlayer;
    Thread::Mutex*                  mLock;

    int 					        mOrginalVolume;	        ///< setVolume() �Őݒ肳���A���̉���
    LNSoundFadeState		        mFadeState;
    Animation::FadeValue< int >     mFadeValue;
    
    u32                             mFlags;                 ///< LNAudioPlayerFlag �̒l
    u32                             mGroupFlags;
    bool					        mIsFading;		        ///< �t�F�[�h�C���A�܂��̓t�F�[�h�A�E�g���t���O
    bool                            mOnlyRefFromManager;    ///< Manager::deleteAudioPlayer() ���瑀�삳���
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Audio
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================