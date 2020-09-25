//=============================================================================
//【 LCamera 】
//-----------------------------------------------------------------------------
///**
//  @file       LCamera.h
//  @brief      LCamera
//  @version    1.0
//  @date       2012/2/7
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Core/Scene/Manager.h"
#include "../../Core/Scene/SceneGraph.h"
#include "../../Core/Scene/SceneNode/Camera.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// ■ LCamera クラス
//-----------------------------------------------------------------------------
///**
//  @brief      カメラのクラス
//*/
//=============================================================================
class LCamera
    : public LRefClass< LCamera, Core::Scene::Camera >
{
public:

    /// デフォルトカメラの取得
    static LCamera getDefaultCamera();

public:

    /// 位置の取得
    const LVector3& getPosition() const;

	/// 位置の設定
    void setPosition( const LVector3& position_ );

	/// 注視点の取得
    const LVector3& getLookAt() const;

	/// 注視点の設定
    void setLookAt( const LVector3& at_ );

	/// 上方向ベクトルの取得
    const LVector3& getUpDirection() const;

	/// 上方向ベクトルの設定
    void setUpDirection( const LVector3& up_ );

    /// Y 方向視野角の取得
    lnfloat getFovY() const;

	/// Y 方向視野角の設定
    void setFovY( lnfloat fov_y_ );

    /// 最も近いビュープレーン位置の取得
    lnfloat getNearClip() const;

	/// 最も近いビュープレーン位置の設定
    void setNearClip( lnfloat near_ );

    /// 最も遠いビュープレーン位置の取得
    lnfloat getFarClip() const;

	/// 最も遠いビュープレーン位置の設定
    void setFarClip( lnfloat far_ );

    /// カメラタイプの設定
    void setType( LNCameraType type_ );

    /// カメラから見た 3D 座標を 2D 座標に変換する
    void convertWorldPosToScreenPos( LVector3* out_, const LVector3& world_pos_ );

    /// ビュー行列の取得
    const LMatrix& getViewMatrix();

    /// プロジェクション行列の取得
    const LMatrix& getProjectionMatrix();

    /// ビュー行列とプロジェクション行列の積の取得
    const LMatrix& getViewProjectionMatrix();

    /// ビュー行列の逆行列の取得
    const LMatrix& getViewMatrixInverse();

    /// 正射影行列の取得
    const LMatrix& getOrthoMatrix();


    LN_UTIL_REFCLASS_METHODS( LCamera, Core::Scene::Camera );
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================