//=============================================================================
//�y GameController �z
//-----------------------------------------------------------------------------
//**
//  @file       GameController.h
//  @brief      GameController
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
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
// �� GameController �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      ���z�Q�[���R���g���[���̃N���X
//*/
//=============================================================================
class GameController
    : public Base::ReferenceObject
    , public IGameController
{
public:

    /// �R���X�g���N�^ (�W���C�p�b�h���Ȃ��ꍇ�ł��A�ԍ���n��)
	GameController( Manager* manager_, int joypad_id_ );

	/// �f�X�g���N�^ 
	virtual ~GameController();

    LN_REFOBJ_METHODS;

public:
	
	/// �{�^�������݉�����Ă��邩�𔻒肷��
	virtual bool isPress( LNGameControllerButton button_ );

	/// �{�^�����V���������ꂽ�u�Ԃ𔻒肷��
	virtual bool isOnTrigger( LNGameControllerButton button_ );

	/// �L�[�������ꂽ�u�Ԃ𔻒肷��
	virtual bool isOffTrigger( LNGameControllerButton button_ );

	/// �{�^�����V���������ꂽ�u�Ԃƃ��s�[�g��Ԃ𔻒肷��
	virtual bool isRepeat( LNGameControllerButton button_ );

    /// �A�i���O���͒l�̎擾
	virtual float getPower( LNGameControllerButton button_ );

    /// ���̏�Ԃ̎擾
	virtual const LVector2& getAxisState( int axis_ );

    /// �V���������ꂽ���z�{�^���̎擾
	virtual int getPressedButton();

    /// �{�^���z�u (�L�[�R���t�B�O) ��ݒ肷��
	virtual void attachVirtualButton( LNGameControllerAttachTarget target_, int v_button_, bool duplication_ ) ;

    /// �{�^���z�u (�L�[�R���t�B�O) ����������
	virtual void detachVirtualButton( LNGameControllerAttachTarget target_, int v_button_ );

    /// ���͂��󂯎��f�o�C�X�̎w��
    virtual void setDeviceAttachFlag( u8 flags_ ) { mDeviceAttachFlag = flags_; }

public:

    /// ���s�[�g�Ԋu�̐ݒ�
    void setRepeatInterval( s32 first_, s32 interval_ ) { mFirstRepeatInterval = first_; mRemRepeatInterval = interval_; }

    /// �t���[���X�V
	void update();

private:

    typedef std::vector< int >  AttachedVButtons;

private:

    Manager*	        mManager;							    ///< �Ǘ��N���X
    IInputDevice*       mInputDevice;
	lnS32			    mButtonStatus[ LN_MAX_BUTTONS ];	    ///< �{�^���̏��
	float			    mButtonPower[ LN_MAX_BUTTONS ];
    AttachedVButtons    mAttachedVButtons[ LN_MAX_AT_TARGETS ];

    int					mJoypadID;							    ///< ���蓖�Ă��Ă���W���C�p�b�h�̔ԍ�
    LVector2	        mAxisState[ LN_MAX_JOYPAD_AXIS / 2 ];   ///< ���̏��

    u32                 mDeviceAttachFlag;

    s32					mFirstRepeatInterval;				    ///< ���s�[�g�J�n�܂ł̑҂��t���[����
	s32					mRemRepeatInterval;					    ///< ���s�[�g����1��̑҂��t���[����
    bool				mPressedTriggerFlag;				    ///< getPressedButton() �Ŏg����t���O


    /*
	Manager*			mManager;							///< �Ǘ��N���X
	s8					mButtonStatus[ LN_MAX_BUTTONS ];	///< �{�^���̏��
	LVector2	        mAxisState[ MAX_AXES ];				///< ���̏��
	s32					mFirstRepeatInterval;				///< ���s�[�g�J�n�܂ł̑҂��t���[����
	s32					mRemRepeatInterval;					///< ���s�[�g����1��̑҂��t���[����
	
	//float				mAxisInvalidRange;					///< ���̖����͈�
	//u8					mInputEnableFlags;					///< ���ۂɎg�p����f�o�C�X�̃t���O
	bool				mAssignedkeyboard;					///< �L�[�{�[�h�����蓖�Ă��Ă���t���O
	bool				mPressedTriggerFlag;				///< getPressedButton() �Ŏg����t���O
    //bool                mEnableNormalize;                   ///< true �̏ꍇ�A���𐳋K������


    ConfigData mConfigData;
	
	friend class Manager;
    */
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