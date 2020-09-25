//=============================================================================
//【 DummyWindow 】
//-----------------------------------------------------------------------------
///**
//  @file       DummyWindow.h
//  @brief      DummyWindow
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{
class Manager;

//=============================================================================
// ■ DummyWindow クラス
//-----------------------------------------------------------------------------
///**
//  @brief      ウィンドウハンドルをラップするクラス
//*/
//=============================================================================
class DummyWindow
    : public Base::ReferenceObject
    , public IWindow
{
public:

    DummyWindow( void* hwnd_ )
        : mWindowHandle ( hwnd_ )
        , mIsActive     ( true )
    {}

    virtual ~DummyWindow() {}

    LN_REFOBJ_METHODS;

public:

    //

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

    /// ウィンドウの可視状態を設定する
    virtual void setVisible( bool flag_ ) {}

    /// フルスクリーンの有効設定
    virtual void setEnableFullScreen( bool flag_ ) {}

    /// フルスクリーン状態かを判定する
    virtual bool isFullScreen() { return false; }

    /// ウィンドウのアクティブ状態を判定する
    virtual bool isActive() { return mIsActive; }

    /// タイトルバーの文字列の後ろに追加で表示する文字列を設定する
    virtual void setWindowTextFooter( const lnChar* format_, ... ) {}

#if defined(LNOTE_WINDOWS)
    /// ウィンドウハンドルの取得
    virtual HWND getWindowHandle() { return static_cast< HWND >( mWindowHandle ); }
#elif defined(LNOTE_X11)
    /// ウィンドウハンドルの取得
    virtual ::Window* getWindowHandle() { return static_cast< ::Window* >( mWindowHandle ); }
#endif

private:

    void*			mWindowHandle;
    Geometry::Size	mClientSize;
    bool			mIsActive;      ///< Manager 経由で設定される

    friend class Manager;
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