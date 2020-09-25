//=============================================================================
//�y Interface �z
//-----------------------------------------------------------------------------
//**
//  @file       Interface.h
//  @brief      Interface
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Geometry/Vector2.h"
#include "../System/Interface.h"
//#include "InputConfigData.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

static const int LN_MAX_JOYPADS = 4;            ///< �W���C�p�b�h�̍ő吔
static const int LN_MAX_JOYPAD_BUTTONS = 16;    ///< �W���C�p�b�h�̃{�^���̍ő吔
static const int LN_MAX_JOYPAD_AXIS = 8;        ///< �W���C�p�b�h�̎��̍ő吔
static const int LN_MAX_GAME_CONTROLLERS = 4;   ///< ���z�R���g���[���̍ő吔
#define          LN_JOYPAD_PLAY_RANGE   (0.2f)  ///< �W���C�p�b�h�̂����є͈�
#define          LN_JOYPAD_SWITCH_RANGE (0.8f)

/// �}�E�X�̃{�^���ԍ�
enum LNMouseButton
{
	LN_MOUSE_BUTTON_1 = 0,
	LN_MOUSE_BUTTON_2,
	LN_MOUSE_BUTTON_3,
	LN_MOUSE_BUTTON_4,
	LN_MOUSE_BUTTON_5,
	LN_MOUSE_BUTTON_6,
	LN_MOUSE_BUTTON_7,
	LN_MOUSE_BUTTON_8,

	LN_MAX_MOUSE_BUTTONS
};

/// �Q�[���{�^����\���l (()���̓f�t�H���g�Ŋ��蓖�Ă�Ă���L�[�A�{�^���A��)
enum LNGameControllerButton
{
	LN_BUTTON_A = 0,        ///< A�{�^�� (�L�[�{�[�h='Z' �W���C�p�b�h=0�ԃ{�^��)
	LN_BUTTON_B,            ///< B�{�^�� (�L�[�{�[�h='X' �W���C�p�b�h=1)
	LN_BUTTON_C,            ///< C�{�^�� (�L�[�{�[�h='C' �W���C�p�b�h=2)
	LN_BUTTON_X,            ///< X�{�^�� (�L�[�{�[�h='A' �W���C�p�b�h=3)
	LN_BUTTON_Y,            ///< Y�{�^�� (�L�[�{�[�h='S' �W���C�p�b�h=4)
	LN_BUTTON_Z,            ///< Z�{�^�� (�L�[�{�[�h='D' �W���C�p�b�h=5)
	LN_BUTTON_L,            ///< L�{�^�� (�L�[�{�[�h='Q' �W���C�p�b�h=6)
	LN_BUTTON_R,            ///< R�{�^�� (�L�[�{�[�h='W' �W���C�p�b�h=7)
	LN_BUTTON_L2,           ///< L2�{�^�� (�L�[�{�[�h=None �W���C�p�b�h=8)
	LN_BUTTON_R2,           ///< R2�{�^�� (�L�[�{�[�h=None �W���C�p�b�h=9)
	LN_BUTTON_L3,           ///< L3�{�^�� (�L�[�{�[�h=None �W���C�p�b�h=10)
	LN_BUTTON_R3,           ///< R3�{�^�� (�L�[�{�[�h=None �W���C�p�b�h=11)
	LN_BUTTON_START,        ///< R3�{�^�� (�L�[�{�[�h=None �W���C�p�b�h=12)
	LN_BUTTON_SELECT,       ///< R3�{�^�� (�L�[�{�[�h=None �W���C�p�b�h=13)
	LN_BUTTON_SUB1,         ///< R3�{�^�� (�L�[�{�[�h=None �W���C�p�b�h=14)
	LN_BUTTON_SUB2,         ///< R3�{�^�� (�L�[�{�[�h=None �W���C�p�b�h=15)
	LN_BUTTON_DOWN,         ///< ���{�^�� (�L�[�{�[�h=LN_KEY_DOWN  �W���C�p�b�h=POV��&��1Y��+)
	LN_BUTTON_LEFT,         ///< ���{�^�� (�L�[�{�[�h=LN_KEY_LEFT  �W���C�p�b�h=POV��&��1X��-)
	LN_BUTTON_RIGHT,        ///< �E�{�^�� (�L�[�{�[�h=LN_KEY_RIGHT �W���C�p�b�h=POV�E&��1X��+)
	LN_BUTTON_UP,           ///< ��{�^�� (�L�[�{�[�h=LN_KEY_UP    �W���C�p�b�h=POV��&��1Y��-)

