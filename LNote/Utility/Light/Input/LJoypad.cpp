//=============================================================================
//�y LJoypad �z
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
// �� LJoypad �N���X
//=============================================================================
#if 0
	// �{�^�������݉�����Ă��邩�𔻒肷��
	bool LJoypad::isPress( int button_ ) const
	{
		return _impl->isPress( button_ );
	}

	// �{�^�����V���������ꂽ���𔻒肷��
	bool LJoypad::isOnTrigger( int button_ ) const
	{
		return _impl->isPress( button_ );
	}

	// �{�^���������ꂽ���𔻒肷��
	bool LJoypad::isOffTrigger( int button_ ) const
	{
		return _impl->isOffTrigger( button_ );
	}

	// �{�^�����V���������ꂽ & ���s�[�g��Ԃ𔻒肷��
	bool LJoypad::isRepeat( int button_ ) const
	{
		return _impl->isRepeat( button_ );
	}

	// ���̏�Ԃ̎擾
	LVector2 LJoypad::getAxisState( int axis_ ) const
	{
		return _impl->getAxisState( axis_ );
	}

	// POV �̏�Ԃ̎擾
	int LJoypad::getPOVState() const
	{
		return _impl->getPOVState();
	}
   
    // ���̒l�̐��K���̗L������
	bool LJoypad::isEnableNormalize() const
    {
        return _impl->isEnableNormalize();
    }

    // ���̒l�̐��K���̗L���ݒ�
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