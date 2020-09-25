//=============================================================================
//�y Common �z
//-----------------------------------------------------------------------------
///**
//  @file       Common.h
//  @brief      Common
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include <list>
#include <map>
#include "../Base/RefString.h"
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
    
    class SceneGraph;
    class SceneNode;
    class Camera;
    class Light;
    class Viewport;


    class Manager;
    class DrawCommandContainer;
    class SceneShaderManager;
    class SceneShader;
    class DummyNode;
    class Camera;
    class Light;
    class EffectBatchProxy;
    class BatchSpriteProxy;
    class PhysicsDebugDrawProxy;
    class Pane;

    class SceneGraphContext;
    class SceneNodeContext;
    class PaneContext;
    class CameraContext;
    class LightContext;


    typedef std::vector< SceneNode* >               SceneNodeArray;
    typedef std::list< SceneNode* >                 SceneNodeList;
    typedef std::map< LRefTString, SceneNode* >     SceneNodeNameMap;
    typedef std::pair< LRefTString, SceneNode* >    SceneNodeNamePair;
    typedef std::vector< lnU32 >                    SubsetIndexArray;
    typedef std::vector< Light* >                   LightArray;
    typedef std::list< Viewport* >                  ViewportList;
    typedef std::vector< Pane* >    PaneArray;
    typedef std::vector< Camera* >  CameraList;
    typedef std::vector< Light* >   LightList;

    typedef std::vector< SceneNodeContext* >    SceneNodeContextArray;
    typedef std::vector< CameraContext* >       CameraContextArray;
    typedef std::vector< LightContext* >        LightContextArray;
    

// (�`��R�}���h�̃T�C�Y��剻�΍�̂��߂ɗp��)
class DrawingContext
    : public Base::ReferenceObject
{
public:

    /// (�����͌�� Graphics::Manager �ɕς��邩���c)
    virtual void draw(
        Graphics::IRenderer* renderer_,
        Graphics::GeometryRenderer* geometry_renderer_ ) = 0;
};

} // namespace Scene
} // namespace Core

//-------------------------------------------------------------

/// �f�t�H���g���C�g�̗D��x
static const int LN_DEFAULT_LIGHT_PRIORITY = 1000;

/// �f�t�H���g�ŃZ�b�g�����m�[�h�̗D��x
enum LNDefaultSceneNodePriority
{
    LN_SCENENODEPRIORITY_EFFECT_BATCH_SPRITE = 1000000,   ///< Effect �p�o�b�`�X�v���C�g (2D/3D)
    LN_SCENENODEPRIORITY_PHYSICS_DEBUG = 1000000,           ///< �������Z�̃f�o�b�O�`��
    LN_SCENENODEPRIORITY_PROXY = 1000000,
};

/// ���W���
enum LNDrawCoordSpace
{
    LN_DRAWCOORD_UNKNOWN = 0,   ///< NULL �m�[�h�A�ėp�m�[�h�p�A�܂��͖��ݒ�
    LN_DRAWCOORD_2D,
    LN_DRAWCOORD_3D
};

/// �m�[�h�̎��
enum LNSceneNodeType
{
	LN_SCENENODE_UNKNOWN = 0,
    //LN_SCENENODE_PANE,
	LN_SCENENODE_VIEWPORT,          ///< �r���[�|�[�g (OffscreenScene::addSceneNode() �œo�^���͂����Ă���)
    LN_SCENENODE_GRID,
    LN_SCENENODE_LIGHT,
    LN_SCENENODE_SPRITE3D,
    LN_SCENENODE_BATCHSPRITE,
    LN_SCENENODE_BATCHSPRITE3D,
    LN_SCENENODE_BATCHSPRITEPROXY,
    LN_SCENENODE_MODEL,
    LN_SCENENODE_PARTICLE,
    LN_SCENENODE_TILEMAP,
    LN_SCENENODE_EFFECT,
    LN_SCENENODE_EFFECTSPRITEPROXY,
    LN_SCENENODE_PHYSICSDEBUGDRAWPROXY,
};



