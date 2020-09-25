//=============================================================================
//�y Vector4 �z
//-----------------------------------------------------------------------------
///**
//  @file       Vector4.h
//  @brief      Vector4
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Vector3.h"
#include "Vector2.h"
#include "Math.h"

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
// �� Vector4 �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      4�����x�N�g���̃N���X
//
//  @par
//              ��{�I�� 2�A3 �����x�N�g�����V�F�[�_�ɓn�����߂Ɏg���̂�
//              ����Ȃɋ@�\�͑����Ȃ��ł��B
//*/
//=============================================================================
class Vector4
{
public:

	lnfloat x, y, z, w;

public:

	/// �f�t�H���g�R���X�g���N�^ ( ���ׂĂ̗v�f�� 0.0 �ŏ����� )
	Vector4();

    /// �R���X�g���N�^
	Vector4( lnfloat x_, lnfloat y_, lnfloat z_, lnfloat w_ );

	/// �R���X�g���N�^ ( ���ׂĂ̗v�f�ɓ����l��ݒ� )
    explicit Vector4( lnfloat a_ );

    /// ( x, y, z, 1.0 )
    explicit Vector4( const LVector3& vec_ );

    /// ( vec_.x, vec_.y, z, w )
    Vector4( const LVector2& vec_, lnfloat z_, lnfloat w_ );

    /// ( vec_.x, vec_.y, vec_.z, w )
    Vector4( const LVector3& vec_, lnfloat w_ );

    /// �R�s�[�R���X�g���N�^
    Vector4( const Vector4& vec_ );

public:

	/// �l�̐ݒ�
	void set( lnfloat a_ );

	/// �l�̐ݒ�
	void set( lnfloat x_, lnfloat y_, lnfloat z_, lnfloat w_ );

    /// �w�肳�ꂽ�ő�l�ƍŏ��l�͈̔͂ɃN�����v����
    void clamp( lnfloat min_xyzw_, lnfloat max_xyzw_ );

public:

	Vector4& operator = ( const lnfloat a_ );
	Vector4& operator = ( const Vector4& vec_ );
	void operator += ( const lnfloat a_ );
	void operator += ( const Vector4& vec_ );
	void operator -= ( const lnfloat a_ );
	void operator -= ( const Vector4& vec_ );
	void operator *= ( const lnfloat a_ );
	void operator *= ( const Vector4& vec_ );
	void operator /= ( const lnfloat a_ );
	void operator /= ( const Vector4& vec_ );
	Vector4 operator + ( const lnfloat a_ ) const;
	Vector4 operator + ( const Vector4& vec_ ) const;
	Vector4 operator - ( const lnfloat a_ ) const;
	Vector4 operator - ( const Vector4& vec_ ) const;
	Vector4 operator * ( const lnfloat a_ ) const;
	Vector4 operator * ( const Vector4& vec_ ) const;
	Vector4 operator / ( const lnfloat a_ ) const;
	Vector4 operator / ( const Vector4& vec_ ) const;
	bool operator == ( const Vector4& vec_ ) const;
	bool operator != ( const Vector4& vec_ ) const;

	/// �W���o�͂֏o�͂���
    void dump( const lnChar* str_ = NULL ) const
    {
        _tprintf( "%s ( %f, %f, %f, %f )\n",
            ( str_ ) ? str_ : _T( "Vector4" ),
            x, y, z, w );
    }
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

    // �f�t�H���g�R���X�g���N�^ ( ���ׂĂ̗v�f�� 0.0 �ŏ����� )
	inline Vector4::Vector4()
	{
		x = y = z = w = 0.0f;
	}

	// �R���X�g���N�^
	inline Vector4::Vector4( lnfloat x_, lnfloat y_, lnfloat z_, lnfloat w_ )
	{
		x = x_;
		y = y_;
		z = z_;
        w = w_;
	}

    // �R���X�g���N�^ ( ���ׂĂ̗v�f�ɓ����l��ݒ� )
	inline Vector4::Vector4( lnfloat a_ )
	{
		x = y = z = w = a_;
	}

    // ( x, y, z, 1.0 )
    inline Vector4::Vector4( const LVector3& vec_ )
    {
        x = vec_.x;
		y = vec_.y;
        z = vec_.z;
        w = 1.0f;
    }

    /// ( vec_.x, vec_.y, z, w )
    inline Vector4::Vector4( const LVector2& vec_, lnfloat z_, lnfloat w_ )
    {
        x = vec_.x;
		y = vec_.y;
        z = z_;
        w = w_;
    }

    /// ( vec_.x, vec_.y, vec_.z, w )
    inline Vector4::Vector4( const LVector3& vec_, lnfloat w_ )
    {
        x = vec_.x;
		y = vec_.y;
        z = vec_.z;
        w = w_;
    }

