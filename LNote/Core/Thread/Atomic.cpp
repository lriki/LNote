//=============================================================================
//�y Atomic �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Atomic.h"

#ifdef LNOTE_USE_BOOST
    #include <boost/interprocess/detail/atomic.hpp>
#endif

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
// �� Atomic �N���X (pthread)
//=============================================================================

#if defined(LNOTE_GNUC)
    
    // __sync_fetch_and_add �ł��ł��邯�ǁA���ˑ�������ۂ��B
    // http://fallabs.com/blog-ja/promenade.cgi?id=68
    // �Ƃ肠�������͊m���ȃ~���[�e�b�N�X�ŁB
    
    // �R���X�g���N�^
    Atomic::Atomic( lnS32 value_ )
        : mValue( value_ )
    {
        pthread_mutex_init( &mMutex, NULL );
    }

	// �f�X�g���N�^
    Atomic::~Atomic()
    {
        pthread_mutex_destroy( &mMutex );
    }

    // �l���擾����
	lnS32 Atomic::get()
    {
		pthread_mutex_lock( &mMutex );
        lnS32 v = mValue;
        pthread_mutex_unlock( &mMutex );
        return v;
    }

    // �l��ݒ肷��
    void Atomic::set( lnS32 value_ )
    {
        pthread_mutex_lock( &mMutex );
        mValue = value_;
        pthread_mutex_unlock( &mMutex );
    }

    // �l���C���N�������g����
    lnS32 Atomic::increment()
    {
        pthread_mutex_lock( &mMutex );
        ++mValue;
        lnS32 v = mValue;
        pthread_mutex_unlock( &mMutex );
        return v;
    }

    // �l���f�N�������g����
    lnS32 Atomic::decrement()
    {
        pthread_mutex_lock( &mMutex );
        --mValue;
        lnS32 v = mValue;
        pthread_mutex_unlock( &mMutex );
        return v;
    }

#else
    
//=============================================================================
// �� Atomic �N���X (win32)
//=============================================================================
    
    // �R���X�g���N�^
    Atomic::Atomic( lnS32 value_ )
        : mValue( value_ )
    {
    }

	// �f�X�g���N�^
    Atomic::~Atomic()
    {
    }
    
	// �l���擾����
	lnS32 Atomic::get()
    {
        return ::InterlockedExchangeAdd( const_cast< volatile lnS32* >( &mValue ), 0 );
    }

    // �l��ݒ肷��
    void Atomic::set( lnS32 value_ )
    {
	    ::InterlockedExchange( &mValue, value_ );
    }

    // �l���C���N�������g����
    lnS32 Atomic::increment()
    {
	    return ::InterlockedIncrement( &mValue );
    }

    // �l���f�N�������g����
    lnS32 Atomic::decrement()
    {
	    return ::InterlockedDecrement( &mValue );
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