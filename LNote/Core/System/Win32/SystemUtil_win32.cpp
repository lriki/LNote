//=============================================================================
//【 SystemUtil 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "SystemUtil.h"

#if defined(LNOTE_WINDOWS)

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{


namespace Win32
{

//---------------------------------------------------------------------
// ● メッセージをライブラリ用のイベントメッセージに変換する
//---------------------------------------------------------------------
LRESULT translateMessageToLN( Event** lnev_, HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_, bool* handled_ )
{
    *lnev_ = NULL;
    *handled_ = false;

    switch( msg_ )
	{
		/////////////////////////////////////////////// クローズボタンが押された
		case WM_CLOSE:
        {
            /*
			ここは DefWindowProc() を呼ばないで終了する。
			こうしないと、×ボタンが押された瞬間にウィンドウが破棄されて、
			終了直前の最後の描画時に DirectX の EndScene() で
			「描画先のウィンドウがありません」ということでエラーが出る。

            代わりに終了フラグだけを立てて、それをメインループで検出、
            その後の finalize() 呼び出しで DestroyWindow() を呼び出す。
            */
            *lnev_ = createEvent< Event >( LNEV_CLOSE );
            *handled_ = true;
            break;
        }
        /////////////////// ウィンドウがアクティブ・非アクティブになった場合
		case WM_ACTIVATE:
		{
		    if ( wparam_ & 0xffff0000 )
		    {
                *lnev_ = createEvent< Event >( LNEV_APP_DEACTIVATE );
		    }
		    else if ( ( wparam_ & 0xffff ) != 0 )
		    {
                *lnev_ = createEvent< Event >( LNEV_APP_ACTIVATE );
		    }
            else
            {
                *lnev_ = createEvent< Event >( LNEV_APP_DEACTIVATE );
            }
            break;
		}
        /////////////////////////////////////////////// Alt + Enter 確認
		case WM_SYSKEYDOWN:
        {
            if ( wparam_ == VK_RETURN )
            {
                *handled_ = true;
                *lnev_ = createEvent< Event >( LNEV_ALTENTER );
                
                //return 0;
                return MAKELONG( -1, MNC_EXECUTE );     // 第一引数はメニューの属性。今回はメニュー使ってないのでとりあえずこのまま
            }
            // ここは以降に DefWindowProc() を呼ぶ。
			// こうしないと Alt + F4 とかが無効になってそれで終了できなくなる。
			break;
        }
        /////////////////////////////////////////////// Alt + Enter の警告音カット http://www26.atwiki.jp/gunsyu3/pages/11.html
        case WM_SYSCHAR:
        {
            *handled_ = true;
            break;
        }
        /////////////////////////////////////////////// マウスボタン入力
        case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		{
            MouseEvent* e = NULL;

            switch( msg_ )
	        {
                case WM_LBUTTONDOWN:
                    e = createEvent< MouseEvent >( LNEV_MOUSE_DOWN );
                    e->Button = LN_MOUSE_LEFT;
                    break;
                case WM_LBUTTONUP:
                    e = createEvent< MouseEvent >( LNEV_MOUSE_UP );
                    e->Button = LN_MOUSE_LEFT;
                    break;
                case WM_RBUTTONDOWN:
                    e = createEvent< MouseEvent >( LNEV_MOUSE_DOWN );
                    e->Button = LN_MOUSE_RIGHT;
                    break;
                case WM_RBUTTONUP:
                    e = createEvent< MouseEvent >( LNEV_MOUSE_UP );
                    e->Button = LN_MOUSE_RIGHT;
                    break;
                case WM_MBUTTONDOWN:
                    e = createEvent< MouseEvent >( LNEV_MOUSE_DOWN );
                    e->Button = LN_MOUSE_MIDDLE;
                    break;
                case WM_MBUTTONUP:
                    e = createEvent< MouseEvent >( LNEV_MOUSE_UP );
                    e->Button = LN_MOUSE_MIDDLE;
                    break;
            }  

            e->X = LOWORD( lparam_ ); 
	        e->Y = HIWORD( lparam_ );
            e->Delta = 0;

            *handled_ = true;
            *lnev_ = (Event*)e;
			break;
		}
        /////////////////////////////////////////////// マウス移動
		case WM_MOUSEMOVE:
		{
            MouseEvent* e = createEvent< MouseEvent >( LNEV_MOUSE_MOVE );
	        e->Button   = LN_MOUSE_NONE;
	        e->Delta    = 0;
	        e->X        = static_cast< short >( LOWORD( lparam_ ) );     // 一度 short にキャストしないと、
	        e->Y        = static_cast< short >( HIWORD( lparam_ ) );     // マイナス値になったとき 65535 とか値が入る

            *handled_ = true;
            *lnev_ = (Event*)e;
			break;
		}
        /////////////////////////////////////////////// クライアント領域外でマウスが移動した
		case WM_NCMOUSEMOVE:
		{
			// 念のためホントにクライアント領域外かチェック
			if ( wparam_ != HTCLIENT )
			{
                MouseEvent* e = createEvent< MouseEvent >( LNEV_MOUSE_MOVE );  // LNEV_MOUSE_MOVE として送る
                e->Button   = LN_MOUSE_NONE;
	            e->Delta    = 0;
                e->X        = -1;
	            e->Y        = -1;

                *handled_ = true;
	            *lnev_ = (Event*)e;
            }
			break;
		}
        ///////////////////////////////////////////// マウスホイールが操作された
		case WM_MOUSEWHEEL:
		{
            MouseEvent* e = createEvent< MouseEvent >( LNEV_MOUSE_WHEEL );
            e->Button   = LN_MOUSE_NONE;
            e->Delta    = GET_WHEEL_DELTA_WPARAM( wparam_ ) / WHEEL_DELTA;
            e->X        = static_cast< short >( LOWORD( lparam_ ) ); 
		    e->Y        = static_cast< short >( HIWORD( lparam_ ) ); 

            *handled_ = true;
            *lnev_ = (Event*)e;
			break;
		}
        ///////////////////////////////////////////// キー↓
		case WM_KEYDOWN:
		{
            KeyEvent* e = createEvent< KeyEvent >( LNEV_KEY_DOWN );
            e->KeyCode      = convertVirtualKeyCode( wparam_ );	// 仮想キーコード
            e->IsAlt        = ::GetKeyState( VK_MENU ) < 0;
			e->IsShift      = ::GetKeyState( VK_SHIFT ) < 0;
			e->IsControl    = ::GetKeyState( VK_CONTROL ) < 0;

            *handled_ = true;
			*lnev_ = (Event*)e;
			break;
		}
		
		///////////////////////////////////////////// キー↑
		case WM_KEYUP:
		{
            KeyEvent* e = createEvent< KeyEvent >( LNEV_KEY_UP );
            e->KeyCode      = convertVirtualKeyCode( wparam_ );	// 仮想キーコード
            e->IsAlt        = ::GetKeyState( VK_MENU ) < 0;
			e->IsShift      = ::GetKeyState( VK_SHIFT ) < 0;
			e->IsControl    = ::GetKeyState( VK_CONTROL ) < 0;

            *handled_ = true;
            *lnev_ = (Event*)e;
			break;
		}
			
		///////////////////////////////////////////// 文字入力
		case WM_CHAR:
		{	
			// 文字のみ送る
			if ( 0x20 <= wparam_ && wparam_ <= 0x7E )
			{
                KeyEvent* e = createEvent< KeyEvent >( LNEV_KEY_CHAR );
                e->KeyCode      = wparam_;
                e->IsAlt        = ::GetKeyState( VK_MENU ) < 0;
			    e->IsShift      = ::GetKeyState( VK_SHIFT ) < 0;
			    e->IsControl    = ::GetKeyState( VK_CONTROL ) < 0;

                *handled_ = true;
                *lnev_ = (Event*)e;
			}
			break;
		}
        /////////////////////////////////////////////// 不明なメッセージ
        //default:
        //{
        //    lnev_->Type = LNEV_NULL;
        //    break;
        //}
    }
    return 0;
}

//---------------------------------------------------------------------
// ● ライブラリで必要なメッセージかをチェックする
//---------------------------------------------------------------------
bool checkNecessaryMessage( UINT msg_ )
{
    switch ( msg_ )
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
    return false;
}

//---------------------------------------------------------------------
// ● VK_～ => LN_KEY_～ へのキーコード変換
//---------------------------------------------------------------------
LNVirtualKeyCode convertVirtualKeyCode( u32 win_vk_ )
{
    if ( 'A' <= win_vk_ && win_vk_ <= 'Z' ) return (LNVirtualKeyCode)(LN_KEY_A + ( win_vk_ - 'A' ));
	if ( '0' <= win_vk_ && win_vk_ <= '9' ) return (LNVirtualKeyCode)(LN_KEY_0 + ( win_vk_ - '0' ));
	if ( VK_F1 <= win_vk_ && win_vk_ <= VK_F12 ) return (LNVirtualKeyCode)(LN_KEY_F1 + ( win_vk_ - VK_F1 ));

	switch ( win_vk_ )
	{
		case VK_SPACE: 
			return LN_KEY_SPACE;
		case VK_ESCAPE: 
			return LN_KEY_ESCAPE;
		case VK_UP: 
			return LN_KEY_UP;
		case VK_DOWN: 
			return LN_KEY_DOWN;
		case VK_LEFT: 
			return LN_KEY_LEFT;
		case VK_RIGHT: 
			return LN_KEY_RIGHT;
		case VK_LSHIFT: 
			return LN_KEY_LSHIFT;
		case VK_RSHIFT: 
			return LN_KEY_RSHIFT;
		case VK_LCONTROL: 
			return LN_KEY_LCTRL;
		case VK_RCONTROL: 
			return LN_KEY_RCTRL;
		case VK_LMENU: 
			return LN_KEY_LALT;
		case VK_RMENU: 
			return LN_KEY_RALT;
		case VK_TAB: 
			return LN_KEY_TAB;
		case VK_RETURN: 
			return LN_KEY_ENTER;
		case VK_BACK: 
			return LN_KEY_BACKSPACE;
		case VK_INSERT: 
			return LN_KEY_INSERT;
		case VK_DELETE: 
			return LN_KEY_DELETE;
		case VK_PRIOR: 
			return LN_KEY_PAGEUP;
		case VK_NEXT: 
			return LN_KEY_PAGEDOWN;
		case VK_HOME: 
			return LN_KEY_HOME;
		case VK_END: 
			return LN_KEY_END;

		case VK_OEM_1: 
			return LN_KEY_COLON;
		case VK_OEM_PLUS: 
			return LN_KEY_SEMICOLON;
		case VK_OEM_COMMA: 
			return LN_KEY_COMMA;
		case VK_OEM_PERIOD: 
			return LN_KEY_PERIOD;
		case VK_OEM_2: 
			return LN_KEY_SLASH;
		case VK_OEM_MINUS: 
			return LN_KEY_MINUS;
		case VK_OEM_102: 
			return LN_KEY_BACKSLASH;
		case VK_OEM_5: 
			return LN_KEY_YEN;
		case VK_OEM_7: 
			return LN_KEY_CARET;
		case VK_OEM_4: 
			return LN_KEY_LBRACKET;
		case VK_OEM_6: 
			return LN_KEY_RBRACKET;
/*
		case 'A': return LN_KEY_A;
		case 'B': return LN_KEY_B;
		case 'C': return LN_KEY_C;
		case 'D': return LN_KEY_D;
		case 'E': return LN_KEY_E;
		case 'F': return LN_KEY_F;
		case 'G': return LN_KEY_G;
		case 'H': return LN_KEY_H;
		case 'I': return LN_KEY_I;
		case 'J': return LN_KEY_J;
		case 'K': return LN_KEY_K;
		case 'L': return LN_KEY_L;
		case 'M': return LN_KEY_M;
		case 'N': return LN_KEY_N;
		case 'O': return LN_KEY_O;
		case 'P': return LN_KEY_P;
		case 'Q': return LN_KEY_Q;
		case 'R': return LN_KEY_R;
		case 'S': return LN_KEY_S;
		case 'T': return LN_KEY_T;
		case 'U': return LN_KEY_U;
		case 'V': return LN_KEY_V;
		case 'W': return LN_KEY_W;
		case 'X': return LN_KEY_X;
		case 'Y': return LN_KEY_Y;
		case 'Z': return LN_KEY_Z;

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
		*/
	}
	return LN_KEY_UNKNOWN;
}

} // namespace Win32

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace System
} // namespace Core
} // namespace LNote

#endif // LNOTE_WINDOWS

//=============================================================================
//                              end of file
//=============================================================================