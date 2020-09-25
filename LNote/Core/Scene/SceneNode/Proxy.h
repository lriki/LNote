//=============================================================================
//�y Proxy �z
//-----------------------------------------------------------------------------
///**
//  @file       Proxy.h
//  @brief      Proxy
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Common/Grid.h"
#include "SceneNode.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

//=============================================================================
// �� EffectBatchProxy �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�G�t�F�N�g�̃X�v���C�g���A�o�b�`�`����s���N���X
//*/
//=============================================================================
class EffectBatchProxy
    : public SceneNode
{
public:

    EffectBatchProxy( SceneGraph* scene_ );
    virtual ~EffectBatchProxy();

public:

    /// ������
    void initialize( Core::Effect::Manager* effect_manager_, LNDrawCoordSpace cs_ );

public:

    /// �m�[�h�̎�ނ̎擾
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_EFFECTSPRITEPROXY; }

    /// �T�u�Z�b�g��`�悷��
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    Core::Effect::Manager*    mEffectManager;
};

//=============================================================================
// �� BatchSpriteProxy
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class BatchSpriteProxy
    : public SceneNode
{
public:

    BatchSpriteProxy( SceneGraph* scene_ );
    virtual ~BatchSpriteProxy();

public:

    /// ������
    void initialize( bool is_3d_ );

    Graphics::SpriteRenderer* getSpriteRenderer() { return mSpriteRenderer; }

public:

    /// �m�[�h�̎�ނ̎擾
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_EFFECTSPRITEPROXY; }

    /// �T�u�Z�b�g��`�悷��
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    Graphics::SpriteRenderer*   mSpriteRenderer;
};

//=============================================================================
// �� PhysicsDebugDrawProxy
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class PhysicsDebugDrawProxy
    : public SceneNode
{
public:

    PhysicsDebugDrawProxy( SceneGraph* scene_ );

    virtual ~PhysicsDebugDrawProxy() {}

public:

    void initialize();

public:

    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_PHYSICSDEBUGDRAWPROXY; }

    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    Graphics::GeometryRenderer* mGeometryRenderer;

    class DrawingContext;
    friend class DrawingContext;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================