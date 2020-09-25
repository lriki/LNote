//=============================================================================
//�y SceneShaderTechnique �z
//-----------------------------------------------------------------------------
///**
//  @file       SceneShaderTechnique.h
//  @brief      SceneShaderTechnique
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Interface.h"
#include "../DrawCommand.h"
#include "MMETypes.h"
#include "SceneShader.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

class SceneShader;
class DrawCommandContainer;

/// �X�N���v�g�̃R�}���h���
enum MMEScriptCommandType
{
    MME_SCRIPTCOM_UNKNOWN = 0,

    MME_SCRIPTCOM_RenderColorTarget,
    MME_SCRIPTCOM_RenderDepthStencilTarget,

    MME_SCRIPTCOM_ClearSetColor,
    MME_SCRIPTCOM_ClearSetDepth,

    MME_SCRIPTCOM_Clear_Color,              ///< Clear=Color
    MME_SCRIPTCOM_Clear_Depth,              ///< Clear=Depth

    MME_SCRIPTCOM_ScriptExternal_Color,     ///< ScriptExternal=Color

    MME_SCRIPTCOM_Pass,                     ///< Pass=(�p�X��) (���ۂɂ͎g���ĂȂ����ǈꉞ)

    MME_SCRIPTCOM_BeginPass,                ///< �p�X�̊J�n
    MME_SCRIPTCOM_EndPass,                  ///< �p�X�̏I��

    MME_SCRIPTCOM_LoopByCount,              ///< LoopByCount=(�p�����[�^��)
    MME_SCRIPTCOM_LoopEnd,                  ///< LoopEnd=
    MME_SCRIPTCOM_LoopGetIndex,             ///< LoopGetIndex=(�p�����[�^��)

    MME_SCRIPTCOM_Draw_Geometry,            ///< Draw=Geometry
    MME_SCRIPTCOM_Draw_Buffer,              ///< Draw=Buffer

    MME_SCRIPTCOM_RenderPort,               ///< RenderPort=*  ���Ή�
};


/// �X�N���v�g�̃R�}���h�ЂƂ�
struct MMEScriptCommand
{
    MMEScriptCommandType Type;      ///< �R�}���h�̎��

    union
    {
        struct
        {
            MMEShaderVariable*          Variable;   ///< RT �����V�F�[�_�ϐ�
            u32                         Index;      ///< �ݒ肷��C���f�b�N�X
        };
        //struct
        //{
        //    u32                         Color;      ///< �N���A�J���[
        //    lnfloat                     Depth;      ///< �N���A�[�x�l
        //};
        struct
        {
            Graphics::IShaderVariable*  CoreVariable;       ///< �N���A�J���[�A�[�x�p
        };
        struct
        {
            Graphics::IShaderPass*      Pass;       ///< ���s�p�X
        };
        struct
        {
            int                         LoopCount;  ///< ���[�v��
        };
        struct
        {
            Graphics::IShaderVariable*  LoopVar;    ///< ���[�v�񐔂��i�[����ϐ�
        };
    };
};

typedef std::vector< MMEScriptCommand >   MMEScriptCommandList;

//=============================================================================
// �� SceneShaderTechnique �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class SceneShaderTechnique
{
public:

    SceneShaderTechnique();

    virtual ~SceneShaderTechnique();

public:

    /// ������
    LNRESULT initialize( SceneShader* shader_, Graphics::IShaderTechnique* tech_ );

    /// �`��p�X���擾����
    MMDPass getMMDPass() const { return mMMDPass; }

    /// �`��R�}���h�̍쐬
    void makeDrawCommand( DrawCommandContainer* container_, SceneNode* object_ );

    /// �`��R�}���h�̍쐬 ("ScriptExternal=Color" �̑O�܂�)
    void makeDrawCommandPreExternal( DrawCommandContainer* container_, SceneNode* object_ );

    /// �`��R�}���h�̍쐬 ("ScriptExternal=Color" ����I�[�܂�)
    void makeDrawCommandPostExternal( DrawCommandContainer* container_, SceneNode* object_ );

    /// �X�N���v�g�� "ScriptExternal=Color" �����邩�ǂ����𔻒肷��
    bool isPostProcess() const { return ( mCommandExistFlags & MME_SCEXIST_ScriptExternal_Color ); }

    /// ����̃R�}���h�̑��݂��m�F���邽�߂̃t���O
    lnU32 getCommandExistFlags() const { return mCommandExistFlags; }

private:

    /// �X�N���v�g������̕ϐ����󂯎���� mScriptCommandList �ɃR�}���h��ǉ����Ă���
    //      pass_ : �p�X���̃X�N���v�g����͂���ꍇ�A���̃p�X��n�� (Draw="Geometry" �̎��s�ɓn��pass)
    LNRESULT _lexScriptCommandString( Graphics::IShaderVariable* var_, Graphics::IShaderPass* pass_ );

    /// �ЂƂ��̕`��R�}���h������� container_ �ɒǉ�
    LNRESULT _makeDrawCommand( DrawCommandContainer* container_, MMEScriptCommand* command_, SceneNode* object_ );


private:

    SceneShader*                mSceneShader;
    Graphics::IShader*          mCoreShader;
    Graphics::IShaderTechnique* mTechnique;
    MMDPass                     mMMDPass;
    lnU32                       mFlags;      ///< MMETechEnableFlag �̑g�ݍ��킹

    SubsetIndexArray            mSubsetIndexArray;

    MMEScriptCommandList            mScriptCommandList;     
    MMEScriptCommandList::iterator  mScriptExternalIt;      ///< mScriptCommandList ����"ScriptExternal=Color" �̃R�}���h�̈ʒu���w���C�e���[�^
    lnU32                           mCommandExistFlags;     ///< MMEScriptCommandExistFlag �̑g�ݍ��킹
    
    lnU32                       mNextClearColor;
    lnfloat                     mNextClearDepth;

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