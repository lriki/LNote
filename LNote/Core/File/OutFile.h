//=============================================================================
//�y OutFile �z
//-----------------------------------------------------------------------------
///**
//  @file       OutFile.h
//  @brief      OutFile
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
// �� OutFile �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �������ݗp�̃t�@�C���������N���X
//*/
//=============================================================================
class OutFile
    : public Base::ReferenceObject
    , public IOutFile
    , public ASyncProcessObject
{
public:

    /// �R���X�g���N�^
	OutFile( Manager* manager_, const lnChar* filename_, const void* buffer_, int size_ );

	/// �f�X�g���N�^
	virtual ~OutFile();

public:

    /// �f�[�^��ۑ�����
	virtual void save( bool async_ );

	/// save() �ł̔񓯊��������݂̊����𔻒肷��
	virtual bool isSaveFinished() { return ASyncProcessObject::getProcessState() == LN_ASYNCSTATE_FINISH || mSaveFinished; } 

    /// save() �ł̏������݂ŃG���[�������������𔻒肷��
	virtual bool isError() { return ASyncProcessObject::isError(); }

    LN_REFOBJ_METHODS;

protected:

    /// �񓯊������R�[���o�b�N
    virtual bool operator()() { return onSaving(); }

	/// �S�f�[�^�ǂݍ���
	virtual bool onSaving();

private:
			
    ln_std_tstring  mFilename;
    const void*     mData;
    int		        mSize;
    bool            mSaveFinished;
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