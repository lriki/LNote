//=============================================================================
//【 Manager 】
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
// ■ Manager クラス
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

    /// 初期化データ
	struct InitData
	{
        Base::LogFile*      LogFile;
    };

public:

    Manager();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

    /// 終了処理
    void finalize();

    /// フレームレートの設定 (デフォルトは 60)
    void setFrameRate( int frame_rate_ );

    /// シミュレーション更新
    void update( lnfloat elapsed_time_ );

    /// 接触している物体をチェックしてイベントを発生させる
    void updateContactPoints();

    /// 重力の設定
    void setGravity( const LVector3& gravity_ );

    /// ボックスの作成
    Box* createBox( const LVector3& size_, const LVector3 pos_, lnfloat mass_, lnU16 group_ = 0xffff, lnU16 mask_ = 0xffff  );

    /// 床(無限平面)の作成
    Plane* createFloor( lnU16 group_ = 0xffff, lnU16 mask_ = 0xffff );

    /// メッシュの作成
    TriMesh* createStaticTriangleMesh( const LNTriangleMeshBodyInfo& info_, lnU16 group_ = 0xffff, lnU16 mask_ = 0xffff );

    /// メッシュの作成
    TriMesh* createStaticTriangleMesh( const LNTriangleMeshBodyInfo* info_, lnU16 info_count_, lnU16 group_ = 0xffff, lnU16 mask_ = 0xffff );

    /// レイキャスト
    //bool rayTestSingle( const BodyBase* target_body_, const LVector3& from_, const LVector3& to_, LVector3* hit_pos_, LVector3* hit_normal_ );

    /// レイキャスト
    bool rayCast( const LVector3& from_, const LVector3& to_, LVector3* hit_pos_, LVector3* hit_normal_, BodyBase** hit_body_, lnU16 group_ = 0xffff, lnU16 mask_ = 0xffff );

    /// デバッグ描画
    LNRESULT drawDebugShape( IDebugRenderer* renderer_ );

public:

    /// 物理演算ワールドの取得
    btDiscreteDynamicsWorld* getDynamicsWorld() { return mBtWorld; }

    /// RigidBody のコンストラクタから呼ばれる (create でリストに追加するのもいいけど、RigidBody を継承した新しい剛体を作りたい場合、この Manager に create～を作らなくて済む)
    void addRigidBody( RigidBody* body_ ) { mRigidBodyList.push_back( body_ ); }

    /// RigidBody のデストラクタから呼ばれる
    void deleteRigidBody( RigidBody* body_ );

    /// JointBase の initialize() から呼ばれる
    void addJoint( JointBase* joint_ ) { mJointList.push_back( joint_ ); }

    /// JointBase のデストラクタから呼ばれる
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