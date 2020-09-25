//=============================================================================
//【 Keyboard 】
//-----------------------------------------------------------------------------
///**
//  @file       Keyboard.h
//  @brief      Keyboard
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
class Manager;

//=============================================================================
// ■ Keyboard クラス
//-----------------------------------------------------------------------------
///**
//  @brief      キーボードからの入力を扱うクラス
//*/
//=============================================================================
class Keyboard
    : public Base::ReferenceObject
    , public IKeyboard
{
public:

	/// コンストラクタ
	Keyboard( Manager* manager_ );

    /// デストラクタ 
	virtual ~Keyboard() {};

    LN_REFOBJ_METHODS;

public:
	
	/// キーが現在押されているかを判定する
	virtual bool isPress( LNVirtualKeyCode vkey_ );

	/// キーが新しく押された瞬間を判定する
	virtual bool isOnTrigger( LNVirtualKeyCode vkey_ );

	/// キーが離された瞬間を判定する
	virtual bool isOffTrigger( LNVirtualKeyCode vkey_ );

	/// ボタンが新しく押された瞬間とリピート状態を判定する
	virtual bool isRepeat( LNVirtualKeyCode vkey_ );

    /// キーボードのバッファリングデータの取得
    virtual LNVirtualKeyCode getKeyBufferState() { return mInputDevice->getKeyBufferState(); }

    /// リピート間隔の設定
	virtual void setRepeatInterval( u32 first_, u32 interval_ );

public:

	/// フレーム更新 ( Manager から呼ばれる )
	void update();

private:

	Manager*        mManager;					///< 管理クラス
    IInputDevice*   mInputDevice;
	s8              mKeyStatus[ LN_MAX_KEYS ];  ///< キーの状態
	s8			    mFirstRepeatInterval;		///< リピート開始までの待ちフレーム数
	s8			    mRemRepeatInterval;			///< リピート中の1回の待ちフレーム数
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