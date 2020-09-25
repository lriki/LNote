//=============================================================================
//【 Atomic 】
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
// ■ Atomic クラス (pthread)
//=============================================================================

#if defined(LNOTE_GNUC)
    
    // __sync_fetch_and_add でもできるけど、環境依存あるっぽい。
    // http://fallabs.com/blog-ja/promenade.cgi?id=68
    // とりあえず今は確実なミューテックスで。
    
    // コンストラクタ
    Atomic::Atomic( lnS32 value_ )
        : mValue( value_ )
    {
        pthread_mutex_init( &mMutex, NULL );
    }

	// デストラクタ
    Atomic::~Atomic()
    {
        pthread_mutex_destroy( &mMutex );
    }

    // 値を取得する
	lnS32 Atomic::get()
    {
		pthread_mutex_lock( &mMutex );
        lnS32 v = mValue;
        pthread_mutex_unlock( &mMutex );
        return v;
    }

    // 値を設定する
    void Atomic::set( lnS32 value_ )
    {
        pthread_mutex_lock( &mMutex );
        mValue = value_;
        pthread_mutex_unlock( &mMutex );
    }

    // 値をインクリメントする
    lnS32 Atomic::increment()
    {
        pthread_mutex_lock( &mMutex );
        ++mValue;
        lnS32 v = mValue;
        pthread_mutex_unlock( &mMutex );
        return v;
    }

    // 値をデクリメントする
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
// ■ Atomic クラス (win32)
//=============================================================================
    
    // コンストラクタ
    Atomic::Atomic( lnS32 value_ )
        : mValue( value_ )
    {
    }

	// デストラクタ
    Atomic::~Atomic()
    {
    }
    
	// 値を取得する
	lnS32 Atomic::get()
    {
        return ::InterlockedExchangeAdd( const_cast< volatile lnS32* >( &mValue ), 0 );
    }

    // 値を設定する
    void Atomic::set( lnS32 value_ )
    {
	    ::InterlockedExchange( &mValue, value_ );
    }

    // 値をインクリメントする
    lnS32 Atomic::increment()
    {
	    return ::InterlockedIncrement( &mValue );
    }

    // 値をデクリメントする
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