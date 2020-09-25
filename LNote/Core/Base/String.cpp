//=============================================================================
//【 String 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "StringUtil.h"
#include "String.h"

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
// ■ String クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● デフォルトコンストラクタ
	//---------------------------------------------------------------------
	String::String()
		: mString   ( NULL )
        , mSize     ( 0 )
	{
        mString = LN_NEW char[ 1 ];
        mString[ 0 ] = '\0';
    }

	//---------------------------------------------------------------------
	// ● コンストラクタ ( 文字列設定 )
	//---------------------------------------------------------------------
	String::String( const char* str_ )
		: mString( NULL )
	{
		mString = StringUtil::copyLength( str_, -1 );
        mSize = strlen( mString );
	}

	//---------------------------------------------------------------------
	// ● コンストラクタ ( 長さ指定 )
	//---------------------------------------------------------------------
	String::String( const char* str_, int length_ )
		: mString( NULL )
	{
		mString = StringUtil::copyLength( str_, length_ );
        mSize = strlen( mString );
	}

	//---------------------------------------------------------------------
	// ● コンストラクタ ( コピー開始インデックスと長さを指定 )
	//---------------------------------------------------------------------
	String::String( const char* str_, int begin_, int length_ )
		: mString( NULL )
	{
		mString = StringUtil::copyLength( str_, begin_, length_ );
        mSize = strlen( mString );
	}

	//---------------------------------------------------------------------
	// ● コンストラクタ ( ワイド文字列から作成・文字列設定 )
	//---------------------------------------------------------------------
	String::String( const wchar_t* str_ )
		: mString( NULL )
	{
		mString = StringUtil::convWideToMulti( str_ );
        mSize = strlen( mString );
	}

	//---------------------------------------------------------------------
	// ● コンストラクタ ( ワイド文字列から作成・長さ指定 )
	//---------------------------------------------------------------------
	String::String( const wchar_t* str_, int length_ )
		: mString( NULL )
	{
		wchar_t* s = StringUtil::copyLength( str_, length_ );
		mString = StringUtil::convWideToMulti( s );
		SAFE_DELETE_ARRAY( s );
        mSize = strlen( mString );
	}

	//---------------------------------------------------------------------
	// ● コンストラクタ ( ワイド文字列から作成・コピー開始インデックスと長さを指定 )
	//---------------------------------------------------------------------
	String::String( const wchar_t* str_, int begin_, int length_ )
		: mString( NULL )
	{
		wchar_t* s = StringUtil::copyLength( str_, begin_, length_ );
		mString = StringUtil::convWideToMulti( s );
		SAFE_DELETE_ARRAY( s );
        mSize = strlen( mString );

	}

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	String::~String()
	{
		SAFE_DELETE_ARRAY( mString );
        mSize = 0;
	}


    void String::operator += ( const char* str_ )
    {
        char* new_str = StringUtil::merge( mString, str_ );
        SAFE_DELETE_ARRAY( mString );
        mString = new_str;
    }

    bool String::operator == ( const String& obj_ ) const
    {
        if ( mSize == obj_.mSize )
        {
            return ( strcmp( mString, obj_.mString ) == 0 );
        }
        return false;
    }

    bool String::operator == ( const char* str_ ) const
    {
        return ( strcmp( mString, str_ ) == 0 );
    }

    bool String::operator != ( const String& obj_ ) const
    {
        return ( strcmp( mString, obj_.mString ) != 0 );
    }

    bool String::operator != ( const char* str_ ) const
    {
        return ( strcmp( mString, str_ ) != 0 );
    }

    bool String::operator < ( const String& obj_ ) const
    {
        return ( strcmp( mString, obj_.mString ) < 0 );
    }

    bool String::operator < ( const char* str_ ) const
    {
        return ( strcmp( mString, str_ ) < 0 );
    }

//=============================================================================
// ■ WString クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● デフォルトコンストラクタ
	//---------------------------------------------------------------------
	WString::WString()
		: mString   ( NULL )
        , mSize     ( 0 )
	{
        mString = LN_NEW wchar_t[ 1 ];
        mString[ 0 ] = L'\0';
    }

	//---------------------------------------------------------------------
	// ● コンストラクタ ( 文字列設定 )
	//---------------------------------------------------------------------
	WString::WString( const wchar_t* str_ )
		: mString( NULL )
	{
		mString = StringUtil::copyLength( str_, -1 );
        mSize = wcslen( mString );
	}

	//---------------------------------------------------------------------
	// ● コンストラクタ ( 長さ指定 )
	//---------------------------------------------------------------------
	WString::WString( const wchar_t* str_, int length_ )
		: mString( NULL )
	{
		mString = StringUtil::copyLength( str_, length_ );
        mSize = wcslen( mString );
	}

	//---------------------------------------------------------------------
	// ● コンストラクタ ( コピー開始インデックスと長さを指定 )
	//---------------------------------------------------------------------
	WString::WString( const wchar_t* str_, int begin_, int length_ )
		: mString( NULL )
	{
		mString = StringUtil::copyLength( str_, begin_, length_ );
        mSize = wcslen( mString );
	}

	//---------------------------------------------------------------------
	// ● コンストラクタ ( マルチバイト文字列から作成・文字列設定 )
	//---------------------------------------------------------------------
	WString::WString( const char* str_ )
		: mString( NULL )
	{
		mString = StringUtil::convMultiToWide( str_ );
        mSize = wcslen( mString );
	}

	//---------------------------------------------------------------------
	// ● コンストラクタ ( マルチバイト文字列から作成・長さ指定 )
	//---------------------------------------------------------------------
	WString::WString( const char* str_, int length_ )
		: mString( NULL )
	{
		char* s = StringUtil::copyLength( str_, length_ );
		mString = StringUtil::convMultiToWide( s );
		SAFE_DELETE_ARRAY( s );
        mSize = wcslen( mString );
	}

	//---------------------------------------------------------------------
	// ● コンストラクタ ( マルチバイト文字列から作成・コピー開始インデックスと長さを指定 )
	//---------------------------------------------------------------------
	WString::WString( const char* str_, int begin_, int length_ )
		: mString( NULL )
	{
		char* s = StringUtil::copyLength( str_, begin_, length_ );
		mString = StringUtil::convMultiToWide( s );
		SAFE_DELETE_ARRAY( s );
        mSize = wcslen( mString );
	}

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	WString::~WString()
	{
		SAFE_DELETE_ARRAY( mString );
        mSize = 0;
	}


    void WString::operator += ( const wchar_t* str_ )
    {
        wchar_t* new_str = StringUtil::merge( mString, str_ );
        SAFE_DELETE_ARRAY( mString );
        mString = new_str;
    }

    bool WString::operator == ( const WString& obj_ ) const
    {
        if ( mSize == obj_.mSize )
        {
            return ( wcscmp( mString, obj_.mString ) == 0 );
        }
        return false;
    }

    bool WString::operator == ( const wchar_t* str_ ) const
    {
        return ( wcscmp( mString, str_ ) == 0 );
    }

    bool WString::operator != ( const WString& obj_ ) const
    {
        return ( wcscmp( mString, obj_.mString ) != 0 );
    }

    bool WString::operator != ( const wchar_t* str_ ) const
    {
        return ( wcscmp( mString, str_ ) != 0 );
    }

    bool WString::operator < ( const WString& obj_ ) const
    {
        return ( wcscmp( mString, obj_.mString ) < 0 );
    }

    bool WString::operator < ( const wchar_t* str_ ) const
    {
        return ( wcscmp( mString, str_ ) < 0 );
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================