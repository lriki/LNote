//=============================================================================
//【 Semaphore 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Semaphore.h"

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
// ■ Semaphore クラス (win32)
//=============================================================================
#if defined(LNOTE_WINDOWS)
    
	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	Semaphore::Semaphore( int start_count_, int max_count_ )
		: mHandle( NULL )
		, mMaxCount( 0 )
	{
		//LN_ASSERT_S_( ( start_count_ < 0 ), "< Core::Thread::Semaphore::Semaphore >\n\nstart_count_ < 0" );
		//LN_ASSERT_S_( ( max_count_   < 1 ), "< Core::Thread::Semaphore::Semaphore >\n\nstart_count_ < 1" );
		//LN_ASSERT_S_( ( start_count_ > max_count_ ), "< Core::Thread::Semaphore::Semaphore >\n\nstart_count_ > max_count_\n( %d > %d )", start_count_, max_count_ );
	
		mMaxCount = max_count_;
		mHandle = ::CreateSemaphore( NULL, start_count_, max_count_, NULL );
	}

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	Semaphore::~Semaphore()
	{
		if ( mHandle )
		{
			::CloseHandle( mHandle );
		}
	}

	//---------------------------------------------------------------------
	// ● ロック
	//---------------------------------------------------------------------
	void Semaphore::decrease()
	{
		if ( mHandle )
		{
#if LNOTE_ENABLE_THREAD_DEBUG
			if (::WaitForSingleObject( mHandle, 10000 ) != WAIT_OBJECT_0)
			{
				printf("Semaphore::decrease() timeout");
				*reinterpret_cast< int* >( 0 ) = 0;
			}
#else
			::WaitForSingleObject( mHandle, INFINITE );
#endif
		}
	}

	//---------------------------------------------------------------------
	// ● アンロック
	//---------------------------------------------------------------------
	void Semaphore::increase()
	{
		if ( mHandle )
		{
			LONG prev;
			::ReleaseSemaphore( mHandle, 1, &prev );

			//LN_ASSERT_S_EXP_( ( prev + 1 > mMaxCount ), LNException::Thread, "< Core::Thread::Semaphore::increase >\n\nReleaseSemaphore" );
		}
	}

    //---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    bool Semaphore::decrease( lnU32 ms_ )
    {
        if ( mHandle )
		{
			return (::WaitForSingleObject( mHandle, ms_ ) == WAIT_OBJECT_0);
		}
        return false;
    }

    
//=============================================================================
// ■ Semaphore クラス (pthread)
//=============================================================================
#else
    
    //---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	Semaphore::Semaphore( int start_count_, int max_count_ )
		: mValue    ( start_count_ )
        , mMaxCount ( max_count_ )
	{
        pthread_mutex_init( &mMutex, 0 );
        pthread_cond_init( &mCondition, 0 );
	}

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	Semaphore::~Semaphore()
	{
        pthread_cond_destroy( &mCondition );
        pthread_mutex_destroy( &mMutex );
	}

    //---------------------------------------------------------------------
    // ● ロック
    //---------------------------------------------------------------------
    void Semaphore::decrease()
    {
        pthread_mutex_lock( &mMutex );
        while ( mValue < 1 )
        {
            pthread_cond_wait( &mCondition, &mMutex );
        }
        mValue -= 1;
        if ( mValue < 0 )
        {
            mValue = 0;
        }
        pthread_cond_broadcast( &mCondition );
        pthread_mutex_unlock( &mMutex );
    }

    //---------------------------------------------------------------------
    // ● アンロック
    //---------------------------------------------------------------------
    void Semaphore::increase()
    {
        pthread_mutex_lock( &mMutex );
        mValue += 1;
        if ( mValue > mMaxCount )
        {
            mValue = mMaxCount;
        }
        pthread_cond_broadcast( &mCondition );
        pthread_mutex_unlock( &mMutex );
    }
    
#endif
    
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Thread
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================