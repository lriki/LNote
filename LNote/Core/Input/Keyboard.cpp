//=============================================================================
//【 Keyboard 】
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
// ■ Keyboard クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
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
	// ● キーが現在押されているかを判定する
	//---------------------------------------------------------------------
	bool Keyboard::isPress( LNVirtualKeyCode vkey_ )
	{
        if ( vkey_ < 0 || vkey_ >= LN_MAX_KEYS ) { 
	return false; }
		return ( mKeyStatus[ vkey_ ] > 0 );
	}

	//---------------------------------------------------------------------
	// ● キーが新しく押されたかを判定する
	//---------------------------------------------------------------------
	bool Keyboard::isOnTrigger( LNVirtualKeyCode vkey_ )
	{
		if ( vkey_ < 0 || vkey_ >= LN_MAX_KEYS ) { 
	return false; }
		return ( mKeyStatus[ vkey_ ] == 1 );
	}

	//---------------------------------------------------------------------
	// ● キーが離されたかを判定する
	//---------------------------------------------------------------------
	bool Keyboard::isOffTrigger( LNVirtualKeyCode vkey_ )
	{
		if ( vkey_ < 0 || vkey_ >= LN_MAX_KEYS ) { 
	return false; }
		return ( mKeyStatus[ vkey_ ] == -1 );
	}

	//---------------------------------------------------------------------
	// ● キーが新しく押された & リピート状態を判定する
	//---------------------------------------------------------------------
	bool Keyboard::isRepeat( LNVirtualKeyCode vkey_ )
	{
		if ( vkey_ < 0 || vkey_ >= LN_MAX_KEYS ) { 
	return false; }
		int state = mKeyStatus[ vkey_ ];
		return ( ( state == 1 )  ||  ( state > mFirstRepeatInterval && state % mRemRepeatInterval == 0 ) );
	}

	//---------------------------------------------------------------------
	// ● リピート間隔の設定
	//---------------------------------------------------------------------
	void Keyboard::setRepeatInterval( u32 first_, u32 interval_ )
	{
		mFirstRepeatInterval = static_cast< s8 >( first_ );
		mRemRepeatInterval   = static_cast< s8 >( interval_ );
	}

	//---------------------------------------------------------------------
	// ● フレーム更新
	//---------------------------------------------------------------------
	void Keyboard::update()
	{
		for ( int i = 0; i < LN_MAX_KEYS; ++i )
		{
			// が押されている場合
			if ( mInputDevice->getKeyState( (LNVirtualKeyCode)i ) )
			{
				// 値を増やしていく
				++mKeyStatus[ i ];

				if ( mKeyStatus[ i ] > mRemRepeatInterval + mFirstRepeatInterval )
				{
					mKeyStatus[ i ] = mFirstRepeatInterval;
				}
			}
			// 押されていない場合、直前まで押されていた場合は -1、それ以外は 0 を設定する
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