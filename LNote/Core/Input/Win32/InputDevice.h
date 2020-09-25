//=============================================================================
//【 InputDevice 】
//-----------------------------------------------------------------------------
///**
//  @file       InputDevice.h
//  @brief      InputDevice
//  @version    1.0
//  @date       2012/2/16
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <deque>
#define DIRECTINPUT_VERSION 0x0800
#include "XInputModule.h"
#include <dinput.h>
#include "../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Input
{
namespace Win32
{
class JoypadDevice;

//=============================================================================
// ■ InputDevice クラス
//-----------------------------------------------------------------------------
///**
// @brief      DirectInput および XInput
//*/
//=============================================================================
class InputDevice
    : public Base::ReferenceObject
    , public IInputDevice
{
public:

    /// initialize() に渡す初期化データ
	struct InitData
	{

        System::IWindow*    Window;     ///< DirectInput の初期化に使うウィンドウ
	};

public:

	/// コンストラクタ
	InputDevice();

	/// デストラクタ
	virtual ~InputDevice();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
	LNRESULT initialize( const InitData& init_data_ );

    /// 接続済みのジョイパッド数の取得
    virtual int getJoypadNum() { return mJoypadNum; }

    /// マウスの状態の取得
    virtual const LNMouseDeviceState& getMouseState() { return mMouseState; }

    /// キーボードの状態の取得
    virtual bool getKeyState( LNVirtualKeyCode vkey_ );

    /// キーボードのバッファリングデータの取得
    virtual LNVirtualKeyCode getKeyBufferState();

    /// ジョイパッドの状態の取得
	virtual void getJoypadState( int joypad_id_, LNJoypadDeviceState* state_ );

    /// ジョイパッドの振動を開始する
	virtual void startVibration( int joypad_id_, int power_, int time_ );

    /// ジョイパッドの振動を停止する
	virtual void stopVibration( int joypad_id_ );

    /// タッチ情報の取得
    virtual void getTouchState( int touch_id_, LNTouchDeviceState* state_ ) { state_->Touch = false; }

    /// 仮想ボタンの状態の取得
	virtual float getVirtualButtonState( u32 button_, bool keyboard_, bool mouse_, int joypad_id_ );

    /// マウス座標の設定
	virtual void setMousePoint( const LVector2& point_ );

    /// マウスカーソルの位置をクライアント領域の中央に設定する
    virtual void setMouseCursorCenter();

    /// 入力情報の更新
	virtual LNRESULT update();

    /// デバイスの再セットアップ
	virtual LNRESULT refresh();

    /// 関連付けられている IWindow の取得
    virtual System::IWindow* getWindow() { return mWindow; }

    /// イベント処理 (Manager::onEvent() から呼ばれる)
    virtual bool onEvent( const System::Event& args_ );

private:

    /// 解放
    void _release();

    /// ジョイパッドの解放
    void _releaseJoypads();

    /// マウスデバイスの初期化
    LNRESULT _initMouse();

private:

    /// ジョイスティックを列挙するコールバック関数
	static BOOL CALLBACK _enumJoysticksCallback( const DIDEVICEINSTANCE* instance_, VOID* this_ );

	/// ジョイスティックのデバイスオブジェクトを作成する
	void _createJoysticksDevice( const DIDEVICEINSTANCE* instance_ );

    /// Xbox コントローラかを判定する
    static BOOL _isXInputDevice( const GUID* pGuidProductFromDirectInput );

private:

    typedef std::deque< LNVirtualKeyCode >     KeyBuffer;

private:

    System::IWindow*            mWindow;
    HWND                        mWindowHandle;
    IDirectInput8*              mDirectInput;
    XInputModule*               mXInputModule;

    bool                        mKeyStatus[ LN_MAX_KEYS ];
    KeyBuffer                   mKeyBuffer;

    IDirectInputDevice8*	    mMouseDevice;				///< マウスデバイス
	LNMouseDeviceState	        mMouseState;				///< マウスの状態

    JoypadDevice*               mJoypads[ LN_MAX_JOYPADS ];
    int                         mJoypadNum;
    int                         mXInputNo;

    
    bool                        mCoInited;
	bool						mEnableKeyBuffering;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Win32
} // namespace Input
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================