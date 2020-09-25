//=============================================================================
//【 Interface 】
//-----------------------------------------------------------------------------
//**
//  @file       Interface.h
//  @brief      Interface
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Geometry/Vector2.h"
#include "../System/Interface.h"
//#include "InputConfigData.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

static const int LN_MAX_JOYPADS = 4;            ///< ジョイパッドの最大数
static const int LN_MAX_JOYPAD_BUTTONS = 16;    ///< ジョイパッドのボタンの最大数
static const int LN_MAX_JOYPAD_AXIS = 8;        ///< ジョイパッドの軸の最大数
static const int LN_MAX_GAME_CONTROLLERS = 4;   ///< 仮想コントローラの最大数
#define          LN_JOYPAD_PLAY_RANGE   (0.2f)  ///< ジョイパッドのあそび範囲
#define          LN_JOYPAD_SWITCH_RANGE (0.8f)

/// マウスのボタン番号
enum LNMouseButton
{
	LN_MOUSE_BUTTON_1 = 0,
	LN_MOUSE_BUTTON_2,
	LN_MOUSE_BUTTON_3,
	LN_MOUSE_BUTTON_4,
	LN_MOUSE_BUTTON_5,
	LN_MOUSE_BUTTON_6,
	LN_MOUSE_BUTTON_7,
	LN_MOUSE_BUTTON_8,

	LN_MAX_MOUSE_BUTTONS
};

/// ゲームボタンを表す値 (()内はデフォルトで割り当てらているキー、ボタン、軸)
enum LNGameControllerButton
{
	LN_BUTTON_A = 0,        ///< Aボタン (キーボード='Z' ジョイパッド=0番ボタン)
	LN_BUTTON_B,            ///< Bボタン (キーボード='X' ジョイパッド=1)
	LN_BUTTON_C,            ///< Cボタン (キーボード='C' ジョイパッド=2)
	LN_BUTTON_X,            ///< Xボタン (キーボード='A' ジョイパッド=3)
	LN_BUTTON_Y,            ///< Yボタン (キーボード='S' ジョイパッド=4)
	LN_BUTTON_Z,            ///< Zボタン (キーボード='D' ジョイパッド=5)
	LN_BUTTON_L,            ///< Lボタン (キーボード='Q' ジョイパッド=6)
	LN_BUTTON_R,            ///< Rボタン (キーボード='W' ジョイパッド=7)
	LN_BUTTON_L2,           ///< L2ボタン (キーボード=None ジョイパッド=8)
	LN_BUTTON_R2,           ///< R2ボタン (キーボード=None ジョイパッド=9)
	LN_BUTTON_L3,           ///< L3ボタン (キーボード=None ジョイパッド=10)
	LN_BUTTON_R3,           ///< R3ボタン (キーボード=None ジョイパッド=11)
	LN_BUTTON_START,        ///< R3ボタン (キーボード=None ジョイパッド=12)
	LN_BUTTON_SELECT,       ///< R3ボタン (キーボード=None ジョイパッド=13)
	LN_BUTTON_SUB1,         ///< R3ボタン (キーボード=None ジョイパッド=14)
	LN_BUTTON_SUB2,         ///< R3ボタン (キーボード=None ジョイパッド=15)
	LN_BUTTON_DOWN,         ///< 下ボタン (キーボード=LN_KEY_DOWN  ジョイパッド=POV下&第1Y軸+)
	LN_BUTTON_LEFT,         ///< 左ボタン (キーボード=LN_KEY_LEFT  ジョイパッド=POV左&第1X軸-)
	LN_BUTTON_RIGHT,        ///< 右ボタン (キーボード=LN_KEY_RIGHT ジョイパッド=POV右&第1X軸+)
	LN_BUTTON_UP,           ///< 上ボタン (キーボード=LN_KEY_UP    ジョイパッド=POV上&第1Y軸-)

	LN_MAX_BUTTONS
};

