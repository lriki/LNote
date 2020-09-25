//=============================================================================
// 【 Error 】
//-----------------------------------------------------------------------------
///**
//  @file       Error.h
//  @brief      Error
//  @author     Riki
//*/
//=============================================================================

#pragma once

#include "Common.h"

// __FILE__ の TCHAR 版 LN_FILE
#define LN_PREFILE( x )	_T( x )
#define LN_FILE	        LN_PREFILE( __FILE__ )

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
    

    enum LNErrorProcMode
    {
        LN_ERROR_PROC_CONTINUE = 0,         ///< メッセージボックス出力後、継続する
        //LN_ERROR_PROC_CONTINUE_CLEAR = 0,   ///< メッセージボックス出力後、処理したエラーをクリアして継続する
        LN_ERROR_PROC_ABORT,                ///< メッセージボックス出力後、強制停止する
        LN_ERROR_PROC_IMMABORT,             ///< エラーが発生した瞬間に動作を停止する (ライブラリ内部のデバッグ用)
    };
}

// HRESULT 使おうか迷ったけど、自分で定義したの使った方がスッキリすると思う。
// HRESULT だと 例えば0除算なんかはDISP_E_DIVBYZEROとか見慣れないの使わないとならないみたいだし。
// あと、なんか同じような意味の値がいくつかあった気がする。

//-------------------------------------------------------------------------
// 公開用エラー発生マクロ

#define LN_ASSERT_S( r_, format_, ... )	    { if (!(r_)) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, ( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorString( LN_ERR_ASSERT ); LNErrMD::process( LN_ERR_ASSERT ); } }

#ifdef LNOTE_DEBUG
    #define LN_ASSERT( r_, format_, ... )   { if (!(r_)) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, ( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorString( LN_ERR_ASSERT ); LNErrMD::process( LN_ERR_ASSERT ); } }
#else
    #define LN_ASSERT( r_, format_, ... )
#endif


//-------------------------------------------------------------------------
// LNRESULT チェック系マクロ

#define LN_SUCCEEDED( lr_ )                         (((LNRESULT)(lr_)) >= 0)
#define LN_FAILED( lr_ )                            ( ((LNRESULT)(lr_)) < 0 )
#define LN_EVENT_HANDLED( lr_ )                     ( lr_ < 0 || lr_ == LN_HANDLED )

// LNRESULT を返す関数の結果をそのまま返す場合に使う
#define LN_CALL_R( x_ )                             { lr = ( x_ ); if( LN_FAILED( lr ) ) { return lr; } }
#define LN_CALL_GOTO( label_, x_ )                  { lr = ( x_ ); if( LN_FAILED( lr ) ) { goto label_; } }

// エラー処理付き関数呼び出し
#define LN_PCALL_S( x_ )                            { LNErrMD::process( x_ ); }
#define LN_PCALLF_S( x_, format_, ... )             { LNErrMD::process( x_, _T( __FILE__ ),  __LINE__, format_, __VA_ARGS__ ); }

#ifdef LNOTE_DEBUG
    #define LN_PCALL( x_ )                          { LNErrMD::process( x_ ); }
    #define LN_PCALLF( x_, format_, ... )           { LNErrMD::process( x_, _T( __FILE__ ),  __LINE__, format_, __VA_ARGS__ ); }
#else
    #define LN_PCALL( x_ )                          { LNErrMD::process( x_ ); }
    #define LN_PCALLF( x_, format_, ... )           { LNErrMD::process( x_ ); }
#endif

//-------------------------------------------------------------------------
// 内部用のエラー設定マクロ



#define LN_SETERR_S( lr_, format_, ... )	        {                  LNErrMD::setPreErrorFormat( LN_FILE, __LINE__, _T( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorString( lr_ ); }
#define LN_SETERR_R_S( lr_, format_, ... )	        {                  LNErrMD::setPreErrorFormat( LN_FILE, __LINE__, _T( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorString( lr_ ); return lr_; }
#define LN_SETERR_R_S_( r_, lr_, format_, ... )     { if ( !( r_ ) ) { LNErrMD::setPreErrorFormat( LN_FILE, __LINE__, _T( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorString( lr_ ); return lr_; } }

#ifdef LNOTE_DEBUG
    #define LN_SETERR_R_( r_, lr_, format_, ... )   { if ( !( r_ ) ) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, ( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorString( lr_ ); return lr_; } }
#else
    #define LN_SETERR_R_( r_, lr_, format_, ... )
#endif

// ヘッダ文字列の設定 (ファイル名など)
// LN_FAILED() でエラー判定を行ってから設定すること。
//#define LN_SETERR_HEADER( format_, ... )            { }//LNErrMD::setHeaderString( format_, __VA_ARGS__ ); }
//#define LN_SETERR_GOTO_S_( r_, label_, lr_, format_, ... )  { if ( !( r_ ) ) { LNErrMD::setPreErrorFormat( LN_FILE, __LINE__, _T( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorString( lr_ ); goto label_; } }


