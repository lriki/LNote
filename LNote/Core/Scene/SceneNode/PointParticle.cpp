//=============================================================================
//y PointParticle z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Graphics/Common/PointParticle.h"
#include "../SceneGraph.h"
#include "Camera.h"
#include "PointParticle.h"

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
// ¡ PointParticle
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    PointParticle::PointParticle( SceneGraph* scene_ )
        : SceneNode         ( scene_ )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    PointParticle::~PointParticle()
    {
        LN_SAFE_RELEASE( mPointParticle );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void PointParticle::initialize( const Graphics::PointParticleParameter& param_ )
    {
        SceneNode::initialize( 1, LN_DRAWCOORD_3D );

        mPointParticle = LN_NEW Graphics::PointParticle();
        mPointParticle->initialize(
            this->mSceneGraph->getGraphicsManager()->getGraphicsDevice(),
            param_ );

        this->setShader( NULL );

        this->mNodeRenderParam.RenderState.PointSprite = true;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void PointParticle::setTexture( Graphics::ITexture* texture_ )
    {
        mPointParticle->setTexture( texture_ );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    //Graphics::ITexture* PointParticle::getTexture()
    //{
    //    return 
    //}

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void PointParticle::update( const Game::GameTime& time_ )
    {
        mPointParticle->advanceTime( time_.getElapsedGameTime() );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    LNRESULT PointParticle::drawSubset( lnU32 index_ )
    {
        mPointParticle->update( this->mContext->getCombinedWorldMatrix() );

        
        this->mSceneGraph->getGraphicsManager()->getGeometryRenderer()->drawPointParticle(
            this->mContext->getCombinedWorldMatrix(),
            this->mContext->getCameraContext()->getProjectionMatrix(),
            this->mContext->getCameraContext()->getViewProjectionMatrix(),
            this->mContext->getCameraContext()->getPosition(),
            mPointParticle );
        
        return LN_OK;
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================