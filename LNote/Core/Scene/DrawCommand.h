//=============================================================================
//�y DrawCommand �z
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

/// �`��R�}���h�̎��
enum LNDrawCommandType
{
    LN_DRAWCOM_SET_ACTIVE_PANE_CONTEXT = 1,

    LN_DRAWCOM_SET_RT,              ///< �����_�[�^�[�Q�b�g�̕ύX
    LN_DRAWCOM_SET_DEPTH,           ///< �[�x�o�b�t�@��ύX����
    LN_DRAWCOM_CLEAR_COLOR,         ///< �J���[�^�[�Q�b�g���N���A����
    LN_DRAWCOM_CLEAR_DEPTH,         ///< �[�x�^�[�Q�b�g���N���A����

    LN_DRAWCOM_SET_VIEWPORT,        ///< �r���[�|�[�g�̈��ύX����
    LN_DRAWCOM_REMOVE_VIEWPORT,     ///< �r���[�|�[�g�̈�����ɖ߂�
    LN_DRAWCOM_SET_SCISSOR,         ///< �V�U�[�̈��ύX����
    LN_DRAWCOM_REMOVE_SCISSOR,      ///< �V�U�[�̈�����ɖ߂�

    LN_DRAWCOM_LOOPBEGIN,           ///< ���[�v�J�n
    LN_DRAWCOM_LOOPEND,             ///< ���[�v�I��
    LN_DRAWCOM_SET_LOOPIDX,         ///< ���[�v�񐔂��V�F�[�_�ϐ��ɐݒ肷��

    LN_DRAWCOM_PRE_EXTERNAL_BEGIN,
    LN_DRAWCOM_PRE_EXTERNAL_END,
    LN_DRAWCOM_POST_EXTERNAL_BEGIN,
    LN_DRAWCOM_POST_EXTERNAL_END,

    LN_DRAWCOM_SET_CAMERA_PARAM,    ///< �J�����P�ʂ̃p�����[�^���V�F�[�_�ɐݒ�
    LN_DRAWCOM_SET_OBJ_PARAM,       ///< �I�u�W�F�N�g�P�ʂ̃p�����[�^���V�F�[�_�ɐݒ�
    LN_DRAWCOM_DRAW_GEOMETRY,       ///< "Draw=Geometry"
    LN_DRAWCOM_DRAW_BUFFER,         ///< "Draw=Buffer"
    LN_DRAWCOM_DRAW_GEOMETRY_NO_SHADER,       ///< �V�F�[�_���g�p���Ȃ� (�Ƃ肠����)
    LN_DRAWCOM_DRAW_SCREEN,
    LN_DRAWCOM_DRAW_CONTEXT,
    LN_DRAWCOM_DRAW_DEBUG,          ///< �f�o�b�O�`��
};

/// �`��R�}���h
struct DrawCommand
{
    lnU32     Type;       ///< LNDrawCommandType �̒l

    union
    {
        struct
        {
            PaneContext*    Context;
            bool            Is3D;
        } SetActivePaneContext;

        // �����_�[�^�[�Q�b�g�A�[�x�o�b�t�@�̕ύX
        struct
        {
            Graphics::ITexture* Texture;
            lnU32               Index;
            bool                PushStack;
        } SetRT, SetDepth;

        // �J���[�^�[�Q�b�g���N���A����
        struct
        {
            lnU32   Color;  // 32�r�b�g�l
        } ClearColor;

        // �[�x�^�[�Q�b�g���N���A����
        struct
        {
            float   Depth;
        } ClearDepth;

        // �r���[�|�[�g�̈�̕ύX�A�V�U�[�̈�̕ύX
        struct
        {
            Geometry::Rect*   Area;
        } SetViewport, SetScissor;

        // �����_�[�X�e�[�g�̐ݒ�
        struct
        {
            const LRenderState*     RenderState;
        } SetRenderState;

        // ���[�v�J�n
        struct
        {
            lnU32     Count;
        } LoopBegin;

        // ���[�v�񐔐ݒ�
        struct
        {
            Graphics::IShaderVariable*    Variable;
        } SetLoopIdx;

        // �J�����P�ʂ̃p�����[�^���V�F�[�_�ɐݒ�
        struct
        {
            Scene::Camera*              Camera;

        } SetCameraParamToShader;

        // �I�u�W�F�N�g�P�ʂ̃p�����[�^���V�F�[�_�ɐݒ�
        struct
        {
            //NodeRenderParam*        Param;
            //SceneNode*              Object;
            SceneNodeContext*       Context;
            SceneShader*            Shader;     ///< �D��p�����[�^�Ή��̂��߁A����

        } SetObjParam;

        // "Draw=Geometry"
        struct
        {
            SceneNode*              Object;
            const SubsetIndexArray* Indices;
            Graphics::IShaderPass*  Pass;
            LNPriorityParameter*    PriorityParameter;  ///< �I�t�X�N���[��RT����̐ݒ�p (�g��Ȃ��ꍇ��NULL)

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
        
        // �f�o�b�O�`��
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
// �� DrawCommandContainer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �`��R�}���h�̊i�[�E���s�N���X
//*/
//=============================================================================
class DrawCommandContainer
{
public:

    DrawCommandContainer();

public:

    /// ������
    void initialize(
        Graphics::IGraphicsDevice* device_,
        Graphics::GeometryRenderer* gr_,
        SceneShaderManager* sm_,
        SceneGraphContext* scene_context_ );

    /// �R�}���h�쐬�A���s�̑O����
    //      �X�^�b�N����̏�Ԃ� add_SetRT( NULL ) ���ĂԂƋ����I�Ƀo�b�N�o�b�t�@���ݒ肳��Ă��܂����Ƃ�h�����߁A
    //      ���ׂẴX�^�b�N���N���A���A�Ăяo�������_�ł̂��ׂĂ�RT�Ɛ[�x�o�b�t�@���X�^�b�N�ɐςށB
    void prepare();

