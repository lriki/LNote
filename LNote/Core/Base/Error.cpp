//=============================================================================
// 【 Error 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <stdarg.h>
#include "StringUtil.h"
#include "LogFile.h"
#include "Error.h"
#include "../Thread/Mutex.h"

#if defined(LNOTE_DIRECTX)
#include <d3d9.h>
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
// ■ Error モジュール
//=============================================================================

    static const int		BUFFER_SIZE = 1024;
    static const int		MBTITLE_SIZE = 128;     ///< メッセージボックスのタイトル用に確保するバッファサイズ
    static const int		CONTENTS_BUFFER_SIZE = 512;    ///< 内容、行、ファイル名部分のバッファサイズ
    static const int        LN_MAX_ERROR_CAP = 2;

    const lnChar*  HEADER_FORMAT_STRING =
        _T( "--------------------------------------------------------------------\n" )
        _T( "◆ %s\n" )
        _T( "\n" )
        _T( "%s\n" );

    const lnChar*  HEADER_FORMAT_STRING_DX =
        _T( "--------------------------------------------------------------------\n" )
        _T( "◆ %s\n" )
        _T( "\n" )
        _T( "%s\n" )
        _T( "\n" )
        _T( "%s\n" );

    const lnChar*  FORMAT_STRING =
        _T( "%s\n" )          // setLastErrorString() で設定された文字列
        //_T( "%s\n" )          // ミドルウェア等毎のエラー文字列 ( DirectX のエラー文字列とか )
        _T( "\n" )
        _T( "file : %s\n" )
        _T( "line : %d\n" );

    LogFile*        mLogFile = NULL;                                ///< エラーを出力するログファイル

    lnChar         sPreFormatString[ CONTENTS_BUFFER_SIZE ]     = { 0 };    ///< setPreErrorFormat() で設定される
	lnChar         sErrorContentsString[ CONTENTS_BUFFER_SIZE ] = { 0 };    ///< ヘッダ、フッタ以外の文字列
    lnChar			sLastErrorString[ BUFFER_SIZE ]     = { 0 };    ///< 最後に設定されたエラー文字列 ( [0] が '\0' の場合、使われていない )

    LNRESULT        sLastLNRESULT = LN_OK;                          ///< 最後に設定されたエラーの LNRESULT 値
    //void*           sWindowHandle = NULL;                           ///< メッセージボックスと関連付けるウィンドウ
    
    ErrorObserver*  mErrorObserver = NULL;

    LNErrorProcMode sErrorProcMode = LN_ERROR_PROC_ABORT;           ///< エラー処理の動作方法
    int             sErrorNum = 0;
    bool            sEnableMinimize = false;                        ///< メッセージボックス出力時のウィンドウ最小化の有効設定
    bool            sErrorHold = false;
    bool            sIsSetNecessarily = false;                      ///< Release 状態でも表示する文字列がある場合 true
    bool            sProcessed = false;                             ///< 処理済みフラグ
    
#ifdef LNOTE_DEBUG
    bool		    sIsReleaseMode = false;
#else
    bool		    sIsReleaseMode = true;
