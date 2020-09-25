//=============================================================================
//【 AndroidInputDevice 】
//-----------------------------------------------------------------------------
///**
//  @file       AndroidInputDevice.h
//  @brief      AndroidInputDevice
//  @version    1.0
//  @date       2012/2/16
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
namespace Input
{
namespace Android
{

/*
    タッチ入力とマウス入力を関連付けるか？
        AndroidInputDevice 上では関連付けない。
        GameController あたりで調整する。
   
    ・タッチ ON を決定ボタンと関連付けるかの設定
        このへんはGameControllerで。
*/

//=============================================================================
// ■ AndroidInputDevice クラス
//-----------------------------------------------------------------------------
///**
// @brief      DirectInput および XInput
//*/
//=============================================================================
class AndroidInputDevice
    : public Base::ReferenceObject
    , public IInputDevice
{
public:

    /// initialize() に渡す初期化データ
	struct InitData
	{

        
	};

public:

	/// コンストラクタ
	AndroidInputDevice();

	/// デストラクタ
	virtual ~AndroidInputDevice();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
	void initialize( const InitData& init_data_ );

    /// 接続済みのジョイパッド数の取得
    virtual int getJoypadNum() { return 0; }

    /// マウスの状態の取得
    virtual const LNMouseDeviceState& getMouseState() { return mMouseState; }

    /// キーボードの状態の取得
    virtual bool getKeyState( LNVirtualKeyCode vkey_ ) { return false; }

    /// キーボードのバッファリングデータの取得
    virtual LNVirtualKeyCode getKeyBufferState() { return LN_KEY_UNKNOWN; }

    /// ジョイパッドの状態の取得
    virtual void getJoypadState( int joypad_id_, LNJoypadDeviceState* state_ ) { }

    /// ジョイパッドの振動を開始する
    virtual void startVibration( int joypad_id_, int power_, int time_ ) { }

    /// ジョイパッドの振動を停止する
	virtual void stopVibration( int joypad_id_ ) { }
    
    /// タッチ情報の取得
    virtual void getTouchState( int touch_id_, LNTouchDeviceState* state_ ) { *state_ = mTouchState[touch_id_]; }

    /// 仮想ボタンの状態の取得
	virtual float getVirtualButtonState( u32 button_, bool keyboard_, bool mouse_, int joypad_id_ );

    /// マウス座標の設定
    virtual void setMousePoint( const LVector2& point_ );

    /// マウスカーソルの位置をクライアント領域の中央に設定する
    virtual void setMouseCursorCenter();

    /// 入力情報の更新
	virtual LNRESULT update();

    /// デバイスの再セットアップ
    virtual LNRESULT refresh() { return LN_OK; }

    /// 関連付けられている IWindow の取得
    virtual System::IWindow* getWindow() { return NULL; }
    
    /// イベント処理 (Manager::onEvent() から呼ばれる)
    virtual bool onEvent( const System::Event& args_ );

private:


private:
    
    LNTouchDeviceState          mTouchState[LN_MAX_TOUCH_DATA];
	LNMouseDeviceState	        mMouseState;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Android
} // namespace Input
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================