//=============================================================================
//�y VertexBuffer �z
//-----------------------------------------------------------------------------
///**
//  @file       VertexBuffer.h
//  @brief      VertexBuffer
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
// �� VertexBuffer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
class VertexBuffer
    : public GLResource//: public Base::ReferenceObject
    , public IVertexBuffer
{
public:

    /// �R���X�g���N�^
    VertexBuffer();

    /// �f�X�g���N�^
    virtual ~VertexBuffer();

    LN_REFOBJ_METHODS;

public:

    /// ������
    LNRESULT initialize( GraphicsDevice* device_, LNVertexElemenst* elements_, u32 vertex_num_, const void* data_, bool is_dynamic_ );

    /// ���_���̎擾
	virtual u32 getVertexNum() const { return mVertexNum; }

	/// ���_�ЂƂ��̃T�C�Y�̎擾
	virtual u32 getVertexStride() const { return mVertexStride; }

	/// ���I�Ȓ��_���𔻒肷��	
	virtual bool isDynamic() const { return mIsDynamic; }

	/// ���b�N
	virtual void* lock();

	/// �A�����b�N
	virtual void unlock();

public:

    GLuint getGLVertexBuffer() { return mGLVertexBuffer; }

    LNGLVertexAttribPointerData* getGLVertexAttribPointerData() { return mLNGLAttribData; }

    virtual void onCommitGLResource();
    virtual void onDisposeGLResource();
    void onLostDevice() { GLResource::onLostDevice(); }
    void onResetDevice() { GLResource::onResetDevice(); }

private:

	GraphicsDevice*	                mGraphicsDevice;
    GLuint                          mGLVertexBuffer;
	LNGLVertexAttribPointerData*	mLNGLAttribData;    ///< ���_�錾
    lnByte*                         mBufferData;        ///< CPU ���ŊǗ������o�b�t�@�f�[�^ (�ÓI�o�b�t�@�̎��ɍ쐬�����)
	u32		                        mVertexNum;		    ///< ���_�̐�
	u32		                        mVertexStride;	    ///< ���_�ЂƂ��̃T�C�Y
    GLenum                          mUsage;
    bool                            mIsDynamic;
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