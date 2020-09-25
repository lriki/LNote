//=============================================================================
//�y LInput �z
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
// �� LInput �N���X
//=============================================================================

	// �C���X�^���X�̎擾
	LInput LInput::getInstance()
	{
		LInput o;
        o._impl = gFramework->getInputManager();
		return o;
	}

	// �ڑ��ς݂̃W���C�p�b�h�̐��̎擾
	int LInput::getJoypadNum() const
	{
		return _impl->getJoypadNum();
	}

	// LGameController �C���X�^���X�̎擾
	LGameController LInput::getGameController( int index_ )
	{
		return LGameController( _impl->getGameController( index_ ) );
	}

	// LMouse �C���X�^���X�̎擾
	LMouse LInput::getMouse()
	{
		return LMouse( _impl->getMouse() );
	}

	// LKeyboard �C���X�^���X�̎擾
	LKeyboard LInput::getKeyboard()
	{
		return LKeyboard( _impl->getKeyboard() );
	}

	// LJoypad �C���X�^���X�̎擾
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