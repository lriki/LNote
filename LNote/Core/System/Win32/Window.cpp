//=============================================================================
//�y Window �z
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
// �� Win32WindowBase �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �v���V�[�W���֐� 
    //---------------------------------------------------------------------
    LRESULT Win32WindowBase::WndProc( HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_ )
    {
        switch( msg_ )
		{
            /////////////////// �E�B���h�E���A�N�e�B�u�E��A�N�e�B�u�ɂȂ����ꍇ
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
            /////////////////// �}�E�X�L���v�`���̏���
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

        // ���C�u�����ɕK�v�ȃ��b�Z�[�W�̏ꍇ
        if ( Win32::checkNecessaryMessage( msg_ ) )
        {
            // WM_SYSKEYDOWN �֌W���ނ�݂� postMessage() ����ƂȂ��������I������̂ŕK�v�Ȃ��̂����t�B���^�����O
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

                //return 0;   // �����ς݂Ƃ���
            }
        }

        return ::DefWindowProc( hwnd_, msg_, wparam_, lparam_ );
    }

    

//=============================================================================
// �� Window �N���X
//=============================================================================

    const lnChar* Window::WINDOW_CLASS_NAME = _T( "_LNote_" );
	const lnChar* Window::PROP_WINPROC      = _T( "_LNoteProp_" );

    const u32   WINDOWED_STYLE = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    const u32 FULLSCREEN_STYLE = WS_POPUP;

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
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
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    Window::~Window()
    {
        LN_SAFE_RELEASE( mManager );
    }



    //---------------------------------------------------------------------
	// �� ������
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

        // �E�B���h�E�A�C�R��
        HICON icon = NULL;
        if ( init_data_.IconResourceID )
        {
            icon = ::LoadIcon( inst, MAKEINTRESOURCE( init_data_.IconResourceID ) );
        }

        // �E�B���h�E�N���X�̐ݒ�
		WNDCLASSEX	wcex = {
			sizeof( WNDCLASSEX ),			    // ���̍\���̂̃T�C�Y
			NULL,							    // �E�C���h�E�̃X�^�C��
			_staticMsgProc,					    // ���b�Z�[�W�����֐�
			0, 0,							    // �ʏ�͎g��Ȃ��̂ŏ��0
			inst,				                // �C���X�^���X�n���h��
			icon,							    // �A�C�R��
            ::LoadCursor( NULL, IDC_ARROW ),	// �J�[�\���̌`
			(HBRUSH)( COLOR_WINDOW + 1 ),	    // �w�i�F
			NULL,							    // ���j���[�Ȃ�
            ( init_data_.WinClassName ) ? init_data_.WinClassName : WINDOW_CLASS_NAME,				// �N���X���̎w��
			NULL };							    // ���A�C�R���i�Ȃ��j

		// �E�B���h�E�N���X�̓o�^
		if ( !::RegisterClassEx( &wcex ) )
		{
            LN_SETERR_R_S( LN_ERR_SYSTEM, "< Window::initialize >\nRegisterClassEx" );
		}

        // �E�B���h�E�̍쐬
		mWindowHandle = ::CreateWindow(
				( init_data_.WinClassName ) ? init_data_.WinClassName : WINDOW_CLASS_NAME,
				mTitleText.c_str(),
                ( init_data_.Windowed ) ? WINDOWED_STYLE : FULLSCREEN_STYLE,
				CW_USEDEFAULT, CW_USEDEFAULT, 
				CW_USEDEFAULT, CW_USEDEFAULT,
				NULL, NULL, inst, NULL );
        LN_SETERR_R_S_( ( mWindowHandle ), LN_ERR_SYSTEM, "< Window::initialize >\nCreateWindow" );

		// �E�B���h�E�n���h���Ƃ��̃N���X�̃|�C���^���֘A�t����
		BOOL r = ::SetProp( mWindowHandle, PROP_WINPROC, this );
        LN_SETERR_R_S_( ( r != FALSE ), LN_ERR_SYSTEM, "< Window::initialize >\nSetProp" );

        // �A�N�Z�����[�^�̍쐬
        ACCEL accels[ 1 ] =
        {
            { FALT | FVIRTKEY, VK_RETURN, 0 }
        };
        mAccelerators = ::CreateAcceleratorTable( accels, 1 );
        LN_SETERR_R_S_( ( mAccelerators ), LN_ERR_SYSTEM, "< Window::initialize >\nCreateAcceleratorTable" );

        // WM_PAINT���Ă΂�Ȃ��悤�ɂ���
		::ValidateRect( mWindowHandle, 0 );

		// �E�B���h�E�̑傫����ύX����
        _resize( !init_data_.Windowed );

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� �I������
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
	// �� �E�B���h�E�̉���Ԃ�ݒ肷��
    //---------------------------------------------------------------------
    void Window::setVisible( bool flag_ )
    {
        ::ShowWindow( mWindowHandle, ( flag_ ) ? SW_SHOWNORMAL : SW_HIDE );
    }

    //---------------------------------------------------------------------
	// �� �t���X�N���[���̗L���ݒ�
    //---------------------------------------------------------------------
    void Window::setEnableFullScreen( bool flag_ )
    {
        // �t���X�N���[���ɂ���ꍇ
		if ( flag_ )
		{
            mFullScreen = true;
            ::SetWindowLong( mWindowHandle, GWL_STYLE, FULLSCREEN_STYLE );
            _resize( true );
		}
		// �E�B���h�E���[�h�ɂ���ꍇ
		else
		{
            mFullScreen = false;
            _resize( false );
		}
    }

    //---------------------------------------------------------------------
	// �� �^�C�g���o�[�̕�����̌��ɒǉ��ŕ\�����镶�����ݒ肷��
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
	// �� �E�B���h�E�T�C�Y��ύX����
    //---------------------------------------------------------------------
    void Window::_resize( bool fullscreen_ )
    {
		BOOL res;
		int x, y, sw, sh;
		int win_w = mOrginalWidth;
		int win_h = mOrginalHeight;

		// �t���X�N���[�����[�h�̏ꍇ
		if ( fullscreen_ )
		{
            
            /*
            // ��u�h��Ԃ�
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


            // ��ʂ����ς�
            mClientSize.w = ::GetSystemMetrics( SM_CXSCREEN );
			mClientSize.h = ::GetSystemMetrics( SM_CYSCREEN );
            

            ::SetForegroundWindow( mWindowHandle );

            x = ( ::GetSystemMetrics( SM_CXSCREEN ) - mClientSize.w ) / 2;
			y = ( ::GetSystemMetrics( SM_CYSCREEN ) - mClientSize.h ) / 2;

			//res = ::SetWindowPos( mWindowHandle, HWND_TOP, 0, 0, mWidth, mHeight, SWP_SHOWWINDOW );
            ::SetWindowPos( mWindowHandle, NULL, x, y, mClientSize.w, mClientSize.h, SWP_SHOWWINDOW );
		}
        // �E�B���h�E���[�h�̏ꍇ�͎��ۂɃE�B���h�E�̈ʒu��ݒ肷��
        else
        {
            mClientSize.w  = mOrginalWidth;
		    mClientSize.h = mOrginalHeight;

            // �V�����N���C�A���g�̈�̋�`���쐬
		    mWindowRect.left = 0;
		    mWindowRect.top = 0;
		    mWindowRect.right = win_w;
		    mWindowRect.bottom = win_h;

            // �O�̂���
            ::SetWindowLong( mWindowHandle, GWL_STYLE, WINDOWED_STYLE );

            // �g�̑傫�����܂߂��E�B���h�E�T�C�Y���擾
		    res = ::AdjustWindowRect( &mWindowRect, WINDOWED_STYLE, FALSE );

            // �K�v�ȃE�B���h�E�̕��ƍ������v�Z
		    win_w = mWindowRect.right - mWindowRect.left;
		    win_h = mWindowRect.bottom - mWindowRect.top;

			// �f�B�X�v���C�S�̂̃T�C�Y���擾
			sw = ::GetSystemMetrics( SM_CXSCREEN );
			sh = ::GetSystemMetrics( SM_CYSCREEN );

			// �f�B�X�v���C�̒����ɕ\���ł�����W���v�Z
			x = ( sw - win_w ) / 2;
			y = ( sh - win_h ) / 2;

			// �ʒu�Ƒ傫����ύX (SWP_SHOWWINDOW ���w�肵�Ȃ��ƁA�t���X�N���[������̕��A���ɉ����`�悳��Ȃ��Ȃ���)
			res = ::SetWindowPos( mWindowHandle, NULL, x, y, win_w, win_h, SWP_SHOWWINDOW );
        }
    }

    //---------------------------------------------------------------------
	// �� �Ăяo�����v���V�[�W���֐�
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
	// �� �v���V�[�W���֐�
    //---------------------------------------------------------------------
    LRESULT Window::WndProc( HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_ )
    {
        switch ( msg_ )
        {
            /////////////////// �E�B���h�E���j�����ꂽ�ꍇ
            case WM_DESTROY:
            {
			    ::PostQuitMessage( 0 );

                // �E�B���h�E�n���h���ƃ|�C���^�̊֘A�t��������
		        ::RemoveProp( hwnd_, PROP_WINPROC );
			    return 0;
            }
            /////////////////// ��L�ȊO�͊��ɉ�
            default:
            {
                return Win32WindowBase::WndProc( hwnd_, msg_, wparam_, lparam_ );
            }
        }
    }

  
     /*
	    if ( window )
	    {
            // ���C���X���b�h�Ƃ͕ʂŃ��b�Z�[�W�������s���Ă���ꍇ�́A
            // �K�v�ȃ��b�Z�[�W���������C���X���b�h�ɑ���
            if ( window->mMainThreadID )
            {
                
                if ( Win32::checkNecessaryMessage( msg_ ) )
                {
                    // ���[�J�[�X���b�h���瑗��ꂽ Alt Enter �ł��邱�Ƃ������t���O��t����
                    if ( msg_ == WM_SYSKEYDOWN && wparam_ == VK_RETURN )
                    {
                        lparam_ = LN_SYSTEMTHREADMSG_MAGIC;
                    }

                    ::PostThreadMessage( window->mMainThreadID, msg_, wparam_, lparam_ );
                    return 0;   // �����ς݂Ƃ���
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



            // �A�C�R��
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
SetClassLong(hMainWnd,GCL_HICONSM,hIcon) '�E�B���h�E�n���h���͓K�X�ύX���Ă�������
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