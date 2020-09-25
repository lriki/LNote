//=============================================================================
// Delegate
//=============================================================================

template < LN_DELEGATE_TEMPLATE_ARGS >
class LN_DELEGATE_CLASS_NAME
{
private:
	class DelegateHolderBase
		: public Base::ReferenceObject
	{
	public:
		virtual void call( LN_DELEGATE_ARGS_DECL ) const = 0;
	};

	class DelegateHolderStatic
		: public DelegateHolderBase
	{
	private:
		typedef void (LNOTE_STDCALL *StaticFunction)( LN_DELEGATE_TEMPLATE_TYPES );

	private:
		StaticFunction	mFunction;

	public:
		DelegateHolderStatic( StaticFunction function )
			: mFunction	( function )
		{
		}

		virtual void call( LN_DELEGATE_ARGS_DECL ) const
		{
			mFunction( LN_DELEGATE_CALL_ARGS );
		}
	};

	template < typename T >
	class DelegateHolderDynamics
		: public DelegateHolderBase
	{
	private:
		typedef void (T::*ClassMethod)( LN_DELEGATE_ARGS_DECL );

	private:
		T*			mClass;
		ClassMethod	mMethod;

	public:
		DelegateHolderDynamics( T* class_ptr, ClassMethod method )
			: mClass		( class_ptr )
			, mMethod		( method )
		{
		}

		virtual void call( LN_DELEGATE_ARGS_DECL ) const
		{
			(mClass->*mMethod)( LN_DELEGATE_CALL_ARGS );
		}
	};

private:
	DelegateHolderBase*		mDelegate;

public:
	LN_DELEGATE_CLASS_NAME()
		: mDelegate	( NULL )
	{
	}

	LN_DELEGATE_CLASS_NAME( const LN_DELEGATE_CLASS_NAME& value )
		: mDelegate	( NULL )
	{
		*this = value;
	}

	template < typename T >
	LN_DELEGATE_CLASS_NAME( T* class_ptr, void (T::*method)(LN_DELEGATE_ARGS_DECL) )
		: mDelegate	( LN_NEW DelegateHolderDynamics<T>( class_ptr, method ) )
	{
	}

	LN_DELEGATE_CLASS_NAME( void (LNOTE_STDCALL *function)(LN_DELEGATE_ARGS_DECL) )
		: mDelegate ( LN_NEW DelegateHolderStatic( function ) )
	{
	}

	~LN_DELEGATE_CLASS_NAME()
	{
		reset();
	}

public:
	void reset()
	{
		SAFE_RELEASE( mDelegate );
	}

	bool isEmpty() const
	{
		return mDelegate == NULL;
	}

	void call( LN_DELEGATE_ARGS_DECL ) const
	{
		if ( mDelegate != NULL )
		{
			mDelegate->call( LN_DELEGATE_CALL_ARGS );
		}
	}

	void operator ()( LN_DELEGATE_ARGS_DECL ) const
	{
		call( LN_DELEGATE_CALL_ARGS );
	}

	LN_DELEGATE_CLASS_NAME& operator = ( const LN_DELEGATE_CLASS_NAME& value )
	{
		reset();
		mDelegate = value.mDelegate;
		LN_SAFE_ADDREF( mDelegate );

		return *this;
	}
};

template < typename T, LN_DELEGATE_TEMPLATE_ARGS >
LN_DELEGATE_CLASS_NAME< LN_DELEGATE_TEMPLATE_TYPES > createDelegate( T* class_, void (T::*function_)(LN_DELEGATE_TEMPLATE_TYPES) )
{
	return LN_DELEGATE_CLASS_NAME< LN_DELEGATE_TEMPLATE_TYPES >( class_, function_ );
}

#undef LN_DELEGATE_CLASS_NAME
#undef LN_DELEGATE_TEMPLATE_ARGS
#undef LN_DELEGATE_ARGS_DECL
#undef LN_DELEGATE_CALL_ARGS
#undef LN_DELEGATE_TEMPLATE_TYPES

//=============================================================================
//
//=============================================================================