//=============================================================================
//【 mrubyScene 】
//-----------------------------------------------------------------------------
///**
//  @file       mrubyScene.h
//  @brief      mrubyScene
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
// ■ Class_SceneNode
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class Class_SceneNode
{
public:

    /// Ruby にオブジェクトを登録する
    static void registerRubyObject();

    /// オブジェクトの解放処理
    static void deleteObject( mrb_state* mrb_, void* ptr_ );

public:

    /// set_position(x, y[, z])
    static mrb_value set_position( mrb_state* mrb_, mrb_value self_ );

    /// update
    static mrb_value update( mrb_state* mrb_, mrb_value self_ );

public:

    static const mrb_data_type MRB_DATA_TYPE;

    static RClass*  mRubyClass;
};

//=============================================================================
// ■ Class_Viewport
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class Class_Viewport
{
public:

    /// Ruby にオブジェクトを登録する
    static void registerRubyObject( mrb_state* mrb_ );

public:

    /// Viewport.new(id) 
    static mrb_value newObject( mrb_state* mrb_, mrb_value self_ );

    /// root
    static mrb_value root( mrb_state* mrb_, mrb_value self_ ) { return mRootViewport; }

    /// root_2d
    static mrb_value root_2d( mrb_state* mrb_, mrb_value self_ ) { return m2DRootViewport; }

    /// root_3d
    static mrb_value root_3d( mrb_state* mrb_, mrb_value self_ ) { return m3DRootViewport; }

    /// set_tone(red, green, blue, gray, duration = 0)
    static mrb_value set_tone( mrb_state* mrb_, mrb_value self_ );

private:

    static RClass*  mRubyClass;

    static mrb_value mRootViewport;
    static mrb_value m3DRootViewport;
    static mrb_value m2DRootViewport;
};

//=============================================================================
// ■ Class_Sprite
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class Class_Sprite
{
public:

    /// Ruby にオブジェクトを登録する
    static void registerRubyObject( mrb_state* mrb_ );

public:

    /// Sprite.new([texture]) 
    static mrb_value newObject( mrb_state* mrb_, mrb_value self_ );

    /// set_texture(texture)
    static mrb_value set_texture( mrb_state* mrb_, mrb_value self_ );

private:

    static RClass*  mRubyClass;
};

//=============================================================================
// ■ Class_Sprite3D
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class Class_Sprite3D
{
public:

    /// Ruby にオブジェクトを登録する
    static void registerRubyObject();

public:

    static mrb_value newObject( mrb_state* mrb_, mrb_value self_ );

    static mrb_value set_texture( mrb_state* mrb_, mrb_value self_ );

private:

    static RClass*  mRubyClass;
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