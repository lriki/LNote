//=============================================================================
//【 LConfig 】
//-----------------------------------------------------------------------------
///**
//  @file       LConfig.h
//  @brief      LConfig
//  @version    1.0
//  @date       2011/09/10
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
//#include "../../Kernel/FileIO/ArchiveMethod.h"
#include "../../Core/Framework/Framework.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

// FileIO のほうに移動しておく
//typedef LNote::FileIO::DecodeArgs		LNDecodeArgs;
//typedef LNote::FileIO::Decoder		LNDecoder;

//=============================================================================
// ■ LConfig クラス
//-----------------------------------------------------------------------------
///**
//  @brief      LightNote の初期化データを扱うクラス
//*/
//=============================================================================
class LConfig
{
public:

    //-----------------------------------------------------------------
    // ▼ Framework

    //-----------------------------------------------------------------
	///**
    //  @brief      ログファイルの出力設定【 初期値 : Debug = true / Release = false 】
    //*/	
	//-----------------------------------------------------------------
    void setEnableLogFile( bool flag_ )             { _c->EnableMakeLogFile = flag_; }

    //-----------------------------------------------------------------
	///**
    //  @brief      使用する機能の指定【 LN_USECOMP_ALL 】
    //
    //  @par
    //              グラフィックの機能だけ、入力の機能だけ、といったように
    //              使用する機能を個別に指定する時に使います。
    //*/	
	//-----------------------------------------------------------------
    //void setUseComponentFlags( u32 flags_ )         { _c->setUseComponentFlags( flags_ ); }

    //---------------------------------------------------------------------
	///**
    //  @brief      Windows メッセージの処理方法の設定【 初期値 : LN_SYSPROCMODE_ACTIVE_ONRY 】
    //
    //  @param[in]  mode_ : 処理方法
    //
    //  @par
    //              メッセージ処理とゲームループの呼び出し方法を設定します。<br><br>
    //
    //              通常はデフォルトの値で問題ないと思いますが、
    //              ネットワークゲームでは LN_SYSPROCMODE_USING_THREAD を設定する方が無難です。
    //
    //  @attention
    //              LN_SYSPROCMODE_USING_THREAD のテストがまだ終わってません。
    //              うまく動かない可能性大。
    //*/
    //---------------------------------------------------------------------
    //void setProcessMode( LNSystemProcMode mode_ )   { _c->SystemProcMode = mode_; }

    //-----------------------------------------------------------------
	///**
    //  @brief      多重起動の有効設定【 初期値 : false 】
    //
    //  @param[in]  flag_ : true の場合、多重起動を許可する
    //*/	
	//-----------------------------------------------------------------
    void setEnableMultipleRunning( bool flag_ )     { _c->EnableMultipleRunning = flag_; }

    //-----------------------------------------------------------------
	///**
    //  @brief      システムウィンドウのウィンドウクラス名の設定【 初期値 : "_lnote_" 】
    //
    //  @param[in]  name_ : ウィンドウクラス名
    //*/	
	//-----------------------------------------------------------------
    //void setWindowClassName( const lnChar* name_ )   { _c->WindowClassName = name_; }

    //-----------------------------------------------------------------
	///**
    //  @brief      ウィンドウタイトル文字列の設定【 初期値 : "LightNote" 】
    //
    //  @param[in]  title_text_ : タイトル名
    //*/	
	//-----------------------------------------------------------------
    void setWindowTitle( const lnChar* title_text_ )   { _c->WindowTitle = title_text_; }

    //-----------------------------------------------------------------
	///**
    //  @brief      ウィンドウ(バックバッファ)サイズの設定【 初期値 : (640, 480) 】
    //
    //  @param[in]  width_  : 幅 (ピクセル単位)
    //  @param[in]  height_ : 高さ (ピクセル単位)
    //*/	
	//-----------------------------------------------------------------
    void setWindowSize( u32 width_, u32 height_ )   { _c->WindowWidth = width_; _c->WindowHeight = height_; }

    //-----------------------------------------------------------------
	///**
    //  @brief      ウィンドウのタイトルバーに現在の FPS を表示するかの設定【 初期値 : Debug = true / Release = false 】
    //*/	
	//-----------------------------------------------------------------
    void setEnableShowFPS( bool flag_ )         { _c->EnableShowFPS = flag_; }


    //-----------------------------------------------------------------
    // ▼ FileIO
  
	//-----------------------------------------------------------------
	///**
    //  @brief      アーカイブファイルを追加する
    //*/	
	//-----------------------------------------------------------------
	//void addArchive( const char* filename_, const char* key_, LNDecoder decoder_ = NULL );

    //-----------------------------------------------------------------
    // ▼ System



