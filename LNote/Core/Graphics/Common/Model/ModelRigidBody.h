//=============================================================================
//【 ModelRigidBody 】
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
    COLSHAPE_SPHERE = 0,    ///< 球
    COLSHAPE_BOX,           ///< 箱
    COLSHAPE_CAPSULE,       ///< カプセル
};

enum RigidBodyType
{
    RIGIDBODYTYPE_CONTROLLED_BY_BONE = 0,   ///< Bone追従
    RIGIDBODYTYPE_PHYSICS,                  ///< 物理演算
    RIGIDBODYTYPE_PHYSICS_ALIGNMENT,        ///< 物理演算(Bone位置合せ)
}; 

//=============================================================================
// ■ ModelRigidBody クラス
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class ModelRigidBody
    : public Physics::RigidBody
{
public:

    ModelRigidBody() {}   // 苦肉のデフォルトコンストラクタ。new 直後に配置 new で初期化すること。
    ModelRigidBody( ModelManager* manager_ );

    virtual ~ModelRigidBody();

public:

    /// 初期化
    void initializePMD( const PMD_RigidBody *pmd_rigidbody_, ModelFrame* frame_ );


    /// ボーン位置合わせ (RIGIDBODYTYPE_PHYSICS_ALIGNMENT のときのみ有効。ボーンの位置に剛体を移動する)
    void fixPosition( float fElapsedFrame );
		
    /// ボーンの姿勢を剛体の姿勢と一致させる(そのフレームのシミュレーション終了後に呼ぶ)
    void updateBoneTransform();
		
    /// 剛体をボーンの位置へ強制的に移動させる
    void moveToBonePos();


    /// デバッグ用の形状描画
    virtual LNRESULT drawDebugShape( Physics::IDebugRenderer* renderer_ );

    

private:

    /// 解放
    void _release();

private:

    /// 形状データ
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

    lnU16	            mColGroup;          // 衝突グループ
	lnU16	            mColGroupMask;	    // 衝突グループマスク
    LVector3	        mInitialPosition;	// 位置(ボーン相対)
	LVector3	        mInitialRotation;	// 回転(radian)
    lnfloat             mMass;			    // 質量
	lnfloat			    mLinearDamping;	    // 移動減
	lnfloat			    mAngularDamping;    // 回転減
	lnfloat			    mRestitution;	    // 反発力
	lnfloat			    mFriction;		    // 摩擦力

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