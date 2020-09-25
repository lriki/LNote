//=============================================================================
//【 PointParticle 】
//-----------------------------------------------------------------------------
///**
//  @file       PointParticle.h
//  @brief      PointParticle
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "SceneNode.h"
#include "../../Graphics/Common/PointParticleParameter.h"

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
// ■ PointParticle
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class PointParticle
    : public SceneNode
{
public:

    PointParticle( SceneGraph* scene_ );

    virtual ~PointParticle();

public:

    /// 初期化
    void initialize( const Graphics::PointParticleParameter& param_ );

    /// テクスチャの設定
    void setTexture( Graphics::ITexture* texture_ );

    /// テクスチャの取得
    //Graphics::ITexture* getTexture();

public:

    /// ノードの種類の取得
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_PARTICLE; }

    /// フレーム更新
    virtual void update( const Game::GameTime& time_ );

    /// サブセットを描画する
    virtual LNRESULT drawSubset( lnU32 index_ );

private:
    
    Graphics::PointParticle*    mPointParticle;
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