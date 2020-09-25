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

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{
class X11Application;
    
//=============================================================================
// ■ X11WindowBase クラス
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class X11WindowBase
    : public Base::ReferenceObject
    , public WindowBase
{
public:

    X11WindowBase( X11Application* app_ )
        : mApplication  ( app_ )
        , mIsActive     ( false )
    {}

    virtual ~X11WindowBase() {}

    //LN_REFOBJ_METHODS;

public:

    /// 終了処理
    virtual void finalize() {}

    /// ダミーウィンドウかを判定する
    virtual bool isDummy() = 0;

    /// ウィンドウのアクティブ状態を判定する
    virtual bool isActive() { return mIsActive; }

    /// クロースボタンが押されたときに呼び出され、このウィンドウと関連付けられたクローズボタンかを判定する
    virtual bool checkCloseButtonOfThis( Atom a1_, Atom a2_ ) { return false; }

protected:

    X11Application* mApplication;
    bool            mIsActive;      ///< Manager 経由で設定される
};

//=============================================================================
// ■ X11Window クラス
//-----------------------------------------------------------------------------
///**
//  @brief      ウィンドウを扱うクラス
//*/
//=============================================================================
class X11Window
    : public X11WindowBase
{
public:

    /// 初期化データ
    struct InitData
    {
        Base::LogFile*      LogFile;        ///< ログを出力するログファイルオブジェクト
        const lnChar*	    TitleText;		///< ウィンドウタイトルの文字列
        int				    Width;			///< クライアント領域の幅
        int				    Height;			///< クライアント領域の高さ
        bool			    Windowed;		///< ウィンドウモードで初期化する場合 true

        /// デフォルト値
        InitData()          
            : LogFile           ( NULL )
            , TitleText         ( LNTEXT( "default" ) )
            , Width             ( 640 )
            , Height            ( 480 )
            , Windowed          ( true )
        {}
    };

public:

    X11Window( X11Application* manager_, Display* display_ );

	virtual ~X11Window();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

    /// 終了処理
    void finalize();

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
    
    /// ダミーウィンドウかを判定する
    virtual bool isDummy() { return false; }

    /// クロースボタンが押されたときに呼び出され、このウィンドウと関連付けられたクローズボタンかを判定する
    virtual bool checkCloseButtonOfThis( Atom a1_, Atom a2_ ) { return ( a1_ == mAtomForCloseButton1 && a2_ == mAtomForCloseButton2 ); }

private:

    /// ウィンドウサイズを変更する
    void _resize( bool fullscreen_ );

private:
	static const int		MAX_WINDOW_TEXT = 256;			///< ウィンドウタイトル文字列の最大文字数
	static const int		MAX_WINDOW_TEXT_FOOTER = 64;	///< ウィンドウタイトル文字列のフッタ情報の最大文字数

private:

    X11Application*             mManager;
    Base::LogFile*              mLogFile;           ///< ログを出力するログファイルオブジェクト
    ln_std_tstring    		    mTitleText;			///< ウィンドウタイトルの文字列
    Geometry::Size				mClientSize;        ///< 現在のクライアント領域のサイズ
    int                         mOrginalWidth;      ///< initialize() または setSize() で設定されたクライアント領域の幅
    int                         mOrginalHeight;     ///< initialize() または setSize() で設定されたクライアント領域の高さ
    
    
    Display*                    mXDisplay;
    int                         mXDefaultScreen;
    Window                      mXRootWindow;
    Window                      mXWindow;
    XSetWindowAttributes        mWindowAttributes;
    
    bool                        mFullScreen;        ///< フルスクリーンモード ( 用のスタイル ) の場合 true
    

    Atom        mAtomForCloseButton1;
    Atom        mAtomForCloseButton2;

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