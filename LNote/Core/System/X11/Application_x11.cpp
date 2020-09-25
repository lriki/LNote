//=============================================================================
//�y Application �z
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
// �� X11Application �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    X11Application::X11Application()
        : mXDisplay         ( NULL )
        , mX11MainWindow    ( NULL )
    {
        mXDisplay = XOpenDisplay( NULL );
    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    X11Application::~X11Application()
    {
        XCloseDisplay( mXDisplay );
    }

    //---------------------------------------------------------------------
	// �� ���C���E�B���h�E�����
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
	// �� �_�~�[�E�B���h�E�����
    //---------------------------------------------------------------------
    WindowBase* X11Application::onCreateDummyWindow( void* window_handle_ )
    {
        mX11MainWindow = NULL;
        return NULL;//LN_NEW DummyWindow( this, window_handle_ );
    }

    //---------------------------------------------------------------------
	// �� �E�B���h�E�� finalize ���Ăяo���ĎQ�ƃJ�E���g���ЂƂ��炷
    //---------------------------------------------------------------------
    void X11Application::onFinalizeWindow( WindowBase* window_ )
    {
        window_->finalize();
        LN_SAFE_RELEASE( window_ );
    }

    //---------------------------------------------------------------------
	// �� �R���\�[�����J��
    //---------------------------------------------------------------------
    void X11Application::onAllocConsole()
    {
        
    }

    //---------------------------------------------------------------------
	// �� �R���\�[�������
    //---------------------------------------------------------------------
    void X11Application::onFreeConsole()
    {
        
    }

    //---------------------------------------------------------------------
	// �� ���b�Z�[�W����
    //---------------------------------------------------------------------
    void X11Application::onProcMessage()
    {
        while ( XPending ( mXDisplay ) > 0 )
        {
            XEvent xev;
            XNextEvent( mXDisplay, &xev );

            // �����̂ւ�� Manager::WndProc �ɉ񂹂�Ƃ������������B
            
            switch ( xev.type )
            {
                case ClientMessage:
                {
                    // �N���[�Y�{�^���������ꂽ�ꍇ
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

            // ���C�u�����ɕK�v�ȃ��b�Z�[�W�̏ꍇ
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
	// �� �}�E�X�J�[�\���̕\����Ԃ�ݒ肷��
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