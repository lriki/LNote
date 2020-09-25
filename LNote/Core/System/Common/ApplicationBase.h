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
#include "../../Thread/Mutex.h"
#include "../../Thread/Thread.h"
#include "../../Thread/EventFlag.h"
#include "../Interface.h"
#include "WindowBase.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{
class MouseCursor;

#define LN_SYSTEMTHREADMSG_MAGIC 123456789


//=============================================================================
// ■ ApplicationBase クラス
//-----------------------------------------------------------------------------
///**
//  @brief		アプリケーションの基底処理 (メッセージ処理、マウスカーソル処理)
//*/
//=============================================================================
class ApplicationBase
    : public Base::ReferenceObject
    , public Thread::ThreadObject
{
public:

	/// 初期化データ
	struct InitData
	{
        Base::LogFile*      LogFile;            ///< ログを出力するログファイルオブジェクト
		Manager*			SystemManager;
        bool			    UseConsole;         ///< コンソールを割り当てる場合 true
        bool                UseThread;          ///< メッセージ処理に専用のスレッドを使う場合は true
        void*               WindowHandle;       ///< ユーザー定義のウィンドウハンドル (windows の場合は HWND、X11 は Window*。ただし、X11 は未対応)
    
        const lnChar*	    TitleText;		///< ウィンドウタイトルの文字列
		int				    Width;			///< クライアント領域の幅
		int				    Height;			///< クライアント領域の高さ
		bool			    Windowed;		///< ウィンドウモードで初期化する場合 true
    };

public:

    ApplicationBase();

	virtual ~ApplicationBase();

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

    /// 終了処理
    void finalize();

    /// ウィンドウを取得する
    IWindow* getMainWindow() { return mMainWindow; }

    /// マウスカーソルの表示設定 (flag_ が false の場合、count だけ待ってから消す。0ですぐ消す)
    void setMouseCursorVisibleState( bool flag_, int count_ );

    /// マウスが移動したときに呼び出す
    void onMoveCursor( bool on_client_area_ );

    /// 更新
    void update();

    /// イベントメッセージをポストする
    void postMessage( Event* event_ );

    /// 処理するべきイベントメッセージがあるかを確認する (true=ある)
    bool peekMessage();

    /// 処理するべきイベントメッセージを取得する (取得した場合、キューから削除する )
    const Event* getMessage();

    /// スレッド関数
    virtual void run();

protected:

    /// メインウィンドウを作る
    virtual LNRESULT onCreateMainWindow( WindowBase** window_ ) = 0;

    /// ダミーウィンドウを作る
    virtual WindowBase* onCreateDummyWindow( void* window_handle_ ) = 0;

    /// ウィンドウの finalize を呼び出して参照カウントをひとつ減らす
    virtual void onFinalizeWindow( WindowBase* window_ ) = 0;

    /// コンソールを開く
    virtual void onAllocConsole() = 0;

    /// コンソールを閉じる
    virtual void onFreeConsole() = 0;

    /// メッセージ処理 (マウスが移動したときに onMoveCursor() を呼ぶこと)
    virtual void onProcMessage() = 0;

    /// マウスカーソルの表示状態を設定する
	virtual void onShowCursor( bool flag_ ) = 0;

private:

    /// 初期化
    LNRESULT _initialize();

    /// 終了処理
    void _finalize();

    /// 更新
    void _update();

protected:

    typedef std::list< Event* >    EventQueue;      // const Event* にすると delete できないので注意

protected:

    Base::LogFile*      mLogFile;           ///< ログを出力するログファイルオブジェクト
	Manager*			mManager;
    InitData            mInitData;
    WindowBase*         mMainWindow;		///< アプリケーションのメインウィンドウ

    EventQueue          mEventQueue;

    MouseCursor*        mMouseCursor;

    Thread::EventFlag   mFinishedInit;
    Thread::EventFlag   mEndRequest;
    Thread::Mutex       mLock;
    LNRESULT            mResultInit;
    bool                mThreaded;

    bool                mConsoleAlloced;    ///< コンソールを割り当てた場合 true
    bool                mIsSystemMouseShow;

    friend class MouseCursor;
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