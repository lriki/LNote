//=============================================================================
//�y RubyEngine �z
//-----------------------------------------------------------------------------
///**
//  @file       RubyEngine.h
//  @brief      RubyEngine
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../ScriptLinker.h"

typedef unsigned long VALUE;

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Binding
{
namespace Ruby
{

//=============================================================================
// �� Engine
//=============================================================================
class Engine
{
public:

    /// �G���g���[�|�C���g
    static int entryPoint(const LNote::Core::Base::CommandLineArguments& args_, const LNote::Core::ConfigData& data_);

public:

    /// ������
    static void initialize(const Core::Base::CommandLineArguments& args_, const Core::ConfigData& data_);

    /// �I������
    static void finalize();

    /// ���s
    static void run();

    /// Framework �擾
    static Core::Framework* getFramework() { return mFramework; }

    static VALUE getRubyModule() { return mRubyModule; }
    static VALUE getCoreModule() { return mCoreModule; }
   
private:

    /// p ���\�b�h
    static VALUE ruby_p(int argc_, VALUE* argv_, VALUE self_);

private:

    static ScriptLinker*    mScriptLinker;
    static Core::Framework* mFramework;
    static VALUE            mRubyModule;
    static VALUE            mCoreModule;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Ruby
} // namespace Binding
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================