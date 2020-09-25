//=============================================================================
//【 RefPtr 】
//-----------------------------------------------------------------------------
///**
//  @file       RefPtr.h
//  @brief      RefPtr
//  @version    Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "ReferenceObject.h"
#include "Exception.h"

#define LRefPtr ::LNote::Core::Base::RefPtr

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Base
{
#define LN_REFPTR_ASSERT    LN_THROW_InvalidCall

//=============================================================================
// ■ RefPtr
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
template <class T> 
class RefPtr
{
public:

    // デフォルトコンストラクタ
    RefPtr()
        : p( NULL )
    {}

    // コンストラクタ
    explicit RefPtr( T* p_, bool add_ref_ = false )
        : p( p_ )
    {
        if ( p && add_ref_ ) p->addRef();
    }

    // コピーコンストラクタ
	explicit RefPtr( const RefPtr<T>& obj_ )
        : p( obj_.p )
    {
	    if ( p ) p->addRef();
    }

    // デストラクタ
    ~RefPtr()
	{
		release();
	}

public:

    // 関連付け
    void attach( T* p_ )
    { 
        release();
        p = p_; 
    }

    // convert
    operator T* () const { return p; }

    // =
    T* operator = ( const T* p_ )
    {
        LN_SAFE_ADDREF( p_ );
        release();
	    p = p_;
	    return p;
    }

    // *
    T& operator* () const
    {
        LN_REFPTR_ASSERT( p != NULL );
        return *p;
    }

    // ->
    T* operator -> () const
    {
        LN_REFPTR_ASSERT( p != NULL );
        return p;//(T*)this;
    }

    // !
    bool operator ! () const { return ( p == NULL ); }
    
    // == 
    bool operator == ( const T* p_ ) const { return ( p == p_ ); }

    // !=
    bool operator != ( const T* p_ ) const { return ( p != p_ ); }

    // < (for STL cmp)
    bool operator < ( const T* p_ ) const { return p < p_; }

public:

    void release() { LN_SAFE_RELEASE( p ); }

private:

    T* p;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================