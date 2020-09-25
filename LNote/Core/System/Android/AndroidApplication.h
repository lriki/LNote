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
#include "../Interface.h"
#include "../Common/ApplicationBase.h"

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

//=============================================================================
// ■ AndroidApplication クラス
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class AndroidApplication
    : public ApplicationBase
{
public:

    /// 初期化データ
	struct InitData
	{
        Base::LogFile*      LogFile;        ///< ログを出力するログファイルオブジェクト
        Manager*			SystemManager;
		int				    Width;			///< クライアント領域の幅
		int				    Height;			///< クライアント領域の高さ
        android_app*        AndroidApp;
    };

public:

    AndroidApplication();

	virtual ~AndroidApplication();

    /// 初期化
    void initialize( const AndroidApplication::InitData& init_data_ );

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
    
public:
    
    android_app* getAndroidApp() { return mAndroidApp; }

private:

    /// イベントコールバック
    static void engine_handle_cmd( android_app* app_, int32_t cmd_ );

    /// 入力イベントコールバック
    static int32_t engine_handle_input( android_app* android_app_, AInputEvent* event_ );
    
    /// Android -> LNKey キーコード変換
    static lnU32 _convertKeyCodeAndroidToLN( lnU32 key_ );
    
    
    
private:

    InitData            mAndroidInitData;
    android_app*        mAndroidApp;
    
    Geometry::Point		mLastTouchPoints[LN_MAX_TOUCH_DATA];
    
    bool            mWindowInited;
    bool            mFocused;
    bool            mAnimating;     ///< 今のところ mFocused と同じ

    bool            mIsDeviceLost;
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