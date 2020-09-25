//=============================================================================
//�y Vector2 �z
//-----------------------------------------------------------------------------
///**
//  @file       Vector2.h
//  @brief      Vector2
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
class Matrix;

//=============================================================================
// �� Vector2 �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      2�����x�N�g���̃N���X
//*/
//=============================================================================
class Vector2
{
public:

    lnfloat x, y;

public:

    static const Vector2   ZERO;       ///< Vector2( 0, 0 )
    static const Vector2   UNIT_X;     ///< Vector2( 1, 0 )
    static const Vector2   UNIT_Y;     ///< Vector2( 0, 1 )
    static const Vector2   UNIT_SCALE; ///< Vector2( 1, 1 )

public:

    /// �f�t�H���g�R���X�g���N�^ ( ���ׂĂ̗v�f�� 0.0 �ŏ����� )
    Vector2();

    /// �R���X�g���N�^
    Vector2( lnfloat x_, lnfloat y_ );

    /// �R���X�g���N�^ ( ���ׂĂ̗v�f�ɓ����l��ݒ� )
    Vector2( lnfloat a_ );

    /// �R�s�[�R���X�g���N�^
    Vector2( const Vector2& vec_ );

public:

    /// �l�̐ݒ�
    void set( lnfloat a_ );

    /// �l�̐ݒ�
    void set( lnfloat x_, lnfloat y_ );

    /// �x�N�g���̒�����Ԃ�
    lnfloat getLength() const;

    /// �x�N�g���̒����� 2 ���Ԃ�
    lnfloat getSquareLength() const;

    /// �x�N�g���𐳋K������
    void normalize();

    //---------------------------------------------------------------------
    ///**
    //  @brief      �x�N�g���𐳋K������
    //
    //  @param[in]  def_vec_ : vec_ �̒����� 0 �������ꍇ�ɐݒ肷��x�N�g��
    //*/
    //---------------------------------------------------------------------
    void normalize( const Vector2& def_vec_ );

    //---------------------------------------------------------------------
    ///**
	//	@brief		�w�肳�ꂽ�s����g���č��W�ϊ�����
    //
    //  @par
    //              �x�N�g���� (x, y, 0.0, 1.0) �Ƃ��č��W�ϊ����s���A
    //              ���ʂ� w �ŏ��Z���܂��B
	//*/
	//---------------------------------------------------------------------
	void transform( const Matrix& matrix_ );

public:

    Vector2& operator = ( const lnfloat a_ );
    Vector2& operator = ( const Vector2& vec_ );

    void operator += ( const lnfloat a_ );
    void operator += ( const Vector2& vec_ );
    void operator -= ( const lnfloat a_ );
    void operator -= ( const Vector2& vec_ );
    void operator *= ( const lnfloat a_ );
    void operator *= ( const Vector2& vec_ );
    void operator /= ( const lnfloat a_ );
    void operator /= ( const Vector2& vec_ );

    Vector2 operator + ( const lnfloat a_ ) const;
    Vector2 operator + ( const Vector2& vec_ ) const;
    Vector2 operator - ( const lnfloat a_ ) const;
    Vector2 operator - ( const Vector2& vec_ ) const;
    Vector2 operator * ( const lnfloat a_ ) const;
    Vector2 operator * ( const Vector2& vec_ ) const;
    Vector2 operator / ( const lnfloat a_ ) const;
    Vector2 operator / ( const Vector2& vec_ ) const;

    friend Vector2 operator + ( const lnfloat a_, const Vector2& vec_ ) { return Vector2( a_ + vec_.x, a_ + vec_.y ); }
    friend Vector2 operator - ( const lnfloat a_, const Vector2& vec_ ) { return Vector2( a_ - vec_.x, a_ - vec_.y ); }
    friend Vector2 operator * ( const lnfloat a_, const Vector2& vec_ ) { return Vector2( a_ * vec_.x, a_ * vec_.y ); }
    friend Vector2 operator / ( const lnfloat a_, const Vector2& vec_ ) { return Vector2( a_ / vec_.x, a_ / vec_.y ); }
    
