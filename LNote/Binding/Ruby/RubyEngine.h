//=============================================================================
//【 RubyEngine 】
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
// ■ Engine
//=============================================================================
class Engine
{
public:

    /// エントリーポイント
    static int entryPoint(const LNote::Core::Base::CommandLineArguments& args_, const LNote::Core::ConfigData& data_);

public:

    /// 初期化
    static void initialize(const Core::Base::CommandLineArguments& args_, const Core::ConfigData& data_);

    /// 終了処理
    static void finalize();

    /// 実行
    static void run();

    /// Framework 取得
    static Core::Framework* getFramework() { return mFramework; }

    static VALUE getRubyModule() { return mRubyModule; }
    static VALUE getCoreModule() { return mCoreModule; }
   
private:

    /// p メソッド
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