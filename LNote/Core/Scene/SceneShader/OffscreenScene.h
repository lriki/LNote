//=============================================================================
//�y OffscreenScene �z
//-----------------------------------------------------------------------------
///**
//  @file       OffscreenScene.h
//  @brief      OffscreenScene
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Common.h"
#include "../SceneNode/NodeRenderParam.h"

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
class SceneShader;

//=============================================================================
// �� OffscreenScene �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class OffscreenScene
    : public Base::ReferenceObject
{
public:

    struct InitData
    {
        LVector4            ClearColor;
        lnfloat             ClearDepth;
        LRefTString         Description;
        LRefTString         DefaultEffectStrings;
        Graphics::ITexture* RTTexture;              ///< addRef
        //bool                Antialias;
    };

public:

    /// �R���X�g���N�^
    OffscreenScene( SceneShaderManager* manager_, SceneShader* shader_ );

    /// �f�X�g���N�^
    ~OffscreenScene();

public:
   
    /// ������ (�A���`�G�C���A�X���Ή�)
    LNRESULT initialize( const InitData& init_data_ );

    /// �����_�����O�^�[�Q�b�g�N���A����F�̐ݒ�
    void setClearColor( const LVector4& color_ );

    /// �����_�����O�^�[�Q�b�g�N���A����F�̎擾 (�J���[�R�[�h)
    u32 getClearColor() const { return mClearColor; }

    /// �[�x�^�[�Q�b�g�N���A����Z�l�̐ݒ�
    void setClearDepth( lnfloat depth_ ) { mClearDepth = depth_; }

    /// �[�x�^�[�Q�b�g�N���A����Z�l�̎擾
    lnfloat getClearDepth() const { return mClearDepth; }

    /// �G�t�F�N�g�t�@�C���̐������̐ݒ�
    void setDescription( LRefTString desc_ ) { mDescription = desc_; }

    /// �G�t�F�N�g�t�@�C���̐������̎擾
    LRefTString getDescription() const { return mDescription; }

    /// �G�t�F�N�g�t�@�C���̊��蓖�ĕ��@�̐ݒ� (DefaultEffect �A�m�e�[�V����)
    void setDefaultEffectStrings( LRefTString effects_ ) { mDefaultEffectStrings = effects_; }

    /// �G�t�F�N�g�t�@�C���̊��蓖�ĕ��@�̎擾
    //DefaultEffectStrings getDefaultEffectStrings() { return mDefaultEffectStrings; }

    /// �m�[�h��DefaultEffect �̎w��ƈ�v����ꍇ�ɒǉ����� (�V�����쐬���ꂽ�m�[�h�ɑ΂��Ă͂��ׂĈ�x������Ă�)
    void addSceneNode( SceneNode* node_ );

    /// �m�[�h���O�� (SceneShaderManager::removeSceneNodeFromOffscreenScene() ����Ă΂��)
    void removeSceneNode( SceneNode* node_ );

    /// �V�[���ɓo�^����Ă��邷�ׂẴm�[�h�� addSceneNode() ���Ă� (makeDrawCommand �œǂ�ł���������)
    void refreshAffectNodes();

    /// DefaultEffect �� "self=" ���܂܂�Ă���
    bool isThereSelf() const { return mIsThereSelf; }

    /// ���ׂẴI�t�X�N���[��RT�̕`��R�}���h���쐬����
    void makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ );

private:

    /// �G�t�F�N�g�t�@�C���̊��蓖�čX�V
    //      �K�v�ȃV�F�[�_���܂� Manager �ɓo�^����Ă��Ȃ��ꍇ�͐V�����쐬����B
    //      ���ꂾ���Ȃ珉�����̂Ƃ��ɂ���΂����Ǝv�������ǁA
    //      ���̃N���X�� initialzie() �̓V�F�[�_�̏���������Ă΂��B
    //      �Ƃ������Ƃ́AManager::createShader() ���ċA�I�ɌĂ΂�邱�ƂɂȂ��Ă��܂��A
    //      �Ȃ񂩕s���B(���ہAcreateShader() �ł� FileManager �̔r���������s���Ă�)
    //      ����Ȃ킯�ł�����Ɩʓ|���������ǃV�F�[�_�����I�������A
    //      �������㏈�����Ă����`�Ō����E�쐬�����Ă����B
    //
    //      [12/8/12] initialize() �� SceneShader::refreshResource() ����Ăяo���悤�ɂ����̂ŁA
    //      �ʓr������ĂԕK�v�͍��̂Ƃ���Ȃ��B
    LNRESULT _refreshEffects();

    // �I�t�X�N���[�� RT ���ō쐬�����V�F�[�_���������
    void _releaseEffects();

private:

    enum EntryType
    {
        ENTRY_EFFECT = 0,   ///< �G�t�F�N�g�t�@�C��
        ENTRY_NONE,         ///< �G�t�F�N�g�Ȃ�
        ENTRY_HIDE          ///< ��\��
    };

    struct DefaultEffectEntry
    {
        EntryType       Type;
        LRefTString     ObjectName;     ///< (���C���h�J�[�h�w��\ * ?)
        LRefTString     EffectName;     ///< (���C���h�J�[�h�w��s��)
        SceneShader*    Shader;         ///< EffectName �ō쐬�����V�F�[�_
    };

    struct SceneNodeEntry
    {
        SceneNode*          Node;   ///< �`�悷��m�[�h
        LNPriorityParameter PriorityParameter;  ///< �`��R�}���h�ɓn��
        //SceneShader*    Shader; ///< �K�p����D��V�F�[�_
        //bool            Hide;
    };

private:

    typedef std::vector< DefaultEffectEntry >   DefaultEffectEntryList;
    typedef std::vector< SceneNodeEntry >       SceneNodeEntryList;
    
private:

    SceneShaderManager*     mManager;
    SceneShader*            mSceneShader;
    //SceneGraph*             mScene;
    lnU32                   mClearColor;
    lnfloat                 mClearDepth;
    LRefTString             mDescription;
    LRefTString             mDefaultEffectStrings;
    Graphics::ITexture*     mRTTexture;

    SceneNodeEntryList      mSceneNodeEntryArray;            ///< ���̃I�t�X�N���[��RT�ŕ`�悷��m�[�h�̈ꗗ

    DefaultEffectEntryList  mDefaultEffectEntryList;
    LRefTString             mErrorMessage;

    bool                    mIsThereSelf;
    bool                    mNeedRefreshAffectNodes;
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