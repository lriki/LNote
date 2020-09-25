//=============================================================================
//【 AudioSource 】
//-----------------------------------------------------------------------------
///**
//  @file       AudioSource.h
//  @brief      AudioSource
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <list> // Midi で使ってるので…
#include "../../../Dependencies/libogg/include/ogg/ogg.h"
#include "../../../Dependencies/libvorbis/include/vorbis/vorbisfile.h"
#include "../../File/Interface.h"
#include "../Interface.h"
#include "AudioResourceManager.h"

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
//-----------------------------------------------------------------------------
///**
//  @brief      音声データを扱うクラスの基底
//*/
//=============================================================================
class AudioSourceBase
    : public IAudioSource
    , public Base::ReferenceObject
    , public AudioSourceCache::CachedObject
{
public:
    
    LN_CACHED_REFOBJ_METHODS;

    /// コンストラクタ
    AudioSourceBase( ResourceManager* manager_ );

    /// デストラクタ
    virtual ~AudioSourceBase();

    /// fillBufferAndReleaseStream() スレッドセーフ
    virtual void fillBufferSafe()
    {
        Thread::ScopedLock lock( mMutex );
        this->fillBufferAndReleaseStream();
    }

    /// seek + reed + スレッドセーフ
    virtual void readSafe( void* buffer_, u32 buffer_size_, u32 offset_, u32* read_size_, u32* write_size_ )
    {
        Thread::ScopedLock lock( mMutex );
        this->seek( offset_ );
        this->read( buffer_, buffer_size_, read_size_, write_size_ );
    }


protected:

    ResourceManager*    mManager;
    Thread::Mutex       mMutex;
};

//=============================================================================
// ■ Wave クラス
//-----------------------------------------------------------------------------
///**
//  @brief      Wave データを扱うクラス
//*/
//=============================================================================
class Wave
    : public AudioSourceBase
{
public:

	/// コンストラクタ
	Wave( ResourceManager* manager_ );

	/// デストラクタ
	virtual ~Wave();

public:

	/// ファイルフォーマットの取得
	virtual LNAudioSourceFormat getSourceFormat() const { return LN_AUDIOSOURCE_WAVE; }

	/// 元データのサイズの取得 ( ストリーミング再生での終了判定等で使う )
	virtual u32 getSourceDataSize() const { return mSourceDataSize; }

	/// PCM フォーマットの取得
	virtual const LNWaveFormat* getWaveFormat() const { return &mWaveFormat; }

	/// 全体の再生時間の取得 ( ミリ秒 )
	virtual u32 getTotalTime() const { return mTotalTime; }

    /// 全体のサンプル数の取得
    virtual u32 getTotalUnits() const { return mTotalSamples; }

	/// オンメモリ再生用のバッファの先頭アドレス取得
	virtual u8* getOnmemoryPCMBuffer() const { return mOnmemoryPCMBuffer; }

	/// オンメモリ再生時の全バッファサイズの取得
	virtual u32 getOnmemoryPCMBufferSize() const { return mOnmemoryPCMBufferSize; }

	/// 1 秒あたりのバイト数取得
	virtual u32 getBytesPerSec() const { return mWaveFormat.AvgBytesPerSec; }

    /// ループ開始位置と終了位置の取得
    virtual void getLoopState( u32* begin_, u32* length_ ) const { *begin_ = 0; *length_ = 0; }

	/// オーディオファイルとして扱うストリームを設定する
	virtual LNRESULT setStream( File::IInStream* stream_ );

	/// オンメモリ再生用に全てのデータを読み込む
	virtual LNRESULT fillBufferAndReleaseStream();

	/// データをデコードし、buffer_ に書き込む
	virtual LNRESULT read( void* buffer_, u32 buffer_size_, u32* read_size_, u32* write_size_ );

	/// ファイルポインタ移動
	virtual LNRESULT seek( u32 offset_ );

	/// デコード状態のリセット
	virtual void reset() {}

private:

	/// wave ファイルのヘッダ
	struct WaveFileHeader
	{
		u32	RIFF;
		u32	Size;
		u32	WaveHeader;
		u32	FmtChunk;
		u32	FmtSize;
	};

private:

    File::IInStream*        mInFile;				///< 入力ストリーム
	LNWaveFormat	        mWaveFormat;			///< PCM 用フォーマット
	u32				        mSourceDataSize;		///< 音声データ部分のサイズ
	u32				        mDataOffset;			///< ストリーム内の PCM データの先頭までのオフセットバイト数
	u32				        mPCMDataSize;			///< ストリーム内の PCM データのサイズ
	u32				        mTotalTime;				///< 全体の再生時間 ( ミリ秒 )
	u8*				        mOnmemoryPCMBuffer;		///< オンメモリ再生用のデータを読み込むバッファ
	u32				        mOnmemoryPCMBufferSize;	///< mOnmemoryPCMBuffer のサイズ ( 今のところ mDataOffset と同じ )
    u32                     mTotalSamples;          ///< 全体の再生サンプル数
};

