//=============================================================================
//Åy Delegate Åz
//-----------------------------------------------------------------------------
///**
//  @file       Delegate.h
//  @brief      Delegate
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "STLUtil.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Base
{

//=============================================================================
// Å° Delegate00
//=============================================================================
class Delegate00
{
private:

    /// DelegateHolderBase
	class DelegateHolderBase
		: public Base::ReferenceObject
	{
	public:

		virtual void call() const = 0;
	};

    // DelegateHolderStatic
	class DelegateHolderStatic
		: public DelegateHolderBase
	{
	private:

		typedef void (LNOTE_STDCALL *StaticFunction)();

	private:

		StaticFunction	mFunction;

	public:

		DelegateHolderStatic( StaticFunction function )
			: mFunction	( function )
        {}

		virtual void call() const { mFunction(); }
	};

    /// DelegateHolderDynamics
	template < typename T >
	class DelegateHolderDynamics
		: public DelegateHolderBase
	{
	private:

		typedef void (T::*ClassMethod)();

	private:

		T*			mClass;
		ClassMethod	mMethod;

	public:

		DelegateHolderDynamics( T* class_, ClassMethod method_ )
			: mClass		( class_ )
			, mMethod		( method_ )
		{}

		virtual void call() const { (mClass->*mMethod)(); }
	};

private:

	DelegateHolderBase* mDelegate;

public:

	Delegate00()
		: mDelegate ( NULL )
	{}

	Delegate00( const Delegate00& d_ )
		: mDelegate ( NULL )
	{
		*this = d_;
	}

    Delegate00( void (LNOTE_STDCALL *method_)() )
		: mDelegate ( new DelegateHolderStatic( method_ ) )
	{}

	template < typename T >
	Delegate00( T* class_, void (T::*method_)() )
		: mDelegate ( new DelegateHolderDynamics<T>( class_, method_ ) )
	{}

	~Delegate00() { releaseMethod(); }

public:

	void releaseMethod() { LN_SAFE_RELEASE( mDelegate ); }

	bool isEmpty() const { return mDelegate == NULL; }

	void call() const { if ( mDelegate != NULL ) mDelegate->call(); }

	void operator ()() const { call(); }

	Delegate00& operator = ( const Delegate00& d_ )
	{
        LN_SAFE_ADDREF( d_.mDelegate );
		releaseMethod();
		mDelegate = d_.mDelegate;
		return *this;
	}
};

/// createDelegate
template < typename T >
Delegate00 createDelegate( T* pClass, void (T::*pFunction)() )
{
	return Delegate00( pClass, pFunction );
}

//=============================================================================
// Å° Delegate xx
//=============================================================================
#define LN_DELEGATE_CLASS_NAME		Delegate01
#define LN_DELEGATE_TEMPLATE_ARGS 	typename A1
#define LN_DELEGATE_ARGS_DECL 		A1 a1
#define LN_DELEGATE_CALL_ARGS 		a1
#define LN_DELEGATE_TEMPLATE_TYPES  A1
#include "Delegate.inc"

#define LN_DELEGATE_CLASS_NAME		Delegate02
#define LN_DELEGATE_TEMPLATE_ARGS 	typename A1, typename A2
#define LN_DELEGATE_ARGS_DECL 		A1 a1, A2 a2
#define LN_DELEGATE_CALL_ARGS 		a1, a2
#define LN_DELEGATE_TEMPLATE_TYPES  A1, A2
#include "Delegate.inc"

#define LN_DELEGATE_CLASS_NAME		Delegate03
#define LN_DELEGATE_TEMPLATE_ARGS 	typename A1, typename A2, typename A3
#define LN_DELEGATE_ARGS_DECL 		A1 a1, A2 a2, A3 a3
#define LN_DELEGATE_CALL_ARGS 		a1, a2, a3
#define LN_DELEGATE_TEMPLATE_TYPES  A1, A2, A3
#include "Delegate.inc"

#define LN_DELEGATE_CLASS_NAME		Delegate04
#define LN_DELEGATE_TEMPLATE_ARGS 	typename A1, typename A2, typename A3, typename A4
#define LN_DELEGATE_ARGS_DECL 		A1 a1, A2 a2, A3 a3, A4 a4
#define LN_DELEGATE_CALL_ARGS 		a1, a2, a3, a4
#define LN_DELEGATE_TEMPLATE_TYPES  A1, A2, A3, A4
#include "Delegate.inc"

#define LN_DELEGATE_CLASS_NAME		Delegate05
#define LN_DELEGATE_TEMPLATE_ARGS 	typename A1, typename A2, typename A3, typename A4, typename A5
#define LN_DELEGATE_ARGS_DECL 		A1 a1, A2 a2, A3 a3, A4 a4, A5 a5
#define LN_DELEGATE_CALL_ARGS 		a1, a2, a3, a4, a5
#define LN_DELEGATE_TEMPLATE_TYPES  A1, A2, A3, A4, A5
#include "Delegate.inc"

#define LN_DELEGATE_CLASS_NAME		Delegate06
#define LN_DELEGATE_TEMPLATE_ARGS 	typename A1, typename A2, typename A3, typename A4, typename A5, typename A6
#define LN_DELEGATE_ARGS_DECL 		A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6
#define LN_DELEGATE_CALL_ARGS 		a1, a2, a3, a4, a5, a6
#define LN_DELEGATE_TEMPLATE_TYPES  A1, A2, A3, A4, A5, A6
#include "Delegate.inc"

#define LN_DELEGATE_CLASS_NAME		Delegate07
#define LN_DELEGATE_TEMPLATE_ARGS 	typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7
#define LN_DELEGATE_ARGS_DECL 		A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7
#define LN_DELEGATE_CALL_ARGS 		a1, a2, a3, a4, a5, a6, a7
#define LN_DELEGATE_TEMPLATE_TYPES  A1, A2, A3, A4, A5, A6, A7
#include "Delegate.inc"

#define LN_DELEGATE_CLASS_NAME		Delegate08
#define LN_DELEGATE_TEMPLATE_ARGS 	typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8
#define LN_DELEGATE_ARGS_DECL 		A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8
#define LN_DELEGATE_CALL_ARGS 		a1, a2, a3, a4, a5, a6, a7, a8
#define LN_DELEGATE_TEMPLATE_TYPES  A1, A2, A3, A4, A5, A6, A7, A8
#include "Delegate.inc"

//=============================================================================
// Å° Event01 (ñv)
//=============================================================================
template < typename A1 >
class Event01
{
public:

    typedef Delegate01< A1 > DelegateType;

public:

    void operator += ( DelegateType delegate_ )
    {
        mDelegateArray.push_back();
    }

    void operator -= ( DelegateType delegate_ )
    {
        STLUtil::remove( mDelegateArray, delegate_ );
    }

    void call( A1 a1 )
	{
        if ( !mDelegateArray.empty() )
		{
            ln_foreach( DelegateType& d, mDelegateArray )
            {
                d.call( a1 );
            }
		}
	}

    void removeClass( void* ptr_ )
    {
        STLUtil::remove( mDelegateArray, ptr_ );
    }

private:

    typedef std::vector< DelegateType > DelegateArray;

    DelegateArray mDelegateArray;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================