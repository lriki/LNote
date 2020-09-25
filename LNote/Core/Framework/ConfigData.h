//=============================================================================
//【 Public 】
//-----------------------------------------------------------------------------
///**
//  @file       Public.h
//  @brief      Public
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Types.h"
#include "../Geometry/DrawingShapes.h"
#include "../Audio/Interface.h"
#include "../Graphics/Common/Types.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{

//=============================================================================
// ■ ConfigData クラス
//-----------------------------------------------------------------------------
///**
//  @brief      ライブラリの初期化情報
//*/
//=============================================================================
class ConfigData
{
public:

    //-----------------------------------------------
    // 全般

    lnU32                   ClassInitializeFlag;    ///< 初期化する機能の指定 (LNClassFlag の組み合わせ)
    lnU32                   ClassUpdateFlag;        ///< 自動更新する機能の指定 (LNClassFlag の組み合わせ)
    bool                    EnableMakeLogFile;      ///< ログファイルを出力するかを示すフラグ
    const lnChar*           LogSaveDir;             ///< ログファイルの保存先ディレクトリ
    LNUpdateMode            UpdateMode;             ///< フレームワークの更新方法
    bool                    ForbidAutoFullscreen;   ///< Alt + Enter によるフルスクリーン切り替えの有効フラグ
    lnU32                   FrameRate;              ///< フレームレート
    bool                    AllInitialize;          ///< Framework::initialize() ですべての機能を初期化する
    bool                    EnableMultipleRunning;  ///< true の場合、多重起動を許可する
    bool                    EnableShowFPS;          ///< true の場合、ウィンドウのタイトルバーに現在の FPS を表示する

    //-----------------------------------------------
    // System 関係
    
    const lnChar*           WindowTitle;            ///< ウィンドウタイトルの文字列
    lnU32                   WindowWidth;            ///< ウィンドウのクライアント領域の幅 (Android では無効)
    lnU32                   WindowHeight;           ///< ウィンドウのクライアント領域の高さ (Android では無効)
    void*                   WindowHandle;           ///< ユーザー定義のウィンドウハンドル (Windows のみ)
    bool                    EnableConsole;          ///< コンソールを割り当てる
    //bool                    EnableDummyWindow;      ///< (WindowHandle == NULL でもウィンドウを作成しない)

    //-----------------------------------------------
    // File 関係

    LNArchiveOpenDataList   ArchiveList;            ///< (addArchive() を使って追加すること)
    LNFileAccessMode        FileAccessMode;         ///< ファイルアクセスの優先モード

    //-----------------------------------------------
    // Audio 関係

    lnU32                   SoundCacheSize;         ///< 音声ファイルのキャッシュサイズ (ファイル数)
    LNDirectMusicUsingMode  DMInitMode;             ///< DirectMusic の初期化方法

    //-----------------------------------------------
    // Graphics 関係

    LNGraphicsAPI           GraphicsAPI;            ///< 描画に使用するグラフィックス API
    LNFontNameList          FontFilenameList;       ///< 使用するフォントファイル名のリスト (システムにインストールされていないフォントを使うとき用)
    Geometry::Size			BackbufferSize;
    bool                    EnableFPUPreserve;      ///< true の場合、DirectX の初期化で浮動小数点演算ユニットの演算精度を落とさないようにする
    bool                    EnableFullscreen;       ///< 起動時フルスクリーンの有効設定

    //-----------------------------------------------
    // Android
#if defined(LNOTE_ANDROID)
    android_app*            AndroidApp;
#endif

public:

    //-------------------------------------------------------------
    /// デフォルト値
    ConfigData()
    {
        ClassInitializeFlag     = LN_CLASS_ALL;
        ClassUpdateFlag         = LN_CLASS_ALL;
        EnableMakeLogFile       = false;
        LogSaveDir              = NULL;
        UpdateMode              = LN_UPDATEMODE_THREADED;//LN_UPDATEMODE_ALWAYS;//
        ForbidAutoFullscreen    = true;
        FrameRate               = 60;
        AllInitialize           = true;
        EnableMultipleRunning   = false;
        EnableShowFPS           = false;

        WindowTitle             = LNOTE_NAME " " LNOTE_VERSION_STR;
        WindowWidth             = 640;
        WindowHeight            = 480;
        WindowHandle            = NULL;
        EnableConsole           = false;
        //EnableDummyWindow       = false;

        FileAccessMode          = LN_FILEACCESS_DIRECTORY_FIRST;

        SoundCacheSize          = 32;
        DMInitMode              = LN_DMUSINGMODE_NOTUSE;

        GraphicsAPI             = LN_GRAPHICSAPI_DIRECTX9;
        BackbufferSize          = Geometry::Size( 640, 480 );
        EnableFPUPreserve       = false;
        EnableFullscreen        = false;

#ifdef LNOTE_DEBUG
        EnableMakeLogFile       = true;
        EnableConsole           = true;
        EnableShowFPS           = true;
#endif  

#if defined(LNOTE_ANDROID)
        AndroidApp              = NULL;
#endif
    }

public:

    /// アーカイブの追加
    void addArchive( const lnChar* directory_, const lnChar* filename_, const lnChar* key_ )
    {
        LNArchiveOpenData data;
        data.Directory   = directory_;
        data.ArchiveName = filename_;
        data.ArchiveKey  = key_;
        ArchiveList.push_back( data );
    }

    /// フォントファイルの追加
    void addFontFile( const lnChar* filename_ )
    {
        FontFilenameList.push_back( ln_std_tstring( filename_ ) );
    }

};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================