//=============================================================================
//�y RefString �z
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
// �� RefString �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      ������̎Q�ƃN���X
// 
//  @par
//              std::string �̑���Ɏg���܂��B
//              std::string �Ƃ͈Ⴂ�A��������Q�ƃJ�E���g�ŊǗ����܂��B			<br><br>
//              
//              �Ⴆ�΁A������Z�q ( = ) ��p���đ�����s���ƁAstd::string �ł�
//              �����񂪊ۂ��ƕ�������܂����ALString �ł͕�����̎Q�ƃJ�E���g
//              ( ���ۂ͓����ɕ�����������N���X������A����̎Q�ƃJ�E���g�𑀍삵�Ă��܂� )
//              �𑝂₷���ƂŁA�ЂƂ̕���������L���Ďg���܂��B					<br><br>
//              
//              ���̂��߁A�����֐��̒l�n���̑���� std::string ���������ł��B
//              �������A�ЂƂ̕���������L���Ă��邽�߁A�����ꂩ�� LString ��
//              ������𑀍삷��ƁA����Ɠ�����������Q�Ƃ��Ă��� LString �S�Ă�
//              �e�����ł܂��B														<br><br>
//               
//              �܂��ARefWString �N���X�Ƒg�ݍ��킹�邱�Ƃ�
//              �}���`�o�C�g������ �� ���C�h������̕ϊ����e�Ղɍs�������ł��܂��B
//              ( �ϊ��ɂ͎��Ԃ������邽�ߑ��p�͔����Ă������� )
//
//  @note
//              NULL �Ɣ�r�����ꍇ�ɗ�����o�O�����邯�ǁA����͂ǂ����悤���c�B
//*/
//=============================================================================
class RefString
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���X�g���N�^ ( �󂯎������������R�s�[���č쐬 )
    // 
    //  @param[in]  str_    : �R�s�[���̕�����
    //*/
    //---------------------------------------------------------------------
    RefString( const char* str_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���X�g���N�^ ( �������w�� )
    //
    //  @param[in]  str_    : �R�s�[���̕�����
    //  @param[in]  length_ : �R�s�[���钷�� ( -1 �� \0 �܂ŃR�s�[ )
    //*/
    //---------------------------------------------------------------------
    RefString( const char* str_, int length_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���X�g���N�^ ( �R�s�[�J�n�C���f�b�N�X�ƒ������w�� )
    //
    //  @param[in]  str_    : �R�s�[���̕�����
    //  @param[in]  begin_  : �R�s�[���̐擪�C���f�b�N�X
    //  @param[in]  length_ : �R�s�[���钷�� ( -1 �� \0 �܂ŃR�s�[ )
    //*/
    //---------------------------------------------------------------------
    RefString( const char* str_, int begin_, int length_ );

    /// �R���X�g���N�^ ( ���C�h�����񂩂�쐬�E�󂯎������������R�s�[���č쐬 )
    RefString( const wchar_t* str_ );

    /// �R���X�g���N�^ ( ���C�h�����񂩂�쐬�E�������w�� )
    RefString( const wchar_t* str_, int length_ );

    /// �R���X�g���N�^ ( ���C�h�����񂩂�쐬�E�R�s�[�J�n�C���f�b�N�X�ƒ������w�� )
    RefString( const wchar_t* str_, int begin_, int length_ );

    /// �R���X�g���N�^ ( LWString ����쐬 )
    RefString( const RefWString str_ );

    /// ������Z�q ( char* ���� )
    RefString& operator = ( const char* str_ );

    /// ������Z�q ( wchar_t* ���� )
    RefString& operator = ( const wchar_t* str_ );

    /// ������Z�q ( LWString ���� )
    RefString& operator = ( const RefWString str_ );

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      �������̎擾
    //
    //  @par
    //              strlen() �Ɠ������A�I�[�� '\0' �͊܂݂܂���B<br>
    //              ���ۂɕ����񂪊m�ۂ���Ă��Ȃ��Ă� 0 ��Ԃ��܂��B
    //              ( Cache ���̂̃L�[�Ƃ��Ďg�����ꍇ�A��r�񐔂����Ȃ����邽�� )
    //*/
    //---------------------------------------------------------------------
    int size() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ������ւ̃|�C���^�̎擾
    //*/
    //---------------------------------------------------------------------
    //char* c_str();

    /// const �ŁBHash �Ŏg���̂ňꉞ�p��
    const char* c_str() const;

    //---------------------------------------------------------------------
	///**
    //  @brief      �������w�肵�ĕ�������쐬����
    //*/
    //---------------------------------------------------------------------
    void format( const char* format_, ... );

    /// �N���A
    void clear();

    /// �󕶎��񂩂𔻒肷��
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
// �� RefWString �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      ���C�h������̎Q�ƃN���X
//*/
//=============================================================================
class RefWString
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���X�g���N�^ ( �󂯎������������R�s�[���č쐬 )
    // 
    //  @param[in]  str_    : �R�s�[���̕�����
    //*/
    //---------------------------------------------------------------------
    RefWString( const wchar_t* str_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���X�g���N�^ ( �������w�� )
    // 
    //  @param[in]  str_    : �R�s�[���̕�����
    //  @param[in]  length_ : �R�s�[���钷�� ( -1 �� \0 �܂ŃR�s�[ )
    //*/
    //---------------------------------------------------------------------
    RefWString( const wchar_t* str_, int length_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���X�g���N�^ ( �R�s�[�J�n�C���f�b�N�X�ƒ������w�� )
    // 
    //  @param[in]  str_    : �R�s�[���̕�����
    //  @param[in]  begin_  : �R�s�[���̐擪�C���f�b�N�X
    //  @param[in]  length_ : �R�s�[���钷�� ( -1 �� \0 �܂ŃR�s�[ )
    //*/
    //---------------------------------------------------------------------
    RefWString( const wchar_t* str_, int begin_, int length_ );

    /// �R���X�g���N�^ ( �}���`�o�C�g�����񂩂�쐬�E�󂯎������������R�s�[���č쐬 )
    RefWString( const char* str_ );

    /// �R���X�g���N�^ ( �}���`�o�C�g�����񂩂�쐬�E�������w�� )
    RefWString( const char* str_, int length_ );

    /// �R���X�g���N�^ ( �}���`�o�C�g�����񂩂�쐬�E�R�s�[�J�n�C���f�b�N�X�ƒ������w�� )
    RefWString( const char* str_, int begin_, int length_ );

    /// �R���X�g���N�^ ( LString ����쐬 )
    RefWString( const RefString str_ );

    /// ������Z�q ( wchar_t* ���� )
    RefWString& operator = ( const wchar_t* str_ );

    /// ������Z�q ( char* ���� )
    RefWString& operator = ( const char* str_ );

    /// ������Z�q ( LString ���� )
    RefWString& operator = ( const RefString str_ );

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      �������̎擾
    //
    //  @par
    //              strlen() �Ɠ������A�I�[�� '\0' �͊܂݂܂���B
    //*/
    //---------------------------------------------------------------------
    int size() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ������ւ̃|�C���^�̎擾
    //*/
    //---------------------------------------------------------------------
    //wchar_t* c_str();

    /// const �ŁBHash �Ŏg���̂ňꉞ�p��
    const wchar_t* c_str() const;

    //---------------------------------------------------------------------
	///**
    //  @brief      �������w�肵�ĕ�������쐬����
    //*/
    //---------------------------------------------------------------------
    void format( const wchar_t* format_, ... );

    /// �N���A
    void clear();

    /// �󕶎��񂩂𔻒肷��
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