//=============================================================================
//【 AudioUtil 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../File/Interface.h"
#include "AudioUtil.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Audio
{

//=============================================================================
// ■ AudioUtil クラス
//=============================================================================

#if defined(LNOTE_WINDOWS)

    //---------------------------------------------------------------------
    // ● LNWaveFormat から WAVEFORMATEX への変換
    //---------------------------------------------------------------------
    void AudioUtil::convLNWaveFormatToWAVEFORMATEX( const LNWaveFormat& ln_fmt_, WAVEFORMATEX* wav_fmt_ )
    {
        wav_fmt_->wFormatTag       = ln_fmt_.FormatTag;
        wav_fmt_->nChannels        = ln_fmt_.Channels;
        wav_fmt_->nSamplesPerSec   = ln_fmt_.SamplesPerSec;
        wav_fmt_->nAvgBytesPerSec  = ln_fmt_.AvgBytesPerSec;
        wav_fmt_->nBlockAlign      = ln_fmt_.BlockAlign;
        wav_fmt_->wBitsPerSample   = ln_fmt_.BitsPerSample;
        wav_fmt_->cbSize           = ln_fmt_.EXSize;
    }

    //---------------------------------------------------------------------
    // ● WAVEFORMATEX から LNWaveFormat への変換
    //---------------------------------------------------------------------
    void AudioUtil::convWAVEFORMATEXToLNWaveFormat( const WAVEFORMATEX& wav_fmt_, LNWaveFormat* ln_fmt_ )
    {
        ln_fmt_->FormatTag      = wav_fmt_.wFormatTag;
        ln_fmt_->Channels       = wav_fmt_.nChannels;
        ln_fmt_->SamplesPerSec  = wav_fmt_.nSamplesPerSec;
        ln_fmt_->AvgBytesPerSec = wav_fmt_.nAvgBytesPerSec;
        ln_fmt_->BlockAlign     = wav_fmt_.nBlockAlign;
        ln_fmt_->BitsPerSample  = wav_fmt_.wBitsPerSample;
        ln_fmt_->EXSize         = wav_fmt_.cbSize;
    }   

    //---------------------------------------------------------------------
	// ● WAVEFORMATEX 構造体をコンソール画面に表示
	//---------------------------------------------------------------------
    void AudioUtil::dumpWAVEFORMATEX( const WAVEFORMATEX& wav_fmt_, const char* str_ )
    {
        if ( str_ )
		{
			printf( "%s\n", str_ );
		}
		else
		{
            printf( "◆ AudioUtil::dumpWAVEFORMATEX\n" );
		}
		printf( "    フォーマットID           %hu\n", wav_fmt_.wFormatTag );
		printf( "    チャンネル数             %hu\n", wav_fmt_.nChannels );
		printf( "    サンプリングレート       %lu\n", wav_fmt_.nSamplesPerSec );
		printf( "    データ速度(Byte/sec)     %lu\n", wav_fmt_.nAvgBytesPerSec );
		printf( "    ブロックサイズ           %hu\n", wav_fmt_.nBlockAlign );
		printf( "    サンプルあたりのビット数 %hu\n", wav_fmt_.wBitsPerSample );
		printf( "    拡張部分のサイズ         %hu\n", wav_fmt_.cbSize );
    }

#endif

    //---------------------------------------------------------------------
	// ● ファイルのフォーマットチェック
	//---------------------------------------------------------------------
    LNAudioSourceFormat AudioUtil::checkFormat( File::IInStream* stream_ )
	{
        if ( !stream_ || stream_->getSize() < 4 ) { 
	return LN_AUDIOSOURCE_UNKNOWN; }

        LNAudioSourceFormat format = LN_AUDIOSOURCE_UNKNOWN;

		u8 head[ 4 ];
		stream_->read( head, 4, 4 );
		if ( memcmp( head, "RIFF", 4 ) == 0 )	// http://www.web-sky.org/program/other/wave.php 'WAVE'もしらべる
		{
			format = LN_AUDIOSOURCE_WAVE;
		}
		else if ( memcmp( head, "OggS", 4 ) == 0 )
		{
			format = LN_AUDIOSOURCE_OGG;
		}
		else if ( memcmp( head, "MThd", 4 ) == 0 )
		{
			format = LN_AUDIOSOURCE_MIDI;
		}
		else if ( memcmp( head, "ID3", 3 ) == 0 )
		{
			format = LN_AUDIOSOURCE_MP3;
		}

        if ( format == LN_AUDIOSOURCE_UNKNOWN )
        {
		    // mp3 の場合はヘッダが ID3 だったり後ろの方に TAG があったり
		    // 同期ヘッダだけあったりするので全部チェックする
		    if ( head[ 0 ] == 0xff && head[ 1 ] >> 5 == 0x07 )
		    {
			    format = LN_AUDIOSOURCE_MP3;
		    }
            else
            {
		        stream_->seek( -128, SEEK_END );
		        char data[ 3 ];
		        stream_->read( data, 3 );
		        if ( data[ 0 ] == 'T' && data[ 1 ] == 'A' && data[ 2 ] == 'G' )
		        {
			        format = LN_AUDIOSOURCE_MP3;
		        }
            }
        }
    		
		stream_->seek( 0 );

		return format;
	}

    //---------------------------------------------------------------------
	// ● 要求している type_ と AudioSouce から正しい type を返す
    //---------------------------------------------------------------------
    LNSoundPlayType AudioUtil::checkAudioPlayType( LNSoundPlayType type_, IAudioSource* audio_source_, u32 limit_size_ )
    {
        LNSoundPlayType type = type_;

		// 作成するオーディオプレイヤーの種類を決めていく
		if ( audio_source_->getSourceFormat() == LN_AUDIOSOURCE_MIDI )
		{
			type = LN_SOUNDPLAYTYPE_MIDI;
		}
		else
		{
			if ( type == LN_SOUNDPLAYTYPE_AUTO )
			{
				if ( audio_source_->getOnmemoryPCMBufferSize() > limit_size_ )
				{
					type = LN_SOUNDPLAYTYPE_STREAMING;
				}
				else
				{
					type = LN_SOUNDPLAYTYPE_ONMEMORY;
				}
			}
		}

        return type;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Audio
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================