//=============================================================================
//�y GUIEvent �z
//-----------------------------------------------------------------------------
///**
//  @file       GUIEvent.h
//  @brief      GUIEvent
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../System/Event.h"

namespace LNote
{

enum LNGUIEventType
{
    LN_GUIEV_MOUSE_DOWN     = LNEV_MOUSE_DOWN,  ///< [Mouse] �}�E�X�{�^���������ꂽ
    LN_GUIEV_MOUSE_UP       = LNEV_MOUSE_UP,    ///< [Mouse] �}�E�X�{�^���������ꂽ
    LN_GUIEV_MOUSE_MOVE     = LNEV_MOUSE_MOVE,  ///< [Mouse] �}�E�X���ړ�����
    LN_GUIEV_MOUSE_WHEEL    = LNEV_MOUSE_WHEEL, ///< [Mouse] �}�E�X�z�C�[�������삳�ꂽ


    LN_GUIEV_MOUSE_LEAVE = 1001,    ///< [Mouse]
    LN_GUIEV_MOUSE_ENTER,           ///< [Mouse]

    LN_GUIEV_GOT_FOCUS,             ///< [none]
    LN_GUIEV_LOST_FOCUS,            ///< [none]
};

namespace Core
{
namespace GUI
{

class GUIElement;
class GUIControl;
class GUIDesktop;

struct Event
{
    LNGUIEventType      Type;
    GUIElement*         Sender;

    union
    {
        struct
        {
            LNSystemMouseButton	Button;		///< �{�^���ԍ�
            lnS16			    Delta;		///< �}�E�X�z�C�[���̉�]�񐔁E����
            lnS16			    X;			///< �}�E�X�C�x���g�������̃}�E�X�� X ���W
            lnS16			    Y;			///< �}�E�X�C�x���g�������̃}�E�X�� Y ���W
            lnS16               MoveX;	    ///< X ���W�̈ړ��� (GUI �Ŏg���Ƃ��̂ݗL��)
            lnS16               MoveY;      ///< Y ���W�̈ړ���
        } Mouse;

        struct
        {
            lnU32   KeyCode;			    ///< �L�[�R�[�h (LNVirtualKeyCode �̒l�܂��͕����R�[�h(LNEV_KEY_CHAR ���B'a'-'z' 'A-Z' '0'-'9' LN_KEY_BACKSPACE LN_KEY_SPACE))
            bool	IsAlt;				    ///< Alt �L�[��������Ă���ꍇ true
            bool	IsShift;			    ///< Shift �L�[��������Ă���ꍇ true
            bool	IsControl;			    ///< Ctrl �L�[��������Ă���ꍇ true
        } Key;

        struct
        {
            lnS16     TouchID;    ///< �^�b�`�ԍ� (�^�b�`�������̎w�̔ԍ�)
            lnS16     X;          ///< �}�E�X�C�x���g�������̃}�E�X�� X ���W
            lnS16     Y;          ///< �}�E�X�C�x���g�������̃}�E�X�� Y ���W
            lnS16     MoveX;      ///< X ���W�̈ړ���
            lnS16     MoveY;      ///< Y ���W�̈ړ���
        } ;
    };

    Event() {}

    Event( LNGUIEventType type_ )
        : Type( type_ )
    {}
};


} // namespace GUI
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================