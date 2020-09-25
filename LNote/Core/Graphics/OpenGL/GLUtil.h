//=============================================================================
//�y GLUtil �z
//-----------------------------------------------------------------------------
///**
//  @file       GLUtil.h
//  @brief      GLUtil
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Thread/EventFlag.h"
#include "../Interface.h"
#include "GLPlatformContext.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace OpenGL
{

/// (�Ƃ肠���� GLSL �p�B�����̒l�� IShader::getVariableByDeclUsage() �ɓn���� attribute �ϐ����擾����)
enum LNVertexDeclUsage
{
    LN_VERTEXDECLUSAGE_UNKNOWN = 0,

    LN_VERTEXDECLUSAGE_POSITION,        ///< ���_�ʒu
    LN_VERTEXDECLUSAGE_BLENDWEIGHT,     ///< �u�����h�E�F�C�g�l
    LN_VERTEXDECLUSAGE_BLENDINDICES,    ///< �u�����h�C���f�b�N�X�l
    LN_VERTEXDECLUSAGE_NORMAL,          ///< �@��
    LN_VERTEXDECLUSAGE_TEXCOORD0,       ///< UV
    LN_VERTEXDECLUSAGE_TEXCOORD1,       ///< �ǉ�UV1
    LN_VERTEXDECLUSAGE_TEXCOORD2,       ///< �ǉ�UV2
    LN_VERTEXDECLUSAGE_TEXCOORD3,       ///< �ǉ�UV3
    LN_VERTEXDECLUSAGE_TEXCOORD4,       ///< �ǉ�UV4
    LN_VERTEXDECLUSAGE_TEXCOORD5,       ///< Sdef - C�l
    LN_VERTEXDECLUSAGE_TEXCOORD6,       ///< Sdef - R0�l
    LN_VERTEXDECLUSAGE_TEXCOORD7,       ///< Sdef - R1�l
    LN_VERTEXDECLUSAGE_TEXCOORD8,       ///< �G�b�W�E�F�C�g
    LN_VERTEXDECLUSAGE_PSIZE15,         ///< ���_�C���f�b�N�X�l

    LN_VERTEXDECLUSAGE_COLOR,           ///< ���_�J���[

    LN_VERTEXDECLUSAGE_MAX,
};

/// glVertexAttribPointer() �̑�2�����ȍ~�̂܂Ƃ�
struct LNGLVertexAttribPointerData
{
    LNVertexDeclUsage   Usage;          ///< IShader::getVariableByDeclUsage() �ɓn���� attribute �ϐ����擾����

    GLint               Size;
    GLenum              Type;       ///< (�I�[��0)
    GLboolean           Normalized;
    GLsizei             Stride;
    lnU8                ByteOffset;
};

//=============================================================================
// �� Util �N���X
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
class Util
{
public:

    /// LNVertexElemenst �z�񂩂� glVertexAttribPointer() �ɐݒ肷������쐬���� (�v delete[]�B�I�[�v�f�� Size == -1)
    static LNGLVertexAttribPointerData* createVertexAttribData( const LNVertexElemenst* elements_ );

    /// LNDeclType -> GLenum, size(�v�f�� LN_DECL_FLOAT3 �Ȃ� 3), normalized(���K���̗L��)
    static void convertDeclTypeLNToGL( LNDeclType ln_type_, GLenum* gl_type_, GLint* size_, GLboolean* normalized_ );

    /// LNDeclUsage, index -> LNVertexDeclUsage
    static LNVertexDeclUsage convertDeclUsage( LNDeclUsage usage_, lnU32 index_ );
};

//=============================================================================
// �� GLResource �N���X
//-----------------------------------------------------------------------------
///**
//  @brief     
//
//  @note
//              ���̃N���X��requestCommitGLResource() �ȊO�̊֐���
//              ���ׂĕ`��X���b�h����Ăяo�����ƁB
//*/
//=============================================================================
class GLResource
    : public Base::ReferenceObject
{
public:

    GLResource()
        : mInitialized(false)
    {}

    virtual ~GLResource()
    {
        // �_���B�T�u�N���X�ŌĂԂ��ƁB
        //onDisposeGLResource();
    }

    /// (called from GraphicsDevice::commitChangesGLResource())
    void commitChanges()
    {
        if (!mInitialized.isTrue())
        {
            onCommitGLResource();
            mInitialized.setTrue();
        }
    }

    void onLostDevice() { onDisposeGLResource(); }

    void onResetDevice() { mInitialized.setFalse(); }

    void requestCommitGLResource() { mInitialized.setFalse(); }

protected:

    /// GL���\�[�X�쐬 (Renderer::beginScene() ���炢�낢��o�R�ŌĂ΂��)
    virtual void onCommitGLResource() = 0;

    /// GL���\�[�X���
    virtual void onDisposeGLResource() = 0;

protected:

    Thread::EventFlag mInitialized;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace OpenGL
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================