#endif



    

    

    static Thread::Mutex gMtx;    // initialize() 呼ばなくても一応使えるようにする細工
    Thread::Mutex*       sMutex = &gMtx;//NULL;




    //---------------------------------------------------------------------
	// ● 初期化
	//---------------------------------------------------------------------
    bool Error::initialize( Thread::Mutex* mutex_ )
    {
        setlocale( LC_ALL, "" );

        finalize();

        

        if ( mutex_ )
        {
            sMutex = mutex_;
        }
        else
        {
            sMutex = LN_NEW Thread::Mutex();
        }

        reset();

        sErrorNum = 0;

        return ( sMutex != NULL );
    }

    //---------------------------------------------------------------------
	// ● 終了処理
	//---------------------------------------------------------------------
    void Error::finalize()
    {
        if ( sMutex != &gMtx )
        {
            SAFE_DELETE( sMutex );
        }
        LN_SAFE_RELEASE( mLogFile );
    }

    //---------------------------------------------------------------------
	// ● エラー文字列を出力するログファイルを設定する
    //---------------------------------------------------------------------
    void Error::setLogFile( LogFile* log_file_ )
    {
        LN_SAFE_RELEASE( mLogFile );
        mLogFile = log_file_;
        LN_SAFE_ADDREF( mLogFile );
    }


    //---------------------------------------------------------------------
	// ● エラー文字列の設定の直前処理 (書式指定の共通部分をまとめたもの)
	//---------------------------------------------------------------------
    void Error::setPreErrorFormat( const lnChar* filename_, const int line_, const lnChar* format_, ... )
    {
        if ( !sErrorHold || sLastErrorString[ 0 ] == _T( '\0' ) )
        {
            // ファイルパスのうち、ファイル名の部分だけを取る
            int i = _tcslen( filename_ );
            for ( ; ( i > 0 ) && filename_[ i ] != _T( '\\' ) && filename_[ i ] != _T( ':' ) ; --i ){
}
            const lnChar* fn = &filename_[ i + 1 ];

            // ファイル名の長さ + 行数 (20 桁まで) + '\n' + '\0'
	        int meta_len = _tcslen( fn ) + 20 + 1 + 1;

            const lnChar* format_top;
            if ( format_[ 0 ] == _T( '#' ) )
            {
                format_top = &( format_[ 1 ] );
                sIsSetNecessarily = true;
            }
            else
            {
                format_top = &( format_[ 0 ] );
                sIsSetNecessarily = false;
            }

            // 可変引数から文字列を作って tmp_buf に格納する
            // シェーダのエラーとかは 1000 を余裕で超えることがあるので動的確保
            va_list args;
            va_start( args, format_ );

            // gcc にはこれに相当する関数がなかったので、今はコレで。
            int args_len = 2048;//_vsctprintf( format_, args ) + 1;
            lnChar* tmp_buf = static_cast< lnChar* >( malloc( args_len * sizeof( lnChar ) ) );
            _vstprintf_s( tmp_buf, args_len, format_top, args );
            
            // CONTENTS_BUFFER_SIZE を超える場合は切る
            if ( args_len > CONTENTS_BUFFER_SIZE )
            {
                tmp_buf[ CONTENTS_BUFFER_SIZE - meta_len - 5 ] = _T( ' ' );
                tmp_buf[ CONTENTS_BUFFER_SIZE - meta_len - 4 ] = _T( '.' );
                tmp_buf[ CONTENTS_BUFFER_SIZE - meta_len - 3 ] = _T( '.' );
                tmp_buf[ CONTENTS_BUFFER_SIZE - meta_len - 2 ] = _T( '.' );
                tmp_buf[ CONTENTS_BUFFER_SIZE - meta_len - 1 ] = _T( '\0' );
            }

            _tcscpy( sErrorContentsString, tmp_buf );

            SAFE_FREE( tmp_buf );

            va_end( args );

            // 全体を結合したときの長さがバッファサイズ以上になる場合は、args_len を縮め、文字列を途中で区切る
	        if ( meta_len + args_len >= CONTENTS_BUFFER_SIZE )
	        {
		        args_len = ( CONTENTS_BUFFER_SIZE - meta_len ) - 1;
		        sErrorContentsString[ args_len ] = _T( '\0' );
	        }

            // sPreFormatString に書き出す
            _stprintf_s( sPreFormatString, CONTENTS_BUFFER_SIZE, FORMAT_STRING, sErrorContentsString, fn, line_ );

        }
    }

    //---------------------------------------------------------------------
	// ● エラー文字列を設定する
	//---------------------------------------------------------------------
    void Error::setLastErrorString( const LNRESULT lr_ )
    {
        sMutex->lock();

		// 既に文字列が設定されている場合はなにもしない
		if ( !sErrorHold || sLastErrorString[ 0 ] == _T( '\0' ) )
        {
            sLastLNRESULT = lr_;

            _stprintf_s(
                sLastErrorString,
                BUFFER_SIZE,
                HEADER_FORMAT_STRING,
                _getLNErrorMessage( lr_ ),
                sPreFormatString );

            if ( sErrorNum < LN_MAX_ERROR_CAP )
            {
                LN_LOG_WRITE( _T( "【エラー発生】\n%s\n" ), sLastErrorString );
                ++sErrorNum;
            }
        }

        if ( sErrorProcMode == LN_ERROR_PROC_IMMABORT )
		{ 
        	*reinterpret_cast< int* >( 0 ) = 0;
		}

		sMutex->unlock();
    }

