//=============================================================================
//【 Camera 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Camera.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

//=============================================================================
// ■ Camera クラス
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    Camera::Camera()
        : mCameraOperator   ( NULL )
        , mProjection2D     ( false )
        , mModified         ( false )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    Camera::~Camera()
    {
        LN_SAFE_RELEASE( mCameraOperator );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Camera::initialize( bool projection_2d )
    {
        mProjection2D = projection_2d;
        if ( mProjection2D )
        {
            mPosition.set( 0.0f, 0.0f, 0.0f );
            mNearClip = 0.0f;
            mFarClip = 10000.0f;
        }
        else
        {
            mPosition.set( 0.0f, 2.0f, -10.0f );
            mNearClip = 1.0f;
            mFarClip = 1000.0f;
        }

        mLookAt.set( 0, 0, 0 );
		mUpDirection.set( 0, 1, 0 );
        mDirection = mLookAt - mPosition;
        mDirection.normalize();

        mFovY = LMath::PI * 0.125f;

		mModified = true;
    }

    //---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void Camera::setPosition( const LVector3& pos )
	{
		mPosition = pos;
        mDirection = mLookAt - mPosition;
        mDirection.normalize();
		mModified = true;
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void Camera::setLookAt( const LVector3& at )
	{
		mLookAt = at;
        mDirection = mLookAt - mPosition;
        mDirection.normalize();
		mModified = true;
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void Camera::setUpDirection( const LVector3& up )
	{
		mUpDirection = up;
		mModified = true;
	}

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
	void Camera::setFovY( lnfloat fov_y )
    {
        mFovY = fov_y;
        mModified = true;
    }

	//---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
	void Camera::setNearClip( lnfloat near_clip )
    {
        mNearClip = near_clip;
        mModified = true;
    }

	//---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
	void Camera::setFarClip( lnfloat far_clip )
    {
        mFarClip = far_clip;
        mModified = true;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Camera::setCameraOperator( CameraOperator* camera_operator )
    {
        mCameraOperator = camera_operator;
        if ( mCameraOperator )
            mCameraOperator->mTargetCamera = this;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Camera::convertWorldPosToScreenPos( LVector3* out, const LVector3& world_pos, const LVector2& view_size ) const
    {
		_updateMatrix();

        LVector3::transformCoord( out, world_pos, mViewProjMatrix );
        
        out->y *= -1.0f;  // y は下が + なので上下反転

        out->x = ( out->x + 1.0f ) * 0.5f;    // 0.0～2.0  にしたあと、0.0～1.0 にする
        out->y = ( out->y + 1.0f ) * 0.5f;    // (左上を原点にしたあと、)

        out->x *= view_size.x;
        out->y *= view_size.y;
    }

    //---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void Camera::_updateMatrix() const
	{
        if ( mModified )
		{
            // 2D カメラ用
            if ( mProjection2D )
            {
                LMatrix::translation(
                    &mViewMatrix, -mPosition.x, -mPosition.y, -mPosition.z );

                LMatrix::perspective2DLH( 
                    &mProjectionMatrix,
                    mViewSize.x, mViewSize.y, mNearClip, mFarClip );

                mViewProjMatrix = mViewMatrix * mProjectionMatrix;

                LMatrix::inverse( &mViewProjMatrixInv, mViewMatrix );

                mViewFrustum.create2DProjection( mViewSize.x, mViewSize.y, mNearClip, mFarClip );
                
            }
            // 3D カメラ用
            else
            {
                LMatrix::viewTransformLH( &mViewMatrix, mPosition, mLookAt, mUpDirection );
                LMatrix::perspectiveFovLH( &mProjectionMatrix, mFovY, mViewSize.x, mViewSize.y, mNearClip, mFarClip );
               
                mViewProjMatrix = mViewMatrix * mProjectionMatrix;

                LMatrix::inverse( &mViewProjMatrixInv, mViewMatrix );

                //LMath::MatrixOrthoLH( &mOrthoMatrix, mViewSize.x, mViewSize.y, 1000.0f, 0.0f );

                mViewFrustum.create( mFovY, mViewSize.x / mViewSize.y, mNearClip, mFarClip );

            }

            mModified = false;
        }
	}

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    bool Camera::onEvent( const System::Event& args )
    {
        if ( mCameraOperator && mCameraOperator->onEvent( args ) ) return true;
        return false;
    }

//=============================================================================
// ■ CylinderMouseMoveCameraOperator
//=============================================================================

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    CylinderMouseMoveCameraOperator::CylinderMouseMoveCameraOperator()
    {
        mMouseData.RDrag = false;
        mMouseData.MDrag = false;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    bool CylinderMouseMoveCameraOperator::onEvent( const System::Event& args_ )
    {
        const System::MouseEvent& ev_mouse = (const System::MouseEvent&)(args_);

        switch ( args_.Type )
        {
            /////////////////////////////////////////////// マウスボタンが押された
            case LNEV_MOUSE_DOWN:
		    {
                // 右ボタン
                if ( ev_mouse.Button == LN_MOUSE_RIGHT )
                {
                    mMouseData.RDrag = true;

                    // マウス位置記憶
                    mMouseData.PrevPos.x = ev_mouse.X;
                    mMouseData.PrevPos.y = ev_mouse.Y;
                    return true;
                    
                }
                // 中ボタン
                if ( ev_mouse.Button == LN_MOUSE_MIDDLE )
                {
                    mMouseData.MDrag = true;
                    mMouseData.PrevPos.x = ev_mouse.X;
                    mMouseData.PrevPos.y = ev_mouse.Y;
                    return true;
                }

			    break;
		    }

            /////////////////////////////////////////////// マウスボタンが離された
            case LNEV_MOUSE_UP:
		    {
                // 右ボタン
                if ( ev_mouse.Button == LN_MOUSE_RIGHT )
                {
                    mMouseData.RDrag = false;
                    return true;
                }
                // 中ボタン
                if ( ev_mouse.Button == LN_MOUSE_MIDDLE )
                {
                    mMouseData.MDrag = false;
                    return true;
                }
			    break;
		    }

		    /////////////////////////////////////////////// マウス移動
            case LNEV_MOUSE_MOVE:
		    {
                // 右ボタンドラッグ中の場合
	            if ( mMouseData.RDrag )
	            {
                    const LVector2& view_size = getTargetCamera()->getViewSize();

		            _doMouseMoveR(
			            static_cast< lnfloat >( ev_mouse.X - mMouseData.PrevPos.x ),
			            static_cast< lnfloat >( ev_mouse.Y - mMouseData.PrevPos.y ),
                        view_size.x, view_size.y );
		            mMouseData.PrevPos.x = ev_mouse.X;
                    mMouseData.PrevPos.y = ev_mouse.Y;
		            return true;
	            }
				// 中ボタンドラッグ中の場合
	            if ( mMouseData.MDrag )
	            {
		            _doMouseMoveM(
			            static_cast< lnfloat >( ev_mouse.X - mMouseData.PrevPos.x ),
			            static_cast< lnfloat >( ev_mouse.Y - mMouseData.PrevPos.y ) );
                    mMouseData.PrevPos.x = ev_mouse.X;
                    mMouseData.PrevPos.y = ev_mouse.Y;
		            return true;
	            }

			    break;
		    }
			
            /////////////////////////////////////////////// マウスホイールが操作された
		    case LNEV_MOUSE_WHEEL:
		    {
                _doMouseWheelEvent( ev_mouse.Delta );
			    return true;
		    }
        }

        return false;
    }

    //---------------------------------------------------------------------
    //
	//---------------------------------------------------------------------
	void CylinderMouseMoveCameraOperator::_doMouseMoveR( lnfloat dx, lnfloat dy, lnfloat width, lnfloat height )
	{
		LVector3 view;
		LVector3 vup = getTargetCamera()->getUpDirection();
        LVector3 pos = getTargetCamera()->getPosition();
        LVector3 look_at = getTargetCamera()->getLookAt();
		vup.normalize();

		// 注視点中心回転
		if ( 1 )
		{
			view = pos - look_at;
		}
		// 視点中心回転
		else
		{
			view = look_at - pos;
		}

		LMatrix m;
		lnfloat d;

		if ( dx != 0 )
		{
			d = LMath::PI * dx / width;
			// Y 軸周りの回転を逆にする場合 (右手系)
			if ( 0 ) // CAMTYPE_REV_ROT_Y
			{
				d = -d;
			}
			
			// vup を軸にする場合
			//if ( m_type & CAMTYPE_AROUND_UP )
			//	D3DXMatrixRotationAxis( &m, &vup, D3DXToRadian(d) );
			//else
			//	D3DXMatrixRotationY( &m, D3DXToRadian(d) );

			m.rotationY( d );
			view.transform( m );
		}
		if ( dy != 0 )
		{
			// 球タイプ
			if ( 1 )
			{
				LVector3 vaxis;
                LVector3::cross( &vaxis, vup, view );
				vaxis.normalize();
				d = -( LMath::PI * dy / height );
                
                LMatrix::rotationAxis( &m, vaxis, d );
				view.transform( m );


				//D3DXVec3Cross( &vaxis, &vup, &view );
				//D3DXVec3Normalize( &vaxis, &vaxis );
				//d = (float)180.0 * (float)dy / (float)prc->bottom;
				//D3DXMatrixRotationAxis( &m, &vaxis, D3DXToRadian(d) );
				//D3DXVec3TransformNormal( &view, &view, &m );
			}
			else
			{
				//if( m_type & CAMTYPE_AROUND_UP )
				//	view += ( vup * (float)dy );
				//else
				//	view.y += (float)dy;
			}
		}

		// 注視点中心回転
		if ( 1 )
		{
            LVector3 old = pos;

			pos = look_at + view;

            // 上または下にドラッグし続けた場合、中天を通り過ぎたところで
            // XZ上の反対側の象限にカメラが移動して画面がちらちらするのを防ぐ処理
            if ( ( ( old.x < 0 && pos.x > 0 ) || ( old.x > 0 && pos.x < 0 ) ) &&
                 ( ( old.z < 0 && pos.z > 0 ) || ( old.z > 0 && pos.z < 0 ) ) )
            {
                pos = old;
            }

		}
		// 視点中心回転
		else
		{
			pos += view;
		}

        getTargetCamera()->setPosition( pos );
	}

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
	void CylinderMouseMoveCameraOperator::_doMouseMoveM( lnfloat dx, lnfloat dy )
    {
        LVector3 pos = getTargetCamera()->getPosition();
        LVector3 look_at = getTargetCamera()->getLookAt();

        lnfloat s = 0.00175f * ( pos - look_at ).getLength( 0.0f );

		LVector3 view;
		LMatrix mat;
        LMatrix::inverse( &mat, getTargetCamera()->getViewMatrix() );
        mat.m30 = mat.m31 = mat.m32 = 0.0f;
		view.x = -dx * s;
		view.y = dy * s;
		view.z = 0.f;
		view.transform( mat );

        getTargetCamera()->setPosition( pos + view );
        getTargetCamera()->setLookAt( look_at + view );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
	void CylinderMouseMoveCameraOperator::_doMouseWheelEvent( int pos )
    {
        LVector3 view = getTargetCamera()->getPosition() - getTargetCamera()->getLookAt();
		if ( pos >= 0 )
		{
			view *= 0.9f;
		}
		else
		{
			view *= 1.1f;
		}
		getTargetCamera()->setPosition( getTargetCamera()->getLookAt() + view );
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================