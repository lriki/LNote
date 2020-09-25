//=============================================================================
//【 Application 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Manager.h"
#include "Window.h"
#include "Application.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{

//=============================================================================
// ■ Win32Application クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    Win32Application::Win32Application()
    {

    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    Win32Application::~Win32Application()
    {

    }

    //---------------------------------------------------------------------
	// ● メインウィンドウを作る
    //---------------------------------------------------------------------
    LNRESULT Win32Application::onCreateMainWindow( WindowBase** window_ )
    {
        Window::InitData data;
        data.LogFile        = mLogFile;
        data.TitleText      = mInitData.TitleText;
        data.Width          = mInitData.Width;
        data.Height         = mInitData.Height;
        data.Windowed       = mInitData.Windowed;
        data.MainThreadID   = 0;
        data.WinClassName   = NULL;
        Window* w = LN_NEW Window( this );
        *window_ = w;
        return w->initialize( data );
    }


    //---------------------------------------------------------------------
	// ● ダミーウィンドウを作る
    //---------------------------------------------------------------------
    WindowBase* Win32Application::onCreateDummyWindow( void* window_handle_ )
    {
        return LN_NEW DummyWindow( this, window_handle_ );
    }

    //---------------------------------------------------------------------
	// ● ウィンドウの finalize を呼び出して参照カウントをひとつ減らす
    //---------------------------------------------------------------------
    void Win32Application::onFinalizeWindow( WindowBase* window_ )
    {
        window_->finalize();
        LN_SAFE_RELEASE( window_ );
    }

    //---------------------------------------------------------------------
	// ● コンソールを開く
    //---------------------------------------------------------------------
    void Win32Application::onAllocConsole()
    {
        // コンソールアプリとして実行している場合はこの if に入ることはない
        if ( ::AllocConsole() )
        {
	        freopen( "CON", "r", stdin );
	        freopen( "CON", "w", stdout );
            freopen( "CON", "w", stderr );
	        mConsoleAlloced = true;

	        // 割り当てた後はコンソールのウィンドウが最前面になるので、
	        // ゲームのウィンドウを最前面に移動しておく
            ::SetForegroundWindow( mMainWindow->getWindowHandle() );
        }
    }

    //---------------------------------------------------------------------
	// ● コンソールを閉じる
    //---------------------------------------------------------------------
    void Win32Application::onFreeConsole()
    {
        // 割り当てたコンソールが残っていれば解放
		if ( mConsoleAlloced )
		{
			printf( "Enter キーを押すと終了します...\n" );
         
			getchar();
			::FreeConsole();
            mConsoleAlloced = false;

            // コンソールアプリとして実行している場合は終了時に「キーを押すと終了～」と
		    // メッセージが出て一時停止するけど、AllocConsole() で割り当てた場合は停止しない。
		    // そのため、一度メッセージを出して getchar() で停止してから終了する
		}
    }

    //---------------------------------------------------------------------
	// ● メッセージ処理
    //---------------------------------------------------------------------
    void Win32Application::onProcMessage()
    {
        MSG msg;
        while ( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
        {
            if ( ::GetMessage( &msg, NULL, 0, 0 ) )
            {
				bool handled = false;
				this->mManager->WndProc(msg.hwnd, msg.message, msg.wParam, msg.lParam, &handled);
				if ( !handled ) 
				{

					switch ( msg.message )
					{
						case WM_MOUSEMOVE:
							this->onMoveCursor( true );
							break;
						case WM_NCMOUSEMOVE:
							this->onMoveCursor( false );
							break;
					}

					::TranslateMessage( &msg );
					::DispatchMessage( &msg );
				}
            }
        }
    }

    //---------------------------------------------------------------------
	// ● マウスカーソルの表示状態を設定する
    //---------------------------------------------------------------------
    void Win32Application::onShowCursor( bool flag_ )
	{
		// マウスカーソルの表示は内部で参照カウントのようなもので管理されている。
		// http://msdn.microsoft.com/ja-jp/library/cc364876.aspx
		// そのため、こんな感じで if を噛ませておかないと、カウントが増えすぎたりして不具合が出る。
		if ( flag_ != mIsSystemMouseShow )
		{
			if ( flag_ )
			{
				::ShowCursor( TRUE );
			}
			else
			{
				::ShowCursor( FALSE );
			}
			mIsSystemMouseShow = flag_;
		}
	}

    
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace System
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================