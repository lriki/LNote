//=============================================================================
//【 DX9Renderer 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "GraphicsDevice.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Renderer.h"

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
// ■ Renderer クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    Renderer::Renderer()
        : mLogFile              ( NULL )
        , mGraphicsDevice       ( NULL )
        , mDxDevice             ( NULL )
        , mDxCaps               ( NULL )
        , mCurrentDepthBuffer   ( NULL )
        , mCurrentVertexBuffer  ( NULL )
        , mCurrentIndexBuffer   ( NULL )
        , mEnableScissorTest    ( false )
        , mSceneBegan           ( false )
    {
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    Renderer::~Renderer()
    {
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT Renderer::initialize( const InitData& init_data_ )
    {
        mGraphicsDevice = init_data_.GraphicsDevice;
        mDxDevice       = mGraphicsDevice->getDX9Device();
        mDxCaps         = init_data_.DxCaps;

        mCurrentRenderState.BlendMode   = LN_BLEND_ALPHA;
        mCurrentRenderState.Culling     = LN_CULL_FRONT;
        mCurrentRenderState.FillMode    = LN_FILL_SOLID;
        mCurrentRenderState.AlphaTest   = true;
        mCurrentRenderState.DepthTest   = true;
        mCurrentRenderState.DepthWrite  = true;
        //mCurrentRenderState.ScissorTest = false;

        D3DVIEWPORT9 vp;
        mDxDevice->GetViewport( &vp );

        setViewport( Geometry::Rect( 0, 0, vp.Width, vp.Height ) );

        return onResetDevice();
    }

    //---------------------------------------------------------------------
    // ● レンダリングステートの設定
    //---------------------------------------------------------------------
    LNRESULT Renderer::setRenderState( const LRenderState& state_ )
    {
        mRequestRenderState = state_;
        return _updateRenderState();
    }

    //---------------------------------------------------------------------
    // ● レンダリングターゲットの設定
    //---------------------------------------------------------------------
    LNRESULT Renderer::setRenderTarget( u32 index_, ITexture* texture_, bool reset_ )
    {
        LN_SETERR_R_S_( ( index_ < mDxCaps->NumSimultaneousRTs ), LN_ERR_FATAL,
            "< Renderer::setRenderTarget >\nindex_ = %d ( max = %d )", index_, mDxCaps->NumSimultaneousRTs );

        if ( mCurrentRenderTargets[ index_ ] != texture_ || reset_ )
        {
            LVector2 viewsize;

            // 0 番に NULL をセットするときはデフォルトのバックバッファにする
            if ( index_ == 0 && !texture_ )
            {
                texture_ = mGraphicsDevice->getDefaultCanvas()->getBackbuffer();
                mDxDevice->SetRenderTarget(
                    0,
                    ( (DX9TextureBase*)texture_ )->getDxSurface() );
                viewsize = texture_->getSize();
                mCurrentRenderTargets[ 0 ] = texture_;
            }
            else
            {
                if ( texture_ )
                {
                    mDxDevice->SetRenderTarget( index_, ( (DX9TextureBase*)texture_ )->getDxSurface() );
                }
                else
                {
                    mDxDevice->SetRenderTarget( index_, NULL );
                }

                mCurrentRenderTargets[ index_ ] = texture_;
                viewsize = mCurrentRenderTargets[ 0 ]->getRealSize();
            }

            // 0 番またはバックバッファのサイズをビューポートとして設定
            Geometry::Rect rc(
                0, 0,
                static_cast< int >( viewsize.x ),
                static_cast< int >( viewsize.y ) );

            setViewport( rc );
        }

        return LN_OK;


        // index_ が 0 以外に設定された場合、0 番に定すされている
        // テクスチャの解像度でビューポートを再設定するのが吉。

        // ここでは mCurrentRenderTargets[ 0 ] が NULL のときはバックバッファをそれとして扱うので
        // テクスチャの参照カウントを増やす必要はナシ。DxTexture として DxDevice に管理させる
    }

    //---------------------------------------------------------------------
	// ● 深度バッファの設定
    //---------------------------------------------------------------------
    LNRESULT Renderer::setDepthBuffer( ITexture* texture_ )
    {
        if ( texture_ != mCurrentDepthBuffer )
        {
            if ( !texture_ )
	        {
		        mDxDevice->SetDepthStencilSurface( NULL );
	        }
	        else
	        {
		        mDxDevice->SetDepthStencilSurface( ( (DX9TextureBase*)texture_ )->getDxSurface() );
	        }
            mCurrentDepthBuffer = texture_;
        }
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● ビューポート矩形の取得
	//---------------------------------------------------------------------
    const Geometry::Rect& Renderer::getViewport() const
	{
		return mCurrentViewportRect;
	}

    //---------------------------------------------------------------------
	// ● ビューポート矩形の設定
	//---------------------------------------------------------------------
	LNRESULT Renderer::setViewport( const Geometry::Rect& viewport_rect_ )
	{
		mCurrentViewportRect    = viewport_rect_;
        D3DVIEWPORT9 viewport;
	    viewport.X		= static_cast< DWORD >( viewport_rect_.x );
	    viewport.Y      = static_cast< DWORD >( viewport_rect_.y );
	    viewport.Width  = static_cast< DWORD >( viewport_rect_.w );
	    viewport.Height = static_cast< DWORD >( viewport_rect_.h );
		viewport.MinZ   = 0.0f;
		viewport.MaxZ   = 1.0f;
		HRESULT hr;
        
        LN_DXCALL_R( mDxDevice->SetViewport( &viewport ) );
        return LN_OK;
	}

    //---------------------------------------------------------------------
	// ● シザー矩形の取得
    //---------------------------------------------------------------------
    const Geometry::Rect& Renderer::getScissor() const
    {
        return mCurrentScissorRect;
    }

	//---------------------------------------------------------------------
	// ● シザー矩形の設定
    //---------------------------------------------------------------------
    LNRESULT Renderer::setScissor( const Geometry::Rect& rect_ )
    {
        bool b;

        if ( rect_ == mCurrentViewportRect )
        {
            b = false;
        }
        else if ( rect_.x == 0 && rect_.y == 0 && rect_.w == 0 && rect_.h == 0 )
        {
            b = false;
        }
        else
        {
            b = true;
        }

        //if ( b )//!= mEnableScissorTest )
	    {
		    const u32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, tb[ b ] );
            if ( b )
            {
                _setScissorRect( rect_ );
            }
            mEnableScissorTest = b;
	    }
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 頂点バッファの設定
	//---------------------------------------------------------------------
	LNRESULT Renderer::setVertexBuffer( IVertexBuffer* vertex_buffer_, bool reset_ )
	{
        VertexBuffer* vb = static_cast< VertexBuffer* >( vertex_buffer_ );
            //dynamic_cast< VertexBuffer* >( vertex_buffer_ );

		if ( vb == mCurrentVertexBuffer && !reset_ ) return LN_OK;

        if ( vb )
        {
		    HRESULT hr;
                
            // 頂点宣言設定
            LN_DXCALL_R(
                mDxDevice->SetVertexDeclaration( vb->getDxVertexDeclaration() ) );

		    // 頂点バッファを設定
            LN_DXCALL_R(
                mDxDevice->SetStreamSource( 0, vb->getDxVertexBuffer(), 0, vb->getVertexStride() ) );
        }


		mCurrentVertexBuffer = vb;

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● インデックスバッファの設定
	//---------------------------------------------------------------------
	LNRESULT Renderer::setIndexBuffer( IIndexBuffer* index_buffer_, bool reset_ )
	{
        IndexBuffer* ib = static_cast< IndexBuffer* >( index_buffer_ );
            //dynamic_cast< IndexBuffer* >( index_buffer_ );

		if ( ib == mCurrentIndexBuffer && !reset_ ) { 
	return LN_OK; }
		
        if ( ib )
        {
		    HRESULT hr;
            LN_DXCALL_R(
                mDxDevice->SetIndices( ib->getDxIndexBuffer() ) );
        }
		
		mCurrentIndexBuffer = ib;

        return LN_OK;
	}

    //---------------------------------------------------------------------
	// ● 設定されているカラーバッファ、深度バッファをクリアする
    //---------------------------------------------------------------------
	LNRESULT Renderer::clear( bool target_, bool depth_, u32 color_, lnfloat z_ )
    {
        HRESULT hr;
        u32 flag = 0;
        if ( target_ ) flag |= D3DCLEAR_TARGET;  
        if ( mCurrentDepthBuffer && depth_ ) flag |= ( D3DCLEAR_ZBUFFER );// | D3DCLEAR_STENCIL

        if ( flag == 0 ) return LN_OK;

        //D3DVIEWPORT9 vp;
        //mDxDevice->GetViewport(&vp);
        //vp.Width = 640;
        //vp.Height = 480;
        //mDxDevice->SetViewport(&vp);


        LN_DXCALL_R(
            mDxDevice->Clear( 0, NULL, flag, color_, z_, 0x00000000 ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● プリミティブ描画
	//---------------------------------------------------------------------
	LNRESULT Renderer::drawPrimitive( LNPrimitiveType primitive_, u32 start_vertex_, u32 primitive_count_ )
	{
        LN_SETERR_R_( ( mCurrentVertexBuffer ), LN_ERR_ASSERT, "< Renderer::drawPrimitive >\n\n頂点バッファがセットされていません。" );
		
        HRESULT hr;
		D3DPRIMITIVETYPE dx_prim = D3DPT_TRIANGLELIST;
		switch ( primitive_ )
		{
			//case LN_PRIMITIVE_TRIANGLELIST:   dx_prim = D3DPT_TRIANGLELIST; break;
			case LN_PRIM_TRIANGLESTRIP:    
				dx_prim = D3DPT_TRIANGLESTRIP; break;
			case LN_PRIM_LINELIST:         
				dx_prim = D3DPT_LINELIST; break;
            case LN_PRIM_LINESTRIP:        
            	dx_prim = D3DPT_LINESTRIP; break;
			case LN_PRIM_POINTLIST:        
				dx_prim = D3DPT_POINTLIST; break;
		}

		// 動的な頂点バッファとして作成している場合は DrawPrimitiveUP で描画する
		if ( mCurrentVertexBuffer->isDynamic() )
		{
            const char* vertices = (const char*)mCurrentVertexBuffer->lock();
            u32 stride = mCurrentVertexBuffer->getVertexStride();
            vertices += stride * start_vertex_;

			LN_DXCALL_R(
                mDxDevice->DrawPrimitiveUP(
				    dx_prim,
				    primitive_count_,
				    vertices,
                    stride ) );
		}
		else
		{
            LN_DXCALL_R(
                mDxDevice->DrawPrimitive( 
				    dx_prim,
				    start_vertex_,
				    primitive_count_ ) );
		}
        
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● プリミティブ描画 ( インデックス付き )
	//---------------------------------------------------------------------
	LNRESULT Renderer::drawPrimitiveIndexed( LNPrimitiveType prim_, u32 start_index_, u32 primitive_count_ )
	{
		LN_SETERR_R_( ( mCurrentVertexBuffer ), LN_ERR_ASSERT, "< Renderer::drawPrimitiveIndexed >\n\n頂点バッファがセットされていません。" );
		LN_SETERR_R_( ( mCurrentIndexBuffer ),  LN_ERR_ASSERT, "< Renderer::drawPrimitiveIndexed >\n\nインデックスバッファがセットされていません。" );
        LN_SETERR_R_( ( start_index_ < mCurrentIndexBuffer->getIndexNum() ),
            LN_ERR_INVALIDCALL, "< Renderer::drawPrimitiveIndexed >\n\nstart_index_ : %d", start_index_ );

        HRESULT hr;
		D3DPRIMITIVETYPE dx_prim = D3DPT_TRIANGLELIST;
		switch ( prim_ )
		{
			//case LN_PRIMITIVE_TRIANGLELIST:     dx_prim = D3DPT_TRIANGLELIST; break;
			case LN_PRIM_TRIANGLESTRIP:    
				dx_prim = D3DPT_TRIANGLESTRIP; break;
			case LN_PRIM_LINELIST:         
				dx_prim = D3DPT_LINELIST; break;
            case LN_PRIM_LINESTRIP:        
            	dx_prim = D3DPT_LINESTRIP; break;
			case LN_PRIM_POINTLIST:        
				dx_prim = D3DPT_POINTLIST; break;
		}

		bool vd = mCurrentVertexBuffer->isDynamic();
		bool id = mCurrentIndexBuffer->isDynamic();

		// 両方が dynamic
		if ( vd && id )
		{
            D3DFORMAT ib_fmt;
            const void* index_data;
            if ( mCurrentIndexBuffer->getIndexStride() == 2 )
            {
                ib_fmt = D3DFMT_INDEX16;
                index_data = &((lnU16*)mCurrentIndexBuffer->lock())[ start_index_ ];
            }
            else
            {
                ib_fmt = D3DFMT_INDEX32;
                index_data = &((lnU32*)mCurrentIndexBuffer->lock())[ start_index_ ];
            }

            //LN_DXCALL_R(
            LN_COMCALL( 
                mDxDevice->DrawIndexedPrimitiveUP(
				    dx_prim,
				    0,
				    mCurrentVertexBuffer->getVertexNum(),
				    primitive_count_,
				    index_data,
				    ib_fmt,
				    mCurrentVertexBuffer->lock(),
				    mCurrentVertexBuffer->getVertexStride() ) );
		}
        // 両方が static
		else if ( !vd && !id )
		{
            LN_DXCALL_R(
			    mDxDevice->DrawIndexedPrimitive( 
				    dx_prim,
				    0,
				    0,
				    mCurrentVertexBuffer->getVertexNum(),
				    start_index_,
				    primitive_count_ ) );
		}
        return LN_OK;
	}

    //---------------------------------------------------------------------
    // ● シーン開始
    //---------------------------------------------------------------------
	LNRESULT Renderer::beginScene()
    {
        // 描画開始済みフラグが ON 場合は中断
		if ( mSceneBegan ) return LN_OK;

		HRESULT hr;
        
        LN_DXCALL_R( mDxDevice->BeginScene() );

        mDxDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0 );

		mSceneBegan = true;

        return LN_OK;
    }

	//---------------------------------------------------------------------
    // ● シーン終了
    //---------------------------------------------------------------------
	LNRESULT Renderer::endScene()
    {
        HRESULT hr;

        // 描画開始済みフラグが OFF 場合は中断
		if ( !mSceneBegan ) return LN_OK;

        mSceneBegan = false;
 
        LN_DXCALL_R( mDxDevice->EndScene() );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● デバイスリセット直前 GraphicsDevice から呼ばれる
    //---------------------------------------------------------------------
    LNRESULT Renderer::onLostDevice()
    {
        mCurrentVertexBuffer = NULL;
        mCurrentIndexBuffer  = NULL;
        mCurrentDepthBuffer  = NULL;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● デバイスリセット後に GraphicsDevice から呼ばれる
    //---------------------------------------------------------------------
    LNRESULT Renderer::onResetDevice()
    {
        LNRESULT lr;

        

        LN_CALL_R( _initRenderState() );
        LN_CALL_R( _resetRenderState() );

        memset( mCurrentRenderTargets, 0, sizeof( mCurrentRenderTargets ) );

        LN_CALL_R( setRenderTarget( 0, NULL, true ) );
           
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 後で変更しないレンダリングステートをデバイスに設定する
    //---------------------------------------------------------------------
    LNRESULT Renderer::_initRenderState()
    {
        HRESULT hr = S_OK;

        mDxDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE );
        //printf("aa\n");
		// アルファテストの時の基準アルファ値 ( Clearで塗りつぶす色に透明色が使えなく云々 )
		//hr += mDxDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000001 );
		// アルファテスト時の比較関数の指定 ( D3DCMPFUNC 列挙型  デフォルトは D3DCMP_ALWAYS )
		hr += mDxDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
		// アルファブレンド有効
		//hr += mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		// 法線の正規化を設定する ( ちょっと時間かかるっぽい )
		//hr += mDxDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );

        /*
		// カリングモードの設定
		hr += mDxDevice->SetRenderState( D3DRS_CULLMODE, mCurrentCullMode );
		// Zバッファリングの有効設定
		hr += mDxDevice->SetRenderState( D3DRS_ZENABLE, mCurrentDepthTest );

		// Zバッファへの書き込み許可設定
		hr += mDxDevice->SetRenderState( D3DRS_ZWRITEENABLE, mCurrentDepthWrite );
		// アルファテストの有効設定
		hr += mDxDevice->SetRenderState( D3DRS_ALPHATESTENABLE, mCurrentAlphaTest );
		// テクスチャ縮小の補正
		hr += mDxDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, mCurrentMinFilter );
		// テクスチャ拡大の補正
		hr += mDxDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, mCurrentMagFilter );
		// ミップフィルタ
		hr += mDxDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, mCurrentMipFilter );
		// 異方性
		hr += mDxDevice->SetSamplerState( 0, D3DSAMP_MAXANISOTROPY, mMaxAnisotropy );
        */
		
		// テクスチャブレンディングの設定
		// テクスチャと
		hr += mDxDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		// 頂点色の
		hr += mDxDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		// 色を乗算
		hr += mDxDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		// テクスチャと
		hr += mDxDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		// 頂点色の
		hr += mDxDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		// アルファ値を乗算
		hr += mDxDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 実際にレンダリングステートを設定する
    //---------------------------------------------------------------------
    LNRESULT Renderer::_updateRenderState()
    {
        // 合成方法
	    if ( mRequestRenderState.BlendMode != mCurrentRenderState.BlendMode )
	    {
		    switch ( mRequestRenderState.BlendMode )
		    {
		        case LN_BLEND_NORMAL:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  127 );
			        break;
		        case LN_BLEND_ALPHA:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  1 );

                    /*
                    mDxDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
					mDxDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
					mDxDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
					mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
					
					// アルファ個別ブレンド有効
					mDxDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
					mDxDevice->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_SRCALPHA );
					mDxDevice->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_DESTALPHA );
			        */
                    break;
		        case LN_BLEND_ADD:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_ADD_ALPHADISABLE:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_SUB:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_REVSUBTRACT );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_SUB_ALPHADISABLE:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_REVSUBTRACT );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_MUL:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_SCREEN:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_INVDESTCOLOR );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		    }
	    }

        // カリング
	    if ( mRequestRenderState.Culling != mCurrentRenderState.Culling )
	    {
		    const u32 tb[] = { D3DCULL_CCW, D3DCULL_CW, D3DCULL_NONE };
		    mDxDevice->SetRenderState( D3DRS_CULLMODE, tb[ mRequestRenderState.Culling ] );
	    }

        // 塗りつぶし方法
	    if ( mRequestRenderState.FillMode != mCurrentRenderState.FillMode )
	    {
		    const u32 tb[] = { D3DFILL_SOLID, D3DFILL_WIREFRAME, D3DFILL_POINT };
		    mDxDevice->SetRenderState( D3DRS_FILLMODE, tb[ mRequestRenderState.FillMode ] );
	    }
	    
	    // アルファテスト
	    if ( mRequestRenderState.AlphaTest != mCurrentRenderState.AlphaTest )
	    {
		    const u32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_ALPHATESTENABLE, tb[ mRequestRenderState.AlphaTest ] );
	    }
	    // 深度テスト
	    if ( mRequestRenderState.DepthTest != mCurrentRenderState.DepthTest )
	    {
		    const u32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_ZENABLE, tb[ mRequestRenderState.DepthTest ] );
	    }
	    // 深度書き込み
        if ( mRequestRenderState.DepthWrite != mCurrentRenderState.DepthWrite )
	    {
		    const u32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_ZWRITEENABLE, tb[ mRequestRenderState.DepthWrite ] );
	    }
        // ポイントスプライト
        if ( mRequestRenderState.PointSprite != mCurrentRenderState.PointSprite )
	    {
		    const u32 tb[] = { FALSE, TRUE };
            mDxDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, tb[ mRequestRenderState.PointSprite ] );
	    }
	   
        // シザーテスト
        
     //   if ( mRequestRenderState.ScissorTest != mCurrentRenderState.ScissorTest )
	    //{
		   // const u32 tb[] = { FALSE, TRUE };
		   // mDxDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, tb[ mRequestRenderState.ScissorTest ] );
     //       _setScissorRect( mRequestRenderState.ScissorRect );
	    //}
        

        mCurrentRenderState = mRequestRenderState;

        /*
        switch ( type_ )
			{
				case BLEND_ADD:		// 加算
					mDxDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
					mDxDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
					//hr = mDxDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR );
					mDxDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA ); // こっちの方がなめらかだけど・・？
					mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
					mDxDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, FALSE );
					break;

				case BLEND_SUB:		// 減算
					mDxDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
					mDxDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT );
					mDxDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
					mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
					mDxDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, FALSE );
					break;

				case BLEND_REVERSE:	// リバース
					mDxDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
					mDxDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
					mDxDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR );
					mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR ); 
					mDxDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, FALSE );
					break;

				default:	// 通常 ( 半透明 )
					mDxDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
					mDxDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
					mDxDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
					mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
					
					// アルファ個別ブレンド有効
					mDxDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
					mDxDevice->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_SRCALPHA );
					mDxDevice->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_DESTALPHA );
					break;
			}
            */

        return LN_OK;
    }


    //---------------------------------------------------------------------
    // ● 現在のレンダリングステートをデバイスに再設定する
    //---------------------------------------------------------------------
    LNRESULT Renderer::_resetRenderState()
    {
        // 合成方法
	    {
		    switch ( mCurrentRenderState.BlendMode )
		    {
		        case LN_BLEND_NORMAL:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  127 );
			        break;
		        case LN_BLEND_ALPHA:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  1 );
			        break;
		        case LN_BLEND_ADD:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_ADD_ALPHADISABLE:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_SUB:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_REVSUBTRACT );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_SUB_ALPHADISABLE:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_REVSUBTRACT );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_MUL:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_SCREEN:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_INVDESTCOLOR );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
                case LN_BLEND_REVERSE:
                    mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
                    mDxDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
					mDxDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR );
					mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
                    mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
                    /*
					mDxDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
					mDxDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
					mDxDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR );
					mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR ); 
					mDxDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, FALSE );
                    */
					break;
		    }
	    }

        // カリング
	    {
		    const u32 tb[] = { D3DCULL_CCW, D3DCULL_CW, D3DCULL_NONE };
		    mDxDevice->SetRenderState( D3DRS_CULLMODE, tb[ mCurrentRenderState.Culling ] );
	    }

        // 塗りつぶし方法
	    {
		    const u32 tb[] = { D3DFILL_SOLID, D3DFILL_WIREFRAME, D3DFILL_POINT };
		    mDxDevice->SetRenderState( D3DRS_FILLMODE, tb[ mCurrentRenderState.FillMode ] );
	    }
	    
	    // アルファテスト
	    {
		    const u32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_ALPHATESTENABLE, tb[ mCurrentRenderState.AlphaTest ] );
	    }
	    // 深度テスト
	    {
		    const u32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_ZENABLE, tb[ mCurrentRenderState.DepthTest ] );
	    }
	    // 深度書き込み
	    {
		    const u32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_ZWRITEENABLE, tb[ mCurrentRenderState.DepthWrite ] );
	    }
        // ポイントスプライト
        if ( mRequestRenderState.PointSprite != mCurrentRenderState.PointSprite )
	    {
		    const u32 tb[] = { FALSE, TRUE };
            mDxDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, tb[ mRequestRenderState.PointSprite ] );
	    }
	   
	   
        // シザーテスト
	    //{
		   // const u32 tb[] = { FALSE, TRUE };
		   // mDxDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, tb[ mEnableScissorTest ] );
     //       _setScissorRect( mScissorRect );
	    //}

        float pt_size = 5.0f;
        mDxDevice->SetRenderState( D3DRS_POINTSIZE, *((DWORD*)&pt_size) );
        
        return LN_OK;
    }


    //---------------------------------------------------------------------
    // ● シザー矩形の設定
    //---------------------------------------------------------------------
    LNRESULT Renderer::_setScissorRect( const Geometry::Rect& rect_ )
    {
        RECT rect = {
		    rect_.x,
		    rect_.y,
		    rect_.x + rect_.w,
		    rect_.y + rect_.h,
	    };
        HRESULT hr;
        LN_DXCALL_R( mDxDevice->SetScissorRect( &rect ) );

        mCurrentScissorRect = rect_;
        return LN_OK;
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