//=============================================================================
//【 GameTime 】
//-----------------------------------------------------------------------------
/**
//  @file       GameTime.h
//  @brief      GameTime
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Game
{

//=============================================================================
// ■ GameTime クラス
//-----------------------------------------------------------------------------
///**
//  @brief      ゲーム内での時間を扱うクラス
//*/
//=============================================================================
class GameTime
{
public:

	static const GameTime ZERO_TIME;

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      現在の FPS 値の取得
    //*/
    //---------------------------------------------------------------------
	float getFPS() const { return _m.FPS; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      前回の更新から経過したゲーム時間の取得
    //*/
    //---------------------------------------------------------------------
	float getElapsedGameTime() const { return _m.ElapsedGameTime; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      前回の更新から経過した実時間の取得
    //*/
    //---------------------------------------------------------------------
	float getElapsedRealTime() const { return _m.ElapsedRealTime; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      ゲームの開始以降のゲーム時間の取得
    //*/
    //---------------------------------------------------------------------
	double getTotalGameTime() const { return _m.TotalGameTime; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      ゲームの開始以降の実時間の取得
    //*/
    //---------------------------------------------------------------------
	double getTotalRealTime() const { return _m.TotalRealTime; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      ゲームの開始以降の経過フレーム数の取得
    //*/
    //---------------------------------------------------------------------
	lnU32 getTotalFrameCount() const { return _m.TotalFrameCount; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      ゲームループの時間がフレームレートを超えているかを判定する
    //*/
    //---------------------------------------------------------------------
	bool isRunningSlowly() const { return _m.IsRunningSlowly; }

public:

    /// コンストラクタ
    GameTime()
    {
        _m.FPS = 0;
        _m.ElapsedGameTime = 0;
        _m.ElapsedRealTime = 0;
        _m.TotalGameTime   = 0;
        _m.TotalRealTime   = 0;
        _m.IsRunningSlowly = false;
    }

private:

    struct
    {
        float   FPS;
        float   ElapsedGameTime;
        float   ElapsedRealTime;
        double  TotalGameTime;
        double  TotalRealTime;
        lnU32   TotalFrameCount;
        bool    IsRunningSlowly;
    } _m;

    friend class FPSController;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Game
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================