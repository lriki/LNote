//=============================================================================
//�y mrubyInput �z
//-----------------------------------------------------------------------------
///**
//  @file       mrubyInput.h
//  @brief      mrubyInput
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
// �� Module_GameController
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class Module_GameController
{
public:

    /// Ruby �ɃI�u�W�F�N�g��o�^����
    static void registerRubyObject( mrb_state* mrb_ );

public:





private:

    static RClass*              mRubyModule;
};

//=============================================================================
// �� Module_Touch
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class Module_Touch
{
public:

    /// Ruby �ɃI�u�W�F�N�g��o�^����
    static void registerRubyObject( mrb_state* mrb_ );

public:

    /// press?(id)
	static mrb_value is_press( mrb_state* mrb_, mrb_value self_ );

	/// trigger?(id)
	static mrb_value is_trigger( mrb_state* mrb_, mrb_value self_ );

	/// off_trigger?(id)
	static mrb_value is_off_trigger( mrb_state* mrb_, mrb_value self_ );

	/// repeat?(id)
	static mrb_value is_repeat( mrb_state* mrb_, mrb_value self_ );

    /// point(id)
	static mrb_value point( mrb_state* mrb_, mrb_value self_ );

    /// moved_distance(id)
	static mrb_value moved_distance( mrb_state* mrb_, mrb_value self_ );

private:

    static RClass*          mRubyModule;
    static Input::Touch*    mTouch;
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