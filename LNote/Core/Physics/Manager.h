//=============================================================================
//�y Manager �z
//-----------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include "../Geometry/Vector3.h"
#include "Types.h"

class  btDefaultCollisionConfiguration;
class  btCollisionDispatcher;
struct btDbvtBroadphase;
class  btAxisSweep3;
class  btSequentialImpulseConstraintSolver;
class  btDiscreteDynamicsWorld;
class btCollisionShape;
class btRigidBody;
class btTypedConstraint;
class btTriangleIndexVertexArray;

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Physics
{

//=============================================================================
// �� Manager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

    /// �������f�[�^
	struct InitData
	{
        Base::LogFile*      LogFile;
    };

public:

    Manager();

    LN_REFOBJ_METHODS;

public:

    /// ������
    LNRESULT initialize( const InitData& init_data_ );

    /// �I������
    void finalize();

    /// �t���[�����[�g�̐ݒ� (�f�t�H���g�� 60)
    void setFrameRate( int frame_rate_ );

    /// �V�~�����[�V�����X�V
    void update( lnfloat elapsed_time_ );

    /// �ڐG���Ă��镨�̂��`�F�b�N���ăC�x���g�𔭐�������
    void updateContactPoints();

    /// �d�͂̐ݒ�
    void setGravity( const LVector3& gravity_ );

    /// �{�b�N�X�̍쐬
    Box* createBox( const LVector3& size_, const LVector3 pos_, lnfloat mass_, lnU16 group_ = 0xffff, lnU16 mask_ = 0xffff  );

    /// ��(��������)�̍쐬
    Plane* createFloor( lnU16 group_ = 0xffff, lnU16 mask_ = 0xffff );

    /// ���b�V���̍쐬
    TriMesh* createStaticTriangleMesh( const LNTriangleMeshBodyInfo& info_, lnU16 group_ = 0xffff, lnU16 mask_ = 0xffff );

    /// ���b�V���̍쐬
    TriMesh* createStaticTriangleMesh( const LNTriangleMeshBodyInfo* info_, lnU16 info_count_, lnU16 group_ = 0xffff, lnU16 mask_ = 0xffff );

    /// ���C�L���X�g
    //bool rayTestSingle( const BodyBase* target_body_, const LVector3& from_, const LVector3& to_, LVector3* hit_pos_, LVector3* hit_normal_ );

    /// ���C�L���X�g
    bool rayCast( const LVector3& from_, const LVector3& to_, LVector3* hit_pos_, LVector3* hit_normal_, BodyBase** hit_body_, lnU16 group_ = 0xffff, lnU16 mask_ = 0xffff );

    /// �f�o�b�O�`��
    LNRESULT drawDebugShape( IDebugRenderer* renderer_ );

public:

    /// �������Z���[���h�̎擾
    btDiscreteDynamicsWorld* getDynamicsWorld() { return mBtWorld; }

    /// RigidBody �̃R���X�g���N�^����Ă΂�� (create �Ń��X�g�ɒǉ�����̂��������ǁARigidBody ���p�������V�������̂���肽���ꍇ�A���� Manager �� create�`�����Ȃ��čς�)
    void addRigidBody( RigidBody* body_ ) { mRigidBodyList.push_back( body_ ); }

    /// RigidBody �̃f�X�g���N�^����Ă΂��
    void deleteRigidBody( RigidBody* body_ );

    /// JointBase �� initialize() ����Ă΂��
    void addJoint( JointBase* joint_ ) { mJointList.push_back( joint_ ); }

    /// JointBase �̃f�X�g���N�^����Ă΂��
    void removeJoint( JointBase* joint_ ) { mJointList.push_back( joint_ ); }
    

protected:

    virtual ~Manager();

private:

    typedef std::vector< RigidBody* > RigidBodyList;
    typedef std::vector< JointBase* > JointList;

private:

    Base::LogFile*              mLogFile;

    btDefaultCollisionConfiguration*        mBtCollisionConfig;
	btCollisionDispatcher*                  mBtCollisionDispatcher;
    btDbvtBroadphase*					    mBtBroadphase;
	//btAxisSweep3*                         mBtOverlappingPairCache;
	btSequentialImpulseConstraintSolver*    mBtSolver;
	btDiscreteDynamicsWorld*                mBtWorld;

    //btSoftRigidDynamicsWorld*			            mBtWorld;
    //btSoftBodyWorldInfo							mBtSoftBodyWorldInfo;
	//btSoftBodyRigidBodyCollisionConfiguration*	mBtCollisionConfiguration;

    RigidBodyList           mRigidBodyList;
    JointList               mJointList;

	
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Physics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================