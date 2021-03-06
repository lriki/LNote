//=============================================================================
//【 ITaskExecuteHandler 】
//-----------------------------------------------------------------------------
///**
//  @file       ITaskExecuteHandler.h
//  @brief      ITaskExecuteHandler
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
namespace Task
{

//=============================================================================
// ■ ITaskExecuteHandler クラス
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class ITaskExecuteHandler
{
public:

    virtual ~ITaskExecuteHandler() {}

public:
    
    /// メインスレッドのフレーム待ち処理を行う
    virtual void OnFrameWait() = 0;

    /// 描画情報の構築を行う
    virtual void OnUpdateDrawContents() = 0;
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