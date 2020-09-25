//=============================================================================
//【 Application 】
//-----------------------------------------------------------------------------
///**
//  @file       Application.h
//  @brief      Application
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <list>
#include "../Thread/Thread.h"
#include "../Thread/EventFlag.h"
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
class X11WindowBase;

//=============================================================================
// ■ X11Application クラス
//-----------------------------------------------------------------------------
///**
//  @brief		ひとつのアプリケーション内で必要な共通処理
//*/
//=============================================================================
class X11Application
    : public ApplicationBase
{
public:

    X11Application();

	virtual ~X11Application();

protected:

    /// メインウィンドウを作る
    virtual LNRESULT onCreateMainWindow( WindowBase** window_ );

    /// ダミーウィンドウを作る (Manager::initialize() にウィンドウハンドルが渡されたときに onCreateMainWindow() の代わりに呼ばれる)
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

private:

    Display*        mXDisplay;
    X11WindowBase*  mX11MainWindow;
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