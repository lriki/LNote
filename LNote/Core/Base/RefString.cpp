//=============================================================================
//【 RefString 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "String.h"
#include "StringUtil.h"
#include "RefString.h"

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
// ■ RefString クラス
//=============================================================================

    static RefString EMPTY_STRING_A( "" );

	// コンストラクタ ( 受け取った文字列をコピーして作成 )
	RefString::RefString( const char* str_ )
	{
		mImpl = LN_NEW String( str_ );
	}
	RefString::RefString( const char* str_, int length_ )
	{
		mImpl = LN_NEW String( str_, length_ );
	}
	RefString::RefString( const char* str_, int begin_, int length_ )
	{
		mImpl = LN_NEW String( str_, length_, length_ );
	}

	// ( ワイド文字列から作成 )
	RefString::RefString( const wchar_t* str_ )
	{
		mImpl = LN_NEW String( str_ );
	}
	RefString::RefString( const wchar_t* str_, int length_ )
	{
		mImpl = LN_NEW String( str_, length_ );
	}
	RefString::RefString( const wchar_t* str_, int begin_, int length_ )
	{
		mImpl = LN_NEW String( str_, length_, length_ );
	}

	// コンストラクタ ( RefWString から作成 )
	RefString::RefString( const RefWString str_ )
	{
		mImpl = LN_NEW String( str_.c_str() );
	}

	// 代入演算子 ( char* から )
	RefString& RefString::operator = ( const char* str_ )
	{
		LN_SAFE_RELEASE( mImpl );
        if ( str_ )
        {
		    mImpl = LN_NEW String( str_ );
        }
        else
        {
            *this = EMPTY_STRING_A;
        }
		return *this;
	}

	// 代入演算子 ( wchar_t* から )
	RefString& RefString::operator = ( const wchar_t* str_ )
	{
		LN_SAFE_RELEASE( mImpl );
        if ( str_ )
        {
		    mImpl = LN_NEW String( str_ );
        }
        else
        {
            *this = EMPTY_STRING_A;
        }
		return *this;
	}

	// 代入演算子 ( RefWString から )
	RefString& RefString::operator = ( const RefWString str_ )
	{
		LN_SAFE_RELEASE( mImpl );
		mImpl = LN_NEW String( str_.c_str() );
		return *this;
	}

    // 文字数の取得
	int RefString::size() const
	{
        return ( mImpl ) ? mImpl->size() : 0;
	}

	// 文字列へのポインタの取得
	//char* RefString::c_str()
	//{
	//	return ( mImpl ) ? mImpl->c_str() : NULL;
	//}

    // const 版
    const char* RefString::c_str() const
    {
        return ( mImpl ) ? mImpl->c_str() : "";   // const だからとりあえずこれで OK
    }

    // 書式を指定して文字列を作成する
    void RefString::format( const char* format_, ... )
    {
        va_list args;
        va_start( args, format_ );

        size_t size = _vscprintf( format_, args ) + 1;

        //char* tmp_buf = static_cast< char* >( malloc( size * sizeof( char ) ) );
        char* tmp_buf = LN_NEW char[ size ];
        vsnprintf( tmp_buf, size, format_, args );
        tmp_buf[ size - 1 ] = '\0';

        LN_SAFE_RELEASE( mImpl );
		mImpl = LN_NEW String( tmp_buf );

        //SAFE_FREE( tmp_buf );
        SAFE_DELETE_ARRAY( tmp_buf );
        va_end( args );
    }

    // クリア
    void RefString::clear()
    {
        *this = EMPTY_STRING_A;
    }

    // 空文字列かを判定する
    bool RefString::empty() const
    {
        return ( mImpl && mImpl->size() != 0 ) ? false : true;
    }

    void RefString::operator += ( const char* str_ )
    {
        String*	impl = mImpl;
        
		mImpl = LN_NEW String( impl->c_str() );
        ( *mImpl ) += str_;

        LN_SAFE_RELEASE( impl );
    }

    bool RefString::operator == ( const RefString& obj_ ) const
    {
        return ( *mImpl == *obj_.mImpl );
    }

    bool RefString::operator == ( const char* str_ ) const
    {
        return ( *mImpl == str_ );
    }

    bool RefString::operator != ( const RefString& obj_ ) const
    {
        return ( *mImpl != *obj_.mImpl );
    }

    bool RefString::operator != ( const char* str_ ) const
    {
        return ( *mImpl != str_ );
    }

    bool RefString::operator < ( const RefString& obj_ ) const
    {
        return ( *mImpl < *obj_.mImpl );
    }

    bool RefString::operator < ( const char* str_ ) const
    {
        return ( *mImpl < str_ );
    }

    char& RefString::operator [] ( int idx_ )
    {
        return mImpl->c_str()[ idx_ ];
    }

    const char& RefString::operator [] ( int idx_ ) const
    {
        return mImpl->c_str()[ idx_ ];
    }

    RefString::operator std::string () const
    {
        return std::string( mImpl->c_str() );
    }

	// 参照カウント用
	#define CLASSNAME RefString
	#define IMPLCLASS String
    #define NOTCMP
	#include "RefClassTemplate.h"


