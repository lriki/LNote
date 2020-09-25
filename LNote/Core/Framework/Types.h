//=============================================================================
//【 Types 】
//-----------------------------------------------------------------------------
///**
//  @file       Types.h
//  @brief      Types
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------


/// 初期化または更新する機能の指定
enum LNClassFlag
{
    LN_CLASS_FRAMEWORK  = 0x0001,
    LN_CLASS_FILE       = 0x0002,
    LN_CLASS_SYSTEM     = 0x0004,
    LN_CLASS_INPUT      = 0x0008,
    LN_CLASS_PHYSICS    = 0x0010,
    LN_CLASS_GRAPHICS   = 0x0020,
    LN_CLASS_EFFECT     = 0x0040,
    LN_CLASS_SCENE      = 0x0080,
    LN_CLASS_AUDIO      = 0x0100,
    LN_CLASS_GAME       = 0x0200,
    LN_CLASS_GUI        = 0x0400,

    LN_CLASS_ALL        = 0xffff

    /*
    LN_INIT_FRAMEWORK   = 0x0001,   ///< Framework 内の機能を初期化する ( FPSController、GameTime )
    LN_INIT_FILE        = 0x0002,   ///< File モジュールを初期化する
    LN_INIT_SYSTEM      = 0x0004,   ///< System モジュールを初期化する
    LN_INIT_INPUT       = 0x0008,   ///< Input モジュールを初期化する
    LN_INIT_PHYSICS     = 0x0010,   ///< Physics モジュールを初期化する
    LN_INIT_GRAPHICS    = 0x0020,   ///< Graphice モジュールを初期化する
    LN_INIT_EFFECT      = 0x0040,   ///< Effect モジュールを初期化する
    LN_INIT_SCENE       = 0x0080,   ///< Scene モジュールを初期化する
    LN_INIT_AUDIO       = 0x0100,   ///< Audio モジュールを初期化する
    LN_INIT_GAME        = 0x0200,   ///< Game モジュールを初期化する
    LN_INIT_GUI         = 0x0400,   ///< GUI モジュールを初期化する

    LN_INIT_ALL         = 0xffff    ///< すべてのフラグの組み合わせ
    */
    /*
        LN_INIT_FRAMEWORK | LN_INIT_FILE      | LN_INIT_SYSTEM    |
        LN_INIT_INPUT     | LN_INIT_PHYSICS   | LN_INIT_GRAPHICS  | LN_INIT_EFFECT |
        LN_INIT_SCENE     | LN_INIT_AUDIO     | LN_INIT_GAME      | LN_INIT_GUI
        */
    
};

/// フレームワークの更新方法
enum LNUpdateMode
{
    LN_UPDATEMODE_ACTIVE_ONLY = 0,  ///< ゲームウィンドウがアクティブのときのみ更新する
    LN_UPDATEMODE_ALWAYS,           ///< アクティブ状態にかかわらず更新する
    LN_UPDATEMODE_THREADED          ///< OS のメッセージにかかわらず、常に更新する (タイトルバーをドラッグ中も更新)
};

/// デフォルトでセットされるイベントリスナーの優先度
enum LNDefaultELPriority
{
    LN_DEFAULT_EL_PRIORITY_GUI          = 100,
    LN_DEFAULT_EL_PRIORITY_INPUT        = -1000,
    LN_DEFAULT_EL_PRIORITY_FRAMEWORK    = -1001
};

/// デフォルトのタスク実行フェーズ
enum LNDefaultTaskPhase
{
    LN_TASKPHASE_UPDATE_RENDERING_CONTENT = 5,  ///< 描画情報の構築
    LN_TASKPHASE_MASTER_RENDER = 5,             ///< メインの描画タスク (他の 5 のフェーズで同期をとる)
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Core

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================