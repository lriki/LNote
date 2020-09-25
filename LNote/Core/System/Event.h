//=============================================================================
//�y Event �z
//-----------------------------------------------------------------------------
///**
//  @file       Event.h
//  @brief      Event
//  @author     Riki
//*/
//-----------------------------------------------------------------------------
/*
    �ȑO�� struct - union �̃X�^�C�����ƁASystem �ȊO�̃��W���[������C�x���g��
    �������������ꍇ�ɁALNEvent �\���̂̊g��������ɂȂ�B
    ���̂��߁A�C�x���g�̎�ނň����𔻕ʂ��ăL���X�g��������ɂ���B

    �n���h���֐��̈����� (int type, void* args) �� (struct Event* event) �����������ǁA
    C# .NET �� WndProc ����̌�҂Ȃ̂ŁA������ɍ��킹�Ă݂�B

    �C�x���g�\���̂� new �œ��I�m�ۂ���邪�A��{�I�Ƀ��[�U�[�͉�������ɂ���
    �C�ɂ���K�v�͂Ȃ��B (Manager::processMessage() �ŁA������ɉ�������)
    ���̂��߁A�n���h���֐��̈����́udelete �̕K�v�͂Ȃ��ł��v�Ƃ������Ƃ�
    �|�C���^�ł͂Ȃ��Q�Ƃɂ���B


    ���ӓ_�c
            Android �Ń\�t�g�E�F�A�L�[�{�[�h���g�����ꍇ�A
            LNEV_KEY_DOWN �� LNEV_KEY_UP �̓L�[�𗣂����u�Ԃɓ����^�C�~���O�Ŕ������Ă��܂��B
*/
//=============================================================================

#pragma once

#define LNEV_NULL               0   ///< ����`�̃C�x���g (�ʏ�͑��M����Ȃ�)

#define LNEV_QUIT               1
#define LNEV_CLOSE              2   ///< �E�B���h�E�̃N���[�Y�{�^���������ꂽ
#define LNEV_APP_ACTIVATE       3   ///< �E�B���h�E���A�N�e�B�u�ɂȂ���
#define LNEV_APP_DEACTIVATE     4   ///< �E�B���h�E����A�N�e�B�u�ɂȂ���
#define LNEV_ALTENTER           5   ///< Alt + Enter

#define LNEV_MOUSE_DOWN         6   ///< [ LNEvent::Mouse ] �}�E�X�{�^���������ꂽ
#define LNEV_MOUSE_UP           7   ///< [ LNEvent::Mouse ] �}�E�X�{�^���������ꂽ
#define LNEV_MOUSE_MOVE         8   ///< [ LNEvent::Mouse ] �}�E�X���ړ�����
#define LNEV_MOUSE_WHEEL        9   ///< [ LNEvent::Mouse ] �}�E�X�z�C�[�������삳�ꂽ
 
#define LNEV_KEY_DOWN           10  ///< [ LNEvent::Key ] �L�[������
#define LNEV_KEY_UP             11  ///< [ LNEvent::Key ] �L�[�����グ
#define LNEV_KEY_CHAR           12  ///< [ LNEvent::Key ] �������� (���{����� OFF �̏�ԂŃL�[��������ꍇ)
    
#define LNEV_TOUCH_DOWN         13  ///< [ LNEvent::Touch ] ��ʂ��^�b�`���ꂽ
#define LNEV_TOUCH_UP           14  ///< [ LNEvent::Touch ] ��ʂ��痣���ꂽ
#define LNEV_TOUCH_MOVE         15  ///< [ LNEvent::Touch ] �^�b�`���Ɉړ�����

#define LNEV_ANDROID_PAUSED     101 ///< Android APP_CMD_RESUME
#define LNEV_ANDROID_RESUMED    102 ///< Android APP_CMD_PAUSE

// Graphics
#define LNEV_DEVICE_LOST		201	///< �f�o�C�X���Z�b�g�O����
#define LNEV_DEVICE_RESET		202	///< �f�o�C�X���Z�b�g�㏈��

#define LNEV_MOUSE_BEGIN    LNEV_MOUSE_DOWN
#define LNEV_MOUSE_END      LNEV_MOUSE_WHEEL

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

