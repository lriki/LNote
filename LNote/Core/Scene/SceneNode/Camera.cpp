//=============================================================================
//【 Camera 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../SceneGraph.h"
#include "Camera.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

//=============================================================================
// ■ Camera クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    Camera::Camera( SceneGraph* scene_ )
        : mScene                ( scene_ )
		, mHolderPane			( NULL )
        , mZSortFunc            ( LN_ZSORTF_IMM_Z )
        , mContext              ( NULL )
        , mMemberChanged        ( false )
        , mProjection2D         ( false )
    {
        if ( mScene )
        {
            mScene->addCamera( this );
        }
        LN_SAFE_ADDREF( mScene );
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    Camera::~Camera()
    {
        SAFE_DELETE( mContext );
        if ( mScene )
        {
            mScene->removeCamera( this );
        }
        LN_SAFE_RELEASE( mScene );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Camera::initialize( bool projection_2d_ )
    {
        mCoreCamera.initialize( projection_2d_ );
        mContext = LN_NEW CameraContext();
        mContext->initialize( this );
    }

    //---------------------------------------------------------------------
	// ● カメラから見た 3D 座標を 2D 座標に変換する
    //---------------------------------------------------------------------
    void Camera::convertWorldPosToScreenPos( LVector3* out, const LVector3& world_pos ) const
    {
		mCoreCamera.convertWorldPosToScreenPos( out, world_pos, mHolderPane->getViewSize() );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
	void Camera::setHolderPane( Pane* pane )
    {
        mHolderPane = pane;
        mCoreCamera.setViewSize( mHolderPane->getViewSize() );
    }

    //---------------------------------------------------------------------
	// ● カメラ単位でシェーダに送るデータの取得
    //---------------------------------------------------------------------
    LNCameraSceneParam* Camera::getCameraSceneParam()
    {
        mCameraSceneParam.Mat_CameraView     = &mCoreCamera.getViewMatrix();
        mCameraSceneParam.Mat_CameraProj     = &mCoreCamera.getProjectionMatrix();
        mCameraSceneParam.Mat_CameraViewProj = &mCoreCamera.getViewProjectionMatrix();
        mCameraSceneParam.CameraPosition     = LVector4( mCoreCamera.getPosition() );
        mCameraSceneParam.CameraDirection    = LVector4( mCoreCamera.getDirection() );
        mCameraSceneParam.calcMatrices();
        return &mCameraSceneParam;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    LNRESULT Camera::onEvent( const System::Event& args )
    {
        if ( mCoreCamera.onEvent( args ) ) return LN_HANDLED;
        return LN_OK;
    }

//=============================================================================
// ■ CameraContext
//=============================================================================

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    CameraContext::CameraContext()
        : mCamera       ( NULL )
        , mFovY         ( 0 )
        , mNearClip     ( 0 )
        , mFarClip      ( 0 )
        , mZSortFunc    ( LN_ZSORTF_IMM_Z )
        , mProjection2D ( false )
        , mOrthographic ( false )
    {
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    CameraContext::~CameraContext()
    {
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void CameraContext::initialize( Camera* camera_ )
    {
        mCamera = camera_;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void CameraContext::updateContext()
    {
        mPosition = LVector4( mCamera->mCoreCamera.getPosition(), 0 );
        mDirection = mCamera->mCoreCamera.getDirection();

        mFovY = mCamera->mCoreCamera.getFovY();
        mNearClip = mCamera->mCoreCamera.getNearClip();
        mFarClip = mCamera->mCoreCamera.getFarClip();

        mViewMatrix = mCamera->mCoreCamera.getViewMatrix();
        mProjectionMatrix= mCamera->mCoreCamera.getProjectionMatrix();
        mViewProjMatrix = mCamera->mCoreCamera.getViewProjectionMatrix();

        mZSortFunc = mCamera->mZSortFunc;

        mCameraSceneParam.Mat_CameraView = &mViewMatrix;
        mCameraSceneParam.Mat_CameraProj = &mProjectionMatrix;
        mCameraSceneParam.Mat_CameraViewProj = &mViewProjMatrix;
        mCameraSceneParam.CameraPosition = mPosition;
        mCameraSceneParam.CameraDirection = LVector4( mDirection );
        //mCameraSceneParam.calcMatrices();

        mProjection2D = mCamera->mProjection2D;
        mOrthographic = mCamera->mOrthographic;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void CameraContext::setProjectionMatrix( const LMatrix& proj_ )
    {
        mProjectionMatrix = proj_;
        mViewProjMatrix = mViewMatrix * mProjectionMatrix;
        mCameraSceneParam.calcMatrices();
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================