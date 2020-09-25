//=============================================================================
//�y SceneShader �z
//-----------------------------------------------------------------------------
///**
//  @file       SceneShader.h
//  @brief      SceneShader
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Interface.h"
#include "../Common.h"
#include "MMETypes.h"
#include "SceneShaderManager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
class SceneShaderTechnique;
class NodeRenderParam;
class DrawCommandContainer;

//=============================================================================
// �� SceneShader �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �V�[���p�V�F�[�_�̃N���X
//*/
//=============================================================================
class SceneShader
    : public Base::ReferenceObject
    , public SceneShaderCache::CachedObject
{
public:

    struct InitData
    {
        const void*     ShaderData;
        lnU32           ShaderDataSize;
        LRefTString     ShaderName;

    };

public:
    SceneShader( SceneShaderManager* manager_ );
    LN_CACHED_REFOBJ_METHODS;

protected:
    virtual ~SceneShader();

public:

    /// ������
    LNRESULT initialize( const InitData& init_data_ );

    /// �ēǂݍ���
    LNRESULT reload();

    /// ���\�[�X�̍č\�z
    LNRESULT refreshResource( const LVector2& view_size_ );

    /// Graphics::IShader �̎擾
    Graphics::IShader* getCoreShader() { return mShader; }

    /// �G���[������̐ݒ� (SceneShaderTechnique �N���X������Ă΂��)
    void setErrorMessage( LRefTString str_ ) { mErrorMessage = str_; }

    /// �G�t�F�N�g�t�@�C���̎g�p�ړI�̎擾
    MMEScriptClass getScriptClass() const { return mScriptClass; }

    /// �G�t�F�N�g�t�@�C���̎��s�^�C�~���O�̎擾
    MMEScriptOrder getScriptOrder() const { return mScriptOrder; }

    /// �|�X�g�G�t�F�N�g���A�|�X�g�G�t�F�N�g�̏ꍇ true
    bool isPostEffect() const { return ( mScriptOrder == MME_SCRORDER_preprocess || mScriptOrder == MME_SCRORDER_postprocess ); }

    


public: // �� �����p

    /// SceneShaderManager �擾
    SceneShaderManager* getSceneShaderManager() { return mManager; }

    /// �ݒ肷��K�v�����郉�C�g�̐��̎擾
    lnU32 getRequiredLightNum() const { return mRequiredLightNum; }

    /// �ϐ����ɑΉ����� MMEShaderVariable ���������� (MMETechnique�Ŏg���B���̃|�C���^�̓V�F�[�_�����邤���͕s�ςȂ̂ŁAMMETechnique �Ŏg���Ȃ� initialize() �Ŏ擾���Ă����Ă�OK)
    MMEShaderVariable* findShaderVariable( const lnChar* name_ );

    

    /// Scene �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
    void updateSceneParam( const MMESceneParam& param_ );

    /// Camera �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
    void updateCameraParam( const LNCameraSceneParam& param_ );

    /// Node �P�ʂŕK�v�ȃp�����[�^��ݒ肷�� (�m�[�h���Ƃɉe�����郉�C�g�͈قȂ邽�߁A���C�g�����̒�)
    LNRESULT updateNodeParam( const SceneNodeContext* param_ );// const NodeRenderParam& param_ );

    /// Subset �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
    void updateSubsetParam( const LNSubsetRenderParam& param_ );

    /// CONTROLOBJECT ��v�����Ă���ϐ��̒l�����ׂčX�V����
    void updateControlObjectVariables();

    /// �`��R�}���h�̍쐬 (�e�N�j�b�N���|�X�g�G�t�F�N�g�̏ꍇ�͉����s��Ȃ��BSceneNode::makeDrawCommand() ����Ă΂��)
    void makeDrawCommand( DrawCommandContainer* container_, SceneNode* object_, MMDPass pass_ );

    /// �`��R�}���h�̍쐬 ("ScriptExternal=Color" �̑O�܂�)
    void makeDrawCommandPreExternal( DrawCommandContainer* container_, SceneNode* object_, MMDPass pass_ );

    /// �`��R�}���h�̍쐬 ("ScriptExternal=Color" ����I�[�܂�)
    void makeDrawCommandPostExternal( DrawCommandContainer* container_, SceneNode* object_, MMDPass pass_ );


    /// �I�t�X�N���[��RT DefaultEffect �� "self" �Ŏ��ʂ����m�[�h�̎擾 (onSettingShader() �Őݒ�)
    SceneNode* getSelfSceneNode() { return mSelfSceneNode; }

    /// ���L�\�ȃV�F�[�_�̏ꍇ true
    bool isShareable() const { return mShareable; }

    /// SceneNode::setShader() �ł̐擪�ŌĂ΂��
    void onSettingShader( SceneNode* node_ );


    /// ���o��
    void dumpInfo( FILE* stream_ = NULL );

private:

    /// �p�����[�^�Ȃǂ����ۂɐݒ肵�Ă���
    LNRESULT _build();

    /// ���
    void _release();

    /// ���\�[�X���
    void _releaseResource();

    /// �V�F�[�_�ϐ��̃Z�}���e�B�N�X�𒲂ׂđΉ�����v�����ڂ�Ԃ�
    MMEVariableRequest _checkVariableRequest( Graphics::IShaderVariable* var_, MMEShaderVariable* sv_ );

private:

    typedef std::vector< SceneShaderTechnique* >    SceneShaderTechniqueArray;
    typedef std::vector< OffscreenScene* >          OffscreenSceneList;

private:

    SceneShaderManager*     mManager;
    Graphics::IShader*      mShader;                ///< �V�F�[�_�v���O�����{��
    LRefTString             mName;

    MMEScriptOutput         mScriptOutput;          ///< STANDARDSGLOBAL �� ScriptOutput (��� "color")
    MMEScriptClass          mScriptClass;           ///< �G�t�F�N�g�t�@�C���̎g�p�ړI (�f�t�H���g�� "object")
    MMEScriptOrder          mScriptOrder;           ///< �G�t�F�N�g�t�@�C���̎��s�^�C�~���O (�f�t�H���g�� "standard")

    MMEShaderVariableArray  mShaderVariableArray;   ///< �V�F�[�_�v���O�������̕ϐ��ꗗ
    MMEShaderVariableMap    mShaderVariableMap;     ///< �ϐ�����MMEShaderVariable�̑Ή��\ (��Ƀe�N�j�b�N�̃X�N���v�g�Ŏg��)
    MMEShaderVariableArray  mControlObjectVarArray; ///< CONTROLOBJECT ��v�����Ă���ϐ����X�g
    SceneShaderTechniqueArray   mTechniqueList;         ///< �e�N�j�b�N�̈ꗗ
    OffscreenSceneList      mOffscreenSceneList;

    lnU32                   mRequiredLightNum;
    lnU32                   mWorldMatrixCalcMask;   ///< MMEWorldMatrixCalcFlags �̑g�ݍ��킹 (������������g��Ȃ�����)

    SceneNode*              mSelfSceneNode;

    LRefTString             mErrorMessage;
    bool                    mIsError;
    bool                    mShareable;
};

