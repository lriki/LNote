//=============================================================================
//【 Hash 】
//-----------------------------------------------------------------------------
///**
//  @file       Hash.h
//  @brief      Hash
//  @author     Riki
//  @note
//              参考にさせていただいたサイト
//              http://marupeke296.com
//              http://burtleburtle.net/bob/c/crc.c
//              http://burtleburtle.net/bob/hash/doobs.html
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
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
// ■ Hash クラス
//-----------------------------------------------------------------------------
///**
//  @brief      文字列から 32 ビットのハッシュ値を求めるモジュール
//*/
//=============================================================================
class Hash
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief		ハッシュ値を求める
    //*/
    //---------------------------------------------------------------------
    static u32 calcHash( const char* str_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      ハッシュ値を求める ( ワイド文字列から )
    //*/
    //---------------------------------------------------------------------
    static u32 calcHash( const wchar_t* str_ );

};

//=============================================================================
// ■ GenericHash クラス
//-----------------------------------------------------------------------------
///**
//	@brief      汎用ハッシュ計算クラス
//*/
//=============================================================================
template< class TYPE_ > class GenericHash
{
public:

    /// v_ から値をを求め、mod_ で割ったあまりを返す
	unsigned value( const TYPE_& v_, unsigned mod_ ) const;

    /// 等しいかどうかチェック
	bool isEqual( const TYPE_& v0_, const TYPE_& v1_ ) const;
};





//-------------------------------------------------------------------------
// GenericHash

// 標準版
template< class TYPE_ > inline unsigned GenericHash< TYPE_ >::value( const TYPE_& v, unsigned m ) const
{
	const unsigned char* p = reinterpret_cast< const unsigned char* >( &v );
	unsigned h = 0;
	for ( int i = 0; i < sizeof( TYPE_ ); ++i )
    {
		h *= 137;
		h += p[ i ];
	}
	h %= m;
	return h;
}

template< class TYPE_ > inline bool GenericHash< TYPE_ >::isEqual( const TYPE_& a, const TYPE_& b ) const {
	return ( a == b );
}

//ポインタ用特別版
template<> inline bool GenericHash< const char* >::isEqual( const char* const& a, const char* const& b ) const {
	const char* pa = a;
	const char* pb = b;

	while ( *pa == *pb ){ //片方が先に終わってしまう時も検出できる
		if ( *pa == '\0' ){ //ここまで等しくて同時に0になったってことは等しいってことだ。
			return true;
		}
		++pa;
		++pb;
	}
	return false;
}

// string ヘッダを include してる場合はコレも
#ifdef _STRING_

template<> inline bool GenericHash< const std::string* >::isEqual( const std::string* const& a, const std::string* const& b ) const {
	if ( a->size() != b->size() ){ //なくてもいいが、これで早く終われる可能性は高い
		return false;
	}
	//サイズは一緒。
	const char* pa = a->c_str();
	const char* pb = b->c_str();

	while ( *pa == *pb ){ //片方が先に終わってしまう時も検出できる
		if ( *pa == '\0' ){ //ここまで等しくて同時に0になったってことは等しいってことだ。
			return true;
		}
		++pa;
		++pb;
	}
	return false;
}

#endif

// Base::RefString
template<> inline bool GenericHash< const RefString* >::isEqual( const RefString* const& a, const RefString* const& b ) const
{
	if ( a->size() != b->size() )
    {
		return false;
	}

    const char* pa = a->c_str();
	const char* pb = b->c_str();

	while ( *pa == *pb )
    {
		if ( *pa == '\0' )
        {
			return true;
		}
		++pa;
		++pb;
	}
	return false;
}

// Base::RefWString
template<> inline bool GenericHash< const RefWString* >::isEqual( const RefWString* const& a, const RefWString* const& b ) const
{
	if ( a->size() != b->size() )
    {
		return false;
	}

    const wchar_t* pa = a->c_str();
	const wchar_t* pb = b->c_str();

	while ( *pa == *pb )
    {
		if ( *pa == L'\0' )
        {
			return true;
		}
		++pa;
		++pb;
	}
	return false;
}





// 特別バージョンを一通り用意
template<> inline unsigned GenericHash< int >::value( const int& v, unsigned m ) const {
	return static_cast< unsigned >( v ) % m;
}

template<> inline unsigned GenericHash< unsigned > ::value( const unsigned& v, unsigned m ) const {
	return v % m;
}

template<> inline unsigned GenericHash< long >::value( const long& v, unsigned m ) const {
	return static_cast< unsigned >( v ) % m;
}

template<> inline unsigned GenericHash< unsigned long >::value( const unsigned long& v, unsigned m ) const {
	return static_cast< unsigned >( v ) % m;
}

template<> inline unsigned GenericHash< short >::value( const short& v, unsigned m ) const {
	return v % m;
}

template<> inline unsigned GenericHash< unsigned short >::value( const unsigned short& v, unsigned m ) const {
	return v % m;
}

template<> inline unsigned GenericHash< char >::value( const char& v, unsigned m ) const {
	return v % m;
}

template<> inline unsigned GenericHash< unsigned char >::value( const unsigned char& v, unsigned m ) const {
	return v % m;
}

template<> inline unsigned GenericHash< float >::value( const float& v, unsigned m ) const {
	const unsigned* p = reinterpret_cast< const unsigned* >( &v );
	return *p % m;
}

template<> inline unsigned GenericHash< double >::value( const double& v, unsigned m ) const {
	const unsigned* p = reinterpret_cast< const unsigned* >( &v );
	unsigned h = p[ 0 ] * 137 + p[ 1 ];
	h %= m;
	return h;
}

template<> inline unsigned GenericHash< const char* >::value( const char* const& v, unsigned m ) const {
	const unsigned char* p = reinterpret_cast< const unsigned char* >( v );
	unsigned h = 0;
	for ( int i = 0; p[ i ] != '\0'; ++i ){
		h *= 137;
		h += p[ i ];
	}
	h %= m;
	return h;
}

#ifdef _STRING_

template<> inline unsigned GenericHash< std::string >::value( const std::string& v, unsigned m ) const {
	const unsigned char* p = reinterpret_cast< const unsigned char* >( v.c_str() );
	unsigned h = 0;
	for ( int i = 0; p[ i ] != '\0'; ++i ){
		h *= 137;
		h += p[ i ];
	}
	h %= m;
	return h;
}

template<> inline unsigned GenericHash< const std::string* >::value( const std::string* const& v, unsigned m ) const {
	const unsigned char* p = reinterpret_cast< const unsigned char* >( v->c_str() );
	unsigned h = 0;
	for ( int i = 0; p[ i ] != '\0'; ++i ){
		h *= 137;
		h += p[ i ];
	}
	h %= m;
	return h;
}

#endif

template<> inline unsigned GenericHash< RefString >::value( const RefString& v, unsigned m ) const {
	const unsigned char* p = reinterpret_cast< const unsigned char* >( v.c_str() );
	unsigned h = 0;
	for ( int i = 0; i < v.size(); ++i ){
		h *= 137;
		h += p[ i ];
	}
	h %= m;
	return h;
}

template<> inline unsigned GenericHash< const RefString* >::value( const RefString* const& v, unsigned m ) const {
	const unsigned char* p = reinterpret_cast< const unsigned char* >( v->c_str() );
	unsigned h = 0;
	for ( int i = 0; i < v->size(); ++i ){
		h *= 137;
		h += p[ i ];
	}
	h %= m;
	return h;
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