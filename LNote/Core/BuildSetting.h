//=============================================================================
// BuildSetting
//=============================================================================

#pragma once

//-----------------------------------------------------------------------------
//

// シンプルな識別子の型やユーティリティの非定義設定
//      他のライブラリと衝突しそうなマクロ、型、関数を定義しないようにする。
//      （_p、_wp）
//#define LNOTE_DISABLE_SIMPLE_TYPEDEF

// インラインアセンブラの使用設定
#define LNOTE_ENABLE_INLINE_ASM

// warning 4996 を無視する
#define LNOTE_DISABLE_WARNING_4996

// Common.h 内で include するファイルの設定
//		ReferenceObject.h 等、主要なクラスではほぼ必ず使用するもののみ include する。
//#define LNOTE_COMMON_INCLUDE_REQUIRED_FILES_ONRY

// デバッグ用メモリ管理方法の設定
//		LN_NEW の定義に CRT のメモリ管理を使用する。
//#define LNOTE_ENABLE_CRT_DEBUG


//-----------------------------------------------------------------------------
// 以下は基本的にライブラリビルド時にだけ操作するもの

// 環境依存ヘッダファイルの include 設定
//      基本的に lib のビルド時のみ有効にする。
#define LNOTE_INCLUDE_ENV_DEPEND_HEADER

// Base/tchar.h の include 設定
//      Base/tchar.h のデバッグ用。Win32 環境でのみ有効。
//      Win32 以外ではこの設定にかかわらず Base/tchar.h が include される。
#define LNOTE_INCLUDE_INTERNAL_TCHAR

// 文字コードセットを、プロジェクトの設定からではなくソースコードから指定するフラグ
//      文字コードセットを変更する時、全てのプロジェクト毎に設定し直さなくても
//      簡単に変更できるようにするため用意したもの。
#define LNOTE_SPECIFIED_CHARCODESET_MBS

// UNICODE 文字セット使用の強制
//#define LNOTE_SPECIFIED_CHARCODESET_UNICODE

// Win32 環境でスレッドの生成に AfxBeginThread() を使用する
#define LNOTE_ENABLE_WIN32_RUNTIME_THREAD

//=============================================================================
//
//=============================================================================