/// �������Ƃ̂ł����ʃ^�b�`���̍ő吔 (�F���ł���w�̐�)
static const int LN_MAX_TOUCH_DATA = 2;

/// �}�E�X�{�^��
enum LNSystemMouseButton
{
	LN_MOUSE_LEFT = 0,	        ///< ���{�^��   ( 1 �� )
	LN_MOUSE_RIGHT,		        ///< �E�{�^��   ( 2 �� )
	LN_MOUSE_MIDDLE,	        ///< �����{�^�� ( 3 �� )

    LN_MOUSE_NONE = 0xff        ///< �����܂��͉�����Ă��Ȃ����Ƃ������t���O
};

/// �L�[�{�[�h�̉��z�L�[�R�[�h (NumPad �̓T�|�[�g���Ă��Ȃ�)
enum LNVirtualKeyCode
{
	LN_KEY_UNKNOWN = 0,

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

	LN_KEY_F1,
	LN_KEY_F2,
	LN_KEY_F3,
	LN_KEY_F4,
	LN_KEY_F5,
	LN_KEY_F6,
	LN_KEY_F7,
	LN_KEY_F8,
	LN_KEY_F9,
	LN_KEY_F10,
	LN_KEY_F11,
	LN_KEY_F12,

	LN_KEY_SPACE,	// 0x20 = ' ' (�X�y�[�X)
	LN_KEY_ESCAPE,
	LN_KEY_UP,
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
	LN_KEY_ENTER,
	LN_KEY_BACKSPACE,
	LN_KEY_INSERT,
	LN_KEY_DELETE,
	LN_KEY_PAGEUP,
	LN_KEY_PAGEDOWN,
	LN_KEY_HOME,
	LN_KEY_END,

    LN_KEY_COLON,	    ///< :
    LN_KEY_SEMICOLON,   ///< ;
    LN_KEY_COMMA,		///< ,
    LN_KEY_PERIOD,	    ///< .
	LN_KEY_SLASH,	    ///< /
	LN_KEY_MINUS,		///< -
	LN_KEY_BACKSLASH,   ///< \(�o�b�N�X���b�V��)
	LN_KEY_YEN,			///< \(BaskSpace�L�[�̍���\�L�[)
	LN_KEY_CARET,	    ///< ^
	LN_KEY_LBRACKET,    ///< [
	LN_KEY_RBRACKET,    ///< ]

    LN_KEY_ANDROID_PLUS,	    ///< Android '+'

    LN_MAX_KEYS = 256,
};

/// �}�E�X�J�[�\���̋���
enum LNMouseCursorMode
{
	LN_MCMODE_VISIBLE = 0,		    ///< ��ɕ\������
	LN_MCMODE_INVISIBLE,		    ///< �N�A�C�A���g�̈���ł͏�ɕ\�����Ȃ� (GUI �g���ꍇ�̓R��)
	LN_MCMODE_MOVER_VISIBLE,	    ///< ��莞�ԓ����Ȃ��ꍇ�͏���
};

namespace Core
{
namespace System
{

#define LN_EVENT_STRUCT_HEADER  int Type;

struct Event
{
    LN_EVENT_STRUCT_HEADER;
};

/// �}�E�X�C�x���g�̈���
struct MouseEvent
{
    LN_EVENT_STRUCT_HEADER;

    LNSystemMouseButton	Button;		///< �{�^���ԍ�
    int				    Delta;		///< �}�E�X�z�C�[���̉�]�� (windows �ł� 1 ��ɂ� 120 ���i�[����邪�A����͐��܂��͕��̉�)
    int				    X;			///< �}�E�X�C�x���g�������̃}�E�X�� X ���W
    int				    Y;			///< �}�E�X�C�x���g�������̃}�E�X�� Y ���W
    int                 MoveX;	    ///< X ���W�̈ړ��� (GUI �Ŏg���Ƃ��̂ݗL��)
    int                 MoveY;      ///< Y ���W�̈ړ���
};

/// �L�[���̓C�x���g�̈���
struct KeyEvent
{
    LN_EVENT_STRUCT_HEADER;

