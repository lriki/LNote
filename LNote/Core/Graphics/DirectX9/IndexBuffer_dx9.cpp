//=============================================================================
//�y IndexBuffer �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "GraphicsDevice.h"
#include "IndexBuffer.h"

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

//=============================================================================
// �� IndexBuffer �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    IndexBuffer::IndexBuffer()
        : mGraphicsDevice   ( NULL )
        , mIndexBuffer      ( NULL )
        , mIndices          ( NULL )
        , mIndexNum         ( 0 )
        , mIndexStride      ( 0 )
    {
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    IndexBuffer::~IndexBuffer()
    {
        SAFE_DELETE_ARRAY( mIndices );
		SAFE_RELEASE( mIndexBuffer );
        LN_SAFE_RELEASE( mGraphicsDevice );
    }

    //---------------------------------------------------------------------
    // �� ������
    //---------------------------------------------------------------------
    LNRESULT IndexBuffer::initialize( GraphicsDevice* device_, int index_num_, const u16* data_, bool is_dynamic_, bool is_16bit_ )
    {
        mGraphicsDevice = device_;
        mGraphicsDevice->addRef();
        mIndexNum = index_num_;

        mIndexStride = ( is_16bit_ ) ? 2 : 4;

        // ���I�ȃC���f�b�N�X�o�b�t�@�Ƃ��č��ꍇ
		if ( is_dynamic_ )
		{
            if ( is_16bit_ )
            {
			    mIndices = LN_NEW u16[ mIndexNum ];
            }
            else
            {
                mIndices = LN_NEW lnU32[ mIndexNum ];
            }
		}
		// �ÓI�ȃC���f�b�N�X�o�b�t�@�Ƃ��č��ꍇ
		else
		{
			HRESULT hr;
            
            LN_DXCALL_R(
                mGraphicsDevice->getDX9Device()->CreateIndexBuffer( 
				    mIndexStride * mIndexNum,
				    0,
                    ( is_16bit_ ) ? D3DFMT_INDEX16 : D3DFMT_INDEX32,
				    D3DPOOL_MANAGED,
				    &mIndexBuffer,
				    NULL ) );
		}

		// �C���f�b�N�X�f�[�^���󂯎���Ă���ꍇ�͂���ŁA
		// �󂯂Ƃ��Ă��Ȃ��ꍇ�� 0 �Ńo�b�t�@�𖄂߂�
		void* buf = lock();
		if ( data_ )
		{
			memcpy( buf, data_, mIndexStride * mIndexNum );
		}
		else
		{
			memset( buf, 0, mIndexStride * mIndexNum );
		}
		unlock();

        return LN_OK;
    }

	//---------------------------------------------------------------------
    // �� ���b�N
    //---------------------------------------------------------------------
	void* IndexBuffer::lock()
    {
        if ( mIndexBuffer )
		{
			void* r = 0;
			mIndexBuffer->Lock( 0, 0, &r, 0 );
			return r;//static_cast< u16* >( r );
		}
		return mIndices;
    }

	//---------------------------------------------------------------------
    // �� �A�����b�N
    //---------------------------------------------------------------------
	void IndexBuffer::unlock()
    {
        if ( mIndexBuffer )
		{
			mIndexBuffer->Unlock();
		}
    }


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