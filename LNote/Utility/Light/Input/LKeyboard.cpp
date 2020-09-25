//=============================================================================
//�y LKeyboard �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../UtilCommon.h"
#include "LKeyboard.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// �� LKeyboard �N���X
//=============================================================================

	// �{�^�������݉�����Ă��邩�𔻒肷��
	bool LKeyboard::isPress( LNVirtualKeyCode vkey_ ) const
	{
		return _impl->isPress( vkey_ );
	}

	// �{�^�����V���������ꂽ���𔻒肷��
	bool LKeyboard::isOnTrigger( LNVirtualKeyCode vkey_ ) const
	{
		return _impl->isOnTrigger( vkey_ );
	}

	// �{�^���������ꂽ���𔻒肷��
	bool LKeyboard::isOffTrigger( LNVirtualKeyCode vkey_ ) const
	{
		return _impl->isOffTrigger( vkey_ );
	}

	// �{�^�����V���������ꂽ & ���s�[�g��Ԃ𔻒肷��
	bool LKeyboard::isRepeat( LNVirtualKeyCode vkey_ ) const
	{
		return _impl->isRepeat( vkey_ );
	}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================