    lnU32   KeyCode;			    ///< �L�[�R�[�h (LNVirtualKeyCode �̒l�܂��͕����R�[�h(LNEV_KEY_CHAR ���B'a'-'z' 'A-Z' '0'-'9' LN_KEY_BACKSPACE LN_KEY_SPACE))
    bool	IsAlt;				    ///< Alt �L�[��������Ă���ꍇ true
    bool	IsShift;			    ///< Shift �L�[��������Ă���ꍇ true
    bool	IsControl;			    ///< Ctrl �L�[��������Ă���ꍇ true
};

/// �^�b�`�C�x���g�̈���
struct TouchEvent
{
    LN_EVENT_STRUCT_HEADER;

    int     TouchID;    ///< �^�b�`�ԍ� (�^�b�`�������̎w�̔ԍ�)
    int     X;          ///< �}�E�X�C�x���g�������̃}�E�X�� X ���W
    int     Y;          ///< �}�E�X�C�x���g�������̃}�E�X�� Y ���W
    int     MoveX;      ///< X ���W�̈ړ���
    int     MoveY;      ///< Y ���W�̈ړ���
};

/// IME �C�x���g�̈���
struct IMEEvent
{
    LN_EVENT_STRUCT_HEADER;

    bool IsCOMPATTR;			    ///< �ҏW������̃A�g���r���[�g���擾���ꂽ�A�܂��͍ŐV�����ꂽ 
    bool IsCOMPCLAUSE;			    ///< �ҏW������̕��ߏ�񂪎擾���ꂽ�A�܂��͍ŐV�����ꂽ 
    bool IsCOMPREADATTR;		    ///< ���݂̕ҏW�̃��[�f�B���O�����񂪎擾���ꂽ�A�܂��͍ŐV�����ꂽ 
    bool IsCOMPREADCLAUSE;		    ///< �ҏW������̃��[�f�B���O������̕��ߏ�񂪎擾���ꂽ�A�܂��͍ŐV�����ꂽ 
    bool IsCOMPREADSTR;			    ///< ���݂̕ҏW�̃��[�f�B���O�����񂪎擾���ꂽ�A�܂��͍ŐV�����ꂽ 
    bool IsCOMPSTR;				    ///< ���݂̕ҏW�����񂪎擾���ꂽ�A�܂��͍ŐV�����ꂽ 
    bool IsCURSORPOS;			    ///< �ҏW������̃J�[�\���ʒu���擾�����A�܂��͍ŐV�����ꂽ 
    bool IsDELTASTART;			    ///< �ҏW������̕ω��̊J�n�ʒu���擾���ꂽ�A�܂��͍ŐV�����ꂽ 
    bool IsRESULTCLAUSE;		    ///< �m�蕶����̕��ߏ�񂪎擾���ꂽ�A�܂��͍ŐV�����ꂽ 
    bool IsRESULTREADCLAUSE;	    ///< ���[�f�B���O������̕��ߏ�񂪎擾���ꂽ�A�܂��͍ŐV�����ꂽ 
    bool IsRESULTREADSTR;		    ///< ���[�f�B���O��������擾�����A�܂��͍ŐV�����ꂽ 
    bool IsRESULTSTR;			    ///< �m�蕶���񂪎擾���ꂽ�A�܂��͍ŐV�����ꂽ
};
    
//#define LNEV_DEVICE_LOST,           ///< �f�o�C�X���X�g�����̗v�� (Graphics �� post �����B�`��X���b�h����ʒm����邽�߁Asend �ł͂Ȃ� post)
    //LNEV_DEVICE_RESET,          ///< �f�o�C�X���Z�b�g�����̗v��

// Event �쐬���[�e�B���e�B
template< typename T_ >
T_* createEvent( int type_ )
{
    T_* e = LN_NEW T_;
    e->Type = type_;
    return e;
}

#if 0
/// �C�x���g���b�Z�[�W (�R�����g�� [] ���̂��̂̓��b�Z�[�W�ƃZ�b�g�ɂȂ�����̌^)
enum LNEventType
{
	LNEV_NULL = 0,             ///< ����`�̃C�x���g

    LNEV_QUIT,

