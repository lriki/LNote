//=============================================================================
//【 mrubyScene 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "mrubyGraphics.h"
#include "mrubyScene.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Binding
{
namespace mruby
{

//=============================================================================
// ■ Class_SceneNode
//=============================================================================

    const mrb_data_type Class_SceneNode::MRB_DATA_TYPE = { "SceneNode", deleteObject };
    RClass* Class_SceneNode::mRubyClass = NULL;
    
    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_SceneNode::registerRubyObject()
    {
        mrb_state* mrb = Engine::getMRBState();
        mRubyClass = mrb_define_class( mrb, "SceneNode", mrb->object_class );
        mrb_define_method( mrb, mRubyClass, "set_position", set_position, ARGS_REQ(2) | ARGS_OPT(1) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_SceneNode::deleteObject( mrb_state* mrb_, void* ptr_ )
    {
        Scene::SceneNode* obj = reinterpret_cast< Scene::SceneNode* >( ptr_ );
        LN_SAFE_RELEASE( obj );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_SceneNode::set_position( mrb_state* mrb_, mrb_value self_ )
    {
        mrb_float a1, a2, a3;
        int argc = mrb_get_args( mrb_, "ff|f", &a1, &a2, &a3 );

        Scene::SceneNode* obj = (Scene::SceneNode*)mrb_get_datatype( mrb_, self_, &MRB_DATA_TYPE );

        switch ( argc )
        {
            case 2:
                obj->setPosition( LVector3( a1, a2, 0 ) );
                break;
            case 3:
                obj->setPosition( LVector3( a1, a2, a3 ) );
                break;
        }

        return self_;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    //mrb_value Class_SceneNode::update( mrb_state* mrb_, mrb_value self_ )
    //{

    //}


//=============================================================================
// ■ Class_Viewport
//=============================================================================

    RClass*  Class_Viewport::mRubyClass = NULL;
    mrb_value Class_Viewport::mRootViewport;
    mrb_value Class_Viewport::m3DRootViewport;
    mrb_value Class_Viewport::m2DRootViewport;

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_Viewport::registerRubyObject( mrb_state* mrb_ )
    {
        mRubyClass = mrb_define_class( mrb_, "Viewport", Class_SceneNode::mRubyClass );
        //mrb_define_class_method( mrb_, mRubyClass, "new", newObject, ARGS_REQ(1) | ARGS_OPT(1) );
    
        mrb_define_class_method( mrb_, mRubyClass, "root", root, ARGS_NONE() );
        mrb_define_class_method( mrb_, mRubyClass, "root_2d", root_2d, ARGS_NONE() );
        mrb_define_class_method( mrb_, mRubyClass, "root_3d", root_3d, ARGS_NONE() );

        mrb_define_method( mrb_, mRubyClass, "set_tone", set_tone, ARGS_REQ(4) | ARGS_OPT(1) );

        // デフォルトビューポートを ruby インスタンス化
        //      deleteObject() が呼ばれたときに release されるため、ここで addRef している。
        //      Data_Wrap_Struct() で作ったインスタンスは mrb_state が管理してくれる。
        //      何回も Data_Wrap_Struct() しない限り、ruby 側に何回 mRootViewport を返しても
        //      deleteObject() が呼ばれるのは 1 度だけ。(mrb_state の解放時)
        Scene::Viewport* obj;

        obj = Engine::getFramework()->getSceneManager()->getDefaultSceneGraph()->getDefaultNode( LN_DEFAULT_SCENENODE_ROOT );
        mRootViewport = mrb_obj_value( Data_Wrap_Struct( mrb_, mRubyClass, &Class_SceneNode::MRB_DATA_TYPE, obj ) );
        obj->addRef();

        obj = Engine::getFramework()->getSceneManager()->getDefaultSceneGraph()->getDefaultNode( LN_DEFAULT_SCENENODE_2D );
        m2DRootViewport = mrb_obj_value( Data_Wrap_Struct( mrb_, mRubyClass, &Class_SceneNode::MRB_DATA_TYPE, obj ) );
        obj->addRef();

        obj = Engine::getFramework()->getSceneManager()->getDefaultSceneGraph()->getDefaultNode( LN_DEFAULT_SCENENODE_3D );
        m3DRootViewport = mrb_obj_value( Data_Wrap_Struct( mrb_, mRubyClass, &Class_SceneNode::MRB_DATA_TYPE, obj ) );
        obj->addRef();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    //static mrb_value newObject( mrb_state* mrb_, mrb_value self_ )
    //{
    //    mrb_int a1, a2;
    //    int argc = mrb_get_args( mrb_, "i|i", &a1, &a2 );

    //    //// 隠し関数的な感じでデフォルトのものを取得
    //    //if ( argc == 1 )
    //    //{
    //    //    LNDefaultSceneNode type = LN_MAX_DEFAULT_NODES;
    //    //    
    //    //    switch ( a1 )
    //    //    {
    //    //        case 100001: type = LN_DEFAULT_SCENENODE_ROOT; break;
    //    //        case 100002: type = LN_DEFAULT_SCENENODE_3D; break;
    //    //        case 100003: type = LN_DEFAULT_SCENENODE_2D; break;
    //    //        default: return mrb_->nil_class;
    //    //    }

    //    //    Scene::Viewport* obj = Engine::getFramework()->getSceneManager()->getDefaultSceneGraph()->getDefaultNode( type );
    //    //    return mrb_obj_value( Data_Wrap_Struct( mrb_, mRubyClass, &Class_SceneNode::MRB_DATA_TYPE, obj ) );
    //    //}
    //    //else
    //    //{
    //    //}

    //    return mrb_->nil_class;
    //}

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Viewport::set_tone( mrb_state* mrb_, mrb_value self_ )
    {
        mrb_float a1, a2, a3, a4, a5;
        int argc = mrb_get_args( mrb_, "ffff|f", &a1, &a2, &a3, &a4, &a5 );

        Scene::Viewport* obj = (Scene::Viewport*)mrb_get_datatype( mrb_, self_, &Class_SceneNode::MRB_DATA_TYPE );

        switch ( argc )
        {
            case 4:
                obj->setTone( LTone( a1, a2, a3, a4 ), 0 );
                break;
            case 5:
                obj->setTone( LTone( a1, a2, a3, a4 ), a5 );
                break;
        }

        return self_;
    }

//=============================================================================
// ■ Class_Sprite
//=============================================================================

    RClass*  Class_Sprite::mRubyClass = NULL;

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_Sprite::registerRubyObject( mrb_state* mrb_ )
    {
        mRubyClass = mrb_define_class( mrb_, "Sprite", Class_SceneNode::mRubyClass );
        mrb_define_class_method( mrb_, mRubyClass, "new", newObject, ARGS_OPT(1) );
        mrb_define_method( mrb_, mRubyClass, "set_texture", set_texture, ARGS_REQ(1) );
        mrb_define_method( mrb_, mRubyClass, "texture=", set_texture, ARGS_REQ(1) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Sprite::newObject( mrb_state* mrb_, mrb_value self_ )
    {
        mrb_value a1;
        int argc = mrb_get_args( mrb_, "|o", &a1 );

        Scene::Sprite* obj = LN_NEW Scene::Sprite(
            Engine::getFramework()->getSceneManager()->getDefaultSceneGraph() );
        obj->initialize();

        if ( argc == 1 )
        {
            obj->setTexture( Class_Texture::getObject( mrb_, a1 ) );
        }

        return mrb_obj_value( Data_Wrap_Struct( mrb_, mRubyClass, &Class_SceneNode::MRB_DATA_TYPE, obj ) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Sprite::set_texture( mrb_state* mrb_, mrb_value self_ )
    {
        mrb_value a1;
        mrb_get_args( mrb_, "o", &a1 );

        Scene::Sprite*      obj = (Scene::Sprite*)mrb_get_datatype( mrb_, self_, &Class_SceneNode::MRB_DATA_TYPE );
        Graphics::ITexture* tex = (Graphics::ITexture*)mrb_get_datatype( mrb_, a1, &Class_Texture::MRB_DATA_TYPE );
        
        obj->setTexture( tex );
        
        return self_;
    }

//=============================================================================
// ■ Class_Sprite3D
//=============================================================================

    RClass* Class_Sprite3D::mRubyClass   = NULL;

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_Sprite3D::registerRubyObject()
    {
        mrb_state* mrb = Engine::getMRBState();
        mRubyClass = mrb_define_class( mrb, "Sprite3D", Class_SceneNode::mRubyClass );//mrb->object_class );
        mrb_define_class_method( mrb, mRubyClass, "new", newObject, ARGS_REQ(2) );
        mrb_define_method( mrb, mRubyClass, "set_texture", set_texture, ARGS_REQ(1) );

    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Sprite3D::newObject( mrb_state* mrb_, mrb_value self_ )
    {
        mrb_float a1, a2;
        mrb_get_args( Engine::getMRBState(), "ff", &a1, &a2 );

        Scene::Sprite3D* obj = LN_NEW Scene::Sprite3D(
            Engine::getFramework()->getSceneManager()->getDefaultSceneGraph() );
        obj->initialize(
            static_cast< lnfloat >( a1 ),
            static_cast< lnfloat >( a2 ) );

        return mrb_obj_value( Data_Wrap_Struct( Engine::getMRBState(), mRubyClass, &Class_SceneNode::MRB_DATA_TYPE, obj ) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Sprite3D::set_texture( mrb_state* mrb_, mrb_value self_ )
    {
        mrb_value a1;
        mrb_get_args( mrb_, "o", &a1 );

        Scene::Sprite3D*    obj = (Scene::Sprite3D*)mrb_get_datatype( mrb_, self_, &Class_SceneNode::MRB_DATA_TYPE );
        Graphics::ITexture* tex = (Graphics::ITexture*)mrb_get_datatype( mrb_, a1, &Class_Texture::MRB_DATA_TYPE );
        
        obj->setTexture( tex );
        
        return self_;
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace mruby
} // namespace Binding
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================