//=============================================================================
//�y Vector3 �z
//-----------------------------------------------------------------------------
///**
//  @file       Vector3.h
//  @brief      Vector3
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Common.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Geometry
{
class Vector2;
class Matrix;

//=============================================================================
// �� Vector3 �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      3�����x�N�g���̃N���X
//*/
//=============================================================================
class Vector3
{
public:

	lnfloat x, y, z;

public:

    static const Vector3   ZERO;       ///< ( 0, 0 )
    static const Vector3   UNIT_X;     ///< ( 1, 0, 0 )
    static const Vector3   UNIT_Y;     ///< ( 0, 1, 0 )
    static const Vector3   UNIT_Z;     ///< ( 0, 0, 1 )
    static const Vector3   UNIT_SCALE; ///< ( 1, 1, 1 )

public:

	/// �f�t�H���g�R���X�g���N�^ ( ���ׂĂ̗v�f�� 0.0 �ŏ����� )
	Vector3();

    /// �R���X�g���N�^
	Vector3( lnfloat x_, lnfloat y_, lnfloat z_ );

	/// �R���X�g���N�^ ( ���ׂĂ̗v�f�ɓ����l��ݒ� )
	explicit Vector3( lnfloat a_ );

    /// ( vec_.x, vec_.y, z)
	explicit Vector3( const Vector2& vec_, lnfloat z_ );

	/// �R�s�[�R���X�g���N�^
	Vector3( const Vector3& vec_ );

public:

	/// �l�̐ݒ�
	void set( lnfloat a_ );

	/// �l�̐ݒ�
	void set( lnfloat x_, lnfloat y_, lnfloat z_ );

	//---------------------------------------------------------------------
	///**
    //	@brief      �x�N�g���̒����̎擾
    //*/
	//---------------------------------------------------------------------
	lnfloat getLength() const;

    //---------------------------------------------------------------------
	///**
    //	@brief      �x�N�g���̒����̎擾 (�l��p)
    //
    //  @param[in]  default_ : �������v�Z�ł��Ȃ��ꍇ�A����ɕԂ��l
    //*/
	//---------------------------------------------------------------------
	lnfloat getLength( lnfloat default_ ) const;

	//---------------------------------------------------------------------
	///**
	//	@brief		�x�N�g���̒�����2���Ԃ�
	//*/
	//---------------------------------------------------------------------
	lnfloat getSquareLength() const;

	//---------------------------------------------------------------------
	///**
	//	@brief		�x�N�g���𐳋K������
	//*/
	//---------------------------------------------------------------------
	void normalize();

    //---------------------------------------------------------------------
    ///**
    //  @brief      �x�N�g���𐳋K������
    //
    //  @param[in]  def_vec_ : vec_ �̒����� 0 �������ꍇ�ɐݒ肷��x�N�g��
    //*/
    //---------------------------------------------------------------------
    void normalize( const Vector3& def_vec_ );

    //---------------------------------------------------------------------
    ///**
	//	@brief		�w�肳�ꂽ�s����g���č��W�ϊ�����
    //
    //  @par
    //              �x�N�g���� (x, y, z, 1.0) �Ƃ��č��W�ϊ����s���A
    //              ���ʂ� w �ŏ��Z���܂��B
	//*/
	//---------------------------------------------------------------------
	void transform( const Matrix& matrix_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      �x�N�g���̒����� 0 ���𔻒肷��
	//*/
	//---------------------------------------------------------------------
	bool isZero() const { return ( x == 0 && y == 0 && z == 0 ); }

    //---------------------------------------------------------------------
	///**
	//  @brief      NaN �`�F�b�N
	//*/
	//---------------------------------------------------------------------
    bool isNaN() const { return LMath::isNaN( x ) || LMath::isNaN( y ) || LMath::isNaN( z ); }

    /// Vector2
    Vector2 getXY() const;

public:

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

	Vector3 operator + ( const lnfloat a_ ) const;
	Vector3 operator + ( const Vector3& vec_ ) const;
	Vector3 operator - ( const lnfloat a_ ) const;
	Vector3 operator - ( const Vector3& vec_ ) const;
	Vector3 operator * ( const lnfloat a_ ) const;
	Vector3 operator * ( const Vector3& vec_ ) const;
	Vector3 operator / ( const lnfloat a_ ) const;
	Vector3 operator / ( const Vector3& vec_ ) const;

    friend Vector3 operator + ( const lnfloat a_, const Vector3& vec_ ) { return Vector3( a_ + vec_.x, a_ + vec_.y, a_ + vec_.z ); }
    friend Vector3 operator - ( const lnfloat a_, const Vector3& vec_ ) { return Vector3( a_ - vec_.x, a_ - vec_.y, a_ - vec_.z ); }
    friend Vector3 operator * ( const lnfloat a_, const Vector3& vec_ ) { return Vector3( a_ * vec_.x, a_ * vec_.y, a_ * vec_.z ); }
    friend Vector3 operator / ( const lnfloat a_, const Vector3& vec_ ) { return Vector3( a_ / vec_.x, a_ / vec_.y, a_ / vec_.z ); }
    
    const Vector3& operator + () const;
    Vector3        operator - () const;

	bool operator == ( const Vector3& vec_ ) const;
	bool operator != ( const Vector3& vec_ ) const;

	/// �W���o�͂֏o�͂���
	void dump( const lnChar* str_ = NULL ) const;

public:

    //-------------------------------------------------------------------------
	// �� static

    //---------------------------------------------------------------------
    ///**
    //  @brief      3D �x�N�g���̒�����Ԃ�
    //
    //  @param[in]  vec_ : �����̊�ɂȂ� Vector3
    //*/
    //---------------------------------------------------------------------
	//lnfloat length( const Vector3& vec_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      3D �x�N�g���̒�����2���Ԃ�
    //
    //  @param[in]  vec_ : �����̊�ɂȂ� Vector3
    //
    //  @par
    //              Vector3Length() �̒��ɂ͕������̌v�Z ( sqrt() ) ������A
    //              ���̌v�Z�͎��Ԃ̂����鏈���ł��B
    //              �Q�[���̒��ő����̃I�u�W�F�N�g�ǂ����̋����̔�r�����铙�A
    //              Vector3Length() ���ʂɌĂԏꍇ�A����ɂ��̊֐����g������
    //              ��������}�邱�Ƃ��ł��܂��B
    //*/
    //---------------------------------------------------------------------
	static lnfloat squareLength( const Vector3& vec_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      3D �x�N�g���̐��K�������x�N�g����Ԃ�
    //
    //  @param[out] out_ : ���Z���ʂ��i�[���� Vector3 �ւ̃|�C���^
    //  @param[in]  vec_ : �����̊�ɂȂ� Vector3
    //*/
    //---------------------------------------------------------------------
	static void normalize( Vector3* out_, const Vector3& vec_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      2D �x�N�g���̐��K�������x�N�g����Ԃ�
    //
    //  @param[out] out_     : ���Z���ʂ��i�[���� Vector2 �ւ̃|�C���^
    //  @param[in]  vec_     : �����̊�ɂȂ� Vector2
    //  @param[in]  def_vec_ : vec_ �̒����� 0 �������ꍇ�ɕԂ��x�N�g��
    //*/
    //---------------------------------------------------------------------
	static void normalize( Vector3* out_, const Vector3& vec_, const Vector3& def_vec_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      2 �� 3D �x�N�g���̓��ς��v�Z����
    //
    //  @param[in]  vec1_ : �����̊�ɂȂ� Vector3
    //  @param[in]  vec2_ : �����̊�ɂȂ� Vector3
    //*/
    //---------------------------------------------------------------------
	static lnfloat dot( const Vector3& vec1_, const Vector3& vec2_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      2 �� 3D �x�N�g���̊O�ς��v�Z����
    //
    //  @param[out] out_  : ���Z���ʂ��i�[���� Vector3 �ւ̃|�C���^
    //  @param[in]  vec1_ : �����̊�ɂȂ� Vector3
    //  @param[in]  vec2_ : �����̊�ɂȂ� Vector3
    //*/
    //---------------------------------------------------------------------
	static void cross( Vector3* out_, const Vector3& vec1_, const Vector3& vec2_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �w�肳�ꂽ�@���Ŕ��˂������x�N�g�����v�Z����
    //
    //  @param[out] out_    : ���Z���ʂ��i�[���� Vector3 �ւ̃|�C���^
    //  @param[in]  vec_    : �����̊�ɂȂ� Vector3
    //  @param[in]  normal_ : �ʕ�����\���@�� Vector3
    //*/
    //---------------------------------------------------------------------
	static void reflect( Vector3* out_, const Vector3& vec_, const Vector3& normal_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �w�肳�ꂽ�@���ƕ������犊��x�N�g�����v�Z����
    //
    //  @param[out] out_    : ���Z���ʂ��i�[���� Vector3 �ւ̃|�C���^
    //  @param[in]  vec_    : �����̊�ɂȂ� Vector3
    //  @param[in]  normal_ : �ʕ�����\���@�� Vector3
    //*/
    //---------------------------------------------------------------------
	static void slide( Vector3* out_, const Vector3& vec_, const Vector3& normal_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      2 �� 3D �x�N�g���Ԃ̐��`��Ԃ����s����
    //
    //  @param[out] out_  : ���Z���ʂ��i�[���� Vector3 �ւ̃|�C���^
    //  @param[in]  vec1_ : �����̊�ɂȂ� Vector3 ( t_ = 0.0 �̂Ƃ��̒l )
    //  @param[in]  vec2_ : �����̊�ɂȂ� Vector3 ( t_ = 1.0 �̂Ƃ��̒l )
    //  @param[in]  t_    : ��Ԓl ( 0.0 �` 1.0 )
    //*/
    //---------------------------------------------------------------------
	static void lerp( Vector3* out_, const Vector3& vec1_, const Vector3& vec2_, lnfloat t_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �w�肳�ꂽ 3D �x�N�g�����g�p���āACatmull-Rom ��Ԃ����s����
    //
    //  @param[out] out_  : ���Z���ʂ��i�[���� Vector3 �ւ̃|�C���^
    //  @param[in]  vec1_ : �����̊�ɂȂ� Vector3
    //  @param[in]  vec2_ : �����̊�ɂȂ� Vector3 ( t_ = 0.0 �̂Ƃ��̒l )
    //  @param[in]  vec3_ : �����̊�ɂȂ� Vector3 ( t_ = 1.0 �̂Ƃ��̒l )
    //  @param[in]  vec4_ : �����̊�ɂȂ� Vector3
    //  @param[in]  t_    : ��Ԓl ( 0.0 �` 1.0 )
    //*/
    //---------------------------------------------------------------------
	static void catmullRom( Vector3* out_, const Vector3& vec1_, const Vector3& vec2_, const Vector3& vec3_, const Vector3& vec4_, lnfloat t_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �w�肳�ꂽ�s����g���č��W�ϊ�����
    //
    //  @param[out] out_ : ���Z���ʂ��i�[���� Vector3 �ւ̃|�C���^
    //  @param[in]  vec_ : �����̊�ɂȂ� Vector3
    //  @param[in]  mat_ : �����̊�ɂȂ� Matrix
    //
    //  @par
    //              �x�N�g���� ( x, y, z, 1 ) �Ƃ��č��W�ϊ����܂��B
    //              ���ʂ� w �͏o�͂���܂���B<br>
    //              ���ʂ� w = 1 �Ɏˉe���� ( x y z �� w �ŏ��Z���� ) �ꍇ��
    //              Vector3TransformCoord() ���g���Ă��������B
    //*/
    //---------------------------------------------------------------------
	static void transform( Vector3* out_, const Vector3& vec_, const Matrix& mat_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �w�肳�ꂽ�s����g���č��W�ϊ����A���ʂ� w = 1 �Ɏˉe����
    //
    //  @param[out] out_ : ���Z���ʂ��i�[���� Vector3 �ւ̃|�C���^
    //  @param[in]  vec_ : �����̊�ɂȂ� Vector3
    //  @param[in]  mat_ : �����̊�ɂȂ� Matrix
    //
    //  @par
    //              �x�N�g���� ( x, y, z, 1 ) �Ƃ��č��W�ϊ����A
    //              ���ʂ� w = 1 �Ɏˉe ( x y z �� w �ŏ��Z���� ) ���܂��B
    //*/
    //---------------------------------------------------------------------
	static void transformCoord( Vector3* out_, const Vector3& vec_, const Matrix& mat_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �s��̉�]�������g���č��W�ϊ�����
    //
    //  @param[out] out_ : ���Z���ʂ��i�[���� Vector3 �ւ̃|�C���^
    //  @param[in]  vec_ : �����̊�ɂȂ� Vector3
    //  @param[in]  mat_ : �����̊�ɂȂ� Matrix
    //*/
    //---------------------------------------------------------------------
	static void rotate( Vector3* out_, const Vector3& vec_, const Matrix& mat_ );



public:

#if LNOTE_INCLUDED_DIRECTX_HEADER

    // D3DXVECTOR3 �ɕϊ�
	inline D3DXVECTOR3 Vector3::toD3DXVECTOR3() const
    {
        return D3DXVECTOR3(
            static_cast< float >( x ),
            static_cast< float >( y ),
            static_cast< float >( z ) );
    }

#endif // LNOTE_DIRECTX
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

	// �f�t�H���g�R���X�g���N�^ ( ���ׂĂ̗v�f�� 0.0 �ŏ����� )
	inline Vector3::Vector3()
	{
		x = y = z = 0.0f;
	}

	// �R���X�g���N�^
	inline Vector3::Vector3( lnfloat x_, lnfloat y_, lnfloat z_ )
	{
		x = x_;
		y = y_;
		z = z_;
	}

	// �R���X�g���N�^ ( ���ׂĂ̗v�f�ɓ����l��ݒ� )
	inline Vector3::Vector3( lnfloat a_ )
	{
		x = y = z = a_;
	}

    

    // �R�s�[�R���X�g���N�^
	inline Vector3::Vector3( const Vector3& vec_ )
	{
		x = vec_.x;
		y = vec_.y;
		z = vec_.z;
	}

	// �l�̐ݒ�
	inline void Vector3::set( lnfloat a_ )
	{
		x = y = z = a_;
	}

	// �l�̐ݒ�
	inline void Vector3::set( lnfloat x_, lnfloat y_, lnfloat z_ )
	{
		x = x_;
		y = y_;
		z = z_;
	}

	//---------------------------------------------------------------------
	// �� �x�N�g���̒����̎擾
	//---------------------------------------------------------------------
	inline lnfloat Vector3::getLength() const
	{
        return Asm::sqrt( x * x + y * y + z * z );
	}

    //---------------------------------------------------------------------
	// �� �x�N�g���̒����̎擾
	//---------------------------------------------------------------------
	inline lnfloat Vector3::getLength( lnfloat default_ ) const
	{
        lnfloat t = x * x + y * y + z * z;
        if ( t == 0.0f )
        {
            return default_;
        }
		return Asm::sqrt( t );
	}

	//---------------------------------------------------------------------
	// �� 3D �x�N�g���̒�����2���Ԃ�
	//---------------------------------------------------------------------
	inline lnfloat Vector3::getSquareLength() const
	{
		return ( x * x + y * y + z * z );
	}

	//---------------------------------------------------------------------
	/// �� �x�N�g���𐳋K������
	//---------------------------------------------------------------------
	inline void Vector3::normalize()
	{
        lnfloat t = 1.0f / Asm::sqrt( x * x + y * y + z * z );
		x *= t;
		y *= t;
		z *= t;
	}

    inline void Vector3::normalize( const Vector3& def_vec_ )
    {
        lnfloat t = x * x + y * y + z * z;
        if ( t == 0 )
        {
            *this = def_vec_;
            return;
        }
        t = 1.0f / Asm::sqrt( t );
        x *= t;
		y *= t;
		z *= t;
    }

    // transform �̒�`�� Matrix

	//---------------------------------------------------------------------
	// �� ���Z�q�I�[�o�[���[�h
	//---------------------------------------------------------------------
	inline Vector3& Vector3::operator = ( const lnfloat a_ )
	{
		x = y = z = a_;
		return *this;
	}

	inline Vector3& Vector3::operator = ( const Vector3& vec_ )
	{
		x = vec_.x;
		y = vec_.y;
		z = vec_.z;
		return *this;
	}

	inline void Vector3::operator += ( const lnfloat a_ )
	{
		x += a_;
		y += a_;
		z += a_;
	}

	inline void Vector3::operator += ( const Vector3& vec_ )
	{
		x += vec_.x;
		y += vec_.y;
		z += vec_.z;
	}

	inline void Vector3::operator -= ( const lnfloat a_ )
	{
		x -= a_;
		y -= a_;
		z -= a_;
	}

	inline void Vector3::operator -= ( const Vector3& vec_ )
	{
		x -= vec_.x;
		y -= vec_.y;
		z -= vec_.z;
	}

	inline void Vector3::operator *= ( const lnfloat a_ )
	{
		x *= a_;
		y *= a_;
		z *= a_;
	}

	inline void Vector3::operator *= ( const Vector3& vec_ )
	{
		x *= vec_.x;
		y *= vec_.y;
		z *= vec_.z;
	}

	inline void Vector3::operator /= ( const lnfloat a_ )
	{
		lnfloat r = 1.0f / a_;
		x *= r;
		y *= r;
		z *= r;
	}

	inline void Vector3::operator /= ( const Vector3& vec_ )
	{
		x /= vec_.x;
		y /= vec_.y;
		z /= vec_.z;
	}

    // �l�����Z
	inline Vector3 Vector3::operator + ( const lnfloat a_ ) const
	{
		return Vector3( x + a_, y + a_, z + a_ );
	}

	inline Vector3 Vector3::operator + ( const Vector3& vec_ ) const
	{
		return Vector3( x + vec_.x, y + vec_.y, z + vec_.z );
	}

	inline Vector3 Vector3::operator - ( const lnfloat a_ ) const
	{
		return Vector3( x - a_, y - a_, z - a_ );
	}

	inline Vector3 Vector3::operator - ( const Vector3& vec_ ) const
	{
		return Vector3( x - vec_.x, y - vec_.y, z - vec_.z );
	}

	inline Vector3 Vector3::operator * ( const lnfloat a_ ) const
	{
		return Vector3( x * a_, y * a_, z * a_ );
	}

	inline Vector3 Vector3::operator * ( const Vector3& vec_ ) const
	{
		return Vector3( x * vec_.x, y * vec_.y, z * vec_.z );
	}

	inline Vector3 Vector3::operator / ( const lnfloat a_ ) const
	{
		lnfloat d = 1.0f / a_;
		return Vector3( x * d, y * d, z * d );
	}

	inline Vector3 Vector3::operator / ( const Vector3& vec_ ) const
	{
		return Vector3( x / vec_.x, y / vec_.y, z / vec_.z );
	}

    // +-
    inline const Vector3& Vector3::operator + () const
    {
        return *this;
    }

    inline Vector3 Vector3::operator - () const
    {
        return Vector3( -x, -y, -z );
    }

    // ��r
	inline bool Vector3::operator == ( const Vector3& vec_ ) const
	{
		return ( x == vec_.x && y == vec_.y && z == vec_.z );
	}

	inline bool Vector3::operator != ( const Vector3& vec_ ) const
	{
		return ( x != vec_.x || y != vec_.y || z != vec_.z );
	}

    // �R���\�[���ɏo�͂���
	inline void Vector3::dump( const lnChar* str_ ) const
	{
        _tprintf( "%s ( %f, %f, %f )\n",
            ( str_ ) ? str_ : _T( "Vector3" ),
            x, y, z );
	}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Geometry
} // namespace Core

typedef Core::Geometry::Vector3	LVector3;

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================