//=============================================================================
//�y RigidBody �z
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
    btCollisionShape*   Shape;      ///< (BodyBase  �폜���� delete �����)
    btRigidBody*        RigidBody;  ///< (RigidBody �폜���� delete �����)
    lnU16               Group;
    lnU16               Mask;
};

//=============================================================================
// �� RigidBody �N���X
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class RigidBody
    : public BodyBase
{
public:
    RigidBody() {};    // �z�� new �̂��߂Ɏd���Ȃ��c
    RigidBody( Manager* manager_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~RigidBody();

public:

    /// ������ (���̂��󂯎���ă��[���h�ɒǉ�����)
    void initialize( const RigidBodyInitArgs& args_ );

    btRigidBody* getBtRigidBody() { return mBtRigidBody; }

    /// ���˗��̐ݒ�
    void setRestitution( float value_ );

    /// ���˗��̎擾
    float getRestitution() const;

    /// ���C�W���̐ݒ�
    void setFriction( float value_ );

    /// ���C�W���̎擾
    float getFriction() const;

    /// �V�~�����[�V�����X�V���O�̏���
    virtual void preUpdate();

    /// �V�~�����[�V�����X�V����̏���
    virtual void postUpdate();

protected:

    btRigidBody*    mBtRigidBody;
    LMatrix         mWorldMatrix;   ///< (postUpdate() �Őݒ肳���)
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