	LNEV_APP_ACTIVATE,		    ///< �E�B���h�E���A�N�e�B�u�ɂȂ���
	LNEV_APP_DEACTIVATE,	    ///< �E�B���h�E����A�N�e�B�u�ɂȂ���
	LNEV_ALTENTER,             ///< Alt + Enter

	LNEV_MOUSE_DOWN,		    ///< [ LNEvent::Mouse ] �}�E�X�{�^���������ꂽ
	LNEV_MOUSE_UP,	            ///< [ LNEvent::Mouse ] �}�E�X�{�^���������ꂽ
	LNEV_MOUSE_MOVE,		    ///< [ LNEvent::Mouse ] �}�E�X���ړ�����
	LNEV_MOUSE_WHEEL,		    ///< [ LNEvent::Mouse ] �}�E�X�z�C�[�������삳�ꂽ
 
	LNEV_KEY_DOWN,			    ///< [ LNEvent::Key ] �L�[������
	LNEV_KEY_UP,			    ///< [ LNEvent::Key ] �L�[�����グ
	LNEV_KEY_CHAR,			    ///< [ LNEvent::Key ] �������� (���{����� OFF �̏�ԂŃL�[��������ꍇ)
    
    LNEV_TOUCH_DOWN,            ///< [ LNEvent::Touch ] ��ʂ��^�b�`���ꂽ
    LNEV_TOUCH_UP,              ///< [ LNEvent::Touch ] ��ʂ��痣���ꂽ
    LNEV_TOUCH_MOVE,            ///< [ LNEvent::Touch ] �^�b�`���Ɉړ�����


    LNEV_CLOSE,                 ///< �E�B���h�E�̃N���[�Y�{�^���������ꂽ
    

    LNEV_DEVICE_LOST,           ///< �f�o�C�X���X�g�����̗v�� (Graphics �� post �����B�`��X���b�h����ʒm����邽�߁Asend �ł͂Ȃ� post)
    //LNEV_DEVICE_RESET,          ///< �f�o�C�X���Z�b�g�����̗v��



	//LM_IME_COMPOSITION_START,	///< IME �ҏW�J�n
	//LM_IME_COMPOSITION_END,		///< IME �ҏW�I��
	//LM_IME_COMPOSITION,			///< �L�[�̓��͂̌��ʁA�ϊ������񂪕ω����� ( IME �L�����̂� )

	//LM_IME_CANDIDATE_OPEN,		///< ���E�B���h�E���J�����Ƃ��Ă���
	//LM_IME_CANDIDATE_CLOSE,		///< ���E�B���h�E����悤�Ƃ��Ă���
	//LM_IME_CANDIDATE_UPDATE,	///< IME�����E�B���h�E�̓��e��ς��悤�Ƃ����Ƃ�

    //LNEV_DEVICECHANGE,         ///< (USB) �f�o�C�X���ڑ��A�܂��͎��O���ꂽ

    LNEV_ANDROID_INIT_WINDOW,   ///< Android �p�B�E�B���h�E�����������ꂽ

    
    // �ȉ��A�͈͂�\���萔
    LNEV_APP_BEGIN     = LNEV_APP_ACTIVATE,
    LNEV_APP_END       = LNEV_APP_DEACTIVATE,
    LNEV_MOUSE_BEGIN   = LNEV_MOUSE_DOWN,
    LNEV_MOUSE_END     = LNEV_MOUSE_WHEEL,
    LNEV_KEY_BEGIN     = LNEV_KEY_DOWN,
	LNEV_KEY_END       = LNEV_KEY_CHAR,
    LNEV_FORCUS_BEGIN  = LNEV_MOUSE_WHEEL,
    LNEV_FORCUS_END    = LNEV_KEY_CHAR,

	LNEV_END

    /*
        ���ӓ_�c
            Android �Ń\�t�g�E�F�A�L�[�{�[�h���g�����ꍇ�A
            LNEV_KEY_DOWN �� LNEV_KEY_UP �̓L�[�𗣂����u�Ԃɓ����^�C�~���O�Ŕ������Ă��܂��B
    */
};

/// �C�x���g���b�Z�[�W�̈���
struct LNEvent
{
    LNEventType     Type;

