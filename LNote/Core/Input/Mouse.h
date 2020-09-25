//=============================================================================
//【 Mouse 】
//-----------------------------------------------------------------------------
///**
//  @file       Mouse.h
//  @brief      Mouse
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
// ■ Mouse クラス
//-----------------------------------------------------------------------------
///**
//  @brief      マウスからの入力を扱うクラス
//
//  @par
//              このクラスのインスタンスは Engine::Input::Manager から取得します。<br><br>
//
//              isPress() 等の入力の判定で範囲外の値を渡したときの戻り値は常に false です。
//*/
//=============================================================================
class Mouse
    : public Base::ReferenceObject
    , public IMouse
{
public:

    /// コンストラクタ
	Mouse( Manager* manager_ );

    /// デストラクタ 
	virtual ~Mouse();

    LN_REFOBJ_METHODS;

public:

	/// ボタンが現在押されているかを判定する
	virtual bool isPress( LNMouseButton button_ );

	/// ボタンが新しく押された瞬間を判定する
	virtual bool isOnTrigger( LNMouseButton button_ );

	/// ボタンが離された瞬間を判定する
	virtual bool isOffTrigger( LNMouseButton button_ );

	/// ボタンが新しく押された瞬間とリピート状態を判定する
	virtual bool isRepeat( LNMouseButton button_ );

    /// マウス座標の設定
    virtual void setPoint( const LVector2& point_ ) { mInputDevice->setMousePoint( point_ ); }

    /// マウス座標の取得
	virtual const LVector2& getPoint() { return mPosition; }

    /// 1 フレーム前からの移動量の取得
	virtual const LVector2& getMoveDistance() { return mDistance; }

    /// 1 フレームのホイールの回転量の取得
	virtual int getWheelState() { return mWheelState; }

    /// 位置を中央に固定するかの設定
    virtual void setEnableCenterFix( bool flag_ ) { mIsCenterFix = flag_; }

    /// リピート間隔の設定
	virtual void setRepeatInterval( u32 first_, u32 interval_ );

public:

    /// フレーム更新 (Manager から呼ばれる)
	void update();

private:

	

	/// 何からかのマウス操作メッセージがある場合に呼ぶ ( Manager から呼ばれる )
	void _onOperatedMouse( bool is_client_ );

	/// マウスカーソルの表示状態を設定する ( setCursorMode() _postMoveCursor() から呼ばれる )
	void _showCursor( bool flag_ );

	/// マウスカーソルの操作後に呼んで、現在の設定に応じてマウスカーソルの表示・非表示を切り替える ( クライアント領域内でのマウス移動の場合、true を渡す )
	void _postMoveCursor( bool is_client_ );

protected:

	

private:

	static const int MAX_BUTTON_NUM = LN_MAX_MOUSE_BUTTONS;	///< マウスボタンの最大数
	static const int MOUSE_CURSOR_COUNT_MAX = 100;			///< マウスを動かさない時、消えるまでのフレーム数

private:

	Manager*			    mManager;							///< 管理クラス
    IInputDevice*           mInputDevice;						///< デバイスクラス
	s32					    mButtonStatus[ MAX_BUTTON_NUM ];	///< ボタンの状態
	s32					    mFirstRepeatInterval;				///< リピート開始までの待ちフレーム数
	s32					    mRemRepeatInterval;					///< リピート中の1回の待ちフレーム数
    bool                    mIsCenterFix;

	LVector2	        mPosition;							///< 現在位置
	LVector2	        mLastPosition;						///< 1 フレーム前の位置
	LVector2	                mDistance;							///< 1 フレーム前からの移動量
	long				        mWheelState;						///< ホイールの状態
	//LNMouseCursorMode	        mMouseCursorMode;					///< マウスカーソルの挙動
	int					        mMouseVisibleCount;					///< マウスカーソルを一定時間動かない場合に消す処理に使うカウンタ
	bool				        mIsSystemMouseShow;					///< システムのマウスカーソル表示中は true

	friend class Manager;
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