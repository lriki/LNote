//=============================================================================
//�y Manager �z
//-----------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include <list>
#include <string>
#include <stack>
#include "../Thread/Mutex.h"
#include "../Thread/EventFlag.h"
#include "../Thread/Thread.h"
#include "Interface.h"
#include "StreamObject.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File
{
class Archive;



//=============================================================================
// �� Manager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�t�@�C�����o�͂̊Ǘ��N���X
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
    , public IManager
{
public:

	/// �������f�[�^
	struct InitData
	{
        Base::LogFile*  LogFile;        ///< ���O���o�͂��郍�O�t�@�C���I�u�W�F�N�g
        LNArchiveOpenDataList   ArchiveList;    ///< �A�[�J�C�u�̐ݒ�f�[�^�̃��X�g
		LNFileAccessMode        AccessMode;     ///< �t�@�C���A�N�Z�X���[�h
	};

public:

    Manager();

	virtual ~Manager();

    LN_REFOBJ_METHODS;

public:

    /// ������
    void initialize( const InitData& init_data_ );

    /// �I������
    void finalize();

    /// �ǂݍ��ݗp�J�����g�p�X�̐ݒ� (�ʂ̃f�B���N�g������f�t�H���g�ւ̃p�X��n���Ă� "" �ɂȂ�Ȃ��o�O����)
	virtual void setCurrentPath( const lnChar* directory_ );

    /// �J�����g�p�X�̎擾
    const lnChar* getCurrentPath() const { return mCurrentPath.c_str(); }

    /// filename_ �ɃA�N�Z�X���邽�߂̐�΃p�X�̎擾 (���C�u�����V�X�e�����̐�΃p�X�B�擪�� '$' ���t���B����'$' �����Ă���ꍇ�͂��̂܂ܕԂ�)
    LRefTString getAbsolutePath( const lnChar* filename_ );

    /// InFile �̍쐬
	virtual LNRESULT createInFile( IInFile** file_, const lnChar* filename_, LNFileOpenMode mode_ = LN_OPENMODE_BINARY );

    /// OutFile �̍쐬
	virtual LNRESULT createOutFile( IOutFile** file_, const lnChar* filename_, const void* buffer_, int size_ );

    /// �ǂݍ��ݗp�J�����g�p�X�̈ړ�
	virtual void moveCurrentPath( const lnChar* directory_ );

    /// �ǂݍ��ݗp�J�����g�p�X�̕��A
	virtual void returnCurrentPath();

    /// �t�@�C���A�N�Z�X�̃��b�N (moveCurrentPath() �ł̕ύX��r���I�ɂ��邽�߂Ɏg��)
	virtual void lock();

    /// �t�@�C���A�N�Z�X�̃A�����b�N
	virtual void unlock();

    /// �񓯊������̃��N�G�X�g
	virtual void requestASyncProcess( ASyncProcessObject* obj_ );

    /// �L���[�ɂ���񓯊������̃��N�G�X�g���O��
	virtual void removeASyncProcess( ASyncProcessObject* obj_ );

public:

    /// �|�[�����O (LoadThread �N���X����Ă΂��)
	void polling();

    /// �I���v�������邩�𔻒肷�� (���[�f�B���O�X���b�h���I���`�F�b�N���邽�߂ɌĂ�)
	bool isEndRequested() const { return mEndFlag->isTrue(); }

private:

    /// ���[�f�B���O�������Ăяo���X���b�h
    class LoadThread
        : public Thread::ThreadObject
	{
	public:

		/// �R���X�g���N�^
		LoadThread( Manager* manager_ )
			: mManager( manager_ )
		{}

		/// �f�X�g���N�^
		~LoadThread()
		{
			wait();
			mManager = NULL;
		}

	private:

		virtual void run()
		{
			while ( mManager->isEndRequested() == false )
			{
				mManager->polling();
			}
		}
		Manager* mManager;
	};

	typedef std::list< ASyncProcessObject* >    ASyncProcessList;
    typedef std::vector< IArchive* >            ArchiveList;

private:

    Base::LogFile*                  mLogFile;           ///< ���O���o�͂��郍�O�t�@�C���I�u�W�F�N�g
    Archive*                        mArchives;
	int					            mArchiveNum;

    ArchiveList                     mArchiveList;       ///< ���Ԃ�����Ƃ��� mArchives �͂������Ɋ܂߂�

    Thread::Mutex*                  mLoadThreadLock;    ///< mASyncProcessRequests �ւ̃A�N�Z�X�u���b�N�p
    Thread::EventFlag*              mEndFlag;
	LoadThread*			            mLoadThread;
    Thread::Mutex*                  mLock;

    ASyncProcessList                mASyncProcessRequests;

    ln_std_tstring                  mCurrentPath;       ///< ���z�J�����g�f�B���N�g�� (EXE�̃f�B���N�g���� ""�ŁA����ȊO�͏I�[���K�� \\)
    std::stack< ln_std_tstring >    mPathStack;
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