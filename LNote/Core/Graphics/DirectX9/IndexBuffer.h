//=============================================================================
//�y IndexBuffer �z
//-----------------------------------------------------------------------------
///**
//  @file       IndexBuffer.h
//  @brief      IndexBuffer
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
// �� IndexBuffer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
class IndexBuffer
    : public Base::ReferenceObject 
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
    LNRESULT initialize( GraphicsDevice* device_, int index_num_, const u16* data_, bool is_dynamic_, bool is_16bit_ );

    /// �C���f�b�N�X���̎擾
    virtual u32 getIndexNum() const { return mIndexNum; }

    /// �C���f�b�N�X�ЂƂ��̃T�C�Y�̎擾 (2 �܂��� 4)
    virtual lnU32 getIndexStride() const { return mIndexStride; }

	/// ���I�ȃC���f�b�N�X�o�b�t�@���𔻒肷��	
	virtual bool isDynamic() const { return ( !mIndexBuffer ); }

	/// ���b�N
	virtual void* lock();

	/// �A�����b�N
	virtual void unlock();

	/// IDirect3DIndexBuffer9 �ւ̃|�C���^�擾
	IDirect3DIndexBuffer9* getDxIndexBuffer() const { return mIndexBuffer; }

private:

	GraphicsDevice*	        mGraphicsDevice;
	IDirect3DIndexBuffer9*	mIndexBuffer;	    ///< IDirect3DIndexBuffer9 �ւ̃|�C���^
	void*	                mIndices;			///< ���I�ȃC���f�b�N�X�����ꍇ�͂����ɍ��
	u32		                mIndexNum;			///< �C���f�b�N�X�̐�
    lnU32                   mIndexStride;       ///< 2 �܂��� 4
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