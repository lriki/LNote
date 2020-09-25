//=============================================================================
//【 Window 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Base/StringUtil.h"
#include "SystemUtil.h"
#include "Application.h"
#include "Window.h"

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

//=============================================================================
// ■ Win32WindowBase クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● プロシージャ関数 
    //---------------------------------------------------------------------
    LRESULT Win32WindowBase::WndProc( HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_ )
    {
        switch( msg_ )
		{
            /////////////////// ウィンドウがアクティブ・非アクティブになった場合
			case WM_ACTIVATE:
			{
			    if ( wparam_ & 0xffff0000 )
			    {
				    mIsActive = false;
			    }
			    else
			    {
				    mIsActive = ( ( wparam_ & 0xffff ) != 0 );
			    }
			    break;
			}
            /////////////////// マウスキャプチャの処理
            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN:
                ::SetCapture( hwnd_ );
		        break;
	        case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            case WM_MBUTTONUP:
		        ::ReleaseCapture();
		        break;
        }

        // ライブラリに必要なメッセージの場合
        if ( Win32::checkNecessaryMessage( msg_ ) )
        {
            // WM_SYSKEYDOWN 関係をむやみに postMessage() するとなぜか強制終了するので必要なものだけフィルタリング
            if ( msg_ != WM_SYSKEYDOWN || (msg_ == WM_SYSKEYDOWN && wparam_ == VK_RETURN) )
            {
                Event* ev;
                bool handled;
                LRESULT lr;
                lr = Win32::translateMessageToLN( &ev, hwnd_, msg_, wparam_, lparam_, &handled );

                if ( ev )
                {
                    mApplication->postMessage( ev );
                }

                if ( handled ) return lr;

                //return 0;   // 処理済みとする
            }
        }

        return ::DefWindowProc( hwnd_, msg_, wparam_, lparam_ );
    }

    

