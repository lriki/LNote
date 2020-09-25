//=============================================================================
//【 GLCanvas 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../System/Interface.h"
#include "../Manager.h"
#include "LNGL/LNGL.h"
#include "GLGraphicsDevice.h"
#include "GLCanvas.h"


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
// ■ DefaultCanvas クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    DefaultCanvas::DefaultCanvas( GraphicsDevice* device_ )
        : mGraphicsDevice       ( device_ )
        //, mPlatformContext      ( NULL )
        , mWindow               ( NULL )
        , mBackgroundColorCode  ( 0 )
        , mCanvasBuffer         ( NULL )
        , mCanvasDepthBuffer    ( NULL )
    {
        LN_SAFE_ADDREF( mGraphicsDevice );
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    DefaultCanvas::~DefaultCanvas()
    {
        if ( mGraphicsDevice )
        {
            LN_SAFE_RELEASE( mGraphicsDevice );
        }
        LN_SAFE_RELEASE( mCanvasDepthBuffer );
        LN_SAFE_RELEASE( mCanvasBuffer );
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    void DefaultCanvas::initialize( const InitData& init_data_ )
    {
        //mPlatformContext = init_data_.PlatformContext;
        mWindow = init_data_.Window;
        mCanvasSize = init_data_.CanvasSize;//mWindow->getSize();
        setWindowSize( mWindow->getSize() );
        onResetDevice();
    }

    //---------------------------------------------------------------------
    // ● 仮想スクリーンサイズを変更する
    //---------------------------------------------------------------------
    LNRESULT DefaultCanvas::resize( lnU32 width_, lnU32 height_ )
    {
        mCanvasSize.set( width_, height_ );

        // RT 作り直し
        onResetDevice();
        //LN_PRINT_NOT_IMPL_FUNCTION

        // 同じウィンドウサイズを渡していろいろ更新
        setWindowSize( mWindowSize );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    LNRESULT DefaultCanvas::activate()
    {
        mGraphicsDevice->getRenderer()->setRenderTarget(0, mCanvasBuffer, true );
        mGraphicsDevice->getRenderer()->setDepthBuffer(mCanvasDepthBuffer);
        
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● バックバッファの内容を転送する
    //---------------------------------------------------------------------
    LNRESULT DefaultCanvas::present()
    {
        // ここでRT をバックバッファに戻しておく
        IRenderer* r = mGraphicsDevice->getRenderer();//r->clear(true,true,0xff0000ff,1.0f);
        r->setRenderTarget( 0, NULL );
        r->setDepthBuffer( NULL );

        LVector2 size( (lnfloat)mCanvasSize.w, (lnfloat)mCanvasSize.h );

        r->beginScene();
        r->setRenderState( LRenderState::BUFFER_COPY_STATE );
        

        // TODO:TexUV Offset にしたい
        GeometryRenderer* gr = mGraphicsDevice->getManager()->getGeometryRenderer();
        gr->setMatrix( mTransformMatrix );
        gr->setViewSize( size );
        gr->setTexture( mCanvasBuffer );
        gr->begin();
        gr->drawSquare(
            0.0f,   0.0f,   0xffffffff, 0.0f, 0.0f, 
            size.x, 0.0f,   0xffffffff, 1.0f, 0.0f, 
            0.0f,   size.y, 0xffffffff, 0.0f, 1.0f, 
            size.x, size.y, 0xffffffff, 1.0f, 1.0f );
        gr->end();
        r->endScene();

        //mPlatformContext->present();
        LNGL::present();

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

        mScreenTransformMatrix.identity();
        mScreenTransformMatrix.scaling( sw / new_w, sh / new_h, 1.0f );
        mScreenTransformMatrix.translation( -new_x * sw / new_w, -new_y * sh / new_h, 0 );

        mTransformMatrix.identity();
        mTransformMatrix.scaling( new_w / dw, new_h / dh, 1.0f );
        mTransformMatrix.translation( new_x * sw / dw, new_y * sh / dh, 0 );

    }

    //---------------------------------------------------------------------
    // ● デバイスリセット直前 GraphicsDevice から呼ばれる
    //---------------------------------------------------------------------
    void DefaultCanvas::onLostDevice()
    {
        LN_SAFE_RELEASE( mCanvasDepthBuffer );
        LN_SAFE_RELEASE( mCanvasBuffer );
    }

    //---------------------------------------------------------------------
    // ● デバイスリセット後に GraphicsDevice から呼ばれる
    //---------------------------------------------------------------------
    void DefaultCanvas::onResetDevice()
    {
        // 新しいウィンドウサイズでRT作り直し

        //LN_SAFE_RELEASE( mCanvasBuffer );

        LN_SAFE_RELEASE( mCanvasDepthBuffer );
        LN_SAFE_RELEASE( mCanvasBuffer );
        
        mGraphicsDevice->createRenderTarget(
            &mCanvasBuffer,
            mCanvasSize.w,
            mCanvasSize.h,
            1,
            LN_FMT_A8R8G8B8 );

        mGraphicsDevice->createDepthBuffer( 
            &mCanvasDepthBuffer, mCanvasSize.w, mCanvasSize.h, LN_FMT_D24S8 );

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