//=============================================================================
//【 SceneGraph 】
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
// ■ SceneGraph クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
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
    // ● デストラクタ
    //---------------------------------------------------------------------
    SceneGraph::~SceneGraph()
    {
        SAFE_DELETE( mSceneGraphContext );

        // SceneShaderManager は SceneNode のデストラクタからいろいろ経由して
        // オフスクリーン RT からノードを外すところまで使う。
        // そのため、finalize ではなく一番最後のデストラクタで開放する
        LN_SAFE_RELEASE( mSceneShaderManager );
        LN_SAFE_RELEASE( mManager );
    }

    //---------------------------------------------------------------------
    // ● 初期化
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

        // シェーダ管理クラス
        SceneShaderManager::InitData ss_data;
        ss_data.LogFile         = init_data_.LogFile;
        ss_data.FileManager     = init_data_.FileManager;
        ss_data.GraphicsManager = init_data_.GraphicsManager;
        ss_data.ShaderCacheSize = init_data_.ShaderCacheSize;
        ss_data.UseSemantics    = true;
        mSceneShaderManager = LN_NEW SceneShaderManager( this );
        LN_CALL_R( mSceneShaderManager->initialize( ss_data ) );

        // デフォルトのシェーダプログラムを作る
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

        // デフォルトのノード
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

        // デフォルトのカメラ
        mDefaultCamera = LN_NEW Camera( this );
        mDefaultCamera->initialize( false );

        mDefault2DCamera = LN_NEW Camera( this );
        mDefault2DCamera->initialize( true );

        // デフォルトのライト
        mDefaultLight = LN_NEW Light( this );
        LN_CALL_R( mDefaultLight->initialize() );
        mDefaultLight->setLightingPriority( LN_DEFAULT_LIGHT_PRIORITY );

        // デフォルトのペイン
        mDefaultRootPane = LN_NEW Pane( this );
        mDefaultRootPane->initialize( view_size );
        mDefaultRootPane->setCamera( mDefaultCamera, mDefault2DCamera );
        mDefaultRootPane->setRootNode(
            mDefaultSceneNodes[ LN_DEFAULT_SCENENODE_3D ],
            mDefaultSceneNodes[ LN_DEFAULT_SCENENODE_2D ] );
        mDefaultRootPane->setEffectMode( LN_SCREENEFFECTMODE_BUFFERINGEFFECT );

        // 描画プロキシ
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

        // 描画コマンドリストの初期化
        mDrawCommand.initialize(
            mGraphicsManager->getGraphicsDevice(),
            mGraphicsManager->getGeometryRenderer(),
            mSceneShaderManager,
            mSceneGraphContext );

        printf("SceneNode の add remove は 解放リストが必要。マルチスレッド対策。追加は必要なさそう。\n");

        
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 終了処理
    //---------------------------------------------------------------------
    void SceneGraph::finalize()
    {
        // すべてのノードを管理リストから取り除く
        ln_foreach( SceneNode* node, mSceneNodeList )
        {
            node->release();
        }
        mSceneNodeList.clear();

        // 追加リストからも取り除く
        
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
    // ● デフォルトで作成されるカメラの取得 (0:3D 1:2D)
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
    // ● フレーム更新 (makeDrawCommand() の前に一度呼ぶ) 
    //---------------------------------------------------------------------
    void SceneGraph::update( const Game::GameTime& game_time_ )
    {
        // 追加リストにノードがある場合はメインの管理リストに追加
        if ( !mSceneNodeAdditionalList.empty() )
        {
            ln_foreach( SceneNode* node, mSceneNodeAdditionalList )
            {
                mSceneNodeList.push_back( node );
            }
            mSceneNodeAdditionalList.clear();
        }

        // 削除チェック & フレーム更新
        SceneNodeArray::iterator itr = mSceneNodeList.begin();
        SceneNodeArray::iterator end = mSceneNodeList.end();
        while ( itr != end )
        {
            SceneNode* node = (*itr);
            // 管理リストからのみしか参照されていない場合は削除
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

        // ワールド行列を階層的に更新する
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
    // ● 視錐台 (カメラまたはシャドウ用ライト) 更新
    //---------------------------------------------------------------------
    void SceneGraph::updateViewFrustum( Camera* camera_ )
    {
        return;

        Camera*    camera    = ( camera_ ) ? camera_ : mActiveCamera;
        mActiveViewFrustum = camera;

        //-----------------------------------------------------
        // エフェクトへの行列設定

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

        

        // setActiveCamera でもいいかも？
        mDrawCommand.add_SetCameraParamToShader( camera );

        //-----------------------------------------------------
        // ライトを視錐台カリング

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
                // ディレクショナルライトの場合はそのまま追加
                if ( light->getType() == LN_LIGHTTYPE_DIRECTIONAL )
                {
                    light_data.ActiveLight = light;
                    light_data.Distance = LVector3::squareLength( light->getPosition() - camera->getPosition() );
                    mActiveLightDataArray.push_back( light_data );
                }
                // ポイント、スポットは有効距離を含めて視錐台に入っているかを確認
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
        // Z ソート基準値計算、Z ソート、ビルボード行列計算、カメラのView Proj行列が必要なシェーダ用行列の計算
        // 適用するライトの列挙、ライトのView Proj行列が必要なシェーダ用行列の計算

        
        // デフォルトで作成されるノードは mSceneNodeList には追加されていないので別途更新
        //for ( int i = 0; i < LN_MAX_DEFAULT_NODES; ++i )
        //{
        //    mDefaultSceneNodes[ i ]->updateParamFromCameraAndLight( camera );
        //}

        // TODO:カリング後のノードだけ処理するようにすれば少し最適化できるかも
        ln_foreach( SceneNode* node, mSceneNodeList )
        {
            if ( node->getDrawCoordSpace() == camera->getCoordSpace() )
            {
                node->updateParamFromCameraAndLight( camera );
            }
        }

        
    }

    //---------------------------------------------------------------------
    // ● すべてのオフスクリーンRTの描画コマンド作成
    //---------------------------------------------------------------------
    void SceneGraph::makeOfscreenSceneDrawCommand()
    {
        mSceneShaderManager->makeDrawCommand( &mDrawCommand, MMD_PASS_object );
    }

    //---------------------------------------------------------------------
    // ● 描画コマンドを作成する
    //---------------------------------------------------------------------
    LNRESULT SceneGraph::makeDrawCommand( SceneNode* root_node_ )//, Camera* camera_ )
    {
        /*
        たとえプレ描画リストを作っても、
        グループを超えたソートは行わない方がいい。
        （ビューポートの外に描画することになる。）

        なお、ビューポートはGUIのように階層的にクリッピングしていくのが正しい。
        そのため、グループを超えたソートはもはや論外。
        */

        // NULL を受け取った場合はデフォルトのものを使う
        //SceneNode* root_node = ( root_node_ ) ? root_node_ : mDefaultSceneNodes[ LN_DEFAULT_SCENENODE_ROOT ];


        
        
        

        //-----------------------------------------------------
        // 

        mDrawCommand.add_ClearDepth( 1.0f );


        

        //-----------------------------------------------------
        // メインのスクリーンに描画

        // すべての優先パラメータをリセット
        ln_foreach( SceneNode* node, mSceneNodeList )
        {
            node->setPriorityParameter( NULL );
        }

        // 全ノード
        makeSceneNodeDrawCommand( root_node_, MMD_PASS_object );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● SceneNode の描画コマンドを作成する
    //---------------------------------------------------------------------
    void SceneGraph::makeSceneNodeDrawCommand( SceneNode* root_node_, MMDPass pass_ )
    {
        root_node_->makeDrawCommand( &mDrawCommand, pass_ );
    }

    //---------------------------------------------------------------------
    // ● 全ノードの drawDebug() を呼び出す
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
    // ● 描画コマンドを実行する
    //---------------------------------------------------------------------
    LNRESULT SceneGraph::render()
    {
        LNRESULT lr = mDrawCommand.execute();
        mDrawCommand.clear();
        return lr;
    }

    //---------------------------------------------------------------------
    // ● メッセージ処理 (Manager から呼ばれる)
    //---------------------------------------------------------------------
    LNRESULT SceneGraph::onEvent( const System::Event& args_ )
    {
        LNRESULT lr;

        LN_CALL_R( mSceneShaderManager->onEvent( args_ ) );

		ln_foreach( Pane* n, mPaneList )
		{
			if ( n->onEvent( args_ ) ) return LN_HANDLED;
		}
		
        // カメラのイベント処理
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
    // ● (SceneNode のコンストラクタから呼ばれる。ツリーではなく管理リストに追加する)
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
    // ● (SceneNode のデストラクタから呼ばれる)
    //---------------------------------------------------------------------
    void SceneGraph::onDeleteNode( SceneNode* node_ )
    {
        // update() で参照カウントをチェックして削除するようにしたので、
        // ここではリストから外さない
        //mSceneNodeList.remove( node_ );

        // 名前と、自身のポインタに一致する要素を取り除く
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
                        break;  // すべて除く場合はこれを外して it を再取得するようにする
                    }
                }
            }
        }

        mSceneShaderManager->removeSceneNodeFromOffscreenScene( node_ );
    }

    //---------------------------------------------------------------------
    // ● SceneNode の名前を変更する直前に呼び出す (SceneNode の setName() から呼ばれる)
    //---------------------------------------------------------------------
    void SceneGraph::onSetNodeName( SceneNode* node_, const lnChar* old_name_, const lnChar* new_name_ )
    {
        // 前の名前と、自身のポインタに一致する要素を取り除く
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
                        break;  // すべて除く場合はこれを外して it を再取得するようにする
                    }
                }
            }
        }

        // 新しい名前で map に追加
        if ( _tcslen( new_name_ ) )
        {
            mNodeNameMap.insert( SceneNodeNamePair( new_name_, node_ ) );
        }
    }

    //---------------------------------------------------------------------
    // ● 有効ライトの中からライトを列挙する 
    //---------------------------------------------------------------------
    void SceneGraph::enumLights( LightArray* out_, const LVector3& ref_pos_ )
    {
        // ソート基準値の設定
        ActiveLightDataArray::iterator itr = mActiveLightDataArray.begin();
        ActiveLightDataArray::iterator end = mActiveLightDataArray.end();
        for ( ; itr != end; ++itr )
        {
            itr->Distance = LVector3::squareLength( itr->ActiveLight->getPosition() - ref_pos_ );//itr->DistanceToCamera;
        }

        // ソート (先頭から順に、優先度高、基準値の昇順)
        std::sort( mActiveLightDataArray.begin(), mActiveLightDataArray.end(), _cmpLights );

        // 出力 (足りない分は NULL で埋める)
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
    // ● ライト列挙用の比較関数
    //---------------------------------------------------------------------
    bool SceneGraph::_cmpLights( const ActiveLightData& l_, const ActiveLightData& r_ )
    {
        if ( l_.ActiveLight->getLightingPriority() == r_.ActiveLight->getLightingPriority() )
        {
            // 昇順
            return l_.Distance <= r_.Distance;
        }
        // 降順
        return l_.ActiveLight->getLightingPriority() >= r_.ActiveLight->getLightingPriority();
    }

//=============================================================================
// ■ SceneGraphContext
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
            // ソート基準値の設定
            ln_foreach( LightContext* lc, mActiveLightContextArray)
            {
                lc->DistanceForSort = LVector3::squareLength( lc->getPosition() - ref_pos_ );
            }

            // ソート (先頭から順に、優先度高、基準値の昇順)
            std::sort( mActiveLightContextArray.begin(), mActiveLightContextArray.end(), _cmpLights );

            // 出力
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
        //  カメラ選択

        CameraContext* camera_context_3d;
        CameraContext* camera_context_2d;
        pane_context_->getCameraContext( &camera_context_3d, &camera_context_2d );
        CameraContext* camera_context = (is_3d_) ? camera_context_3d : camera_context_2d;

        //-----------------------------------------------------
        //  プロジェクション行列・視錐台の作成

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
        // エフェクトへの行列設定

        mSceneGraph->mEffectManager->setViewProjection(
            camera_context->getViewMatrix(), proj, is_3d_ );

        //-----------------------------------------------------
        // シェーダへのカメラデータ設定

        mSceneGraph->mSceneShaderManager->updateAllShaderCameraParam( camera_context->getCameraSceneParam() );

        //-----------------------------------------------------
        // ライトを視錐台カリング

        mActiveLightContextArray.clear();

        ln_foreach( LightContext* lc, mLightContextArray )
        {
            if ( lc->isEnable() )
            {
                // ディレクショナルライトの場合はそのまま追加
                if ( lc->getType() == LN_LIGHTTYPE_DIRECTIONAL )
                {
                    mActiveLightContextArray.push_back( lc );
                }
                // ポイント、スポットは有効距離を含めて視錐台に入っているかを確認
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
        // ノードのコンテキストにも通知

        // TODO:カリング後のノードだけ処理するようにすれば少し最適化できるかも
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
            // 昇順
            return l_->DistanceForSort <= r_->DistanceForSort;
        }
        // 降順
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