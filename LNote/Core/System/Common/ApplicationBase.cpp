//=============================================================================
//�y Application �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
//#include "Window.h"
#include "ApplicationBase.h"

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
// �� MouseCursor �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�}�E�X�J�[�\���̕\���Ǘ�
//*/
//=============================================================================
class MouseCursor
{
public:

    MouseCursor( ApplicationBase* app_ )
        : mApplication          ( app_ )
        , mMouseVisibleCount    ( 0 )
        , mMouseVisibleCountMax ( 0 )
        , mVisibleMouseCursor   ( true )
        //, mIsSystemMouseShow    ( true )
        , mRequest              ( true )
    {}

public:

    /// �}�E�X�J�[�\���̕\���ݒ� (count �� �}�E�X�J�[�\�����u���A�����܂ł̃t���[�����B0 �ŏ�ɏ���)
    void setMouseCursorVisibleState( bool flag_, int count_ )
    {
        mVisibleMouseCursor   = flag_;
        mMouseVisibleCountMax = std::max( 0, count_ );

        // �����ɏ����ꍇ
        if ( !mVisibleMouseCursor && mMouseVisibleCountMax == 0 )
        {
            mRequest = false;
        }
        else
        {
            mRequest = true;
        }
    }

    /// �t���[���X�V
    void update()
    {
        // �}�E�X�����΂炭�����Ȃ���΃J�[�\������������
		if ( !mVisibleMouseCursor && mMouseVisibleCountMax > 0 )
		{
			if ( mMouseVisibleCount < mMouseVisibleCountMax )
			{
				++mMouseVisibleCount;
				if ( mMouseVisibleCount == mMouseVisibleCountMax )
				{
					//_showCursor( false );
                    mRequest = false;
                    
				}
			}
		}
    }

    /// �\����ԍX�V (���[�J�[�X���b�h����ĂԂ̂͂��̊֐�����)
    void updateVisibleState()
    {
        mApplication->onShowCursor( mRequest );
    }

    /// �}�E�X�J�[�\���̑����ɌĂ�ŁA���݂̐ݒ�ɉ����ă}�E�X�J�[�\���̕\���E��\����؂�ւ���
	void onMoveCursor( bool on_client_area_ )
	{
        // �\�����Ȃ��ꍇ�̓N���C�A���g�̈���ł̂ݕ\�����Ȃ�
        if ( !mVisibleMouseCursor && mMouseVisibleCountMax == 0 )
        {
            if ( on_client_area_ )
            {
                mRequest = false;
            }
            else
            {
                mRequest = true;
                mMouseVisibleCount = 0;
            }
        }
        else
        {
            mRequest = true;
            mMouseVisibleCount = 0;
        }
	}

    

private:

    ApplicationBase* mApplication;
    volatile int     mMouseVisibleCount;
    volatile int     mMouseVisibleCountMax;  ///< �}�E�X�J�[�\�����u���A�����܂ł̃t���[����
    volatile bool    mVisibleMouseCursor;
    
    volatile bool    mRequest;

};

