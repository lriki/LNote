//=============================================================================
//【 Joypad 】
//-----------------------------------------------------------------------------
///**
//  @file       Joypad.h
//  @brief      Joypad
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
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
class XInputModule;

//=============================================================================
// ■ JoypadDevice クラス
//-----------------------------------------------------------------------------
/**
// @brief      ジョイパッドを扱うクラス
//*/
//=============================================================================
class JoypadDevice
{
public:

	/// スティックの状態を表す構造体
	struct AnalogState
	{
		s32	X1;
		s32	Y1;
		s32	X2;
		s32	Y2;
	};

public:

	/// コンストラクタ
	JoypadDevice();

	/// デストラクタ
	~JoypadDevice();

public:

    /// 初期化 (xinput_no_ は XInput 用コントローラの割り当て番号。XInput 以外のものは -1 を渡すこと)
	LNRESULT initialize( IDirectInputDevice8* device_, HWND hwnd_, int xinput_no_, XInputModule* xinput_, bool forcefeedback_ );

    /// 解放
	void dispose();

    /// ボタン状態の取得
    bool getButtonState( u32 button_ ) const { return mButtonState[ button_ ] != 0; }

    /// ボタン状態の取得
	void getButtonState( u8* buf_ ) const;

    /// 方向コントローラの状態の取得
	s32 getPOVState() const { return mPOVState; }

    /// 軸の状態の取得 (MAX_AXIS_NUM 個)
	void getAxisState( float* axes_ ) const;

    /// 軸の状態の取得
    float getAxisState( u32 axis_ ) const { return mAxisState[ axis_ ]; }

    /// XInput デバイスかを判定する
    bool isXInputDevice() const { return mXInputNo >= 0; }

    /// ジョイパッドの振動を開始する
    void startVibration( int power_, int time_ );

    /// ジョイパッドの振動を停止する
    void stopVibration();

    /// フレーム更新
	LNRESULT update();

private:

    /// ジョイスティックの軸を列挙する関数
	static BOOL CALLBACK _enumAxesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi_, LPVOID this_ );

public:

	static const int AXIS_RANGE     = 1000;		///< 軸の範囲 ( -1000～1000 )
    static const int MAX_BUTTON_NUM = LN_MAX_JOYPAD_BUTTONS;	///< ボタンの数
	static const int MAX_AXIS_NUM   = LN_MAX_JOYPAD_AXIS;	    ///< 軸の数

private:

    Base::LogFile*          mLogFile;
    XInputModule*           mXInputModule;
    IDirectInputDevice8*	mDevice;

    IDirectInputEffect*     mDeviceEffect;
    DWORD                   mDIAxes[ 2 ];
    LONG                    mDIDirection[ 2 ];
    DIPERIODIC              mDIPeriodic;
    DIENVELOPE              mDIEnvelope;
    DIEFFECT                mDIEffect;
    lnU32                   mVibrationStartTime;
    lnU32                   mVibrationTime;

    float                   mPlayRange;

	u8		                mButtonState[ MAX_BUTTON_NUM ];	///< ボタンの状態
	u32		                mPOVState;						///< POV 状態 (POVDir の組み合わせ)
	float	                mAxisState[ MAX_AXIS_NUM ];		///< 軸の状態
    int                     mXInputNo;
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