#if defined(LNOTE_DIRECTX)
    //---------------------------------------------------------------------
	// ● エラー文字列を設定する ( DirectX 用 )
    //---------------------------------------------------------------------
    void Error::setLastErrorStringDx( const HRESULT hr_ )
    {
        sMutex->lock();

		if ( !sErrorHold || sLastErrorString[ 0 ] == _T( '\0' ) )
        {
            sLastLNRESULT = LN_ERR_DIRECTX;

            _stprintf_s(
                sLastErrorString,
                BUFFER_SIZE,
                HEADER_FORMAT_STRING_DX,
                _getLNErrorMessage( LN_ERR_DIRECTX ),
                _getDxErrorMessage( hr_ ),
                sPreFormatString );

            if ( sErrorNum < LN_MAX_ERROR_CAP )
            {
                LN_LOG_WRITE( _T( "【エラー発生】\n%s\n" ), sLastErrorString );
                ++sErrorNum;
            }
        }

        if ( sErrorProcMode == LN_ERROR_PROC_IMMABORT ) { 
        	*reinterpret_cast< int* >( 0 ) = 0; }

		sMutex->unlock();
    }
#endif

    //---------------------------------------------------------------------
	// ● 最後に設定されたエラー文字列を取得する
	//---------------------------------------------------------------------
    const lnChar* Error::getLastErrorString()
    {
        return sPreFormatString;
    }

    //---------------------------------------------------------------------
	// ● エラー処理後の動作方法の設定
    //---------------------------------------------------------------------
    void Error::setProcessMode( LNErrorProcMode mode_ )
    {
        sMutex->lock();
        sErrorProcMode = mode_;
        sMutex->unlock();
    }

    //---------------------------------------------------------------------
	// ● process() で出力されるエラーメッセージを公開版のものにする
    //---------------------------------------------------------------------
    void Error::setEnableReleaseMode( bool flag_ )
    {
        sMutex->lock();
        sIsReleaseMode = flag_;
        sMutex->unlock();
    }

    //---------------------------------------------------------------------
	// ● メッセージボックス出力時のウィンドウ最小化の有効設定
	//---------------------------------------------------------------------
    void Error::setEnableMinimize( bool flag_ )
    {
        sMutex->lock();
        sEnableMinimize = flag_;
        sMutex->unlock();
    }

    //---------------------------------------------------------------------
	// ● エラーを保持するかの設定
	//---------------------------------------------------------------------
    void Error::setEnableErrorHold( bool flag_ )
    {
        sMutex->lock();
        sErrorHold = flag_;
        sMutex->unlock();
    }

    //---------------------------------------------------------------------
	// ● メッセージ文字列をクリアする
	//---------------------------------------------------------------------
    void Error::reset()
    {
        sMutex->lock();

		// 先頭が \0 かどうかのフラグで判定しているのでこれで OK
		sLastErrorString[ 0 ] = _T( '\0' );
        sLastLNRESULT = LN_OK;
        sProcessed = false;

        LN_LOG_WRITE( "最後に発生したエラーをクリアしました。\n\n" );
        LN_LOG_WRITE( "最後に発生したエラーをクリアしました。%d\n\n", 0 );

        LN_LOG_WRITE( _T( "最後に発生したエラーをクリアしました。\n\n" ) );

		sMutex->unlock();
    }
    

    //---------------------------------------------------------------------
	// ● エラー発生時、デフォルトの処理を行う
	//---------------------------------------------------------------------
    void Error::_process( LNRESULT lr_ )
    {
        sMutex->lock();

        // エラー未処理の場合に処理をする
        if ( !sProcessed )
        {
            // エラーが発生している場合
            if ( LN_FAILED( sLastLNRESULT ) )
            {
                // 必要があればウィンドウを最小化
                if ( sEnableMinimize && mErrorObserver )
                {
                    mErrorObserver->onShowWindow( false );
                }

                lnChar contents[ BUFFER_SIZE ];
                lnChar title[ MBTITLE_SIZE ];
                _makeOutputString( contents, BUFFER_SIZE, title, MBTITLE_SIZE );

                _showErrorMessage( contents, title );

                
                //::MessageBox( sWindowHandle, contents, title, MB_OK | MB_ICONERROR );

                // 強制停止させる場合
                if ( sErrorProcMode == LN_ERROR_PROC_ABORT )
                {
                    *reinterpret_cast< int* >( 0 ) = 0;
                }
                else if ( !sErrorHold )
                {
                    // 先頭が \0 かどうかのフラグで判定しているのでこれで OK
		            sLastErrorString[ 0 ] = _T( '\0' );
                    sLastLNRESULT = LN_OK;
                    sProcessed = false;
                    LN_LOG_WRITE( _T( "最後に発生したエラーをクリアしました。\n\n" ) );
                }
                else if ( sErrorHold )
                {
                    // 一時用。連続呼び出しを防止する。
                    // もう一度呼ぶためには一度 reset() を呼ぶこと。
                    sProcessed = true;
                }
            }
            // エラー文字列はセットされていないけど引数でエラーを受け取っている場合
            else if ( LN_FAILED( lr_ ) )
            {
                
                lnChar contents[ BUFFER_SIZE ];
                lnChar title[ MBTITLE_SIZE ];
                _makeOutputString( contents, BUFFER_SIZE, title, MBTITLE_SIZE );
                _showErrorMessage( contents, title );
                


                // 強制停止させる場合
                if ( sErrorProcMode == LN_ERROR_PROC_ABORT )
                {
                    *reinterpret_cast< int* >( 0 ) = 0;
                }
                
                // 一時用。連続呼び出しを防止する。
                // もう一度呼ぶためには一度 reset() を呼ぶこと。
                sProcessed = true;
            }
        }
       
        sMutex->unlock();
    }

    //---------------------------------------------------------------------
	// ● 追加情報付きで標準のエラー処理を行う
    //---------------------------------------------------------------------
    void Error::process( LNRESULT lr_, const lnChar* filename_, const int line_, const lnChar* format_, ... )
    {
        // 書式文字列がないか、公開版の場合は process() に任せる
        if ( format_ == NULL || sIsReleaseMode )
        {
            process( lr_ );
            return;
        }

        sMutex->lock();


        lnChar output_buffer[ BUFFER_SIZE ];
        lnChar output_title[ MBTITLE_SIZE ];

        // 可変引数から文字列を作って buf に格納する
        va_list args;
        va_start( args, format_ );

        int args_len = 2048;//_vsctprintf( format_, args ) + 1;
        lnChar* buf = static_cast< lnChar* >( malloc( args_len * sizeof( lnChar ) ) );
        _vstprintf_s( buf, args_len, format_, args );

        va_end( args );

        // output_buffer に、bufとファイル名と行数を合わせて出力
        int exp_len = _stprintf_s(
            output_buffer,
            BUFFER_SIZE,
            _T( "--------------------------------------------------------------------\n◆ %s\n\nFile:%s\nLine:%d\n\n" ), 
            buf, filename_, line_ );

        SAFE_FREE( buf );

        // 現在の状態から出力文字列を作る
        _makeOutputString( &output_buffer[ exp_len ], BUFFER_SIZE - exp_len - 1, output_title, MBTITLE_SIZE );

        // 出力
        _showErrorMessage( output_buffer, output_title );

        // 強制停止させる場合
        if ( sErrorProcMode == LN_ERROR_PROC_ABORT )
        {
            *reinterpret_cast< int* >( 0 ) = 0;
        }
        else if ( !sErrorHold )
        {
            // 先頭が \0 かどうかのフラグで判定しているのでこれで OK
            sLastErrorString[ 0 ] = _T( '\0' );
            sLastLNRESULT = LN_OK;
            sProcessed = false;
            LN_LOG_WRITE( _T( "最後に発生したエラーをクリアしました。\n\n" ) );
        }
        else if ( sErrorHold )
        {
        
            // 一時用。連続呼び出しを防止する。
            // もう一度呼ぶためには一度 reset() を呼ぶこと。
            sProcessed = true;
        }


        sMutex->unlock();
    }

    //---------------------------------------------------------------------
	// ● エラー発生時のコールバックインターフェイスの設定
    //---------------------------------------------------------------------
    void Error::setErrorObserver( ErrorObserver* eo_ )
    { 
        mErrorObserver = eo_;
    }

    //---------------------------------------------------------------------
	// ● 最後に設定された LNRESULT 値の取得
    //---------------------------------------------------------------------
    LNRESULT Error::getLastResult()
    {
        return sLastLNRESULT;
    }

    //---------------------------------------------------------------------
	// ● 現在の状態から、最終的に出力する文字列を作成する
    //---------------------------------------------------------------------
    void Error::_makeOutputString( lnChar* buffer_, int size_, lnChar* title_, int title_size_ )
    {
        // 公開版の場合
        if ( sIsReleaseMode )
        {
            // リリース版でも表示する文字列があれば、それを出力する
            if ( sIsSetNecessarily )
            {
                _tcsncpy( buffer_, sErrorContentsString, size_ );
            }
            // 無い場合はデフォルト
            else
            {
                _stprintf_s( buffer_, size_, _T( "内部エラーが発生しました。[ %d ]" ), sLastLNRESULT );
            }

            _tcscpy( title_, _T( "Error \n" ) );
        }
        // 公開版ではない場合
        else
        {
            _tcsncpy( buffer_, sLastErrorString, size_ );
            _tcsncpy( title_, _T( "LNote エラー" ), title_size_ );
        }
    }

    /// 文字列を出力する (ErrorObserver が登録されていなければ標準出力に出力する)
    void Error::_showErrorMessage( lnChar* text_, lnChar* caption_ )
    {
        if ( mErrorObserver )
        {
            mErrorObserver->onShowMessageBox( text_, caption_ );
        }
        else
        {
            _tprintf( "--------------------------------\n[%s]\n\n%s\n--------------------------------\n", caption_, text_ );
        }
    }
	
    /*
    //---------------------------------------------------------------------
	// ● エラー文字列を設定する ( wchar_t )
    //---------------------------------------------------------------------
    void Error::_setLastErrorStringW( const wchar_t* filename_, const int line_, const wchar_t* format_, va_list args_, const wchar_t* sub_msg_ )
    {
        // ファイルパスのうち、ファイル名の部分だけを取る
        int i = wcslen( filename_ );
        for ( ; ( i > 0 ) && filename_[ i ] != L'\\' && filename_[ i ] != L':' ; --i ){}
        const wchar_t* fn = &filename_[ i + 1 ];

	    // 可変引数から得られる文字列の長さ以外の長さ
	    int meta_len = 0;
	    meta_len += wcslen( fn );			    // ファイル名の長さ
	    meta_len += 20;						    // ライン数。20 桁まで
	    meta_len += wcslen( FORMAT_STRING_W );  // fmt 文字列の長さ
	    meta_len += wcslen( sub_msg_ );		    // サブ文字列

	    // 可変引数から文字列を作って args_buf に格納する
	    int args_len = _vscwprintf( format_, args_ ) + 1;
	    wchar_t* args_buf = static_cast< wchar_t* >( malloc( sizeof( wchar_t ) * args_len ) );
	    vswprintf_s( args_buf, args_len, format_, args_ );

	    // 全体を結合したときの長さがバッファサイズ以上になる場合は、args_len を縮め、文字列を途中で区切る
	    if ( meta_len + args_len >= BUFFER_SIZE )
	    {
		    args_len = ( BUFFER_SIZE - meta_len ) - 1;
		    args_buf[ args_len ] = '\0';
	    }

	    // 先頭が # の場合はそれを飛ばす
	    wchar_t* arg_s = ( args_buf[ 0 ] == RELEASE_OUT_KEY_W ) ? &( args_buf[ 1 ] ) : &( args_buf[ 0 ] );

	    // sLastErrorStringW に設定する
	    swprintf( sLastErrorStringW, FORMAT_STRING_W, sub_msg_, arg_s, fn, line_ );

        wcscpy( sLastContentsString, arg_s );

	    SAFE_FREE( args_buf );

        LN_LOG_WRITE( L"【エラー発生】\n%s\n", sLastErrorStringW );
        
        sNecessarilyString[ 0 ] = L'\0';

        // 公開版の場合
        if ( sIsReleaseMode )
        {
            if ( format_[ 0 ] == RELEASE_OUT_KEY_W )
		    {
			    // 可変引数から文字列を作って args_buf に格納する
			    int args_len = _vscwprintf( format_, args_ ) + 1;
			    wchar_t* args_buf = static_cast< wchar_t* >( malloc( sizeof( wchar_t ) * args_len ) );
			    vswprintf_s( args_buf, args_len, format_, args_ );

			    // 全体を結合したときの長さがバッファサイズ以上になる場合は、args_len を縮め、文字列を途中で区切る
			    if ( args_len >= BUFFER_SIZE )
			    {
				    args_len = BUFFER_SIZE - 1;
				    args_buf[ args_len ] = L'\0';
			    }

			    wcscpy_s( sNecessarilyString, BUFFER_SIZE, &( args_buf[ 1 ] ) );
    			
			    SAFE_FREE( args_buf );

                sNecessarilyStringIsWide = true;
		    }
        }
    }
    */

