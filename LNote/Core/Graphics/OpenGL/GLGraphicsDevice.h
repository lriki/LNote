//=============================================================================
//【 GLGraphicsDevice 】
//-----------------------------------------------------------------------------
///**
//  @file       GLGraphicsDevice.h
//  @brief      GLGraphicsDevice
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <list>
#include "../../Base/Cache.h"
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
class Renderer;
class CanvasBase;
class DefaultCanvas;
class VertexBuffer;
class IndexBuffer;
class Texture;
class RenderTargetTexture;
class DepthBuffer;
class Shader;
class ShaderPass;
class ShaderManager;
typedef Base::CacheObjectManager< Texture >     TextureCache;

//=============================================================================
// ■ GraphicsDevice クラス
//-----------------------------------------------------------------------------
///**
//  @brief		グラフィックスデバイス
//*/
//=============================================================================
class GraphicsDevice
    : public Base::ReferenceObject
    , public TextureCache
    , public IGraphicsDevice
{
public:

	/// 初期化データ
	struct InitData
	{
        Base::LogFile*      LogFile;
        Graphics::Manager*  Manager;
        System::Manager*    SystemManager;
        File::Manager*      FileManager;
        Task::Manager*      TaskManager;        ///< OpenGL Shader 用暫定対応
        Geometry::Size		CanvasSize;         ///< キャンバス (バックバッファ) のサイズ
	    bool	            EnableVSyncWait;    ///< VSync を待つ場合は true (Android では自動調整な気もするけど…)
        lnU32               TextureCacheSize;
        lnU32               ShaderCacheSize;
    };

public:

    GraphicsDevice();

	virtual ~GraphicsDevice();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    void initialize( const InitData& init_data_ );

    /// 終了処理
    virtual void finalize();

    /// 使用中の GraphicsAPI タイプの取得
    virtual LNGraphicsAPI getGraphicsAPI() { return LN_GRAPHICSAPI_OPENGL; }

    /// 描画クラスの取得
    virtual IRenderer* getRenderer();

    /// デフォルトのバックバッファを示すキャンバスの取得
    virtual ICanvas* getDefaultCanvas();

    /// 頂点バッファの作成
    virtual LNRESULT createVertexBuffer( IVertexBuffer** obj_, LNVertexElemenst* elements_, u32 vertex_num_, const void* data_, bool is_dynamic_ = false );

    /// インデックスバッファの作成
    virtual LNRESULT createIndexBuffer( IIndexBuffer** obj_, u32 index_num_, const u16* data_, bool is_dynamic_, bool is_16bit_ );

    /// テクスチャの作成
    virtual LNRESULT createTexture( ITexture** obj_, u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ );

    /// テクスチャの作成 ( メモリに展開された画像データから )
    virtual LNRESULT createTexture( ITexture** obj_, const void* data_, u32 size_, u32 color_key_ = 0, u32 levels_ = 1, LNSurfaceFormat format_ = LN_FMT_A8R8G8B8, LSharingKey key_ = lnNullKey );

    /// テクスチャの作成 (入力ストリームから)
    virtual LNRESULT createTexture( ITexture** obj_, File::IInStream* stream_, u32 color_key_ = 0, u32 levels_ = 1, LNSurfaceFormat format_ = LN_FMT_UNKNOWN, LSharingKey key_ = lnNullKey );

    /// テクスチャの作成 (ファイルから)
    virtual void createTexture( ITexture** obj_, const lnChar* filename_, u32 color_key_, u32 levels_, LNSurfaceFormat format_, LSharingKey key_ );

    /// レンダーターゲットテクスチャの作成
    virtual LNRESULT createRenderTarget( ITexture** obj_, u32 width_, u32 height_, u32 levels_ = 1, LNSurfaceFormat format_ = LN_FMT_A8R8G8B8 );

    /// 深度バッファの作成
    virtual LNRESULT createDepthBuffer( ITexture** obj_, u32 width_, u32 height_, LNSurfaceFormat format_ );

    /// シェーダの作成
    virtual LNRESULT createShader( IShader** obj_, const void* data_, u32 size_, LSharingKey key_ = lnNullKey );

    /// シェーダの作成 ( 入力ストリームから )
    virtual LNRESULT createShader( IShader** obj_, File::IInStream* stream_, LSharingKey key_ = lnNullKey );
	
    /// シェーダの作成 (ファイルから)
    virtual void createShader( IShader** obj_, const lnChar* filename_, LSharingKey key_ );

    /// キャッシュのクリア
    virtual void clearCache( bool texture_, bool shader_ );

    /// デバイスリセットの必要がある場合は true
    virtual bool isDeviceLost() { return false; }

    /// デバイスリセット前処理
    virtual void onLostDevice();

    /// デバイスリセット後処理
    virtual void onResetDevice();

    /// デバイスリセット
    virtual void resetDevice() { LN_PRINT_NOT_IMPL_FUNCTION; } 

#if defined(LNOTE_DIRECTX)
    virtual IDirect3DDevice9* getDX9Device() { return NULL; }
#endif
    
public:

    Manager* getManager() { return mManager; }

    File::Manager* getFileManager() { return mFileManager; }

    /// デバイスのリセット (DefaultCanvas から呼び出される。フルスクリーンにするとき、幅と高さは無効) (window_bb_size_ はウィンドウモード時のバックバッファサイズ)
    LNRESULT resetDevice( bool fullscreen_, const Geometry::Size& window_bb_size_ );

    /// フルスクリーンに変更可能かを判定する
    bool canFullScreen() const { return false; }

    /// フルスクリーンかを判定する
    bool isFullscreen() const { return false; }

    /// (VertexBuffer デストラクタから呼ばれる)
    void removeVertexBuffer( VertexBuffer* vb_ ) { mVertexBufferList.remove( vb_ ); }

    /// (IndexBuffer デストラクタから呼ばれる)
    void removeIndexBuffer( IndexBuffer* ib_ ) { mIndexBufferList.remove( ib_ ); }

    /// (Texture デストラクタから呼ばれる)
    void removeTexture( Texture* texture_ ) { mTextureList.remove( texture_ ); }

    /// (RenderTargetTexture デストラクタから呼ばれる)
    void removeRenderTarget( RenderTargetTexture* texture_ ) { mRenderTargetList.remove( texture_ ); }

    /// (DepthBuffer の基底の デストラクタから呼ばれる)
    void removeDepthBuffer( DepthBuffer* texture_ ) { mDepthBufferList.remove( texture_ ); }

    /// (Renderer::beginScene() から)
    void commitChangesGLResource();

    


    /// 一時バッファ取得
    Base::TempBuffer& getTempBuffer() { return mTempBuffer; }


    void setCurrentShaderrPass( ShaderPass* pass_ );

    ShaderPass* getCurrentShaderrPass() { return mCurrentShaderrPass; }

    /// RenderTarget、DepthBuffer を作るときに呼ばれる。true の場合、2のべき乗で作る
    bool IsPowerOfTwoBufferTexture() { return true; }

    //bool isUseDrawThread() { return true; }

private:

    /// デバイスの性能チェック
    LNRESULT _checkDeviceInformation();

    /// デバイスリセット前処理
    LNRESULT _onLostDevice();

    /// デバイスリセット後処理
    LNRESULT _onResetDevice();

private:

    typedef std::list< VertexBuffer* >          VertexBufferList;
    typedef std::list< IndexBuffer* >           IndexBufferList;
    typedef std::list< Texture* >               TextureList;
    typedef std::list< RenderTargetTexture* >   RenderTargetList;
    typedef std::list< DepthBuffer* >           DepthBufferList;

private:

    Base::LogFile*              mLogFile;
    Manager*                    mManager;
    System::Manager*            mSystemManager;
    File::Manager*              mFileManager;
    Renderer*                   mRenderer;
    ShaderManager*              mShaderManager;
    
    /*PlatformContext*            mPlatformContext;*/

    DefaultCanvas*              mDefaultCanvas;
    VertexBufferList            mVertexBufferList;
    IndexBufferList             mIndexBufferList;
    TextureList                 mTextureList;
    RenderTargetList            mRenderTargetList;
    DepthBufferList             mDepthBufferList;

    Base::TempBuffer            mTempBuffer;
    ShaderPass*                 mCurrentShaderrPass;

    bool                        mIsDeviceLost;

    friend class Renderer;
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