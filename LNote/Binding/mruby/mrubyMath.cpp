//=============================================================================
//y mrubyMath z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "mrubyMath.h"

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
// ¡ Class_Vector2
//=============================================================================

    const mrb_data_type Class_Vector2::MRB_DATA_TYPE = { "Vector2", deleteObject };
    RClass*             Class_Vector2::mRubyClass   = NULL;

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_Vector2::registerRubyObject( mrb_state* mrb_ )
    {
        mRubyClass = mrb_define_class( mrb_, "Vector2", mrb_->object_class );
        //mrb_define_class_method( mrb_, mRubyClass, "new", newObject, ARGS_OPT(2) );

        
        mrb_define_class_method( mrb_, mRubyClass, "new", newObject, ARGS_REQ(2) );
mrb_define_class_method( mrb_, mRubyClass, "new", newObject, ARGS_NONE() );
        mrb_define_method( mrb_, mRubyClass, "x=", set_x, ARGS_REQ(1) );
        mrb_define_method( mrb_, mRubyClass, "x",  get_x, ARGS_NONE() );
        mrb_define_method( mrb_, mRubyClass, "y=", set_y, ARGS_REQ(1) );
        mrb_define_method( mrb_, mRubyClass, "y",  get_y, ARGS_NONE() );

        mrb_define_method( mrb_, mRubyClass, "length", length, ARGS_NONE() );
        mrb_define_method( mrb_, mRubyClass, "square_length", square_length, ARGS_NONE() );
        mrb_define_method( mrb_, mRubyClass, "normalize",  normalize, ARGS_NONE() );

        mrb_define_method( mrb_, mRubyClass, "+", add, ARGS_REQ(1) );
        mrb_define_method( mrb_, mRubyClass, "-", subtract, ARGS_REQ(1) );
        mrb_define_method( mrb_, mRubyClass, "*", multiply, ARGS_REQ(1) );
        mrb_define_method( mrb_, mRubyClass, "/", divide, ARGS_REQ(1) );

        mrb_define_method( mrb_, mRubyClass, "p", dump_p, ARGS_OPT(1) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Class_Vector2::deleteObject( mrb_state* mrb_, void* ptr_ )
    {
        LVector2* ptr = (LVector2*)ptr_;
        SAFE_DELETE( ptr );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Vector2::newObject( mrb_state* mrb_, mrb_value self_ )
    {
        mrb_float a1, a2;
        int argc = mrb_get_args( mrb_, "|ff", &a1, &a2 );

        LVector2* obj;
        
        if ( argc == 2 )
        {
            obj = LN_NEW LVector2( a1, a2 );
        }
        else if ( argc == 0 )
        {
            obj = LN_NEW LVector2();
        }
        else
        {
            Class_LNException::raiseArgumentError_wrong_number_of_arguments();
        }

        return mrb_obj_value( Data_Wrap_Struct( mrb_, mRubyClass, &MRB_DATA_TYPE, obj ) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Vector2::set_x( mrb_state* mrb_, mrb_value self_ )
    {
        mrb_float a1;
        mrb_get_args( mrb_, "f", &a1 );
        LNMRUBY_ACCESS_MEMBER( LVector2, x ) = a1;
        return mrb_float_value( a1 );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Vector2::get_x( mrb_state* mrb_, mrb_value self_ )
    {
        return mrb_float_value( LNMRUBY_ACCESS_MEMBER( LVector2, x ) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Vector2::set_y( mrb_state* mrb_, mrb_value self_ )
    {
        mrb_float a1;
        mrb_get_args( mrb_, "f", &a1 );
        LNMRUBY_ACCESS_MEMBER( LVector2, y ) = a1;
        return mrb_float_value( a1 );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Vector2::get_y( mrb_state* mrb_, mrb_value self_ )
    {
        return mrb_float_value( LNMRUBY_ACCESS_MEMBER( LVector2, y ) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Vector2::length( mrb_state* mrb_, mrb_value self_ )
    {
        return mrb_float_value( LNMRUBY_ACCESS_MEMBER( LVector2, getLength() ) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Vector2::square_length( mrb_state* mrb_, mrb_value self_ )
    {
        return mrb_float_value( LNMRUBY_ACCESS_MEMBER( LVector2, getSquareLength() ) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Vector2::normalize( mrb_state* mrb_, mrb_value self_ )
    {
        LNMRUBY_ACCESS_MEMBER( LVector2, normalize() );
        return self_;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    //mrb_value Class_Vector2::transform( mrb_state* mrb_, mrb_value self_ )
    //{
    //}    

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Vector2::add( mrb_state* mrb_, mrb_value self_ )
    {
        return LNMRUBY_DATA_WRAP(
            LN_NEW LVector2( LNMRUBY_CAST( LVector2, self_ ) + LNMRUBY_GET_ARG1( LVector2 ) ) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Vector2::subtract( mrb_state* mrb_, mrb_value self_ )
    {
        return LNMRUBY_DATA_WRAP(
            LN_NEW LVector2( LNMRUBY_CAST( LVector2, self_ ) - LNMRUBY_GET_ARG1( LVector2 ) ) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Vector2::multiply( mrb_state* mrb_, mrb_value self_ )
    {
        return LNMRUBY_DATA_WRAP(
            LN_NEW LVector2( LNMRUBY_CAST( LVector2, self_ ) * LNMRUBY_GET_ARG1( LVector2 ) ) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Class_Vector2::divide( mrb_state* mrb_, mrb_value self_ )
    {
        return LNMRUBY_DATA_WRAP(
            LN_NEW LVector2( LNMRUBY_CAST( LVector2, self_ ) / LNMRUBY_GET_ARG1( LVector2 ) ) );
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