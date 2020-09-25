//=============================================================================
//�y Hash �z
//-----------------------------------------------------------------------------
///**
//  @file       Hash.h
//  @brief      Hash
//  @author     Riki
//  @note
//              �Q�l�ɂ����Ă����������T�C�g
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
// �� Hash �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �����񂩂� 32 �r�b�g�̃n�b�V���l�����߂郂�W���[��
//*/
//=============================================================================
class Hash
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief		�n�b�V���l�����߂�
    //*/
    //---------------------------------------------------------------------
    static u32 calcHash( const char* str_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �n�b�V���l�����߂� ( ���C�h�����񂩂� )
    //*/
    //---------------------------------------------------------------------
    static u32 calcHash( const wchar_t* str_ );

};

//=============================================================================
// �� GenericHash �N���X
//-----------------------------------------------------------------------------
///**
//	@brief      �ėp�n�b�V���v�Z�N���X
//*/
//=============================================================================
template< class TYPE_ > class GenericHash
{
public:

    /// v_ ����l�������߁Amod_ �Ŋ��������܂��Ԃ�
	unsigned value( const TYPE_& v_, unsigned mod_ ) const;

    /// ���������ǂ����`�F�b�N
	bool isEqual( const TYPE_& v0_, const TYPE_& v1_ ) const;
};





//-------------------------------------------------------------------------
// GenericHash

// �W����
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

//�|�C���^�p���ʔ�
template<> inline bool GenericHash< const char* >::isEqual( const char* const& a, const char* const& b ) const {
	const char* pa = a;
	const char* pb = b;

	while ( *pa == *pb ){ //�Е�����ɏI����Ă��܂��������o�ł���
		if ( *pa == '\0' ){ //�����܂œ������ē�����0�ɂȂ������Ă��Ƃ͓��������Ă��Ƃ��B
			return true;
		}
		++pa;
		++pb;
	}
	return false;
}

// string �w�b�_�� include ���Ă�ꍇ�̓R����
#ifdef _STRING_

template<> inline bool GenericHash< const std::string* >::isEqual( const std::string* const& a, const std::string* const& b ) const {
	if ( a->size() != b->size() ){ //�Ȃ��Ă��������A����ő����I����\���͍���
		return false;
	}
	//�T�C�Y�͈ꏏ�B
	const char* pa = a->c_str();
	const char* pb = b->c_str();

	while ( *pa == *pb ){ //�Е�����ɏI����Ă��܂��������o�ł���
		if ( *pa == '\0' ){ //�����܂œ������ē�����0�ɂȂ������Ă��Ƃ͓��������Ă��Ƃ��B
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





// ���ʃo�[�W��������ʂ�p��
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