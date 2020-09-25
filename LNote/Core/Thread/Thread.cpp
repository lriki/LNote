//=============================================================================
//�y Thread �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "EventFlag.h"
#include "Thread.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Thread
{

//=============================================================================
// �� ThreadObject �N���X (win32)
//=============================================================================
#if defined(LNOTE_WINDOWS)
    
    class _thr_prx
    {
    public:

        static DWORD LNOTE_STDCALL _execute_static( void* ptr_ )
        {
            ThreadObject* obj = static_cast< ThreadObject* >( ptr_ );

            // ���s
            obj->run();
            //( *( obj ) )();

            // �I���t���O�𗧂Ă�
            obj->mFinished->setTrue();

            return 0;
        }
    };

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
	//---------------------------------------------------------------------
	ThreadObject::ThreadObject()
		: mFinished		( NULL )
        , mThread       ( NULL )
        , mThreadID     ( 0 )
	{
		}

	//---------------------------------------------------------------------
	// �� �f�X�g���N�^
	//---------------------------------------------------------------------
	ThreadObject::~ThreadObject()
	{
        if ( mFinished )
        {
            printf( "< ThreadObject::~ThreadObject >\n" );
            printf( "�X���b�h���I�����Ă��܂���Bwait() �őҋ@���܂��B\n" );
            printf( "���������������Ȃ����ꍇ�̓X���b�h���������[�v�ɂȂ��Ă��Ȃ����m�F���Ă��������B" );

            wait();
        }
		//LN_ASSERT_S_EXP_( ( mFinished ), LNException::Thread, "< Core::Thread::Thread::~Thread >\n\nstart() �ŊJ�n�����X���b�h��\n�K�� wait() ���Ăяo���ďI�����Ă��������B" );
	}

	//---------------------------------------------------------------------
	// �� ���s�J�n
	//---------------------------------------------------------------------
	void ThreadObject::start()
	{
		// �O�̂��I�����Ă��Ȃ��ꍇ�͑҂�
		wait();

		// �I���t���O������ĊJ�n
		mFinished = LN_NEW EventFlag( false );

#ifdef LNOTE_ENABLE_WIN32_RUNTIME_THREAD
        //mThread = (HANDLE)(::AfxBeginThread(_thr_prx::_execute_static, this));

        mThread = CreateThread(
		    NULL, //�Z�L�����e�B����
		    0, //�X�^�b�N�T�C�Y
		    _thr_prx::_execute_static, //�X���b�h�֐�
		    this, //�X���b�h�֐��ɓn������
		    0, //�쐬�I�v�V����(0�܂���CREATE_SUSPENDED)
		    &mThreadID);//�X���b�hID
#else
        mThread = reinterpret_cast< HANDLE >( _beginthreadex( NULL, 0, _thr_prx::_execute_static, this, 0, &mThreadID ) );
#endif
    }

	//---------------------------------------------------------------------
	// �� �I���҂�
	//---------------------------------------------------------------------
	void ThreadObject::wait()
	{
		if ( mFinished )
		{
			mFinished->wait();
			LN_SAFE_RELEASE( mFinished );
		}
	}

	//---------------------------------------------------------------------
	// �� �I�����Ă��邩�ǂ����̔���
	//---------------------------------------------------------------------
	bool ThreadObject::isFinished()
	{
		bool res = false;
		if ( mFinished )
		{
			// �I����Ă���ꍇ
			if ( mFinished->isTrue() )
			{
				LN_SAFE_RELEASE( mFinished );
				res = true;
			}
		}
		else
		{
			res = true;
		}
		return res;
	}

    //---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    long ThreadObject::getThreadID() const
    { 
        return mThreadID;
    }
    
#else
    
//=============================================================================
// �� ThreadObject �N���X (pthread)
//=============================================================================
    
    class _thr_prx
    {
    public:

        static void* _execute_static( void* ptr_ )
        {
            ThreadObject* obj = static_cast< ThreadObject* >( ptr_ );

            // ���s
            ( *( obj ) )();

            // �I���t���O�𗧂Ă�
            obj->mFinished->setTrue();

            return 0;
        }
    };

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
	//---------------------------------------------------------------------
	ThreadObject::ThreadObject()
		: mFinished		( NULL )
	{
		}

	//---------------------------------------------------------------------
	// �� �f�X�g���N�^
	//---------------------------------------------------------------------
	ThreadObject::~ThreadObject()
	{
        if ( mFinished )
        {
            printf( "< ThreadObject::~ThreadObject >\n" );
            printf( "�X���b�h���I�����Ă��܂���Bwait() �őҋ@���܂��B\n" );
            printf( "���������������Ȃ����ꍇ�̓X���b�h���������[�v�ɂȂ��Ă��Ȃ����m�F���Ă��������B" );

            wait();
        }
        //LN_ASSERT_S_EXP_( ( mFinished ), LNException::Thread, "< Core::Thread::Thread::~Thread >\n\nstart() �ŊJ�n�����X���b�h��\n�K�� wait() ���Ăяo���ďI�����Ă��������B" );
    }

    //---------------------------------------------------------------------
    // �� ���s�J�n
    //---------------------------------------------------------------------
    void ThreadObject::start()
    {
        // �O�̂��I�����Ă��Ȃ��ꍇ�͑҂�
        wait();

        // �I���t���O������ĊJ�n
        mFinished = LN_NEW EventFlag( false );

        pthread_create( &mThread,  NULL, _thr_prx::_execute_static, this );
    }

    //---------------------------------------------------------------------
    // �� �I���҂�
    //---------------------------------------------------------------------
    void ThreadObject::wait()
    {
        if ( mFinished )
        {
            mFinished->wait();
            LN_SAFE_RELEASE( mFinished );
        }
    }

    //---------------------------------------------------------------------
    // �� �I�����Ă��邩�ǂ����̔���
    //---------------------------------------------------------------------
    bool ThreadObject::isFinished()
    {
        bool res = false;
        if ( mFinished )
        {
            // �I����Ă���ꍇ
            if ( mFinished->isTrue() )
            {
                LN_SAFE_RELEASE( mFinished );
                res = true;
            }
        }
        else
        {
            res = true;
        }
        return res;
    }

    //---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    long ThreadObject::getThreadID() const
    { 
        return mThread;
    }
    
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

// ���ԑ҂�
void sleep( int ms_ )
{
#if defined(LNOTE_WINDOWS)
    ::Sleep( ms_ );
#else
    ::usleep( ms_ * 1000 );
#endif
}

//---------------------------------------------------------------------
// 
//---------------------------------------------------------------------
long getCurrentThreadID()
{
#if defined(LNOTE_WINDOWS)
    return ::GetCurrentThreadId();
#else
    return pthread_self();
#endif
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Thread
} // namespace Core

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================