/*
	◆ WaveFileHeader 構造体についてめも

	正しくは

		struct RiffHeader
		{
			u32 riff;	// 'RIFF'
			s32 size;	// ファイルの全体サイズ - 8 の値
			u32 type;	// 音声データの場合は 'WAVE'
		};

	というヘッダの後に、

		struct FormatChunk
		{
			u32 id;			// 'fmt '
			s32 size;		// id と size を除くfmtチャンクサイズ
			s16 format_tag;	// 波形データのフォーマット ( 基本は WAVE_FORMAT_PCM。これ以外の場合は extended があるらしい )
			u16 channels;
			u32 samplerate;
			u32 bytepersec;
			u16 blockalign;
			u16 bitswidth;
		};
		u16 extended_size;
		u8  extended [];

	…と続いていく。
	そのあとに実際のデータチャンクがある。

		struct DataChunk
		{
			u32 id;		// 'data'
			s32 size;
			u8  waveformData [];
		};

	
	このライブラリで使ってる↑の WaveFileHeader 構造体は、

	RiffHeader 全部とFormatChunk の size までは
	全部同じだから(ファイル全体のサイズはいらないし)ファイルフォーマットのチェックにしか使わないよね！
	ってことでひとつにまとめてます。

*/

#if defined(LNOTE_MSVC)
//=============================================================================
// ■ MP3 クラス
//-----------------------------------------------------------------------------
///**
//  @brief      MP3 データを扱うクラス
//
//  @note
//              ストリーミング再生時のコンバート情報はこのクラスの内部に
//              持っているため、複数のプレイヤークラスでこのソースを共有して
//              ストリーミング再生することはできない。( 実際はできるけど、
//              音が変な感じに飛ぶ )
//              そのため、このクラスは共有しないようにすること。
//              ( ストリーミング再生が必要なほど長い音声を複数再生することは
//              あまりないだろうし…。 )
//              <br>
//              <br>
//              ちなみにこれを解決するには、オーディオソースからストリーミング再生の情報
//              を取得し、read() を呼ぶときにそれを渡して音声データを取得するようにする。
//              ( これは次バージョンでの実装になるかなぁ… )
//*/
//=============================================================================
class MP3
    : public AudioSourceBase
{
public:

	/// コンストラクタ
	MP3( ResourceManager* manager_ );

	/// デストラクタ
	virtual ~MP3();

public:

	/// ファイルフォーマットの取得
	virtual LNAudioSourceFormat getSourceFormat() const { return LN_AUDIOSOURCE_MP3; }

	/// PCM フォーマットの取得
	virtual const LNWaveFormat* getWaveFormat() const { return &mWaveFormat; }

	/// 元データのサイズの取得 ( ストリーミング再生での終了判定等で使う )
	virtual u32 getSourceDataSize() const { return mSourceDataSize; }

	/// 全体の再生時間の取得 ( ミリ秒 )
	virtual u32 getTotalTime() const { return mTotalTime; }

    /// 全体のサンプル数の取得
    virtual u32 getTotalUnits() const { return mTotalSamples; }

	/// オンメモリ再生用のバッファの先頭アドレス取得
	virtual u8* getOnmemoryPCMBuffer() const { return mOnmemoryPCMBuffer; }

	/// オンメモリ再生時の全バッファサイズの取得
	virtual u32 getOnmemoryPCMBufferSize() const { return mOnmemoryPCMBufferSize; }

	/// 1 秒あたりのバイト数取得
	virtual u32 getBytesPerSec() const { return mStreamingPCMBufferSize; }

    /// ループ開始位置と終了位置の取得
    virtual void getLoopState( u32* begin_, u32* length_ ) const { *begin_ = 0; *length_ = 0; }

	/// オーディオファイルとして扱うファイルを設定する
	virtual LNRESULT setStream( File::IInStream* stream_ );

	/// オンメモリ再生用に全てのデータを読み込む
	virtual LNRESULT fillBufferAndReleaseStream();

	/// データをデコードし、buffer_ に書き込む
	virtual LNRESULT read( void* buffer_, u32 buffer_size_, u32* read_size_, u32* write_size_ );

	/// ファイルポインタ移動
	virtual LNRESULT seek( u32 offset_ );

	/// デコード状態のリセット
	virtual void reset() { mResetFlag = true; }

private:

	/// どのフォーマットの mp3 か調べて、データまでのオフセットやタグフィールドのサイズをメンバに格納
	LNRESULT _checkId3v();

	/// mp3 の PCM フォーマットを調べてメンバに格納する
	LNRESULT _getPCMFormat();

private:

	/// ID3v2 形式のヘッダ情報
	struct ID3v2Header
	{
		u8	ID[ 3 ];
		u8	Version[ 2 ];
		u8	Flag;
		u8	Size[ 4 ];
	};

private:

    File::IInStream*        mInFile;					///< 入力ストリーム
	LNWaveFormat			mWaveFormat;				///< PCM 用フォーマット
    //WAVEFORMATEX            mWaveFormatEX;
	MPEGLAYER3WAVEFORMAT*	mMP3WaveFormat;				///< MPEGLAYER3WAVEFORMAT 構造体へのポインタ
	HACMSTREAM*				mACMStreamHandle;			///< HACMSTREAM 構造体へのポインタ
	u32						mSourceDataSize;			///< ソースデータのサイズ ( いまのところ mInFile.size() と同じ )
	u32						mDataOffset;				///< ストリーム内の PCM データの先頭までのオフセットバイト数
	u32						mID3vTagFieldSize;			///< Id3v2 形式の場合のタグフィールド(ヘッダ情報)部分のサイズ
	u8*						mOnmemoryPCMBuffer;			///< オンメモリ再生用のデータを読み込むバッファ
	u32						mOnmemoryPCMBufferSize;		///< ( fillBufferAndReleaseStream() で値が設定される )
	u32						mStreamingPCMBufferSize;	///< 1 秒分の mp3 データを変換した時の、最適な転送先 PCM バッファサイズ
	u8*						mMP3SourceBufferParSec;		///< デコード時にファイルから読む 1 秒分の mp3 データを一時的に格納するバッファ
	u32						mMP3SourceBufferSizeParSec;	///< mMP3SourceBufferParSec のサイズ
	u32						mTotalTime;					///< 全体の再生時間 ( ミリ秒 )
    u32                     mTotalSamples;              ///< 全体のサンプル数
	bool					mResetFlag;					///< デコード状態のリセットを要求するフラグ ( read() でのデコード時のフラグ指定に使う )
};

