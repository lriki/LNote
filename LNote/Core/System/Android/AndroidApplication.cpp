//=============================================================================
//【 AndroidApplication 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"

#if defined(LNOTE_ANDROID)
#include "../../../AndroidMain/native_app_glue.h"
#include <android/native_window.h>
#include "../Manager.h"
#include "AndroidWindow.h"
#include "AndroidApplication.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{

//=============================================================================
// ■ AndroidApplication クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    AndroidApplication::AndroidApplication()
        : mAndroidApp   ( NULL )
        , mWindowInited ( false )
        , mFocused      ( false )
        , mAnimating    ( false )
        , mIsDeviceLost ( false )
    {

    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    AndroidApplication::~AndroidApplication()
    {

    }

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    void AndroidApplication::initialize( const AndroidApplication::InitData& init_data_ )
    {
        this->mManager = init_data_.SystemManager;
        
        mAndroidInitData = init_data_;
        mAndroidApp = init_data_.AndroidApp;
        
        mAndroidApp->userData = this;
        mAndroidApp->onAppCmd = engine_handle_cmd;
        mAndroidApp->onInputEvent = engine_handle_input;
        
        
        
        // ウィンドウの初期化が完全に終わるまで待つ
        while ( !mWindowInited )
        {
            onProcMessage();
        }
        
        onCreateMainWindow( &this->mMainWindow );

        mIsDeviceLost = false;
    }

    //---------------------------------------------------------------------
	// ● メインウィンドウを作る
    //---------------------------------------------------------------------
    LNRESULT AndroidApplication::onCreateMainWindow( WindowBase** window_ )
    {
        AndroidWindow::InitData data;
        data.LogFile        = mLogFile;
        data.Width          = mAndroidInitData.Width;
        data.Height         = mAndroidInitData.Height;
        AndroidWindow* w = LN_NEW AndroidWindow( this );
        *window_ = w;
        return w->initialize( data );
    }


    //---------------------------------------------------------------------
	// ● ダミーウィンドウを作る
    //---------------------------------------------------------------------
    WindowBase* AndroidApplication::onCreateDummyWindow( void* window_handle_ )
    {
        LN_THROW_NotImpl( 0, "" );
        return NULL;
    }

    //---------------------------------------------------------------------
	// ● ウィンドウの finalize を呼び出して参照カウントをひとつ減らす
    //---------------------------------------------------------------------
    void AndroidApplication::onFinalizeWindow( WindowBase* window_ )
    {
        window_->finalize();
        LN_SAFE_RELEASE( window_ );
    }

    //---------------------------------------------------------------------
	// ● コンソールを開く
    //---------------------------------------------------------------------
    void AndroidApplication::onAllocConsole()
    {
        LN_THROW_NotImpl( 0, "" );
    }

    //---------------------------------------------------------------------
	// ● コンソールを閉じる
    //---------------------------------------------------------------------
    void AndroidApplication::onFreeConsole()
    {
    }

#define unlikely(x) __builtin_expect(!!(x), 0)
    
    //---------------------------------------------------------------------
	// ● メッセージ処理
    //---------------------------------------------------------------------
    void AndroidApplication::onProcMessage()
    {
        int ident;
        int events;
        android_poll_source* source;

        while ( ( ident = ALooper_pollAll( mAnimating ? 0 : -1, NULL, &events, (void**)&source ) ) >= 0 )
        {
            // Process this event.
            if ( source != NULL )
            {
                source->process( mAndroidApp, source );
            }
            
            if ( unlikely( mAndroidApp->destroyRequested != 0 ) )
            {
                break; 
            }
        }
    }

    //---------------------------------------------------------------------
	// ● マウスカーソルの表示状態を設定する
    //---------------------------------------------------------------------
    void AndroidApplication::onShowCursor( bool flag_ )
	{
		
	}

    //---------------------------------------------------------------------
	// ● イベントコールバック
    //---------------------------------------------------------------------
    void AndroidApplication::engine_handle_cmd( android_app* app_, int32_t cmd_ )
    {
        //struct engine* engine = (struct engine*)app->userData;
        AndroidApplication* app = (AndroidApplication*)app_->userData;

        switch ( cmd_ )
        {
            case APP_CMD_INPUT_CHANGED:
                _ln_printf_core("APP_CMD_INPUT_CHANGED");
                break;
            case APP_CMD_INIT_WINDOW:
            {
                _ln_printf_core("APP_CMD_INIT_WINDOW");
                
                app->mWindowInited = true;
                /*
                LNEvent ev;
                ev.Type = ;
                app->postMessage( ev );
                */
                /*
                Event* e = createEvent< Event >( LNEV_ANDROID_INIT_WINDOW );
                this->mManager->sendEvent( e );
                */
                break;
            }
            case APP_CMD_TERM_WINDOW:
            {
                _ln_printf_core("APP_CMD_TERM_WINDOW");
                app->mWindowInited = false;
                app->mAnimating = false;
                break;
            }
            case APP_CMD_WINDOW_RESIZED:
                _ln_printf_core("APP_CMD_WINDOW_RESIZED");
                break;
            case APP_CMD_WINDOW_REDRAW_NEEDED:
                _ln_printf_core("APP_CMD_WINDOW_REDRAW_NEEDED");
                break;
            case APP_CMD_CONTENT_RECT_CHANGED:
                _ln_printf_core("APP_CMD_CONTENT_RECT_CHANGED");
                break;
            case APP_CMD_GAINED_FOCUS:
            {
                _ln_printf_core("APP_CMD_GAINED_FOCUS");
                if ( app_->window == NULL )
                {
                    //LN_LOG_WRITE( _T( "Application gained focus but window was collapsed." ) );
                    
                    break;
                }
            
                if ( app->mFocused ) break;
                
                app->mFocused = true;
                app->mAnimating = true;
                break;
            }
            case APP_CMD_LOST_FOCUS:
            {
                _ln_printf_core("APP_CMD_LOST_FOCUS");
                if ( !app->mFocused ) break;
                
                app->mFocused = false;
                app->mAnimating = false;
                break;
            }
            case APP_CMD_CONFIG_CHANGED:
                _ln_printf_core("APP_CMD_CONFIG_CHANGED");
                break;
            case APP_CMD_LOW_MEMORY:
                _ln_printf_core("APP_CMD_LOW_MEMORY");
                break;
            case APP_CMD_START:
                _ln_printf_core("APP_CMD_START");
                break;
            case APP_CMD_RESUME:
                _ln_printf_core("APP_CMD_RESUME");
                if (app->mIsDeviceLost)
                {
                    Event* e = createEvent< Event >( LNEV_ANDROID_RESUMED );
                    app->mManager->sendEvent( e );
                    app->mIsDeviceLost = false;
                }
                break;
            case APP_CMD_SAVE_STATE:
                _ln_printf_core("APP_CMD_SAVE_STATE");
                //engine->app->savedState = malloc(sizeof(struct saved_state));
                //*((struct saved_state*)engine->app->savedState) = engine->state;
                //engine->app->savedStateSize = sizeof(struct saved_state);
                break;
            case APP_CMD_PAUSE:
                _ln_printf_core("APP_CMD_PAUSE");
                if (!app->mIsDeviceLost)
                {
                    Event* e = createEvent< Event >( LNEV_ANDROID_PAUSED );
                    app->mManager->sendEvent( e );
                    app->mIsDeviceLost = true;
                }
                break;
            case APP_CMD_STOP:
                _ln_printf_core("APP_CMD_STOP %d", app_->destroyRequested);
                break;
            case APP_CMD_DESTROY:
            {
                //LNote::JavaGlue::call_Void_Void( "setForground" );
                _ln_printf_core("APP_CMD_DESTROY");
                Event* e = createEvent< Event >( LNEV_CLOSE );
                app->postMessage( e );
                break;
            }
        }
    }
    
    //---------------------------------------------------------------------
	// ● 入力イベントコールバック
    //---------------------------------------------------------------------
    int32_t AndroidApplication::engine_handle_input( android_app* android_app_, AInputEvent* event_ )
    {
        AndroidApplication* app = (AndroidApplication*)android_app_->userData;
        
        int32_t type;
        float x, y;
        int32_t action, keycode;
        char* action_str;

        // 入力の種類を取得
        type = AInputEvent_getType( event_ );

        // タッチの場合
        if ( type == AINPUT_EVENT_TYPE_MOTION )
        {
            lnU32 pointer_count = AMotionEvent_getPointerCount( event_ );
            for ( lnU32 i = 0; i < pointer_count; ++i )
            {
                lnU32 pointer_id = AMotionEvent_getPointerId( event_, i );
                lnU32 action = AMotionEvent_getAction( event_ ) & AMOTION_EVENT_ACTION_MASK;
                lnU32 pointer_index = i;
            
                if ( action == AMOTION_EVENT_ACTION_POINTER_DOWN || action == AMOTION_EVENT_ACTION_POINTER_UP )
                {
                    pointer_index = (AMotionEvent_getAction( event_ ) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                    pointer_id = AMotionEvent_getPointerId( event_, pointer_index );
                }
                
                if ( pointer_id > LN_MAX_TOUCH_DATA ) continue;
                
                TouchEvent* ev = NULL;
                
                switch ( action )
                {
                    case AMOTION_EVENT_ACTION_DOWN:
                    case AMOTION_EVENT_ACTION_POINTER_DOWN:
                        ev = createEvent< TouchEvent >( LNEV_TOUCH_DOWN );
                        break;
                    case AMOTION_EVENT_ACTION_UP:
                    case AMOTION_EVENT_ACTION_POINTER_UP:
                        ev = createEvent< TouchEvent >( LNEV_TOUCH_UP );
                        break;
                    case AMOTION_EVENT_ACTION_MOVE:
                        ev = createEvent< TouchEvent >( LNEV_TOUCH_MOVE );
                        break;
                    default:
                        continue;
                }
                
                if (ev)
                {
                    ev->TouchID = pointer_id;
                    ev->X       = AMotionEvent_getX( event_, pointer_index );
                    ev->Y       = AMotionEvent_getY( event_, pointer_index );
                    
                    if ( ev->Type == LNEV_TOUCH_MOVE )
                    {
                        ev->MoveX = ev->X - app->mLastTouchPoints[pointer_id].x;
                        ev->MoveY = ev->Y - app->mLastTouchPoints[pointer_id].y;
                    }
                    else
                    {
                        ev->MoveX = 0;
                        ev->MoveY = 0;
                    }
                    
                    app->mLastTouchPoints[pointer_id].x = ev->X;
                    app->mLastTouchPoints[pointer_id].y = ev->Y;
                    
                    /*
                    _ln_printf_core( "id:%d (%d, %d) (%d, %d)",
                        ev->TouchID,
                        ev->X, ev->Y,
                        ev->MoveX, ev->MoveY );
                    */
                    
                    app->postMessage( (Event*)ev );
                }
            }
            // 処理済みにする
            return 1;
        } 
        // キー操作の場合
        else if( type == AINPUT_EVENT_TYPE_KEY )
        {
            _ln_printf_core( "AINPUT_EVENT_TYPE_KEY" );
            
            KeyEvent* ev = NULL;
            
            action = AKeyEvent_getAction( event_ );

            switch ( action )
            {
                case AKEY_EVENT_ACTION_DOWN:    
                    ev = createEvent< KeyEvent >( LNEV_KEY_DOWN );
                    break;
                case AKEY_EVENT_ACTION_UP:  
                    ev = createEvent< KeyEvent >( LNEV_KEY_UP );
                    break;
                case AKEY_EVENT_ACTION_MULTIPLE:
                    ev = createEvent< KeyEvent >( LNEV_KEY_CHAR );
                    break;
                default : 
                	return 0;
            }
            
            
            /*
           if(action == AKEY_EVENT_ACTION_DOWN)
            action_str = "down";
            else if(action == AKEY_EVENT_ACTION_UP)
                action_str = "up";
            else if(action == AKEY_EVENT_ACTION_MULTIPLE)
                action_str = "multiple";
            */
           
            if (ev)
            {
                ev->KeyCode   = _convertKeyCodeAndroidToLN( AKeyEvent_getKeyCode( event_ ) );
	            ev->IsAlt     = false;
	            ev->IsShift   = false;
	            ev->IsControl = false;
                
                if ( ev->KeyCode != LN_KEY_UNKNOWN )
                {
                    //_ln_printf_core( "KeyEvent:%s %d %c", action_str, ev.Key.KeyCode, ev.Key.KeyCode );
                    
                    app->postMessage( (Event*)ev );
                    
                    // 自分で処理を行うので1を返す
                    return 1;
                }
            }
            return 0;
        }

        return 0;
    }
    
    
    
    //---------------------------------------------------------------------
	// ● Android -> LNKey キーコード変換
    //---------------------------------------------------------------------
    lnU32 AndroidApplication::_convertKeyCodeAndroidToLN( lnU32 key_ )
    {
        static lnU32 gKeyTable[256] = { 0 };
        static bool gKeyTableInit = false;
        if ( !gKeyTableInit )
        {
            gKeyTable[AKEYCODE_UNKNOWN]     = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_SOFT_LEFT]   = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_SOFT_RIGHT]  = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_HOME]        = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_BACK]        = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_CALL]        = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_ENDCALL]     = LN_KEY_UNKNOWN;
            
            gKeyTable[AKEYCODE_0]           = LN_KEY_0;
            gKeyTable[AKEYCODE_1]           = LN_KEY_1;
            gKeyTable[AKEYCODE_2]           = LN_KEY_2;
            gKeyTable[AKEYCODE_3]           = LN_KEY_3;
            gKeyTable[AKEYCODE_4]           = LN_KEY_4;
            gKeyTable[AKEYCODE_5]           = LN_KEY_5;
            gKeyTable[AKEYCODE_6]           = LN_KEY_6;
            gKeyTable[AKEYCODE_7]           = LN_KEY_7;
            gKeyTable[AKEYCODE_8]           = LN_KEY_8;
            gKeyTable[AKEYCODE_9]           = LN_KEY_9;
            
            gKeyTable[AKEYCODE_STAR]        = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_POUND]       = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_DPAD_UP]     = LN_KEY_UP;
            gKeyTable[AKEYCODE_DPAD_DOWN]   = LN_KEY_DOWN;
            gKeyTable[AKEYCODE_DPAD_LEFT]   = LN_KEY_LEFT;
            gKeyTable[AKEYCODE_DPAD_RIGHT]  = LN_KEY_RIGHT;
            gKeyTable[AKEYCODE_DPAD_CENTER] = LN_KEY_ENTER;
            gKeyTable[AKEYCODE_VOLUME_UP]   = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_VOLUME_DOWN] = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_POWER]       = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_CAMERA]      = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_CLEAR]       = LN_KEY_UNKNOWN;
            
            gKeyTable[AKEYCODE_A]           = LN_KEY_A;
            gKeyTable[AKEYCODE_B]           = LN_KEY_B;
            gKeyTable[AKEYCODE_C]           = LN_KEY_C;
            gKeyTable[AKEYCODE_D]           = LN_KEY_D;
            gKeyTable[AKEYCODE_E]           = LN_KEY_E;
            gKeyTable[AKEYCODE_F]           = LN_KEY_F;
            gKeyTable[AKEYCODE_G]           = LN_KEY_G;
            gKeyTable[AKEYCODE_H]           = LN_KEY_H;
            gKeyTable[AKEYCODE_I]           = LN_KEY_I;
            gKeyTable[AKEYCODE_J]           = LN_KEY_J;
            gKeyTable[AKEYCODE_K]           = LN_KEY_K;
            gKeyTable[AKEYCODE_L]           = LN_KEY_L;
            gKeyTable[AKEYCODE_M]           = LN_KEY_M;
            gKeyTable[AKEYCODE_N]           = LN_KEY_N;
            gKeyTable[AKEYCODE_O]           = LN_KEY_O;
            gKeyTable[AKEYCODE_P]           = LN_KEY_P;
            gKeyTable[AKEYCODE_Q]           = LN_KEY_Q;
            gKeyTable[AKEYCODE_R]           = LN_KEY_R;
            gKeyTable[AKEYCODE_S]           = LN_KEY_S;
            gKeyTable[AKEYCODE_T]           = LN_KEY_T;
            gKeyTable[AKEYCODE_U]           = LN_KEY_U;
            gKeyTable[AKEYCODE_V]           = LN_KEY_V;
            gKeyTable[AKEYCODE_W]           = LN_KEY_W;
            gKeyTable[AKEYCODE_X]           = LN_KEY_X;
            gKeyTable[AKEYCODE_Y]           = LN_KEY_Y;
            gKeyTable[AKEYCODE_Z]           = LN_KEY_Z;
            
            gKeyTable[AKEYCODE_COMMA]       = LN_KEY_COMMA;
            gKeyTable[AKEYCODE_PERIOD]      = LN_KEY_PERIOD;
            gKeyTable[AKEYCODE_ALT_LEFT]    = LN_KEY_LALT;
            gKeyTable[AKEYCODE_ALT_RIGHT]   = LN_KEY_RALT;
            gKeyTable[AKEYCODE_SHIFT_LEFT]  = LN_KEY_LSHIFT;
            gKeyTable[AKEYCODE_SHIFT_RIGHT] = LN_KEY_RSHIFT;
            gKeyTable[AKEYCODE_TAB]         = LN_KEY_TAB;
            gKeyTable[AKEYCODE_SPACE]       = LN_KEY_SPACE;
            gKeyTable[AKEYCODE_SYM]         = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_EXPLORER]    = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_ENVELOPE]    = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_ENTER]       = LN_KEY_ENTER;
            gKeyTable[AKEYCODE_DEL]         = LN_KEY_BACKSPACE;
            gKeyTable[AKEYCODE_GRAVE]       = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_MINUS]       = LN_KEY_MINUS;
            gKeyTable[AKEYCODE_EQUALS]      = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_LEFT_BRACKET]    = '[';
            gKeyTable[AKEYCODE_RIGHT_BRACKET]   = ']';
            gKeyTable[AKEYCODE_BACKSLASH]       = '\\';
            gKeyTable[AKEYCODE_SEMICOLON]       = LN_KEY_SEMICOLON;
            gKeyTable[AKEYCODE_APOSTROPHE]      = '\'';
            gKeyTable[AKEYCODE_SLASH]           = LN_KEY_SLASH;
            gKeyTable[AKEYCODE_AT]              = '@';
            gKeyTable[AKEYCODE_NUM]             = '\\';
            gKeyTable[AKEYCODE_HEADSETHOOK]     = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_FOCUS]           = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_PLUS]            = LN_KEY_ANDROID_PLUS;
            gKeyTable[AKEYCODE_MENU]            = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_NOTIFICATION]    = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_SEARCH]          = LN_KEY_UNKNOWN;
            
            gKeyTable[AKEYCODE_MEDIA_PLAY_PAUSE]    = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_MEDIA_STOP]          = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_MEDIA_NEXT]          = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_MEDIA_PREVIOUS]      = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_MEDIA_REWIND]        = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_MEDIA_FAST_FORWARD]  = LN_KEY_UNKNOWN;
            
            gKeyTable[AKEYCODE_MUTE]            = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_PAGE_UP]         = LN_KEY_PAGEUP;
            gKeyTable[AKEYCODE_PAGE_DOWN]       = LN_KEY_PAGEDOWN;
            gKeyTable[AKEYCODE_PICTSYMBOLS]     = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_SWITCH_CHARSET]  = LN_KEY_UNKNOWN;
            
            gKeyTable[AKEYCODE_BUTTON_A]        = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_BUTTON_B]        = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_BUTTON_C]        = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_BUTTON_X]        = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_BUTTON_Y]        = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_BUTTON_Z]        = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_BUTTON_L1]       = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_BUTTON_R1]       = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_BUTTON_L2]       = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_BUTTON_R2]       = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_BUTTON_THUMBL]   = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_BUTTON_THUMBR]   = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_BUTTON_START]    = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_BUTTON_SELECT]   = LN_KEY_UNKNOWN;
            gKeyTable[AKEYCODE_BUTTON_MODE]     = LN_KEY_UNKNOWN;
            
            gKeyTableInit = true;
        }
        
        return gKeyTable[key_];
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace System
} // namespace Core

} // namespace LNote

#endif // LNOTE_ANDROID
//=============================================================================
//								end of file
//=============================================================================