//=============================================================================
//【 RefString 】
//-----------------------------------------------------------------------------
///**
//  @file       RefString.h
//  @brief      RefString
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <string>
#include "RefClass.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Base
{
class String;
class WString;
class RefWString;

#ifdef LNOTE_UNICODE
    #define RefTString  RefWString
#else
    #define RefTString  RefString
#endif

//=============================================================================
// ■ RefString クラス
//-----------------------------------------------------------------------------
///**
//  @brief      文字列の参照クラス
// 
//  @par
//              std::string の代わりに使えます。
//              std::string とは違い、文字列を参照カウントで管理します。			<br><br>
//              
//              例えば、代入演算子 ( = ) を用いて代入を行うと、std::string では
//              文字列が丸ごと複製されますが、LString では文字列の参照カウント
//              ( 実際は内部に文字列を扱うクラスがあり、それの参照カウントを操作しています )
//              を増やすことで、ひとつの文字列を共有して使います。					<br><br>
//              
//              このため、代入や関数の値渡しの操作は std::string よりも高速です。
//              ただし、ひとつの文字列を共有しているため、いずれかの LString が
//              文字列を操作すると、それと同じ文字列を参照している LString 全てに
//              影響がでます。														<br><br>
//               
//              また、RefWString クラスと組み合わせることで
//              マルチバイト文字列 ⇔ ワイド文字列の変換も容易に行う事ができます。
//              ( 変換には時間がかかるため多用は避けてください )
//
//  @note
//              NULL と比較した場合に落ちるバグがあるけど、これはどうしようか…。
//*/
//=============================================================================
class RefString
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ ( 受け取った文字列をコピーして作成 )
    // 
    //  @param[in]  str_    : コピー元の文字列
    //*/
    //---------------------------------------------------------------------
    RefString( const char* str_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ ( 長さを指定 )
    //
    //  @param[in]  str_    : コピー元の文字列
    //  @param[in]  length_ : コピーする長さ ( -1 で \0 までコピー )
    //*/
    //---------------------------------------------------------------------
    RefString( const char* str_, int length_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ ( コピー開始インデックスと長さを指定 )
    //
    //  @param[in]  str_    : コピー元の文字列
    //  @param[in]  begin_  : コピー元の先頭インデックス
    //  @param[in]  length_ : コピーする長さ ( -1 で \0 までコピー )
    //*/
    //---------------------------------------------------------------------
    RefString( const char* str_, int begin_, int length_ );

    /// コンストラクタ ( ワイド文字列から作成・受け取った文字列をコピーして作成 )
    RefString( const wchar_t* str_ );

    /// コンストラクタ ( ワイド文字列から作成・長さを指定 )
    RefString( const wchar_t* str_, int length_ );

    /// コンストラクタ ( ワイド文字列から作成・コピー開始インデックスと長さを指定 )
    RefString( const wchar_t* str_, int begin_, int length_ );

    /// コンストラクタ ( LWString から作成 )
    RefString( const RefWString str_ );

    /// 代入演算子 ( char* から )
    RefString& operator = ( const char* str_ );

    /// 代入演算子 ( wchar_t* から )
    RefString& operator = ( const wchar_t* str_ );

    /// 代入演算子 ( LWString から )
    RefString& operator = ( const RefWString str_ );

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      文字数の取得
    //
    //  @par
    //              strlen() と同じく、終端の '\0' は含みません。<br>
    //              実際に文字列が確保されていなくても 0 を返します。
    //              ( Cache 等ののキーとして使った場合、比較回数を少なくするため )
    //*/
    //---------------------------------------------------------------------
    int size() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      文字列へのポインタの取得
    //*/
    //---------------------------------------------------------------------
    //char* c_str();

    /// const 版。Hash で使うので一応用意
    const char* c_str() const;

    //---------------------------------------------------------------------
	///**
    //  @brief      書式を指定して文字列を作成する
    //*/
    //---------------------------------------------------------------------
    void format( const char* format_, ... );

    /// クリア
    void clear();

    /// 空文字列かを判定する
    bool empty() const;

public:

    void operator += ( const char* str_ );
    bool operator == ( const RefString& obj_ ) const;
    bool operator == ( const char* str_ ) const;
    bool operator != ( const RefString& obj_ ) const;
    bool operator != ( const char* str_ ) const;
    bool operator <  ( const RefString& obj_ ) const;
    bool operator <  ( const char* str_ ) const;
    char& operator [] ( int idx_ );
    const char& operator [] ( int idx_ ) const;
    operator std::string () const;

    LN_REFERENCE_TYPE_METHODS_NOT_CMP( RefString, String );

private:

	String*	mImpl;
};

//=============================================================================
// ■ RefWString クラス
//-----------------------------------------------------------------------------
///**
//  @brief      ワイド文字列の参照クラス
//*/
//=============================================================================
class RefWString
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ ( 受け取った文字列をコピーして作成 )
    // 
    //  @param[in]  str_    : コピー元の文字列
    //*/
    //---------------------------------------------------------------------
    RefWString( const wchar_t* str_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ ( 長さを指定 )
    // 
    //  @param[in]  str_    : コピー元の文字列
    //  @param[in]  length_ : コピーする長さ ( -1 で \0 までコピー )
    //*/
    //---------------------------------------------------------------------
    RefWString( const wchar_t* str_, int length_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ ( コピー開始インデックスと長さを指定 )
    // 
    //  @param[in]  str_    : コピー元の文字列
    //  @param[in]  begin_  : コピー元の先頭インデックス
    //  @param[in]  length_ : コピーする長さ ( -1 で \0 までコピー )
    //*/
    //---------------------------------------------------------------------
    RefWString( const wchar_t* str_, int begin_, int length_ );

    /// コンストラクタ ( マルチバイト文字列から作成・受け取った文字列をコピーして作成 )
    RefWString( const char* str_ );

    /// コンストラクタ ( マルチバイト文字列から作成・長さを指定 )
    RefWString( const char* str_, int length_ );

    /// コンストラクタ ( マルチバイト文字列から作成・コピー開始インデックスと長さを指定 )
    RefWString( const char* str_, int begin_, int length_ );

    /// コンストラクタ ( LString から作成 )
    RefWString( const RefString str_ );

    /// 代入演算子 ( wchar_t* から )
    RefWString& operator = ( const wchar_t* str_ );

    /// 代入演算子 ( char* から )
    RefWString& operator = ( const char* str_ );

    /// 代入演算子 ( LString から )
    RefWString& operator = ( const RefString str_ );

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      文字数の取得
    //
    //  @par
    //              strlen() と同じく、終端の '\0' は含みません。
    //*/
    //---------------------------------------------------------------------
    int size() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      文字列へのポインタの取得
    //*/
    //---------------------------------------------------------------------
    //wchar_t* c_str();

    /// const 版。Hash で使うので一応用意
    const wchar_t* c_str() const;

    //---------------------------------------------------------------------
	///**
    //  @brief      書式を指定して文字列を作成する
    //*/
    //---------------------------------------------------------------------
    void format( const wchar_t* format_, ... );

    /// クリア
    void clear();

    /// 空文字列かを判定する
    bool empty() const;

public:

    void operator += ( const wchar_t* str_ );
    bool operator == ( const RefWString& obj_ ) const;
    bool operator == ( const wchar_t* str_ ) const;
    bool operator != ( const RefWString& obj_ ) const;
    bool operator != ( const wchar_t* str_ ) const;
    bool operator <  ( const RefWString& obj_ ) const;
    bool operator <  ( const wchar_t* str_ ) const;
    wchar_t& operator [] ( int idx_ );
    const wchar_t& operator [] ( int idx_ ) const;
    operator std::wstring () const;

	LN_REFERENCE_TYPE_METHODS_NOT_CMP( RefWString, WString );

private:

	WString*	mImpl;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core

typedef Core::Base::RefString  LRefString;
typedef Core::Base::RefWString LRefWString;

#ifdef LNOTE_UNICODE
    #define LRefTString  LRefWString
#else
    #define LRefTString  LRefString
#endif

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================