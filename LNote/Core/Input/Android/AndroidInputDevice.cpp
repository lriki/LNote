//=============================================================================
//【 AndroidInputDevice 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"

#if defined(LNOTE_ANDROID)
#include "AndroidInputDevice.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Input
{
namespace Android
{

//=============================================================================
// ■ AndroidInputDevice クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    AndroidInputDevice::AndroidInputDevice()
    {
        memset( mTouchState, 0, sizeof( mTouchState ) );
        memset( &mMouseState, 0, sizeof( mMouseState ) );
    }

	//---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    AndroidInputDevice::~AndroidInputDevice()
    {
    }

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
	void AndroidInputDevice::initialize( const InitData& init_data_ )
    {
        refresh();
        update();
    }

    //---------------------------------------------------------------------
	// ● 仮想ボタンの状態の取得
    //---------------------------------------------------------------------
    float AndroidInputDevice::getVirtualButtonState( u32 button_, bool keyboard_, bool mouse_, int joypad_id_ )
    {
        // キーボード
        if ( keyboard_ && LN_VBUTTON_KEYBOARD_BEGIN <= button_ && button_ <= LN_VBUTTON_KEYBOARD_END )
        {
            return ( getKeyState( (LNVirtualKeyCode)button_ ) ) ? 1.0f : 0.0f;
        }

        // マウス
        if ( mouse_ && LN_VBUTTON_MOUSE_BEGIN <= button_ && button_ <= LN_VBUTTON_MOUSE_END )
        {
            return ( mMouseState.Buttons[ button_ - LN_VBUTTON_MOUSE_BEGIN ] ) ? 1.0f : 0.0f;
        }

        // ジョイパッド
        /*
        if ( joypad_id_ >= 0 && joypad_id_ < mJoypadNum )
        {

        }
        */

        return 0.0f;
    }

    //---------------------------------------------------------------------
	// ● マウス座標の設定
    //---------------------------------------------------------------------
    void AndroidInputDevice::setMousePoint( const LVector2& point_ )
    {
        
    }

    //---------------------------------------------------------------------
	// ● マウスカーソルの位置をクライアント領域の中央に設定する
    //---------------------------------------------------------------------
    void AndroidInputDevice::setMouseCursorCenter()
    {

    }

    //---------------------------------------------------------------------
	// ● 入力情報の更新
    //---------------------------------------------------------------------
    LNRESULT AndroidInputDevice::update()
    {
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● イベント処理 (Manager::onEvent() から呼ばれる)
    //---------------------------------------------------------------------
    bool AndroidInputDevice::onEvent( const System::Event& args_ )
    {
        const System::TouchEvent& ev_touch = (const System::TouchEvent&)args_;

        switch ( args_.Type )
        {
            case LNEV_TOUCH_DOWN:
            {
                mTouchState[ev_touch.TouchID].Touch = true;
                mTouchState[ev_touch.TouchID].X = ev_touch.X;
                mTouchState[ev_touch.TouchID].Y = ev_touch.Y;
                mTouchState[ev_touch.TouchID].RelX = ev_touch.MoveX;
                mTouchState[ev_touch.TouchID].RelY = ev_touch.MoveY;
                break;
            }
            case LNEV_TOUCH_UP:
            {
                mTouchState[ev_touch.TouchID].Touch = false;
                mTouchState[ev_touch.TouchID].X = ev_touch.X;
                mTouchState[ev_touch.TouchID].Y = ev_touch.Y;
                mTouchState[ev_touch.TouchID].RelX = ev_touch.MoveX;
                mTouchState[ev_touch.TouchID].RelY = ev_touch.MoveY;
                break;
            }
            case LNEV_TOUCH_MOVE:
            {
                mTouchState[ev_touch.TouchID].Touch = true;
                mTouchState[ev_touch.TouchID].X = ev_touch.X;
                mTouchState[ev_touch.TouchID].Y = ev_touch.Y;
                mTouchState[ev_touch.TouchID].RelX = ev_touch.MoveX;
                mTouchState[ev_touch.TouchID].RelY = ev_touch.MoveY;
                break;
            }
        }
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Android
} // namespace Input
} // namespace Core
} // namespace LNote

#endif // LNOTE_ANDROID

//=============================================================================
//								end of file
//=============================================================================