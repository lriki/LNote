//=============================================================================
//y Grid z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Graphics/Manager.h"
#include "../../Effect/Manager.h"
#include "../SceneGraph.h"
#include "Camera.h"
#include "Proxy.h"

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
// ¡ EffectBatchProxy ƒNƒ‰ƒX
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    EffectBatchProxy::EffectBatchProxy( SceneGraph* scene_ )
        : SceneNode         ( scene_ )
        , mEffectManager    ( NULL )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    EffectBatchProxy::~EffectBatchProxy()
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void EffectBatchProxy::initialize( Core::Effect::Manager* effect_manager_, LNDrawCoordSpace cs_ )
    {
        mEffectManager = effect_manager_;
        SceneNode::initialize( 0, cs_ );

        mSubsetNum = 1;
        this->setShader( NULL );
        this->setPriority( LN_SCENENODEPRIORITY_EFFECT_BATCH_SPRITE );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    LNRESULT EffectBatchProxy::drawSubset( lnU32 index_ )
    {
        if ( this->getDrawCoordSpace() == LN_DRAWCOORD_3D )
        {
            mEffectManager->drawBatch3D();
        }
        else
        {
            mEffectManager->drawBatch3D();
        }

        return LN_OK;
    }

//=============================================================================
// ¡ BatchSpriteProxy
//=============================================================================

    //---------------------------------------------------------------------
    // Context
    //---------------------------------------------------------------------
    class BatchSpriteProxyContext
        : public SceneNodeContext
    {
    public:

        void initialize( BatchSpriteProxy* node_, lnU32 subset_num_ )
        {
            mBatchSpriteProxy = node_;
            SceneNodeContext::initialize( node_, subset_num_ );
        }

        virtual void OnSetActivePaneContext( SceneGraphContext* scene_context_, PaneContext* pane_context_, CameraContext* camera_context_ )
        {
            mBatchSpriteProxy->getSpriteRenderer()->setViewProjMatrix(
                camera_context_->getViewMatrix(),
                camera_context_->getProjectionMatrix() );
        }

    private:

        BatchSpriteProxy*   mBatchSpriteProxy;
    };

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    BatchSpriteProxy::BatchSpriteProxy( SceneGraph* scene_ )
        : SceneNode         ( scene_ )
        , mSpriteRenderer   ( NULL )
    {
        
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    BatchSpriteProxy::~BatchSpriteProxy()
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void BatchSpriteProxy::initialize( bool is_3d_ )
    {
        BatchSpriteProxyContext* context = LN_NEW BatchSpriteProxyContext();
        context->initialize( this, 0 );

        SceneNode::initialize( 0, (is_3d_) ? LN_DRAWCOORD_3D : LN_DRAWCOORD_2D , context );

        mSubsetNum = 1;
        this->setShader( NULL );
        this->setPriority( LN_SCENENODEPRIORITY_PROXY );

        if ( is_3d_ )
        {
            mSpriteRenderer = mSceneGraph->getGraphicsManager()->getSprite3DRenderer();
        }
        else
        {
            mSpriteRenderer = mSceneGraph->getGraphicsManager()->getSprite2DRenderer();
        }

        mSpriteRenderer->setSortMode( LN_SPRITESORT_DEPTH_BACK_TO_FRONT, is_3d_ );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    LNRESULT BatchSpriteProxy::drawSubset( lnU32 index_ )
    {
        mSpriteRenderer->flash();
        return LN_OK;
    }

//=============================================================================
// ¡ PhysicsDebugDrawProxy
//=============================================================================

    //---------------------------------------------------------------------
    // Context
    //---------------------------------------------------------------------
    class PhysicsDebugDrawProxy::DrawingContext
        : public SceneNodeContext
    {
    public:

        virtual void OnSetActivePaneContext( SceneGraphContext* scene_context_, PaneContext* pane_context_, CameraContext* camera_context_ )
        {
            SceneNodeContext::OnSetActivePaneContext( scene_context_, pane_context_, camera_context_ );

            ((PhysicsDebugDrawProxy*)this->mSceneNode)->mGeometryRenderer->setViewProjMatrix(
                camera_context_->getViewProjectionMatrix() );
        }
    };

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    PhysicsDebugDrawProxy::PhysicsDebugDrawProxy( SceneGraph* scene_ )
        : SceneNode         ( scene_ )
        , mGeometryRenderer ( NULL )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void PhysicsDebugDrawProxy::initialize()
    {
        mGeometryRenderer = this->mSceneGraph->getGeometryRenderer();

        DrawingContext* context = LN_NEW DrawingContext();
        context->initialize( this, 1 );
        SceneNode::initialize( 1, LN_DRAWCOORD_3D, context );

        this->setShader( NULL );
        //this->setShader( this->mSceneGraph->getDefaultShader( LN_DEFSHADER_NOLIGHTING ) );
        this->setPriority( LN_SCENENODEPRIORITY_PHYSICS_DEBUG );

        this->setEnableDepthTest( true );
        this->setEnableDepthWrite( false );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    LNRESULT PhysicsDebugDrawProxy::drawSubset( lnU32 index_ )
    {
        mGeometryRenderer->setMatrix( LMatrix::IDENTITY );
        mGeometryRenderer->begin();
        this->mSceneGraph->getPhysicsManager()->drawDebugShape( mGeometryRenderer );
        mGeometryRenderer->end();

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