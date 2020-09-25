//=============================================================================
//【 GameScene 】
//-----------------------------------------------------------------------------
///**
//  @file       GameScene.h
//  @brief      GameScene
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Common.h"
#include "GameObjectManager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Game
{

//=============================================================================
// ■ GameScene
//-----------------------------------------------------------------------------
///**
//  @brief      
//
//  
//*/
//=============================================================================
class GameScene
    : public Base::ReferenceObject
{
public:

    GameScene();
    virtual ~GameScene();

public:

    /// シーン開始
    virtual void onStart() {}

    /// 再開 (子シーンが終了し、このシーンがアクティブになった)
	virtual void onRestart() {}
    //virtual void onResume() {}

    /// フレーム更新
    virtual void onUpdate();

    /// 停止 (子シーンがアクティブになった)
    virtual void onPause() {}

    /// シーン終了
    virtual void onTerminate() {}

    /// メインループ
    //virtual void main();

    GameScene* getParentGameScene() { return mParentGameScene; };

	GameObjectManager* getGameObjectManager() { return mGameObjectManager; }

public:

    /// [内部用]
    void initialize();

    /// [内部用]
    void finalize();

    /// [内部用] GameObject をシーンに追加する
    //void addGameObject(GameObject* object_);

protected:

    Manager*            mManager;           ///< シーン切り替え時に Manager から直接設定
    GameScene*          mParentGameScene;

	GameObjectManager*	mGameObjectManager;
    //GameObjectArray     mGameObjectArray;

    friend class Manager;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Game
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================