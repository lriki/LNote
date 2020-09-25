//=============================================================================
//【 Manager 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <algorithm>
#include "../Base/STLUtil.h"
#include "RigidBody/RigidBody.h"
#include "RigidBody/Plane.h"
#include "RigidBody/Box.h"
#include "RigidBody/TriangleMesh.h"
#include "Manager.h"

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
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    Manager::Manager()
        : mLogFile                  ( NULL )
        , mBtCollisionConfig        ( NULL )
        , mBtCollisionDispatcher    ( NULL )
        , mBtBroadphase             ( NULL )
        , mBtSolver                 ( NULL )
        , mBtWorld                  ( NULL )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    Manager::~Manager()
    {
        SAFE_DELETE( mBtCollisionConfig );
        SAFE_DELETE( mBtCollisionDispatcher );
        SAFE_DELETE( mBtBroadphase );
        SAFE_DELETE( mBtSolver );
        SAFE_DELETE( mBtWorld );
    }

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    LNRESULT Manager::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        // コリジョンコンフィグ
        mBtCollisionConfig = LN_NEW btDefaultCollisionConfiguration();

        // コリジョンディスパッチャ
	    mBtCollisionDispatcher = LN_NEW btCollisionDispatcher( mBtCollisionConfig );

        // ブロードフェーズアルゴリズム
        /*
            btDbvtBroadphase
                AABB木による高速で動的な階層空間を使います。
            btAxisSweep3 bt32BitAxisSweep3
                逐次3Dスイープ&プルーン(incremental 3D sweep and prune)を実装しています。
            btCudaBroadphase
                GPUを使用した高速な一様グリッドを実装しています。
            http://bulletjpn.web.fc2.com/05_BulletCollisionDetection.html
        */
        //btVector3	btv3WorldAabbMin( -3000.0f, -3000.0f, -3000.0f );
	    //btVector3	btv3WorldAabbMax(  3000.0f,  3000.0f,  3000.0f );
	    //int			iMaxProxies = 1024;
	    //mBtOverlappingPairCache = new btAxisSweep3( btv3WorldAabbMin, btv3WorldAabbMax, iMaxProxies );
        mBtBroadphase = LN_NEW btDbvtBroadphase();

        // ソルバ
	    mBtSolver = LN_NEW btSequentialImpulseConstraintSolver();

        // ワールドの作成
	    //mBtWorld = LN_NEW btSoftRigidDynamicsWorld( m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration );
        mBtWorld = LN_NEW btDiscreteDynamicsWorld( mBtCollisionDispatcher, mBtBroadphase, mBtSolver, mBtCollisionConfig );

        // 重力
        setGravity( LVector3( 0.0f, -9.8f, 0.0f ) );

        LN_LOG_INIT_END;
        return LN_OK;
    }


    //---------------------------------------------------------------------
	// ● 終了処理
    //---------------------------------------------------------------------
    void Manager::finalize()
    {
        LN_LOG_FIN_BEGIN;

      
        LN_LOG_FIN_END;
    }

    //---------------------------------------------------------------------
	// ● ミュレーション更新
    //---------------------------------------------------------------------
    void Manager::update( lnfloat elapsed_time_ )
    {
        // 更新
        mBtWorld->stepSimulation( elapsed_time_, 1 );//, 1.0f / 30.0f );//, elapsed_time_ / 30.0f );

        // 後処理
        RigidBodyList::iterator itr = mRigidBodyList.begin();
        RigidBodyList::iterator end = mRigidBodyList.end();
        for ( ; itr != end; ++itr )
        {
            (*itr)->postUpdate();
        }
    }

    //---------------------------------------------------------------------
	// ● 接触している物体をチェックしてイベントを発生させる
    //---------------------------------------------------------------------
    void Manager::updateContactPoints()
    {
        /*
        m_ContactPointList.clear();
#endif

	Sint32 ManifoldCount = m_pDispatcher->getNumManifolds();
	for ( Sint32 i = 0; i < ManifoldCount; i++ )
	{
		btPersistentManifold* pManifold = m_pDispatcher->getManifoldByIndexInternal( i );
		btCollisionObject* pObjA = static_cast<btCollisionObject*>(pManifold->getBody0());
		btCollisionObject* pObjB = static_cast<btCollisionObject*>(pManifold->getBody1());

		Body::SContactInfo Info;
		float Min = FLT_MAX;
		Sint32 ContactCount = pManifold->getNumContacts();
		for ( Sint32 j = 0; j < ContactCount; j++ )
		{
			btManifoldPoint& Point = pManifold->getContactPoint( j );
			float Distance = Point.getDistance();
			if ( Distance < 0.0f )
			{
				if ( Distance < Min )
				{
					const btVector3& PointB = Point.m_positionWorldOnB;
					const btVector3& NormalB = Point.m_normalWorldOnB;

					Min = Distance;

					Info.Position.x = PointB.getX();
					Info.Position.y = PointB.getY();
					Info.Position.z = PointB.getZ();
					Info.Normal.x = NormalB.getX();
					Info.Normal.y = NormalB.getY();
					Info.Normal.z = NormalB.getZ();

#if defined(SLN_DEBUG)
					SContactPoint Contact;
					Contact.Position.x = PointB.getX();
					Contact.Position.y = PointB.getY();
					Contact.Position.z = PointB.getZ();
					Contact.Normal.x = NormalB.getX();
					Contact.Normal.y = NormalB.getY();
					Contact.Normal.z = NormalB.getZ();
					m_ContactPointList.push_back( Contact );
#endif
				}
			}
		}

		if ( Min < 0.0f )
		{
			Body::RigidBodyBase* pBodyA = static_cast<Body::RigidBodyBase*>(pObjA->getUserPointer());
			Body::RigidBodyBase* pBodyB = static_cast<Body::RigidBodyBase*>(pObjB->getUserPointer());

			Info.pTarget = pBodyB->GetIBody();
			pBodyA->AddContact( Info );
			Info.pTarget = pBodyA->GetIBody();
			pBodyB->AddContact( Info );
		}
	}

    */
    }

    //---------------------------------------------------------------------
	// ● 重力の設定
    //---------------------------------------------------------------------
    void Manager::setGravity( const LVector3& gravity_ )
    {
        mBtWorld->setGravity( btVector3( gravity_.x, gravity_.y, gravity_.z ) );
    }

    //---------------------------------------------------------------------
	// ● ボックスの作成
    //---------------------------------------------------------------------
    Box* Manager::createBox( const LVector3& size_, const LVector3 pos_, lnfloat mass_, lnU16 group_, lnU16 mask_ )
    {
        btCollisionShape* box_shape = new btBoxShape( btVector3( size_.x * 0.5f, size_.y * 0.5f, size_.z * 0.5f ) );
	    btVector3 local_inertia( 0.0f, 0.0f, 0.0f );
	    btTransform transform;

	    // トランスフォームを設定
	    transform.setIdentity();
	    transform.setOrigin( btVector3( pos_.x, pos_.y, pos_.z ) );

	    // 慣性テンソルを計算する
	    if ( mass_ > FLT_MIN )
	    {
		    box_shape->calculateLocalInertia( mass_, local_inertia );
	    }
	    else
	    {
		    mass_ = 0.0f;
	    }

	    // 剛体を作成する
	    btDefaultMotionState* motion_state = LN_NEW btDefaultMotionState( transform );
	    btRigidBody::btRigidBodyConstructionInfo body_info( mass_, motion_state, box_shape, local_inertia );

	    btRigidBody* rigid_body = new btRigidBody( body_info );
	    rigid_body->setCollisionFlags( rigid_body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );

	    // Box 作成
        RigidBodyInitArgs args;
        args.Shape      = box_shape;
        args.RigidBody  = rigid_body;
        args.Group      = group_;
        args.Mask       = mask_;
	    Box* box = LN_NEW Box( this );
        box->initialize( args, size_ );
	    rigid_body->setUserPointer( box );

        // ワールドに追加
        //mBtWorld->addRigidBody( rigid_body, group_, mask_ );

	    return box;
    }

    //---------------------------------------------------------------------
	// ● 床(無限平面)の作成
    //---------------------------------------------------------------------
    Plane* Manager::createFloor( lnU16 group_, lnU16 mask_ )
    {
	    btCollisionShape* bt_shape = LN_NEW btStaticPlaneShape( btVector3( 0.0f, 1.0f, 0.0f ), 0.0f );

	    // トランスフォームを設定
	    btTransform transform;
	    transform.setIdentity();

	    // 剛体を作成する
	    btVector3 local_inertia( 0.0f, 0.0f, 0.0f );

	    btDefaultMotionState* motion_state = LN_NEW btDefaultMotionState( transform );
	    btRigidBody::btRigidBodyConstructionInfo body_info( 0.0, motion_state, bt_shape, local_inertia );

	    btRigidBody* bt_rigid_body = new btRigidBody( body_info );
	    bt_rigid_body->setCollisionFlags( bt_rigid_body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );

	    // Plane 作成
        RigidBodyInitArgs args;
        args.Shape      = bt_shape;
        args.RigidBody  = bt_rigid_body;
        args.Group      = group_;
        args.Mask       = mask_;
	    Plane* plane = LN_NEW Plane( this );
        plane->initialize( args );
	    bt_rigid_body->setUserPointer( plane );

        // ワールドに追加
        //mBtWorld->addRigidBody( bt_rigid_body, group_, mask_ );

	    return plane;
    }

    //-----------------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------------
    TriMesh* Manager::createStaticTriangleMesh( const LNTriangleMeshBodyInfo& info_, lnU16 group_, lnU16 mask_ )
    {
	    return createStaticTriangleMesh( &info_, 1, group_, mask_ );
    }

    //-----------------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------------
    TriMesh* Manager::createStaticTriangleMesh( const LNTriangleMeshBodyInfo* info_, lnU16 info_count_, lnU16 group_, lnU16 mask_ )
    {
	    btTriangleIndexVertexArray* bt_tri_mesh_data = new btTriangleIndexVertexArray();

	    for ( lnU32 i = 0; i < info_count_; ++i )
	    {
		    btIndexedMesh bt_mesh;
		    bt_mesh.m_numTriangles        = info_[i].IndexCount / 3;
		    bt_mesh.m_triangleIndexBase   = reinterpret_cast<const lnU8*>(info_[i].IndexTable);
		    bt_mesh.m_triangleIndexStride = info_[i].IndexStride * 3;
		    bt_mesh.m_numVertices         = info_[i].VertexCount;
		    bt_mesh.m_vertexBase          = reinterpret_cast<const lnU8*>(info_[i].VertexTable);
		    bt_mesh.m_vertexStride        = sizeof(LVector3);
            bt_mesh.m_indexType           = (info_[i].IndexStride == 4) ? PHY_INTEGER : PHY_SHORT;
		    if ( (bt_mesh.m_numTriangles >= 1) && (bt_mesh.m_numVertices >= 3) )
		    {
			    bt_tri_mesh_data->addIndexedMesh( bt_mesh, bt_mesh.m_indexType );
		    }
	    }

	    btBvhTriangleMeshShape* bt_tri_mesh_shape =
            new btBvhTriangleMeshShape( bt_tri_mesh_data, true, true );

	    btVector3 local_inertia( 0.0f, 0.0f, 0.0f );

	    // 剛体を作成する
	    btDefaultMotionState* motion_state = LN_NEW btDefaultMotionState();
	    btRigidBody::btRigidBodyConstructionInfo body_info( 0.0f, motion_state, bt_tri_mesh_shape, local_inertia );
	    btRigidBody* bt_rigid_body = new btRigidBody( body_info );
	    bt_rigid_body->setCollisionFlags( bt_rigid_body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );

        // TriMesh
        RigidBodyInitArgs args;
        args.Shape      = bt_tri_mesh_shape;
        args.RigidBody  = bt_rigid_body;
        args.Group      = group_;
        args.Mask       = mask_;
	    TriMesh* triangle_mesh = LN_NEW TriMesh( this );
        triangle_mesh->initialize( args, bt_tri_mesh_data );
	    bt_rigid_body->setUserPointer( triangle_mesh );

	    return triangle_mesh;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    //bool Manager::rayTestSingle( const BodyBase* target_body_, const LVector3& from_, const LVector3& to_, LVector3* hit_pos_, LVector3* hit_normal_ )
    //{
    //    mBtWorld->
    //    rayTest
    //    btRayAabb
    //}

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    bool Manager::rayCast( const LVector3& from_, const LVector3& to_, LVector3* hit_pos_, LVector3* hit_normal_, BodyBase** hit_body_, lnU16 group_, lnU16 mask_ )
    {
        btVector3 from( from_.x, from_.y, from_.z );
	    btVector3 to( to_.x, to_.y, to_.z );

	    btCollisionWorld::ClosestRayResultCallback result_callback( from, to );
	    result_callback.m_collisionFilterGroup = group_;
	    result_callback.m_collisionFilterMask  = mask_;

	    mBtWorld->rayTest( from, to, result_callback );

	    if ( result_callback.hasHit() )
	    {
            if ( hit_body_ )
            {
		        *hit_body_ = static_cast<BodyBase*>(result_callback.m_collisionObject->getUserPointer());
            }

            if ( hit_pos_ )
            {
		        hit_pos_->x = result_callback.m_hitPointWorld.getX();
		        hit_pos_->y = result_callback.m_hitPointWorld.getY();
		        hit_pos_->z = result_callback.m_hitPointWorld.getZ();
            }

            if ( hit_normal_ )
            {
		        hit_normal_->x = result_callback.m_hitNormalWorld.getX();
		        hit_normal_->y = result_callback.m_hitNormalWorld.getY();
		        hit_normal_->z = result_callback.m_hitNormalWorld.getZ();
            }
		    return true;
	    }

	    return false;
    }

    //---------------------------------------------------------------------
	// ● デバッグ描画
    //---------------------------------------------------------------------
    LNRESULT Manager::drawDebugShape( IDebugRenderer* renderer_ )
    {
        LNRESULT lr;

        RigidBodyList::iterator itr = mRigidBodyList.begin();
        RigidBodyList::iterator end = mRigidBodyList.end();
        for ( ; itr != end; ++itr )
        {
            LN_CALL_R( (*itr)->drawDebugShape( renderer_ ) );
        }
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● RigidBody のデストラクタから呼ばれる
    //---------------------------------------------------------------------
    void Manager::deleteRigidBody( RigidBody* body_ )
    {
        Base::STLUtil::remove( mRigidBodyList, body_ );

        btRigidBody* bt_rb = body_->getBtRigidBody();

	    btMotionState* pState = bt_rb->getMotionState();
	    SAFE_DELETE( pState );

	    btCollisionShape* pShape = bt_rb->getCollisionShape();
	    SAFE_DELETE( pShape );

	    mBtWorld->removeCollisionObject( bt_rb );
	    SAFE_DELETE( bt_rb );
    }

    

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Physics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================