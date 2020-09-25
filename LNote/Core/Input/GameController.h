//=============================================================================
//【 GameController 】
//-----------------------------------------------------------------------------
//**
//  @file       GameController.h
//  @brief      GameController
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include "Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Input
{
class Manager;

//=============================================================================
// ■ GameController クラス
//-----------------------------------------------------------------------------
///**
//  @brief      仮想ゲームコントローラのクラス
//*/
//=============================================================================
class GameController
    : public Base::ReferenceObject
    , public IGameController
{
public:

    /// コンストラクタ (ジョイパッドがない場合でも、番号を渡す)
	GameController( Manager* manager_, int joypad_id_ );

	/// デストラクタ 
	virtual ~GameController();

    LN_REFOBJ_METHODS;

public:
	
	/// ボタンが現在押されているかを判定する
	virtual bool isPress( LNGameControllerButton button_ );

	/// ボタンが新しく押された瞬間を判定する
	virtual bool isOnTrigger( LNGameControllerButton button_ );

	/// キーが離された瞬間を判定する
	virtual bool isOffTrigger( LNGameControllerButton button_ );

	/// ボタンが新しく押された瞬間とリピート状態を判定する
	virtual bool isRepeat( LNGameControllerButton button_ );

    /// アナログ入力値の取得
	virtual float getPower( LNGameControllerButton button_ );

    /// 軸の状態の取得
	virtual const LVector2& getAxisState( int axis_ );

    /// 新しく押された仮想ボタンの取得
	virtual int getPressedButton();

    /// ボタン配置 (キーコンフィグ) を設定する
	virtual void attachVirtualButton( LNGameControllerAttachTarget target_, int v_button_, bool duplication_ ) ;

    /// ボタン配置 (キーコンフィグ) を解除する
	virtual void detachVirtualButton( LNGameControllerAttachTarget target_, int v_button_ );

    /// 入力を受け取るデバイスの指定
    virtual void setDeviceAttachFlag( u8 flags_ ) { mDeviceAttachFlag = flags_; }

public:

    /// リピート間隔の設定
    void setRepeatInterval( s32 first_, s32 interval_ ) { mFirstRepeatInterval = first_; mRemRepeatInterval = interval_; }

    /// フレーム更新
	void update();

private:

    typedef std::vector< int >  AttachedVButtons;

private:

    Manager*	        mManager;							    ///< 管理クラス
    IInputDevice*       mInputDevice;
	lnS32			    mButtonStatus[ LN_MAX_BUTTONS ];	    ///< ボタンの状態
	float			    mButtonPower[ LN_MAX_BUTTONS ];
    AttachedVButtons    mAttachedVButtons[ LN_MAX_AT_TARGETS ];

    int					mJoypadID;							    ///< 割り当てられているジョイパッドの番号
    LVector2	        mAxisState[ LN_MAX_JOYPAD_AXIS / 2 ];   ///< 軸の状態

    u32                 mDeviceAttachFlag;

    s32					mFirstRepeatInterval;				    ///< リピート開始までの待ちフレーム数
	s32					mRemRepeatInterval;					    ///< リピート中の1回の待ちフレーム数
    bool				mPressedTriggerFlag;				    ///< getPressedButton() で使われるフラグ


    /*
	Manager*			mManager;							///< 管理クラス
	s8					mButtonStatus[ LN_MAX_BUTTONS ];	///< ボタンの状態
	LVector2	        mAxisState[ MAX_AXES ];				///< 軸の状態
	s32					mFirstRepeatInterval;				///< リピート開始までの待ちフレーム数
	s32					mRemRepeatInterval;					///< リピート中の1回の待ちフレーム数
	
	//float				mAxisInvalidRange;					///< 軸の無効範囲
	//u8					mInputEnableFlags;					///< 実際に使用するデバイスのフラグ
	bool				mAssignedkeyboard;					///< キーボードが割り当てられているフラグ
	bool				mPressedTriggerFlag;				///< getPressedButton() で使われるフラグ
    //bool                mEnableNormalize;                   ///< true の場合、軸を正規化する


    ConfigData mConfigData;
	
	friend class Manager;
    */
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