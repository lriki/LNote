//=============================================================================
//【 Event 】
//-----------------------------------------------------------------------------
///**
//  @file       Event.h
//  @brief      Event
//  @author     Riki
//*/
//-----------------------------------------------------------------------------
/*
    以前の struct - union のスタイルだと、System 以外のモジュールからイベントを
    発生させたい場合に、LNEvent 構造体の拡張が困難になる。
    そのため、イベントの種類で引数を判別してキャストする方向にする。

    ハンドラ関数の引数は (int type, void* args) か (struct Event* event) か迷ったけど、
    C# .NET の WndProc が大体後者なので、こちらに合わせてみる。

    イベント構造体は new で動的確保されるが、基本的にユーザーは解放処理について
    気にする必要はない。 (Manager::processMessage() で、処理後に解放される)
    そのため、ハンドラ関数の引数は「delete の必要はないです」ということで
    ポインタではなく参照にする。


    注意点…
            Android でソフトウェアキーボードを使った場合、
            LNEV_KEY_DOWN と LNEV_KEY_UP はキーを離した瞬間に同じタイミングで発生してしまう。
*/
//=============================================================================

#pragma once

#define LNEV_NULL               0   ///< 未定義のイベント (通常は送信されない)

#define LNEV_QUIT               1
#define LNEV_CLOSE              2   ///< ウィンドウのクローズボタンが押された
#define LNEV_APP_ACTIVATE       3   ///< ウィンドウがアクティブになった
#define LNEV_APP_DEACTIVATE     4   ///< ウィンドウが非アクティブになった
#define LNEV_ALTENTER           5   ///< Alt + Enter

#define LNEV_MOUSE_DOWN         6   ///< [ LNEvent::Mouse ] マウスボタンが押された
#define LNEV_MOUSE_UP           7   ///< [ LNEvent::Mouse ] マウスボタンが離された
#define LNEV_MOUSE_MOVE         8   ///< [ LNEvent::Mouse ] マウスが移動した
#define LNEV_MOUSE_WHEEL        9   ///< [ LNEvent::Mouse ] マウスホイールが操作された
 
#define LNEV_KEY_DOWN           10  ///< [ LNEvent::Key ] キー押し下
#define LNEV_KEY_UP             11  ///< [ LNEvent::Key ] キー押し上げ
#define LNEV_KEY_CHAR           12  ///< [ LNEvent::Key ] 文字入力 (日本語入力 OFF の状態でキーが押され場合)
    
#define LNEV_TOUCH_DOWN         13  ///< [ LNEvent::Touch ] 画面がタッチされた
#define LNEV_TOUCH_UP           14  ///< [ LNEvent::Touch ] 画面から離された
#define LNEV_TOUCH_MOVE         15  ///< [ LNEvent::Touch ] タッチ中に移動した

#define LNEV_ANDROID_PAUSED     101 ///< Android APP_CMD_RESUME
#define LNEV_ANDROID_RESUMED    102 ///< Android APP_CMD_PAUSE

// Graphics
#define LNEV_DEVICE_LOST		201	///< デバイスリセット前処理
#define LNEV_DEVICE_RESET		202	///< デバイスリセット後処理

#define LNEV_MOUSE_BEGIN    LNEV_MOUSE_DOWN
#define LNEV_MOUSE_END      LNEV_MOUSE_WHEEL

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

/// 扱うことのできる画面タッチ情報の最大数 (認識できる指の数)
static const int LN_MAX_TOUCH_DATA = 2;

/// マウスボタン
enum LNSystemMouseButton
{
	LN_MOUSE_LEFT = 0,	        ///< 左ボタン   ( 1 番 )
	LN_MOUSE_RIGHT,		        ///< 右ボタン   ( 2 番 )
	LN_MOUSE_MIDDLE,	        ///< 中央ボタン ( 3 番 )

    LN_MOUSE_NONE = 0xff        ///< 無効または押されていないことを示すフラグ
};

/// キーボードの仮想キーコード (NumPad はサポートしていない)
enum LNVirtualKeyCode
{
	LN_KEY_UNKNOWN = 0,

    LN_KEY_A,
    LN_KEY_B,
    LN_KEY_C,
    LN_KEY_D,
    LN_KEY_E,
    LN_KEY_F,
    LN_KEY_G,
    LN_KEY_H,
    LN_KEY_I,
    LN_KEY_J,
    LN_KEY_K,
    LN_KEY_L,
    LN_KEY_M,
    LN_KEY_N,
    LN_KEY_O,
    LN_KEY_P,
    LN_KEY_Q,
    LN_KEY_R,
    LN_KEY_S,
    LN_KEY_T,
    LN_KEY_U,
    LN_KEY_V,
    LN_KEY_W,
    LN_KEY_X,
    LN_KEY_Y,
    LN_KEY_Z,