//=============================================================================
// �� ApplicationBase �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    ApplicationBase::ApplicationBase() 
        : mLogFile          ( NULL )
		, mManager			( NULL )
        , mMainWindow       ( NULL )
        , mMouseCursor      ( NULL )
        , mFinishedInit     ( false )
        , mEndRequest       ( true )
        , mResultInit       ( LN_OK )
        , mThreaded         ( false )
        , mConsoleAlloced   ( false )
        , mIsSystemMouseShow( true )
    {

    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    ApplicationBase::~ApplicationBase()
    {

    }

    //---------------------------------------------------------------------
	// �� ������
    //---------------------------------------------------------------------
    LNRESULT ApplicationBase::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        mFinishedInit.setFalse();

        LNRESULT lr;

        mInitData = init_data_;
		mManager = mInitData.SystemManager;

        // ���[�U�[��`�̃E�B���h�E�n���h��������ꍇ�̓_�~�[�E�B���h�E�����
        if ( init_data_.WindowHandle )
        {
            //LN_LOG_WRITE( "���[�U�[��`�̃E�B���h�E�n���h�����g�p���܂��B" );
            LN_LOG_WRITE( L"    UserHWND : %p", init_data_.WindowHandle );
            
            mMainWindow = onCreateDummyWindow( init_data_.WindowHandle );
        }
        // �E�B���h�E�n���h�����󂯎���Ă��Ȃ��ꍇ�͕��ʂɍ��
        else
        {
            mThreaded = mInitData.UseThread;
            LN_LOG_WRITE( L"    Threaded : %d", mThreaded );

            if ( mThreaded )
            {
                this->start();

                // ���������I���܂őҋ@����
                mFinishedInit.wait();

                if ( LN_FAILED( mResultInit ) )
                {
                    return mResultInit;
                }
            }
            else
            {
                LN_CALL_R( _initialize() );
            }
        }

        // �R���\�[�����g���ꍇ
        if ( init_data_.UseConsole )
	    {
            onAllocConsole();
        }

        mMouseCursor = LN_NEW MouseCursor( this );

        LN_LOG_INIT_END;
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� �I������
    //---------------------------------------------------------------------
    void ApplicationBase::finalize()
    {
        LN_LOG_FIN_BEGIN;

        if ( mThreaded )
        {
            mEndRequest.setTrue();
            this->wait();
        }
        else
        {
            _finalize();
        }

        onFreeConsole();

        ln_foreach( Event* e, mEventQueue )
        {
            SAFE_DELETE( e );
        }
        mEventQueue.clear();

        SAFE_DELETE( mMouseCursor );

        LN_LOG_FIN_END;
    }


    //---------------------------------------------------------------------
	// �� �}�E�X�J�[�\���̕\���ݒ� (flag_ �� false �̏ꍇ�Acount �����҂��Ă�������B0�ł�������)
    //---------------------------------------------------------------------
    void ApplicationBase::setMouseCursorVisibleState( bool flag_, int count_ )
    {
        if ( mMouseCursor ) mMouseCursor->setMouseCursorVisibleState( flag_, count_ );
    }

    //---------------------------------------------------------------------
	// �� �}�E�X���ړ������Ƃ��ɌĂяo��
    //---------------------------------------------------------------------
    void ApplicationBase::onMoveCursor( bool on_client_area_ )
    {
        if ( mMouseCursor ) mMouseCursor->onMoveCursor( on_client_area_ );
    }

    //---------------------------------------------------------------------
	// �� �X�V
    //---------------------------------------------------------------------
    void ApplicationBase::update()
    {
        if ( !mThreaded )
        {
            _update();
        }
        if ( mMouseCursor ) mMouseCursor->update();
    }

    //---------------------------------------------------------------------
	// �� �C�x���g���b�Z�[�W���|�X�g����
    //---------------------------------------------------------------------
    void ApplicationBase::postMessage( Event* event_ )
    {
        if ( event_ )
        {
            mLock.lock();
            mEventQueue.push_back( event_ );
            mLock.unlock();
        }
    }

    //---------------------------------------------------------------------
	// �� ��������ׂ��C�x���g���b�Z�[�W�����邩���m�F���� (true=����)
    //---------------------------------------------------------------------
    bool ApplicationBase::peekMessage()
    {
        return ( !mEventQueue.empty() );
    }

    //---------------------------------------------------------------------
	// �� ��������ׂ��C�x���g���b�Z�[�W���擾���� (�擾�����ꍇ�A�L���[����폜���� )
    //---------------------------------------------------------------------
    const Event* ApplicationBase::getMessage()
    {
        mLock.lock();

        const Event* e = mEventQueue.front();
        mEventQueue.pop_front();

        mLock.unlock();
        return e;
    }

    //---------------------------------------------------------------------
	// �� �X���b�h�֐�
    //---------------------------------------------------------------------
    void ApplicationBase::run()
    {
        _initialize();
        while ( !mEndRequest.isTrue() )
        {
            _update();
            Thread::sleep( 16 );
        }
        _finalize();
    }


    //---------------------------------------------------------------------
	// �� ������
    //---------------------------------------------------------------------
    LNRESULT ApplicationBase::_initialize()
    {
        mEndRequest.setTrue();
        mResultInit = LN_OK;

        mResultInit = onCreateMainWindow( &mMainWindow );

        // �������I��
        mFinishedInit.setTrue();

        if ( LN_SUCCEEDED( mResultInit ) )
        {
            mEndRequest.setFalse();
        }
        return mResultInit;
    }

    //---------------------------------------------------------------------
	// �� �I������
    //---------------------------------------------------------------------
    void ApplicationBase::_finalize()
    {
        if ( mMainWindow )
        {
            onFinalizeWindow( mMainWindow );
            mMainWindow = NULL;
        }
    }

    //---------------------------------------------------------------------
	// �� �X�V
    //---------------------------------------------------------------------
    void ApplicationBase::_update()
    {
        onProcMessage();
        if ( mMouseCursor ) mMouseCursor->updateVisibleState();
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