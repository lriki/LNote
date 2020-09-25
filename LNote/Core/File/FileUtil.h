//=============================================================================
//【 FileUtil 】
//-----------------------------------------------------------------------------
///**
//  @file       FileUtil.h
//  @brief      FileUtil
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <string>
#include "../Base/RefString.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File
{
class IInStream;

	//---------------------------------------------------------------------
	///**
	//  @brief      数値をリトルエンディアンとして書き込む
	//
	//  @param[in]  stream_ : ファイルストリーム
	//  @param[in]  v_      : 書き込む値
	//
	//  @par
	//              環境のエンディアンを考慮して4バイトの数値を書き込みます。
	//              書き込まれた値はリトルエンディアンになります。
	//*/
	//---------------------------------------------------------------------
	void writeU32Little( FILE* stream_, u32 v_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      ファイルパスの拡張子を取り除いたものを返す ( char )
	//
	//  @return     作成された文字列 ( delete[] で解放する必要があります )
	//*/
	//---------------------------------------------------------------------
	//char* getExtName( const char* path_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      ファイルパスの中からディレクトリパスの部分を取り出す ( char )
	//
	//  @return     作成された文字列 ( delete[] で解放する必要があります )
	//*/
	//---------------------------------------------------------------------
	//char* getDirectoryPath( const char* path_ );


	//---------------------------------------------------------------------
	///**
	//  @brief      ファイルパスからディレクトリパスとファイルパスを返す ( char )
	//
	//  @param[in]  path_     : 操作元のファイルパス
	//  @param[in]  dir_      : ディレクトリパスを格納するバッファへのポインタ または NULL
	//  @param[in]  name_     : ファイル名を格納するバッファへのポインタ または NULL
	//
	//  @return     現在は常に true を返します。
	//
	//  @par
	//              dir_ と name_ は LN_MAX_PATH 分の文字を格納できるサイズが必要です。<br>
	//              <br>
	//              path_ がディレクトリパスだけの場合は name_ にディレクトリ名が
	//              格納されます。<br>
	//              <br>
	//              "C:/Source/Test.cpp"   => dir_ = "C:/Source",  name_ = "Test.cpp"<br>
	//              "C:\\Source\\Test.cpp" => dir_ = "C:\\Source", name_ = "Test.cpp"<br>
	//              "Test.cpp"             => dir_ = "",           name_ = "Test.cpp"<br>
	//*/
	//---------------------------------------------------------------------
	bool getDirPathAndFilePath( const char* path_, char* dir_, char* name_ );

	/// ファイルパスからディレクトリパスとファイルパスを返す ( wchar_t )
	bool getDirPathAndFilePath( const wchar_t* path_, wchar_t* dir_, wchar_t* name_ );



	//---------------------------------------------------------------------
	///**
	//  @brief      ファイルポインタの移動計算
	//
	//  @note
	//              現在の位置とデータ(ファイル)サイズ、オフセット、基準を受け取って
	//              新しいシーク位置を返す。
	//              ストリームクラスの seek() はほとんど同じ処理なので
	//              それらをまとめたもの。<br>
	//              新しい位置が範囲外の場合は修正される。
	//*/
	//---------------------------------------------------------------------
	int setSeekPoint( int now_point_, int max_size_, int offset_, int origin_ );


	//---------------------------------------------------------------------
	///**
	//  @brief      スラッシュをすべてバックスラッシュ化する
	//
	//  @param[in,out]  str_ : 操作対象の文字列
	//*/
	//---------------------------------------------------------------------
	inline void normalizePathName( lnChar* str_ )
	{
		for ( lnChar* c = str_; *c != _T( '\0' ); ++c )
		{
			if ( *c == _T( '/' ) )
			{
				*c = _T( '\\' );
			}
		}
	}

	//---------------------------------------------------------------------
	///**
	//  @brief      スラッシュをすべてバックスラッシュ化する
	//
	//  @param[in,out]  str_ : 操作対象の文字列
	//*/
	//---------------------------------------------------------------------
	inline void normalizePathName( LRefTString* str_ )
	{
		if ( str_ && str_->size() > 0 )
		{
			for ( lnChar* c = &(*str_)[ 0 ]; *c != _T( '\0' ); ++c )
			{
				if ( *c == _T( '/' ) )
				{
					*c = _T( '\\' );
				}
			}
		}
	}

	//---------------------------------------------------------------------
	///**
	//  @brief      ファイルサイズを取得する
	//*/
	//---------------------------------------------------------------------
	inline size_t getFileSize( FILE* stream_ )
	{
		fseek( stream_, 0, SEEK_END );
		size_t s = ftell( stream_ );
		fseek( stream_, 0, SEEK_SET );
		return s;
	}

	//---------------------------------------------------------------------
	///**
	//  @brief      ファイルパスの拡張子を取り除いたものを返す ( char )
	//*/
	//---------------------------------------------------------------------
	//ln_std_tstring getExtName( const char* path_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      ファイルパスの中からディレクトリパスの部分を取り出す ( char )
	//*/
	//---------------------------------------------------------------------
	//std::string getDirectoryPath( const char* path_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      ファイルパスの中からディレクトリパスの部分を取り出す
	//*/
	//---------------------------------------------------------------------
	//std::string getDirectoryPath( const std::string& path_ );




	//---------------------------------------------------------------------
	///**
	//  @brief      ディレクトリから直接ファイルを開いて入力ストリームを作成する
	//*/
	//---------------------------------------------------------------------
	LNRESULT createInStream( IInStream** stream_, const lnChar* filename_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      メモリ上に展開されたデータから入力ストリームを作成する
	//
	//  @par
	//              元のデータはストリームの作成直後に解放することができます。
	//*/
	//---------------------------------------------------------------------
	void createInStreamFromMemoryManaged( IInStream** stream_, const void* buffer_, int size_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      メモリ上に展開されたデータから入力ストリームを作成する
	//
	//  @par
	//              元のデータを単純に参照するだけのストリームです。
	//*/
	//---------------------------------------------------------------------
	void createInStreamFromMemoryUnManaged( IInStream** stream_, const void* buffer_, int size_ );



	//---------------------------------------------------------------------
	///**
	//  @brief      数値 ( 2バイト ) の読み込み
	//
	//  @param[in]  stream_ : 入力ファイルストリーム
	//
	//  @return     読み込んだ数値
	//
	//  @par
	//              現在のファイルポインタの位置から 2 バイト分の数値を読み込みます。
	//              呼び出した後はファイルポインタが 2 進みます。
	//*/
	//---------------------------------------------------------------------
	u16 readU16( IInStream* stream_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      数値 ( 4バイト ) の読み込み
	//
	//  @param[in]  stream_ : 入力ファイルストリーム
	//
	//  @return     読み込んだ数値
	//
	//  @par
	//              現在のファイルポインタの位置から 4 バイト分の数値を読み込みます。
	//              呼び出した後はファイルポインタが 4 進みます。
	//*/
	//---------------------------------------------------------------------
	u32 readU32( IInStream* stream_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      数値 ( 2バイト ) の読み込み ( ビッグエンディアンから )
	//
	//  @param[in]  stream_ : 入力ファイルストリーム
	//
	//  @return     読み込んだ数値
	//
	//  @par
	//              現在のファイルポインタの位置から 2 バイト分の数値を読み込みます。
	//              呼び出した後はファイルポインタが 2 進みます。
	//*/
	//---------------------------------------------------------------------
	u16 readU16Big( IInStream* stream_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      数値 ( 4バイト ) の読み込み ( ビッグエンディアンから )
	//
	//  @param[in]  stream_ : 入力ファイルストリーム
	//
	//  @return     読み込んだ数値
	//
	//  @par
	//              現在のファイルポインタの位置から 4 バイト分の数値を読み込みます。
	//              呼び出した後はファイルポインタが 4 進みます。
	//*/
	//---------------------------------------------------------------------
	u32 readU32Big( IInStream* stream_ );

	//---------------------------------------------------------------------
	//**
	//  @brief		ファイルの存在をチェックする
	//
	//  @param[in]  path_ : チェックするファイルパス
	//
	//  @retval     true  : 存在する
	//  @retval     false : 存在しない ( または path_ が NULL )
	//
	//  @par
	//              fopen() を使って開けるかどうかで確認しています。
	//*/
	//---------------------------------------------------------------------
	bool isExist( const lnChar* path_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      ファイルパスの拡張子を取り除いたものを返す
	//*/
	//---------------------------------------------------------------------
	LRefTString getExtName( const lnChar* path_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      ファイルパスの中からディレクトリパスの部分を取り出す
	//*/
	//---------------------------------------------------------------------
	LRefTString getDirectoryPath( const lnChar* path_ );


	//---------------------------------------------------------------------
	///**
	//  @brief      ファイルパスの中から拡張子を含むファイル名の部分を返す
	//*/
	//---------------------------------------------------------------------
	LRefTString getFileName( const lnChar* path_ );

	//---------------------------------------------------------------------
	///**
	//  @brief      ファイルパスからディレクトリパスとファイルパスを返す
	//
	//  @param[in]  path_     : 操作元のファイルパス
	//  @param[in]  dir_      : ディレクトリパスを格納するバッファへのポインタ または NULL
	//  @param[in]  name_     : ファイル名を格納するバッファへのポインタ または NULL
	//
	//  @return     現在は常に true を返します。
	//
	//  @par
	//              dir_ と name_ は LN_MAX_PATH 分の文字を格納できるサイズが必要です。<br>
	//              <br>
	//              path_ がディレクトリパスだけの場合は name_ にディレクトリ名が
	//              格納されます。<br>
	//              <br>
	//              "C:/Source/Test.cpp"   => dir_ = "C:/Source",  name_ = "Test.cpp"<br>
	//              "C:\\Source\\Test.cpp" => dir_ = "C:\\Source", name_ = "Test.cpp"<br>
	//              "Test.cpp"             => dir_ = "",           name_ = "Test.cpp"<br>
	//*/
	//---------------------------------------------------------------------
	bool getDirPathAndFilePath( const char* path_, char* dir_, char* name_ );


	//---------------------------------------------------------------------
	///**
	//  @brief      スラッシュをすべてバックスラッシュ化して、先頭が ".\\" だった場合は取り除く
	//
	//  @param[in,out]  str_ : 操作対象の文字列
	//*/
	//---------------------------------------------------------------------
	inline void normalizePathName( ln_std_tstring* out_, const lnChar* path_ )
	{
		if ( _tcslen( path_ ) < 2 )
		{
			(*out_) = path_;
			return;
		}

		// 先頭が "./" ".\" の場合は飛ばす (././ とか続けて書くことはまずないと思うので、先頭2文字だけチェック)
		if ( path_[ 0 ] == _T( '.' ) && ( path_[ 1 ] == _T( '/' ) || path_[ 1 ] == _T( '\\' ) ) )
		{
			(*out_) = &path_[ 2 ];
		}
		else
		{
			(*out_) = path_;
		}

		// バックスラッシュ化
		for ( size_t i = 0; i < out_->size(); ++i )
		{
			if ( (*out_)[ i ] == _T( '/' ) )
			{
				(*out_)[ i ] = _T( '\\' );
			}
		}
	}

	//---------------------------------------------------------------------
	///**
	//  @brief      できるだけ単純なファイルパスを作成する
	//
	//  @par
	//              path_ の名前のうち、'/' は '\\' に変換済みであること。
    //              "..\..\TestMat\Audio\test.ogg"          => ..\..\TestMat\Audio\test.ogg
    //              "Data\Graphics\Model\..\Texture\t1.png" => Data\Graphics\Texture\t1.png
    //              "C:\Data\\..\Model"                     => C:\Model
	//*/
	//---------------------------------------------------------------------
	void makeFileName( ln_std_tstring* out_, const lnChar* path_ );

	/// CR の数を数える (アーカイブファイル内のファイルに対して使用)
	inline int getFileSize( lnChar* str_, size_t len_ )
	{
		int cnt = 0;
		lnChar* c = str_;
		for (; (*c) && (len_ > 0); +c, --len_)
		{
			if (*c == 13) ++cnt;
		}
		return cnt;
	}

	/// path がフルパスであるかを判定する ("C:/AAA"、"/AAA" 両方判定)
	bool isFullPath( const lnChar* path );

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace File
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================