    LN_KEY_0,
    LN_KEY_1,
    LN_KEY_2,
    LN_KEY_3,
    LN_KEY_4,
    LN_KEY_5,
    LN_KEY_6,
    LN_KEY_7,
    LN_KEY_8,
    LN_KEY_9,

	LN_KEY_F1,
	LN_KEY_F2,
	LN_KEY_F3,
	LN_KEY_F4,
	LN_KEY_F5,
	LN_KEY_F6,
	LN_KEY_F7,
	LN_KEY_F8,
	LN_KEY_F9,
	LN_KEY_F10,
	LN_KEY_F11,
	LN_KEY_F12,

	LN_KEY_SPACE,	// 0x20 = ' ' (スペース)
	LN_KEY_ESCAPE,
	LN_KEY_UP,
	LN_KEY_DOWN,
	LN_KEY_LEFT,
	LN_KEY_RIGHT,
	LN_KEY_LSHIFT,
	LN_KEY_RSHIFT,
	LN_KEY_LCTRL,
	LN_KEY_RCTRL,
	LN_KEY_LALT,
	LN_KEY_RALT,
	LN_KEY_TAB,
	LN_KEY_ENTER,
	LN_KEY_BACKSPACE,
	LN_KEY_INSERT,
	LN_KEY_DELETE,
	LN_KEY_PAGEUP,
	LN_KEY_PAGEDOWN,
	LN_KEY_HOME,
	LN_KEY_END,

    LN_KEY_COLON,	    ///< :
    LN_KEY_SEMICOLON,   ///< ;
    LN_KEY_COMMA,		///< ,
    LN_KEY_PERIOD,	    ///< .
	LN_KEY_SLASH,	    ///< /
	LN_KEY_MINUS,		///< -
	LN_KEY_BACKSLASH,   ///< \(バックスラッシュ)
	LN_KEY_YEN,			///< \(BaskSpaceキーの左の\キー)
	LN_KEY_CARET,	    ///< ^
	LN_KEY_LBRACKET,    ///< [
	LN_KEY_RBRACKET,    ///< ]

    LN_KEY_ANDROID_PLUS,	    ///< Android '+'

    LN_MAX_KEYS = 256,
};

/// マウスカーソルの挙動
enum LNMouseCursorMode
{
	LN_MCMODE_VISIBLE = 0,		    ///< 常に表示する
	LN_MCMODE_INVISIBLE,		    ///< クアイアント領域内では常に表示しない (GUI 使う場合はコレ)
	LN_MCMODE_MOVER_VISIBLE,	    ///< 一定時間動かない場合は消す
};

namespace Core
{
namespace System
{

#define LN_EVENT_STRUCT_HEADER  int Type;

struct Event
{
    LN_EVENT_STRUCT_HEADER;
};

/// マウスイベントの引数
struct MouseEvent
{
    LN_EVENT_STRUCT_HEADER;

    LNSystemMouseButton	Button;		///< ボタン番号
    int				    Delta;		///< マウスホイールの回転回数 (windows では 1 回につき 120 が格納されるが、これは正または負の回数)
    int				    X;			///< マウスイベント生成時のマウスの X 座標
    int				    Y;			///< マウスイベント生成時のマウスの Y 座標
    int                 MoveX;	    ///< X 座標の移動量 (GUI で使うときのみ有効)
    int                 MoveY;      ///< Y 座標の移動量
};

/// キー入力イベントの引数
struct KeyEvent
{
    LN_EVENT_STRUCT_HEADER;

    lnU32   KeyCode;			    ///< キーコード (LNVirtualKeyCode の値または文字コード(LNEV_KEY_CHAR 時。'a'-'z' 'A-Z' '0'-'9' LN_KEY_BACKSPACE LN_KEY_SPACE))
    bool	IsAlt;				    ///< Alt キーが押されている場合 true
    bool	IsShift;			    ///< Shift キーが押されている場合 true
    bool	IsControl;			    ///< Ctrl キーが押されている場合 true
};

/// タッチイベントの引数
struct TouchEvent
{
    LN_EVENT_STRUCT_HEADER;

