//=============================================================================
//【 Manager 】
//-----------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Geometry/Matrix.h"
#include "Interface.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Touch.h"
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
class Joypad;

//=============================================================================
// ■ Manager クラス
//-----------------------------------------------------------------------------
///**
//  @brief      入力モジュールの管理クラス
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
    , public IManager
    , public System::IEventListener
{
public:

	/// コンストラクタに渡す初期化データ
	struct InitData
	{
        Base::LogFile*      LogFile;
        System::IWindow*    Window;		        ///< DirectInput の初期化に使うウィンドウハンドル
	};

public:

	/// コンストラクタ
	Manager();

	/// デストラクタ
	virtual ~Manager();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    LNRESULT initialize( const InitData& data_ );

    /// 終了処理
    void finalize();

    /// GameController へのポインタの取得
    virtual IGameController* getGameController( int index_ ) { return mGameControllers[ index_ ]; }

	/// Mouse へのポインタの取得
	virtual IMouse* getMouse() { return mMouse; }

	/// Keyboard へのポインタの取得
	virtual IKeyboard* getKeyboard() { return mKeyboard; }

	/// Joypad へのポインタの取得
	virtual IJoypad* getJoypad( int index_ );
    
    /// Touch へのポインタの取得
	virtual Touch* getTouch() { return mTouch; }

	/// 接続されているジョイパッドの数の取得
	virtual int getJoypadNum() { return 0; }

    /// IInputDevice へのポインタの取得
    virtual IInputDevice* getInputDevice();

    /// フレーム更新
    virtual void update();

    /// メッセージ処理コールバック
    virtual bool onEvent( const System::Event& args_ );

    void setMouseTransform( const LMatrix& transform_ ) { mMouseTransform = transform_; }

public:

    /// マウス位置の変換行列の取得
    const LMatrix& getMouseTransform() const { return mMouseTransform; }

    /// マウス位置が設定できるか
    bool canSetMousePoint() const;

private:

    Base::LogFile*              mLogFile;
    //Win32::InputDevice*	        mInputDevice;
    IInputDevice*	            mInputDevice;
    LMatrix                     mMouseTransform;
	Mouse*			            mMouse;
	Keyboard*		            mKeyboard;
	Joypad*			            mJoypads[ LN_MAX_JOYPADS ];
    Touch*                      mTouch;
	GameController*	            mGameControllers[ LN_MAX_GAME_CONTROLLERS ];

    bool                        mCanSetMousePoint;

};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Input
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================