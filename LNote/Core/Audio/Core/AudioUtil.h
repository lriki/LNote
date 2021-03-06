//=============================================================================
//【 AudioUtil 】
//-----------------------------------------------------------------------------
///**
//  @file       AudioUtil.h
//  @brief      AudioUtil
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File { class IInStream; }

namespace Audio
{
class IAudioSource;

//=============================================================================
// ■ AudioUtil クラス
//-----------------------------------------------------------------------------
///**
//  @brief      音声ユーティリティ
//*/
//=============================================================================
class AudioUtil
{
public:

#if defined(LNOTE_WINDOWS)

    //---------------------------------------------------------------------
    ///**
    //  @brief      LNWaveFormat から WAVEFORMATEX への変換
    //*/
    //---------------------------------------------------------------------
    static void convLNWaveFormatToWAVEFORMATEX( const LNWaveFormat& ln_fmt_, WAVEFORMATEX* wav_fmt_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      WAVEFORMATEX から LNWaveFormat への変換
    //*/
    //---------------------------------------------------------------------
    static void convWAVEFORMATEXToLNWaveFormat( const WAVEFORMATEX& wav_fmt_, LNWaveFormat* ln_fmt_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      WAVEFORMATEX 構造体をコンソール画面に表示
    //
    //  @param[in]  stream_ : WAVEFORMATEX
    //	@param[in]	str_    : ヘッダ文字列
    //*/
    //---------------------------------------------------------------------
    static void dumpWAVEFORMATEX( const WAVEFORMATEX& wav_fmt_, const char* str_ = NULL );

#endif

    //---------------------------------------------------------------------
    ///**
    //  @brief      フォーマットチェック
    //
    //  @param[in]  stream_ : 入力ストリーム
    //
    //  @par
    //              扱える音声ファイルかどうかを調べて、その種類を返します。<br>
    //              調べた後、ファイルポインタはファイルの先頭に戻ります。
    //*/
    //---------------------------------------------------------------------
    static LNAudioSourceFormat checkFormat( File::IInStream* stream_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      要求している type_ と IAudioSouce から正しい type を返す
    //
    //  @param[in]  type_       : 要求再生方法
    //  @param[in]  source_     : チェックに使うオーディオソース
    //  @param[in]  limit_size_ : オンメモリとストリーミングの境界サイズ
    //
    //  @par
    //              LN_SOUNDPLAYTYPE_AUTO が要求された場合、オーディオソースのサイズが
    //              limit_size_ のサイズを超えると LN_SOUNDPLAYTYPE_STREAMING を返します。
    //*/
    //---------------------------------------------------------------------
    static LNSoundPlayType checkAudioPlayType( LNSoundPlayType type_, IAudioSource* audio_source_, u32 limit_size_ );
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Audio
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================