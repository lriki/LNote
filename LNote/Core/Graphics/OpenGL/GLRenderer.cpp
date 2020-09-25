//=============================================================================
//【 GLRenderer 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "LNGL/LNGL.h"
#include "GLPlatformContext.h"
#include "GLGraphicsDevice.h"
#include "GLVertexBuffer.h"
#include "GLIndexBuffer.h"
#include "GLCanvas.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "GLRenderer.h"

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

//=============================================================================
// ■ Renderer クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    Renderer::Renderer()
        : mLogFile              ( NULL )
        , mGraphicsDevice       ( NULL )
        //, mPlatformContext      ( NULL )
        , mGLFramebuffer        ( NULL )
        , mCurrentDepthBuffer   ( NULL )
        , mCurrentVertexBuffer  ( NULL )
        , mCurrentIndexBuffer   ( NULL )
        , mEnableScissorTest    ( false )
        , mSceneBegan           ( false )
        , mIsDeviceLost         ( false )
    {
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    Renderer::~Renderer()
    {
        onLostDevice();
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    void Renderer::initialize( const InitData& init_data_ )
    {
        mGraphicsDevice  = init_data_.GraphicsDevice;
        //mPlatformContext = init_data_.PlatformContext;
       
        mCurrentRenderState.BlendMode   = LN_BLEND_ALPHA;
        mCurrentRenderState.Culling     = LN_CULL_FRONT;
        mCurrentRenderState.FillMode    = LN_FILL_SOLID;
        mCurrentRenderState.AlphaTest   = true;
        mCurrentRenderState.DepthTest   = true;
        mCurrentRenderState.DepthWrite  = true;

        mIsDeviceLost = true;
        onResetDevice();
    }

    //---------------------------------------------------------------------
    // ● レンダリングステートの設定
    //---------------------------------------------------------------------
    LNRESULT Renderer::setRenderState( const LRenderState& state_ )
    {
        mRequestRenderState = state_;
        return _updateRenderState( false );
    }

    //---------------------------------------------------------------------
    // ● レンダリングターゲットの設定
    //---------------------------------------------------------------------
    LNRESULT Renderer::setRenderTarget( lnU32 index_, ITexture* texture_, bool reset_ )
    {
        LN_THROW_InvalidCall( index_ < LN_MAX_MULTI_RENDERTARGETS, "index : %d", index_ );

        // カラーバッファの設定
        if ( mCurrentRenderTargets[ index_ ] != texture_ || reset_ )
        {
            mCurrentRenderTargets[ index_ ] = texture_;

            //_refreshFramebuffer();
        }

        // カレントのターゲットサイズを更新
        if ( mCurrentRenderTargets[0] )
        {
            const LVector2& size = mCurrentRenderTargets[0]->getSize();
            const LVector2& realsize = mCurrentRenderTargets[0]->getRealSize();
            mCurrentRenderTarget0Size.w = static_cast< int >( size.x );
            mCurrentRenderTarget0Size.h = static_cast< int >( size.y );
            mCurrentRenderTarget0RealSize.w = static_cast< int >( realsize.x );
            mCurrentRenderTarget0RealSize.h = static_cast< int >( realsize.y );
        }
        else
        {
            mCurrentRenderTarget0Size = mCurrentRenderTarget0RealSize = ((DefaultCanvas*)mGraphicsDevice->getDefaultCanvas())->getWindowSize();
        }

        // ビューポートリセット
        setViewport( Geometry::Rect::ZERO );

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 深度バッファの設定
    //---------------------------------------------------------------------
    LNRESULT Renderer::setDepthBuffer( ITexture* texture_ )
    {
        if ( texture_ != mCurrentDepthBuffer )
        {
            mCurrentDepthBuffer = texture_;

            //_refreshFramebuffer();
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
        mCurrentViewportRect = viewport_rect_;

        if ( mCurrentViewportRect == Geometry::Rect::ZERO )
        {
            mCurrentViewportRealRect.x = 0;
            mCurrentViewportRealRect.y = 0;
            mCurrentViewportRealRect.w = mCurrentRenderTarget0RealSize.w;
            mCurrentViewportRealRect.h = mCurrentRenderTarget0RealSize.h;
        }
        else
        {
            mCurrentViewportRealRect.x = mCurrentRenderTarget0RealSize.w * viewport_rect_.x / mCurrentRenderTarget0Size.w;
            mCurrentViewportRealRect.w = mCurrentRenderTarget0RealSize.w * viewport_rect_.w / mCurrentRenderTarget0Size.w;
            mCurrentViewportRealRect.y = mCurrentRenderTarget0RealSize.h * viewport_rect_.y / mCurrentRenderTarget0Size.h;
            mCurrentViewportRealRect.h = mCurrentRenderTarget0RealSize.h * viewport_rect_.h / mCurrentRenderTarget0Size.h;
        }

        if (mIsDeviceLost) return LN_OK;

        LNGL::glViewport(
            mCurrentViewportRealRect.x,
            mCurrentRenderTarget0RealSize.h - mCurrentViewportRealRect.y - mCurrentViewportRealRect.h,
            mCurrentViewportRealRect.w,
            mCurrentViewportRealRect.h );
        //LN_CHECK_GLERROR();

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
        mEnableScissorTest = ( rect_ != Geometry::Rect::ZERO );

        Geometry::Rect real_rect;
        if ( mEnableScissorTest )
        {
            real_rect.x = mCurrentRenderTarget0RealSize.w * rect_.x / mCurrentRenderTarget0Size.w;
            real_rect.w = mCurrentRenderTarget0RealSize.w * rect_.w / mCurrentRenderTarget0Size.w;
            real_rect.y = mCurrentRenderTarget0RealSize.h * rect_.y / mCurrentRenderTarget0Size.h;
            real_rect.h = mCurrentRenderTarget0RealSize.h * rect_.h / mCurrentRenderTarget0Size.h;
        }
       
        mCurrentScissorRect = rect_;

        if (mIsDeviceLost) return LN_OK;

        if ( mEnableScissorTest )
        {
            LNGL::glEnable( GL_SCISSOR_TEST );
            //LN_CHECK_GLERROR();

            LNGL::glScissor(
                real_rect.x,
                mCurrentRenderTarget0RealSize.h - real_rect.y - real_rect.h,
                real_rect.w,
                real_rect.h );
            //LN_CHECK_GLERROR();
        }
        else
        {
            LNGL::glDisable( GL_SCISSOR_TEST );
            //LN_CHECK_GLERROR();
        }

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 頂点バッファの設定
	//---------------------------------------------------------------------
	LNRESULT Renderer::setVertexBuffer( IVertexBuffer* vertex_buffer_, bool reset_ )
	{
        VertexBuffer* vb = static_cast< VertexBuffer* >( vertex_buffer_ );

        if ( vb == mCurrentVertexBuffer && !reset_ ) return LN_OK;

        if ( vb )
        {
            //glBindBuffer( GL_ARRAY_BUFFER, vb->getGLVertexBuffer() );
            //LN_CHECK_GLERROR();
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

        if ( ib == mCurrentIndexBuffer && !reset_ ) return LN_OK;

        //if ( ib )
        //{
        //    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ib->getGLIndexBuffer() );
        //    LN_CHECK_GLERROR();
        //}

        mCurrentIndexBuffer = ib;

        return LN_OK;
	}

    //---------------------------------------------------------------------
	// ● 設定されているカラーバッファ、深度バッファをクリアする
    //---------------------------------------------------------------------
	LNRESULT Renderer::clear( bool target_, bool depth_, u32 color_, lnfloat z_ )
    {
        if (mIsDeviceLost) return LN_OK;

        _refreshFramebuffer();

        LColor c( color_, LN_GRAPHICSAPI_OPENGL );
        LNGL::lnClear(target_, depth_, &c, z_);

        LNGL::glBindFramebuffer( GL_FRAMEBUFFER, 0 );
#if 0
        // クリア色を設定
        LColor c( color_, LN_GRAPHICSAPI_OPENGL );
        LNGL::glClearColor( c.red, c.green, c.blue, c.alpha );
        //LN_CHECK_GLERROR();

        // クリア深度値を設定
        LNGL::glClearDepth( z_ );
        //LN_CHECK_GLERROR();

        // ステンシル値
        //glClearStencil( 0 );
        //LN_CHECK_GLERROR();

        // カラーバッファとデプスバッファをクリア
        LNGL::glClear(
            ( ( target_ ) ? GL_COLOR_BUFFER_BIT : 0 ) | 
            ( ( depth_  ) ? GL_DEPTH_BUFFER_BIT : 0 ) );// |
            //GL_STENCIL_BUFFER_BIT );
        //LN_CHECK_GLERROR();
#endif
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● プリミティブ描画
	//---------------------------------------------------------------------
	LNRESULT Renderer::drawPrimitive( LNPrimitiveType primitive_, u32 start_vertex_, u32 primitive_count_ )
	{
        if (mIsDeviceLost) return LN_OK;

        ShaderPass* current_pass = mGraphicsDevice->getCurrentShaderrPass();

        if ( !mCurrentVertexBuffer || !current_pass ) return LN_OK;

        mCurrentVertexBuffer->commitChanges();

        

        LNGL::glBindBuffer( GL_ARRAY_BUFFER, mCurrentVertexBuffer->getGLVertexBuffer() );
        //LN_CHECK_GLERROR();

        LNGL::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
        //LN_CHECK_GLERROR();

        // シェーダの頂点属性の更新
        int i = 0;
        LNGLVertexAttribPointerData* attrs = mCurrentVertexBuffer->getGLVertexAttribPointerData();
        while ( 1 )
        {
            if ( !attrs[i].Type ) break;
            GLint loc = current_pass->getUsageEntryLocation( attrs[i].Usage );

            if ( loc == -1 )
            {
                ++i;
                continue;
            }

            LNGL::glVertexAttribPointer(
                loc,
                attrs[i].Size,
                attrs[i].Type,
                attrs[i].Normalized,
                attrs[i].Stride,
                (void*)(attrs[i].ByteOffset) );
            //LN_CHECK_GLERROR();

            LNGL::glEnableVertexAttribArray( loc );
            //LN_CHECK_GLERROR();

            ++i;
        }

        // プリミティブ選択
        GLenum gl_prim;
        lnU32 v_count;
        switch ( primitive_ )
		{
            default:
			case LN_PRIM_TRIANGLELIST:
                gl_prim = GL_TRIANGLES;
                v_count = primitive_count_ * 3;
                break;
			case LN_PRIM_TRIANGLESTRIP:
                gl_prim = GL_TRIANGLE_STRIP;
                v_count = 2 + primitive_count_;
                break;
			case LN_PRIM_LINELIST:
                gl_prim = GL_LINES;
                v_count = primitive_count_ * 2;
                break;
            case LN_PRIM_LINESTRIP:
                gl_prim = GL_LINE_STRIP;
                v_count = 1 + primitive_count_;
                break;
			case LN_PRIM_POINTLIST:
                gl_prim = GL_POINTS;
                v_count = primitive_count_;
                break;
		}

        if ( v_count > mCurrentVertexBuffer->getVertexNum() )
        {
            v_count = mCurrentVertexBuffer->getVertexNum();
        }

        _refreshFramebuffer();

        // 描画
        LNGL::glDrawArrays( gl_prim, start_vertex_, v_count );

        LNGL::glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        //LN_CHECK_GLERROR();

//printf( "rrd%d\n", glGetError() );
        /*
        glDrawElements( GL_QUADS, sizeof(lnU16), GL_UNSIGNED_INT, NULL );

        glDrawElements(GL_TRIANGLE_STRIP, torusNumPrimVtx, 
                        GL_UNSIGNED_SHORT, (void *)offset);
                        */

        //glDisableVertexAttribArray( 0 );
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● プリミティブ描画 ( インデックス付き )
	//---------------------------------------------------------------------
	LNRESULT Renderer::drawPrimitiveIndexed( LNPrimitiveType primitive_, u32 start_index_, u32 primitive_count_ )
	{
        if (mIsDeviceLost) return LN_OK;

        ShaderPass* current_pass = mGraphicsDevice->getCurrentShaderrPass();

        if ( !mCurrentVertexBuffer || !current_pass ) return LN_OK;

        mCurrentVertexBuffer->commitChanges();

        LNGL::glBindBuffer( GL_ARRAY_BUFFER, mCurrentVertexBuffer->getGLVertexBuffer() );

        

        // シェーダの頂点属性の更新
        int i = 0;
        LNGLVertexAttribPointerData* attrs = mCurrentVertexBuffer->getGLVertexAttribPointerData();
        while ( 1 )
        {
            if ( !attrs[i].Type ) break;
            GLint loc = current_pass->getUsageEntryLocation( attrs[i].Usage );

            if ( loc == -1 )
            {
                ++i;
                continue;
            }

            LNGL::glVertexAttribPointer(
                loc,
                attrs[i].Size,
                attrs[i].Type,
                attrs[i].Normalized,
                attrs[i].Stride,
                (void*)(attrs[i].ByteOffset) );
            //LN_CHECK_GLERROR();

            LNGL::glEnableVertexAttribArray( loc );
            //LN_CHECK_GLERROR();

            ++i;
        }


        mCurrentIndexBuffer->commitChanges();
        LNGL::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mCurrentIndexBuffer->getGLIndexBuffer() );

        // プリミティブ選択
        GLenum gl_prim;
        lnU32 v_count;
        switch ( primitive_ )
		{
            default:
			case LN_PRIM_TRIANGLELIST:
                gl_prim = GL_TRIANGLES;
                v_count = primitive_count_ * 3;
                break;
			case LN_PRIM_TRIANGLESTRIP:
                gl_prim = GL_TRIANGLE_STRIP;
                v_count = 2 + primitive_count_;
                break;
			case LN_PRIM_LINELIST:
                gl_prim = GL_LINES;
                v_count = primitive_count_ * 2;
                break;
            case LN_PRIM_LINESTRIP:
                gl_prim = GL_LINE_STRIP;
                v_count = 1 + primitive_count_;
                break;
			case LN_PRIM_POINTLIST:
                gl_prim = GL_POINTS;
                v_count = primitive_count_;
                break;
		}

        if ( v_count > mCurrentVertexBuffer->getVertexNum() )
        {
            v_count = mCurrentVertexBuffer->getVertexNum();
        }

        _refreshFramebuffer();

        /*LNGL::glDrawElements(
            gl_prim,
            mCurrentIndexBuffer->getIndexNum(),
            (mCurrentIndexBuffer->getIndexStride() == 2) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
            NULL );*/

        // 引数 start end には、本来であれば0～v_countまでのインデックスの中の最大、最小の値を渡す。
        // http://wiki.livedoor.jp/mikk_ni3_92/d/glDrawRangeElements%A4%CB%A4%E8%A4%EB%C9%C1%B2%E8
        // ただ、全範囲を渡しても特に問題なさそうなのでこのまま。
        if (mCurrentIndexBuffer->getIndexStride() == 2)
        {
            LNGL::glDrawElements(
                gl_prim,
                v_count,
                GL_UNSIGNED_SHORT,
                (GLvoid*) (sizeof(GLushort) * start_index_));
        }
        else
        {
            LNGL::glDrawElements(
                gl_prim,
                v_count,
                GL_UNSIGNED_INT,
                (GLvoid*) (sizeof(GLuint) * start_index_));
        }

        LNGL::glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        /*
        LNGL::glDrawRangeElements(
            gl_prim, 
            start_index_,
            mCurrentIndexBuffer->getIndexNum(),
            v_count,    // インデックスの数
            (mCurrentIndexBuffer->getIndexStride() == 2) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
            NULL );
        */


        return LN_OK;
	}

    //---------------------------------------------------------------------
    // ● シーン開始
    //---------------------------------------------------------------------
	LNRESULT Renderer::beginScene()
    {
        if ( mSceneBegan ) return LN_OK;

        if (!mIsDeviceLost)
        {
            //mPlatformContext->beginScene();
            LNGL::beginScene();
            mGraphicsDevice->commitChangesGLResource();
        }

        mSceneBegan = true;
        return LN_OK;
    }

	//---------------------------------------------------------------------
    // ● シーン終了
    //---------------------------------------------------------------------
	LNRESULT Renderer::endScene()
    {
        if ( !mSceneBegan ) return LN_OK;

        if (!mIsDeviceLost)
        {
            //mPlatformContext->endScene();
            LNGL::glBindFramebuffer(GL_FRAMEBUFFER, 0);
            LNGL::endScene();
        }

        mSceneBegan = false;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● デバイスリセット直前 GraphicsDevice から呼ばれる
    //---------------------------------------------------------------------
    void Renderer::onLostDevice()
    {
        if (!mIsDeviceLost)
        {
            setVertexBuffer( NULL );
            setIndexBuffer( NULL );
            setDepthBuffer( NULL );
            for ( int i = 0; i < LN_MAX_MULTI_RENDERTARGETS; ++i )
            {
                setRenderTarget( i, NULL );
            }

            if ( mGLFramebuffer )
            {
                LNGL::glDeleteFramebuffers( 1, &mGLFramebuffer );
                //glDeleteFramebuffersEXT( 1, &mGLFramebuffer );
                //glGetError();
            }

            mIsDeviceLost = true;
        }
    }

    //---------------------------------------------------------------------
    // ● デバイスリセット後に GraphicsDevice から呼ばれる
    //---------------------------------------------------------------------
    void Renderer::onResetDevice()
    {
        if (mIsDeviceLost)
        {
            _initRenderState();
            _updateRenderState( true );

            /*
            _p(glGetError());
            
            PFNGLGENFRAMEBUFFERSOESPROC m_glGenFramebuffersOES = (PFNGLGENFRAMEBUFFERSOESPROC)eglGetProcAddress( "glGenFramebuffersOES" );
            _p(m_glGenFramebuffersOES);
            
            mGLFramebuffer = 0;
            m_glGenFramebuffersOES( 1, &mGLFramebuffer );
            _p(mGLFramebuffer);
            _p(glGetError());
            
            
            mGLFramebuffer = 0;
            glGenFramebuffers( 1, &mGLFramebuffer );
            _p(mGLFramebuffer);
            _p(glGetError());
            //glGenFramebuffersEXT( 1, &mGLFramebuffer );
            //LN_CHECK_GLERROR();
            */

            LNGL::glGenFramebuffers( 1, &mGLFramebuffer );
            //LN_CHECK_GLERROR();

            //memset( mCurrentRenderTargets, 0, sizeof( mCurrentRenderTargets ) );
            //setRenderTarget( 0, NULL );
            //setDepthBuffer(NULL);

            mIsDeviceLost = false;
        }
    }

    //---------------------------------------------------------------------
    // ● 後で変更しないレンダリングステートをデバイスに設定する
    //---------------------------------------------------------------------
    void Renderer::_initRenderState()
    {
        //if (mIsDeviceLost) return;

        // デフォルトの値は GL_ALWAYS, 0.0
        //glAlphaFunc( GL_GREATER, 0.01f );
        //LN_CHECK_GLERROR();
        
        // 右回りの面を正面とする (DirectX に合わせる)
        LNGL::glFrontFace( GL_CW );
        //LN_CHECK_GLERROR();
        
        // カリング有効
        LNGL::glEnable( GL_CULL_FACE );
        //LN_CHECK_GLERROR();
    }

    //---------------------------------------------------------------------
    // ● 実際にレンダリングステートを設定する
    //---------------------------------------------------------------------
    LNRESULT Renderer::_updateRenderState( bool reset_ )
    {
        //if (mIsDeviceLost) return LN_OK;

        LNGL::lnSetRenderState(&mRequestRenderState);
#if 0
        // 合成方法
	    if ( mRequestRenderState.BlendMode != mCurrentRenderState.BlendMode || reset_ )
	    {
		    switch ( mRequestRenderState.BlendMode )
		    {
		        case LN_BLEND_NORMAL:
                    LNGL::glDisable( GL_BLEND );         //         LN_CHECK_GLERROR();
		            LNGL::glBlendEquation( GL_FUNC_ADD );//         LN_CHECK_GLERROR();
		            LNGL::glBlendFunc( GL_ONE, GL_ZERO );//         LN_CHECK_GLERROR();
		            //glAlphaFunc( GL_GREATER, 0.5f );        LN_CHECK_GLERROR();
			        break;
		        case LN_BLEND_ALPHA:
		            LNGL::glEnable( GL_BLEND );                               //    LN_CHECK_GLERROR();
		            LNGL::glBlendEquation( GL_FUNC_ADD );                     //    LN_CHECK_GLERROR();
		            LNGL::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );//    LN_CHECK_GLERROR();
		            //glAlphaFunc( GL_GREATER, 0.01f );                       LN_CHECK_GLERROR();
                    break;
		        case LN_BLEND_ADD:
		            LNGL::glEnable( GL_BLEND );                  // LN_CHECK_GLERROR();
		            LNGL::glBlendEquation( GL_FUNC_ADD );        // LN_CHECK_GLERROR();
		            LNGL::glBlendFunc( GL_SRC_ALPHA, GL_ONE );   // LN_CHECK_GLERROR();
		            //glAlphaFunc( GL_GREATER, 0.0f );        LN_CHECK_GLERROR();
			        break;
		        case LN_BLEND_ADD_ALPHADISABLE:
		            LNGL::glEnable( GL_BLEND );                   //LN_CHECK_GLERROR();
		            LNGL::glBlendEquation( GL_FUNC_ADD );        // LN_CHECK_GLERROR();
		            LNGL::glBlendFunc( GL_ONE, GL_ONE );         // LN_CHECK_GLERROR();
		            //glAlphaFunc( GL_GREATER, 0.0f );        LN_CHECK_GLERROR();
			        break;
		        case LN_BLEND_SUB:
		            LNGL::glEnable( GL_BLEND );                        //   LN_CHECK_GLERROR();
		            LNGL::glBlendEquation( GL_FUNC_REVERSE_SUBTRACT ); ///   LN_CHECK_GLERROR();
		            LNGL::glBlendFunc( GL_SRC_ALPHA, GL_ONE );         //   LN_CHECK_GLERROR();
		            //glAlphaFunc( GL_GREATER, 0.0f );                LN_CHECK_GLERROR();
			        break;
		        case LN_BLEND_SUB_ALPHADISABLE:
		            LNGL::glEnable( GL_BLEND );                          // LN_CHECK_GLERROR();
		            LNGL::glBlendEquation( GL_FUNC_REVERSE_SUBTRACT );   // LN_CHECK_GLERROR();
		            LNGL::glBlendFunc( GL_ONE, GL_ONE );                //  LN_CHECK_GLERROR();
		            //glAlphaFunc( GL_GREATER, 0.0f );                LN_CHECK_GLERROR();
			        break;
		        case LN_BLEND_MUL:
		            LNGL::glEnable( GL_BLEND );                         //  LN_CHECK_GLERROR();
		            LNGL::glBlendEquation( GL_FUNC_ADD );               //  LN_CHECK_GLERROR();
		            LNGL::glBlendFunc( GL_BLEND_SRC_RGB, GL_ZERO );     //  LN_CHECK_GLERROR();
		            //glAlphaFunc( GL_GREATER, 0.0f );                LN_CHECK_GLERROR();
			        break;
		        case LN_BLEND_SCREEN:
		            LNGL::glEnable( GL_BLEND );                          // LN_CHECK_GLERROR();
		            LNGL::glBlendEquation( GL_FUNC_ADD );                // LN_CHECK_GLERROR();
		            LNGL::glBlendFunc( GL_ONE, GL_ONE_MINUS_DST_ALPHA ); // LN_CHECK_GLERROR();
		            //glAlphaFunc( GL_GREATER, 0.0f );                LN_CHECK_GLERROR();
			        break;

                // http://memo.devjam.net/clip/538
                // http://d.hatena.ne.jp/melpon/20070824/p1
		    }
	    }

        // カリング
	    if ( mRequestRenderState.Culling != mCurrentRenderState.Culling || reset_ )
	    {
		    const GLenum tb[] = { GL_BACK, GL_FRONT, GL_FRONT_AND_BACK };
            LNGL::glCullFace( tb[ mRequestRenderState.FillMode ] );
            //LN_CHECK_GLERROR();
	    }
#if !defined(LNOTE_GLES)
        // 塗りつぶし方法
	    if ( mRequestRenderState.FillMode != mCurrentRenderState.FillMode || reset_ )
	    {

		    const GLenum tb[] = { GL_FILL, GL_LINE, GL_POINT };
	        LNGL::glPolygonMode( GL_FRONT_AND_BACK, tb[ mRequestRenderState.FillMode ] );
            //LN_CHECK_GLERROR();
            //glPolygonMode(GL_FRONT, GL_FILL) // 表は塗りつぶす
            //glPolygonMode(GL_BACK, GL_LINE)  // 裏は線

	    }
	    
	    // アルファテスト
	    if ( mRequestRenderState.AlphaTest != mCurrentRenderState.AlphaTest || reset_ )
	    {
	        if ( mRequestRenderState.AlphaTest )
            {
	            LNGL::glEnable( GL_ALPHA_TEST );
                //LN_CHECK_GLERROR();
            }
	        else
            {
	            LNGL::glDisable( GL_ALPHA_TEST );
                //LN_CHECK_GLERROR();
            }
	    }
#endif
	    // 深度テスト
	    if ( mRequestRenderState.DepthTest != mCurrentRenderState.DepthTest || reset_ )
	    {
	        if ( mRequestRenderState.DepthTest )
            {
	            LNGL::glEnable( GL_DEPTH_TEST );
                //LN_CHECK_GLERROR();
            }
	        else
            {
	            LNGL::glDisable( GL_DEPTH_TEST );
                //LN_CHECK_GLERROR();
            }
	    }
	    // 深度書き込み
        if ( mRequestRenderState.DepthWrite != mCurrentRenderState.DepthWrite || reset_ )
	    {
		    const GLboolean tb[] = { GL_FALSE, GL_TRUE };
	        LNGL::glDepthMask( tb[ mRequestRenderState.DepthWrite ] );
            //LN_CHECK_GLERROR();
	    }
        // ポイントスプライト
        if ( mRequestRenderState.PointSprite != mCurrentRenderState.PointSprite || reset_ )
	    {
            if ( mRequestRenderState.DepthTest )
            {
	            LNGL::glEnable( GL_POINT_SPRITE );
                //LN_CHECK_GLERROR();
            }
	        else
            {
	            LNGL::glDisable( GL_POINT_SPRITE );
                //LN_CHECK_GLERROR();
            }
	    }
#endif
        mCurrentRenderState = mRequestRenderState;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 実際にカラー、深度バッファを設定する
    //---------------------------------------------------------------------
    void Renderer::_refreshFramebuffer()
    {
        if (mIsDeviceLost) return;

        if ( mCurrentRenderTargets[0] || mCurrentDepthBuffer )
        {
            //LNGL::glBindFramebuffer(GL_FRAMEBUFFER, 0);
            //LNGL::glBindTexture(GL_TEXTURE_2D, 0);
            LNGL::glBindFramebuffer( GL_FRAMEBUFFER, mGLFramebuffer );
            if (mGLFramebuffer == 0)
            {
                printf("_refreshFramebuffer() mGLFramebuffer is 0\n");
            }

            if (mCurrentRenderTargets[0])
            {
                OpenGL::TextureBase* rt = (OpenGL::TextureBase*)mCurrentRenderTargets[0];
                rt->commitChanges();

                //-------------------------------------------------
                // カラーバッファ

                GLuint color_tex = rt->getGLTexture();
                if (color_tex == 0)
                {
                    printf("_refreshFramebuffer() color_tex is 0\n");
                }

                LNGL::glFramebufferTexture2D(
                //glFramebufferTexture2DEXT(
                    GL_FRAMEBUFFER,//GL_FRAMEBUFFER_EXT,
                    GL_COLOR_ATTACHMENT0,//GL_COLOR_ATTACHMENT0_EXT,
                    GL_TEXTURE_2D,
                    color_tex,
                    0 );

                //GLuint color_tex = ( mCurrentRenderTargets[0] ) ? ((OpenGL::TextureBase*)mCurrentRenderTargets[0])->getGLTexture() : 0;

    //_p(mGLFramebuffer);
    //_p(color_tex);
                //glBindFramebuffer( GL_FRAMEBUFFER, mGLFramebuffer );
                //glBindFramebufferEXT( GL_FRAMEBUFFER, mGLFramebuffer );
                //LN_CHECK_GLERROR();
                /*
    GLint   bufferOld;
                glGetIntegerv( GL_COLOR_ATTACHMENT0, &bufferOld );
    _p(bufferOld);
                LN_CHECK_GLERROR();
                */
                
                //_p(GL_INVALID_ENUM);
                //_p(GL_INVALID_OPERATION);

    //GL_FRAMEBUFFER_BINDING
            }
            else
            {
                LNGL::glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    GL_COLOR_ATTACHMENT0,
                    GL_TEXTURE_2D,
                    0,
                    0 );
            }

            
            //LN_CHECK_GLERROR();

            //glBindFramebuffer( GL_FRAMEBUFFER, 0 );
            //glBindFramebufferEXT( GL_FRAMEBUFFER, 0 );
            //LN_CHECK_GLERROR();
       
            //-------------------------------------------------
            // 深度バッファ

            if (mCurrentDepthBuffer)
            {
                OpenGL::TextureBase* db = (OpenGL::TextureBase*)mCurrentDepthBuffer;
                db->commitChanges();
                GLuint depth_tex = db->getGLTexture();

                if (depth_tex == 0)
                {
                    printf("_refreshFramebuffer() depth_tex is 0\n");
                }
#if 1
                //LNGL::glBindRenderbuffer( GL_RENDERBUFFER, depth_tex );

                LNGL::glFramebufferRenderbuffer(
                //glFramebufferRenderbufferEXT(
                    GL_FRAMEBUFFER,//GL_FRAMEBUFFER_EXT,//
                    GL_DEPTH_ATTACHMENT,//GL_DEPTH_ATTACHMENT_EXT,//
                    GL_RENDERBUFFER,//GL_RENDERBUFFER_EXT,//
                    depth_tex );
                //LN_CHECK_GLERROR();
                //_p(depth_tex);
#else
                LNGL::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex, 0);
#endif
                /*
                
                */
            }
            else
            {
                LNGL::glFramebufferRenderbuffer(
                    GL_FRAMEBUFFER,
                    GL_DEPTH_ATTACHMENT,
                    GL_RENDERBUFFER,
                    0 );
            }

 
            LNGL::glViewport(
                mCurrentViewportRealRect.x,
                mCurrentRenderTarget0RealSize.h - mCurrentViewportRealRect.y - mCurrentViewportRealRect.h,
                mCurrentViewportRealRect.w,
                mCurrentViewportRealRect.h );
        }
        else
        {
            LNGL::glBindFramebuffer( GL_FRAMEBUFFER, 0 );
            //glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, mGLFramebuffer );
            //LN_CHECK_GLERROR();
        }
    }

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