//=============================================================================
//y mrubyFramework z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "mrubyException.h"
#include "mrubyFramework.h"

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
// ¡ Module_Framework
//=============================================================================

    RClass* Module_Framework::mRubyModule_Framework = NULL;

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Module_Framework::registerRubyObject()
    {
        mRubyModule_Framework = mrb_define_module( Engine::getMRBState(), "Framework" );
        mrb_define_module_function( Engine::getMRBState(), mRubyModule_Framework, "update", update, ARGS_NONE() );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Module_Framework::update( mrb_state* mrb_, mrb_value self_ )
    {
        Core::Graphics::DebugText* dt = Engine::getFramework()->getGraphicsManager()->getDebugText();
        dt->printf("fps:%.2f cap:%.2f\n", Engine::getFramework()->getGameTime().getFPS(), Engine::getFramework()->getFPSController()->getCapacityFPS());
        (*dt) << _T("count:") << Engine::getFramework()->getGameTime().getTotalFrameCount();

        if ( !Engine::getFramework()->update() )
        {
            Class_LNException::raiseExit();
        }
        return mrb_nil_value();
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