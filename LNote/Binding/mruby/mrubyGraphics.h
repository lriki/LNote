//=============================================================================
//�y mrubyGraphics �z
//-----------------------------------------------------------------------------
///**
//  @file       mrubyGraphics.h
//  @brief      mrubyGraphics
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
// �� Class_Texture
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class Class_Texture
{
public:

    /// Ruby �ɃI�u�W�F�N�g��o�^����
    static void registerRubyObject();

public:

    /*
        #Texture.new(width, height) 

        Texture.new(filename[, color_key[, levels[, format]]]) 

        
    */
    static mrb_value newObject( mrb_state* mrb_, mrb_value self_ );

    static void deleteObject( mrb_state* mrb_, void* ptr_ );

    static Core::Graphics::ITexture* getObject( mrb_state* mrb_, mrb_value v_ ) { return (Core::Graphics::ITexture*)mrb_get_datatype( mrb_, v_, &Class_Texture::MRB_DATA_TYPE ); }
    
public:

    static const mrb_data_type MRB_DATA_TYPE;

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