    /// �`��R�}���h���X�g�̃N���A
    void clear() { mDrawCommandList.clear(); }

    /// �`��R�}���h�̎��s
    LNRESULT execute();

    /// �`��R�}���h���R���\�[���ɏo�͂��� (�f�o�b�O�p)
    void dump();

public:

    /// Pane �̐ݒ� (SceneGraphContext::OnSetActivePaneContext() ���Ă΂��)
    void add_SetActivePaneContext( PaneContext* pane_context_, bool is_3d_ );

    /// �����_�[�^�[�Q�b�g�̕ύX
    // (�X�^�b�N�ɂ͐ς܂Ȃ��BNULL ��n���ƌ��݂̃X�^�b�N�̐擪��RT�� Renderer �ɐݒ肷��B���̎����X�^�b�N�ɕω��͖���)
    //  push_stack_ �� true �̏ꍇ�̓X�^�b�N�ɐςށBpush_stack_ �� true ���� rt �� NULL �̏ꍇ�̓|�b�v����B
    //  �X�^�b�N����� index_ �Ԃŋ�ʂ����X�^�b�N���Ƃɍs����B
    void add_SetRT( u32 index_, Graphics::ITexture* rt_, bool push_stack_ );

    /// �[�x�o�b�t�@��ύX����
    void add_SetDepth( Graphics::ITexture* db_, bool push_stack_ );

    /// �J���[�^�[�Q�b�g���N���A����
    void add_ClearColor( lnU32 color_ );

    /// �[�x�^�[�Q�b�g���N���A����
    void add_ClearDepth( lnfloat depth_ );

    /// �r���[�|�[�g��ύX���� (�Â��r���[�|�[�g�̓X�^�b�N�ɐς܂��)
    void add_SetViewport( Geometry::Rect* rect_ );

    /// �r���[�|�[�g�̈���X�^�b�N������o���čĐݒ肷��
    void add_RemoveViewport();

    /// �V�U�[�̈��ύX���� (�Â��r���[�|�[�g�̓X�^�b�N�ɐς܂��)
    void add_SetScissor( Geometry::Rect* rect_ );

    /// �V�U�[�̈���X�^�b�N������o���čĐݒ肷��
    void add_RemoveScissor();

    /// ���[�v�J�n
    void add_LoopBegin( lnU32 count_ );

    /// ���[�v�I��
    void add_LoopEnd();

    /// ���[�v�񐔐ݒ�
    void add_SetLoopCount( Graphics::IShaderVariable* var_ );

    /// �|�X�g�G�t�F�N�g�� "ScriptExternal=Color" �J�n
    void add_PreExternalBegin();

    /// �|�X�g�G�t�F�N�g�� "ScriptExternal=Color" �܂ł̃u���b�N�̏I��
    void add_PreExternalEnd();

    /// �|�X�g�G�t�F�N�g�� "ScriptExternal=Color" ����I�[�܂ł̃u���b�N�̊J�n
    void add_PostExternalBegin();

    /// �|�X�g�G�t�F�N�g�� "ScriptExternal=Color" ����I�[�܂ł̃u���b�N�̏I��
    void add_PostExternalEnd();

    /// �J�����P�ʂ̕`��p�����[�^���V�F�[�_�ɐݒ肷��  ���폜�\��
    //      �ȑO�� SceneGraph::makeDrawCommand() �ōs���Ă������A
    //      ���ۂɃR�}���h�����s����O�ɕ����� SceneGraph::makeDrawCommand() ���Ă΂��킯�ŁA
    //      �Â��l�͏���������Ă��܂��B����Ȃ킯�ŁA�`��R�}���h�ɑg�ݍ��ށB
    void add_SetCameraParamToShader( Camera* camera_ );

    /// �I�u�W�F�N�g�P�ʂ̕`��p�����[�^���V�F�[�_�ɐݒ肷��(���[���h�s��Ȃ�)
    void add_SetObjectParamToShader( SceneNodeContext* context_ );//SceneNode* node_ );//NodeRenderParam* param_ );

    /// ������� Draw=Geometry�ASetObjectParamToShader �Ŏg�p����D��p�����[�^�̐ݒ� (�K�v�Ȃ��Ȃ����� NULL ��n������)
    void add_SetPriorityParameter( LNPriorityParameter* param_ ) { mPriorityParameter = param_; }

    /// "Draw=Geometry"
    // TODO: pass �𒼐ڎw�肷��ƁA�D��V�F�[�_�Ƃ��܂�����Ȃ��Ȃ�
    void add_DrawGeometry( SceneNode* object_, const SubsetIndexArray* subset_indices_, Graphics::IShaderPass* pass_ );

    /// "Draw=Buffer"
    void add_DrawBuffer( Graphics::IShaderPass* pass_ );

    /// "Draw=Geometry" �V�F�[�_�Ȃ� (drawSubset ���Ăԉ񐔂� 1 �x����)
    void add_DrawBufferNoShader( SceneNode* object_ );

    /// ��ʑS�̂ɕ`�� (RT�R�s�[�p)
    void add_DrawScreen( Graphics::ITexture* texture_, const LVector2& view_size_ );

    /// DrawingContext::draw()
    void add_DrawContext( DrawingContext* context_ );

    /// �f�o�b�O�`��
    void add_DrawDebug( SceneNode* object_, lnU32 flags_ );

private:

    /// �C�e���[�^�̈ʒu������s���� (���[�v�l�X�g�̂��߂̕���)
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