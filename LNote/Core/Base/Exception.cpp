//=============================================================================
// 【 Exception 】
//=============================================================================


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Resource/LNResource.h"
#include "../Thread/Mutex.h"
#include "Exception.h"

#if defined(LNOTE_ANDROID)
    #include <EGL/egl.h>
    #include <EGL/eglext.h>
    #include <GLES/gl.h>
    #include <GLES/glext.h> // for glGenFramebuffersOES
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Base
{

//=============================================================================
// ■ ExceptionManager
//=============================================================================

    LogFile*        ExceptionManager::mLogFile = NULL;
    Exception       ExceptionManager::mLastException( "default", 0, "default", LN_OK );
    static Thread::Mutex  gExceptionManagerMutex;

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    void ExceptionManager::initialize( LogFile* log_file_ )
    {
        mLogFile = log_file_;
        LN_SAFE_ADDREF( mLogFile );
    }

    //---------------------------------------------------------------------
	// ● 終了処理
    //---------------------------------------------------------------------
    void ExceptionManager::finalize()
    {
        LN_SAFE_RELEASE( mLogFile );
    }

    //---------------------------------------------------------------------
	// ● 例外のデフォルト処理
    //---------------------------------------------------------------------
    void ExceptionManager::procException( const Exception& exp_ )
    {
        if ( LN_FAILED( exp_.getErrorCode() ) )
        {
            gExceptionManagerMutex.lock();

        
            _tprintf( "--------------------------------\n" );
            _tprintf( "%s\n", exp_.getOutputMessage() );
            _tprintf( "--------------------------------\n" );

            if ( mLogFile )
            {
                mLogFile->write(
                    "\n-------------------------------------------------------\n[ Error ]\n%s\n-------------------------------------------------------\n",
                    exp_.getOutputMessage() );
            }

            gExceptionManagerMutex.unlock();
        }
    }

    //---------------------------------------------------------------------
	// ● 
    //---------------------------------------------------------------------
    void ExceptionManager::setLastException( const Exception& exp_ )
    {
        gExceptionManagerMutex.lock();
        mLastException = exp_;
        gExceptionManagerMutex.unlock();
    }




    static LRefTString makeErrorString(
        const lnChar* format_,
        va_list args_ )
    {
        /*
        // ファイルパスのうち、ファイル名の部分だけを取る
        int i = _tcslen( filename_ );
        for ( ; ( i > 0 ) && filename_[ i ] != _T( '\\' ) && filename_[ i ] != _T( ':' ) ; --i ){}
        const lnChar* fn = &filename_[ i + 1 ];

        // ファイル名の長さ + 行数 (20 桁まで) + '\n' + '\0'
        int meta_len = _tcslen( fn ) + 20 + 1 + 1;
        */

        // 先頭が # の場合はその次の文字から
        const lnChar* format_top;
        if ( format_[ 0 ] == _T( '#' ) )
        {
            format_top = &( format_[ 1 ] );
            //sIsSetNecessarily = true;
        }
        else
        {
            format_top = &( format_[ 0 ] );
            //sIsSetNecessarily = false;
        }

#if defined(LNOTE_WINDOWS)
        int args_len = _vsctprintf( format_, args_ ) + 1;
        lnChar* tmp_buf = LN_NEW lnChar[ args_len ];
        _vstprintf_s( tmp_buf, args_len, format_top, args_ );
     
#else
        static const int CONTENTS_BUFFER_SIZE = 2000;

        // gcc にはこれに相当する関数がなかったので、今はコレで。
        int args_len = CONTENTS_BUFFER_SIZE;
        lnChar* tmp_buf = LN_NEW lnChar[ args_len ];
        _vstprintf_s( tmp_buf, args_len, format_top, args_ );

        // CONTENTS_BUFFER_SIZE を超える場合は終端を ... にする
        if ( args_len > CONTENTS_BUFFER_SIZE )
        {
            tmp_buf[ CONTENTS_BUFFER_SIZE - 5 ] = _T( ' ' );
            tmp_buf[ CONTENTS_BUFFER_SIZE - 4 ] = _T( '.' );
            tmp_buf[ CONTENTS_BUFFER_SIZE - 3 ] = _T( '.' );
            tmp_buf[ CONTENTS_BUFFER_SIZE - 2 ] = _T( '.' );
            tmp_buf[ CONTENTS_BUFFER_SIZE - 1 ] = _T( '\0' );
        }
#endif

        LRefTString out = tmp_buf;
        SAFE_DELETE_ARRAY( tmp_buf );
        return out;
    }

    //-------------------------------------------------------------------------
    // Exception

    Exception::Exception( const lnChar* file_, const int line_, const lnChar* func_, LNResultCode err_code_ )
    {
        mFile = file_;
        mLine = line_;
        mFunc = func_;
        mErrorCode = err_code_;
    }

    Exception::Exception( const Exception& exp_ )
    {
        mFile = exp_.mFile;
        mLine = exp_.mLine;
        mFunc = exp_.mFunc;
        mErrorMessage = exp_.mErrorMessage;
        mOutputMessage = exp_.mOutputMessage;
        mErrorCode = exp_.mErrorCode;
    }

    Exception& Exception::operator = ( const Exception& exp_ )
    {
        mFile = exp_.mFile;
        mLine = exp_.mLine;
        mFunc = exp_.mFunc;
        mErrorMessage = exp_.mErrorMessage;
        mOutputMessage = exp_.mOutputMessage;
        mErrorCode = exp_.mErrorCode;
        return *this;
    }

    // 例外のデフォルト処理
    void Exception::process() const
    {
         ExceptionManager::procException( *this );
    }

    // 最後に発生した例外の取得
    Exception& Exception::getLastException()
    {
        return ExceptionManager::getLastException();
    }

    void Exception::_updateOutputMessage( const lnChar* caption_ )
    {
        LRefTString tmp;
        tmp.format(
            _T( "%s\n\n%s\n\nfile:%s\nline:%d\nfunc:%s" ),
            caption_,
            mErrorMessage.c_str(),
            mFile.c_str(),
            mLine,
            mFunc.c_str() );
        mOutputMessage = tmp.c_str();

        ExceptionManager::setLastException( *this );
    }

    //-------------------------------------------------------------------------
    // InvalidCallException

    InvalidCallException::InvalidCallException( const lnChar* file_, const int line_, const lnChar* func_ )
        : Exception( file_, line_, func_, LN_ERR_INVALIDCALL )
    {
        _updateOutputMessage( Resource::String::ERR_InvalidCallException );
    }

    InvalidCallException::InvalidCallException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* format_, ... )
        : Exception( file_, line_, func_, LN_ERR_INVALIDCALL )
    {
        va_list args;
        va_start( args, format_ );
        mErrorMessage = makeErrorString( format_, args );
        va_end( args );

        _updateOutputMessage( Resource::String::ERR_InvalidCallException );
    }

    //-------------------------------------------------------------------------
    // OutOfMemoryException

    OutOfMemoryException::OutOfMemoryException( const lnChar* file_, const int line_, const lnChar* func_ )
        : Exception( file_, line_, func_, LN_ERR_OUTOFMEMORY )
    {
        _updateOutputMessage( Resource::String::ERR_InvalidCallException );
    }

    OutOfMemoryException::OutOfMemoryException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* format_, ... )
        : Exception( file_, line_, func_, LN_ERR_OUTOFMEMORY )
    {
        va_list args;
        va_start( args, format_ );
        mErrorMessage = makeErrorString( format_, args );
        va_end( args );

        _updateOutputMessage( Resource::String::ERR_InvalidCallException );
    }

    //-------------------------------------------------------------------------
    // FileNotFoundException

    FileNotFoundException::FileNotFoundException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* filename_ )
        : Exception( file_, line_, func_, LN_ERR_FILENOTFOUND )
    {
        mErrorMessage = filename_;
        _updateOutputMessage( Resource::String::ERR_FileNotFoundException );
    }

    FileNotFoundException::FileNotFoundException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* filename_, const lnChar* format_, ... )
        : Exception( file_, line_, func_, LN_ERR_FILENOTFOUND )
    {
        va_list args;
        va_start( args, format_ );
        mErrorMessage = filename_;
        mErrorMessage += _T( "\n\n" );
        mErrorMessage += makeErrorString( format_, args ).c_str();
        va_end( args );

        _updateOutputMessage( Resource::String::ERR_FileNotFoundException );
    }

    //-------------------------------------------------------------------------
    // SystemCallException

    SystemCallException::SystemCallException( const lnChar* file_, const int line_, const lnChar* func_ )
        : Exception( file_, line_, func_, LN_ERR_SYSTEM )
    {
        _updateOutputMessage( Resource::String::ERR_SystemCallException );
    }

    SystemCallException::SystemCallException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* format_, ... )
        : Exception( file_, line_, func_, LN_ERR_SYSTEM )
    {
        va_list args;
        va_start( args, format_ );
        mErrorMessage = makeErrorString( format_, args );
        va_end( args );

        _updateOutputMessage( Resource::String::ERR_SystemCallException );
    }

    //-------------------------------------------------------------------------
    // FileFormatException

    FileFormatException::FileFormatException( const lnChar* file_, const int line_, const lnChar* func_ )
        : Exception( file_, line_, func_, LN_ERR_FORMAT )
    {
        _updateOutputMessage( Resource::String::ERR_FileFormatException );
    }

    FileFormatException::FileFormatException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* format_, ... )
        : Exception( file_, line_, func_, LN_ERR_FORMAT )
    {
        va_list args;
        va_start( args, format_ );
        mErrorMessage = makeErrorString( format_, args );
        va_end( args );

        _updateOutputMessage( Resource::String::ERR_FileFormatException );
    }

    //-------------------------------------------------------------------------
    // NotImplementedException

    NotImplementedException::NotImplementedException( const lnChar* file_, const int line_, const lnChar* func_ )
        : Exception( file_, line_, func_, LN_ERR_NOTIMPL )
    {
        _updateOutputMessage( Resource::String::ERR_NotImplementedException );
    }

    NotImplementedException::NotImplementedException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* format_, ... )
        : Exception( file_, line_, func_, LN_ERR_NOTIMPL )
    {
        va_list args;
        va_start( args, format_ );
        mErrorMessage = makeErrorString( format_, args );
        va_end( args );

        _updateOutputMessage( Resource::String::ERR_NotImplementedException );
    }

    //-------------------------------------------------------------------------
    // InitializedException

    InitializedException::InitializedException( const lnChar* file_, const int line_, const lnChar* func_ )
        : Exception( file_, line_, func_, LN_ERR_NOINITIALIZE )
    {
        _updateOutputMessage( Resource::String::ERR_InitializedException );
    }

    InitializedException::InitializedException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* format_, ... )
        : Exception( file_, line_, func_, LN_ERR_NOINITIALIZE )
    {
        va_list args;
        va_start( args, format_ );
        mErrorMessage = makeErrorString( format_, args );
        va_end( args );

        _updateOutputMessage( Resource::String::ERR_InitializedException );
    }

    //-------------------------------------------------------------------------
    // FatalException

    FatalException::FatalException( const lnChar* file_, const int line_, const lnChar* func_ )
        : Exception( file_, line_, func_, LN_ERR_FATAL )
    {
        _updateOutputMessage( Resource::String::ERR_FatalException );
    }

    FatalException::FatalException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* format_, ... )
        : Exception( file_, line_, func_, LN_ERR_FATAL )
    {
        va_list args;
        va_start( args, format_ );
        mErrorMessage = makeErrorString( format_, args );
        va_end( args );

        _updateOutputMessage( Resource::String::ERR_FatalException );
    }

    
