//=============================================================================
//�y Barrier �z
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
// �� Barrier �N���X (win32)
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
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    Barrier::Barrier( int max_num_ )
        : mNotifyFlag	( false )
		, mAllLeaved	( true )
    {
        mMaxNum = ( max_num_ < 1 ) ? 1 : max_num_;    // �Œ� 1
        //mWaitThreadNum.set( mMaxNum );
        mWaitThreadNum = mMaxNum;

        mFirstThreadSignal = ::CreateEvent( NULL, TRUE, FALSE, NULL );
        mSignalWasDone     = ::CreateEvent( NULL, TRUE, FALSE, NULL );
        
        ::InitializeCriticalSection( &mCS );
        ::InitializeCriticalSection( &mThreadNumCS );

    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
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
    // �� ���̃X���b�h��҂�
    //---------------------------------------------------------------------
    void Barrier::wait()
    {
        if ( mNotifyFlag.isTrue() )
        {
            return;
        }
        //printf( "�� in wait(%d)\n", id_ );

        // ���Ȃ炸2�ȏ�̃X���b�h���҂��Ȃ��Ƃ���



		mAllLeaved.wait();

//printf("wait\n");
        mExternalMutex.lock();
//printf("reav\n");
        //--mWaitThreadNum;
        //mWaitThreadNum.decrement();


        //printf( "%d\n", mWaitThreadNum );

        // notify_all

        // �Ō�̃X���b�h�҂���ԂɂȂ낤�Ƃ��Ă��鎞 (���ׂĊJ�n����)
        if ( mWaitThreadNum == 1 )
        {
            //printf( "rd... %d\n", id_ );

            //mExternalMutex.unlock();

			mAllLeaved.setFalse();

            ::ResetEvent( mSignalWasDone );
            ::SetEvent( mFirstThreadSignal );   // �ŏ��ɑ҂��Ă�X���b�h���ĊJ
            
            //::WaitForSingleObject( mSignalWasDone, INFINITE );   // ���ׂĂ̑ҋ@�X���b�h���J�n����܂ő҂�
			DEBUG_WaitForSingleObject(mSignalWasDone, "Barrier::wait() 1 timeout");

            ::ResetEvent( mFirstThreadSignal );
            //mExternalMutex.lock();

			mAllLeaved.setTrue();

        }
        else
        {
            --mWaitThreadNum;
            mExternalMutex.unlock();

            ::EnterCriticalSection( &mCS ); // �ŏ��̂ЂƂȊO�͂����ő҂�
            //::WaitForSingleObject( mFirstThreadSignal, INFINITE );    // �ŏ��̂ЂƂ͂����ő҂�
			DEBUG_WaitForSingleObject(mFirstThreadSignal, "Barrier::wait() 2 timeout");
            ++mWaitThreadNum;
            
            ::LeaveCriticalSection( &mCS );

            if ( mWaitThreadNum == mMaxNum )
            {
                ::SetEvent( mSignalWasDone );
            }
            
            // ��x�J�n���Ă��Anotify_all �̕������Ă񂾃X���b�h (�Ō�̃X���b�h) ��
            // ���� unlock() ���ĂԂ܂ł͂����őҋ@����

            mExternalMutex.lock();

            //printf( "go %d\n", id_ );
        }

        

        mExternalMutex.unlock();
        
        //printf( "�� end wait(%d)\n", id_ );
    }

    //---------------------------------------------------------------------
    // �� ������ ON/OFF ��ݒ肷��
    //---------------------------------------------------------------------
    void Barrier::setNotifyFlag( bool flag_ )
    {
        if ( flag_ )
        {
            mNotifyFlag.setTrue();

            // �ҋ@�X���b�h������ꍇ�͂��ׂĊJ�n���Ă��玩�����J�n����
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
// �� Barrier �N���X (pthread)
//=============================================================================
#else
    
    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    Barrier::Barrier( int max_num_ )
        : mNotifyFlag( false )
    {
        mMaxNum = ( max_num_ < 1 ) ? 1 : max_num_;    // �Œ� 1
        mWaitThreadNum = 0;
        
        pthread_mutex_init( &mMutex, NULL );
        pthread_cond_init( &mCond, NULL );
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    Barrier::~Barrier()
    {
        pthread_mutex_init( &mMutex, NULL );
        pthread_cond_init( &mCond, NULL );
    }

    //---------------------------------------------------------------------
    // �� ���̃X���b�h��҂�
    //---------------------------------------------------------------------
    void Barrier::wait()
    {
        pthread_mutex_lock( &mMutex );
        
        ++mWaitThreadNum;
        
        // �Ō�̃X���b�h��������S���ĊJ
        if ( mWaitThreadNum == mMaxNum )
        {
            pthread_cond_broadcast( &mCond );
        }
        else
        {
            // ������ mutex �̓A�����b�N�����B
            // pthread_cond_broadcast() ���Ă΂�� cond ���g�p�\�ɂȂ��
            // mutex ���Ăу��b�N���čĊJ����B
            pthread_cond_wait( &mCond, &mMutex );
        }
        
        --mWaitThreadNum;
        
        pthread_mutex_unlock( &mMutex );
        
    }
    
    //---------------------------------------------------------------------
    // �� ������ ON/OFF ��ݒ肷��
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
    // �� �ҋ@��Ԃ̃X���b�h�����ׂĎ��s��Ԃɂ���
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
// �� EventBarrier �N���X
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

		mMutex.lock(); // mWaitingSubNum.increment() �Ǝ���if�܂łɍĂ�increment����Ȃ��悤�ɔ�����
        mWaitingSubNum.increment();
        if ( mWaitingSubNum.get() == mMaxWaitingSubNum )
        {
			mMutex.unlock();
			mAllLeaved.setFalse();
            mMainNotifyFlag.setTrue();  // ���C���X���b�h���s
			
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

		// �ЂƂ�����s���Ă��̊֐�����o�āA
		// ���̃X���b�h�����̊֐�����o��O��
		// ������x�߂��Ă��� increment ���ꂽ�ꍇ�c
		//		���C���X���b�h�� waitMain() �ŃT�u�X���b�h��
		//		���ׂđ҂O�ɏ������J�n���Ă��܂��B

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