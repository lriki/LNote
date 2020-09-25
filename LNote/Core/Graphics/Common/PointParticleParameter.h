//=============================================================================
//�y PointParticleParameter �z
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
// �� PointParticleParameter �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�p�[�e�B�N���̍쐬���
//*/
//=============================================================================
class PointParticleParameter
{
public:

	u32				ParticleNum;		///< �쐬���闱�q�̐�
    lnfloat         GenerationInterval; ///< ���q���쐬����Ԋu (���̐����܂łɑ҂t���[�����B1.0 ��菬�����ꍇ�A������������B0 �ň�x�ɑS�č��)
    lnfloat         TimeScale;          ///< �Đ����Ԃ̃X�P�[���l (�ʏ�A�Đ��������t���[�����[�g��n��)
    int             RandSeed;           ///< �����V�[�h (0 �̏ꍇ�� LRandom �ɐݒ肵���f�t�H���g�̃V�[�h���g��)


	LVector3		Position;			///< �����ʒu
	LVector3		PositionRand;       ///< �����ʒu�̃����_����

	LVector3		Velocity;			///< �����x
	LVector3		VelocityRand;       ///< �����x�̃����_����

	LVector3		Accel;				///< �����x
	LVector3		AccelRand;          ///< �����x�̃����_����

	LVector3		Axis;				///< ��]��
	LVector3		AxisRand;           ///< ��]���̃����_����

	lnfloat			Angle;				///< �����p�x (Axis ������̉�])
	lnfloat			AngleRand;          ///< �����p�x�̃����_����

	lnfloat			AngleVelocity;	    ///< �p���x (Axis ������̉�])
	lnfloat			AngleVelocityRand;  ///< �����p�x�̃����_����

	lnfloat			AngleAccel;			///< �p�����x
	lnfloat			AngleAccelRand;     ///< �p�����x�̃����_����

	lnfloat			Size;				///< �T�C�Y
	lnfloat			SizeRand;           ///< �T�C�Y�̃����_����

	lnfloat			SizeVelocity;	    ///< �T�C�Y�ω����x
	lnfloat			SizeVelocityRand;   ///< �T�C�Y�ω����x�̃����_����

	lnfloat			SizeAccel;			///< �T�C�Y�ω������x
	lnfloat			SizeAccelRand;      ///< �T�C�Y�ω������x�̃����_����

	lnfloat			Life;				///< ���q�ЂƂ����݂���t���[�����܂��͎���
	lnfloat			LifeRand;           ///< ���q�ЂƂ����݂���t���[�����̃����_����

	lnfloat			FadeIn;				///< ���q�ЂƂ̃t�F�[�h�C���t���[�����܂��͎���
	lnfloat			FadeInRand;         ///< ���q�ЂƂ̃t�F�[�h�C���t���[�����̃����_����

	lnfloat			FadeOut;			///< ���q�ЂƂ̃t�F�[�h�A�E�g�t���[�����܂��͎���
	lnfloat			FadeOutRand;        ///< ���q�ЂƂ̃t�F�[�h�A�E�g�t���[�����̃����_����


	u8				PosRandTypeX;		///< �����ʒu�����_���^�C�v (�ȉ��ALNExRandRateType �̒l)
	u8				PosRandTypeY;
	u8				PosRandTypeZ;
	u8				VelRandTypeX;		///< �����x�̃����_���^�C�v
	u8				VelRandTypeY;
	u8				VelRandTypeZ;
	u8				AccelRandTypeX;		///< �����x�̃����_���^�C�v
	u8				AccelRandTypeY;
	u8				AccelRandTypeZ;
	u8				AxisRandTypeX;		///< ��]���̃����_���^�C�v
	u8				AxisRandTypeY;
	u8				AxisRandTypeZ;
	u8				AngleRandType;		///< �����p�x�̃����_���^�C�v
	u8				AngVelRandType;		///< �p���x�̃����_���^�C�v
	u8				AngAccelRandType;	///< �p�����x�̃����_���^�C�v
	u8				SizeRandType;		///< �T�C�Y�̃����_���^�C�v
	u8				SizeVelRandType;	///< �T�C�Y���x�̃����_���^�C�v
	u8				SizeAccelRandType;	///< �T�C�Y�����x�̃����_���^�C�v
	u8				LifeRandType;		///< ���݃t���[�����̃����_���^�C�v
	u8				FadeInRandType;		///< �t�F�[�h�C���t���[�����̃����_���^�C�v
	u8				FadeOutRandType;	///< �t�F�[�h�A�E�g�t���[�����̃����_���^�C�v