	LN_MAX_BUTTONS
};

/// �R���g���[���̃A�^�b�` (�L�[�R���t�B�O) �^�[�Q�b�g
enum LNGameControllerAttachTarget
{
	LN_AT_BUTTON_A = 0,     ///< A�{�^��
	LN_AT_BUTTON_B,         ///< B�{�^��
	LN_AT_BUTTON_C,         ///< C�{�^��
	LN_AT_BUTTON_X,         ///< X�{�^��
	LN_AT_BUTTON_Y,         ///< Y�{�^��
	LN_AT_BUTTON_Z,         ///< Z�{�^��
	LN_AT_BUTTON_L,         ///< L�{�^��
	LN_AT_BUTTON_R,         ///< R�{�^��
	LN_AT_BUTTON_L2,        ///< L2�{�^��
	LN_AT_BUTTON_R2,        ///< R2�{�^��
	LN_AT_BUTTON_L3,        ///< L3�{�^��
	LN_AT_BUTTON_R3,        ///< R3�{�^��
	LN_AT_BUTTON_START,     ///< R3�{�^��
	LN_AT_BUTTON_SELECT,    ///< R3�{�^��
	LN_AT_BUTTON_SUB1,      ///< R3�{�^��
	LN_AT_BUTTON_SUB2,      ///< R3�{�^��
	LN_AT_BUTTON_DOWN,      ///< ���{�^��
	LN_AT_BUTTON_LEFT,      ///< ���{�^��
	LN_AT_BUTTON_RIGHT,     ///< �E�{�^��
	LN_AT_BUTTON_UP,        ///< ��{�^��

    LN_AT_AXIS_1X_MINUIS,   ///< ��1���o�[ X- ����
    LN_AT_AXIS_1X_PLUS,     ///< ��1���o�[ X+ ����
    LN_AT_AXIS_1Y_MINUIS,   ///< ��1���o�[ Y- ����
    LN_AT_AXIS_1Y_PLUS,     ///< ��1���o�[ Y+ ����
    LN_AT_AXIS_2X_MINUIS,   ///< ��2���o�[ X- ����
    LN_AT_AXIS_2X_PLUS,     ///< ��2���o�[ X+ ����
    LN_AT_AXIS_2Y_MINUIS,   ///< ��2���o�[ Y- ����
    LN_AT_AXIS_2Y_PLUS,     ///< ��2���o�[ Y+ ����


	LN_MAX_AT_TARGETS,

    LN_AT_BUTTON_BEGIN = LN_AT_BUTTON_A,
    LN_AT_BUTTON_END   = LN_AT_BUTTON_UP,
    LN_AT_AXIS_BEGIN   = LN_AT_AXIS_1X_MINUIS,
    LN_AT_AXIS_END     = LN_AT_AXIS_2Y_PLUS,

};

/// �S�Ẵf�o�C�X�p�̉��z�{�^���ԍ� (�L�[�R���t�B�O�Ŏg��  ��)attachButton( LN_BUTTON_L2, LN_VBUTTON_JOYPAD_AXIS_3_PLUS ))
enum LNVirtualButton
{
    LN_VBUTTON_NULL = 0,

    LN_VBUTTON_MOUSE_1 = 256,
    LN_VBUTTON_MOUSE_2,
    LN_VBUTTON_MOUSE_3,
    LN_VBUTTON_MOUSE_4,
    LN_VBUTTON_MOUSE_5,
    LN_VBUTTON_MOUSE_6,
    LN_VBUTTON_MOUSE_7,
    LN_VBUTTON_MOUSE_8,

