//=============================================================================
//【 DrawCommand 】
//-----------------------------------------------------------------------------
///**
//  @file       DrawCommand.h
//  @brief      DrawCommand
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <stack>
#include <vector>
#include "Common.h"
#include "../Graphics/Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
class Camera;
class NodeRenderParam;
class SceneShaderManager;
class SceneShader;
struct LNPriorityParameter;

/// 描画コマンドの種類
enum LNDrawCommandType
{
    LN_DRAWCOM_SET_ACTIVE_PANE_CONTEXT = 1,

    LN_DRAWCOM_SET_RT,              ///< レンダーターゲットの変更
    LN_DRAWCOM_SET_DEPTH,           ///< 深度バッファを変更する
    LN_DRAWCOM_CLEAR_COLOR,         ///< カラーターゲットをクリアする
    LN_DRAWCOM_CLEAR_DEPTH,         ///< 深度ターゲットをクリアする

    LN_DRAWCOM_SET_VIEWPORT,        ///< ビューポート領域を変更する
    LN_DRAWCOM_REMOVE_VIEWPORT,     ///< ビューポート領域を元に戻す
    LN_DRAWCOM_SET_SCISSOR,         ///< シザー領域を変更する
    LN_DRAWCOM_REMOVE_SCISSOR,      ///< シザー領域を元に戻す

    LN_DRAWCOM_LOOPBEGIN,           ///< ループ開始
    LN_DRAWCOM_LOOPEND,             ///< ループ終了
    LN_DRAWCOM_SET_LOOPIDX,         ///< ループ回数をシェーダ変数に設定する

    LN_DRAWCOM_PRE_EXTERNAL_BEGIN,
    LN_DRAWCOM_PRE_EXTERNAL_END,
    LN_DRAWCOM_POST_EXTERNAL_BEGIN,
    LN_DRAWCOM_POST_EXTERNAL_END,

    LN_DRAWCOM_SET_CAMERA_PARAM,    ///< カメラ単位のパラメータをシェーダに設定
    LN_DRAWCOM_SET_OBJ_PARAM,       ///< オブジェクト単位のパラメータをシェーダに設定
    LN_DRAWCOM_DRAW_GEOMETRY,       ///< "Draw=Geometry"
    LN_DRAWCOM_DRAW_BUFFER,         ///< "Draw=Buffer"
    LN_DRAWCOM_DRAW_GEOMETRY_NO_SHADER,       ///< シェーダを使用しない (とりあえず)
    LN_DRAWCOM_DRAW_SCREEN,
    LN_DRAWCOM_DRAW_CONTEXT,
    LN_DRAWCOM_DRAW_DEBUG,          ///< デバッグ描画
};

/// 描画コマンド
struct DrawCommand
{
    lnU32     Type;       ///< LNDrawCommandType の値

    union
    {
        struct
        {
            PaneContext*    Context;
            bool            Is3D;
        } SetActivePaneContext;

        // レンダーターゲット、深度バッファの変更
        struct
        {
            Graphics::ITexture* Texture;
            lnU32               Index;
            bool                PushStack;
        } SetRT, SetDepth;

        // カラーターゲットをクリアする
        struct
        {
            lnU32   Color;  // 32ビット値
        } ClearColor;

        // 深度ターゲットをクリアする
        struct
        {
            float   Depth;
        } ClearDepth;

        // ビューポート領域の変更、シザー領域の変更
        struct
        {
            Geometry::Rect*   Area;
        } SetViewport, SetScissor;

        // レンダーステートの設定
        struct
        {
            const LRenderState*     RenderState;
        } SetRenderState;

        // ループ開始
        struct
        {
            lnU32     Count;
        } LoopBegin;

        // ループ回数設定
        struct
        {
            Graphics::IShaderVariable*    Variable;
        } SetLoopIdx;

        // カメラ単位のパラメータをシェーダに設定
        struct
        {
            Scene::Camera*              Camera;

        } SetCameraParamToShader;

        // オブジェクト単位のパラメータをシェーダに設定
        struct
        {
            //NodeRenderParam*        Param;
            //SceneNode*              Object;
            SceneNodeContext*       Context;
            SceneShader*            Shader;     ///< 優先パラメータ対応のため、分離

        } SetObjParam;

        // "Draw=Geometry"
        struct
        {
            SceneNode*              Object;
            const SubsetIndexArray* Indices;
            Graphics::IShaderPass*  Pass;
            LNPriorityParameter*    PriorityParameter;  ///< オフスクリーンRTからの設定用 (使わない場合はNULL)

        } DrawGeometry;

        // "Draw=Buffer"
        struct
        {
            Graphics::IShaderPass*  Pass;
        } DrawBuffer;

        // 
        struct
        {
            Graphics::ITexture*     Texture;
            lnfloat                 ViewSizeW;
            lnfloat                 ViewSizeH;
        } DrawScreen;

        // DrawingContext::draw()
        struct
        {
            DrawingContext* Context;
        } DrawContext;
        
        // デバッグ描画
        struct
        {
            SceneNode*  Object;
            lnU32       Flags;
        } DrawDebug;
    };
};

typedef std::vector< DrawCommand >  DrawCommandList;
typedef DrawCommandList::iterator DrawCommandListIterator;

//=============================================================================
// ■ DrawCommandContainer クラス
//-----------------------------------------------------------------------------
///**
//  @brief      描画コマンドの格納・実行クラス
//*/
//=============================================================================
class DrawCommandContainer
{
public:

    DrawCommandContainer();

public:

    /// 初期化
    void initialize(
        Graphics::IGraphicsDevice* device_,
        Graphics::GeometryRenderer* gr_,
        SceneShaderManager* sm_,
        SceneGraphContext* scene_context_ );

