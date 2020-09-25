//=============================================================================
//【 GUIEvent 】
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
    LN_GUIEV_MOUSE_DOWN     = LNEV_MOUSE_DOWN,  ///< [Mouse] マウスボタンが押された
    LN_GUIEV_MOUSE_UP       = LNEV_MOUSE_UP,    ///< [Mouse] マウスボタンが離された
    LN_GUIEV_MOUSE_MOVE     = LNEV_MOUSE_MOVE,  ///< [Mouse] マウスが移動した
    LN_GUIEV_MOUSE_WHEEL    = LNEV_MOUSE_WHEEL, ///< [Mouse] マウスホイールが操作された


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
            LNSystemMouseButton	Button;		///< ボタン番号
            lnS16			    Delta;		///< マウスホイールの回転回数・方向
            lnS16			    X;			///< マウスイベント生成時のマウスの X 座標
            lnS16			    Y;			///< マウスイベント生成時のマウスの Y 座標
            lnS16               MoveX;	    ///< X 座標の移動量 (GUI で使うときのみ有効)
            lnS16               MoveY;      ///< Y 座標の移動量
        } Mouse;

        struct
        {
            lnU32   KeyCode;			    ///< キーコード (LNVirtualKeyCode の値または文字コード(LNEV_KEY_CHAR 時。'a'-'z' 'A-Z' '0'-'9' LN_KEY_BACKSPACE LN_KEY_SPACE))
            bool	IsAlt;				    ///< Alt キーが押されている場合 true
            bool	IsShift;			    ///< Shift キーが押されている場合 true
            bool	IsControl;			    ///< Ctrl キーが押されている場合 true
        } Key;

        struct
        {
            lnS16     TouchID;    ///< タッチ番号 (タッチした順の指の番号)
            lnS16     X;          ///< マウスイベント生成時のマウスの X 座標
            lnS16     Y;          ///< マウスイベント生成時のマウスの Y 座標
            lnS16     MoveX;      ///< X 座標の移動量
            lnS16     MoveY;      ///< Y 座標の移動量
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