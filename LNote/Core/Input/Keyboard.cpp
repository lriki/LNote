//=============================================================================
//�y Keyboard �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Manager.h"
#include "Keyboard.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Input
{

//=============================================================================
// �� Keyboard �N���X
//=============================================================================

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
	//---------------------------------------------------------------------
	Keyboard::Keyboard( Manager* manager_ )
		: mManager              ( manager_ )
        , mInputDevice          ( manager_->getInputDevice() )
		, mFirstRepeatInterval  ( 20 )
		, mRemRepeatInterval    ( 5 )
	{
		memset( mKeyStatus, 0, sizeof( mKeyStatus ) );
	}

	//---------------------------------------------------------------------
	// �� �L�[�����݉�����Ă��邩�𔻒肷��
	//---------------------------------------------------------------------
	bool Keyboard::isPress( LNVirtualKeyCode vkey_ )
	{
        if ( vkey_ < 0 || vkey_ >= LN_MAX_KEYS ) { 
	return false; }
		return ( mKeyStatus[ vkey_ ] > 0 );
	}

	//---------------------------------------------------------------------
	// �� �L�[���V���������ꂽ���𔻒肷��
	//---------------------------------------------------------------------
	bool Keyboard::isOnTrigger( LNVirtualKeyCode vkey_ )
	{
		if ( vkey_ < 0 || vkey_ >= LN_MAX_KEYS ) { 
	return false; }
		return ( mKeyStatus[ vkey_ ] == 1 );
	}

	//---------------------------------------------------------------------
	// �� �L�[�������ꂽ���𔻒肷��
	//---------------------------------------------------------------------
	bool Keyboard::isOffTrigger( LNVirtualKeyCode vkey_ )
	{
		if ( vkey_ < 0 || vkey_ >= LN_MAX_KEYS ) { 
	return false; }
		return ( mKeyStatus[ vkey_ ] == -1 );
	}

	//---------------------------------------------------------------------
	// �� �L�[���V���������ꂽ & ���s�[�g��Ԃ𔻒肷��
	//---------------------------------------------------------------------
	bool Keyboard::isRepeat( LNVirtualKeyCode vkey_ )
	{
		if ( vkey_ < 0 || vkey_ >= LN_MAX_KEYS ) { 
	return false; }
		int state = mKeyStatus[ vkey_ ];
		return ( ( state == 1 )  ||  ( state > mFirstRepeatInterval && state % mRemRepeatInterval == 0 ) );
	}

	//---------------------------------------------------------------------
	// �� ���s�[�g�Ԋu�̐ݒ�
	//---------------------------------------------------------------------
	void Keyboard::setRepeatInterval( u32 first_, u32 interval_ )
	{
		mFirstRepeatInterval = static_cast< s8 >( first_ );
		mRemRepeatInterval   = static_cast< s8 >( interval_ );
	}

	//---------------------------------------------------------------------
	// �� �t���[���X�V
	//---------------------------------------------------------------------
	void Keyboard::update()
	{
		for ( int i = 0; i < LN_MAX_KEYS; ++i )
		{
			// ��������Ă���ꍇ
			if ( mInputDevice->getKeyState( (LNVirtualKeyCode)i ) )
			{
				// �l�𑝂₵�Ă���
				++mKeyStatus[ i ];

				if ( mKeyStatus[ i ] > mRemRepeatInterval + mFirstRepeatInterval )
				{
					mKeyStatus[ i ] = mFirstRepeatInterval;
				}
			}
			// ������Ă��Ȃ��ꍇ�A���O�܂ŉ�����Ă����ꍇ�� -1�A����ȊO�� 0 ��ݒ肷��
			else
			{
				if ( mKeyStatus[ i ] > 0 )
				{
					mKeyStatus[ i ] = -1;
				}
				else
				{
					mKeyStatus[ i ] = 0;
				}
			}
        }
	}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Input
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================