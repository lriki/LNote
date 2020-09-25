//=============================================================================
//【 String 】
//-----------------------------------------------------------------------------
///**
//  @file       String.h
//  @brief      String
//  @author     Riki
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
namespace Core
{
namespace Base
{

//=============================================================================
// ■ String クラス
//-----------------------------------------------------------------------------
///**
// @brief       参照カウント付き文字列クラス (char)
//*/
//=============================================================================
class String
    : public ReferenceObject
{
public:

	/// デフォルトコンストラクタ ( 空の文字列 "" で作成 )
	String();

	/// コンストラクタ ( 文字列設定 )
	String( const char* str_ );

	/// コンストラクタ ( 長さ指定 )
	String( const char* str_, int length_ );

	/// コンストラクタ ( コピー開始インデックスと長さを指定 )
	String( const char* str_, int begin_, int length_ );

	/// コンストラクタ ( ワイド文字列から作成・文字列設定 )
	String( const wchar_t* str_ );

	/// コンストラクタ ( ワイド文字列から作成・長さ指定 )
	String( const wchar_t* str_, int length_ );

	/// コンストラクタ ( ワイド文字列から作成・コピー開始インデックスと長さを指定 )
	String( const wchar_t* str_, int begin_, int length_ );

//protected:

	/// デストラクタ 
	virtual ~String();

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      文字数の取得
    //
    //  @par
    //              strlen() と同じく、終端の '\0' は含みません。
    //*/
    //---------------------------------------------------------------------
    int size() const { return mSize; }

    //---------------------------------------------------------------------
	///**
    //  @brief      文字列へのポインタの取得
    //*/
    //---------------------------------------------------------------------
	char* c_str() { return mString; }

    const char* c_str() const { return mString; }

public:

    void operator += ( const char* str_ );
    bool operator == ( const String& obj_ ) const;
    bool operator == ( const char* str_ ) const;
    bool operator != ( const String& obj_ ) const;
    bool operator != ( const char* str_ ) const;
    bool operator <  ( const String& obj_ ) const;
    bool operator <  ( const char* str_ ) const;

private:

	char*	    mString;
    int         mSize;
};

//=============================================================================
// ■ WString クラス
//-----------------------------------------------------------------------------
///**
//  @brief      参照カウント付き文字列クラス (wchar_t)
//*/
//=============================================================================
class WString
    : public ReferenceObject
{
public:

	/// デフォルトコンストラクタ ( 空の文字列 L"" で作成 )
	WString();

	/// コンストラクタ ( 文字列設定 )
	WString( const wchar_t* str_ );

	/// コンストラクタ ( 長さ指定 )
	WString( const wchar_t* str_, int length_ );

	/// コンストラクタ ( コピー開始インデックスと長さを指定 )
	WString( const wchar_t* str_, int begin_, int length_ );

	/// コンストラクタ ( マルチバイト文字列から作成・文字列設定 )
	WString( const char* str_ );

	/// コンストラクタ ( マルチバイト文字列から作成・長さ指定 )
	WString( const char* str_, int length_ );

	/// コンストラクタ ( マルチバイト文字列から作成・コピー開始インデックスと長さを指定 )
	WString( const char* str_, int begin_, int length_ );

protected:

	/// デストラクタ
	virtual ~WString();

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      文字数の取得
    //
    //  @par
    //              strlen() と同じく、終端の '\0' は含みません。
    //*/
    //---------------------------------------------------------------------
    int size() const { return mSize; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      文字列へのポインタの取得
    //*/
    //---------------------------------------------------------------------
	wchar_t* c_str() { return mString; }

    const wchar_t* c_str() const { return mString; }

public:

    void operator += ( const wchar_t* str_ );
    bool operator == ( const WString& obj_ ) const;
    bool operator == ( const wchar_t* str_ ) const;
    bool operator != ( const WString& obj_ ) const;
    bool operator != ( const wchar_t* str_ ) const;
    bool operator <  ( const WString& obj_ ) const;
    bool operator <  ( const wchar_t* str_ ) const;

private:

	wchar_t*	mString;
    int         mSize;
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