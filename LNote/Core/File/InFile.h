//=============================================================================
//【 InFile 】
//-----------------------------------------------------------------------------
///**
//  @file       InFile.h
//  @brief      InFile
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Interface.h"
#include "ASyncProcessObject.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File
{
class Manager;

//=============================================================================
// ■ InFile クラス
//-----------------------------------------------------------------------------
///**
//  @brief      読み込み用のファイルを扱うクラス
//*/
//=============================================================================
class InFile
    : public Base::ReferenceObject
    , public IInFile
    , public ASyncProcessObject
{
public:

    /// コンストラクタ
	InFile( Manager* manager_, IInStream* stream_ );

	/// デストラクタ
	virtual ~InFile();

public:

    /// ファイルサイズの取得
    virtual int getSize() { return mInStream->getSize(); }

    /// ファイルポインタの位置の取得
	virtual int getPosition() { return mInStream->getPosition(); }

    /// データの読み込み
    virtual int read( void* buffer_, int buffer_size_, int read_size_ );

    /// ファイルポインタの設定
    virtual void seek( int offset_, int origin_ ) { mInStream->seek( offset_, origin_ ); }

    /// ファイルの内容を全て読み込む
	virtual void load( bool async_ );

    /// load() で読み込まれたデータの取得
    virtual void* getData() { return mData; }

    /// load() での非同期読み込みの完了を判定する
    virtual bool isLoadFinished() { return ASyncProcessObject::getProcessState() == LN_ASYNCSTATE_FINISH || mLoadFinished; } 

    /// load() での読み込みでエラーが発生したかを判定する
    virtual bool isError() { return ASyncProcessObject::isError(); }

    LN_REFOBJ_METHODS;

protected:

    /// 非同期処理コールバック
    virtual bool operator()() { return onLoading(); }

	/// 全データ読み込み
	virtual bool onLoading();

private:
			
    IInStream*  mInStream;      ///< 読むストリーム(FILE* とか)が入っている入力ストリームクラス
    lnByte*     mData;          ///< load() で読み込んだデータ
    bool        mLoadFinished;  ///< 読み込みの終了フラグ
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