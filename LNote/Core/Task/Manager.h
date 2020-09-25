//=============================================================================
//【 Manager 】
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
// ■ Manager
//-----------------------------------------------------------------------------
///**
//  @brief		
//
//  @note
//              とりあえずバリア同期の単位を「フェーズ」と呼んでみる。
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

    Manager();

    virtual ~Manager();

public:

    /// 初期化
    void initialize(ITaskExecuteHandler* handler_);

    /// 終了処理
    void finalize();

    /// コマンドの作成開始 (全パイプラインの終了待ちも行う。コマンドっていう名前変えるかも…)
    void beginMakeCommand();

    /// コマンドの作成終了
    void endMakeCommand();

    /// 実行
    void execute();

    /// 固定タスク追加 (デリゲートから登録。phase_ に LN_EXTERNAL_SUB_TASK_EXECUTE_PHASE を指定すると、制御を返した後～次の begin() まで)
    //      固定タスク：Manager の更新処理など、initialize() 後、一度だけ登録されて、以降削除しないタスク
    void pushConstTask( Base::Delegate00 method_, int phase_ );

    /// マスタータスクの設定 (execute() の外に出ても実行されているタスク。sync_phase_ は同期をとるフェーズ)
    //      sync_phase_ のフェーズに入った時、そのフェーズのタスクを実行する前に
    //      マスタータスクの終了を待つ。次のフェーズの実行前にマスタータスクを開始する。
    void setMasterTask( Base::Delegate00 method_, Base::Delegate00 init_method_, int sync_phase_ );

    /// マスタータスクで実行するサブメソッドのポスト (実行後、pop)
    void postMasterTaskSubDelegate(Base::Delegate00 delegate_) { mMasterPipelineThread->postDelegate(delegate_); }

    //bool isMasterTaskWaiting() const { return  }

    void waitMasterTaskWait() {mMasterPipelineThread->waitForInitialized();};
    

private:

    void _executeCurrentPhase( int current_phase_ );

    /// タスクのソート関数
    static bool _cmpTasks( const TaskBase* l_, const TaskBase* r_ );


    void _executePipeline( int pipeline_index_ );

private:

    /// PipelineThread
    class PipelineThread
        : public Thread::ThreadObject
    {
    public:

        /// pipeline_index_ は 1 スタート
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
 
        /// スレッドの初期化処理が終了し、メインループの先頭で待機するのをまつ
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

		// 旧
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
    int                 mPipelineNum;           ///< メインスレッドも含むパイプライン数

    TaskArray           mTaskArray;
    TaskArray           mConstTaskArray;
    TaskArray           mExternalConstTaskArray;
    TaskPipelineArray   mTaskPipelineArray;     ///< メインスレッドも含む、パイプラインごとの実行タスク配列
    PipelineThreadArray mPipelineThreadArray;   ///< メインスレッド以外のスレッド配列
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

    /// (OpenGL Shader 用暫定対応)
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