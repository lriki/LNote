//=============================================================================
//【 AndroidWindow 】
//-----------------------------------------------------------------------------
///**
//  @file       AndroidWindow.h
//  @brief      AndroidWindow
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Common/WindowBase.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#if defined(LNOTE_ANDROID)

namespace LNote
{
namespace Core
{
namespace System
{
class AndroidApplication;

//=============================================================================
// ■ AndroidWindow クラス
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class AndroidWindow
    : public Base::ReferenceObject
    , public WindowBase
{
public:

    /// 初期化データ
	struct InitData
	{
        Base::LogFile*          LogFile;        ///< ログを出力するログファイルオブジェクト
		int				        Width;			///< クライアント領域の幅
		int				        Height;			///< クライアント領域の高さ
    };

public:

    AndroidWindow( AndroidApplication* app_ )
        : mApplication  ( app_ )
        , mIsActive     ( false )
    {}

    virtual ~AndroidWindow() {}

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

    /// 終了処理
    virtual void finalize() {}

    /// ダミーウィンドウかを判定する
    virtual bool isDummy() { return false; }

    /// クライアント領域のサイズを取得する
    virtual const Geometry::Size& getSize() { return mClientSize; }

    /// ウィンドウの可視状態を設定する
    virtual void setVisible( bool flag_ ) { }

    /// フルスクリーンの有効設定
    virtual void setEnableFullScreen( bool flag_ ) {  }

    /// フルスクリーン状態かを判定する
    virtual bool isFullScreen() { return true; }

    /// ウィンドウのアクティブ状態を判定する
    virtual bool isActive() { return true; }

    /// タイトルバーの文字列の後ろに追加で表示する文字列を設定する
    virtual void setWindowTextFooter( const lnChar* format_, ... ) { }

    
    /// ANativeWindow の取得
    virtual ANativeWindow* getNativeWindow();

protected:

    AndroidApplication* mApplication;
    Geometry::Size		mClientSize;    ///< クライアント領域の大きさ
    bool                mIsActive;      ///< Manager 経由で設定される
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace System
} // namespace Core
} // namespace LNote

#endif // LNOTE_ANDROID
//=============================================================================
//								end of file
//=============================================================================