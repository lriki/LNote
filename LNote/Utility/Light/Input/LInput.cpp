//=============================================================================
//【 LInput 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../UtilCommon.h"
#include "../../Core/Input/Manager.h"
#include "../../Core/Framework/Framework.h"
#include "LInput.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// ■ LInput クラス
//=============================================================================

	// インスタンスの取得
	LInput LInput::getInstance()
	{
		LInput o;
        o._impl = gFramework->getInputManager();
		return o;
	}

	// 接続済みのジョイパッドの数の取得
	int LInput::getJoypadNum() const
	{
		return _impl->getJoypadNum();
	}

	// LGameController インスタンスの取得
	LGameController LInput::getGameController( int index_ )
	{
		return LGameController( _impl->getGameController( index_ ) );
	}

	// LMouse インスタンスの取得
	LMouse LInput::getMouse()
	{
		return LMouse( _impl->getMouse() );
	}

	// LKeyboard インスタンスの取得
	LKeyboard LInput::getKeyboard()
	{
		return LKeyboard( _impl->getKeyboard() );
	}

	// LJoypad インスタンスの取得
	//LJoypad LInput::getJoypad( int index_ )
	//{
	//	return LJoypad( _impl->getJoypadInstance( index_ ) );
	//}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================