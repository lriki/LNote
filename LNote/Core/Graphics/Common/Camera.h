//=============================================================================
//【 Camera 】
//-----------------------------------------------------------------------------
///**
//  @file       Camera.h
//  @brief      Camera
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../System/Event.h"
#include "../../Geometry/DrawingShapes.h"
#include "../../Geometry/Geometry.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
class CameraOperator;

//=============================================================================
// ■ Camera
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class Camera
    : public Base::ReferenceObject
{
public:

    Camera();

    virtual ~Camera();

public:

    void initialize( bool projection_2d );

    void setPosition( const LVector3& position );

	const LVector3& getPosition() const { return mPosition; }

	void setLookAt( const LVector3& at );

	const LVector3& getLookAt() const { return mLookAt; }

    void setUpDirection( const LVector3& up );

	const LVector3& getUpDirection() const { return mUpDirection; }

    const LVector3& getDirection() const { return mDirection; }

	void setFovY( lnfloat fov_y );

	lnfloat getFovY() const { return mFovY; }

	void setNearClip( lnfloat near_clip );

	lnfloat getNearClip() const { return mNearClip; }

	void setFarClip( lnfloat far_clip );

	lnfloat getFarClip() const { return mFarClip; }

    void setViewSize( const LVector2& size ) { mViewSize = size; mModified = true; }

	const LVector2& getViewSize() const { return mViewSize; }

    const LMatrix& getViewMatrix() const { _updateMatrix(); return mViewMatrix; }

	const LMatrix& getProjectionMatrix() const { _updateMatrix(); return mProjectionMatrix; }

	const LMatrix& getViewProjectionMatrix() const { _updateMatrix(); return mViewProjMatrix; }

	const LMatrix& getViewMatrixInverse() const { _updateMatrix(); return mViewProjMatrixInv; }

    const LViewFrustum& getViewFrustum() const { _updateMatrix(); return mViewFrustum; }

public:

    /// カメラ操作クラスの設定 (参照カウントはインクリメントしないが、デストラクト時に release())
    void setCameraOperator( CameraOperator* camera_operator );

    /// カメラから見た 3D 座標を 2D 座標に変換する
    void convertWorldPosToScreenPos( LVector3* out, const LVector3& world_pos, const LVector2& view_size ) const;

    /// (オーバーライドしているわけではないので注意)
    bool onEvent( const System::Event& args );

private:

    void _updateMatrix() const;

private:

    CameraOperator*         mCameraOperator;

    LVector3	            mPosition;
	LVector3	            mLookAt;
	LVector3	            mUpDirection;
    LVector3                mDirection;

    lnfloat                 mFovY;
    lnfloat                 mNearClip;
    lnfloat                 mFarClip;
    LVector2                mViewSize;

    mutable LMatrix         mViewMatrix;        ///< ビュー行列
    mutable LMatrix         mProjectionMatrix;  ///< プロジェクション行列
    mutable LMatrix         mViewProjMatrix;    ///< ビュー行列とプロジェクション行列の積
    mutable LMatrix         mViewProjMatrixInv; ///< ビュー行列の逆行列
    mutable LViewFrustum    mViewFrustum;

    mutable bool            mModified;
    bool                    mProjection2D;
};

//=============================================================================
// ■ CameraOperator
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class CameraOperator
    : public Base::ReferenceObject
{
public:

    virtual ~CameraOperator() {}

    virtual bool onEvent( const System::Event& args ) = 0;

    Camera* getTargetCamera() const { return mTargetCamera; }

private:

    Camera* mTargetCamera;
    friend class Camera;
};

//=============================================================================
// ■ CylinderMouseMoveCameraOperator
//-----------------------------------------------------------------------------
///**
//  @brief		マウスによって操作する (ろくろ)
//*/
//=============================================================================
class CylinderMouseMoveCameraOperator
    : public CameraOperator
{
public:

    static CylinderMouseMoveCameraOperator* create() { return LN_NEW CylinderMouseMoveCameraOperator(); }

public:

    CylinderMouseMoveCameraOperator();

    virtual bool onEvent( const System::Event& args );

private:

    void _doMouseMoveR( lnfloat dx, lnfloat dy, lnfloat width, lnfloat height );

	void _doMouseMoveM( lnfloat dx, lnfloat dy );

	void _doMouseWheelEvent( int pos );

private:

    struct
    {
        Geometry::Point	PrevPos;
        bool			RDrag;
        bool			MDrag;

    } mMouseData;
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================