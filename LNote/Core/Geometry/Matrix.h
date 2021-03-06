//=============================================================================
//【 Matrix 】
//-----------------------------------------------------------------------------
///**
//  @file       Matrix.h
//  @brief      Matrix
//  @author     Riki
//*/
//=============================================================================

#pragma once
#define __LNOTE_MATRIX_H__

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Common.h"
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
class Quaternion;
class SQTTransform;

//=============================================================================
// ■ Matrix クラス
//-----------------------------------------------------------------------------
///**
//  @brief      4x4 の行列クラス
//*/
//=============================================================================
class Matrix
{
public:

    static const Matrix IDENTITY;  ///< 単位行列

public:

    union
	{
		struct
		{
			lnfloat _11, _12, _13, _14;
			lnfloat _21, _22, _23, _24;
			lnfloat _31, _32, _33, _34;
			lnfloat _41, _42, _43, _44;
		};
		struct
		{
			lnfloat m00, m01, m02, m03;
			lnfloat m10, m11, m12, m13;
			lnfloat m20, m21, m22, m23;
			lnfloat m30, m31, m32, m33;
		};
        float m[ 4 ][ 4 ];
	};

public:

    /// デフォルトコンストラクタ (単位行列の作成)
	Matrix();

	/// コピーコンストラクタ
	Matrix( const Matrix& mat_ );

	/// コンストラクタ
	Matrix(
		lnfloat m00_, lnfloat m01_, lnfloat m02_, lnfloat m03_,
		lnfloat m10_, lnfloat m11_, lnfloat m12_, lnfloat m13_,
		lnfloat m20_, lnfloat m21_, lnfloat m22_, lnfloat m23_,
		lnfloat m30_, lnfloat m31_, lnfloat m32_, lnfloat m33_ );

    /// コンストラクタ (クォータニオンから作成する)
	Matrix( const Quaternion& q_ );

    /// コンストラクタ (SQTTransform から作成する)
	Matrix( const SQTTransform& t_ );

public:

    /// 右方向の取得
    const Vector3& getRight() const;

    /// 上方向の取得
    const Vector3& getUp() const;

    /// 正面方向の取得
    const Vector3& getFront() const;

    /// 位置の取得
    const Vector3& getPosition() const;

    //---------------------------------------------------------------------
	///**
    //  @brief      単位行列を作成する
    //*/
	//---------------------------------------------------------------------
    void identity();

    //---------------------------------------------------------------------
	///**
    //  @brief      単位行列かを判定する
    //*/
	//---------------------------------------------------------------------
    bool isIdentity() const;

