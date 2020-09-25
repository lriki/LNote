//=============================================================================
//y mrubyAudio z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "mrubyAudio.h"

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
// ¡ Module_Audio
//=============================================================================

    RClass*             Module_Audio::mRubyModule   = NULL;
    Audio::IGameAudio*  Module_Audio::mGameAudio    = NULL;

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Module_Audio::registerRubyObject()
    {
        mRubyModule = mrb_define_module( Engine::getMRBState(), "Audio" );
        mrb_define_module_function( Engine::getMRBState(), mRubyModule, "play_bgm", play_bgm, ARGS_REQ(1) | ARGS_OPT(3) );

        mGameAudio = Engine::getFramework()->getAudioManager()->getGameAudio();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Module_Audio::play_bgm( mrb_state* mrb_, mrb_value self_ )
    {
        mrb_value a1;
        mrb_int   a2, a3, a4;
        int argc = mrb_get_args( Engine::getMRBState(), "S|iii", &a1, &a2, &a3, &a4 );
        switch ( argc )
        {
            case 1:
                mGameAudio->playBGM( RSTRING_PTR( a1 ) );
                break;
            case 2:
                mGameAudio->playBGM( RSTRING_PTR( a1 ), a2 );
                break;
            case 3:
                mGameAudio->playBGM( RSTRING_PTR( a1 ), a2, a3 );
                break;
            case 4:
                mGameAudio->playBGM( RSTRING_PTR( a1 ), a2, a3, a4 );
                break;
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