//=============================================================================
// ■ RefWString クラス
//=============================================================================

    static RefString EMPTY_STRING_W( L"" );

	// コンストラクタ ( 受け取った文字列をコピーして作成 )
	RefWString::RefWString( const wchar_t* str_ )
	{
		mImpl = LN_NEW WString( str_ );
	}
	RefWString::RefWString( const wchar_t* str_, int length_ )
	{
		mImpl = LN_NEW WString( str_, length_ );
	}
	RefWString::RefWString( const wchar_t* str_, int begin_, int length_ )
	{
		mImpl = LN_NEW WString( str_, length_, length_ );
	}

	// ( マルチバイト文字列から作成 )
	RefWString::RefWString( const char* str_ )
	{
		mImpl = LN_NEW WString( str_ );
	}
	RefWString::RefWString( const char* str_, int length_ )
	{
		mImpl = LN_NEW WString( str_, length_ );
	}
	RefWString::RefWString( const char* str_, int begin_, int length_ )
	{
		mImpl = LN_NEW WString( str_, length_, length_ );
	}

	/// コンストラクタ ( LString から作成 )
	RefWString::RefWString( const RefString str_ )
	{
		mImpl = LN_NEW WString( str_.c_str() );
	}

	// 代入演算子 ( wchar_t* から )
	RefWString& RefWString::operator = ( const wchar_t* str_ )
	{
		LN_SAFE_RELEASE( mImpl );
		if ( str_ )
        {
		    mImpl = LN_NEW WString( str_ );
        }
        else
        {
            *this = EMPTY_STRING_W;
        }
		return *this;
	}

	// 代入演算子 ( char* から )
	RefWString& RefWString::operator = ( const char* str_ )
	{
		LN_SAFE_RELEASE( mImpl );
        if ( str_ )
        {
		    mImpl = LN_NEW WString( str_ );
        }
        else
        {
            *this = EMPTY_STRING_W;
        }
		return *this;
	}

	// 代入演算子 ( LString から )
	RefWString& RefWString::operator = ( const RefString str_ )
	{
		LN_SAFE_RELEASE( mImpl );
        mImpl = LN_NEW WString( str_.c_str() );
		return *this;
	}

    // 文字数の取得
	int RefWString::size() const
	{
		return ( mImpl ) ? mImpl->size() : 0;
	}

	// 文字列へのポインタの取得
	//wchar_t* RefWString::c_str()
	//{
	//	return ( mImpl ) ? mImpl->c_str() : NULL;
	//}

    // const 版
    const wchar_t* RefWString::c_str() const
    {
        return ( mImpl ) ? mImpl->c_str() : L"";
    }

    // 書式を指定して文字列を作成する
    void RefWString::format( const wchar_t* format_, ... )
    {
        va_list args;
        va_start( args, format_ );

        size_t size = _vscwprintf( format_, args ) + 1;

        //wchar_t* tmp_buf = static_cast< wchar_t* >( malloc( size * sizeof( wchar_t ) ) );
        wchar_t* tmp_buf = LN_NEW wchar_t[ size ];
        vswprintf( tmp_buf, size, format_, args );
        tmp_buf[ size - 1 ] = L'\0';

        LN_SAFE_RELEASE( mImpl );
		mImpl = LN_NEW WString( tmp_buf );

        //SAFE_FREE( tmp_buf );
        SAFE_DELETE_ARRAY( tmp_buf );
        va_end( args );

        /*
        String ts = format_;    // to char
        std::string sts = ts.c_str();
        //std::wstring ts = format_;
        StringUtil::replaceString( &sts, "%s", "%ls" );
        StringUtil::replaceString( &sts, "%c", "%lc" );

        

        _p( sts.c_str() );
        _p( vsnprintf( NULL, 0, sts.c_str(), args ) );

        
        */
    }

    // クリア
    void RefWString::clear()
    {
        *this = EMPTY_STRING_W;
    }

    // 空文字列かを判定する
    bool RefWString::empty() const
    {
        return ( mImpl && mImpl->size() != 0 ) ? false : true;
    }

    void RefWString::operator += ( const wchar_t* str_ )
    {
        WString*	impl = mImpl;
        
		mImpl = LN_NEW WString( impl->c_str() );
        ( *mImpl ) += str_;

        LN_SAFE_RELEASE( impl );
    }

    bool RefWString::operator == ( const RefWString& obj_ ) const
    {
        return ( *mImpl == *obj_.mImpl );
    }

    bool RefWString::operator == ( const wchar_t* str_ ) const
    {
        return ( *mImpl == str_ );
    }

    bool RefWString::operator != ( const RefWString& obj_ ) const
    {
        return ( *mImpl != *obj_.mImpl );
    }

    bool RefWString::operator != ( const wchar_t* str_ ) const
    {
        return ( *mImpl != str_ );
    }

    bool RefWString::operator < ( const RefWString& obj_ ) const
    {
        return ( *mImpl < *obj_.mImpl );
    }

    bool RefWString::operator < ( const wchar_t* str_ ) const
    {
        return ( *mImpl < str_ );
    }

    wchar_t& RefWString::operator [] ( int idx_ )
    {
        return mImpl->c_str()[ idx_ ];
    }

    const wchar_t& RefWString::operator [] ( int idx_ ) const
    {
        return mImpl->c_str()[ idx_ ];
    }

    RefWString::operator std::wstring () const
    {
        return std::wstring( mImpl->c_str() );
    }

	// 参照カウント用
	#define CLASSNAME RefWString
	#define IMPLCLASS Base::WString
    #define NOTCMP
	#include "RefClassTemplate.h"


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================