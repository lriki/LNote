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
#include <list>
#include "../Thread/EventFlag.h"
#include "Interface.h"
#include "OS.h"

#if defined(LNOTE_ANDROID)
struct android_app;
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{
class ApplicationBase;

//=============================================================================
// ■ Manager クラス
//-----------------------------------------------------------------------------
///**
//  @brief		システム (OS 周り) 管理クラス
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
    //, public IManager
{
public:

	/// 初期化データ
	struct InitData
	{
        Base::LogFile*      LogFile;            ///< ログを出力するログファイルオブジェクト
        bool			    UseConsole;         ///< [win32] コンソールを割り当てる場合 true
        bool                UseThread;          ///< [win32] メッセージ処理に専用のスレッドを使う場合は true
        const lnChar*	    WindowTitleText;    ///< [win32] ウィンドウタイトルの文字列
        lnU32               WindowWidth;        ///< ウィンドウのクライアント領域の幅
        lnU32               WindowHeight;       ///< ウィンドウのクライアント領域の高さ
        bool			    Windowed;		    ///< [win32] ウィンドウモードで初期化する場合 true
        void*               WindowHandle;       ///< [win32] ユーザー定義のウィンドウハンドル (windows の場合は HWND、X11 は Window*。ただし、X11 は未対応)
#if defined(LNOTE_ANDROID)
        android_app*        AndroidApp;
#endif
    };

public:

    Manager();

	virtual ~Manager();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

    /// 終了処理
    void finalize();

	/// ウィンドウを取得する
    virtual IWindow* getWindow();

    /// メッセージ処理
	virtual bool processMessage();

    /// イベントリスナーのアタッチ (必ずデタッチすること)
    virtual void attachEventListener( IEventListener* listener_, int priority_ );

    /// イベントリスナーのデタッチ
    virtual void detachEventListener( IEventListener* listener_ );

    /// イベントを送信する
    void sendEvent( Event* event_ );

    /// イベントをポストする
    virtual void postEvent( Event* event_ );

#if defined(LNOTE_WINDOWS)
    /// ウィンドウメッセージの処理
    virtual LRESULT WndProc( HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_, bool* handled_ );
#endif

    /// メッセージコールバック
	bool lnMsgProc( const Event& args_ );

public:

    struct ListenerEntry
    {
        IEventListener* EventListener;
        int             Priority;
    };
    typedef std::list< ListenerEntry > ListenerEntryList;

    class IsEven;

private:

    /// EventListener ソート用の比較関数
    static bool _cmpEventListener( const ListenerEntry& l_, const ListenerEntry& r_ )
    {
        return l_.Priority < r_.Priority;
    }

private:

    Base::LogFile*      mLogFile;           ///< ログを出力するログファイルオブジェクト

    ApplicationBase*    mApplication;
    ListenerEntryList   mListenerEntryList;
    

    bool                mEndRequested;      ///< 終了要求
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