//=============================================================================
// ■ Window クラス
//=============================================================================

    const lnChar* Window::WINDOW_CLASS_NAME = _T( "_LNote_" );
	const lnChar* Window::PROP_WINPROC      = _T( "_LNoteProp_" );

    const u32   WINDOWED_STYLE = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    const u32 FULLSCREEN_STYLE = WS_POPUP;

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    Window::Window( Win32Application* manager_ )
        : Win32WindowBase   ( manager_ )
        , mManager          ( manager_ )
        , mLogFile          ( NULL )
        , mOrginalWidth     ( 640 )
        , mOrginalHeight    ( 480 )
        , mWindowHandle     ( NULL )
        , mAccelerators     ( NULL )
        , mMainThreadID     ( 0 )
        , mFullScreen       ( false )
        , mIsActive         ( false )
    {
        LN_SAFE_ADDREF( mManager );
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    Window::~Window()
    {
        LN_SAFE_RELEASE( mManager );
    }



    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    LNRESULT Window::initialize( const InitData& init_data_ )
    {
        mTitleText      = init_data_.TitleText;
        mClientSize.w   = init_data_.Width;
        mClientSize.h   = init_data_.Height;
        //mEventListener  = init_data_.EventListener;
        mMainThreadID   = init_data_.MainThreadID;
        mFullScreen     = false;
        mOrginalWidth   = mClientSize.w;
        mOrginalHeight  = mClientSize.h;

        HINSTANCE inst = (HINSTANCE)::GetModuleHandle( NULL );

        // ウィンドウアイコン
        HICON icon = NULL;
        if ( init_data_.IconResourceID )
        {
            icon = ::LoadIcon( inst, MAKEINTRESOURCE( init_data_.IconResourceID ) );
        }

        // ウィンドウクラスの設定
		WNDCLASSEX	wcex = {
			sizeof( WNDCLASSEX ),			    // この構造体のサイズ
			NULL,							    // ウインドウのスタイル
			_staticMsgProc,					    // メッセージ処理関数
			0, 0,							    // 通常は使わないので常に0
			inst,				                // インスタンスハンドル
			icon,							    // アイコン
            ::LoadCursor( NULL, IDC_ARROW ),	// カーソルの形
			(HBRUSH)( COLOR_WINDOW + 1 ),	    // 背景色
			NULL,							    // メニューなし
            ( init_data_.WinClassName ) ? init_data_.WinClassName : WINDOW_CLASS_NAME,				// クラス名の指定
			NULL };							    // 小アイコン（なし）

		// ウィンドウクラスの登録
		if ( !::RegisterClassEx( &wcex ) )
		{
            LN_SETERR_R_S( LN_ERR_SYSTEM, "< Window::initialize >\nRegisterClassEx" );
		}

        // ウィンドウの作成
		mWindowHandle = ::CreateWindow(
				( init_data_.WinClassName ) ? init_data_.WinClassName : WINDOW_CLASS_NAME,
				mTitleText.c_str(),
                ( init_data_.Windowed ) ? WINDOWED_STYLE : FULLSCREEN_STYLE,
				CW_USEDEFAULT, CW_USEDEFAULT, 
				CW_USEDEFAULT, CW_USEDEFAULT,
				NULL, NULL, inst, NULL );
        LN_SETERR_R_S_( ( mWindowHandle ), LN_ERR_SYSTEM, "< Window::initialize >\nCreateWindow" );

		// ウィンドウハンドルとこのクラスのポインタを関連付ける
		BOOL r = ::SetProp( mWindowHandle, PROP_WINPROC, this );
        LN_SETERR_R_S_( ( r != FALSE ), LN_ERR_SYSTEM, "< Window::initialize >\nSetProp" );

        // アクセラレータの作成
        ACCEL accels[ 1 ] =
        {
            { FALT | FVIRTKEY, VK_RETURN, 0 }
        };
        mAccelerators = ::CreateAcceleratorTable( accels, 1 );
        LN_SETERR_R_S_( ( mAccelerators ), LN_ERR_SYSTEM, "< Window::initialize >\nCreateAcceleratorTable" );

        // WM_PAINTが呼ばれないようにする
		::ValidateRect( mWindowHandle, 0 );

		// ウィンドウの大きさを変更する
        _resize( !init_data_.Windowed );

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 終了処理
    //---------------------------------------------------------------------
    void Window::finalize()
    {
		if ( mWindowHandle )
		{
			::DestroyWindow( mWindowHandle );
			//UnregisterClass( WINDOW_CLASS_NAME, mInstanceHandle );
			mWindowHandle = NULL;
		}
    }

    
    //---------------------------------------------------------------------
	// ● ウィンドウの可視状態を設定する
    //---------------------------------------------------------------------
    void Window::setVisible( bool flag_ )
    {
        ::ShowWindow( mWindowHandle, ( flag_ ) ? SW_SHOWNORMAL : SW_HIDE );
    }

    //---------------------------------------------------------------------
	// ● フルスクリーンの有効設定
    //---------------------------------------------------------------------
    void Window::setEnableFullScreen( bool flag_ )
    {
        // フルスクリーンにする場合
		if ( flag_ )
		{
            mFullScreen = true;
            ::SetWindowLong( mWindowHandle, GWL_STYLE, FULLSCREEN_STYLE );
            _resize( true );
		}
		// ウィンドウモードにする場合
		else
		{
            mFullScreen = false;
            _resize( false );
		}
    }

    //---------------------------------------------------------------------
	// ● タイトルバーの文字列の後ろに追加で表示する文字列を設定する
    //---------------------------------------------------------------------
    void Window::setWindowTextFooter( const lnChar* format_, ... )
    {
        lnChar buf[ MAX_WINDOW_TEXT_FOOTER ];
		va_list args;
		int len;
		va_start( args, format_ );
		len = _vsctprintf( format_, args ) + 1;
		_vstprintf_s( buf, len, format_, args );
		va_end( args );

        lnChar text[ MAX_WINDOW_TEXT ] = { 0 };
        _stprintf_s( text, MAX_WINDOW_TEXT, _T( "%s%s" ), mTitleText.c_str(), buf );
        ::SetWindowText( mWindowHandle, text );
    }

    //---------------------------------------------------------------------
	// ● ウィンドウサイズを変更する
    //---------------------------------------------------------------------
    void Window::_resize( bool fullscreen_ )
    {
		BOOL res;
		int x, y, sw, sh;
		int win_w = mOrginalWidth;
		int win_h = mOrginalHeight;

		// フルスクリーンモードの場合
		if ( fullscreen_ )
		{
            
            /*
            // 一瞬塗りつぶす
            HDC hdc;
	        PAINTSTRUCT ps;
            hdc = BeginPaint(mWindowHandle , &ps);

		    //SelectObject(hdc , CreateSolidBrush(RGB(0xFF , 0 , 0)));
		    PatBlt(hdc , 10 , 10 , 200 , 100 , PATCOPY);
		    //DeleteObject(
			   // SelectObject(
				  //  hdc , GetStockObject(WHITE_BRUSH)
			   // )
		    //);

		    EndPaint(mWindowHandle , &ps);
            */


            // 画面いっぱい
            mClientSize.w = ::GetSystemMetrics( SM_CXSCREEN );
			mClientSize.h = ::GetSystemMetrics( SM_CYSCREEN );
            

            ::SetForegroundWindow( mWindowHandle );

            x = ( ::GetSystemMetrics( SM_CXSCREEN ) - mClientSize.w ) / 2;
			y = ( ::GetSystemMetrics( SM_CYSCREEN ) - mClientSize.h ) / 2;

			//res = ::SetWindowPos( mWindowHandle, HWND_TOP, 0, 0, mWidth, mHeight, SWP_SHOWWINDOW );
            ::SetWindowPos( mWindowHandle, NULL, x, y, mClientSize.w, mClientSize.h, SWP_SHOWWINDOW );
		}
        // ウィンドウモードの場合は実際にウィンドウの位置を設定する
        else
        {
            mClientSize.w  = mOrginalWidth;
		    mClientSize.h = mOrginalHeight;

            // 新しいクライアント領域の矩形を作成
		    mWindowRect.left = 0;
		    mWindowRect.top = 0;
		    mWindowRect.right = win_w;
		    mWindowRect.bottom = win_h;

            // 念のため
            ::SetWindowLong( mWindowHandle, GWL_STYLE, WINDOWED_STYLE );

            // 枠の大きさも含めたウィンドウサイズを取得
		    res = ::AdjustWindowRect( &mWindowRect, WINDOWED_STYLE, FALSE );

            // 必要なウィンドウの幅と高さを計算
		    win_w = mWindowRect.right - mWindowRect.left;
		    win_h = mWindowRect.bottom - mWindowRect.top;

			// ディスプレイ全体のサイズを取得
			sw = ::GetSystemMetrics( SM_CXSCREEN );
			sh = ::GetSystemMetrics( SM_CYSCREEN );

			// ディスプレイの中央に表示できる座標を計算
			x = ( sw - win_w ) / 2;
			y = ( sh - win_h ) / 2;

			// 位置と大きさを変更 (SWP_SHOWWINDOW を指定しないと、フルスクリーンからの復帰時に何も描画されなくなった)
			res = ::SetWindowPos( mWindowHandle, NULL, x, y, win_w, win_h, SWP_SHOWWINDOW );
        }
    }

    //---------------------------------------------------------------------
	// ● 呼び出し元プロシージャ関数
	//---------------------------------------------------------------------
	LRESULT CALLBACK Window::_staticMsgProc( HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_ )
	{
        //static int i = 0;
        //printf( "%d %4x\n", i++, msg_ );
        /*
        0x000C SPI_LANGDRIVER
        0x00AE ?
        0x007F SPI_SETMOUSEDOCKTHRESHOLD
        */

        Window* window = (Window*)::GetProp( hwnd_, PROP_WINPROC );

        if ( window )
        {
            return window->WndProc( hwnd_, msg_, wparam_, lparam_ );
        }
        else
        {
            return ::DefWindowProc( hwnd_, msg_, wparam_, lparam_ );
        }
	}

    //---------------------------------------------------------------------
	// ● プロシージャ関数
    //---------------------------------------------------------------------
    LRESULT Window::WndProc( HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_ )
    {
        switch ( msg_ )
        {
            /////////////////// ウィンドウが破棄された場合
            case WM_DESTROY:
            {
			    ::PostQuitMessage( 0 );

                // ウィンドウハンドルとポインタの関連付けを解除
		        ::RemoveProp( hwnd_, PROP_WINPROC );
			    return 0;
            }
            /////////////////// 上記以外は基底に回す
            default:
            {
                return Win32WindowBase::WndProc( hwnd_, msg_, wparam_, lparam_ );
            }
        }
    }

  
     /*
	    if ( window )
	    {
            // メインスレッドとは別でメッセージ処理を行っている場合は、
            // 必要なメッセージだけをメインスレッドに送る
            if ( window->mMainThreadID )
            {
                
                if ( Win32::checkNecessaryMessage( msg_ ) )
                {
                    // ワーカースレッドから送られた Alt Enter であることを示すフラグを付ける
                    if ( msg_ == WM_SYSKEYDOWN && wparam_ == VK_RETURN )
                    {
                        lparam_ = LN_SYSTEMTHREADMSG_MAGIC;
                    }

                    ::PostThreadMessage( window->mMainThreadID, msg_, wparam_, lparam_ );
                    return 0;   // 処理済みとする
                }
            }
            else
            {
                LNEvent args;
                bool handled;
                LRESULT lr = Win32::translateMessageToLN( &args, hwnd_, msg_, wparam_, lparam_, &handled );

                bool r = window->mManager->lnMsgProc( args );
                if ( r || handled ) { return lr; }
            }
        }
        */



            // アイコン
        /*
        HICON icon = (HICON)LoadImage(
            (HINSTANCE)::GetModuleHandle(NULL),_T("D:/Downloads/4f578bb0953eb.ico"), 
            IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE );
        //SendMessage(mMainWindow->getWindowHandle(), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        
        */

        /*
        Dim hIcon As HICON
Dim AndBits[31] As Byte,XorBits[31] As Byte
FillMemory(AndBits,32,&HFF)
FillMemory(XorBits,32,&H00)
hIcon=CreateIcon(CreateStruct.hInstance,16,16,1,1,AndBits,XorBits)
SetClassLong(hMainWnd,GCL_HICONSM,hIcon) 'ウィンドウハンドルは適宜変更してください
        */

        // 32*8bit = 256bit

        // 16*16=256


        /*
        BYTE AndBits[ 8192 ] =
        {
            255, 0,
            255, 255,
            0, 0,
            0, 0,
            0, 0,
            255, 255,
            0, 0,
            0, 0,
            0, 0,
            255, 255,
            0, 0,
            0, 0,
            0, 0,
            255, 255,
            0, 0,
            0, 0,
        };

        u32 XorBits[ 256 ] =
        {
            0xffffffff, 0xffff0000, 0xffffffff, 0xffffffff,     0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,     0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,     0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
            0xffffffff, 0xff000000, 0x00ffffff, 0xff000000,
            0, 0,
            0, 0,
            0, 0,
            255, 255,
            0, 0,
            0, 0,
            0, 0,
            255, 255,
            0, 0,
            0, 0,
            0, 0,
            255, 255,
            0, 0,
            1, 1,
        };
        FillMemory(AndBits,8192,0x00);
        //FillMemory(XorBits,sizeof(XorBits),0xff);
        HICON icon=CreateIcon(hinst,16,16,1,32,AndBits,(BYTE*)XorBits);
  

*/

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace System
} // namespace Core
} // namespace LNote

#endif // LNOTE_WINDOWS

//=============================================================================
//								end of file
//=============================================================================