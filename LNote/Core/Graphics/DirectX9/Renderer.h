//=============================================================================
//【 Renderer 】
//-----------------------------------------------------------------------------
///**
//  @file       Renderer.h
//  @brief      Renderer
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
        Base::LogFile*      LogFile;
        GraphicsDevice*     GraphicsDevice;
        D3DCAPS9*           DxCaps;
    }; 

public:

    Renderer();

    virtual ~Renderer();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

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

    /// デバイスリセット直前 GraphicsDevice から呼ばれる
    LNRESULT onLostDevice();

    /// デバイスリセット後に GraphicsDevice から呼ばれる
    LNRESULT onResetDevice();

private:

    /// 後で変更しないレンダリングステートをデバイスに設定する
    LNRESULT _initRenderState();

    /// 実際にレンダリングステートをデバイスに設定する
    LNRESULT _updateRenderState();

    /// 現在のレンダリングステートをデバイスに再設定する
    LNRESULT _resetRenderState();

    // シザー矩形の設定
    LNRESULT _setScissorRect( const Geometry::Rect& rect_ );

private:

    Base::LogFile*      mLogFile;
    GraphicsDevice*	    mGraphicsDevice;    ///< 参照カウントは増やさない

    IDirect3DDevice9*       mDxDevice;
    D3DCAPS9*               mDxCaps;

    LRenderState            mCurrentRenderState;
    LRenderState            mRequestRenderState;

    ITexture*               mCurrentRenderTargets[ LN_MAX_MULTI_RENDERTARGETS ];
    ITexture*               mCurrentDepthBuffer;

    Geometry::Rect			mCurrentViewportRect;

    VertexBuffer*           mCurrentVertexBuffer;
    IndexBuffer*            mCurrentIndexBuffer;

    Geometry::Rect			mCurrentScissorRect;
    bool                    mEnableScissorTest;
    bool                    mSceneBegan;


    // mCurrentRenderTargets は、ITexture* で不具合出るようなら IDirect3DSurface9* でも OK
    //  ↑・・・と思ったけど、デバイスリセット時に再設定するときに必要かも。
    //    もし、1フレーム終了時にすべてのターゲットを NULL にするとかなら問題ないけど。
    
    //SamplerState        mCurrentSamplerState[ TEXTURE_STAGE_MAX_COUNT ];
    //SamplerState        mRequestSamplerState[ TEXTURE_STAGE_MAX_COUNT ];
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