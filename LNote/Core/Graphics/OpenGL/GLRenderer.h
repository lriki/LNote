//=============================================================================
//【 GLRenderer 】
//-----------------------------------------------------------------------------
///**
//  @file       GLRenderer.h
//  @brief      GLRenderer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
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
class GraphicsDevice;
class VertexBuffer;
class IndexBuffer;

//=============================================================================
// ■ Renderer クラス
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
class Renderer
    : public Base::ReferenceObject
    , public IRenderer
{
public:

    /// initialize() に渡す初期化データ
    struct InitData
    {
        Base::LogFile*              LogFile;
        OpenGL::GraphicsDevice*     GraphicsDevice;
        //OpenGL::PlatformContext*    PlatformContext;
    }; 

public:

    Renderer();

    virtual ~Renderer();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    void initialize( const InitData& init_data_ );

    /// レンダリングステートの設定
    virtual LNRESULT setRenderState( const LRenderState& state_ );

    /// レンダリングターゲットの取得 (参照カウントに変化はナシ)
    virtual ITexture* getRenderTarget( u32 index_ ) { return mCurrentRenderTargets[ index_ ]; }

    // レンダリングターゲットの設定
    virtual LNRESULT setRenderTarget( u32 index_, ITexture* texture_, bool reset_ = false );

    /// 深度バッファの取得 (参照カウントに変化はナシ)
    virtual ITexture* getDepthBuffer() { return mCurrentDepthBuffer; }

    /// 深度バッファの設定
    virtual LNRESULT setDepthBuffer( ITexture* texture_ );

    /// ビューポート矩形の取得
    virtual const Geometry::Rect& getViewport() const;

	/// ビューポート矩形の設定
	virtual LNRESULT setViewport( const Geometry::Rect& viewport_rect_ );

    /// シザー矩形の取得
    virtual const Geometry::Rect& getScissor() const;

	/// シザー矩形の設定
	virtual LNRESULT setScissor( const Geometry::Rect& rect_ );

    /// 頂点バッファの設定
	virtual LNRESULT setVertexBuffer( IVertexBuffer* vertex_buffer_, bool reset_ = false );

    /// インデックスバッファの設定
	virtual LNRESULT setIndexBuffer( IIndexBuffer* index_buffer_, bool reset_ = false );

    /// 設定されているカラーバッファ、深度バッファをクリアする
	virtual LNRESULT clear( bool target_, bool depth_, u32 color_, lnfloat z_ = 1.0f );

    /// プリミティブ描画
	virtual LNRESULT drawPrimitive( LNPrimitiveType primitive_, u32 start_vertex_, u32 primitive_count_ );

    /// プリミティブ描画 ( インデックス付き )
	virtual LNRESULT drawPrimitiveIndexed( LNPrimitiveType primitive_, u32 start_index_, u32 primitive_count_ );

    /// シーン開始
	virtual LNRESULT beginScene();

	/// シーン終了
	virtual LNRESULT endScene();

public: 

    /// GraphicsDevice::onLostDevice() から呼ばれる
    void onLostDevice();

    /// GraphicsDevice::onResetDevice() から呼ばれる
    void onResetDevice();

private:

    /// 後で変更しないレンダリングステートをデバイスに設定する
    void _initRenderState();

    /// 実際にレンダリングステートをデバイスに設定する
    LNRESULT _updateRenderState( bool reset_ );

    /// 実際にカラー、深度バッファを設定する
    void _refreshFramebuffer();

private:

    Base::LogFile*          mLogFile;
    GraphicsDevice*	        mGraphicsDevice;    ///< 参照カウントは増やさない
    //PlatformContext*        mPlatformContext;

    GLuint                  mGLFramebuffer;

    LRenderState            mCurrentRenderState;
    LRenderState            mRequestRenderState;

    ITexture*               mCurrentRenderTargets[ LN_MAX_MULTI_RENDERTARGETS ];
    ITexture*               mCurrentDepthBuffer;
    Geometry::Size			mCurrentRenderTarget0Size;
    Geometry::Size			mCurrentRenderTarget0RealSize;


    Geometry::Rect			mCurrentViewportRect;
    Geometry::Rect			mCurrentViewportRealRect;

    VertexBuffer*           mCurrentVertexBuffer;
    IndexBuffer*            mCurrentIndexBuffer;

    Geometry::Rect			mCurrentScissorRect;
    bool                    mEnableScissorTest;
    bool                    mSceneBegan;
    bool                    mIsDeviceLost;

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