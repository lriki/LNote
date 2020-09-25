//=============================================================================
//【 Archive 】
//-----------------------------------------------------------------------------
///**
//  @file       Archive.h
//  @brief      Archive
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <map>
#include <string>
#include "../Base/RefString.h"
#include "../Thread/Mutex.h"
#include "Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File
{
class ArchiveInStream;

//=============================================================================
// ■ Archive クラス
//-----------------------------------------------------------------------------
///**
//  @brief      アーカイブファイルを扱うクラス
//*/
//=============================================================================
class Archive
    : public Base::ReferenceObject
{
public:

	/// initialize() が返すエラーコード
	enum ErrorCode
	{
		SUCCESS = 0,	///< 成功
		ERR_NOT_OPEN,	///< アーカイブファイルを開けなかった
		ERR_FORMAT,		///< アーカイブファイルではない
		ERR_KEY			///< キーが不一致
	};

public:

	/// コンストラクタ
	Archive();

	/// デストラクタ
	virtual ~Archive();

public:

    //---------------------------------------------------------------------
    /**
    //  @brief      アーカイブファイルを開いてアクセスの準備をする
    //
    //  @param[in]  filename_ : アーカイブファイルのファイル名
    //  @param[in]  key_      : 複合に使うキー ( \0 終端文字列 または NULL )
    //
    //  @par
    //              filename_ に NULL を渡すと、ダミーアーカイブとして初期化します。
    //              ダミーアーカイブは、直接ディレクトリ上のファイルを扱います。
    //
    //              filename_ にディレクトリパスが含まれている場合、
    //              アーカイブに含まれているファイルはすべてそのディレクトリパス
    //              以下のファイルとして扱います。
@code
// アーカイブファイル内に「Chara.png」「Map/Ground.png」というファイルがある場合…

// 例 1 )
// 実行ファイルと同じフォルダにあるアーカイブファイル "Image.lna" を使う
archive.initialize( "Image.lna" );

file.open( "Image/Chara.png" );
file.open( "Image/Map/Ground.png" );


// 例 2 )
// 実行ファイルと同じフォルダにある「Data」フォルダ内のアーカイブファイル "Image.lna" を使う
archive.initialize( "Data/Image.lna" );

file.open( "Data/Image/Chara.png" );
file.open( "Data/Image/Map/Ground.png" );
@endcode
    //*/
    //---------------------------------------------------------------------
    ErrorCode initialize( const lnChar* directory_, const lnChar* filename_, const char* key_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      ファイルストリームとデータ位置までのオフセット取得 【 安全性のため、使用非推奨。createStream() を使ってください 】
    //
    //  @param[in]  filename_    : ファイル名 (/ は \\ に変換済みであること)
    //  @param[out] out_stream_  : ファイルストリームへのポインタを格納する変数へのポインタ
    //  @param[out] data_offset_ : データがある場所までのオフセット ( バイト単位 ) を格納する変数へのポインタ
    //  @param[out] data_size_   : ファイルサイズを格納する変数へのポインタ
    //
    //  @par
    //              ファイルが見つからなかった場合は out_stream_ に NULL が代入されます。   <br>
    //                                                                                      <br>
    //              ダミーアーカイブの場合、out_stream_ は常に NULL です。          
    //
    //  @attention 
    //              この関数で取得したファイルストリームは必ずこのクラスの close() 関数で
    //              閉じてください。fclose() で閉じるとアクセス違反が発生します。
    //*/
    //---------------------------------------------------------------------
    void open( const lnChar* filename_, FILE** out_stream_, int* data_offset_, int* data_size_, LNFileOpenMode mode_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      アーカイブ内のファイルにアクセスするためのストリームの作成
    //
    //  @param[out] stream_   : 作成されたオブジェクトを指すポインタ変数へのアドレス
    //  @param[in]  filename_ : ファイル名 (/ は \\ に変換済みであること)
    //
    //  @par
    //              ファイルが見つからなかった場合は stream_ に NULL が代入されます。   <br>
    //              <br>
    //              ダミーアーカイブでも、ファイルが見つかれば ArchiveInStream は作成されます。<br>
    //              ArchiveInStream は通常のアーカイブ、ダミーアーカイブの区別なく
    //              同じように使うことができます。
    //
    //  @note
    //              この関数の呼び出しがわ ( File::Manager ) では、複数の
    //              アーカイブをチェックして、全部なかったらエラーにするようにしたい。
    //              そのため、この関数の中でエラーにはしたくない。
    //              そんなわけで、この関数の戻り値は void。
    //*/
    //---------------------------------------------------------------------
    void createStream( IInStream** stream_, const lnChar* filename_, LNFileOpenMode mode_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      アーカイブの中にファイルがあるかどうか確認する
    //
    //  @par
    //              ダミーアーカイブの場合はディレクトリにファイルがあるかどうかを確認します。<br>
    //                                                                                        <br>
    //              ファイルの存在を確認して、すぐにそのファイルを開く場合は
    //              open() で返されるファイルストリームが NULL かどうかを
    //              チェックする方が高速に動作します。
    //*/
    //---------------------------------------------------------------------
    bool isExist( const char* filename_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief		ダミーアーカイブかを判定する
    //*/
    //---------------------------------------------------------------------
    bool isDummy() const { return mIsDummy; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      データを読み込む
    //
    //  @param[out] buffer_ : データを書き込むバッファへのポインタ
    //  @param[in]  count_	: 書き込むデータサイズ ( バイト数 )
    //  @param[in]  stream_ : ファイルストリーム
    //
    //  @return     読み込んだデータサイズ
    //
    //  @par
    //              データの暗号を解除しながら読み込みを行います。
    //              fread() の代わりに使います。
    //              fseek() で正しい位置を指しているストリームを渡してください。<br>
    //              ダミーアーカイブの場合の動作は fread() と同じです
    //*/
    //---------------------------------------------------------------------
    u32 read( void* buffer_, u32 count_, FILE* stream_, LNFileOpenMode mode_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      open() で取得したファイルストリームを閉じる
    //*/
    //---------------------------------------------------------------------
    void close( FILE* stream_ );

private:

    // 数値を 16 にそろえるために加算する数値「( 16 - ( v_ % 16 ) ) % 16」の最適化 ( 5 は 11、27 は 5 等 )
    int _padding16( int v_ ) const { return ( v_ != 0 ) ? ( 16 - ( v_ & 0x0000000f ) ) & 0x0000000f : 16; }
    u32 _padding16( u32 v_ ) const { return ( v_ != 0 ) ? ( 16 - ( v_ & 0x0000000f ) ) & 0x0000000f : 16; }

    /// 一時バッファのサイズチェック
    void _checkTempBuffer( u32 request_size_ );

    /// パディングを考慮して整数を読み込む
	u32 _readU32Padding16();

    /// パディングを考慮して整数を読み込む (ファイル名長さ、ファイルサイズ用)
	void _readU32Padding16( u32* v0_, u32* v1_ );

    /// パディングを考慮してデータを読み込む
	int _readPadding16( void* buffer_, int count_ );



	/// 4 バイト読み込み
	//u32 _readU32();

	/// デコード付き read
	//u32 _readWithDecord( const void* buffer_, u32 count_ );

    /// デフォルトのデコーダ
	//static void _decoder( DecodeArgs& args_ );

private:

	/// ファイルひとつ分の情報
	struct Entry
	{
		u32		mOffset;		///< ストリーム先頭からファイルの位置までのオフセット
		u32		mSize;			///< ファイルサイズ
	};

	typedef std::map< Base::RefTString, Entry > EntriesMap;

private:

    static const int KEY_SIZE  = 128;

    Base::RefTString    mArchiveDirectory;  ///< アーカイブファイルをディレクトリに見立てた時の、そこまのパス ( [パス]/[拡張子を除いたアーカイブファイル名] )
	EntriesMap	        mEntriesMap;	    ///< ファイル名に対応するファイル情報を格納する map
	FILE*		        mStream;		    ///< アーカイブファイルのストリーム
	int			        mFileNum;		    ///< アーカイブファイル内のファイル数
    Base::RefTString    mKey;			    ///< 復号キー (char)
	bool		        mIsDummy;		    ///< ダミーアーカイブの場合 true ( ディレクトリから直接読む )
    lnByte              mKeyTable[ 256 ];
    lnByte*             mTempBuffer;
    u32                 mTempBufferSize;
    Thread::Mutex*      mLock;
};

//=============================================================================
// ■ ArchiveInStream クラス
//-----------------------------------------------------------------------------
///**
//  @brief      アーカイブからファイルを読むためのストリームクラス
//
//  @note
//              コンストラクタは private です。
//              Archive クラスから作成してください。
//*/
//=============================================================================
class ArchiveInStream
    : public IInStream
    , public Base::ReferenceObject
{
private:

    /// コンストラクタ
	ArchiveInStream( Archive* archive_, FILE* stream_, u32 data_offset_, u32 data_size_, LNFileOpenMode mode_, bool is_dir_ );

	/// デストラクタ
	virtual ~ArchiveInStream();

    LN_REFOBJ_METHODS;

public:

	/// ファイル ( データ ) サイズの取得
    virtual int getSize() { return mDataSize; }

	/// ファイルポインタの位置の取得
	virtual int getPosition() { return mSeekPoint; }

	/// データの読み込み
	virtual int read( void* buffer_, int buffer_size_, int read_size_ = -1 );

	/// ファイルポインタの設定
	virtual void seek( int offset_, int origin_ = SEEK_SET );

private:

    Archive*        mArchive;           ///< このクラスを作成したアーカイブクラス
    FILE*           mStream;            ///< ファイルストリーム
    u32             mDataOffset;        ///< ファイルの先頭からデータの先頭位置までのオフセット
    u32             mDataSize;          ///< データサイズ
    u32             mSeekPoint;         ///< シーク位置
    LNFileOpenMode  mOpenMode;
    bool            mIsReadDirectory;   ///< ディレクトリから呼んでいる場合 true

    friend class Archive;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace File
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================