/*
    //---------------------------------------------------------------------
	// ● エラー処理 ( char )
    //---------------------------------------------------------------------
    bool Error::_processA()
    {
        bool r;

        // 公開版の場合
        if ( sIsReleaseMode )
        {
            char buf[ 32 ];
            sprintf( buf, "エラー ( %d )", -sLastLNRESULT );
            ::MessageBoxA( sWindowHandle, sLastErrorStringA, "エラー", MB_OK | MB_ICONERROR );
            r = false;
        }
        // 普通に表示
        else
        {
            ::MessageBoxA( sWindowHandle, sLastErrorStringA, "LNote Error", MB_OK | MB_ICONERROR );
            r = false;
        }
        return false;
    }

    //---------------------------------------------------------------------
	// ● エラー処理 ( wchar_t )
    //---------------------------------------------------------------------
    bool Error::_processW()
    {
        bool r;

        // 公開版の場合
        if ( sIsReleaseMode )
        {
            wchar_t buf[ 32 ];
            wsprintf( buf, L"エラー ( %d )", -sLastLNRESULT );
            ::MessageBoxW( sWindowHandle, sLastErrorStringW, L"エラー", MB_OK | MB_ICONERROR );
            r = false;
        }
        // 普通に表示
        else
        {
            ::MessageBoxW( sWindowHandle, sLastErrorStringW, L"LNote Error", MB_OK | MB_ICONERROR );
            r = false;
        }
        return false;
    }
*/
    //---------------------------------------------------------------------
	// ● LNRESULT のエラーメッセージ文字列の取得 ( char )
    //---------------------------------------------------------------------
    const lnChar* Error::_getLNErrorMessage( const LNRESULT lr_ )
    {
        switch ( lr_ )
        {
            case LN_ERR_ASSERT:        
            	return _T( "エラーが発生しました" );
            case LN_ERR_INVALIDCALL:   
            	return _T( "メソッドの呼び出しが無効です。引数に不正な値が渡された可能性があります" );
            case LN_ERR_OUTOFMEMORY:   
            	return _T( "この操作を完了するのに十分なメモリ領域がありません" );
            //case LN_ERR_INDEX:         
            //	return _T( "配列の範囲外を参照しようとしました" );
            case LN_ERR_NULLPOINTER:   
            	return _T( "NULL ポインタにアクセスしようとしました" );
            case LN_ERR_FILENOTFOUND:  
            	return _T( "ファイルが見つかりませんでした" );
            case LN_ERR_SYSTEM:        
            	return _T( "システム機能の呼び出しでエラーが発生しました" );
            case LN_ERR_INITIALIZED:   
            	return _T( "オブジェクトは既に初期化されています" );
            case LN_ERR_FATAL:         
            	return _T( "致命的なエラーが発生しました" );
            case LN_ERR_DIRECTX:       
            	return _T( "DirectX のシステムでエラーが発生しました" );
            default:                   
            	return _T( "[ 不明なエラーコード ]" );
        }
    }

