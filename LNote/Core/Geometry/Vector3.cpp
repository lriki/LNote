//=============================================================================
//【 Vector3 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Matrix.h"
#include "Vector2.h"
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
// ■ Vector3 クラス
//=============================================================================

    const Vector3   Vector3::ZERO( 0, 0, 0 );
    const Vector3   Vector3::UNIT_X( 1, 0, 0 );
    const Vector3   Vector3::UNIT_Y( 0, 1, 0 );
    const Vector3   Vector3::UNIT_Z( 0, 0, 1 );
    const Vector3   Vector3::UNIT_SCALE( 1, 1, 1 );

    //---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    Vector3::Vector3( const Vector2& vec_, lnfloat z_ )
    {
        x = vec_.x;
		y = vec_.y;
		z = z_;
    }

    //---------------------------------------------------------------------
	// ● 指定された行列を使って座標変換する
	//---------------------------------------------------------------------
    void Vector3::transform( const Matrix& matrix_ )
	{
		// ( x, y, z, 1 ) として計算する
		lnfloat tx = x;
		lnfloat ty = y;
        lnfloat w = 1.0f / ( tx * matrix_.m03 + ty * matrix_.m13 + z * matrix_.m23 + matrix_.m33 );
		x = ( tx * matrix_.m00 + ty * matrix_.m10 + z * matrix_.m20 + matrix_.m30 ) * w;	// 一番後ろは普通はw * matrix_._03だけど、
		y = ( tx * matrix_.m01 + ty * matrix_.m11 + z * matrix_.m21 + matrix_.m31 ) * w;	// ここでは w=1 として計算
		z = ( tx * matrix_.m02 + ty * matrix_.m12 + z * matrix_.m22 + matrix_.m32 ) * w;
	}

    //---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    Vector2 Vector3::getXY() const
    {
        return Vector2( x, y );
    }

    //-------------------------------------------------------------------------
	// ▼ static

    //---------------------------------------------------------------------
	// ● 3D ベクトルの長さの取得
	//---------------------------------------------------------------------
	//lnfloat Vector3Length( const Vector3& vec_ )
	//{
 //       return Asm::sqrt( vec_.x * vec_.x + vec_.y * vec_.y + vec_.z * vec_.z );
	//}

	//---------------------------------------------------------------------
	// ● 3D ベクトルの長さの2乗を返す
	//---------------------------------------------------------------------
	lnfloat Vector3::squareLength( const Vector3& vec_ )
	{
		return vec_.x * vec_.x + vec_.y * vec_.y + vec_.z * vec_.z;
	}

	//---------------------------------------------------------------------
	// ● 3D ベクトルの正規化したベクトルを返す
	//---------------------------------------------------------------------
    void Vector3::normalize( Vector3* out_, const Vector3& vec_ )
	{
		lnfloat t = 1.0f / Asm::sqrt( vec_.x * vec_.x + vec_.y * vec_.y + vec_.z * vec_.z );
        out_->x = vec_.x * t;
        out_->y = vec_.y * t;
        out_->z = vec_.z * t;
	}

    void Vector3::normalize( Vector3* out_, const Vector3& vec_, const Vector3& def_vec_ )
    {
        lnfloat t = vec_.x * vec_.x + vec_.y * vec_.y + vec_.z * vec_.z;
        if ( t == 0 )
        {
            *out_ = def_vec_;
            return;
        }
        t = 1.0f / Asm::sqrt( t );
        out_->x = vec_.x * t;
        out_->y = vec_.y * t;
        out_->z = vec_.z * t;
    }

	//---------------------------------------------------------------------
	// ● 2 つの 3D ベクトルの内積を計算する
	//---------------------------------------------------------------------
    lnfloat Vector3::dot( const Vector3& vec1_, const Vector3& vec2_ )
	{
		return ( vec1_.x * vec2_.x + vec1_.y * vec2_.y + vec1_.z * vec2_.z );
	}

	//---------------------------------------------------------------------
	// ● 2 つの 3D ベクトルの外積を計算する
	//---------------------------------------------------------------------
	void Vector3::cross( Vector3* out_, const Vector3& vec1_, const Vector3& vec2_ )
	{
        *out_ = Vector3(
            ( vec1_.y * vec2_.z ) - ( vec1_.z * vec2_.y ),
			( vec1_.z * vec2_.x ) - ( vec1_.x * vec2_.z ),
			( vec1_.x * vec2_.y ) - ( vec1_.y * vec2_.x ) );
	}

	//---------------------------------------------------------------------
	// ● vec_ を normal_ の法線で反射させたベクトルを計算する
	//---------------------------------------------------------------------
	void Vector3::reflect( Vector3* out_, const Vector3& vec_, const Vector3& normal_ )
	{
		lnfloat t = -( normal_.x * vec_.x + normal_.y * vec_.y + normal_.z * vec_.z ) / ( normal_.x * normal_.x + normal_.y * normal_.y + normal_.z * normal_.z );
		*out_ = Vector3(
			vec_.x + t * normal_.x * 2.0f,
			vec_.y + t * normal_.y * 2.0f,
			vec_.z + t * normal_.z * 2.0f );
	}

	//---------------------------------------------------------------------
	// ● vec_ と normal_ の滑りベクトルを計算する
	//---------------------------------------------------------------------
	void Vector3::slide( Vector3* out_, const Vector3& vec_, const Vector3& normal_ )
	{
		lnfloat t = -( normal_.x * vec_.x + normal_.y * vec_.y + normal_.z * vec_.z ) / ( normal_.x * normal_.x + normal_.y * normal_.y + normal_.z * normal_.z );
        *out_ = Vector3(
			vec_.x + t * normal_.x,
			vec_.y + t * normal_.y,
			vec_.z + t * normal_.z );
	}

	//---------------------------------------------------------------------
	// ● 2 つの 3D ベクトル間の線形補間を実行する
	//---------------------------------------------------------------------
	void Vector3::lerp( Vector3* out_, const Vector3& vec1_, const Vector3& vec2_, lnfloat t_ )
	{
		*out_ = Vector3(
            LMath::linear( vec1_.x, vec2_.x, t_ ),
			LMath::linear( vec1_.y, vec2_.y, t_ ),
			LMath::linear( vec1_.z, vec2_.z, t_ ) );
	}

	//---------------------------------------------------------------------
	// ● 指定された 3D ベクトルを使用して、Catmull-Rom 補間を実行する
	//---------------------------------------------------------------------
	void Vector3::catmullRom( Vector3* out_, const Vector3& vec1_, const Vector3& vec2_, const Vector3& vec3_, const Vector3& vec4_, lnfloat t_ )
	{
		*out_ = Vector3(
			LMath::catmullRom( vec1_.x, vec2_.x, vec3_.x, vec4_.x, t_ ),
			LMath::catmullRom( vec1_.y, vec2_.y, vec3_.y, vec4_.y, t_ ),
			LMath::catmullRom( vec1_.z, vec2_.z, vec3_.z, vec4_.z, t_ ) );
	}

    //---------------------------------------------------------------------
    // ● 指定された行列を使って座標変換する
	//---------------------------------------------------------------------
    void Vector3::transform( Vector3* out_, const Vector3& v_, const Matrix& m_ )
    {
        Vector3 vec;
	    vec.x = v_.x * m_.m00 + v_.y * m_.m10 + v_.z * m_.m20 + m_.m30;
	    vec.y = v_.x * m_.m01 + v_.y * m_.m11 + v_.z * m_.m21 + m_.m31;
	    vec.z = v_.x * m_.m02 + v_.y * m_.m12 + v_.z * m_.m22 + m_.m32;
        *out_ = vec;
    }

    //---------------------------------------------------------------------
    // ● 指定された行列を使って座標変換し、結果を w = 1 に射影する
	//---------------------------------------------------------------------
    void Vector3::transformCoord( Vector3* out_, const Vector3& v_, const Matrix& m_ )
    {
        lnfloat rw = 1.0f / ( v_.x * m_.m03 + v_.y * m_.m13 + v_.z * m_.m23 + m_.m33 );
        Vector3 vec;
        vec.x = ( v_.x * m_.m00 + v_.y * m_.m10 + v_.z * m_.m20 + m_.m30 ) * rw;
	    vec.y = ( v_.x * m_.m01 + v_.y * m_.m11 + v_.z * m_.m21 + m_.m31 ) * rw;
	    vec.z = ( v_.x * m_.m02 + v_.y * m_.m12 + v_.z * m_.m22 + m_.m32 ) * rw;
        *out_ = vec;
    }

    //---------------------------------------------------------------------
    // ● 行列の回転だけを使って座標変換する
    //---------------------------------------------------------------------
    void Vector3::rotate( Vector3* out_, const Vector3& vec_, const Matrix& mat_ )
    {
        Vector3	tmp;
        tmp.x = vec_.x * mat_.m00 + vec_.y * mat_.m10 + vec_.z * mat_.m20;
        tmp.y = vec_.x * mat_.m01 + vec_.y * mat_.m11 + vec_.z * mat_.m21;
	    tmp.z = vec_.x * mat_.m02 + vec_.y * mat_.m12 + vec_.z * mat_.m22;
	    *out_ = tmp;
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Geometry
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================