//-------------------------------------------------------------------------
// DirectX 用のエラー設定マクロ (fn_ : 呼び出し側関数名)

#define LN_SETDXERR_S( hr_, format_, ... )          {              if ( FAILED( hr_ ) ) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, ( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorStringDx( hr_ ); } }
#define LN_SETDXERR_R_S( hr_, format_, ... )        {              if ( FAILED( hr_ ) ) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, ( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorStringDx( hr_ ); return LN_ERR_DIRECTX; } }
#define LN_DXCALL_S( x_, fn_ )                      { hr = ( x_ ); if ( FAILED( hr  ) ) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, "< %s >\n%s", fn_, #x_ ); LNErrMD::setLastErrorStringDx( hr  ); } }
#define LN_DXCALL_R_S( x_, fn_ )                    { hr = ( x_ ); if ( FAILED( hr  ) ) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, "< %s >\n%s", fn_, #x_ ); LNErrMD::setLastErrorStringDx( hr  ); return LN_ERR_DIRECTX; } }

#ifdef LNOTE_DEBUG
    #define LN_DXCALL_R( x_ )                       { hr = ( x_ ); if ( FAILED( hr  ) ) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, "< %s >\n%s", __FUNCTION__, #x_ ); LNErrMD::setLastErrorStringDx( hr ); return LN_ERR_DIRECTX; } }
    #define LN_DXCALLF_R( x_, format_, ... )        { hr = ( x_ ); if ( FAILED( hr  ) ) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, "< %s >\n%s\n\n"format_, __FUNCTION__, #x_, __VA_ARGS__ ); LNErrMD::setLastErrorStringDx( hr ); return LN_ERR_DIRECTX; } }
#else
    #define LN_DXCALL_R( x_ )                       { hr = ( x_ ); if ( FAILED( hr  ) ) { return LN_ERR_DIRECTX; } }
    #define LN_DXCALLF_R( x_, format_, ... )        { hr = ( x_ ); }
#endif


//-------------------------------------------------------------------------
// エラー処理用マクロ

////#define LN_ERRPROC_S()                              { LNErrMD::process(); }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Thread { class Mutex; }

namespace Base
{

class LogFile;
class ErrorObserver;

//=============================================================================
// ■ Error モジュール
//-----------------------------------------------------------------------------
///**
//	@brief      エラーに関する情報を扱うモジュール
//
//  @par
//              ◆ 使い方<br>
//                  ・initialize() で初期化<br>
//                  ・エラー用マクロ ( LN_SETERR 等 ) でエラーを設定<br>
//                  ・process() でエラー処理<br>
//                  ・finalize() で終了<br>
//                  <br>
//                  ( マクロ使わないで直接 setLastErrorStringA とか呼んでもいいけど、マクロ使った方がずっと簡単です )<br>
//              <br>
//              基本的にこのモジュールは LNRESULT 関係のエラーを扱います。<br>
//              そのため、このモジュールの関数が LNRESULT のエラーを返しても
//              あんまり意味がない気がするのでここでは bool で成功/失敗を
//              返しています。
//*/
//=============================================================================
class Error
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      初期化
    //
    //  @param[in]  log_file_ : ログを出力するときに使うログクラス
    //  @param[in]  mutex_    : スレッドセーフで実装するときのミューテックス
    //
    //  @retval     true  : 成功
    //  @retval     false : 失敗
    //
    //  @par
    //              エラー処理モジュールを初期化します。
    //              通常、プログラムの先頭で一度だけ呼び出します。<br><br>
    //              
    //              log_file_ に NULL を指定するとログを出力しません。<br><br>
    //              
    //              mutex_ が省略された場合はデフォルトのミューテックスが使われます。
    //*/
    //---------------------------------------------------------------------
    static bool initialize( Thread::Mutex* mutex_ = NULL );

    //---------------------------------------------------------------------
    ///**
    //  @brief      終了処理
    //*/
    //---------------------------------------------------------------------
    static void finalize();

    //---------------------------------------------------------------------
    ///**
    //  @brief      エラー文字列を出力するログファイルを設定する (参照カウントがひとつ増える)
    //*/
    //---------------------------------------------------------------------
    static void setLogFile( LogFile* log_file_ );


    //---------------------------------------------------------------------
    ///**
    //  @brief      エラー文字列の設定の直前処理
    //
    //  @note
    //              書式指定の共通部分をまとめたもの。
    ///             setLastErrorString() 等の直前に呼び出す。
    //*/
    //---------------------------------------------------------------------
    static void setPreErrorFormat( const lnChar* filename_, const int line_, const lnChar* format_, ... );


