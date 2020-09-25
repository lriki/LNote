//=============================================================================
//【 Barrier 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Barrier.h"

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
// ■ Barrier クラス (win32)
//=============================================================================
#if defined(LNOTE_WINDOWS)

#if LNOTE_ENABLE_THREAD_DEBUG
	#define DEBUG_WaitForSingleObject( h_, str_ ) \
		if (::WaitForSingleObject( h_, 10000 ) != WAIT_OBJECT_0) \
		{ \
			printf(str_); \
			*reinterpret_cast< int* >( 0 ) = 0; \
		}
#else
	#define DEBUG_WaitForSingleObject( h_, str_ ) ::WaitForSingleObject( h_, INFINITE );
#endif




    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    Barrier::Barrier( int max_num_ )
        : mNotifyFlag	( false )
		, mAllLeaved	( true )
    {
        mMaxNum = ( max_num_ < 1 ) ? 1 : max_num_;    // 最低 1
        //mWaitThreadNum.set( mMaxNum );
        mWaitThreadNum = mMaxNum;

        mFirstThreadSignal = ::CreateEvent( NULL, TRUE, FALSE, NULL );
        mSignalWasDone     = ::CreateEvent( NULL, TRUE, FALSE, NULL );
        
        ::InitializeCriticalSection( &mCS );
        ::InitializeCriticalSection( &mThreadNumCS );

    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    Barrier::~Barrier()
    {
        ::DeleteCriticalSection( &mThreadNumCS );
        ::DeleteCriticalSection( &mCS );

        if ( mFirstThreadSignal )
        {
            ::CloseHandle( mFirstThreadSignal );
            mFirstThreadSignal = NULL;
        }
        if ( mSignalWasDone )
        {
            ::CloseHandle( mSignalWasDone );
            mSignalWasDone = NULL;
        }
    }

    //---------------------------------------------------------------------
    // ● 他のスレッドを待つ
    //---------------------------------------------------------------------
    void Barrier::wait()
    {
        if ( mNotifyFlag.isTrue() )
        {
            return;
        }
        //printf( "▼ in wait(%d)\n", id_ );

        // かならず2つ以上のスレッドが待たないとだめ



		mAllLeaved.wait();

//printf("wait\n");
        mExternalMutex.lock();
//printf("reav\n");
        //--mWaitThreadNum;
        //mWaitThreadNum.decrement();


        //printf( "%d\n", mWaitThreadNum );

        // notify_all

        // 最後のスレッド待ち状態になろうとしている時 (すべて開始する)
        if ( mWaitThreadNum == 1 )
        {
            //printf( "rd... %d\n", id_ );

            //mExternalMutex.unlock();

			mAllLeaved.setFalse();

            ::ResetEvent( mSignalWasDone );
            ::SetEvent( mFirstThreadSignal );   // 最初に待ってるスレッドを再開
            
            //::WaitForSingleObject( mSignalWasDone, INFINITE );   // すべての待機スレッドが開始するまで待つ
			DEBUG_WaitForSingleObject(mSignalWasDone, "Barrier::wait() 1 timeout");

            ::ResetEvent( mFirstThreadSignal );
            //mExternalMutex.lock();

			mAllLeaved.setTrue();

        }
        else
        {
            --mWaitThreadNum;
            mExternalMutex.unlock();

            ::EnterCriticalSection( &mCS ); // 最初のひとつ以外はここで待つ
            //::WaitForSingleObject( mFirstThreadSignal, INFINITE );    // 最初のひとつはここで待つ
			DEBUG_WaitForSingleObject(mFirstThreadSignal, "Barrier::wait() 2 timeout");
            ++mWaitThreadNum;
            
            ::LeaveCriticalSection( &mCS );

            if ( mWaitThreadNum == mMaxNum )
            {
                ::SetEvent( mSignalWasDone );
            }
            
            // 一度開始しても、notify_all の部分を呼んだスレッド (最後のスレッド) が
            // ↓の unlock() を呼ぶまではここで待機する

            mExternalMutex.lock();

            //printf( "go %d\n", id_ );
        }

        

        mExternalMutex.unlock();
        
        //printf( "▲ end wait(%d)\n", id_ );
    }

    //---------------------------------------------------------------------
    // ● 同期の ON/OFF を設定する
    //---------------------------------------------------------------------
    void Barrier::setNotifyFlag( bool flag_ )
    {
        if ( flag_ )
        {
            mNotifyFlag.setTrue();

            // 待機スレッドがある場合はすべて開始してから自分を開始する
            if ( mWaitThreadNum != mMaxNum )
            {
                ::ResetEvent( mSignalWasDone );
                ::SetEvent( mFirstThreadSignal );
                //::WaitForSingleObject( mSignalWasDone, INFINITE );
				DEBUG_WaitForSingleObject(mSignalWasDone, "Barrier::setNotifyFlag() timeout");
                ::ResetEvent( mFirstThreadSignal );
            }
        }
        else
        {
            mNotifyFlag.setFalse();
        }
    }

//=============================================================================
// ■ Barrier クラス (pthread)
//=============================================================================
#else
    
    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    Barrier::Barrier( int max_num_ )
        : mNotifyFlag( false )
    {
        mMaxNum = ( max_num_ < 1 ) ? 1 : max_num_;    // 最低 1
        mWaitThreadNum = 0;
        
        pthread_mutex_init( &mMutex, NULL );
        pthread_cond_init( &mCond, NULL );
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    Barrier::~Barrier()
    {
        pthread_mutex_init( &mMutex, NULL );
        pthread_cond_init( &mCond, NULL );
    }

    //---------------------------------------------------------------------
    // ● 他のスレッドを待つ
    //---------------------------------------------------------------------
    void Barrier::wait()
    {
        pthread_mutex_lock( &mMutex );
        
        ++mWaitThreadNum;
        
        // 最後のスレッドが来たら全部再開
        if ( mWaitThreadNum == mMaxNum )
        {
            pthread_cond_broadcast( &mCond );
        }
        else
        {
            // ここで mutex はアンロックされる。
            // pthread_cond_broadcast() が呼ばれて cond が使用可能になると
            // mutex を再びロックして再開する。
            pthread_cond_wait( &mCond, &mMutex );
        }
        
        --mWaitThreadNum;
        
        pthread_mutex_unlock( &mMutex );
        
    }
    
    //---------------------------------------------------------------------
    // ● 同期の ON/OFF を設定する
    //---------------------------------------------------------------------
    void Barrier::setNotifyFlag( bool flag_ )
    {
        if ( flag_ )
        {
            mNotifyFlag.setTrue();
            pthread_cond_broadcast( &mCond );
        }
        else
        {
            mNotifyFlag.setFalse();
        }
    }

    //---------------------------------------------------------------------
    // ● 待機状態のスレッドをすべて実行状態にする
    //---------------------------------------------------------------------
    /*
    void Barrier::notify()
    {
        
        
        pthread_mutex_lock( &mMutex );
        pthread_cond_broadcast( &mCond );
        //mWaitThreadNum = 0;
        pthread_mutex_unlock( &mMutex );
    }
    */

#endif


//=============================================================================
// ■ EventBarrier クラス
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    EventBarrier::EventBarrier( int max_sub_num_ )
        : mMaxWaitingSubNum ( max_sub_num_ )
        , mWaitingSubNum    ( 0 )
        , mBarrier          ( max_sub_num_ + 1 )
        , mMainNotifyFlag   ( true )
		, mAllLeaved		( true )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    EventBarrier::~EventBarrier()
    {
    }
	
    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void EventBarrier::waitMain()
    {
        mMainNotifyFlag.wait();
        mMainNotifyFlag.setFalse();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void EventBarrier::waitSub()
    {
		mAllLeaved.wait();

		mMutex.lock(); // mWaitingSubNum.increment() と次のifまでに再びincrementされないように備える
        mWaitingSubNum.increment();
        if ( mWaitingSubNum.get() == mMaxWaitingSubNum )
        {
			mMutex.unlock();
			mAllLeaved.setFalse();
            mMainNotifyFlag.setTrue();  // メインスレッド実行
			
        }
		else
		{
			mMutex.unlock();
		}
        mBarrier.wait();

		mMutex.lock();
        mWaitingSubNum.decrement();
		if ( mWaitingSubNum.get() == 0 )
        {
			mAllLeaved.setTrue();
		}
		mMutex.unlock();

		// ひとつだけ先行してこの関数から出て、
		// 他のスレッドがこの関数から出る前に
		// もう一度戻ってきて increment された場合…
		//		メインスレッドが waitMain() でサブスレッドを
		//		すべて待つ前に処理を開始してしまう。

    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void EventBarrier::notifyAllSub()
    {
        mBarrier.wait();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void EventBarrier::setNotifyFlag( bool flag_ )
    {
        mBarrier.setNotifyFlag( flag_ );
        if ( flag_ )
        {
            mMainNotifyFlag.setTrue();
        }
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