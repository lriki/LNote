//=============================================================================
//【 Canvas 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "GraphicsDevice.h"
#include "Canvas.h"

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
// ■ CanvasBase クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    CanvasBase::CanvasBase( GraphicsDevice* device_ )
    {
        mGraphicsDevice = device_;
        mGraphicsDevice->addCanvas( this );
        LN_SAFE_ADDREF( mGraphicsDevice );
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    CanvasBase::~CanvasBase()
    {
        if ( mGraphicsDevice )
        {
            mGraphicsDevice->removeCanvas( this );
            LN_SAFE_RELEASE( mGraphicsDevice );
        }
    }

//=============================================================================
// ■ DefaultCanvas クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    DefaultCanvas::DefaultCanvas( GraphicsDevice* device_ )
        : CanvasBase            ( device_ )
        , mDxDevice             ( NULL )
        , mBackbuffer           ( NULL )
        , mDepthBuffer          ( NULL )
        , mBackgroundColorCode  ( 0 )
		, mTargetWindowHandle	( NULL )
    {
        
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    DefaultCanvas::~DefaultCanvas()
    {
        LN_SAFE_RELEASE( mBackbuffer );
        LN_SAFE_RELEASE( mDepthBuffer );
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT DefaultCanvas::initialize( const Geometry::Size& size_ )
    {
        LNRESULT lr;

        mDxDevice       = mGraphicsDevice->getDX9Device();

        mBackbuffer = LN_NEW SystemCreatedTexture();

        

        mCanvasSize = size_;

        setWindowSize( size_ );
        
        // レンダリングターゲット作っておく必要があるが、
        // 今は仮想フルスクリーン限定で行くので一応OK

        return onResetDevice();
    }

    //---------------------------------------------------------------------
    // ● 仮想スクリーンサイズを変更する
    //---------------------------------------------------------------------
    LNRESULT DefaultCanvas::resize( u32 width_, u32 height_ )
    {
        mCanvasSize.set( width_, height_ );

		// ソフト的にデバイスロスト状態にして、
		// 次回のロストチェック時にバックバッファを作り直す
        mGraphicsDevice->postDeviceLostEvent();

        setWindowSize( mCanvasSize );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● バックバッファのレンダリングターゲット、深度バッファを設定する
    //---------------------------------------------------------------------
    LNRESULT DefaultCanvas::activate()
    {
        LNRESULT lr;

        LN_CALL_R( mGraphicsDevice->getRenderer()->setRenderTarget( 0, mBackbuffer, true ) );
        LN_CALL_R( mGraphicsDevice->getRenderer()->setDepthBuffer( mDepthBuffer ) );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● バックバッファの内容を転送する
    //---------------------------------------------------------------------
    LNRESULT DefaultCanvas::present()
    {
        if ( mGraphicsDevice->isDeviceLost() ) return LN_OK;

        LNRESULT lr;

        // ここでRT をバックバッファに戻しておく
        LN_CALL_R( activate() );


        //HRESULT hr = mDxDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffff00ff, 0, 0x00000000 );
        HRESULT hr = mDxDevice->Present( NULL, &mViewDestRect, mTargetWindowHandle, NULL );
        // hr = mDxDevice->Present( NULL, NULL, NULL, NULL );

        // デバイスロスト状態のときは復帰処理
        if ( hr == D3DERR_DEVICELOST )
        {
            // 以前このへんは while でフラグが元に戻るのをチェックしてたけど、
            // 例えばウィンドウが非アクティブになったのでデバイスロストしたとかいう場合、
            // もう一度アクティブにするにはメッセージ処理が必要。
            // でも、while で回してるだけじゃ処理ができなくて、結果無限ループに陥ってた。

            hr = mDxDevice->TestCooperativeLevel();

		    switch ( hr )
		    {
			    // デバイスロスト
		        case D3DERR_DEVICELOST:
			        //::SleepEx( 1000, true );      
			        break;
			    // デバイスロスト：リセット可能状態
		        case D3DERR_DEVICENOTRESET:
                    mGraphicsDevice->postDeviceLostEvent();
			        //LN_CALL_R( mGraphicsDevice->resetDevice( mGraphicsDevice->isFullscreen(), mCanvasSize ) );
			        break;

                default:
                    return LN_ERR_FATAL;
		    }
            
        }

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 描画先のウィンドウサイズを設定する
    //---------------------------------------------------------------------
    void DefaultCanvas::setWindowSize( const Geometry::Size& size_ )
    { 
        if ( size_.w <= 0 || size_.h <= 0 ) return;

        mWindowSize = size_;

        lnfloat sw = static_cast< lnfloat >( mCanvasSize.w );   // 転送元
	    lnfloat sh = static_cast< lnfloat >( mCanvasSize.h );
        lnfloat dw = static_cast< lnfloat >( mWindowSize.w );   // 転送先
	    lnfloat dh = static_cast< lnfloat >( mWindowSize.h );

	    lnfloat new_x, new_y;
	    lnfloat new_w, new_h;

        lnfloat ratio_w;
        lnfloat ratio_h;

	    // バックバッファサイズとスクリーンサイズが同じ場合
	    if ( sw == dw && sh == dh )
	    {
		    // そのまま設定
		    new_x = 0;
		    new_y = 0;
		    new_w = sw;
		    new_h = sh;
            ratio_w = 1.0f;
            ratio_h = 1.0f;
	    }
	    else
	    {
		    // 現在のスクリーンサイズ(デフォルトビューポートのサイズ)と画面サイズの比率計算
		    ratio_w = dw / sw;
		    ratio_h = dh / sh;

		    // 縦方向に合わせる ( 左右が余る )
		    if ( ratio_w > ratio_h )
		    {
			    new_w = static_cast< lnfloat >( sw * ratio_h );
			    new_h = static_cast< lnfloat >( dh );
			    new_x = static_cast< lnfloat >( ( dw / 2 ) - ( new_w / 2 ) );
			    new_y = 0;
		    }
		    //横方向にあわせる
		    else
		    {
			    new_w = static_cast< lnfloat >( dw );
			    new_h = static_cast< lnfloat >( sh * ratio_w );
			    new_x = 0;
			    new_y = static_cast< lnfloat >( ( dh / 2 ) - ( new_h / 2 ) );
		    }
	    }

        //mViewSrcRect.set( new_x, new_y, new_w, new_h );
        ::SetRect(
            &mViewDestRect,
            static_cast< int >( new_x ),
            static_cast< int >( new_y ),
            static_cast< int >( new_x + new_w ),
            static_cast< int >( new_y + new_h ) );

        mScreenTransformMatrix.identity();
        mScreenTransformMatrix.translation( -new_x, -new_y, 0 );
        mScreenTransformMatrix.scaling( 1.0f / ( new_w / sw ), 1.0f / ( new_h / sh ), 1 );
    }

    //---------------------------------------------------------------------
    // ● デバイスリセット直前 GraphicsDevice から呼ばれる
    //---------------------------------------------------------------------
    LNRESULT DefaultCanvas::onLostDevice()
    {
        LN_SAFE_RELEASE( mDepthBuffer );
        return mBackbuffer->setTextureAndSurface( NULL, NULL );
    }

    //---------------------------------------------------------------------
    // ● デバイスリセット後に GraphicsDevice から呼ばれる
    //---------------------------------------------------------------------
    LNRESULT DefaultCanvas::onResetDevice()
    {
        // Framework との連携上、2回よばれることがあるので
        onLostDevice();

        HRESULT hr;

        IDirect3DSurface9* surface_;
        LN_DXCALL_R( mDxDevice->GetRenderTarget( 0, &surface_ ) );

        LNRESULT lr = mBackbuffer->setTextureAndSurface( NULL, surface_ );  // surface_ の参照カウントは+1
    
        SAFE_RELEASE( surface_ );

        LN_CALL_R( mGraphicsDevice->createDepthBuffer( 
            &mDepthBuffer, mCanvasSize.w, mCanvasSize.h, LN_FMT_D24S8 ) );

        return lr;
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