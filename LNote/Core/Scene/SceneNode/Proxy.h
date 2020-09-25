//=============================================================================
//【 Proxy 】
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
// ■ EffectBatchProxy クラス
//-----------------------------------------------------------------------------
///**
//  @brief		エフェクトのスプライト等、バッチ描画を行うクラス
//*/
//=============================================================================
class EffectBatchProxy
    : public SceneNode
{
public:

    EffectBatchProxy( SceneGraph* scene_ );
    virtual ~EffectBatchProxy();

public:

    /// 初期化
    void initialize( Core::Effect::Manager* effect_manager_, LNDrawCoordSpace cs_ );

public:

    /// ノードの種類の取得
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_EFFECTSPRITEPROXY; }

    /// サブセットを描画する
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    Core::Effect::Manager*    mEffectManager;
};

//=============================================================================
// ■ BatchSpriteProxy
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

    /// 初期化
    void initialize( bool is_3d_ );

    Graphics::SpriteRenderer* getSpriteRenderer() { return mSpriteRenderer; }

public:

    /// ノードの種類の取得
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_EFFECTSPRITEPROXY; }

    /// サブセットを描画する
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    Graphics::SpriteRenderer*   mSpriteRenderer;
};

//=============================================================================
// ■ PhysicsDebugDrawProxy
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