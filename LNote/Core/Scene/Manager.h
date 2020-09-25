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
#include "../Geometry/Vector2.h"
#include "../Thread/EventFlag.h"
#include "../System/Interface.h"
#include "../Game/GameTime.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
class SceneGraph;
class SceneNode;

//=============================================================================
// ■ Manager クラス
//-----------------------------------------------------------------------------
///**
//  @brief		シーンの管理クラス
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
        Graphics::Manager*  GraphicsManager;
        Effect::Manager*    EffectManager;      ///< デフォルトのシーンに設定される EffectManage
        Physics::Manager*   PhysicsManager;     ///< デバッグ描画用
        LVector2            VirtualViewSize;    ///< カメラの2Dプロジェクション、3D→2Dの座標変換、シェーダのVIEWPIXELSIZE
        lnU32               ShaderCacheSize;
        bool                EnableDrawThread;
    };

public:
    Manager();
    LN_REFOBJ_METHODS;

protected:
	virtual ~Manager();

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

    /// 終了処理
    void finalize();

    /// デフォルトのシーンを取得する
	SceneGraph* getDefaultSceneGraph() { return mDefaultScene; }

    /// 描画 (beginScene endScene present の一連の流れも行う)
    LNRESULT updateAndRender( const Game::GameTime& time_ );

    /// 描画に専用のスレッドを使う場合、true を渡す
    void enableMulticoreOptimization( bool f_ ) { mMulticoreOptimization = f_; }


    /// [描画前同期タスク] 視錐台更新、描画コマンド構築など
    void updateDefaultSceneRenderContent();

    /// [描画タスク] デフォルトのシーンを描画する
    void renderDefaultScene();

    /// メッセージ処理 (Framework から呼ばれる)
    LNRESULT onEvent( const System::Event& args_ );

private:

    /// 描画 (描画スレッドを使う場合は DrawThread クラスから呼ばれる)
	LNRESULT _render();

private:

    class DrawThread;
    friend class DrawThread;

private:

    Base::LogFile*      mLogFile;
    Graphics::Manager*  mGraphicsManager;
    SceneGraph*         mDefaultScene;
    Effect::Manager*    mEffectManager;     ///< マルチコア最適化の処理のため。時間があるとき、ちゃんと TaskManager で実装したい

    DrawThread*         mDrawThread;
    Thread::EventFlag*  mEndFlag;
    Thread::EventFlag*  mDrawEnd;
    Thread::EventFlag*  mDrawing;

    Game::GameTime      mGameTime;
    bool                mMulticoreOptimization;
    Thread::EventFlag   mRenderReady;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================