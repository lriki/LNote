//=============================================================================
//【 mrubyEngine 】
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
// ■ Engine
//=============================================================================
class Engine
{
public:

    /// 初期化
    static void initialize( const Core::Base::CommandLineArguments& args_, const Core::ConfigData& data_ );

    /// 終了処理
    static void finalize();

    /// 実行
    static void run();

    /// Framework 取得
    static Core::Framework* getFramework() { return mFramework; }

    /// mrb_state 取得
    static mrb_state* getMRBState() { return mMRubyState; }

private:

    /// p メソッド
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