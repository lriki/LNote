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

//-------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------
namespace LNote
{
struct LNWaveFormat
{
    lnU16       FormatTag;
    lnU16       Channels;
    lnU32       SamplesPerSec;
    lnU32       AvgBytesPerSec;
    lnU16       BlockAlign;
    lnU16       BitsPerSample;
    lnU16       EXSize;
};

/// 音声ファイルフォーマット
enum LNAudioSourceFormat
{
	LN_AUDIOSOURCE_UNKNOWN = 0,		///< 不明なファイル
	LN_AUDIOSOURCE_WAVE,			///< WAVE
	LN_AUDIOSOURCE_MP3,				///< MP3
	LN_AUDIOSOURCE_OGG,				///< OGG
	LN_AUDIOSOURCE_MIDI				///< MIDI
};

/// 再生方法 ( オーディオプレイヤーの種類 )
enum LNSoundPlayType
{
	LN_SOUNDPLAYTYPE_AUTO = 0,		///< 自動選択 ( デフォルトではデコード後のサイズが 10000 バイト以上になる場合はストリーミング、そうでない場合はオンメモリ再生になります )
	LN_SOUNDPLAYTYPE_ONMEMORY,		///< オンメモリ
	LN_SOUNDPLAYTYPE_STREAMING,		///< ストリーミング
	LN_SOUNDPLAYTYPE_MIDI,  		///< SMF

    LN_SOUNDPLAYTYPE_UNKNOWN = 0xffff,	///< 不明な再生方法 ( 後で自動選択とまとめておいてもいいかも？ )
};

/// DirectMusic の初期化方法
enum LNDirectMusicUsingMode
{
    LN_DMUSINGMODE_NOTUSE = 0,       ///< DirectMusic を使用しない ( LConfig から設定するために用意 )

	LN_DMUSINGMODE_USE,		        ///< 通常
	LN_DMUSINGMODE_THREAD_WAIT,		///< 別スレッドで初期化して、再生時に未完了の場合は待つ
	LN_DMUSINGMODE_THREAD_REQUEST,	///< 別スレッドで初期化して、再生時に未完了の場合は再生を予約する
};

/// 音声の再生状態
enum LNSoundPlayState
{
    LN_SOUNDPLAYSTATE_STOP = 0,     ///< 停止中
    LN_SOUNDPLAYSTATE_PLAYING,      ///< 再生中
    LN_SOUNDPLAYSTATE_PAUSING       ///< 一時停止中
};

/// 音量フェード完了時の動作の記述
enum LNSoundFadeState
{
    LN_SOUNDFADE_CONTINUE = 0,	    ///< 継続(なにもしない)
	LN_SOUNDFADE_STOP,		        ///< 停止する (stop())
	LN_SOUNDFADE_STOP_RESET,	    ///< 停止して、音量を元の値に戻す
	LN_SOUNDFADE_PAUSE,			    ///< 一時停止する (pause( true ))
	LN_SOUNDFADE_PAUSE_RESET,	    ///< 一時停止して、音量を元の値に戻す
};

/// グループの記述
enum LNAudioPlayerGroupFlag
{
	LN_AUDIOPLAYER_GROUP_BGM        = 0x0002,	///< BGM グループに属するフラグ
	LN_AUDIOPLAYER_GROUP_BGS        = 0x0004,	///< BGS グループに属するフラグ
	LN_AUDIOPLAYER_GROUP_SE         = 0x0008,	///< SE  グループに属するフラグ
    LN_AUDIOPLAYER_GROUP_1          = 0x0010,   ///< グループ 1 に属するフラグ
    LN_AUDIOPLAYER_GROUP_2          = 0x0020,   ///< グループ 2 に属するフラグ
    LN_AUDIOPLAYER_GROUP_3          = 0x0040,   ///< グループ 3 に属するフラグ
    LN_AUDIOPLAYER_GROUP_4          = 0x0080,   ///< グループ 4 に属するフラグ
};

/// 音声の再生状態
struct LNAudioPlayState
{

    u32                 TotalUnits;     ///< 全サンプル数 ( Midi の場合はミュージックタイム単位 ( 四分音符ひとつ分が 768 ) また、Midi の場合は1度でも play() で再生していないと取得できない )
    u32                 CurrentUnits;   ///< 現在の再生サンプル数
    lnU32               SamplesPerSec;  ///< サンプリング周波数 (MIDI の場合は LN_MUSICTIMEBASE)
    LNSoundPlayState    SoundPlayState;
};

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================