	LVector3		GlobalAccel;		///< �S�̂ɓK�p��������x (�d�͂Ȃ�)
	lnfloat			GravityPower;		///< ���S (���[�J�����W�� 0, 0, 0) �ւ̈��͂̋��� (���̒l�Ő˗͂ɂȂ�)

	u32				AfterImageNum;		///< �c�����c���t���[����

	bool		    Loop;				///< ���[�v�Đ�����ꍇ�� true
    bool            Dynamic;
	bool		    IsDegree;	        ///< true �̏ꍇ�A�p�x�̒P�ʂ�x���@(1��360�x)�Ƃ��Ĉ���

	/// �����l
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


	/// PosRandTypeX PosRandTypeY PosRandTypeZ �ɒl��ݒ肷��
	void setPositionRandType( LNExRandRateType type_ )
	{
		PosRandTypeX = PosRandTypeY = PosRandTypeZ = (u8)type_;
	}

	/// VelRandTypeX VelRandTypeY VelRandTypeZ �ɒl��ݒ肷��
	void setVelRandType( LNExRandRateType type_ )
	{
		VelRandTypeX = VelRandTypeY = VelRandTypeZ = (u8)type_;
	}

	/// AccelRandTypeX AccelRandTypeY AccelRandTypeZ �ɒl��ݒ肷��
	void setAccelRandType( LNExRandRateType type_ )
	{
		AccelRandTypeX = AccelRandTypeY = AccelRandTypeZ = (u8)type_;
	}