/// ���C�g�̎��
enum LNLightType
{
    LN_LIGHTTYPE_DIRECTIONAL = 1,   ///< �f�B���N�V���i�����C�g
	LN_LIGHTTYPE_POINT,             ///< �|�C���g���C�g
	LN_LIGHTTYPE_SPOT,              ///< �X�|�b�g���C�g
};

/// Z �\�[�g�̂��߂̋����̌v�Z���@
enum LNZSortFunc
{
    LN_ZSORTF_IMM_Z = 0,        ///< Z �l�𒼐ڎg�� (2D �p)
    LN_ZSORTF_CAMERA_DIST,      ///< �J��������̒�������
    LN_ZSORTF_CAMERA_SCREEN,    ///< �J�������猩���X�N���[�����ʂ���̋���
};

/// ���]���@
enum LNFlipFlag
{
    LN_FLIP_NONE    = 0x00,     ///< ���]����
    LN_FLIP_V       = 0x01,     ///< �㉺���]
    LN_FLIP_H       = 0x02,     ///< ���E���]
};

/// �f�o�b�O�`��t���O
enum LNSceneDebugDrawFlag
{
    LN_SCENEDEBUGDRAW_NONE              = 0,
    LN_SCENEDEBUGDRAW_CAMERA            = 0x01,     ///< �J�����̃C���[�W
    LN_SCENEDEBUGDRAW_CAMERA_FRUSTUM    = 0x02,     ///< �J�����̎�����
    LN_SCENEDEBUGDRAW_LIGHT             = 0x04,     ///< ���C�g�̃C���[�W
    LN_SCENEDEBUGDRAW_LIGHT_FRUSTUM     = 0x08,     ///< ���C�g�̎�����

    LN_SCENEDEBUGDRAW_ALL = 0xffffffff,
};

/// �r���[�|�[�g�G�t�F�N�g�̎��
enum LNScreenEffectMode
{
    LN_SCREENEFFECTMODE_NONE = 0,         ///< ������ʂ��g�p���Ȃ�
    LN_SCREENEFFECTMODE_BUFFERINGEFFECT,  ///< �u���\�A�g�����W�V�������T�|�[�g����
};

/// �^�C���}�b�v�쐬���̌���
enum LNTilemapDirection
{
    LN_TILEMAPDIR_XYR = 0,     ///< XY ���ʂɑ΂��� Z+ �����ɍ쐬���� (2D �̏ꍇ�͂�����w�肷��)
    //LN_TILEMAPDIR_XYM          ///< XY ���ʂ̑� 4 �ی��ɁAZ- �����ɍ쐬���� (Z- �������猩���Ƃ��� LN_TILEMAPDIR_XYR �Ɠ����z�u�ɂȂ� http://www.suriken.com/knowledge/glossary/fourth-quadrant.html)

    LN_TILEMAPDIR_XY            ///< XY ���ʂɑ΂��āA�}�b�v�̍��������_�ɂȂ�悤�ɍ쐬���� (���ی��ɍ쐬����)
};

//-------------------------------------------------------------

/// �f�t�H���g�ō쐬�����V�F�[�_�v���O�����̔ԍ�
enum LNDefaultSceneShader
{
    LN_DEFSHADER_BASIC_2D = 0,
    LN_DEFSHADER_BASIC_3D,

    LN_DEFSHADER_NOLIGHTING,

	//LN_DEFSHADER_DEBUGSHAPES_3D = 0,

    //
 //   LN_DEFSHADER_PARTICLE_3D,

    LN_MAX_DEFAULT_SHADERS
};

/// �f�t�H���g�ō쐬����� SceneNode
enum LNDefaultSceneNode
{
    LN_DEFAULT_SCENENODE_ROOT = 0,  ///< ���[�g�m�[�h
    LN_DEFAULT_SCENENODE_3D,        ///< 3D ��Ԃ̃��[�g
    LN_DEFAULT_SCENENODE_2D,        ///< 2D ��Ԃ̃��[�g
    LN_MAX_DEFAULT_NODES
};

