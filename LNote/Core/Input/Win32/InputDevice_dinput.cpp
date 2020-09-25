//=============================================================================
//【 InputDevice 】
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <wbemidl.h>
#include <oleauto.h>
#include "JoypadDevice.h"
#include "InputDevice.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Input
{
namespace Win32
{

//-----------------------------------------------------------------------------
// DirectInput

// マウスの GUID
GUID GUIDDIMOUSE2[3] =
{
	0xa36d02e0, 0xc9f3, 0x11cf, 0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00, 
	0xa36d02e1, 0xc9f3, 0x11cf, 0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00, 
	0xa36d02e2, 0xc9f3, 0x11cf, 0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00, 
} ;

// マウスのデバイスオブジェクトデータ
DIOBJECTDATAFORMAT C_ODFDIMOUSE2[11] =
{
	&GUIDDIMOUSE2[0],	0x00, 0x00ffff03, 0x000,
	&GUIDDIMOUSE2[1],	0x04, 0x00ffff03, 0x000,
	&GUIDDIMOUSE2[2],	0x08, 0x80ffff03, 0x000,
	NULL,				0x0c, 0x00ffff0c, 0x000,
	NULL,				0x0d, 0x00ffff0c, 0x000,
	NULL,				0x0e, 0x80ffff0c, 0x000,
	NULL, 				0x0f, 0x80ffff0c, 0x000,
	NULL, 				0x10, 0x80ffff0c, 0x000,
	NULL, 				0x11, 0x80ffff0c, 0x000,
	NULL, 				0x12, 0x80ffff0c, 0x000,
	NULL, 				0x13, 0x80ffff0c, 0x000,
} ;

// マウスのデバイスフォーマット
const DIDATAFORMAT c_dfDIMouse2 =
{
	24,		// dwSize
	16,		// dwObjSize
	2,		// dwFlags
	20,		// dwDataSize
	11,		// dwNumObjs
	C_ODFDIMOUSE2,
};

struct LOKeyboardData
{
    lnByte Key[ LN_MAX_KEYS ];
};

// キーボードのオブジェクトフォーマット (キーひとつ単位の設定項目。値の設定は initalize で)
static DIOBJECTDATAFORMAT gKeyboardObjectFormat[ LN_MAX_KEYS ];

// キーボードのデータフォーマット
static DIDATAFORMAT gKeyboardDataFormat =
{ 
	sizeof(DIDATAFORMAT),
	sizeof(DIOBJECTDATAFORMAT),
	DIDF_RELAXIS,
	sizeof(LOKeyboardData),
	LN_MAX_KEYS,
	gKeyboardObjectFormat,
};

// 仮想キーコード → DirectInput のキーコードに変換するテーブル
//static u32 gVirtualKeyTable[ LN_MAX_KEYS ] = { 0 };

// DirectInput のキーコード → 仮想キーコードに変換するテーブル
//static u32 gVirtualKeyTableDItoLN[ LN_MAX_KEYS ] = { 0 };

//=============================================================================
// ■ InputDevice クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    InputDevice::InputDevice()
        : mWindow				( NULL )
        , mWindowHandle			( NULL )
        , mDirectInput			( NULL )
        , mXInputModule			( NULL )
        , mMouseDevice			( NULL )
        , mJoypadNum			( 0 )
        , mXInputNo				( 0 )
        , mCoInited				( false )
		, mEnableKeyBuffering	( false )
    {
        memset( mKeyStatus, 0, sizeof( mKeyStatus ) );
        memset( &mMouseState, 0, sizeof( mMouseState ) );
        memset( mJoypads, 0, sizeof( mJoypads ) );
    }

	//---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    InputDevice::~InputDevice()
    {
        _release();

        if ( mCoInited )
        {
            ::CoUninitialize();
            mCoInited = false;
        }

        LN_SAFE_RELEASE( mWindow );
    }

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
	LNRESULT InputDevice::initialize( const InitData& init_data_ )
    {
        LN_THROW_InvalidCall(init_data_.Window);
        LN_THROW_InvalidCall(init_data_.Window->getWindowHandle());
        //LN_SETERR_R_S_( init_data_.Window->getWindowHandle(), LN_ERR_INVALIDCALL, "< InputDevice::initialize >\n無効なウィンドウが渡されました。" );

        LNRESULT lr;
        HRESULT  hr;

        mWindow        = init_data_.Window;
        mWindowHandle  = init_data_.Window->getWindowHandle();
        //mKeyBufferSize = 32;
        LN_SAFE_ADDREF( mWindow );

        //----------------------------------------------------------
	    // キーボードのデータフォーマットの初期化
	    for ( int i = 0; i < LN_MAX_KEYS; ++i )
	    {
		    gKeyboardDataFormat.rgodf[ i ].pguid	= &GUID_Key;
		    gKeyboardDataFormat.rgodf[ i ].dwOfs	= FIELD_OFFSET( LOKeyboardData, Key[ i ] );
		    gKeyboardDataFormat.rgodf[ i ].dwType	= 0x80000000 | DIDFT_BUTTON | DIDFT_ENUMCOLLECTION( i );
		    gKeyboardDataFormat.rgodf[ i ].dwFlags	= 0;
	    }

        //----------------------------------------------------------
	    // キーコード変換テーブルの初期化
#if 0
        gVirtualKeyTable[ LN_KEY_A ] = DIK_A;
        gVirtualKeyTable[ LN_KEY_B ] = DIK_B;
        gVirtualKeyTable[ LN_KEY_C ] = DIK_C;
        gVirtualKeyTable[ LN_KEY_D ] = DIK_D;
        gVirtualKeyTable[ LN_KEY_E ] = DIK_E;
        gVirtualKeyTable[ LN_KEY_F ] = DIK_F;
        gVirtualKeyTable[ LN_KEY_G ] = DIK_G;
        gVirtualKeyTable[ LN_KEY_H ] = DIK_H;
        gVirtualKeyTable[ LN_KEY_I ] = DIK_I;
        gVirtualKeyTable[ LN_KEY_J ] = DIK_J;
        gVirtualKeyTable[ LN_KEY_K ] = DIK_K;
        gVirtualKeyTable[ LN_KEY_L ] = DIK_L;
        gVirtualKeyTable[ LN_KEY_M ] = DIK_M;
        gVirtualKeyTable[ LN_KEY_N ] = DIK_N;
        gVirtualKeyTable[ LN_KEY_O ] = DIK_O;
        gVirtualKeyTable[ LN_KEY_P ] = DIK_P;
        gVirtualKeyTable[ LN_KEY_Q ] = DIK_Q;
        gVirtualKeyTable[ LN_KEY_R ] = DIK_R;
        gVirtualKeyTable[ LN_KEY_S ] = DIK_S;
        gVirtualKeyTable[ LN_KEY_T ] = DIK_T;
        gVirtualKeyTable[ LN_KEY_U ] = DIK_U;
        gVirtualKeyTable[ LN_KEY_V ] = DIK_V;
        gVirtualKeyTable[ LN_KEY_W ] = DIK_W;
        gVirtualKeyTable[ LN_KEY_X ] = DIK_X;
        gVirtualKeyTable[ LN_KEY_Y ] = DIK_Y;
        gVirtualKeyTable[ LN_KEY_Z ] = DIK_Z;

        gVirtualKeyTable[ LN_KEY_SPACE ] = DIK_SPACE;
        gVirtualKeyTable[ LN_KEY_ESCAPE ] = DIK_ESCAPE;
        gVirtualKeyTable[ LN_KEY_F1 ] = DIK_F1;
        gVirtualKeyTable[ LN_KEY_F2 ] = DIK_F2;
        gVirtualKeyTable[ LN_KEY_F3 ] = DIK_F3;
        gVirtualKeyTable[ LN_KEY_F4 ] = DIK_F4;
        gVirtualKeyTable[ LN_KEY_F5 ] = DIK_F5;
        gVirtualKeyTable[ LN_KEY_F6 ] = DIK_F6;
        gVirtualKeyTable[ LN_KEY_F7 ] = DIK_F7;
        gVirtualKeyTable[ LN_KEY_F8 ] = DIK_F8;
        gVirtualKeyTable[ LN_KEY_F9 ] = DIK_F9;
        gVirtualKeyTable[ LN_KEY_F10 ] = DIK_F10;
        gVirtualKeyTable[ LN_KEY_F11 ] = DIK_F11;
        gVirtualKeyTable[ LN_KEY_F12 ] = DIK_F12;
        gVirtualKeyTable[ LN_KEY_UP ] = DIK_UP;
        gVirtualKeyTable[ LN_KEY_DOWN ] = DIK_DOWN;
        gVirtualKeyTable[ LN_KEY_LEFT ] = DIK_LEFT;
        gVirtualKeyTable[ LN_KEY_RIGHT ] = DIK_RIGHT;
        gVirtualKeyTable[ LN_KEY_LSHIFT ] = DIK_LSHIFT;
        gVirtualKeyTable[ LN_KEY_RSHIFT ] = DIK_RSHIFT;
        gVirtualKeyTable[ LN_KEY_LCTRL ] = DIK_LCONTROL;
        gVirtualKeyTable[ LN_KEY_RCTRL ] = DIK_RCONTROL;
        gVirtualKeyTable[ LN_KEY_LALT ] = DIK_LMENU;
        gVirtualKeyTable[ LN_KEY_RALT ] = DIK_RMENU;
        gVirtualKeyTable[ LN_KEY_TAB ] = DIK_TAB;
        gVirtualKeyTable[ LN_KEY_ENTER ] = DIK_RETURN;
        gVirtualKeyTable[ LN_KEY_BACKSPACE ] = DIK_BACK;
        gVirtualKeyTable[ LN_KEY_INSERT ] = DIK_INSERT;
        gVirtualKeyTable[ LN_KEY_DELETE ] = DIK_DELETE;
        gVirtualKeyTable[ LN_KEY_PAGEUP ] = DIK_PRIOR;
        gVirtualKeyTable[ LN_KEY_PAGEDOWN ] = DIK_NEXT;
        gVirtualKeyTable[ LN_KEY_HOME ] = DIK_HOME;
        gVirtualKeyTable[ LN_KEY_END ] = DIK_END;
        gVirtualKeyTable[ LN_KEY_NUMPAD0 ] = DIK_0; // DIK_NUMPAD0
        gVirtualKeyTable[ LN_KEY_NUMPAD1 ] = DIK_1;
        gVirtualKeyTable[ LN_KEY_NUMPAD2 ] = DIK_2;
        gVirtualKeyTable[ LN_KEY_NUMPAD3 ] = DIK_2;
        gVirtualKeyTable[ LN_KEY_NUMPAD4 ] = DIK_4;
        gVirtualKeyTable[ LN_KEY_NUMPAD5 ] = DIK_5;
        gVirtualKeyTable[ LN_KEY_NUMPAD6 ] = DIK_6;
        gVirtualKeyTable[ LN_KEY_NUMPAD7 ] = DIK_7;
        gVirtualKeyTable[ LN_KEY_NUMPAD8 ] = DIK_8;
        gVirtualKeyTable[ LN_KEY_NUMPAD9 ] = DIK_9;

        gVirtualKeyTable[ LN_KEY_ADD ] = DIK_ADD;
        gVirtualKeyTable[ LN_KEY_SUBTRACT ] = DIK_SUBTRACT;
        gVirtualKeyTable[ LN_KEY_MULTIPLY ] = DIK_MULTIPLY;
        gVirtualKeyTable[ LN_KEY_DIVIDE ] = DIK_DIVIDE;
        gVirtualKeyTable[ LN_KEY_DECIMAL ] = DIK_DECIMAL;

        gVirtualKeyTable[ LN_KEY_COLON ] = DIK_COLON;
        gVirtualKeyTable[ LN_KEY_SEMICOLON ] = DIK_SEMICOLON;
        gVirtualKeyTable[ LN_KEY_COMMA ] = DIK_COMMA;
        gVirtualKeyTable[ LN_KEY_PERIOD ] = DIK_PERIOD;

        gVirtualKeyTable[ LN_KEY_NUM_LOCK ] = DIK_NUMLOCK;
        gVirtualKeyTable[ LN_KEY_CAPS_LOCK ] = DIK_CAPITAL;
        //gVirtualKeyTable[ LN_KEY_SCROLL_LOCK ] = DIK_SCROLL;
        //gVirtualKeyTable[ LN_KEY_PAUSE ] = DIK_PAUSE;

        gVirtualKeyTable[ LN_KEY_SHIFT ] = DIK_LSHIFT;      //
        gVirtualKeyTable[ LN_KEY_CONTROL ] = DIK_LCONTROL;  //
        gVirtualKeyTable[ LN_KEY_ALT ] = DIK_LALT;          //


        gVirtualKeyTableDItoLN[ DIK_ESCAPE ] = LN_KEY_ESCAPE;
        gVirtualKeyTableDItoLN[ DIK_1 ] = LN_KEY_NUMPAD1;
        gVirtualKeyTableDItoLN[ DIK_2 ] = LN_KEY_NUMPAD2;
        gVirtualKeyTableDItoLN[ DIK_3 ] = LN_KEY_NUMPAD3;
        gVirtualKeyTableDItoLN[ DIK_4 ] = LN_KEY_NUMPAD4;
        gVirtualKeyTableDItoLN[ DIK_5 ] = LN_KEY_NUMPAD5;
        gVirtualKeyTableDItoLN[ DIK_6 ] = LN_KEY_NUMPAD6;
        gVirtualKeyTableDItoLN[ DIK_7 ] = LN_KEY_NUMPAD7;
        gVirtualKeyTableDItoLN[ DIK_8 ] = LN_KEY_NUMPAD8;
        gVirtualKeyTableDItoLN[ DIK_9 ] = LN_KEY_NUMPAD9;
        gVirtualKeyTableDItoLN[ DIK_0 ] = LN_KEY_NUMPAD0;
        gVirtualKeyTableDItoLN[ DIK_MINUS ] = LN_KEY_SUBTRACT;    // ◆
        //gVirtualKeyTableDItoLN[ DIK_EQUALS ] = LN_KEY_EQUALS;
        gVirtualKeyTableDItoLN[ DIK_BACK ] = LN_KEY_BACKSPACE;
        gVirtualKeyTableDItoLN[ DIK_TAB ] = LN_KEY_TAB;
        gVirtualKeyTableDItoLN[ DIK_Q ] = LN_KEY_Q;
        gVirtualKeyTableDItoLN[ DIK_W ] = LN_KEY_W;
        gVirtualKeyTableDItoLN[ DIK_E ] = LN_KEY_E;
        gVirtualKeyTableDItoLN[ DIK_R ] = LN_KEY_R;
        gVirtualKeyTableDItoLN[ DIK_T ] = LN_KEY_T;
        gVirtualKeyTableDItoLN[ DIK_Y ] = LN_KEY_Y;
        gVirtualKeyTableDItoLN[ DIK_U ] = LN_KEY_U;
        gVirtualKeyTableDItoLN[ DIK_I ] = LN_KEY_I;
        gVirtualKeyTableDItoLN[ DIK_O ] = LN_KEY_O;
        gVirtualKeyTableDItoLN[ DIK_P ] = LN_KEY_P;
        gVirtualKeyTableDItoLN[ DIK_LBRACKET ] = '[';
        gVirtualKeyTableDItoLN[ DIK_RBRACKET ] = ']';
        gVirtualKeyTableDItoLN[ DIK_RETURN ] = LN_KEY_ENTER;    /* Enter on main keyboard */
        gVirtualKeyTableDItoLN[ DIK_LCONTROL ] = LN_KEY_CONTROL;    // ■
        gVirtualKeyTableDItoLN[ DIK_A ] = LN_KEY_A;
        gVirtualKeyTableDItoLN[ DIK_S ] = LN_KEY_S;
        gVirtualKeyTableDItoLN[ DIK_D ] = LN_KEY_D;
        gVirtualKeyTableDItoLN[ DIK_F ] = LN_KEY_F;
        gVirtualKeyTableDItoLN[ DIK_G ] = LN_KEY_G;
        gVirtualKeyTableDItoLN[ DIK_H ] = LN_KEY_H;
        gVirtualKeyTableDItoLN[ DIK_J ] = LN_KEY_J;
        gVirtualKeyTableDItoLN[ DIK_K ] = LN_KEY_K;
        gVirtualKeyTableDItoLN[ DIK_L ] = LN_KEY_L;
        gVirtualKeyTableDItoLN[ DIK_SEMICOLON ] = LN_KEY_SEMICOLON;
        //gVirtualKeyTableDItoLN[ DIK_APOSTROPHE ] = 
        gVirtualKeyTableDItoLN[ DIK_GRAVE ] = '`';
        gVirtualKeyTableDItoLN[ DIK_LSHIFT ] = LN_KEY_SHIFT;    // ■
        //gVirtualKeyTableDItoLN[ DIK_BACKSLASH ] = LN_KEY_BACKSLASH;
        gVirtualKeyTableDItoLN[ DIK_Z ] = LN_KEY_Z;
        gVirtualKeyTableDItoLN[ DIK_X ] = LN_KEY_X;
        gVirtualKeyTableDItoLN[ DIK_C ] = LN_KEY_C;
        gVirtualKeyTableDItoLN[ DIK_V ] = LN_KEY_V;
        gVirtualKeyTableDItoLN[ DIK_B ] = LN_KEY_B;
        gVirtualKeyTableDItoLN[ DIK_N ] = LN_KEY_N;
        gVirtualKeyTableDItoLN[ DIK_M ] = LN_KEY_M;
        gVirtualKeyTableDItoLN[ DIK_COMMA ] = LN_KEY_COMMA;
        gVirtualKeyTableDItoLN[ DIK_PERIOD ] = LN_KEY_PERIOD;
        gVirtualKeyTableDItoLN[ DIK_SLASH ] = LN_KEY_DIVIDE;    // ◆ /* / on main keyboard */
        gVirtualKeyTableDItoLN[ DIK_RSHIFT ] = LN_KEY_SHIFT;    // ■
        gVirtualKeyTableDItoLN[ DIK_MULTIPLY ] = LN_KEY_MULTIPLY;    /* * on numeric keypad */
        gVirtualKeyTableDItoLN[ DIK_LMENU ] = LN_KEY_ALT;    // ■   /* left Alt */
        gVirtualKeyTableDItoLN[ DIK_SPACE ] = LN_KEY_SPACE;
        gVirtualKeyTableDItoLN[ DIK_CAPITAL ] = LN_KEY_CAPS_LOCK;
        gVirtualKeyTableDItoLN[ DIK_F1 ] = LN_KEY_F1;
        gVirtualKeyTableDItoLN[ DIK_F2 ] = LN_KEY_F2;
        gVirtualKeyTableDItoLN[ DIK_F3 ] = LN_KEY_F3;
        gVirtualKeyTableDItoLN[ DIK_F4 ] = LN_KEY_F4;
        gVirtualKeyTableDItoLN[ DIK_F5 ] = LN_KEY_F5;
        gVirtualKeyTableDItoLN[ DIK_F6 ] = LN_KEY_F6;
        gVirtualKeyTableDItoLN[ DIK_F7 ] = LN_KEY_F7;
        gVirtualKeyTableDItoLN[ DIK_F8 ] = LN_KEY_F8;
        gVirtualKeyTableDItoLN[ DIK_F9 ] = LN_KEY_F9;
        gVirtualKeyTableDItoLN[ DIK_F10 ] = LN_KEY_F10;

        //gVirtualKeyTableDItoLN[ DIK_NUMLOCK         0x45
        //gVirtualKeyTableDItoLN[ DIK_SCROLL          0x46    /* Scroll Lock */
        gVirtualKeyTableDItoLN[ DIK_NUMPAD7 ] = LN_KEY_NUMPAD7;
        gVirtualKeyTableDItoLN[ DIK_NUMPAD8 ] = LN_KEY_NUMPAD8;
        gVirtualKeyTableDItoLN[ DIK_NUMPAD9 ] = LN_KEY_NUMPAD9;
        gVirtualKeyTableDItoLN[ DIK_SUBTRACT ] = LN_KEY_SUBTRACT;    /* - on numeric keypad */
        gVirtualKeyTableDItoLN[ DIK_NUMPAD4 ] = LN_KEY_NUMPAD4;
        gVirtualKeyTableDItoLN[ DIK_NUMPAD5 ] = LN_KEY_NUMPAD5;
        gVirtualKeyTableDItoLN[ DIK_NUMPAD6 ] = LN_KEY_NUMPAD6;
        gVirtualKeyTableDItoLN[ DIK_ADD ] = LN_KEY_ADD;    /* + on numeric keypad */
        gVirtualKeyTableDItoLN[ DIK_NUMPAD1 ] = LN_KEY_NUMPAD1;
        gVirtualKeyTableDItoLN[ DIK_NUMPAD2 ] = LN_KEY_NUMPAD2;
        gVirtualKeyTableDItoLN[ DIK_NUMPAD3 ] = LN_KEY_NUMPAD3;
        gVirtualKeyTableDItoLN[ DIK_NUMPAD0 ] = LN_KEY_NUMPAD0;
        gVirtualKeyTableDItoLN[ DIK_DECIMAL ] = LN_KEY_DECIMAL;    /* . on numeric keypad */
        //gVirtualKeyTableDItoLN[ DIK_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
        gVirtualKeyTableDItoLN[ DIK_F11 ] = LN_KEY_F11;
        gVirtualKeyTableDItoLN[ DIK_F12 ] = LN_KEY_F12;
        //gVirtualKeyTableDItoLN[ DIK_F13 ] = LN_KEY_F13;
        //gVirtualKeyTableDItoLN[ DIK_F14 ] = LN_KEY_F14;
        //gVirtualKeyTableDItoLN[ DIK_F15 ] = LN_KEY_F15;
        #if 0
        gVirtualKeyTableDItoLN[ DIK_KANA            0x70    /* (Japanese keyboard)            */
        gVirtualKeyTableDItoLN[ DIK_ABNT_C1         0x73    /* /? on Brazilian keyboard */
        gVirtualKeyTableDItoLN[ DIK_CONVERT         0x79    /* (Japanese keyboard)            */
        gVirtualKeyTableDItoLN[ DIK_NOCONVERT       0x7B    /* (Japanese keyboard)            */
        gVirtualKeyTableDItoLN[ DIK_YEN             0x7D    /* (Japanese keyboard)            */
        gVirtualKeyTableDItoLN[ DIK_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
        gVirtualKeyTableDItoLN[ DIK_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
        gVirtualKeyTableDItoLN[ DIK_PREVTRACK       0x90    /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
        gVirtualKeyTableDItoLN[ DIK_AT              0x91    /*                     (NEC PC98) */
        #endif
        gVirtualKeyTableDItoLN[ DIK_COLON ] = LN_KEY_COLON;
        #if 0
        gVirtualKeyTableDItoLN[ DIK_UNDERLINE       0x93    /*                     (NEC PC98) */
        gVirtualKeyTableDItoLN[ DIK_KANJI           0x94    /* (Japanese keyboard)            */
        gVirtualKeyTableDItoLN[ DIK_STOP            0x95    /*                     (NEC PC98) */
        gVirtualKeyTableDItoLN[ DIK_AX              0x96    /*                     (Japan AX) */
        gVirtualKeyTableDItoLN[ DIK_UNLABELED       0x97    /*                        (J3100) */
        gVirtualKeyTableDItoLN[ DIK_NEXTTRACK       0x99    /* Next Track */
        gVirtualKeyTableDItoLN[ DIK_NUMPADENTER     0x9C    /* Enter on numeric keypad */
        #endif
        gVirtualKeyTableDItoLN[ DIK_RCONTROL ] = LN_KEY_CONTROL;  // ■
        #if 0
        gVirtualKeyTableDItoLN[ DIK_MUTE            0xA0    /* Mute */
        gVirtualKeyTableDItoLN[ DIK_CALCULATOR      0xA1    /* Calculator */
        gVirtualKeyTableDItoLN[ DIK_PLAYPAUSE       0xA2    /* Play / Pause */
        gVirtualKeyTableDItoLN[ DIK_MEDIASTOP       0xA4    /* Media Stop */
        gVirtualKeyTableDItoLN[ DIK_VOLUMEDOWN      0xAE    /* Volume - */
        gVirtualKeyTableDItoLN[ DIK_VOLUMEUP        0xB0    /* Volume + */
        gVirtualKeyTableDItoLN[ DIK_WEBHOME         0xB2    /* Web home */
        #endif
        gVirtualKeyTableDItoLN[ DIK_NUMPADCOMMA ] = LN_KEY_COMMA;
        gVirtualKeyTableDItoLN[ DIK_DIVIDE ] = LN_KEY_DIVIDE;
        #if 0
        gVirtualKeyTableDItoLN[ DIK_SYSRQ           0xB7
        gVirtualKeyTableDItoLN[ DIK_RMENU           0xB8    /* right Alt */
        gVirtualKeyTableDItoLN[ DIK_PAUSE           0xC5    /* Pause */
        gVirtualKeyTableDItoLN[ DIK_HOME            0xC7    /* Home on arrow keypad */
        #endif
        gVirtualKeyTableDItoLN[ DIK_UP ] = LN_KEY_UP;
        //gVirtualKeyTableDItoLN[ DIK_PRIOR           0xC9    /* PgUp on arrow keypad */
        gVirtualKeyTableDItoLN[ DIK_LEFT ] = LN_KEY_LEFT;
        gVirtualKeyTableDItoLN[ DIK_RIGHT ] = LN_KEY_RIGHT;
        //gVirtualKeyTableDItoLN[ DIK_END             0xCF    /* End on arrow keypad */
        gVirtualKeyTableDItoLN[ DIK_DOWN ] = LN_KEY_DOWN;
        //gVirtualKeyTableDItoLN[ DIK_NEXT            0xD1    /* PgDn on arrow keypad */
        gVirtualKeyTableDItoLN[ DIK_INSERT ] = LN_KEY_INSERT;
        gVirtualKeyTableDItoLN[ DIK_DELETE ] = LN_KEY_DELETE;
        #if 0
        gVirtualKeyTableDItoLN[ DIK_LWIN            0xDB    /* Left Windows key */
        gVirtualKeyTableDItoLN[ DIK_RWIN            0xDC    /* Right Windows key */
        gVirtualKeyTableDItoLN[ DIK_APPS            0xDD    /* AppMenu key */
        gVirtualKeyTableDItoLN[ DIK_POWER           0xDE    /* System Power */
        gVirtualKeyTableDItoLN[ DIK_SLEEP           0xDF    /* System Sleep */
        gVirtualKeyTableDItoLN[ DIK_WAKE            0xE3    /* System Wake */
        gVirtualKeyTableDItoLN[ DIK_WEBSEARCH       0xE5    /* Web Search */
        gVirtualKeyTableDItoLN[ DIK_WEBFAVORITES    0xE6    /* Web Favorites */
        gVirtualKeyTableDItoLN[ DIK_WEBREFRESH      0xE7    /* Web Refresh */
        gVirtualKeyTableDItoLN[ DIK_WEBSTOP         0xE8    /* Web Stop */
        gVirtualKeyTableDItoLN[ DIK_WEBFORWARD      0xE9    /* Web Forward */
        gVirtualKeyTableDItoLN[ DIK_WEBBACK         0xEA    /* Web Back */
        gVirtualKeyTableDItoLN[ DIK_MYCOMPUTER      0xEB    /* My Computer */
        gVirtualKeyTableDItoLN[ DIK_MAIL            0xEC    /* Mail */
        gVirtualKeyTableDItoLN[ DIK_MEDIASELECT     0xED    /* Media Select */
        #endif
#endif
        
        //----------------------------------------------------------
	    // COM 初期化
        if ( SUCCEEDED( ::CoInitializeEx( NULL, COINIT_MULTITHREADED ) ) )
        {
            mCoInited = true;
        }

        //----------------------------------------------------------
	    // XInput利用する
        //XInputEnable( true );
        mXInputModule = LN_NEW XInputModule();
        LN_CALL_R( mXInputModule->initialize() );

        //----------------------------------------------------------
	    // DirectInput

        LN_COMCALL(
            CoCreateInstance(
				CLSID_DirectInput8,
				NULL, 
				CLSCTX_ALL,
				IID_IDirectInput8,
				(void**)&mDirectInput ) );

        // DirectInput の初期化
        LN_COMCALL( mDirectInput->Initialize( ::GetModuleHandle( NULL ), DIRECTINPUT_VERSION ) );

        // マウスデバイスの初期化
        LN_CALL_R( _initMouse() );

        refresh();
        
        return update();
    }

    //---------------------------------------------------------------------
	// ● キーボードの状態の取得
    //---------------------------------------------------------------------
    bool InputDevice::getKeyState( LNVirtualKeyCode vkey_ )
    {
        return mKeyStatus[ vkey_ ];
    }

    //---------------------------------------------------------------------
	// ● キーボードのバッファリングデータの取得
    //---------------------------------------------------------------------
    LNVirtualKeyCode InputDevice::getKeyBufferState()
    {
        if ( !mKeyBuffer.empty() )
        {
            LNVirtualKeyCode key = mKeyBuffer.front();
            mKeyBuffer.pop_front();
            return key;
        }
        /*
        u32 key = 0;

	    if ( mKeyBufferCount < mKeyBufferMax )
	    {
            do {
                if ( mKeyBuffer[ mKeyBufferCount ].dwData & 0x80 )
		        {
			        key = mKeyBuffer[ mKeyBufferCount ].dwOfs;
		        }
                mKeyBufferCount++;

            } while ( key == 0 && mKeyBufferCount < mKeyBufferMax );
 
	    }

	    return gVirtualKeyTableDItoLN[ key ];
        */
        return LN_KEY_UNKNOWN;
    }

    //---------------------------------------------------------------------
	// ● ジョイパッドの状態の取得
    //---------------------------------------------------------------------
    void InputDevice::getJoypadState( int joypad_id_, LNJoypadDeviceState* state_ )
    {
        mJoypads[ joypad_id_ ]->getAxisState( state_->Axes );
        mJoypads[ joypad_id_ ]->getButtonState( state_->Buttons );
        state_->POV = mJoypads[ joypad_id_ ]->getPOVState();
    }

    //---------------------------------------------------------------------
	// ● ジョイパッドの振動を開始する
    //---------------------------------------------------------------------
    void InputDevice::startVibration( int joypad_id_, int power_, int time_ )
    {
        if ( mWindow->isActive() )
        {
            if ( 0 <= joypad_id_ && joypad_id_ < mJoypadNum )
            {
                mJoypads[ joypad_id_ ]->startVibration( power_, time_ );
            }
        }
    }

    //---------------------------------------------------------------------
	// ● ジョイパッドの振動を停止する
    //---------------------------------------------------------------------
    void InputDevice::stopVibration( int joypad_id_ )
    {
        if ( 0 <= joypad_id_ && joypad_id_ < mJoypadNum )
        {
            mJoypads[ joypad_id_ ]->stopVibration();
        }
    }

    //---------------------------------------------------------------------
	// ● 仮想ボタンの状態の取得
    //---------------------------------------------------------------------
    float InputDevice::getVirtualButtonState( u32 button_, bool keyboard_, bool mouse_, int joypad_id_ )
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
        if ( joypad_id_ >= 0 && joypad_id_ < mJoypadNum )
        {
            // ボタン
            if ( LN_VBUTTON_JOYPAD_BUTTON_BEGIN <= button_ && button_ <= LN_VBUTTON_JOYPAD_BUTTON_END )
            {
                return ( mJoypads[ joypad_id_ ]->getButtonState( button_ - LN_VBUTTON_JOYPAD_BUTTON_BEGIN ) ) ? 1.0f : 0.0f;
            }

            // POV
            if ( LN_VBUTTON_JOYPAD_POV_BEGIN <= button_ && button_ <= LN_VBUTTON_JOYPAD_POV_END )
            {
                if ( button_ == LN_VBUTTON_JOYPAD_POV_UP )    { return ( mJoypads[ joypad_id_ ]->getPOVState() & POV_UP )    ? 1.0f : 0.0f; }
                if ( button_ == LN_VBUTTON_JOYPAD_POV_RIGHT ) { return ( mJoypads[ joypad_id_ ]->getPOVState() & POV_RIGHT ) ? 1.0f : 0.0f; }
                if ( button_ == LN_VBUTTON_JOYPAD_POV_DOWN )  { return ( mJoypads[ joypad_id_ ]->getPOVState() & POV_DOWN )  ? 1.0f : 0.0f; }
                if ( button_ == LN_VBUTTON_JOYPAD_POV_LEFT )  { return ( mJoypads[ joypad_id_ ]->getPOVState() & POV_LEFT )  ? 1.0f : 0.0f; }
            }

            // 軸
            if ( LN_VBUTTON_JOYPAD_AXIS_BEGIN <= button_ && button_ <= LN_VBUTTON_JOYPAD_AXIS_END )
            {
                u32 axis_id = ( button_ - LN_VBUTTON_JOYPAD_AXIS_BEGIN ) / 2;
                lnfloat v = mJoypads[ joypad_id_ ]->getAxisState( axis_id );


                if ( ( button_ - LN_VBUTTON_JOYPAD_AXIS_BEGIN ) & 1 )  // 奇数はプラスかを判定
                {
                    return v;
                }
                else    
                {
                    return -v;
                }
            }
        }

        return 0.0f;
    }

    //---------------------------------------------------------------------
	// ● マウス座標の設定
    //---------------------------------------------------------------------
    void InputDevice::setMousePoint( const LVector2& point_ )
    {
        if ( mWindow->isActive() )
        {
            POINT pt;
		    pt.x = static_cast< LONG >( point_.x );
		    pt.y = static_cast< LONG >( point_.y );
		    ::ClientToScreen( mWindowHandle, &pt );
		    ::SetCursorPos( pt.x, pt.y );
        }
    }

    //---------------------------------------------------------------------
	// ● マウスカーソルの位置をクライアント領域の中央に設定する
    //---------------------------------------------------------------------
    void InputDevice::setMouseCursorCenter()
    {
        if ( mWindow->isActive() )
        {
            const Geometry::Size& size = mWindow->getSize();
            
                setMousePoint(
                    LVector2(
                        static_cast< lnfloat >( size.w / 2 ),
                        static_cast< lnfloat >( size.h / 2 ) ) );
            
        }
    }

    //---------------------------------------------------------------------
	// ● 入力情報の更新
    //---------------------------------------------------------------------
    LNRESULT InputDevice::update()
    {
        HRESULT hr;

        //-----------------------------------------------------
        // マウス

        DIMOUSESTATE2 dims;
        hr = mMouseDevice->GetDeviceState( sizeof( DIMOUSESTATE2 ), &dims );

        if ( SUCCEEDED( hr ) )
        {
            POINT pt;
		    ::GetCursorPos( &pt );
		    ::ScreenToClient( mWindowHandle, &pt );

		    mMouseState.X = pt.x;
		    mMouseState.Y = pt.y;
		    mMouseState.RelX = dims.lX;
		    mMouseState.RelY = dims.lY;
		    mMouseState.Wheel = dims.lZ / WHEEL_DELTA;
		    mMouseState.Succeeded = true;
		    for ( int i = 0; i < LN_MAX_MOUSE_BUTTONS; ++i )
		    {
			    mMouseState.Buttons[ i ] = ( dims.rgbButtons[ i ] & 0x80 ) != 0;
		    }
            /*
            printf( "X:%d Y:%d RX:%d RY:%d W:%d BT:%d %d %d %d\n", 
                        mMouseState.X,
				        mMouseState.Y,
				        mMouseState.RelX,
				        mMouseState.RelY,
				        mMouseState.Wheel,
                        mMouseState.Buttons[ 0 ],
                        mMouseState.Buttons[ 1 ],
                        mMouseState.Buttons[ 2 ],
                        mMouseState.Buttons[ 3 ] );
            */
        }
        else
        {
            mMouseDevice->Acquire();
            memset( &mMouseState, 0, sizeof( mMouseState ) );
        }

        //-----------------------------------------------------
        // ジョイパッド

        for ( int i = 0; i < mJoypadNum; ++i )
		{
			mJoypads[ i ]->update();
		}


        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● デバイスの再セットアップ
    //---------------------------------------------------------------------
    LNRESULT InputDevice::refresh()
    {
        HRESULT  hr;

        _releaseJoypads();


        // 接続済みのゲームコントローラーデバイスを列挙する
		LN_DXCALL_R(
            mDirectInput->EnumDevices(
			    //DI8DEVCLASS_GAMECTRL,
                DI8DEVTYPE_JOYSTICK,
                _enumJoysticksCallback,
			    this,
                DIEDFL_ATTACHEDONLY ) );

        return LN_OK;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    bool InputDevice::onEvent( const System::Event& args_ )
    {
        const System::KeyEvent& ev_key = (const System::KeyEvent&)(args_);

        switch (args_.Type)
        {
            case LNEV_KEY_DOWN:
                if ( !mKeyStatus[ev_key.KeyCode] )
                {
                    mKeyStatus[ev_key.KeyCode] = true;
					if (mEnableKeyBuffering)
					{
						mKeyBuffer.push_back((LNVirtualKeyCode)ev_key.KeyCode);
					}
                }
                break;

            case LNEV_KEY_UP:
                mKeyStatus[ev_key.KeyCode] = false;
                break;
        }
        return false;
    }

    //---------------------------------------------------------------------
	// ● 解放
    //---------------------------------------------------------------------
    void InputDevice::_release()
    {
        _releaseJoypads();

        SAFE_RELEASE( mMouseDevice );
        SAFE_RELEASE( mDirectInput );
        SAFE_DELETE( mXInputModule );
    }

    //---------------------------------------------------------------------
	// ● ジョイパッドの解放
    //---------------------------------------------------------------------
    void InputDevice::_releaseJoypads()
    {
        for ( int i = 0; i < LN_MAX_JOYPADS; ++i )
		{
			SAFE_DELETE( mJoypads[ i ] );
		}
		mJoypadNum = 0;
        mXInputNo = 0;
    }

    //---------------------------------------------------------------------
	// ● マウスデバイスの初期化
    //---------------------------------------------------------------------
    LNRESULT InputDevice::_initMouse()
    {
        HRESULT  hr;
        memset( &mMouseState, 0, sizeof( mMouseState ) );

        // マウスデバイス作成
		LN_COMCALL( mDirectInput->CreateDevice( GUID_SysMouse, &mMouseDevice, NULL ) );

		// データ形式を設定
		LN_COMCALL( mMouseDevice->SetDataFormat( &c_dfDIMouse2 ) );

		// フォアグラウンド & 非排他モードに設定
		LN_COMCALL( mMouseDevice->SetCooperativeLevel( mWindowHandle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ) );

		// 軸モードを設定（相対値モードに設定）
		DIPROPDWORD diprop;
		diprop.diph.dwSize			= sizeof( diprop ); 
		diprop.diph.dwHeaderSize	= sizeof( diprop.diph ); 
		diprop.diph.dwObj			= 0;
		diprop.diph.dwHow			= DIPH_DEVICE;
		diprop.dwData				= DIPROPAXISMODE_REL;
		//diprop.dwData				= DIPROPAXISMODE_ABS;	// 絶対値モードの場合(マウスの場合は無効っぽいけど…？)
		LN_COMCALL( mMouseDevice->SetProperty( DIPROP_AXISMODE, &diprop.diph ) );

        // 入力制御開始
	    mMouseDevice->Acquire();

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● ジョイスティックを列挙するコールバック関数
	//---------------------------------------------------------------------
	BOOL CALLBACK InputDevice::_enumJoysticksCallback( const DIDEVICEINSTANCE* instance_, VOID* this_ )
	{
		InputDevice* self = static_cast< InputDevice* >( this_ );
		self->_createJoysticksDevice( instance_ );
		return DIENUM_CONTINUE;
	}

	//---------------------------------------------------------------------
	// ● ジョイスティックのデバイスオブジェクトを作成する
	//---------------------------------------------------------------------
	void InputDevice::_createJoysticksDevice( const DIDEVICEINSTANCE* instance_ )
	{
		if ( mJoypadNum >= LN_MAX_JOYPADS ) { return; }

        HRESULT hr;


        if ( _isXInputDevice( &instance_->guidProduct ) )
        {
            // デバイスの情報を取得するため、一度デバイスを作る
            IDirectInputDevice8* device = NULL;
            hr = mDirectInput->CreateDevice( instance_->guidInstance, &device, NULL );
		    if SUCCEEDED( hr )
		    {
                DIDEVICEINSTANCE di = { 0 };

                device->GetDeviceInfo( &di );

                //printf( "XInput Device  No:%d Name:%s\n", mXInputNo, di.tszInstanceName );
            }

            SAFE_RELEASE( device );

            mJoypads[ mJoypadNum ] = LN_NEW JoypadDevice();
            mJoypads[ mJoypadNum ]->initialize( device, mWindowHandle, mXInputNo, mXInputModule, true );
		    ++mJoypadNum;
            ++mXInputNo;
        }
        else
        {


            IDirectInputDevice8* device = NULL;
            hr = mDirectInput->CreateDevice( instance_->guidInstance, &device, NULL );
            if ( SUCCEEDED( hr ) )
            {
                // デバイスの情報取得
                DIDEVCAPS caps;
                memset( &caps, 0, sizeof( caps ) );
		        caps.dwSize = sizeof( caps );
                if ( SUCCEEDED( device->GetCapabilities( &caps ) ) )
                {

                    //DXST_ERRORLOGFMT_ADDA(( "デバイスの登録名:%s" , State.tszInstanceName )) ;

		            //DXST_ERRORLOGFMT_ADDA(( "デバイスの製品登録名:%s" , State.tszProductName )) ;

                    mJoypads[ mJoypadNum ] = LN_NEW JoypadDevice();
                    mJoypads[ mJoypadNum ]->initialize( device, mWindowHandle, -1, NULL, ( caps.dwFlags & DIDC_FORCEFEEDBACK ) != 0 );
			        ++mJoypadNum;
                }
            }
        }

        /*

		// デバイスのタイプを取得する ( 下位バイトがタイプ )
		u32 type = instance_->dwDevType & 0xff;
		// ジョイスティックの場合
		// (DI8DEVTYPE_JOYSTICK だけだとヒットしないコントローラもあるので DI8DEVTYPE_GAMEPADもチェック)
		if ( type == DI8DEVTYPE_JOYSTICK || type == DI8DEVTYPE_GAMEPAD )
		{
			// ジョイスティックのデバイスオブジェクトを作成する
			IDirectInputDevice8* device = NULL;
			HRESULT hr = mDirectInput->CreateDevice( instance_->guidInstance, &device, NULL );
			if ( SUCCEEDED( hr ) )
			{
				// ジョイスティックの管理クラスを作成する
				mJoypads[ mJoypadNum ] = NEW JoypadDevice();
                mJoypads[ mJoypadNum ]->initialize( device, mWindowHandle );
				++mJoypadNum;
			}
		}
        */

	}

    //-----------------------------------------------------------------------------
	// Enum each PNP device using WMI and check each device ID to see if it contains 
	// "IG_" (ex. "VID_045E&PID_028E&IG_00").  If it does, then it's an XInput device
	// Unfortunately this information can not be found by just using DirectInput 
	//-----------------------------------------------------------------------------
	// http://msdn.microsoft.com/ja-jp/library/bb173051(v=vs.85).aspx
	BOOL InputDevice::_isXInputDevice( const GUID* pGuidProductFromDirectInput )
	{
		IWbemLocator*           pIWbemLocator  = NULL;
		IEnumWbemClassObject*   pEnumDevices   = NULL;
		IWbemClassObject*       pDevices[20]   = {0};
		IWbemServices*          pIWbemServices = NULL;
		BSTR                    bstrNamespace  = NULL;
		BSTR                    bstrDeviceID   = NULL;
		BSTR                    bstrClassName  = NULL;
		DWORD                   uReturned      = 0;
		bool                    bIsXinputDevice= false;
		UINT                    iDevice        = 0;
		VARIANT                 var;
		HRESULT                 hr;

		// CoInit if needed
		hr = CoInitialize(NULL);
		bool bCleanupCOM = SUCCEEDED(hr);

		// Create WMI
		hr = CoCreateInstance(
			__uuidof(WbemLocator),
			NULL,
			CLSCTX_INPROC_SERVER,
			__uuidof(IWbemLocator),
			(LPVOID*) &pIWbemLocator );
		if( FAILED(hr) || pIWbemLocator == NULL )
			goto LCleanup;

		bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" );if( bstrNamespace == NULL ) goto LCleanup;        
		bstrClassName = SysAllocString( L"Win32_PNPEntity" );   if( bstrClassName == NULL ) goto LCleanup;        
		bstrDeviceID  = SysAllocString( L"DeviceID" );          if( bstrDeviceID == NULL )  goto LCleanup;        
	    
		// Connect to WMI 
		hr = pIWbemLocator->ConnectServer( bstrNamespace, NULL, NULL, 0L, 
										   0L, NULL, NULL, &pIWbemServices );
		if( FAILED(hr) || pIWbemServices == NULL )
			goto LCleanup;

		// Switch security level to IMPERSONATE. 
		CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, 
						   RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE );                    

		hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, NULL, &pEnumDevices ); 
		if( FAILED(hr) || pEnumDevices == NULL )
			goto LCleanup;

		// Loop over all devices
		for( ;; )
		{
			// Get 20 at a time
			hr = pEnumDevices->Next( 10000, 20, pDevices, &uReturned );
			if( FAILED(hr) )
				goto LCleanup;
			if( uReturned == 0 )
				break;

			for( iDevice=0; iDevice<uReturned; iDevice++ )
			{
				// For each device, get its device ID
				hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, NULL, NULL );
				if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != NULL )
				{
					// Check if the device ID contains "IG_".  If it does, then it's an XInput device
						// This information can not be found from DirectInput 
					if( wcsstr( var.bstrVal, L"IG_" ) )
					{
						// If it does, then get the VID/PID from var.bstrVal
						DWORD dwPid = 0, dwVid = 0;
						WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
						if( strVid && swscanf( strVid, L"VID_%4X", &dwVid ) != 1 )
							dwVid = 0;
						WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
						if( strPid && swscanf( strPid, L"PID_%4X", &dwPid ) != 1 )
							dwPid = 0;

						// Compare the VID/PID to the DInput device
						DWORD dwVidPid = MAKELONG( dwVid, dwPid );
						if( dwVidPid == pGuidProductFromDirectInput->Data1 )
						{
							bIsXinputDevice = true;
							goto LCleanup;
						}
					}
				}   
				SAFE_RELEASE( pDevices[iDevice] );
			}
		}

	LCleanup:
		if(bstrNamespace)
			SysFreeString(bstrNamespace);
		if(bstrDeviceID)
			SysFreeString(bstrDeviceID);
		if(bstrClassName)
			SysFreeString(bstrClassName);
		for( iDevice=0; iDevice<20; iDevice++ )
			SAFE_RELEASE( pDevices[iDevice] );
		SAFE_RELEASE( pEnumDevices );
		SAFE_RELEASE( pIWbemLocator );
		SAFE_RELEASE( pIWbemServices );

		if( bCleanupCOM )
			CoUninitialize();

		return bIsXinputDevice;
	}



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Win32
} // namespace Input
} // namespace Core

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================