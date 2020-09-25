//=============================================================================
//【 Manager 】
//-----------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../File/Manager.h"
#include "../Physics/Manager.h"
#include "../System/Manager.h"
#include "Interface.h"
#include "Common/GeometryRenderer.h"
#include "Common/DebugText.h"
#include "Font/FontManagerBase.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace Win32 { class Win32FontManager; }

class SpriteRenderer;
class ModelManager;
class DebugText;

//=============================================================================
// ■ Manager クラス
//-----------------------------------------------------------------------------
///**
//  @brief		グラフィックス機能の管理クラス
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

	/// 初期化データ
	struct InitData
	{
        Base::LogFile*      LogFile;
        File::Manager*      FileManager;
        Physics::Manager*   PhysicsManager;
        System::Manager*    SystemManager;
        Task::Manager*      TaskManager;        ///< OpenGL Shader 用暫定対応
        //System::IWindow*    MainWindow;
        LNGraphicsAPI       GraphicsAPI;
        Geometry::Size		CanvasSize;         ///< キャンバス (バックバッファ) のサイズ
        bool                EnableFPUPreserve;  ///< 浮動小数点の演算精度を落とさないようにする場合は true
        LNFontNameList      FontFilenameList;
    };

public:

    Manager();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

    /// 終了処理
    void finalize();

    /// 使用中の GraphicsAPI タイプの取得
    LNGraphicsAPI getGraphicsAPI() { return mGraphicsAPI; }

    /// IGraphicsDevice の取得
    IGraphicsDevice* getGraphicsDevice();

    /// フォントの作成 (フォント名が NULL の場合はデフォルトのフォントが使われる)
    LNRESULT createFont( IFont** obj_, const lnChar* fontname_ );

    /// デフォルトのフォントを取得する
    IFont* getDefaultFont();

    /// 形状描画クラスの取得
    GeometryRenderer* getGeometryRenderer() { return mGeometryRenderer; }

    /// 3Dスプライト描画クラスの取得
    SpriteRenderer* getSprite3DRenderer() { return mSprite3DRenderer; }

    /// 2Dスプライト描画クラスの取得
    SpriteRenderer* getSprite2DRenderer() { return mSprite2DRenderer; }

    /// モデル管理クラスの取得
    ModelManager* getModelManager() { return mModelManager; }

    /// デバッグ文字列描画クラスの取得
    DebugText* getDebugText() { return mDebugText; }

    /// ダミー用テクスチャの取得 (32x32、不透明の白)
    ITexture* getDummyTexture() { return mDummyTexture; }

    /// テクスチャをファイルから作成する
    //LNRESULT createTextureFromFile( ITexture** tex_, const lnChar* filename_, u32 color_key_ = 0, u32 levels_ = 1, LNSurfaceFormat format_ = LN_FMT_UNKNOWN, LSharingKey key_ = lnNullKey );

    

    Task::Manager* getTaskManager() { return mTaskManager; }

protected:

	virtual ~Manager();

private:

    Base::LogFile*              mLogFile;
    File::Manager*              mFileManager;
    Task::Manager*              mTaskManager;        ///< OpenGL Shader 用暫定対応
    IGraphicsDevice*            mGraphicsDevice;
    LNGraphicsAPI               mGraphicsAPI;
    FontManagerBase*            mFontManager;
    //Win32::Win32FontManager*    mFontManager;

    GeometryRenderer*           mGeometryRenderer;
    SpriteRenderer*             mSprite3DRenderer;
    SpriteRenderer*             mSprite2DRenderer;
    ModelManager*               mModelManager;
    DebugText*                  mDebugText;
    ITexture*                   mDummyTexture;

};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================