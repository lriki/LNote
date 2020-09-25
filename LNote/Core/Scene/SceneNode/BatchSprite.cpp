//=============================================================================
//【 BatchSprite 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Graphics/Common/SpriteRenderer.h"
#include "../../Graphics/Manager.h"
#include "../SceneGraph.h"
#include "Util.h"
#include "Proxy.h"
#include "BatchSprite.h"

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
// ■ BatchSprite3D
//=============================================================================

    //---------------------------------------------------------------------
    // Context
    //---------------------------------------------------------------------
    class BatchSprite3D::BatchSprite3DContext
        : public SceneNodeContext
    {
    public:

        BatchSprite3DContext()
            : mTexture      ( NULL )
            , mFlipFlags    ( LN_FLIP_NONE )
        {}

        virtual ~BatchSprite3DContext()
        {
            releaseAllResource();
        }

        virtual void updateContext( SceneNodeContext* parent_context_ )
        {
            SceneNodeContext::updateContext( parent_context_ );

            BatchSprite3D* t = (BatchSprite3D*)this->mSceneNode;
            mSize = t->mSize;
            mTexture = t->mTexture;
            LN_SAFE_ADDREF( mTexture );
            mSrcRect = t->mSrcRect;
            mFlipFlags = t->mFlipFlags;
        }

        virtual void releaseAllResource()
        {
            SceneNodeContext::releaseAllResource();
            LN_SAFE_RELEASE( mTexture );
        }

    private:

        LVector2            mSize;
        Graphics::ITexture* mTexture;
        Geometry::Rect		mSrcRect;
        lnU8                mFlipFlags;
    };

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    BatchSprite3D::BatchSprite3D( SceneGraph* scene_ )
        : SceneNode         ( scene_ )
        , mBatchSpriteProxy ( scene_->getBatchSpriteProxy( true ) )
        , mAADir            ( LN_AADIR_RZ )
        , mTexture          ( NULL )
        , mFlipFlags        ( LN_FLIP_NONE )
        , mCalledSetSrcRect ( false )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    BatchSprite3D::~BatchSprite3D()
    {
        LN_SAFE_RELEASE( mTexture );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void BatchSprite3D::initialize( lnfloat w_size_, lnfloat h_size_, LNAxisAlignedDir dir_ )
    {
        BatchSprite3DContext* c = LN_NEW BatchSprite3DContext();
        c->initialize( this, 1 );
        SceneNode::initialize( 1, LN_DRAWCOORD_3D, c );
        mAADir = dir_;
        mSize.set( w_size_, h_size_ );
        this->setShader( NULL );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void BatchSprite3D::setTexture( Graphics::ITexture* texture_ )
    {
        LN_SAFE_ADDREF( texture_ );
        LN_SAFE_RELEASE( mTexture );
        mTexture = texture_;
        
        if ( mTexture )
        {
            // 一度も setSrcRect() が呼ばれていなければ、テクスチャ全体を転送するようにする
            if ( !mCalledSetSrcRect )
            {
                mSrcRect.set(
                    0, 0,
                    static_cast< int >( mTexture->getSize().x ),
                    static_cast< int >( mTexture->getSize().y ) );
                mCalledSetSrcRect = true;
            }
        }
        else
        {
            mCalledSetSrcRect = false;
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void BatchSprite3D::setSrcRect( const Geometry::Rect& rect_ )
    {
        mSrcRect = rect_;
        mCalledSetSrcRect = true;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    LNRESULT BatchSprite3D::drawSubset( lnU32 index_ )
    {
        if ( mTexture )
        {
            mBatchSpriteProxy->getSpriteRenderer()->setTransform(
                this->mContext->getCombinedWorldMatrix() );

            const LColor& cs = this->mContext->getSubsetRenderParam( 0 ).ColorScale;
            LColor c(
                cs.red,
                cs.green,
                cs.blue,
                cs.alpha * this->getOpacity() );
            u32 color = c.to32Bit( mSceneGraph->getGraphicsManager()->getGraphicsAPI() );
            u32 color_table[ 4 ] = { color, color, color, color };
            
            return mBatchSpriteProxy->getSpriteRenderer()->drawRequest3D(
                LVector3( 0, 0, 0 ),//this->mPosition,
                LVector3( 0, 0, 0 ),//this->mCenter,
                mSize,
                mTexture,
                mSrcRect,
                color_table,
                mAADir );
        }
        return LN_OK;
    }

//=============================================================================
// ■ BatchSprite
//=============================================================================

    //---------------------------------------------------------------------
    // Context
    //---------------------------------------------------------------------
    class BatchSprite::BatchSpriteContext
        : public SceneNodeContext
    {
    public:

        BatchSpriteContext()
            : mTexture      ( NULL )
            , mFlipFlags    ( LN_FLIP_NONE )
        {}

        virtual ~BatchSpriteContext()
        {
            releaseAllResource();
        }

        virtual void updateContext( SceneNodeContext* parent_context_ )
        {
            SceneNodeContext::updateContext( parent_context_ );

            BatchSprite* t = (BatchSprite*)this->mSceneNode;
            mTexture = t->mTexture;
            LN_SAFE_ADDREF( mTexture );
            mSrcRect = t->mSrcRect;
            mFlipFlags = t->mFlipFlags;
        }

        virtual void releaseAllResource()
        {
            SceneNodeContext::releaseAllResource();
            LN_SAFE_RELEASE( mTexture );
        }

    private:

        Graphics::ITexture* mTexture;
        Geometry::Rect		mSrcRect;
        lnU8                mFlipFlags;
    };

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    BatchSprite::BatchSprite( SceneGraph* scene_ )
        : SceneNode         ( scene_ )
        , mBatchSpriteProxy ( scene_->getBatchSpriteProxy( false ) )
        , mTexture          ( NULL )
        , mFlipFlags        ( LN_FLIP_NONE )
        , mCalledSetSrcRect ( false )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    BatchSprite::~BatchSprite()
    {
        LN_SAFE_RELEASE( mTexture );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void BatchSprite::initialize()
    {
        BatchSpriteContext* c = LN_NEW BatchSpriteContext();
        c->initialize( this, 1 );
        SceneNode::initialize( 1, LN_DRAWCOORD_2D, c );
        this->setShader( NULL );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void BatchSprite::setTexture( Graphics::ITexture* texture_ )
    {
        LN_SAFE_ADDREF( texture_ );
        LN_SAFE_RELEASE( mTexture );
        mTexture = texture_;
        
        if ( mTexture )
        {
            // 一度も setSrcRect() が呼ばれていなければ、テクスチャ全体を転送するようにする
            if ( !mCalledSetSrcRect )
            {
                mSrcRect.set(
                    0, 0,
                    static_cast< int >( mTexture->getSize().x ),
                    static_cast< int >( mTexture->getSize().y ) );
                mCalledSetSrcRect = true;
            }
        }
        else
        {
            mCalledSetSrcRect = false;
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void BatchSprite::setSrcRect( const Geometry::Rect& rect_ )
    {
        mSrcRect = rect_;
        mCalledSetSrcRect = true;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    LNRESULT BatchSprite::drawSubset( lnU32 index_ )
    {
        if ( mTexture )
        {
            mBatchSpriteProxy->getSpriteRenderer()->setTransform( 
                this->mContext->getCombinedWorldMatrix() );

            /*
            const LColor& cs = this->getColorScale();
            LColor c(
                cs.red,
                cs.green,
                cs.blue,
                cs.alpha * this->getOpacity() );
            u32 color = c.to32Bit( mSceneGraph->getGraphicsManager()->getGraphicsAPI() );
            u32 color_table[ 4 ] = { color, color, color, color };
            */
            
            return mBatchSpriteProxy->getSpriteRenderer()->drawRequest2D(
                LVector3( 0, 0, 0 ),
                LVector3( 0, 0, 0 ),
                mTexture,
                mSrcRect,
                this->mContext->getSubsetRenderParam( 0 ).ColorScale.to32Bit( mSceneGraph->getGraphicsManager()->getGraphicsAPI() ) );
        }
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