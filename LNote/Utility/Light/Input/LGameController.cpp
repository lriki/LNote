//=============================================================================
//【 LGameController 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../UtilCommon.h"
#include "LGameController.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// ■ LGameController クラス
//=============================================================================

	// ボタンが現在押されているかを判定する
	bool LGameController::isPress( LNGameControllerButton button_ ) const
	{
		return _impl->isPress( button_ );
	}

	// ボタンが新しく押されたかを判定する
	bool LGameController::isOnTrigger( LNGameControllerButton button_ ) const
	{
		return _impl->isOnTrigger( button_ );
	}

	// ボタンが離されたかを判定する
	bool LGameController::isOffTrigger( LNGameControllerButton button_ ) const
	{
		return _impl->isOffTrigger( button_ );
	}

	// ボタンが新しく押された & リピート状態を判定する
	bool LGameController::isRepeat( LNGameControllerButton button_ ) const
	{
		return _impl->isRepeat( button_ );
	}

	// 軸の状態の取得
	LVector2 LGameController::getAxisState( int axis_ ) const
	{
		return _impl->getAxisState( axis_ );
	}

	// 新しく押された仮想ボタンの取得
	int LGameController::getPressedButton()
	{
		return _impl->getPressedButton();
	}

#if 0
    // 新しく押されたボタン番号とデバイスの種類の取得
	bool LGameController::getPressedButton( LNInputDeviceType* out_type_, int* out_button_ )
    {
        return _impl->getPressedButton( out_type_, out_button_ );
    }

    // ボタン配置 (キーコンフィグ) を設定する
	void LGameController::setKeyConfig( LNGameControllerButton ln_button_, LNInputDeviceType dev_type_, int new_button_, bool duplication_ )
    {
        _impl->setKeyConfig( ln_button_, dev_type_, new_button_, duplication_ );
    }

	// コントローラに対するコンフィグデータを扱うクラスの取得
	//void LGameController::getConfigData( LKeyConfigData* config_ ) const
    const LKeyConfigData& LGameController::getConfigData()
    {
        return _impl->getConfigData();
        //_impl->getConfigData( config_ );
    }

    // コントローラに対するコンフィグデータを扱うクラスの取得
	void LGameController::setConfigData( const LKeyConfigData& config_ )
    {
        _impl->setConfigData( config_ );
    }
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================