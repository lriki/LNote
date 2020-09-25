//=============================================================================
//【 OffscreenScene 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../SceneGraph.h"
#include "../DrawCommand.h"
#include "SceneShader.h"
#include "SceneShaderManager.h"
#include "OffscreenScene.h"

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
// ■ OffscreenScene クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    OffscreenScene::OffscreenScene( SceneShaderManager* manager_, SceneShader* shader_ )
        : mManager      ( manager_ )
        , mSceneShader  ( shader_ )
        , mClearColor   ( 0 )
        , mRTTexture    ( NULL )
        , mIsThereSelf  ( false )
        , mNeedRefreshAffectNodes   ( true )
    {
        LN_SAFE_ADDREF( mManager );
        mManager->addOffscreenScene( this );
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    OffscreenScene::~OffscreenScene()
    {
        _releaseEffects();

        LN_SAFE_RELEASE( mRTTexture );
        
        if ( mManager )
        {
            mManager->removeOffscreenScene( this );
            LN_SAFE_RELEASE( mManager );
        }
    }

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    LNRESULT OffscreenScene::initialize( const InitData& init_data_ )
    {
        mErrorMessage.clear();
        setClearColor( init_data_.ClearColor );
        setClearDepth( init_data_.ClearDepth );
        setDescription( init_data_.Description );
        setDefaultEffectStrings( init_data_.DefaultEffectStrings );

        mRTTexture = init_data_.RTTexture;
        LN_SAFE_ADDREF( mRTTexture );

        _refreshEffects();

        mNeedRefreshAffectNodes = true;
        return LN_OK;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void OffscreenScene::setClearColor( const LVector4& color_ )
    {
        mClearColor = reinterpret_cast< const LColor& >( color_ ).to32Bit( this->mManager->getGraphicsManager()->getGraphicsAPI() );
    }

    //---------------------------------------------------------------------
	// ● ノードをDefaultEffect の指定と一致する場合に追加する
    //---------------------------------------------------------------------
    void OffscreenScene::addSceneNode( SceneNode* node_ )
    {
        // ビューポートの場合ははじく
        if ( node_->getSceneNodeType() == LN_SCENENODE_VIEWPORT ) return;

        ln_foreach( DefaultEffectEntry& e, mDefaultEffectEntryList )
        {
            // そのまま追加
            if ( e.ObjectName == _T( "*" ) )
            {
                if ( e.Type != ENTRY_HIDE )
                {
                    SceneNodeEntry se;
                    se.Node = node_;
                    se.PriorityParameter.Shader = e.Shader;
                    se.PriorityParameter.Hide   = false;
                    mSceneNodeEntryArray.push_back( se );
                }
                return;
            }
            // このオフスクリーンRTを持つシェーダが設定されているノード自身
            else if ( e.ObjectName == _T( "self" ) )
            {
                if ( node_ == mSceneShader->getSelfSceneNode() )
                {
                    if ( e.Type != ENTRY_HIDE )
                    {
                        SceneNodeEntry se;
                        se.Node   = node_;
                        se.PriorityParameter.Shader = e.Shader;
                        se.PriorityParameter.Hide   = false;
                        mSceneNodeEntryArray.push_back( se );
                    }
                    return;
                }
            }
            // ワイルドカード付きの比較
            else if ( Base::StringUtil::match( node_->getName(), e.ObjectName.c_str() ) )
            {
                if ( e.Type != ENTRY_HIDE )
                {
                    SceneNodeEntry se;
                    se.Node   = node_;
                    se.PriorityParameter.Shader = e.Shader;
                    se.PriorityParameter.Hide   = false;
                    mSceneNodeEntryArray.push_back( se );
                }
                return;
            }
        }
    }

    //---------------------------------------------------------------------
	// ● ノードを外す
    //---------------------------------------------------------------------
    void OffscreenScene::removeSceneNode( SceneNode* node_ )
    {
        SceneNodeEntryList::iterator itr = mSceneNodeEntryArray.begin();
        SceneNodeEntryList::iterator end = mSceneNodeEntryArray.end();
        while ( itr != end )
        {
            if ( itr->Node == node_ )
            {
                itr = mSceneNodeEntryArray.erase( itr );
                end = mSceneNodeEntryArray.end();
                continue;
            }
            ++itr;
        }
    }

    //---------------------------------------------------------------------
	// ● シーンに登録されているすべてのノードで addSceneNode() を呼ぶ
    //---------------------------------------------------------------------
    void OffscreenScene::refreshAffectNodes()
    {
        /*
            この処理は initialize() に書くのはNG。
            なぜかというと、Model::initalize() の同名シェーダの自動読み込みではなく、
            ユーザー自身がシェーダ作って SceneNode::setShader() でシェーダを設定する場合に
            対応すできなくなるから。
            →initialize() で処理する場合、その時点で self として扱う SceneNode を
              知らないとならない。対応するには createSceneshader に SceneNode を渡すように
              しないとならないけど、setShader() を別途呼び出い場合は
              「createShader 前に SceneNode を作っておかないとダメ」だし、
              やっぱり createSceneshader に SceneNode を渡さないといけないっていうのがヤダ。
        */

        SceneNodeArray& node_list = mManager->getScene()->getSceneNodeArray();
        ln_foreach( SceneNode* node, node_list )
        {
            addSceneNode( node );
        }
    }

    //---------------------------------------------------------------------
	// ● すべてのオフスクリーンRTの描画コマンドを作成する
    //---------------------------------------------------------------------
    void OffscreenScene::makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ )
    {
        // 初回実行時等、すべてのノードに対してこのオフスクリーンRTで描画するべきかをチェックして選択
        if ( mNeedRefreshAffectNodes )
        {
            refreshAffectNodes();
            mNeedRefreshAffectNodes = false;
        }

        // オフスクリーン描画時の基準カメラをデフォルトのもの以外にするときは
        // この辺に更新処理を書いておく

        // 以下のノード配列の取得も、事前に視錐台カリングした後の
        // 一覧について行えばもっと最適化できるかも


        // このオフスクリーンRTで描画するノードには優先パラメータを設定。
        // 描画しない場合は hide。
        SceneNodeArray& node_list = mManager->getScene()->getSceneNodeArray();
        ln_foreach( SceneNode* node, node_list )
        {
            node->setPriorityParameter( &LNPriorityParameter::HIDE_PARAM );
        }
        ln_foreach( SceneNodeEntry& e, mSceneNodeEntryArray )
        {
            //LNPriorityParameter pp;
            //pp.Shader = e.Shader;
            //pp.Hide   = true;       // mSceneNodeEntryArray に入っているノードはすべて false でOK
            e.Node->setPriorityParameter( &e.PriorityParameter );
        }


        // 前準備
        container_->add_SetRT( 0, mRTTexture, true );
        container_->add_ClearColor( mClearColor );
        container_->add_ClearDepth( mClearDepth );
        
        // コマンド作成
        mManager->getScene()->makeSceneNodeDrawCommand(
            mManager->getScene()->getDefaultNode( LN_DEFAULT_SCENENODE_3D ), pass_ );

        // 後処理
        container_->add_SetRT( 0, NULL, true );
    }

    //---------------------------------------------------------------------
	// ● エフェクトファイルの割り当て更新
    //---------------------------------------------------------------------
    LNRESULT OffscreenScene::_refreshEffects()
    {
        LNRESULT lr;
        ln_std_tstring tms;
        mIsThereSelf = false;

        _releaseEffects();
        mDefaultEffectEntryList.clear();

        // ; で分割 (終端が ; の場合、それは無視)
        std::vector< ln_std_tstring > entrys = 
            Base::StringUtil::split( mDefaultEffectStrings.c_str(), _T( ";" ) );

        ln_foreach( ln_std_tstring& entry, entrys )
        {
            // 次に = で分割
            std::vector< ln_std_tstring > items = 
                Base::StringUtil::split( entry, _T( "=" ) );

            // 分割後の要素数は必ず 2。それ以外はエラー
            if ( items.size() == 2 )
            {
                // 前後のスペースを削除
                Base::StringUtil::trimHFSpace( &items[0], items[0] );
                Base::StringUtil::trimHFSpace( &items[1], items[1] );

                DefaultEffectEntry e;
                e.ObjectName = items[0].c_str();

                if ( items[1] == _T( "none" ) )
                {
                    e.Type = ENTRY_NONE;
                    e.Shader = NULL;
                }
                else if ( items[1] == _T( "hide" ) )
                {
                    e.Type = ENTRY_HIDE;
                    e.Shader = NULL;
                }
                else
                {
                    // 必要なシェーダを作成
                    SceneShader* shader;
                    LN_CALL_R( mManager->createSceneShader( &shader, items[1].c_str() ) );
                    e.Type       = ENTRY_EFFECT;
                    e.EffectName = items[1].c_str();
                    e.Shader     = shader;
                }

                mDefaultEffectEntryList.push_back( e );

                // "self" がある？
                if ( items[1].c_str() == _T( "self" ) )
                {
                    mIsThereSelf = true;
                }
            }
            else
            {
                mErrorMessage = _T( "DefaultEffect item is disable." );
                return LN_ERR_INVALIDCALL;
            }
        }

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● オフスクリーン RT 内で作成したシェーダを解放する
    //---------------------------------------------------------------------
    void OffscreenScene::_releaseEffects()
    {
        ln_foreach( DefaultEffectEntry& e, mDefaultEffectEntryList )
        {
            LN_SAFE_RELEASE( e.Shader );
        }
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