    // �R�s�[�R���X�g���N�^
	inline Vector4::Vector4( const Vector4& vec_ )
	{
		x = vec_.x;
		y = vec_.y;
		z = vec_.z;
        w = vec_.w;
	}

	// �l�̐ݒ�
	inline void Vector4::set( lnfloat a_ )
	{
		x = y = z = w = a_;
	}

	// �l�̐ݒ�
	inline void Vector4::set( lnfloat x_, lnfloat y_, lnfloat z_, lnfloat w_ )
	{
		x = x_;
		y = y_;
		z = z_;
        w = w_;
	}

    /// �w�肳�ꂽ�ő�l�ƍŏ��l�͈̔͂ɃN�����v����
    inline void Vector4::clamp( lnfloat min_xyzw_, lnfloat max_xyzw_ )
    {
        x = LMath::limit( x, min_xyzw_, max_xyzw_ );
        y = LMath::limit( y, min_xyzw_, max_xyzw_ );
        z = LMath::limit( z, min_xyzw_, max_xyzw_ );
        w = LMath::limit( w, min_xyzw_, max_xyzw_ );
    }

	inline Vector4& Vector4::operator = ( const lnfloat a_ )
	{
		x = y = z = w = a_;
		return *this;
	}

	inline Vector4& Vector4::operator = ( const Vector4& vec_ )
	{
		x = vec_.x;
		y = vec_.y;
		z = vec_.z;
        w = vec_.w;
		return *this;
	}

	inline void Vector4::operator += ( const lnfloat a_ )
	{
		x += a_;
		y += a_;
		z += a_;
        w += a_;
	}

	inline void Vector4::operator += ( const Vector4& vec_ )
	{
		x += vec_.x;
		y += vec_.y;
		z += vec_.z;
        w += vec_.w;
	}

	inline void Vector4::operator -= ( const lnfloat a_ )
	{
		x -= a_;
		y -= a_;
		z -= a_;
        w -= a_;
	}

	inline void Vector4::operator -= ( const Vector4& vec_ )
	{
		x -= vec_.x;
		y -= vec_.y;
		z -= vec_.z;
        w -= vec_.w;
	}

	inline void Vector4::operator *= ( const lnfloat a_ )
	{
		x *= a_;
		y *= a_;
		z *= a_;
        w *= a_;
	}

	inline void Vector4::operator *= ( const Vector4& vec_ )
	{
		x *= vec_.x;
		y *= vec_.y;
		z *= vec_.z;
        w *= vec_.w;
	}

	inline void Vector4::operator /= ( const lnfloat a_ )
	{
        lnfloat r = 1.0f / a_;
		x *= r;
		y *= r;
		z *= r;
        w *= r;
	}

	inline void Vector4::operator /= ( const Vector4& vec_ )
	{
		x /= vec_.x;
		y /= vec_.y;
		z /= vec_.z;
        w /= vec_.w;
	}


	inline Vector4 Vector4::operator + ( const lnfloat a_ ) const
	{
		return Vector4( x + a_, y + a_, z + a_, w + a_ );
	}

	inline Vector4 Vector4::operator + ( const Vector4& vec_ ) const
	{
		return Vector4( x + vec_.x, y + vec_.y, z + vec_.z, w + vec_.w );
	}

	inline Vector4 Vector4::operator - ( const lnfloat a_ ) const
	{
		return Vector4( x - a_, y - a_, z - a_, w - a_ );
	}

	inline Vector4 Vector4::operator - ( const Vector4& vec_ ) const
	{
		return Vector4( x - vec_.x, y - vec_.y, z - vec_.z, w - vec_.w );
	}

	inline Vector4 Vector4::operator * ( const lnfloat a_ ) const
	{
		return Vector4( x * a_, y * a_, z * a_, w * a_ );
	}

	inline Vector4 Vector4::operator * ( const Vector4& vec_ ) const
	{
		return Vector4( x * vec_.x, y * vec_.y, z * vec_.z, w * vec_.w );
	}

	inline Vector4 Vector4::operator / ( const lnfloat a_ ) const
	{
		lnfloat d = static_cast< lnfloat >( 1.0 ) / a_;
		return Vector4( x * d, y * d, z * d, w * d );
	}

	inline Vector4 Vector4::operator / ( const Vector4& vec_ ) const
	{
		return Vector4( x / vec_.x, y / vec_.y, z / vec_.z, w / vec_.w );
	}

	inline bool Vector4::operator == ( const Vector4& vec_ ) const
	{
		return ( x == vec_.x && y == vec_.y && z == vec_.z && w == vec_.w );
	}

	inline bool Vector4::operator != ( const Vector4& vec_ ) const
	{
		return ( x != vec_.x || y != vec_.y || z != vec_.z || w != vec_.w );
	}



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Geometry
} // namespace Core

typedef Core::Geometry::Vector4	LVector4;

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================