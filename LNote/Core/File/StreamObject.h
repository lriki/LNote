//=============================================================================
//�y StreamObject �z
//-----------------------------------------------------------------------------
///**
//  @file       StreamObject.h
//  @brief      StreamObject
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File
{

//=============================================================================
// �� InFileStream �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �P���ȓ��̓t�@�C���̃N���X (�o�C�i���`��)
//*/
//=============================================================================
class InFileStream
    : public IInStream
    , public Base::ReferenceObject
{
public:

    /// �R���X�g���N�^
	InFileStream();

	/// �f�X�g���N�^
	virtual ~InFileStream();

    LN_REFOBJ_METHODS;

public:

    /// ������
    void initialize( const lnChar* filename_ );

	/// �t�@�C�� ( �f�[�^ ) �T�C�Y�̎擾
    virtual int getSize() { return mSize; }

	/// �t�@�C���|�C���^�̈ʒu�̎擾
	virtual int getPosition() { return ftell( mStream ); }

    /// �f�[�^�̓ǂݍ���
	virtual int read( void* buffer_, int buffer_size_, int read_size_ );

	/// �t�@�C���|�C���^�̐ݒ�
	virtual void seek( int offset_, int origin_ );

    

private:

    /// �t�@�C�������
    void _close();

private:

    FILE*   mStream;
    int     mSize;
};

//=============================================================================
// �� InManagedMemoryStream �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      ��������ɓW�J���ꂽ�f�[�^����̓X�g���[���Ƃ��Ĉ����N���X
//
//  @note
//              initialize() �Ŏ󂯎�����f�[�^�́A����Ɠ����傫����
//              ���������m�ۂ��ăR�s�[���܂��B<br>
//              ���̃������� initialize() �̒���ɉ�����邱�Ƃ��ł��܂��B
//*/
//=============================================================================
class InManagedMemoryStream
    : public IInStream
    , public Base::ReferenceObject
{
public:

	InManagedMemoryStream();

	virtual ~InManagedMemoryStream();

    LN_REFOBJ_METHODS;

public:

    /// ������
    void initialize( const void* buffer_, int size_ );

	/// �t�@�C�� ( �f�[�^ ) �T�C�Y�̎擾
    virtual int getSize() { return mSize; }

	/// �t�@�C���|�C���^�̈ʒu�̎擾
    virtual int getPosition() { return mSeekPoint; }

    /// �f�[�^�̓ǂݍ��� ( read_size_ = -1 �ł��ׂẴf�[�^��ǂݍ��� )
	virtual int read( void* buffer_, int buffer_size_, int read_size_ );

	/// �t�@�C���|�C���^�̐ݒ�
	virtual void seek( int offset_, int origin_ );

private:

    void _dispose();

private:

    int     mSize;          ///< �f�[�^�T�C�Y
    char*   mBuffer;        ///< �f�[�^�{��
    int     mSeekPoint;     ///< �V�[�N�ʒu

};

//=============================================================================
// �� InUnManagedMemoryStream �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      ��������ɓW�J���ꂽ�f�[�^����̓X�g���[���Ƃ��Ĉ����N���X
//*/
//=============================================================================
class InUnManagedMemoryStream
    : public IInStream
    , public Base::ReferenceObject
{
public:

	InUnManagedMemoryStream();

	virtual ~InUnManagedMemoryStream();

    LN_REFOBJ_METHODS;

public:

    /// ������
    void initialize( const void* buffer_, int size_ );

	/// �t�@�C�� ( �f�[�^ ) �T�C�Y�̎擾
    virtual int getSize() { return mSize; }

	/// �t�@�C���|�C���^�̈ʒu�̎擾
    virtual int getPosition() { return mSeekPoint; }

    /// �f�[�^�̓ǂݍ��� ( read_size_ = -1 �ł��ׂẴf�[�^��ǂݍ��� )
	virtual int read( void* buffer_, int buffer_size_, int read_size_ );

	/// �t�@�C���|�C���^�̐ݒ�
	virtual void seek( int offset_, int origin_ );

private:

    int             mSize;          ///< �f�[�^�T�C�Y
    const lnByte*   mBuffer;        ///< �f�[�^�{��
    int             mSeekPoint;     ///< �V�[�N�ʒu

};



class IArchive
{
public:

    /// ���̓X�g���[���̍쐬
    virtual IInStream* createStream( const lnChar* filename_ ) = 0;
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace File
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================