	/// AxisRandTypeX AxisRandTypeY AxisRandTypeZ �ɒl��ݒ肷��
	void setAxisRandType( LNExRandRateType type_ )
	{
		AxisRandTypeX = AxisRandTypeY = AxisRandTypeZ = (u8)type_;
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      �p�x�����W�A���p�ɕϊ�����
    //
    //  @note
    //              �G�t�F�N�g�G�f�B�^����l��ݒ肷�鎞�ɓ��g���B
    //              ���̊֐��̓����o�ϐ��� IsDegree �Ƃ��S���֌W�Ȃ���
    //              �f�[�^�𑀍삷��̂ŁA���Ƀ��W�A���p�̊p�x�������
    //              �l�����������Ȃ�_�ɒ��ӁB
    //              <br>
    //              IsDegree = true �̏�ԂŃp�[�e�B�N�����쐬����ƁA
    //              �����ł��̊֐����Ă΂�ĕϊ������B
    //              ���̂��߁A�ʏ�͌ĂԕK�v�i�V�B
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



	// �� �ȉ��͍œK���O�̕��сB�T�C�Y�� 368 �o�C�g�B���̕��тɂ���ƃT�C�Y�� 308 �o�C�g�ɂȂ�B

	/*
	u32				ParticleNum;		//!< �쐬���闱�q�̐�

	LVector3		Position;			//!< �����ʒu
	LVector3		PosRandMin;
	LVector3		PosRandMax;
	u32				PosRandTypeX;
	u32				PosRandTypeY;
	u32				PosRandTypeZ;

	LVector3		Velocity;			//!< �����x
	LVector3		VelRandMin;
	LVector3		VelRandMax;
	u32				VelRandTypeX;
	u32				VelRandTypeY;
	u32				VelRandTypeZ;

	LVector3		Accel;				//!< �����x
	LVector3		AccelRandMin;
	LVector3		AccelRandMax;
	u32				AccelRandTypeX;
	u32				AccelRandTypeY;
	u32				AccelRandTypeZ;

	LVector3		Axis;				//!< ��]��
	LVector3		AxisRandMin;
	LVector3		AxisRandMax;
	u32				AxisRandTypeX;
	u32				AxisRandTypeY;
	u32				AxisRandTypeZ;

	lfloat			Angle;				//!< �����p�x
	lfloat			AngleRandMin;
	lfloat			AngleRandMax;
	u32				AngleRandType;

	lfloat			AngVel;				//!< �p���x
	lfloat			AngVelRandMin;
	lfloat			AngVelRandMax;
	u32				AngVelRandType;

	lfloat			AngAccel;			//!< �p�����x
	lfloat			AngAccelRandMin;
	lfloat			AngAccelRandMax;
	u32				AngAccelRandType;

	lfloat			Size;				//!< �T�C�Y
	lfloat			SizeRandMin;
	lfloat			SizeRandMax;
	u32				SizeRandType;

	lfloat			SizeVel;			//!< �T�C�Y���x
	lfloat			SizeVelRandMin;
	lfloat			SizeVelRandMax;
	u32				SizeVelRandType;

	lfloat			SizeAccel;			//!< �T�C�Y�����x
	lfloat			SizeAccelRandMin;
	lfloat			SizeAccelRandMax;
	u32				SizeAccelRandType;

	long			Life;				//!< ���݂���t���[����
	long			LifeRandMin;
	long			LifeRandMax;
	u32				LifeRandType;

	long			FadeIn;				//!< �t�F�[�h�C���ɂ�����t���[����
	long			FadeInRandMin;
	long			FadeInRandMax;
	u32				FadeInRandType;

	long			FadeOut;			//!< �t�F�[�h�A�E�g�ɂ�����t���[����
	long			FadeOutRandMin;
	long			FadeOutRandMax;
	u32				FadeOutRandType;
	
	LVector3		GlobalAccel;		//!< �S�̂ɓK�p��������x ( �d�͂Ƃ� )
	lfloat			GravityPower;		//!< ���S ( ���[�J�����W�� 0, 0, 0 ) �ւ̈��͂̋��� ( ���̒l�Ő˗͂ɂȂ� )

	u32				NumAtOneFrame;		//!< 1�t���[���ɍ�闱�q�̐� ( ��x�ɑS�����Ƃ��Ƃ��� 0 )
	u32				AfterImageNum;		//!< �c�����c���t���[����

	bool			Loop;				//!< �S�̂����[�v�Đ�����t���O ( ParticleLoop=true �̂Ƃ��͊�{�I�� false �ɂ��Ă������� )
	bool			ParticleLoop;		//!< �p�[�e�B�N���P�ʂŃ��[�v�Đ�����t���O
	bool			Sequential;			//!< 1�t���[�����V�������q�����t���O
	*/

	/*
		���Ԃ͊�{�I��
			Life >= FadeIn + FadeOut
		�Ŏw�肷��B
		�����_���̓K�p��ݒ�~�X�ȂǂŁ��̎����������Ȃ��Ȃ����ꍇ��
		�I�[�o�[���������� Life ��������B

		Loop �� false �̏ꍇ�A�S�̂̍Đ��t���[������
			���������t���[���ʒu + Life
		����ԑ傫�����q�̃t���[�����ɂȂ�B

		Sequential �� false �̏ꍇ�A�ŏ��ɑS�Ă̗��q���쐬����B
		true �̏ꍇ�͉����Ȃ���Ԃ��珙�X�ɗ��q�������Ă������A
		false �̏ꍇ�͍ŏ����痱�q�����݂��Ă���B
		�Ⴆ�Δw�i�I�u�W�F�N�g�ƂȂ鏼���̉����A�r�؂�Ăق����Ȃ��G�t�F�N�g�Ɏg���B
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