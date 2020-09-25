//=============================================================================
//【 Application 】
//-----------------------------------------------------------------------------
///**
//  @file       Application.h
//  @brief      Application
//  @version    1.0
//  @date       2012/4/30
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <list>
#include "../../Thread/Thread.h"
#include "../../Thread/EventFlag.h"
#include "../Interface.h"
#include "../Common/ApplicationBase.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{
class Win32WindowBase;

//=============================================================================
// ■ Win32Application クラス
//-----------------------------------------------------------------------------
///**
//  @brief		ひとつのアプリケーション内で必要な共通処理
//*/
//=============================================================================
class Win32Application
    : public ApplicationBase
{
public:

    Win32Application();

	virtual ~Win32Application();

protected:

    /// メインウィンドウを作る
    virtual LNRESULT onCreateMainWindow( WindowBase** window_ );

    /// ダミーウィンドウを作る
    virtual WindowBase* onCreateDummyWindow( void* window_handle_ );

    /// ウィンドウの finalize を呼び出して参照カウントをひとつ減らす
    virtual void onFinalizeWindow( WindowBase* window_ );

    /// コンソールを開く
    virtual void onAllocConsole();

    /// コンソールを閉じる
    virtual void onFreeConsole();

    /// メッセージ処理
    virtual void onProcMessage();

    /// マウスカーソルの表示状態を設定する
	virtual void onShowCursor( bool flag_ );
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace System
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================