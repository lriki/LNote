//=============================================================================
//�y LMouse �z
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
// �� LMouse �N���X
//=============================================================================

	// �{�^�������݉�����Ă��邩�𔻒肷��
	bool LMouse::isPress( LNMouseButton button_ ) const
	{
		return _impl->isPress( button_ );
	}

	// �{�^�����V���������ꂽ���𔻒肷��
	bool LMouse::isOnTrigger( LNMouseButton button_ ) const
	{
		return _impl->isOnTrigger( button_ );
	}

	// �{�^���������ꂽ���𔻒肷��
	bool LMouse::isOffTrigger( LNMouseButton button_ ) const
	{
		return _impl->isOffTrigger( button_ );
	}

	// �{�^�����V���������ꂽ & ���s�[�g��Ԃ𔻒肷��
	bool LMouse::isRepeat( LNMouseButton button_ ) const
	{
		return _impl->isRepeat( button_ );
	}

    // �}�E�X���W�̎擾
	LVector2 LMouse::getPoint() const
	{
		return _impl->getPoint();
	}

	// 1 �t���[���O����̈ړ��ʂ̎擾
	LVector2 LMouse::getDistance() const
	{
		return _impl->getMoveDistance();
	}

	// �z�C�[���̏�Ԃ̎擾
	int LMouse::getWheelState() const
	{
		return _impl->getWheelState();
	}

    // �}�E�X�J�[�\���̋����̐ݒ�
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