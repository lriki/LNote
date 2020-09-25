//=============================================================================
//�y InFile �z
//-----------------------------------------------------------------------------
///**
//  @file       InFile.h
//  @brief      InFile
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Interface.h"
#include "ASyncProcessObject.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File
{
class Manager;

//=============================================================================
// �� InFile �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �ǂݍ��ݗp�̃t�@�C���������N���X
//*/
//=============================================================================
class InFile
    : public Base::ReferenceObject
    , public IInFile
    , public ASyncProcessObject
{
public:

    /// �R���X�g���N�^
	InFile( Manager* manager_, IInStream* stream_ );

	/// �f�X�g���N�^
	virtual ~InFile();

public:

    /// �t�@�C���T�C�Y�̎擾
    virtual int getSize() { return mInStream->getSize(); }

    /// �t�@�C���|�C���^�̈ʒu�̎擾
	virtual int getPosition() { return mInStream->getPosition(); }

    /// �f�[�^�̓ǂݍ���
    virtual int read( void* buffer_, int buffer_size_, int read_size_ );

    /// �t�@�C���|�C���^�̐ݒ�
    virtual void seek( int offset_, int origin_ ) { mInStream->seek( offset_, origin_ ); }

    /// �t�@�C���̓��e��S�ēǂݍ���
	virtual void load( bool async_ );

    /// load() �œǂݍ��܂ꂽ�f�[�^�̎擾
    virtual void* getData() { return mData; }

    /// load() �ł̔񓯊��ǂݍ��݂̊����𔻒肷��
    virtual bool isLoadFinished() { return ASyncProcessObject::getProcessState() == LN_ASYNCSTATE_FINISH || mLoadFinished; } 

    /// load() �ł̓ǂݍ��݂ŃG���[�������������𔻒肷��
    virtual bool isError() { return ASyncProcessObject::isError(); }

    LN_REFOBJ_METHODS;

protected:

    /// �񓯊������R�[���o�b�N
    virtual bool operator()() { return onLoading(); }

	/// �S�f�[�^�ǂݍ���
	virtual bool onLoading();

private:
			
    IInStream*  mInStream;      ///< �ǂރX�g���[��(FILE* �Ƃ�)�������Ă�����̓X�g���[���N���X
    lnByte*     mData;          ///< load() �œǂݍ��񂾃f�[�^
    bool        mLoadFinished;  ///< �ǂݍ��݂̏I���t���O
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