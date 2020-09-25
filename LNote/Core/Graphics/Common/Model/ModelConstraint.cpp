//=============================================================================
//【 ModelConstraint 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "ModelCore.h"
#include "ModelRigidBody.h"
#include "ModelConstraint.h"

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
// ■ ModelConstraint クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    ModelConstraint::ModelConstraint()
        : mBtDofConstraint  ( NULL )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    ModelConstraint::~ModelConstraint()
    {
       
    }

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    void ModelConstraint::initializePMD( ModelManager* manager_, const PMD_Constraint* pmd_constraint_, ModelRigidBody* body_a_, ModelRigidBody* body_b_ )
    {
        btVector3   mRotation;
        btVector3   mPosition;
        btVector3   mPosLimitL;
        btVector3   mPosLimitU;
        btVector3   mRotLimitL;
        btVector3   mRotLimitU;
        btVector3   mSpringPos;
        btVector3   mSpringRot;

        mRotation.setValue( pmd_constraint_->vec3Rotation.x, pmd_constraint_->vec3Rotation.y, pmd_constraint_->vec3Rotation.z );
        mPosition.setValue( pmd_constraint_->vec3Position.x, pmd_constraint_->vec3Position.y, pmd_constraint_->vec3Position.z );
        mPosLimitL.setValue( pmd_constraint_->vec3PosLimitL.x, pmd_constraint_->vec3PosLimitL.y, pmd_constraint_->vec3PosLimitL.z );
        mPosLimitU.setValue( pmd_constraint_->vec3PosLimitU.x, pmd_constraint_->vec3PosLimitU.y, pmd_constraint_->vec3PosLimitU.z );
        mRotLimitL.setValue( pmd_constraint_->vec3RotLimitL.x, pmd_constraint_->vec3RotLimitL.y, pmd_constraint_->vec3RotLimitL.z );
        mRotLimitU.setValue( pmd_constraint_->vec3RotLimitU.x, pmd_constraint_->vec3RotLimitU.y, pmd_constraint_->vec3RotLimitU.z );
        mSpringPos.setValue( pmd_constraint_->vec3SpringPos.x, pmd_constraint_->vec3SpringPos.y, pmd_constraint_->vec3SpringPos.z );
        mSpringRot.setValue( pmd_constraint_->vec3SpringRot.x, pmd_constraint_->vec3SpringRot.y, pmd_constraint_->vec3SpringRot.z );


        //_p(pmd_constraint_->szName);
        //_p(pmd_constraint_->ulRigidA);
        //_p(pmd_constraint_->ulRigidB);

        // 回転行列作成
	    btMatrix3x3	btmRotationMat;
	    btmRotationMat.setEulerZYX( mRotation.x(), mRotation.y(), mRotation.z() );

	    // コンストレイントのトランスフォームを作成
	    btTransform bttrTransform;
	    bttrTransform.setIdentity();
	    bttrTransform.setOrigin( mPosition );
	    bttrTransform.setBasis( btmRotationMat );

	    // 剛体A,Bから見たコンストレイントのトランスフォームを作成 
	    btTransform		bttrRigidAInvTransform = body_a_->getBtRigidBody()->getWorldTransform().inverse(),
					    bttrRigidBInvTransform = body_b_->getBtRigidBody()->getWorldTransform().inverse();

	    bttrRigidAInvTransform = bttrRigidAInvTransform * bttrTransform;
	    bttrRigidBInvTransform = bttrRigidBInvTransform * bttrTransform;

	    mBtDofConstraint = new btGeneric6DofSpringConstraint(
            *body_a_->getBtRigidBody(),
            *body_b_->getBtRigidBody(),
		    bttrRigidAInvTransform,
            bttrRigidBInvTransform,
            true );

	    // 各種制限パラメータのセット
	    mBtDofConstraint->setLinearLowerLimit( mPosLimitL );
	    mBtDofConstraint->setLinearUpperLimit( mPosLimitU );

	    mBtDofConstraint->setAngularLowerLimit( mRotLimitL );
	    mBtDofConstraint->setAngularUpperLimit( mRotLimitU );

		    // 0 : translation X
	    if ( mSpringPos.x() != 0.0f )
	    {
		    mBtDofConstraint->enableSpring( 0, true );
		    mBtDofConstraint->setStiffness( 0, mSpringPos.x() );
	    }

		    // 1 : translation Y
	    if ( mSpringPos.y() != 0.0f )
	    {
		    mBtDofConstraint->enableSpring( 1, true );
		    mBtDofConstraint->setStiffness( 1, mSpringPos.y() );
	    }

		    // 2 : translation Z
	    if ( mSpringPos.z() != 0.0f )
	    {
		    mBtDofConstraint->enableSpring( 2, true );
		    mBtDofConstraint->setStiffness( 2, mSpringPos.z() );
	    }

		    // 3 : rotation X (3rd Euler rotational around new position of X axis, range [-PI+epsilon, PI-epsilon] )
		    // 4 : rotation Y (2nd Euler rotational around new position of Y axis, range [-PI/2+epsilon, PI/2-epsilon] )
		    // 5 : rotation Z (1st Euler rotational around Z axis, range [-PI+epsilon, PI-epsilon] )
	    mBtDofConstraint->enableSpring( 3, true );	mBtDofConstraint->setStiffness( 3, mSpringRot.x() );
	    mBtDofConstraint->enableSpring( 4, true );	mBtDofConstraint->setStiffness( 4, mSpringRot.y() );
	    mBtDofConstraint->enableSpring( 5, true );	mBtDofConstraint->setStiffness( 5, mSpringRot.z() );

	    // シミュレーションワールドに追加
        Physics::JointBase::initialize( manager_->getPhysicsManager(), mBtDofConstraint );
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