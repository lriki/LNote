//=============================================================================
//【 ModelConstraint 】
//-----------------------------------------------------------------------------
///**
//  @file       ModelConstraint.h
//  @brief      ModelConstraint
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../Base/RefString.h"
#include "../../../Physics/Joint/JointBase.h"
#include "PMDTypes.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
class ModelRigidBody;

//=============================================================================
// ■ ModelConstraint クラス
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class ModelConstraint
    : public Physics::JointBase
{
public:

    ModelConstraint();

    virtual ~ModelConstraint();

public:

    /// 初期化
    void initializePMD( ModelManager* manager_, const PMD_Constraint* pmd_constraint_, ModelRigidBody* body_a_, ModelRigidBody* body_b_ );

private:

    btGeneric6DofSpringConstraint*  mBtDofConstraint;   ///< (Physics::JointBase に管理されるので delete の必要はナシ)
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