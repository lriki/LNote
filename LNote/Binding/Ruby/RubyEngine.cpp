//=============================================================================
//【 RubyEngine 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "RubyEngine.h"
#include "RubyException.h"
#include "RubyFramework.h"
#include "RubyGame.h"

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

    ScriptLinker*   Engine::mScriptLinker = NULL;
    Framework*      Engine::mFramework      = NULL;
    VALUE           Engine::mRubyModule;
    VALUE           Engine::mCoreModule;

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    int Engine::entryPoint(const LNote::Core::Base::CommandLineArguments& args_, const LNote::Core::ConfigData& data_)
    {
        initialize(args_, data_);
        run();
        finalize();
        return 0;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Engine::initialize(const LNote::Core::Base::CommandLineArguments& args_, const ConfigData& data_)
    {
        //---------------------------------------------------------
        // ライブラリ初期化

        ConfigData data = data_;
        //data.GraphicsAPI = LN_GRAPHICSAPI_OPENGL;
        data.GraphicsAPI = LN_GRAPHICSAPI_DIRECTX9;
        data.EnableMakeLogFile  = true;//args_.isExist( "-log" );
        data.EnableConsole      = true;//args_.isExist( "-console" );

        mFramework = createFramework();
        mFramework->initialize( data );


        //---------------------------------------------------------
        // Ruby 初期化

        ruby_init();
        ruby_init_loadpath();
        rb_gc_enable();
        rb_gc_start();

        mRubyModule = rb_define_module("LNote");
        mCoreModule = rb_define_module_under(mRubyModule, "Core");

        Class_LNException::registerRubyObject();
        Module_Framework::registerRubyObject();
        Module_Core_GameSceneManager::registerRubyObject();
        Class_Core_GameScene::registerRubyObject();

        // include LNote
        rb_include_module( rb_cObject, mRubyModule );

        // p メソッドをオーバーライド
        rb_define_global_function( "p", TO_RUBY_FUNC( ruby_p ), -1 );

        //---------------------------------------------------------
        // スクリプト読み込み

        mScriptLinker = LN_NEW ScriptLinker();
        mScriptLinker->initialize(mFramework->getFileManager());

        File::IInFile* file;
        File::Manager* fm = mFramework->getFileManager();
        fm->createInFile( &file, _T( "Data/Scripts/import_list" ), LN_OPENMODE_TEXT );
        file->load( false );
        std::string tmp_str = (char*)file->getData();
        std::vector< std::string > import_list = Base::StringUtil::split( tmp_str, _T("\n") );

        ln_foreach( const std::string& filename, import_list )
        {
            if ( !filename.empty() )
            {
                mScriptLinker->addFile(filename.c_str());
            }
        }

        LN_SAFE_RELEASE(file);
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Engine::finalize()
    {
        rb_gc();
        rb_gc_disable();
        ruby_cleanup( 0 );

        SAFE_DELETE(mScriptLinker);

        if ( mFramework )
        {
            mFramework->finalize();
            LN_SAFE_RELEASE( mFramework );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Engine::run()
    {
        int state;
        rb_eval_string_protect( mScriptLinker->getScriptCode(), &state );
        if ( state )
        {
            // エラーメッセージを出力
            VALUE exception = rb_eval_string("$!.to_s");
            char* err = RSTRING_PTR(exception);
            printf("ruby error:\n%s\n", err);
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    VALUE Engine::ruby_p(int argc_, VALUE* argv_, VALUE self_)
    {
        // Rubyの文字列として初期化
        VALUE str = rb_str_new( "", 0 );

        // 引数の数だけループ
        for ( int i = 0; i < argc_; ++i )
        {
            // 間にカンマを挿入
            if ( i > 0 )
            {
                rb_str_cat( str, ", ", 2 );
            }

            // 引数に渡されたオブジェクトを文字列化して変数strに加える
            rb_str_concat( str, rb_inspect( argv_[i] ) );
        }

        // 出力
        printf( "%s\n", StringValueCStr( str ) );
        
        // 関数の戻り値としてnilを返す
        return Qnil;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Ruby
} // namespace Binding
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================