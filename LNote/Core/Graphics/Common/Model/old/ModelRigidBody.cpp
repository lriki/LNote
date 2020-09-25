//=============================================================================
//【 ModelRigidBody 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <btBulletDynamicsCommon.h>
#include "ModelFrame.h"
#include "ModelCore.h"
#include "ModelRigidBody.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Graphics
{

/// Kinematic用モーションステート (ボーン -> 剛体)
struct KinematicMotionState
    : public btMotionState
{
	btTransform m_graphicsWorldTrans;
	btTransform	m_BoneOffset;
	btTransform m_startWorldTrans;
	ModelFrame* mFrame;

	KinematicMotionState( const btTransform& startTrans, const btTransform& boneOffset, ModelFrame* frame_ )
        : m_BoneOffset( boneOffset )
        , m_startWorldTrans( startTrans )
        , mFrame( frame_ )
	{
	}

	///synchronizes world transform from user to physics
	virtual void getWorldTransform( btTransform& centerOfMassWorldTrans ) const
	{
		btTransform		bttrBoneTransform;

        LMatrix gl_mat = mFrame->getWorldMatrix();
        
		bttrBoneTransform.setFromOpenGLMatrix( (float *)(&gl_mat) );

		centerOfMassWorldTrans = bttrBoneTransform * m_BoneOffset;
	}

	///synchronizes world transform from physics to user
	///Bullet only calls the update of worldtransform for active objects
	virtual void setWorldTransform( const btTransform& centerOfMassWorldTrans )
	{
		m_graphicsWorldTrans = centerOfMassWorldTrans;
	}
};


//=============================================================================
// ■ ModelRigidBody クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    ModelRigidBody::ModelRigidBody( ModelManager* manager_ )
        : Physics::RigidBody    ( manager_->getPhysicsManager() )
        , mColShape             ( NULL )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    ModelRigidBody::~ModelRigidBody()
    {
        _release();
    }

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    void ModelRigidBody::initializePMD( const PMD_RigidBody *pmd_rigidbody_, ModelFrame* frame_ )
    {
        mFrame = frame_;

        // シェイプの作成
	    switch ( pmd_rigidbody_->cbShapeType )
	    {
		    case 0 :	// 球
                mColShapeData.Type = COLSHAPE_SPHERE;
                mColShapeData.Sphere.Radius = pmd_rigidbody_->fWidth;
			    mColShape = new btSphereShape( mColShapeData.Sphere.Radius );
			    break;

		    case 1 :	// 箱
                mColShapeData.Type = COLSHAPE_BOX;
                mColShapeData.Box.Width  = pmd_rigidbody_->fWidth;
                mColShapeData.Box.Height = pmd_rigidbody_->fHeight;
                mColShapeData.Box.Depth  = pmd_rigidbody_->fDepth;
			    mColShape = new btBoxShape( btVector3( mColShapeData.Box.Width, mColShapeData.Box.Height, mColShapeData.Box.Depth ) );
			    break;

		    case 2 :	// カプセル
                mColShapeData.Type = COLSHAPE_CAPSULE;
                mColShapeData.Capsule.Radius = pmd_rigidbody_->fWidth;
                mColShapeData.Capsule.Height = pmd_rigidbody_->fHeight;
			    mColShape = new btCapsuleShape( mColShapeData.Capsule.Radius, mColShapeData.Capsule.Height );
			    break;
	    }

        // 剛体の種類
        switch( pmd_rigidbody_->cbRigidBodyType )
	    {
            default:
            case 0: 
            	mRigidBodyType = RIGIDBODYTYPE_CONTROLLED_BY_BONE; break;
            case 1: 
            	mRigidBodyType = RIGIDBODYTYPE_PHYSICS; break;
            case 2: 
            	mRigidBodyType = RIGIDBODYTYPE_PHYSICS_ALIGNMENT; break;
        }

        mColGroup           = 0x0001 << pmd_rigidbody_->cbColGroupIndex;
        mColGroupMask       = pmd_rigidbody_->unColGroupMask;
        mInitialPosition    = pmd_rigidbody_->vec3Position;
        mInitialRotation    = pmd_rigidbody_->vec3Rotation;
        mMass               = pmd_rigidbody_->fMass;
        mLinearDamping      = pmd_rigidbody_->fLinearDamping;
        mAngularDamping     = pmd_rigidbody_->fAngularDamping;
        mRestitution        = pmd_rigidbody_->fRestitution;
        mFriction           = pmd_rigidbody_->fFriction;
        

        // ボーンオフセット用トランスフォーム作成
	    btMatrix3x3	btmRotationMat;
	    btmRotationMat.setEulerZYX( mInitialRotation.x, mInitialRotation.y, mInitialRotation.z );

	    mBtBoneOffset.setIdentity();
	    mBtBoneOffset.setOrigin( btVector3( mInitialPosition.x, mInitialPosition.y, mInitialPosition.z ) );
	    mBtBoneOffset.setBasis( btmRotationMat );

        mBtInvBoneOffset = mBtBoneOffset.inverse();

        if ( mFrame->getName() == _T( "センター" ) )    mNoCopyToBone = true;
	    else										    mNoCopyToBone = false;





        

	    // 質量と慣性テンソルの設定
	    btScalar	btsMass( 0.0f );
	    btVector3	btv3LocalInertia( 0.0f, 0.0f ,0.0f );

	    // ボーン追従でない場合は質量を設定
	    if ( mRigidBodyType != RIGIDBODYTYPE_CONTROLLED_BY_BONE )	btsMass = mMass;

	    // 慣性テンソルの計算
	    if ( btsMass != 0.0f )	mColShape->calculateLocalInertia( btsMass, btv3LocalInertia );

	    // ボーンの位置取得
	    LVector3		vec3BonePos = mFrame->getWorldMatrix().getPosition();

	    // 剛体の初期トランスフォーム作成
	    btTransform		bttrTransform;

	    bttrTransform.setIdentity();
	    bttrTransform.setOrigin( btVector3( vec3BonePos.x, vec3BonePos.y, vec3BonePos.z ) );
	    bttrTransform = bttrTransform * mBtBoneOffset;

	    // MotionStateの作成
	    btMotionState	*pbtMotionState;

	    switch( mRigidBodyType )
	    {
		    case RIGIDBODYTYPE_CONTROLLED_BY_BONE : 
		    	pbtMotionState = new KinematicMotionState( bttrTransform, mBtBoneOffset, mFrame ); break;
		    case RIGIDBODYTYPE_PHYSICS            : 
		    	pbtMotionState = new btDefaultMotionState( bttrTransform ); break;
		    case RIGIDBODYTYPE_PHYSICS_ALIGNMENT  : 
		    	pbtMotionState = new btDefaultMotionState( bttrTransform ); break;
		    //case 2 : pbtMotionState = new btKinematicMotionState( bttrTransform, bttrBoneOffset, pBone );	break;
	    }

	    // 剛体のパラメータの設定
	    btRigidBody::btRigidBodyConstructionInfo	btRbInfo( btsMass, pbtMotionState, mColShape, btv3LocalInertia );
	    btRbInfo.m_linearDamping  = mLinearDamping;	    // 移動減
	    btRbInfo.m_angularDamping = mAngularDamping;	// 回転減
	    btRbInfo.m_restitution    = mRestitution;	    // 反発力
	    btRbInfo.m_friction       = mFriction;		    // 摩擦力
	    btRbInfo.m_additionalDamping = true;


	    // 剛体の作成
	    btRigidBody* rigit_body = new btRigidBody( btRbInfo );

	    // Bone 追従の場合、Kinematic設定
	    if ( mRigidBodyType == RIGIDBODYTYPE_CONTROLLED_BY_BONE )
	    {
            // btCollisionObject::CF_KINEMATIC_OBJECT フラグを追加しておくと、
            // btMotionState::getWorldTransform() が毎フレーム呼ばれる
		    rigit_body->setCollisionFlags( rigit_body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
            rigit_body->setActivationState( rigit_body->getActivationState() | DISABLE_DEACTIVATION );
	    }
	    rigit_body->setSleepingThresholds( 0.0f, 0.0f );

	    // 剛体をシミュレーションワールドに追加
        Physics::RigidBodyInitArgs args;
        args.Shape      = mColShape;
        args.RigidBody  = rigit_body;
        args.Group      = mColGroup;
        args.Mask       = mColGroupMask;
        Physics::RigidBody::initialize( args );
    }


    //---------------------------------------------------------------------
	// ● 解放
    //---------------------------------------------------------------------
    void ModelRigidBody::_release()
    {
    }

    //---------------------------------------------------------------------
	// ● ボーン位置合わせ
    //---------------------------------------------------------------------
    void ModelRigidBody::fixPosition( float fElapsedFrame )
    {
	    if ( mRigidBodyType == RIGIDBODYTYPE_PHYSICS_ALIGNMENT )
	    {
            const LVector3& bone_pos = mFrame->getWorldMatrix().getPosition();

		    btTransform rb_transform = this->mBtRigidBody->getCenterOfMassTransform();

		    rb_transform.setOrigin( btVector3( 0.0f, 0.0f, 0.0f ) );
		    rb_transform = mBtBoneOffset * rb_transform;

		    rb_transform.setOrigin( rb_transform.getOrigin() + btVector3( bone_pos.x, bone_pos.y, bone_pos.z ) );
    //		rb_transform.setBasis( m_pbtRigidBody->getWorldTransform().getBasis() );

    //		this->mBtRigidBody->setCenterOfMassTransform( bttrRbTransform );

		    float	fRate = 0.2f * fElapsedFrame;
		    if( fRate > 1.0f )	fRate = 1.0f;
		    this->mBtRigidBody->translate(
                (rb_transform.getOrigin() - this->mBtRigidBody->getCenterOfMassTransform().getOrigin()) * fRate );
	    }
    }

    //---------------------------------------------------------------------
	// ● ボーンの姿勢を剛体の姿勢と一致させる(そのフレームのシミュレーション終了後に呼ぶ)
    //---------------------------------------------------------------------
    void ModelRigidBody::updateBoneTransform()
    {
        if ( mRigidBodyType != RIGIDBODYTYPE_CONTROLLED_BY_BONE && !mNoCopyToBone )
	    {
		    btTransform		bttrBoneTransform = mBtRigidBody->getCenterOfMassTransform() * mBtInvBoneOffset;

		    bttrBoneTransform.getOpenGLMatrix( (float *)mFrame->getWorldMatrixPtr() );
	    }
    }

    //---------------------------------------------------------------------
	// ● 剛体をボーンの位置へ強制的に移動させる
    //---------------------------------------------------------------------
    void ModelRigidBody::moveToBonePos()
    {
	    if ( mRigidBodyType != RIGIDBODYTYPE_CONTROLLED_BY_BONE )
	    {
            const btVector3 ZERO( 0.0f, 0.0f, 0.0f );

            
            const LVector3& bone_pos = mFrame->getWorldMatrix().getPosition();
		    btTransform		bt_transform = mBtRigidBody->getCenterOfMassTransform();

		    bt_transform.setOrigin( ZERO );
		    bt_transform = mBtBoneOffset * bt_transform;

		    bt_transform.setOrigin( bt_transform.getOrigin() + btVector3( bone_pos.x, bone_pos.y, bone_pos.z ) );
		    bt_transform.setBasis( mBtRigidBody->getWorldTransform().getBasis() );


            LQuaternion q;
            LQuaternion::rotationMatrix( &q, mFrame->getWorldMatrix() );

            
            bt_transform.setRotation( btQuaternion( q.x, q.y, q.z, q.w ) );

		    mBtRigidBody->setCenterOfMassTransform( bt_transform );
            
           
		    mBtRigidBody->setLinearVelocity( ZERO );
		    mBtRigidBody->setAngularVelocity( ZERO );
		    mBtRigidBody->setInterpolationLinearVelocity( ZERO );
		    mBtRigidBody->setInterpolationAngularVelocity( ZERO );
		    mBtRigidBody->setInterpolationWorldTransform( mBtRigidBody->getCenterOfMassTransform() );
		    mBtRigidBody->clearForces();


            mBtRigidBody->activate(true);
	    }
    }

    //---------------------------------------------------------------------
	// ● デバッグ用の形状描画
    //---------------------------------------------------------------------
    LNRESULT ModelRigidBody::drawDebugShape( Physics::IDebugRenderer* renderer_ )
    {
        switch ( mColShapeData.Type )
	    {
            case COLSHAPE_SPHERE:
            {
                renderer_->setMatrix( this->mWorldMatrix );
                return renderer_->drawSphere( LVector3::ZERO, mColShapeData.Sphere.Radius, LColor( 0, 0, 1, 0.5f ) );
            }
            case COLSHAPE_BOX:
            {
                LVector3 bbmin( -mColShapeData.Box.Width * 0.5f, -mColShapeData.Box.Height * 0.5f, -mColShapeData.Box.Depth * 0.5f );
                LVector3 bbmax(  mColShapeData.Box.Width * 0.5f,  mColShapeData.Box.Height * 0.5f,  mColShapeData.Box.Depth * 0.5f );
                renderer_->setMatrix( this->mWorldMatrix );
                return renderer_->drawBox( bbmin, bbmax, LColor( 0, 0, 1, 0.5f ) );
            }
            case COLSHAPE_CAPSULE:
            {
                renderer_->setMatrix( this->mWorldMatrix );
                return renderer_->drawCapsule( mColShapeData.Capsule.Radius, mColShapeData.Capsule.Height, LColor( 0, 0, 1, 0.5f ) );
            }
        }
        return LN_OK;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Lumine

//=============================================================================
//								end of file
//=============================================================================