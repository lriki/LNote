//=============================================================================
//�y mrubyEngine �z
//-----------------------------------------------------------------------------
///**
//  @file       mrubyEngine.h
//  @brief      mrubyEngine
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
// �� Engine
//=============================================================================
class Engine
{
public:

    /// ������
    static void initialize( const Core::Base::CommandLineArguments& args_, const Core::ConfigData& data_ );

    /// �I������
    static void finalize();

    /// ���s
    static void run();

    /// Framework �擾
    static Core::Framework* getFramework() { return mFramework; }

    /// mrb_state �擾
    static mrb_state* getMRBState() { return mMRubyState; }

private:

    /// p ���\�b�h
    static mrb_value p( mrb_state* mrb_, mrb_value self_ );

private:

    static Core::Framework* mFramework;
    static mrb_state*       mMRubyState;
    static mrbc_context*    mMRubyContext;
    static std::string*     mScript;
    
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