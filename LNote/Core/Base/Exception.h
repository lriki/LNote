//=============================================================================
//【 Exception 】
//-----------------------------------------------------------------------------
///**
//  @file       Exception.h
//  @brief      Exception
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <string>
#include "Common.h"

/*
    本来であれば
        #define LN_THROW( type_, format_, ... )
    というように format_ も引数として定義するべき。

    ただ、前回のエラー処理マクロでは数が増えすぎて混乱したので、
    今回はシンプルにまとめるためにこんな形にした。

    こうしておくと、同じマクロで書式文字列が省略可能。
    以下のようにオーバーロードしてるっぽく書ける。
        LN_THROW_InvalidCall( r, );
        LN_THROW_InvalidCall( r, "Error %d", 999 );
*/

#define LN_THROW( type_, ... )                      { throw type_( _T( __FILE__ ), __LINE__, __FUNCTION__, ## __VA_ARGS__ ); }

#define LN_THROW_InvalidCall( r_, ... )             { if ( !(r_) ) { LN_THROW( LNote::Core::Base::InvalidCallException, ## __VA_ARGS__ ); } }//throw   ( _T( __FILE__ ), __LINE__, __FUNCTION__, __VA_ARGS__ ); } }
#define LN_THROW_OutOfMemory( r_, ... )             { if ( !(r_) ) { LN_THROW( LNote::Core::Base::OutOfMemoryException, ## __VA_ARGS__ ); } }//throw LNote::Base::OutOfMemoryException  ( _T( __FILE__ ), __LINE__, __FUNCTION__, __VA_ARGS__ ); } }
#define LN_THROW_FileNotFound( r_, filename_, ... ) { if ( !(r_) ) { LN_THROW( LNote::Core::Base::FileNotFoundException, filename_, ## __VA_ARGS__ ); } }//throw LNote::Base::FileNotFoundException ( _T( __FILE__ ), __LINE__, __FUNCTION__, __VA_ARGS__ ); } }
#define LN_THROW_SystemCall( r_, ... )              { if ( !(r_) ) { LN_THROW( LNote::Core::Base::SystemCallException, ## __VA_ARGS__ ); } }//throw LNote::Base::SystemCallException   ( _T( __FILE__ ), __LINE__, __FUNCTION__, __VA_ARGS__ ); } }
#define LN_THROW_FileFormat( r_, ... )              { if ( !(r_) ) { LN_THROW( LNote::Core::Base::FileFormatException, ## __VA_ARGS__ ); } }//throw LNote::Base::FileFormatException   ( _T( __FILE__ ), __LINE__, __FUNCTION__, __VA_ARGS__ ); } }
#define LN_THROW_NotImpl( r_, ... )                 { if ( !(r_) ) { LN_THROW( LNote::Core::Base::NotImplementedException, ## __VA_ARGS__ ); } }//throw LNote::Base::FatalException        ( _T( __FILE__ ), __LINE__, __FUNCTION__, __VA_ARGS__ ); } }
#define LN_THROW_Initialized( r_, ... )             { if ( !(r_) ) { LN_THROW( LNote::Core::Base::InitializedException, ## __VA_ARGS__ ); } }//throw LNote::Base::FatalException        ( _T( __FILE__ ), __LINE__, __FUNCTION__, __VA_ARGS__ ); } }
#define LN_THROW_Fatal( r_, ... )                   { if ( !(r_) ) { LN_THROW( LNote::Core::Base::FatalException, ## __VA_ARGS__ ); } }//throw LNote::Base::FatalException        ( _T( __FILE__ ), __LINE__, __FUNCTION__, __VA_ARGS__ ); } }
#define LN_THROW_Com( hr_, ... )                    { if ( FAILED( hr ) ) { LN_THROW( LNote::Core::Base::ComException, hr_, ## __VA_ARGS__ ); } }//throw LNote::Base::ComException          ( _T( __FILE__ ), __LINE__, __FUNCTION__, hr_, __VA_ARGS__ ); } }

// x_ は HRESULT を返す関数呼び出し  例: LN_COMCALL( Test2() );
#define LN_COMCALL( x_ )                            { hr = ( x_ ); if ( FAILED( hr ) ) { LN_THROW_Com( hr, _T( "%s\n" ), #x_ ); } }
#define LN_COMCALLF( x_, format_, ... )             { hr = ( x_ ); if ( FAILED( hr ) ) { LN_THROW_Com( hr, _T( format_"\n\n%s\n" ), __VA_ARGS__, #x_ ); } }

// glGetError() でエラーチェック (各 gl～ の後で必ず呼ばないと正しいエラーが取れないので注意)
#define LN_CHECK_GLERROR( ... )                     { GLenum lnglerr = glGetError(); if ( lnglerr != GL_NO_ERROR ) { LN_THROW( LNote::Core::Base::OpenGLException, lnglerr, ## __VA_ARGS__ ); } }

