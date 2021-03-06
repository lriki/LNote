//=============================================================================
//【 LJoypad 】
//-----------------------------------------------------------------------------
///**
//  @file       LJoypad.h
//  @brief      LJoypad
//  @version    1.0
//  @date       2011/08/23
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Core/Input/Joypad.h"
#include "../Math/LMath.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
#if 0
//=============================================================================
// ■ LJoypad クラス
//-----------------------------------------------------------------------------
///**
//  @brief      Input::Joypad の参照クラス
//
//  @par
//              このクラスのインスタンスにアクセスるためには、
//              LInput::getJoypad() によってインスタンスを取得する必要があります。
//
//  @attention
//              ジョイパッドのボタン番号は 1 スタートです。0 は無効を表します
//*/
//=============================================================================
class LJoypad
    : public LRefClass< LJoypad, Input::Joypad >
{
public:
	
	//---------------------------------------------------------------------
    ///**
    //  @brief      ボタンが現在押されているかを判定する
    //
    //  @param[in]  button_ : ボタン番号 ( 1 〜 16 )
    //*/
    //---------------------------------------------------------------------
	bool isPress( int button_ ) const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ボタンが新しく押された瞬間を判定する
    //
    //  @param[in]  button_ : ボタン番号 ( 1 〜 16 )
    //*/
    //---------------------------------------------------------------------
	bool isOnTrigger( int button_ ) const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ボタンが離された瞬間を判定する
    //
    //  @param[in]  button_ : ボタン番号 ( 1 〜 16 )
    //*/
    //---------------------------------------------------------------------
	bool isOffTrigger( int button_ ) const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ボタンが新しく押された瞬間とリピート状態を判定する
    //
    //  @param[in]  button_ : ボタン番号 ( 1 〜 16 )
    //*/
    //---------------------------------------------------------------------
	bool isRepeat( int button_ ) const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      軸の状態の取得
    //
    //  @param[in]  button_ : ボタン番号 ( 0 〜 3 )
    //*/
    //---------------------------------------------------------------------
    LVector2 getAxisState( int axis_ ) const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      POV の状態の取得
    //*/
    //---------------------------------------------------------------------
	int getPOVState() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      軸の値の正規化の有効判定
    //*/
    //---------------------------------------------------------------------
	bool isEnableNormalize() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      軸の値の正規化の有効設定
    //
    //  @param[in]  flag_ : true の場合、getAxisState() で取得する値を正規化するようにする
    //
    //  @par
    //              通常、軸の値の最大値は ( 1.0, 1.0 ) で、これの長さは √2 です。
    //              入力した方向にかかわらず、常に長さを 1 にしたい場合に使います。<br><br>
    //              
    //              この値はデフォルトで true です。
    //*/
    //---------------------------------------------------------------------
	void setEnableNormalize( bool flag_ );

    LN_UTIL_REFCLASS_METHODS( LJoypad, Input::Joypad );
};
#endif
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================