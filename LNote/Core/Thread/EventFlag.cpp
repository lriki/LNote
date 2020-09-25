//=============================================================================
//�y EventFlag �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "EventFlag.h"

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
// �� EventFlag �N���X (Windows)
//=============================================================================
#if defined(LNOTE_WINDOWS)

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    EventFlag::EventFlag()
    	: mHandle( NULL )
    {
        mHandle = CreateEvent( NULL, TRUE, FALSE, NULL );
        ::InitializeCriticalSection( &mCS );
    }

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    EventFlag::EventFlag( bool init_flag_ )
    	: mHandle( NULL )
    {
        BOOL t = ( init_flag_ ) ? TRUE : FALSE;
        // �蓮���Z�b�g�ō쐬
        mHandle = CreateEvent( NULL, TRUE, t, NULL );

        ::InitializeCriticalSection( &mCS );
       
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    EventFlag::~EventFlag()
    {
         ::DeleteCriticalSection( &mCS );
        if ( mHandle )
        {
            ::CloseHandle( mHandle );
            mHandle = NULL;
        }
    }

    //---------------------------------------------------------------------
    // �� �l�� true �ɂ���
    //---------------------------------------------------------------------
    void EventFlag::setTrue()
    {
        if ( mHandle )
        {
            ::SetEvent( mHandle );
        }
    }

    //---------------------------------------------------------------------
    // �� �l�� false �ɂ���
    //---------------------------------------------------------------------
    void EventFlag::setFalse()
    {
        if ( mHandle )
        {
            ::ResetEvent( mHandle );
        }
    }

    //---------------------------------------------------------------------
    // �� �l�� true ���𔻒肷��
    //---------------------------------------------------------------------
    bool EventFlag::isTrue()
    {
        if ( mHandle )
        {
            return ( ::WaitForSingleObject( mHandle, 0 ) == WAIT_OBJECT_0 );
        }
        return true;
    }

    //---------------------------------------------------------------------
    // �� true �ɂȂ�܂őҋ@����
    //---------------------------------------------------------------------
    void EventFlag::wait()
    {
        if ( mHandle )
        {
#if LNOTE_ENABLE_THREAD_DEBUG
			if (::WaitForSingleObject( mHandle, 10000 ) != WAIT_OBJECT_0)
			{
				printf("EventFlag::wait() timeout");
				*reinterpret_cast< int* >( 0 ) = 0;
			}
#else
			::WaitForSingleObject( mHandle, INFINITE );
#endif
        }
    }
    
//=============================================================================
// �� EventFlag �N���X (pthread)
//=============================================================================
#else
    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    EventFlag::EventFlag( bool init_flag_ )
    {
        pthread_mutex_init( &mMutex, NULL );
        pthread_cond_init( &mWait, NULL );
        mSignal = init_flag_;
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    EventFlag::~EventFlag()
    {
        pthread_cond_destroy( &mWait );
        pthread_mutex_destroy( &mMutex );
    }

    //---------------------------------------------------------------------
    // �� �l�� true �ɂ���
    //---------------------------------------------------------------------
    void EventFlag::setTrue()
    {
        // �~���[�e�b�N�X�����b�N����
        int r = pthread_mutex_lock( &mMutex );
        if ( r != 0 )
        { 
        	errno = r; 
        	return;
        }
        
        mSignal = true;
        
        // ���f���Ă���X���b�h�ɒʒm�𑗂�
        r = pthread_cond_broadcast( &mWait );//pthread_cond_signal( &mWait );
        if ( r != 0 )
        { 
        	errno = r; 
	        return;
        }
     
        // �~���[�e�b�N�X���A�����b�N����
        r = pthread_mutex_unlock( &mMutex );
        if ( r != 0 )
        { 
        	errno = r; 
	        return;
        }
    }

    //---------------------------------------------------------------------
    // �� �l�� false �ɂ���
    //---------------------------------------------------------------------
    void EventFlag::setFalse()
    {
        pthread_mutex_lock( &mMutex );
        mSignal = false;
        pthread_mutex_unlock( &mMutex );
    }

    //---------------------------------------------------------------------
    // �� �l�� true ���𔻒肷��
    //---------------------------------------------------------------------
    bool EventFlag::isTrue()
    {
        pthread_mutex_lock( &mMutex );
        bool b = mSignal;
        pthread_mutex_unlock( &mMutex );
        return b;
    }

    //---------------------------------------------------------------------
    // �� true �ɂȂ�܂őҋ@����
    //---------------------------------------------------------------------
    void EventFlag::wait()
    {
        // �~���[�e�b�N�X�����b�N����
        int r = pthread_mutex_lock( &mMutex );
        if ( r != 0 ) { 
	errno = r; 
	return; }
        
        // true �Ȃ�A�����b�N���Ē��f���Ă݂�
        if ( mSignal )
        {
            pthread_mutex_unlock( &mMutex );
            return;
        }
        
        // �X���b�h���X���[�v������
        r = pthread_cond_wait( &mWait, &mMutex );
        if ( r != 0 ) { 
        	errno = r; 
	return; }
        
        // �A�����b�N
        pthread_mutex_unlock( &mMutex );
        if ( r != 0 ) { 
        	errno = r; 
	return; }
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