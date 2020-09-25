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
#include "Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Input
{

//=============================================================================
// ■ Joypad クラス
//-----------------------------------------------------------------------------
///**
//  @brief      ジョイパッドからの入力を扱うクラス
//*/
//=============================================================================
class Joypad
    : public Base::ReferenceObject
    , public IJoypad
{
public:

    /// コンストラクタ
	Joypad( Manager* manager_, int joypad_id_ );

    /// デストラクタ 
	virtual ~Joypad() {};

    LN_REFOBJ_METHODS;

public:

    /// 接続済みかをチェックする
    virtual bool isConnected() { return ( mJoypadID >= 0 ); }

    /// ボタンが現在押されているかを判定する
	virtual bool isPress( int button_ );

    /// ボタンが新しく押された瞬間を判定する
	virtual bool isOnTrigger( int button_ );

    /// ボタンが離された瞬間を判定する
	virtual bool isOffTrigger( int button_ );

    /// ボタンが新しく押された瞬間とリピート状態を判定する
	virtual bool isRepeat( int button_ );

    /// 軸の状態の取得
    virtual float getAxisState( int axis_ );

    /// POV の状態の取得
	virtual u32 getPOVState() { return mPOVState; }

    /// ジョイパッドの振動を開始する
    virtual void startVibration( int power_, int time_ ) { mInputDevice->startVibration( mJoypadID, power_, time_ ); }

    /// ジョイパッドの振動を停止する
	virtual void stopVibration() { mInputDevice->stopVibration( mJoypadID ); }

	/// リピート間隔の設定 ( Manager から呼ばれる )
	virtual void setRepeatInterval( u32 first_, u32 interval_ );

public:

	/// フレーム更新 ( Manager から呼ばれる )
	void update();

private:

	Manager*        mManager;							    ///< 管理クラス
    IInputDevice*   mInputDevice;
	int		        mJoypadID;							    ///< 割り当てられているジョイパッドの番号
	s32		        mButtonStatus[ LN_MAX_JOYPAD_BUTTONS ]; ///< ボタンの状態
	float	        mAxisStatus[ LN_MAX_JOYPAD_AXIS ];	    ///< 軸の状態
	u32		        mPOVState;							    ///< POV の状態
	s32		        mFirstRepeatInterval;				    ///< リピート開始までの待ちフレーム数
	s32		        mRemRepeatInterval;					    ///< リピート中の1回の待ちフレーム数
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Input
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================