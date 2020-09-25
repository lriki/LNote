//=============================================================================
//【 Task 】
//-----------------------------------------------------------------------------
///**
//  @file       Task.h
//  @brief      Task
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Base/Delegate.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Task
{

//=============================================================================
// ■ TaskBase クラス
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class TaskBase
{
public:

    TaskBase( int phase_ )
        : mTaskExecutePhase( phase_ )
    {}

    virtual ~TaskBase() {}

public:

    /// 実行
    virtual void execute() = 0;

protected:

    int mTaskExecutePhase;      ///< (マスタータスクの場合は待機するフェーズ)

    friend class Manager;
};

//=============================================================================
// ■ DelegateTask クラス
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class DelegateTask
    : public TaskBase
{
public:

    DelegateTask( Base::Delegate00 method_, int phase_ )
        : TaskBase  ( phase_ )
        , mMethod   ( method_ )
    {}

    virtual ~DelegateTask() {}

public:

    /// 実行
    virtual void execute() { mMethod.call(); }

private:

    Base::Delegate00   mMethod;
};

#if 0
//=============================================================================
// ■ MasterTask クラス
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class MasterTask
    : public TaskBase
{
public:

    MasterTask( Base::Delegate00& method_, int sync_phase_ )
        : TaskBase  ( sync_phase_ )
        , mMethod   ( method_ )
        , mWaiting  ( false )
        , mStartRequested( false )
    {}

    virtual ~MasterTask() {}

public:

    /// 実行 (無限ループ)
    virtual void execute();

    void startRequest() { mStartRequested.setTrue(); mWaiting.setFalse(); }

    void waitForTaskWait() { mWaiting.wait(); }

    void endRequest() { mEndRequested.setFalse(); }


private:

    Base::Delegate00&   mMethod;
    Thread::EventFlag   mWaiting;
    Thread::EventFlag   mStartRequested;
    Thread::EventFlag   mEndRequested;
};
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Task
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================