//-------------------------------------------------------------

/// �J�����P�ʂ̐ݒ荀�� (1�t���[����1�x�����ݒ肷��΂����p�����[�^�BCamera::getCameraSceneParam() �Ŏ擾)
struct LNCameraSceneParam
{
    const LMatrix*   Mat_CameraView;
    const LMatrix*   Mat_CameraProj;
    const LMatrix*   Mat_CameraViewProj;    ///< �J�����̃r���[�ƃv���W�F�N�V�����s��̐�

    LVector4	     CameraPosition;	    ///< �J�����̈ʒu
    LVector4	     CameraDirection;	    ///< �J�����̌���

    // �ȉ��� calcMatrices() �Őݒ肷��
    LMatrix  Mat_CameraView_Inverse;
    LMatrix  Mat_CameraProj_Inverse;
    LMatrix  Mat_CameraViewProj_Inverse;
    LMatrix  Mat_CameraView_Transpose;
    LMatrix  Mat_CameraProj_Transpose;
    LMatrix  Mat_CameraViewProj_Transpose;
    LMatrix  Mat_CameraView_InverseTranspose;
    LMatrix  Mat_CameraProj_InverseTranspose;
    LMatrix  Mat_CameraViewProj_InverseTranspose;

    void calcMatrices()
    {
        LMatrix::inverse( &Mat_CameraView_Inverse, *Mat_CameraView );
        LMatrix::inverse( &Mat_CameraProj_Inverse, *Mat_CameraProj );
        LMatrix::inverse( &Mat_CameraViewProj_Inverse, *Mat_CameraViewProj );
        LMatrix::transpose( &Mat_CameraView_Transpose, *Mat_CameraView );
        LMatrix::transpose( &Mat_CameraProj_Transpose, *Mat_CameraProj );
        LMatrix::transpose( &Mat_CameraViewProj_Transpose, *Mat_CameraViewProj );
        Mat_CameraView_InverseTranspose = Mat_CameraView_Inverse * Mat_CameraView_Transpose;
        Mat_CameraProj_InverseTranspose = Mat_CameraProj_Inverse * Mat_CameraProj_Transpose;
        Mat_CameraViewProj_InverseTranspose = Mat_CameraViewProj_Inverse * Mat_CameraViewProj_Transpose;
    }
};

/// �T�u�Z�b�g�P�ʂ̕`��p�����[�^
struct LNSubsetRenderParam
{
    LMaterial                   Material;       ///< �`��I�u�W�F�N�g�̃}�e���A�� (Node �ЂƂɂ� Shader �ЂƂ������Ȃ̂ŁA���̒��� Shader �͎g��Ȃ�)
    LVector4		            ToonColor;      ///< �`��I�u�W�F�N�g�̃g�D�[���J���[
    LVector4		            EdgeColor;      ///< �`��I�u�W�F�N�g�̗֊s�F

    lnfloat                     Opacity;        ///< �s�����x (�V�F�[�_�ɏ�Z�F�𑗂�Ƃ��A���̃��l�ɏ�Z�����)
    LColor	                    ColorScale;     ///< ��Z����F
    LColor	                    BlendColor;     ///< �u�����h����F
    LTone	                    Tone;           ///< �F��
    LMatrix		                UVTransform;    ///< �e�N�X�`�����W�ϊ��s��

    /// �����l
    LNSubsetRenderParam()
        : Opacity           ( 1.0f )
        , ToonColor         ( 1.0f, 1.0f, 1.0f, 1.0f ) 
        , EdgeColor         ( 0.0f, 0.0f, 0.0f, 1.0f )
        , ColorScale        ( 1.0f, 1.0f, 1.0f, 1.0f ) 
        , BlendColor        ( 0.0f, 0.0f, 0.0f, 0.0f )
        , Tone              ( 0.0f, 0.0f, 0.0f, 0.0f )
    {}
};



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================