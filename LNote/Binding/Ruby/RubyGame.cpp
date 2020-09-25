//=============================================================================
//【 RubyGame 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Core/Game/GameSceneManager.h"
#include "RubyEngine.h"
#include "RubyGame.h"

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

    VALUE           Module_Core_GameSceneManager::mRubyModule;
    Game::Manager*  Module_Core_GameSceneManager::mGameManager = NULL;

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Module_Core_GameSceneManager::registerRubyObject()
    {
        mGameManager = Engine::getFramework()->getGameManager();

        mRubyModule = rb_define_module_under( Engine::getCoreModule(), "GameSceneManager" );
        //rb_define_module_function(mRubyModule, "register_active_scene_key_name", TO_RUBY_FUNC(register_active_scene_key_name), 1);
        rb_define_module_function(mRubyModule, "run", TO_RUBY_FUNC(run), 1);
        rb_define_module_function(mRubyModule, "goto", TO_RUBY_FUNC(goto_), 1);
        rb_define_module_function(mRubyModule, "call", TO_RUBY_FUNC(call), 1);
        rb_define_module_function(mRubyModule, "return", TO_RUBY_FUNC(return_), 0);
        rb_define_module_function(mRubyModule, "clear", TO_RUBY_FUNC(clear), 0);
        rb_define_module_function(mRubyModule, "exit", TO_RUBY_FUNC(exit), 0);
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    VALUE Module_Core_GameSceneManager::register_active_scene_key_name(VALUE self_, VALUE name_)
    {
        Check_Type(name_, T_STRING);
        mGameManager->registerActiveSceneKeyName(RSTRING_PTR(name_));
        return self_;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    VALUE Module_Core_GameSceneManager::run(VALUE self_, VALUE next_scene_)
    {
        Class_Core_GameScene* scene = GET_STRUCT_PTR(next_scene_, Class_Core_GameScene);
        scene->addRef();
        mGameManager->run(scene);
        return Qnil;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    VALUE Module_Core_GameSceneManager::goto_(VALUE self_, VALUE next_scene_)
    {
        Class_Core_GameScene* scene = GET_STRUCT_PTR(next_scene_, Class_Core_GameScene);
        scene->addRef();
        mGameManager->gotoGameScene(scene);
        return Qnil;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    VALUE Module_Core_GameSceneManager::call(VALUE self_, VALUE next_scene_)
    {
        Class_Core_GameScene* scene = GET_STRUCT_PTR(next_scene_, Class_Core_GameScene);
        scene->addRef();
        mGameManager->callGameScene(scene);
        return Qnil;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    VALUE Module_Core_GameSceneManager::return_(VALUE self_)
    {
        Class_Core_GameScene* scene = (Class_Core_GameScene*)mGameManager->getCurrentGameScene();
        mGameManager->returnGameScene();
        return ((Class_Core_GameScene*)scene->getParentGameScene())->mSelf;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    VALUE Module_Core_GameSceneManager::clear(VALUE self_)
    {
        return Qnil;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    VALUE Module_Core_GameSceneManager::exit(VALUE self_)
    {
        return Qnil;
    }

//=============================================================================
// ■ Class_Core_GameScene
//=============================================================================

    VALUE Class_Core_GameScene::mRubyClass;

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_Core_GameScene::registerRubyObject()
    {
        mRubyClass = rb_define_class_under(Engine::getCoreModule(), "GameScene", rb_cObject);
        rb_define_alloc_func(mRubyClass, allocateObject);
        rb_define_private_method(mRubyClass, "initialize", TO_RUBY_FUNC(initializeObject), 0);
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    VALUE Class_Core_GameScene::allocateObject(VALUE klass_)
    {
        Class_Core_GameScene* scene = LN_NEW Class_Core_GameScene();
        scene->mSelf = Data_Wrap_Struct(klass_, markObject, deleteObject, scene);
        return scene->mSelf;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    VALUE Class_Core_GameScene::initializeObject(VALUE self_)
    {
        Class_Core_GameScene* scene;
        Data_Get_Struct(self_, Class_Core_GameScene, scene);
        //scene->…
        return self_;
    }
    
    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_Core_GameScene::deleteObject(Core::Game::GameScene* scene_)
    {
        LN_SAFE_RELEASE(scene_);
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_Core_GameScene::markObject(Class_Core_GameScene* obj_)
    {
        rb_gc_mark(obj_->mSelf);

        // 親へ再帰的にマーク
        Class_Core_GameScene* parent = (Class_Core_GameScene*)obj_->getParentGameScene();
        if (parent) markObject(parent);
    }
    

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_Core_GameScene::onStart()
    {

        printf("onStart\n");
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_Core_GameScene::onResume()
    {
        printf("onResume\n");
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_Core_GameScene::onUpdate()
    {
        //printf("onUpdate\n");
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_Core_GameScene::onPause()
    {
        printf("onPause\n");
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_Core_GameScene::onTerminate()
    {
        printf("onTerminate\n");
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Ruby
} // namespace Binding
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================