    int     TouchID;    ///< タッチ番号 (タッチした順の指の番号)
    int     X;          ///< マウスイベント生成時のマウスの X 座標
    int     Y;          ///< マウスイベント生成時のマウスの Y 座標
    int     MoveX;      ///< X 座標の移動量
    int     MoveY;      ///< Y 座標の移動量
};

/// IME イベントの引数
struct IMEEvent
{
    LN_EVENT_STRUCT_HEADER;

    bool IsCOMPATTR;			    ///< 編集文字列のアトリビュートが取得された、または最新化された 
    bool IsCOMPCLAUSE;			    ///< 編集文字列の文節情報が取得された、または最新化された 
    bool IsCOMPREADATTR;		    ///< 現在の編集のリーディング文字列が取得された、または最新化された 
    bool IsCOMPREADCLAUSE;		    ///< 編集文字列のリーディング文字列の文節情報が取得された、または最新化された 
    bool IsCOMPREADSTR;			    ///< 現在の編集のリーディング文字列が取得された、または最新化された 
    bool IsCOMPSTR;				    ///< 現在の編集文字列が取得された、または最新化された 
    bool IsCURSORPOS;			    ///< 編集文字列のカーソル位置が取得した、または最新化された 
    bool IsDELTASTART;			    ///< 編集文字列の変化の開始位置が取得された、または最新化された 
    bool IsRESULTCLAUSE;		    ///< 確定文字列の文節情報が取得された、または最新化された 
    bool IsRESULTREADCLAUSE;	    ///< リーディング文字列の文節情報が取得された、または最新化された 
    bool IsRESULTREADSTR;		    ///< リーディング文字列を取得した、または最新化された 
    bool IsRESULTSTR;			    ///< 確定文字列が取得された、または最新化された
};
    
//#define LNEV_DEVICE_LOST,           ///< デバイスロスト処理の要求 (Graphics で post される。描画スレッドから通知されるため、send ではなく post)
    //LNEV_DEVICE_RESET,          ///< デバイスリセット処理の要求

// Event 作成ユーティリティ
template< typename T_ >
T_* createEvent( int type_ )
{
    T_* e = LN_NEW T_;
    e->Type = type_;
    return e;
}

#if 0
/// イベントメッセージ (コメントの [] 内のものはメッセージとセットになる引数の型)
enum LNEventType
{
	LNEV_NULL = 0,             ///< 未定義のイベント

    LNEV_QUIT,

	LNEV_APP_ACTIVATE,		    ///< ウィンドウがアクティブになった
	LNEV_APP_DEACTIVATE,	    ///< ウィンドウが非アクティブになった
	LNEV_ALTENTER,             ///< Alt + Enter

	LNEV_MOUSE_DOWN,		    ///< [ LNEvent::Mouse ] マウスボタンが押された
	LNEV_MOUSE_UP,	            ///< [ LNEvent::Mouse ] マウスボタンが離された
	LNEV_MOUSE_MOVE,		    ///< [ LNEvent::Mouse ] マウスが移動した
	LNEV_MOUSE_WHEEL,		    ///< [ LNEvent::Mouse ] マウスホイールが操作された
 
	LNEV_KEY_DOWN,			    ///< [ LNEvent::Key ] キー押し下
	LNEV_KEY_UP,			    ///< [ LNEvent::Key ] キー押し上げ
	LNEV_KEY_CHAR,			    ///< [ LNEvent::Key ] 文字入力 (日本語入力 OFF の状態でキーが押され場合)
    
    LNEV_TOUCH_DOWN,            ///< [ LNEvent::Touch ] 画面がタッチされた
    LNEV_TOUCH_UP,              ///< [ LNEvent::Touch ] 画面から離された
    LNEV_TOUCH_MOVE,            ///< [ LNEvent::Touch ] タッチ中に移動した


    LNEV_CLOSE,                 ///< ウィンドウのクローズボタンが押された
    

    LNEV_DEVICE_LOST,           ///< デバイスロスト処理の要求 (Graphics で post される。描画スレッドから通知されるため、send ではなく post)
    //LNEV_DEVICE_RESET,          ///< デバイスリセット処理の要求



	//LM_IME_COMPOSITION_START,	///< IME 編集開始
	//LM_IME_COMPOSITION_END,		///< IME 編集終了
	//LM_IME_COMPOSITION,			///< キーの入力の結果、変換文字列が変化した ( IME 有効時のみ )

