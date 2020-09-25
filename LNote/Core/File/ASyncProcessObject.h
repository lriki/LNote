//=============================================================================
//【 ASyncProcessObject 】
//-----------------------------------------------------------------------------
///**
//  @file       ASyncProcessObject.h
//  @brief      ASyncProcessObject
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Thread/EventFlag.h"
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
// ■ ASyncProcessObject クラス
//-----------------------------------------------------------------------------
///**
//  @brief      非同期処理を行う関数オブジェクトクラスの基底
//*/
//=============================================================================
class ASyncProcessObject
{
protected:

	/// コンストラクタ
	ASyncProcessObject( IManager* manager_ );

	/// デストラクタ
	virtual ~ASyncProcessObject();

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      実行する関数
    //
    //  @par
    //              処理が失敗した場合は false を返すようにします。
    //*/		
    //---------------------------------------------------------------------
    virtual bool operator()() = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      実行開始
    //*/		
    //---------------------------------------------------------------------
	void start();

    //---------------------------------------------------------------------
	///**
    //  @brief      開始したリクエスト実行中の場合、終了するまで待つ
    //*/		
    //---------------------------------------------------------------------
	void wait() { _m.Finished->wait(); }

    //---------------------------------------------------------------------
	///**
    //  @brief      管理クラスへのポインタの取得
    //*/		
    //---------------------------------------------------------------------
    IManager* getManager() const { return _m.Manager; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      状態を調べる
    //
    //  @retval     true  : 読み込みが完了している
    //  @retval     false : 読み込み中
    //*/
    //---------------------------------------------------------------------
    LNASyncProcessState getProcessState() const { return _m.State; }

    //---------------------------------------------------------------------
	///**
    //  @brief      エラーが発生したかを判定する
    //
    //  @retval     true  : エラーが発生した
    //  @retval     false : 正常
    //*/		
    //---------------------------------------------------------------------
    bool isError() const { return _m.Failed->isTrue(); }

private:

    struct
    {
	    IManager*	                    Manager;    ///< 管理クラス
        volatile LNASyncProcessState    State;      ///< 状態
        Thread::EventFlag*	            Finished;   ///< 非同期読み込みが完了したかどうかのフラグ (開始時 true)
        Thread::EventFlag*	            Failed;     ///< エラーが発生した場合 treu
        bool                            Requested;  ///< start() を呼んだ場合 true
    } _m;

	friend class Manager;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
// コンストラクタ
inline ASyncProcessObject::ASyncProcessObject( IManager* manager_ )
{
    _m.Manager   = manager_;
    _m.State     = LN_ASYNCSTATE_WAIT;
    _m.Finished  = LN_NEW Thread::EventFlag( true );
    _m.Failed    = LN_NEW Thread::EventFlag( false );
    _m.Requested = false;

    LN_SAFE_ADDREF( _m.Manager );
}

// デストラクタ
inline ASyncProcessObject::~ASyncProcessObject()
{
    if ( _m.Manager )
    {
        // 読み込みリクエストから外す
        _m.Manager->removeASyncProcess( this );
    }

    if ( _m.Finished )
    {
	    // 完了していない場合は終了を待つ
	    _m.Finished->wait();
    }

	LN_SAFE_RELEASE( _m.Finished );
    LN_SAFE_RELEASE( _m.Failed );
    LN_SAFE_RELEASE( _m.Manager );
}

// 非同期処理をリクエストする
inline void ASyncProcessObject::start()
{
    if ( _m.State == LN_ASYNCSTATE_WAIT )
    {
        _m.Manager->requestASyncProcess( this );
    }
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace File
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================