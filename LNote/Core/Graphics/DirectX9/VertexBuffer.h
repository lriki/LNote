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

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace DirectX9
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
    : public Base::ReferenceObject
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
    LNRESULT initialize( GraphicsDevice* device_, LNVertexElemenst* elements_, u32 vertex_num_, const void* data_, bool is_dynamic_ = false );

    /// ������ (�n���ꂽ IDirect3DVertexBuffer9 �� D3DVERTEXELEMENT9 ���Q�Ƃ���)
    LNRESULT initialize( GraphicsDevice* device_, IDirect3DVertexBuffer9* device_obj_, D3DVERTEXELEMENT9* layout_ );

public:

    /// ���_���̎擾
	virtual u32 getVertexNum() const { return mVertexNum; }

	/// ���_�ЂƂ��̃T�C�Y�̎擾
	virtual u32 getVertexStride() const { return mVertexStride; }

	/// ���I�Ȓ��_���𔻒肷��	
	virtual bool isDynamic() const { return ( !mVertexBuffer ); }

	/// ���b�N
	virtual void* lock();

	/// �A�����b�N
	virtual void unlock();

public:

	/// IDirect3DVertexBuffer9 �ւ̃|�C���^�擾
	IDirect3DVertexBuffer9* getDxVertexBuffer() { return mVertexBuffer; }

    /// IDirect3DVertexDeclaration9 �ւ̃|�C���^�擾
	IDirect3DVertexDeclaration9* getDxVertexDeclaration() { return mVertexDecl; }

private:

	GraphicsDevice*	                mGraphicsDevice;
    IDirect3DVertexBuffer9*	        mVertexBuffer;	    ///< ���_�o�b�t�@
	void*					        mVertices;		    ///< ���I�Ȓ��_�o�b�t�@�Ƃ��č�����ꍇ�͂������ɔz�񂪍���� ( union �ł��������ȁE�E�E )
	IDirect3DVertexDeclaration9*	mVertexDecl;	    ///< ���_�錾
	u32		                        mVertexNum;		    ///< ���_�̐�
	u32		                        mVertexStride;	    ///< ���_�ЂƂ��̃T�C�Y
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace DirectX9
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================