//=============================================================================
// �� SceneShaderUtil
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class SceneShaderUtil
{
public:

    /// �e�N�X�`�����\�[�X�̐������
    struct TextureResourceData
    {
        LRefTString         ResourceName;
        LVector3            Dimensions;
        LVector2            ViewportRatio;
        int                 Miplevels;
        LNSurfaceFormat     Format;

        // �A�j���[�V�����e�N�X�`���p
        lnfloat             Offset;
        lnfloat             Speed;
        LRefTString         SeekVariable;

        // �I�t�X�N���[��RT�p
        LVector4            ClearColor;
        lnfloat             ClearDepth;
        bool                AntiAlias;
        LRefTString         Description;
        LRefTString         DefaultEffect;
    };

    

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �V�F�[�_�ϐ��̃Z�}���e�B�N�X�𒲂ׂđΉ�����v�����ڂ�Ԃ�
    //
    //  @param[out] script_output_  : STANDARDSGLOBAL �� ScriptOutput �̒l���i�[����ϐ��̃A�h���X (��� "color")
    //  @param[out] script_class_   : �G�t�F�N�g�t�@�C���̎g�p�ړI (�f�t�H���g�� "object")
    //  @param[out] script_order_   : �G�t�F�N�g�t�@�C���̎��s�^�C�~���O (�f�t�H���g�� "standard")            
    //  @param[out] is_controllobject_ : �L���� CONTROLOBJECT �Z�}���e�B�N�X�̏ꍇ�Atrue ���i�[�����
    //
    //  @note
    //              script_output_�Ascript_class_�Ascript_order_ �� STANDARDSGLOBAL ��
    //              ����ꍇ�̂ݐݒ肳���B���̊֐��̌Ăяo������ sv_->Request == MME_VARREQ_STANDARDSGLOBAL ��
    //              �`�F�b�N��A�ϐ��̒��g���m�F���邱�ƁB
    //*/
    //---------------------------------------------------------------------
    static void checkVariableRequest(
        Graphics::IShaderVariable* var_,    // const��������
        const MMESemanticsMap& semantics_map_,
        MMEShaderVariable* sv_,
        MMEScriptOutput* script_output_,
        MMEScriptClass* script_class_,
        MMEScriptOrder* script_order_,
        bool* is_controllobject_ );

    /// �ϐ��� "Object" �A�m�e�[�V�����𒲂ׂāA"Light" �̏ꍇ�� fales�A����ȊO("Camera") �� true ��Ԃ�
    static bool checkAnnotationCameraOrLight( Graphics::IShaderVariable* var_ );

    /// �ϐ��� "Object" �A�m�e�[�V�����𒲂ׂāA"Geometry" �̏ꍇ�� 1�A"Light" �̏ꍇ�� 2�A����ȊO�� 0 ��Ԃ�
    static int checkAnnotationGeometryOrLight( Graphics::IShaderVariable* var_ );

    /// ���O���w�肵�ăA�m�e�[�V�������������� (�啶���������̋�ʖ����BIShaderVariable ����͑召��������ʂ���ł������Ȃ��̂ŗp��)
    static Graphics::IShaderVariable* getAnnotationByName( Graphics::IShaderVariable* var_, const lnChar* name_ );

    /// �v�����ڂ���A�v�Z����K�v�̂���s��}�X�N�̃r�b�g����쐬���ĕԂ�
    static lnU32 checkMatrixMask( MMEVariableRequest req_ );

    /// �e�N�X�`�����\�[�X���쐬����K�v�̂���v�����𔻒肷��
    static bool needCreateTexture( MMEVariableRequest req_ ) { return ( MME_VARREQ_TEXTURE <= req_ && req_ <= MME_VARREQ_OFFSCREENRENDERTARGET ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �e�N�X�`�����\�[�X�̏���Ԃ�
    //  
    //  @return     �e�N�X�`�������K�v������ꍇ (�ϐ��̃Z�}���e�B�N�X���e�N�X�`���n) true ��Ԃ�
    //*/
    //---------------------------------------------------------------------
    static bool checkTextureResourceData(
        MMEShaderVariable* sv_,  // ����� const �ɂ������c
        const MMEAnnotationMap& annotation_map_,
        TextureResourceData* data_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �e�N�X�`�����\�[�X�̏�񂩂�A�e�N�X�`���T�C�Y��Ԃ�
    //
    //  @param[in]  viewport_size_ : �V�[���̉��z�X�N���[���T�C�Y
    //*/
    //---------------------------------------------------------------------
    static void checkTextureSize(
        int* width_,
        int* height_,
        const LVector3& dimensions_,
        const LVector2& viewport_ratio_,
        const LVector2& viewport_size_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �`��T�u�Z�b�g�����������񂩂�A�T�u�Z�b�g�ԍ��̔z����쐬����
    //
    //  @note
    //              '-' �ɂ��͈͎w��Łux�Ԉȍ~���ׂāv��`�悷��ꍇ�A
    //              �z��̏I�[�ɂ� LN_INVALID_INDEX ���i�[�����B
    //*/
    //---------------------------------------------------------------------
    static void makeSubsetIndexArray( SubsetIndexArray* list_, const lnChar* subset_text_ );

private:

    static const int MAX_SEMANTICS_NAME_LEN = 256;

    struct SubsetIndexOrder
    {
        lnU32   Index;
        int     RangeState;    ///< �͈͂̎w�� (0=�Ȃ� 1=����SubsetIndex�̒l�܂� 2=�T�u�Z�b�g���̍ő�܂�)
    };

    typedef std::list< SubsetIndexOrder > SubsetIndexOrderList;

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