//=============================================================================
//【 Manager 】
//-----------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include <list>
#include <string>
#include <stack>
#include "../Thread/Mutex.h"
#include "../Thread/EventFlag.h"
#include "../Thread/Thread.h"
#include "Interface.h"
#include "StreamObject.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File
{
class Archive;



//=============================================================================
// ■ Manager クラス
//-----------------------------------------------------------------------------
///**
//  @brief		ファイル入出力の管理クラス
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
    , public IManager
{
public:

	/// 初期化データ
	struct InitData
	{
        Base::LogFile*  LogFile;        ///< ログを出力するログファイルオブジェクト
        LNArchiveOpenDataList   ArchiveList;    ///< アーカイブの設定データのリスト
		LNFileAccessMode        AccessMode;     ///< ファイルアクセスモード
	};

public:

    Manager();

	virtual ~Manager();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    void initialize( const InitData& init_data_ );

    /// 終了処理
    void finalize();

    /// 読み込み用カレントパスの設定 (別のディレクトリからデフォルトへのパスを渡しても "" にならないバグあり)
	virtual void setCurrentPath( const lnChar* directory_ );

    /// カレントパスの取得
    const lnChar* getCurrentPath() const { return mCurrentPath.c_str(); }

    /// filename_ にアクセスするための絶対パスの取得 (ライブラリシステム内の絶対パス。先頭に '$' が付く。既に'$' がついている場合はそのまま返す)
    LRefTString getAbsolutePath( const lnChar* filename_ );

    /// InFile の作成
	virtual LNRESULT createInFile( IInFile** file_, const lnChar* filename_, LNFileOpenMode mode_ = LN_OPENMODE_BINARY );

    /// OutFile の作成
	virtual LNRESULT createOutFile( IOutFile** file_, const lnChar* filename_, const void* buffer_, int size_ );

    /// 読み込み用カレントパスの移動
	virtual void moveCurrentPath( const lnChar* directory_ );

    /// 読み込み用カレントパスの復帰
	virtual void returnCurrentPath();

    /// ファイルアクセスのロック (moveCurrentPath() での変更を排他的にするために使う)
	virtual void lock();

    /// ファイルアクセスのアンロック
	virtual void unlock();

    /// 非同期処理のリクエスト
	virtual void requestASyncProcess( ASyncProcessObject* obj_ );

    /// キューにある非同期処理のリクエストを外す
	virtual void removeASyncProcess( ASyncProcessObject* obj_ );

public:

    /// ポーリング (LoadThread クラスから呼ばれる)
	void polling();

    /// 終了要求があるかを判定する (ローディングスレッドが終了チェックするために呼ぶ)
	bool isEndRequested() const { return mEndFlag->isTrue(); }

private:

    /// ローディング処理を呼び出すスレッド
    class LoadThread
        : public Thread::ThreadObject
	{
	public:

		/// コンストラクタ
		LoadThread( Manager* manager_ )
			: mManager( manager_ )
		{}

		/// デストラクタ
		~LoadThread()
		{
			wait();
			mManager = NULL;
		}

	private:

		virtual void run()
		{
			while ( mManager->isEndRequested() == false )
			{
				mManager->polling();
			}
		}
		Manager* mManager;
	};

	typedef std::list< ASyncProcessObject* >    ASyncProcessList;
    typedef std::vector< IArchive* >            ArchiveList;

private:

    Base::LogFile*                  mLogFile;           ///< ログを出力するログファイルオブジェクト
    Archive*                        mArchives;
	int					            mArchiveNum;

    ArchiveList                     mArchiveList;       ///< 時間があるときに mArchives はこっちに含める

    Thread::Mutex*                  mLoadThreadLock;    ///< mASyncProcessRequests へのアクセスブロック用
    Thread::EventFlag*              mEndFlag;
	LoadThread*			            mLoadThread;
    Thread::Mutex*                  mLock;

    ASyncProcessList                mASyncProcessRequests;

    ln_std_tstring                  mCurrentPath;       ///< 仮想カレントディレクトリ (EXEのディレクトリは ""で、それ以外は終端が必ず \\)
    std::stack< ln_std_tstring >    mPathStack;
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