    const Vector2& operator + () const;
    Vector2        operator - () const;

    bool operator == ( const Vector2& vec_ ) const;
    bool operator != ( const Vector2& vec_ ) const;

    /// �W���o�͂֏o�͂���
    void dump( const lnChar* str_ = NULL ) const
    {
        _tprintf( "%s ( %f, %f )\n",
            ( str_ ) ? str_ : _T( "Vector2" ),
            x, y );
    }

public:

    //-------------------------------------------------------------------------
	// �� static

    //---------------------------------------------------------------------
    ///**
    //  @brief      2D �x�N�g���̐��K�������x�N�g����Ԃ�
    //
    //  @param[out] out_     : ���Z���ʂ��i�[���� Vector2 �ւ̃|�C���^
    //  @param[in]  vec_     : �����̊�ɂȂ� Vector2
    //*/
    //---------------------------------------------------------------------
	static void normalize( Vector2* out_, const Vector2& vec_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      2D �x�N�g���̐��K�������x�N�g����Ԃ�
    //
    //  @param[out] out_     : ���Z���ʂ��i�[���� Vector2 �ւ̃|�C���^
    //  @param[in]  vec_     : �����̊�ɂȂ� Vector2
    //  @param[in]  def_vec_ : vec_ �̒����� 0 �������ꍇ�ɕԂ��x�N�g��
    //*/
    //---------------------------------------------------------------------
	static void normalize( Vector2* out_, const Vector2& vec_, const Vector2& def_vec_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      2 �� 2D �x�N�g���̓��ς��v�Z����
    //
    //  @param[in]  vec1_ : �����̊�ɂȂ� Vector2
    //  @param[in]  vec2_ : �����̊�ɂȂ� Vector2
    //*/
    //---------------------------------------------------------------------
	static lnfloat dot( const Vector2& vec1_, const Vector2& vec2_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      2 �� 2D �x�N�g���̊O�ς��v�Z���Az �v�f��Ԃ�
    //
    //  @param[in]  vec1_ : �����̊�ɂȂ� Vector2
    //  @param[in]  vec2_ : �����̊�ɂȂ� Vector2
    //
    //  @par
    //              �����̌����ɑ΂��đ���͍��E�ǂ���ɂ��邩�Ƃ������蓙�Ɏg���܂��B
    //*/
    //---------------------------------------------------------------------
	static lnfloat crossZ( const Vector2& vec1_, const Vector2& vec2_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �w�肳�ꂽ�s����g���č��W�ϊ�����
    //
    //  @param[out] out_ : ���Z���ʂ��i�[���� Vector2 �ւ̃|�C���^
    //  @param[in]  vec_ : �����̊�ɂȂ� Vector2
    //  @param[in]  mat_ : �����̊�ɂȂ� Matrix
    //
    //  @par
    //              �x�N�g���� ( x, y, 0.0, 1 ) �Ƃ��č��W�ϊ����A
    //              ���ʂ� w �ŏ��Z���܂��B
    //*/
    //---------------------------------------------------------------------
	static void transform( Vector2* out_, const Vector2& vec_, const Matrix& mat_ );
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

	// �R���X�g���N�^
	inline Vector2::Vector2()
	{
		x = y = 0;
	}

    // �R���X�g���N�^
	inline Vector2::Vector2( lnfloat x_, lnfloat y_ )
	{
		x = x_;
		y = y_;
	}

	// �R���X�g���N�^ ( ���ׂĂ̗v�f�ɓ����l��ݒ� )
	inline Vector2::Vector2( lnfloat a_ )
	{
		x = y = a_;
	}

    // �R�s�[�R���X�g���N�^
	inline Vector2::Vector2( const Vector2& vec_ )
	{
		x = vec_.x;
		y = vec_.y;
	}

	// �l�̐ݒ�
	inline void Vector2::set( lnfloat a_ )
	{
		x = y = a_;
	}
	inline void Vector2::set( lnfloat x_, lnfloat y_ )
	{
		x = x_;
		y = y_;
	}

    // �x�N�g���̒�����Ԃ�
    inline lnfloat Vector2::getLength() const
    {
        return Asm::sqrt( x * x + y * y );
    }

    // �x�N�g���̒�����2���Ԃ�
	inline lnfloat Vector2::getSquareLength() const
    {
        return ( x * x + y * y );
    }

    // �x�N�g���𐳋K������
    inline void Vector2::normalize()
    {
        lnfloat t = x * x + y * y;
        t = 1.0f / Asm::sqrt( t );
		x *= t;
		y *= t;
    }

    // �x�N�g���𐳋K������
    inline void Vector2::normalize( const Vector2& def_vec_ )
    {
        lnfloat t = x * x + y * y;
        if ( t == 0 )
        {
            x = def_vec_.x;
            y = def_vec_.y;
            return;
        }
        t = 1.0f / Asm::sqrt( t );
		x *= t;
		y *= t;
    }

    


	inline Vector2& Vector2::operator = ( const lnfloat a_ )
	{
		x = y = a_;
		return *this;
	}

	inline Vector2& Vector2::operator = ( const Vector2& vec_ )
	{
		x = vec_.x;
		y = vec_.y;
		return *this;
	}

	inline void Vector2::operator += ( const lnfloat a_ )
	{
		x += a_;
		y += a_;
	}

	inline void Vector2::operator += ( const Vector2& vec_ )
	{
		x += vec_.x;
		y += vec_.y;
	}

	inline void Vector2::operator -= ( const lnfloat a_ )
	{
		x -= a_;
		y -= a_;
	}

	inline void Vector2::operator -= ( const Vector2& vec_ )
	{
		x -= vec_.x;
		y -= vec_.y;
	}

	inline void Vector2::operator *= ( const lnfloat a_ )
	{
		x *= a_;
		y *= a_;
	}

	inline void Vector2::operator *= ( const Vector2& vec_ )
	{
		x *= vec_.x;
		y *= vec_.y;
	}

	inline void Vector2::operator /= ( const lnfloat a_ )
	{
		x /= a_;
		y /= a_;
	}

	inline void Vector2::operator /= ( const Vector2& vec_ )
	{
		x /= vec_.x;
		y /= vec_.y;
	}


	inline Vector2 Vector2::operator + ( const lnfloat a_ ) const
	{
		return Vector2( x + a_, y + a_ );
	}

	inline Vector2 Vector2::operator + ( const Vector2& vec_ ) const
	{
		return Vector2( x + vec_.x, y + vec_.y );
	}

	inline Vector2 Vector2::operator - ( const lnfloat a_ ) const
	{
		return Vector2( x - a_, y - a_ );
	}

	inline Vector2 Vector2::operator - ( const Vector2& vec_ ) const
	{
		return Vector2( x - vec_.x, y - vec_.y );
	}

	inline Vector2 Vector2::operator * ( const lnfloat a_ ) const
	{
		return Vector2( x * a_, y * a_ );
	}

	inline Vector2 Vector2::operator * ( const Vector2& vec_ ) const
	{
		return Vector2( x * vec_.x, y * vec_.y );
	}

	inline Vector2 Vector2::operator / ( const lnfloat a_ ) const
	{
		lnfloat d = static_cast< lnfloat >( 1.0 ) / a_;
		return Vector2( x * d, y * d );
	}

	inline Vector2 Vector2::operator / ( const Vector2& vec_ ) const
	{
		return Vector2( x / vec_.x, y / vec_.y );
	}

    // +-
    inline const Vector2& Vector2::operator + () const
    {
        return *this;
    }

    inline Vector2 Vector2::operator - () const
    {
        return Vector2( -x, -y );
    }

    // ��r

	inline bool Vector2::operator == ( const Vector2& vec_ ) const
	{
		return ( x == vec_.x && y == vec_.y );
	}

	inline bool Vector2::operator != ( const Vector2& vec_ ) const
	{
		return ( x != vec_.x || y != vec_.y );
	}

	


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Geometry
} // namespace Core

typedef Core::Geometry::Vector2	LVector2;

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================