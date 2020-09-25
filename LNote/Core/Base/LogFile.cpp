//=============================================================================
// 【 LogFile 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <time.h>
#include <string>
#include "../Thread/Mutex.h"
#include "../System/OS.h"
#include "RefString.h"
#include "LogFile.h"

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
// ■ Log モジュール
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    LogFile::LogFile()
        : mMutex        ( NULL )
        , mLogStartTime ( 0 )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    LogFile::~LogFile()
    {
        if ( mLogFilePath[ 0 ] != _T( '\0' ) )
        {
            FILE* stream = _tfopen( mLogFilePath, _T( "a+" ) );
            if ( stream )
            {
                _ftprintf(
                    stream,
                    _T( "===============================================================\n" )
                    _T( "                          end of file\n" )
                    _T( "===============================================================" ) );

                fclose( stream );
            }
                

            LN_SAFE_RELEASE( mMutex );
            //::DeleteCriticalSection( &mCriticalSection );
        }
    }

	//---------------------------------------------------------------------
	// ● 初期化
	//---------------------------------------------------------------------
    bool LogFile::initialize( const lnChar* log_name_, const lnChar* log_dir_ )
    {
        if ( log_dir_ )
        {
            _tcscpy( mLogFilePath, log_dir_ );
            size_t len = _tcslen( log_dir_ );
            mLogFilePath[ len ] = '/';//'\\';
            _tcscpy( &mLogFilePath[ len + 1 ], log_name_ );
        }
        else
        {
            System::getCurrentDirectory( mLogFilePath );
            size_t len = _tcslen( mLogFilePath );
            mLogFilePath[ len ] = '/';//'\\';
            _tcscpy( &mLogFilePath[ len + 1 ], log_name_ );

            /*
            // mLogFilePath にログファイルのフルパスを作る
            DWORD  dir_len  = ::GetCurrentDirectory( 0, NULL );
            //size_t name_len = _tcslen( log_name_ );
            ::GetCurrentDirectory( MAX_PATH, mLogFilePath );
            mLogFilePath[ dir_len - 1 ] = _T( '\\' );
            _tcscpy( &mLogFilePath[ dir_len ], log_name_ );
            */
        }

        
        FILE* stream = _tfopen( mLogFilePath, _T( "w+" ) );

        if ( !stream )
        {
            mLogFilePath[ 0 ] = _T( '\0' );
            return false;
        }

        time_t timer;
        time( &timer );
        char* ts = asctime( localtime( &timer ) );
        ln_std_tstring time_str( ts, strlen( ts ) - 1 );

        _ftprintf( stream, _T( "===============================================================\n" ) );
        _ftprintf( stream, _T( " LNote Log    Version %s    %s\n" ), LNOTE_VERSION_STR, time_str.c_str() );
        _ftprintf( stream, _T( "---------------------------------------------------------------\n" ) );

#ifdef LNOTE_DEBUG
        _ftprintf( stream, _T( "BuildConfig : Debug\n" ) );
#else
        _ftprintf( stream, _T( "BuildConfig : Release\n" ) );
#endif

#ifdef LNOTE_UNICODE
        _ftprintf( stream, _T( "CharSet     : Unicode\n" ) );
#else
        _ftprintf( stream, _T( "CharSet     : MultiByte\n" ) );
#endif

        _ftprintf( stream, _T( "Flags\n" ) );

#ifdef LNOTE_SPECIFIED_CHARCODESET
        _ftprintf( stream, _T( "    LNOTE_SPECIFIED_CHARCODESET\n" ) );
#endif

        
/*
#ifdef UNICODE
        _ftprintf( stream, _T( "UNICODE " ) );
#endif
#ifdef _UNICODE
        _ftprintf( stream, _T( "_UNICODE " ) );
#endif
#ifdef _UNICODE
        _ftprintf( stream, _T( "_MBCS " ) );
#endif
        _ftprintf( stream, _T( ")\n" ) );
        */


        _ftprintf( stream, _T( "===============================================================\n" ) );


        fclose( stream );

        mMutex = LN_NEW Thread::Mutex();
        //::InitializeCriticalSection( &mCriticalSection );

        mLogStartTime = System::getSystemTime();
       
        return true;
    }

    //---------------------------------------------------------------------
	// ● ファイルに追加モードで書き込む
	//---------------------------------------------------------------------
    bool LogFile::write( const char *format_, ... )
	{
        //if ( mLogStartTime )
        {
            mMutex->lock();

            char* buf = (char*)mTempBuffer;

            // mTempBuffer に文字列を作成
            va_list args;
            va_start( args, format_ );
            vsnprintf( buf, TEMP_BUFFER_SIZE, format_, args );
            va_end( args );

            // ファイルを開いて文字列を書き込む
		    FILE* stream = _tfopen( mLogFilePath, _T( "a+" ) );
		    if ( stream )
		    {
                fprintf( stream, "%d:%s\n", System::getSystemTime() - mLogStartTime, buf );
			    fclose( stream );
		    }

            mMutex->unlock();



/*
            static const int BUF_SIZE = 256;
            char buf[ BUF_SIZE ];
            char* str_buf = buf;
		    va_list args;

		    // 引数リストを取得
		    va_start( args, format_ );

		    // 書式指定文字列内の文字数を記憶
		    int len = 2048;//_vscprintf( format_, args ) + 1;

            // 文字列がバッファサイズを超える場合は新しいものを確保
            if ( len >= BUF_SIZE )
            {
		        str_buf = (char*)malloc( len );
            }

		    if ( str_buf )
		    {
			    // バッファに文字列を書き込む
			    vsprintf( str_buf, format_, args );

			    // ファイルを開いて文字列を書き込む
			    FILE* stream = _tfopen( mLogFilePath, _T( "a+" ) );
			    if ( stream )
			    {
                    fprintf( stream, "%d:%s\n", System::getSystemTime() - mLogStartTime, str_buf );
				    fclose( stream );
			    }
    			
                // メモリが確保されていた場合は解放
			    if ( str_buf != buf )
                {
                    SAFE_FREE( str_buf );
                }
		    }

		    va_end( args );

            mMutex->unlock();
            //::LeaveCriticalSection( &mCriticalSection );]
            */
        }

        return true;
	}

    //---------------------------------------------------------------------
	// ● ファイルに追加モードで書き込む ( wchar_t )
    //---------------------------------------------------------------------
    bool LogFile::write( const wchar_t *format_, ... )
    {
        //if ( mLogStartTime )
        {
            mMutex->lock();

            // mTempBuffer に文字列を作成
            va_list args;
            va_start( args, format_ );
            vswprintf( mTempBuffer, TEMP_BUFFER_SIZE, format_, args );
            va_end( args );

            // ファイルを開いて文字列を書き込む
		    FILE* stream = _tfopen( mLogFilePath, _T( "a+" ) );
		    if ( stream )
		    {
                fwprintf( stream, L"%d:%s\n", System::getSystemTime() - mLogStartTime, mTempBuffer );
			    fclose( stream );
		    }

            mMutex->unlock();

            /*
            mMutex->lock();

            static const int BUF_SIZE = 256;
            wchar_t buf[ BUF_SIZE ];
            wchar_t* str_buf = buf;
		    va_list args;

		    va_start( args, format_ );

		    int len = 2048;//_vscwprintf( format_, args ) + 1;

            if ( len >= BUF_SIZE )
            {
		        str_buf = (wchar_t*)malloc( sizeof( wchar_t ) * ( len ) );
            }

		    if ( str_buf )
		    {
			    // バッファに文字列を書き込む
			    vswprintf( str_buf, format_, args );

			    // ファイルを開いて文字列を書き込む
			    FILE* stream = _tfopen( mLogFilePath, _T( "a+" ) );
			    if ( stream )
			    {
                    fwprintf( stream, L"%d:%s\n", ::timeGetTime() - mLogStartTime, str_buf );
				    fclose( stream );
			    }
    			
                // メモリが確保されていた場合は解放
			    if ( str_buf != buf )
                {
                    SAFE_FREE( str_buf );
                }
		    }

		    va_end( args );

            mMutex->unlock();
            //::LeaveCriticalSection( &mCriticalSection );
            */
        }

        return true;
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================