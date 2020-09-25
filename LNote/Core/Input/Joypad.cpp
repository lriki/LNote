//=============================================================================
//【 Joypad 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Manager.h"
#include "Joypad.h"

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
// ■ Joypad クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	Joypad::Joypad( Manager* manager_, int joypad_id_ )
		: mManager				( manager_ )
        , mInputDevice          ( manager_->getInputDevice() )
		, mJoypadID				( joypad_id_ )
		, mPOVState				( 0 )
        , mFirstRepeatInterval	( 20 )
		, mRemRepeatInterval	( 5 )
	{
		memset( mButtonStatus, 0, sizeof( mButtonStatus ) );
		memset( mAxisStatus, 0, sizeof( mAxisStatus ) );
	}

	//---------------------------------------------------------------------
	// ● ボタンが現在押されているかを判定する
	//---------------------------------------------------------------------
	bool Joypad::isPress( int button_ )
	{
        if ( button_ < 0 || LN_MAX_JOYPAD_BUTTONS <= button_ ) { 
	return false; }
		return ( mButtonStatus[ button_ ] > 0 );
	}

	//---------------------------------------------------------------------
	// ● ボタンが新しく押されたかを判定する
	//---------------------------------------------------------------------
	bool Joypad::isOnTrigger( int button_ )
	{
		if ( button_ < 0 || LN_MAX_JOYPAD_BUTTONS <= button_ ) { 
	return false; }
		return ( mButtonStatus[ button_ ] == 1 );
	}

	//---------------------------------------------------------------------
	// ● ボタンが離されたかを判定する
	//---------------------------------------------------------------------
	bool Joypad::isOffTrigger( int button_ )
	{
		if ( button_ < 0 || LN_MAX_JOYPAD_BUTTONS <= button_ ) { 
	return false; }
		return ( mButtonStatus[ button_ ] == -1 );
	}

	//---------------------------------------------------------------------
	// ● ボタンが新しく押された & リピート状態を判定する
	//---------------------------------------------------------------------
	bool Joypad::isRepeat( int button_ )
	{
		if ( button_ < 0 || LN_MAX_JOYPAD_BUTTONS <= button_ ) { 
	return false; }
		int state = mButtonStatus[ button_ ];
		return ( ( state == 1 )  ||  ( state > mFirstRepeatInterval && state % mRemRepeatInterval == 0 ) );
	}

	//---------------------------------------------------------------------
	// ● 軸の状態を取得する
	//---------------------------------------------------------------------
    float Joypad::getAxisState( int axis_ )
	{
        if ( axis_ < 0 || LN_MAX_JOYPAD_AXIS <= axis_ ) { 
	return 0.0f; }
		return mAxisStatus[ axis_ ];
	}

	//---------------------------------------------------------------------
	// ● リピート間隔の設定
	//---------------------------------------------------------------------
	void Joypad::setRepeatInterval( u32 first_, u32 interval_ )
	{
		mFirstRepeatInterval = first_;
		mRemRepeatInterval = interval_;
	}

	//---------------------------------------------------------------------
	// ● フレーム更新
	//---------------------------------------------------------------------
	void Joypad::update()
	{
		if ( mJoypadID >= mManager->getInputDevice()->getJoypadNum() ) { 
	return; }

        LNJoypadDeviceState state;
        mManager->getInputDevice()->getJoypadState( mJoypadID, &state );

		// ボタン状態の記憶
		for ( int i = 0; i < LN_MAX_JOYPAD_BUTTONS; ++i )
		{
			if ( state.Buttons[ i ] )
			{
				++mButtonStatus[ i ];

				if ( mButtonStatus[ i ] > mRemRepeatInterval + mFirstRepeatInterval )
				{
					mButtonStatus[ i ] = mFirstRepeatInterval;
				}
			}
			else
			{
				if ( mButtonStatus[ i ] > 0 )
				{
					mButtonStatus[ i ] = -1;
				}
				else
				{
					mButtonStatus[ i ] = 0;
				}
			}
		}

		// 軸状態の記憶
		for ( int i = 0; i < LN_MAX_JOYPAD_AXIS; ++i )
		{
			mAxisStatus[ i ] = state.Axes[ i ];
		}

		// POV
		mPOVState = state.POV;
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