    //---------------------------------------------------------------------
	///**
    //  @brief      コンソールウィンドウの有効設定
    //
    //  @param[in]  flag_ : true の場合、コンソールを割り当てる【 初期値 : Debug = true / Release = false 】
    //
    //  @par
    //              標準出力としてコンソールウィンドウを作成し、表示します。
    //              以降、printf() 等でデータを出力することができます。         <br>
    //              変数の値を調べたいとき等に活用してください。                <br>
    //              <br>
    //              このフラグはプロジェクトを Win32 アプリケーション として
    //              作成した場合にのみ有効です。                                <br>
    //              コンソールアプリケーションとして作成した場合は常に表示されます。
    //              <br>
    //              このフラグによってコンソールを割り当てた場合、LightNote の終了処理の最後で
    //              「Enter キーを押すと終了します...」という文字を出力します。 <br>
    //              Enter キーを押して終了してください。
    //
    //  @attention	Win32 アプリケーション として作成し、このフラグによって
    //              コンソールを割り当てた場合、「Enter キーを押すと終了します...」
    //              という文字が出力されているときはまだプログラムは終了していません。   <br>
    //              このため、グローバル、静的領域に置かれたインスタンスの
    //              デストラクタはまだ呼ばれていないので注意してください。   <br>
    //              <br>
    //              コンソールアプリケーションとして起動した場合、この問題はありません。
    //*/
    //---------------------------------------------------------------------
    void setEnableConsole( bool flag_ )             { _c->EnableConsole = flag_; }

  
    //-----------------------------------------------------------------
    // ▼ Graphics

	//-----------------------------------------------------------------
	///**
    //  @brief      FPU (浮動小数点演算ユニット) の演算精度を落とさないようにする【 false 】
    //
    //  @param[in]  flag_ : true = 精度を落とさない  false = 精度を落として Direct3D のパフォーマンスを上げる
    //
    //  @par
    //              Direct3D の初期化に D3DCREATE_FPU_PRESERVE フラグを
    //              使うかどうかの設定です。<br>
    //              スクリプト言語の Lua を使う場合など、プログラムの動作が
    //              演算精度によって影響を受けてしまう場合に使います。
    //*/	
	//-----------------------------------------------------------------
    void setEnableFPUPreserve( bool flag_ )      { _c->EnableFPUPreserve = flag_; }

    //-----------------------------------------------------------------
	///**
    //  @brief      フルスクリーンの有効設定【 false 】
    //
    //  @param[in]  flag_ : true = フルスクリーン  false = ウィンドウモード
    //
    //  @par
    //              フルスクリーンの状態でスタートするかの設定です。<br>
    //              <br>
    //              フルスクリーンモードのとき、ゲーム画面とディスプレイの
    //              アスペクト比が一致しない場合は画面の上下または左右に
    //              レターボックス (黒帯) が追加されます。
    //*/	
	//-----------------------------------------------------------------
    void setEnableFullscreen( bool flag_ )          { _c->EnableFullscreen = flag_; }

    //-----------------------------------------------------------------
	///**
    //  @brief      Alt + Enter でのフルスクリーン切り替えの有効設定【 true 】
    //
    //  @param[in]  flag_ : true = 切り替えを許可しない  false = 切り替えを許可する
    //
    //  @par
    //              ウィンドウがアクティブのときに Alt + Enter キーを押すと
    //              スクリーンモードを切り替えることができますが、
    //              ツールを作成する時など、この機能を切りたい場合に使います。<br>
    //              <br>
    //              Alt + Enter が無効になるだけで、setEnableFullscreen() で
    //              フルスクリーンを有効にする場合や、Framework::setEnableFullScreen() 等の
    //              関数を呼んで切り替える場合には影響しません。
    //*/	
	//-----------------------------------------------------------------
    void setForbidAutoFullscreen( bool flag_ )      { _c->ForbidAutoFullscreen = flag_; }

    //-----------------------------------------------------------------
	///**
    //  @brief      テクスチャのキャッシュサイズの設定【 128 】
    //
    //  @param[in]  num_ : キャッシュできるテクスチャの個数
    //*/	
	//-----------------------------------------------------------------
    //void setTextureCacheSize( u32 num_ )            { _c->TextureCacheSize = num_; }

    //-----------------------------------------------------------------
	///**
    //  @brief      シェーダプログラムのキャッシュサイズの設定【 128 】
    //
    //  @param[in]  num_ : キャッシュできるシェーダプログラムの個数
    //*/	
	//-----------------------------------------------------------------
    //void setShaderCacheSize( u32 num_ )             { _c->ShaderCacheSize = num_; }


    //-----------------------------------------------------------------
    // ▼ Audio

	//-----------------------------------------------------------------
	///**
    //  @brief      DirectMusic の初期化方法の設定
    //
    //  @param[in]  mode_ : 初期化方法【 初期値 : LN_DMINITMODE_NOTUSE (使用しない) 】
    //*/	
	//-----------------------------------------------------------------
    void setDirectMusicInitMode( LNDirectMusicUsingMode mode_ )  { _c->DMInitMode = mode_; }

public:

    /// lnConfig() 内でこの変数に true をセットすると、LightNote の初期化をせずにプログラムを終了します。lnMain() も呼ばれません。
    bool   Failed;


private:

    Core::ConfigData* _c;
    //Framework::InitData* _c;

public:

    LConfig( Core::ConfigData* data_ );
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================