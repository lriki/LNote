//=============================================================================
//y mrubyGraphics z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "mrubyGraphics.h"

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
// ¡ Class_Texture
//=============================================================================

    RClass* Class_Texture::mRubyClass   = NULL;

    const mrb_data_type Class_Texture::MRB_DATA_TYPE = { "Texture", Class_Texture::deleteObject };

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_Texture::registerRubyObject()
    {
        mrb_state* mrb = Engine::getMRBState();
        mRubyClass = mrb_define_class( mrb, "Texture", mrb->object_class );
        mrb_define_class_method( mrb, mRubyClass, "new", newObject, ARGS_REQ(1) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Texture::newObject( mrb_state* mrb, mrb_value self_ )
    {
        mrb_value a1;
        mrb_get_args( mrb, "o", &a1 );

        Graphics::ITexture* obj;

        if ( mrb_type( a1 ) == MRB_TT_STRING )
        {
            Engine::getFramework()->getGraphicsManager()->getGraphicsDevice()->createTexture(
                &obj, RSTRING_PTR( a1 ) );
        }
        else
        {
            mrb_raise( mrb, E_ARGUMENT_ERROR, "Texture.new" );
        }

        return mrb_obj_value( Data_Wrap_Struct( mrb, mRubyClass, &MRB_DATA_TYPE, obj ) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_Texture::deleteObject( mrb_state* mrb_, void* ptr_ )
    {
        Graphics::ITexture* obj = reinterpret_cast< Graphics::ITexture* >( ptr_ );
        LN_SAFE_RELEASE( obj );
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