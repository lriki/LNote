//=============================================================================
//【 Manager 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <algorithm>
#include "Manager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Task
{
#define TRACE( ... ) //printf( __VA_ARGS__ )

//=============================================================================
// ■ Manager クラス
//=============================================================================

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    Manager::Manager()
        : mHandler          ( NULL )
        , mPipelineNum      ( 2 )
        , mMasterTask       ( NULL )
        , mMasterPipelineThread ( NULL )
        //, mMasterTaskWaiting    ( NULL )
        , mPipelineBarrier  ( NULL )
        , mEndRequested     ( NULL )
        , mTaskRunning     ( NULL )
        , mPipelineEventBarrier ( NULL )
    {
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    Manager::~Manager()
    {
        
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::initialize(ITaskExecuteHandler* handler_)
    {
        mHandler = handler_;

        mMasterPipelineThread = LN_NEW MasterPipelineThread( this );
        mPipelineBarrier = LN_NEW Thread::Barrier( mPipelineNum );
        mEndRequested = LN_NEW Thread::EventFlag( false );
        mTaskRunning = LN_NEW Thread::EventFlag( false );
        mPipelineEventBarrier = LN_NEW Thread::EventBarrier( 1 );

        mPipelineThreadArray.push_back( LN_NEW PipelineThread( this, 1 ) );
        mTaskPipelineArray.resize( mPipelineNum );

        ln_foreach( PipelineThread* t, mPipelineThreadArray )
        {
            t->start(); // 開始直後、バリアで待機状態になる
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::finalize()
    {
        _p(1);
        if ( mEndRequested )
        {
            mEndRequested->setTrue();_p(2);
            mPipelineBarrier->setNotifyFlag( true );_p(2);
            mPipelineEventBarrier->setNotifyFlag( true );_p(2);
            mTaskRunning->setTrue();_p(2);

            ln_foreach( PipelineThread* t, mPipelineThreadArray )
            {
                t->wait();_p(t);
            }

            LN_SAFE_RELEASE( mPipelineBarrier );_p(2);
            LN_SAFE_RELEASE( mEndRequested );_p(2);
            LN_SAFE_RELEASE( mTaskRunning );_p(2);
            LN_SAFE_RELEASE( mPipelineEventBarrier );_p(2);
            
        }
_p(6);
        if ( mMasterPipelineThread )
        {
            mMasterPipelineThread->endRequest();_p(3);
            mMasterPipelineThread->wait();_p(3);
            SAFE_DELETE( mMasterPipelineThread );_p(3);
        }
_p(4);
        // 固定タスク
        ln_foreach( TaskBase* t, mConstTaskArray )
        {
            SAFE_DELETE( t );_p(t);
        }
        mConstTaskArray.clear();
_p(5);
        ln_foreach( TaskBase* t, mExternalConstTaskArray )
        {
            SAFE_DELETE( t );_p(t);
        }
        mExternalConstTaskArray.clear();
_p(7);
        // サブスレッド
        ln_foreach( PipelineThread* t, mPipelineThreadArray )
        {
            SAFE_DELETE( t );_p(t);
        }
        _p(7);
        mPipelineThreadArray.clear(); 
        _p(5);
        SAFE_DELETE( mMasterTask );

        _p(1456465655);
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::beginMakeCommand()
    {
        mPipelineEventBarrier->waitMain();
        //mPipelineBarrier->wait();   // ここから execute() まで、メインスレッドとマスタータスク以外は空の状態で実行される
        mTaskArray.clear();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::endMakeCommand()
    {
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::execute()
    {
        if ( mTaskArray.empty() && mConstTaskArray.empty() ) return;

        TRACE( "---- execute begin ----\n" );
        // 固定タスクを追加して、実行フェーズでソート
        mTaskArray.insert( mTaskArray.end(), mConstTaskArray.begin(), mConstTaskArray.end() );
        std::sort( mTaskArray.begin(), mTaskArray.end(), _cmpTasks );

        int current_phase = mTaskArray.front()->mTaskExecutePhase;
        int pipeline_index = 0;
        TaskArray::iterator itr = mTaskArray.begin();
        TaskArray::iterator end = mTaskArray.end();
        for ( ; itr != end; ++itr )
        {
            if ( current_phase == (*itr)->mTaskExecutePhase )
            {
                mTaskPipelineArray[ pipeline_index ].push_back( *itr );
                pipeline_index = ++pipeline_index % mPipelineNum;
            }
            // 違うフェーズが見つかったら、そこまでプッシュしたタスクを実行
            else
            {
                _executeCurrentPhase( current_phase );

                current_phase = (*itr)->mTaskExecutePhase;
            }
        }

        // 最後の分
        _executeCurrentPhase( current_phase );


        //-----------------------------------------------------
        // 同期フェーズ
        
        mHandler->OnFrameWait();

        // 描画が終了している場合
        if (mMasterPipelineThread->isWaiting())
        {
            mMasterPipelineThread->waitForTaskWait();   // 旧との整合性確保のため
            mHandler->OnUpdateDrawContents();
            mMasterPipelineThread->enableRunning();
        }

        //-----------------------------------------------------
        // 

        // 外部並列実行タスクを準備
		//		(ユーザーに制御を返した後も、メインスレッド以外で動作するタスク。
		//		メインスレッドのパイプライン以外に詰める)
        mPipelineEventBarrier->waitMain();
        pipeline_index = 0;
        ln_foreach( TaskBase* t, mExternalConstTaskArray )
        {
            mTaskPipelineArray[ 1 + pipeline_index ].push_back( t );
            pipeline_index = ++pipeline_index % (mPipelineNum - 1);
        }

        mPipelineEventBarrier->notifyAllSub();

        // 最後の _executeCurrentPhase() でマスタータスクが停止したとき用
        //mMasterPipelineThread->enableRunning();

        TRACE( "---- execute end ----\n" );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::pushConstTask( Base::Delegate00 method_, int phase_ )
    {
        if ( phase_ == LN_EXTERNAL_SUB_TASK_EXECUTE_PHASE )
        {
            mExternalConstTaskArray.push_back( LN_NEW DelegateTask( method_, phase_ ) );
        }
        else
        {
            mConstTaskArray.push_back( LN_NEW DelegateTask( method_, phase_ ) );
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::setMasterTask( Base::Delegate00 method_, Base::Delegate00 init_method_, int sync_phase_ )
    {
        //printf("setMasterTask s\n");
        SAFE_DELETE( mMasterTask );
        mMasterTaskInitializeMethod = init_method_;
        mMasterTask = LN_NEW DelegateTask( method_, sync_phase_ );
        //mMasterPipelineThread->enableRunning();
        //printf("start s\n");
        mMasterPipelineThread->start();
		//mMasterPipelineThread->waitForTaskWait();
        //printf("waitForInitialized s\n");
        mMasterPipelineThread->waitForInitialized();
        //printf("waitForInitialized e\n");
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::_executeCurrentPhase( int current_phase_ )
    {
        TRACE( "    < Task Execute Phase %d >\n", current_phase_ );
#if 1
        //if ( mMasterTask->mTaskExecutePhase == current_phase_ )
        //{
        //    mHandler->OnFrameWait();

        //    // 描画が終了している場合
        //    if (mMasterPipelineThread->isWaiting())
        //    {
        //        mHandler->OnUpdateDrawContents();
        //    }
        //}

        // メインタスクが入ることで全パイプラインを実行状態にする
        mPipelineEventBarrier->notifyAllSub();

        // メインスレッドは 0 番パイプラインを実行
        _executePipeline( 0 );

#else
        // マスタータスクの待機フェーズの場合は、マスタータスクが待機するまで待つ
        if ( mMasterTask->mTaskExecutePhase == current_phase_ )
        {
            //mMasterPipelineThread->disableRunning();
            mMasterPipelineThread->waitForTaskWait();
        }
        // マスタータスク開始 
        //      ホントはひとつ前のフェーズで停止したかを保持して判定する方がいいけど、
        //      今のところは問題なのでこのまま。
        else
        {
            mMasterPipelineThread->enableRunning();
        }

        // メインタスクが入ることで全パイプラインを実行状態にする
        mPipelineEventBarrier->notifyAllSub();

        // メインスレッドは 0 番パイプラインを実行
        _executePipeline( 0 );
#endif
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    bool Manager::_cmpTasks( const TaskBase* l_, const TaskBase* r_ )
    {
        // 昇順
        return l_->mTaskExecutePhase < r_->mTaskExecutePhase;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::_executePipeline( int pipeline_index_ )
    {
        TRACE( "_executePipeline(%d)\n", pipeline_index_ );

        TaskArray& list = mTaskPipelineArray[ pipeline_index_ ];
        ln_foreach( TaskBase* t, list )
        {
            t->execute();
        }
        list.clear();
    }

//=============================================================================
// ■ PipelineThread
//=============================================================================

    void Manager::PipelineThread::run()
    {
        while ( true )
        {

            //mWaiting.setTrue();
            //mManager->mPipelineBarrier->wait();
            mManager->mPipelineEventBarrier->waitSub();
            //mWaiting.setFalse();

            if ( mManager->mEndRequested->isTrue() ) return;

            mManager->_executePipeline( mPipelineIndex );
        }
    }

//=============================================================================
// ■ MasterPipelineThread
//=============================================================================

    void Manager::MasterPipelineThread::run()
    {
        try
        {
            if (!mManager->mMasterTaskInitializeMethod.isEmpty())
            {
                mManager->mMasterTaskInitializeMethod.call();
            }
		    //mRunnning.setFalse();

            int state = 0;

            while ( true )
            {
    			switch (state)
                {
                    // 待機
                    case 0:
                    {
                        mEndNotifyFlag.setTrue();

                        while (!mBeginNotifyFlag.isTrue())
                        {
                            mDelegateQueueMutex.lock();
                            while (!mDelegateQueue.empty())
                            {
                                mDelegateQueue.front().call();
                                mDelegateQueue.pop();
                            }
                            mDelegateQueueMutex.unlock();

                            Thread::sleep(1);
                        }
                        mBeginNotifyFlag.setFalse();

                        state = 1;

                        mStandbyNotifyFlag.setTrue();

                        if ( mEndRequested.isTrue() ) return;
                        break;
                    }
                    // 実行(描画)
                    case 1:
                    {
                        mManager->mMasterTask->execute();
                        ::Sleep(40);
                        state = 0;
                        break;
                    }
                }
                
                
    			
			    ////mMutex.lock();
			    //mRunnning.setFalse();
			    //mWaiting.setTrue();
       //         mRunnning.wait();
			    //mWaiting.setFalse();
    			
			    //mMutex.unlock();
                //mStartRequested.setFalse();

                /*
			    mEndNotifyFlag.setTrue();

			    mBeginNotifyFlag.wait();
			    mBeginNotifyFlag.setFalse();

			    mStandbyNotifyFlag.setTrue();
                

                if ( mEndRequested.isTrue() ) return;
     
                mManager->mMasterTask->execute();
                */
            }
        }
        catch ( Base::Exception e )
        {
            e.process();
        }
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Task
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================