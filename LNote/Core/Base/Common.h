//=============================================================================
//【 Common 】
//-----------------------------------------------------------------------------
///**
//  @file       Common.h
//  @brief      Common
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

/// 結果・エラーコード
enum LNResultCode
{
    LN_OK                   = 0,
    LN_FALSE                = 1,
    LN_HANDLED              = 0x00042002,    ///< 処理済み (OSメッセージ・GUI 用。削除予定)

    LN_ERR_ASSERT           = 0x80042001,   ///< 警告 (LN_ASSERT)
    LN_ERR_INVALIDCALL      = 0x80042002,   ///< 関数の引数が無効
    LN_ERR_OUTOFMEMORY      = 0x80042003,   ///< メモリ不足
    LN_ERR_INDEX            = LN_ERR_INVALIDCALL,   ///< 配列外参照 (LN_ERR_INVALIDCALL に統合予定)
    LN_ERR_NULLPOINTER      = 0x80042005,   ///< NULL アクセス
    LN_ERR_FILENOTFOUND     = 0x80042006,   ///< ファイルが無効
    LN_ERR_SYSTEM           = 0x80042007,   ///< システム API 呼び出しエラー
    LN_ERR_INITIALIZED      = 0x80042009,   ///< 初期化済み
    LN_ERR_NOINITIALIZE     = 0x8004200a,   ///< 初期化されていない
    LN_ERR_FORMAT           = 0x8004200b,   ///< 非対応ファイル形式
    LN_ERR_NOTIMPL          = 0x8004200c,   ///< 実装されていない機能を呼び出そうとした
    LN_ERR_FATAL            = 0x8004200d,   ///< 継続不能なエラー
    LN_ERR_DIRECTX          = 0x8004200e,   ///< DirectX 関係のエラー
    LN_ERR_OPENGL           = 0x8004200f,   ///< OpenGL 関係のエラー

    LN_ERR_COM  = LN_ERR_DIRECTX,

    // ※コンバータでの変換を容易にするため、MAKE_HRESULT ではなく直値で指定
};

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================