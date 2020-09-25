//=============================================================================
//�y GLShader �z
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
// �� Shader �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
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
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    Shader::~Shader()
    {
        if (mManager)
        {
            mManager->removeShader( this );
        }

        onDisposeGLResource();

        // ShaderTechnique ���
        LN_SAFE_RELEASE( mShaderTechnique );

        // ShaderVariable ���
        for ( u32 i = 0; i < mShaderVariableArray.size(); ++i )
        {
            SAFE_DELETE( mShaderVariableArray[ i ] );
        }
        mShaderVariableArray.clear();


        LN_SAFE_RELEASE( mManager );
    }

    //---------------------------------------------------------------------
    // �� ������
    //---------------------------------------------------------------------
    void Shader::initialize( const lnChar* filename_ )
    {
        mFilename = filename_;

        // ������ ShaderVariable �������ׂč\�z�ł��Ă��Ȃ��ƁA
        // �Q�[���X���b�h�����珉���l�̐ݒ�Ȃǂ��ł��Ȃ��Ȃ�B
        // �Ȃ̂ŁA�`��X���b�h�̑ҋ@�҂�����̍쐬���s���B
        // �Ȃ񂩂����p�t�H�[�}���X�ቺ�͂�ނ𓾂Ȃ��Ƃ������ƂŁc�B

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

        // �X�N���v�g���s
        if ( !mScript.empty() )
        {
            mManager->doScript( mScript.c_str(), this );
        }
    }

    //---------------------------------------------------------------------
    // �� �u�[���l�̐ݒ�
    //---------------------------------------------------------------------
	LNRESULT Shader::setBool( const lnChar* name_, bool b_ )
    {
        return getVariableByName( name_ )->setBool( b_ );
    }

    //---------------------------------------------------------------------
    // �� �����l�̐ݒ�
    //---------------------------------------------------------------------
	LNRESULT Shader::setInt( const lnChar* name_, int value_ )
    {
        return getVariableByName( name_ )->setInt( value_ );
    }

    //---------------------------------------------------------------------
    // �� �����l�̐ݒ�
    //---------------------------------------------------------------------
	LNRESULT Shader::setFloat( const lnChar* name_, lnfloat value_ )
    {
        return getVariableByName( name_ )->setFloat( value_ );
    }

    //---------------------------------------------------------------------
    // �� �x�N�g���̐ݒ�
    //---------------------------------------------------------------------
	LNRESULT Shader::setVector( const lnChar* name_, const LVector4& vec_ )
    { 
        return getVariableByName( name_ )->setVector( vec_ );
    }

    //---------------------------------------------------------------------
    // �� �x�N�g���z��̐ݒ�
    //---------------------------------------------------------------------
	LNRESULT Shader::setVectorArray( const lnChar* name_, const LVector4* vec_, u32 count_ )
    {
        return getVariableByName( name_ )->setVectorArray( vec_, count_ );
    }

    //---------------------------------------------------------------------
    // �� �s��̐ݒ�
    //---------------------------------------------------------------------
	LNRESULT Shader::setMatrix( const lnChar* name_, const LMatrix& matrix_ )
    {
        return getVariableByName( name_ )->setMatrix( matrix_ );
    }

    //---------------------------------------------------------------------
    // �� �s��̔z��̐ݒ�
    //---------------------------------------------------------------------
	LNRESULT Shader::setMatrixArray( const lnChar* name_, const LMatrix* matrices_, u32 count_ )
    {
        return getVariableByName( name_ )->setMatrixArray( matrices_, count_ );
    }
   
    //---------------------------------------------------------------------
    // �� �e�N�X�`���̐ݒ�
    //---------------------------------------------------------------------
	LNRESULT Shader::setTexture( const lnChar* name_, ITexture* texture_ )
    {
        return getVariableByName( name_ )->setTexture( texture_ );
    }

    //---------------------------------------------------------------------
    // �� ���O�ɂ���ĕϐ����擾����
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
    // �� �C���f�b�N�X�ɂ���ĕϐ����擾����
    //---------------------------------------------------------------------
    IShaderVariable* Shader::getVariableByIndex( u32 index_ )
    {
        if ( index_ >= mShaderVariableArray.size() ) { return NULL; }
        return mShaderVariableArray[ index_ ];
    }

    //// �Z�}���e�B�N�X�ɂ���ĕϐ����擾����
    //IShaderVariable* Shader::getVariableBySemantic( const char* name_ )
    //{
    //    return NULL;
    //}
    
    //---------------------------------------------------------------------
    // �� ���O�ɂ���ăe�N�j�b�N���擾����
    //---------------------------------------------------------------------
    IShaderTechnique* Shader::getTechniqueByName( const lnChar* name_ )
    {
        return mShaderTechnique;
    }

    //---------------------------------------------------------------------
    // �� �C���f�b�N�X�ɂ���ăe�N�j�b�N���擾����
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

        // onCommit() �͕ʓr begin() ������Ă΂��̂ŁA�����ł͕K�v�Ȃ�����
        //onCommit()
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Shader::onDisposeGLResource()
    {
        // �q�v�f
        ln_foreach(ShaderVariable* v, mShaderVariableArray)
        {
            v->onDisposeGLResource();
        }
        if (mShaderTechnique)
        {
            mShaderTechnique->onDisposeGLResource();
        }

        // �V�F�[�_�I�u�W�F�N�g�̍폜
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

        // �v���O�����̍폜
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
        // �V�F�[�_�v���O�����̃r���h
        _buildFromFile();

        //---------------------------------------------------------
        // ���ׂĂ̕ϐ����

        // uniform �ϐ��̐��𒲂ׂāA���̐��� ShaderVariable �쐬
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
        // �V�F�[�_���

        mDesc.ParameterNum = params;
        mDesc.TechniqueNum = 1;

        //---------------------------------------------------------
        // �e�N�j�b�N�쐬

        mShaderTechnique = LN_NEW ShaderTechnique( this );


        //---------------------------------------------------------
        // GL���\�[�X�m��
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
        // �V�F�[�_�v���O�����̃r���h
        _buildFromFile();

        //---------------------------------------------------------
        // GL���\�[�X�m��
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

        // mTempBuffer �ɑS���ǂݍ���
        lnU32 size = file->getSize();
        char* buf = (char*)mManager->getTempBuffer().lock( size + 1 );
        file->read( buf, size );
        buf[ size ] = '\0';

        _build( buf );

        // TODO:lock�~�X�B
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
        mCompileResult = LN_SHADERRESULT_ERROR; // �G���[�ɂ��Ă���
        mCompileErrors.clear();

        //---------------------------------------------------------
        // �R�[�h����
#if 0
#define VERTEX_SHADER_TAG       "VERTEX_SHADER:"
#define VERTEX_SHADER_TAG_LEN   ( 14 )
#define FRAGMENT_SHADER_TAG     "FRAGMENT_SHADER:"
#define FRAGMENT_SHADER_TAG_LEN ( 16 )
#define METADATA_TAG            "METADATA:"
#define METADATA_TAG_LEN        ( 9 )

        std::string str( code_ );
        
        // "VERTEX_SHADER:" ��T��
	    std::string::size_type vs_index = str.find( VERTEX_SHADER_TAG );
        if ( vs_index == std::string::npos )
        {
            mCompileErrors = "not found "VERTEX_SHADER_TAG;
            return;
        }
	    
        // "FRAGMENT_SHADER:" ��T��
	    std::string::size_type fs_index = str.find( FRAGMENT_SHADER_TAG );
        if ( fs_index == std::string::npos )
        {
            mCompileErrors = "not found "FRAGMENT_SHADER_TAG;
            return;
        }
	    
        // ���_�V�F�[�_�R�[�h
        std::string vs_code( str, vs_index + VERTEX_SHADER_TAG_LEN, fs_index - vs_index - VERTEX_SHADER_TAG_LEN );
        
        // "METADATA:" ��T��
	    std::string::size_type mt_index = str.find( METADATA_TAG );
        if ( fs_index == std::string::npos )
        {
            mCompileErrors = "not found "METADATA_TAG;
            return;
        }

        // �t���O�����g�V�F�[�_�R�[�h
        std::string fs_code( str, fs_index + FRAGMENT_SHADER_TAG_LEN, mt_index - fs_index - FRAGMENT_SHADER_TAG_LEN );

        // ���^�f�[�^�R�[�h
        mScript = std::string( str.c_str() + (mt_index + METADATA_TAG_LEN) );
#endif

        // �\�[�X����
        LNFXShaderSource source;
        source.initialize(code_, LNFXShaderSource::SHADER_GLSL);

        // ���^�f�[�^�R�[�h
        mScript = source.getGLSLMetaDataSource();





        // ���_�V�F�[�_
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

        // �t���O�����g�V�F�[�_
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
        // �v���O�����̃r���h

        mVertexShader   = LNGL::glCreateShader( GL_VERTEX_SHADER );
        //LN_CHECK_GLERROR();

	    mFragmentShader = LNGL::glCreateShader( GL_FRAGMENT_SHADER );
        //LN_CHECK_GLERROR();

        //_p(vs_code.c_str());
        //_p(fs_code.c_str());

        // ���_�V�F�[�_
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

        // �t���O�����g�V�F�[�_
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
        
        // �v���O�����I�u�W�F�N�g�̍쐬
        mProgram = LNGL::glCreateProgram();
        //LN_CHECK_GLERROR();

        // �V�F�[�_�I�u�W�F�N�g�̃V�F�[�_�v���O�����ւ̓o�^
        LNGL::glAttachShader( mProgram, mVertexShader );
        //LN_CHECK_GLERROR();

        LNGL::glAttachShader( mProgram, mFragmentShader );
        //LN_CHECK_GLERROR();

        // �V�F�[�_�I�u�W�F�N�g�̍폜
        //glDeleteShader( mVertexShader );
        //glDeleteShader( mFragmentShader );

        // �V�F�[�_�v���O�����̃����N
        GLint linked;
        LNGL::glLinkProgram( mProgram );
        //LN_CHECK_GLERROR();

        LNGL::glGetProgramiv( mProgram, GL_LINK_STATUS, &linked );
        //LN_CHECK_GLERROR();

        // ���O�����邩�`�F�b�N
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
        // ���O������ꍇ�͂Ƃ肠�����x���ɂ��Ă���

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
        // �V�F�[�_�I�u�W�F�N�g�Ƀ\�[�X�v���O�������Z�b�g
        LNGL::glShaderSource( shader_, code_count_, (const GLchar **)code_, size_ );
        //LN_CHECK_GLERROR();
      
        // �V�F�[�_�̃R���p�C��
        GLint compiled;
        LNGL::glCompileShader( shader_ );
        //LN_CHECK_GLERROR();

        LNGL::glGetShaderiv( shader_, GL_COMPILE_STATUS, &compiled );
        //LN_CHECK_GLERROR();
        
        // ���O�����邩�`�F�b�N (���O�̒����́A�Ō��NULL�������܂�)
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
// �� ShaderVariable �N���X
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
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    ShaderVariable::ShaderVariable( Shader* shader_, int var_index_ )
        : mShader       ( shader_ )
        , mVarIndex     ( var_index_ )
        , mLocation     ( -1 )
        , mSamplerState ( NULL )
    {
        
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
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
    // �� �u�[���l�̎擾
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::getBool( bool* b_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �u�[���l�̐ݒ�
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
    // �� �����l�̎擾
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::getInt( int* value_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �����l�̐ݒ�
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
    // �� �����l�̎擾
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::getFloat( lnfloat* value_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �����l�̐ݒ�
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
    // �� �x�N�g���̎擾
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::getVector( LVector4* vec_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �x�N�g���̐ݒ�
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
    // �� �x�N�g���z��̎擾
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::getVectorArray( LVector4* vec_, u32 count_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }
    
    //---------------------------------------------------------------------
    // �� �x�N�g���z��̐ݒ�
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
    // �� �s��̎擾
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::getMatrix( LMatrix* matrix_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �s��̐ݒ�
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
    // �� �s��̔z��̎擾
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::getMatrixArray( LMatrix* matrices_, u32 count_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �s��̔z��̐ݒ�
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
    // �� �e�N�X�`���̐ݒ�
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
    // �� ������̎擾
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::getString( const char** str_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� ������̐ݒ�
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::setString( const char* str_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : setting the string value is not supported." );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �ϐ����̎擾
    //---------------------------------------------------------------------
    const lnChar* ShaderVariable::getName() const
    {
        return mName.c_str();
    }

    //---------------------------------------------------------------------
    // �� �Z�}���e�B�N�X���̎擾
    //---------------------------------------------------------------------
    const lnChar* ShaderVariable::getSemanticName() const
    {
        return mSemanticName.c_str();
    }

    //---------------------------------------------------------------------
    // �� �^���̎擾
    //---------------------------------------------------------------------
    const LNShaderVariableTypeDesc& ShaderVariable::getTypeDesc() const
    {
        return mVariableTypeDesc;
    }

    //---------------------------------------------------------------------
    // �� �C���f�b�N�X�ɂ���ăA�m�e�[�V�������擾����
    //---------------------------------------------------------------------
    IShaderVariable* ShaderVariable::getAnnotationByIndex( u32 index_ )
    {
        if ( index_ >= mAnnotationArray.size() ) { return NULL; }
        return mAnnotationArray[ index_ ];
    }

    //---------------------------------------------------------------------
    // �� ���O�ɂ���ăA�m�e�[�V�������擾����
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
        GLsizei var_size = 0;   // �z��T�C�Y���ۂ�
        GLenum  var_type = 0;
        GLchar  name[256] = { 0 };
        LNGL::glGetActiveUniform( mShader->getGLProgram(), mVarIndex, 256, &name_len, &var_size, &var_type, name );
        //LN_CHECK_GLERROR();

        // �ϐ����ϊ�
        convertVariableTypeGLToLN( var_type, var_size, &mVariableTypeDesc );

        // ���O�L��
        mName = name;

        // �T���v���̏ꍇ�͊��蓖�Ă�ׂ��e�N�X�`�����j�b�g�ԍ����擾����
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

        // �l�̈ꎞ�i�[
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
        // onCreateGLResource() �Ŋm�ۂ����̂ł����ŊJ���c
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
           
            // �e�N�X�`���ϊ��s��̐ݒ�
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
// �� ShaderAnnotation �N���X
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
// �� ShaderTechnique �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    ShaderTechnique::ShaderTechnique( Shader* shader_ )
        : mShader       ( shader_ )
        , mShaderPass   ( NULL )
        , mName         ( _T( "GLTechnique" ) )
    {
        mShaderPass = LN_NEW ShaderPass( mShader );
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
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
    // �� ���O�ɂ���ăA�m�e�[�V�������擾����
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
    // �� �C���f�b�N�X�ɂ���ăA�m�e�[�V�������擾����
    //---------------------------------------------------------------------
    IShaderVariable* ShaderTechnique::getAnnotationByIndex( u32 index_ )
    {
        if ( index_ >= mAnnotationArray.size() ) { return NULL; }
        return mAnnotationArray[ index_ ];
    }

    //---------------------------------------------------------------------
    // �� ���O�ɂ���ăp�X���擾����
    //---------------------------------------------------------------------
    IShaderPass* ShaderTechnique::getPassByName( const lnChar* name_ )
    {
        return mShaderPass;
    }

    //---------------------------------------------------------------------
    // �� �C���f�b�N�X�ɂ���ăp�X���擾����
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
// �� ShaderPass �N���X
//=============================================================================

    static const char* gGLSL_LNAttributeName[LN_VERTEXDECLUSAGE_MAX] =
    {
        "",                 // LN_VERTEXDECLUSAGE_UNKNOWN = 0,
        "lnPosition",       // LN_VERTEXDECLUSAGE_POSITION,        ///< ���_�ʒu
        "lnBlendWeight",    // LN_VERTEXDECLUSAGE_BLENDWEIGHT,     ///< �u�����h�E�F�C�g�l
        "lnBlendIndices",   // LN_VERTEXDECLUSAGE_BLENDINDICES,    ///< �u�����h�C���f�b�N�X�l
        "lnNormal",         // LN_VERTEXDECLUSAGE_NORMAL,          ///< �@��
        "lnTexCoord0",      // LN_VERTEXDECLUSAGE_TEXCOORD0,       ///< UV
        "lnTexCoord1",      // LN_VERTEXDECLUSAGE_TEXCOORD1,       ///< �ǉ�UV1
        "lnTexCoord2",      // LN_VERTEXDECLUSAGE_TEXCOORD2,       ///< �ǉ�UV2
        "lnTexCoord3",      // LN_VERTEXDECLUSAGE_TEXCOORD3,       ///< �ǉ�UV3
        "lnTexCoord4",      // LN_VERTEXDECLUSAGE_TEXCOORD4,       ///< �ǉ�UV4
        "lnTexCoord5",      // LN_VERTEXDECLUSAGE_TEXCOORD5,       ///< Sdef - C�l
        "lnTexCoord6",      // LN_VERTEXDECLUSAGE_TEXCOORD6,       ///< Sdef - R0�l
        "lnTexCoord7",      // LN_VERTEXDECLUSAGE_TEXCOORD7,       ///< Sdef - R1�l
        "lnTexCoord8",      // LN_VERTEXDECLUSAGE_TEXCOORD8,       ///< �G�b�W�E�F�C�g
        "lnPSize15",        // LN_VERTEXDECLUSAGE_PSIZE15,         ///< ���_�C���f�b�N�X�l
        "lnColor",          // LN_VERTEXDECLUSAGE_COLOR,
    };

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    ShaderPass::ShaderPass( Shader* shader_ )
        : mShader   ( shader_ )
        , mBegan    ( false )
    {
        
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    ShaderPass::~ShaderPass()
    {
        for ( u32 i = 0; i < mAnnotationArray.size(); ++i )
        {
            SAFE_DELETE( mAnnotationArray[ i ] );
        }
    }

    //---------------------------------------------------------------------
    // �� ���O�ɂ���ăA�m�e�[�V�������擾����
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
    // �� �C���f�b�N�X�ɂ���ăA�m�e�[�V�������擾����
    //---------------------------------------------------------------------
    IShaderVariable* ShaderPass::getAnnotationByIndex( u32 index_ )
    {
        if ( index_ >= mAnnotationArray.size() ) { return NULL; }
        return mAnnotationArray[ index_ ];
    }

    //---------------------------------------------------------------------
    // �� �p�X�̓K�p���J�n����
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
    // �� �p�X�̓K�p���I������
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
    // �� �p�X�̎��s���ɕω������X�e�[�g���f�o�C�X�ɑ��M����
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

        // ������
        for ( int i = 0; i < LN_VERTEXDECLUSAGE_MAX; ++i )
        {
            mUsageEntryLocation[i] = -1;
        }

        // �������肾���ǂ܂����̂Ƃ��됔���Ȃ��̂ł���ŁE�E�E
        for ( int i = 0; i < params; ++i )
        {
            GLsizei name_len = 0;
            GLsizei var_size = 0;   // �z��T�C�Y���ۂ�
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
// �e�N�X�`���ő吔
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
������؂育�ƂɃe�N�X�`�����J��Ԃ��܂�

 D3DTADDRESS_CLAMP      GL_CLAMP_TO_EDGE
 ���E�����L�΂�

 D3DTADDRESS_BORDER     GL_CLAMP_TO_BORDER
 �͈͊O�h��Ԃ�

            );
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, );
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE

                GL_CLAMP_TO_BORDER
GL_NEAREST
GL_LINEAR

        GL_REPEAT�@���@�摜���J��Ԃ���Ă���Ƃ���Ƃ��ĉ摜�̐F���E��
GL_CLAMP�@���@0�`�P�͈̔͂ɌŒ�
�@���@�摜�̒[�̃s�N�Z���̐F���g���B
GL_CLAMP_TO_BORDER�@���@���E�̐F��ݒ肵�Ă�����g���B(OpenGL 1.3�ȏ�)
GL_MIRRORED_REPEAT�@���@�J��Ԃ��Ƃ��摜�𔽓]������ (OpenGL 1.4 �ȏ�)
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