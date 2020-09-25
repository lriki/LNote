//=============================================================================
//【 IndexBuffer 】
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
// ■ IndexBuffer クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
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
    // ● デストラクタ
    //---------------------------------------------------------------------
    IndexBuffer::~IndexBuffer()
    {
        SAFE_DELETE_ARRAY( mIndices );
		SAFE_RELEASE( mIndexBuffer );
        LN_SAFE_RELEASE( mGraphicsDevice );
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT IndexBuffer::initialize( GraphicsDevice* device_, int index_num_, const u16* data_, bool is_dynamic_, bool is_16bit_ )
    {
        mGraphicsDevice = device_;
        mGraphicsDevice->addRef();
        mIndexNum = index_num_;

        mIndexStride = ( is_16bit_ ) ? 2 : 4;

        // 動的なインデックスバッファとして作る場合
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
		// 静的なインデックスバッファとして作る場合
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

		// インデックスデータを受け取っている場合はそれで、
		// 受けとっていない場合は 0 でバッファを埋める
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
    // ● ロック
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
    // ● アンロック
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