//=============================================================================
//�y System �z
//-----------------------------------------------------------------------------
///**
//  @file       System.h
//  @brief      System
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Geometry/DrawingShapes.h"

#if defined(LNOTE_ANDROID)
struct ANativeWindow;
#endif

//-------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------
namespace LNote
{



} // namespace LNote

#include "Event.h"

//-------------------------------------------------------------------------
// �� interface
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{
class IWindow;
class IEventListener;

#if 0
//=============================================================================
// �� IManager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �V�X�e�� (OS ����) �̊Ǘ��C���^�[�t�F�C�X
//*/
//=============================================================================
class IManager
    : public Base::Interface
{
public:

    /// �E�B���h�E���擾����
    virtual IWindow* getWindow() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief		���b�Z�[�W����
    //
    //  @reval		true  : ����
    //  @reval		false : �I���v�������邽�߁A�A�v���P�[�V�������I������K�v������
    //
    //  @par
    //              ���[�U�[��`�̃E�B���h�E�n���h�����g�p���Ă���ꍇ��
    //              ��؂̏������s�킸�A��� false ��Ԃ��܂��B
    //*/
    //---------------------------------------------------------------------
    virtual bool processMessage() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief		�C�x���g���X�i�[�̃A�^�b�`
    //
    //  @par
    //              �C�x���g���X�i�[��o�^���܂��B���X�i�[�͕����o�^���邱�Ƃ��ł��A
    //              priority_ �̒l���傫�����̂��珇�� (�����ꍇ�̓A�^�b�`���ꂽ����) �C�x���g�����M����܂��B<br>   
    //              �A�^�b�`�������X�i�[�͊J������O�ɕK�� detachEventListener() ��
    //              �o�^���������Ă��������B<br>
    //              <br>
    //              Engine::Framework ���玩���I�ɓo�^����郊�X�i�[�̗D��x��
    //              enum LNDefaultELPriority �Œ�`����Ă��܂��B
    //*/
    //---------------------------------------------------------------------
    virtual void attachEventListener( IEventListener* listener_, int priority_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief		�C�x���g���X�i�[�̃f�^�b�`
    //
    //  @par
    //              �n���ꂽ���X�i�[���A�^�b�`����Ă���ꍇ�A�o�^���������܂��B
    //*/
    //---------------------------------------------------------------------
    virtual void detachEventListener( IEventListener* listener_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief		�C�x���g���|�X�g����
    //*/
    //---------------------------------------------------------------------
    virtual void postEvent( const LNEvent& event_ ) = 0;

#if defined(LNOTE_WINDOWS)

    //---------------------------------------------------------------------
    ///**
    //  @brief		�E�B���h�E���b�Z�[�W�̏���
    //
    //  @param[out] handled_ : ���b�Z�[�W�������s��ꂽ���������t���O���i�[����ϐ��ւ̃|�C���^
    //
    //  @par
    //              ���[�U�[��`�̃E�B���h�E�n���h�����g�p���Ă���ꍇ��
    //              ���̃E�B���h�E�̃E�B���h�E�v���V�[�W���֐��̒���
    //              ���̊֐����Ăяo���Ă��������B
    //*/
    //---------------------------------------------------------------------
    virtual LRESULT WndProc( HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_, bool* handled_ ) = 0;

#elif defined(LNOTE_X11)

    //---------------------------------------------------------------------
    ///**
    //  @brief		�E�B���h�E���b�Z�[�W�̏���
    //
    //  @param[out] handled_ : ���b�Z�[�W�������s��ꂽ���������t���O���i�[����ϐ��ւ̃|�C���^
    //
    //  @par
    //              ���[�U�[��`�̃E�B���h�E�n���h�����g�p���Ă���ꍇ��
    //              ���̃E�B���h�E�̃E�B���h�E�v���V�[�W���֐��̒���
    //              ���̊֐����Ăяo���Ă��������B
    //*/
    //---------------------------------------------------------------------
    //virtual void WndProc( XEvent event_, bool* handled_ ) = 0;

#endif

protected:

    virtual ~IManager() {};
};
#endif

//=============================================================================
// �� IWindow �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �E�B���h�E�̃C���^�[�t�F�C�X
//*/
//=============================================================================
class IWindow
    : public Base::Interface
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �N���C�A���g�̈�̃T�C�Y���擾����
    //*/	
    //---------------------------------------------------------------------
	virtual const Geometry::Size& getSize() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �E�B���h�E�̉���Ԃ�ݒ肷��
    //
    //  @param[in]  flag_ : true ��ݒ肷��ƕ\������
    //*/	
    //---------------------------------------------------------------------
	virtual void setVisible( bool flag_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �t���X�N���[���̗L���ݒ�
    // 
    //  @param[in]  flag_ : �L���t���O
    // 
    //  @par
    //              flag_ �� true ��ݒ肷��ƃt���X�N���[���A
    //              false ��ݒ肷��ƃE�B���h�E���[�h�ɂȂ�܂��B<br>
    //              ���̊֐��̓E�B���h�E�X�^�C����ύX���čő剻���邾���ł��B<br>
    //              <br>
    //              ���[�U�[��`�E�B���h�E�̏ꍇ�͂Ȃɂ����܂���B<br>
    //*/	
    //---------------------------------------------------------------------
	virtual void setEnableFullScreen( bool flag_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �t���X�N���[����Ԃ��𔻒肷��
    //
    //  @par
    //              ���[�U�[��`�E�B���h�E�̏ꍇ�͏�� false �ł��B
    //*/	
    //---------------------------------------------------------------------
    virtual bool isFullScreen() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �E�B���h�E�̃A�N�e�B�u��Ԃ𔻒肷��
    //
    //  @retval     true  : �A�N�e�B�u
    //  @retval     false : ��A�N�e�B�u
    //*/	
    //---------------------------------------------------------------------
	virtual bool isActive() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �^�C�g���o�[�̕�����̌��ɒǉ��ŕ\�����镶�����ݒ肷��
    //
    //  @note
    //              ��{�I�ɓ����p�BFPS �̕\���Ɏg���Ă܂��B
    //              64 �����܂Őݒ�\�B<br>
    //              <br>
    //              ���[�U�[��`�E�B���h�E�̏ꍇ�͂Ȃɂ����܂���B<br>
    //*/	
    ////---------------------------------------------------------------------
	virtual void setWindowTextFooter( const lnChar* format_, ... ) = 0;


#if defined(LNOTE_WINDOWS)
    /// �E�B���h�E�n���h���̎擾
    virtual HWND getWindowHandle() = 0;
#elif defined(LNOTE_ANDROID)
    /// ANativeWindow �̎擾
    virtual ANativeWindow* getNativeWindow() = 0;
#endif

protected:

    virtual ~IWindow() {}
};

//=============================================================================
// �� IEventListener �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �C�x���g�������s���N���X�̊��
//*/
//=============================================================================
class IEventListener
{
public:

    //---------------------------------------------------------------------
    ///**
    //	@brief      ���b�Z�[�W�����R�[���o�b�N
    //
    //  @return     true �̏ꍇ�A�Ăяo�����ő��̏������s�킸�ɁA�����ɏ������I�����܂��B
    //*/	
    //---------------------------------------------------------------------
    virtual bool onEvent( const Event& event_ ) = 0;//const LNEvent& args_ ) = 0;


#if defined(LNOTE_WINDOWS)

    //---------------------------------------------------------------------
    ///**
    //	@brief      ���b�Z�[�W����
    //
    //  @param[out] handled_ : ���b�Z�[�W�̏����ς݂�`����ꍇ�� true ��Ԃ��悤�ɂ���
    //
    //  @par
    //              ���̊֐��� LNote �� windows �̃��b�Z�[�W�������n�߂�O�ɌĂ΂�܂��B<br>
    //              LNote ����������ׂ����b�Z�[�W�܂߂đS�Ẵ��b�Z�[�W�������ė��܂��B<br>
    //              <br>
    //              ���̊֐����g���ă��b�Z�[�W������������A�Ăяo������
    //              DefWindowProc() �⑼�̏������Ăяo���K�v���Ȃ��ꍇ (WndProc() �I����A�������Ăق����Ȃ��Ƃ�) ��
    //              handled_ �� true ��Ԃ��Ă��������B<br>
    //              ���̊֐����I�������シ���Ƀv���V�[�W���֐��𔲂���悤�ɂȂ�܂��B
    //*/	
    //---------------------------------------------------------------------
    virtual LRESULT WndProc( HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_, bool* handled_ ) { return 0; *handled_ = false; }

#endif
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace System
} // namespace Core

} // namespace LNote

#if 0
/// �L�[�{�[�h�̉��z�L�[�R�[�h
enum LNVirtualKeyCode
{
	LN_KEY_UNKNOWN = 0,

#if defined(LNOTE_WINDOWS)
    LN_KEY_A                    = 'A',
    LN_KEY_B                    = 'B',
    LN_KEY_C                    = 'C',
    LN_KEY_D                    = 'D',
    LN_KEY_E                    = 'E',
    LN_KEY_F                    = 'F',
    LN_KEY_G                    = 'G',
    LN_KEY_H                    = 'H',
    LN_KEY_I                    = 'I',
    LN_KEY_J                    = 'J',
    LN_KEY_K                    = 'K',
    LN_KEY_L                    = 'L',
    LN_KEY_M                    = 'M',
    LN_KEY_N                    = 'N',
    LN_KEY_O                    = 'O',
    LN_KEY_P                    = 'P',
    LN_KEY_Q                    = 'Q',
    LN_KEY_R                    = 'R',
    LN_KEY_S                    = 'S',
    LN_KEY_T                    = 'T',
    LN_KEY_U                    = 'U',
    LN_KEY_V                    = 'V',
    LN_KEY_W                    = 'W',
    LN_KEY_X                    = 'X',
    LN_KEY_Y                    = 'Y',
    LN_KEY_Z                    = 'Z',

    LN_KEY_0                    = '0',
    LN_KEY_1                    = '1',
    LN_KEY_2                    = '2',
    LN_KEY_3                    = '3',
    LN_KEY_4                    = '4',
    LN_KEY_5                    = '5',
    LN_KEY_6                    = '6',
    LN_KEY_7                    = '7',
    LN_KEY_8                    = '8',
    LN_KEY_9                    = '9',

	LN_KEY_F1					= VK_F1,
	LN_KEY_F2					= VK_F2,
	LN_KEY_F3					= VK_F3,
	LN_KEY_F4					= VK_F4,
	LN_KEY_F5					= VK_F5,
	LN_KEY_F6					= VK_F6,
	LN_KEY_F7					= VK_F7,
	LN_KEY_F8					= VK_F8,
	LN_KEY_F9					= VK_F9,
	LN_KEY_F10					= VK_F10,
	LN_KEY_F11					= VK_F11,
	LN_KEY_F12					= VK_F12,

	LN_KEY_SPACE				= VK_SPACE,		// 0x20 = ' ' (�X�y�[�X)
	LN_KEY_ESCAPE				= VK_ESCAPE,
	LN_KEY_UP					= VK_UP,
	LN_KEY_DOWN					= VK_DOWN,
	LN_KEY_LEFT					= VK_LEFT,
	LN_KEY_RIGHT				= VK_RIGHT,
	LN_KEY_LSHIFT				= VK_LSHIFT,
	LN_KEY_RSHIFT				= VK_RSHIFT,
	LN_KEY_LCTRL				= VK_LCONTROL,
	LN_KEY_RCTRL				= VK_RCONTROL,
	LN_KEY_LALT					= VK_LMENU,
	LN_KEY_RALT					= VK_RMENU,
	LN_KEY_TAB					= VK_TAB,
	LN_KEY_ENTER				= VK_RETURN,
	LN_KEY_BACKSPACE			= VK_BACK,
	LN_KEY_INSERT				= VK_INSERT,
	LN_KEY_DELETE				= VK_DELETE,
	LN_KEY_PAGEUP				= VK_PRIOR,
	LN_KEY_PAGEDOWN				= VK_NEXT,
	LN_KEY_HOME					= VK_HOME,
	LN_KEY_END					= VK_END,

    LN_KEY_COLON                = VK_OEM_1,			///< :
    LN_KEY_SEMICOLON            = VK_OEM_PLUS,		///< ;
    LN_KEY_COMMA                = VK_OEM_COMMA,		///< ,
    LN_KEY_PERIOD               = VK_OEM_PERIOD,	///< .
	LN_KEY_SLASH				= VK_OEM_2,			///< /
	LN_KEY_MINUS				= VK_OEM_MINUS,		///< -
	LN_KEY_BACKSLASH			= VK_OEM_102,		///< \(�o�b�N�X���b�V��)
	LN_KEY_YEN					= VK_OEM_4,			///< \(BaskSpace�L�[�̍���\�L�[)
	LN_KEY_CARET				= VK_OEM_7,			///< ^
	LN_KEY_LBRACKET				= VK_OEM_4,			//< [
	LN_KEY_RBRACKET				= VK_OEM_6,			//< ]




	/*LN_KEY_NUMPAD0				= VK_NUMPAD0,
	LN_KEY_NUMPAD1				= VK_NUMPAD1,
	LN_KEY_NUMPAD2				= VK_NUMPAD2,
	LN_KEY_NUMPAD3				= VK_NUMPAD2,
	LN_KEY_NUMPAD4				= VK_NUMPAD4,
	LN_KEY_NUMPAD5				= VK_NUMPAD5,
	LN_KEY_NUMPAD6				= VK_NUMPAD6,
	LN_KEY_NUMPAD7				= VK_NUMPAD7,
	LN_KEY_NUMPAD8				= VK_NUMPAD8,
	LN_KEY_NUMPAD9				= VK_NUMPAD9,
	NumPad �� + - �����K�v�B���͕ۗ� */

	//LN_KEY_ADD					= VK_ADD,		    ///< +
	//LN_KEY_SUBTRACT				= VK_SUBTRACT,	    ///< -
	//LN_KEY_MULTIPLY				= VK_MULTIPLY,	    ///< *
	//LN_KEY_DIVIDE				= VK_DIVIDE,		///< /
	//LN_KEY_DECIMAL				= VK_DECIMAL,		///< .

	//LN_KEY_NUM_LOCK				= VK_NUMLOCK,
	//LN_KEY_CAPS_LOCK			= VK_OEM_ATTN,

	//LN_KEY_SHIFT				= VK_SHIFT,
	//LN_KEY_CONTROL				= VK_CONTROL,
	//LN_KEY_ALT					= VK_MENU,
    
#elif defined(LNOTE_X11)
    LN_KEY_A                    = XK_A,
    LN_KEY_B                    = XK_B,
    LN_KEY_C                    = XK_C,
    LN_KEY_D                    = XK_D,
    LN_KEY_E                    = XK_E,
    LN_KEY_F                    = XK_F,
    LN_KEY_G                    = XK_G,
    LN_KEY_H                    = XK_H,
    LN_KEY_I                    = XK_I,
    LN_KEY_J                    = XK_J,
    LN_KEY_K                    = XK_K,
    LN_KEY_L                    = XK_L,
    LN_KEY_M                    = XK_M,
    LN_KEY_N                    = XK_N,
    LN_KEY_O                    = XK_O,
    LN_KEY_P                    = XK_P,
    LN_KEY_Q                    = XK_Q,
    LN_KEY_R                    = XK_R,
    LN_KEY_S                    = XK_S,
    LN_KEY_T                    = XK_T,
    LN_KEY_U                    = XK_U,
    LN_KEY_V                    = XK_V,
    LN_KEY_W                    = XK_W,
    LN_KEY_X                    = XK_X,
    LN_KEY_Y                    = XK_Y,
    LN_KEY_Z                    = XK_Z,
    LN_KEY_0                    = XK_0,
    LN_KEY_1                    = XK_1,
    LN_KEY_2                    = XK_2,
    LN_KEY_3                    = XK_3,
    LN_KEY_4                    = XK_4,
    LN_KEY_5                    = XK_5,
    LN_KEY_6                    = XK_6,
    LN_KEY_7                    = XK_7,
    LN_KEY_8                    = XK_8,
    LN_KEY_9                    = XK_9,

    LN_KEY_SPACE				= XK_space,		// 0x20 = ' ' (�X�y�[�X)
	LN_KEY_ESCAPE				= XK_Escape,
	LN_KEY_F1					= XK_F1,
	LN_KEY_F2					= XK_F2,
	LN_KEY_F3					= XK_F3,
	LN_KEY_F4					= XK_F4,
	LN_KEY_F5					= XK_F5,
	LN_KEY_F6					= XK_F6,
	LN_KEY_F7					= XK_F7,
	LN_KEY_F8					= XK_F8,
	LN_KEY_F9					= XK_F9,
	LN_KEY_F10					= XK_F10,
	LN_KEY_F11					= XK_F11,
	LN_KEY_F12					= XK_F12,
	LN_KEY_UP					= XK_Up,
	LN_KEY_DOWN					= XK_Down,
	LN_KEY_LEFT					= XK_Left,
	LN_KEY_RIGHT				= XK_Right,
	LN_KEY_LSHIFT				= XK_Shift_L,
	LN_KEY_RSHIFT				= XK_Shift_R,
	LN_KEY_LCTRL				= XK_Control_L,
	LN_KEY_RCTRL				= XK_Control_R,
	LN_KEY_LALT					= XK_Alt_L,
	LN_KEY_RALT					= XK_Alt_R,
	LN_KEY_TAB					= XK_Tab,
	LN_KEY_ENTER				= XK_Return,
	LN_KEY_BACKSPACE			= XK_BackSpace,
	LN_KEY_INSERT				= XK_Insert,
	LN_KEY_DELETE				= XK_KP_Delete,
	LN_KEY_PAGEUP				= XK_KP_Page_Up,
	LN_KEY_PAGEDOWN				= XK_KP_Page_Down,
	LN_KEY_HOME					= XK_KP_Home,
	LN_KEY_END					= XK_KP_End,
	LN_KEY_NUMPAD0				= XK_KP_0,
	LN_KEY_NUMPAD1				= XK_KP_1,
	LN_KEY_NUMPAD2				= XK_KP_2,
	LN_KEY_NUMPAD3				= XK_KP_3,
	LN_KEY_NUMPAD4				= XK_KP_4,
	LN_KEY_NUMPAD5				= XK_KP_5,
	LN_KEY_NUMPAD6				= XK_KP_6,
	LN_KEY_NUMPAD7				= XK_KP_7,
	LN_KEY_NUMPAD8				= XK_KP_8,
	LN_KEY_NUMPAD9				= XK_KP_9,

	LN_KEY_ADD					= XK_KP_Add,		    ///< +
	LN_KEY_SUBTRACT				= XK_KP_Subtract,	    ///< -
	LN_KEY_MULTIPLY				= XK_KP_Multiply,	    ///< *
	LN_KEY_DIVIDE				= XK_KP_Divide,		    ///< /
	LN_KEY_DECIMAL				= XK_KP_Decimal,		///< .

    LN_KEY_COLON                = XK_colon,             ///< :
    LN_KEY_SEMICOLON            = XK_semicolon,         ///< ;
    LN_KEY_COMMA                = XK_comma,             ///< ,
    LN_KEY_PERIOD               = XK_period,            ///< .
    // XK_slash
	LN_KEY_NUM_LOCK				= XK_Num_Lock,
	LN_KEY_CAPS_LOCK			= XK_Caps_Lock,
	//LN_KEY_SCROLL_LOCK	    = 
	//LN_KEY_PAUSE				=

	LN_KEY_SHIFT				= XK_Shift_L,
	LN_KEY_CONTROL				= XK_Control_L,
	LN_KEY_ALT					= XK_Alt_L,
    
#elif defined(LNOTE_ANDROID)
    LN_KEY_A,
    LN_KEY_B,
    LN_KEY_C,
    LN_KEY_D,
    LN_KEY_E,
    LN_KEY_F,
    LN_KEY_G,
    LN_KEY_H,
    LN_KEY_I,
    LN_KEY_J,
    LN_KEY_K,
    LN_KEY_L,
    LN_KEY_M,
    LN_KEY_N,
    LN_KEY_O,
    LN_KEY_P,
    LN_KEY_Q,
    LN_KEY_R,
    LN_KEY_S,
    LN_KEY_T,
    LN_KEY_U,
    LN_KEY_V,
    LN_KEY_W,
    LN_KEY_X,
    LN_KEY_Y,
    LN_KEY_Z,
    LN_KEY_0,
    LN_KEY_1,
    LN_KEY_2,
    LN_KEY_3,
    LN_KEY_4,
    LN_KEY_5,
    LN_KEY_6,
    LN_KEY_7,
    LN_KEY_8,
    LN_KEY_9,

	LN_KEY_SPACE,
	LN_KEY_ESCAPE,      ///< [�~Android]
	
	LN_KEY_UP,          ///< KEYCODE_DPAD_DOWN
	LN_KEY_DOWN,
	LN_KEY_LEFT,
	LN_KEY_RIGHT,
	LN_KEY_LSHIFT,
	LN_KEY_RSHIFT,
	LN_KEY_LCTRL,
	LN_KEY_RCTRL,
	LN_KEY_LALT,
	LN_KEY_RALT,
	LN_KEY_TAB,
	LN_KEY_ENTER,           ///< [Android] AKEYCODE_DPAD_CENTER��
	LN_KEY_BACKSPACE,
	LN_KEY_INSERT,      ///< [�~Android]
	LN_KEY_DELETE,      ///< [�~Android]
	LN_KEY_PAGEUP,
	LN_KEY_PAGEDOWN,
    LN_KEY_HOME,            ///< [�~Android] (�[���� HOME �� Windows �� Home �ł͈Ӗ����Ⴄ�̂ōl����)
	LN_KEY_END,        ///< [�~Android]

	LN_KEY_ADD,
	LN_KEY_SUBTRACT,
	LN_KEY_MULTIPLY,
	LN_KEY_DIVIDE,

    LN_KEY_COLON,        ///< [�~Android]
    LN_KEY_SEMICOLON,
    LN_KEY_COMMA,
    LN_KEY_PERIOD,
    
    LN_KEY_F1,        ///< [�~Android]
	LN_KEY_F2,        ///< [�~Android]
	LN_KEY_F3,        ///< [�~Android]
	LN_KEY_F4,        ///< [�~Android]
	LN_KEY_F5,        ///< [�~Android]
	LN_KEY_F6,        ///< [�~Android]
	LN_KEY_F7,        ///< [�~Android]
	LN_KEY_F8,        ///< [�~Android]
	LN_KEY_F9,        ///< [�~Android]
	LN_KEY_F10,        ///< [�~Android]
	LN_KEY_F11,        ///< [�~Android]
	LN_KEY_F12,        ///< [�~Android]

	LN_KEY_NUM_LOCK,        ///< [�~Android]
	LN_KEY_CAPS_LOCK,       ///< [�~Android]

	LN_KEY_SHIFT,
	LN_KEY_CONTROL,
	LN_KEY_ALT,

    
#if 0
    LN_KEY_A                    = AKEYCODE_A,
    LN_KEY_B                    = AKEYCODE_B,
    LN_KEY_C                    = AKEYCODE_C,
    LN_KEY_D                    = AKEYCODE_D,
    LN_KEY_E                    = AKEYCODE_E,
    LN_KEY_F                    = AKEYCODE_F,
    LN_KEY_G                    = AKEYCODE_G,
    LN_KEY_H                    = AKEYCODE_H,
    LN_KEY_I                    = AKEYCODE_I,
    LN_KEY_J                    = AKEYCODE_J,
    LN_KEY_K                    = AKEYCODE_K,
    LN_KEY_L                    = AKEYCODE_L,
    LN_KEY_M                    = AKEYCODE_M,
    LN_KEY_N                    = AKEYCODE_N,
    LN_KEY_O                    = AKEYCODE_O,
    LN_KEY_P                    = AKEYCODE_P,
    LN_KEY_Q                    = AKEYCODE_Q,
    LN_KEY_R                    = AKEYCODE_R,
    LN_KEY_S                    = AKEYCODE_S,
    LN_KEY_T                    = AKEYCODE_T,
    LN_KEY_U                    = AKEYCODE_U,
    LN_KEY_V                    = AKEYCODE_V,
    LN_KEY_W                    = AKEYCODE_W,
    LN_KEY_X                    = AKEYCODE_X,
    LN_KEY_Y                    = AKEYCODE_Y,
    LN_KEY_Z                    = AKEYCODE_Z,
    
    LN_KEY_0                    = AKEYCODE_0,
    LN_KEY_1                    = AKEYCODE_1,
    LN_KEY_2                    = AKEYCODE_2,
    LN_KEY_3                    = AKEYCODE_3,
    LN_KEY_4                    = AKEYCODE_4,
    LN_KEY_5                    = AKEYCODE_5,
    LN_KEY_6                    = AKEYCODE_6,
    LN_KEY_7                    = AKEYCODE_7,
    LN_KEY_8                    = AKEYCODE_8,
    LN_KEY_9                    = AKEYCODE_9,
    /*
                                AKEYCODE_SOFT_LEFT       = 1,
                                AKEYCODE_SOFT_RIGHT      = 2,
                                AKEYCODE_HOME            = 3,
                                AKEYCODE_BACK            = 4,
                                AKEYCODE_CALL            = 5,
                                AKEYCODE_ENDCALL         = 6,
                                
                                AKEYCODE_STAR            = 17,
                                AKEYCODE_POUND           = 18,
                                AKEYCODE_DPAD_CENTER     = 23,
                                AKEYCODE_VOLUME_UP       = 24,
                                AKEYCODE_VOLUME_DOWN     = 25,
                                AKEYCODE_POWER           = 26,
                                AKEYCODE_CAMERA          = 27,
                                AKEYCODE_CLEAR           = 28,
    */

	LN_KEY_SPACE				= AKEYCODE_SPACE,		// 0x20 = ' ' (�X�y�[�X)
	LN_KEY_ESCAPE				= 0,
	LN_KEY_F1					= 0,
	LN_KEY_F2					= 0,
	LN_KEY_F3					= 0,
	LN_KEY_F4					= 0,
	LN_KEY_F5					= 0,
	LN_KEY_F6					= 0,
	LN_KEY_F7					= 0,
	LN_KEY_F8					= 0,
	LN_KEY_F9					= 0,
	LN_KEY_F10					= 0,
	LN_KEY_F11					= 0,
	LN_KEY_F12					= 0,
	LN_KEY_UP					= AKEYCODE_DPAD_UP,
	LN_KEY_DOWN					= AKEYCODE_DPAD_DOWN,
	LN_KEY_LEFT					= AKEYCODE_DPAD_LEFT,
	LN_KEY_RIGHT				= AKEYCODE_DPAD_RIGHT,
	LN_KEY_LSHIFT				= AKEYCODE_SHIFT_LEFT,
	LN_KEY_RSHIFT				= AKEYCODE_SHIFT_RIGHT,
	LN_KEY_LCTRL				= 0,
	LN_KEY_RCTRL				= 0,
	LN_KEY_LALT					= AKEYCODE_ALT_LEFT,
	LN_KEY_RALT					= AKEYCODE_ALT_RIGHT,
	LN_KEY_TAB					= AKEYCODE_TAB,
	LN_KEY_ENTER				= AKEYCODE_ENTER,
	LN_KEY_BACKSPACE			= AKEYCODE_DEL,
	LN_KEY_INSERT				= 0,
	LN_KEY_DELETE				= 0,
	LN_KEY_PAGEUP				= AKEYCODE_PAGE_UP,
	LN_KEY_PAGEDOWN				= AKEYCODE_PAGE_DOWN,
	LN_KEY_HOME					= 0,            
	LN_KEY_END					= 0,
	LN_KEY_NUMPAD0				= AKEYCODE_0,
	LN_KEY_NUMPAD1				= AKEYCODE_1,
	LN_KEY_NUMPAD2				= AKEYCODE_2,
	LN_KEY_NUMPAD3				= AKEYCODE_2,
	LN_KEY_NUMPAD4				= AKEYCODE_4,
	LN_KEY_NUMPAD5				= AKEYCODE_5,
	LN_KEY_NUMPAD6				= AKEYCODE_6,
	LN_KEY_NUMPAD7				= AKEYCODE_7,
	LN_KEY_NUMPAD8				= AKEYCODE_8,
	LN_KEY_NUMPAD9				= AKEYCODE_9,

	LN_KEY_ADD					= AKEYCODE_PLUS,		    ///< +
	LN_KEY_SUBTRACT				= AKEYCODE_MINUS,	    ///< -
	LN_KEY_MULTIPLY				= VK_MULTIPLY,	    ///< *
	LN_KEY_DIVIDE				= AKEYCODE_SLASH,		///< /
	LN_KEY_DECIMAL				= VK_DECIMAL,		///< .

    LN_KEY_COLON                = 0xBA,             ///< :
    LN_KEY_SEMICOLON            = AKEYCODE_SEMICOLON,             ///< ;
    LN_KEY_COMMA                = AKEYCODE_COMMA,             ///< ,
    LN_KEY_PERIOD               = AKEYCODE_PERIOD,             ///< .

	LN_KEY_NUM_LOCK				= VK_NUMLOCK,
	LN_KEY_CAPS_LOCK			= VK_OEM_ATTN,
	//LN_KEY_SCROLL_LOCK	    = VK_SCROLL,
	//LN_KEY_PAUSE				= VK_PAUSE,

	LN_KEY_SHIFT				= VK_SHIFT,
	LN_KEY_CONTROL				= VK_CONTROL,
	LN_KEY_ALT					= VK_MENU,
    
    
    AKEYCODE_SYM             = 63,
    AKEYCODE_EXPLORER        = 64,
    AKEYCODE_ENVELOPE        = 65,

    AKEYCODE_GRAVE           = 68,
    
    AKEYCODE_EQUALS          = 70,
    AKEYCODE_LEFT_BRACKET    = 71,
    AKEYCODE_RIGHT_BRACKET   = 72,
    AKEYCODE_BACKSLASH       = 73,

    AKEYCODE_APOSTROPHE      = 75,

    AKEYCODE_AT              = 77,
    AKEYCODE_NUM             = 78,
    AKEYCODE_HEADSETHOOK     = 79,
    AKEYCODE_FOCUS           = 80,   // *Camera* focus

    AKEYCODE_MENU            = 82,
    AKEYCODE_NOTIFICATION    = 83,
    AKEYCODE_SEARCH          = 84,
    AKEYCODE_MEDIA_PLAY_PAUSE= 85,
    AKEYCODE_MEDIA_STOP      = 86,
    AKEYCODE_MEDIA_NEXT      = 87,
    AKEYCODE_MEDIA_PREVIOUS  = 88,
    AKEYCODE_MEDIA_REWIND    = 89,
    AKEYCODE_MEDIA_FAST_FORWARD = 90,
    AKEYCODE_MUTE            = 91,

    AKEYCODE_PICTSYMBOLS     = 94,
    AKEYCODE_SWITCH_CHARSET  = 95,
    AKEYCODE_BUTTON_A        = 96,
    AKEYCODE_BUTTON_B        = 97,
    AKEYCODE_BUTTON_C        = 98,
    AKEYCODE_BUTTON_X        = 99,
    AKEYCODE_BUTTON_Y        = 100,
    AKEYCODE_BUTTON_Z        = 101,
    AKEYCODE_BUTTON_L1       = 102,
    AKEYCODE_BUTTON_R1       = 103,
    AKEYCODE_BUTTON_L2       = 104,
    AKEYCODE_BUTTON_R2       = 105,
    AKEYCODE_BUTTON_THUMBL   = 106,
    AKEYCODE_BUTTON_THUMBR   = 107,
    AKEYCODE_BUTTON_START    = 108,
    AKEYCODE_BUTTON_SELECT   = 109,
    AKEYCODE_BUTTON_MODE     = 110,
#endif
#endif

	LN_KEY_LAST,

    LN_MAX_KEYS = 256,

    // �o�b�N�X���b�V���A= ������
};
#endif

//=============================================================================
//								end of file
//=============================================================================