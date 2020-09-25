//=============================================================================
//【 LRefClass 】
//-----------------------------------------------------------------------------
///**
//  @file       LRefClass.h
//  @brief      LRefClass
//  @version    1.0
//  @date       2010/10/06
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// ■ LRefClass クラス
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
template < class TYPE_, class IMPL_TYPE_ > class LRefClass
{
public:

    LRefClass()
        : _impl( NULL )
    {}

	LRefClass( lnNullPtr nil_ )
        : _impl( NULL )
    {}

	LRefClass( const LRefClass& obj_ )
        : _impl( NULL )
    {
        _impl = obj_._impl;

	    if ( _impl )
	    {
		    _impl->addRef();
	    }
    }

	virtual ~LRefClass()
    {
        release();
    }

	LRefClass& operator = ( const LRefClass& obj_ )
    {
	    // コピー元と同じ場合は何もしない
	    if ( _impl == obj_._impl ) { return *this; }

	    // まずはカウンタ減算
	    release();

	    _impl = obj_._impl;

	    // ちゃんと本体がある場合はカウンタを増やす
	    if ( _impl )
	    {
		    _impl->addRef();
	    }

	    return *this;
    }

    bool operator == ( const LRefClass& obj_ ) const
    {
	    return ( _impl == obj_._impl );
    }

	bool operator != ( const LRefClass& obj_ ) const
    {
	    return ( _impl != obj_._impl );
    }

	TYPE_& operator = ( const lnNullPtr null_ ) { release(); return *this; };
	bool operator == ( const lnNullPtr null_ ) const { return ( _impl == NULL ); }
	bool operator != ( const lnNullPtr null_ ) const { return ( _impl != NULL ); }

	void release()
    {
        LN_SAFE_RELEASE( _impl );
    }


    TYPE_* operator -> () { return (TYPE_*)this; }

    /// ラップしているインターフェイスへのポインタを取得する
    IMPL_TYPE_* getInterface() { return _impl; }

    //// ※受け取ったオブジェクトの参照カウントをひとつ増やす
    //IMPL_TYPE_* _setImpl( IMPL_TYPE_* impl_ )
    //{
    //    _impl = impl_;
    //    LN_SAFE_ADDREF( _impl );
    //}


    // ※受け取ったオブジェクトの参照カウントをひとつ増やす
    LRefClass( IMPL_TYPE_* impl_ )
        : _impl( impl_ )
    {
        LN_SAFE_ADDREF( _impl );
    }

protected:

    IMPL_TYPE_* _impl;
};

#define LN_UTIL_REFCLASS_METHODS( type_, impl_type_ ) \
    public: \
        type_(){} \
        type_( impl_type_* impl_ ) : LRefClass< type_, impl_type_ >( impl_ ) {} \
        type_& operator = ( const lnNullPtr null_ ) { release(); return *this; };



//=============================================================================
// ■ LSingletonRefClass クラス
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
template < class TYPE_, class IMPL_TYPE_ > class LSingletonRefClass
{
public:

    /// コンストラクタ
    LSingletonRefClass()
        : _impl( NULL )
    {}

    /// ラップしているインターフェイスへのポインタを取得する
    IMPL_TYPE_* getInterface() { return _impl; }

    /// -> アクセス
    TYPE_* operator -> () { return (TYPE_*)this; }

	bool operator == ( const lnNullPtr null_ ) const { return ( _impl == NULL ); }
	bool operator != ( const lnNullPtr null_ ) const { return ( _impl != NULL ); }

protected:

    IMPL_TYPE_* _impl;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================