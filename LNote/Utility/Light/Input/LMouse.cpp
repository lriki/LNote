//=============================================================================
//【 LMouse 】
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../UtilCommon.h"
#include "LMouse.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// ■ LMouse クラス
//=============================================================================

	// ボタンが現在押されているかを判定する
	bool LMouse::isPress( LNMouseButton button_ ) const
	{
		return _impl->isPress( button_ );
	}

	// ボタンが新しく押されたかを判定する
	bool LMouse::isOnTrigger( LNMouseButton button_ ) const
	{
		return _impl->isOnTrigger( button_ );
	}

	// ボタンが離されたかを判定する
	bool LMouse::isOffTrigger( LNMouseButton button_ ) const
	{
		return _impl->isOffTrigger( button_ );
	}

	// ボタンが新しく押された & リピート状態を判定する
	bool LMouse::isRepeat( LNMouseButton button_ ) const
	{
		return _impl->isRepeat( button_ );
	}

    // マウス座標の取得
	LVector2 LMouse::getPoint() const
	{
		return _impl->getPoint();
	}

	// 1 フレーム前からの移動量の取得
	LVector2 LMouse::getDistance() const
	{
		return _impl->getMoveDistance();
	}

	// ホイールの状態の取得
	int LMouse::getWheelState() const
	{
		return _impl->getWheelState();
	}

    // マウスカーソルの挙動の設定
	//void LMouse::setCursorMode( LNMouseCursorMode mode_ )
 //   {
 //       _impl->setCursorMode( mode_ );
 //   }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================