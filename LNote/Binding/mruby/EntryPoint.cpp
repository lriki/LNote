//=============================================================================
//【 Main 】
//-----------------------------------------------------------------------------
//  文字セットの設定はプロジェクトのプロパティで行っています。
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"


//-------------------------------------------------------------------------
// 内部エントリーポイント
//-------------------------------------------------------------------------
int lnMRubyInternalEntryPoint( const LNote::Core::Base::CommandLineArguments& args_, const Core::ConfigData& data_ )
{
    ConfigData data = data_;
    
#if defined(LNOTE_WINDOWS)
#if defined(LNOTE_DEBUG)
    printf( "changing current directory for debug ...\n" );
    ::SetCurrentDirectory( _T( "C:/Users/admin/Documents/TalesNote/EngineTest" ) );
#else    
    data.addArchive( ".", "Data.lna", "pass" );
#endif
#endif

    try
    {
        LNote::Binding::mruby::Engine::initialize( args_, data );
        LNote::Binding::mruby::Engine::run();
    }
    catch ( Core::Base::Exception e )
    {
        e.process();
    }
    LNote::Binding::mruby::Engine::finalize();

    return 0;
}

//=============================================================================
//								end of file
//=============================================================================