#if defined(LNOTE_DIRECTX)
    //---------------------------------------------------------------------
	// ● DirectX のエラーメッセージ文字列の取得 ( char )
    //---------------------------------------------------------------------
    const lnChar* Error::_getDxErrorMessage( const HRESULT hr_ )
    {
        switch ( hr_ )
		{
            // D3DERR
			case D3DOK_NOAUTOGEN:
				return _T( "[ D3DOK_NOAUTOGEN ]\n ※成功コード　または、ミップマップは自動生成されません。" );
				break;
			case D3DERR_CONFLICTINGRENDERSTATE:
				return _T( "[ D3DERR_CONFLICTINGRENDERSTATE ]\n現在設定されているレンダリングステートは同時には使えません。" );
				break;
			case D3DERR_CONFLICTINGTEXTUREFILTER:
				return _T( "[ D3DERR_CONFLICTINGTEXTUREFILTER ]\n現在のテクスチャフィルタは同時には使えません。" );
				break;
			case D3DERR_CONFLICTINGTEXTUREPALETTE:
				return _T( "[ D3DERR_CONFLICTINGTEXTUREPALETTE ]\n現在のテクスチャは同時には使えません。" );
				break;
			case D3DERR_DEVICELOST:
				return _T( "[ D3DERR_DEVICELOST ]\nデバイスロストが発生していてリセットできません。" );
				break;
			case D3DERR_DEVICENOTRESET:
				return _T( "[ D3DERR_DEVICENOTRESET ]\nデバイスロストが発生していますがリセットできます。" );
				break;
			case D3DERR_DRIVERINTERNALERROR:
				return _T( "[ D3DERR_DRIVERINTERNALERROR ]\n内部ドライバエラーです。" );
				break;
			case D3DERR_INVALIDCALL:
				return _T( "[ D3DERR_INVALIDCALL ]\nメソッドの呼び出しが無効です。\n引数に不正な値が渡された可能性があります。" );
				break;
			case D3DERR_INVALIDDEVICE:
				return _T( "[ D3DERR_INVALIDDEVICE ]\n要求されたデバイスの種類が無効です。" );
				break;
			case D3DERR_MOREDATA:
				return _T( "[ D3DERR_MOREDATA ]\n指定されたバッファ サイズに保持できる以上のデータが存在します。" );
				break;
			case D3DERR_NOTAVAILABLE:
				return _T( "[ D3DERR_NOTAVAILABLE ]\nこのデバイスは、照会されたテクニックをサポートしていません。" );
				break;
			case D3DERR_NOTFOUND:
				return _T( "[ D3DERR_NOTFOUND ]\n要求された項目が見つかりませんでした。" );
				break;
			case D3DERR_OUTOFVIDEOMEMORY:
				return _T( "[ D3DERR_OUTOFVIDEOMEMORY ]\nDirect3D が処理を行うのに十分なディスプレイメモリがありません。" );
				break;
			case D3DERR_TOOMANYOPERATIONS:
				return _T( "[ D3DERR_TOOMANYOPERATIONS ]\nデバイスがサポートしている数より多くの\nテクスチャフィルタリング処理が要求されました。" );
				break;
			case D3DERR_UNSUPPORTEDALPHAARG:
				return _T( "[ D3DERR_UNSUPPORTEDALPHAARG ]\nアルファチャンネルに対して指定されている\nテクスチャブレンディング引数を、デバイスがサポートしていません。" );
				break;
			case D3DERR_UNSUPPORTEDALPHAOPERATION:
				return _T( "[ D3DERR_UNSUPPORTEDALPHAOPERATION ]\nアルファチャンネルに対して指定されている\nテクスチャブレンディング処理を、デバイスがサポートしていません。" );
				break;
			case D3DERR_UNSUPPORTEDCOLORARG:
				return _T( "[ D3DERR_UNSUPPORTEDCOLORARG ]\n色値に対して指定されているテクスチャブレンディング引数を、\nデバイスがサポートしていません。" );
				break;
			case D3DERR_UNSUPPORTEDCOLOROPERATION:
				return _T( "[ D3DERR_UNSUPPORTEDCOLOROPERATION ]\n色値に対して指定されているテクスチャブレンディング処理を、\nデバイスがサポートしていません。" );
				break;
			case D3DERR_UNSUPPORTEDFACTORVALUE:
				return _T( "[ D3DERR_UNSUPPORTEDFACTORVALUE ]\nデバイスが指定されたテクスチャ係数値をサポートしていません。" );
				break;
			case D3DERR_UNSUPPORTEDTEXTUREFILTER:
				return _T( "[ D3DERR_UNSUPPORTEDTEXTUREFILTER ]\nデバイスが指定されたテクスチャ フィルタをサポートしていません。" );
				break;
			case D3DERR_WRONGTEXTUREFORMAT:
				return _T( "[ D3DERR_WRONGTEXTUREFORMAT ]\nテクスチャ サーフェイスのピクセル フォーマットが無効です。" );
				break;

            // D3DXERR
            case D3DXERR_INVALIDDATA:
				return _T( "[ D3DXERR_INVALIDDATA ]" );
				break;

            // E_
			case E_FAIL:
				return _T( "[ E_FAIL ]\nDirect3D サブシステム内で原因不明のエラーが発生しました。" );
				break;
			case E_INVALIDARG:
				return _T( "[ E_INVALIDARG ]\n無効なパラメータが関数に渡されました。" );
				break;
			case E_OUTOFMEMORY:
				return _T( "[ E_OUTOFMEMORY ]\nDirect3D が呼び出しを完了するための十分なメモリを割り当てることができませんでした。" );
				break;
			default:
				return _T( "[ 不明なエラーコード ]" );
				break;
		}
    }
#endif


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================