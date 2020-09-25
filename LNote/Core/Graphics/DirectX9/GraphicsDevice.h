//=============================================================================
//【 GraphicsDevice 】
//-----------------------------------------------------------------------------
///**
//  @file       GraphicsDevice.h
//  @brief      GraphicsDevice
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <list>
#include "../../Base/Cache.h"
#include "../../Thread/EventFlag.h"
#include "../../Thread/Semaphore.h"
#include "../../System/Manager.h"
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
class Manager;

namespace DirectX9
{
class Renderer;
class CanvasBase;
class DefaultCanvas;
class Texture;
class RenderTargetTexture;
class DepthBuffer;
class Shader;
typedef Base::CacheObjectManager< Texture >     TextureCache;
typedef Base::CacheObjectManager< Shader >      ShaderCache;

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
    , public ShaderCache
    , public IGraphicsDevice
{
public:

	/// 初期化データ
	struct InitData
	{
        Base::LogFile*      LogFile;
        Manager*            Manager;
        //System::IWindow*    MainWindow;
        System::Manager*    SystemManager;
        File::Manager*      FileManager;
        Geometry::Size		CanvasSize;         ///< キャンバス (バックバッファ) のサイズ
        bool	            FullScreen;	        ///< フルスクリーンで起動する場合は true
        bool	            EnableVSyncWait;    ///< VSync を待つ場合は true
        bool                EnableFPUPreserve;  ///< 浮動小数点の演算精度を落とさないようにする場合は true
        u32                 TextureCacheSize;
        u32                 ShaderCacheSize;
    };

public:

    GraphicsDevice();

	virtual ~GraphicsDevice();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

    /// 終了処理
    virtual void finalize();

    /// 使用中の GraphicsAPI タイプの取得
    virtual LNGraphicsAPI getGraphicsAPI() { return LN_GRAPHICSAPI_DIRECTX9; }

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
    virtual LNRESULT createRenderTarget( ITexture** obj_, u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ );

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

    /// IDirect3DDevice9 へのポインタの取得
    virtual IDirect3DDevice9* getDX9Device() { return mDxDevice; }

    /// デバイスリセット前処理
    virtual void onLostDevice();

    /// デバイスリセット後処理
    virtual void onResetDevice();

    /// デバイスリセット
    virtual void resetDevice();

public:

    Manager* getManager() { return mManager; }

    /// デバイスのリセット (DefaultCanvas から呼び出される。フルスクリーンにするとき、幅と高さは無効) (window_bb_size_ はウィンドウモード時のバックバッファサイズ)
    LNRESULT resetDevice( bool fullscreen_, const Geometry::Size& window_bb_size_ );

    /// フルスクリーンに変更可能かを判定する
    bool canFullScreen() const { return ( mCanvasList.size() == 1 ); }

    /// フルスクリーンかを判定する
    bool isFullscreen() const { return mIsFullScreen; }

    /// CanvasBase のコンストラクタから呼ばれる
    void addCanvas( CanvasBase* canvas_ ) { mCanvasList.push_back( canvas_ ); }

    /// CanvasBase のデストラクタから呼ばれる
    void removeCanvas( CanvasBase* canvas_ ) { mCanvasList.remove( canvas_ ); }

    /// RenderTargetTexture デストラクタから呼ばれる
    void removeRenderTarget( RenderTargetTexture* texture_ ) { mRenderTargetList.remove( texture_ ); }

    /// DepthBuffer の基底の デストラクタから呼ばれる
    void removeDepthBuffer( DepthBuffer* texture_ ) { mDepthBufferList.remove( texture_ ); }

    /// Shader のデストラクタから呼ばれる
    void removeShader( Shader* shader_ ) { mShaderList.remove( shader_ ); }

    /// デバイスリセットの必要がある場合は true
    bool isDeviceLost() { return mIsDeviceLost.isTrue(); }

    /// (Canvas::preset() から呼ばれる。一度呼ばれると isDeviceLost() が true を返すようになる)
    void postDeviceLostEvent();

    D3DPRESENT_PARAMETERS* getDXPresentParameters() { return &mPresentParameters; }

    /// (ShaderPass::begin() から呼ばれる)
    void setSamplerState( int index_, const LSamplerState& state_, bool reset_ = false );

private:

    /// デバイスの性能チェック
    LNRESULT _checkDeviceInformation();

    /// スクリーンモード ( mIsFullScreen ) に応じて mPresentParameters 設定
	void _setPresentParameters( const Geometry::Size& window_bb_size_ );

private:

    typedef std::list< CanvasBase* >            CanvasList;
    typedef std::list< RenderTargetTexture* >   RenderTargetList;
    typedef std::list< DepthBuffer* >           DepthBufferList;
    typedef std::list< Shader* >                ShaderList;

private:

    Base::LogFile*              mLogFile;
    Manager*                    mManager;
    System::Manager*            mSystemManager;
    System::IWindow*            mMainWindow;
    File::Manager*              mFileManager;
    Renderer*                   mRenderer;

    IDirect3D9*		            mDirect3D;
    IDirect3DDevice9*	        mDxDevice;
    D3DPRESENT_PARAMETERS       mPresentParameters;	///< プレゼンテーションパラメータ

    // 調査結果
    D3DCAPS9                    mDxCaps;
    D3DDISPLAYMODE              mDxDisplayMode;
    D3DTEXTUREFILTERTYPE        mCurrentMinFilter;
	D3DTEXTUREFILTERTYPE        mCurrentMagFilter;
	D3DTEXTUREFILTERTYPE        mCurrentMipFilter;
	D3DTEXTUREFILTERTYPE        mBestMinFilter;
	D3DTEXTUREFILTERTYPE        mBestMagFilter;
	D3DTEXTUREFILTERTYPE        mBestMipFilter;
    DWORD                       mMSAAQualityWindowed;
    DWORD                       mMSAAQualityFullScreen;
    D3DDEVTYPE				    mDeviceType;        ///< デバイスの種類。_checkDeviceInformation() でチェックして initialize() に伝えるために用意

    CanvasList                  mCanvasList;
    DefaultCanvas*              mDefaultCanvas;
    RenderTargetList            mRenderTargetList;
    DepthBufferList             mDepthBufferList;
    ShaderList                  mShaderList;        ///< キーを持たないものの onLostDevice() のために用意。一応キーを持つものもすべて持っている

    Base::TempBuffer            mTempBuffer;
    Thread::EventFlag           mIsDeviceLost;
    Thread::Semaphore           mDeviceLostEventSemaphore;
    bool                        mIsFullScreen;
    bool                        mEnableFPUPreserve;

    LSamplerState               mSamplerStatus[LN_MAX_SAMPLER_STAGE_INDEX];
    
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