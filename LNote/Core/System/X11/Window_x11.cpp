//=============================================================================
//�y Window �z
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
// �� Window �N���X
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
    // �� �R���X�g���N�^
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
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    X11Window::~X11Window()
    {
        //LN_SAFE_RELEASE( mManager );
    }



    //---------------------------------------------------------------------
	// �� ������
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
        
        // �N���[�Y�{�^���֌W�̐ݒ�
        mAtomForCloseButton1 = XInternAtom( mXDisplay, "WM_PROTOCOLS", False );
        mAtomForCloseButton2 = XInternAtom( mXDisplay, "WM_DELETE_WINDOW", False );
        XSetWMProtocols( mXDisplay, mXWindow, &mAtomForCloseButton2, 1 );
        
        // �E�B���h�E�̃X�^�C�����o���Ă���
        //XGetWindowAttributes( mXDisplay, mXWindow, &mWindowAttributes );
        mWindowAttributes.backing_store = WhenMapped;
        XChangeWindowAttributes( mXDisplay, mXWindow, CWBackingStore, &mWindowAttributes ); 
       
        
        
		// �E�B���h�E�̑傫����ύX����
        _resize( !init_data_.Windowed );

        XFree( visual );
        visual = NULL;
        
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� �I������
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
	// �� �E�B���h�E�̉���Ԃ�ݒ肷��
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
	// �� �t���X�N���[���̗L���ݒ�
    //---------------------------------------------------------------------
    void X11Window::setEnableFullScreen( bool flag_ )
    {
        // �t���X�N���[���ɂ���ꍇ
        if ( flag_ )
        {
            mFullScreen = true;
            
            //�E�B���h�E�̑�����ύX
            mWindowAttributes.override_redirect = True;
            XChangeWindowAttributes( mXDisplay, mXWindow, CWOverrideRedirect, &mWindowAttributes );
            _resize( true );
        }
        // �E�B���h�E���[�h�ɂ���ꍇ
        else
        {
            mFullScreen = false;
            _resize( false );
            mWindowAttributes.override_redirect = False;
            XChangeWindowAttributes( mXDisplay, mXWindow, CWOverrideRedirect, &mWindowAttributes );
        }
    }

    //---------------------------------------------------------------------
	// �� �^�C�g���o�[�̕�����̌��ɒǉ��ŕ\�����镶�����ݒ肷��
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
	// �� �E�B���h�E�T�C�Y��ύX����
    //---------------------------------------------------------------------
    void X11Window::_resize( bool fullscreen_ )
    {
		int x, y;

		// �t���X�N���[�����[�h�̏ꍇ
		if ( fullscreen_ )
		{
		    mClientSize.w = DisplayWidth( mXDisplay, 0 );
		    mClientSize.h = DisplayHeight( mXDisplay, 0 );
		    
		    // �őO�ʂɈړ�
		    XRaiseWindow( mXDisplay, mXWindow );
		    
		    XMoveResizeWindow( mXDisplay, mXWindow, 0, 0, mClientSize.w, mClientSize.h );
		    
		}
        // �E�B���h�E���[�h�̏ꍇ�͎��ۂɃE�B���h�E�̈ʒu��ݒ肷��
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