//=============================================================================
//【 LJoypad 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../UtilCommon.h"
#include "LJoypad.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// ■ LJoypad クラス
//=============================================================================
#if 0
	// ボタンが現在押されているかを判定する
	bool LJoypad::isPress( int button_ ) const
	{
		return _impl->isPress( button_ );
	}

	// ボタンが新しく押されたかを判定する
	bool LJoypad::isOnTrigger( int button_ ) const
	{
		return _impl->isPress( button_ );
	}

	// ボタンが離されたかを判定する
	bool LJoypad::isOffTrigger( int button_ ) const
	{
		return _impl->isOffTrigger( button_ );
	}

	// ボタンが新しく押された & リピート状態を判定する
	bool LJoypad::isRepeat( int button_ ) const
	{
		return _impl->isRepeat( button_ );
	}

	// 軸の状態の取得
	LVector2 LJoypad::getAxisState( int axis_ ) const
	{
		return _impl->getAxisState( axis_ );
	}

	// POV の状態の取得
	int LJoypad::getPOVState() const
	{
		return _impl->getPOVState();
	}
   
    // 軸の値の正規化の有効判定
	bool LJoypad::isEnableNormalize() const
    {
        return _impl->isEnableNormalize();
    }

    // 軸の値の正規化の有効設定
	void LJoypad::setEnableNormalize( bool flag_ )
    {
        _impl->setEnableNormalize( flag_ );
    }
#endif
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================