//=============================================================================
//【 Vector3 】
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
// ■ Vector3 クラス
//-----------------------------------------------------------------------------
///**
//  @brief      3次元ベクトルのクラス
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

	/// デフォルトコンストラクタ ( すべての要素を 0.0 で初期化 )
	Vector3();

    /// コンストラクタ
	Vector3( lnfloat x_, lnfloat y_, lnfloat z_ );

	/// コンストラクタ ( すべての要素に同じ値を設定 )
	explicit Vector3( lnfloat a_ );

    /// ( vec_.x, vec_.y, z)
	explicit Vector3( const Vector2& vec_, lnfloat z_ );

	/// コピーコンストラクタ
	Vector3( const Vector3& vec_ );

public:

	/// 値の設定
	void set( lnfloat a_ );

	/// 値の設定
	void set( lnfloat x_, lnfloat y_, lnfloat z_ );

	//---------------------------------------------------------------------
	///**
    //	@brief      ベクトルの長さの取得
    //*/
	//---------------------------------------------------------------------
	lnfloat getLength() const;

    //---------------------------------------------------------------------
	///**
    //	@brief      ベクトルの長さの取得 (値代用)
    //
    //  @param[in]  default_ : 長さが計算できない場合、代わりに返す値
    //*/
	//---------------------------------------------------------------------
	lnfloat getLength( lnfloat default_ ) const;

	//---------------------------------------------------------------------
	///**
	//	@brief		ベクトルの長さの2乗を返す
	//*/
	//---------------------------------------------------------------------
	lnfloat getSquareLength() const;

	//---------------------------------------------------------------------
	///**
	//	@brief		ベクトルを正規化する
	//*/
	//---------------------------------------------------------------------
	void normalize();

    //---------------------------------------------------------------------
    ///**
    //  @brief      ベクトルを正規化する
    //
    //  @param[in]  def_vec_ : vec_ の長さが 0 だった場合に設定するベクトル
    //*/
    //---------------------------------------------------------------------
    void normalize( const Vector3& def_vec_ );

    //---------------------------------------------------------------------
    ///**
	//	@brief		指定された行列を使って座標変換する
    //
    //  @par
    //              ベクトルを (x, y, z, 1.0) として座標変換を行い、
    //              結果を w で除算します。
	//*/
	//---------------------------------------------------------------------
	void transform( const Matrix& matrix_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      ベクトルの長さが 0 かを判定する
	//*/
	//---------------------------------------------------------------------
	bool isZero() const { return ( x == 0 && y == 0 && z == 0 ); }

    //---------------------------------------------------------------------
	///**
	//  @brief      NaN チェック
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

	/// 標準出力へ出力する
	void dump( const lnChar* str_ = NULL ) const;

public:

    //-------------------------------------------------------------------------
	// ▼ static

    //---------------------------------------------------------------------
    ///**
    //  @brief      3D ベクトルの長さを返す
    //
    //  @param[in]  vec_ : 処理の基になる Vector3
    //*/
    //---------------------------------------------------------------------
	//lnfloat length( const Vector3& vec_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      3D ベクトルの長さの2乗を返す
    //
    //  @param[in]  vec_ : 処理の基になる Vector3
    //
    //  @par
    //              Vector3Length() の中には平方根の計算 ( sqrt() ) があり、
    //              この計算は時間のかかる処理です。
    //              ゲームの中で多くのオブジェクトどうしの距離の比較をする等、
    //              Vector3Length() を大量に呼ぶ場合、代わりにこの関数を使う事で
    //              高速化を図ることができます。
    //*/
    //---------------------------------------------------------------------
	static lnfloat squareLength( const Vector3& vec_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      3D ベクトルの正規化したベクトルを返す
    //
    //  @param[out] out_ : 演算結果を格納する Vector3 へのポインタ
    //  @param[in]  vec_ : 処理の基になる Vector3
    //*/
    //---------------------------------------------------------------------
	static void normalize( Vector3* out_, const Vector3& vec_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      2D ベクトルの正規化したベクトルを返す
    //
    //  @param[out] out_     : 演算結果を格納する Vector2 へのポインタ
    //  @param[in]  vec_     : 処理の基になる Vector2
    //  @param[in]  def_vec_ : vec_ の長さが 0 だった場合に返すベクトル
    //*/
    //---------------------------------------------------------------------
	static void normalize( Vector3* out_, const Vector3& vec_, const Vector3& def_vec_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      2 つの 3D ベクトルの内積を計算する
    //
    //  @param[in]  vec1_ : 処理の基になる Vector3
    //  @param[in]  vec2_ : 処理の基になる Vector3
    //*/
    //---------------------------------------------------------------------
	static lnfloat dot( const Vector3& vec1_, const Vector3& vec2_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      2 つの 3D ベクトルの外積を計算する
    //
    //  @param[out] out_  : 演算結果を格納する Vector3 へのポインタ
    //  @param[in]  vec1_ : 処理の基になる Vector3
    //  @param[in]  vec2_ : 処理の基になる Vector3
    //*/
    //---------------------------------------------------------------------
	static void cross( Vector3* out_, const Vector3& vec1_, const Vector3& vec2_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      指定された法線で反射させたベクトルを計算する
    //
    //  @param[out] out_    : 演算結果を格納する Vector3 へのポインタ
    //  @param[in]  vec_    : 処理の基になる Vector3
    //  @param[in]  normal_ : 面方向を表す法線 Vector3
    //*/
    //---------------------------------------------------------------------
	static void reflect( Vector3* out_, const Vector3& vec_, const Vector3& normal_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      指定された法線と方向から滑りベクトルを計算する
    //
    //  @param[out] out_    : 演算結果を格納する Vector3 へのポインタ
    //  @param[in]  vec_    : 処理の基になる Vector3
    //  @param[in]  normal_ : 面方向を表す法線 Vector3
    //*/
    //---------------------------------------------------------------------
	static void slide( Vector3* out_, const Vector3& vec_, const Vector3& normal_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      2 つの 3D ベクトル間の線形補間を実行する
    //
    //  @param[out] out_  : 演算結果を格納する Vector3 へのポインタ
    //  @param[in]  vec1_ : 処理の基になる Vector3 ( t_ = 0.0 のときの値 )
    //  @param[in]  vec2_ : 処理の基になる Vector3 ( t_ = 1.0 のときの値 )
    //  @param[in]  t_    : 補間値 ( 0.0 ～ 1.0 )
    //*/
    //---------------------------------------------------------------------
	static void lerp( Vector3* out_, const Vector3& vec1_, const Vector3& vec2_, lnfloat t_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      指定された 3D ベクトルを使用して、Catmull-Rom 補間を実行する
    //
    //  @param[out] out_  : 演算結果を格納する Vector3 へのポインタ
    //  @param[in]  vec1_ : 処理の基になる Vector3
    //  @param[in]  vec2_ : 処理の基になる Vector3 ( t_ = 0.0 のときの値 )
    //  @param[in]  vec3_ : 処理の基になる Vector3 ( t_ = 1.0 のときの値 )
    //  @param[in]  vec4_ : 処理の基になる Vector3
    //  @param[in]  t_    : 補間値 ( 0.0 ～ 1.0 )
    //*/
    //---------------------------------------------------------------------
	static void catmullRom( Vector3* out_, const Vector3& vec1_, const Vector3& vec2_, const Vector3& vec3_, const Vector3& vec4_, lnfloat t_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      指定された行列を使って座標変換する
    //
    //  @param[out] out_ : 演算結果を格納する Vector3 へのポインタ
    //  @param[in]  vec_ : 処理の基になる Vector3
    //  @param[in]  mat_ : 処理の基になる Matrix
    //
    //  @par
    //              ベクトルを ( x, y, z, 1 ) として座標変換します。
    //              結果の w は出力されません。<br>
    //              結果を w = 1 に射影する ( x y z を w で除算する ) 場合は
    //              Vector3TransformCoord() を使ってください。
    //*/
    //---------------------------------------------------------------------
	static void transform( Vector3* out_, const Vector3& vec_, const Matrix& mat_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      指定された行列を使って座標変換し、結果を w = 1 に射影する
    //
    //  @param[out] out_ : 演算結果を格納する Vector3 へのポインタ
    //  @param[in]  vec_ : 処理の基になる Vector3
    //  @param[in]  mat_ : 処理の基になる Matrix
    //
    //  @par
    //              ベクトルを ( x, y, z, 1 ) として座標変換し、
    //              結果を w = 1 に射影 ( x y z を w で除算する ) します。
    //*/
    //---------------------------------------------------------------------
	static void transformCoord( Vector3* out_, const Vector3& vec_, const Matrix& mat_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      行列の回転だけを使って座標変換する
    //
    //  @param[out] out_ : 演算結果を格納する Vector3 へのポインタ
    //  @param[in]  vec_ : 処理の基になる Vector3
    //  @param[in]  mat_ : 処理の基になる Matrix
    //*/
    //---------------------------------------------------------------------
	static void rotate( Vector3* out_, const Vector3& vec_, const Matrix& mat_ );



public:

#if LNOTE_INCLUDED_DIRECTX_HEADER

    // D3DXVECTOR3 に変換
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

	// デフォルトコンストラクタ ( すべての要素を 0.0 で初期化 )
	inline Vector3::Vector3()
	{
		x = y = z = 0.0f;
	}

	// コンストラクタ
	inline Vector3::Vector3( lnfloat x_, lnfloat y_, lnfloat z_ )
	{
		x = x_;
		y = y_;
		z = z_;
	}

	// コンストラクタ ( すべての要素に同じ値を設定 )
	inline Vector3::Vector3( lnfloat a_ )
	{
		x = y = z = a_;
	}

    

    // コピーコンストラクタ
	inline Vector3::Vector3( const Vector3& vec_ )
	{
		x = vec_.x;
		y = vec_.y;
		z = vec_.z;
	}

	// 値の設定
	inline void Vector3::set( lnfloat a_ )
	{
		x = y = z = a_;
	}

	// 値の設定
	inline void Vector3::set( lnfloat x_, lnfloat y_, lnfloat z_ )
	{
		x = x_;
		y = y_;
		z = z_;
	}

	//---------------------------------------------------------------------
	// ● ベクトルの長さの取得
	//---------------------------------------------------------------------
	inline lnfloat Vector3::getLength() const
	{
        return Asm::sqrt( x * x + y * y + z * z );
	}

    //---------------------------------------------------------------------
	// ● ベクトルの長さの取得
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
	// ● 3D ベクトルの長さの2乗を返す
	//---------------------------------------------------------------------
	inline lnfloat Vector3::getSquareLength() const
	{
		return ( x * x + y * y + z * z );
	}

	//---------------------------------------------------------------------
	/// ● ベクトルを正規化する
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

    // transform の定義は Matrix

	//---------------------------------------------------------------------
	// ● 演算子オーバーロード
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

    // 四則演算
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

    // 比較
	inline bool Vector3::operator == ( const Vector3& vec_ ) const
	{
		return ( x == vec_.x && y == vec_.y && z == vec_.z );
	}

	inline bool Vector3::operator != ( const Vector3& vec_ ) const
	{
		return ( x != vec_.x || y != vec_.y || z != vec_.z );
	}

    // コンソールに出力する
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