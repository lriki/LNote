//=============================================================================
//【 Main 】
//-----------------------------------------------------------------------------
//  文字セットの設定はプロジェクトのプロパティで行っています。
//
//  
/*
    コマンドライン引数
        -test   ゲームデータのディレクトリを、実行ファイルのディレクトリ/Data にする。
                IDEから実行するときは常に指定されている。
*/
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"

//-------------------------------------------------------------------------
// EntryPoint
//-------------------------------------------------------------------------
int CommonEntryPoint(const LNote::Core::Base::CommandLineArguments& args_)
{
    LNote::Core::ConfigData data; 

    if (args_.isExist(_T("-test")))
    {
    }
    else
    {
        data.addArchive( ".", "Data.lna", "pass" );
    }

    //return lnMRubyInternalEntryPoint( args, data );
    return LNote::Binding::Ruby::Engine::entryPoint( args_, data );
}

//-------------------------------------------------------------------------
// WinMain
//-------------------------------------------------------------------------
#if defined(LNOTE_WINDOWS)
int WINAPI WinMain( HINSTANCE instance_, HINSTANCE prev_inst_, LPSTR cmdline_, int showcmd_ )
{
    LNote::Core::Base::CommandLineArguments args;
    args.createWin32();
    return CommonEntryPoint(args);
}
#endif

//-------------------------------------------------------------------------
// main
//-------------------------------------------------------------------------
int main( int argc_, char* argv_[] )
{
    LNote::Core::Base::CommandLineArguments args;
    args.create( argc_, argv_ );
    return CommonEntryPoint(args);
}


//=============================================================================
//								end of file
//=============================================================================