//=============================================================================
//Åy mrubyMath Åz
//-----------------------------------------------------------------------------
///**
//  @file       mrubyMath.h
//  @brief      mrubyMath
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

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
// Å° Class_Vector2
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class Class_Vector2
{
public:

    /// Ruby Ç…ÉIÉuÉWÉFÉNÉgÇìoò^Ç∑ÇÈ
    static void registerRubyObject( mrb_state* mrb_ );

    /// Vector2.new()
    /// Vector2.new(x, y)
    static mrb_value newObject( mrb_state* mrb_, mrb_value self_ );

    /// (delete)
    static void deleteObject( mrb_state* mrb_, void* ptr_ );

public:

    /// x=
    static mrb_value set_x( mrb_state* mrb_, mrb_value self_ );

    /// x
    static mrb_value get_x( mrb_state* mrb_, mrb_value self_ );

    /// y=
    static mrb_value set_y( mrb_state* mrb_, mrb_value self_ );

    /// y
    static mrb_value get_y( mrb_state* mrb_, mrb_value self_ );

    /// length
    static mrb_value length( mrb_state* mrb_, mrb_value self_ );

    /// square_length
    static mrb_value square_length( mrb_state* mrb_, mrb_value self_ );

    /// normalize
    static mrb_value normalize( mrb_state* mrb_, mrb_value self_ );

    /// transform
    //static mrb_value transform( mrb_state* mrb_, mrb_value self_ ); 

    /// + (+=)
    static mrb_value add( mrb_state* mrb_, mrb_value self_ );

    /// - (-=)
    static mrb_value subtract( mrb_state* mrb_, mrb_value self_ );

    /// * (*=)
    static mrb_value multiply( mrb_state* mrb_, mrb_value self_ );

    /// / (/=)
    static mrb_value divide( mrb_state* mrb_, mrb_value self_ );

    /// p
    LNMRUBY_DUMP_P( LVector2 );

public:

    /// mrb_value âª
    static mrb_value createValue( mrb_state* mrb_, const LVector2& v_ ) { return LNMRUBY_DATA_WRAP( LN_NEW LVector2( v_ ) ); }

private:

    static const mrb_data_type MRB_DATA_TYPE;

    static RClass* mRubyClass;
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace mruby
} // namespace Binding
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================