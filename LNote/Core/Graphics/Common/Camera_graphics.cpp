//=============================================================================
//�y Camera �z
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
// �� Camera �N���X
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
        
        out->y *= -1.0f;  // y �͉��� + �Ȃ̂ŏ㉺���]

        out->x = ( out->x + 1.0f ) * 0.5f;    // 0.0�`2.0  �ɂ������ƁA0.0�`1.0 �ɂ���
        out->y = ( out->y + 1.0f ) * 0.5f;    // (��������_�ɂ������ƁA)

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
            // 2D �J�����p
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
            // 3D �J�����p
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
// �� CylinderMouseMoveCameraOperator
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
            /////////////////////////////////////////////// �}�E�X�{�^���������ꂽ
            case LNEV_MOUSE_DOWN:
		    {
                // �E�{�^��
                if ( ev_mouse.Button == LN_MOUSE_RIGHT )
                {
                    mMouseData.RDrag = true;

                    // �}�E�X�ʒu�L��
                    mMouseData.PrevPos.x = ev_mouse.X;
                    mMouseData.PrevPos.y = ev_mouse.Y;
                    return true;
                    
                }
                // ���{�^��
                if ( ev_mouse.Button == LN_MOUSE_MIDDLE )
                {
                    mMouseData.MDrag = true;
                    mMouseData.PrevPos.x = ev_mouse.X;
                    mMouseData.PrevPos.y = ev_mouse.Y;
                    return true;
                }

			    break;
		    }

            /////////////////////////////////////////////// �}�E�X�{�^���������ꂽ
            case LNEV_MOUSE_UP:
		    {
                // �E�{�^��
                if ( ev_mouse.Button == LN_MOUSE_RIGHT )
                {
                    mMouseData.RDrag = false;
                    return true;
                }
                // ���{�^��
                if ( ev_mouse.Button == LN_MOUSE_MIDDLE )
                {
                    mMouseData.MDrag = false;
                    return true;
                }
			    break;
		    }

		    /////////////////////////////////////////////// �}�E�X�ړ�
            case LNEV_MOUSE_MOVE:
		    {
                // �E�{�^���h���b�O���̏ꍇ
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
				// ���{�^���h���b�O���̏ꍇ
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
			
            /////////////////////////////////////////////// �}�E�X�z�C�[�������삳�ꂽ
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

		// �����_���S��]
		if ( 1 )
		{
			view = pos - look_at;
		}
		// ���_���S��]
		else
		{
			view = look_at - pos;
		}

		LMatrix m;
		lnfloat d;

		if ( dx != 0 )
		{
			d = LMath::PI * dx / width;
			// Y ������̉�]���t�ɂ���ꍇ (�E��n)
			if ( 0 ) // CAMTYPE_REV_ROT_Y
			{
				d = -d;
			}
			
			// vup �����ɂ���ꍇ
			//if ( m_type & CAMTYPE_AROUND_UP )
			//	D3DXMatrixRotationAxis( &m, &vup, D3DXToRadian(d) );
			//else
			//	D3DXMatrixRotationY( &m, D3DXToRadian(d) );

			m.rotationY( d );
			view.transform( m );
		}
		if ( dy != 0 )
		{
			// ���^�C�v
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

		// �����_���S��]
		if ( 1 )
		{
            LVector3 old = pos;

			pos = look_at + view;

            // ��܂��͉��Ƀh���b�O���������ꍇ�A���V��ʂ�߂����Ƃ����
            // XZ��̔��Α��̏ی��ɃJ�������ړ����ĉ�ʂ����炿�炷��̂�h������
            if ( ( ( old.x < 0 && pos.x > 0 ) || ( old.x > 0 && pos.x < 0 ) ) &&
                 ( ( old.z < 0 && pos.z > 0 ) || ( old.z > 0 && pos.z < 0 ) ) )
            {
                pos = old;
            }

		}
		// ���_���S��]
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