//=============================================================================
//【 Manager 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#if defined(LNOTE_WINDOWS)
    #include "Win32/InputDevice.h"
#elif defined(LNOTE_ANDROID)
    #include "Android/AndroidInputDevice.h"
#endif
#include "Joypad.h"
#include "Manager.h"

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
// ■ Manager クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    Manager::Manager()
        : mLogFile      ( NULL )
        , mInputDevice  ( NULL )
		, mMouse        ( NULL )
		, mKeyboard     ( NULL )
        , mTouch        ( NULL )
        , mCanSetMousePoint ( false )
    {
        memset( mGameControllers, 0, sizeof( mGameControllers ) );
		memset( mJoypads, 0, sizeof( mJoypads ) );
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    Manager::~Manager()
    {
        for ( int i = 0; i < LN_MAX_GAME_CONTROLLERS; ++i )
		{
		    LN_SAFE_RELEASE( mGameControllers[ i ] );
		}
        for ( int i = 0; i < LN_MAX_JOYPADS; ++i )
		{
			LN_SAFE_RELEASE( mJoypads[ i ] );
		}
        LN_SAFE_RELEASE( mTouch );
        LN_SAFE_RELEASE( mKeyboard );
        LN_SAFE_RELEASE( mMouse );
        LN_SAFE_RELEASE( mInputDevice );
    }

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    LNRESULT Manager::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        LNRESULT lr;

#if defined(LNOTE_WINDOWS)
        Win32::InputDevice::InitData data;
		data.Window		= init_data_.Window;
		Win32::InputDevice* device = LN_NEW Win32::InputDevice();
        LN_CALL_R( device->initialize( data ) );
        mInputDevice = device;
#elif defined(LNOTE_ANDROID)
        Android::AndroidInputDevice::InitData data;
        Android::AndroidInputDevice* device = LN_NEW Android::AndroidInputDevice();
        device->initialize( data );
		mInputDevice = device;
#endif
        
        mMouse    = LN_NEW Mouse( this );
        mKeyboard = LN_NEW Keyboard( this );
        mTouch    = LN_NEW Touch( this );

		for ( int i = 0; i < LN_MAX_JOYPADS; ++i )
		{
			mJoypads[ i ] = LN_NEW Joypad( this, i );
		}

        for ( int i = 0; i < LN_MAX_GAME_CONTROLLERS; ++i )
		{
		    mGameControllers[ i ] = LN_NEW GameController( this, i );
		}


        LN_LOG_INIT_END;
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 終了処理
    //---------------------------------------------------------------------
    void Manager::finalize()
    {
        LN_LOG_FIN_BEGIN;

        LN_LOG_FIN_END;
    }

    //---------------------------------------------------------------------
	// ● Joypad へのポインタの取得
    //---------------------------------------------------------------------
    IJoypad* Manager::getJoypad( int index_ )
    {
        if ( 0 <= index_ && index_ < LN_MAX_JOYPADS )
        {
            return mJoypads[ index_ ];
        }
        return NULL;
    }

    //---------------------------------------------------------------------
	// ● IInputDevice へのポインタの取得
    //---------------------------------------------------------------------
    IInputDevice* Manager::getInputDevice()
    {
        return mInputDevice;
    }

    //---------------------------------------------------------------------
	// ● フレーム更新
    //---------------------------------------------------------------------
    void Manager::update()
    {
        mInputDevice->update();
        mMouse->update();
        mKeyboard->update();
        mTouch->update();

        for ( int i = 0; i < LN_MAX_JOYPADS; ++i )
		{
			mJoypads[ i ]->update();
		}

        for ( int i = 0; i < LN_MAX_GAME_CONTROLLERS; ++i )
		{
		    mGameControllers[ i ]->update();
		}
    }

    //---------------------------------------------------------------------
	// ● メッセージ処理コールバック
    //---------------------------------------------------------------------
    bool Manager::onEvent( const System::Event& args_ )
    {
        switch ( args_.Type )
        {
            /////////////////// マウスボタンが離された
            case LNEV_MOUSE_UP:
            {
                if ( mInputDevice->getWindow()->isActive() )
                {
                    mCanSetMousePoint = true;
                }
                break;
            }

            /////////////////// ウィンドウが非アクティブになった
            case LNEV_APP_DEACTIVATE:
            {
                mCanSetMousePoint = false;

                // すべてのパッドの振動を止める
                for ( int i = 0; i < LN_MAX_JOYPADS; ++i )
		        {
                    mJoypads[ i ]->stopVibration();
		        }

                break;
            }
        }
        
        if ( mInputDevice->onEvent( args_ ) ) return true;
        
        return false;
    }

    //---------------------------------------------------------------------
	// ● マウス位置が設定できるか
    //---------------------------------------------------------------------
    bool Manager::canSetMousePoint() const
    {
        return ( mInputDevice->getWindow()->isActive() && mCanSetMousePoint ); 
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Input
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================