/// コントローラのアタッチ (キーコンフィグ) ターゲット
enum LNGameControllerAttachTarget
{
	LN_AT_BUTTON_A = 0,     ///< Aボタン
	LN_AT_BUTTON_B,         ///< Bボタン
	LN_AT_BUTTON_C,         ///< Cボタン
	LN_AT_BUTTON_X,         ///< Xボタン
	LN_AT_BUTTON_Y,         ///< Yボタン
	LN_AT_BUTTON_Z,         ///< Zボタン
	LN_AT_BUTTON_L,         ///< Lボタン
	LN_AT_BUTTON_R,         ///< Rボタン
	LN_AT_BUTTON_L2,        ///< L2ボタン
	LN_AT_BUTTON_R2,        ///< R2ボタン
	LN_AT_BUTTON_L3,        ///< L3ボタン
	LN_AT_BUTTON_R3,        ///< R3ボタン
	LN_AT_BUTTON_START,     ///< R3ボタン
	LN_AT_BUTTON_SELECT,    ///< R3ボタン
	LN_AT_BUTTON_SUB1,      ///< R3ボタン
	LN_AT_BUTTON_SUB2,      ///< R3ボタン
	LN_AT_BUTTON_DOWN,      ///< 下ボタン
	LN_AT_BUTTON_LEFT,      ///< 左ボタン
	LN_AT_BUTTON_RIGHT,     ///< 右ボタン
	LN_AT_BUTTON_UP,        ///< 上ボタン

    LN_AT_AXIS_1X_MINUIS,   ///< 第1レバー X- 方向
    LN_AT_AXIS_1X_PLUS,     ///< 第1レバー X+ 方向
    LN_AT_AXIS_1Y_MINUIS,   ///< 第1レバー Y- 方向
    LN_AT_AXIS_1Y_PLUS,     ///< 第1レバー Y+ 方向
    LN_AT_AXIS_2X_MINUIS,   ///< 第2レバー X- 方向
    LN_AT_AXIS_2X_PLUS,     ///< 第2レバー X+ 方向
    LN_AT_AXIS_2Y_MINUIS,   ///< 第2レバー Y- 方向
    LN_AT_AXIS_2Y_PLUS,     ///< 第2レバー Y+ 方向


	LN_MAX_AT_TARGETS,

    LN_AT_BUTTON_BEGIN = LN_AT_BUTTON_A,
    LN_AT_BUTTON_END   = LN_AT_BUTTON_UP,
    LN_AT_AXIS_BEGIN   = LN_AT_AXIS_1X_MINUIS,
    LN_AT_AXIS_END     = LN_AT_AXIS_2Y_PLUS,

};

/// 全てのデバイス用の仮想ボタン番号 (キーコンフィグで使う  例)attachButton( LN_BUTTON_L2, LN_VBUTTON_JOYPAD_AXIS_3_PLUS ))
enum LNVirtualButton
{
    LN_VBUTTON_NULL = 0,

    LN_VBUTTON_MOUSE_1 = 256,
    LN_VBUTTON_MOUSE_2,
    LN_VBUTTON_MOUSE_3,
    LN_VBUTTON_MOUSE_4,
    LN_VBUTTON_MOUSE_5,
    LN_VBUTTON_MOUSE_6,
    LN_VBUTTON_MOUSE_7,
    LN_VBUTTON_MOUSE_8,

    LN_VBUTTON_JOYPAD_BUTTON_1,
    LN_VBUTTON_JOYPAD_BUTTON_2,
    LN_VBUTTON_JOYPAD_BUTTON_3,
    LN_VBUTTON_JOYPAD_BUTTON_4,
    LN_VBUTTON_JOYPAD_BUTTON_5,
    LN_VBUTTON_JOYPAD_BUTTON_6,
    LN_VBUTTON_JOYPAD_BUTTON_7,
    LN_VBUTTON_JOYPAD_BUTTON_8,
    LN_VBUTTON_JOYPAD_BUTTON_9,
    LN_VBUTTON_JOYPAD_BUTTON_10,
    LN_VBUTTON_JOYPAD_BUTTON_11,
    LN_VBUTTON_JOYPAD_BUTTON_12,
    LN_VBUTTON_JOYPAD_BUTTON_13,
    LN_VBUTTON_JOYPAD_BUTTON_14,
    LN_VBUTTON_JOYPAD_BUTTON_15,
    LN_VBUTTON_JOYPAD_BUTTON_16,