/*
	id3v1 … MP3データはファイルの先頭から格納されている
	id3v2 … タグフィールドの後にMP3データが格納されている
*/
#endif // LNOTE_MSVC

//=============================================================================
// ■ Ogg クラス
//-----------------------------------------------------------------------------
/**
//  @brief      Ogg Vorbis 音声データを扱うクラス
//
//  @note
//              気になる点…<br>
//              1サンプルあたりのビット数は16ビットで固定してある。今のところ問題ないけど…？
//*/
//=============================================================================
class Ogg
    : public AudioSourceBase
{
public:

	/// コンストラクタ
	Ogg( ResourceManager* manager_ );

	/// デストラクタ
	virtual ~Ogg();

public:

	/// ファイルフォーマットの取得
	virtual LNAudioSourceFormat getSourceFormat() const { return LN_AUDIOSOURCE_OGG; }

	/// PCM フォーマットの取得
	virtual const LNWaveFormat* getWaveFormat() const { return &mWaveFormat; }

	/// 元データのサイズの取得 ( ストリーミング再生での終了判定等で使う )
	virtual u32 getSourceDataSize() const { return mSourceDataSize; }

	/// 全体の再生時間の取得 ( ミリ秒 )
	virtual u32 getTotalTime() const { return mTotalTime; }

    /// 全体のサンプル数の取得
    virtual u32 getTotalUnits() const { return mTotalSamples; }

	/// オンメモリ再生用のバッファの先頭アドレス取得
	virtual u8* getOnmemoryPCMBuffer() const { return mOnmemoryPCMBuffer; }

	/// オンメモリ再生時の全バッファサイズの取得
	virtual u32 getOnmemoryPCMBufferSize() const { return mOnmemoryPCMBufferSize; }

	/// 1 秒あたりのバイト数取得
	virtual u32 getBytesPerSec() const { return mWaveFormat.AvgBytesPerSec; }

    /// ループ開始位置と終了位置の取得
    virtual void getLoopState( u32* begin_, u32* length_ ) const { *begin_ = mLoopStart; *length_ = mLoopLength; }

	/// オーディオファイルとして扱うストリームを設定する
	virtual LNRESULT setStream( File::IInStream* stream_ );

	/// オンメモリ再生用に全てのデータを読み込む
	virtual LNRESULT fillBufferAndReleaseStream();

	/// データをデコードし、buffer_ に書き込む
	virtual LNRESULT read( void* buffer_, u32 buffer_size_, u32* read_size_, u32* write_size_ );

	/// ファイルポインタ移動
	virtual LNRESULT seek( u32 offset_ );

	/// デコード状態のリセット
	virtual void reset() {}

public:	// 以下は ogg API からのコールバックとして登録する関数。そのため public

	/// ogg 用 read コールバック
	static size_t readOggCallback( void* buffer_, size_t element_size_, size_t count_, void* stream_ );

	/// ogg 用 seek コールバック
	static int seekOggCallback( void* stream_, ogg_int64_t offset_, int whence_ );

	/// ogg 用 close コールバック
	static int closeOggCallback( void* stream_ );

	/// ogg 用 tell コールバック
	static long tellOggCallback( void* stream_ );

private:

	static const int WORD_SIZE = sizeof( lnU16 );
	static const int WORD_BITS = WORD_SIZE * 8;

    File::IInStream*        mInFile;				///< 入力ストリーム
	OggVorbis_File	        mOggVorbisFile;			///< オープン済みの Ogg ファイル
	LNWaveFormat	        mWaveFormat;			///< PCM 用フォーマット
	u32				        mSourceDataSize;		///< 元データのサイズ ( エンコードされた部分 )
	u8*				        mOnmemoryPCMBuffer;	    ///< オンメモリ再生用のデータを読み込むバッファ
	u32				        mOnmemoryPCMBufferSize;	///< ( fillBufferAndReleaseStream() で値が設定される )
	u32				        mTotalTime;				///< 全体の再生時間 ( ミリ秒 )
    u32                     mTotalSamples;          ///< 全体のサンプル数
    u32                     mLoopStart;             ///< ループ領域の先頭サンプル数
    u32                     mLoopLength;            ///< ループ領域の長さ ( サンプル数 )
};

    
#if defined(LNOTE_MSVC)
//=============================================================================
// ■ Midi クラス
//-----------------------------------------------------------------------------
///**
//  @brief      Midi データを扱うクラス
//*/
//=============================================================================
class Midi
    : public AudioSourceBase
{
public:

	/// コンストラクタ
	Midi( ResourceManager* manager_ );

	/// デストラクタ
	virtual ~Midi();

public:

	/// ファイルフォーマットの取得
	virtual LNAudioSourceFormat getSourceFormat() const { return LN_AUDIOSOURCE_MIDI; }

    /// 元データの取得 ( Midi データ本体 )
	virtual void* getSourceData() const { return mSourceData; }

	/// 元データのサイズの取得
	virtual u32 getSourceDataSize() const { return mSourceDataSize; }

    /// 全体の再生時間の取得 ( ミュージックタイム )
	virtual u32 getTotalTime() const { return 0; }

    /// 全体のサンプル数の取得 ( Midi の場合はミュージックタイム単位 )
    virtual u32 getTotalUnits() const { printf( "Midi::getTotalUnits() 未定義" ); return 0; }

    /// CC111 位置とベースタイムの取得
    virtual void getLoopState( u32* cc111time_, u32* base_time_ ) const { *cc111time_ = mCC111Time; *base_time_ = mBaseTime; }

    /// オーディオファイルとして扱うストリームを設定する
	virtual LNRESULT setStream( File::IInStream* stream_ );

    /// 内部にバッファを確保し、Midi データを全て読み込む ( アクセスは getSourceData() から )
	virtual LNRESULT fillBufferAndReleaseStream();

    /// 音量を正規化して読み込むかの設定 ( fillBufferAndReleaseStream() の前に呼ぶこと )
    void setEnableVolumeNormalize( bool flag_ ) { mVolumeNormalize = flag_; }


protected:

	/// PCM フォーマットの取得
	virtual const LNWaveFormat* getWaveFormat() const { return NULL; }

	/// オンメモリ再生用のバッファの先頭アドレス取得
	virtual u8* getOnmemoryPCMBuffer() const { return NULL; }

	/// オンメモリ再生時の全バッファサイズの取得
	virtual u32 getOnmemoryPCMBufferSize() const { return 0; }

	/// 1 秒あたりのバイト数取得
	virtual u32 getBytesPerSec() const { return 0; }

	/// データをデコードし、buffer_ に書き込む
    virtual LNRESULT read( void* buffer_, u32 buffer_size_, u32* read_size_, u32* write_size_ ) { printf( "non\n" ); return LN_OK; }

	/// ファイルポインタ移動
	virtual LNRESULT seek( u32 offset_ ) { printf( "non\n" ); return LN_OK; }

	/// デコード状態のリセット
	virtual void reset() {}

private:

    /// midi データの中の必要なデータをチェックする
	void _searchData();

	/// デルタタイムの読み込み
	u32 _readDelta();

	/// トラック内の CC111 検索
	bool _searchTrack( u32* cc111_time_ );

private:

    /// Midi ファイルのヘッダ
    struct MidiHeader
    {
	    u8      mChunktype[ 4 ];	///< チャンクタイプ ( MThd )
	    u32     mLength;			///< データ長
	    u16     mFormat;			///< フォーマットタイプ
	    u16     mNumtrack;			///< トラック数
	    u16     mTimebase;			///< タイムベース
    };

    /// ボリュームチェンジの位置と値 ( 正規化に使う )
    struct VolumeEntry
	{
		u32     mPosition;	        ///< ボリュームチェンジの値の位置 [ 00 B0 07 46 ] の 07
		u32     mVolume;            ///< ボリューム値
	};

private:

    File::IInStream*            mInFile;				///< 入力ストリーム
    char*                       mSourceData;            ///< 元データ ( Midi データ )
	u32				            mSourceDataSize;		///< 元データのサイズ
    u32                         mCC111Time;
	u32                         mBaseTime;
    std::list< VolumeEntry >    mVolumeEntryList;
    bool                        mVolumeNormalize;
};

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