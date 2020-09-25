//=============================================================================
//�y SceneGraph �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Resource/LNResource.h"
#include "../Graphics/Manager.h"
#include "../Effect/Manager.h"
#include "Manager.h"
#include "SceneNode/SceneNode.h"
#include "SceneNode/DummyNode.h"
#include "SceneNode/Camera.h"
#include "SceneNode/Light.h"
#include "SceneNode/Proxy.h"
#include "SceneShader/SceneShaderManager.h"
#include "SceneShader/SceneShader.h"
#include "SceneGraph.h"

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
// �� SceneGraph �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    SceneGraph::SceneGraph( Manager* manager_ )
        : mLogFile              ( NULL )
        , mManager              ( manager_ )
        , mFileManager          ( NULL )
        , mGraphicsManager      ( NULL )
        , mGeometryRenderer     ( NULL )
        , mEffectManager        ( NULL )
        , mPhysicsManager       ( NULL )
        , mSceneShaderManager   ( NULL )
        , mDefaultCamera        ( NULL )
        , mDefaultLight         ( NULL )
        , mDefaultRootPane      ( NULL )
        , mActiveViewFrustum    ( NULL )
        , mEffect3DBatchProxy   ( NULL )
        , mEffect2DBatchProxy   ( NULL )
        , mBatchSpriteProxy3D   ( NULL )
        , mBatchSpriteProxy2D   ( NULL )
        , mPhysicsDebugDrawProxy( NULL )
        , mSceneGraphContext    ( NULL )
    {
        LN_SAFE_ADDREF( mManager );

        memset( mDefaultSceneNodes, 0, sizeof( mDefaultSceneNodes ) );
        memset( mDefaultShaderArray, 0, sizeof( mDefaultSceneNodes ) );
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    SceneGraph::~SceneGraph()
    {
        SAFE_DELETE( mSceneGraphContext );

        // SceneShaderManager �� SceneNode �̃f�X�g���N�^���炢�낢��o�R����
        // �I�t�X�N���[�� RT ����m�[�h���O���Ƃ���܂Ŏg���B
        // ���̂��߁Afinalize �ł͂Ȃ���ԍŌ�̃f�X�g���N�^�ŊJ������
        LN_SAFE_RELEASE( mSceneShaderManager );
        LN_SAFE_RELEASE( mManager );
    }

    //---------------------------------------------------------------------
    // �� ������
    //---------------------------------------------------------------------
    LNRESULT SceneGraph::initialize( const InitData& init_data_ )
    {
        LNRESULT lr;

        mFileManager        = init_data_.FileManager;
        mGraphicsManager    = init_data_.GraphicsManager;
        mGeometryRenderer   = mGraphicsManager->getGeometryRenderer();
        mEffectManager      = init_data_.EffectManager;
        mPhysicsManager     = init_data_.PhysicsManager;
        mVirtualViewSize    = init_data_.VirtualViewSize;

        // �V�F�[�_�Ǘ��N���X
        SceneShaderManager::InitData ss_data;
        ss_data.LogFile         = init_data_.LogFile;
        ss_data.FileManager     = init_data_.FileManager;
        ss_data.GraphicsManager = init_data_.GraphicsManager;
        ss_data.ShaderCacheSize = init_data_.ShaderCacheSize;
        ss_data.UseSemantics    = true;
        mSceneShaderManager = LN_NEW SceneShaderManager( this );
        LN_CALL_R( mSceneShaderManager->initialize( ss_data ) );

        // �f�t�H���g�̃V�F�[�_�v���O���������
        SceneShader* shader;
        mSceneShaderManager->createSceneShader(
                &shader, _T( "LNResource\\Shader\\SSBasic2D.lnfx" ) );
        mDefaultShaderArray[ LN_DEFSHADER_BASIC_2D ] = shader;

        mSceneShaderManager->createSceneShader(
            &shader, _T( "LNResource\\Shader\\SSBasic3D.lnfx" ) );
        mDefaultShaderArray[ LN_DEFSHADER_BASIC_3D ] = shader;

        mSceneShaderManager->createSceneShader(
            &shader, _T( "LNResource\\Shader\\SSNoLighting.lnfx" ) );
        mDefaultShaderArray[ LN_DEFSHADER_NOLIGHTING ] = shader;

        // �f�t�H���g�̃m�[�h
        Geometry::Size view_size( (int)mVirtualViewSize.x, (int)mVirtualViewSize.y );
        Viewport* node_root = LN_NEW Viewport( this );
        Viewport* node_3d   = LN_NEW Viewport( this );
        Viewport* node_2d   = LN_NEW Viewport( this );
        LN_CALL_R( node_root->initialize( view_size ) );
        LN_CALL_R( node_3d->initialize( view_size ) );
        LN_CALL_R( node_2d->initialize( view_size ) );

        LN_CALL_R( node_root->addChild( node_3d ) );
        LN_CALL_R( node_root->addChild( node_2d ) );

        mDefaultSceneNodes[ LN_DEFAULT_SCENENODE_ROOT ] = node_root;
        mDefaultSceneNodes[ LN_DEFAULT_SCENENODE_3D ]   = node_3d;
        mDefaultSceneNodes[ LN_DEFAULT_SCENENODE_2D ]   = node_2d;

        // �f�t�H���g�̃J����
        mDefaultCamera = LN_NEW Camera( this );
        mDefaultCamera->initialize( false );

        mDefault2DCamera = LN_NEW Camera( this );
        mDefault2DCamera->initialize( true );

        // �f�t�H���g�̃��C�g
        mDefaultLight = LN_NEW Light( this );
        LN_CALL_R( mDefaultLight->initialize() );
        mDefaultLight->setLightingPriority( LN_DEFAULT_LIGHT_PRIORITY );

        // �f�t�H���g�̃y�C��
        mDefaultRootPane = LN_NEW Pane( this );
        mDefaultRootPane->initialize( view_size );
        mDefaultRootPane->setCamera( mDefaultCamera, mDefault2DCamera );
        mDefaultRootPane->setRootNode(
            mDefaultSceneNodes[ LN_DEFAULT_SCENENODE_3D ],
            mDefaultSceneNodes[ LN_DEFAULT_SCENENODE_2D ] );
        mDefaultRootPane->setEffectMode( LN_SCREENEFFECTMODE_BUFFERINGEFFECT );

        // �`��v���L�V
        mEffect3DBatchProxy = LN_NEW EffectBatchProxy( this );
        mEffect3DBatchProxy->initialize( mEffectManager, LN_DRAWCOORD_3D );

        mEffect2DBatchProxy = LN_NEW EffectBatchProxy( this );
        mEffect2DBatchProxy->initialize( mEffectManager, LN_DRAWCOORD_2D );

        mBatchSpriteProxy3D = LN_NEW BatchSpriteProxy( this );
        mBatchSpriteProxy3D->initialize( true );

        mBatchSpriteProxy2D = LN_NEW BatchSpriteProxy( this );
        mBatchSpriteProxy2D->initialize( false );

        mPhysicsDebugDrawProxy= LN_NEW PhysicsDebugDrawProxy( this );
        mPhysicsDebugDrawProxy->initialize();


        mSceneGraphContext = LN_NEW SceneGraphContext();
        mSceneGraphContext->initialize( this );

        // �`��R�}���h���X�g�̏�����
        mDrawCommand.initialize(
            mGraphicsManager->getGraphicsDevice(),
            mGraphicsManager->getGeometryRenderer(),
            mSceneShaderManager,
            mSceneGraphContext );

        printf("SceneNode �� add remove �� ������X�g���K�v�B�}���`�X���b�h�΍�B�ǉ��͕K�v�Ȃ������B\n");

        
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �I������
    //---------------------------------------------------------------------
    void SceneGraph::finalize()
    {
        // ���ׂẴm�[�h���Ǘ����X�g�����菜��
        ln_foreach( SceneNode* node, mSceneNodeList )
        {
            node->release();
        }
        mSceneNodeList.clear();

        // �ǉ����X�g�������菜��
        
        ln_foreach( SceneNode* node, mSceneNodeAdditionalList )
        {
            node->release();
        }
        mSceneNodeAdditionalList.clear();
        

        LN_SAFE_RELEASE( mEffect3DBatchProxy );
        LN_SAFE_RELEASE( mEffect2DBatchProxy );
        LN_SAFE_RELEASE( mBatchSpriteProxy3D );
        LN_SAFE_RELEASE( mBatchSpriteProxy2D );
        LN_SAFE_RELEASE( mPhysicsDebugDrawProxy );

        for ( int i = 0; i < LN_MAX_DEFAULT_NODES; ++i )
        {
            LN_SAFE_RELEASE( mDefaultSceneNodes[ i ] );
        }

        for ( int i = 0; i < LN_MAX_DEFAULT_SHADERS; ++i )
        {
            LN_SAFE_RELEASE( mDefaultShaderArray[ i ] );
        }

        LN_SAFE_RELEASE( mDefaultLight );
        LN_SAFE_RELEASE( mDefaultCamera );
        LN_SAFE_RELEASE( mDefault2DCamera );
        LN_SAFE_RELEASE( mDefaultRootPane );

        if ( mSceneShaderManager )
        {
            mSceneShaderManager->finalize();
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    //void SceneGraph::setVirtualScreenSize( const Geometry::Size& size_ )
    //{
    //    mVirtualViewSize.set( (lnfloat)size_.w, (lnfloat)size_.h );
    //}
   
    //---------------------------------------------------------------------
    // �� �f�t�H���g�ō쐬�����J�����̎擾 (0:3D 1:2D)
    //---------------------------------------------------------------------
    Camera* SceneGraph::getDefaultCamera( int type_ )
    {
        switch ( type_ )
        {
            case 0 : 
            	return mDefaultCamera;
            case 1 : 
            	return mDefault2DCamera;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    // �� �t���[���X�V (makeDrawCommand() �̑O�Ɉ�x�Ă�) 
    //---------------------------------------------------------------------
    void SceneGraph::update( const Game::GameTime& game_time_ )
    {
        // �ǉ����X�g�Ƀm�[�h������ꍇ�̓��C���̊Ǘ����X�g�ɒǉ�
        if ( !mSceneNodeAdditionalList.empty() )
        {
            ln_foreach( SceneNode* node, mSceneNodeAdditionalList )
            {
                mSceneNodeList.push_back( node );
            }
            mSceneNodeAdditionalList.clear();
        }

        // �폜�`�F�b�N & �t���[���X�V
        SceneNodeArray::iterator itr = mSceneNodeList.begin();
        SceneNodeArray::iterator end = mSceneNodeList.end();
        while ( itr != end )
        {
            SceneNode* node = (*itr);
            // �Ǘ����X�g����݂̂����Q�Ƃ���Ă��Ȃ��ꍇ�͍폜
            if ( node->getRefCount() == 1 )
            {
                itr = mSceneNodeList.erase( itr );
				end = mSceneNodeList.end();
				LN_SAFE_RELEASE( node );
            }
			else
			{
				if ( node->isEnableAutoUpdate() )
				{
					node->update( game_time_ );
				}

				++itr;
			}
        }

        mSceneShaderManager->updateShaderList();
        mSceneShaderManager->updateAllShaderGlobalParam( game_time_ );

        // ���[���h�s����K�w�I�ɍX�V����
        mDefaultSceneNodes[ LN_DEFAULT_SCENENODE_ROOT ]->updateTransformHierarchy( LMatrix(), NULL );

        ln_foreach( Pane* c, mPaneList )
        {
            c->update( game_time_ );
        }
        
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void SceneGraph::updateAllContext()
    {
        ln_foreach( SceneNode* n, mSceneNodeList )
        {
            n->updateContext();
        }
        ln_foreach( Camera* c, mCameraList )
        {
            c->updateContext();
        }
        ln_foreach( Light* c, mLightList )
        {
            c->updateContext();
        }
        ln_foreach( Pane* c, mPaneList )
        {
            c->updateContext();
        }

        mSceneGraphContext->updateContext();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void SceneGraph::makeDrawCommand( Pane* pane_ )
    {
        pane_->makeDrawCommand( &mDrawCommand );
        /*
        SceneNode* root_3d;
        SceneNode* root_2d;
        pane_->getRootNode( &root_3d, &root_2d );

        Camera* camera_3d;
        Camera* camera_2d;
        pane_->getCamera( &camera_3d, &camera_2d );

        root_3d->updateParamFromCameraAndLight( camera_3d );
        root_2d->updateParamFromCameraAndLight( camera_2d );

        mDrawCommand.add_SetActivePaneContext( pane_->getPaneContext(), true );
        makeSceneNodeDrawCommand( root_3d, MMD_PASS_object );

        mDrawCommand.add_SetActivePaneContext( pane_->getPaneContext(), false );
        makeSceneNodeDrawCommand( root_2d, MMD_PASS_object );
        */
    }



    //---------------------------------------------------------------------
    // �� ������ (�J�����܂��̓V���h�E�p���C�g) �X�V
    //---------------------------------------------------------------------
    void SceneGraph::updateViewFrustum( Camera* camera_ )
    {
        return;

        Camera*    camera    = ( camera_ ) ? camera_ : mActiveCamera;
        mActiveViewFrustum = camera;

        //-----------------------------------------------------
        // �G�t�F�N�g�ւ̍s��ݒ�

        if ( camera_->getCoordSpace() == LN_DRAWCOORD_3D )
        {
            mEffectManager->getSprite3DRenderer()->setViewProjMatrix(
                camera->getViewMatrix(),
                camera->getProjectionMatrix() );
        }
        else
        {
            mEffectManager->getSprite2DRenderer()->setViewProjMatrix(
                camera->getViewMatrix(),
                camera->getProjectionMatrix() );
        }

        

        // setActiveCamera �ł����������H
        mDrawCommand.add_SetCameraParamToShader( camera );

        //-----------------------------------------------------
        // ���C�g��������J�����O

        mActiveLightDataArray.clear();

        const LViewFrustum& vf = camera->getViewFrustum();
        ActiveLightData light_data;
        Light* light;
        LightList::iterator l_itr = mLightList.begin();
        LightList::iterator l_end = mLightList.end();
        for ( ; l_itr != l_end; ++l_itr )
        {
            light = (*l_itr);
            if ( light->isEnable() )
            {
                // �f�B���N�V���i�����C�g�̏ꍇ�͂��̂܂ܒǉ�
                if ( light->getType() == LN_LIGHTTYPE_DIRECTIONAL )
                {
                    light_data.ActiveLight = light;
                    light_data.Distance = LVector3::squareLength( light->getPosition() - camera->getPosition() );
                    mActiveLightDataArray.push_back( light_data );
                }
                // �|�C���g�A�X�|�b�g�͗L���������܂߂Ď�����ɓ����Ă��邩���m�F
                else
                {
                    if ( vf.checkInside( light->getPosition(), light->getRange() ) )
                    {
                        light_data.ActiveLight = light;
                        light_data.Distance = LVector3::squareLength( light->getPosition() - camera->getPosition() );
                        mActiveLightDataArray.push_back( light_data );
                    }
                }
            }
        }

        //-----------------------------------------------------
        // Z �\�[�g��l�v�Z�AZ �\�[�g�A�r���{�[�h�s��v�Z�A�J������View Proj�s�񂪕K�v�ȃV�F�[�_�p�s��̌v�Z
        // �K�p���郉�C�g�̗񋓁A���C�g��View Proj�s�񂪕K�v�ȃV�F�[�_�p�s��̌v�Z

        
        // �f�t�H���g�ō쐬�����m�[�h�� mSceneNodeList �ɂ͒ǉ�����Ă��Ȃ��̂ŕʓr�X�V
        //for ( int i = 0; i < LN_MAX_DEFAULT_NODES; ++i )
        //{
        //    mDefaultSceneNodes[ i ]->updateParamFromCameraAndLight( camera );
        //}

        // TODO:�J�����O��̃m�[�h������������悤�ɂ���Ώ����œK���ł��邩��
        ln_foreach( SceneNode* node, mSceneNodeList )
        {
            if ( node->getDrawCoordSpace() == camera->getCoordSpace() )
            {
                node->updateParamFromCameraAndLight( camera );
            }
        }

        
    }

    //---------------------------------------------------------------------
    // �� ���ׂẴI�t�X�N���[��RT�̕`��R�}���h�쐬
    //---------------------------------------------------------------------
    void SceneGraph::makeOfscreenSceneDrawCommand()
    {
        mSceneShaderManager->makeDrawCommand( &mDrawCommand, MMD_PASS_object );
    }

    //---------------------------------------------------------------------
    // �� �`��R�}���h���쐬����
    //---------------------------------------------------------------------
    LNRESULT SceneGraph::makeDrawCommand( SceneNode* root_node_ )//, Camera* camera_ )
    {
        /*
        ���Ƃ��v���`�惊�X�g������Ă��A
        �O���[�v�𒴂����\�[�g�͍s��Ȃ����������B
        �i�r���[�|�[�g�̊O�ɕ`�悷�邱�ƂɂȂ�B�j

        �Ȃ��A�r���[�|�[�g��GUI�̂悤�ɊK�w�I�ɃN���b�s���O���Ă����̂��������B
        ���̂��߁A�O���[�v�𒴂����\�[�g�͂��͂�_�O�B
        */

        // NULL ���󂯎�����ꍇ�̓f�t�H���g�̂��̂��g��
        //SceneNode* root_node = ( root_node_ ) ? root_node_ : mDefaultSceneNodes[ LN_DEFAULT_SCENENODE_ROOT ];


        
        
        

        //-----------------------------------------------------
        // 

        mDrawCommand.add_ClearDepth( 1.0f );


        

        //-----------------------------------------------------
        // ���C���̃X�N���[���ɕ`��

        // ���ׂĂ̗D��p�����[�^�����Z�b�g
        ln_foreach( SceneNode* node, mSceneNodeList )
        {
            node->setPriorityParameter( NULL );
        }

        // �S�m�[�h
        makeSceneNodeDrawCommand( root_node_, MMD_PASS_object );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� SceneNode �̕`��R�}���h���쐬����
    //---------------------------------------------------------------------
    void SceneGraph::makeSceneNodeDrawCommand( SceneNode* root_node_, MMDPass pass_ )
    {
        root_node_->makeDrawCommand( &mDrawCommand, pass_ );
    }

    //---------------------------------------------------------------------
    // �� �S�m�[�h�� drawDebug() ���Ăяo��
    //---------------------------------------------------------------------
    void SceneGraph::makeSceneNodeDebugDrawCommand( SceneNode* root_node_, lnU32 flags_ )
    {
        mGeometryRenderer->setViewProjMatrix( mActiveCamera->getViewProjectionMatrix() );
        ln_foreach( SceneNode* node, mSceneNodeList )
        {
            mDrawCommand.add_DrawDebug( node, flags_ );
        }
    }

    //---------------------------------------------------------------------
    // �� �`��R�}���h�����s����
    //---------------------------------------------------------------------
    LNRESULT SceneGraph::render()
    {
        LNRESULT lr = mDrawCommand.execute();
        mDrawCommand.clear();
        return lr;
    }

    //---------------------------------------------------------------------
    // �� ���b�Z�[�W���� (Manager ����Ă΂��)
    //---------------------------------------------------------------------
    LNRESULT SceneGraph::onEvent( const System::Event& args_ )
    {
        LNRESULT lr;

        LN_CALL_R( mSceneShaderManager->onEvent( args_ ) );

		ln_foreach( Pane* n, mPaneList )
		{
			if ( n->onEvent( args_ ) ) return LN_HANDLED;
		}
		
        // �J�����̃C�x���g����
        CameraList::iterator itr = mCameraList.begin();
        CameraList::iterator end = mCameraList.end();
        for ( ; itr != end; ++itr )
        {
            lr = (*itr)->onEvent( args_ );
            if ( LN_EVENT_HANDLED( lr ) )
            {
                return lr;
            }
        }

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� (SceneNode �̃R���X�g���N�^����Ă΂��B�c���[�ł͂Ȃ��Ǘ����X�g�ɒǉ�����)
    //---------------------------------------------------------------------
    void SceneGraph::addNode( SceneNode* node_ )
    {
        if ( node_ )
        {
            //mNodeRegistOrderList.push_back( node_ );
            //mSceneNodeList.push_back( node_ );
            mSceneNodeAdditionalList.push_back( node_ );
            node_->addRef();

            mSceneShaderManager->addSceneNodeToOffscreenScene( node_ );
        }
    }

    //---------------------------------------------------------------------
    // �� (SceneNode �̃f�X�g���N�^����Ă΂��)
    //---------------------------------------------------------------------
    void SceneGraph::onDeleteNode( SceneNode* node_ )
    {
        // update() �ŎQ�ƃJ�E���g���`�F�b�N���č폜����悤�ɂ����̂ŁA
        // �����ł̓��X�g����O���Ȃ�
        //mSceneNodeList.remove( node_ );

        // ���O�ƁA���g�̃|�C���^�Ɉ�v����v�f����菜��
        LRefTString old_name = node_->getName();
        if ( old_name.size() > 0 )
        {
            SceneNodeNameMap::iterator it  = mNodeNameMap.lower_bound( old_name );
            SceneNodeNameMap::iterator end = mNodeNameMap.end();
            if ( it != end )
            {
                SceneNodeNameMap::iterator up = mNodeNameMap.upper_bound( old_name );
                for ( ; it != up; ++it )
                {
                    if ( it->second == node_ )
                    {
                        mNodeNameMap.erase( it );
                        break;  // ���ׂď����ꍇ�͂�����O���� it ���Ď擾����悤�ɂ���
                    }
                }
            }
        }

        mSceneShaderManager->removeSceneNodeFromOffscreenScene( node_ );
    }

    //---------------------------------------------------------------------
    // �� SceneNode �̖��O��ύX���钼�O�ɌĂяo�� (SceneNode �� setName() ����Ă΂��)
    //---------------------------------------------------------------------
    void SceneGraph::onSetNodeName( SceneNode* node_, const lnChar* old_name_, const lnChar* new_name_ )
    {
        // �O�̖��O�ƁA���g�̃|�C���^�Ɉ�v����v�f����菜��
        if ( _tcslen( old_name_ ) )
        {
            SceneNodeNameMap::iterator it  = mNodeNameMap.lower_bound( old_name_ );
            SceneNodeNameMap::iterator end = mNodeNameMap.end();
            if ( it != end )
            {
                SceneNodeNameMap::iterator up = mNodeNameMap.upper_bound( old_name_ );
                for ( ; it != up; ++it )
                {
                    if ( it->second == node_ )
                    {
                        mNodeNameMap.erase( it );
                        break;  // ���ׂď����ꍇ�͂�����O���� it ���Ď擾����悤�ɂ���
                    }
                }
            }
        }

        // �V�������O�� map �ɒǉ�
        if ( _tcslen( new_name_ ) )
        {
            mNodeNameMap.insert( SceneNodeNamePair( new_name_, node_ ) );
        }
    }

    //---------------------------------------------------------------------
    // �� �L�����C�g�̒����烉�C�g��񋓂��� 
    //---------------------------------------------------------------------
    void SceneGraph::enumLights( LightArray* out_, const LVector3& ref_pos_ )
    {
        // �\�[�g��l�̐ݒ�
        ActiveLightDataArray::iterator itr = mActiveLightDataArray.begin();
        ActiveLightDataArray::iterator end = mActiveLightDataArray.end();
        for ( ; itr != end; ++itr )
        {
            itr->Distance = LVector3::squareLength( itr->ActiveLight->getPosition() - ref_pos_ );//itr->DistanceToCamera;
        }

        // �\�[�g (�擪���珇�ɁA�D��x���A��l�̏���)
        std::sort( mActiveLightDataArray.begin(), mActiveLightDataArray.end(), _cmpLights );

        // �o�� (����Ȃ����� NULL �Ŗ��߂�)
        int mn = mActiveLightDataArray.size();
        int n  = out_->size();
        int i = 0;
        for ( ; i < mn; ++i )
        {
            (*out_)[ i ] = mActiveLightDataArray[ i ].ActiveLight;
        }
        for ( ;i < n; ++i )
        {
            (*out_)[ i ] = NULL;
        }
    }

    //---------------------------------------------------------------------
    // �� ���C�g�񋓗p�̔�r�֐�
    //---------------------------------------------------------------------
    bool SceneGraph::_cmpLights( const ActiveLightData& l_, const ActiveLightData& r_ )
    {
        if ( l_.ActiveLight->getLightingPriority() == r_.ActiveLight->getLightingPriority() )
        {
            // ����
            return l_.Distance <= r_.Distance;
        }
        // �~��
        return l_.ActiveLight->getLightingPriority() >= r_.ActiveLight->getLightingPriority();
    }

//=============================================================================
// �� SceneGraphContext
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    SceneGraphContext::SceneGraphContext()
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void SceneGraphContext::initialize( SceneGraph* scene_ )
    {
        mSceneGraph = scene_;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void SceneGraphContext::updateContext()
    {
        mCameraContextArray.clear();
        mLightContextArray.clear();

        mVirtualViewSize = mSceneGraph->mVirtualViewSize;

        int n = mSceneGraph->mCameraList.size();
        mCameraContextArray.resize( n );
        for ( int i = 0; i < n; ++i )
        {
            mCameraContextArray[i] = mSceneGraph->mCameraList[i]->getCameraContext();
        }

        n = mSceneGraph->mLightList.size();
        mLightContextArray.resize( n );
        for ( int i = 0; i < n; ++i )
        {
            mLightContextArray[i] = mSceneGraph->mLightList[i]->getLightContext();
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void SceneGraphContext::selectLights( LightContextArray* out_, const LVector3& ref_pos_ )
    {
        int request_num = out_->size();
        int active_num = mActiveLightContextArray.size();

        if ( request_num >= active_num )
        {
            int i = 0;
            for (; i < active_num; ++i )
            {
                (*out_)[ i ] = mActiveLightContextArray[ i ];
            }
            for ( ;i < request_num; ++i )
            {
                (*out_)[ i ] = &LightContext::DUMMY;
            }
        }
        else
        {
            // �\�[�g��l�̐ݒ�
            ln_foreach( LightContext* lc, mActiveLightContextArray)
            {
                lc->DistanceForSort = LVector3::squareLength( lc->getPosition() - ref_pos_ );
            }

            // �\�[�g (�擪���珇�ɁA�D��x���A��l�̏���)
            std::sort( mActiveLightContextArray.begin(), mActiveLightContextArray.end(), _cmpLights );

            // �o��
            for ( int i = 0; i < request_num; ++i )
            {
                (*out_)[ i ] = mActiveLightContextArray[ i ];
            }
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void SceneGraphContext::OnSetActivePaneContext( PaneContext* pane_context_, bool is_3d_ )
    {
        //-----------------------------------------------------
        //  �J�����I��

        CameraContext* camera_context_3d;
        CameraContext* camera_context_2d;
        pane_context_->getCameraContext( &camera_context_3d, &camera_context_2d );
        CameraContext* camera_context = (is_3d_) ? camera_context_3d : camera_context_2d;

        //-----------------------------------------------------
        //  �v���W�F�N�V�����s��E������̍쐬

        LMatrix proj;
        const LVector2& view_size = pane_context_->getViewSize();
        if ( is_3d_ )
        {
            LMatrix::perspectiveFovLH( 
                &proj, 
                camera_context->getFovY(), 
                view_size.x, 
                view_size.y, 
                camera_context->getNearClip(), 
                camera_context->getFarClip() );

            mActiveViewFrustum.create(
                camera_context->getFovY(),
                view_size.x / view_size.y,
                camera_context->getNearClip(),
                camera_context->getFarClip() );
        }
        else
        {
            LMatrix::perspective2DLH( 
                &proj, 
                view_size.x, 
                view_size.y, 
                camera_context->getNearClip(), 
                camera_context->getFarClip() );

            mActiveViewFrustum.create2DProjection(
                view_size.x,
                view_size.y,
                camera_context->getNearClip(),
                camera_context->getFarClip() );
        }

        camera_context->setProjectionMatrix( proj );

        //-----------------------------------------------------
        // �G�t�F�N�g�ւ̍s��ݒ�

        mSceneGraph->mEffectManager->setViewProjection(
            camera_context->getViewMatrix(), proj, is_3d_ );

        //-----------------------------------------------------
        // �V�F�[�_�ւ̃J�����f�[�^�ݒ�

        mSceneGraph->mSceneShaderManager->updateAllShaderCameraParam( camera_context->getCameraSceneParam() );

        //-----------------------------------------------------
        // ���C�g��������J�����O

        mActiveLightContextArray.clear();

        ln_foreach( LightContext* lc, mLightContextArray )
        {
            if ( lc->isEnable() )
            {
                // �f�B���N�V���i�����C�g�̏ꍇ�͂��̂܂ܒǉ�
                if ( lc->getType() == LN_LIGHTTYPE_DIRECTIONAL )
                {
                    mActiveLightContextArray.push_back( lc );
                }
                // �|�C���g�A�X�|�b�g�͗L���������܂߂Ď�����ɓ����Ă��邩���m�F
                else
                {
                    if ( mActiveViewFrustum.checkInside( lc->getPosition(), lc->getRange() ) )
                    {
                        mActiveLightContextArray.push_back( lc );
                    }
                }
            }
        }

        //-----------------------------------------------------
        // �m�[�h�̃R���e�L�X�g�ɂ��ʒm

        // TODO:�J�����O��̃m�[�h������������悤�ɂ���Ώ����œK���ł��邩��
        ln_foreach( SceneNode* node, mSceneGraph->mSceneNodeList )
        {
            node->getContext()->OnSetActivePaneContext( this, pane_context_, camera_context );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    bool SceneGraphContext::_cmpLights( const LightContext* l_, const LightContext* r_ )
    {
        if ( l_->getLightingPriority() == r_->getLightingPriority() )
        {
            // ����
            return l_->DistanceForSort <= r_->DistanceForSort;
        }
        // �~��
        return l_->getLightingPriority() >= r_->getLightingPriority();
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