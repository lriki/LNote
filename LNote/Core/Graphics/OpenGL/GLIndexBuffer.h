//=============================================================================
//�y GLIndexBuffer �z
//-----------------------------------------------------------------------------
///**
//  @file       GLIndexBuffer.h
//  @brief      GLIndexBuffer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"
#include "GLUtil.h"

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
class GraphicsDevice;

//=============================================================================
// �� IndexBuffer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
class IndexBuffer
    : public GLResource//Base::ReferenceObject 
    , public IIndexBuffer
{
public:

    /// �R���X�g���N�^
    IndexBuffer();

    /// �f�X�g���N�^
    virtual ~IndexBuffer();

    LN_REFOBJ_METHODS;

public:

    /// ������
    LNRESULT initialize( GraphicsDevice* device_, int index_num_, const void* data_, bool is_dynamic_, bool is_16bit_ );

    /// �C���f�b�N�X���̎擾
    virtual u32 getIndexNum() const { return mIndexNum; }

    /// �C���f�b�N�X�ЂƂ��̃T�C�Y�̎擾 (2 �܂��� 4)
    virtual lnU32 getIndexStride() const { return mIndexStride; }

	/// ���I�ȃC���f�b�N�X�o�b�t�@���𔻒肷��	
	virtual bool isDynamic() const { return mIsDynamic; }

	/// ���b�N
	virtual void* lock();

	/// �A�����b�N
	virtual void unlock();

public:

	GLuint getGLIndexBuffer() { return mGLIndexBuffer; }


    virtual void onCommitGLResource();
    virtual void onDisposeGLResource();
    void onLostDevice() { GLResource::onLostDevice(); }
    void onResetDevice() { GLResource::onResetDevice(); }

private:

	GraphicsDevice*     mGraphicsDevice;
    GLuint              mGLIndexBuffer;
    lnByte*             mBufferData;        ///< CPU ���ŊǗ������o�b�t�@�f�[�^ (�ÓI�o�b�t�@�̎��ɍ쐬�����)
	lnU32		        mIndexNum;			///< �C���f�b�N�X�̐�
    lnU32               mIndexStride;       ///< 2 �܂��� 4
    GLenum              mUsage;
    bool                mIsDynamic;
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