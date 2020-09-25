//=============================================================================
//�y OS �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Base/StringUtil.h"
#include "OS.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{

//---------------------------------------------------------------------
// �� �V�X�e���������擾���� (�~���b)
//---------------------------------------------------------------------
lnU32 getSystemTime()
{
#if defined(LNOTE_WINDOWS)

    return ::timeGetTime();

#else

    timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
    //return ( (uint64_t) ts.tv_sec * (uint64_t) 1000000000 + (uint64_t) ts.tv_nsec ) / 100000;
    //return (uint64_t)ts.tv_sec * (uint64_t)10000 + (uint64_t)ts.tv_nsec;

#endif
}

//---------------------------------------------------------------------
// �� �J�����g�f�B���N�g�����擾����
//---------------------------------------------------------------------
void getCurrentDirectory( lnChar* buf_ )
{
#if defined(LNOTE_WINDOWS)
    ::GetCurrentDirectory( LN_MAX_PATH, buf_ );
#else

#ifdef LNOTE_UNICODE
    char buf[ LN_MAX_PATH ];
    ::getcwd( buf_, LN_MAX_PATH );
    Base::StringUtil::convMultiToWide( buf_, LN_MAX_PATH, buf );
#else
    ::getcwd( buf_, LN_MAX_PATH );
#endif

#endif
}



//---------------------------------------------------------------------
// �� ���b�Z�[�W�{�b�N�X�̕\�� (�G���[�p)
//---------------------------------------------------------------------
void showMessageBox( const lnChar* message_, const lnChar* title_ )
{
#if defined(LNOTE_WINDOWS)
    ::MessageBox( NULL, message_, title_, MB_OK | MB_ICONERROR );
#endif // LNOTE_WINDOWS

#ifdef defined(LNOTE_X11)
    Display* display = ::XOpenDisplay( NULL );
    int      screen  = DefaultScreen( display );

    // �_�C�A���O�E�B���h�E�����
    Window dialog = XCreateSimpleWindow(
        display,
        RootWindow( display, screen ),
        0, 0, 200, 200,                     //�E�C���h�E�̃T�C�Y
        1,                                  // �E�B���h�E�̋��E���̕�
        BlackPixel( display, screen ),      //�g�̐ݒ�
        WhitePixel( display, screen ) );    //�w�i�F�̐F�ԍ�

    //----------------------------------------------------------
    // �{�^��

    // �{�^���p�̐F�����
    Colormap cmap;
	XColor c0, c1;
    cmap = DefaultColormap( display, 0 );
    XAllocNamedColor( display, cmap, "rgb:20/20/ff", &c1, &c0 );

    // OK �{�^���쐬 (�E�B���h�E���E�C���h�E�̒��ɍ쐬���ă{�^���ɂ���)
    Window button = XCreateSimpleWindow(
        display,
        dialog,
        100, 100, 40, 20,
        1,
        c1.pixel,
        WhitePixel( display, screen ) );

	// �}�b�v���ĕ\��
    ::XMapWindow( display, button );

	// �C�x���g�}�X�N��o�^
    ::XSelectInput( display, button, ButtonPressMask | ButtonReleaseMask );

	//----------------------------------------------------------
    //

    // �E�B���h�E�̓��e���L��
    ::XSetWindowAttributes attr;
    attr.backing_store = WhenMapped;
    ::XChangeWindowAttributes( display, dialog, CWBackingStore, &attr ); 

    // �E�C���h�E���̐ݒ�
    ::XStoreName( display, dialog, "Error" );
    ::XSetIconName( display, dialog, "Error" );

    // �C�x���g�}�X�N��o�^
    ::XSelectInput( display, dialog, ExposureMask ); 

	// �O���t�B�b�N�R���e�L�X�g���擾
    GC gc = XCreateGC( display, RootWindow( display, screen ), 0, 0 ); 
	
    XSetForeground( display, gc, BlackPixel( display, DefaultScreen( display ) ) );

    // �t�H���g�Z�b�g�𐶐�����
    char** miss;
    char*  def;
    int    n_miss;
    XFontSet fonst_set = ::XCreateFontSet(
        display,
        "r14",
        &miss, &n_miss, &def );



	// �}�b�v���ĕ\��
    ::XMapWindow( display, dialog );
    ::XFlush( display );



    XEvent event;
    while ( 1 )
    {
        ::XNextEvent( display, &event );

    	switch ( event.type )
        {
    		case Expose:
			{
                // �{�^���̕�����`��
                ::XDrawString( display, button, gc, 10, 15, "OK", 2 );

                // ���b�Z�[�W������̕`��
#ifdef LNOTE_UNICODE
                ::XwcDrawText( display, dialog, fonst_set, gc, 8, 32, message_, wcslen( message_ ) );
#else
                ::XmbDrawString( display, dialog, fonst_set, gc, 8, 32, message_, strlen( message_ ) );
#endif

				XFlush( display );
                break;
			}
			// ��ʏ�Ń}�E�X�̃{�^���������ꂽ��
    		case ButtonPress:
            {
    			if ( event.xany.window == button )
                {
                    // �{�[�_�[���C���𑾂�����
                    ::XSetWindowBorderWidth( display, event.xany.window, 2 );
    			}
    			XFlush( display );
    			break;
            }
            // ��ʏ�Ń}�E�X�̃{�^���������ꂽ��
    		case ButtonRelease:
            {
    			if ( event.xany.window == button )
                {
                    // �{�[�_�[���C�����ׂ�����
                    ::XSetWindowBorderWidth( display, event.xany.window, 1 );
    			}
    			XFlush( display );
    	        goto EXIT;
    			break;
            }
    		default:
    			break;
    	}
    }

EXIT:

    ::XDestroyWindow( display, button );
    ::XDestroyWindow( display, dialog );

#endif // LNOTE_X11
}

//---------------------------------------------------------------------
// 
//---------------------------------------------------------------------
float getPhysicalPixelSize()
{
#if defined(LNOTE_WINDOWS)
    HDC hdc = ::GetDC(NULL);
    return ((float)::GetDeviceCaps(hdc, HORZSIZE)) / ((float)::GetDeviceCaps(hdc, HORZRES));
#endif
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