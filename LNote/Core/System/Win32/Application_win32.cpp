//=============================================================================
//�y Application �z
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
// �� Win32Application �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    Win32Application::Win32Application()
    {

    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    Win32Application::~Win32Application()
    {

    }

    //---------------------------------------------------------------------
	// �� ���C���E�B���h�E�����
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
	// �� �_�~�[�E�B���h�E�����
    //---------------------------------------------------------------------
    WindowBase* Win32Application::onCreateDummyWindow( void* window_handle_ )
    {
        return LN_NEW DummyWindow( this, window_handle_ );
    }

    //---------------------------------------------------------------------
	// �� �E�B���h�E�� finalize ���Ăяo���ĎQ�ƃJ�E���g���ЂƂ��炷
    //---------------------------------------------------------------------
    void Win32Application::onFinalizeWindow( WindowBase* window_ )
    {
        window_->finalize();
        LN_SAFE_RELEASE( window_ );
    }

    //---------------------------------------------------------------------
	// �� �R���\�[�����J��
    //---------------------------------------------------------------------
    void Win32Application::onAllocConsole()
    {
        // �R���\�[���A�v���Ƃ��Ď��s���Ă���ꍇ�͂��� if �ɓ��邱�Ƃ͂Ȃ�
        if ( ::AllocConsole() )
        {
	        freopen( "CON", "r", stdin );
	        freopen( "CON", "w", stdout );
            freopen( "CON", "w", stderr );
	        mConsoleAlloced = true;

	        // ���蓖�Ă���̓R���\�[���̃E�B���h�E���őO�ʂɂȂ�̂ŁA
	        // �Q�[���̃E�B���h�E���őO�ʂɈړ����Ă���
            ::SetForegroundWindow( mMainWindow->getWindowHandle() );
        }
    }

    //---------------------------------------------------------------------
	// �� �R���\�[�������
    //---------------------------------------------------------------------
    void Win32Application::onFreeConsole()
    {
        // ���蓖�Ă��R���\�[�����c���Ă���Ή��
		if ( mConsoleAlloced )
		{
			printf( "Enter �L�[�������ƏI�����܂�...\n" );
         
			getchar();
			::FreeConsole();
            mConsoleAlloced = false;

            // �R���\�[���A�v���Ƃ��Ď��s���Ă���ꍇ�͏I�����Ɂu�L�[�������ƏI���`�v��
		    // ���b�Z�[�W���o�Ĉꎞ��~���邯�ǁAAllocConsole() �Ŋ��蓖�Ă��ꍇ�͒�~���Ȃ��B
		    // ���̂��߁A��x���b�Z�[�W���o���� getchar() �Œ�~���Ă���I������
		}
    }

    //---------------------------------------------------------------------
	// �� ���b�Z�[�W����
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
	// �� �}�E�X�J�[�\���̕\����Ԃ�ݒ肷��
    //---------------------------------------------------------------------
    void Win32Application::onShowCursor( bool flag_ )
	{
		// �}�E�X�J�[�\���̕\���͓����ŎQ�ƃJ�E���g�̂悤�Ȃ��̂ŊǗ�����Ă���B
		// http://msdn.microsoft.com/ja-jp/library/cc364876.aspx
		// ���̂��߁A����Ȋ����� if �����܂��Ă����Ȃ��ƁA�J�E���g�������������肵�ĕs����o��B
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