    //---------------------------------------------------------------------
    ///**
    //  @brief      エラー文字列を設定する
    //*/
    //---------------------------------------------------------------------
    static void setLastErrorString( const LNRESULT lr_ );


#if LNOTE_INCLUDED_DIRECTX_HEADER
    //---------------------------------------------------------------------
    ///**
    //  @brief      エラー文字列を設定する (HRESULT 用)
    //*/
    //---------------------------------------------------------------------
    static void setLastErrorStringDx( const HRESULT hr_ );
#endif

    //---------------------------------------------------------------------
    ///**
    //  @brief      最後に設定されたエラー文字列を取得する
    //*/
    //---------------------------------------------------------------------
    static const lnChar* getLastErrorString();



    //---------------------------------------------------------------------
    ///**
    //  @brief      エラー処理の動作方法の設定
    //
    //  @par
    //              デフォルトは LN_ERROR_PROC_ABORT です。
    //*/
    //---------------------------------------------------------------------
    static void setProcessMode( LNErrorProcMode mode_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      process() で出力されるエラーメッセージを公開版のものにする
    //
    //  @par
    //              デフォルトの値は、Debug ビルドでは false、
    //              Release ビルドでは true です。
    //*/
    //---------------------------------------------------------------------
    static void setEnableReleaseMode( bool flag_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      メッセージボックス出力時のウィンドウ最小化の有効設定
    //*/
    //---------------------------------------------------------------------
    static void setEnableMinimize( bool flag_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      エラーを保持するかの設定 (デフォルトで false。true の場合は reset() でクリアしないと新しいエラーが出力されない)
    //*/
    //---------------------------------------------------------------------
    static void setEnableErrorHold( bool flag_ );


    //---------------------------------------------------------------------
    ///**
    //  @brief      設定されているエラーをクリアする
    //
    //  @par
    //              クリアしない場合、setLastErrorString() が呼ばれても新しい
    //              メッセージ文字列を設定しません。		
    //              (文字列を格納するバッファは内部では静的領域に確保されるため、
    //              この関数を呼ばないとメモリリークするという事はありません。)
    //*/
    //---------------------------------------------------------------------
    static void reset();

    //---------------------------------------------------------------------
    ///**
    //  @brief      標準のエラー処理を行う
    //
    //  @param[in]  lr_ : setLastErrorString() 等でエラー文字列が設定されていない場合に使用される (未定義)
    //
    //  @par
    //              エラーが設定されている場合、それの処理を行います。<br>
    //              <br>
    //              process() を連続で呼び出しても、2回目以降は何もしません。<br>
    //              reset() を呼ぶことでもう一度エラーを設定し、呼び出すことができます。
    //*/
    //---------------------------------------------------------------------
    static void _process( LNRESULT lr_ );

    static inline void process( LNRESULT lr_ )
    {
        if ( LN_FAILED( lr_ ) ) { _process( lr_ ); }
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      追加情報付きで標準のエラー処理を行う
    //*/
    //---------------------------------------------------------------------
    static void process( LNRESULT lr_, const lnChar* filename_, const int line_, const lnChar* format_, ... );

    /// エラー発生時のコールバックインターフェイスの設定
    static void setErrorObserver( ErrorObserver* eo_ );

    /// 最後に設定された LNRESULT 値の取得
    static LNRESULT getLastResult();

private:

    /// 現在の状態から、最終的に出力する文字列を作成する
    static void _makeOutputString( lnChar* buffer_, int size_, lnChar* title_, int title_size_ );

    /// 文字列を出力する (ErrorObserver が登録されていなければ標準出力に出力する)
    static void _showErrorMessage( lnChar* text_, lnChar* caption_ );

    /// LNRESULT のエラーメッセージ文字列の取得
    static const lnChar* _getLNErrorMessage( const LNRESULT lr_ );

#if LNOTE_INCLUDED_DIRECTX_HEADER
    /// DirectX のエラーメッセージ文字列の取得
    static const lnChar* _getDxErrorMessage( const HRESULT hr_ );
#endif
};


//=============================================================================
// ■ ErrorObserver クラス
//-----------------------------------------------------------------------------
///**
//	@brief      エラー発生時のコールバックインターフェイス
//*/
//=============================================================================
class ErrorObserver
{
public:

    /// ウィンドウの非表示または最小化が必要な時に呼ばれる
    virtual void onShowWindow( bool show_ ) = 0;

    /// メッセージボックスを表示する時に呼ばれる
    virtual void onShowMessageBox( const lnChar* text_, const lnChar* title_ ) = 0;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

// マクロ内用
typedef LNote::Core::Base::Error LNErrMD;

//=============================================================================
//								end of file
//=============================================================================