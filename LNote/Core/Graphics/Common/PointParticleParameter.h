//=============================================================================
//【 PointParticleParameter 】
//-----------------------------------------------------------------------------
///**
//  @file       PointParticleParameter.h
//  @brief      PointParticleParameter
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Geometry/Vector3.h"
#include "../../Math/Random.h"
#include "../../Math/Math.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

//=============================================================================
// ■ PointParticleParameter クラス
//-----------------------------------------------------------------------------
///**
//  @brief		パーティクルの作成情報
//*/
//=============================================================================
class PointParticleParameter
{
public:

	u32				ParticleNum;		///< 作成する粒子の数
    lnfloat         GenerationInterval; ///< 粒子を作成する間隔 (次の生成までに待つフレーム数。1.0 より小さい場合、複数個生成する。0 で一度に全て作る)
    lnfloat         TimeScale;          ///< 再生時間のスケール値 (通常、再生したいフレームレートを渡す)
    int             RandSeed;           ///< 乱数シード (0 の場合は LRandom に設定したデフォルトのシードを使う)


	LVector3		Position;			///< 初期位置
	LVector3		PositionRand;       ///< 初期位置のランダム幅

	LVector3		Velocity;			///< 初速度
	LVector3		VelocityRand;       ///< 初速度のランダム幅

	LVector3		Accel;				///< 加速度
	LVector3		AccelRand;          ///< 加速度のランダム幅

	LVector3		Axis;				///< 回転軸
	LVector3		AxisRand;           ///< 回転軸のランダム幅

	lnfloat			Angle;				///< 初期角度 (Axis 軸周りの回転)
	lnfloat			AngleRand;          ///< 初期角度のランダム幅

	lnfloat			AngleVelocity;	    ///< 角速度 (Axis 軸周りの回転)
	lnfloat			AngleVelocityRand;  ///< 初期角度のランダム幅

	lnfloat			AngleAccel;			///< 角加速度
	lnfloat			AngleAccelRand;     ///< 角加速度のランダム幅

	lnfloat			Size;				///< サイズ
	lnfloat			SizeRand;           ///< サイズのランダム幅

	lnfloat			SizeVelocity;	    ///< サイズ変化速度
	lnfloat			SizeVelocityRand;   ///< サイズ変化速度のランダム幅

	lnfloat			SizeAccel;			///< サイズ変化加速度
	lnfloat			SizeAccelRand;      ///< サイズ変化加速度のランダム幅

	lnfloat			Life;				///< 粒子ひとつが存在するフレーム数または時間
	lnfloat			LifeRand;           ///< 粒子ひとつが存在するフレーム数のランダム幅

	lnfloat			FadeIn;				///< 粒子ひとつのフェードインフレーム数または時間
	lnfloat			FadeInRand;         ///< 粒子ひとつのフェードインフレーム数のランダム幅

	lnfloat			FadeOut;			///< 粒子ひとつのフェードアウトフレーム数または時間
	lnfloat			FadeOutRand;        ///< 粒子ひとつのフェードアウトフレーム数のランダム幅


	u8				PosRandTypeX;		///< 初期位置ランダムタイプ (以下、LNExRandRateType の値)
	u8				PosRandTypeY;
	u8				PosRandTypeZ;
	u8				VelRandTypeX;		///< 初速度のランダムタイプ
	u8				VelRandTypeY;
	u8				VelRandTypeZ;
	u8				AccelRandTypeX;		///< 加速度のランダムタイプ
	u8				AccelRandTypeY;
	u8				AccelRandTypeZ;
	u8				AxisRandTypeX;		///< 回転軸のランダムタイプ
	u8				AxisRandTypeY;
	u8				AxisRandTypeZ;
	u8				AngleRandType;		///< 初期角度のランダムタイプ
	u8				AngVelRandType;		///< 角速度のランダムタイプ
	u8				AngAccelRandType;	///< 角加速度のランダムタイプ
	u8				SizeRandType;		///< サイズのランダムタイプ
	u8				SizeVelRandType;	///< サイズ速度のランダムタイプ
	u8				SizeAccelRandType;	///< サイズ加速度のランダムタイプ
	u8				LifeRandType;		///< 存在フレーム数のランダムタイプ
	u8				FadeInRandType;		///< フェードインフレーム数のランダムタイプ
	u8				FadeOutRandType;	///< フェードアウトフレーム数のランダムタイプ


