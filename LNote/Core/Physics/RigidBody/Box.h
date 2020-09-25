//=============================================================================
//�y Box �z
//-----------------------------------------------------------------------------
///**
//  @file       Box.h
//  @brief      Box
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "RigidBody.h"

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
// �� Box �N���X
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class Box
    : public RigidBody
{
public:
    Box( Manager* manager_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~Box();

public:

    /// ������
    void initialize( const RigidBodyInitArgs& args_, const LVector3& size_ );

    /// �f�o�b�O�p�̌`��`��
    virtual LNRESULT drawDebugShape( IDebugRenderer* renderer_ );

private:

    LVector3    mSize;
};


//=============================================================================
// �� TriMesh
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class TriMesh
    : public RigidBody
{
public:
    TriMesh( Manager* manager_ );

protected:
    virtual ~TriMesh();

public:

    void initialize( const RigidBodyInitArgs& args_, btTriangleIndexVertexArray* bt_tri_mesh_data_ );

    //virtual LNRESULT drawDebugShape( IDebugRenderer* renderer_ );

private:

    btTriangleIndexVertexArray* mBtTriMeshData;
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