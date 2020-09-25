//=============================================================================
//�y Keyboard �z
//-----------------------------------------------------------------------------
///**
//  @file       Keyboard.h
//  @brief      Keyboard
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
class Manager;

//=============================================================================
// �� Keyboard �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �L�[�{�[�h����̓��͂������N���X
//*/
//=============================================================================
class Keyboard
    : public Base::ReferenceObject
    , public IKeyboard
{
public:

	/// �R���X�g���N�^
	Keyboard( Manager* manager_ );

    /// �f�X�g���N�^ 
	virtual ~Keyboard() {};

    LN_REFOBJ_METHODS;

public:
	
	/// �L�[�����݉�����Ă��邩�𔻒肷��
	virtual bool isPress( LNVirtualKeyCode vkey_ );

	/// �L�[���V���������ꂽ�u�Ԃ𔻒肷��
	virtual bool isOnTrigger( LNVirtualKeyCode vkey_ );

	/// �L�[�������ꂽ�u�Ԃ𔻒肷��
	virtual bool isOffTrigger( LNVirtualKeyCode vkey_ );

	/// �{�^�����V���������ꂽ�u�Ԃƃ��s�[�g��Ԃ𔻒肷��
	virtual bool isRepeat( LNVirtualKeyCode vkey_ );

    /// �L�[�{�[�h�̃o�b�t�@�����O�f�[�^�̎擾
    virtual LNVirtualKeyCode getKeyBufferState() { return mInputDevice->getKeyBufferState(); }

    /// ���s�[�g�Ԋu�̐ݒ�
	virtual void setRepeatInterval( u32 first_, u32 interval_ );

public:

	/// �t���[���X�V ( Manager ����Ă΂�� )
	void update();

private:

	Manager*        mManager;					///< �Ǘ��N���X
    IInputDevice*   mInputDevice;
	s8              mKeyStatus[ LN_MAX_KEYS ];  ///< �L�[�̏��
	s8			    mFirstRepeatInterval;		///< ���s�[�g�J�n�܂ł̑҂��t���[����
	s8			    mRemRepeatInterval;			///< ���s�[�g����1��̑҂��t���[����
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