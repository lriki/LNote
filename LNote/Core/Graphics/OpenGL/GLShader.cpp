//=============================================================================
//【 GLShader 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Task/Manager.h"
#include "../../File/Manager.h"
#include "../../Geometry/Vector4.h"
#include "../../Geometry/Matrix.h"
#include "../Common/GraphicsUtil.h"
#include "../Manager.h"
#include "LNGL/LNGL.h"
#include "GLGraphicsDevice.h"
#include "GLTexture.h"
#include "GLShader.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace OpenGL
{

//=============================================================================
// ■ Shader クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    Shader::Shader( ShaderManager* manager_ )
        : mManager          ( manager_ )
        , mVertexShader     ( NULL )
        , mFragmentShader   ( NULL )
        , mProgram          ( NULL )
        , mCompileResult    ( LN_SHADERRESULT_ERROR )
        , mShaderTechnique  ( NULL )
        , mLastTextureUnit  ( 0 )
    {
        LN_SAFE_ADDREF( mManager );
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    Shader::~Shader()
    {
        if (mManager)
        {
            mManager->removeShader( this );
        }

        onDisposeGLResource();

        // ShaderTechnique 解放
        LN_SAFE_RELEASE( mShaderTechnique );

        // ShaderVariable 解放
        for ( u32 i = 0; i < mShaderVariableArray.size(); ++i )
        {
            SAFE_DELETE( mShaderVariableArray[ i ] );
        }
        mShaderVariableArray.clear();


        LN_SAFE_RELEASE( mManager );
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    void Shader::initialize( const lnChar* filename_ )
    {
        mFilename = filename_;

        // ここで ShaderVariable 等がすべて構築できていないと、
        // ゲームスレッド側から初期値の設定などができなくなる。
        // なので、描画スレッドの待機待ちからの作成を行う。
        // なんかもうパフォーマンス低下はやむを得ないということで…。

        //if (mManager->getGraphicsDevice()->isUseDrawThread())
        //{
        //    Task::Manager* tm = mManager->getGraphicsDevice()->getManager()->getTaskManager();
        //    if (Thread::getCurrentThreadID() == tm->getMasterPipelineThread()->getThreadID())
        //    {
        //        _createOnInitial();
        //    }
        //    else
        //    {
        //        Initializer ir(this);
        //        tm->postMasterTaskSubDelegate(
        //            Base::createDelegate(&ir, &Initializer::initialize ));
        //        ir.waitInitialized();
        //    }
        //}
        //else
        {
            _createOnInitial();
        }

        // スクリプト実行
        if ( !mScript.empty() )
        {
            mManager->doScript( mScript.c_str(), this );
        }
    }

    //---------------------------------------------------------------------
    // ● ブール値の設定
    //---------------------------------------------------------------------
	LNRESULT Shader::setBool( const lnChar* name_, bool b_ )
    {
        return getVariableByName( name_ )->setBool( b_ );
    }

    //---------------------------------------------------------------------
    // ● 整数値の設定
    //---------------------------------------------------------------------
	LNRESULT Shader::setInt( const lnChar* name_, int value_ )
    {
        return getVariableByName( name_ )->setInt( value_ );
    }

    //---------------------------------------------------------------------
    // ● 実数値の設定
    //---------------------------------------------------------------------
	LNRESULT Shader::setFloat( const lnChar* name_, lnfloat value_ )
    {
        return getVariableByName( name_ )->setFloat( value_ );
    }

    //---------------------------------------------------------------------
    // ● ベクトルの設定
    //---------------------------------------------------------------------
	LNRESULT Shader::setVector( const lnChar* name_, const LVector4& vec_ )
    { 
        return getVariableByName( name_ )->setVector( vec_ );
    }

    //---------------------------------------------------------------------
    // ● ベクトル配列の設定
    //---------------------------------------------------------------------
	LNRESULT Shader::setVectorArray( const lnChar* name_, const LVector4* vec_, u32 count_ )
    {
        return getVariableByName( name_ )->setVectorArray( vec_, count_ );
    }

    //---------------------------------------------------------------------
    // ● 行列の設定
    //---------------------------------------------------------------------
	LNRESULT Shader::setMatrix( const lnChar* name_, const LMatrix& matrix_ )
    {
        return getVariableByName( name_ )->setMatrix( matrix_ );
    }

    //---------------------------------------------------------------------
    // ● 行列の配列の設定
    //---------------------------------------------------------------------
	LNRESULT Shader::setMatrixArray( const lnChar* name_, const LMatrix* matrices_, u32 count_ )
    {
        return getVariableByName( name_ )->setMatrixArray( matrices_, count_ );
    }
   
    //---------------------------------------------------------------------
    // ● テクスチャの設定
    //---------------------------------------------------------------------
	LNRESULT Shader::setTexture( const lnChar* name_, ITexture* texture_ )
    {
        return getVariableByName( name_ )->setTexture( texture_ );
    }

    //---------------------------------------------------------------------
    // ● 名前によって変数を取得する
    //---------------------------------------------------------------------
    IShaderVariable* Shader::getVariableByName( const lnChar* name_ )
    {
        ShaderVariableMap::iterator it = mShaderVariableMap.find( name_ );
        if ( it != mShaderVariableMap.end() )
        {
            return it->second;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    // ● インデックスによって変数を取得する
    //---------------------------------------------------------------------
    IShaderVariable* Shader::getVariableByIndex( u32 index_ )
    {
        if ( index_ >= mShaderVariableArray.size() ) { return NULL; }
        return mShaderVariableArray[ index_ ];
    }

    //// セマンティクスによって変数を取得する
    //IShaderVariable* Shader::getVariableBySemantic( const char* name_ )
    //{
    //    return NULL;
    //}
    
    //---------------------------------------------------------------------
    // ● 名前によってテクニックを取得する
    //---------------------------------------------------------------------
    IShaderTechnique* Shader::getTechniqueByName( const lnChar* name_ )
    {
        return mShaderTechnique;
    }

    //---------------------------------------------------------------------
    // ● インデックスによってテクニックを取得する
    //---------------------------------------------------------------------
    IShaderTechnique* Shader::getTechniqueByIndex( u32 index_ )
    {
        if ( index_ == 0 )
        {
            return mShaderTechnique;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Shader::onCommit()
    {
        lnU32 n = mShaderVariableArray.size();
        for ( lnU32 i = 0; i < n; ++i )
        {
            mShaderVariableArray[ i ]->onCommit();
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Shader::onCommitGLResource()
    {
        if (!mProgram)
        {
            _createOnRefresh();
        }

        // onCommit() は別途 begin() 等から呼ばれるので、ここでは必要ないかも
        //onCommit()
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Shader::onDisposeGLResource()
    {
        // 子要素
        ln_foreach(ShaderVariable* v, mShaderVariableArray)
        {
            v->onDisposeGLResource();
        }
        if (mShaderTechnique)
        {
            mShaderTechnique->onDisposeGLResource();
        }

        // シェーダオブジェクトの削除
        //if ( mVertexShader )
        //{
        //    LNGL::glDeleteShader( mVertexShader );
        //    //glGetError();
        //    mVertexShader = NULL;
        //}
        //if ( mFragmentShader )
        //{
        //    LNGL::glDeleteShader( mFragmentShader );
        //    //glGetError();
        //    mFragmentShader = NULL;
        //}

        // プログラムの削除
        if ( mProgram )
        {
            LNGL::glDeleteProgram( mProgram );
            //glGetError();
            mProgram = NULL;
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Shader::_createOnInitial()
    {
        //---------------------------------------------------------
        // シェーダプログラムのビルド
        _buildFromFile();

        //---------------------------------------------------------
        // すべての変数を列挙

        // uniform 変数の数を調べて、その数分 ShaderVariable 作成
        GLint params;
        LNGL::glGetProgramiv( mProgram, GL_ACTIVE_UNIFORMS, &params );
        //LN_CHECK_GLERROR();

        for ( int i = 0; i < params; ++i )
        {
            ShaderVariable* v = LN_NEW ShaderVariable( this, i );
            v->onCreateGLResource();
            mShaderVariableMap.insert( ShaderVariablePair( v->getName(), v ) );
            mShaderVariableArray.push_back( v );
        }

        //---------------------------------------------------------
        // シェーダ情報

        mDesc.ParameterNum = params;
        mDesc.TechniqueNum = 1;

        //---------------------------------------------------------
        // テクニック作成

        mShaderTechnique = LN_NEW ShaderTechnique( this );


        //---------------------------------------------------------
        // GLリソース確保
        //ln_foreach(ShaderVariable* v, mShaderVariableArray)
        //{
        //    
        //}
        mShaderTechnique->onCreateGLResource();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Shader::_createOnRefresh()
    {
        //---------------------------------------------------------
        // シェーダプログラムのビルド
        _buildFromFile();

        //---------------------------------------------------------
        // GLリソース確保
        ln_foreach(ShaderVariable* v, mShaderVariableArray)
        {
            v->onCreateGLResource();
        }
        mShaderTechnique->onCreateGLResource();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Shader::_buildFromFile()
    {
        File::IInFile* file = NULL;
        mManager->getFileManager()->createInFile( &file, mFilename.c_str() );
        Base::FinallyReleasePtr< File::IInFile > file_ptr( file );

        // mTempBuffer に全部読み込む
        lnU32 size = file->getSize();
        char* buf = (char*)mManager->getTempBuffer().lock( size + 1 );
        file->read( buf, size );
        buf[ size ] = '\0';

        _build( buf );

        // TODO:lockミス。
        mManager->getTempBuffer().unlock();

        if ( !mCompileErrors.empty() )
        {
            ln_std_tstring str = mFilename.c_str();
            str += _T("\n");
            str += mCompileErrors.c_str();
            LN_ERRLOG_WRITE( str.c_str() );
            lnprintf( "%s\n", mFilename.c_str() );
            lnprintf( mCompileErrors.c_str() );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Shader::_build( const char* code_ )
    {
        mCompileResult = LN_SHADERRESULT_ERROR; // エラーにしておく
        mCompileErrors.clear();

        //---------------------------------------------------------
        // コード分割
#if 0
#define VERTEX_SHADER_TAG       "VERTEX_SHADER:"
#define VERTEX_SHADER_TAG_LEN   ( 14 )
#define FRAGMENT_SHADER_TAG     "FRAGMENT_SHADER:"
#define FRAGMENT_SHADER_TAG_LEN ( 16 )
#define METADATA_TAG            "METADATA:"
#define METADATA_TAG_LEN        ( 9 )

        std::string str( code_ );
        
        // "VERTEX_SHADER:" を探す
	    std::string::size_type vs_index = str.find( VERTEX_SHADER_TAG );
        if ( vs_index == std::string::npos )
        {
            mCompileErrors = "not found "VERTEX_SHADER_TAG;
            return;
        }
	    
        // "FRAGMENT_SHADER:" を探す
	    std::string::size_type fs_index = str.find( FRAGMENT_SHADER_TAG );
        if ( fs_index == std::string::npos )
        {
            mCompileErrors = "not found "FRAGMENT_SHADER_TAG;
            return;
        }
	    
        // 頂点シェーダコード
        std::string vs_code( str, vs_index + VERTEX_SHADER_TAG_LEN, fs_index - vs_index - VERTEX_SHADER_TAG_LEN );
        
        // "METADATA:" を探す
	    std::string::size_type mt_index = str.find( METADATA_TAG );
        if ( fs_index == std::string::npos )
        {
            mCompileErrors = "not found "METADATA_TAG;
            return;
        }

        // フラグメントシェーダコード
        std::string fs_code( str, fs_index + FRAGMENT_SHADER_TAG_LEN, mt_index - fs_index - FRAGMENT_SHADER_TAG_LEN );

        // メタデータコード
        mScript = std::string( str.c_str() + (mt_index + METADATA_TAG_LEN) );
#endif

        // ソース分割
        LNFXShaderSource source;
        source.initialize(code_, LNFXShaderSource::SHADER_GLSL);

        // メタデータコード
        mScript = source.getGLSLMetaDataSource();





        // 頂点シェーダ
        const char* vs_codes[] = 
        {
            "#define LN_GLSL_VERT 1\n",
            source.getSourceData()
        };
        int vs_sizes[] =
        {
            strlen(vs_codes[0]),
            source.getSourceDataSize(),
        };
        //if ( !_compileShader( mVertexShader, 2, vs_codes, vs_sizes ) ) { return; }

        // フラグメントシェーダ
        const char* fs_codes[] = 
        {
            "#define LN_GLSL_FLAG 1\n",
            source.getSourceData()
        };
        int fs_sizes[] =
        {
            strlen(fs_codes[0]),
            source.getSourceDataSize(),
        };

        LNGL::ShaderBuildArgs args;
        args.vs_codes = vs_codes;
        args.vs_sizes = vs_sizes;
        args.vs_count = 2;
        args.fs_codes = fs_codes;
        args.fs_sizes = fs_sizes;
        args.fs_count = 2;
        bool error = false;
        LNGL::lnBuildShader(&args, &mProgram, &mCompileResult, &mCompileErrors);

#if 0
        //---------------------------------------------------------
        // プログラムのビルド

        mVertexShader   = LNGL::glCreateShader( GL_VERTEX_SHADER );
        //LN_CHECK_GLERROR();

	    mFragmentShader = LNGL::glCreateShader( GL_FRAGMENT_SHADER );
        //LN_CHECK_GLERROR();

        //_p(vs_code.c_str());
        //_p(fs_code.c_str());

        // 頂点シェーダ
        const char* vs_codes[] = 
        {
            "#define LN_GLSL_VERT 1\n",
            source.getSourceData()
        };
        int vs_sizes[] =
        {
            strlen(vs_codes[0]),
            source.getSourceDataSize(),
        };
        if ( !_compileShader( mVertexShader, 2, vs_codes, vs_sizes ) ) { return; }

        // フラグメントシェーダ
        const char* fs_codes[] = 
        {
            "#define LN_GLSL_FLAG 1\n",
            source.getSourceData()
        };
        int fs_sizes[] =
        {
            strlen(fs_codes[0]),
            source.getSourceDataSize(),
        };
        if ( !_compileShader( mFragmentShader, 2, fs_codes, fs_sizes ) ) { return; }
        
        // プログラムオブジェクトの作成
        mProgram = LNGL::glCreateProgram();
        //LN_CHECK_GLERROR();

        // シェーダオブジェクトのシェーダプログラムへの登録
        LNGL::glAttachShader( mProgram, mVertexShader );
        //LN_CHECK_GLERROR();

        LNGL::glAttachShader( mProgram, mFragmentShader );
        //LN_CHECK_GLERROR();

        // シェーダオブジェクトの削除
        //glDeleteShader( mVertexShader );
        //glDeleteShader( mFragmentShader );

        // シェーダプログラムのリンク
        GLint linked;
        LNGL::glLinkProgram( mProgram );
        //LN_CHECK_GLERROR();

        LNGL::glGetProgramiv( mProgram, GL_LINK_STATUS, &linked );
        //LN_CHECK_GLERROR();

        // ログがあるかチェック
        int log_size;
	    int length;
	    LNGL::glGetProgramiv( mProgram, GL_INFO_LOG_LENGTH, &log_size );
        //LN_CHECK_GLERROR();
	    if ( log_size > 1 )
	    {
            char* buf = LN_NEW char[log_size];
            LNGL::glGetProgramInfoLog( mProgram, log_size, &length, buf );
            //LN_CHECK_GLERROR();

            mCompileErrors += "Program Info Log:\n";
            mCompileErrors += buf;
            SAFE_DELETE_ARRAY( buf );
	    }

        if ( linked == GL_FALSE ) { return; }

        //---------------------------------------------------------
        // ログがある場合はとりあえず警告にしておく

        if ( mCompileErrors.empty() )
        {
            mCompileResult = LN_SHADERRESULT_SUCCESS;
        }
        else
        {
            mCompileResult = LN_SHADERRESULT_WARNING;
        }

#endif
    }
    
    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    bool Shader::_compileShader( GLuint shader_, int code_count_, const char** code_, GLint* size_ )
    {
#if 0
        // シェーダオブジェクトにソースプログラムをセット
        LNGL::glShaderSource( shader_, code_count_, (const GLchar **)code_, size_ );
        //LN_CHECK_GLERROR();
      
        // シェーダのコンパイル
        GLint compiled;
        LNGL::glCompileShader( shader_ );
        //LN_CHECK_GLERROR();

        LNGL::glGetShaderiv( shader_, GL_COMPILE_STATUS, &compiled );
        //LN_CHECK_GLERROR();
        
        // ログがあるかチェック (ログの長さは、最後のNULL文字も含む)
        int log_size;
        int length;
        LNGL::glGetShaderiv( shader_, GL_INFO_LOG_LENGTH , &log_size );
        //LN_CHECK_GLERROR();
        if ( log_size > 1 )
        {
            char* buf = LN_NEW char[log_size];
            LNGL::glGetShaderInfoLog( shader_, log_size, &length, buf );
            //LN_CHECK_GLERROR();

            mCompileErrors += "Shader Info Log:\n";
            mCompileErrors += buf;
            SAFE_DELETE_ARRAY( buf );
        }

        return ( compiled != GL_FALSE );
#endif
        return false;
    }

    
//=============================================================================
// ■ ShaderVariable クラス
//=============================================================================


    void convertVariableTypeGLToLN( GLenum gl_type_, GLsizei gl_var_size_, LNShaderVariableTypeDesc* desc_ )
    {
        desc_->Elements = gl_var_size_;
        desc_->Shared = false;

#define SET_LNDESC( c_, t_, row_, col_ ) { desc_->Class = (c_); desc_->Type = (t_); desc_->Rows = (row_); desc_->Columns = (col_); }

        switch ( gl_type_ )
        {
            case GL_FLOAT:      SET_LNDESC( LN_SVC_SCALAR, LN_SVT_FLOAT, 1, 1 ); break;
            case GL_FLOAT_VEC2: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 2 ); break;
            case GL_FLOAT_VEC3: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 3 ); break;
            case GL_FLOAT_VEC4: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 4 ); break;

            case GL_INT:        SET_LNDESC( LN_SVC_SCALAR, LN_SVT_INT, 1, 1 ); break;
            case GL_INT_VEC2:   SET_LNDESC( LN_SVC_VECTOR, LN_SVT_INT, 1, 2 ); break;
            case GL_INT_VEC3:   SET_LNDESC( LN_SVC_VECTOR, LN_SVT_INT, 1, 3 ); break;
            case GL_INT_VEC4:   SET_LNDESC( LN_SVC_VECTOR, LN_SVT_INT, 1, 4 ); break;

            case GL_BOOL:        SET_LNDESC( LN_SVC_SCALAR, LN_SVT_BOOL, 1, 1 ); break;
            case GL_BOOL_VEC2:   SET_LNDESC( LN_SVC_VECTOR, LN_SVT_BOOL, 1, 2 ); break;
            case GL_BOOL_VEC3:   SET_LNDESC( LN_SVC_VECTOR, LN_SVT_BOOL, 1, 3 ); break;
            case GL_BOOL_VEC4:   SET_LNDESC( LN_SVC_VECTOR, LN_SVT_BOOL, 1, 4 ); break;

            case GL_FLOAT_MAT2:     SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 2, 2 ); break;
            case GL_FLOAT_MAT3:     SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 3, 3 ); break;
            case GL_FLOAT_MAT4:     SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 4, 4 ); break;
            
            case GL_SAMPLER_2D:         SET_LNDESC( LN_SVC_SAMPLER, LN_SVT_SAMPLER2D, 1, 1 ); break;
            case GL_SAMPLER_CUBE:       SET_LNDESC( LN_SVC_SAMPLER, LN_SVT_SAMPLERCUBE, 1, 1 ); break;
            
#if !defined(LNOTE_GLES)
            case GL_FLOAT_MAT2x3:   SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 2, 3 ); break;
            case GL_FLOAT_MAT2x4:   SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 2, 4 ); break;
            case GL_FLOAT_MAT3x2:   SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 3, 2 ); break;
            case GL_FLOAT_MAT3x4:   SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 3, 4 ); break;
            case GL_FLOAT_MAT4x2:   SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 4, 2 ); break;
            case GL_FLOAT_MAT4x3:   SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 4, 3 ); break;

            //case GL_FLOAT:      SET_LNDESC( LN_SVC_SCALAR, LN_SVT_FLOAT, 1, 1 ); break;
            //case GL_FLOAT_VEC2: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 2 ); break;
            //case GL_FLOAT_VEC3: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 3 ); break;
            //case GL_FLOAT_VEC4: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 4 ); break;

            case GL_SAMPLER_1D:         SET_LNDESC( LN_SVC_SAMPLER, LN_SVT_SAMPLER1D, 1, 1 ); break;
            case GL_SAMPLER_3D:         SET_LNDESC( LN_SVC_SAMPLER, LN_SVT_SAMPLER3D, 1, 1 ); break;
            
            case GL_SAMPLER_1D_SHADOW:  SET_LNDESC( LN_SVC_SAMPLER, LN_SVT_UNKNOWN, 1, 1 ); break;
            case GL_SAMPLER_2D_SHADOW:  SET_LNDESC( LN_SVC_SAMPLER, LN_SVT_UNKNOWN, 1, 1 ); break;
#endif
        }
    }


    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    ShaderVariable::ShaderVariable( Shader* shader_, int var_index_ )
        : mShader       ( shader_ )
        , mVarIndex     ( var_index_ )
        , mLocation     ( -1 )
        , mSamplerState ( NULL )
    {
        
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    ShaderVariable::~ShaderVariable()
    {
        
        if ( mSamplerState )
        {
            LN_SAFE_RELEASE( mSamplerState->Texture );
            SAFE_DELETE( mSamplerState );
        }
        for ( u32 i = 0; i < mAnnotationArray.size(); ++i )
        {
            SAFE_DELETE( mAnnotationArray[ i ] );
        }
    }

    //---------------------------------------------------------------------
    // ● ブール値の取得
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::getBool( bool* b_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● ブール値の設定
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::setBool( bool b_ )
    {
        if (mVariableBuffer.Type == TYPE_BOOL)
        {
            mVariableBuffer.Bool = b_;
        }
        /*
        glUseProgram( mShader->getGLProgram() );
        LN_CHECK_GLERROR();

        glUniform1i( mLocation, b_ );
        LN_CHECK_GLERROR();
        */
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 整数値の取得
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::getInt( int* value_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 整数値の設定
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::setInt( int value_ )
    {
        if (mVariableBuffer.Type == TYPE_INT)
        {
            mVariableBuffer.Int = value_;
        }
        /*
        glUseProgram( mShader->getGLProgram() );
        LN_CHECK_GLERROR();

        glUniform1i( mLocation, value_ );
        LN_CHECK_GLERROR();
        */
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 実数値の取得
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::getFloat( lnfloat* value_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 実数値の設定
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::setFloat( lnfloat value_ )
    {
        if (mVariableBuffer.Type == TYPE_FLOAT)
        {
            mVariableBuffer.Float = value_;
        }
        /*
        glUseProgram( mShader->getGLProgram() );
        LN_CHECK_GLERROR();

        glUniform1f( mLocation, value_ );
        LN_CHECK_GLERROR();
        */
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● ベクトルの取得
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::getVector( LVector4* vec_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● ベクトルの設定
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::setVector( const LVector4& vec_ )
    {
        if (mVariableBuffer.Type == TYPE_VECTOR)
        {
            *mVariableBuffer.Vector = vec_;
        }

        /*
        glUseProgram( mShader->getGLProgram() );
        LN_CHECK_GLERROR();

        switch ( mVariableTypeDesc.Columns )
        {
            case 2:
                glUniform2f( mLocation, vec_.x, vec_.y );
                LN_CHECK_GLERROR();
                break;
            case 3:
                glUniform3f( mLocation, vec_.x, vec_.y, vec_.z );
                LN_CHECK_GLERROR();
                break;
            case 4:
                glUniform4f( mLocation, vec_.x, vec_.y, vec_.z, vec_.w );
                LN_CHECK_GLERROR();
                break;
        }
        */
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● ベクトル配列の取得
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::getVectorArray( LVector4* vec_, u32 count_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }
    
    //---------------------------------------------------------------------
    // ● ベクトル配列の設定
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::setVectorArray( const LVector4* vec_, u32 count_ )
    {
        if (mVariableBuffer.Type == TYPE_VECTORARRAY)
        {
            memcpy(
                mVariableBuffer.VectorArray.Vectors,
                vec_,
                sizeof(LVector4) * (LMath::min(mVariableBuffer.VectorArray.Count, count_)));
        }

        /*
        glUseProgram( mShader->getGLProgram() );
        LN_CHECK_GLERROR();

        glUniform4fv( mLocation, count_, (const GLfloat*)vec_ );
        LN_CHECK_GLERROR();
        */
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 行列の取得
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::getMatrix( LMatrix* matrix_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 行列の設定
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::setMatrix( const LMatrix& matrix_ )
    {
        if (mVariableBuffer.Type == TYPE_MATRIX)
        {
            LMatrix mat;
            LMatrix::transpose( &mat, matrix_ );
            *mVariableBuffer.Matrix = mat;
        }

        /*
        LMatrix mat;
        LMatrix::transpose( &mat, matrix_ );

        glUseProgram( mShader->getGLProgram() );
        LN_CHECK_GLERROR();

        glUniformMatrix4fv( mLocation, 1, GL_FALSE, (const GLfloat*)&mat );
        LN_CHECK_GLERROR();
        */
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 行列の配列の取得
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::getMatrixArray( LMatrix* matrices_, u32 count_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 行列の配列の設定
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::setMatrixArray( const LMatrix* matrices_, u32 count_ )
    {
        if (mVariableBuffer.Type == TYPE_MATRIXARRAY)
        {
            memcpy(
                mVariableBuffer.VectorArray.Vectors,
                matrices_,
                sizeof(LVector4) * (LMath::min(mVariableBuffer.MatrixArray.Count, count_)));
        }

        /*
        glUseProgram( mShader->getGLProgram() );
        LN_CHECK_GLERROR();

        glUniformMatrix4fv( mLocation, count_, GL_TRUE, (const GLfloat*)&matrices_ );
        LN_CHECK_GLERROR();
        */
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● テクスチャの設定
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::setTexture( ITexture* texture_ )
    {
        if ( mSamplerState )
        {
            TextureBase* t = dynamic_cast< TextureBase* >( texture_ );
            LN_SAFE_ADDREF( t );
            LN_SAFE_RELEASE( mSamplerState->Texture );
            mSamplerState->Texture = t;
        }
        return LN_OK;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::setTexture( ITexture* texture_, const LSamplerState& state_ )
    {
        return setTexture( texture_ );
    }

    //---------------------------------------------------------------------
    // ● 文字列の取得
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::getString( const char** str_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 文字列の設定
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::setString( const char* str_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : setting the string value is not supported." );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 変数名の取得
    //---------------------------------------------------------------------
    const lnChar* ShaderVariable::getName() const
    {
        return mName.c_str();
    }

    //---------------------------------------------------------------------
    // ● セマンティクス名の取得
    //---------------------------------------------------------------------
    const lnChar* ShaderVariable::getSemanticName() const
    {
        return mSemanticName.c_str();
    }

    //---------------------------------------------------------------------
    // ● 型情報の取得
    //---------------------------------------------------------------------
    const LNShaderVariableTypeDesc& ShaderVariable::getTypeDesc() const
    {
        return mVariableTypeDesc;
    }

    //---------------------------------------------------------------------
    // ● インデックスによってアノテーションを取得する
    //---------------------------------------------------------------------
    IShaderVariable* ShaderVariable::getAnnotationByIndex( u32 index_ )
    {
        if ( index_ >= mAnnotationArray.size() ) { return NULL; }
        return mAnnotationArray[ index_ ];
    }

    //---------------------------------------------------------------------
    // ● 名前によってアノテーションを取得する
    //---------------------------------------------------------------------
    IShaderVariable* ShaderVariable::getAnnotationByName( const char* name_ )
    {
        ShaderAnnotationMap::iterator it = mAnnotationMap.find( name_ );
        if ( it != mAnnotationMap.end() )
        {
            return it->second;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    // 
    //---------------------------------------------------------------------
    void ShaderVariable::onCreateGLResource()
    {
        GLsizei name_len = 0;
        GLsizei var_size = 0;   // 配列サイズっぽい
        GLenum  var_type = 0;
        GLchar  name[256] = { 0 };
        LNGL::glGetActiveUniform( mShader->getGLProgram(), mVarIndex, 256, &name_len, &var_size, &var_type, name );
        //LN_CHECK_GLERROR();

        // 変数情報変換
        convertVariableTypeGLToLN( var_type, var_size, &mVariableTypeDesc );

        // 名前記憶
        mName = name;

        // サンプラの場合は割り当てるべきテクスチャユニット番号を取得する
        if ( mVariableTypeDesc.Class == LN_SVC_SAMPLER && !mSamplerState )
        {
            mSamplerState = LN_NEW GLSamplerState;
            mSamplerState->TextureUnitIndex = mShader->getTextureUnitIncr();
            mSamplerState->Texture   = NULL;
            mSamplerState->MagFilter = GL_NEAREST;//GL_LINEAR;
            mSamplerState->MinFilter = GL_NEAREST;//GL_LINEAR;
            mSamplerState->AddressU  = GL_REPEAT;//GL_CLAMP_TO_EDGE;
            mSamplerState->AddressV  = GL_REPEAT;//GL_CLAMP_TO_EDGE;
        }

        mLocation = LNGL::glGetUniformLocation( mShader->getGLProgram(), name );
        //LN_CHECK_GLERROR();

        // 値の一時格納
        memset(&mVariableBuffer, 0, sizeof(mVariableBuffer));
        switch (mVariableTypeDesc.Type)
        {
            case LN_SVT_BOOL: mVariableBuffer.Type = TYPE_BOOL; break;
            case LN_SVT_INT: mVariableBuffer.Type = TYPE_INT; break;
            case LN_SVT_FLOAT: mVariableBuffer.Type = TYPE_FLOAT; break;
        }
        switch (mVariableTypeDesc.Class)
        {
            case LN_SVC_VECTOR:
                // Matrix
                if (mVariableTypeDesc.Elements <= 1)
                {
                    mVariableBuffer.Type = TYPE_VECTOR;
                    mVariableBuffer.Vector = LN_NEW LVector4();
                }
                // MatrixArray
                else
                {
                    mVariableBuffer.Type = TYPE_VECTORARRAY;
                    mVariableBuffer.VectorArray.Count = mVariableTypeDesc.Elements;
                    mVariableBuffer.VectorArray.Vectors = LN_NEW LVector4[mVariableBuffer.VectorArray.Count];
                }
                break;
            case LN_SVC_MATRIX:
                // Matrix
                if (mVariableTypeDesc.Elements <= 1)
                {
                    mVariableBuffer.Type = TYPE_MATRIX;
                    mVariableBuffer.Matrix = LN_NEW LMatrix();
                }
                // MatrixArray
                else
                {
                    mVariableBuffer.Type = TYPE_MATRIXARRAY;
                    mVariableBuffer.MatrixArray.Count = mVariableTypeDesc.Elements;
                    mVariableBuffer.MatrixArray.Matrices = LN_NEW LMatrix[mVariableBuffer.MatrixArray.Count];
                }
                break;
        }
    }

    //---------------------------------------------------------------------
    // 
    //---------------------------------------------------------------------
    void ShaderVariable::onDisposeGLResource()
    {
        // onCreateGLResource() で確保したのでここで開放…
        switch (mVariableBuffer.Type)
        {
            case TYPE_VECTOR:
                SAFE_DELETE(mVariableBuffer.Vector)
                break;
            case TYPE_VECTORARRAY:
                SAFE_DELETE_ARRAY(mVariableBuffer.VectorArray.Vectors);
                break;
            case TYPE_MATRIX:
                SAFE_DELETE(mVariableBuffer.Matrix)
                break;
            case TYPE_MATRIXARRAY:
                SAFE_DELETE_ARRAY(mVariableBuffer.MatrixArray.Matrices);
                break;
        }
    }

    //---------------------------------------------------------------------
    // 
    //---------------------------------------------------------------------
    void ShaderVariable::onCommit()
    {
        if ( mSamplerState && mSamplerState->Texture )
        {
            LNGL::glActiveTexture( GL_TEXTURE0 + mSamplerState->TextureUnitIndex );
            //LN_CHECK_GLERROR();

            LNGL::glBindTexture( GL_TEXTURE_2D, mSamplerState->Texture->getGLTexture() );
            //LN_CHECK_GLERROR();

            LNGL::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mSamplerState->MinFilter );
            //LN_CHECK_GLERROR();
            LNGL::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mSamplerState->MagFilter );
            //LN_CHECK_GLERROR();
            LNGL::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mSamplerState->AddressU );
            //LN_CHECK_GLERROR();
            LNGL::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mSamplerState->AddressV );
            //LN_CHECK_GLERROR();

            LNGL::glUniform1i( mLocation, mSamplerState->TextureUnitIndex );
            //LN_CHECK_GLERROR();
           
            // テクスチャ変換行列の設定
            //glMatrixMode(GL_TEXTURE);
            //glLoadIdentity();
            //glTranslated(-0.5, -0.5, 0.0);


            
        }

        LNGL::glUseProgram( mShader->getGLProgram() );
        //LN_CHECK_GLERROR();

        switch (mVariableBuffer.Type)
        {
            case TYPE_BOOL:
            {
                LNGL::glUniform1i( mLocation, mVariableBuffer.Bool );
                //LN_CHECK_GLERROR();
                break;
            }
            case TYPE_INT:
            {
                LNGL::glUniform1i( mLocation, mVariableBuffer.Int );
                //LN_CHECK_GLERROR();
                break;
            }
            case TYPE_FLOAT:
            {
                LNGL::glUniform1f( mLocation, mVariableBuffer.Float );
                //LN_CHECK_GLERROR();
                break;
            }
            case TYPE_VECTOR:
            {
                LVector4* vec = mVariableBuffer.Vector;
                switch ( mVariableTypeDesc.Columns )
                {
                    case 2:
                        LNGL::glUniform2f( mLocation, vec->x, vec->y );
                        //LN_CHECK_GLERROR();
                        break;
                    case 3:
                        LNGL::glUniform3f( mLocation, vec->x, vec->y, vec->z );
                        //LN_CHECK_GLERROR();
                        break;
                    case 4:
                        LNGL::glUniform4f( mLocation, vec->x, vec->y, vec->z, vec->w );
                        //LN_CHECK_GLERROR();
                        break;
                }
                break;
            }
            case TYPE_VECTORARRAY:
            {
                 LNGL::glUniform4fv(
                    mLocation,
                    mVariableBuffer.VectorArray.Count,
                    (const GLfloat*)mVariableBuffer.VectorArray.Vectors );
                //LN_CHECK_GLERROR();
                break;
            }
            case TYPE_MATRIX:
            {
                 LNGL::glUniformMatrix4fv( mLocation, 1, GL_FALSE, (const GLfloat*)mVariableBuffer.Matrix );
                //LN_CHECK_GLERROR();
                break;
            }
            case TYPE_MATRIXARRAY:
            {
                 LNGL::glUniformMatrix4fv(
                    mLocation,
                    mVariableBuffer.MatrixArray.Count,
                    GL_TRUE,
                    (const GLfloat*)&mVariableBuffer.MatrixArray.Matrices );
                //LN_CHECK_GLERROR();
                break;
            }
        }
    }

    //---------------------------------------------------------------------
    // 
    //---------------------------------------------------------------------
    void ShaderVariable::addAnnotationBool( const char* name_, bool v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderVariable::addAnnotationInt( const char* name_, int v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderVariable::addAnnotationFloat( const char* name_, float v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderVariable::addAnnotationVector( const char* name_, float x_, float y_, float z_, float w_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, x_, y_, z_, w_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderVariable::addAnnotationString( const char* name_, const char* v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    

//=============================================================================
// ■ ShaderAnnotation クラス
//=============================================================================

    void resetLNVariableTypeDesc( LNShaderVariableTypeDesc* desc_ )
    {
        desc_->Class = LN_SVC_SCALAR;
        desc_->Type = LN_SVT_BOOL;
        desc_->Rows = 1;
        desc_->Columns = 1;
        desc_->Elements = 1;
        desc_->Shared = false;
    }

    //---------------------------------------------------------------------
    // 
    //---------------------------------------------------------------------
    ShaderAnnotation::ShaderAnnotation( Shader* shader_, const char* name_, bool v_ )
        : mShader   ( shader_ )
        , mName     ( name_ )
    {
        resetLNVariableTypeDesc( &mVariableTypeDesc );
        mValue.Bool = v_;
    }
    ShaderAnnotation::ShaderAnnotation( Shader* shader_, const char* name_, int v_ )
        : mShader   ( shader_ )
        , mName     ( name_ )
    {
        resetLNVariableTypeDesc( &mVariableTypeDesc );
        mVariableTypeDesc.Type = LN_SVT_INT;
        mValue.Int = v_;
    }
    ShaderAnnotation::ShaderAnnotation( Shader* shader_, const char* name_, float v_ )
        : mShader   ( shader_ )
        , mName     ( name_ )
    {
        resetLNVariableTypeDesc( &mVariableTypeDesc );
        mVariableTypeDesc.Type = LN_SVT_FLOAT;
        mValue.Float = v_;
    }
    ShaderAnnotation::ShaderAnnotation( Shader* shader_, const char* name_, float x_, float y_, float z_, float w_ )
        : mShader   ( shader_ )
        , mName     ( name_ )
    {
        resetLNVariableTypeDesc( &mVariableTypeDesc );
        mVariableTypeDesc.Class = LN_SVC_VECTOR;
        mVariableTypeDesc.Type = LN_SVT_FLOAT;
        mVariableTypeDesc.Columns = 4;
        mValue.Vector.x = x_;
        mValue.Vector.y = y_;
        mValue.Vector.z = z_;
        mValue.Vector.w = w_;
    }
    ShaderAnnotation::ShaderAnnotation( Shader* shader_, const char* name_, const char* v_ )
        : mShader   ( shader_ )
        , mName     ( name_ )
    {
        resetLNVariableTypeDesc( &mVariableTypeDesc );
        mVariableTypeDesc.Type = LN_SVT_STRING;
        mStringValue = v_;
    }


//=============================================================================
// ■ ShaderTechnique クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    ShaderTechnique::ShaderTechnique( Shader* shader_ )
        : mShader       ( shader_ )
        , mShaderPass   ( NULL )
        , mName         ( _T( "GLTechnique" ) )
    {
        mShaderPass = LN_NEW ShaderPass( mShader );
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    ShaderTechnique::~ShaderTechnique()
    {
        for ( u32 i = 0; i < mAnnotationArray.size(); ++i )
        {
            SAFE_DELETE( mAnnotationArray[ i ] );
        }

        LN_SAFE_RELEASE( mShaderPass );
    }

    //---------------------------------------------------------------------
    // ● 名前によってアノテーションを取得する
    //---------------------------------------------------------------------
    IShaderVariable* ShaderTechnique::getAnnotationByName( const char* name_ )
    {
        ShaderAnnotationMap::iterator it = mAnnotationMap.find( name_ );
        if ( it != mAnnotationMap.end() )
        {
            return it->second;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    // ● インデックスによってアノテーションを取得する
    //---------------------------------------------------------------------
    IShaderVariable* ShaderTechnique::getAnnotationByIndex( u32 index_ )
    {
        if ( index_ >= mAnnotationArray.size() ) { return NULL; }
        return mAnnotationArray[ index_ ];
    }

    //---------------------------------------------------------------------
    // ● 名前によってパスを取得する
    //---------------------------------------------------------------------
    IShaderPass* ShaderTechnique::getPassByName( const lnChar* name_ )
    {
        return mShaderPass;
    }

    //---------------------------------------------------------------------
    // ● インデックスによってパスを取得する
    //---------------------------------------------------------------------
    IShaderPass* ShaderTechnique::getPassByIndex( u32 index_ )
    {
        if ( index_ == 0 )
        {
            return mShaderPass;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ShaderTechnique::onCreateGLResource()
    {
        mShaderPass->onCreateGLResource();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ShaderTechnique::onDisposeGLResource()
    {
        mShaderPass->onDisposeGLResource();
    }

    //---------------------------------------------------------------------
    // 
    //---------------------------------------------------------------------
    void ShaderTechnique::addAnnotationBool( const char* name_, bool v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderTechnique::addAnnotationInt( const char* name_, int v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderTechnique::addAnnotationFloat( const char* name_, float v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderTechnique::addAnnotationVector( const char* name_, float x_, float y_, float z_, float w_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, x_, y_, z_, w_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderTechnique::addAnnotationString( const char* name_, const char* v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

//=============================================================================
// ■ ShaderPass クラス
//=============================================================================

    static const char* gGLSL_LNAttributeName[LN_VERTEXDECLUSAGE_MAX] =
    {
        "",                 // LN_VERTEXDECLUSAGE_UNKNOWN = 0,
        "lnPosition",       // LN_VERTEXDECLUSAGE_POSITION,        ///< 頂点位置
        "lnBlendWeight",    // LN_VERTEXDECLUSAGE_BLENDWEIGHT,     ///< ブレンドウェイト値
        "lnBlendIndices",   // LN_VERTEXDECLUSAGE_BLENDINDICES,    ///< ブレンドインデックス値
        "lnNormal",         // LN_VERTEXDECLUSAGE_NORMAL,          ///< 法線
        "lnTexCoord0",      // LN_VERTEXDECLUSAGE_TEXCOORD0,       ///< UV
        "lnTexCoord1",      // LN_VERTEXDECLUSAGE_TEXCOORD1,       ///< 追加UV1
        "lnTexCoord2",      // LN_VERTEXDECLUSAGE_TEXCOORD2,       ///< 追加UV2
        "lnTexCoord3",      // LN_VERTEXDECLUSAGE_TEXCOORD3,       ///< 追加UV3
        "lnTexCoord4",      // LN_VERTEXDECLUSAGE_TEXCOORD4,       ///< 追加UV4
        "lnTexCoord5",      // LN_VERTEXDECLUSAGE_TEXCOORD5,       ///< Sdef - C値
        "lnTexCoord6",      // LN_VERTEXDECLUSAGE_TEXCOORD6,       ///< Sdef - R0値
        "lnTexCoord7",      // LN_VERTEXDECLUSAGE_TEXCOORD7,       ///< Sdef - R1値
        "lnTexCoord8",      // LN_VERTEXDECLUSAGE_TEXCOORD8,       ///< エッジウェイト
        "lnPSize15",        // LN_VERTEXDECLUSAGE_PSIZE15,         ///< 頂点インデックス値
        "lnColor",          // LN_VERTEXDECLUSAGE_COLOR,
    };

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    ShaderPass::ShaderPass( Shader* shader_ )
        : mShader   ( shader_ )
        , mBegan    ( false )
    {
        
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    ShaderPass::~ShaderPass()
    {
        for ( u32 i = 0; i < mAnnotationArray.size(); ++i )
        {
            SAFE_DELETE( mAnnotationArray[ i ] );
        }
    }

    //---------------------------------------------------------------------
    // ● 名前によってアノテーションを取得する
    //---------------------------------------------------------------------
    IShaderVariable* ShaderPass::getAnnotationByName( const lnChar* name_ )
    {
        ShaderAnnotationMap::iterator it = mAnnotationMap.find( name_ );
        if ( it != mAnnotationMap.end() )
        {
            return it->second;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    // ● インデックスによってアノテーションを取得する
    //---------------------------------------------------------------------
    IShaderVariable* ShaderPass::getAnnotationByIndex( u32 index_ )
    {
        if ( index_ >= mAnnotationArray.size() ) { return NULL; }
        return mAnnotationArray[ index_ ];
    }

    //---------------------------------------------------------------------
    // ● パスの適用を開始する
    //---------------------------------------------------------------------
    LNRESULT ShaderPass::begin()
    {
        if ( !mBegan )
        {
            mBegan = true;
            mShader->getGraphicsDevice()->setCurrentShaderrPass( this );
            commit();
        }
        
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● パスの適用を終了する
    //---------------------------------------------------------------------
    LNRESULT ShaderPass::end()
    {
        if ( mBegan )
        {
            LNGL::glUseProgram( NULL );
            //LN_CHECK_GLERROR();
        }
        mBegan = false;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● パスの実行中に変化したステートをデバイスに送信する
    //---------------------------------------------------------------------
    void ShaderPass::commit()
    {
        if ( mBegan )
        {
            LNGL::glUseProgram( NULL );
            //LN_CHECK_GLERROR();

            LNGL::glUseProgram( mShader->getGLProgram() );
            //LN_CHECK_GLERROR();

            mShader->onCommit();
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ShaderPass::onCreateGLResource()
    {
        GLint params;
        LNGL::glGetProgramiv( mShader->getGLProgram(), GL_ACTIVE_ATTRIBUTES, &params );
        //LN_CHECK_GLERROR();

        // 初期化
        for ( int i = 0; i < LN_VERTEXDECLUSAGE_MAX; ++i )
        {
            mUsageEntryLocation[i] = -1;
        }

        // 総当たりだけどまぁ今のところ数少ないのでこれで・・・
        for ( int i = 0; i < params; ++i )
        {
            GLsizei name_len = 0;
            GLsizei var_size = 0;   // 配列サイズっぽい
            GLenum  var_type = 0;
            GLchar  name[256] = { 0 };
            LNGL::glGetActiveAttrib( mShader->getGLProgram(), i, 256, &name_len, &var_size, &var_type, name );
            //LN_CHECK_GLERROR();

            for ( int ln_attr_i = 0; ln_attr_i < LN_VERTEXDECLUSAGE_MAX; ++ln_attr_i )
            {
                if ( Base::StringUtil::strcmpI( name, gGLSL_LNAttributeName[ln_attr_i] ) )
                {
                    //printf( "%s: idx=%d loc:%d\n", name, i, glGetAttribLocation( shader_->getGLProgram(), name ) );
                    mUsageEntryLocation[ln_attr_i] = LNGL::glGetAttribLocation( mShader->getGLProgram(), name );//i;
                    //LN_CHECK_GLERROR();
                    break;
                }
            }
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ShaderPass::onDisposeGLResource()
    {
    }

    //---------------------------------------------------------------------
    // 
    //---------------------------------------------------------------------
    void ShaderPass::addAnnotationBool( const char* name_, bool v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderPass::addAnnotationInt( const char* name_, int v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderPass::addAnnotationFloat( const char* name_, float v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderPass::addAnnotationVector( const char* name_, float x_, float y_, float z_, float w_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, x_, y_, z_, w_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderPass::addAnnotationString( const char* name_, const char* v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

#if 0

    /*
printf( "g]%d\n", glGetError() );

mShader->onCommit();
printf( "%d\n", glGetError() );
// テクスチャ最大数
	GLint num_tex_units;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &num_tex_units);
    std::cout << "Max texture units = " << num_tex_units << std::endl;
	glActiveTexture(GL_TEXTURE0+num_tex_units);
	glBindTexture(GL_TEXTURE_2D, 0);

printf( "%d\n", glGetError() );
            glEnable(GL_TEXTURE_2D);printf( "%d\n", glGetError() );
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

            
            glUniform1i(glGetUniformLocation(mShader->getGLProgram(), "gMaterialSampler"), 0);
printf( "g]%d\n", glGetError() );
*/
           D3DTADDRESS_WRAP        GL_REPEAT
整数区切りごとにテクスチャを繰り返します

 D3DTADDRESS_CLAMP      GL_CLAMP_TO_EDGE
 境界引き伸ばし

 D3DTADDRESS_BORDER     GL_CLAMP_TO_BORDER
 範囲外塗りつぶし

            );
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, );
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE

                GL_CLAMP_TO_BORDER
GL_NEAREST
GL_LINEAR

        GL_REPEAT　→　画像が繰り返されているとするとして画像の色を拾う
GL_CLAMP　→　0～１の範囲に固定
　→　画像の端のピクセルの色を使う。
GL_CLAMP_TO_BORDER　→　境界の色を設定してそれを使う。(OpenGL 1.3以上)
GL_MIRRORED_REPEAT　→　繰り返すとき画像を反転させる (OpenGL 1.4 以上)
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace OpenGL
} // namespace Graphics
} // namespace Core

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================