    union
    {
        /// �}�E�X�C�x���g�̈���
        struct
        {
	        LNSystemMouseButton	Button;		///< �{�^���ԍ�
	        int				    Delta;		///< �}�E�X�z�C�[���̉�]�� (windows �ł� 1 ��ɂ� 120 ���i�[����邪�A����͐��܂��͕��̉�)
	        int				    X;			///< �}�E�X�C�x���g�������̃}�E�X�� X ���W
	        int				    Y;			///< �}�E�X�C�x���g�������̃}�E�X�� Y ���W
            int                 MoveX;	    ///< X ���W�̈ړ��� (GUI �Ŏg���Ƃ��̂ݗL��)
	        int                 MoveY;      ///< Y ���W�̈ړ���

        } Mouse;

        /// �L�[���̓C�x���g�̈���
        struct
        {
	        lnU32   KeyCode;			    ///< �L�[�R�[�h (LNVirtualKeyCode �̒l�܂��͕����R�[�h(LNEV_KEY_CHAR ���B'a'-'z' 'A-Z' '0'-'9' LN_KEY_BACKSPACE LN_KEY_SPACE))
	        bool	IsAlt;				    ///< Alt �L�[��������Ă���ꍇ true
	        bool	IsShift;			    ///< Shift �L�[��������Ă���ꍇ true
	        bool	IsControl;			    ///< Ctrl �L�[��������Ă���ꍇ true

        } Key;
        
        /// �^�b�`�C�x���g�̈���
        struct
        {
            int     TouchID;    ///< �^�b�`�ԍ� (�^�b�`�������̎w�̔ԍ�)
	        int     X;          ///< �}�E�X�C�x���g�������̃}�E�X�� X ���W
	        int     Y;          ///< �}�E�X�C�x���g�������̃}�E�X�� Y ���W
            int     MoveX;      ///< X ���W�̈ړ���
	        int     MoveY;      ///< Y ���W�̈ړ���

        } Touch;

        /// IME �C�x���g�̈���
        struct
        {
	        bool IsCOMPATTR;			    ///< �ҏW������̃A�g���r���[�g���擾���ꂽ�A�܂��͍ŐV�����ꂽ 
	        bool IsCOMPCLAUSE;			    ///< �ҏW������̕��ߏ�񂪎擾���ꂽ�A�܂��͍ŐV�����ꂽ 
	        bool IsCOMPREADATTR;		    ///< ���݂̕ҏW�̃��[�f�B���O�����񂪎擾���ꂽ�A�܂��͍ŐV�����ꂽ 
	        bool IsCOMPREADCLAUSE;		    ///< �ҏW������̃��[�f�B���O������̕��ߏ�񂪎擾���ꂽ�A�܂��͍ŐV�����ꂽ 
	        bool IsCOMPREADSTR;			    ///< ���݂̕ҏW�̃��[�f�B���O�����񂪎擾���ꂽ�A�܂��͍ŐV�����ꂽ 
	        bool IsCOMPSTR;				    ///< ���݂̕ҏW�����񂪎擾���ꂽ�A�܂��͍ŐV�����ꂽ 
	        bool IsCURSORPOS;			    ///< �ҏW������̃J�[�\���ʒu���擾�����A�܂��͍ŐV�����ꂽ 
	        bool IsDELTASTART;			    ///< �ҏW������̕ω��̊J�n�ʒu���擾���ꂽ�A�܂��͍ŐV�����ꂽ 
	        bool IsRESULTCLAUSE;		    ///< �m�蕶����̕��ߏ�񂪎擾���ꂽ�A�܂��͍ŐV�����ꂽ 
	        bool IsRESULTREADCLAUSE;	    ///< ���[�f�B���O������̕��ߏ�񂪎擾���ꂽ�A�܂��͍ŐV�����ꂽ 
	        bool IsRESULTREADSTR;		    ///< ���[�f�B���O��������擾�����A�܂��͍ŐV�����ꂽ 
	        bool IsRESULTSTR;			    ///< �m�蕶���񂪎擾���ꂽ�A�܂��͍ŐV�����ꂽ

        } IME;
    };
};
#endif

} // namespace System
} // namespace Core

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================