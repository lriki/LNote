//=============================================================================
//【 AudioSource 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../File/FileUtil.h"
#include "AudioUtil.h"
#include "AudioResourceManager.h"
#include "AudioSource.h"

#ifndef WAVE_FORMAT_PCM
    #define WAVE_FORMAT_PCM     1
#endif

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
// ■ AudioSourceBase クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    AudioSourceBase::AudioSourceBase( ResourceManager* manager_ )
        : mManager  ( manager_ )
    {
        LN_SAFE_ADDREF( mManager );
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    AudioSourceBase::~AudioSourceBase()
    {
        LN_SAFE_RELEASE( mManager );
    }


//=============================================================================
// ■ Wave クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	Wave::Wave( ResourceManager* manager_ )
        : AudioSourceBase           ( manager_ )
        , mInFile                   ( NULL )
        , mSourceDataSize			( 0 )
		, mDataOffset				( 0 )
		, mPCMDataSize				( 0 )
		, mTotalTime				( 0 )
		, mOnmemoryPCMBuffer		( NULL )
		, mOnmemoryPCMBufferSize	( 0 )
        , mTotalSamples             ( 0 )
	{
        memset( &mWaveFormat, 0, sizeof( mWaveFormat ) );
    }

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	Wave::~Wave()
	{
		SAFE_DELETE_ARRAY( mOnmemoryPCMBuffer );
        LN_SAFE_RELEASE( mInFile );
	}

	//---------------------------------------------------------------------
	// ● オーディオファイルとして扱うストリームを設定する
	//---------------------------------------------------------------------
    LNRESULT Wave::setStream( File::IInStream* stream_ )
	{
		mInFile = stream_;
        mInFile->addRef();

		// 念のためファイルポインタを先頭に戻す
		mInFile->seek( 0 );

		// ヘッダ読み込んで念のためフォーマットチェック
		WaveFileHeader rh;
		mInFile->read( &rh, sizeof( WaveFileHeader ) );
		if ( memcmp( &rh.RIFF, "RIFF", 4 ) != 0 ||
			 memcmp( &rh.WaveHeader, "WAVE", 4 ) != 0 ||
			 memcmp( &rh.FmtChunk, "fmt ", 4 ) != 0 )
		{
		    LN_THROW_FileFormat( 0 );
		}


        mWaveFormat.FormatTag		= File::readU16( mInFile );
		mWaveFormat.Channels		= File::readU16( mInFile );
		mWaveFormat.SamplesPerSec	= File::readU32( mInFile );
		mWaveFormat.AvgBytesPerSec	= File::readU32( mInFile );
		mWaveFormat.BlockAlign		= File::readU16( mInFile );
		mWaveFormat.BitsPerSample	= File::readU16( mInFile );

		// 拡張部分のあるファイルの場合
		if ( mWaveFormat.FormatTag != WAVE_FORMAT_PCM )
		{
			mWaveFormat.EXSize = File::readU16( mInFile );
			// 拡張部分読み込み
			//...
            //mInFile->seek( mWaveFormat.cbSize, SEEK_CUR );

            LN_THROW_FileFormat( 0, "Wave file extensions are not supported.\nmWaveFormat.FormatTag : %d", mWaveFormat.FormatTag );
		}
		else
		{
			mWaveFormat.EXSize = 0;
		}
		
		u8 chunk[ 4 ];
		u32 len;

		// 'data' チャンクを探す
		// ( 拡張部分関係なく、とりあえずデータを読み進めて data を探す )
		while ( mInFile->read( chunk, 4 ) == 4 )
		{
			// データサイズ取得 ( data チャンクの次の 4 バイトがデータサイズ )
			len = File::readU32( mInFile );

			// 'data' チャンクが見つかっている場合は抜ける
			if ( memcmp( chunk, "data", 4 ) == 0 ) { 
				break; }

            if ( mInFile->getPosition() == mInFile->getSize() )
			{
			    LN_THROW_FileFormat( 0, "not found 'data' chunk." );
            }

			mInFile->seek( len, SEEK_CUR );
		}

		// ファイルポインタの現在位置 ( data チャンク内のデータ位置 ) を記憶
        mDataOffset = mInFile->getPosition();

		// 元データのサイズは data チャンク内のデータのサイズ
		mOnmemoryPCMBufferSize = mPCMDataSize = len;

		mSourceDataSize = mOnmemoryPCMBufferSize;


		// 全体の再生時間を計算する
		double t = static_cast< double >( mPCMDataSize ) / ( static_cast< double >( mWaveFormat.AvgBytesPerSec ) * 0.001 );
		mTotalTime = static_cast< u32 >( t );

        // 全体の再生サンプル数
        mTotalSamples = mOnmemoryPCMBufferSize / ( mWaveFormat.BitsPerSample / 8 ) / mWaveFormat.Channels; 

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● オンメモリ再生用に全てのデータを読み込む
	//---------------------------------------------------------------------
	LNRESULT Wave::fillBufferAndReleaseStream()
	{
		if ( mOnmemoryPCMBuffer == NULL )
		{
			// オンメモリ再生するときに必要なバッファのサイズは元データのサイズと同じ
			mOnmemoryPCMBufferSize = mPCMDataSize;

			// メモリ確保
			mOnmemoryPCMBuffer = LN_NEW u8[ mOnmemoryPCMBufferSize ];

			// 現在のシーク位置を覚えておく
            int old_seek = mInFile->getPosition();

			// ファイルポインタをデータがある場所の先頭にセット
			mInFile->seek( mDataOffset );

			// 全部読み込み
			int size = mInFile->read( mOnmemoryPCMBuffer, mOnmemoryPCMBufferSize );

			// 読み込んだサイズが変な場合はエラー
		    LN_THROW_InvalidCall( ( size == mOnmemoryPCMBufferSize ), "read file size is incorrect.\nThere is a possibility that the file is corrupted." );
		   
			// シーク位置を元に戻す
			mInFile->seek( old_seek );

            LN_SAFE_RELEASE( mInFile );
		}
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● データを buffer_ へ読み込む
	//---------------------------------------------------------------------
	LNRESULT Wave::read( void* buffer_, u32 buffer_size_, u32* read_size_, u32* write_size_ )
	{
        if ( !buffer_ || !( buffer_size_ > 0 ) || !( read_size_ ) || !( write_size_ ) ) { 
	return LN_ERR_NULLPOINTER; }
		//LN_SETERR_R_S_( ( buffer_ ),      LN_ERR_ASSERT, "< Audio::Wave::read >\n 'buffer_' is NULL" );
		//LN_SETERR_R_S_( ( buffer_size_ > 0 ),    LN_ERR_ASSERT, "< Audio::Wave::read >\n 'buffer_size_' is 0" );
		//LN_SETERR_R_S_( ( read_size_ ),   LN_ERR_ASSERT, "< Audio::Wave::read >\n 'read_size_' is NULL" );
		//LN_SETERR_R_S_( ( write_size_ ),  LN_ERR_ASSERT, "< Audio::Wave::read >\n 'write_size_' is NULL" );

		// 読み込むサイズ
		u32 read_size = buffer_size_;
		// ソースのサイズを超えている場合はソースサイズ分読む
		if ( buffer_size_ > mPCMDataSize )
		{
			read_size = mPCMDataSize;
		}

		int size = mInFile->read( buffer_, read_size );

		// 元データから読み込んだサイズと、buffer_ へ書き込んだサイズは同じ
		*read_size_ = static_cast< u32 >( size );
		*write_size_ = static_cast< u32 >( size );

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● ファイルポインタ移動
	//---------------------------------------------------------------------
	LNRESULT Wave::seek( u32 offset_ )
	{
		if ( offset_ < mPCMDataSize )
		{
			mInFile->seek( mDataOffset + offset_ );
		}
        return LN_OK;
	}



//=============================================================================
// ■ Audio::Source::MP3 クラス
//=============================================================================
#if defined(LNOTE_MSVC)

	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	MP3::MP3( ResourceManager* manager_ )
        : AudioSourceBase               ( manager_ )
        , mInFile                       ( NULL )
        , mMP3WaveFormat				( NULL )
		, mACMStreamHandle				( NULL )
		, mSourceDataSize				( 0 )
		, mDataOffset					( 0 )
		, mID3vTagFieldSize				( 0 )
		, mOnmemoryPCMBuffer			( NULL )
		, mOnmemoryPCMBufferSize		( 0 )
		, mStreamingPCMBufferSize		( 0 )
		, mMP3SourceBufferParSec		( NULL )
		, mMP3SourceBufferSizeParSec	( 0 )
		, mTotalTime					( 0 )
        , mTotalSamples                 ( 0 )
		, mResetFlag					( true )
	{
		mMP3WaveFormat = LN_NEW MPEGLAYER3WAVEFORMAT;
		mACMStreamHandle = LN_NEW HACMSTREAM;
		HACMSTREAM* acm = (HACMSTREAM*)mACMStreamHandle;
		*acm = 0;
	}

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	MP3::~MP3()
	{
		HACMSTREAM* acm = (HACMSTREAM*)mACMStreamHandle;
		if ( *acm )
		{
			acmStreamClose( *acm, 0 );
		}

		SAFE_DELETE( mMP3WaveFormat );
		SAFE_DELETE( mACMStreamHandle );
		SAFE_DELETE_ARRAY( mMP3SourceBufferParSec );
		SAFE_DELETE_ARRAY( mOnmemoryPCMBuffer );
        LN_SAFE_RELEASE( mInFile );

        //// キャッシュに追加されていない場合は Manager への参照を持っているので解放
        //if ( !isCaching() )
        //{
        //    LN_SAFE_RELEASE( mManager );
        //}
	}

	//---------------------------------------------------------------------
	// ● オーディオファイルとして扱うストリームを設定する
	//---------------------------------------------------------------------
	LNRESULT MP3::setStream( File::IInStream* stream_ )
	{
        LN_SETERR_R_S_( ( !mInFile ), LN_ERR_ASSERT, "< Audio::MP3::setStream >\n複数のストリームを設定しようとしました。" );

        LNRESULT lr;

		mInFile = stream_;
        mInFile->addRef();

		// ファイルポインタを先頭に戻す
		mInFile->seek( 0 );

		mDataOffset = 0;

		// ID3v 確認
        LN_CALL_R( _checkId3v() );
		
		// PCM フォーマット取得
        LN_CALL_R( _getPCMFormat() );

		// mp3 を PCM にデコードした時の wave フォーマットを取得
		MPEGLAYER3WAVEFORMAT* mp3_format = mMP3WaveFormat;
        WAVEFORMATEX wav_fmt_ex;
        wav_fmt_ex.wFormatTag = WAVE_FORMAT_PCM;
		acmFormatSuggest( NULL, &mp3_format->wfx, &wav_fmt_ex, sizeof( WAVEFORMATEX ), ACM_FORMATSUGGESTF_WFORMATTAG );
        
		// ACM 変換ストリームを開く ( mp3 → wave )
		HACMSTREAM* acm = mACMStreamHandle;//(HACMSTREAM*)&mACMStreamHandle;
		acmStreamOpen( acm, NULL, &mp3_format->wfx, &wav_fmt_ex, NULL, 0, 0, 0 );

        AudioUtil::convWAVEFORMATEXToLNWaveFormat( wav_fmt_ex, &mWaveFormat );

		// 全体を変換した時の PCM サイズを mOnmemoryPCMBufferSize に格納
        DWORD pcm_size = 0;
		acmStreamSize( *acm, mSourceDataSize, &pcm_size, ACM_STREAMSIZEF_SOURCE );
        mOnmemoryPCMBufferSize = pcm_size;

		// 1 秒分の mp3 データを変換した時の、最適な転送先 PCM バッファサイズを取得する
		DWORD wave_buffer_size;
		acmStreamSize( *acm, mp3_format->wfx.nAvgBytesPerSec, &wave_buffer_size, ACM_STREAMSIZEF_SOURCE );
		mStreamingPCMBufferSize = wave_buffer_size;

		//HACMSTREAM acm2;
		//MMRESULT m = acmStreamOpen( &acm2, NULL, &mp3_format->wfx, &mWaveFormat, NULL, 0, 0, 0 );
		//LASSERT_STRONG_RETURN( (m!=0), false, "acmStreamOpen"  );
		//acmStreamSize( *acm, mMP3BufferSize, &mMP3BufferSize, ACM_STREAMSIZEF_SOURCE );
		//acmStreamClose( acm2, 0 );


		// ストリーミング再生用のバッファのサイズを取得 ( mp3_format->wfx.nAvgBytesPerSec は仮のサイズ )
		mMP3SourceBufferSizeParSec = mp3_format->wfx.nAvgBytesPerSec;

		// 1 秒分の mp3 データのサイズ分、メモリを確保
		mMP3SourceBufferParSec = LN_NEW u8[ mMP3SourceBufferSizeParSec ];

		// 全体の再生時間を計算する
		double t = static_cast< double >( mOnmemoryPCMBufferSize ) / ( static_cast< double >( mWaveFormat.AvgBytesPerSec ) * 0.001 );
		mTotalTime = static_cast< u32 >( t );

        // 全体の再生サンプル数
        mTotalSamples = mOnmemoryPCMBufferSize / ( mWaveFormat.BitsPerSample / 8 ) / mWaveFormat.Channels;

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● オンメモリ再生用に全てのデータを読み込む
	//---------------------------------------------------------------------
	LNRESULT MP3::fillBufferAndReleaseStream()
	{
		if ( mOnmemoryPCMBuffer == NULL )
		{
			MMRESULT mr;
			HACMSTREAM* acm = (HACMSTREAM*)mACMStreamHandle;

			// mp3 データ全体を格納するバッファを作成して読み込む
			u8* mp3_buffer = (u8*)malloc( mSourceDataSize );
			mInFile->seek( mDataOffset, SEEK_SET );
			mInFile->read( mp3_buffer, mSourceDataSize );

			// 全体を変換した時の PCM サイズを mPCMSize に格納
            DWORD pcm_size = 0;
			acmStreamSize( *acm, mSourceDataSize, &pcm_size, ACM_STREAMSIZEF_SOURCE );
            mOnmemoryPCMBufferSize = pcm_size;

			// 取得したサイズでバッファ確保
			mOnmemoryPCMBuffer = LN_NEW u8[ mOnmemoryPCMBufferSize ];

			// ACM ヘッダに変換バッファ設定
			ACMSTREAMHEADER ash;
			ZeroMemory( &ash, sizeof( ACMSTREAMHEADER ) );
			ash.cbStruct    = sizeof( ACMSTREAMHEADER );
			ash.pbSrc       = mp3_buffer;
			ash.cbSrcLength = mSourceDataSize;
			ash.pbDst       = (LPBYTE)mOnmemoryPCMBuffer;
			ash.cbDstLength = mOnmemoryPCMBufferSize;

			// コンバート実行
			mr = acmStreamPrepareHeader( *acm, &ash, 0 );
            LN_SETERR_R_S_( ( mr == 0 ), LN_ERR_SYSTEM, "< Audio::MP3::fillBufferAndReleaseStream >\n::acmStreamPrepareHeader" );
			mr = acmStreamConvert( *acm, &ash, 0 );
			LN_SETERR_R_S_( ( mr == 0 ), LN_ERR_SYSTEM, "< Audio::MP3::fillBufferAndReleaseStream >\n::acmStreamConvert" );
			mr = acmStreamUnprepareHeader( *acm, &ash, 0 );
			LN_SETERR_R_S_( ( mr == 0 ), LN_ERR_SYSTEM, "< Audio::MP3::fillBufferAndReleaseStream >\n::acmStreamUnprepareHeader" );

			// 実際に PCM バッファに書き込んだデータサイズを記憶する
			mOnmemoryPCMBufferSize = ash.cbDstLengthUsed;

			// 一時バッファ解放
			free( mp3_buffer );
			mp3_buffer = NULL;

            LN_SAFE_RELEASE( mInFile );
		}

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● データをデコードし、buffer_ に書き込む
	//---------------------------------------------------------------------
	LNRESULT MP3::read( void* buffer_, u32 buffer_size_, u32* read_size_, u32* write_size_ )
	{
		ZeroMemory( buffer_, buffer_size_ );
		// ファイルからデータ読み込み
		u32 read_size = mInFile->read( mMP3SourceBufferParSec, mMP3SourceBufferSizeParSec, mMP3SourceBufferSizeParSec );

		DWORD src_length = mMP3SourceBufferSizeParSec;

		// 実際に読み込んだサイズが、読むべきサイズよりも小さかった場合
		if ( read_size < mMP3SourceBufferSizeParSec )
		{
			// とりあえず、読み込めたサイズ分コンバートする
			src_length = read_size;
		}

		// ACM ヘッダに変換バッファ設定
		ACMSTREAMHEADER ash;
		ZeroMemory( &ash, sizeof( ACMSTREAMHEADER ) );
		ash.cbStruct    = sizeof( ACMSTREAMHEADER );
		ash.pbSrc       = mMP3SourceBufferParSec;
		ash.cbSrcLength = src_length;
		ash.pbDst       = (LPBYTE)buffer_;
		ash.cbDstLength = buffer_size_;

		// コンバート実行
		HACMSTREAM* acm = (HACMSTREAM*)mACMStreamHandle;
		acmStreamPrepareHeader( *acm, &ash, 0 );

		DWORD acm_flag = ( mResetFlag == true ) ? ACM_STREAMCONVERTF_START : ACM_STREAMCONVERTF_BLOCKALIGN;
		acmStreamConvert( *acm, &ash, acm_flag );
		
		acmStreamUnprepareHeader( *acm, &ash, 0 );

		// コンバートした結果、実際に使った領域を返す
		*read_size_ = ash.cbSrcLengthUsed;
		*write_size_ = ash.cbDstLengthUsed;

		mResetFlag = false;

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● ファイルポインタ移動
	//---------------------------------------------------------------------
	LNRESULT MP3::seek( u32 offset_ )
	{
		mInFile->seek( mDataOffset + offset_, SEEK_SET );
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● どのフォーマットの mp3 か調べて、データまでのオフセットやタグフィールドのサイズをメンバに格納
	//---------------------------------------------------------------------
	LNRESULT MP3::_checkId3v()
	{
		// とりあえず最初に、ファイルサイズを mp3 データ全体のサイズとする
        mSourceDataSize = mInFile->getSize();

		// とりあえず ID3v2 としてヘッダ部分を読み込む
		ID3v2Header header;
		mInFile->read( &header, sizeof( ID3v2Header ) );

		// Id3v2 形式の場合
		if ( header.ID[ 0 ] == 'I' && header.ID[ 1 ] == 'D' && header.ID[ 2 ] == '3' )
		{
			// タグサイズ取得
			mID3vTagFieldSize  = ( ( header.Size[0] << 21 ) | ( header.Size[1] << 14 ) | ( header.Size[2] << 7 ) | ( header.Size[3] ) ) + 10; 
			
			// 音声データがある位置は、タグの場所の次からとする
			mDataOffset = mID3vTagFieldSize;

			// 音声データ本体のサイズは、ファイル全体のサイズからタグの分を引いたもの
			mSourceDataSize -= mID3vTagFieldSize;
		}
		// Id3v2 形式以外 ( Id3v1 ) の場合
		else
		{
			// タグなし　データがある場所はファイルの先頭から
			mID3vTagFieldSize = 0;
			mDataOffset = 0;

			// ファイル終端から 128 バイト戻ったところを調べる
			u8 data[ 3 ];
			mInFile->seek( -128, SEEK_END );
			mInFile->read( data, 3, 3 );

            //printf( "%c %c %c %c\n", data[ 0 ], data[ 1 ], data[ 2 ], data[ 3 ] );

			// 'TAG' が見つかった
			if ( data[ 0 ] == 'T' && data[ 1 ] == 'A' && data[ 2 ] == 'G' )
			{
				// mp3 データ部分のサイズは、全体からタグの分を引いたもの
				mSourceDataSize -= 128;
			}
		}
		// ファイルポインタを先頭に戻しておく
		mInFile->seek( 0 );
        
        return LN_OK;
	}


	//---------------------------------------------------------------------
	// ● mp3 の PCM フォーマットを調べてメンバに格納する
	//---------------------------------------------------------------------
	LNRESULT MP3::_getPCMFormat()
	{
		BYTE  index;
		BYTE  version;
		BYTE  channel;
		BYTE  padding;
		WORD  wBlockSize;
		DWORD dwBitRate;
		DWORD dwSampleRate;
		DWORD dwBitTableLayer3[][ 16 ] = {
			{ 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0 },
			{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0 }
		};
		DWORD dwSampleTable[][ 3 ] = {
			{ 44100, 48000, 32000 },
			{ 22050, 24000, 16000 }
		};

		// フレームヘッダ部分を読み込む
		u8 data[ 4 ];
		mInFile->seek( mID3vTagFieldSize, SEEK_SET );
		mInFile->read( data, 4 );

		// data がフレームヘッダを指しているか調べる
		if ( data[ 0 ] != 0xff || data[ 1 ] >> 5 != 0x07 )
		{
            // 先頭になければガンガン進めながら探す
            int rs;
            int ends = mInFile->getSize();
            while ( true )
            {
                rs = mInFile->read( data, 4 );
                if ( rs != 4 )
                {
                    LN_SETERR_R_S( LN_ERR_ASSERT, "< Audio::MP3::_getPCMFormat >\nmp3 ファイルのフレームヘッダが見つかりませんでした。( 1 )" );
                }
                if ( data[ 0 ] == 0xff && data[ 1 ] >> 5 == 0x07 )
                {
                    break;
                }
                mInFile->seek( -3, SEEK_CUR );
            }
		}
		
		// MP3 のバージョンは？
		switch ( data[ 1 ] >> 3 & 0x03 )
		{
			case 3:
				version = 1;
				break;

			case 2:
				version = 2;
				break;

			default:
                LN_SETERR_R_S( LN_ERR_ASSERT, "< Audio::MP3::_getPCMFormat >\nmp3 ファイルのフレームヘッダが見つかりませんでした。( 2 )" );
				break;
		}

		// レイヤー 3 ？
		if ( ( data[1] >> 1 & 0x03 ) != 1 )
		{
            LN_SETERR_R_S( LN_ERR_ASSERT, "< Audio::MP3::_getPCMFormat >\n ( ( data[1] >> 1 & 0x03 ) != 1 )" );
		}

		// テーブルで定義したビットレートのうち、当てはまるものを選ぶ
		index     = data[ 2 ] >> 4;
		dwBitRate = dwBitTableLayer3[ version - 1 ][ index ];

		
		// 同じように、サンプリングレートを選ぶ
		index        = data[ 2 ] >> 2 & 0x03;
		dwSampleRate = dwSampleTable[ version - 1 ][ index ];

		// パディングの取得
		padding = data[ 2 ] >> 1 & 0x01;

		// チャンネル数の取得
		channel = ( ( data[ 3 ] >> 6 ) == 3 ) ? 1 : 2;

		wBlockSize = (WORD)((1152 * dwBitRate * 1000 / dwSampleRate) / 8) + padding;
		
		// MPEGLAYER3WAVEFORMAT 構造体にいろいろ格納する
		MPEGLAYER3WAVEFORMAT* format = mMP3WaveFormat;//(MPEGLAYER3WAVEFORMAT*)&mMP3WaveFormat;

		format->wfx.wFormatTag      = WAVE_FORMAT_MPEGLAYER3;
		format->wfx.nChannels       = channel;
		format->wfx.nSamplesPerSec  = dwSampleRate;
		format->wfx.nAvgBytesPerSec = (dwBitRate * 1000) / 8;
		format->wfx.nBlockAlign     = 1;
		format->wfx.wBitsPerSample  = 0;
		format->wfx.cbSize          = MPEGLAYER3_WFX_EXTRA_BYTES;

		format->wID             = MPEGLAYER3_ID_MPEG;
		format->fdwFlags        = padding ? MPEGLAYER3_FLAG_PADDING_ON : MPEGLAYER3_FLAG_PADDING_OFF;
		format->nBlockSize      = wBlockSize;
		format->nFramesPerBlock = 1;
		format->nCodecDelay     = 0x571;

        return LN_OK;
	}

#endif

//=============================================================================
// ■ Ogg クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	Ogg::Ogg( ResourceManager* manager_ )
        : AudioSourceBase           ( manager_ )
        , mInFile                   ( NULL )
        , mSourceDataSize			( 0 )
		, mOnmemoryPCMBuffer		( NULL )
		, mOnmemoryPCMBufferSize	( 0 )
		, mTotalTime				( 0 )
        , mTotalSamples             ( 0 )
        , mLoopStart                ( 0 )
        , mLoopLength               ( 0 )
	{
        memset( &mWaveFormat, 0, sizeof( mWaveFormat ) );
    }

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	Ogg::~Ogg()
	{
        SAFE_DELETE_ARRAY( mOnmemoryPCMBuffer );
		ov_clear( &mOggVorbisFile );
        LN_SAFE_RELEASE( mInFile );
	}

	//---------------------------------------------------------------------
	// ● オーディオファイルとして扱うストリームを設定する
	//---------------------------------------------------------------------
	LNRESULT Ogg::setStream( File::IInStream* stream_ )
	{
	    LN_THROW_InvalidCall( ( !mInFile ) );
        //LN_SETERR_R_S_( ( !mInFile ), LN_ERR_ASSERT, "< Audio::Ogg::setStream >\n複数のストリームを設定しようとしました。" );

		mInFile = stream_;
        mInFile->addRef();

		mInFile->seek( 0 );

		// コールバック関数の配列を作成
		ov_callbacks callbacks = {
			readOggCallback,
			seekOggCallback,
			closeOggCallback,
			tellOggCallback };

		// コールバックを使って Ogg ファイルオープン
		int err = ov_open_callbacks( mInFile, &mOggVorbisFile , 0, 0, callbacks );

		// エラーが発生した場合
		if ( err != 0 )
		{
			const char* es = "";
			switch( err )
			{
				case OV_EREAD:       
					es = "OV_EREAD"; break;
				case OV_ENOTVORBIS:  
					es = "OV_ENOTVORBIS"; break;
				case OV_EVERSION:    
					es = "OV_EVERSION"; break;
				case OV_EBADHEADER:  
					es = "OV_EBADHEADER"; break;
				case OV_EFAULT:      
					es = "OV_EFAULT"; break;
			}
		    LN_THROW_FileFormat( 0, "ov_open_callbacks %s\n", es );
            //LN_SETERR_R_S( LN_ERR_ASSERT, "< Audio::Ogg::setStream >\n );
		}

		// Ogg ファイルの情報取得
		vorbis_info* ogg_info = ov_info( &mOggVorbisFile, -1 );

		int bits_per_sample = WORD_BITS;	// とりあえず 16bit 固定
		mWaveFormat.FormatTag		= 1;	// PCM
		mWaveFormat.Channels		= ogg_info->channels;
		mWaveFormat.SamplesPerSec	= ogg_info->rate;
		mWaveFormat.AvgBytesPerSec	= ogg_info->rate * bits_per_sample / 8 * ogg_info->channels;
		mWaveFormat.BlockAlign		= bits_per_sample / 8 * ogg_info->channels;
		mWaveFormat.BitsPerSample	= bits_per_sample;//readU16( file_ );
        mWaveFormat.EXSize          = 0;

		ogg_int64_t total = ov_pcm_total( &mOggVorbisFile, -1 );
	    LN_THROW_FileFormat( ( total != OV_EINVAL ), "ov_pcm_total %d\n", total );
        //LN_SETERR_R_S_( , LN_ERR_ASSERT, "< Audio::Ogg::setStream >\no" );
		
		// オンメモリに展開する時に必要な PCM サイズ
		mOnmemoryPCMBufferSize = static_cast< u32 >( total ) * WORD_SIZE * mWaveFormat.Channels;		// 2 は 16bit なので

		//printf( "(要チェック！！)mOnmemoryPCMBufferSize: %d\n", mOnmemoryPCMBufferSize );

		mSourceDataSize = mOnmemoryPCMBufferSize;

		// 念のため先頭にシーク
		ov_time_seek( &mOggVorbisFile, 0.0 );

        // 再生時間
		double t = static_cast< double >( mOnmemoryPCMBufferSize ) / ( static_cast< double >( mWaveFormat.AvgBytesPerSec ) * 0.001 );
		mTotalTime = static_cast< u32 >( t );

        // 全体の再生サンプル数
        mTotalSamples = mOnmemoryPCMBufferSize / ( mWaveFormat.BitsPerSample / 8 ) / mWaveFormat.Channels;


        // ファイルに埋め込まれている "LOOPSTART" "LOOPLENGTH" コメントを探す
        vorbis_comment* ogg_comment = ov_comment( &mOggVorbisFile, -1 );
        char* c;
        char buf[ 20 ];
        for ( int i = 0; i < ogg_comment->comments; ++i )
        {
            if ( ogg_comment->comment_lengths[ i ] >= 9 )
            {
                c = ogg_comment->user_comments[ i ];

                if ( memcmp( c, "LOOPSTART", 9 ) == 0 )
                {
                    memset( buf, 0, sizeof( buf ) );
                    memcpy( buf, ( c + 10 ), ( ogg_comment->comment_lengths[ i ] - 10 ) );
                    mLoopStart = static_cast< u32 >( atoi( buf ) );
                }
                else if ( memcmp( c, "LOOPLENGTH", 10 ) == 0 )
                {
                    memset( buf, 0, sizeof( buf ) );
                    memcpy( buf, ( c + 11 ), ( ogg_comment->comment_lengths[ i ] - 11 ) );
                    mLoopLength = static_cast< u32 >( atoi( buf ) );
                }
            }
        }

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● 全てのデータを mOnmemoryPCMBuffer へ読み込む
	//---------------------------------------------------------------------
	LNRESULT Ogg::fillBufferAndReleaseStream()
	{
		if ( mOnmemoryPCMBuffer == NULL && mOnmemoryPCMBufferSize > 0 )
		{
			mOnmemoryPCMBuffer = LN_NEW u8[ mOnmemoryPCMBufferSize ];

			long read_size;
			long point = 0;
			u32 total = 0;
			char* temp_buffer[ 4096 ];
			int bitstream;

			// ファイルの先頭にシーク
			ov_time_seek( &mOggVorbisFile, 0.0 );

			// 全部読み込む
			while ( 1 )
			{
				// 不定長～4096 バイトずつ読んでいく
				read_size = ov_read( &mOggVorbisFile, (char*)temp_buffer, 4096, 0, WORD_SIZE, 1, &bitstream );

				// バッファにコピー
				memcpy( &mOnmemoryPCMBuffer[ point ], temp_buffer, read_size );

				total += read_size;

				// 最後まで読んだ場合は終了
				if ( read_size == EOF || read_size == 0 ) { 
					break; }

				point += read_size;
			}

            LN_SAFE_RELEASE( mInFile );
		}
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● データを buffer_ へ読み込む
	//---------------------------------------------------------------------
	LNRESULT Ogg::read( void* buffer_, u32 buffer_size_, u32* read_size_, u32* write_size_ )
	{
		// 0 クリア
		memset( buffer_, 0, buffer_size_ );
		*read_size_ = 0;

		long size = 0;
		int request_size = 4096;
		int bitstream = 0;
		u32 com_size = 0;
		char* buffer = (char*)buffer_;

		while ( 1 )
		{
			size = ov_read( &mOggVorbisFile, (char*)( buffer + com_size ), request_size, 0, WORD_SIZE, 1, &bitstream );
		
			// ファイルエンド
			if ( size == 0 )
			{
				break;
			}

			com_size += size;

			// バッファを全部埋めた場合
			if ( com_size >= buffer_size_ )
			{
				break;
			}

			// バッファの残りが 4096 未満の場合
			if ( buffer_size_ - com_size < 4096 )
			{
				// 次に読むバイト数は、バッファの残り領域分だけ
				request_size = buffer_size_ - com_size;
			}
		}

		*read_size_ = com_size;
		*write_size_ = com_size;

        return LN_OK;
	}

    

	//---------------------------------------------------------------------
	// ● ファイルポインタ移動
	//---------------------------------------------------------------------
	LNRESULT Ogg::seek( u32 offset_ )
	{
		ov_pcm_seek( &mOggVorbisFile, ( offset_ / ( WORD_SIZE * mWaveFormat.Channels ) ) );
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// ● read コールバック
	//---------------------------------------------------------------------
	size_t Ogg::readOggCallback( void* buffer_, size_t element_size_, size_t count_, void* stream_ )
	{
        File::IInStream* file = (File::IInStream*)stream_;
		size_t read_size = file->read( buffer_, element_size_ * count_, element_size_ * count_ );
		return read_size;
	}

	//---------------------------------------------------------------------
	// ● seek コールバック
	//---------------------------------------------------------------------
	int Ogg::seekOggCallback( void* stream_, ogg_int64_t offset_, int whence_ )
	{
		if ( stream_ == NULL ) { 
	return -1; } // 異常の時は 0 以外の値を返す
		File::IInStream* file = (File::IInStream*)stream_;
		file->seek( static_cast< int >( offset_ ), whence_ );
		return 0;
	}

	//---------------------------------------------------------------------
	// ● close コールバック
	//---------------------------------------------------------------------
	int Ogg::closeOggCallback( void* stream_ )
	{
		return 0;	// InFile のデストラクタで閉じるので、ここでは何もしない。
	}

	//---------------------------------------------------------------------
	// ● tell コールバック
	//---------------------------------------------------------------------
	long Ogg::tellOggCallback( void* stream_ )
	{
		File::IInStream* file = (File::IInStream*)stream_;
		return file->getPosition();
	}

#if defined(LNOTE_MSVC)
//=============================================================================
// ■ Midi クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    Midi::Midi( ResourceManager* manager_ )
        : AudioSourceBase   ( manager_ )
        , mInFile           ( NULL )
        , mSourceData       ( NULL )
        , mSourceDataSize   ( 0 )
        , mCC111Time        ( 0 )
        , mBaseTime         ( 0 )
        , mVolumeNormalize  ( false )
    {
    }

	//---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
	Midi::~Midi()
    {
        SAFE_DELETE_ARRAY( mSourceData );
        LN_SAFE_RELEASE( mInFile );
    }

    //---------------------------------------------------------------------
	// ● オーディオファイルとして扱うストリームを設定する
    //---------------------------------------------------------------------
	LNRESULT Midi::setStream( File::IInStream* stream_ )
    {
        LN_SETERR_R_S_( ( !mInFile ), LN_ERR_ASSERT, "< Audio::Midi::setStream >\n複数のストリームを設定しようとしました。" );

		mInFile = stream_;
        mInFile->addRef();

        mVolumeEntryList.clear();

        _searchData();

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 内部にバッファを確保し、Midi データを全て読み込む
    //---------------------------------------------------------------------
	LNRESULT Midi::fillBufferAndReleaseStream()
    {
        if ( !mSourceData )
        {
            mSourceDataSize = mInFile->getSize();
            mSourceData = LN_NEW char[ mSourceDataSize ];

            mInFile->seek( 0 );
            mInFile->read( mSourceData, mSourceDataSize );

            
            if ( mVolumeNormalize )
			{
				std::list< VolumeEntry >::iterator it;
                std::list< VolumeEntry >::iterator end = mVolumeEntryList.end();

				// ボリュームの最大値を探す
				u32 max_volume = 0;
				for ( it = mVolumeEntryList.begin(); it != end; ++it )
				{
					if ( it->mVolume > max_volume )
					{
						max_volume = it->mVolume;
					}
				}

                // 最大値を 127 にするための値を計算
				int sub = 127 - max_volume;

                // 全ての音量値に加算
				for ( it = mVolumeEntryList.begin(); it != end; ++it )
				{
					mSourceData[ it->mPosition ] += sub;
				}

				mVolumeEntryList.clear();
			}

            LN_SAFE_RELEASE( mInFile );
        }

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● midi データの中の必要なデータをチェックする
    //---------------------------------------------------------------------
	void Midi::_searchData()
    {
        // ファイルポインタを先頭に戻しておく
		mInFile->seek( 0 );

		// Midi ファイルのヘッダ読み込み
		MidiHeader header;
		mInFile->read( &( header.mChunktype ), 4 );
        header.mLength      = File::readU32Big( mInFile );
		header.mFormat      = File::readU16Big( mInFile );
		header.mNumtrack    = File::readU16Big( mInFile );
		header.mTimebase    = File::readU16Big( mInFile );

		// ベースタイム格納
		mBaseTime = header.mTimebase;

        mCC111Time = 0;
        u32 cc111time = 0;

		// トラックの数だけループして、cc111 とボリュームチェンジを探す
		for ( int i = 0; i < header.mNumtrack; ++i )
		{
            _searchTrack( &cc111time );

            if ( mCC111Time == 0 )
            {
                mCC111Time = cc111time;
            }

			/*if ( !_searchTrackCC111( &mCC111Time ) )
			{
				return;
			}*/
		}
    }

	//---------------------------------------------------------------------
	// ● デルタタイムの読み込み
    //---------------------------------------------------------------------
	u32 Midi::_readDelta()
    {
        u8 t;
		u32 dtime = 0;
		for ( int i = 0; i < sizeof( u32 ); ++i )
		{
			mInFile->read( &t, sizeof( u8 ) );
			dtime = ( dtime << 7 ) | ( t & 0x7f );

			// MSBが立っていないならば、次のバイトはデルタタイムではないので抜ける
			if ( !( t & 0x80 ) ) { 
				break; }
		}
		return dtime;
    }

	//---------------------------------------------------------------------
	// ● トラックの捜査
    //---------------------------------------------------------------------
	bool Midi::_searchTrack( u32* cc111_time_ )
    {
        // トラックチャンクのチェック
		u8 chunk[ 4 ];
		mInFile->read( chunk, 4 );
		if ( memcmp( chunk, "MTrk", 4 ) != 0 ) { 
			return false; }

		// トラックの長さ読み込み
		u32   track_length;
		track_length = File::readU32Big( mInFile );

		u8 prev_state = 0; // ひとつ前のイベントのステータスバイトを記憶する変数
		u8 state;
		u8 data1;
		u8 data2;
		u32 track_time = 0;

		while ( 1 )
		{
			// デルタタイムを読み込む
			track_time += _readDelta();

			// ステータスバイトを読み込む
			mInFile->read( &state, sizeof( u8 ) );
			
			// ランニングステータスの場合
			if ( !( state & 0x80 ) )
			{
				// 一つ前のイベントのステータスバイトを代入
				state = prev_state;
				// ファイルポインタを一つ戻す
				mInFile->seek( -1, SEEK_CUR );
			}
			
			// ステータスバイトを基にどのイベントか判別
			switch ( state & 0xf0 )
			{
				// データバイトが 2 バイトのイベント
				case 0x80:	// ノートオフ
				case 0x90:	// ノートオン
				case 0xA0:	// ポリフォニック・キー・プレッシャ
				case 0xE0:	// ピッチベンド
					mInFile->seek( 2, SEEK_CUR );
					break;

				case 0xB0:	// コントロールチェンジ
					mInFile->read( &data1, sizeof( u8 ) );
					mInFile->read( &data2, sizeof( u8 ) );

					// cc111
					if ( data1 == 0x6F )
					{
						*cc111_time_ = track_time;
					}
					// ボリュームチェンジ
					else if ( data1 == 0x07 )
					{
						// データの位置とボリュームをキューに入れて保存
						VolumeEntry entry;
						entry.mPosition = mInFile->getPosition() - 1;
						entry.mVolume = data2;
						mVolumeEntryList.push_back( entry );
						//printf("ボリュームチェンジ %d\n", data2);
					}
					break;

				// データバイトが 1 バイトのイベント
				case 0xC0:	// プログラムチェンジ
				case 0xD0:	// チャンネルプレッシャ
					mInFile->seek( 1, SEEK_CUR );
					break;
				
				// データバイトが可変長のイベント
				case 0xF0:
					// SysExイベント
					if ( state == 0xF0 )
					{
						int data_length = 0;
						// データ長読み込み
						mInFile->read( &data_length, sizeof( u8 ) );
						mInFile->seek( data_length, SEEK_CUR );
					}
					// メタイベント
					else if ( state == 0xFF )
					{
						u8 type;

						// typeの取得
						mInFile->read( &type, sizeof( u8 ) );

						u32 data_length = -1;

						// type 別にデータバイトの長さを取得
						switch ( type )
						{
							case 0x00: 
								data_length = 2; break;
							case 0x01:
							case 0x02:
							case 0x03:
							case 0x04:
							case 0x05:
							case 0x06:
							case 0x07: 
								break;
							case 0x20: 
								data_length = 1; break; 
							case 0x21: 
								data_length = 1; break; 
							case 0x2F: 
								data_length = 0; break; // エンドオブトラック
							case 0x51: 
								data_length = 3; break; // セットテンポ
							case 0x54: 
								data_length = 5; break;
							case 0x58: 
								data_length = 4; break;
							case 0x59: 
								data_length = 2; break;
							case 0x7F: 
								break;

							default:
								//LASSERT_S_( 1, LException::Format(), "< LAudioSourceMIDI::_searchCC111FromTrack >\n存在しないメタイベントです。\n%x", type );
								return false;

						}
						
						u32 temp = data_length;

						// データ長が固定の場合
						if ( data_length != -1 )
						{
							data_length = _readDelta();
							if ( data_length != temp )
							{
								//LASSERT_S_( 1, LException::Format(), "< LAudioSourceMIDI::_searchCC111FromTrack >\n固定長メタイベントのデータ長が不正です。\n%d != %d\n", data_length, temp );
								return false;
							}
						}
						else
						{
							// 任意のデータ長を取得
							data_length = _readDelta(); 
						}

						mInFile->seek( data_length, SEEK_CUR );

						// トラックの終端が見つかった場合は終了
						if ( type == 0x2F )
						{
							return true;
						}
					}

					break;

				default:
					//LASSERT_S_( 1, LException::Format(), "< LAudioSourceMIDI::_searchCC111FromTrack >\nステータスバイトが不正です。%u\n", ( state & 0xf0 ) );
					return false;

			}
			// 次のイベントが前のイベントのステータスバイトを確認できるように保存する
			prev_state = state;
		}
		return true;
    }

#endif // LNOTE_MSVC

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Audio
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================