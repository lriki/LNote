//=============================================================================
//�y ReadWriteMutex �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "ReadWriteMutex.h"

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
// �� ReadWriteMutex �N���X (win32)
//=============================================================================
#if defined(LNOTE_WINDOWS)
    
    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    ReadWriteMutex::ReadWriteMutex()
        : mNoReaders    ( NULL )
        , mReaderCount  ( 0 )
    {
        ::InitializeCriticalSection( &mReaderCountLock );
        ::InitializeCriticalSection( &mWriterLock );
        
        mNoReaders = ::CreateEvent( NULL, TRUE, TRUE, NULL );
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    ReadWriteMutex::~ReadWriteMutex()
    {
        if ( mNoReaders )
        {
            ::CloseHandle( mNoReaders );
            mNoReaders = NULL;
        }
        ::DeleteCriticalSection( &mWriterLock );
        ::DeleteCriticalSection( &mReaderCountLock );
    }

    //---------------------------------------------------------------------
    // �� �ǂݎ�胍�b�N
    //---------------------------------------------------------------------
    void ReadWriteMutex::lockRead()
    {
        ::EnterCriticalSection( &mWriterLock );
        ::EnterCriticalSection( &mReaderCountLock );
        if ( ++mReaderCount == 1 )
        {
            ::ResetEvent( mNoReaders );
        }
        ::LeaveCriticalSection( &mReaderCountLock );
        ::LeaveCriticalSection( &mWriterLock );
    }

    //---------------------------------------------------------------------
    // �� �ǂݎ��A�����b�N
    //---------------------------------------------------------------------
    void ReadWriteMutex::unlockRead()
    {
        ::EnterCriticalSection( &mReaderCountLock );
        //assert( mReaderCount > 0 );
        if ( --mReaderCount == 0 )
        {
            ::SetEvent( mNoReaders );
        }
        ::LeaveCriticalSection( &mReaderCountLock );
    }
    
    //---------------------------------------------------------------------
    // �� �������݃��b�N
    //---------------------------------------------------------------------
    void ReadWriteMutex::lockWrite()
    {
        ::EnterCriticalSection( &mWriterLock );
        if ( mReaderCount > 0 )
        {
            ::WaitForSingleObject( mNoReaders, INFINITE );
        }
    }
    
    //---------------------------------------------------------------------
    // �� �������݃A�����b�N
    //---------------------------------------------------------------------
    void ReadWriteMutex::unlockWrite()
    {
        ::LeaveCriticalSection( &mWriterLock );
    }

//=============================================================================
// �� ReadWriteMutex �N���X (pthread)
//=============================================================================
#else
    
    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    ReadWriteMutex::ReadWriteMutex()
        : mReaders          ( 0 )
        , mWriters          ( 0 )
        , mReadWaiters      ( 0 )
        , mWriteWaiters     ( 0 )
    {
        pthread_mutex_init( &mLock, NULL );
        pthread_cond_init( &mRead, NULL );
        pthread_cond_init( &mWrite, NULL );
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    ReadWriteMutex::~ReadWriteMutex()
    {
        pthread_mutex_destroy( &mLock );
        pthread_cond_destroy( &mRead );
        pthread_cond_destroy( &mWrite );
    }

    //---------------------------------------------------------------------
    // �� �ǂݎ�胍�b�N
    //---------------------------------------------------------------------
    void ReadWriteMutex::lockRead()
    {
        pthread_mutex_lock( &mLock );
        if ( mWriters || mWriteWaiters )
        {
            ++mReadWaiters;
            do pthread_cond_wait( &mRead, &mLock );
            while ( mWriters || mWriteWaiters );
            --mReadWaiters;
        }
        ++mReaders;
        pthread_mutex_unlock( &mLock );
    }

    //---------------------------------------------------------------------
    // �� �ǂݎ��A�����b�N
    //---------------------------------------------------------------------
    void ReadWriteMutex::unlockRead()
    {
        pthread_mutex_lock( &mLock );
        --mReaders;
        if ( mWriteWaiters )
        {
            pthread_cond_signal( &mWrite );
        }
        pthread_mutex_unlock( &mLock );
    }
    
    //---------------------------------------------------------------------
    // �� �������݃��b�N
    //---------------------------------------------------------------------
    void ReadWriteMutex::lockWrite()
    {
        pthread_mutex_lock( &mLock );
        if ( mReaders || mWriters )
        {
            ++mWriteWaiters;
            do pthread_cond_wait( &mWrite, &mLock );
            while ( mReaders || mWriters );
            --mWriteWaiters;
        }
        mWriters = 1;
        pthread_mutex_unlock( &mLock );
    }
    
    //---------------------------------------------------------------------
    // �� �������݃A�����b�N
    //---------------------------------------------------------------------
    void ReadWriteMutex::unlockWrite()
    {
        pthread_mutex_lock( &mLock );
        mWriters = 0;
        if ( mWriteWaiters )
        {
            pthread_cond_signal( &mWrite );
        }
        else if ( mReadWaiters )
        {
            pthread_cond_broadcast( &mRead );
        }
        pthread_mutex_unlock( &mLock );
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