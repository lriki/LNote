//=============================================================================
//�y Joypad �z
//-----------------------------------------------------------------------------
///**
//  @file       Joypad.h
//  @brief      Joypad
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Input
{

//=============================================================================
// �� Joypad �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �W���C�p�b�h����̓��͂������N���X
//*/
//=============================================================================
class Joypad
    : public Base::ReferenceObject
    , public IJoypad
{
public:

    /// �R���X�g���N�^
	Joypad( Manager* manager_, int joypad_id_ );

    /// �f�X�g���N�^ 
	virtual ~Joypad() {};

    LN_REFOBJ_METHODS;

public:

    /// �ڑ��ς݂����`�F�b�N����
    virtual bool isConnected() { return ( mJoypadID >= 0 ); }

    /// �{�^�������݉�����Ă��邩�𔻒肷��
	virtual bool isPress( int button_ );

    /// �{�^�����V���������ꂽ�u�Ԃ𔻒肷��
	virtual bool isOnTrigger( int button_ );

    /// �{�^���������ꂽ�u�Ԃ𔻒肷��
	virtual bool isOffTrigger( int button_ );

    /// �{�^�����V���������ꂽ�u�Ԃƃ��s�[�g��Ԃ𔻒肷��
	virtual bool isRepeat( int button_ );

    /// ���̏�Ԃ̎擾
    virtual float getAxisState( int axis_ );

    /// POV �̏�Ԃ̎擾
	virtual u32 getPOVState() { return mPOVState; }

    /// �W���C�p�b�h�̐U�����J�n����
    virtual void startVibration( int power_, int time_ ) { mInputDevice->startVibration( mJoypadID, power_, time_ ); }

    /// �W���C�p�b�h�̐U�����~����
	virtual void stopVibration() { mInputDevice->stopVibration( mJoypadID ); }

	/// ���s�[�g�Ԋu�̐ݒ� ( Manager ����Ă΂�� )
	virtual void setRepeatInterval( u32 first_, u32 interval_ );

public:

	/// �t���[���X�V ( Manager ����Ă΂�� )
	void update();

private:

	Manager*        mManager;							    ///< �Ǘ��N���X
    IInputDevice*   mInputDevice;
	int		        mJoypadID;							    ///< ���蓖�Ă��Ă���W���C�p�b�h�̔ԍ�
	s32		        mButtonStatus[ LN_MAX_JOYPAD_BUTTONS ]; ///< �{�^���̏��
	float	        mAxisStatus[ LN_MAX_JOYPAD_AXIS ];	    ///< ���̏��
	u32		        mPOVState;							    ///< POV �̏��
	s32		        mFirstRepeatInterval;				    ///< ���s�[�g�J�n�܂ł̑҂��t���[����
	s32		        mRemRepeatInterval;					    ///< ���s�[�g����1��̑҂��t���[����
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Input
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================