	LVector3		GlobalAccel;		///< 全体に適用する加速度 (重力など)
	lnfloat			GravityPower;		///< 中心 (ローカル座標の 0, 0, 0) への引力の強さ (負の値で斥力になる)

	u32				AfterImageNum;		///< 残像を残すフレーム数

	bool		    Loop;				///< ループ再生する場合は true
    bool            Dynamic;
	bool		    IsDegree;	        ///< true の場合、角度の単位を度数法(1周360度)として扱う

	/// 初期値
	PointParticleParameter()
        : ParticleNum       ( 32 )
        , GenerationInterval( 0 )
        , TimeScale         ( 60.0f )
        , RandSeed          ( 0 )

        , Position          ( 0, 0, 0 )
        , PositionRand      ( 0, 0, 0 )
        , Velocity          ( 0, 0, 0 )
        , VelocityRand      ( 0, 0, 0 )
        , Accel             ( 0, 0, 0 )
        , AccelRand         ( 0, 0, 0 )
        , Axis              ( 0, 1, 0 )
        , AxisRand          ( 0, 0, 0 )
        , Angle             ( 0 )
        , AngleRand         ( 0 )
        , AngleVelocity     ( 0 )
        , AngleVelocityRand ( 0 )
        , AngleAccel        ( 0 )
        , AngleAccelRand    ( 0 )
        , Size              ( 1.0f )
        , SizeRand          ( 0 )
        , SizeVelocity      ( 0 )
        , SizeVelocityRand  ( 0 )
        , SizeAccel         ( 0 )
        , SizeAccelRand     ( 0 )

        , Life              ( 120 )
        , LifeRand          ( 0 )
        , FadeIn            ( 20 )
        , FadeInRand        ( 0 )
        , FadeOut           ( 20 )
        , FadeOutRand       ( 0 )

        , PosRandTypeX      ( LN_RANDRATE_NONE )
        , PosRandTypeY      ( LN_RANDRATE_NONE )
        , PosRandTypeZ      ( LN_RANDRATE_NONE )
        , VelRandTypeX      ( LN_RANDRATE_NONE )
        , VelRandTypeY      ( LN_RANDRATE_NONE )
        , VelRandTypeZ      ( LN_RANDRATE_NONE )
        , AccelRandTypeX    ( LN_RANDRATE_NONE )
        , AccelRandTypeY    ( LN_RANDRATE_NONE )
        , AccelRandTypeZ    ( LN_RANDRATE_NONE )
        , AxisRandTypeX     ( LN_RANDRATE_NONE )
        , AxisRandTypeY     ( LN_RANDRATE_NONE )
        , AxisRandTypeZ     ( LN_RANDRATE_NONE )
        , AngleRandType     ( LN_RANDRATE_NONE )
        , AngVelRandType    ( LN_RANDRATE_NONE )
        , AngAccelRandType  ( LN_RANDRATE_NONE )
        , SizeRandType      ( LN_RANDRATE_NONE )
        , SizeVelRandType   ( LN_RANDRATE_NONE )
        , SizeAccelRandType ( LN_RANDRATE_NONE )
        , LifeRandType      ( LN_RANDRATE_NONE )
        , FadeInRandType    ( LN_RANDRATE_NONE )
        , FadeOutRandType   ( LN_RANDRATE_NONE )
        , GlobalAccel       ( 0, 0, 0 )
        , GravityPower      ( 0 )
        , AfterImageNum     ( 0 )
        , Loop              ( false )
        , Dynamic           ( false )
        , IsDegree          ( false )
	{}


	/// PosRandTypeX PosRandTypeY PosRandTypeZ に値を設定する
	void setPositionRandType( LNExRandRateType type_ )
	{
		PosRandTypeX = PosRandTypeY = PosRandTypeZ = (u8)type_;
	}