    //---------------------------------------------------------------------
	///**
    //  @brief      値を設定する
    //*/
	//---------------------------------------------------------------------
    void set(
		lnfloat m00_, lnfloat m01_, lnfloat m02_, lnfloat m03_,
		lnfloat m10_, lnfloat m11_, lnfloat m12_, lnfloat m13_,
		lnfloat m20_, lnfloat m21_, lnfloat m22_, lnfloat m23_,
		lnfloat m30_, lnfloat m31_, lnfloat m32_, lnfloat m33_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      指定された行列から回転要素だけを取り出して設定する
    //
    //  @param[in]  matrix_ : 処理の元になる行列
    //
    //  @par
    //              matrix_ から 3x3 分の要素だけをこの行列にコピーします。
    //              主に平行移動成分を取り除くために使います。<br>
    //              残りの要素は、m33 には 1.0、それ以外には 0.0 が設定されます。
    //*/
	//---------------------------------------------------------------------
    void setRotateMatrix( const Matrix& matrix_ );

	//---------------------------------------------------------------------
	///**
    //  @brief      行列を平行移動する
    //
    //  @param[in]  x_ : X 方向の移動量
    //  @param[in]  y_ : Y 方向の移動量
    //  @param[in]  z_ : Z 方向の移動量
    //
    //  @par
    //              与えられた引数から平行移動行列を作り、現在の行列に乗算します。
    //*/
	//---------------------------------------------------------------------
	void translation( lnfloat x_, lnfloat y_, lnfloat z_ );
    
	//---------------------------------------------------------------------
	///**
	//  @brief      行列を平行移動する
	//
	//  @param[in]  vec_ : 移動量
	//
	//  @par
	//              与えられた引数から平行移動行列を作り、現在の行列に乗算します。
	//*/
	//---------------------------------------------------------------------
	void translation( const Vector3& vec_ );

	//---------------------------------------------------------------------
	///**
	//  @brief       X 軸を回転軸にして行列を回転する ( ラジアン単位 )
	//
	//  @param[in]   r_ : 回転角度 ( 軸方向に対して反時計回り )
	//
	//  @par        
	//              与えられた引数から回転行列を作り、現在の行列に乗算します。
	//*/
	//---------------------------------------------------------------------
	void rotationX( lnfloat r_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      Y 軸を回転軸にして行列を回転する ( ラジアン単位 )
	//
	//  @param[in]  r_ : 回転角度
	//
	//  @par
	//              与えられた引数から回転行列を作り、現在の行列に乗算します。
	//*/
    //---------------------------------------------------------------------
	void rotationY( lnfloat r_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      Z 軸を回転軸にして行列を回転する ( ラジアン単位 )
	//
	//  @param[in]  r_ : 回転角度
	//
	//  @par
	//              与えられた引数から回転行列を作り、現在の行列に乗算します。
	//*/
    //---------------------------------------------------------------------
	void rotationZ( lnfloat r_ );

	//---------------------------------------------------------------------
	///**
    //  @brief      行列を回転する ( ラジアン単位 )
    //
    //  @param[in]  x_      : X 軸を回転軸とした角度
    //  @param[in]  y_      : Y 軸を回転軸とした角度
    //  @param[in]  z_      : Z 軸を回転軸とした角度
    //  @param[in]  order_  : 回転順序の指定
    //
    //  @par
    //              与えられた引数から回転行列を作り、現在の行列に乗算します。
    //*/
	//---------------------------------------------------------------------
	void rotation( lnfloat x_, lnfloat y_, lnfloat z_, LNRotOrder order_ = LN_ROTORDER_XYZ );

	//---------------------------------------------------------------------
	///**
    //  @brief      行列を回転する ( ラジアン単位 )
    // 
    //  @param[in]  vec_    : 各軸の回転角度
    //  @param[in]  order_  : 回転順序の指定
    //
    //  @par
    //              与えられた引数から回転行列を作り、現在の行列に乗算します。
    //*/
	//---------------------------------------------------------------------
	void rotation( const Vector3& vec_, LNRotOrder order_ = LN_ROTORDER_XYZ );

	//---------------------------------------------------------------------
	///**
    //  @brief      任意の軸を回転軸にして行列を回転する ( ラジアン単位 )
    //
    //  @param[in]  axis_ : 回転軸
    //  @param[in]  r_    : 回転角度
    //
    //  @par
    //              与えられた引数から回転行列を作り、現在の行列に乗算します。<br>
    //              与えられた軸が正規化されていない場合は正規化したものを使って計算します。
    //*/
	//---------------------------------------------------------------------
	void rotationAxis( const Vector3& axis_, lnfloat r_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      任意の軸を回転軸にして行列を回転する ( ラジアン単位 )
    //
    //  @param[in]  axis_     : 回転軸
    //  @param[in]  r_        : 回転角度
    //  @param[in]  def_axis_ : axis_ の長さが 0 だった場合に設定するベクトル
    //
    //  @par
    //              与えられた引数から回転行列を作り、現在の行列に乗算します。<br>
    //              与えられた軸が正規化されていない場合は正規化したものを使って計算します。
    //*/
	//---------------------------------------------------------------------
	void rotationAxis( const Vector3& axis_, lnfloat r_, const Vector3& def_axis_ );

    //---------------------------------------------------------------------
	///**
	//  @brief       X 軸を回転軸にして行列を回転する ( インデックス角度 )
	//
	//  @param[in]   r_ : 回転角度 ( 軸方向に対して反時計回り )
	//
	//  @par        
	//              与えられた引数から回転行列を作り、現在の行列に乗算します。
	//*/
	//---------------------------------------------------------------------
	void rotationXIdx( int r_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      Y 軸を回転軸にして行列を回転する ( インデックス角度 )
	//
	//  @param[in]  r_ : 回転角度
	//
	//  @par
	//              与えられた引数から回転行列を作り、現在の行列に乗算します。
	//*/
    //---------------------------------------------------------------------
	void rotationYIdx( int r_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      Z 軸を回転軸にして行列を回転する ( インデックス角度 )
	//
	//  @param[in]  r_ : 回転角度
	//
	//  @par
	//              与えられた引数から回転行列を作り、現在の行列に乗算します。
	//*/
    //---------------------------------------------------------------------
	void rotationZIdx( int r_ );

	//---------------------------------------------------------------------
	///**
    //  @brief      行列を回転する ( インデックス角度 )
    //
    //  @param[in]  x_      : X 軸を回転軸とした角度
    //  @param[in]  y_      : Y 軸を回転軸とした角度
    //  @param[in]  z_      : Z 軸を回転軸とした角度
    //  @param[in]  order_  : 回転順序の指定
    //
    //  @par
    //              与えられた引数から回転行列を作り、現在の行列に乗算します。
    //*/
	//---------------------------------------------------------------------
	void rotationIdx( int x_, int y_, int z_, LNRotOrder order_ = LN_ROTORDER_XYZ );

	//---------------------------------------------------------------------
	///**
    //  @brief      任意の軸を回転軸にして行列を回転する ( インデックス角度 )
    //
    //  @param[in]  axis_ : 回転軸
    //  @param[in]  r_    : 回転角度
    //
    //  @par
    //              与えられた引数から回転行列を作り、現在の行列に乗算します。<br>
    //              与えられた軸が正規化されていない場合は正規化したものを使って計算します。
    //*/
	//---------------------------------------------------------------------
	void rotationAxisIdx( const Vector3& axis_, int r_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      任意の軸を回転軸にして行列を回転する ( インデックス角度 )
    //
    //  @param[in]  axis_ : 回転軸
    //  @param[in]  r_    : 回転角度
    //  @param[in]  def_axis_ : axis_ の長さが 0 だった場合に設定するベクトル
    //
    //  @par
    //              与えられた引数から回転行列を作り、現在の行列に乗算します。<br>
    //              与えられた軸が正規化されていない場合は正規化したものを使って計算します。
    //*/
	//---------------------------------------------------------------------
	void rotationAxisIdx( const Vector3& axis_, int r_, const Vector3& def_axis_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      クォータニオンを使って行列を回転する
    //
    //  @param[in]  qua_ : 処理の基になる Quaternion
    //*/
    //---------------------------------------------------------------------
    void rotationQuaternion( const Quaternion& qua_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      行列をスケーリングする
    //
    //  @param[in]  x_   : X 方向の拡大率
    //  @param[in]  y_   : Y 方向の拡大率
    //  @param[in]  z_   : Z 方向の拡大率
    //
    //  @par
    //              与えられた引数からスケーリング行列を作り、現在の行列に乗算します。<br>
    //*/
    //---------------------------------------------------------------------
	void scaling( lnfloat x_, lnfloat y_, lnfloat z_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      行列をスケーリングする
    //
    //  @param[in]  scale_ : 各方向の拡大率
    //
    //  @par
    //              与えられた引数からスケーリング行列を作り、現在の行列に乗算します。<br>
    //*/
    //---------------------------------------------------------------------
	void scaling( const Vector3& scale_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      行列をスケーリングする
    //
    //  @param[in]  xyz_ : X Y Z 全ての方向に適用する拡大率 ( X Y Z を同じ値で拡大 )
    //
    //  @par
    //              与えられた引数からスケーリング行列を作り、現在の行列に乗算します。<br>
    //*/
    //---------------------------------------------------------------------
	void scaling( lnfloat xyz_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      逆行列化する
    //*/
    //---------------------------------------------------------------------
	void inverse();

    //---------------------------------------------------------------------
    ///**
    //  @brief      転置行列化する
    //*/
    //---------------------------------------------------------------------
	void transpose();
 
    //---------------------------------------------------------------------
	///**
    //  @brief      行列の 3x3 部分だけを乗算して設定する
    //
    //  @param[in]  matrix_ : この行列に対して乗算を行う行列
    //
    //  @par
    //              この行列と matrix_ との乗算結果をこの行列に設定します。
    //              これは operator *= と同じ動作ですが、
    //              この関数では行列の左上から 3x3 の部分だけに対して
    //              乗算を行います。<br>
    //              3x3 以外の残りの要素は、m33 には 1.0、それ以外には 0.0 が設定されます。
    //*/
	//---------------------------------------------------------------------
    void setMul3x3( const Matrix& matrix_ );

    // ×
    void setMul4x3( const Matrix& matrix_ );

public:

    Matrix& operator = ( const SQTTransform t_ );

    void operator *= ( const Matrix& matrix_ );
	Matrix operator * ( const Matrix& matrix_ ) const;

	/// 標準出力へ出力する
	void cdump( const lnChar* str_ = NULL, int indent_ = 4 ) const;

public:

    //-------------------------------------------------------------------------
	// ▼ static
	
    //---------------------------------------------------------------------
    ///**
    //  @brief      単位行列を作成する
    //
    //  @param[out] mat_ : 単位行列を設定する Matrix へのポインタ
    //*/
    //---------------------------------------------------------------------
    static void identity( Matrix* mat_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      平行移動行列を作成する
    //
    //  @param[out] mat_ : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  x_   : x 座標の移動量
    //  @param[in]  y_   : y 座標の移動量
    //  @param[in]  z_   : z 座標の移動量
    //*/
    //---------------------------------------------------------------------
	static void translation( Matrix* mat_, lnfloat x_, lnfloat y_, lnfloat z_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      平行移動行列を作成する
    //
    //  @param[out] mat_ : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  vec_ : 移動量
    //*/
    //---------------------------------------------------------------------
	static void translation( Matrix* mat_, const Vector3& vec_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      X 軸を回転軸にして回転する行列を作成する ( ラジアン角度 )
    //
    //  @param[out] mat_ : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  r_   : 回転の角度 ( ラジアン単位 )
    //*/
    //---------------------------------------------------------------------
	static void rotationX( Matrix* mat_, lnfloat r_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      Y 軸を回転軸にして回転する行列を作成する ( ラジアン角度 )
    //
    //  @param[out] mat_ : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  r_   : 回転の角度 ( ラジアン単位 )
    //*/
    //---------------------------------------------------------------------
	static void rotationY( Matrix* mat_, lnfloat r_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      Z 軸を回転軸にして回転する行列を作成する ( ラジアン角度 )
    //
    //  @param[out] mat_ : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  r_   : 回転の角度 ( ラジアン単位 )
    //*/
    //---------------------------------------------------------------------
	static void rotationZ( Matrix* mat_, lnfloat r_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      回転行列を作成する ( ラジアン角度 )
    //
    //  @param[out] mat_   : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  x_     : X 軸を中心とする回転の角度
    //  @param[in]  y_     : Y 軸を中心とする回転の角度
    //  @param[in]  z_     : Z 軸を中心とする回転の角度
    //  @param[in]  order_ : 回転順序の指定
    //*/
    //---------------------------------------------------------------------
	static void rotation( Matrix* mat_, lnfloat x_, lnfloat y_, lnfloat z_, LNRotOrder order_ = LN_ROTORDER_XYZ );

    //---------------------------------------------------------------------
	///**
    //  @brief      任意の軸を回転軸にして回転する行列を作成する ( ラジアン角度 )
    //
    //  @param[out] mat_  : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  axis_ : 回転軸
    //  @param[in]  r_    : 回転角度
    //
    //  @par
    //              与えられた軸が正規化されていない場合は正規化したものを使って計算します。
    //*/
	//---------------------------------------------------------------------
	static void rotationAxis( Matrix* mat_, const Vector3& axis_, lnfloat r_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      任意の軸を回転軸にして回転する行列を作成する ( ラジアン角度 )
    //
    //  @param[out] mat_      : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  axis_     : 回転軸
    //  @param[in]  r_        : 回転角度
    //  @param[in]  def_axis_ : axis_ の長さが 0 だった場合に設定するベクトル
    //
    //  @par
    //              与えられた軸が正規化されていない場合は正規化したものを使って計算します。
    //*/
    //---------------------------------------------------------------------
	static void rotationAxisIdx( Matrix* mat_, const Vector3& axis_, lnfloat r_, const Vector3& def_axis_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      X 軸を回転軸にして回転する行列を作成する ( インデックス角度 )
    //
    //  @param[out] mat_ : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  r_   : 回転の角度 ( ラジアン単位 )
    //*/
    //---------------------------------------------------------------------
	static void rotationXIdx( Matrix* mat_, int r_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      Y 軸を回転軸にして回転する行列を作成する ( インデックス角度 )
    //
    //  @param[out] mat_ : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  r_   : 回転の角度 ( ラジアン単位 )
    //*/
    //---------------------------------------------------------------------
	static void rotationYIdx( Matrix* mat_, int r_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      Z 軸を回転軸にして回転する行列を作成する ( インデックス角度 )
    //
    //  @param[out] mat_ : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  r_   : 回転の角度 ( ラジアン単位 )
    //*/
    //---------------------------------------------------------------------
	static void rotationZIdx( Matrix* mat_, int r_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      回転行列を作成する ( インデックス角度 )
    //
    //  @param[out] mat_   : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  x_     : X 軸を中心とする回転の角度
    //  @param[in]  y_     : Y 軸を中心とする回転の角度
    //  @param[in]  z_     : Z 軸を中心とする回転の角度
    //  @param[in]  order_ : 回転順序の指定
    //*/
    //---------------------------------------------------------------------
	static void rotationIdx( Matrix* mat_, int x_, int y_, int z_, LNRotOrder order_ = LN_ROTORDER_XYZ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      任意の軸を回転軸にして回転する行列を作成する ( インデックス角度 )
    //
    //  @param[out] mat_  : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  axis_ : 回転軸
    //  @param[in]  r_    : 回転角度
    //
    //  @par
    //              与えられた軸が正規化されていない場合は正規化したものを使って計算します。
    //*/
    //---------------------------------------------------------------------
	static void rotationAxisIdx( Matrix* mat_, const Vector3& axis_, int r_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      任意の軸を回転軸にして回転する行列を作成する ( インデックス角度 )
    //
    //  @param[out] mat_      : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  axis_     : 回転軸
    //  @param[in]  r_        : 回転角度
    //  @param[in]  def_axis_ : axis_ の長さが 0 だった場合に設定するベクトル
    //
    //  @par
    //              与えられた軸が正規化されていない場合は正規化したものを使って計算します。
    //*/
    //---------------------------------------------------------------------
	static void rotationAxisIdx( Matrix* mat_, const Vector3& axis_, int r_, const Vector3& def_axis_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      クォータニオンから回転行列を作成する
    //
    //  @param[out] out_ : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  qua_ : 処理の基になる Quaternion
    //*/
    //---------------------------------------------------------------------
    static void rotationQuaternion( Matrix* out_, const Quaternion& qua_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      スケーリングする行列を作成する
    //
    //  @param[out] mat_ : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  x_   : X 方向の拡大率
    //  @param[in]  y_   : Y 方向の拡大率
    //  @param[in]  z_   : Z 方向の拡大率
    //*/
    //---------------------------------------------------------------------
	static void scaling( Matrix* mat_, lnfloat x_, lnfloat y_, lnfloat z_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      スケーリングする行列を作成する
    //
    //  @param[out] mat_   : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  scale_ : 各方向の拡大率
    //*/
    //---------------------------------------------------------------------
	static void scaling( Matrix* mat_, const Vector3& scale_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      スケーリングする行列を作成する
    //
    //  @param[out] mat_ : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  xyz_ : X Y Z 全ての方向に適用する拡大率 ( X Y Z を同じ値で拡大 )
    //*/
    //---------------------------------------------------------------------
	static void scaling( Matrix* mat_, lnfloat xyz_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      行列の逆行列を計算する
    //
    //  @param[out] out_ : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  mat_ : 処理の基になる Matrix
    //*/
    //---------------------------------------------------------------------
	static void inverse( Matrix* out_, const Matrix& mat_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      行列の転置行列を計算する
    //
    //  @param[out] out_ : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  mat_ : 処理の基になる Matrix
    //*/
    //---------------------------------------------------------------------
	static void transpose( Matrix* out_, const Matrix& mat_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      左手座標系ビュー行列を作成する
    //
    //  @param[out] out_      : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  position_ : 視点の位置
    //  @param[in]  look_at_  : 注視点
    //  @param[in]  up_       : 上方向
    //*/
    //---------------------------------------------------------------------
	static void viewTransformLH( Matrix* out_, const Vector3& position_, const Vector3& look_at_, const Vector3& up_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      右手座標系ビュー行列を作成する
    //
    //  @param[out] out_      : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  position_ : 視点の位置
    //  @param[in]  look_at_  : 注視点
    //  @param[in]  up_       : 上方向
    //*/
    //---------------------------------------------------------------------
	static void viewTransformRH( Matrix* out_, const Vector3& position_, const Vector3& look_at_, const Vector3& up_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      左手座標系射影行列の作成
    //
    //  @param[out] out_    : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  fov_y_  : Y 方向への視野角 ( ラジアン単位 )
    //  @param[in]  width_  : スクリーンの幅
    //  @param[in]  height_ : スクリーンの高さ
    //  @param[in]  near_   : 一番近いビュー平面の Z 値
    //  @param[in]  far_    : 一番遠いビュー平面の Z 値
    //*/
    //---------------------------------------------------------------------
	static void perspectiveFovLH( Matrix* out_, lnfloat fov_y_, lnfloat width_, lnfloat height_, lnfloat near_, lnfloat far_ );
	
    //---------------------------------------------------------------------
    ///**
    //  @brief      左手座標系射影行列の作成
    //
    //  @param[out] out_    : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  fov_y_  : Y 方向への視野角 ( ラジアン単位 )
    //  @param[in]  width_  : スクリーンの幅
    //  @param[in]  height_ : スクリーンの高さ
    //  @param[in]  near_   : 一番近いビュー平面の Z 値
    //  @param[in]  far_    : 一番遠いビュー平面の Z 値
    //*/
    //---------------------------------------------------------------------
	static void perspectiveFovRH( Matrix* out_, lnfloat fov_y_, lnfloat width_, lnfloat height_, lnfloat near_, lnfloat far_ );
	
    //---------------------------------------------------------------------
    ///**
    //  @brief      左手座標系正射影行列の作成
    //
    //  @param[out] out_    : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  width_  : スクリーンの幅
    //  @param[in]  height_ : スクリーンの高さ
    //  @param[in]  near_   : 一番近いビュー平面の Z 値
    //  @param[in]  far_    : 一番遠いビュー平面の Z 値
    //*/
    //---------------------------------------------------------------------
	static void orthoLH( Matrix* out_, lnfloat width_, lnfloat height_, lnfloat near_, lnfloat far_ );
	
    //---------------------------------------------------------------------
    ///**
    //  @brief      右手座標系正射影行列の作成
    //
    //  @param[out] out_    : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  width_  : スクリーンの幅
    //  @param[in]  height_ : スクリーンの高さ
    //  @param[in]  near_   : 一番近いビュー平面の Z 値
    //  @param[in]  far_    : 一番遠いビュー平面の Z 値
    //*/
    //---------------------------------------------------------------------
	static void orthoRH( Matrix* out_, lnfloat width_, lnfloat height_, lnfloat near_, lnfloat far_ );
	
    //---------------------------------------------------------------------
    ///**
    //  @brief      ピクセル単位の2D描画に使う射影行列の作成
    //
    //  @param[out] out_    : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  width_  : スクリーンの幅
    //  @param[in]  height_ : スクリーンの高さ
    //  @param[in]  near_   : 一番近いビュー平面の Z 値
    //  @param[in]  far_    : 一番遠いビュー平面の Z 値
    //
    //  @par
    //              ピクセル単位で定義したポリゴンの頂点に対して
    //              この行列で座標変換を行うと、スクリーンの左上を
    //              原点とした空間に射影されます。(下が Y+ 方向)<br>
    //              <br>
    //              また、2D描画の際、頂点座標に適用する -0.5 ピクセルのオフセットも
    //              この行列に含まれています。<br>
    //*/
    //---------------------------------------------------------------------
	static void perspective2DLH( Matrix* out_, lnfloat width_, lnfloat height_, lnfloat near_, lnfloat far_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      行列の 3x3 部分だけを乗算する (それ以外の要素はそのまま)
    //
    //  @param[out] out_  : 演算結果を格納する Matrix へのポインタ
    //  @param[in]  mat0_ : 処理の基になる Matrix
    //  @param[in]  mat1_ : 処理の基になる Matrix
    //*/
    //---------------------------------------------------------------------
	//static void mul3x3( Matrix* out_, const Matrix& mat0_, const Matrix& mat1_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      SQTTransform から変換する
    //
    //  @param[out] out_ : 演算結果を格納する Matrix へのポインタ
    //*/
    //---------------------------------------------------------------------
    static void createFromSQTTransform( Matrix* out_, const SQTTransform& t_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      SQTTransform との乗算
    //
    //  @param[out] out_ : 演算結果を格納する Matrix へのポインタ
    //*/
    //---------------------------------------------------------------------
    //static void multiply( Matrix* out_, const SQTTransform& t_, const Matrix& mat_ );
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Geometry
} // namespace Core

typedef Core::Geometry::Matrix	LMatrix;

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================