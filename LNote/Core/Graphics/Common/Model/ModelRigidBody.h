//=============================================================================
//�y ModelRigidBody �z
//-----------------------------------------------------------------------------
///**
//  @file       ModelRigidBody.h
//  @brief      ModelRigidBody
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <btBulletDynamicsCommon.h>
#include "../../../Base/RefString.h"
#include "../../../Physics/RigidBody/RigidBody.h"
#include "PMDTypes.h"
#include "ModelManager.h"
#include "ModelFrame.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
class ModelManager;


enum CollisionShapeType
{
    COLSHAPE_SPHERE = 0,    ///< ��
    COLSHAPE_BOX,           ///< ��
    COLSHAPE_CAPSULE,       ///< �J�v�Z��
};

enum RigidBodyType
{
    RIGIDBODYTYPE_CONTROLLED_BY_BONE = 0,   ///< Bone�Ǐ]
    RIGIDBODYTYPE_PHYSICS,                  ///< �������Z
    RIGIDBODYTYPE_PHYSICS_ALIGNMENT,        ///< �������Z(Bone�ʒu����)
}; 

//=============================================================================
// �� ModelRigidBody �N���X
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class ModelRigidBody
    : public Physics::RigidBody
{
public:

    ModelRigidBody() {}   // ����̃f�t�H���g�R���X�g���N�^�Bnew ����ɔz�u new �ŏ��������邱�ƁB
    ModelRigidBody( ModelManager* manager_ );

    virtual ~ModelRigidBody();

public:

    /// ������
    void initializePMD( const PMD_RigidBody *pmd_rigidbody_, ModelFrame* frame_ );


    /// �{�[���ʒu���킹 (RIGIDBODYTYPE_PHYSICS_ALIGNMENT �̂Ƃ��̂ݗL���B�{�[���̈ʒu�ɍ��̂��ړ�����)
    void fixPosition( float fElapsedFrame );
		
    /// �{�[���̎p�������̂̎p���ƈ�v������(���̃t���[���̃V�~�����[�V�����I����ɌĂ�)
    void updateBoneTransform();
		
    /// ���̂��{�[���̈ʒu�֋����I�Ɉړ�������
    void moveToBonePos();


    /// �f�o�b�O�p�̌`��`��
    virtual LNRESULT drawDebugShape( Physics::IDebugRenderer* renderer_ );

    

private:

    /// ���
    void _release();

private:

    /// �`��f�[�^
    struct CollisionShapeData
    {
        CollisionShapeType Type;
        union
        {
            struct
            {
                lnfloat Radius;
            } Sphere;

            struct
            {
                lnfloat Width;
                lnfloat Height;
	            lnfloat Depth;
            } Box;

            struct
            {
                lnfloat Radius;
                lnfloat Height;
            } Capsule;
        };
    };

private:

    ModelFrame*         mFrame;

    CollisionShapeData  mColShapeData;
    btCollisionShape*   mColShape;

    RigidBodyType       mRigidBodyType;
    //btRigidBody*        mRigidBody;

    lnU16	            mColGroup;          // �Փ˃O���[�v
	lnU16	            mColGroupMask;	    // �Փ˃O���[�v�}�X�N
    LVector3	        mInitialPosition;	// �ʒu(�{�[������)
	LVector3	        mInitialRotation;	// ��](radian)
    lnfloat             mMass;			    // ����
	lnfloat			    mLinearDamping;	    // �ړ���
	lnfloat			    mAngularDamping;    // ��]��
	lnfloat			    mRestitution;	    // ������
	lnfloat			    mFriction;		    // ���C��

    btTransform			mBtBoneOffset;
    btTransform			mBtInvBoneOffset;

    bool                mNoCopyToBone;

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