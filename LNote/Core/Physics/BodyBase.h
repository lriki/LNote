//=============================================================================
//�y BodyBase �z
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
// �� BodyBase �N���X
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

    /// ������
    void initialize( btCollisionShape* shape_ );

    /// �V�~�����[�V�����X�V���O�̏���
    virtual void preUpdate() {}

    /// �V�~�����[�V�����X�V����̏���
    virtual void postUpdate() {}

    /// �f�o�b�O�p�̌`��`��
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