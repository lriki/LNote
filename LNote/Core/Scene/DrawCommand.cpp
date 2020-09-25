//=============================================================================
//�y DrawCommand �z
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
// �� DrawCommandContainer �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    DrawCommandContainer::DrawCommandContainer()
        : mGraphicsDevice       ( NULL )
        , mRenderer             ( NULL )
        , mGeometryRenderer     ( NULL )
        , mPriorityParameter    ( NULL )
        , mSceneGraphContext    ( NULL )
    {
        // �Ƃ肠�����K���Ɋm��
        mDrawCommandList.resize( 512 );
        mDrawCommandList.clear();
    }

    //---------------------------------------------------------------------
    // �� ������
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
    // �� �R�}���h�쐬�A���s�̑O����
    //---------------------------------------------------------------------
    void DrawCommandContainer::prepare()
    {
        // �����_�����O�^�[�Q�b�g
        for ( u32 i = 0; i < LN_MAX_MULTI_RENDERTARGETS; ++i )
        {
            while ( !mRenderTargetStack[ i ].empty() )
            {
                mRenderTargetStack[ i ].pop();
            }
            mRenderTargetStack[ i ].push( mRenderer->getRenderTarget( i ) );
        }

        // �[�x�o�b�t�@
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
    // �� �����_�[�^�[�Q�b�g�̕ύX
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
    // �� �[�x�o�b�t�@��ύX����
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
    // �� �J���[�^�[�Q�b�g���N���A����
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_ClearColor( lnU32 color_ )
    {
        DrawCommand command;
        command.Type             = LN_DRAWCOM_CLEAR_COLOR;
        command.ClearColor.Color = color_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // �� �[�x�^�[�Q�b�g���N���A����
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_ClearDepth( lnfloat depth_ )
    {
        DrawCommand command;
        command.Type             = LN_DRAWCOM_CLEAR_DEPTH;
        command.ClearDepth.Depth = depth_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // �� �r���[�|�[�g�̈��ύX����
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_SetViewport( Geometry::Rect* rect_ )
    {
        DrawCommand command;
        command.Type             = LN_DRAWCOM_SET_VIEWPORT;
        command.SetViewport.Area = rect_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // �� �r���[�|�[�g�̈�����ɖ߂�
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_RemoveViewport()
    {
        DrawCommand command;
        command.Type = LN_DRAWCOM_REMOVE_VIEWPORT;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // �� �V�U�[�̈��ύX����
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_SetScissor( Geometry::Rect* rect_ )
    {
        DrawCommand command;
        command.Type            = LN_DRAWCOM_SET_SCISSOR;
        command.SetScissor.Area = rect_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // �� �V�U�[�̈�����ɖ߂�
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_RemoveScissor()
    {
        DrawCommand command;
        command.Type = LN_DRAWCOM_REMOVE_SCISSOR;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // �� ���[�v�J�n
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_LoopBegin( lnU32 count_ )
    {
        DrawCommand command;
        command.Type            = LN_DRAWCOM_LOOPBEGIN;
        command.LoopBegin.Count = count_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // �� ���[�v�I��
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_LoopEnd()
    {
        DrawCommand command;
        command.Type            = LN_DRAWCOM_LOOPEND;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // �� ���[�v�񐔐ݒ�
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_SetLoopCount( Graphics::IShaderVariable* var_ )
    {
        DrawCommand command;
        command.Type                = LN_DRAWCOM_SET_LOOPIDX;
        command.SetLoopIdx.Variable = var_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // �� �|�X�g�G�t�F�N�g�� "ScriptExternal=Color" �J�n
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_PreExternalBegin()
    {
        DrawCommand command;
        command.Type              = LN_DRAWCOM_PRE_EXTERNAL_BEGIN;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // �� �|�X�g�G�t�F�N�g�� "ScriptExternal=Color" �܂ł̃u���b�N�̏I��
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_PreExternalEnd()
    {
        DrawCommand command;
        command.Type              = LN_DRAWCOM_PRE_EXTERNAL_END;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // �� �|�X�g�G�t�F�N�g�� "ScriptExternal=Color" ����I�[�܂ł̃u���b�N�̊J�n
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_PostExternalBegin()
    {
        DrawCommand command;
        command.Type              = LN_DRAWCOM_POST_EXTERNAL_BEGIN;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // �� �|�X�g�G�t�F�N�g�� "ScriptExternal=Color" ����I�[�܂ł̃u���b�N�̏I��
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_PostExternalEnd()
    {
        DrawCommand command;
        command.Type              = LN_DRAWCOM_POST_EXTERNAL_END;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // �� �J�����P�ʂ̕`��p�����[�^���V�F�[�_�ɐݒ肷��
    //---------------------------------------------------------------------
    void DrawCommandContainer::add_SetCameraParamToShader( Camera* camera_ )
    {
        DrawCommand command;
        command.Type                          = LN_DRAWCOM_SET_CAMERA_PARAM;
        command.SetCameraParamToShader.Camera = camera_;
        mDrawCommandList.push_back( command );
    }

    //---------------------------------------------------------------------
    // �� �I�u�W�F�N�g�p�����[�^�̐ݒ�
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
    // �� "Draw=Geometry"
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
    // �� "Draw=Buffer"
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
    // �� �f�o�b�O�`��
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
    // �� �`��R�}���h�̎��s
    //---------------------------------------------------------------------
    LNRESULT DrawCommandContainer::execute()
    {
        prepare();
        //dump();
        //printf( "-----------------------\n" );

        // Android (g++) �ł� _execute( mDrawCommandList.begin(), 1 ); �Ə�����
        // �uno matching function for call to�v�ƃG���[�ɂȂ�
        DrawCommandListIterator itr = mDrawCommandList.begin();
        _execute( itr, 1 );

        // ��RT�ADepth �̃X�^�b�N�`�F�b�N

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
        //        "�`��R�}���h���s�O��̃����_�����O�^�[�Q�b�g�܂���\n"
        //        "�[�x�o�b�t�@�^�[�Q�b�g�̃X�^�b�N����v���܂���ł����B\n" );
        //}
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �`��R�}���h���R���\�[���ɏo�͂��� (�f�o�b�O�p)
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
    // �� �C�e���[�^�̈ʒu������s���� (���[�v�l�X�g�̂��߂̕���)
    //---------------------------------------------------------------------
    void DrawCommandContainer::_execute( DrawCommandListIterator& itr_, u32 loop_cnt_ )
    {
        DrawCommandList::iterator end = mDrawCommandList.end();
        DrawCommand* cmd;
        u32 loop_cnt = 0;
        DrawCommandList::iterator loop_entry = itr_;    // ���[�v�̏ꍇ�ALOOP_BEGIN �̎����w��

        for ( ; itr_ != end; ++itr_ )
        {
            cmd = &(*itr_);

            //_p(cmd->Type);

            switch ( cmd->Type )
            {
                ///////////////////////////////// Pane �Z�b�g
                case LN_DRAWCOM_SET_ACTIVE_PANE_CONTEXT:
                {
                    mSceneGraphContext->OnSetActivePaneContext(
                        cmd->SetActivePaneContext.Context,
                        cmd->SetActivePaneContext.Is3D );
                    break;
                    
                }
                ///////////////////////////////// �����_�[�^�[�Q�b�g��ύX����
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
                        // �ݒ肷��ׂ��^�[�Q�b�g������ꍇ�͂��̂܂܃Z�b�g
                        if ( cmd->SetRT.Texture )
                        {
                            mRenderer->setRenderTarget( index, cmd->SetRT.Texture );
                        }
                        else
                        {
                            // �X�^�b�N����Ȃ�f�t�H���g�� RT ��
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
                ///////////////////////////////// �[�x�o�b�t�@��ύX����
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
                ///////////////////////////////// �r���[�|�[�g�̐ݒ�
                case LN_DRAWCOM_SET_VIEWPORT:
                {
                    mViewportStack.push( cmd->SetViewport.Area );
                    mRenderer->setViewport( *cmd->SetViewport.Area );
                    break;
                }
                ///////////////////////////////// �r���[�|�[�g�̕��A
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
                ///////////////////////////////// �V�U�[�̈�̐ݒ�
                case LN_DRAWCOM_SET_SCISSOR:
                {
                    mScissorStack.push( cmd->SetScissor.Area );
                    mRenderer->setScissor( *cmd->SetScissor.Area );
                    break;
                }
                ///////////////////////////////// �V�U�[�̈�̕��A
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
                ///////////////////////////////// ���[�v�J�n
                case LN_DRAWCOM_LOOPBEGIN:
                {
                    if ( cmd->LoopBegin.Count > 0 )
                    {
                        // �C�e���[�^�͂��̎��_�� LOOPBEGIN ���w���Ă���̂ŁA
                        // �ЂƂ��̂��̂��w���Ă���ċN�Ăяo��
                        ++itr_;
                        _execute( itr_, cmd->LoopBegin.Count );

                        // _execute() ����߂��Ă������� LOOPEND ���w���Ă���B
                        // ���̂܂� for �� ++itr_ �����s����Ď��̖��߂Ɉڂ�̂ł��̂܂܂� OK
                    }
                    break;
                }
                ///////////////////////////////// ���[�v�I��
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
                ///////////////////////////////// ���[�v�񐔂��V�F�[�_�ϐ��ɐݒ肷��
                case LN_DRAWCOM_SET_LOOPIDX:
                {
                    cmd->SetLoopIdx.Variable->setInt( loop_cnt );
                    break;
                }
                ///////////////////////////////// 
                case LN_DRAWCOM_PRE_EXTERNAL_BEGIN:
                case LN_DRAWCOM_PRE_EXTERNAL_END:
                {
                    // ���݂̃^�[�Q�b�g�Ɛ[�x�o�b�t�@���X�^�b�N�ɐς�
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
                    // �X�^�b�N�̐擪�ɂ�����̂�ݒ肵�āA�X�^�b�N�����菜��
                    for ( u32 i = 0; i < LN_MAX_MULTI_RENDERTARGETS; ++i )
                    {
                        mRenderer->setRenderTarget( i, mRenderTargetStack[ i ].top() );
                        mRenderTargetStack[ i ].pop();
                    }
                    mRenderer->setDepthBuffer( mDepthBufferStack.top() );
                    mDepthBufferStack.pop();

                    // ���ۂ̂Ƃ���u�X�^�b�N�̐擪�ɂ�����̂�ݒ�v�͕ی��݂����Ȃ��́B
                    // ���ʂ̓X�^�b�N������o������A���������̂������Őݒ肷��B

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
                    // ���̕ӂŎ~�܂�ꍇ�AupdateContext �������ƌĂ΂�Ă��邩�`�F�b�N���邱�ƁB
                    // SceneGraph::update() �� SceheGraph::updateAllContext() �̏��ŌĂ΂Ȃ��ƁA
                    // �ǉ����X�g�ɓ����Ă���m�[�h�� updateContext() ���Ă΂�Ȃ��B

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

                    // �����_�[�X�e�[�g�̐ݒ�
                    mRenderer->setRenderState( context->getRenderState() );
					
                    // �T�u�Z�b�g�͈͂̎w�肪����
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
                    // �T�u�Z�b�g�̎w�肪�Ȃ���΂��ׂĕ`�悷��
                    else
                    {
                        for ( u32 i = 0; i < subset_num; ++i )
                        {
                            /// Subset �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
                            shader->updateSubsetParam(
                                context->getSubsetRenderParam( i ) );

                            // �`��
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
                        -1.0f,  1.0f, 0xffffffff, 0.0f, 0.0f,   // ����
                         1.0f,  1.0f, 0xffffffff, 1.0f, 0.0f,   // �E��
                        -1.0f, -1.0f, 0xffffffff, 0.0f, 1.0f,
                         1.0f, -1.0f, 0xffffffff, 1.0f, 1.0f );

                    cmd->DrawBuffer.Pass->end();
                    break;
                }
                ///////////////////////////////// "Draw=Geometry" �V�F�[�_�Ȃ�
                case LN_DRAWCOM_DRAW_GEOMETRY_NO_SHADER:
                {
					mRenderer->setRenderState( cmd->DrawGeometry.Object->getContext()->getRenderState() );
                    cmd->DrawGeometry.Object->drawSubset( 0 );
                    break;   
                }
                ///////////////////////////////// �X�N���[���S�̕`��
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
                        -1.0f,  1.0f, 0xffffffff, 0.0f + ou, 0.0f + ov,   // ����
                         1.0f,  1.0f, 0xffffffff, 1.0f + ou, 0.0f + ov,   // �E��
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
                ///////////////////////////////// �f�o�b�O�`��
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