#if defined(LNOTE_WINDOWS)
    //-------------------------------------------------------------------------
    // ComException

    ComException::ComException( const lnChar* file_, const int line_, const lnChar* func_, HRESULT hr_ )
        : Exception( file_, line_, func_, LN_ERR_COM )
    {
        LRefTString tmp;
        tmp.format( _T( "HRESULT 0x%x" ), hr_ );
        mErrorMessage = tmp;
        _updateOutputMessage( Resource::String::ERR_ComException );
    }

    ComException::ComException( const lnChar* file_, const int line_, const lnChar* func_, HRESULT hr_, const lnChar* format_, ... )
        : Exception( file_, line_, func_, LN_ERR_COM )
    {
        va_list args;
        va_start( args, format_ );
        mErrorMessage = makeErrorString( format_, args );
        va_end( args );

        LRefTString tmp;
        tmp.format( _T( "\nHRESULT 0x%x" ), hr_ );
        mErrorMessage += tmp.c_str();

        _updateOutputMessage( Resource::String::ERR_ComException );
    }
#endif

    //-------------------------------------------------------------------------
    // OpenGLException

    OpenGLException::OpenGLException( const lnChar* file_, const int line_, const lnChar* func_, lnU32 gl_err_ )
        : Exception( file_, line_, func_, LN_ERR_OPENGL )
    {
        mGLErrorCode = gl_err_;
        
        LRefTString tmp;
        tmp.format( _T( "GLenum 0x%x\n%s" ), gl_err_, convertGLErrorToString(gl_err_) );
        mErrorMessage = tmp;
        _updateOutputMessage( Resource::String::ERR_OpenGLException );
    }

    OpenGLException::OpenGLException( const lnChar* file_, const int line_, const lnChar* func_, lnU32 gl_err_, const lnChar* format_, ... )
        : Exception( file_, line_, func_, LN_ERR_OPENGL )
    {
        mGLErrorCode = gl_err_;
        
        va_list args;
        va_start( args, format_ );
        mErrorMessage = makeErrorString( format_, args );
        va_end( args );

        LRefTString tmp;
        tmp.format( _T( "GLenum 0x%x\n%s" ), gl_err_, convertGLErrorToString(gl_err_) );
        mErrorMessage += tmp.c_str();

        _updateOutputMessage( Resource::String::ERR_OpenGLException );
    }

    const lnChar* OpenGLException::convertGLErrorToString(lnU32 gl_enum_)
    {
        switch (gl_enum_)
        {
            case GL_INVALID_ENUM: return _T("GL_INVALID_ENUM");
            case GL_INVALID_VALUE: return _T("GL_INVALID_VALUE");
            case GL_INVALID_OPERATION: return _T("GL_INVALID_OPERATION");
#if defined(LNOTE_GLES)
            case GL_INVALID_FRAMEBUFFER_OPERATION_OES: return _T("GL_INVALID_FRAMEBUFFER_OPERATION_OES");
#endif
        }
        return _T("");
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================