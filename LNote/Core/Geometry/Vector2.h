//=============================================================================
//【 Vector2 】
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
// ■ Vector2 クラス
//-----------------------------------------------------------------------------
///**
//  @brief      2次元ベクトルのクラス
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

    /// デフォルトコンストラクタ ( すべての要素を 0.0 で初期化 )
    Vector2();

    /// コンストラクタ
    Vector2( lnfloat x_, lnfloat y_ );

    /// コンストラクタ ( すべての要素に同じ値を設定 )
    Vector2( lnfloat a_ );

    /// コピーコンストラクタ
    Vector2( const Vector2& vec_ );

public:

    /// 値の設定
    void set( lnfloat a_ );

    /// 値の設定
    void set( lnfloat x_, lnfloat y_ );

    /// ベクトルの長さを返す
    lnfloat getLength() const;

    /// ベクトルの長さの 2 乗を返す
    lnfloat getSquareLength() const;

    /// ベクトルを正規化する
    void normalize();

    //---------------------------------------------------------------------
    ///**
    //  @brief      ベクトルを正規化する
    //
    //  @param[in]  def_vec_ : vec_ の長さが 0 だった場合に設定するベクトル
    //*/
    //---------------------------------------------------------------------
    void normalize( const Vector2& def_vec_ );

    //---------------------------------------------------------------------
    ///**
	//	@brief		指定された行列を使って座標変換する
    //
    //  @par
    //              ベクトルを (x, y, 0.0, 1.0) として座標変換を行い、
    //              結果を w で除算します。
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

    /// 標準出力へ出力する
    void dump( const lnChar* str_ = NULL ) const
    {
        _tprintf( "%s ( %f, %f )\n",
            ( str_ ) ? str_ : _T( "Vector2" ),
            x, y );
    }

public:

    //-------------------------------------------------------------------------
	// ▼ static

    //---------------------------------------------------------------------
    ///**
    //  @brief      2D ベクトルの正規化したベクトルを返す
    //
    //  @param[out] out_     : 演算結果を格納する Vector2 へのポインタ
    //  @param[in]  vec_     : 処理の基になる Vector2
    //*/
    //---------------------------------------------------------------------
	static void normalize( Vector2* out_, const Vector2& vec_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      2D ベクトルの正規化したベクトルを返す
    //
    //  @param[out] out_     : 演算結果を格納する Vector2 へのポインタ
    //  @param[in]  vec_     : 処理の基になる Vector2
    //  @param[in]  def_vec_ : vec_ の長さが 0 だった場合に返すベクトル
    //*/
    //---------------------------------------------------------------------
	static void normalize( Vector2* out_, const Vector2& vec_, const Vector2& def_vec_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      2 つの 2D ベクトルの内積を計算する
    //
    //  @param[in]  vec1_ : 処理の基になる Vector2
    //  @param[in]  vec2_ : 処理の基になる Vector2
    //*/
    //---------------------------------------------------------------------
	static lnfloat dot( const Vector2& vec1_, const Vector2& vec2_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      2 つの 2D ベクトルの外積を計算し、z 要素を返す
    //
    //  @param[in]  vec1_ : 処理の基になる Vector2
    //  @param[in]  vec2_ : 処理の基になる Vector2
    //
    //  @par
    //              自分の向きに対して相手は左右どちらにいるかという判定等に使います。
    //*/
    //---------------------------------------------------------------------
	static lnfloat crossZ( const Vector2& vec1_, const Vector2& vec2_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      指定された行列を使って座標変換する
    //
    //  @param[out] out_ : 演算結果を格納する Vector2 へのポインタ
    //  @param[in]  vec_ : 処理の基になる Vector2
    //  @param[in]  mat_ : 処理の基になる Matrix
    //
    //  @par
    //              ベクトルを ( x, y, 0.0, 1 ) として座標変換し、
    //              結果を w で除算します。
    //*/
    //---------------------------------------------------------------------
	static void transform( Vector2* out_, const Vector2& vec_, const Matrix& mat_ );
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

	// コンストラクタ
	inline Vector2::Vector2()
	{
		x = y = 0;
	}

    // コンストラクタ
	inline Vector2::Vector2( lnfloat x_, lnfloat y_ )
	{
		x = x_;
		y = y_;
	}

	// コンストラクタ ( すべての要素に同じ値を設定 )
	inline Vector2::Vector2( lnfloat a_ )
	{
		x = y = a_;
	}

    // コピーコンストラクタ
	inline Vector2::Vector2( const Vector2& vec_ )
	{
		x = vec_.x;
		y = vec_.y;
	}

	// 値の設定
	inline void Vector2::set( lnfloat a_ )
	{
		x = y = a_;
	}
	inline void Vector2::set( lnfloat x_, lnfloat y_ )
	{
		x = x_;
		y = y_;
	}

    // ベクトルの長さを返す
    inline lnfloat Vector2::getLength() const
    {
        return Asm::sqrt( x * x + y * y );
    }

    // ベクトルの長さの2乗を返す
	inline lnfloat Vector2::getSquareLength() const
    {
        return ( x * x + y * y );
    }

    // ベクトルを正規化する
    inline void Vector2::normalize()
    {
        lnfloat t = x * x + y * y;
        t = 1.0f / Asm::sqrt( t );
		x *= t;
		y *= t;
    }

    // ベクトルを正規化する
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

    // 比較

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