//=============================================================================
//�y Mouse �z
//-----------------------------------------------------------------------------
///**
//  @file       Mouse.h
//  @brief      Mouse
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
// �� Mouse �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �}�E�X����̓��͂������N���X
//
//  @par
//              ���̃N���X�̃C���X�^���X�� Engine::Input::Manager ����擾���܂��B<br><br>
//
//              isPress() ���̓��͂̔���Ŕ͈͊O�̒l��n�����Ƃ��̖߂�l�͏�� false �ł��B
//*/
//=============================================================================
class Mouse
    : public Base::ReferenceObject
    , public IMouse
{
public:

    /// �R���X�g���N�^
	Mouse( Manager* manager_ );

    /// �f�X�g���N�^ 
	virtual ~Mouse();

    LN_REFOBJ_METHODS;

public:

	/// �{�^�������݉�����Ă��邩�𔻒肷��
	virtual bool isPress( LNMouseButton button_ );

	/// �{�^�����V���������ꂽ�u�Ԃ𔻒肷��
	virtual bool isOnTrigger( LNMouseButton button_ );

	/// �{�^���������ꂽ�u�Ԃ𔻒肷��
	virtual bool isOffTrigger( LNMouseButton button_ );

	/// �{�^�����V���������ꂽ�u�Ԃƃ��s�[�g��Ԃ𔻒肷��
	virtual bool isRepeat( LNMouseButton button_ );

    /// �}�E�X���W�̐ݒ�
    virtual void setPoint( const LVector2& point_ ) { mInputDevice->setMousePoint( point_ ); }

    /// �}�E�X���W�̎擾
	virtual const LVector2& getPoint() { return mPosition; }

    /// 1 �t���[���O����̈ړ��ʂ̎擾
	virtual const LVector2& getMoveDistance() { return mDistance; }

    /// 1 �t���[���̃z�C�[���̉�]�ʂ̎擾
	virtual int getWheelState() { return mWheelState; }

    /// �ʒu�𒆉��ɌŒ肷�邩�̐ݒ�
    virtual void setEnableCenterFix( bool flag_ ) { mIsCenterFix = flag_; }

    /// ���s�[�g�Ԋu�̐ݒ�
	virtual void setRepeatInterval( u32 first_, u32 interval_ );

public:

    /// �t���[���X�V (Manager ����Ă΂��)
	void update();

private:

	

	/// �����炩�̃}�E�X���상�b�Z�[�W������ꍇ�ɌĂ� ( Manager ����Ă΂�� )
	void _onOperatedMouse( bool is_client_ );

	/// �}�E�X�J�[�\���̕\����Ԃ�ݒ肷�� ( setCursorMode() _postMoveCursor() ����Ă΂�� )
	void _showCursor( bool flag_ );

	/// �}�E�X�J�[�\���̑����ɌĂ�ŁA���݂̐ݒ�ɉ����ă}�E�X�J�[�\���̕\���E��\����؂�ւ��� ( �N���C�A���g�̈���ł̃}�E�X�ړ��̏ꍇ�Atrue ��n�� )
	void _postMoveCursor( bool is_client_ );

protected:

	

private:

	static const int MAX_BUTTON_NUM = LN_MAX_MOUSE_BUTTONS;	///< �}�E�X�{�^���̍ő吔
	static const int MOUSE_CURSOR_COUNT_MAX = 100;			///< �}�E�X�𓮂����Ȃ����A������܂ł̃t���[����

private:

	Manager*			    mManager;							///< �Ǘ��N���X
    IInputDevice*           mInputDevice;						///< �f�o�C�X�N���X
	s32					    mButtonStatus[ MAX_BUTTON_NUM ];	///< �{�^���̏��
	s32					    mFirstRepeatInterval;				///< ���s�[�g�J�n�܂ł̑҂��t���[����
	s32					    mRemRepeatInterval;					///< ���s�[�g����1��̑҂��t���[����
    bool                    mIsCenterFix;

	LVector2	        mPosition;							///< ���݈ʒu
	LVector2	        mLastPosition;						///< 1 �t���[���O�̈ʒu
	LVector2	                mDistance;							///< 1 �t���[���O����̈ړ���
	long				        mWheelState;						///< �z�C�[���̏��
	//LNMouseCursorMode	        mMouseCursorMode;					///< �}�E�X�J�[�\���̋���
	int					        mMouseVisibleCount;					///< �}�E�X�J�[�\������莞�ԓ����Ȃ��ꍇ�ɏ��������Ɏg���J�E���^
	bool				        mIsSystemMouseShow;					///< �V�X�e���̃}�E�X�J�[�\���\������ true

	friend class Manager;
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