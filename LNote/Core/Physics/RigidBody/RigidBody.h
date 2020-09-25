//=============================================================================
//【 RigidBody 】
//-----------------------------------------------------------------------------
///**
//  @file       RigidBody.h
//  @brief      RigidBody
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../BodyBase.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Physics
{

struct RigidBodyInitArgs
{
    btCollisionShape*   Shape;      ///< (BodyBase  削除時に delete される)
    btRigidBody*        RigidBody;  ///< (RigidBody 削除時に delete される)
    lnU16               Group;
    lnU16               Mask;
};

//=============================================================================
// ■ RigidBody クラス
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class RigidBody
    : public BodyBase
{
public:
    RigidBody() {};    // 配列 new のために仕方なく…
    RigidBody( Manager* manager_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~RigidBody();

public:

    /// 初期化 (剛体を受け取ってワールドに追加する)
    void initialize( const RigidBodyInitArgs& args_ );

    btRigidBody* getBtRigidBody() { return mBtRigidBody; }

    /// 反射率の設定
    void setRestitution( float value_ );

    /// 反射率の取得
    float getRestitution() const;

    /// 摩擦係数の設定
    void setFriction( float value_ );

    /// 摩擦係数の取得
    float getFriction() const;

    /// シミュレーション更新直前の処理
    virtual void preUpdate();

    /// シミュレーション更新直後の処理
    virtual void postUpdate();

protected:

    btRigidBody*    mBtRigidBody;
    LMatrix         mWorldMatrix;   ///< (postUpdate() で設定される)
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