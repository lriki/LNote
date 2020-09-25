//=============================================================================
//【 VertexBuffer 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "DX9Module.h"
#include "GraphicsDevice.h"
#include "VertexBuffer.h"

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
// ■ VertexBuffer クラス
//=============================================================================


    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    VertexBuffer::VertexBuffer()
        : mGraphicsDevice   ( NULL )
        , mVertexBuffer     ( NULL )
        , mVertices         ( NULL )
        , mVertexDecl       ( NULL )
        , mVertexNum        ( 0 )
        , mVertexStride     ( 0 )
    {
    }


    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    VertexBuffer::~VertexBuffer()
    {
        SAFE_DELETE_ARRAY( mVertices );
		SAFE_RELEASE( mVertexBuffer );
		SAFE_RELEASE( mVertexDecl );
        LN_SAFE_RELEASE( mGraphicsDevice );
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT VertexBuffer::initialize( GraphicsDevice* device_, LNVertexElemenst* elements_, u32 vertex_num_, const void* data_, bool is_dynamic_ )
    {
        mGraphicsDevice = device_;
        mGraphicsDevice->addRef();
        mVertexNum = vertex_num_;

        HRESULT hr;

		// 頂点宣言の数を調べる
		int elem_size = 0;
		while ( 1 )
		{
			if ( elements_[ elem_size ].Type == LN_DECL_UNKNOWN )
			{
				break;
			}
			++elem_size;
		}
		LN_SETERR_R_S_( ( elem_size != 0 ), LN_ERR_ASSERT, "< VertexBuffer::initialize >\n無効な頂点宣言です。" );

		// D3DVERTEXELEMENT9 を作成して、elements_ から DirectX用の頂点宣言を作る
		D3DVERTEXELEMENT9* dxelem = LN_NEW D3DVERTEXELEMENT9[ elem_size + 1 ];

		u8 offset = 0;
		u8 to;
		for ( int i = 0; i < elem_size; ++i )
		{
			dxelem[ i ].Stream = 0;
			dxelem[ i ].Offset = offset;
			dxelem[ i ].Method = D3DDECLMETHOD_DEFAULT;
			dxelem[ i ].UsageIndex = elements_[ i ].UsageIndex;
			
            DX9Module::convertElementLNToDX( &elements_[ i ], &dxelem[ i ].Type, &to, &dxelem[ i ].Usage );
			offset += to;
		}
		dxelem[ elem_size ].Stream = 0xff;
		dxelem[ elem_size ].Offset = 0;
		dxelem[ elem_size ].Type = D3DDECLTYPE_UNUSED;
		dxelem[ elem_size ].Method = 0;
		dxelem[ elem_size ].Usage = 0;
		dxelem[ elem_size ].UsageIndex = 0;

		// 頂点ひとつ分のサイズ
		mVertexStride = offset;

		if ( is_dynamic_ )
		{
			mVertices = LN_NEW char[ mVertexStride * mVertexNum ];
		}
		else
		{
			// 頂点バッファ作成
            LN_DXCALL_R(
                mGraphicsDevice->getDX9Device()->CreateVertexBuffer(
				    mVertexStride * mVertexNum,
				    0, 0,
				    D3DPOOL_MANAGED,
				    &mVertexBuffer,
				    NULL ) );
		}

		// 頂点宣言作成
		LN_DXCALL_R( mGraphicsDevice->getDX9Device()->CreateVertexDeclaration( dxelem, &mVertexDecl ) );
		
		SAFE_DELETE( dxelem );

		// 頂点として設定するデータがある場合
		if ( data_ )
		{
			void* v = lock();
			memcpy_s( v, mVertexStride * mVertexNum, data_, mVertexStride * mVertexNum );
			unlock();
		}

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 初期化 (渡された IDirect3DVertexBuffer9 と D3DVERTEXELEMENT9 を参照する)
    //---------------------------------------------------------------------
    LNRESULT VertexBuffer::initialize( GraphicsDevice* device_, IDirect3DVertexBuffer9* device_obj_, D3DVERTEXELEMENT9* layout_ )
    {
        HRESULT hr;

        mGraphicsDevice = device_;
        LN_SAFE_ADDREF( mGraphicsDevice );

        mVertexBuffer = device_obj_;
        SAFE_ADDREF( mVertexBuffer );

        // 頂点宣言作成
		LN_DXCALL_R( mGraphicsDevice->getDX9Device()->CreateVertexDeclaration( layout_, &mVertexDecl ) );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● ロック
	//---------------------------------------------------------------------
	void* VertexBuffer::lock()
	{
		if ( mVertexBuffer )
		{
			void* v = NULL;
			mVertexBuffer->Lock( 0, 0, &v, 0 );
			return v;
		}
		return mVertices;
	}

	//---------------------------------------------------------------------
    // ● アンロック
	//---------------------------------------------------------------------
	void VertexBuffer::unlock()
	{
		if ( mVertexBuffer )
		{
			mVertexBuffer->Unlock();
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