//=============================================================================
//【 Framework 】
//-----------------------------------------------------------------------------
///**
//  @file       Framework.h
//  @brief      Framework
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "ConfigData.h"
#include "../System/Interface.h"
#include "../Game/FPSController.h"
#include "../Task/ITaskExecuteHandler.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{

//=============================================================================
// ■ Framework クラス
//-----------------------------------------------------------------------------
///**
//  @brief		ライブラリ全体の管理クラス
//*/
//=============================================================================
class Framework
    : public Base::ReferenceObject
    , public System::IEventListener
    , public Task::ITaskExecuteHandler
{
public:

    LN_REFOBJ_METHODS;

    Framework();

protected:

	virtual ~Framework();

public:

    /// 初期化
    LNRESULT initialize( const ConfigData& data_ );

    /// エディタツール用初期化
    void initializeForEditor( const ConfigData& data_ );

    /// 音声ライブラリ用初期化
    void initializeForAudioLibrary( const ConfigData& data_ );


    /// 終了処理
    void finalize();

    /// タスク管理クラスの初期化
    void initializeTaskManager();

    /// システム管理クラスの初期化
    LNRESULT initializeSystemManager();

    /// ファイル管理クラスの初期化
    void initializeFileManager();

    /// 入力管理クラスの初期化
    LNRESULT initializeInputManager();

    /// オーディオ管理クラスの初期化
    LNRESULT initializeAudioManager();

    /// 物理演算管理クラスの初期化
    LNRESULT initializePhysicsManager();
    
    /// グラフィックス管理クラスの初期化
    void initializeGraphicsManager();

    /// エフェクト管理クラスの初期化
    void initializeEffectManager();

    /// シーン管理クラスの初期化
    LNRESULT initializeSceneManager();

    /// システム管理クラスの取得
    System::Manager* getSystemManager() { return mSystemManager; }

    /// ファイル管理クラスの取得
    File::Manager* getFileManager() { return mFileManager; }

    /// 入力管理クラスの取得
    Input::Manager* getInputManager() { return mInputManager; }

    /// オーディオ管理クラスの取得
    Audio::Manager* getAudioManager() { return mAudioManager; }

    /// 物理演算管理クラスの取得
    Physics::Manager* getPhysicsManager() { return mPhysicsManager; }

    /// グラフィックス管理クラスの取得
    Graphics::Manager* getGraphicsManager() { return mGraphicsManager; }

    /// エフェクト管理クラスの取得
    Effect::Manager* getEffectManager() { return mEffectManager; }

    /// シーン管理クラスの取得
    Scene::Manager* getSceneManager() { return mSceneManager; }

    /// GUI 管理クラスの取得
    GUI::Manager* getGUIManager() { return mGUIManager; }

    /// ゲームシーン管理クラスの取得
    Game::Manager* getGameManager() { return mGameManager; }
    

    /// ゲームタイムの取得
    const Game::GameTime& getGameTime() const { return mFPSController->getGameTime(); }

    /// FPS 操作クラスの取得
    Game::FPSController* getFPSController() { return mFPSController; }

    // ライブラリ全体に終了を通知する
    void exit() { mEndRequested = true; }

    /// フレーム更新
    bool update();

    /// エディタ、あるいは組み込み用のフレーム更新
    void updateAsLibrary();

    /// 時間がかかる処理の後に呼び出すことで、FPS 処理、ゲームタイムへの影響をリセットする
    void resetDelayTime();

    /// デバイスリセットが必要であればonLostDevice() → リセット onResetDevice() と呼び出す (同期フェーズで実行。Managed Wrapper で使うので public 化)
    void checkDeviceState();

public:

    /// メッセージ処理コールバック
    virtual bool onEvent( const System::Event& args_ );

    /// メインスレッドのフレーム待ち処理を行う
    virtual void OnFrameWait();

    /// 描画情報の構築を行う
    virtual void OnUpdateDrawContents();

private:

    /// デバイスリセット前処理
	//		_onLostDevice()、_onResetDevice() は Graphics 以外に発生を伝えるための処理。
    void _onLostDevice();

    /// デバイスリセット後処理
    void _onResetDevice();

    /// ゲーム本体の更新前に処理するべきもの
    void _preUpdate();

    /// ゲーム本体の更新後に処理するべきもの
    bool _postUpdate();

    /// 同期フェーズの処理
    void _onUpdateSyncPhase();

private:

    Base::LogFile*      mLogFile;           ///< ログを出力するログファイルオブジェクト
    ConfigData          mConfigData;        ///< 初期化情報

    Task::Manager*      mTaskManager;
    System::Manager*    mSystemManager;
    File::Manager*      mFileManager;
    Input::Manager*     mInputManager;
    Audio::Manager*     mAudioManager;
    Physics::Manager*   mPhysicsManager;
    Graphics::Manager*  mGraphicsManager;
    Effect::Manager*    mEffectManager;
    Scene::Manager*     mSceneManager;
    GUI::Manager*       mGUIManager;
    Game::Manager*      mGameManager;

    Game::FPSController*    mFPSController;

    LNUpdateMode        mUpdateMode;
    bool                mInitialized;
    bool                mEndRequested;
};

//---------------------------------------------------------------------
///**
//  @brief      Framework インスタンスの作成
//*/
//---------------------------------------------------------------------
Framework* LNOTE_STDCALL createFramework();

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================