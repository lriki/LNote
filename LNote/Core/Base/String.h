//=============================================================================
//�y String �z
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
// �� String �N���X
//-----------------------------------------------------------------------------
///**
// @brief       �Q�ƃJ�E���g�t��������N���X (char)
//*/
//=============================================================================
class String
    : public ReferenceObject
{
public:

	/// �f�t�H���g�R���X�g���N�^ ( ��̕����� "" �ō쐬 )
	String();

	/// �R���X�g���N�^ ( ������ݒ� )
	String( const char* str_ );

	/// �R���X�g���N�^ ( �����w�� )
	String( const char* str_, int length_ );

	/// �R���X�g���N�^ ( �R�s�[�J�n�C���f�b�N�X�ƒ������w�� )
	String( const char* str_, int begin_, int length_ );

	/// �R���X�g���N�^ ( ���C�h�����񂩂�쐬�E������ݒ� )
	String( const wchar_t* str_ );

	/// �R���X�g���N�^ ( ���C�h�����񂩂�쐬�E�����w�� )
	String( const wchar_t* str_, int length_ );

	/// �R���X�g���N�^ ( ���C�h�����񂩂�쐬�E�R�s�[�J�n�C���f�b�N�X�ƒ������w�� )
	String( const wchar_t* str_, int begin_, int length_ );

//protected:

	/// �f�X�g���N�^ 
	virtual ~String();

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      �������̎擾
    //
    //  @par
    //              strlen() �Ɠ������A�I�[�� '\0' �͊܂݂܂���B
    //*/
    //---------------------------------------------------------------------
    int size() const { return mSize; }

    //---------------------------------------------------------------------
	///**
    //  @brief      ������ւ̃|�C���^�̎擾
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
// �� WString �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �Q�ƃJ�E���g�t��������N���X (wchar_t)
//*/
//=============================================================================
class WString
    : public ReferenceObject
{
public:

	/// �f�t�H���g�R���X�g���N�^ ( ��̕����� L"" �ō쐬 )
	WString();

	/// �R���X�g���N�^ ( ������ݒ� )
	WString( const wchar_t* str_ );

	/// �R���X�g���N�^ ( �����w�� )
	WString( const wchar_t* str_, int length_ );

	/// �R���X�g���N�^ ( �R�s�[�J�n�C���f�b�N�X�ƒ������w�� )
	WString( const wchar_t* str_, int begin_, int length_ );

	/// �R���X�g���N�^ ( �}���`�o�C�g�����񂩂�쐬�E������ݒ� )
	WString( const char* str_ );

	/// �R���X�g���N�^ ( �}���`�o�C�g�����񂩂�쐬�E�����w�� )
	WString( const char* str_, int length_ );

	/// �R���X�g���N�^ ( �}���`�o�C�g�����񂩂�쐬�E�R�s�[�J�n�C���f�b�N�X�ƒ������w�� )
	WString( const char* str_, int begin_, int length_ );

protected:

	/// �f�X�g���N�^
	virtual ~WString();

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      �������̎擾
    //
    //  @par
    //              strlen() �Ɠ������A�I�[�� '\0' �͊܂݂܂���B
    //*/
    //---------------------------------------------------------------------
    int size() const { return mSize; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      ������ւ̃|�C���^�̎擾
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