//=============================================================================
//�y LGameController �z
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
// �� LGameController �N���X
//=============================================================================

	// �{�^�������݉�����Ă��邩�𔻒肷��
	bool LGameController::isPress( LNGameControllerButton button_ ) const
	{
		return _impl->isPress( button_ );
	}

	// �{�^�����V���������ꂽ���𔻒肷��
	bool LGameController::isOnTrigger( LNGameControllerButton button_ ) const
	{
		return _impl->isOnTrigger( button_ );
	}

	// �{�^���������ꂽ���𔻒肷��
	bool LGameController::isOffTrigger( LNGameControllerButton button_ ) const
	{
		return _impl->isOffTrigger( button_ );
	}

	// �{�^�����V���������ꂽ & ���s�[�g��Ԃ𔻒肷��
	bool LGameController::isRepeat( LNGameControllerButton button_ ) const
	{
		return _impl->isRepeat( button_ );
	}

	// ���̏�Ԃ̎擾
	LVector2 LGameController::getAxisState( int axis_ ) const
	{
		return _impl->getAxisState( axis_ );
	}

	// �V���������ꂽ���z�{�^���̎擾
	int LGameController::getPressedButton()
	{
		return _impl->getPressedButton();
	}

#if 0
    // �V���������ꂽ�{�^���ԍ��ƃf�o�C�X�̎�ނ̎擾
	bool LGameController::getPressedButton( LNInputDeviceType* out_type_, int* out_button_ )
    {
        return _impl->getPressedButton( out_type_, out_button_ );
    }

    // �{�^���z�u (�L�[�R���t�B�O) ��ݒ肷��
	void LGameController::setKeyConfig( LNGameControllerButton ln_button_, LNInputDeviceType dev_type_, int new_button_, bool duplication_ )
    {
        _impl->setKeyConfig( ln_button_, dev_type_, new_button_, duplication_ );
    }

	// �R���g���[���ɑ΂���R���t�B�O�f�[�^�������N���X�̎擾
	//void LGameController::getConfigData( LKeyConfigData* config_ ) const
    const LKeyConfigData& LGameController::getConfigData()
    {
        return _impl->getConfigData();
        //_impl->getConfigData( config_ );
    }

    // �R���g���[���ɑ΂���R���t�B�O�f�[�^�������N���X�̎擾
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