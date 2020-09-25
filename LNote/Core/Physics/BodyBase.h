//=============================================================================
//【 BodyBase 】
//-----------------------------------------------------------------------------
///**
//  @file       BodyBase.h
//  @brief      BodyBase
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Geometry/Matrix.h"
#include "IDebugRenderer.h"
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
// ■ BodyBase クラス
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class BodyBase
    : public Base::ReferenceObject
{
public:
    BodyBase() {}
    BodyBase( Manager* manager_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~BodyBase();

public:

    /// 初期化
    void initialize( btCollisionShape* shape_ );

    /// シミュレーション更新直前の処理
    virtual void preUpdate() {}

    /// シミュレーション更新直後の処理
    virtual void postUpdate() {}

    /// デバッグ用の形状描画
    virtual LNRESULT drawDebugShape( IDebugRenderer* renderer_ ) { return LN_OK; }

protected:

    Manager*				    mManager;
	btDiscreteDynamicsWorld*    mDynamicsWorld;
	btCollisionShape*		    mCollisionShape;
	void*				        mUserData;
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