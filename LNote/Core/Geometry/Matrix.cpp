//=============================================================================
//【 Matrix 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Common.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "SQTTransform.h"
#include "Matrix.h"

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
// ■ Matrix クラス
//=============================================================================

    const Matrix Matrix::IDENTITY = Matrix();

	// デフォルトコンストラクタ ( 単位行列の作成 )
	Matrix::Matrix()
	{
		m00 = m11 = m22 = m33 = 1.0f;
		m01 = m02 = m03 = 0.0f;
		m10 = m12 = m13 = 0.0f;
		m20 = m21 = m23 = 0.0f;
		m30 = m31 = m32 = 0.0f;
	}

	// コピーコンストラクタ
	Matrix::Matrix( const Matrix& mat_ )
	{
		memcpy( this, &mat_, sizeof( Matrix ) );
	}

	// コンストラクタ
	Matrix::Matrix(
		lnfloat m00_, lnfloat m01_, lnfloat m02_, lnfloat m03_,
		lnfloat m10_, lnfloat m11_, lnfloat m12_, lnfloat m13_,
		lnfloat m20_, lnfloat m21_, lnfloat m22_, lnfloat m23_,
		lnfloat m30_, lnfloat m31_, lnfloat m32_, lnfloat m33_ )
	{
		m00 = m00_; m01 = m01_; m02 = m02_; m03 = m03_;
		m10 = m10_; m11 = m11_; m12 = m12_; m13 = m13_;
		m20 = m20_; m21 = m21_; m22 = m22_; m23 = m23_;
		m30 = m30_; m31 = m31_; m32 = m32_; m33 = m33_;
	}

    /// コンストラクタ ( クォータニオンから作成する )
	Matrix::Matrix( const Quaternion& q_ )
    {
        /*
        float qw, qx, qy, qz;
    float x2, y2, z2;
    float xy, yz, zx;
    float wx, wy, wz;

    qw = lpQ.w; qx = lpQ.x; qy = lpQ.y; qz = lpQ.z;

    x2 = 2.0f * qx * qx;
    y2 = 2.0f * qy * qy;
    z2 = 2.0f * qz * qz;

    xy = 2.0f * qx * qy;
    yz = 2.0f * qy * qz;
    zx = 2.0f * qz * qx;
        
    wx = 2.0f * qw * qx;
    wy = 2.0f * qw * qy;
    wz = 2.0f * qw * qz;

    m00 = 1.0f - y2 - z2;
    m01 = xy - wz;
    m02 = zx + wy;
    m03 = 0.0f;

    m10 = xy + wz;
    m11 = 1.0f - z2 - x2;
    m12 = yz - wx;
    m13 = 0.0f;

    m20 = zx - wy;
    m21 = yz + wx;
    m22 = 1.0f - x2 - y2;
    m23 = 0.0f;

    m03 = m13 = m23 = m30 = m31 = m32 = 0.0f;
        m33 = 1.0f;
        */
        
        m00 = 1.0f - 2.0f * q_.y * q_.y - 2.0f * q_.z * q_.z;
        m01 = 2.0f * q_.x * q_.y + 2.0f * q_.w * q_.z;
        m02 = 2.0f * q_.x * q_.z - 2.0f * q_.w * q_.y;

        m10 = 2.0f * q_.x * q_.y - 2.0f * q_.w * q_.z;
        m11 = 1.0f - 2.0f * q_.x * q_.x - 2.0f * q_.z * q_.z;
        m12 = 2.0f * q_.y * q_.z + 2.0f * q_.w * q_.x;

        m20 = 2.0f * q_.x * q_.z + 2.0f * q_.w * q_.y;
        m21 = 2.0f * q_.y * q_.z - 2.0f * q_.w * q_.x;
        m22 = 1.0f - 2.0f * q_.x * q_.x - 2.0f * q_.y * q_.y;

        m03 = m13 = m23 = m30 = m31 = m32 = 0.0f;
        m33 = 1.0f;
        
    }

    /// コンストラクタ (SQTTransform から作成する)
	Matrix::Matrix( const SQTTransform& t_ )
    {
        //this->scaling( t_.Scale );
        Matrix::scaling( this, t_.Scale );
        this->rotationQuaternion( t_.Rotation );
        this->translation( t_.Translation );
    }

    // 右方向の取得
    const Vector3& Matrix::getRight() const { 
    	return (Vector3&)m00; }

    // 上方向の取得
    const Vector3& Matrix::getUp() const { 
    	return (Vector3&)m10; }

    // 正面方向の取得
    const Vector3& Matrix::getFront() const { 
    	return (Vector3&)m20; }

    // 位置の取得
    const Vector3& Matrix::getPosition() const { 
    	return (Vector3&)m30; }

    //---------------------------------------------------------------------
	// ● 単位行列を作成する
	//---------------------------------------------------------------------
    void Matrix::identity()
    {
        m00 = m11 = m22 = m33 = 1.0f;
		m01 = m02 = m03 = 0.0f;
		m10 = m12 = m13 = 0.0f;
		m20 = m21 = m23 = 0.0f;
		m30 = m31 = m32 = 0.0f;
    }

    //---------------------------------------------------------------------
	// ● 単位行列かを判定する
	//---------------------------------------------------------------------
    bool Matrix::isIdentity() const
    {
        static const Matrix m;
        return ( memcmp( this, &m, sizeof( Matrix ) ) == 0 );//( (*this) == m );
    }

    //---------------------------------------------------------------------
	// ● 値を設定する
    //---------------------------------------------------------------------
    void Matrix::set(
		lnfloat m00_, lnfloat m01_, lnfloat m02_, lnfloat m03_,
		lnfloat m10_, lnfloat m11_, lnfloat m12_, lnfloat m13_,
		lnfloat m20_, lnfloat m21_, lnfloat m22_, lnfloat m23_,
		lnfloat m30_, lnfloat m31_, lnfloat m32_, lnfloat m33_ )
    {
		m00 = m00_; m01 = m01_; m02 = m02_; m03 = m03_;
		m10 = m10_; m11 = m11_; m12 = m12_; m13 = m13_;
		m20 = m20_; m21 = m21_; m22 = m22_; m23 = m23_;
		m30 = m30_; m31 = m31_; m32 = m32_; m33 = m33_;
	}

    //---------------------------------------------------------------------
	// ● 指定された行列から回転要素だけを取り出して設定する
	//---------------------------------------------------------------------
    void Matrix::setRotateMatrix( const Matrix& matrix_ )
    {
        m00 = matrix_.m00; m01 = matrix_.m01; m02 = matrix_.m02; 
		m10 = matrix_.m10; m11 = matrix_.m11; m12 = matrix_.m12;
		m20 = matrix_.m20; m21 = matrix_.m21; m22 = matrix_.m22;
        m03 = m13 = m23 = m30 = m31 = m32 = 0.0f;
        m33 = 1.0f;
    }

    //---------------------------------------------------------------------
	// ● 行列を平行移動する
	//---------------------------------------------------------------------
	void Matrix::translation( lnfloat x_, lnfloat y_, lnfloat z_ )
	{
		/* 普通の行列計算
		m00, m01, m02, m03   1, 0, 0, 0
		m10, m11, m12, m13   0, 1, 0, 0
		m20, m21, m22, m23 * 0, 0, 1, 0
		m30, m31, m32, m33   x, y, z, 1
		*/
		/* 計算イメージ
		m00 = m00 * 1 + m01 * 0 + m02 * 0 + m03 * x;
		m01 = m00 * 0 + m01 * 1 + m02 * 0 + m03 * y;
		m02 = m00 * 0 + m01 * 0 + m02 * 1 + m03 * z;
		m03 = m00 * 0 + m01 * 0 + m02 * 0 + m03 * 1;

		m10 = m10 * 1 + m11 * 0 + m12 * 0 + m13 * x;
		m11 = m10 * 0 + m11 * 1 + m12 * 0 + m13 * y;
		m12 = m10 * 0 + m11 * 0 + m12 * 1 + m13 * z;
		m13 = m10 * 0 + m11 * 0 + m12 * 0 + m13 * 1;

		m20 = m20 * 1 + m21 * 0 + m22 * 0 + m23 * x;
		m21 = m20 * 0 + m21 * 1 + m22 * 0 + m23 * y;
		m22 = m20 * 0 + m21 * 0 + m22 * 1 + m23 * z;
		m23 = m20 * 0 + m21 * 0 + m22 * 0 + m23 * 1;

		m30 = m30 * 1 + m31 * 0 + m32 * 0 + m33 * x;
		m31 = m30 * 0 + m31 * 1 + m32 * 0 + m33 * y;
		m32 = m30 * 0 + m31 * 0 + m32 * 1 + m33 * z;
		m33 = m30 * 0 + m31 * 0 + m32 * 0 + m33 * 1;
		*/
		/* 正しく計算できるようにしたもの
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 * 1 + mx1 * 0 + mx2 * 0 + m03 * x;
		m01 = mx0 * 0 + mx1 * 1 + mx2 * 0 + m03 * y;
		m02 = mx0 * 0 + mx1 * 0 + mx2 * 1 + m03 * z;
		m03 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m03 * 1;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * 1 + mx1 * 0 + mx2 * 0 + m13 * x;
		m11 = mx0 * 0 + mx1 * 1 + mx2 * 0 + m13 * y;
		m12 = mx0 * 0 + mx1 * 0 + mx2 * 1 + m13 * z;
		m13 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m13 * 1;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * 1 + mx1 * 0 + mx2 * 0 + m23 * x;
		m21 = mx0 * 0 + mx1 * 1 + mx2 * 0 + m23 * y;
		m22 = mx0 * 0 + mx1 * 0 + mx2 * 1 + m23 * z;
		m23 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m23 * 1;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * 1 + mx1 * 0 + mx2 * 0 + m33 * x;
		m31 = mx0 * 0 + mx1 * 1 + mx2 * 0 + m33 * y;
		m32 = mx0 * 0 + mx1 * 0 + mx2 * 1 + m33 * z;
		m33 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m33 * 1;
		*/
		/* 単純に * 0 とかの無駄なところを切る
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 + m03 * x;
		m01 = mx1 + m03 * y;
		m02 = mx2 + m03 * z;
		m03 = m03;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 + m13 * x;
		m11 = mx1 + m13 * y;
		m12 = mx2 + m13 * z;
		m13 = m13;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 + m23 * x;
		m21 = mx1 + m23 * y;
		m22 = mx2 + m23 * z;
		m23 = m23;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 + m33 * x;
		m31 = mx1 + m33 * y;
		m32 = mx2 + m33 * z;
		m33 = m33;
		*/
		/* 自分自身を代入しているところを切る
		m00 = m00 + m03 * x;
		m01 = m01 + m03 * y;
		m02 = m02 + m03 * z;

		m10 = m10 + m13 * x;
		m11 = m11 + m13 * y;
		m12 = m12 + m13 * z;

		m20 = m20 + m23 * x;
		m21 = m21 + m23 * y;
		m22 = m22 + m23 * z;

		m30 = m30 + m33 * x;
		m31 = m31 + m33 * y;
		m32 = m32 + m33 * z;
		*/
		/* += にできるところを直す
		m00 += m03 * x;
		m01 += m03 * y;
		m02 += m03 * z;

		m10 += m13 * x;
		m11 += m13 * y;
		m12 += m13 * z;

		m20 += m23 * x;
		m21 += m23 * y;
		m22 += m23 * z;

		m30 += m33 * x;
		m31 += m33 * y;
		m32 += m33 * z;
		*/
		// m03 m13 m23 = 0  m33 = 1  とした場合
		m30 += x_;
		m31 += y_;
		m32 += z_;
	}

	void Matrix::translation( const Vector3& vec_ )
	{
		m30 += vec_.x;
		m31 += vec_.y;
		m32 += vec_.z;
	}

    //---------------------------------------------------------------------
	// ● X 軸を回転軸にして行列を回転する ( ラジアン単位 )
	//---------------------------------------------------------------------
	void Matrix::rotationX( lnfloat r_ )
	{
		lnfloat c, s;
        Asm::sincos( r_, &s, &c );

		/* 普通の行列計算
		m00, m01, m02, m03   1, 0, 0, 0
		m10, m11, m12, m13   0, c,-s, 0
		m20, m21, m22, m23 * 0, s, c, 0
		m30, m31, m32, m33   0, 0, 0, 1
		*/
		/* 計算イメージ
		m00 = m00 * 1 + m01 * 0 + m02 * 0 + m03 * 0;
		m01 = m00 * 0 + m01 * c + m02 * s + m03 * 0;
		m02 = m00 * 0 + m01 *-s + m02 * c + m03 * 0;
		m03 = m00 * 0 + m01 * 0 + m02 * 0 + m03 * 1;

		m10 = m10 * 1 + m11 * 0 + m12 * 0 + m13 * 0;
		m11 = m10 * 0 + m11 * c + m12 * s + m13 * 0;
		m12 = m10 * 0 + m11 *-s + m12 * c + m13 * 0;
		m13 = m10 * 0 + m11 * 0 + m12 * 0 + m13 * 1;

		m20 = m20 * 1 + m21 * 0 + m22 * 0 + m23 * 0;
		m21 = m20 * 0 + m21 * c + m22 * s + m23 * 0;
		m22 = m20 * 0 + m21 *-s + m22 * c + m23 * 0;
		m23 = m20 * 0 + m21 * 0 + m22 * 0 + m23 * 1;

		m30 = m30 * 1 + m31 * 0 + m32 * 0 + m33 * 0;
		m31 = m30 * 0 + m31 * c + m32 * s + m33 * 0;
		m32 = m30 * 0 + m31 *-s + m32 * c + m33 * 0;
		m33 = m30 * 0 + m31 * 0 + m32 * 0 + m33 * 1;
		*/
		/* 正しく計算できるようにしたもの
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 * 1 + mx1 * 0 + mx2 * 0 + m03 * 0;
		m01 = mx0 * 0 + mx1 * c + mx2 * s + m03 * 0;
		m02 = mx0 * 0 + mx1 *-s + mx2 * c + m03 * 0;
		m03 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m03 * 1;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * 1 + mx1 * 0 + mx2 * 0 + m13 * 0;
		m11 = mx0 * 0 + mx1 * c + mx2 * s + m13 * 0;
		m12 = mx0 * 0 + mx1 *-s + mx2 * c + m13 * 0;
		m13 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m13 * 1;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * 1 + mx1 * 0 + mx2 * 0 + m23 * 0;
		m21 = mx0 * 0 + mx1 * c + mx2 * s + m23 * 0;
		m22 = mx0 * 0 + mx1 *-s + mx2 * c + m23 * 0;
		m23 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m23 * 1;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * 1 + mx1 * 0 + mx2 * 0 + m33 * 0;
		m31 = mx0 * 0 + mx1 * c + mx2 * s + m33 * 0;
		m32 = mx0 * 0 + mx1 *-s + mx2 * c + m33 * 0;
		m33 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m33 * 1;
		*/
		/* 単純に * 0 とかの無駄なところを切る
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0;
		m01 = mx1 * c + mx2 * s;
		m02 = mx1 *-s + mx2 * c;
		m03 = m03;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0;
		m11 = mx1 * c + mx2 * s;
		m12 = mx1 *-s + mx2 * c;
		m13 = m13;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0;
		m21 = mx1 * c + mx2 * s;
		m22 = mx1 *-s + mx2 * c;
		m23 = m23;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0;
		m31 = mx1 * c + mx2 * s;
		m32 = mx1 *-s + mx2 * c;
		m33 = m33;
		*/
		
		// 自分自身を代入しているところを切る
        lnfloat mx1 = m01;
		m01 = mx1 * c + m02 *-s;
		m02 = mx1 * s + m02 * c;

		mx1 = m11;
		m11 = mx1 * c + m12 *-s;
		m12 = mx1 * s + m12 * c;

		mx1 = m21;
		m21 = mx1 * c + m22 *-s;
		m22 = mx1 * s + m22 * c;

		mx1 = m31;
		m31 = mx1 * c + m32 *-s;
		m32 = mx1 * s + m32 * c;

        /* OpenGL
        lnfloat mx1 = m01;
		m01 = mx1 * c + m02 * s;
		m02 = mx1 *-s + m02 * c;

		mx1 = m11;
		m11 = mx1 * c + m12 * s;
		m12 = mx1 *-s + m12 * c;

		mx1 = m21;
		m21 = mx1 * c + m22 * s;
		m22 = mx1 *-s + m22 * c;

		mx1 = m31;
		m31 = mx1 * c + m32 * s;
		m32 = mx1 *-s + m32 * c;
        */
	}

    //---------------------------------------------------------------------
	// ● Y 軸を回転軸にして行列を回転する ( ラジアン単位 )
	//---------------------------------------------------------------------
	void Matrix::rotationY( lnfloat r_ )
	{
		lnfloat c, s;
        Asm::sincos( r_, &s, &c );

		/* 普通の行列計算
		m00, m01, m02, m03    c, 0, s, 0
		m10, m11, m12, m13    0, 1, 0, 0
		m20, m21, m22, m23 * -s, 0, c, 0
		m30, m31, m32, m33    0, 0, 0, 1
		*/
		/* 計算イメージ
		m00 = m00 * c + m01 * 0 + m02 *-s + m03 * 0;
		m01 = m00 * 0 + m01 * 1 + m02 * 0 + m03 * 0;
		m02 = m00 * s + m01 * 0 + m02 * c + m03 * 0;
		m03 = m00 * 0 + m01 * 0 + m02 * 0 + m03 * 1;

		m10 = m10 * c + m11 * 0 + m12 *-s + m13 * 0;
		m11 = m10 * 0 + m11 * 1 + m12 * 0 + m13 * 0;
		m12 = m10 * s + m11 * 0 + m12 * c + m13 * 0;
		m13 = m10 * 0 + m11 * 0 + m12 * 0 + m13 * 1;

		m20 = m20 * c + m21 * 0 + m22 *-s + m23 * 0;
		m21 = m20 * 0 + m21 * 1 + m22 * 0 + m23 * 0;
		m22 = m20 * s + m21 * 0 + m22 * c + m23 * 0;
		m23 = m20 * 0 + m21 * 0 + m22 * 0 + m23 * 1;

		m30 = m30 * c + m31 * 0 + m32 *-s + m33 * 0;
		m31 = m30 * 0 + m31 * 1 + m32 * 0 + m33 * 0;
		m32 = m30 * s + m31 * 0 + m32 * c + m33 * 0;
		m33 = m30 * 0 + m31 * 0 + m32 * 0 + m33 * 1;
		*/
		/* 正しく計算できるようにしたもの
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 * c + mx1 * 0 + mx2 *-s + m03 * 0;
		m01 = mx0 * 0 + mx1 * 1 + mx2 * 0 + m03 * 0;
		m02 = mx0 * s + mx1 * 0 + mx2 * c + m03 * 0;
		m03 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m03 * 1;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * c + mx1 * 0 + mx2 *-s + m13 * 0;
		m11 = mx0 * 0 + mx1 * 1 + mx2 * 0 + m13 * 0;
		m12 = mx0 * s + mx1 * 0 + mx2 * c + m13 * 0;
		m13 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m13 * 1;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * c + mx1 * 0 + mx2 *-s + m23 * 0;
		m21 = mx0 * 0 + mx1 * 1 + mx2 * 0 + m23 * 0;
		m22 = mx0 * s + mx1 * 0 + mx2 * c + m23 * 0;
		m23 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m23 * 1;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * c + mx1 * 0 + mx2 *-s + m33 * 0;
		m31 = mx0 * 0 + mx1 * 1 + mx2 * 0 + m33 * 0;
		m32 = mx0 * s + mx1 * 0 + mx2 * c + m33 * 0;
		m33 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m33 * 1;
		*/
		/* 単純に * 0 とかの無駄なところを切る
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 * c + mx2 *-s;
		m01 = mx1;
		m02 = mx0 * s + mx2 * c;
		m03 = m03;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * c + mx2 *-s;
		m11 = mx1;
		m12 = mx0 * s + mx2 * c;
		m13 = m13;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * c + mx2 *-s;
		m21 = mx1;
		m22 = mx0 * s + mx2 * c;
		m23 = mx3;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * c + mx2 *-s;
		m31 = mx1;
		m32 = mx0 * s + mx2 * c;
		m33 = m33;
		*/

		// 自分自身を代入しているところを切る
		lnfloat mx0 = m00;
		m00 = mx0 * c + m02 * s;
		m02 = mx0 *-s + m02 * c;

		mx0 = m10;
		m10 = mx0 * c + m12 * s;
		m12 = mx0 *-s + m12 * c;

		mx0 = m20;
		m20 = mx0 * c + m22 * s;
		m22 = mx0 *-s + m22 * c;

		mx0 = m30;
		m30 = mx0 * c + m32 * s;
		m32 = mx0 *-s + m32 * c;

        /* OpenGL
		lnfloat mx0 = m00;
		m00 = mx0 * c + m02 *-s;
		m02 = mx0 * s + m02 * c;

		mx0 = m10;
		m10 = mx0 * c + m12 *-s;
		m12 = mx0 * s + m12 * c;

		mx0 = m20;
		m20 = mx0 * c + m22 *-s;
		m22 = mx0 * s + m22 * c;

		mx0 = m30;
		m30 = mx0 * c + m32 *-s;
		m32 = mx0 * s + m32 * c;
        */
	}

    //---------------------------------------------------------------------
	// ● Z 軸を回転軸にして行列を回転する ( ラジアン単位 )
	//---------------------------------------------------------------------
	void Matrix::rotationZ( lnfloat r_ )
	{
		lnfloat c, s;
        Asm::sincos( r_, &s, &c );

		/* 基本の計算イメージ
		m00 = m00 * c + m01 * s + m02 * 0 + m03 * 0;
		m01 = m00 *-s + m01 * c + m02 * 0 + m03 * 0;
		m02 = m00 * 0 + m01 * 0 + m02 * 1 + m03 * 0;
		m03 = m00 * 0 + m01 * 0 + m02 * 0 + m03 * 1;

		m10 = m10 * c + m11 * s + m12 * 0 + m13 * 0;
		m11 = m10 *-s + m11 * c + m12 * 0 + m13 * 0;
		m12 = m10 * 0 + m11 * 0 + m12 * 1 + m13 * 0;
		m13 = m10 * 0 + m11 * 0 + m12 * 0 + m13 * 1;

		m20 = m20 * c + m21 * s + m22 * 0 + m23 * 0;
		m21 = m20 *-s + m21 * c + m22 * 0 + m23 * 0;
		m22 = m20 * 0 + m21 * 0 + m22 * 1 + m23 * 0;
		m23 = m20 * 0 + m21 * 0 + m22 * 0 + m23 * 1;

		m30 = m30 * c + m31 * s + m32 * 0 + m33 * 0;
		m31 = m30 *-s + m31 * c + m32 * 0 + m33 * 0;
		m32 = m30 * 0 + m31 * 0 + m32 * 1 + m33 * 0;
		m33 = m30 * 0 + m31 * 0 + m32 * 0 + m33 * 1;
		*/
		/* 正しく計算できるようにしたもの
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 * c + mx1 * s + mx2 * 0 + m03 * 0;
		m01 = mx0 *-s + mx1 * c + mx2 * 0 + m03 * 0;
		m02 = mx0 * 0 + mx1 * 0 + mx2 * 1 + m03 * 0;
		m03 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m03 * 1;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * c + mx1 * s + mx2 * 0 + m13 * 0;
		m11 = mx0 *-s + mx1 * c + mx2 * 0 + m13 * 0;
		m12 = mx0 * 0 + mx1 * 0 + mx2 * 1 + m13 * 0;
		m13 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m13 * 1;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * c + mx1 * s + mx2 * 0 + m23 * 0;
		m21 = mx0 *-s + mx1 * c + mx2 * 0 + m23 * 0;
		m22 = mx0 * 0 + mx1 * 0 + mx2 * 1 + m23 * 0;
		m23 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m23 * 1;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * c + mx1 * s + mx2 * 0 + m33 * 0;
		m31 = mx0 *-s + mx1 * c + mx2 * 0 + m33 * 0;
		m32 = mx0 * 0 + mx1 * 0 + mx2 * 1 + m33 * 0;
		m33 = mx0 * 0 + mx1 * 0 + mx2 * 0 + m33 * 1;
		*/
		/* 単純に * 0 とかの無駄なところを切る
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 * c + mx1 * s;
		m01 = mx0 *-s + mx1 * c;
		m02 = mx2;
		m03 = m03;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * c + mx1 * s;
		m11 = mx0 *-s + mx1 * c;
		m12 = mx2;
		m13 = m13;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * c + mx1 * s;
		m21 = mx0 *-s + mx1 * c;
		m22 = mx2;
		m23 = m23;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * c + mx1 * s;
		m31 = mx0 *-s + mx1 * c;
		m32 = mx2;
		m33 = m33;
		*/
		// 自分自身を代入しているところを切る
        lnfloat mx0 = m00;
		m00 = mx0 * c + m01 *-s;
		m01 = mx0 * s + m01 * c;

		mx0 = m10;
		m10 = mx0 * c + m11 *-s;
		m11 = mx0 * s + m11 * c;

		mx0 = m20;
		m20 = mx0 * c + m21 *-s;
		m21 = mx0 * s + m21 * c;

		mx0 = m30;
		m30 = mx0 * c + m31 *-s;
		m31 = mx0 * s + m31 * c;
        

        // OpenGL
        /* 
		lnfloat mx0 = m00;
		m00 = mx0 * c + m01 * s;
		m01 = mx0 *-s + m01 * c;

		mx0 = m10;
		m10 = mx0 * c + m11 * s;
		m11 = mx0 *-s + m11 * c;

		mx0 = m20;
		m20 = mx0 * c + m21 * s;
		m21 = mx0 *-s + m21 * c;

		mx0 = m30;
		m30 = mx0 * c + m31 * s;
		m31 = mx0 *-s + m31 * c;
        */
	}

    //---------------------------------------------------------------------
	// ● 回転順序を指定して行列を回転する ( ラジアン単位 )
	//---------------------------------------------------------------------
	void Matrix::rotation( lnfloat x_, lnfloat y_, lnfloat z_, LNRotOrder order_ )
	{
		switch ( order_ )
		{
			case LN_ROTORDER_XYZ: 
				rotationX( x_ ); rotationY( y_ ); rotationZ( z_ ); break;
			case LN_ROTORDER_XZY: 
				rotationX( x_ ); rotationZ( z_ ); rotationY( y_ ); break;
			case LN_ROTORDER_YXZ: 
				rotationY( y_ ); rotationX( x_ ); rotationZ( z_ ); break;
			case LN_ROTORDER_YZX: 
				rotationY( y_ ); rotationZ( z_ ); rotationX( x_ ); break;
			case LN_ROTORDER_ZXY: 
				rotationZ( z_ ); rotationX( x_ ); rotationY( y_ ); break;
			case LN_ROTORDER_ZYX: 
				rotationZ( z_ ); rotationY( y_ ); rotationX( x_ ); break;
		}
	}

    //---------------------------------------------------------------------
	// ● 行列を回転する ( ラジアン単位 )
	//---------------------------------------------------------------------
	void Matrix::rotation( const Vector3& vec_, LNRotOrder order_ )
    {
        switch ( order_ )
		{
			case LN_ROTORDER_XYZ: 
				rotationX( vec_.x ); rotationY( vec_.y ); rotationZ( vec_.z ); break;
			case LN_ROTORDER_XZY: 
				rotationX( vec_.x ); rotationZ( vec_.z ); rotationY( vec_.y ); break;
			case LN_ROTORDER_YXZ: 
				rotationY( vec_.y ); rotationX( vec_.x ); rotationZ( vec_.z ); break;
			case LN_ROTORDER_YZX: 
				rotationY( vec_.y ); rotationZ( vec_.z ); rotationX( vec_.x ); break;
			case LN_ROTORDER_ZXY: 
				rotationZ( vec_.z ); rotationX( vec_.x ); rotationY( vec_.y ); break;
			case LN_ROTORDER_ZYX: 
				rotationZ( vec_.z ); rotationY( vec_.y ); rotationX( vec_.x ); break;
		}
    }

    //---------------------------------------------------------------------
	// ● 任意の軸を回転軸にして行列を回転する ( ラジアン単位 )
	//---------------------------------------------------------------------
	void Matrix::rotationAxis( const Vector3& axis_, lnfloat r_ )
    {
        //Vector3& axis = Vector3( axis_ );
        Vector3 axis = axis_;
        axis.normalize();

        lnfloat s, c;
        Asm::sincos( r_, &s, &c );
		lnfloat mc = 1.0f - c;

		/* 計算イメージ
		_00 = ( axis_.x * axis_.x ) * mc + c;
		_01 = ( axis_.x * axis_.y ) * mc + ( axis_.z * s );
		_02 = ( axis_.x * axis_.z ) * mc - ( axis_.y * s );
		_03 = 0;

		_10 = ( axis_.x * axis_.y ) * mc - ( axis_.z * s );
		_11 = ( axis_.y * axis_.y ) * mc + c;
		_12 = ( axis_.y * axis_.z ) * mc + ( axis_.x * s );
		_13 = 0;

		_20 = ( axis_.x * axis_.z ) * mc + ( axis_.y * s );
		_21 = ( axis_.y * axis_.z ) * mc - ( axis_.x * s );
		_22 = ( axis_.z * axis_.z ) * mc + c;
		_23 = 0;

		_30 = _31 = _32 = 0;
		_33 = 1;

		m00 = m00 * _00 + m01 * _10 + m02 * _20 + m03 * _30;
		m01 = m00 * _01 + m01 * _11 + m02 * _21 + m03 * _31;
		m02 = m00 * _02 + m01 * _12 + m02 * _22 + m03 * _32;
		m03 = m00 * _03 + m01 * _13 + m02 * _23 + m03 * _33;

		m10 = m10 * _00 + m11 * _10 + m12 * _20 + m13 * _30;
		m11 = m10 * _01 + m11 * _11 + m12 * _21 + m13 * _31;
		m12 = m10 * _02 + m11 * _12 + m12 * _22 + m13 * _32;
		m13 = m10 * _03 + m11 * _13 + m12 * _23 + m13 * _33;

		m20 = m20 * _00 + m21 * _10 + m22 * _20 + m23 * _30;
		m21 = m20 * _01 + m21 * _11 + m22 * _21 + m23 * _31;
		m22 = m20 * _02 + m21 * _12 + m22 * _22 + m23 * _32;
		m23 = m20 * _03 + m21 * _13 + m22 * _23 + m23 * _33;

		m30 = m30 * _00 + m31 * _10 + m32 * _20 + m33 * _30;
		m31 = m30 * _01 + m31 * _11 + m32 * _21 + m33 * _31;
		m32 = m30 * _02 + m31 * _12 + m32 * _22 + m33 * _32;
		m33 = m30 * _03 + m31 * _13 + m32 * _23 + m33 * _33;
		*/

		/* 正しく計算できるようにしたもの
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 * _00 + mx1 * _10 + mx2 * _20 + m03 * _30;
		m01 = mx0 * _01 + mx1 * _11 + mx2 * _21 + m03 * _31;
		m02 = mx0 * _02 + mx1 * _12 + mx2 * _22 + m03 * _32;
		m03 = mx0 * _03 + mx1 * _13 + mx2 * _23 + m03 * _33;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * _00 + mx1 * _10 + mx2 * _20 + m13 * _30;
		m11 = mx0 * _01 + mx1 * _11 + mx2 * _21 + m13 * _31;
		m12 = mx0 * _02 + mx1 * _12 + mx2 * _22 + m13 * _32;
		m13 = mx0 * _03 + mx1 * _13 + mx2 * _23 + m13 * _33;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * _00 + mx1 * _10 + mx2 * _20 + m23 * _30;
		m21 = mx0 * _01 + mx1 * _11 + mx2 * _21 + m23 * _31;
		m22 = mx0 * _02 + mx1 * _12 + mx2 * _22 + m23 * _32;
		m23 = mx0 * _03 + mx1 * _13 + mx2 * _23 + m23 * _33;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * _00 + mx1 * _10 + mx2 * _20 + m33 * _30;
		m31 = mx0 * _01 + mx1 * _11 + mx2 * _21 + m33 * _31;
		m32 = mx0 * _02 + mx1 * _12 + mx2 * _22 + m33 * _32;
		m33 = mx0 * _03 + mx1 * _13 + mx2 * _23 + m33 * _33;
		*/

		/* 0 を乗算してるところと、*1 で自分自身代入しているところを切る
		lfloat mx0 = m00;
		lfloat mx1 = m01;
		lfloat mx2 = m02;
		m00 = mx0 * _00 + mx1 * _10 + mx2 * _20;
		m01 = mx0 * _01 + mx1 * _11 + mx2 * _21;
		m02 = mx0 * _02 + mx1 * _12 + mx2 * _22;

		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * _00 + mx1 * _10 + mx2 * _20;
		m11 = mx0 * _01 + mx1 * _11 + mx2 * _21;
		m12 = mx0 * _02 + mx1 * _12 + mx2 * _22;

		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * _00 + mx1 * _10 + mx2 * _20;
		m21 = mx0 * _01 + mx1 * _11 + mx2 * _21;
		m22 = mx0 * _02 + mx1 * _12 + mx2 * _22;

		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * _00 + mx1 * _10 + mx2 * _20;
		m31 = mx0 * _01 + mx1 * _11 + mx2 * _21;
		m32 = mx0 * _02 + mx1 * _12 + mx2 * _22;
		*/

		lnfloat _00 = ( axis.x * axis.x ) * mc + c;
		lnfloat _01 = ( axis.x * axis.y ) * mc + ( axis.z * s );
		lnfloat _02 = ( axis.x * axis.z ) * mc - ( axis.y * s );

		lnfloat _10 = ( axis.x * axis.y ) * mc - ( axis.z * s );
		lnfloat _11 = ( axis.y * axis.y ) * mc + c;
		lnfloat _12 = ( axis.y * axis.z ) * mc + ( axis.x * s );

		lnfloat _20 = ( axis.x * axis.z ) * mc + ( axis.y * s );
		lnfloat _21 = ( axis.y * axis.z ) * mc - ( axis.x * s );
		lnfloat _22 = ( axis.z * axis.z ) * mc + c;

		lnfloat mx0 = m00;
		lnfloat mx1 = m01;
		m00 = mx0 * _00 + mx1 * _10 + m02 * _20;
		m01 = mx0 * _01 + mx1 * _11 + m02 * _21;
		m02 = mx0 * _02 + mx1 * _12 + m02 * _22;

		mx0 = m10;
		mx1 = m11;
		m10 = mx0 * _00 + mx1 * _10 + m12 * _20;
		m11 = mx0 * _01 + mx1 * _11 + m12 * _21;
		m12 = mx0 * _02 + mx1 * _12 + m12 * _22;

		mx0 = m20;
		mx1 = m21;
		m20 = mx0 * _00 + mx1 * _10 + m22 * _20;
		m21 = mx0 * _01 + mx1 * _11 + m22 * _21;
		m22 = mx0 * _02 + mx1 * _12 + m22 * _22;

		mx0 = m30;
		mx1 = m31;
		m30 = mx0 * _00 + mx1 * _10 + m32 * _20;
		m31 = mx0 * _01 + mx1 * _11 + m32 * _21;
		m32 = mx0 * _02 + mx1 * _12 + m32 * _22;
    }

    //---------------------------------------------------------------------
	// ● 任意の軸を回転軸にして行列を回転する ( ラジアン単位 )
	//---------------------------------------------------------------------
	void Matrix::rotationAxis( const Vector3& axis_, lnfloat r_, const Vector3& def_axis_ )
    {
        rotationAxis( ( axis_.isZero() ) ? def_axis_ : axis_, r_ );
    }
    

    //---------------------------------------------------------------------
	// ●  X 軸を回転軸にして行列を回転する ( インデックス角度 )
	//---------------------------------------------------------------------
	void Matrix::rotationXIdx( int r_ )
    {
        lnfloat s = LMath::sinIdx( r_ );
        lnfloat c = LMath::cosIdx( r_ );

        lnfloat mx1 = m01;
		m01 = mx1 * c + m02 *-s;
		m02 = mx1 * s + m02 * c;

		mx1 = m11;
		m11 = mx1 * c + m12 *-s;
		m12 = mx1 * s + m12 * c;

		mx1 = m21;
		m21 = mx1 * c + m22 *-s;
		m22 = mx1 * s + m22 * c;

		mx1 = m31;
		m31 = mx1 * c + m32 *-s;
		m32 = mx1 * s + m32 * c;
    }

	//---------------------------------------------------------------------
	// ● Y 軸を回転軸にして行列を回転する ( インデックス角度 )
    //---------------------------------------------------------------------
	void Matrix::rotationYIdx( int r_ )
    {
        lnfloat s = LMath::sinIdx( r_ );
        lnfloat c = LMath::cosIdx( r_ );

		lnfloat mx0 = m00;
		m00 = mx0 * c + m02 * s;
		m02 = mx0 *-s + m02 * c;

		mx0 = m10;
		m10 = mx0 * c + m12 * s;
		m12 = mx0 *-s + m12 * c;

		mx0 = m20;
		m20 = mx0 * c + m22 * s;
		m22 = mx0 *-s + m22 * c;

		mx0 = m30;
		m30 = mx0 * c + m32 * s;
		m32 = mx0 *-s + m32 * c;
    }

	//---------------------------------------------------------------------
	// ● Z 軸を回転軸にして行列を回転する ( インデックス角度 )
    //---------------------------------------------------------------------
	void Matrix::rotationZIdx( int r_ )
    {
        lnfloat s = LMath::sinIdx( r_ );
        lnfloat c = LMath::cosIdx( r_ );

        lnfloat mx0 = m00;
		m00 = mx0 * c + m01 *-s;
		m01 = mx0 * s + m01 * c;

		mx0 = m10;
		m10 = mx0 * c + m11 *-s;
		m11 = mx0 * s + m11 * c;

		mx0 = m20;
		m20 = mx0 * c + m21 *-s;
		m21 = mx0 * s + m21 * c;

		mx0 = m30;
		m30 = mx0 * c + m31 *-s;
		m31 = mx0 * s + m31 * c;
    }

	//---------------------------------------------------------------------
	// ● 行列を回転する ( インデックス角度 )
	//---------------------------------------------------------------------
	void Matrix::rotationIdx( int x_, int y_, int z_, LNRotOrder order_ )
    {
        switch ( order_ )
		{
			case LN_ROTORDER_XYZ: 
				rotationXIdx( x_ ); rotationYIdx( y_ ); rotationZIdx( z_ ); break;
			case LN_ROTORDER_XZY: 
				rotationXIdx( x_ ); rotationZIdx( z_ ); rotationYIdx( y_ ); break;
			case LN_ROTORDER_YXZ: 
				rotationYIdx( y_ ); rotationXIdx( x_ ); rotationZIdx( z_ ); break;
			case LN_ROTORDER_YZX: 
				rotationYIdx( y_ ); rotationZIdx( z_ ); rotationXIdx( x_ ); break;
			case LN_ROTORDER_ZXY: 
				rotationZIdx( z_ ); rotationXIdx( x_ ); rotationYIdx( y_ ); break;
			case LN_ROTORDER_ZYX: 
				rotationZIdx( z_ ); rotationYIdx( y_ ); rotationXIdx( x_ ); break;
		}
    }

	//---------------------------------------------------------------------
	// ● 任意の軸を回転軸にして行列を回転する ( インデックス角度 )
	//---------------------------------------------------------------------
	void Matrix::rotationAxisIdx( const Vector3& axis_, int r_ )
    {
        Vector3 axis = axis_;
        axis.normalize();

        lnfloat s = LMath::sinIdx( r_ );
        lnfloat c = LMath::cosIdx( r_ );
		lnfloat mc = 1.0f - c;

        lnfloat _00 = ( axis.x * axis.x ) * mc + c;
		lnfloat _01 = ( axis.x * axis.y ) * mc + ( axis.z * s );
		lnfloat _02 = ( axis.x * axis.z ) * mc - ( axis.y * s );

		lnfloat _10 = ( axis.x * axis.y ) * mc - ( axis.z * s );
		lnfloat _11 = ( axis.y * axis.y ) * mc + c;
		lnfloat _12 = ( axis.y * axis.z ) * mc + ( axis.x * s );

		lnfloat _20 = ( axis.x * axis.z ) * mc + ( axis.y * s );
		lnfloat _21 = ( axis.y * axis.z ) * mc - ( axis.x * s );
		lnfloat _22 = ( axis.z * axis.z ) * mc + c;

		lnfloat mx0 = m00;
		lnfloat mx1 = m01;
		m00 = mx0 * _00 + mx1 * _10 + m02 * _20;
		m01 = mx0 * _01 + mx1 * _11 + m02 * _21;
		m02 = mx0 * _02 + mx1 * _12 + m02 * _22;

		mx0 = m10;
		mx1 = m11;
		m10 = mx0 * _00 + mx1 * _10 + m12 * _20;
		m11 = mx0 * _01 + mx1 * _11 + m12 * _21;
		m12 = mx0 * _02 + mx1 * _12 + m12 * _22;

		mx0 = m20;
		mx1 = m21;
		m20 = mx0 * _00 + mx1 * _10 + m22 * _20;
		m21 = mx0 * _01 + mx1 * _11 + m22 * _21;
		m22 = mx0 * _02 + mx1 * _12 + m22 * _22;

		mx0 = m30;
		mx1 = m31;
		m30 = mx0 * _00 + mx1 * _10 + m32 * _20;
		m31 = mx0 * _01 + mx1 * _11 + m32 * _21;
		m32 = mx0 * _02 + mx1 * _12 + m32 * _22;
    }

    //---------------------------------------------------------------------
	// ● 任意の軸を回転軸にして行列を回転する ( インデックス角度 )
	//---------------------------------------------------------------------
	void Matrix::rotationAxisIdx( const Vector3& axis_, int r_, const Vector3& def_axis_ )
    {
        rotationAxisIdx( ( axis_.isZero() ) ? def_axis_ : axis_, r_ );
    }

    //---------------------------------------------------------------------
    // ● クォータニオンを使って行列を回転する
    //---------------------------------------------------------------------
    void Matrix::rotationQuaternion( const Quaternion& qua_ )
    {
        Matrix t;
        Matrix::rotationQuaternion( &t, qua_ );
        (*this) *= t;
    }

    //---------------------------------------------------------------------
	// ● 行列をスケーリングする
	//---------------------------------------------------------------------
	void Matrix::scaling( lnfloat x_, lnfloat y_, lnfloat z_ )
	{
		m00 *= x_;
		m01 *= y_;
		m02 *= z_;
		m10 *= x_;
		m11 *= y_;
		m12 *= z_;
		m20 *= x_;
		m21 *= y_;
		m22 *= z_;
        m30 *= x_;
		m31 *= y_;
		m32 *= z_;
	}
	void Matrix::scaling( const Vector3& vec_ )
	{
		m00 *= vec_.x;
		m01 *= vec_.y;
		m02 *= vec_.z;
		m10 *= vec_.x;
		m11 *= vec_.y;
		m12 *= vec_.z;
		m20 *= vec_.x;
		m21 *= vec_.y;
		m22 *= vec_.z;
        m30 *= vec_.x;
		m31 *= vec_.y;
		m32 *= vec_.z;
	}
	void Matrix::scaling( lnfloat xyz_ )
	{
		m00 *= xyz_;
		m01 *= xyz_;
		m02 *= xyz_;
		m10 *= xyz_;
		m11 *= xyz_;
		m12 *= xyz_;
		m20 *= xyz_;
		m21 *= xyz_;
		m22 *= xyz_;
        m30 *= xyz_;
		m31 *= xyz_;
		m32 *= xyz_;
	}

    //---------------------------------------------------------------------
    // ● 逆行列化する
    //---------------------------------------------------------------------
	void Matrix::inverse()
    {
        // http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/tech23.html

		float m0011 = this->m00 * this->m11;
		float m0012 = this->m00 * this->m12;
		float m0021 = this->m00 * this->m21;
		float m0022 = this->m00 * this->m22;

		float m0110 = this->m01 * this->m10;
		float m0112 = this->m01 * this->m12;
		float m0120 = this->m01 * this->m20;
		float m0122 = this->m01 * this->m22;

		float m0210 = this->m02 * this->m10;
		float m0211 = this->m02 * this->m11;
		float m0220 = this->m02 * this->m20;
		float m0221 = this->m02 * this->m21;

		float m1021 = this->m10 * this->m21;
		float m1022 = this->m10 * this->m22;

		float m1120 = this->m11 * this->m20;
		float m1122 = this->m11 * this->m22;

		float m1220 = this->m12 * this->m20;
		float m1221 = this->m12 * this->m21;


		float m1122_m1221 = m1122 - m1221;
		float m1220_m1022 = m1220 - m1022;
		float m1021_m1120 = m1021 - m1120;

		
		float delta = this->m00*( m1122_m1221 ) + this->m01*( m1220_m1022 ) + this->m02*( m1021_m1120 );
		float rcpDelta = 1.f / delta;


		m00 = ( m1122_m1221 ) * rcpDelta;
		m10 = ( m1220_m1022 ) * rcpDelta;
		m20 = ( m1021_m1120 ) * rcpDelta;
		m01 = ( m0221-m0122 ) * rcpDelta;
		m11 = ( m0022-m0220 ) * rcpDelta;
		m21 = ( m0120-m0021 ) * rcpDelta;
		m02 = ( m0112-m0211 ) * rcpDelta;
		m12 = ( m0210-m0012 ) * rcpDelta;
		m22 = ( m0011-m0110 ) * rcpDelta;

		float t03 = this->m03;
		float t13 = this->m13;
		m03 = -( m00 * t03 + m01 * t13 + m02 * this->m23 );
		m13 = -( m10 * t03 + m11 * t13 + m12 * this->m23 );
		m23 = -( m20 * t03 + m21 * t13 + m22 * this->m23 );
    }

    //---------------------------------------------------------------------
    // ● 転置行列化する
    //---------------------------------------------------------------------
	void Matrix::transpose()
    {
        lnfloat t;

        t = m01;
        m01 = m10;
        m10 = t;

        t = m02;
        m02 = m20;
        m20 = t;

        t = m03;
        m03 = m30;
        m30 = t;
        
        t = m12;
        m12 = m21;
        m21 = t;

        t = m13;
        m13 = m31;
        m31 = t;
        
        t = m23;
        m23 = m32;
        m32 = t;
    }

    //---------------------------------------------------------------------
	// ● 行列の 3x3 部分だけを乗算して設定する
	//---------------------------------------------------------------------
    void Matrix::setMul3x3( const Matrix& matrix_ )
    {
        lnfloat mx0 = m00;
		lnfloat mx1 = m01;
		lnfloat mx2 = m02;
		m00 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m03 * matrix_.m30;
		m01 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m03 * matrix_.m31;
		m02 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m03 * matrix_.m32;
        mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m13 * matrix_.m30;
		m11 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m13 * matrix_.m31;
		m12 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m13 * matrix_.m32;
		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m23 * matrix_.m30;
		m21 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m23 * matrix_.m31;
		m22 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m23 * matrix_.m32;
        m03 = m13 = m23 = m30 = m31 = m32 = 0.0f;
        m33 = 1.0f;
    }

    void Matrix::setMul4x3( const Matrix& matrix_ )
    {
        lnfloat mx0 = m00;
		lnfloat mx1 = m01;
		lnfloat mx2 = m02;

		m00 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m03 * matrix_.m30;
		m01 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m03 * matrix_.m31;
		m02 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m03 * matrix_.m32;
        mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m13 * matrix_.m30;
		m11 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m13 * matrix_.m31;
		m12 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m13 * matrix_.m32;
		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m23 * matrix_.m30;
		m21 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m23 * matrix_.m31;
		m22 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m23 * matrix_.m32;
        mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m33 * matrix_.m30;
		m31 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m33 * matrix_.m31;
		m32 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m33 * matrix_.m32;

        m03 = m13 = m23 = m30 = m31 = m32 = 0.0f;
        m33 = 1.0f;
    }

	//---------------------------------------------------------------------
	// ● operator
	//---------------------------------------------------------------------
    Matrix& Matrix::operator = ( const SQTTransform t_ )
    {
        //this->scaling( t_.Scale );
        Matrix::scaling( this, t_.Scale );
        this->rotationQuaternion( t_.Rotation );
        this->translation( t_.Translation );
        return *this;
    }

	void Matrix::operator *= ( const Matrix& matrix_ )
	{
		lnfloat mx0 = m00;
		lnfloat mx1 = m01;
		lnfloat mx2 = m02;
		m00 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m03 * matrix_.m30;
		m01 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m03 * matrix_.m31;
		m02 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m03 * matrix_.m32;
		m03 = mx0 * matrix_.m03 + mx1 * matrix_.m13 + mx2 * matrix_.m23 + m03 * matrix_.m33;
		mx0 = m10;
		mx1 = m11;
		mx2 = m12;
		m10 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m13 * matrix_.m30;
		m11 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m13 * matrix_.m31;
		m12 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m13 * matrix_.m32;
		m13 = mx0 * matrix_.m03 + mx1 * matrix_.m13 + mx2 * matrix_.m23 + m13 * matrix_.m33;
		mx0 = m20;
		mx1 = m21;
		mx2 = m22;
		m20 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m23 * matrix_.m30;
		m21 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m23 * matrix_.m31;
		m22 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m23 * matrix_.m32;
		m23 = mx0 * matrix_.m03 + mx1 * matrix_.m13 + mx2 * matrix_.m23 + m23 * matrix_.m33;
		mx0 = m30;
		mx1 = m31;
		mx2 = m32;
		m30 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m33 * matrix_.m30;
		m31 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m33 * matrix_.m31;
		m32 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m33 * matrix_.m32;
		m33 = mx0 * matrix_.m03 + mx1 * matrix_.m13 + mx2 * matrix_.m23 + m33 * matrix_.m33;
	}

	Matrix Matrix::operator * ( const Matrix& matrix_ ) const
	{
		Matrix out;
		out.m00 = m00 * matrix_.m00 + m01 * matrix_.m10 + m02 * matrix_.m20 + m03 * matrix_.m30;
		out.m01 = m00 * matrix_.m01 + m01 * matrix_.m11 + m02 * matrix_.m21 + m03 * matrix_.m31;
		out.m02 = m00 * matrix_.m02 + m01 * matrix_.m12 + m02 * matrix_.m22 + m03 * matrix_.m32;
		out.m03 = m00 * matrix_.m03 + m01 * matrix_.m13 + m02 * matrix_.m23 + m03 * matrix_.m33;

		out.m10 = m10 * matrix_.m00 + m11 * matrix_.m10 + m12 * matrix_.m20 + m13 * matrix_.m30;
		out.m11 = m10 * matrix_.m01 + m11 * matrix_.m11 + m12 * matrix_.m21 + m13 * matrix_.m31;
		out.m12 = m10 * matrix_.m02 + m11 * matrix_.m12 + m12 * matrix_.m22 + m13 * matrix_.m32;
		out.m13 = m10 * matrix_.m03 + m11 * matrix_.m13 + m12 * matrix_.m23 + m13 * matrix_.m33;

		out.m20 = m20 * matrix_.m00 + m21 * matrix_.m10 + m22 * matrix_.m20 + m23 * matrix_.m30;
		out.m21 = m20 * matrix_.m01 + m21 * matrix_.m11 + m22 * matrix_.m21 + m23 * matrix_.m31;
		out.m22 = m20 * matrix_.m02 + m21 * matrix_.m12 + m22 * matrix_.m22 + m23 * matrix_.m32;
		out.m23 = m20 * matrix_.m03 + m21 * matrix_.m13 + m22 * matrix_.m23 + m23 * matrix_.m33;

		out.m30 = m30 * matrix_.m00 + m31 * matrix_.m10 + m32 * matrix_.m20 + m33 * matrix_.m30;
		out.m31 = m30 * matrix_.m01 + m31 * matrix_.m11 + m32 * matrix_.m21 + m33 * matrix_.m31;
		out.m32 = m30 * matrix_.m02 + m31 * matrix_.m12 + m32 * matrix_.m22 + m33 * matrix_.m32;
		out.m33 = m30 * matrix_.m03 + m31 * matrix_.m13 + m32 * matrix_.m23 + m33 * matrix_.m33;
		return out;
	}

	//---------------------------------------------------------------------
	// ● コンソールに出力する
	//---------------------------------------------------------------------
	void Matrix::cdump( const lnChar* str_, int indent_ ) const
	{
		if ( str_ )
		{
			printf( "%s\n", str_ );
		}
		else
		{
			printf( "◆ Matrix\n" );
		}
		int i;
		for ( i = 0; i < indent_; ++i ) { 
			printf( " " ); }
		printf( "[ %f, %f, %f, %f ]\n", m00, m01, m02, m03 );
		for ( i = 0; i < indent_; ++i ) { 
			printf( " " ); }
		printf( "[ %f, %f, %f, %f ]\n", m10, m11, m12, m13 );
		for ( i = 0; i < indent_; ++i ) { 
			printf( " " ); }
		printf( "[ %f, %f, %f, %f ]\n", m20, m21, m22, m23 );
		for ( i = 0; i < indent_; ++i ) { 
			printf( " " ); }
		printf( "[ %f, %f, %f, %f ]\n", m30, m31, m32, m33 );
	}


    //-------------------------------------------------------------------------
	// ▼ static

    //---------------------------------------------------------------------
	// ● 単位行列を作成する
	//---------------------------------------------------------------------
	void Matrix::identity( Matrix* mat_ )
	{
        mat_->m00 = mat_->m11 = mat_->m22 = mat_->m33 = 1.0f;
		mat_->m01 = mat_->m02 = mat_->m03 = 0.0f;
		mat_->m10 = mat_->m12 = mat_->m13 = 0.0f;
		mat_->m20 = mat_->m21 = mat_->m23 = 0.0f;
		mat_->m30 = mat_->m31 = mat_->m32 = 0.0f;
	}

	//---------------------------------------------------------------------
	// ● 平行移動行列を作成する
	//---------------------------------------------------------------------
	void Matrix::translation( Matrix* mat_, lnfloat x_, lnfloat y_, lnfloat z_ )
	{
        mat_->set(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			x_, y_, z_, 1.0f );
	}

	//---------------------------------------------------------------------
	// ● 平行移動行列を作成する
	//---------------------------------------------------------------------
	void Matrix::translation( Matrix* mat_, const Vector3& vec_ )
	{
		mat_->set(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			vec_.x, vec_.y, vec_.z, 1.0f );
	}

	//---------------------------------------------------------------------
	// ● X 軸を回転軸にして回転する行列を作成する ( ラジアン角度 )
	//---------------------------------------------------------------------
	void Matrix::rotationX( Matrix* mat_, lnfloat r_ )
	{
		lnfloat c, s;
        Asm::sincos( r_, &s, &c );
		mat_->set(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f,    c,    s, 0.0f,
			0.0f,   -s,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );

        /* OpenGL
        mat_->set(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f,    c,   -s, 0.0f,
			0.0f,    s,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        */
	}

	//---------------------------------------------------------------------
	// ● Y 軸を回転軸にして回転する行列を作成する ( ラジアン角度 )
	//---------------------------------------------------------------------
	void Matrix::rotationY( Matrix* mat_, lnfloat r_ )
	{
		lnfloat c, s;
        Asm::sincos( r_, &s, &c );
        mat_->set(
			   c, 0.0f,   -s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			   s, 0.0f,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        /* OpenGL
		mat_->set(
			   c, 0.0f,    s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			  -s, 0.0f,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        */
	}

	//---------------------------------------------------------------------
	// ● Z 軸を回転軸にして回転する行列を作成する ( ラジアン角度 )
	//---------------------------------------------------------------------
	void Matrix::rotationZ( Matrix* mat_, lnfloat r_ )
	{
		lnfloat c, s;
        Asm::sincos( r_, &s, &c );
        mat_->set(
			   c,    s, 0.0f, 0.0f,
			  -s,    c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        /* OpenGL
		mat_->set(
			   c,   -s, 0.0f, 0.0f,
			   s,    c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        */
	}



	//---------------------------------------------------------------------
	// ● 回転行列を作成する ( ラジアン角度 )
	//---------------------------------------------------------------------
	void Matrix::rotation( Matrix* mat_, lnfloat x_, lnfloat y_, lnfloat z_, LNRotOrder order_ )
	{
        mat_->identity();
		switch ( order_ )
		{
            default:              
            	mat_->rotationX( x_ ); mat_->rotationY( y_ ); mat_->rotationZ( z_ ); break;
			case LN_ROTORDER_XZY: 
				mat_->rotationX( x_ ); mat_->rotationZ( z_ ); mat_->rotationY( y_ ); break;
			case LN_ROTORDER_YXZ: 
				mat_->rotationY( y_ ); mat_->rotationX( x_ ); mat_->rotationZ( z_ ); break;
			case LN_ROTORDER_YZX: 
				mat_->rotationY( y_ ); mat_->rotationZ( z_ ); mat_->rotationX( x_ ); break;
			case LN_ROTORDER_ZXY: 
				mat_->rotationZ( z_ ); mat_->rotationX( x_ ); mat_->rotationY( y_ ); break;
			case LN_ROTORDER_ZYX: 
				mat_->rotationZ( z_ ); mat_->rotationY( y_ ); mat_->rotationX( x_ ); break;
		}
	}

	//---------------------------------------------------------------------
	// ● 任意の軸を回転軸にして回転する行列を作成する ( ラジアン角度 )
	//---------------------------------------------------------------------
	void Matrix::rotationAxis( Matrix* mat_, const Vector3& axis_, lnfloat r_ )
	{
		// http://marupeke296.com/DXG_No58_RotQuaternionTrans.html
		// http://homepage2.nifty.com/skimp-studio/htm/crawl/1_7_transform2.htm

        Vector3 axis = axis_;
        axis.normalize();

		lnfloat s, c;
        Asm::sincos( r_, &s, &c );
		lnfloat mc = 1.0f - c;

        mat_->set(
			( axis.x * axis.x ) * mc + c,			    ( axis.x * axis.y ) * mc + ( axis.z * s ),	( axis.x * axis.z ) * mc - ( axis.y * s ),	0.0f,
			( axis.x * axis.y ) * mc - ( axis.z * s ),	( axis.y * axis.y ) * mc + c,			    ( axis.y * axis.z ) * mc + ( axis.x * s ),	0.0f,
			( axis.x * axis.z ) * mc + ( axis.y * s ),	( axis.y * axis.z ) * mc - ( axis.x * s ),	( axis.z * axis.z ) * mc + c,			    0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
	}

    //---------------------------------------------------------------------
	// ● 任意の軸を回転軸にして回転する行列を作成する ( ラジアン角度 )
	//---------------------------------------------------------------------
	//void Matrix::rotationAxis( Matrix* mat_, const Vector3& axis_, lnfloat r_, const Vector3& def_axis_ )
	//{
 //       Matrix::rotationAxis( mat_, ( axis_.isZero() ) ? def_axis_ : axis_, r_ );
 //   }

	//---------------------------------------------------------------------
	// ● X 軸を回転軸にして回転する行列を作成する ( インデックス角度 )
	//---------------------------------------------------------------------
	void Matrix::rotationXIdx( Matrix* mat_, int r_ )
	{
        lnfloat c = LMath::cosIdx( r_ );
		lnfloat s = LMath::sinIdx( r_ );
		mat_->set(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f,    c,    s, 0.0f,
			0.0f,   -s,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );

        /* OpenGL
        mat_->set(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f,    c,   -s, 0.0f,
			0.0f,    s,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        */
	}

	//---------------------------------------------------------------------
	// ● Y 軸を回転軸にして回転する行列を作成する ( インデックス角度 )
	//---------------------------------------------------------------------
	void Matrix::rotationYIdx( Matrix* mat_, int r_ )
	{
		lnfloat c = LMath::cosIdx( r_ );
		lnfloat s = LMath::sinIdx( r_ );
		mat_->set(
			   c, 0.0f,   -s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			   s, 0.0f,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        /* OpenGL
		mat_->set(
			   c, 0.0f,    s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			  -s, 0.0f,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        */
	}

	//---------------------------------------------------------------------
	// ● Z 軸を回転軸にして回転する行列を作成する ( インデックス角度 )
	//---------------------------------------------------------------------
	void Matrix::rotationZIdx( Matrix* mat_, int r_ )
	{
		lnfloat c = LMath::cosIdx( r_ );
		lnfloat s = LMath::sinIdx( r_ );
		mat_->set(
			   c,    s, 0.0f, 0.0f,
			  -s,    c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        /* OpenGL
		mat_->set(
			   c,   -s, 0.0f, 0.0f,
			   s,    c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
        */
	}

	//---------------------------------------------------------------------
	// ● 回転行列を作成する ( インデックス角度 )
	//---------------------------------------------------------------------
	void Matrix::rotationIdx( Matrix* mat_, int x_, int y_, int z_, LNRotOrder order_ )
	{
		mat_->identity();
		switch ( order_ )
		{
            default:              
            	mat_->rotationXIdx( x_ ); mat_->rotationYIdx( y_ ); mat_->rotationZIdx( z_ ); break;
			case LN_ROTORDER_XZY: 
				mat_->rotationXIdx( x_ ); mat_->rotationZIdx( z_ ); mat_->rotationYIdx( y_ ); break;
			case LN_ROTORDER_YXZ: 
				mat_->rotationYIdx( y_ ); mat_->rotationXIdx( x_ ); mat_->rotationZIdx( z_ ); break;
			case LN_ROTORDER_YZX: 
				mat_->rotationYIdx( y_ ); mat_->rotationZIdx( z_ ); mat_->rotationXIdx( x_ ); break;
			case LN_ROTORDER_ZXY: 
				mat_->rotationZIdx( z_ ); mat_->rotationXIdx( x_ ); mat_->rotationYIdx( y_ ); break;
			case LN_ROTORDER_ZYX: 
				mat_->rotationZIdx( z_ ); mat_->rotationYIdx( y_ ); mat_->rotationXIdx( x_ ); break;
		}
	}

	//---------------------------------------------------------------------
	// ● 任意の軸を回転軸にして回転する行列を作成する ( インデックス角度 )
	//---------------------------------------------------------------------
	void Matrix::rotationAxisIdx( Matrix* mat_, const Vector3& axis_, int r_ )
	{
        Vector3 axis = axis_;
        axis.normalize();

		lnfloat s = LMath::sinIdx( r_ );
		lnfloat c = LMath::cosIdx( r_ );
		lnfloat mc = 1.0f - LMath::cosIdx( r_ );
		mat_->set(
			( axis.x * axis.x ) * mc + c,			    ( axis.x * axis.y ) * mc + ( axis.z * s ),	( axis.x * axis.z ) * mc - ( axis.y * s ),	0.0f,
			( axis.x * axis.y ) * mc - ( axis.z * s ),	( axis.y * axis.y ) * mc + c,			    ( axis.y * axis.z ) * mc + ( axis.x * s ),	0.0f,
			( axis.x * axis.z ) * mc + ( axis.y * s ),	( axis.y * axis.z ) * mc - ( axis.x * s ),	( axis.z * axis.z ) * mc + c,			    0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
	}

    //---------------------------------------------------------------------
    // ● 任意の軸を回転軸にして回転する行列を作成する ( インデックス角度 )
    //---------------------------------------------------------------------
	void Matrix::rotationAxisIdx( Matrix* mat_, const Vector3& axis_, int r_, const Vector3& def_axis_ )
    {
        Matrix::rotationAxisIdx( mat_, ( axis_.isZero() ) ? def_axis_ : axis_, r_ );
    }

    //---------------------------------------------------------------------
    // ● クォータニオンから回転行列を作成する
    //---------------------------------------------------------------------
    void Matrix::rotationQuaternion( Matrix* out_, const Quaternion& qua_ )
    {
        out_->m00 = 1.0f - 2.0f * qua_.y * qua_.y - 2.0f * qua_.z * qua_.z;
        out_->m01 = 2.0f * qua_.x * qua_.y + 2.0f * qua_.w * qua_.z;
        out_->m02 = 2.0f * qua_.x * qua_.z - 2.0f * qua_.w * qua_.y;

        out_->m10 = 2.0f * qua_.x * qua_.y - 2.0f * qua_.w * qua_.z;
        out_->m11 = 1.0f - 2.0f * qua_.x * qua_.x - 2.0f * qua_.z * qua_.z;
        out_->m12 = 2.0f * qua_.y * qua_.z + 2.0f * qua_.w * qua_.x;

        out_->m20 = 2.0f * qua_.x * qua_.z + 2.0f * qua_.w * qua_.y;
        out_->m21 = 2.0f * qua_.y * qua_.z - 2.0f * qua_.w * qua_.x;
        out_->m22 = 1.0f - 2.0f * qua_.x * qua_.x - 2.0f * qua_.y * qua_.y;

        out_->m03 = out_->m13 = out_->m23 = out_->m30 = out_->m31 = out_->m32 = 0.0f;
        out_->m33 = 1.0f;
    }

	//---------------------------------------------------------------------
	// ● スケーリングする行列を作成する
	//---------------------------------------------------------------------
	void Matrix::scaling( Matrix* mat_, lnfloat x_, lnfloat y_, lnfloat z_ )
	{
		mat_->set(
			x_, 0.0f, 0.0f, 0.0f,
			0.0f, y_, 0.0f, 0.0f,
			0.0f, 0.0f, z_, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
	}

	//---------------------------------------------------------------------
	// ● スケーリングする行列を作成する
	//---------------------------------------------------------------------
	void Matrix::scaling( Matrix* mat_, const Vector3& scale_ )
	{
		mat_->set(
			scale_.x, 0.0f, 0.0f, 0.0f,
			0.0f, scale_.y, 0.0f, 0.0f,
			0.0f, 0.0f, scale_.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
	}

	//---------------------------------------------------------------------
	// ● スケーリングする行列を作成する
	//---------------------------------------------------------------------
	void Matrix::scaling( Matrix* mat_, lnfloat xyz_ )
	{
		mat_->set(
			xyz_, 0.0f, 0.0f, 0.0f,
			0.0f, xyz_, 0.0f, 0.0f,
			0.0f, 0.0f, xyz_, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f );
	}


    

    

	//---------------------------------------------------------------------
	// ● 行列の逆行列を計算する
	//---------------------------------------------------------------------
	void Matrix::inverse( Matrix* out_, const Matrix& mat_ )
	{
        int i, j;
        lnfloat buf;

        Matrix	tm( mat_ );
	    identity( out_ );

	    // 掃き出し法
	    for ( i = 0 ; i < 4 ; ++i )
	    {
            buf = 1.0f / tm.m[ i ][ i ];
		    for ( j = 0 ; j < 4 ; ++j )
		    {
			    tm.m[ i ][ j ] *= buf;
			    out_->m[ i ][ j ] *= buf;
		    }
		    for ( j = 0 ; j < 4 ; ++j )
		    {
			    if ( i != j )
			    {
				    buf = tm.m[ j ][ i ];
				    for( int k = 0 ; k < 4 ; ++k )
				    {
					    tm.m[ j ][ k ] -= tm.m[ i ][ k ] * buf;
					    out_->m[ j ][ k ] -= out_->m[ i ][ k ] * buf;
				    }
			    }
		    }
	    }

		//D3DXMATRIX dxmout;
		//return Matrix( *( (Matrix*)D3DXMatrixInverse( &dxmout, NULL, (D3DXMATRIX*)&mat_ ) ) );
		//return Matrix();
#if 0
		// http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/tech23.html

		float m0011 = mat_.m00 * mat_.m11;
		float m0012 = mat_.m00 * mat_.m12;
		float m0021 = mat_.m00 * mat_.m21;
		float m0022 = mat_.m00 * mat_.m22;

		float m0110 = mat_.m01 * mat_.m10;
		float m0112 = mat_.m01 * mat_.m12;
		float m0120 = mat_.m01 * mat_.m20;
		float m0122 = mat_.m01 * mat_.m22;

		float m0210 = mat_.m02 * mat_.m10;
		float m0211 = mat_.m02 * mat_.m11;
		float m0220 = mat_.m02 * mat_.m20;
		float m0221 = mat_.m02 * mat_.m21;

		float m1021 = mat_.m10 * mat_.m21;
		float m1022 = mat_.m10 * mat_.m22;

		float m1120 = mat_.m11 * mat_.m20;
		float m1122 = mat_.m11 * mat_.m22;

		float m1220 = mat_.m12 * mat_.m20;
		float m1221 = mat_.m12 * mat_.m21;


		float m1122_m1221 = m1122 - m1221;
		float m1220_m1022 = m1220 - m1022;
		float m1021_m1120 = m1021 - m1120;

		
		float delta = mat_.m00*( m1122_m1221 ) + mat_.m01*( m1220_m1022 ) + mat_.m02*( m1021_m1120 );
		float rcpDelta = 1.f / delta;

		Matrix mat;

		mat.m00 = ( m1122_m1221 ) * rcpDelta;
		mat.m10 = ( m1220_m1022 ) * rcpDelta;
		mat.m20 = ( m1021_m1120 ) * rcpDelta;
		mat.m01 = ( m0221-m0122 ) * rcpDelta;
		mat.m11 = ( m0022-m0220 ) * rcpDelta;
		mat.m21 = ( m0120-m0021 ) * rcpDelta;
		mat.m02 = ( m0112-m0211 ) * rcpDelta;
		mat.m12 = ( m0210-m0012 ) * rcpDelta;
		mat.m22 = ( m0011-m0110 ) * rcpDelta;

		float t03 = mat_.m03;
		float t13 = mat_.m13;
		mat.m03 = -( mat.m00 * t03 + mat.m01 * t13 + mat.m02 * mat_.m23 );
		mat.m13 = -( mat.m10 * t03 + mat.m11 * t13 + mat.m12 * mat_.m23 );
		mat.m23 = -( mat.m20 * t03 + mat.m21 * t13 + mat.m22 * mat_.m23 );

		return mat;
#endif
	}

    //---------------------------------------------------------------------
    // ● 行列の転置行列を計算する
    //---------------------------------------------------------------------
	void Matrix::transpose( Matrix* out_, const Matrix& mat_ )
    {
        out_->m01 = mat_.m10;
        out_->m02 = mat_.m20;
        out_->m03 = mat_.m30;
        out_->m12 = mat_.m21;
        out_->m13 = mat_.m31;
        out_->m23 = mat_.m32;
        out_->m10 = mat_.m01;
        out_->m20 = mat_.m02;
        out_->m21 = mat_.m12;
        out_->m30 = mat_.m03;
        out_->m31 = mat_.m13;
        out_->m32 = mat_.m23;
        out_->m00 = mat_.m00;
        out_->m11 = mat_.m11;
        out_->m22 = mat_.m22;
        out_->m33 = mat_.m33;

        //lnfloat t;
        //t = m01; m01 = m10; m10 = t;
        //t = m02; m02 = m20; m20 = t;
        //t = m12; m12 = m21; m21 = t;
        //t = m03; m03 = m30; m30 = t;
        //t = m13; m13 = m31; m31 = t;
        //t = m23; m23 = m32; m32 = t;
    }

    //---------------------------------------------------------------------
	// ● 左手座標系ビュー行列を作成する
	//---------------------------------------------------------------------
	void Matrix::viewTransformLH( Matrix* out_, const Vector3& position_, const Vector3& look_at_, const Vector3& up_ )
	{
        Vector3 xaxis, yaxis, zaxis;
		// 注視点からカメラ位置までのベクトルをZ軸とする
		zaxis = look_at_;
		zaxis -= position_;
		zaxis.normalize();
		// Z軸と上方向のベクトルの外積をとるとX軸が分かる
        Vector3::cross( &xaxis, up_, zaxis );
		xaxis.normalize();
		// 2つの軸がわかったので、その2つの外積は残りの軸(Y軸)になる
        Vector3::cross( &yaxis, zaxis, xaxis );

		out_->set(
			xaxis.x, yaxis.x, zaxis.x, 0.0f,
			xaxis.y, yaxis.y, zaxis.y, 0.0f,
			xaxis.z, yaxis.z, zaxis.z, 0.0f,
			-( xaxis.x * position_.x + xaxis.y * position_.y + xaxis.z * position_.z ),
			-( yaxis.x * position_.x + yaxis.y * position_.y + yaxis.z * position_.z ),
			-( zaxis.x * position_.x + zaxis.y * position_.y + zaxis.z * position_.z ),
			1.0f );

		
	}

    //---------------------------------------------------------------------
	// ● 右手座標系ビュー行列を作成する
	//---------------------------------------------------------------------
	void Matrix::viewTransformRH( Matrix* out_, const Vector3& position_, const Vector3& look_at_, const Vector3& up_ )
	{
        Vector3 xaxis, yaxis, zaxis;
		// 注視点からカメラ位置までのベクトルをZ軸とする
		zaxis = position_;
		zaxis -= look_at_;
		zaxis.normalize();
		// Z軸と上方向のベクトルの外積をとるとX軸が分かる
        Vector3::cross( &xaxis, up_, zaxis );
		xaxis.normalize();
		// 2つの軸がわかったので、その2つの外積は残りの軸(Y軸)になる
        Vector3::cross( &yaxis, zaxis, xaxis );

		out_->set(
			xaxis.x, yaxis.x, zaxis.x, 0.0f,
			xaxis.y, yaxis.y, zaxis.y, 0.0f,
			xaxis.z, yaxis.z, zaxis.z, 0.0f,
			-( xaxis.x * position_.x + xaxis.y * position_.y + xaxis.z * position_.z ),
			-( yaxis.x * position_.x + yaxis.y * position_.y + yaxis.z * position_.z ),
			-( zaxis.x * position_.x + zaxis.y * position_.y + zaxis.z * position_.z ),
			1.0f );
        //D3DXMATRIX mat;
        //D3DXMatrixLookAtRH(&mat, 
        //    (const D3DXVECTOR3 *)&position_,
        //    (const D3DXVECTOR3 *)&look_at_,
        //    (const D3DXVECTOR3 *)&up_ );
        //*out_ = *((Matrix*)&mat);
        /*
        
            */
	}

	//---------------------------------------------------------------------
	// ● 左手座標系射影行列の作成
	//---------------------------------------------------------------------
	void Matrix::perspectiveFovLH( Matrix* out_, lnfloat fov_y_, lnfloat width_, lnfloat height_, lnfloat near_, lnfloat far_ )
	{
		lnfloat h = 1.f / tan( fov_y_ * 0.5f );	// cot(fovY/2)
		lnfloat aspect = width_ / height_;		// アスペクト比

        out_->set(
			h / aspect,		0.0f,		0.0f,									0.0f,
			0.0f,			h,			0.0f,									0.0f,
			0.0f,			0.0f,		far_ / ( far_ - near_ ),				1.0f,
			0.0f,			0.0f,		( -near_ * far_ ) / ( far_ - near_ ),	0.0f );
	}

    //---------------------------------------------------------------------
	// ● 右手座標系射影行列の作成
	//---------------------------------------------------------------------
	void Matrix::perspectiveFovRH( Matrix* out_, lnfloat fov_y_, lnfloat width_, lnfloat height_, lnfloat near_, lnfloat far_ )
	{
        lnfloat h = 1.f / tan( fov_y_ * 0.5f );	// cot(fovY/2)
		lnfloat aspect = width_ / height_;		// アスペクト比
        out_->set(
			h / aspect,		0.0f,		0.0f,									0.0f,
			0.0f,			h,			0.0f,									0.0f,
			0.0f,			0.0f,		far_ / ( near_ - far_ ),				-1.0f,
			0.0f,			0.0f,		( near_ * far_ ) / ( near_ - far_ ),	0.0f );

        /*
		h = cot(fovY/2)
		w = h / Aspect

		w       0       0               0
		0       h       0               0
		0       0       zf/(zf-zn)      1
		0       0       -zn*zf/(zf-zn)  0

		*/

		//Matrix mat;

        //D3DXMATRIX mat;
        //D3DXMatrixPerspectiveFovRH(&mat, fov_y_, width_ / height_, near_, far_);

        //*out_ = *((Matrix*)&mat);
        
        /*
		lnfloat h = 1.f / tan( fov_y_ * 0.5f );	// cot(fovY/2)
		lnfloat aspect = width_ / height_;		// アスペクト比

        out_->set(
			h / aspect,		0.0f,		0.0f,									0.0f,
			0.0f,			h,			0.0f,									0.0f,
			0.0f,			0.0f,		far_ / ( far_ - near_ ),				1.0f,
			0.0f,			0.0f,		( -near_ * far_ ) / ( far_ - near_ ),	0.0f );
            */

        /*
        lnfloat aspect = width_ / height_;		// アスペクト比
		float xymax = near_ * tan(fov_y_ *  0.00872664626);
          float ymin = -xymax;
          float xmin = -xymax;

          float width = xymax - xmin;
          float height = xymax - ymin;

          float depth = far_ - near_;
          float q = -(far_ + near_) / depth;
          float qn = -2 * (far_ * near_) / depth;

          float w = 2 * near_ / width;
          w = w / aspect;
          float h = 2 * near_ / height;

          out_->set(
			w,		0.0f,		0.0f,									0.0f,
			0.0f,			h,			0.0f,									0.0f,
			0.0f,			0.0f,		q,				-1.0f,
			0.0f,			0.0f,		qn,	0.0f );
*/
        
	}

    //---------------------------------------------------------------------
    // ● 左手座標系正射影行列の作成
    //---------------------------------------------------------------------
	void Matrix::orthoLH( Matrix* out_, lnfloat width_, lnfloat height_, lnfloat near_, lnfloat far_ )
    {
        out_->set(
			2.0f / width_,  0.0f,		    0.0f,					    0.0f,
			0.0f,			2.0f / height_, 0.0f,					    0.0f,
			0.0f,			0.0f,		    1.0f / ( far_ - near_ ),    0.0f,
			0.0f,			0.0f,		    near_ / ( near_ - far_ ),	1.0f );
    }

    //---------------------------------------------------------------------
    // ● 右手座標系正射影行列の作成
    //---------------------------------------------------------------------
	void Matrix::orthoRH( Matrix* out_, lnfloat width_, lnfloat height_, lnfloat near_, lnfloat far_ )
    {
        out_->set(
			2.0f / width_,  0.0f,		    0.0f,					    0.0f,
			0.0f,			2.0f / height_, 0.0f,					    0.0f,
			0.0f,			0.0f,		    1.0f / ( near_ - far_ ),    0.0f,
			0.0f,			0.0f,		    near_ / ( near_ - far_ ),	1.0f );
    }

    //---------------------------------------------------------------------
    // ● ピクセル単位の2D描画に使う射影行列の作成
    //---------------------------------------------------------------------
	void Matrix::perspective2DLH( Matrix* out_, lnfloat width_, lnfloat height_, lnfloat near_, lnfloat far_ )
    {
        // 平行移動成分は、画面半分 + 0.5 ピクセルの調整
        out_->set(
			2.0f / width_,              0.0f,		                0.0f,					    0.0f,
			0.0f,			            -2.0f / height_,            0.0f,					    0.0f,
			0.0f,			            0.0f,		                1.0f / ( far_ - near_ ),    0.0f,
			-1.0f - ( 0.5f / width_ ),  1.0f + ( 0.5f / height_ ),  near_ / ( near_ - far_ ),	1.0f );
    }

    //---------------------------------------------------------------------
    // ● 行列の 3x3 部分だけを乗算する
    //---------------------------------------------------------------------
  //  void Matrix::mul3x3( Matrix* out_, const Matrix& mat0_, const Matrix& mat1_ )
  //  {
  //      lnfloat mx0 = m00;
		//lnfloat mx1 = m01;
		//lnfloat mx2 = m02;
		//m00 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m03 * matrix_.m30;
		//m01 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m03 * matrix_.m31;
		//m02 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m03 * matrix_.m32;
  //      mx0 = m10;
		//mx1 = m11;
		//mx2 = m12;
		//m10 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m13 * matrix_.m30;
		//m11 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m13 * matrix_.m31;
		//m12 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m13 * matrix_.m32;
		//mx0 = m20;
		//mx1 = m21;
		//mx2 = m22;
		//m20 = mx0 * matrix_.m00 + mx1 * matrix_.m10 + mx2 * matrix_.m20 + m23 * matrix_.m30;
		//m21 = mx0 * matrix_.m01 + mx1 * matrix_.m11 + mx2 * matrix_.m21 + m23 * matrix_.m31;
		//m22 = mx0 * matrix_.m02 + mx1 * matrix_.m12 + mx2 * matrix_.m22 + m23 * matrix_.m32;
  //      m03 = m13 = m23 = m30 = m31 = m32 = 0.0f;
  //      m33 = 1.0f;
  //  }

    //---------------------------------------------------------------------
    // ● SQTTransform から変換する
    //---------------------------------------------------------------------
    void Matrix::createFromSQTTransform( Matrix* out_, const SQTTransform& t_ )
    {
        Matrix::scaling( out_, t_.Scale );
        out_->rotationQuaternion( t_.Rotation );
        out_->translation( t_.Translation );
    }

    //---------------------------------------------------------------------
    // ● SQTTransform との乗算
    //---------------------------------------------------------------------
    //void Matrix::multiply( Matrix* out_, const SQTTransform& t_, const Matrix& mat_ )
    //{
    //    Matrix::createFromSQTTransform( out_, t_ );
    //    (*out_) *= mat_;
    //}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Geometry
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================