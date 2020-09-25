//=============================================================================
//【 Canvas 】
//-----------------------------------------------------------------------------
///**
//  @file       Canvas.h
//  @brief      Canvas
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"
#include "Texture.h"

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

//=============================================================================
// ■ CanvasBase クラス
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class CanvasBase
    : public Base::ReferenceObject
    , public ICanvas
{
public:

    /// コンストラクタ
    CanvasBase( GraphicsDevice* device_ );

    /// デストラクタ
    virtual ~CanvasBase();

    LN_REFOBJ_METHODS;

public:

    /// デバイスリセット直前 GraphicsDevice から呼ばれる
    virtual LNRESULT onLostDevice() = 0;

    /// デバイスリセット後に GraphicsDevice から呼ばれる
    virtual LNRESULT onResetDevice() = 0;

protected:

    GraphicsDevice* mGraphicsDevice;
};

//=============================================================================
// ■ DefaultCanvas クラス
//-----------------------------------------------------------------------------
///**
//  @brief		DX9 のデフォルトで作成されるスワップチェインを使ったキャンバス
//*/
//=============================================================================
class DefaultCanvas
    : public CanvasBase
{
public:

    DefaultCanvas( GraphicsDevice* device_ );

    virtual ~DefaultCanvas();

public:

    /// 初期化
    LNRESULT initialize( const Geometry::Size& size_ );

    /// キャンバスサイズを取得する
    virtual const Geometry::Size& getSize() { return mCanvasSize; }

    /// キャンバスサイズを変更する
    virtual LNRESULT resize( u32 width_, u32 height_ );

    /// バックバッファの取得
    virtual ITexture* getBackbuffer() { return mBackbuffer; }

    /// バックバッファのレンダリングターゲット、深度バッファを設定する
    virtual LNRESULT activate();

    /// バックバッファの内容をウィンドウへ転送する
    virtual LNRESULT present();

    /// 描画先のウィンドウサイズを設定する
    virtual void setWindowSize( const Geometry::Size& size_ );

    /// キャンバス→ウィンドウへ転送する際の座標変換行列の取得
    virtual const LMatrix& getScreenTransform() const { return mScreenTransformMatrix; }

	/// present() の転送先ウィンドウのウィンドウハンドルの設定
	virtual void setTargetWindowHandle(HWND hwnd) { mTargetWindowHandle = hwnd; }

public:

    /// デバイスリセット直前 GraphicsDevice から呼ばれる
    virtual LNRESULT onLostDevice();

    /// デバイスリセット後に GraphicsDevice から呼ばれる
    virtual LNRESULT onResetDevice();

private:

    IDirect3DDevice9*       mDxDevice;
    SystemCreatedTexture*   mBackbuffer;
    ITexture*               mDepthBuffer;

    Geometry::Size			mWindowSize;
    Geometry::Size			mCanvasSize;
    LColor                  mBackgroundColor;
    u32                     mBackgroundColorCode;

    RECT                    mViewDestRect;           ///< 転送先矩形
    LMatrix                 mScreenTransformMatrix;

	HWND					mTargetWindowHandle;
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