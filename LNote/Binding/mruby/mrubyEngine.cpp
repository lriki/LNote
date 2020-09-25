//=============================================================================
//�y mrubyEngine �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "mrubyException.h"
#include "mrubyMath.h"
#include "mrubyFramework.h"
#include "mrubyInput.h"
#include "mrubyAudio.h"
#include "mrubyGraphics.h"
#include "mrubyScene.h"
#include "mrubyEngine.h"

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

    Framework*      Engine::mFramework      = NULL;
    mrb_state*      Engine::mMRubyState     = NULL;
    mrbc_context*   Engine::mMRubyContext   = NULL;
    std::string*    Engine::mScript         = NULL;

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Engine::initialize( const LNote::Core::Base::CommandLineArguments& args_, const ConfigData& data_ )
    {
        //---------------------------------------------------------
        // ���C�u����������

        ConfigData data = data_;
#if defined(LNOTE_ANDROID)
        data.GraphicsAPI = LN_GRAPHICSAPI_OPENGL;
#else
        data.GraphicsAPI = LN_GRAPHICSAPI_OPENGL;
        //data.GraphicsAPI = LN_GRAPHICSAPI_DIRECTX9;
#endif
        data.EnableMakeLogFile  = true;//args_.isExist( "-log" );
        data.EnableConsole      = true;//args_.isExist( "-console" );

        mFramework = createFramework();
        mFramework->initialize( data );

        //_p(args_.isExist( "-log" ));
        //_p(args_.isExist( "-console" ));
        //getchar();

        //---------------------------------------------------------
        // Ruby ������

        mMRubyState = mrb_open();
        mMRubyContext = mrbc_context_new( mMRubyState );

        Class_LNException::registerRubyObject();
        Class_Vector2::registerRubyObject( mMRubyState );
        Module_Framework::registerRubyObject();
        Module_Touch::registerRubyObject( mMRubyState );
        Module_Audio::registerRubyObject();
        Class_Texture::registerRubyObject();
        Class_SceneNode::registerRubyObject();
        Class_Viewport::registerRubyObject( mMRubyState );
        Class_Sprite::registerRubyObject( mMRubyState );
        Class_Sprite3D::registerRubyObject();
        
        //---------------------------------------------------------
        // �X�N���v�g�ǂݍ���

        mScript = LN_NEW std::string(); //"# -*- encoding: UTF-8 -*-\n" );

        // �t�@�C�����X�g����K�v�ȃt�@�C����ǂ�
        File::IInFile* file;
        File::Manager* fm = mFramework->getFileManager();
        fm->createInFile( &file, _T( "Data/Scripts/import_list" ) );
        file->load( false );

        std::string tmp_str = (char*)file->getData();
        _p(tmp_str.c_str());
        std::vector< std::string > import_list = Base::StringUtil::split( tmp_str, _T( ";" ) );

        //fm->lock();
        //fm->moveCurrentPath( _T( "Data/Scripts" ) );

        ln_foreach( const std::string& filename, import_list )
        {
            if ( !filename.empty() )
            {
                _p(filename.c_str());
                File::IInFile* sf;
                fm->createInFile( &sf, filename.c_str() );
                sf->load( false );
                (*mScript) += (char*)sf->getData();
                LN_SAFE_RELEASE( sf );
            }
        }

        //fm->returnCurrentPath();
        //fm->unlock();
         
        LN_SAFE_RELEASE( file );


        


        //---------------------------------------------------------
        // ���ׂĂ̍s�R�����g���X�y�[�X�ɒu������
        //      �G���[�ӏ��̓���̂��߁A���s�͍폜���Ȃ��B

        std::string::size_type pos   = 0;
        std::string::size_type begin = 0;
        std::string::size_type end   = 0;
        std::string to( " " );
        while ( begin = mScript->find( "#", pos ), begin != std::string::npos )
        {
            end = mScript->find( "\n", begin );

            // �s��
            if ( end != std::string::npos )
            {

                mScript->replace( begin, end - begin, to );
                pos += to.length();
            }
            // ������I�[
            else
            {
                mScript->replace( begin, mScript->size() - begin, to );
                pos += to.length();
            }
        }

        //LRenderState rs;
        ////rs.BlendMode = LN_BLEND_ALPHA;
        //rs.BlendMode = LN_BLEND_NORMAL;
        ////rs.FillMode = LN_FILL_WIRE;
        //rs.Culling = LN_CULL_DOUBLE;
        //mFramework->getGraphicsManager()->getGraphicsDevice()->getRenderer()->setRenderState( rs );
        
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Engine::finalize()
    {
        SAFE_DELETE( mScript );

        if ( mMRubyContext )
        {
            mrbc_context_free( mMRubyState, mMRubyContext );
            mMRubyContext = NULL;
        }
        if ( mMRubyState )
        {
            mrb_close( mMRubyState );
            mMRubyState = NULL;
        }
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
        if ( mScript )
        {
            mrb_parser_state* parser       = mrb_parse_string( mMRubyState, mScript->c_str(), mMRubyContext );
            int               byte_code    = mrb_generate_code( mMRubyState, parser );
            mrb_value return_value = mrb_run(
                mMRubyState,
                mrb_proc_new( mMRubyState, mMRubyState->irep[byte_code] ),
                mrb_top_self( mMRubyState ) );
            //mrb_load_string_cxt( mMRubyState, mScript->c_str(), mMRubyContext );

            if ( mMRubyState->exc ) // if exception occurred ...
            {
                mrb_p( mMRubyState, return_value );
                mMRubyState->exc = 0;
            }
        }
        
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    mrb_value Engine::p( mrb_state* mrb_, mrb_value self_ )
    {
        // char* str[256];
        mrb_value str;
        mrb_get_args( mrb_, "S", &str );

        const char* c_str = (const char*)mrb_string_value_cstr( mrb_, &str );
        printf( "%s\n", c_str );

        //    int nSize = 0;
        //ConvUtf8toSJis( pSource, NULL, &nSize );
        //BYTE* pDist = new BYTE[ nSize + 1 ];
        //ZeroMemory( pDist, nSize + 1 );
        //ConvUtf8toSJis( pSource, pDist, &nSize );


        //printf("p %s\n", pDist);//RSTRING_PTR(str));
        return self_;
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace mruby
} // namespace Binding
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================