//=============================================================================
//【 DrawCommand 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Graphics/Common/GeometryRenderer.h"
#include "SceneNode/SceneNode.h"
#include "SceneNode/Camera.h"
#include "SceneNode/Sprite.h"
#include "SceneShader/SceneShaderManager.h"
#include "SceneShader/SceneShader.h"
#include "SceneGraph.h"
#include "DrawCommand.h"

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
// ■ DrawCommandContainer クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    DrawCommandContainer::DrawCommandContainer()
        : mGraphicsDevice       ( NULL )
        , mRenderer             ( NULL )
        , mGeometryRenderer     ( NULL )
        , mPriorityParameter    ( NULL )
        , mSceneGraphContext    ( NULL )
    {
        // とりあえず適当に確保
        mDrawCommandList.resize( 512 );
        mDrawCommandList.clear();
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    void DrawCommandContainer::initialize(
        Graphics::IGraphicsDevice* device_,
        Graphics::GeometryRenderer* gr_,
        SceneShaderManager* sm_,
        SceneGraphContext* scene_context_ )
    {
        mGraphicsDevice = device_;
        mRenderer = mGraphicsDevice->getRenderer();
        mGeometryRenderer = gr_;
        mSceneShaderManager = sm_;
        mSceneGraphContext = scene_context_;
    }

    //---------------------------------------------------------------------
    // ● コマンド作成、実行の前準備
    //---------------------------------------------------------------------
    void DrawCommandContainer::prepare()
    {
        // レンダリングターゲット
        for ( u32 i = 0; i < LN_MAX_MULTI_RENDERTARGETS; ++i )
        {
            while ( !mRenderTargetStack[ i ].empty() )
            {
                mRenderTargetStack[ i ].pop();
            }
            mRenderTargetStack[ i ].push( mRenderer->getRenderTarget( i ) );
        }

        // 深度バッファ
        while ( !mDepthBufferStack.empty() )
        {
            mDepthBufferStack.pop();
        }
        mDepthBufferStack.push( mRenderer->getDepthBuffer() );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_SetActivePaneContext( PaneContext* pane_context_, bool is_3d_ )
    {
        DrawCommand command;
        command.Type                            = LN_DRAWCOM_SET_ACTIVE_PANE_CONTEXT;
        command.SetActivePaneContext.Context    = pane_context_;
        command.SetActivePaneContext.Is3D       = is_3d_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● レンダーターゲットの変更
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_SetRT( u32 index_, Graphics::ITexture* rt_, bool push_stack_ )
    {
        DrawCommand command;
        command.Type            = LN_DRAWCOM_SET_RT;
        command.SetRT.Texture   = rt_;
        command.SetRT.Index     = index_;
        command.SetRT.PushStack = push_stack_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● 深度バッファを変更する
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_SetDepth( Graphics::ITexture* db_, bool push_stack_ )
    {
        DrawCommand command;
        command.Type                = LN_DRAWCOM_SET_DEPTH;
        command.SetDepth.Texture    = db_;
        command.SetDepth.Index      = 0;
        command.SetDepth.PushStack  = push_stack_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● カラーターゲットをクリアする
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_ClearColor( lnU32 color_ )
    {
        DrawCommand command;
        command.Type             = LN_DRAWCOM_CLEAR_COLOR;
        command.ClearColor.Color = color_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● 深度ターゲットをクリアする
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_ClearDepth( lnfloat depth_ )
    {
        DrawCommand command;
        command.Type             = LN_DRAWCOM_CLEAR_DEPTH;
        command.ClearDepth.Depth = depth_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● ビューポート領域を変更する
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_SetViewport( Geometry::Rect* rect_ )
    {
        DrawCommand command;
        command.Type             = LN_DRAWCOM_SET_VIEWPORT;
        command.SetViewport.Area = rect_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● ビューポート領域を元に戻す
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_RemoveViewport()
    {
        DrawCommand command;
        command.Type = LN_DRAWCOM_REMOVE_VIEWPORT;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● シザー領域を変更する
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_SetScissor( Geometry::Rect* rect_ )
    {
        DrawCommand command;
        command.Type            = LN_DRAWCOM_SET_SCISSOR;
        command.SetScissor.Area = rect_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● シザー領域を元に戻す
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_RemoveScissor()
    {
        DrawCommand command;
        command.Type = LN_DRAWCOM_REMOVE_SCISSOR;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● ループ開始
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_LoopBegin( lnU32 count_ )
    {
        DrawCommand command;
        command.Type            = LN_DRAWCOM_LOOPBEGIN;
        command.LoopBegin.Count = count_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● ループ終了
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_LoopEnd()
    {
        DrawCommand command;
        command.Type            = LN_DRAWCOM_LOOPEND;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● ループ回数設定
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_SetLoopCount( Graphics::IShaderVariable* var_ )
    {
        DrawCommand command;
        command.Type                = LN_DRAWCOM_SET_LOOPIDX;
        command.SetLoopIdx.Variable = var_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● ポストエフェクトの "ScriptExternal=Color" 開始
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_PreExternalBegin()
    {
        DrawCommand command;
        command.Type              = LN_DRAWCOM_PRE_EXTERNAL_BEGIN;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● ポストエフェクトの "ScriptExternal=Color" までのブロックの終了
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_PreExternalEnd()
    {
        DrawCommand command;
        command.Type              = LN_DRAWCOM_PRE_EXTERNAL_END;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● ポストエフェクトの "ScriptExternal=Color" から終端までのブロックの開始
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_PostExternalBegin()
    {
        DrawCommand command;
        command.Type              = LN_DRAWCOM_POST_EXTERNAL_BEGIN;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● ポストエフェクトの "ScriptExternal=Color" から終端までのブロックの終了
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_PostExternalEnd()
    {
        DrawCommand command;
        command.Type              = LN_DRAWCOM_POST_EXTERNAL_END;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● カメラ単位の描画パラメータをシェーダに設定する
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_SetCameraParamToShader( Camera* camera_ )
    {
        DrawCommand command;
        command.Type                          = LN_DRAWCOM_SET_CAMERA_PARAM;
        command.SetCameraParamToShader.Camera = camera_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● オブジェクトパラメータの設定
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_SetObjectParamToShader( SceneNodeContext* context_ )
    {
        DrawCommand command;
        command.Type    = LN_DRAWCOM_SET_OBJ_PARAM;
        command.SetObjParam.Context  = context_;
        if ( mPriorityParameter )
        {
            command.SetObjParam.Shader  = mPriorityParameter->Shader;
        }
        else
        {
            command.SetObjParam.Shader  = context_->getShader();
        }
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● "Draw=Geometry"
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_DrawGeometry( SceneNode* object_, const SubsetIndexArray* subset_indices_, Graphics::IShaderPass* pass_ )
    {
        DrawCommand command;
        command.Type                 = LN_DRAWCOM_DRAW_GEOMETRY;
        command.DrawGeometry.Object  = object_;
        command.DrawGeometry.Indices = subset_indices_;
        command.DrawGeometry.Pass    = pass_;
        command.DrawGeometry.PriorityParameter = mPriorityParameter;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● "Draw=Buffer"
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_DrawBuffer( Graphics::IShaderPass* pass_ )
    {
        DrawCommand command;
        command.Type            = LN_DRAWCOM_DRAW_BUFFER;
        command.DrawBuffer.Pass = pass_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_DrawBufferNoShader( SceneNode* object_ )
    {
        DrawCommand command;
        command.Type                 = LN_DRAWCOM_DRAW_GEOMETRY_NO_SHADER;
        command.DrawGeometry.Object  = object_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_DrawScreen( Graphics::ITexture* texture_, const LVector2& view_size_ )
    {
        DrawCommand command;
        command.Type                 = LN_DRAWCOM_DRAW_SCREEN;
        command.DrawScreen.Texture   = texture_;
        command.DrawScreen.ViewSizeW = view_size_.x;
        command.DrawScreen.ViewSizeH = view_size_.y;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_DrawContext( DrawingContext* context_ )
    {
        DrawCommand command;
        command.Type                 = LN_DRAWCOM_DRAW_CONTEXT;
        command.DrawContext.Context  = context_;
        LN_SAFE_ADDREF( context_ );
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● デバッグ描画
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_DrawDebug( SceneNode* object_, lnU32 flags_ )
    {
        DrawCommand command;
        command.Type             = LN_DRAWCOM_DRAW_DEBUG;
        command.DrawDebug.Object = object_;
        command.DrawDebug.Flags  = flags_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // ● 描画コマンドの実行
    //---------------------------------------------------------------------
    LNRESULT DrawCommandContainer::execute()
    {
        prepare();
        //dump();
        //printf( "-----------------------\n" );

        // Android (g++) では _execute( mDrawCommandList.begin(), 1 ); と書くと
        // 「no matching function for call to」とエラーになる
        DrawCommandListIterator itr = mDrawCommandList.begin();
        _execute( itr, 1 );

        // ↓RT、Depth のスタックチェック

        //bool f = false;
        //for ( u32 i = 0; i < LN_MAX_MULTI_RENDERTARGETS; ++i )
        //{
        //    f |= ( !mRenderTargetStack[ i ].empty() );
        //}
        //f |= !mDepthBufferStack.empty();
        //if ( f )
        //{
        //    LN_THROW_InvalidCall(
        //         ( !f ),
        //        "< DrawCommandContainer::execute() >\n\n"
        //        "描画コマンド実行前後のレンダリングターゲットまたは\n"
        //        "深度バッファターゲットのスタックが一致しませんでした。\n" );
        //}
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 描画コマンドをコンソールに出力する (デバッグ用)
    //---------------------------------------------------------------------
    void DrawCommandContainer::dump()
    {

        DrawCommandList::iterator itr = mDrawCommandList.begin();
        DrawCommandList::iterator end = mDrawCommandList.end();
        for ( ; itr != end; ++itr )
        {
            switch ( itr->Type )
            {
                case LN_DRAWCOM_SET_RT:                 
                	printf( "[ LN_DRAWCOM_SET_RT ]          Index=%u RenderTarget=%p\n", itr->SetRT.Index, itr->SetRT.Texture ); break;
                case LN_DRAWCOM_SET_DEPTH:              
                	printf( "[ LN_DRAWCOM_SET_DEPTH ]       DepthTarget=%p\n", itr->SetDepth.Texture ); break;
                case LN_DRAWCOM_CLEAR_COLOR:            
                	printf( "[ LN_DRAWCOM_CLEAR_COLOR ]     Color=%x\n", itr->ClearColor.Color ); break;
                case LN_DRAWCOM_CLEAR_DEPTH:            
                	printf( "[ LN_DRAWCOM_CLEAR_DEPTH ]     Depth=%f\n", itr->ClearDepth.Depth ); break;

                case LN_DRAWCOM_SET_VIEWPORT:           
                	printf( "[ LN_DRAWCOM_SET_VIEWPORT ]    ViewportRect=%p\n", itr->SetViewport.Area ); break;
                case LN_DRAWCOM_REMOVE_VIEWPORT:        
                	printf( "[ LN_DRAWCOM_REMOVE_VIEWPORT ]\n" ); break;

                case LN_DRAWCOM_LOOPBEGIN:              
                	printf( "[ LN_DRAWCOM_LOOPBEGIN ]\n" ); break;
                case LN_DRAWCOM_LOOPEND:                
                	printf( "[ LN_DRAWCOM_LOOPEND ]\n" ); break;
                case LN_DRAWCOM_SET_LOOPIDX:            
                	printf( "[ LN_DRAWCOM_SET_LOOPIDX ]\n" ); break;

                case LN_DRAWCOM_PRE_EXTERNAL_BEGIN:     
                	printf( "[ LN_DRAWCOM_PRE_EXTERNAL_BEGIN ]\n" ); break;
                case LN_DRAWCOM_PRE_EXTERNAL_END:       
                	printf( "[ LN_DRAWCOM_PRE_EXTERNAL_END ]\n" ); break;
                case LN_DRAWCOM_POST_EXTERNAL_BEGIN:    
                	printf( "[ LN_DRAWCOM_POST_EXTERNAL_BEGIN ]\n" ); break;
                case LN_DRAWCOM_POST_EXTERNAL_END:      
                	printf( "[ LN_DRAWCOM_POST_EXTERNAL_END ]\n" ); break;

                case LN_DRAWCOM_SET_CAMERA_PARAM:       
                	printf( "[ LN_DRAWCOM_SET_CAMERA_PARAM ]\n" ); break;
                case LN_DRAWCOM_SET_OBJ_PARAM:          
                	printf( "[ LN_DRAWCOM_SET_OBJ_PARAM ]\n" ); break;
                case LN_DRAWCOM_DRAW_GEOMETRY:          
                	printf( "[ LN_DRAWCOM_DRAW_GEOMETRY ]   Obj=%p\n", itr->DrawGeometry.Object ); break;
                case LN_DRAWCOM_DRAW_BUFFER:            
                	printf( "[ LN_DRAWCOM_DRAW_BUFFER ]\n" ); break;
             
            }
        }
    }


    //---------------------------------------------------------------------
    // ● イテレータの位置から実行する (ループネストのための分離)
    //---------------------------------------------------------------------
    void DrawCommandContainer::_execute( DrawCommandListIterator& itr_, u32 loop_cnt_ )
    {
        DrawCommandList::iterator end = mDrawCommandList.end();
        DrawCommand* cmd;
        u32 loop_cnt = 0;
        DrawCommandList::iterator loop_entry = itr_;    // ループの場合、LOOP_BEGIN の次を指す

        for ( ; itr_ != end; ++itr_ )
        {
            cmd = &(*itr_);

            //_p(cmd->Type);

            switch ( cmd->Type )
            {
                ///////////////////////////////// Pane セット
                case LN_DRAWCOM_SET_ACTIVE_PANE_CONTEXT:
                {
                    mSceneGraphContext->OnSetActivePaneContext(
                        cmd->SetActivePaneContext.Context,
                        cmd->SetActivePaneContext.Is3D );
                    break;
                    
                }
                ///////////////////////////////// レンダーターゲットを変更する
                case LN_DRAWCOM_SET_RT:
                {
                    u32 index = cmd->SetRT.Index;

                    if (cmd->SetRT.PushStack)
                    {
                        if ( cmd->SetRT.Texture )
                        {
                            mRenderer->setRenderTarget( index, cmd->SetRT.Texture );
                            mRenderTargetStack[ index ].push( cmd->SetRT.Texture );
                        }
                        else
                        {
                            mRenderTargetStack[ index ].pop();
                            mRenderer->setRenderTarget( index, mRenderTargetStack[ index ].top() );
                        }
                    }
                    else
                    {
                        // 設定するべきターゲットがある場合はそのままセット
                        if ( cmd->SetRT.Texture )
                        {
                            mRenderer->setRenderTarget( index, cmd->SetRT.Texture );
                        }
                        else
                        {
                            // スタックが空ならデフォルトの RT に
                            if ( mRenderTargetStack[ index ].empty() )
                            {
                                mRenderer->setRenderTarget( index, NULL );
                            }
                            else
                            {
                                mRenderer->setRenderTarget( index, mRenderTargetStack[ index ].top() );
                            }
                        }
                    }
                    break;
                }
                ///////////////////////////////// 深度バッファを変更する
                case LN_DRAWCOM_SET_DEPTH:
                {
                    if (cmd->SetDepth.PushStack)
                    {
                        if ( cmd->SetDepth.Texture )
                        {
                            mRenderer->setDepthBuffer( cmd->SetDepth.Texture );
                            mDepthBufferStack.push( cmd->SetDepth.Texture );
                        }
                        else
                        {
                            mDepthBufferStack.pop();
                            mRenderer->setDepthBuffer( mDepthBufferStack.top() );
                        }
                    }
                    else
                    {
                        if ( cmd->SetDepth.Texture )
                        {
                            mRenderer->setDepthBuffer( cmd->SetDepth.Texture );
                        }
                        else
                        {
                            if ( mDepthBufferStack.empty() )
                            {
                                mRenderer->setDepthBuffer( NULL );
                            }
                            else
                            {
                                mRenderer->setDepthBuffer( mDepthBufferStack.top() );
                            }
                        }
                    }
                    break;
                }
                ///////////////////////////////// 
                case LN_DRAWCOM_CLEAR_COLOR:
                {
                    mRenderer->clear( true, false, cmd->ClearColor.Color, 1.0f );
                    break;
                }
                ///////////////////////////////// 
                case LN_DRAWCOM_CLEAR_DEPTH:
                {
                    mRenderer->clear( false, true, 0, cmd->ClearDepth.Depth );
                    break;
                }
                ///////////////////////////////// ビューポートの設定
                case LN_DRAWCOM_SET_VIEWPORT:
                {
                    mViewportStack.push( cmd->SetViewport.Area );
                    mRenderer->setViewport( *cmd->SetViewport.Area );
                    break;
                }
                ///////////////////////////////// ビューポートの復帰
                case LN_DRAWCOM_REMOVE_VIEWPORT:
                {
                    if ( mViewportStack.size() >= 2 )
                    {
                        mViewportStack.pop();
                        mRenderer->setViewport( *mViewportStack.top() );
                    }
                    else if ( mViewportStack.size() == 1 )
                    {
                        const LVector2& size = mRenderer->getRenderTarget( 0 )->getRealSize();
                        Geometry::Rect rc( 0, 0, static_cast< int >( size.x ), static_cast< int >( size.y ) );
                        mRenderer->setViewport( rc );
                    }
                    break;
                }
                ///////////////////////////////// シザー領域の設定
                case LN_DRAWCOM_SET_SCISSOR:
                {
                    mScissorStack.push( cmd->SetScissor.Area );
                    mRenderer->setScissor( *cmd->SetScissor.Area );
                    break;
                }
                ///////////////////////////////// シザー領域の復帰
                case LN_DRAWCOM_REMOVE_SCISSOR:
                {
                    if ( mScissorStack.size() >= 2 )
                    {
                        mScissorStack.pop();
                        mRenderer->setScissor( *mScissorStack.top() );
                    }
                    else if ( mScissorStack.size() == 1 )
                    {
                        const LVector2& size = mRenderer->getRenderTarget( 0 )->getRealSize();
                        Geometry::Rect rc( 0, 0, static_cast< int >( size.x ), static_cast< int >( size.y ) );
                        mRenderer->setScissor( rc );
                    }
                    break;
                }
                ///////////////////////////////// ループ開始
                case LN_DRAWCOM_LOOPBEGIN:
                {
                    if ( cmd->LoopBegin.Count > 0 )
                    {
                        // イテレータはこの時点で LOOPBEGIN を指しているので、
                        // ひとつ次のものを指してから再起呼び出し
                        ++itr_;
                        _execute( itr_, cmd->LoopBegin.Count );

                        // _execute() から戻ってきた時は LOOPEND を指している。
                        // このまま for の ++itr_ が実行されて次の命令に移るのでこのままで OK
                    }
                    break;
                }
                ///////////////////////////////// ループ終了
                case LN_DRAWCOM_LOOPEND:
                {
                    ++loop_cnt;
                    if ( loop_cnt >= loop_cnt_ )
                    {
                        return;
                    }
                    itr_ = loop_entry;
                    continue;

                    break;
                }
                ///////////////////////////////// ループ回数をシェーダ変数に設定する
                case LN_DRAWCOM_SET_LOOPIDX:
                {
                    cmd->SetLoopIdx.Variable->setInt( loop_cnt );
                    break;
                }
                ///////////////////////////////// 
                case LN_DRAWCOM_PRE_EXTERNAL_BEGIN:
                case LN_DRAWCOM_PRE_EXTERNAL_END:
                {
                    // 現在のターゲットと深度バッファをスタックに積む
                    for ( u32 i = 0; i < LN_MAX_MULTI_RENDERTARGETS; ++i )
                    {
                        mRenderTargetStack[ i ].push( mRenderer->getRenderTarget( i ) );
                    }
                    mDepthBufferStack.push( mRenderer->getDepthBuffer() );

                    break;
                }

                /////////////////////////////////
                case LN_DRAWCOM_POST_EXTERNAL_BEGIN:
                case LN_DRAWCOM_POST_EXTERNAL_END:
                {
                    // スタックの先頭にあるものを設定して、スタックから取り除く
                    for ( u32 i = 0; i < LN_MAX_MULTI_RENDERTARGETS; ++i )
                    {
                        mRenderer->setRenderTarget( i, mRenderTargetStack[ i ].top() );
                        mRenderTargetStack[ i ].pop();
                    }
                    mRenderer->setDepthBuffer( mDepthBufferStack.top() );
                    mDepthBufferStack.pop();

                    // 実際のところ「スタックの先頭にあるものを設定」は保険みたいなもの。
                    // 普通はスタックから取り出した後、正しいものを自分で設定する。

                    break;
                }

                ///////////////////////////////// 
                case LN_DRAWCOM_SET_CAMERA_PARAM:
                {
                    mSceneShaderManager->updateAllShaderCameraParam( *cmd->SetCameraParamToShader.Camera->getCameraSceneParam() );
                    break;
                }

                ///////////////////////////////// 
                case LN_DRAWCOM_SET_OBJ_PARAM:
                {
                    // この辺で止まる場合、updateContext がちゃんと呼ばれているかチェックすること。
                    // SceneGraph::update() → SceheGraph::updateAllContext() の順で呼ばないと、
                    // 追加リストに入っているノードの updateContext() が呼ばれない。

                    cmd->SetObjParam.Shader->updateNodeParam( cmd->SetObjParam.Context );
                    cmd->SetObjParam.Context->onSetNodeParameter( cmd->SetObjParam.Shader );
                    break;
                }

                ///////////////////////////////// "Draw=Geometry"
                case LN_DRAWCOM_DRAW_GEOMETRY:
                {
                    const SubsetIndexArray& indices = (*cmd->DrawGeometry.Indices);
                    lnU32 subset_num = cmd->DrawGeometry.Object->getSubsetNum();
                    //NodeRenderParam* param = cmd->DrawGeometry.Object->getNodeRenderParam();
                    SceneNodeContext* context = cmd->DrawGeometry.Object->getContext();

                    SceneShader* shader;
                    if ( cmd->DrawGeometry.PriorityParameter )
                    {
                        shader = cmd->DrawGeometry.PriorityParameter->Shader;
                    }
                    else
                    {
                        shader = cmd->DrawGeometry.Object->getShader();
                    }

                    // レンダーステートの設定
                    mRenderer->setRenderState( context->getRenderState() );
					
                    // サブセット範囲の指定がある
                    if ( !indices.empty() )
                    {
                        lnU32 idx;
                        lnU32 size = indices.size();
                        for ( lnU32 i = 0; i < size; ++i )
                        {
                            idx = indices[ i ];
                            if ( idx < subset_num )
                            {
                                shader->updateSubsetParam( context->getSubsetRenderParam( idx ) );
                                cmd->DrawGeometry.Pass->begin();
                                cmd->DrawGeometry.Object->drawSubset( i );
                                cmd->DrawGeometry.Pass->end();
                            }
                            else if ( idx == 0xffffffff )
                            {
                                for ( lnU32 j = indices[ i - 1 ] + 1; j < subset_num; ++j )
                                {
                                    shader->updateSubsetParam( context->getSubsetRenderParam( idx ) );
                                    cmd->DrawGeometry.Pass->begin();
                                    cmd->DrawGeometry.Object->drawSubset( i );
                                    cmd->DrawGeometry.Pass->end();
                                }
                            }
                        }
                    }
                    // サブセットの指定がなければすべて描画する
                    else
                    {
                        for ( u32 i = 0; i < subset_num; ++i )
                        {
                            /// Subset 単位で必要なパラメータを設定する
                            shader->updateSubsetParam(
                                context->getSubsetRenderParam( i ) );

                            // 描画
                            cmd->DrawGeometry.Pass->begin();
                            cmd->DrawGeometry.Object->drawSubset( i );
                            cmd->DrawGeometry.Pass->end();
                        }
                    }

                    break;
                }
                ///////////////////////////////// "Draw=Buffer"
                case LN_DRAWCOM_DRAW_BUFFER:
                {
                    cmd->DrawBuffer.Pass->begin();

                    LRenderState state;
                    state.DepthTest = false;
                    state.DepthWrite = false;
                    mRenderer->setRenderState( state );

                    mGeometryRenderer->drawSquare(
                        -1.0f,  1.0f, 0xffffffff, 0.0f, 0.0f,   // 左上
                         1.0f,  1.0f, 0xffffffff, 1.0f, 0.0f,   // 右上
                        -1.0f, -1.0f, 0xffffffff, 0.0f, 1.0f,
                         1.0f, -1.0f, 0xffffffff, 1.0f, 1.0f );

                    cmd->DrawBuffer.Pass->end();
                    break;
                }
                ///////////////////////////////// "Draw=Geometry" シェーダなし
                case LN_DRAWCOM_DRAW_GEOMETRY_NO_SHADER:
                {
					mRenderer->setRenderState( cmd->DrawGeometry.Object->getContext()->getRenderState() );
                    cmd->DrawGeometry.Object->drawSubset( 0 );
                    break;   
                }
                ///////////////////////////////// スクリーン全体描画
                case LN_DRAWCOM_DRAW_SCREEN:
                {
                    mRenderer->setRenderState( LRenderState::BUFFER_COPY_STATE );

                    mGeometryRenderer->setTexture( cmd->DrawScreen.Texture );
                    mGeometryRenderer->setMatrix( LMatrix::IDENTITY );
                    mGeometryRenderer->setViewProjMatrix( LMatrix::IDENTITY );
                    mGeometryRenderer->begin();

                    lnfloat ou = 0.5f / cmd->DrawScreen.ViewSizeW;
                    lnfloat ov = 0.5f / cmd->DrawScreen.ViewSizeH;
                    mGeometryRenderer->drawSquare(
                        -1.0f,  1.0f, 0xffffffff, 0.0f + ou, 0.0f + ov,   // 左上
                         1.0f,  1.0f, 0xffffffff, 1.0f + ou, 0.0f + ov,   // 右上
                        -1.0f, -1.0f, 0xffffffff, 0.0f + ou, 1.0f + ov,
                         1.0f, -1.0f, 0xffffffff, 1.0f + ou, 1.0f + ov );

                    mGeometryRenderer->end();
                    
                    break;
                }
                ///////////////////////////////// Context::draw()
                case LN_DRAWCOM_DRAW_CONTEXT:
                {
                    cmd->DrawContext.Context->draw( mRenderer, mGeometryRenderer );
                    LN_SAFE_RELEASE( cmd->DrawContext.Context );
                    break;
                }
                ///////////////////////////////// デバッグ描画
                case LN_DRAWCOM_DRAW_DEBUG:
                {
                    cmd->DrawDebug.Object->drawDebug( cmd->DrawDebug.Flags );
                    break;
                }
            }
        }
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