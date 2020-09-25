//=============================================================================
//y mrubyInput z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "mrubyMath.h"
#include "mrubyInput.h"

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
// ¡ Module_GameController
//=============================================================================

    RClass*             Module_GameController::mRubyModule   = NULL;

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Module_GameController::registerRubyObject( mrb_state* mrb_ )
    {
    }

//=============================================================================
// ¡ Module_Touch
//=============================================================================

    RClass*          Module_Touch::mRubyModule = NULL;
    Input::Touch*    Module_Touch::mTouch = NULL;

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Module_Touch::registerRubyObject( mrb_state* mrb_ )
    {
        mRubyModule = mrb_define_module( mrb_, "Touch" );

        mrb_define_module_function( mrb_, mRubyModule, "press?", is_press, ARGS_REQ(1) );
        mrb_define_module_function( mrb_, mRubyModule, "trigger?", is_trigger, ARGS_REQ(1) );
        mrb_define_module_function( mrb_, mRubyModule, "off_trigger?", is_off_trigger, ARGS_REQ(1) );
        mrb_define_module_function( mrb_, mRubyModule, "repeat?", is_repeat, ARGS_REQ(1) );
        mrb_define_module_function( mrb_, mRubyModule, "point", point, ARGS_REQ(1) );
        mrb_define_module_function( mrb_, mRubyModule, "moved_distance", moved_distance, ARGS_REQ(1) );
    
        mTouch = Engine::getFramework()->getInputManager()->getTouch();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Module_Touch::is_press( mrb_state* mrb_, mrb_value self_ )
    {
        mrb_int a1;
        mrb_get_args( mrb_, "i", &a1 );
        return LN_MRB_BOOL_VALUE( mTouch->isPress( a1 ) );
    }

	//---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Module_Touch::is_trigger( mrb_state* mrb_, mrb_value self_ )
    {
        mrb_int a1;
        mrb_get_args( mrb_, "i", &a1 );
        return LN_MRB_BOOL_VALUE( mTouch->isOnTrigger( a1 ) );
    }

	//---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Module_Touch::is_off_trigger( mrb_state* mrb_, mrb_value self_ )
    {
        mrb_int a1;
        mrb_get_args( mrb_, "i", &a1 );
        return LN_MRB_BOOL_VALUE( mTouch->isOffTrigger( a1 ) );
    }

	//---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Module_Touch::is_repeat( mrb_state* mrb_, mrb_value self_ )
    {
        mrb_int a1;
        mrb_get_args( mrb_, "i", &a1 );
        return LN_MRB_BOOL_VALUE( mTouch->isRepeat( a1 ) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Module_Touch::point( mrb_state* mrb_, mrb_value self_ )
    {
        mrb_int a1;
        mrb_get_args( mrb_, "i", &a1 );
        return Class_Vector2::createValue( mrb_, mTouch->getPoint( a1 ) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Module_Touch::moved_distance( mrb_state* mrb_, mrb_value self_ )
    {
        mrb_int a1;
        mrb_get_args( mrb_, "i", &a1 );
        return Class_Vector2::createValue( mrb_, mTouch->getMoveDistance( a1 ) );
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