    LN_VBUTTON_JOYPAD_POV_UP,
    LN_VBUTTON_JOYPAD_POV_RIGHT,
    LN_VBUTTON_JOYPAD_POV_DOWN,
    LN_VBUTTON_JOYPAD_POV_LEFT,

    LN_VBUTTON_JOYPAD_AXIS_1_MINUIS,      ///< X1 -1.0
    LN_VBUTTON_JOYPAD_AXIS_1_PLUS,        ///< X1  1.0
    LN_VBUTTON_JOYPAD_AXIS_2_MINUIS,      ///< Y1 -1.0
    LN_VBUTTON_JOYPAD_AXIS_2_PLUS,        ///< Y1  1.0
    LN_VBUTTON_JOYPAD_AXIS_3_MINUIS,      ///< X2 -1.0
    LN_VBUTTON_JOYPAD_AXIS_3_PLUS,        ///< X2  1.0
    LN_VBUTTON_JOYPAD_AXIS_4_MINUIS,      ///< Y2 -1.0
    LN_VBUTTON_JOYPAD_AXIS_4_PLUS,        ///< Y2  1.0
    LN_VBUTTON_JOYPAD_AXIS_5_MINUIS,      ///< X3 -1.0
    LN_VBUTTON_JOYPAD_AXIS_5_PLUS,        ///< X3  1.0     XInput LT
    LN_VBUTTON_JOYPAD_AXIS_6_MINUIS,      ///< Y3 -1.0
    LN_VBUTTON_JOYPAD_AXIS_6_PLUS,        ///< Y3  1.0     XInput RT
    LN_VBUTTON_JOYPAD_AXIS_7_MINUIS,      ///< X4 -1.0
    LN_VBUTTON_JOYPAD_AXIS_7_PLUS,        ///< X4  1.0
    LN_VBUTTON_JOYPAD_AXIS_8_MINUIS,      ///< Y4 -1.0
    LN_VBUTTON_JOYPAD_AXIS_8_PLUS,        ///< Y4  1.0

    LN_VBUTTON_LAST,

    LN_VBUTTON_KEYBOARD_BEGIN      = 0,        // 0～255 はとりあえずキーボード用
    LN_VBUTTON_KEYBOARD_END        = 255,
    LN_VBUTTON_MOUSE_BEGIN         = LN_VBUTTON_MOUSE_1,
    LN_VBUTTON_MOUSE_END           = LN_VBUTTON_MOUSE_8,
    LN_VBUTTON_JOYPAD_BUTTON_BEGIN = LN_VBUTTON_JOYPAD_BUTTON_1,
    LN_VBUTTON_JOYPAD_BUTTON_END   = LN_VBUTTON_JOYPAD_BUTTON_16,
    LN_VBUTTON_JOYPAD_POV_BEGIN    = LN_VBUTTON_JOYPAD_POV_UP,
    LN_VBUTTON_JOYPAD_POV_END      = LN_VBUTTON_JOYPAD_POV_LEFT,
    LN_VBUTTON_JOYPAD_AXIS_BEGIN   = LN_VBUTTON_JOYPAD_AXIS_1_MINUIS,
    LN_VBUTTON_JOYPAD_AXIS_END     = LN_VBUTTON_JOYPAD_AXIS_8_PLUS,
};

/// 入力デバイスの有効フラグ
enum LNInputAttachFlags
{
	LN_INPUTATTACH_NONE		= 0x00,
	LN_INPUTATTACH_MOUSE	= 0x01,		///< マウスから入力を受け取る
    LN_INPUTATTACH_KEYBOARD	= 0x02,		///< キーボードから入力を受け取る
	LN_INPUTATTACH_JOYPAD	= 0x04,		///< ジョイパッドから入力を受け取る

    LN_INPUTATTACH_ALL      = 0xff,     ///< 上記すべてのデバイスから入力を受け取る
};

/// マウスの状態を表す構造体
struct LNMouseDeviceState
{
	long	X;
	long	Y;
	long	RelX;						        ///< 1 フレーム前の位置からの相対 X 座標
	long	RelY;
	long	Wheel;						        ///< ホイール
	bool	Buttons[ LN_MAX_MOUSE_BUTTONS ];	///< ボタンを押し下している場合は true
	bool	Succeeded;					        ///< データが正常であることを示すフラグ ( false の場合、直前に取得できた値が格納されている )
};

