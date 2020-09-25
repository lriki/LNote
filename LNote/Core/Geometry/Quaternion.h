//=============================================================================
//�y Quaternion �z
//-----------------------------------------------------------------------------
///**
//  @file       Quaternion.h
//  @brief      Quaternion
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

#include "Vector3.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Geometry
{

//=============================================================================
// �� Quaternion �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �l�����̃N���X
//*/
//=============================================================================
class Quaternion
{
public:

	lnfloat x, y, z, w;

public:

    static const Quaternion    IDENTITY;       ///< ( 0, 0, 0, 1 )

public:

	/// �f�t�H���g�R���X�g���N�^ (IDENTITY �ŏ����� )
	Quaternion();

	/// �R�s�[�R���X�g���N�^
	Quaternion( const Quaternion& vec_ );

	/// �R���X�g���N�^
	Quaternion( lnfloat x_, lnfloat y_, lnfloat z_, lnfloat w_ );

	//---------------------------------------------------------------------
	///**
    //	@brief      �R���X�g���N�^ ( ���Ɖ�]�p�x����쐬 )
    //
    //  @par
    //              �^����ꂽ�������K������Ă��Ȃ��ꍇ�͐��K���������̂��g���Čv�Z���܂��B
    //*/
	//---------------------------------------------------------------------
	Quaternion( const Vector3& axis_, lnfloat r_ );

    //---------------------------------------------------------------------
	///**
    //	@brief      �R���X�g���N�^ ( �I�C���[�p�x����쐬�E���W�A���P�� )
    //
    //  @par
    //              ��]�����͍��̂Ƃ��� X �� Y �� Z �����ł��B
    //*/
	//---------------------------------------------------------------------
	Quaternion( const Vector3& angle_ );

public:

	/// �l�̐ݒ�
	void set( lnfloat x_, lnfloat y_, lnfloat z_, lnfloat w_ );

	//---------------------------------------------------------------------
	///**
    //	@brief      �����̎擾
    //*/
	//---------------------------------------------------------------------
	//lnfloat getLength() const;

	//---------------------------------------------------------------------
	///**
	//	@brief		3D �x�N�g���̒�����2���Ԃ�
	//*/
	//---------------------------------------------------------------------
	//lnfloat getSquareLength() const;

	//---------------------------------------------------------------------
	///**
	//	@brief		���K������
	//*/
	//---------------------------------------------------------------------
	void normalize();

	//---------------------------------------------------------------------
	///**
	//	@brief		������ 0 ( ���ׂĂ̗v�f�� 0 ) ���𔻒肷��
	//*/
	//---------------------------------------------------------------------
	bool isZero() const { return ( x == 0 && y == 0 && z == 0 && w == 0 ); }

public:

    Quaternion& operator = ( const Quaternion& q_ );

	Quaternion operator + ( const Quaternion& q_ ) const;
	Quaternion operator - ( const Quaternion& q_ ) const;
	Quaternion operator * ( const Quaternion& q_ ) const;
	Quaternion operator * ( const lnfloat a_ ) const;
	Quaternion operator / ( const lnfloat a_ ) const;



/*
	Vector3& operator = ( const lnfloat a_ );
	Vector3& operator = ( const Vector3& vec_ );
	void operator += ( const lnfloat a_ );
	void operator += ( const Vector3& vec_ );
	void operator -= ( const lnfloat a_ );
	void operator -= ( const Vector3& vec_ );
	void operator *= ( const lnfloat a_ );
	void operator *= ( const Vector3& vec_ );
	void operator /= ( const lnfloat a_ );
	void operator /= ( const Vector3& vec_ );
	
	bool operator == ( const Vector3& vec_ ) const;
	bool operator != ( const Vector3& vec_ ) const;
	*/

	/// �W���o�͂֏o�͂���
    void cdump( const lnChar* str_ = NULL ) const
    {
        _tprintf( "%s ( %f, %f, %f, %f )",
            ( str_ ) ? str_ : _T( "Quaternion" ),
            x, y, z, w );
    }

public:

    //-------------------------------------------------------------------------
	// �� static

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���K�������N�H�[�^�j�I����Ԃ�
    //
    //  @param[out] out_ : ���Z���ʂ��i�[���� Quaternion �ւ̃|�C���^
    //  @param[in]  qua_ : �����̊�ɂȂ� Quaternion
    //*/
    //---------------------------------------------------------------------
    static void normalize( Quaternion* out_, const Quaternion& qua_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �N�H�[�^�j�I���̋�����Ԃ�
    //
    //  @param[out] out_ : ���Z���ʂ��i�[���� Quaternion �ւ̃|�C���^
    //  @param[in]  qua_ : �����̊�ɂȂ� Quaternion
    //*/
    //---------------------------------------------------------------------
    static void conjugate( Quaternion* out_, const Quaternion& qua_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �C�ӂ̎�����]���ɂ��ĉ�]����N�H�[�^�j�I�����쐬���� ( ���W�A���p�x )
    //
    //  @param[out] out_      : ���Z���ʂ��i�[���� Quaternion �ւ̃|�C���^
    //  @param[in]  axis_     : ��]��
    //  @param[in]  r_        : ��]�p�x
    //
    //  @par
    //              �^����ꂽ�������K������Ă��Ȃ��ꍇ�͐��K���������̂��g���Čv�Z���܂��B
    //*/
    //---------------------------------------------------------------------
	static void rotationAxis( Quaternion* out_, const Vector3& axis_, lnfloat r_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      ��]�s�񂩂�N�H�[�^�j�I�����쐬����
    //
    //  @param[out] out_ : ���Z���ʂ��i�[���� Quaternion �ւ̃|�C���^
    //  @param[in]  mat_ : �����̊�ɂȂ� Matrix
    //*/
	//---------------------------------------------------------------------
    static void rotationMatrix( Quaternion* out_, const Matrix& mat_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �C�ӂ̎�����]���ɂ��ĉ�]����N�H�[�^�j�I�����쐬���� ( �C���f�b�N�X�p�x )
    //
    //  @param[out] out_      : ���Z���ʂ��i�[���� Quaternion �ւ̃|�C���^
    //  @param[in]  axis_     : ��]��
    //  @param[in]  r_        : ��]�p�x
    //
    //  @par
    //              �^����ꂽ�������K������Ă��Ȃ��ꍇ�͐��K���������̂��g���Čv�Z���܂��B
    //*/
    //---------------------------------------------------------------------
	static void rotationAxisIdx( Quaternion* out_, const Vector3& axis_, int r_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      2 �̃N�H�[�^�j�I���̐ς��v�Z����
    //
    //  @param[out] out_  : ���Z���ʂ��i�[���� Quaternion �ւ̃|�C���^
    //  @param[in]  qua1_ : �����̊�ɂȂ� Quaternion
    //  @param[in]  qua2_ : �����̊�ɂȂ� Quaternion
    //
    //  @attention
    //              Direct3D �� D3DXQUATERNION �� D3DXQuaternionMultiply() �̌v�Z�����Ƃ͋t�ł��B<br>
    //              Direct3D �͍s��̏�Z�Ɠ����悤�ȏ��Ԃŏ�Z�ł���悤�ɂ��邽�߁A
    //              ���ʂƂ͈Ⴄ���@���Ƃ��Ă���Ƃ̂��ƁB<br><br>
    //              
    //              ���Ȃ݂� Quaternion �� operator * �̏��Ԃ͂��̊֐��Ɠ����ł��B
    //*/
    //---------------------------------------------------------------------
    static void multiply( Quaternion* out_, const Quaternion& q1_, const Quaternion& q2_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      2 �̃N�H�[�^�j�I���̐ς��v�Z���� ( �t�� )
    //
    //  @param[out] out_  : ���Z���ʂ��i�[���� Quaternion �ւ̃|�C���^
    //  @param[in]  qua1_ : �����̊�ɂȂ� Quaternion
    //  @param[in]  qua2_ : �����̊�ɂȂ� Quaternion
    //
    //  @par
    //              QuaternionMultiply() �Ƃ͋t�̏��Ԃŏ�Z���܂��B<br>
    //              D3DXQuaternionMultiply() �Ɠ�������B
    //*/
    //---------------------------------------------------------------------
    static void multiplyRO( Quaternion* out_, const Quaternion& q1_, const Quaternion& q2_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���ʐ��`��Ԃ��g���āA2 �̃N�H�[�^�j�I�����Ԃ���
    //
    //  @param[out] out_  : ���Z���ʂ��i�[���� Quaternion �ւ̃|�C���^
    //  @param[in]  qua1_ : �����̊�ɂȂ� Quaternion
    //  @param[in]  qua2_ : �����̊�ɂȂ� Quaternion
    //  @param[in]  t_    : ��Ԓl
    //*/
    //---------------------------------------------------------------------
    static void slerp( Quaternion* out_, const Quaternion& qua1_, const Quaternion& qua2_, lnfloat t_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �N�H�[�^�j�I�����I�C���[�p�ɕϊ�����
    //
    //  @param[out] out_  : ���Z���ʂ��i�[���� Vector3 �ւ̃|�C���^
    //  @param[in]  qua1_ : �����̊�ɂȂ� Quaternion
    //*/
    //---------------------------------------------------------------------
    static void toEuler( Vector3* out_, const Quaternion& qua1_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �I�C���[�p����N�H�[�^�j�I�����쐬����
    //
    //  @param[out] out_  : ���Z���ʂ��i�[���� Quaternion �ւ̃|�C���^
    //  @param[in]  qua1_ : �����̊�ɂȂ� Vector3 (�e���̊p�x)
    //*/
    //---------------------------------------------------------------------
    static void fromEuler( Quaternion* out_, const Vector3& angle_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �N�H�[�^�j�I����Yaw(Y��]), Pitch(X��]), Roll(Z��])�ɕ�������
    //
    //  @param[out] z_      : Z����]
    //  @param[out] x_      : X����](-PI/2�`PI/2)
    //  @param[out] y_      : Y����]
    //  @param[in]  qua1_   : �����̊�ɂȂ� Quaternion
    //
    //  @return
    //              �W���o�����b�N��������������false�B�W���o�����b�N��X����]�Ŕ���
    //*/
    //---------------------------------------------------------------------
    static bool factoringZXY( lnfloat* z_, lnfloat* x_, lnfloat* y_, const Quaternion& qua1_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �N�H�[�^�j�I����X,Y,Z��]�ɕ�������
    //
    //  @param[out] x_      : X����]
    //  @param[out] y_      : Y����](-PI/2�`PI/2)
    //  @param[out] z_      : Z����]
    //  @param[in]  qua1_   : �����̊�ɂȂ� Quaternion
    //
    //  @return
    //              �W���o�����b�N��������������false�B�W���o�����b�N��Y����]�Ŕ���
    //*/
    //---------------------------------------------------------------------
    static bool factoringXYZ( lnfloat* x_, lnfloat* y_, lnfloat* z_, const Quaternion& qua1_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �N�H�[�^�j�I����Y,Z,X��]�ɕ�������
    //
    //  @param[out] y_      : Y����]
    //  @param[out] z_      : Z����](-PI/2�`PI/2)
    //  @param[out] x_      : X����]
    //  @param[in]  qua1_   : �����̊�ɂȂ� Quaternion
    //
    //  @return
    //              �W���o�����b�N��������������false�B�W���o�����b�N��Z����]�Ŕ���
    //*/
    //---------------------------------------------------------------------
    static bool factoringYZX( lnfloat* y_, lnfloat* z_, lnfloat* x_, const Quaternion& qua1_ );



};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

    inline Quaternion& Quaternion::operator = ( const Quaternion& q_ )
    {
        x = q_.x;
		y = q_.y;
		z = q_.z;
        w = q_.w;
		return *this;
    }

    inline Quaternion Quaternion::operator + ( const Quaternion& q_ ) const
	{
		return Quaternion( x + q_.x, y + q_.y, z + q_.z, w + q_.w );
	}

	inline Quaternion Quaternion::operator - ( const Quaternion& q_ ) const
	{
		return Quaternion( x - q_.x, y - q_.y, z - q_.z, w - q_.w );
	}

	inline Quaternion Quaternion::operator * ( const Quaternion& q_ ) const
	{
		//return Quaternion(
		//	y * q_.z - z * q_.y + w * q_.x + x * q_.w,
		//	z * q_.x - x * q_.z + w * q_.y + y * q_.w,
		//	x * q_.y - y * q_.x + w * q_.z + z * q_.w,
		//	w * q_.w - x * q_.x - y * q_.y - z * q_.z );

		/*
		lnfloat tx, ty, tz;
		tx = y * q_.z - z * q_.y;
		ty = -( x * q_.z - z * q_.x );
		tz = x * q_.y - y * q_.x;

		return Quaternion(
			w * q_.x + q_.w * x + tx,
			w * q_.y + q_.w * y + ty,
			w * q_.z + q_.w * z + tz,
			w * q_.w - ( x * q_.x + y * q_.y + z * q_.z ) );

		*/

		return Quaternion(
			w * q_.x + x * q_.w + y * q_.z + ( -z * q_.y ),
			w * q_.y + y * q_.w + z * q_.x + ( -x * q_.z ),
			w * q_.z + z * q_.w + x * q_.y + ( -y * q_.x ),
			w * q_.w + ( -x * q_.x ) + ( -y * q_.y ) + ( -z * q_.z ) );
		
	}

	inline Quaternion Quaternion::operator * ( const lnfloat a_ ) const
	{
		return Quaternion( x * a_, y * a_, z * a_, w * a_ );
	}


	inline Quaternion Quaternion::operator / ( const lnfloat a_ ) const
	{
		lnfloat r = 1.0f / a_;
		return Quaternion( x * a_, y * a_, z * a_, w * a_ );
	}

 

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Geometry
} // namespace Core

typedef Core::Geometry::Quaternion	LQuaternion;

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================