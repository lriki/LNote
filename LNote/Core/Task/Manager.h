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
#include <queue>
#include "../Thread/Thread.h"
#include "../Thread/Barrier.h"
#include "Task.h"
#include "ITaskExecuteHandler.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
static const int LN_EXTERNAL_SUB_TASK_EXECUTE_PHASE = -1;

namespace Task
{

//=============================================================================
// �� Manager
//-----------------------------------------------------------------------------
///**
//  @brief		
//
//  @note
//              �Ƃ肠�����o���A�����̒P�ʂ��u�t�F�[�Y�v�ƌĂ�ł݂�B
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

    Manager();

    virtual ~Manager();

public:

    /// ������
    void initialize(ITaskExecuteHandler* handler_);

    /// �I������
    void finalize();

    /// �R�}���h�̍쐬�J�n (�S�p�C�v���C���̏I���҂����s���B�R�}���h���Ă������O�ς��邩���c)
    void beginMakeCommand();

    /// �R�}���h�̍쐬�I��
    void endMakeCommand();

    /// ���s
    void execute();

    /// �Œ�^�X�N�ǉ� (�f���Q�[�g����o�^�Bphase_ �� LN_EXTERNAL_SUB_TASK_EXECUTE_PHASE ���w�肷��ƁA�����Ԃ�����`���� begin() �܂�)
    //      �Œ�^�X�N�FManager �̍X�V�����ȂǁAinitialize() ��A��x�����o�^����āA�ȍ~�폜���Ȃ��^�X�N
    void pushConstTask( Base::Delegate00 method_, int phase_ );

    /// �}�X�^�[�^�X�N�̐ݒ� (execute() �̊O�ɏo�Ă����s����Ă���^�X�N�Bsync_phase_ �͓������Ƃ�t�F�[�Y)
    //      sync_phase_ �̃t�F�[�Y�ɓ��������A���̃t�F�[�Y�̃^�X�N�����s����O��
    //      �}�X�^�[�^�X�N�̏I����҂B���̃t�F�[�Y�̎��s�O�Ƀ}�X�^�[�^�X�N���J�n����B
    void setMasterTask( Base::Delegate00 method_, Base::Delegate00 init_method_, int sync_phase_ );

    /// �}�X�^�[�^�X�N�Ŏ��s����T�u���\�b�h�̃|�X�g (���s��Apop)
    void postMasterTaskSubDelegate(Base::Delegate00 delegate_) { mMasterPipelineThread->postDelegate(delegate_); }

    //bool isMasterTaskWaiting() const { return  }

    void waitMasterTaskWait() {mMasterPipelineThread->waitForInitialized();};
    

private:

    void _executeCurrentPhase( int current_phase_ );

    /// �^�X�N�̃\�[�g�֐�
    static bool _cmpTasks( const TaskBase* l_, const TaskBase* r_ );


    void _executePipeline( int pipeline_index_ );

private:

    /// PipelineThread
    class PipelineThread
        : public Thread::ThreadObject
    {
    public:

        /// pipeline_index_ �� 1 �X�^�[�g
        PipelineThread( Manager* manager_, int pipeline_index_ )
            : mManager          ( manager_ )
            , mPipelineIndex    ( pipeline_index_ )
            //, mWaiting          ( false )
        {}

        virtual void run();

        //void waitForTaskWait() { mWaiting.wait(); }
 
    private:

        Manager*    mManager;
        int         mPipelineIndex;
        //Thread::EventFlag   mWaiting;
    };

    /// MasterPipelineThread
    class MasterPipelineThread
        : public Thread::ThreadObject
    {
    public:

        MasterPipelineThread( Manager* manager_ )
            : mManager          ( manager_ )
			, mEndNotifyFlag( false )
			, mBeginNotifyFlag( false )
			, mStandbyNotifyFlag( false )
            , mWaiting          ( false )
            , mRunnning         ( false )
            , mEndRequested     ( false )
        {}

        virtual void run();
 
        /// �X���b�h�̏������������I�����A���C�����[�v�̐擪�őҋ@����̂��܂�
        void waitForInitialized()
		{
            mEndNotifyFlag.wait();
        }

        void waitForTaskWait()
		{
			//mWaiting.wait();
			mEndNotifyFlag.wait();
			mEndNotifyFlag.setFalse();
		}

		void enableRunning()
		{
			mBeginNotifyFlag.setTrue();
			mStandbyNotifyFlag.wait();
			mStandbyNotifyFlag.setFalse();
			//mMutex.lock();
			//mRunnning.setTrue();
			//mWaiting.setFalse();
			//mMutex.unlock();
		}
  //      void waitForTaskWaitAndWaitNotify()
		//{
		//	//mMutex.lock();
		//	//mRunnning.setFalse();
		//	mWaiting.wait();
		//	//mMutex.unlock();
		//}

        void endRequest()
		{ 
			mEndRequested.setTrue(); 
			//waitForTaskWait(); 
			//waitForTaskWaitAndWaitNotify();
			mStandbyNotifyFlag.setTrue();
			enableRunning(); 
		}

        void postDelegate(Base::Delegate00 delegate_)
        {
            mDelegateQueueMutex.lock();
            mDelegateQueue.push(delegate_);
            mDelegateQueueMutex.unlock();
        }
        
        bool isWaiting() { return mEndNotifyFlag.isTrue(); }

	private:

		// ��
		void disableRunning() { mRunnning.setFalse(); }
		

    private:

		Thread::EventFlag   mEndNotifyFlag;
        Thread::EventFlag   mBeginNotifyFlag;
        Thread::EventFlag   mStandbyNotifyFlag;

        Manager*            mManager;
        Thread::EventFlag   mWaiting;
        Thread::EventFlag   mRunnning;
        Thread::EventFlag   mEndRequested;
		Thread::Mutex		mMutex;

        Thread::Mutex		mDelegateQueueMutex;
        std::queue<Base::Delegate00>   mDelegateQueue;
    };

private:

    typedef std::vector< TaskBase* >        TaskArray;
    typedef std::vector< TaskArray >        TaskPipelineArray;
    typedef std::vector< PipelineThread* >  PipelineThreadArray;

private:

    ITaskExecuteHandler*    mHandler;
    int                 mPipelineNum;           ///< ���C���X���b�h���܂ރp�C�v���C����

    TaskArray           mTaskArray;
    TaskArray           mConstTaskArray;
    TaskArray           mExternalConstTaskArray;
    TaskPipelineArray   mTaskPipelineArray;     ///< ���C���X���b�h���܂ށA�p�C�v���C�����Ƃ̎��s�^�X�N�z��
    PipelineThreadArray mPipelineThreadArray;   ///< ���C���X���b�h�ȊO�̃X���b�h�z��
    TaskBase*               mMasterTask;
    MasterPipelineThread*   mMasterPipelineThread;
    Base::Delegate00        mMasterTaskInitializeMethod;
    //Thread::EventFlag*  mMasterTaskWaiting;

    Thread::Barrier*    mPipelineBarrier;
    Thread::EventFlag*  mEndRequested;
    Thread::EventFlag*  mTaskRunning;

    Thread::EventBarrier*   mPipelineEventBarrier;

    friend class PipelineThread;

public:

    /// (OpenGL Shader �p�b��Ή�)
    MasterPipelineThread* getMasterPipelineThread() { return mMasterPipelineThread; }
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Task
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================