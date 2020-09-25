//=============================================================================
//【 OutFile 】
//-----------------------------------------------------------------------------
///**
//  @file       OutFile.h
//  @brief      OutFile
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
// ■ OutFile クラス
//-----------------------------------------------------------------------------
///**
//  @brief      書き込み用のファイルを扱うクラス
//*/
//=============================================================================
class OutFile
    : public Base::ReferenceObject
    , public IOutFile
    , public ASyncProcessObject
{
public:

    /// コンストラクタ
	OutFile( Manager* manager_, const lnChar* filename_, const void* buffer_, int size_ );

	/// デストラクタ
	virtual ~OutFile();

public:

    /// データを保存する
	virtual void save( bool async_ );

	/// save() での非同期書き込みの完了を判定する
	virtual bool isSaveFinished() { return ASyncProcessObject::getProcessState() == LN_ASYNCSTATE_FINISH || mSaveFinished; } 

    /// save() での書き込みでエラーが発生したかを判定する
	virtual bool isError() { return ASyncProcessObject::isError(); }

    LN_REFOBJ_METHODS;

protected:

    /// 非同期処理コールバック
    virtual bool operator()() { return onSaving(); }

	/// 全データ読み込み
	virtual bool onSaving();

private:
			
    ln_std_tstring  mFilename;
    const void*     mData;
    int		        mSize;
    bool            mSaveFinished;
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