    LN_VBUTTON_JOYPAD_BUTTON_1,
    LN_VBUTTON_JOYPAD_BUTTON_2,
    LN_VBUTTON_JOYPAD_BUTTON_3,
    LN_VBUTTON_JOYPAD_BUTTON_4,
    LN_VBUTTON_JOYPAD_BUTTON_5,
    LN_VBUTTON_JOYPAD_BUTTON_6,
    LN_VBUTTON_JOYPAD_BUTTON_7,
    LN_VBUTTON_JOYPAD_BUTTON_8,
    LN_VBUTTON_JOYPAD_BUTTON_9,
    LN_VBUTTON_JOYPAD_BUTTON_10,
    LN_VBUTTON_JOYPAD_BUTTON_11,
    LN_VBUTTON_JOYPAD_BUTTON_12,
    LN_VBUTTON_JOYPAD_BUTTON_13,
    LN_VBUTTON_JOYPAD_BUTTON_14,
    LN_VBUTTON_JOYPAD_BUTTON_15,
    LN_VBUTTON_JOYPAD_BUTTON_16,

    LN_VBUTTON_JOYPAD_POV_UP,
    LN_VBUTTON_JOYPAD_POV_RIGHT,
    LN_VBUTTON_JOYPAD_POV_DOWN,
    LN_VBUTTON_JOYPAD_POV_LEFT,

    LN_VBUTTON_JOYPAD_AXIS_1_MINUIS,      ///< X1 -1.0
    LN_VBUTTON_JOYPAD_AXIS_1_PLUS,        ///< X1  1.0
    LN_VBUTTON_JOYPAD_AXIS_2_MINUIS,      ///< Y1 -1.0
    LN_VBUTTON_JOYPAD_AXIS_2_PLUS,        ///< Y1  1.0
    LN_VBUTTON_JOYPAD_AXIS_3_MINUIS,      ///< X2 -1.0
    LN_VBUTTON_JOYPAD_AXIS_3_PLUS,        ///< X2  1.0
    LN_VBUTTON_JOYPAD_AXIS_4_MINUIS,      ///< Y2 -1.0
    LN_VBUTTON_JOYPAD_AXIS_4_PLUS,        ///< Y2  1.0
    LN_VBUTTON_JOYPAD_AXIS_5_MINUIS,      ///< X3 -1.0
    LN_VBUTTON_JOYPAD_AXIS_5_PLUS,        ///< X3  1.0     XInput LT
    LN_VBUTTON_JOYPAD_AXIS_6_MINUIS,      ///< Y3 -1.0
    LN_VBUTTON_JOYPAD_AXIS_6_PLUS,        ///< Y3  1.0     XInput RT
    LN_VBUTTON_JOYPAD_AXIS_7_MINUIS,      ///< X4 -1.0
    LN_VBUTTON_JOYPAD_AXIS_7_PLUS,        ///< X4  1.0
    LN_VBUTTON_JOYPAD_AXIS_8_MINUIS,      ///< Y4 -1.0
    LN_VBUTTON_JOYPAD_AXIS_8_PLUS,        ///< Y4  1.0

    LN_VBUTTON_LAST,

    LN_VBUTTON_KEYBOARD_BEGIN      = 0,        // 0�`255 �͂Ƃ肠�����L�[�{�[�h�p
    LN_VBUTTON_KEYBOARD_END        = 255,
    LN_VBUTTON_MOUSE_BEGIN         = LN_VBUTTON_MOUSE_1,
    LN_VBUTTON_MOUSE_END           = LN_VBUTTON_MOUSE_8,
    LN_VBUTTON_JOYPAD_BUTTON_BEGIN = LN_VBUTTON_JOYPAD_BUTTON_1,
    LN_VBUTTON_JOYPAD_BUTTON_END   = LN_VBUTTON_JOYPAD_BUTTON_16,
    LN_VBUTTON_JOYPAD_POV_BEGIN    = LN_VBUTTON_JOYPAD_POV_UP,
    LN_VBUTTON_JOYPAD_POV_END      = LN_VBUTTON_JOYPAD_POV_LEFT,
    LN_VBUTTON_JOYPAD_AXIS_BEGIN   = LN_VBUTTON_JOYPAD_AXIS_1_MINUIS,
    LN_VBUTTON_JOYPAD_AXIS_END     = LN_VBUTTON_JOYPAD_AXIS_8_PLUS,
};