/// ジョイパッドの POV の向きを表すフラグ
enum LNPOVDirFlag
{
    POV_NONE    = 0x00,
    POV_UP      = 0x01,
    POV_RIGHT   = 0x02,
    POV_DOWN    = 0x04,
    POV_LEFT    = 0x08,
};

/// ジョイパッドの状態を表す構造体
struct LNJoypadDeviceState
{
	float	Axes[ LN_MAX_JOYPAD_AXIS ];
	u32     POV;						        ///< POV 状態 (POVDir の組み合わせ)
	u8		Buttons[ LN_MAX_JOYPAD_BUTTONS ];
};

/// タッチの状態を表す構造体
struct LNTouchDeviceState
{
	long	X;
	long	Y;
	long	RelX;   ///< 1 フレーム前の位置からの相対 X 座標
	long	RelY;
	bool	Touch;  ///< 押している場合は true
};

} // namespace LNote

//-------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Input
{

class IMouse;
class IKeyboard;
class IJoypad;
class IGameController;
class IInputDevice;

//=============================================================================
// ■ IManager クラス
//-----------------------------------------------------------------------------
///**
//  @brief      入力モジュールの管理インターフェイス
//*/
//=============================================================================
class IManager
    : private Base::Interface
{
public:

    /// GameController へのポインタの取得 (index_ は 0 ～ 3)
	virtual IGameController* getGameController( int index_ ) = 0;

	/// Mouse へのポインタの取得
	virtual IMouse* getMouse() = 0;

	/// Keyboard へのポインタの取得
	virtual IKeyboard* getKeyboard() = 0;

	/// Joypad へのポインタの取得
	virtual IJoypad* getJoypad( int index_ ) = 0;

	/// 接続されているジョイパッドの数の取得
	virtual int getJoypadNum() = 0;

    /// IInputDevice へのポインタの取得
    virtual IInputDevice* getInputDevice() = 0;

protected:

    virtual ~IManager() {}
};



//=============================================================================
// ■ IGameController クラス
//-----------------------------------------------------------------------------
///**
//  @brief      仮想ゲームコントローラのインターフェイス
//*/
//=============================================================================
class IGameController
    : public Base::Interface
{
public:
	
	/// ボタンが現在押されているかを判定する
	virtual bool isPress( LNGameControllerButton button_ ) = 0;

	/// ボタンが新しく押された瞬間を判定する
	virtual bool isOnTrigger( LNGameControllerButton button_ ) = 0;

	/// キーが離された瞬間を判定する
	virtual bool isOffTrigger( LNGameControllerButton button_ ) = 0;

	/// ボタンが新しく押された瞬間とリピート状態を判定する
	virtual bool isRepeat( LNGameControllerButton button_ ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      アナログ入力値の取得
    //
    //  @param[in]  button_ : ゲームコントローラのボタン
    //
    //  @return     0.0～1.0
    //
    //  @par
    //              指定ボタンの入力をアナログ値で受け取ります。<br>
    //              軸がアタッチされている場合も、正の実数を返します。<br>
    //              Xbox360 コントローラの LT RT ボタンの値を取得するとき等に使えると思います。
    //*/
    //---------------------------------------------------------------------
	virtual float getPower( LNGameControllerButton button_ ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      軸の状態の取得
    //
    //  @param[in]  axis_ : 軸番号 (0 ～ 3)
    //
    //  @par
    //              レバーのアナログ入力を、X と Y をセットにして返します。<br>
    //              IJoypad::getAxisState() とは引数の意味が違うので注意してください。
    //*/
    //---------------------------------------------------------------------
	virtual const LVector2& getAxisState( int axis_ ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      新しく押された仮想ボタンの取得
    //
    //  @return     押されたキー(仮想キーコード)、ボタン、軸に対応する仮想ボタン
    //              
    //  @par
    //              押し判定はトリガーです。isOnTrigger() と同じく、押されたフレームだけ
    //              対応する仮想ボタンを返し、それ以外は LN_VBUTTON_NONE (押されていない) を返します。
    //*/
    //---------------------------------------------------------------------
	virtual int getPressedButton() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ボタン配置 (キーコンフィグ) を設定する
    //
    //  @param[in]  ln_button_   : 変更するボタン
    //  @param[in]  v_button_    : 新しく設定する仮想ボタン
    //  @param[in]  duplication_ : true の場合、重複を許可する
    //
    //  @par
    //              仮想ボタンをゲームコントローラのボタンにアタッチします。<br>
    //              <br>
    //              基本的に getPressedButton() とセットで使い、getPressedButton()から
    //              受け取った仮想ボタン番号をこの関数の v_button_ に渡します。<br>
    //              <br>
    //              duplication_ が false の場合、ln_button_ 以外のボタンに
    //              同じキーが割り当てられている場合、値を交換します。
    //*/
    //---------------------------------------------------------------------
	virtual void attachVirtualButton( LNGameControllerAttachTarget target_, int v_button_, bool duplication_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ボタン配置 (キーコンフィグ) を解除する
    //
    //  @param[in]  ln_button_ : 変更するボタン
    //  @param[in]  v_button_  : 新しく設定する仮想ボタン
    //
    //  @par
    //              v_button_ が ln_button_ にアタッチされている場合、アタッチを解除します。<br>
    //              v_button_ に LN_VBUTTON_NULL を渡すと、すべてのアタッチを解除します。
    //*/
    //---------------------------------------------------------------------
	virtual void detachVirtualButton( LNGameControllerAttachTarget target_, int v_button_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      入力を受け取るデバイスの指定
    // 
    //  @param[in]  flags_ : LNInputAttachFlags のひとつ以上のフラグの組み合わせ
    // 
    //  @par
    //              デフォルトは LN_INPUTATTACH_ALL です。<br>
    //              <br>
    //              例えば LN_INPUTATTACH_MOUSE | LN_INPUTATTACH_KEYBOARD と設定した場合、
    //              このゲームコントローラにジョイパッドが接続されていてもそれからの入力は一切受け取りません。
    //*/
    //---------------------------------------------------------------------
	virtual void setDeviceAttachFlag( u8 flags_ ) = 0;

protected:

    virtual ~IGameController() {}
};



//=============================================================================
// ■ IMouse クラス
//-----------------------------------------------------------------------------
///**
//  @brief      マウスからの入力を扱うインターフェイス
//*/
//=============================================================================
class IMouse
    : public Base::Interface
{
public:

	/// ボタンが現在押されているかを判定する
	virtual bool isPress( LNMouseButton button_ ) = 0;

	/// ボタンが新しく押された瞬間を判定する
	virtual bool isOnTrigger( LNMouseButton button_ ) = 0;

	/// ボタンが離された瞬間を判定する
	virtual bool isOffTrigger( LNMouseButton button_ ) = 0;

	/// ボタンが新しく押された瞬間とリピート状態を判定する
	virtual bool isRepeat( LNMouseButton button_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      マウス座標の設定
    //
    //  @par
    //              座標はクライアント領域の左上を原点 (0, 0) とした値で設定します。
    //              ウィンドウがアクティブではない場合はなにもしません。
    //*/
    //---------------------------------------------------------------------
	virtual void setPoint( const LVector2& point_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      マウス座標の取得
    //
    //  @par
    //              座標はクライアント領域の左上を原点 (0, 0) とした値です。
    //*/
    //---------------------------------------------------------------------
	virtual const LVector2& getPoint() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      1 フレーム前からの移動量の取得
    //*/
    //---------------------------------------------------------------------
	virtual const LVector2& getMoveDistance() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      1 フレームのホイールの回転量の取得
    //
    //  @return
    //              正の値は前(奥)、負の値は後ろ(手前)へ回された事を意味します。
    //              この関数は回した回数を返します。
    //*/
    //---------------------------------------------------------------------
	virtual int getWheelState() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      位置を中央に固定するかの設定
    //
    //  @par        
    //              true を渡すと、マウスカーソルをクライアント領域中央に
    //              固定します。getMoveDistance() で移動量を取得してください。
    //*/
    //---------------------------------------------------------------------
	virtual void setEnableCenterFix( bool flag_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      リピート間隔の設定
    //
    //  @param[in]  first_    : リピートを開始するまでのフレーム数 (デフォルトは 20)
    //  @param[in]  interval_ : リピート間隔フレーム数 (デフォルトは 5)
    //*/
    //---------------------------------------------------------------------
	virtual void setRepeatInterval( u32 first_, u32 interval_ ) = 0;

protected:

    virtual ~IMouse() {}
};

//=============================================================================
// ■ IKeyboard クラス
//-----------------------------------------------------------------------------
///**
//  @brief      キーボードからの入力を扱うインターフェイス
//*/
//=============================================================================
class IKeyboard
    : public Base::Interface
{
public:
	
    //---------------------------------------------------------------------
    ///**
    //  @brief      キーが現在押されているかを判定する
    //
    //  @param[in]  vkey_    : 仮想キーコード
    //*/
    //---------------------------------------------------------------------
	virtual bool isPress( LNVirtualKeyCode vkey_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      キーが新しく押された瞬間を判定する
    //
    //  @param[in]  vkey_    : 仮想キーコード
    //*/
    //---------------------------------------------------------------------
	virtual bool isOnTrigger( LNVirtualKeyCode vkey_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      キーが離された瞬間を判定する
    //
    //  @param[in]  vkey_    : 仮想キーコード
    //*/
    //---------------------------------------------------------------------
	virtual bool isOffTrigger( LNVirtualKeyCode vkey_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ボタンが新しく押された瞬間とリピート状態を判定する
    //
    //  @param[in]  vkey_    : 仮想キーコード
    //*/
    //---------------------------------------------------------------------
	virtual bool isRepeat( LNVirtualKeyCode vkey_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      キーボードのバッファリングデータの取得
    //
    //  @return     仮想キーコード
    //
    //  @par
    //              0 を返すまで while で回して取得してください。
    //*/
    //---------------------------------------------------------------------
    virtual LNVirtualKeyCode getKeyBufferState() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      リピート間隔の設定
    //
    //  @param[in]  first_    : リピートを開始するまでのフレーム数 (デフォルトは 20)
    //  @param[in]  interval_ : リピート間隔フレーム数 (デフォルトは 5)
    //*/
    //---------------------------------------------------------------------
	virtual void setRepeatInterval( u32 first_, u32 interval_ ) = 0;

protected:

	virtual ~IKeyboard() {};
};

//=============================================================================
// ■ IJoypad クラス
//-----------------------------------------------------------------------------
///**
//  @brief      ジョイパッドのインターフェイス
//*/
//=============================================================================
class IJoypad
    : public Base::Interface
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      接続済みかをチェックする
    //
    //  @retval     true  : 接続済み (使用可能)
    //  @retval     false : 未接続 (使用不可)
    //
    //  @par
    //              未接続の場合、ボタンの状態は false、軸は 0.0、POV は 0 を返します。
    //*/
    //---------------------------------------------------------------------
	virtual bool isConnected() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ボタンが現在押されているかを判定する
    //
    //  @param[in]  button_ : ボタン番号 (0 ～ 15)
    //*/
    //---------------------------------------------------------------------
	virtual bool isPress( int button_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ボタンが新しく押された瞬間を判定する
    //
    //  @param[in]  button_ : ボタン番号 (0 ～ 15)
    //*/
    //---------------------------------------------------------------------
	virtual bool isOnTrigger( int button_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ボタンが離された瞬間を判定する
    //
    //  @param[in]  button_ : ボタン番号 (0 ～ 15)
    //*/
    //---------------------------------------------------------------------
	virtual bool isOffTrigger( int button_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ボタンが新しく押された瞬間とリピート状態を判定する
    //
    //  @param[in]  button_ : ボタン番号 (0 ～ 15)
    //*/
    //---------------------------------------------------------------------
	virtual bool isRepeat( int button_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      軸の状態の取得
    //
    //  @param[in]  axis_ : 軸番号 (0 ～ 7)
    //*/
    //---------------------------------------------------------------------
    virtual float getAxisState( int axis_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      POV の状態の取得
    //*/
    //---------------------------------------------------------------------
	virtual u32 getPOVState() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ジョイパッドの振動を開始する
    //
    //  @param[in]  power_ : 振動の強さ (0～1000)
    //  @param[in]  time_  : 振動させる時間 (ミリ秒単位)
    //
    //  @par
    //              time_ に 0 以下の値を設定すると、stopVibration() が
    //              呼ばれるまで振動を続けます。
    //*/
    //---------------------------------------------------------------------
	virtual void startVibration( int power_, int time_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ジョイパッドの振動を停止する
    //*/
    //---------------------------------------------------------------------
	virtual void stopVibration() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      リピート間隔の設定
    //
    //  @param[in]  first_    : リピートを開始するまでのフレーム数 (デフォルトは 20)
    //  @param[in]  interval_ : リピート間隔フレーム数 (デフォルトは 5)
    //*/
    //---------------------------------------------------------------------
	virtual void setRepeatInterval( u32 first_, u32 interval_ ) = 0;

protected:

	virtual ~IJoypad() {};
};


//=============================================================================
// ■ IInputDevice クラス
//-----------------------------------------------------------------------------
///**
//  @brief      入力デバイスを一括して扱うインターフェイス
//
//  @par
//              このインターフェイスは主にライブラリ内部用に使っています。
//*/
//=============================================================================
class IInputDevice
    : public Base::Interface
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      接続済みのジョイパッド数の取得
    //*/
    //---------------------------------------------------------------------
	virtual int getJoypadNum() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      マウスの状態の取得
    //*/
    //---------------------------------------------------------------------
	virtual const LNMouseDeviceState& getMouseState() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      キーボードの状態の取得
    //
    //  @param[in]  vkey_ : 仮想キーコード 
    //*/
    //---------------------------------------------------------------------
	virtual bool getKeyState( LNVirtualKeyCode vkey_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      キーボードのバッファリングデータの取得
    //
    //  @return     仮想キーコード
    //
    //  @par
    //              LN_KEY_UNKNOWN を返すまで while で回して取得してください。
    //*/
    //---------------------------------------------------------------------
    virtual LNVirtualKeyCode getKeyBufferState() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ジョイパッドの状態の取得
    //*/
    //---------------------------------------------------------------------
	virtual void getJoypadState( int joypad_id_, LNJoypadDeviceState* state_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ジョイパッドの振動を開始する
    //*/
    //---------------------------------------------------------------------
	virtual void startVibration( int joypad_id_, int power_, int time_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ジョイパッドの振動を停止する
    //*/
    //---------------------------------------------------------------------
	virtual void stopVibration( int joypad_id_ ) = 0;
    
    /// タッチ情報の取得
    virtual void getTouchState( int touch_id_, LNTouchDeviceState* state_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      仮想ボタンの状態の取得
    //
    //  @param[in]  button_     : 調べる仮想ボタン (仮想キーコードまたは LNVirtualDeviceButton)
    //  @param[in]  keyboard_   : true の場合、キーボードの状態を考慮する
    //  @param[in]  mouse_      : true の場合、マウスの状態を考慮する
    //  @param[in]  joypad_id_  : 0 以上 の場合、その番号のジョイパッドの状態を考慮する
    //
    //  @return
    //              0.0～1.0 の値
    //*/
    //---------------------------------------------------------------------
	virtual float getVirtualButtonState( u32 button_, bool keyboard_, bool mouse_, int joypad_id_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      マウス座標の設定
    //
    //  @par
    //              座標はクライアント領域の左上を原点 (0, 0) とした値で設定します。
    //              ウィンドウがアクティブではない場合はなにもしません。
    //*/
    //---------------------------------------------------------------------
	virtual void setMousePoint( const LVector2& point_ ) = 0;

    /// マウスカーソルの位置をクライアント領域の中央に設定する
    virtual void setMouseCursorCenter() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      入力情報の更新
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT update() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      デバイスの再セットアップ
    //
    //  @par
    //              USB 機器の接続検知時などに呼び出します。
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT refresh() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      関連付けられている IWindow の取得
    //*/
    //---------------------------------------------------------------------
    virtual System::IWindow* getWindow() = 0;
    
    /// イベント処理 (Manager::onEvent() から呼ばれる)
    virtual bool onEvent( const System::Event& args_ ) = 0;

protected:

    virtual ~IInputDevice() {}
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Input
} // namespace Core

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================