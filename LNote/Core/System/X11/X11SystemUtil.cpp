//=============================================================================
//【 X11SystemUtil 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"

#if defined(LNOTE_X11)
#include "X11SystemUtil.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{
namespace X11
{

//---------------------------------------------------------------------
// ● メッセージをライブラリ用のイベントメッセージに変換する
//---------------------------------------------------------------------
void translateMessageToLN( LNEvent* lnev_, const XEvent& xev_, bool* handled_ )
{
    *handled_ = false;

    switch( xev_.type )
	{
        /////////////////////////////////////////////// 不明なメッセージ
        default:
        {
            lnev_->Type = LNEV_NULL;
            break;
        }
    }
}

//---------------------------------------------------------------------
// ● ライブラリで必要なメッセージかをチェックする
//---------------------------------------------------------------------
bool checkNecessaryMessage( const XEvent& xev_ )
{
    /*
    switch ( xev_.Type )
    {
        case WM_CLOSE:

        case WM_ACTIVATE:
        
        case WM_SYSKEYDOWN:
        case WM_SYSCHAR:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MOUSEMOVE:
        //case WM_NCMOUSEMOVE:
        case WM_MOUSEWHEEL:

        case WM_KEYDOWN:
		case WM_KEYUP:
        case WM_CHAR:
        {
            return true;
        }
    }
    */
    return false;
}

//---------------------------------------------------------------------
// ● VK_～ => LN_KEY_～ へのキーコード変換
//---------------------------------------------------------------------
u32 convVirtualKeyCode( u32 win_vk_ )
{
    return win_vk_;
    /*
	switch ( win_vk_ )
	{
		case VK_SPACE		: return LN_KEY_SPACE;
		case VK_ESCAPE		: return LN_KEY_ESCAPE;
		case VK_F1			: return LN_KEY_F1;
		case VK_F2			: return LN_KEY_F2;
		case VK_F3			: return LN_KEY_F3;
		case VK_F4			: return LN_KEY_F4;
		case VK_F5			: return LN_KEY_F5;
		case VK_F6			: return LN_KEY_F6;
		case VK_F7			: return LN_KEY_F7;
		case VK_F8			: return LN_KEY_F8;
		case VK_F9			: return LN_KEY_F9;
		case VK_F10			: return LN_KEY_F10;
		case VK_F11			: return LN_KEY_F11;
		case VK_F12			: return LN_KEY_F12;
		case VK_UP			: return LN_KEY_UP;
		case VK_DOWN		: return LN_KEY_DOWN;
		case VK_LEFT		: return LN_KEY_LEFT;
		case VK_RIGHT		: return LN_KEY_RIGHT;
		case VK_LSHIFT		: return LN_KEY_LSHIFT;
		case VK_RSHIFT		: return LN_KEY_RSHIFT;
		case VK_LCONTROL	: return LN_KEY_LCTRL;
		case VK_RCONTROL	: return LN_KEY_RCTRL;
		case VK_LMENU		: return LN_KEY_LALT;
		case VK_RMENU		: return LN_KEY_RALT;
		case VK_TAB			: return LN_KEY_TAB;
		case VK_RETURN		: return LN_KEY_ENTER;
		case VK_BACK		: return LN_KEY_BACKSPACE;
		case VK_INSERT		: return LN_KEY_INSERT;
		case VK_DELETE		: return LN_KEY_DELETE;
		case VK_PRIOR		: return LN_KEY_PAGEUP;
		case VK_NEXT		: return LN_KEY_PAGEDOWN;
		case VK_HOME		: return LN_KEY_HOME;
		case VK_END			: return LN_KEY_END;
		case VK_NUMPAD0		: return LN_KEY_NUMPAD0;
		case VK_NUMPAD1		: return LN_KEY_NUMPAD1;
		case VK_NUMPAD2		: return LN_KEY_NUMPAD2;
		case VK_NUMPAD3		: return LN_KEY_NUMPAD3;
		case VK_NUMPAD4		: return LN_KEY_NUMPAD4;
		case VK_NUMPAD5		: return LN_KEY_NUMPAD5;
		case VK_NUMPAD6		: return LN_KEY_NUMPAD6;
		case VK_NUMPAD7		: return LN_KEY_NUMPAD7;
		case VK_NUMPAD8		: return LN_KEY_NUMPAD8;
		case VK_NUMPAD9		: return LN_KEY_NUMPAD9;
		case VK_ADD			: return LN_KEY_ADD;
		case VK_SUBTRACT	: return LN_KEY_SUBTRACT;
		case VK_MULTIPLY	: return LN_KEY_MULTIPLY;
		case VK_DIVIDE		: return LN_KEY_DIVIDE;
		case VK_DECIMAL		: return LN_KEY_DECIMAL;
		case VK_NUMLOCK		: return LN_KEY_NUM_LOCK;
		case VK_OEM_ATTN	: return LN_KEY_CAPS_LOCK;
		case VK_SCROLL		: return LN_KEY_SCROLL_LOCK;
		case VK_PAUSE		: return LN_KEY_PAUSE;
		case VK_SHIFT		: return LN_KEY_SHIFT;
		case VK_CONTROL		: return LN_KEY_CONTROL;
		case VK_MENU		: return LN_KEY_ALT;
		default				: return win_vk_;	// 上記以外は文字コードとする
	}
    */
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace X11
} // namespace System
} // namespace Core
} // namespace LNote

#endif // LNOTE_X11

//=============================================================================
//                              end of file
//=============================================================================