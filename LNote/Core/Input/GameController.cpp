//=============================================================================
//【 GameController 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <algorithm>
#include "Manager.h"
#include "GameController.h"

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
// ■ GameController クラス
//=============================================================================


    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    GameController::GameController( Manager* manager_, int joypad_id_ )
        : mManager              ( manager_ )
        , mInputDevice          ( manager_->getInputDevice() )
        , mDeviceAttachFlag     ( LN_INPUTATTACH_ALL )
        , mJoypadID             ( joypad_id_ )
        , mFirstRepeatInterval	( 20 )
		, mRemRepeatInterval	( 5 )
        , mPressedTriggerFlag   ( false )
    {
        memset( mButtonStatus, 0, sizeof( mButtonStatus ) );
        memset( mButtonPower, 0, sizeof( mButtonPower ) );
        memset( mAxisState, 0, sizeof( mAxisState ) );

        

        mAttachedVButtons[ LN_AT_BUTTON_A ].push_back( LN_KEY_Z );
        mAttachedVButtons[ LN_AT_BUTTON_A ].push_back( LN_VBUTTON_JOYPAD_BUTTON_1 );
	    mAttachedVButtons[ LN_AT_BUTTON_B ].push_back( LN_KEY_X );
        mAttachedVButtons[ LN_AT_BUTTON_B ].push_back( LN_VBUTTON_JOYPAD_BUTTON_2 );
	    mAttachedVButtons[ LN_AT_BUTTON_C ].push_back( LN_KEY_C );
        mAttachedVButtons[ LN_AT_BUTTON_C ].push_back( LN_VBUTTON_JOYPAD_BUTTON_3 );
	    mAttachedVButtons[ LN_AT_BUTTON_X ].push_back( LN_KEY_A );
        mAttachedVButtons[ LN_AT_BUTTON_X ].push_back( LN_VBUTTON_JOYPAD_BUTTON_4 );
	    mAttachedVButtons[ LN_AT_BUTTON_Y ].push_back( LN_KEY_S );
        mAttachedVButtons[ LN_AT_BUTTON_Y ].push_back( LN_VBUTTON_JOYPAD_BUTTON_5 );
	    mAttachedVButtons[ LN_AT_BUTTON_Z ].push_back( LN_KEY_D );
        mAttachedVButtons[ LN_AT_BUTTON_Z ].push_back( LN_VBUTTON_JOYPAD_BUTTON_6 );
	    mAttachedVButtons[ LN_AT_BUTTON_L ].push_back( LN_KEY_Q );
        mAttachedVButtons[ LN_AT_BUTTON_L ].push_back( LN_VBUTTON_JOYPAD_BUTTON_7 );
	    mAttachedVButtons[ LN_AT_BUTTON_R ].push_back( LN_KEY_W );
        mAttachedVButtons[ LN_AT_BUTTON_R ].push_back( LN_VBUTTON_JOYPAD_BUTTON_8 );

	    mAttachedVButtons[ LN_AT_BUTTON_L2 ].push_back( LN_VBUTTON_JOYPAD_BUTTON_9 );
	    mAttachedVButtons[ LN_AT_BUTTON_R2 ].push_back( LN_VBUTTON_JOYPAD_BUTTON_10 );
	    mAttachedVButtons[ LN_AT_BUTTON_L3 ].push_back( LN_VBUTTON_JOYPAD_BUTTON_11 );
	    mAttachedVButtons[ LN_AT_BUTTON_R3 ].push_back( LN_VBUTTON_JOYPAD_BUTTON_12 );
	    mAttachedVButtons[ LN_AT_BUTTON_START ].push_back( LN_VBUTTON_JOYPAD_BUTTON_13 );
	    mAttachedVButtons[ LN_AT_BUTTON_SELECT ].push_back( LN_VBUTTON_JOYPAD_BUTTON_14 );
	    mAttachedVButtons[ LN_AT_BUTTON_SUB1 ].push_back( LN_VBUTTON_JOYPAD_BUTTON_15 );
	    mAttachedVButtons[ LN_AT_BUTTON_SUB2 ].push_back( LN_VBUTTON_JOYPAD_BUTTON_16 );

	    mAttachedVButtons[ LN_AT_BUTTON_DOWN ].push_back( LN_KEY_DOWN );
        mAttachedVButtons[ LN_AT_BUTTON_DOWN ].push_back( LN_VBUTTON_JOYPAD_POV_DOWN );
        mAttachedVButtons[ LN_AT_BUTTON_DOWN ].push_back( LN_VBUTTON_JOYPAD_AXIS_2_MINUIS );

	    mAttachedVButtons[ LN_AT_BUTTON_LEFT ].push_back( LN_KEY_LEFT );
        mAttachedVButtons[ LN_AT_BUTTON_LEFT ].push_back( LN_VBUTTON_JOYPAD_POV_LEFT );
        mAttachedVButtons[ LN_AT_BUTTON_LEFT ].push_back( LN_VBUTTON_JOYPAD_AXIS_1_MINUIS );

	    mAttachedVButtons[ LN_AT_BUTTON_RIGHT ].push_back( LN_KEY_RIGHT );
        mAttachedVButtons[ LN_AT_BUTTON_RIGHT ].push_back( LN_VBUTTON_JOYPAD_POV_RIGHT );
        mAttachedVButtons[ LN_AT_BUTTON_RIGHT ].push_back( LN_VBUTTON_JOYPAD_AXIS_1_PLUS );

	    mAttachedVButtons[ LN_AT_BUTTON_UP ].push_back( LN_KEY_UP );
        mAttachedVButtons[ LN_AT_BUTTON_UP ].push_back( LN_VBUTTON_JOYPAD_POV_UP );
        mAttachedVButtons[ LN_AT_BUTTON_UP ].push_back( LN_VBUTTON_JOYPAD_AXIS_2_MINUIS );

        mAttachedVButtons[ LN_AT_AXIS_1X_MINUIS ].push_back( LN_KEY_LEFT );
        mAttachedVButtons[ LN_AT_AXIS_1X_MINUIS ].push_back( LN_VBUTTON_JOYPAD_POV_LEFT );
        mAttachedVButtons[ LN_AT_AXIS_1X_MINUIS ].push_back( LN_VBUTTON_JOYPAD_AXIS_1_MINUIS );

        mAttachedVButtons[ LN_AT_AXIS_1X_PLUS   ].push_back( LN_KEY_RIGHT );
        mAttachedVButtons[ LN_AT_AXIS_1X_PLUS   ].push_back( LN_VBUTTON_JOYPAD_POV_RIGHT );
        mAttachedVButtons[ LN_AT_AXIS_1X_PLUS   ].push_back( LN_VBUTTON_JOYPAD_AXIS_1_PLUS );

        mAttachedVButtons[ LN_AT_AXIS_1Y_MINUIS ].push_back( LN_KEY_UP );
        mAttachedVButtons[ LN_AT_AXIS_1Y_MINUIS ].push_back( LN_VBUTTON_JOYPAD_POV_UP );
        mAttachedVButtons[ LN_AT_AXIS_1Y_MINUIS ].push_back( LN_VBUTTON_JOYPAD_AXIS_2_MINUIS );

        mAttachedVButtons[ LN_AT_AXIS_1Y_PLUS   ].push_back( LN_KEY_DOWN );
        mAttachedVButtons[ LN_AT_AXIS_1Y_PLUS   ].push_back( LN_VBUTTON_JOYPAD_POV_DOWN );
        mAttachedVButtons[ LN_AT_AXIS_1Y_PLUS   ].push_back( LN_VBUTTON_JOYPAD_AXIS_2_PLUS );

        mAttachedVButtons[ LN_AT_AXIS_2X_MINUIS ].push_back( LN_VBUTTON_JOYPAD_AXIS_3_MINUIS );
        mAttachedVButtons[ LN_AT_AXIS_2X_PLUS   ].push_back( LN_VBUTTON_JOYPAD_AXIS_3_PLUS );
        mAttachedVButtons[ LN_AT_AXIS_2Y_MINUIS ].push_back( LN_VBUTTON_JOYPAD_AXIS_4_MINUIS );
        mAttachedVButtons[ LN_AT_AXIS_2Y_PLUS   ].push_back( LN_VBUTTON_JOYPAD_AXIS_4_PLUS );
    }

	//---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    GameController::~GameController()
    {
    }

	
	//---------------------------------------------------------------------
	// ● ボタンが現在押されているかを判定する
    //---------------------------------------------------------------------
    bool GameController::isPress( LNGameControllerButton button_ )
    {
        return ( mButtonStatus[ button_ ] > 0 );
    }

	//---------------------------------------------------------------------
	// ● ボタンが新しく押された瞬間を判定する
    //---------------------------------------------------------------------
    bool GameController::isOnTrigger( LNGameControllerButton button_ )
    {
        return ( mButtonStatus[ button_ ] == 1 );
    }

	//---------------------------------------------------------------------
	// ● キーが離された瞬間を判定する
    //---------------------------------------------------------------------
    bool GameController::isOffTrigger( LNGameControllerButton button_ )
    {
        return ( mButtonStatus[ button_ ] == -1 );
    }

	//---------------------------------------------------------------------
	// ● ボタンが新しく押された瞬間とリピート状態を判定する
    //---------------------------------------------------------------------
    bool GameController::isRepeat( LNGameControllerButton button_ )
    {
        s32 s = mButtonStatus[ button_ ];
		return ( ( s == 1 ) || ( s > mFirstRepeatInterval && s % mRemRepeatInterval == 0 ) );
    }

    //---------------------------------------------------------------------
	// ● アナログ入力値の取得
    //---------------------------------------------------------------------
    float GameController::getPower( LNGameControllerButton button_ )
    {
        return mButtonPower[ button_ ];
    }

    //---------------------------------------------------------------------
	// ● 軸の状態の取得
    //---------------------------------------------------------------------
    const LVector2& GameController::getAxisState( int axis_ )
    {
        if ( axis_ < 0 || LN_MAX_JOYPAD_AXIS / 2 <= axis_ ) return LVector2::ZERO;
		return mAxisState[ axis_ ];
    }

    //---------------------------------------------------------------------
	// ● 新しく押された仮想ボタンの取得
    //---------------------------------------------------------------------
    int GameController::getPressedButton()
    {
        bool mouse    = ( mDeviceAttachFlag & LN_INPUTATTACH_MOUSE ) != 0;
        bool keyboard = ( mDeviceAttachFlag & LN_INPUTATTACH_KEYBOARD ) != 0;
        int  joypad   = ( ( mDeviceAttachFlag & LN_INPUTATTACH_JOYPAD ) != 0 ) ? mJoypadID : -1;
        int  button = LN_VBUTTON_NULL;
        for ( int i = 1; i < LN_VBUTTON_LAST; ++i )
        {
            if ( mInputDevice->getVirtualButtonState( i, keyboard, mouse, joypad ) >= LN_JOYPAD_SWITCH_RANGE )
            {
                button = i;
            }
        }

        // 何か押されていた
        if ( button != LN_VBUTTON_NULL )
        {
            if ( mPressedTriggerFlag )
            {
                button = LN_VBUTTON_NULL;
            }
            else
            {
                mPressedTriggerFlag = true;
            }
        }
        else
        {
            mPressedTriggerFlag = false;
        }
        return button;
    }

    //---------------------------------------------------------------------
	// ● ボタン配置 (キーコンフィグ) を設定する
    //---------------------------------------------------------------------
    void GameController::attachVirtualButton( LNGameControllerAttachTarget target_, int v_button_, bool duplication_ )
    {
        AttachedVButtons& buttons = mAttachedVButtons[ target_ ];

        if ( std::find( buttons.begin(), buttons.end(), v_button_ ) != buttons.end() )
        {
            buttons.push_back( v_button_ );
        }
    }

    //---------------------------------------------------------------------
	// ● ボタン配置 (キーコンフィグ) を解除する
    //---------------------------------------------------------------------
    void GameController::detachVirtualButton( LNGameControllerAttachTarget target_, int v_button_ )
    {
        AttachedVButtons& buttons = mAttachedVButtons[ target_ ];

        AttachedVButtons::iterator end_it = std::remove( buttons.begin(), buttons.end(), v_button_ );
	    buttons.erase( end_it, buttons.end() );
    }

    //---------------------------------------------------------------------
	// ● フレーム更新
    //---------------------------------------------------------------------
	void GameController::update()
    {
        bool mouse    = ( mDeviceAttachFlag & LN_INPUTATTACH_MOUSE ) != 0;
        bool keyboard = ( mDeviceAttachFlag & LN_INPUTATTACH_KEYBOARD ) != 0;
        int  joypad   = ( ( mDeviceAttachFlag & LN_INPUTATTACH_JOYPAD ) != 0 ) ? mJoypadID : -1;
        lnfloat v;
        lnfloat power;

        AttachedVButtons::iterator itr;
        AttachedVButtons::iterator end;

        memset( mAxisState, 0, sizeof( mAxisState ) );

        for ( lnU32 i = 0; i < LN_MAX_AT_TARGETS; ++i )
        {
            power = 0.0f;
            itr = mAttachedVButtons[ i ].begin();
            end = mAttachedVButtons[ i ].end();

            for ( ; itr != end; ++itr )
            {
                v = mInputDevice->getVirtualButtonState( (*itr), keyboard, mouse, joypad );
                if ( v > power )
                {
                    power = v;
                } 
            }

            // 仮想コントローラのボタンに対する入力
            if ( LN_AT_BUTTON_BEGIN <= i && i <= LN_AT_BUTTON_END )
            {
                mButtonPower[ i ] = power;

                // 十分に押されている場合
                if ( power >= LN_JOYPAD_SWITCH_RANGE )
                {
                    ++mButtonStatus[ i ];

				    //if ( mButtonStatus[ i ] > mRemRepeatInterval + mFirstRepeatInterval )
				    //{
					   // mButtonStatus[ i ] = mFirstRepeatInterval;
				    //}
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
            // 仮想コントローラのレバーに対する入力
            else if ( LN_AT_AXIS_BEGIN <= i && i <= LN_AT_AXIS_END )
            {
                if ( ( (i - LN_AT_AXIS_BEGIN) & 1 ) == 0 )
                {
                    power *= -1.0f;
                }
                lnfloat* w = &((lnfloat*)mAxisState)[ ( i - LN_AT_AXIS_BEGIN ) / 2 ];
                *w += power;
                if ( *w > 1.0f )
                {
                    *w = 1.0f;
                }
                else if ( *w < -1.0f )
                {
                    *w = -1.0f;
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