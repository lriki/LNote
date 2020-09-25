//=============================================================================
//�y Touch �z
//-----------------------------------------------------------------------------
///**
//  @file       Touch.h
//  @brief      Touch
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
// �� Touch �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      
//
//  @note
//              �^�b�`ID 0 �̓}�E�X�ƃ����N�B
//              
//*/
//=============================================================================
class Touch
    : public Base::ReferenceObject
{
public:

    /// �R���X�g���N�^
	Touch( Manager* manager_ );

    /// �f�X�g���N�^ 
	virtual ~Touch();

    LN_REFOBJ_METHODS;

public:

	/// �{�^�������݉�����Ă��邩�𔻒肷��
	virtual bool isPress( lnU32 id_ );

	/// �{�^�����V���������ꂽ�u�Ԃ𔻒肷��
	virtual bool isOnTrigger( lnU32 id_ );

	/// �{�^���������ꂽ�u�Ԃ𔻒肷��
	virtual bool isOffTrigger( lnU32 id_ );

	/// �{�^�����V���������ꂽ�u�Ԃƃ��s�[�g��Ԃ𔻒肷��
	virtual bool isRepeat( lnU32 id_ );

    /// �^�b�`���W�̎擾 (�}�E�X�Ɠ������A���z�X�N���[���ւ̕ϊ����K�v�ȏꍇ�͂��̕ϊ���B�^�b�`����Ă��Ȃ��ꍇ�͍Ō�Ƀ^�b�`���ꂽ�Ƃ��̍��W)
	virtual const LVector2& getPoint( lnU32 id_ ) { return mPosition[id_]; }

    /// 1 �t���[���O����̈ړ��ʂ̎擾
	virtual const LVector2& getMoveDistance( lnU32 id_ ) { return mDistance[id_]; }

    /// ���s�[�g�Ԋu�̐ݒ�
	virtual void setRepeatInterval( u32 first_, u32 interval_ );

public:

    /// �t���[���X�V (Manager ����Ă΂��)
	void update();

private:

	Manager*			    mManager;							///< �Ǘ��N���X
    IInputDevice*           mInputDevice;						///< �f�o�C�X�N���X
	s32					    mPressCounts[LN_MAX_TOUCH_DATA];	///< ���������̃t���[���J�E���g
	s32					    mFirstRepeatInterval;				///< ���s�[�g�J�n�܂ł̑҂��t���[����
	s32					    mRemRepeatInterval;					///< ���s�[�g����1��̑҂��t���[����
   
    LNTouchDeviceState      mTouchState[LN_MAX_TOUCH_DATA];
    
	LVector2	            mPosition[LN_MAX_TOUCH_DATA];							///< �ʒu
	LVector2	            mLastPosition[LN_MAX_TOUCH_DATA];						///< 1 �t���[���O�̈ʒu
	LVector2	            mDistance[LN_MAX_TOUCH_DATA];							///< 1 �t���[���O����̈ړ���

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