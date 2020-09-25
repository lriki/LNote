//=============================================================================
//【 Touch 】
//-----------------------------------------------------------------------------
///**
//  @file       Touch.h
//  @brief      Touch
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
// ■ Touch クラス
//-----------------------------------------------------------------------------
///**
//  @brief      
//
//  @note
//              タッチID 0 はマウスとリンク。
//              
//*/
//=============================================================================
class Touch
    : public Base::ReferenceObject
{
public:

    /// コンストラクタ
	Touch( Manager* manager_ );

    /// デストラクタ 
	virtual ~Touch();

    LN_REFOBJ_METHODS;

public:

	/// ボタンが現在押されているかを判定する
	virtual bool isPress( lnU32 id_ );

	/// ボタンが新しく押された瞬間を判定する
	virtual bool isOnTrigger( lnU32 id_ );

	/// ボタンが離された瞬間を判定する
	virtual bool isOffTrigger( lnU32 id_ );

	/// ボタンが新しく押された瞬間とリピート状態を判定する
	virtual bool isRepeat( lnU32 id_ );

    /// タッチ座標の取得 (マウスと同じく、仮想スクリーンへの変換が必要な場合はその変換後。タッチされていない場合は最後にタッチされたときの座標)
	virtual const LVector2& getPoint( lnU32 id_ ) { return mPosition[id_]; }

    /// 1 フレーム前からの移動量の取得
	virtual const LVector2& getMoveDistance( lnU32 id_ ) { return mDistance[id_]; }

    /// リピート間隔の設定
	virtual void setRepeatInterval( u32 first_, u32 interval_ );

public:

    /// フレーム更新 (Manager から呼ばれる)
	void update();

private:

	Manager*			    mManager;							///< 管理クラス
    IInputDevice*           mInputDevice;						///< デバイスクラス
	s32					    mPressCounts[LN_MAX_TOUCH_DATA];	///< 押し下中のフレームカウント
	s32					    mFirstRepeatInterval;				///< リピート開始までの待ちフレーム数
	s32					    mRemRepeatInterval;					///< リピート中の1回の待ちフレーム数
   
    LNTouchDeviceState      mTouchState[LN_MAX_TOUCH_DATA];
    
	LVector2	            mPosition[LN_MAX_TOUCH_DATA];							///< 位置
	LVector2	            mLastPosition[LN_MAX_TOUCH_DATA];						///< 1 フレーム前の位置
	LVector2	            mDistance[LN_MAX_TOUCH_DATA];							///< 1 フレーム前からの移動量

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