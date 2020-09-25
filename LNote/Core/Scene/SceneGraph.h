//=============================================================================
//【 SceneGraph 】
//-----------------------------------------------------------------------------
///**
//  @file       SceneGraph.h
//  @brief      SceneGraph
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <algorithm>
#include "../Base/STLUtil.h"
#include "../Geometry/Vector2.h"
#include "../System/Interface.h"
#include "../Game/GameTime.h"
#include "../Graphics/Manager.h"
#include "Common.h"
#include "DrawCommand.h"
#include "SceneNode/DummyNode.h"
#include "SceneNode/Viewport.h"
#include "SceneNode/Pane.h"

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
//-----------------------------------------------------------------------------
///**
//  @brief      シーングラフ
//*/
//=============================================================================
class SceneGraph
    : public Base::ReferenceObject
{
public:

    struct InitData
    {
        Base::LogFile*      LogFile;
        File::Manager*      FileManager;
        Graphics::Manager*  GraphicsManager;
        Effect::Manager*    EffectManager;
        Physics::Manager*   PhysicsManager;
        LVector2            VirtualViewSize;
        lnU32               ShaderCacheSize;
    };

public:
    SceneGraph( Manager* manager_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~SceneGraph();

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

    /// 終了処理
    void finalize();

 //   /// 仮想スクリーンサイズの設定 (ウィンドウリサイズ時等に呼び出す。基本的に各エディタ用)
	////		バックバッファサイズまで変更する場合、この後にデバイスロストイベントを発生させること。
	////		OnResetDevice() ですべてのビューポートなどのRTを作り直している。
 //   void setVirtualScreenSize( const Geometry::Size& size_ );

 //   /// 仮想スクリーンサイズの取得
 //   const LVector2& getVirtualScreenSize() const { return mVirtualViewSize; }

	/// SceneNode 初期化時に設定されるレンダリングステートの設定
	void setNodeDefaultRenderState( const LRenderState& state_ ) { mNodeDefaultRenderState = state_; }

	/// SceneNode 初期化時に設定されるレンダリングステートの取得
	const LRenderState& getNodeDefaultRenderState() { return mNodeDefaultRenderState; }

    /// シェーダ管理クラスの取得
    SceneShaderManager* getSceneShaderManager() const { return mSceneShaderManager; }

    /// デフォルトで作成されるノードの取得
    Viewport* getDefaultNode( LNDefaultSceneNode node_idx_ ) { return mDefaultSceneNodes[ node_idx_ ]; }

    /// デフォルトで作成されるカメラの取得 (0:3D 1:2D)
    Camera* getDefaultCamera( int type_ = 0 );

    /// デフォルトで作成されるライトの取得
    Light* getDefaultLight() { return mDefaultLight; }
    
    /// デフォルトで作成されるシェーダプログラムの取得
    SceneShader* getDefaultShader( LNDefaultSceneShader shader_idx_ ) { return mDefaultShaderArray[ shader_idx_ ]; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      フレーム更新 (makeDrawCommand() の前に一度呼ぶ) 
    //
    //  @note       
    //              [同期フェーズ]
    //              シーン内で、描画ノード、カメラに依存しない
    //              グローバルなパラメータを最新の状態に更新する。<br>
    //              具体的には、<br>
    //                  - デフォルトライトの状態を全シェーダにセット
    //                  - 時間、マウス情報を全シェーダにセット
    //                  - すべてのノードのワールド行列の階層更新
    //              <br>
    //              複数カメラを使うとき (複数プレイヤー用のビューなど)、
    //              1フレーム中に重複して発生する処理をまとめた感じ。
    //              この関数は 1 フレームに 1 度だけ呼び出せば OK。
    //*/
    //---------------------------------------------------------------------
    void update( const Game::GameTime& game_time_ );

    /// [同期フェーズ][描画が必要な時]
    void updateAllContext();

    Pane* getDefaultPane() { return mDefaultRootPane; }

    /// [同期フェーズ] 一連の流れは update → 必要な分だけ makeDrawCommand() → render();
    void makeDrawCommand( Pane* pane_ );







    /// シェーダのカメラ系パラメータにセットするカメラの設定 (NULL を渡すとデフォルトカメラで更新)
    void setActiveCamera( Camera* camera_ ) { mActiveCamera = ( camera_ ) ? camera_ : mDefaultCamera; }

    /// 視錐台 (カメラまたはシャドウ用ライト) 更新 (NULL を渡すとActiveCameraで更新)
    void updateViewFrustum( Camera* camera_ );

    /// すべてのオフスクリーンRTの描画コマンド作成
    void makeOfscreenSceneDrawCommand();

    //---------------------------------------------------------------------
    ///**
    //  @brief      描画コマンドを作成する
    //
    //  @param[in]  root_node_ : このノード以下を描画する (NULL の場合はすべてのルート)
    //  @param[in]  camera_    : 描画に使うカメラ (NULL の場合はデフォルトカメラ)
    //
    //  @note       
    //              受け取ったノード以下で camera_ を使った描画コマンドを作成・追加する。
    //              複数回呼び出せばその分キューにプッシュされていく。
    //              作成したコマンドは render() を呼び出したときにすべて実行され、
    //              完了するとキューはクリアされる。
    //*/
    //---------------------------------------------------------------------
    LNRESULT makeDrawCommand( SceneNode* root_node_ );//, Camera* camera_ );

    /// SceneNode の描画コマンドを作成する (makeDrawCommand()、OffscreenScene::makeDrawCommand() から呼ばれる)
    void makeSceneNodeDrawCommand( SceneNode* root_node_, MMDPass pass_ );

    /// 全ノードの drawDebug() を呼び出す
    //      3D限定。階層描画ではない点に注意。flags_ は LNSceneDebugDrawFlag の組み合わせ。
    //      getGeometryRenderer() で取得できる GeometryRenderer の setViewProjMatrix() に、
    //      アクティブカメラの行列が設定される。
    void makeSceneNodeDebugDrawCommand( SceneNode* root_node_, lnU32 flags_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      描画コマンドを実行する
    //
    //  @note       
    //              描画用のスレッドで描画する場合、コマンド作成後に
    //              この関数を呼び出すだけで OK。
    //*/
    //---------------------------------------------------------------------
    LNRESULT render();

public:

    /// メッセージ処理 (Manager から呼ばれる)
    LNRESULT onEvent( const System::Event& args_ );

    /// FileManager の取得
    File::Manager* getFileManager() { return mFileManager; }

    /// GraphicsManager の取得
    Graphics::Manager* getGraphicsManager() { return mGraphicsManager; }

    /// GeometryRenderer の取得
    Graphics::GeometryRenderer* getGeometryRenderer() { return mGeometryRenderer; }

    /// EffectManager の取得
    Effect::Manager* getEffectManager() { return mEffectManager; }

    /// PhysicsManager の取得
    Physics::Manager* getPhysicsManager() { return mPhysicsManager; }

    /// (SceneNode のコンストラクタから呼ばれる。ツリーではなく管理リストに追加する)
    void addNode( SceneNode* node_ );

    /// (SceneNode のデストラクタから呼ばれる)
    void onDeleteNode( SceneNode* node_ );//removeNode( SceneNode* node_ );

    /// (Camera のコンストラクタから呼ばれる)
    void addCamera( Camera* camera_ ) { mCameraList.push_back( camera_ ); }

    /// (Camera のデストラクタから呼ばれる)
    void removeCamera( Camera* camera_ ) { Base::STLUtil::remove( mCameraList, camera_ ); }

    /// (Light のコンストラクタから呼ばれる)
    void addLight( Light* light_ ) { mLightList.push_back( light_ ); }

    /// (Light のデストラクタから呼ばれる)
    void removeLight( Light* light_ ) { Base::STLUtil::remove( mLightList, light_ ); }

    /// (Viewport のコンストラクタから呼ばれる)
    void addViewport( Viewport* vp_ ) { mViewportList.push_back( vp_ ); }

    /// (Viewport のデストラクタから呼ばれる)
    void removeViewport( Viewport* vp_ ) { mViewportList.remove( vp_ ); }

    /// (Pane のコンストラクタから呼ばれる)
    void addPane( Pane* pane_ ) { mPaneList.push_back( pane_ ); }

    /// (Pane のデストラクタから呼ばれる)
    void removePane( Pane* pane_ ) { Base::STLUtil::remove( mPaneList, pane_ ); }

    /// SceneNode の名前を変更する直前に呼び出す (SceneNode の setName() から呼ばれる)
    void onSetNodeName( SceneNode* node_, const lnChar* old_name_, const lnChar* new_name_ );

    /// 有効ライトの中からライトを列挙する 
    //      ref_pos_ : 基準位置
    //      makeDrawCommand() 内の SceneNode::updateParamFromCameraAndLight() 経由で呼ばれる。
    //      ライトは out_->size() の数だけ格納する。あらかじめ resize しておくこと。
    void enumLights( LightArray* out_, const LVector3& ref_pos_ );

    /// SceneShader のデストラクタから呼ばれる
    ViewportList& getViewportList() { return mViewportList; }

    /// SceneNode 一覧の取得
    SceneNodeArray& getSceneNodeArray() { return mSceneNodeList; }

    BatchSpriteProxy* getBatchSpriteProxy( bool is_3d_ ) { return (is_3d_) ? mBatchSpriteProxy3D : mBatchSpriteProxy2D; }

private:

    struct ActiveLightData
    {
        Light*      ActiveLight;
        lnfloat     DistanceToCamera;   ///< アクティブなカメラからの距離の2乗
        lnfloat     Distance;           ///< ソート用の基準距離 (ソート直前に設定する。アクティブなカメラからの距離の2乗)
    };

    typedef std::vector< ActiveLightData > ActiveLightDataArray;
    

    /// ライト列挙用の比較関数
    static bool _cmpLights( const ActiveLightData& l_, const ActiveLightData& r_ );

private:

    Base::LogFile*          mLogFile;
    Manager*                mManager;
    File::Manager*          mFileManager;
    Graphics::Manager*      mGraphicsManager;
    Graphics::GeometryRenderer* mGeometryRenderer;
    Effect::Manager*        mEffectManager;
    Physics::Manager*       mPhysicsManager;
    LVector2                mVirtualViewSize;	// 削除予定
	LRenderState			mNodeDefaultRenderState;
    SceneShaderManager*     mSceneShaderManager;

    //SceneNodeList           mNodeRegistOrderList;
    SceneNodeArray          mSceneNodeList;     ///< 作成されている SceneObject のポインタリスト
    SceneNodeNameMap        mNodeNameMap;       ///< 作成されている SceneObject のうち、名前を持つものの対応表
    SceneNodeArray          mSceneNodeAdditionalList;
    CameraList              mCameraList;
    LightList               mLightList;
    ViewportList            mViewportList;
    PaneArray               mPaneList;

    Viewport*               mDefaultSceneNodes[ LN_MAX_DEFAULT_NODES ];
    Camera*                 mDefaultCamera;
    Camera*                 mDefault2DCamera;
    Light*                  mDefaultLight;
    SceneShader*            mDefaultShaderArray[ LN_MAX_DEFAULT_SHADERS ];
    Pane*                   mDefaultRootPane;
    //Pane*                   mDefault3DPane;
    //Pane*                   mDefault2DPane;
    Camera*                 mActiveCamera;
    Camera*                 mActiveViewFrustum;
    ActiveLightDataArray    mActiveLightDataArray;  ///< 有効なライトの一覧 (makeDrawCommand() で作られる)

    EffectBatchProxy*       mEffect3DBatchProxy;
    EffectBatchProxy*       mEffect2DBatchProxy;
    BatchSpriteProxy*       mBatchSpriteProxy3D;
    BatchSpriteProxy*       mBatchSpriteProxy2D;
    PhysicsDebugDrawProxy*  mPhysicsDebugDrawProxy;

    DrawCommandContainer    mDrawCommand;    ///< 描画コマンドの格納場所
    SceneGraphContext*      mSceneGraphContext;
    
    friend class SceneGraphContext;
};

//=============================================================================
// ■ SceneGraphContext
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class SceneGraphContext
{
public:

    SceneGraphContext();

public:

    void initialize( SceneGraph* scene_ );

    void updateContext();

    void selectLights( LightContextArray* out_, const LVector3& ref_pos_ );

    void OnSetActivePaneContext( PaneContext* pane_context_, bool is_3d_ );

private:

    static bool _cmpLights( const LightContext* l_, const LightContext* r_ );

private:

    SceneGraph*             mSceneGraph;
    LVector2                mVirtualViewSize;
    CameraContextArray      mCameraContextArray;    // カメラやライトの AdditionalList を Scene に作れば必要ないけど…
    LightContextArray       mLightContextArray;
    LightContextArray       mActiveLightContextArray;   ///< OnSetActivePaneContext() での視錐台カリングで格納
    LViewFrustum            mActiveViewFrustum;         ///< OnSetActivePaneContext() での視錐台カリングで格納
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