	/// VelRandTypeX VelRandTypeY VelRandTypeZ に値を設定する
	void setVelRandType( LNExRandRateType type_ )
	{
		VelRandTypeX = VelRandTypeY = VelRandTypeZ = (u8)type_;
	}

	/// AccelRandTypeX AccelRandTypeY AccelRandTypeZ に値を設定する
	void setAccelRandType( LNExRandRateType type_ )
	{
		AccelRandTypeX = AccelRandTypeY = AccelRandTypeZ = (u8)type_;
	}

	/// AxisRandTypeX AxisRandTypeY AxisRandTypeZ に値を設定する
	void setAxisRandType( LNExRandRateType type_ )
	{
		AxisRandTypeX = AxisRandTypeY = AxisRandTypeZ = (u8)type_;
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      角度をラジアン角に変換する
    //
    //  @note
    //              エフェクトエディタから値を設定する時に等使う。
    //              この関数はメンバ変数の IsDegree とか全く関係なしに
    //              データを操作するので、既にラジアン角の角度があると
    //              値がおかしくなる点に注意。
    //              <br>
    //              IsDegree = true の状態でパーティクルを作成すると、
    //              内部でこの関数が呼ばれて変換される。
    //              そのため、通常は呼ぶ必要ナシ。
    //*/
	//---------------------------------------------------------------------
	void convDegToRad()
	{
        Angle = LMath::DegToRad( Angle );
		AngleRand = LMath::DegToRad( AngleRand );
		AngleVelocity = LMath::DegToRad( AngleVelocity );
		AngleVelocityRand = LMath::DegToRad( AngleVelocityRand );
		AngleAccel = LMath::DegToRad( AngleAccel );
		AngleAccelRand = LMath::DegToRad( AngleAccelRand );
	}



	// ▼ 以下は最適化前の並び。サイズは 368 バイト。↑の並びにするとサイズは 308 バイトになる。

	/*
	u32				ParticleNum;		//!< 作成する粒子の数

	LVector3		Position;			//!< 初期位置
	LVector3		PosRandMin;
	LVector3		PosRandMax;
	u32				PosRandTypeX;
	u32				PosRandTypeY;
	u32				PosRandTypeZ;

	LVector3		Velocity;			//!< 初速度
	LVector3		VelRandMin;
	LVector3		VelRandMax;
	u32				VelRandTypeX;
	u32				VelRandTypeY;
	u32				VelRandTypeZ;

	LVector3		Accel;				//!< 加速度
	LVector3		AccelRandMin;
	LVector3		AccelRandMax;
	u32				AccelRandTypeX;
	u32				AccelRandTypeY;
	u32				AccelRandTypeZ;

	LVector3		Axis;				//!< 回転軸
	LVector3		AxisRandMin;
	LVector3		AxisRandMax;
	u32				AxisRandTypeX;
	u32				AxisRandTypeY;
	u32				AxisRandTypeZ;

	lfloat			Angle;				//!< 初期角度
	lfloat			AngleRandMin;
	lfloat			AngleRandMax;
	u32				AngleRandType;

	lfloat			AngVel;				//!< 角速度
	lfloat			AngVelRandMin;
	lfloat			AngVelRandMax;
	u32				AngVelRandType;

	lfloat			AngAccel;			//!< 角加速度
	lfloat			AngAccelRandMin;
	lfloat			AngAccelRandMax;
	u32				AngAccelRandType;

	lfloat			Size;				//!< サイズ
	lfloat			SizeRandMin;
	lfloat			SizeRandMax;
	u32				SizeRandType;

	lfloat			SizeVel;			//!< サイズ速度
	lfloat			SizeVelRandMin;
	lfloat			SizeVelRandMax;
	u32				SizeVelRandType;

	lfloat			SizeAccel;			//!< サイズ加速度
	lfloat			SizeAccelRandMin;
	lfloat			SizeAccelRandMax;
	u32				SizeAccelRandType;

	long			Life;				//!< 存在するフレーム数
	long			LifeRandMin;
	long			LifeRandMax;
	u32				LifeRandType;

	long			FadeIn;				//!< フェードインにかけるフレーム数
	long			FadeInRandMin;
	long			FadeInRandMax;
	u32				FadeInRandType;

	long			FadeOut;			//!< フェードアウトにかけるフレーム数
	long			FadeOutRandMin;
	long			FadeOutRandMax;
	u32				FadeOutRandType;
	
	LVector3		GlobalAccel;		//!< 全体に適用する加速度 ( 重力とか )
	lfloat			GravityPower;		//!< 中心 ( ローカル座標の 0, 0, 0 ) への引力の強さ ( 負の値で斥力になる )

	u32				NumAtOneFrame;		//!< 1フレームに作る粒子の数 ( 一度に全部作るときとかは 0 )
	u32				AfterImageNum;		//!< 残像を残すフレーム数

	bool			Loop;				//!< 全体をループ再生するフラグ ( ParticleLoop=true のときは基本的に false にしておくこと )
	bool			ParticleLoop;		//!< パーティクル単位でループ再生するフラグ
	bool			Sequential;			//!< 1フレームずつ新しい粒子を作るフラグ
	*/

	/*
		時間は基本的に
			Life >= FadeIn + FadeOut
		で指定する。
		ランダムの適用や設定ミスなどで↑の式が満たせなくなった場合は
		オーバーした分だけ Life が増える。

		Loop が false の場合、全体の再生フレーム数は
			生成したフレーム位置 + Life
		が一番大きい粒子のフレーム数になる。

		Sequential が false の場合、最初に全ての粒子を作成する。
		true の場合は何もない状態から徐々に粒子が増えていくが、
		false の場合は最初から粒子が存在している。
		例えば背景オブジェクトとなる松明の炎等、途切れてほしくないエフェクトに使う。
	*/

	/*
	LParticleData() :
	ParticleNum( 8 ),
	PosRandTypeX( RAND_NORMAL ),
	PosRandTypeY( RAND_NORMAL ),
	PosRandTypeZ( RAND_NORMAL ),

	VelRandTypeX( RAND_NORMAL ),
	VelRandTypeY( RAND_NORMAL ),
	VelRandTypeZ( RAND_NORMAL ),

	AccelRandTypeX( RAND_NORMAL ),
	AccelRandTypeY( RAND_NORMAL ),
	AccelRandTypeZ( RAND_NORMAL ),

	AxisRandTypeX( RAND_NORMAL ),
	AxisRandTypeY( RAND_NORMAL ),
	AxisRandTypeZ( RAND_NORMAL ),

	Angle( 0 ),
	AngleRandMin( 0 ),
	AngleRandMax( 0 ),
	AngleRandType( RAND_NORMAL ),

	AngVel( 0 ),
	AngVelRandMin( 0 ),
	AngVelRandMax( 0 ),
	AngVelRandType( RAND_NORMAL ),

	AngAccel( 0 ),
	AngAccelRandMin( 0 ),
	AngAccelRandMax( 0 ),
	AngAccelRandType( RAND_NORMAL ),

	Size( 1 ),
	SizeRandMin( 0 ),
	SizeRandMax( 0 ),
	SizeRandType( RAND_NORMAL ),

	SizeVel( 0 ),
	SizeVelRandMin( 0 ),
	SizeVelRandMax( 0 ),
	SizeVelRandType( RAND_NORMAL ),

	SizeAccel( 0 ),
	SizeAccelRandMin( 0 ),
	SizeAccelRandMax( 0 ),
	SizeAccelRandType( RAND_NORMAL ),

	Life( 60 ),
	LifeRandMin( 0 ),
	LifeRandMax( 0 ),
	LifeRandType( RAND_NORMAL ),

	FadeIn( 20 ),
	FadeInRandMin( 0 ),
	FadeInRandMax( 0 ),
	FadeInRandType( RAND_NORMAL ),

	FadeOut( 20 ),
	FadeOutRandMin( 0 ),
	FadeOutRandMax( 0 ),
	FadeOutRandType( RAND_NORMAL ),

	GravityPower( 0 ),
	NumAtOneFrame( 0 ),
	AfterImageNum( 0 ),
	Loop( false ),
	ParticleLoop( false ),
	Sequential( false )
	{}

	*/
    
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================