	//LM_IME_CANDIDATE_OPEN,		///< 候補ウィンドウを開こうとしている
	//LM_IME_CANDIDATE_CLOSE,		///< 候補ウィンドウを閉じようとしている
	//LM_IME_CANDIDATE_UPDATE,	///< IMEが候補ウィンドウの内容を変えようとしたとき

    //LNEV_DEVICECHANGE,         ///< (USB) デバイスが接続、または取り外された

    LNEV_ANDROID_INIT_WINDOW,   ///< Android 用。ウィンドウが初期化された

    
    // 以下、範囲を表す定数
    LNEV_APP_BEGIN     = LNEV_APP_ACTIVATE,
    LNEV_APP_END       = LNEV_APP_DEACTIVATE,
    LNEV_MOUSE_BEGIN   = LNEV_MOUSE_DOWN,
    LNEV_MOUSE_END     = LNEV_MOUSE_WHEEL,
    LNEV_KEY_BEGIN     = LNEV_KEY_DOWN,
	LNEV_KEY_END       = LNEV_KEY_CHAR,
    LNEV_FORCUS_BEGIN  = LNEV_MOUSE_WHEEL,
    LNEV_FORCUS_END    = LNEV_KEY_CHAR,

	LNEV_END

    /*
        注意点…
            Android でソフトウェアキーボードを使った場合、
            LNEV_KEY_DOWN と LNEV_KEY_UP はキーを離した瞬間に同じタイミングで発生してしまう。
    */
};

/// イベントメッセージの引数
struct LNEvent
{
    LNEventType     Type;

    union
    {
        /// マウスイベントの引数
        struct
        {
	        LNSystemMouseButton	Button;		///< ボタン番号
	        int				    Delta;		///< マウスホイールの回転回数 (windows では 1 回につき 120 が格納されるが、これは正または負の回数)
	        int				    X;			///< マウスイベント生成時のマウスの X 座標
	        int				    Y;			///< マウスイベント生成時のマウスの Y 座標
            int                 MoveX;	    ///< X 座標の移動量 (GUI で使うときのみ有効)
	        int                 MoveY;      ///< Y 座標の移動量

        } Mouse;

        /// キー入力イベントの引数
        struct
        {
	        lnU32   KeyCode;			    ///< キーコード (LNVirtualKeyCode の値または文字コード(LNEV_KEY_CHAR 時。'a'-'z' 'A-Z' '0'-'9' LN_KEY_BACKSPACE LN_KEY_SPACE))
	        bool	IsAlt;				    ///< Alt キーが押されている場合 true
	        bool	IsShift;			    ///< Shift キーが押されている場合 true
	        bool	IsControl;			    ///< Ctrl キーが押されている場合 true

        } Key;
        
        /// タッチイベントの引数
        struct
        {
            int     TouchID;    ///< タッチ番号 (タッチした順の指の番号)
	        int     X;          ///< マウスイベント生成時のマウスの X 座標
	        int     Y;          ///< マウスイベント生成時のマウスの Y 座標
            int     MoveX;      ///< X 座標の移動量
	        int     MoveY;      ///< Y 座標の移動量

        } Touch;

        /// IME イベントの引数
        struct
        {
	        bool IsCOMPATTR;			    ///< 編集文字列のアトリビュートが取得された、または最新化された 
	        bool IsCOMPCLAUSE;			    ///< 編集文字列の文節情報が取得された、または最新化された 
	        bool IsCOMPREADATTR;		    ///< 現在の編集のリーディング文字列が取得された、または最新化された 
	        bool IsCOMPREADCLAUSE;		    ///< 編集文字列のリーディング文字列の文節情報が取得された、または最新化された 
	        bool IsCOMPREADSTR;			    ///< 現在の編集のリーディング文字列が取得された、または最新化された 
	        bool IsCOMPSTR;				    ///< 現在の編集文字列が取得された、または最新化された 
	        bool IsCURSORPOS;			    ///< 編集文字列のカーソル位置が取得した、または最新化された 
	        bool IsDELTASTART;			    ///< 編集文字列の変化の開始位置が取得された、または最新化された 
	        bool IsRESULTCLAUSE;		    ///< 確定文字列の文節情報が取得された、または最新化された 
	        bool IsRESULTREADCLAUSE;	    ///< リーディング文字列の文節情報が取得された、または最新化された 
	        bool IsRESULTREADSTR;		    ///< リーディング文字列を取得した、または最新化された 
	        bool IsRESULTSTR;			    ///< 確定文字列が取得された、または最新化された

        } IME;
    };
};
#endif

} // namespace System
} // namespace Core

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================