/// ���̓f�o�C�X�̗L���t���O
enum LNInputAttachFlags
{
	LN_INPUTATTACH_NONE		= 0x00,
	LN_INPUTATTACH_MOUSE	= 0x01,		///< �}�E�X������͂��󂯎��
    LN_INPUTATTACH_KEYBOARD	= 0x02,		///< �L�[�{�[�h������͂��󂯎��
	LN_INPUTATTACH_JOYPAD	= 0x04,		///< �W���C�p�b�h������͂��󂯎��

    LN_INPUTATTACH_ALL      = 0xff,     ///< ��L���ׂẴf�o�C�X������͂��󂯎��
};

/// �}�E�X�̏�Ԃ�\���\����
struct LNMouseDeviceState
{
	long	X;
	long	Y;
	long	RelX;						        ///< 1 �t���[���O�̈ʒu����̑��� X ���W
	long	RelY;
	long	Wheel;						        ///< �z�C�[��
	bool	Buttons[ LN_MAX_MOUSE_BUTTONS ];	///< �{�^�������������Ă���ꍇ�� true
	bool	Succeeded;					        ///< �f�[�^������ł��邱�Ƃ������t���O ( false �̏ꍇ�A���O�Ɏ擾�ł����l���i�[����Ă��� )
};

/// �W���C�p�b�h�� POV �̌�����\���t���O
enum LNPOVDirFlag
{
    POV_NONE    = 0x00,
    POV_UP      = 0x01,
    POV_RIGHT   = 0x02,
    POV_DOWN    = 0x04,
    POV_LEFT    = 0x08,
};

/// �W���C�p�b�h�̏�Ԃ�\���\����
struct LNJoypadDeviceState
{
	float	Axes[ LN_MAX_JOYPAD_AXIS ];
	u32     POV;						        ///< POV ��� (POVDir �̑g�ݍ��킹)
	u8		Buttons[ LN_MAX_JOYPAD_BUTTONS ];
};

/// �^�b�`�̏�Ԃ�\���\����
struct LNTouchDeviceState
{
	long	X;
	long	Y;
	long	RelX;   ///< 1 �t���[���O�̈ʒu����̑��� X ���W
	long	RelY;
	bool	Touch;  ///< �����Ă���ꍇ�� true
};

} // namespace LNote

