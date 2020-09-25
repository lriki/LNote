//=============================================================================
//【 RubyGame 】
//-----------------------------------------------------------------------------
///**
//  @file       RubyGame.h
//  @brief      RubyGame
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Core/Game/GameScene.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Binding
{
namespace Ruby
{

//=============================================================================
// ■ Module_Core_GameSceneManager
//=============================================================================
class Module_Core_GameSceneManager
{
public:

    static void registerRubyObject();

    static VALUE register_active_scene_key_name(VALUE self_, VALUE name_);

    static VALUE run(VALUE self_, VALUE next_scene_);

    static VALUE goto_(VALUE self_, VALUE next_scene_);

    static VALUE call(VALUE self_, VALUE next_scene_);

    static VALUE return_(VALUE self_);

    static VALUE clear(VALUE self_);

    static VALUE exit(VALUE self_);

private:

    static VALUE            mRubyModule;
    static Game::Manager*   mGameManager;
};

//=============================================================================
// ■ Class_Core_GameScene
//=============================================================================
class Class_Core_GameScene
    : public Core::Game::GameScene
{
public:

    static void registerRubyObject();

    static VALUE allocateObject(VALUE klass_);
    static VALUE initializeObject(VALUE self_);
    static void deleteObject(Core::Game::GameScene* scene_);
    static void markObject(Class_Core_GameScene* obj_);
  
private:

    static VALUE mRubyClass;

public:

    /// シーン開始
    virtual void onStart();

    /// 再開 (onStart() 直後、または子シーンが終了し、このシーンがアクティブになった)
    virtual void onResume();

    /// フレーム更新
    virtual void onUpdate();

    /// 停止 (onExit() 直前、または子シーンがアクティブになった)
    virtual void onPause();

    /// シーン終了
    virtual void onTerminate();

private:

    VALUE mSelf;

    friend class Module_Core_GameSceneManager;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Ruby
} // namespace Binding
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================