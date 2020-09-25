//=============================================================================
//【 Window 】
//-----------------------------------------------------------------------------
///**
//  @file       Window.h
//  @brief      Window
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <string>
#include "../Interface.h"
#include "../Common/WindowBase.h"

#if defined(LNOTE_WINDOWS)

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{
class Win32Application;

//=============================================================================
// ■ WindowBase クラス
//-----------------------------------------------------------------------------
///**
//  @brief      ライブラリで必要なウィンドウメッセージを処理するプロシージャを持った基底
//*/
//=============================================================================
class Win32WindowBase
    : public Base::ReferenceObject
    , public WindowBase
{
public:

    Win32WindowBase( Win32Application* app_ )
        : mApplication  ( app_ )
        , mIsActive     ( false )
    {}

    virtual ~Win32WindowBase() {}

    LN_REFOBJ_METHODS;

public:

    /// 終了処理
    virtual void finalize() {}

    /// ダミーウィンドウかを判定する
    virtual bool isDummy() = 0;

    /// ウィンドウのアクティブ状態を判定する
    virtual bool isActive() { return mIsActive; }

    /// プロシージャ関数 (アクティブ状態の変化を処理し、必要なメッセージだけをApplication にイベントを送る)
    virtual LRESULT WndProc( HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_ );

protected:

    Win32Application*   mApplication;
    bool                mIsActive;      ///< Manager 経由で設定される
};

//=============================================================================
// ■ Window クラス
//-----------------------------------------------------------------------------
///**
//  @brief		ウィンドウを扱うクラス
//*/
//=============================================================================
class Window
    : public Win32WindowBase
{
public:

	/// 初期化データ
	struct InitData
	{
        Base::LogFile*          LogFile;        ///< ログを出力するログファイルオブジェクト
		const lnChar*	        TitleText;		///< ウィンドウタイトルの文字列
		int				        Width;			///< クライアント領域の幅
		int				        Height;			///< クライアント領域の高さ
		bool			        Windowed;		///< ウィンドウモードで初期化する場合 true
		//EventListener*          EventListener;	///< イベント処理を行うクラスインスタンスへのポインタまたは NULL
        unsigned                MainThreadID;   ///< processMessageThread() を呼び出すスレッドの識別子または 0
        const lnChar*	        WinClassName;   ///< ウィンドウクラスの名前 または NULL ( NULL の場合、L"_LNote_" が使われる )
        int                     IconResourceID; ///< タイトルバーのアイコン識別子 (IDI_ICON1 等)
	
        /// デフォルト値
        InitData()          
            : LogFile           ( NULL )
            , TitleText         ( _T( "default" ) )
            , Width             ( 640 )
            , Height            ( 480 )
            , Windowed          ( true )
            //, EventListener ( NULL )
            , MainThreadID      ( 0 )
            , WinClassName      ( NULL )
            , IconResourceID    ( 0 )
        {}
    };

public:

    Window( Win32Application* manager_ );

	virtual ~Window();

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

    /// 終了処理
    virtual void finalize();

    /// クライアント領域のサイズを取得する
    virtual const Geometry::Size& getSize() { return mClientSize; }

    /// ウィンドウの可視状態を設定する
    virtual void setVisible( bool flag_ );

    /// フルスクリーンの有効設定
	virtual void setEnableFullScreen( bool flag_ );

    /// フルスクリーン状態かを判定する
    virtual bool isFullScreen() { return mFullScreen; }

    /// ウィンドウのアクティブ状態を判定する
    virtual bool isActive() { return mIsActive; }

    /// タイトルバーの文字列の後ろに追加で表示する文字列を設定する
	virtual void setWindowTextFooter( const lnChar* format_, ... );

#if defined(LNOTE_WINDOWS)

    /// ウィンドウハンドルの取得
    virtual HWND getWindowHandle() { return mWindowHandle; }

#endif

    /// ダミーウィンドウかを判定する
    virtual bool isDummy() { return false; }

private:

    /// ウィンドウサイズを変更する
    void _resize( bool fullscreen_ );

    /// 呼び出し元プロシージャ関数 (ウィンドウを作成したスレッド内で呼ばれる)
	static LRESULT CALLBACK _staticMsgProc( HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_ );

    /// プロシージャ関数 ()
    virtual LRESULT WndProc( HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_ );

private:

	static const lnChar*	WINDOW_CLASS_NAME;				///< ウィンドウクラス名	  "_LNote_"
	static const lnChar*	PROP_WINPROC;					///< SetProp() に渡す名前 "_LNoteProp_"
	static const int		MAX_WINDOW_TEXT = 256;			///< ウィンドウタイトル文字列の最大文字数
	static const int		MAX_WINDOW_TEXT_FOOTER = 64;	///< ウィンドウタイトル文字列のフッタ情報の最大文字数

private:

    Win32Application*           mManager;
    Base::LogFile*              mLogFile;           ///< ログを出力するログファイルオブジェクト
    ln_std_tstring    		    mTitleText;			///< ウィンドウタイトルの文字列    
    Geometry::Size				mClientSize;        ///< クライアント領域の大きさ
    int                         mOrginalWidth;      ///< initialize() または setSize() で設定されたクライアント領域の幅
    int                         mOrginalHeight;     ///< initialize() または setSize() で設定されたクライアント領域の高さ
	HWND				        mWindowHandle;		///< ウィンドウハンドル
	RECT				        mWindowRect;		///< ウィンドウ領域
    HACCEL                      mAccelerators;      ///< アクセラレータ (Alt+Enter の警告音対策のために使ってる)
    unsigned                    mMainThreadID;      ///< マルチスレッド実装のときに使う、メインスレッドの識別子
    bool                        mFullScreen;        ///< フルスクリーンモード ( 用のスタイル ) の場合 true
    bool				        mIsActive;			///< ウィンドウがアクティブの場合 true

};

//=============================================================================
// ■ DummyWindow クラス
//-----------------------------------------------------------------------------
///**
//  @brief      ウィンドウハンドルをラップするクラス
//*/
//=============================================================================
class DummyWindow
    : public Win32WindowBase
{
public:

    DummyWindow( Win32Application* app_, void* hwnd_ )
        : Win32WindowBase    ( app_ )
        , mWindowHandle     ( hwnd_ )
    {}

    virtual ~DummyWindow() {}

public:

    /// クライアント領域のサイズを取得する
	virtual const Geometry::Size& getSize()
    {
        // コンストラクタで mClientSize に格納しておいてもいいと思ったけど、
        // フルスクリーン化等でウィンドウサイズが変わった時の対応が面倒そうなのでこのまま。
#if defined(LNOTE_WINDOWS)
        RECT rc;
        ::GetClientRect( getWindowHandle(), &rc );
        mClientSize.set( rc.right, rc.bottom );
        return mClientSize;
        
#elif defined(LNOTE_X11)
        Display* d = XOpenDisplay( NULL );
        Window root;
        int x, y;
		unsigned int width, height;
		unsigned int border;
		unsigned int depth;
        XGetGeometry(
            d, getWindowHandle(), &root, &x, &y,
            &width, &height, &border, &depth );
        return mClientSize.set( width, height );
#endif
    }

    virtual void setVisible( bool flag_ ) {}

    virtual void setEnableFullScreen( bool flag_ ) {}

    virtual bool isFullScreen() { return false; }

    virtual bool isActive() { return mIsActive; }

    virtual void setWindowTextFooter( const lnChar* format_, ... ) {}

    virtual HWND getWindowHandle() { return static_cast< HWND >( mWindowHandle ); }

    /// ダミーウィンドウかを判定する
    virtual bool isDummy() { return true; }

private:

    void*			mWindowHandle;
    Geometry::Size	mClientSize;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace System
} // namespace Core
} // namespace LNote

#endif // LNOTE_WINDOWS

//=============================================================================
//								end of file
//=============================================================================