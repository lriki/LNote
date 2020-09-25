//=============================================================================
//【 Thread 】
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
// ■ ThreadObject クラス (win32)
//=============================================================================
#if defined(LNOTE_WINDOWS)
    
    class _thr_prx
    {
    public:

        static DWORD LNOTE_STDCALL _execute_static( void* ptr_ )
        {
            ThreadObject* obj = static_cast< ThreadObject* >( ptr_ );

            // 実行
            obj->run();
            //( *( obj ) )();

            // 終了フラグを立てる
            obj->mFinished->setTrue();

            return 0;
        }
    };

	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	ThreadObject::ThreadObject()
		: mFinished		( NULL )
        , mThread       ( NULL )
        , mThreadID     ( 0 )
	{
		}

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	ThreadObject::~ThreadObject()
	{
        if ( mFinished )
        {
            printf( "< ThreadObject::~ThreadObject >\n" );
            printf( "スレッドが終了していません。wait() で待機します。\n" );
            printf( "もし応答が無くなった場合はスレッドが無限ループになっていないか確認してください。" );

            wait();
        }
		//LN_ASSERT_S_EXP_( ( mFinished ), LNException::Thread, "< Core::Thread::Thread::~Thread >\n\nstart() で開始したスレッドは\n必ず wait() を呼び出して終了してください。" );
	}

	//---------------------------------------------------------------------
	// ● 実行開始
	//---------------------------------------------------------------------
	void ThreadObject::start()
	{
		// 前のが終了していない場合は待つ
		wait();

		// 終了フラグを作って開始
		mFinished = LN_NEW EventFlag( false );

#ifdef LNOTE_ENABLE_WIN32_RUNTIME_THREAD
        //mThread = (HANDLE)(::AfxBeginThread(_thr_prx::_execute_static, this));

        mThread = CreateThread(
		    NULL, //セキュリティ属性
		    0, //スタックサイズ
		    _thr_prx::_execute_static, //スレッド関数
		    this, //スレッド関数に渡す引数
		    0, //作成オプション(0またはCREATE_SUSPENDED)
		    &mThreadID);//スレッドID
#else
        mThread = reinterpret_cast< HANDLE >( _beginthreadex( NULL, 0, _thr_prx::_execute_static, this, 0, &mThreadID ) );
#endif
    }

	//---------------------------------------------------------------------
	// ● 終了待ち
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
	// ● 終了しているかどうかの判定
	//---------------------------------------------------------------------
	bool ThreadObject::isFinished()
	{
		bool res = false;
		if ( mFinished )
		{
			// 終わっている場合
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
// ■ ThreadObject クラス (pthread)
//=============================================================================
    
    class _thr_prx
    {
    public:

        static void* _execute_static( void* ptr_ )
        {
            ThreadObject* obj = static_cast< ThreadObject* >( ptr_ );

            // 実行
            ( *( obj ) )();

            // 終了フラグを立てる
            obj->mFinished->setTrue();

            return 0;
        }
    };

	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	ThreadObject::ThreadObject()
		: mFinished		( NULL )
	{
		}

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	ThreadObject::~ThreadObject()
	{
        if ( mFinished )
        {
            printf( "< ThreadObject::~ThreadObject >\n" );
            printf( "スレッドが終了していません。wait() で待機します。\n" );
            printf( "もし応答が無くなった場合はスレッドが無限ループになっていないか確認してください。" );

            wait();
        }
        //LN_ASSERT_S_EXP_( ( mFinished ), LNException::Thread, "< Core::Thread::Thread::~Thread >\n\nstart() で開始したスレッドは\n必ず wait() を呼び出して終了してください。" );
    }

    //---------------------------------------------------------------------
    // ● 実行開始
    //---------------------------------------------------------------------
    void ThreadObject::start()
    {
        // 前のが終了していない場合は待つ
        wait();

        // 終了フラグを作って開始
        mFinished = LN_NEW EventFlag( false );

        pthread_create( &mThread,  NULL, _thr_prx::_execute_static, this );
    }

    //---------------------------------------------------------------------
    // ● 終了待ち
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
    // ● 終了しているかどうかの判定
    //---------------------------------------------------------------------
    bool ThreadObject::isFinished()
    {
        bool res = false;
        if ( mFinished )
        {
            // 終わっている場合
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

// 時間待ち
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