//-------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Input
{

class IMouse;
class IKeyboard;
class IJoypad;
class IGameController;
class IInputDevice;

//=============================================================================
// �� IManager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      ���̓��W���[���̊Ǘ��C���^�[�t�F�C�X
//*/
//=============================================================================
class IManager
    : private Base::Interface
{
public:

    /// GameController �ւ̃|�C���^�̎擾 (index_ �� 0 �` 3)
	virtual IGameController* getGameController( int index_ ) = 0;

	/// Mouse �ւ̃|�C���^�̎擾
	virtual IMouse* getMouse() = 0;

	/// Keyboard �ւ̃|�C���^�̎擾
	virtual IKeyboard* getKeyboard() = 0;

	/// Joypad �ւ̃|�C���^�̎擾
	virtual IJoypad* getJoypad( int index_ ) = 0;

	/// �ڑ�����Ă���W���C�p�b�h�̐��̎擾
	virtual int getJoypadNum() = 0;

    /// IInputDevice �ւ̃|�C���^�̎擾
    virtual IInputDevice* getInputDevice() = 0;

protected:

    virtual ~IManager() {}
};



//=============================================================================
// �� IGameController �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      ���z�Q�[���R���g���[���̃C���^�[�t�F�C�X
//*/
//=============================================================================
class IGameController
    : public Base::Interface
{
public:
	
	/// �{�^�������݉�����Ă��邩�𔻒肷��
	virtual bool isPress( LNGameControllerButton button_ ) = 0;

	/// �{�^�����V���������ꂽ�u�Ԃ𔻒肷��
	virtual bool isOnTrigger( LNGameControllerButton button_ ) = 0;

	/// �L�[�������ꂽ�u�Ԃ𔻒肷��
	virtual bool isOffTrigger( LNGameControllerButton button_ ) = 0;

	/// �{�^�����V���������ꂽ�u�Ԃƃ��s�[�g��Ԃ𔻒肷��
	virtual bool isRepeat( LNGameControllerButton button_ ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      �A�i���O���͒l�̎擾
    //
    //  @param[in]  button_ : �Q�[���R���g���[���̃{�^��
    //
    //  @return     0.0�`1.0
    //
    //  @par
    //              �w��{�^���̓��͂��A�i���O�l�Ŏ󂯎��܂��B<br>
    //              �����A�^�b�`����Ă���ꍇ���A���̎�����Ԃ��܂��B<br>
    //              Xbox360 �R���g���[���� LT RT �{�^���̒l���擾����Ƃ����Ɏg����Ǝv���܂��B
    //*/
    //---------------------------------------------------------------------
	virtual float getPower( LNGameControllerButton button_ ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      ���̏�Ԃ̎擾
    //
    //  @param[in]  axis_ : ���ԍ� (0 �` 3)
    //
    //  @par
    //              ���o�[�̃A�i���O���͂��AX �� Y ���Z�b�g�ɂ��ĕԂ��܂��B<br>
    //              IJoypad::getAxisState() �Ƃ͈����̈Ӗ����Ⴄ�̂Œ��ӂ��Ă��������B
    //*/
    //---------------------------------------------------------------------
	virtual const LVector2& getAxisState( int axis_ ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      �V���������ꂽ���z�{�^���̎擾
    //
    //  @return     �����ꂽ�L�[(���z�L�[�R�[�h)�A�{�^���A���ɑΉ����鉼�z�{�^��
    //              
    //  @par
    //              ��������̓g���K�[�ł��BisOnTrigger() �Ɠ������A�����ꂽ�t���[������
    //              �Ή����鉼�z�{�^����Ԃ��A����ȊO�� LN_VBUTTON_NONE (������Ă��Ȃ�) ��Ԃ��܂��B
    //*/
    //---------------------------------------------------------------------
	virtual int getPressedButton() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �{�^���z�u (�L�[�R���t�B�O) ��ݒ肷��
    //
    //  @param[in]  ln_button_   : �ύX����{�^��
    //  @param[in]  v_button_    : �V�����ݒ肷�鉼�z�{�^��
    //  @param[in]  duplication_ : true �̏ꍇ�A�d����������
    //
    //  @par
    //              ���z�{�^�����Q�[���R���g���[���̃{�^���ɃA�^�b�`���܂��B<br>
    //              <br>
    //              ��{�I�� getPressedButton() �ƃZ�b�g�Ŏg���AgetPressedButton()����
    //              �󂯎�������z�{�^���ԍ������̊֐��� v_button_ �ɓn���܂��B<br>
    //              <br>
    //              duplication_ �� false �̏ꍇ�Aln_button_ �ȊO�̃{�^����
    //              �����L�[�����蓖�Ă��Ă���ꍇ�A�l���������܂��B
    //*/
    //---------------------------------------------------------------------
	virtual void attachVirtualButton( LNGameControllerAttachTarget target_, int v_button_, bool duplication_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �{�^���z�u (�L�[�R���t�B�O) ����������
    //
    //  @param[in]  ln_button_ : �ύX����{�^��
    //  @param[in]  v_button_  : �V�����ݒ肷�鉼�z�{�^��
    //
    //  @par
    //              v_button_ �� ln_button_ �ɃA�^�b�`����Ă���ꍇ�A�A�^�b�`���������܂��B<br>
    //              v_button_ �� LN_VBUTTON_NULL ��n���ƁA���ׂẴA�^�b�`���������܂��B
    //*/
    //---------------------------------------------------------------------
	virtual void detachVirtualButton( LNGameControllerAttachTarget target_, int v_button_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���͂��󂯎��f�o�C�X�̎w��
    // 
    //  @param[in]  flags_ : LNInputAttachFlags �̂ЂƂȏ�̃t���O�̑g�ݍ��킹
    // 
    //  @par
    //              �f�t�H���g�� LN_INPUTATTACH_ALL �ł��B<br>
    //              <br>
    //              �Ⴆ�� LN_INPUTATTACH_MOUSE | LN_INPUTATTACH_KEYBOARD �Ɛݒ肵���ꍇ�A
    //              ���̃Q�[���R���g���[���ɃW���C�p�b�h���ڑ�����Ă��Ă����ꂩ��̓��͈͂�؎󂯎��܂���B
    //*/
    //---------------------------------------------------------------------
	virtual void setDeviceAttachFlag( u8 flags_ ) = 0;

protected:

    virtual ~IGameController() {}
};



//=============================================================================
// �� IMouse �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �}�E�X����̓��͂������C���^�[�t�F�C�X
//*/
//=============================================================================
class IMouse
    : public Base::Interface
{
public:

	/// �{�^�������݉�����Ă��邩�𔻒肷��
	virtual bool isPress( LNMouseButton button_ ) = 0;

	/// �{�^�����V���������ꂽ�u�Ԃ𔻒肷��
	virtual bool isOnTrigger( LNMouseButton button_ ) = 0;

	/// �{�^���������ꂽ�u�Ԃ𔻒肷��
	virtual bool isOffTrigger( LNMouseButton button_ ) = 0;

	/// �{�^�����V���������ꂽ�u�Ԃƃ��s�[�g��Ԃ𔻒肷��
	virtual bool isRepeat( LNMouseButton button_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �}�E�X���W�̐ݒ�
    //
    //  @par
    //              ���W�̓N���C�A���g�̈�̍�������_ (0, 0) �Ƃ����l�Őݒ肵�܂��B
    //              �E�B���h�E���A�N�e�B�u�ł͂Ȃ��ꍇ�͂Ȃɂ����܂���B
    //*/
    //---------------------------------------------------------------------
	virtual void setPoint( const LVector2& point_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �}�E�X���W�̎擾
    //
    //  @par
    //              ���W�̓N���C�A���g�̈�̍�������_ (0, 0) �Ƃ����l�ł��B
    //*/
    //---------------------------------------------------------------------
	virtual const LVector2& getPoint() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      1 �t���[���O����̈ړ��ʂ̎擾
    //*/
    //---------------------------------------------------------------------
	virtual const LVector2& getMoveDistance() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      1 �t���[���̃z�C�[���̉�]�ʂ̎擾
    //
    //  @return
    //              ���̒l�͑O(��)�A���̒l�͌��(��O)�։񂳂ꂽ�����Ӗ����܂��B
    //              ���̊֐��͉񂵂��񐔂�Ԃ��܂��B
    //*/
    //---------------------------------------------------------------------
	virtual int getWheelState() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �ʒu�𒆉��ɌŒ肷�邩�̐ݒ�
    //
    //  @par        
    //              true ��n���ƁA�}�E�X�J�[�\�����N���C�A���g�̈撆����
    //              �Œ肵�܂��BgetMoveDistance() �ňړ��ʂ��擾���Ă��������B
    //*/
    //---------------------------------------------------------------------
	virtual void setEnableCenterFix( bool flag_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���s�[�g�Ԋu�̐ݒ�
    //
    //  @param[in]  first_    : ���s�[�g���J�n����܂ł̃t���[���� (�f�t�H���g�� 20)
    //  @param[in]  interval_ : ���s�[�g�Ԋu�t���[���� (�f�t�H���g�� 5)
    //*/
    //---------------------------------------------------------------------
	virtual void setRepeatInterval( u32 first_, u32 interval_ ) = 0;

protected:

    virtual ~IMouse() {}
};

//=============================================================================
// �� IKeyboard �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �L�[�{�[�h����̓��͂������C���^�[�t�F�C�X
//*/
//=============================================================================
class IKeyboard
    : public Base::Interface
{
public:
	
    //---------------------------------------------------------------------
    ///**
    //  @brief      �L�[�����݉�����Ă��邩�𔻒肷��
    //
    //  @param[in]  vkey_    : ���z�L�[�R�[�h
    //*/
    //---------------------------------------------------------------------
	virtual bool isPress( LNVirtualKeyCode vkey_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L�[���V���������ꂽ�u�Ԃ𔻒肷��
    //
    //  @param[in]  vkey_    : ���z�L�[�R�[�h
    //*/
    //---------------------------------------------------------------------
	virtual bool isOnTrigger( LNVirtualKeyCode vkey_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L�[�������ꂽ�u�Ԃ𔻒肷��
    //
    //  @param[in]  vkey_    : ���z�L�[�R�[�h
    //*/
    //---------------------------------------------------------------------
	virtual bool isOffTrigger( LNVirtualKeyCode vkey_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �{�^�����V���������ꂽ�u�Ԃƃ��s�[�g��Ԃ𔻒肷��
    //
    //  @param[in]  vkey_    : ���z�L�[�R�[�h
    //*/
    //---------------------------------------------------------------------
	virtual bool isRepeat( LNVirtualKeyCode vkey_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L�[�{�[�h�̃o�b�t�@�����O�f�[�^�̎擾
    //
    //  @return     ���z�L�[�R�[�h
    //
    //  @par
    //              0 ��Ԃ��܂� while �ŉ񂵂Ď擾���Ă��������B
    //*/
    //---------------------------------------------------------------------
    virtual LNVirtualKeyCode getKeyBufferState() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���s�[�g�Ԋu�̐ݒ�
    //
    //  @param[in]  first_    : ���s�[�g���J�n����܂ł̃t���[���� (�f�t�H���g�� 20)
    //  @param[in]  interval_ : ���s�[�g�Ԋu�t���[���� (�f�t�H���g�� 5)
    //*/
    //---------------------------------------------------------------------
	virtual void setRepeatInterval( u32 first_, u32 interval_ ) = 0;

protected:

	virtual ~IKeyboard() {};
};

//=============================================================================
// �� IJoypad �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �W���C�p�b�h�̃C���^�[�t�F�C�X
//*/
//=============================================================================
class IJoypad
    : public Base::Interface
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �ڑ��ς݂����`�F�b�N����
    //
    //  @retval     true  : �ڑ��ς� (�g�p�\)
    //  @retval     false : ���ڑ� (�g�p�s��)
    //
    //  @par
    //              ���ڑ��̏ꍇ�A�{�^���̏�Ԃ� false�A���� 0.0�APOV �� 0 ��Ԃ��܂��B
    //*/
    //---------------------------------------------------------------------
	virtual bool isConnected() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �{�^�������݉�����Ă��邩�𔻒肷��
    //
    //  @param[in]  button_ : �{�^���ԍ� (0 �` 15)
    //*/
    //---------------------------------------------------------------------
	virtual bool isPress( int button_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �{�^�����V���������ꂽ�u�Ԃ𔻒肷��
    //
    //  @param[in]  button_ : �{�^���ԍ� (0 �` 15)
    //*/
    //---------------------------------------------------------------------
	virtual bool isOnTrigger( int button_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �{�^���������ꂽ�u�Ԃ𔻒肷��
    //
    //  @param[in]  button_ : �{�^���ԍ� (0 �` 15)
    //*/
    //---------------------------------------------------------------------
	virtual bool isOffTrigger( int button_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �{�^�����V���������ꂽ�u�Ԃƃ��s�[�g��Ԃ𔻒肷��
    //
    //  @param[in]  button_ : �{�^���ԍ� (0 �` 15)
    //*/
    //---------------------------------------------------------------------
	virtual bool isRepeat( int button_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���̏�Ԃ̎擾
    //
    //  @param[in]  axis_ : ���ԍ� (0 �` 7)
    //*/
    //---------------------------------------------------------------------
    virtual float getAxisState( int axis_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      POV �̏�Ԃ̎擾
    //*/
    //---------------------------------------------------------------------
	virtual u32 getPOVState() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �W���C�p�b�h�̐U�����J�n����
    //
    //  @param[in]  power_ : �U���̋��� (0�`1000)
    //  @param[in]  time_  : �U�������鎞�� (�~���b�P��)
    //
    //  @par
    //              time_ �� 0 �ȉ��̒l��ݒ肷��ƁAstopVibration() ��
    //              �Ă΂��܂ŐU���𑱂��܂��B
    //*/
    //---------------------------------------------------------------------
	virtual void startVibration( int power_, int time_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �W���C�p�b�h�̐U�����~����
    //*/
    //---------------------------------------------------------------------
	virtual void stopVibration() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���s�[�g�Ԋu�̐ݒ�
    //
    //  @param[in]  first_    : ���s�[�g���J�n����܂ł̃t���[���� (�f�t�H���g�� 20)
    //  @param[in]  interval_ : ���s�[�g�Ԋu�t���[���� (�f�t�H���g�� 5)
    //*/
    //---------------------------------------------------------------------
	virtual void setRepeatInterval( u32 first_, u32 interval_ ) = 0;

protected:

	virtual ~IJoypad() {};
};


//=============================================================================
// �� IInputDevice �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      ���̓f�o�C�X���ꊇ���Ĉ����C���^�[�t�F�C�X
//
//  @par
//              ���̃C���^�[�t�F�C�X�͎�Ƀ��C�u���������p�Ɏg���Ă��܂��B
//*/
//=============================================================================
class IInputDevice
    : public Base::Interface
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �ڑ��ς݂̃W���C�p�b�h���̎擾
    //*/
    //---------------------------------------------------------------------
	virtual int getJoypadNum() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �}�E�X�̏�Ԃ̎擾
    //*/
    //---------------------------------------------------------------------
	virtual const LNMouseDeviceState& getMouseState() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L�[�{�[�h�̏�Ԃ̎擾
    //
    //  @param[in]  vkey_ : ���z�L�[�R�[�h 
    //*/
    //---------------------------------------------------------------------
	virtual bool getKeyState( LNVirtualKeyCode vkey_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L�[�{�[�h�̃o�b�t�@�����O�f�[�^�̎擾
    //
    //  @return     ���z�L�[�R�[�h
    //
    //  @par
    //              LN_KEY_UNKNOWN ��Ԃ��܂� while �ŉ񂵂Ď擾���Ă��������B
    //*/
    //---------------------------------------------------------------------
    virtual LNVirtualKeyCode getKeyBufferState() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �W���C�p�b�h�̏�Ԃ̎擾
    //*/
    //---------------------------------------------------------------------
	virtual void getJoypadState( int joypad_id_, LNJoypadDeviceState* state_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �W���C�p�b�h�̐U�����J�n����
    //*/
    //---------------------------------------------------------------------
	virtual void startVibration( int joypad_id_, int power_, int time_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �W���C�p�b�h�̐U�����~����
    //*/
    //---------------------------------------------------------------------
	virtual void stopVibration( int joypad_id_ ) = 0;
    
    /// �^�b�`���̎擾
    virtual void getTouchState( int touch_id_, LNTouchDeviceState* state_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���z�{�^���̏�Ԃ̎擾
    //
    //  @param[in]  button_     : ���ׂ鉼�z�{�^�� (���z�L�[�R�[�h�܂��� LNVirtualDeviceButton)
    //  @param[in]  keyboard_   : true �̏ꍇ�A�L�[�{�[�h�̏�Ԃ��l������
    //  @param[in]  mouse_      : true �̏ꍇ�A�}�E�X�̏�Ԃ��l������
    //  @param[in]  joypad_id_  : 0 �ȏ� �̏ꍇ�A���̔ԍ��̃W���C�p�b�h�̏�Ԃ��l������
    //
    //  @return
    //              0.0�`1.0 �̒l
    //*/
    //---------------------------------------------------------------------
	virtual float getVirtualButtonState( u32 button_, bool keyboard_, bool mouse_, int joypad_id_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �}�E�X���W�̐ݒ�
    //
    //  @par
    //              ���W�̓N���C�A���g�̈�̍�������_ (0, 0) �Ƃ����l�Őݒ肵�܂��B
    //              �E�B���h�E���A�N�e�B�u�ł͂Ȃ��ꍇ�͂Ȃɂ����܂���B
    //*/
    //---------------------------------------------------------------------
	virtual void setMousePoint( const LVector2& point_ ) = 0;

    /// �}�E�X�J�[�\���̈ʒu���N���C�A���g�̈�̒����ɐݒ肷��
    virtual void setMouseCursorCenter() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���͏��̍X�V
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT update() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �f�o�C�X�̍ăZ�b�g�A�b�v
    //
    //  @par
    //              USB �@��̐ڑ����m���ȂǂɌĂяo���܂��B
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT refresh() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �֘A�t�����Ă��� IWindow �̎擾
    //*/
    //---------------------------------------------------------------------
    virtual System::IWindow* getWindow() = 0;
    
    /// �C�x���g���� (Manager::onEvent() ����Ă΂��)
    virtual bool onEvent( const System::Event& args_ ) = 0;

protected:

    virtual ~IInputDevice() {}
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