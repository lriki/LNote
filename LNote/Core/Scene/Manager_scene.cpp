//=============================================================================
//�y Manager �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../System/Common/PerfStopwatch.h"
#include "../File/Interface.h"
#include "../Graphics/Interface.h"
#include "../Graphics/Manager.h"
#include "SceneGraph.h"
#include "Manager.h"

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
// �� �`��X���b�h
//=============================================================================
class Manager::DrawThread
    : public Thread::ThreadObject
{
public:

	/// �R���X�g���N�^
	DrawThread( Manager* manager_ )
		: mManager( manager_ )
	{}

	/// �f�X�g���N�^
	~DrawThread()
	{
		wait();
		mManager = NULL;
	}

private:

	virtual void run()
	{
		while ( true )
		{
            if ( mManager->mEndFlag->isTrue() ) return;

            // Manager ����`��J�n�̒ʒm������܂ő҂�
            mManager->mDrawing->wait();

            //printf("DrawThread\n");

            if ( mManager->mEndFlag->isTrue() ) return;

			//mManager->_render();

            mManager->mDrawing->setFalse();
            mManager->mDrawEnd->setTrue();
		}
	}
	Manager* mManager;
};

//=============================================================================
// �� Manager �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    Manager::Manager()
        : mLogFile              ( NULL )
        , mGraphicsManager      ( NULL )
        , mDefaultScene         ( NULL )
        , mEffectManager        ( NULL )
        , mDrawThread           ( NULL )
        , mEndFlag              ( NULL )
        , mDrawEnd              ( NULL )
        , mDrawing              ( NULL )
        , mMulticoreOptimization ( false )
        , mRenderReady          ( false )
    {
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
	Manager::~Manager()
    {
        LN_SAFE_RELEASE( mEndFlag );
        LN_SAFE_RELEASE( mDrawEnd );
        LN_SAFE_RELEASE( mDrawing );
    }

    //---------------------------------------------------------------------
    // �� ������
    //---------------------------------------------------------------------
    LNRESULT Manager::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        LNRESULT lr;

        mGraphicsManager = init_data_.GraphicsManager;
        LN_SAFE_ADDREF( mGraphicsManager );
        mEffectManager = init_data_.EffectManager;

        // �f�t�H���g�̃V�[������� ( finalize() �ł̉���K�{ )
        SceneGraph::InitData data;
        data.LogFile            = mLogFile;
        data.FileManager        = init_data_.FileManager;
        data.GraphicsManager    = init_data_.GraphicsManager;
        data.EffectManager      = init_data_.EffectManager;
        data.PhysicsManager     = init_data_.PhysicsManager;
        data.VirtualViewSize    = init_data_.VirtualViewSize;
        data.ShaderCacheSize    = 32;
        //data.GUIManager            = init_data_.GUIManager; 
        //data.Criterion2DViewSize.x = init_data_.CriterionViewSize.x;
        //data.Criterion2DViewSize.y = init_data_.CriterionViewSize.y;
        //data.Default2DViewDepth    = 10000.0f;
        mDefaultScene = LN_NEW SceneGraph( this );
        LN_CALL_R( mDefaultScene->initialize( data ) );


        
        mEndFlag = LN_NEW Thread::EventFlag( false );
        mDrawEnd = LN_NEW Thread::EventFlag( true );
        mDrawing = LN_NEW Thread::EventFlag( false );

        if ( init_data_.EnableDrawThread )
        {
            mDrawThread = LN_NEW DrawThread( this );
            mDrawThread->start();
        }

        LN_LOG_INIT_END;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �I������
    //---------------------------------------------------------------------
    void Manager::finalize()
    {
        LN_LOG_FIN_BEGIN;

        if ( mEndFlag )
        {
            mEndFlag->setTrue();		    // �I���t���O�𗧂Ă���A
            mDrawing->setTrue();            // �Ƃ肠�����`��X���b�h�𑖂点�āA
        }
        SAFE_DELETE( mDrawThread );         // �X���b�h�̏I����҂�

        if ( mDefaultScene )
        {
            mDefaultScene->finalize();
            LN_SAFE_RELEASE( mDefaultScene );
        }
        LN_SAFE_RELEASE( mGraphicsManager );

        LN_LOG_FIN_END;
    }

    //---------------------------------------------------------------------
    // �� �`��
    //---------------------------------------------------------------------
	LNRESULT Manager::updateAndRender( const Game::GameTime& time_ )
    {
        mGameTime = time_;

        mDefaultScene->update( mGameTime );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::updateDefaultSceneRenderContent()
    {
        
        //_p("updateDefaultSceneRenderContent()");
        
        mDefaultScene->updateAllContext();

        mDefaultScene->makeDrawCommand( mDefaultScene->getDefaultPane() );

        /*
        // 3D
        mDefaultScene->setActiveCamera( mDefaultScene->getDefaultCamera( 0 ) );
        mDefaultScene->updateViewFrustum( mDefaultScene->getDefaultCamera( 0 ) );
        mDefaultScene->makeOfscreenSceneDrawCommand();
        mDefaultScene->makeDrawCommand( mDefaultScene->getDefaultNode( LN_DEFAULT_SCENENODE_3D ) );

        // 3D �f�o�b�O
        //mDefaultScene->makeSceneNodeDebugDrawCommand(
        //    mDefaultScene->getDefaultNode( LN_DEFAULT_SCENENODE_3D ), LN_SCENEDEBUGDRAW_ALL );

        // 2D
        mDefaultScene->setActiveCamera( mDefaultScene->getDefaultCamera( 1 ) );
        mDefaultScene->updateViewFrustum( mDefaultScene->getDefaultCamera( 1 ) );
        mDefaultScene->makeDrawCommand( mDefaultScene->getDefaultNode( LN_DEFAULT_SCENENODE_2D ) );
        */
    
        mRenderReady.setTrue();

        //_p("updateDefaultSceneRenderContent() end");
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::renderDefaultScene()
    {
        

        if ( mRenderReady.isTrue() )
        {
            //_p("renderDefaultScene()");

            _render();

            //_p("renderDefaultScene() end");
        }
    }

    //---------------------------------------------------------------------
    // �� �`��
    //---------------------------------------------------------------------
    LNRESULT Manager::_render()
    {
        LNRESULT lr;

        Graphics::IRenderer* r = mGraphicsManager->getGraphicsDevice()->getRenderer();
        Graphics::ICanvas* canvas = mGraphicsManager->getGraphicsDevice()->getDefaultCanvas();
        
        //LN_CALL_R( r->setRenderTarget( NULL, 0 ) );

        canvas->activate();
        LN_CALL_R( r->beginScene() );
        r->clear( true, true, 0xffccddee, 1.0f );//0, 1.0f );//
        LN_CALL_R( mDefaultScene->render() );
        // �Ƃ肠�����f�o�b�O�t�H���g
        mGraphicsManager->getDebugText()->draw();
       /*r->clear(true,true,0xff0000ff,1.0f);*/
        LN_CALL_R( r->endScene() );
        LN_CALL_R( canvas->present() );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� ���b�Z�[�W���� (Framework ����Ă΂��)
    //---------------------------------------------------------------------
    LNRESULT Manager::onEvent( const System::Event& args_ )
    {
        return mDefaultScene->onEvent( args_ );
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================