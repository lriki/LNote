//=============================================================================
//【 Window 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"

#if defined(LNOTE_X11)

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
//#include "../Base/StringUtil.h"
//#include "PlatformAPI.h"
#include "../Manager.h"
#include "Window.h"



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Engine
{
namespace System
{

//=============================================================================
// ■ Window クラス
//=============================================================================

    int GLX_ATTRS[] = {
      GLX_USE_GL,
      GLX_LEVEL, 0,
      GLX_RGBA,
      GLX_DOUBLEBUFFER,
      GLX_RED_SIZE, 8,
      GLX_GREEN_SIZE, 8,
      GLX_BLUE_SIZE, 8,
      GLX_ALPHA_SIZE, 0,
      GLX_DEPTH_SIZE, 16,
      GLX_STENCIL_SIZE, 0,
      GLX_ACCUM_RED_SIZE, 0,
      GLX_ACCUM_GREEN_SIZE, 0,
      GLX_ACCUM_BLUE_SIZE, 0,
      GLX_ACCUM_ALPHA_SIZE, 0,
      None
    };
    
    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    X11Window::X11Window( X11Application* manager_, Display* display_ )
        : X11WindowBase     ( manager_ )
        , mManager          ( manager_ )
        , mLogFile          ( NULL )
        , mOrginalWidth     ( 640 )
        , mOrginalHeight    ( 480 )
        , mXDisplay         ( display_ )
        , mXDefaultScreen   ( 0 )
        , mXRootWindow      ( 0 )
        , mXWindow          ( 0 )

        , mFullScreen       ( false )
    {
        //LN_SAFE_ADDREF( mManager );
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    X11Window::~X11Window()
    {
        //LN_SAFE_RELEASE( mManager );
    }



    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    LNRESULT X11Window::initialize( const InitData& init_data_ )
    {
        mTitleText      = init_data_.TitleText;
        mClientSize.w   = init_data_.Width;
        mClientSize.h   = init_data_.Height;
        mFullScreen     = false;
        mOrginalWidth   = mClientSize.w;
        mOrginalHeight  = mClientSize.h;
        
  
        mXDefaultScreen = DefaultScreen( mXDisplay );
        mXRootWindow    = RootWindow( mXDisplay, mXDefaultScreen );
        
        
        
    
        //XCloseDisplay(display);
        
        
        XVisualInfo* visual = glXChooseVisual( mXDisplay, mXDefaultScreen, GLX_ATTRS );
        
        printf( "visual %p\n", visual );
        
        Colormap color_map = XCreateColormap(
            mXDisplay,
            mXRootWindow,
            visual->visual,
            AllocNone );
        
        mWindowAttributes.colormap = color_map;
        mWindowAttributes.border_pixel = 0;
        mWindowAttributes.override_redirect = False;
        mWindowAttributes.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;
        
        mXWindow = XCreateWindow(
            mXDisplay,
            mXRootWindow,
            0, 0, 
            mOrginalWidth,
            mOrginalHeight,
            0,                  // Border width
            visual->depth,      // Color depth
            InputOutput,
            visual->visual,
            CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
            &mWindowAttributes );
        
        // クローズボタン関係の設定
        mAtomForCloseButton1 = XInternAtom( mXDisplay, "WM_PROTOCOLS", False );
        mAtomForCloseButton2 = XInternAtom( mXDisplay, "WM_DELETE_WINDOW", False );
        XSetWMProtocols( mXDisplay, mXWindow, &mAtomForCloseButton2, 1 );
        
        // ウィンドウのスタイルを覚えておく
        //XGetWindowAttributes( mXDisplay, mXWindow, &mWindowAttributes );
        mWindowAttributes.backing_store = WhenMapped;
        XChangeWindowAttributes( mXDisplay, mXWindow, CWBackingStore, &mWindowAttributes ); 
       
        
        
		// ウィンドウの大きさを変更する
        _resize( !init_data_.Windowed );

        XFree( visual );
        visual = NULL;
        
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 終了処理
    //---------------------------------------------------------------------
    void X11Window::finalize()
    {
		if ( mXWindow )
		{
			XDestroyWindow( mXDisplay, mXWindow );
			mXWindow = 0;
		}

        if ( mXDisplay )
        {
            mXDisplay = NULL;
        }
    }

    
    //---------------------------------------------------------------------
	// ● ウィンドウの可視状態を設定する
    //---------------------------------------------------------------------
    void X11Window::setVisible( bool flag_ )
    {
        if ( flag_ )
        {
            XMapWindow( mXDisplay, mXWindow );
            XFlush( mXDisplay );
        }
        else
        {
            XUnmapWindow( mXDisplay, mXWindow );
            XFlush( mXDisplay );
        }
    }

    //---------------------------------------------------------------------
	// ● フルスクリーンの有効設定
    //---------------------------------------------------------------------
    void X11Window::setEnableFullScreen( bool flag_ )
    {
        // フルスクリーンにする場合
        if ( flag_ )
        {
            mFullScreen = true;
            
            //ウィンドウの属性を変更
            mWindowAttributes.override_redirect = True;
            XChangeWindowAttributes( mXDisplay, mXWindow, CWOverrideRedirect, &mWindowAttributes );
            _resize( true );
        }
        // ウィンドウモードにする場合
        else
        {
            mFullScreen = false;
            _resize( false );
            mWindowAttributes.override_redirect = False;
            XChangeWindowAttributes( mXDisplay, mXWindow, CWOverrideRedirect, &mWindowAttributes );
        }
    }

    //---------------------------------------------------------------------
	// ● タイトルバーの文字列の後ろに追加で表示する文字列を設定する
    //---------------------------------------------------------------------
    void X11Window::setWindowTextFooter( const lnChar* format_, ... )
    {
        /*
        lnChar buf[ MAX_WINDOW_TEXT_FOOTER ];
		va_list args;
		int len;
		va_start( args, format_ );
		len = _vsctprintf( format_, args ) + 1;
		_vstprintf_s( buf, len, format_, args );
		va_end( args );

		lnChar text[ MAX_WINDOW_TEXT ];
		wsprintf( text, LNTEXT( "%s%s" ), mTitleText, buf );
        ::SetWindowText( mWindowHandle, text );
        */
    }

    //---------------------------------------------------------------------
	// ● ウィンドウサイズを変更する
    //---------------------------------------------------------------------
    void X11Window::_resize( bool fullscreen_ )
    {
		int x, y;

		// フルスクリーンモードの場合
		if ( fullscreen_ )
		{
		    mClientSize.w = DisplayWidth( mXDisplay, 0 );
		    mClientSize.h = DisplayHeight( mXDisplay, 0 );
		    
		    // 最前面に移動
		    XRaiseWindow( mXDisplay, mXWindow );
		    
		    XMoveResizeWindow( mXDisplay, mXWindow, 0, 0, mClientSize.w, mClientSize.h );
		    
		}
        // ウィンドウモードの場合は実際にウィンドウの位置を設定する
        else
        {
            mClientSize.w = mOrginalWidth;
		    mClientSize.h = mOrginalHeight;
            
            x = ( DisplayWidth( mXDisplay, 0 )  - mClientSize.w ) / 2;
            y = ( DisplayHeight( mXDisplay, 0 ) - mClientSize.h ) / 2;
            
            XMoveResizeWindow( mXDisplay, mXWindow, x, y, mClientSize.w, mClientSize.h );
            
        }
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace System
} // namespace Engine
} // namespace Core
} // namespace LNote

#endif

//=============================================================================
//								end of file
//=============================================================================