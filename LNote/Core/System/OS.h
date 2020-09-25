//=============================================================================
//【 OS 】
//-----------------------------------------------------------------------------
///**
//  @file       OS.h
//  @brief      OS
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{



//---------------------------------------------------------------------
///**
//  @brief      システム時刻を取得する (ミリ秒)
//
//  @par
//              Windows では OS 起動時からの経過時間、
//              X11 ではプログラム起動時からの経過時間を返します。
//*/
//---------------------------------------------------------------------
lnU32 getSystemTime();

//---------------------------------------------------------------------
///**
//  @brief      カレントディレクトリを取得する
//
//  @param[out] buf_ : 文字列を書き込むバッファのアドレス (LN_MAX_PATH 文字分のサイズがあること)
//
//  @par
//              gcc では char で取得した文字列を wchar_t に変換しています。
//              そのため、char で扱えない文字は正常に取得できない可能性があります。
//*/
//---------------------------------------------------------------------
void getCurrentDirectory( lnChar* buf_ );

//---------------------------------------------------------------------
///**
//  @brief      メッセージボックスの表示 (エラー用)
//
//  @par
//              X11 では title_ は無視され、常に Error と表示される。
//*/
//---------------------------------------------------------------------
void showMessageBox( const lnChar* message_, const lnChar* title_ );

//---------------------------------------------------------------------
///**
//  @brief      1ピクセルの物理的な1辺のサイズを取得する (mm単位)
//*/
//---------------------------------------------------------------------
float getPhysicalPixelSize();

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace System
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================