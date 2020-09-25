//=============================================================================
//y Pane z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Base/STLUtil.h"
#include "../../Resource/LNResource.h"
#include "../SceneGraph.h"
#include "../SceneShader/SceneShader.h"
#include "Camera.h"
#include "Pane.h"

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
// ¡ Pane ƒNƒ‰ƒX
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    Pane::Pane( SceneGraph* scene_ )
        : mSceneGraph   ( scene_ )
        , mCamera3D     ( NULL )
        , mCamera2D     ( NULL )
        , mRootNode3D   ( NULL )
        , mRootNode2D   ( NULL )
        , mGUIDesktop   ( NULL )
        , mPaneContext  ( NULL )
		, mPanePostDrawContext	( NULL )
        , mEffectMode               ( LN_SCREENEFFECTMODE_NONE )
        , mScreenEffect             ( NULL )
		, mBackgroundColor			( 1.0f, 1.0f, 1.0f, 1.0f )
    {
        mSceneGraph->addPane( this );
        LN_SAFE_ADDREF( mSceneGraph );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    Pane::~Pane()
    {
        _disposeEffectResource();

        ln_foreach( Pane* pane, mChildPaneArray )
        {
            LN_SAFE_RELEASE( pane );
        }
        mChildPaneArray.clear();

        LN_SAFE_RELEASE( mCamera3D );
        LN_SAFE_RELEASE( mCamera2D );
        LN_SAFE_RELEASE( mRootNode3D );
        LN_SAFE_RELEASE( mRootNode2D );
        LN_SAFE_RELEASE( mGUIDesktop );
        SAFE_DELETE( mPaneContext );
        if ( mSceneGraph )
        {
            mSceneGraph->removePane( this );
        }
        LN_SAFE_RELEASE( mSceneGraph );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Pane::initialize( const Geometry::Size& size_ )
    {
        mViewSize.set( (lnfloat)size_.w, (lnfloat)size_.h );

        mPaneContext = LN_NEW PaneContext();
        mPaneContext->initialize( this );
    }

	//---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
	void Pane::setViewSize( const Geometry::Size& size )
	{
		mViewSize.set( (lnfloat)size.w, (lnfloat)size.h );

        if ( mEffectMode == LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
        {
			_disposeEffectResource();
			_createEffectResource();
		}
	}

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Pane::setCamera( Camera* camera_3d_, Camera* camera_2d_ )
    {
        LN_SAFE_ADDREF( camera_3d_ );
        LN_SAFE_RELEASE( mCamera3D );
        mCamera3D = camera_3d_;
		if (mCamera3D) mCamera3D->setHolderPane( this );

        LN_SAFE_ADDREF( camera_2d_ );
        LN_SAFE_RELEASE( mCamera2D );
        mCamera2D = camera_2d_;
		if (mCamera2D) mCamera2D->setHolderPane( this );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Pane::setRootNode( SceneNode* root_3d_, SceneNode* root_2d_ )
    {
        LN_SAFE_ADDREF( root_3d_ );
        LN_SAFE_RELEASE( mRootNode3D );
        mRootNode3D = root_3d_;

        LN_SAFE_ADDREF( root_2d_ );
        LN_SAFE_RELEASE( mRootNode2D );
        mRootNode2D = root_2d_;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Pane::getRootNode( SceneNode** root_3d_, SceneNode** root_2d_ )
    {
        *root_3d_ = mRootNode3D;
        *root_2d_ = mRootNode2D;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Pane::setGUIDesktop( GUI::GUIDesktop* desktop_ )
    {
        LN_SAFE_ADDREF( desktop_ );
        LN_SAFE_RELEASE( mGUIDesktop );
        mGUIDesktop = desktop_;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Pane::setEffectMode( LNScreenEffectMode mode_ )
    {
        if ( mode_ != mEffectMode )
        {
            _disposeEffectResource();

            if ( mode_ == LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
            {
				_createEffectResource();
            }

            mEffectMode = mode_;
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Pane::addChildPane( Pane* pane_ )
    {
        mChildPaneArray.push_back( pane_ );
        LN_SAFE_ADDREF( pane_ );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Pane::removeChildPane( Pane* pane_ )
    {
        if ( Base::STLUtil::remove( mChildPaneArray, pane_ ) )
        {
            LN_SAFE_RELEASE( pane_ );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Pane::blur(
        double duration_,
        lnfloat power_,
        lnfloat scale_,
        const LVector3& center_,
        bool center_is_3d_ )
    {
        LVector3 pos;

        if ( center_is_3d_ )
        {
            LVector3::transformCoord( &pos, center_, mCamera3D->getViewProjectionMatrix() );
        }
        else
        {
            LVector3::transformCoord( &pos, center_, mCamera2D->getProjectionMatrix() );
        }

        mScreenEffect->blur( duration_, power_, scale_, pos );
    }


    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Pane::update( const Game::GameTime& game_time_ )
    {
		if ( mEffectMode != LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
		{
#if 0
			if ( mRootNode3D->getSceneNodeType() == LN_SCENENODE_VIEWPORT )
			{
				((Viewport*)mRootNode3D)->setTone( (const LTone&)mTone.getValue(), 0 );
			}
			if ( mRootNode2D->getSceneNodeType() == LN_SCENENODE_VIEWPORT )
			{
				((Viewport*)mRootNode2D)->setTone( (const LTone&)mTone.getValue(), 0 );
			}
#endif
		}
        else
        {
            mScreenEffect->update();
        }

        // GUI
        if ( mGUIDesktop )
        {
            mGUIDesktop->updateCoordinate( LRect( 0, 0, (int)mViewSize.x, (int)mViewSize.y ) );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Pane::makeDrawCommand( DrawCommandContainer* c_ )
    {
		Graphics::IGraphicsDevice* device = mSceneGraph->getGraphicsManager()->getGraphicsDevice();
		Graphics::IRenderer* renderer = device->getRenderer();

        if ( mEffectMode == LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
        {
            c_->add_SetRT( 0, mScreenEffect->getPrimaryRenderTarget(), true );
            c_->add_SetDepth(  mScreenEffect->getPrimaryDepthBuffer(), true );

			c_->add_ClearColor( mBackgroundColor.to32Bit( device->getGraphicsAPI() ) );
            c_->add_ClearDepth( 1.0f );
        }

        mRootNode3D->updateParamFromCameraAndLight( mCamera3D );
        mRootNode2D->updateParamFromCameraAndLight( mCamera2D );

        c_->add_SetActivePaneContext( mPaneContext, true );
        mRootNode3D->makeDrawCommand( c_, MMD_PASS_object );

        c_->add_SetActivePaneContext( mPaneContext, false );
        mRootNode2D->makeDrawCommand( c_, MMD_PASS_object );

        if ( mEffectMode == LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
        {
            c_->add_DrawContext( mPaneContext );

            c_->add_SetRT( 0, NULL, true );
            c_->add_SetDepth( NULL, true );

            c_->add_DrawContext( mPanePostDrawContext );

            mScreenEffect->swapBuffers();
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Pane::updateContext()
    {
        mPaneContext->updateContext();
		if ( mPanePostDrawContext ) mPanePostDrawContext->updateContext();

        if ( mScreenEffect ) mScreenEffect->updateDrawContent();

        // GUI
        if ( mGUIDesktop )
        {
            mGUIDesktop->updateDrawContents();
        }
    }

	//---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
	bool Pane::onEvent( const System::Event& e )
	{
		switch (e.Type)
		{
			case LNEV_DEVICE_RESET:
			{

				break;
			}
		}

		return false;
	}
    
	//---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Pane::_createEffectResource()
	{
        mScreenEffect = LN_NEW Graphics::ScreenEffect();
        mScreenEffect->initialize( this->mSceneGraph->getGraphicsManager()->getGraphicsDevice(), mViewSize );

		mPanePostDrawContext = LN_NEW PanePostDrawContext();
		mPanePostDrawContext->initialize( this );
	}

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Pane::_disposeEffectResource()
    {
        SAFE_DELETE( mScreenEffect );
		LN_SAFE_RELEASE( mPanePostDrawContext );
    }

//=============================================================================
// ¡ PaneContext
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    PaneContext::PaneContext()
        : mCamera3D     ( NULL )
        , mCamera2D     ( NULL )
        , mGUIDesktop   ( NULL )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    PaneContext::~PaneContext()
    {
        LN_SAFE_RELEASE( mCamera3D );
        LN_SAFE_RELEASE( mCamera2D );
        LN_SAFE_RELEASE( mGUIDesktop );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void PaneContext::initialize( Pane* pane_ )
    {
        mPane = pane_;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void PaneContext::updateContext()
    {
        mViewSize = mPane->mViewSize;
        LN_REFOBJ_SET( mCamera3D, mPane->mCamera3D );
        LN_REFOBJ_SET( mCamera2D, mPane->mCamera2D );
        LN_REFOBJ_SET( mGUIDesktop, mPane->mGUIDesktop );
        mEffectMode = mPane->mEffectMode;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void PaneContext::getCameraContext( CameraContext** camera_context_3d_, CameraContext** camera_context_2d_ )
    {
        *camera_context_3d_ = mCamera3D->getCameraContext();
        *camera_context_2d_ = mCamera2D->getCameraContext();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void PaneContext::draw(
        Graphics::IRenderer* renderer_,
        Graphics::GeometryRenderer* geometry_renderer_ )
    {
        if ( mEffectMode == LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
        {
            mPane->getScreenEffectRenderer()->renderBlurAndTransition( renderer_, geometry_renderer_ );
        }

        // GUI
        if ( mGUIDesktop )
        {
            mGUIDesktop->draw();
        }
    }

//=============================================================================
// ¡ PanePostDrawContext
//=============================================================================

	//---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    PanePostDrawContext::PanePostDrawContext()
	{
	}

	//---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    PanePostDrawContext::~PanePostDrawContext()
	{
	}

	//---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void PanePostDrawContext::initialize( Pane* pane_ )
	{
		mPane = pane_;
	}

	//---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void PanePostDrawContext::updateContext()
	{
	}

	//---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
	void PanePostDrawContext::draw(
        Graphics::IRenderer* renderer_,
        Graphics::GeometryRenderer* geometry_renderer_ )
	{
        mPane->getScreenEffectRenderer()->renderScreen( renderer_, geometry_renderer_ );        
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