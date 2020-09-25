//=============================================================================
//【 StreamObject 】
//-----------------------------------------------------------------------------
///**
//  @file       StreamObject.h
//  @brief      StreamObject
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
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

//=============================================================================
// ■ InFileStream クラス
//-----------------------------------------------------------------------------
///**
//  @brief      単純な入力ファイルのクラス (バイナリ形式)
//*/
//=============================================================================
class InFileStream
    : public IInStream
    , public Base::ReferenceObject
{
public:

    /// コンストラクタ
	InFileStream();

	/// デストラクタ
	virtual ~InFileStream();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    void initialize( const lnChar* filename_ );

	/// ファイル ( データ ) サイズの取得
    virtual int getSize() { return mSize; }

	/// ファイルポインタの位置の取得
	virtual int getPosition() { return ftell( mStream ); }

    /// データの読み込み
	virtual int read( void* buffer_, int buffer_size_, int read_size_ );

	/// ファイルポインタの設定
	virtual void seek( int offset_, int origin_ );

    

private:

    /// ファイルを閉じる
    void _close();

private:

    FILE*   mStream;
    int     mSize;
};

//=============================================================================
// ■ InManagedMemoryStream クラス
//-----------------------------------------------------------------------------
///**
//  @brief      メモリ上に展開されたデータを入力ストリームとして扱うクラス
//
//  @note
//              initialize() で受け取ったデータは、それと同じ大きさの
//              メモリを確保してコピーします。<br>
//              元のメモリは initialize() の直後に解放することができます。
//*/
//=============================================================================
class InManagedMemoryStream
    : public IInStream
    , public Base::ReferenceObject
{
public:

	InManagedMemoryStream();

	virtual ~InManagedMemoryStream();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    void initialize( const void* buffer_, int size_ );

	/// ファイル ( データ ) サイズの取得
    virtual int getSize() { return mSize; }

	/// ファイルポインタの位置の取得
    virtual int getPosition() { return mSeekPoint; }

    /// データの読み込み ( read_size_ = -1 ですべてのデータを読み込む )
	virtual int read( void* buffer_, int buffer_size_, int read_size_ );

	/// ファイルポインタの設定
	virtual void seek( int offset_, int origin_ );

private:

    void _dispose();

private:

    int     mSize;          ///< データサイズ
    char*   mBuffer;        ///< データ本体
    int     mSeekPoint;     ///< シーク位置

};

//=============================================================================
// ■ InUnManagedMemoryStream クラス
//-----------------------------------------------------------------------------
///**
//  @brief      メモリ上に展開されたデータを入力ストリームとして扱うクラス
//*/
//=============================================================================
class InUnManagedMemoryStream
    : public IInStream
    , public Base::ReferenceObject
{
public:

	InUnManagedMemoryStream();

	virtual ~InUnManagedMemoryStream();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    void initialize( const void* buffer_, int size_ );

	/// ファイル ( データ ) サイズの取得
    virtual int getSize() { return mSize; }

	/// ファイルポインタの位置の取得
    virtual int getPosition() { return mSeekPoint; }

    /// データの読み込み ( read_size_ = -1 ですべてのデータを読み込む )
	virtual int read( void* buffer_, int buffer_size_, int read_size_ );

	/// ファイルポインタの設定
	virtual void seek( int offset_, int origin_ );

private:

    int             mSize;          ///< データサイズ
    const lnByte*   mBuffer;        ///< データ本体
    int             mSeekPoint;     ///< シーク位置

};



class IArchive
{
public:

    /// 入力ストリームの作成
    virtual IInStream* createStream( const lnChar* filename_ ) = 0;
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