#define LN_ERRLOG_WRITE( format_, ... )             { LNote::Core::Base::ExceptionManager::getLogFile()->write( ( format_ ), ## __VA_ARGS__ ); }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Base
{
class LogFile;
class Exception;

//=============================================================================
// ■ (通常、1スレッドにつきひとつ必要。今のところ static だけど…)
//=============================================================================
class ExceptionManager
{
public:

    /// 初期化
    static void initialize( LogFile* log_file_ );

    /// 終了処理
    static void finalize();

    /// 例外のデフォルト処理
    static void procException( const Exception& exp_ );

    /// 最後に発生した例外の取得
    static Exception& getLastException() { return mLastException; }

    /// LN_ERRLOG_WRITE() マクロ用
    static LogFile* getLogFile() { return mLogFile; }

private:

    static void setLastException( const Exception& exp_ );

private:

    static LogFile*         mLogFile;
    static Exception        mLastException;

    friend class Exception;
};

/// 全ての例外の基底
class Exception
{
public:

    Exception( const lnChar* file_, const int line_, const lnChar* func_, LNResultCode err_code_ );

    Exception( const Exception& exp_ );

    Exception& operator = ( const Exception& exp_ );

    virtual ~Exception() {}

public:

    const lnChar* getErrorMessage() const { return mErrorMessage.c_str(); }

    const lnChar* getOutputMessage() const { return mOutputMessage.c_str(); }

    LNRESULT getErrorCode() const { return mErrorCode; }

    /// 例外のデフォルト処理
    void process() const;

    /// 最後に発生した例外の取得
    static Exception& getLastException();

protected:

    void _updateOutputMessage( const lnChar* caption_ );

protected:
    
    ln_std_tstring  mFile;
    int             mLine;
    ln_std_tstring  mFunc;
    ln_std_tstring  mErrorMessage;
    ln_std_tstring  mOutputMessage;
    LNRESULT        mErrorCode;
};

/// 無効な関数呼び出し
class InvalidCallException
    : public Exception
{
public:
    InvalidCallException( const lnChar* file_, const int line_, const lnChar* func_ );
    InvalidCallException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* format_, ... );
};

/// メモリ不足 (malloc() またはLN_NEW で確保する用。普通の new は bad_alloc)
class OutOfMemoryException
    : public Exception
{
public:
    OutOfMemoryException( const lnChar* file_, const int line_, const lnChar* func_ );
    OutOfMemoryException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* format_, ... );
};

/// ファイルが見つからなかった
class FileNotFoundException
    : public Exception
{
public:
    FileNotFoundException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* filename_ );
    FileNotFoundException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* filename_, const lnChar* format_, ... );
};

/// システムコールエラー (API 呼び出しエラー)
class SystemCallException
    : public Exception
{
public:
    SystemCallException( const lnChar* file_, const int line_, const lnChar* func_ );
    SystemCallException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* format_, ... );
};

/// 非対応ファイルの形式
class FileFormatException
    : public Exception
{
public:
    FileFormatException( const lnChar* file_, const int line_, const lnChar* func_ );
    FileFormatException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* format_, ... );

    /// 対象のファイル名を追加
    void setFilename( const lnChar* filename_ );
};

/// 初期化済み
class InitializedException
    : public Exception
{
public:
    InitializedException( const lnChar* file_, const int line_, const lnChar* func_ );
    InitializedException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* format_, ... );
};

/// 未実装の機能
class NotImplementedException
    : public Exception
{
public:
    NotImplementedException( const lnChar* file_, const int line_, const lnChar* func_ );
    NotImplementedException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* format_, ... );
};

/// 継続不能な内部エラー
class FatalException
    : public Exception
{
public:
    FatalException( const lnChar* file_, const int line_, const lnChar* func_ );
    FatalException( const lnChar* file_, const int line_, const lnChar* func_, const lnChar* format_, ... );
};



#if LNOTE_INCLUDED_DIRECTX_HEADER
/// COM 呼び出しエラー (HRESULT を戻り値とする呼び出しエラー)
class ComException
    : public Exception
{
public:
    ComException( const lnChar* file_, const int line_, const lnChar* func_, HRESULT hr_ );
    ComException( const lnChar* file_, const int line_, const lnChar* func_, HRESULT hr_, const lnChar* format_, ... );
};
#endif

/// OpenGL エラー
class OpenGLException
    : public Exception
{
public:
    OpenGLException( const lnChar* file_, const int line_, const lnChar* func_, lnU32 gl_enum_ );    //GLenum
    OpenGLException( const lnChar* file_, const int line_, const lnChar* func_, lnU32 gl_enum_, const lnChar* format_, ... );
    lnU32 getGLErrorCode() const { return mGLErrorCode; }

public:

    static const lnChar* convertGLErrorToString(lnU32 gl_enum_);

private:
    
    lnU32   mGLErrorCode;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================