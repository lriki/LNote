//=============================================================================
//【 Application 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#if defined(LNOTE_X11)

#include "X11SystemUtil.h"
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
// ■ X11Application クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    X11Application::X11Application()
        : mXDisplay         ( NULL )
        , mX11MainWindow    ( NULL )
    {
        mXDisplay = XOpenDisplay( NULL );
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    X11Application::~X11Application()
    {
        XCloseDisplay( mXDisplay );
    }

    //---------------------------------------------------------------------
	// ● メインウィンドウを作る
    //---------------------------------------------------------------------
    LNRESULT X11Application::onCreateMainWindow( WindowBase** window_ )
    {
        LNRESULT lr;

        X11Window::InitData data;
        data.LogFile        = mLogFile;
        data.TitleText      = mInitData.TitleText;
        data.Width          = mInitData.Width;
        data.Height         = mInitData.Height;
        data.Windowed       = mInitData.Windowed;
        //data.MainThreadID   = 0;
        //data.WinClassName   = NULL;
        X11Window* w = LN_NEW X11Window( this, mXDisplay );

        lr = w->initialize( data );
        if ( LN_SUCCEEDED( lr ) )
        {
            mX11MainWindow = w;
            *window_ = w;
        }

        return lr;
    }

    //---------------------------------------------------------------------
	// ● ダミーウィンドウを作る
    //---------------------------------------------------------------------
    WindowBase* X11Application::onCreateDummyWindow( void* window_handle_ )
    {
        mX11MainWindow = NULL;
        return NULL;//LN_NEW DummyWindow( this, window_handle_ );
    }

    //---------------------------------------------------------------------
	// ● ウィンドウの finalize を呼び出して参照カウントをひとつ減らす
    //---------------------------------------------------------------------
    void X11Application::onFinalizeWindow( WindowBase* window_ )
    {
        window_->finalize();
        LN_SAFE_RELEASE( window_ );
    }

    //---------------------------------------------------------------------
	// ● コンソールを開く
    //---------------------------------------------------------------------
    void X11Application::onAllocConsole()
    {
        
    }

    //---------------------------------------------------------------------
	// ● コンソールを閉じる
    //---------------------------------------------------------------------
    void X11Application::onFreeConsole()
    {
        
    }

    //---------------------------------------------------------------------
	// ● メッセージ処理
    //---------------------------------------------------------------------
    void X11Application::onProcMessage()
    {
        while ( XPending ( mXDisplay ) > 0 )
        {
            XEvent xev;
            XNextEvent( mXDisplay, &xev );

            // ↓このへんを Manager::WndProc に回せるといい感じかも。
            
            switch ( xev.type )
            {
                case ClientMessage:
                {
                    // クローズボタンが押された場合
                    if ( mX11MainWindow->checkCloseButtonOfThis(
                        xev.xclient.message_type,
                        xev.xclient.data.l[ 0 ] ) )
                    {
                        LNEvent lnev;
                        lnev.Type = LNEV_CLOSE;
                        this->postMessage( lnev );
                    }
			        continue;
                }
            }

            // ライブラリに必要なメッセージの場合
            if ( X11::checkNecessaryMessage( xev ) )
            {
                LNEvent lnev;
                bool handled;
                X11::translateMessageToLN( &lnev, xev, &handled );
                this->postMessage( lnev );
            }
        }
    }

    //---------------------------------------------------------------------
	// ● マウスカーソルの表示状態を設定する
    //---------------------------------------------------------------------
    void X11Application::onShowCursor( bool flag_ )
    {
        if ( flag_ != mIsSystemMouseShow )
        {
            if ( flag_ )
            {
                //::ShowCursor( TRUE );
            }
            else
            {
                //::ShowCursor( FALSE );
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

#endif

//=============================================================================
//								end of file
//=============================================================================