    /// コマンド作成、実行の前準備
    //      スタックが空の状態で add_SetRT( NULL ) を呼ぶと強制的にバックバッファが設定されてしまうことを防ぐため、
    //      すべてのスタックをクリアし、呼び出した時点でのすべてのRTと深度バッファをスタックに積む。
    void prepare();

    /// 描画コマンドリストのクリア
    void clear() { mDrawCommandList.clear(); }

    /// 描画コマンドの実行
    LNRESULT execute();

    /// 描画コマンドをコンソールに出力する (デバッグ用)
    void dump();

public:

    /// Pane の設定 (SceneGraphContext::OnSetActivePaneContext() が呼ばれる)
    void add_SetActivePaneContext( PaneContext* pane_context_, bool is_3d_ );

    /// レンダーターゲットの変更
    // (スタックには積まない。NULL を渡すと現在のスタックの先頭のRTを Renderer に設定する。この時もスタックに変化は無し)
    //  push_stack_ が true の場合はスタックに積む。push_stack_ が true だが rt が NULL の場合はポップする。
    //  スタック操作は index_ 番で区別されるスタックごとに行われる。
    void add_SetRT( u32 index_, Graphics::ITexture* rt_, bool push_stack_ );

    /// 深度バッファを変更する
    void add_SetDepth( Graphics::ITexture* db_, bool push_stack_ );

    /// カラーターゲットをクリアする
    void add_ClearColor( lnU32 color_ );

    /// 深度ターゲットをクリアする
    void add_ClearDepth( lnfloat depth_ );

    /// ビューポートを変更する (古いビューポートはスタックに積まれる)
    void add_SetViewport( Geometry::Rect* rect_ );

    /// ビューポート領域をスタックから取り出して再設定する
    void add_RemoveViewport();

    /// シザー領域を変更する (古いビューポートはスタックに積まれる)
    void add_SetScissor( Geometry::Rect* rect_ );

    /// シザー領域をスタックから取り出して再設定する
    void add_RemoveScissor();

    /// ループ開始
    void add_LoopBegin( lnU32 count_ );

    /// ループ終了
    void add_LoopEnd();

    /// ループ回数設定
    void add_SetLoopCount( Graphics::IShaderVariable* var_ );

    /// ポストエフェクトの "ScriptExternal=Color" 開始
    void add_PreExternalBegin();

    /// ポストエフェクトの "ScriptExternal=Color" までのブロックの終了
    void add_PreExternalEnd();

    /// ポストエフェクトの "ScriptExternal=Color" から終端までのブロックの開始
    void add_PostExternalBegin();

    /// ポストエフェクトの "ScriptExternal=Color" から終端までのブロックの終了
    void add_PostExternalEnd();

    /// カメラ単位の描画パラメータをシェーダに設定する  ※削除予定
    //      以前は SceneGraph::makeDrawCommand() で行っていたが、
    //      実際にコマンドを実行する前に複数回 SceneGraph::makeDrawCommand() が呼ばれるわけで、
    //      古い値は書き換わってしまう。そんなわけで、描画コマンドに組み込む。
    void add_SetCameraParamToShader( Camera* camera_ );

    /// オブジェクト単位の描画パラメータをシェーダに設定する(ワールド行列など)
    void add_SetObjectParamToShader( SceneNodeContext* context_ );//SceneNode* node_ );//NodeRenderParam* param_ );

    /// 次からの Draw=Geometry、SetObjectParamToShader で使用する優先パラメータの設定 (必要なくなったら NULL を渡すこと)
    void add_SetPriorityParameter( LNPriorityParameter* param_ ) { mPriorityParameter = param_; }

    /// "Draw=Geometry"
    // TODO: pass を直接指定すると、優先シェーダとつじつまが合わなくなる
    void add_DrawGeometry( SceneNode* object_, const SubsetIndexArray* subset_indices_, Graphics::IShaderPass* pass_ );

    /// "Draw=Buffer"
    void add_DrawBuffer( Graphics::IShaderPass* pass_ );

    /// "Draw=Geometry" シェーダなし (drawSubset を呼ぶ回数も 1 度だけ)
    void add_DrawBufferNoShader( SceneNode* object_ );

    /// 画面全体に描画 (RTコピー用)
    void add_DrawScreen( Graphics::ITexture* texture_, const LVector2& view_size_ );

    /// DrawingContext::draw()
    void add_DrawContext( DrawingContext* context_ );

    /// デバッグ描画
    void add_DrawDebug( SceneNode* object_, lnU32 flags_ );

private:

    /// イテレータの位置から実行する (ループネストのための分離)
    void _execute( DrawCommandListIterator& itr_, u32 loop_cnt_ );

private:

    typedef std::stack< Graphics::ITexture* >   RenderTargetStack;
    typedef std::stack< Graphics::ITexture* >   DepthBufferStack;
    typedef std::stack< Geometry::Rect* >		ViewportStack;
    typedef std::stack< Geometry::Rect* >		ScissorStack;

private:

    Graphics::IGraphicsDevice*  mGraphicsDevice;
    Graphics::IRenderer*        mRenderer;
    Graphics::GeometryRenderer* mGeometryRenderer;
    SceneShaderManager*         mSceneShaderManager;
    SceneGraphContext*          mSceneGraphContext;

    DrawCommandList             mDrawCommandList;
    RenderTargetStack           mRenderTargetStack[ LN_MAX_MULTI_RENDERTARGETS ];
    DepthBufferStack            mDepthBufferStack;
    ViewportStack               mViewportStack;
    ScissorStack                mScissorStack;

    LNPriorityParameter*        mPriorityParameter;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================