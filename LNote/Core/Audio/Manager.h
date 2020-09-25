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
#include "../Base/Container/NodeList.h"
#include "../Thread/Thread.h"
#include "../Thread/EventFlag.h"
#include "../Thread/Mutex.h"
#include "../File/Interface.h"
#include "../System/Interface.h"
#include "Core/AudioResourceManager.h"
#include "Sound.h"
#include "GameAudio.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File { class Manager; }

namespace Audio
{
//class ISound;
class GameAudio;

//=============================================================================
// ■ Manager クラス
//-----------------------------------------------------------------------------
///**
//  @brief      オーディオ機能の管理クラス
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
    , public IManager
{
public:

	/// initialize() に渡す初期化データ
	struct InitData
	{
        Base::LogFile*              LogFile;
        File::Manager*              FileIOManager;          ///< ファイルからの読み込みに使うファイル管理クラス
        u32                         AudioSourceCacheSize;   ///< キャッシュサイズ (ファイル数)
        LNDirectMusicUsingMode      DMInitMode;             ///< DirectMusic の初期化方法
        System::IWindow*            Window;                 ///< DirectMusic の初期化に使うウィンドウハンドルを持つウィンドウクラス

        /// デフォルト値
        InitData()
            : LogFile               ( NULL )
            , FileIOManager         ( NULL )
            , AudioSourceCacheSize  ( 16 )
            , DMInitMode            ( LN_DMUSINGMODE_NOTUSE )
            , Window                ( NULL )
        {}
    };

public:

    /// コンストラクタ
	Manager();

	/// デストラクタ
	virtual ~Manager();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

    /// 終了処理 ( GameAudio の解放 )
    void finalize();

    /// GameAudio クラスの取得
    virtual IGameAudio* getGameAudio() { return mGameAudio; }

    /// デバイスクラスの取得
    virtual IAudioDevice* getAudioDevice() { return mAudioDevice; }

    /// リソース管理クラスの取得
    virtual IResourceManager* getResourceManager() { return mResourceManager; }

    /// ISound の作成 ( stream_ = NULL でキーを使った検索だけ行う )
    virtual LNRESULT createSound( ISound** sound_, File::IInStream* stream_, LNSoundPlayType type_, bool enable_3d_, LSharingKey key_ );

    /// ISound の作成 ( ファイル名指定 )
    virtual LNRESULT createSound( ISound** sound_, const char* filename_, LNSoundPlayType type_, bool enable_3d_ );

    /// ISound の作成 ( IAudioSource 指定 )
    virtual LNRESULT createSound( ISound** sound_, IAudioSource* source_, LNSoundPlayType type_, bool enable_3d_ );

    /// グループの停止
    virtual void stopGroup( u32 group_ );

public:

    /// Sound のデストラクタから呼ばれる
    void removeSound( Sound* sound_ );

    /// フレーム更新 ( スレッドを使ってポーリングしている場合はそっちから呼ばれる )
    void update( float elapsed_time_ );

    /// 終了要求が来ているかを判定する
    bool isEndRequest() const { return mEndRequest->isTrue(); }


private:

    /// ポーリングスレッドクラス
    class PollingThread
        : public Thread::ThreadObject
	{
	public:

		/// コンストラクタ
		PollingThread( Manager* manager_ )
            : mManager  ( manager_ )
        {}

		/// デストラクタ
		~PollingThread()
		{
			// 再生終了を待つ
			// ( このデストラクタは、Manager のデストラクタから呼ばれる。
			//   その際、Manager 側では delete の直前に mEndRequest を true にする。)
			wait();
			mManager = NULL;
		}

		/// スレッド関数
		virtual void run()
		{
#if defined(LNOTE_DIRECTX)
            // COM 初期化
            HRESULT hr = ::CoInitializeEx( NULL, COINIT_MULTITHREADED );
#endif
			// 終了要求が来るまで回し続ける
			while( !mManager->isEndRequest() )
			{
                mManager->update( 0.020f );
                Thread::sleep( 20 );
			}
#if defined(LNOTE_DIRECTX)
            ::CoUninitialize();
#endif
		}

	private:

		Manager* mManager;		///< Audio::Manager のインスタンスへのポインタ。コンストラクタで受け取る
	};

private:

    typedef Base::NodeList< Sound > SoundList;

private:

    Base::LogFile*          mLogFile;
    IAudioDevice*           mAudioDevice;
    ResourceManager*        mResourceManager;
    SoundList               mSoundList;

    GameAudio*              mGameAudio;
    File::Manager*          mFileIOManager;

    PollingThread*	        mThread;		        ///< ポーリングスレッドクラス
    Thread::EventFlag*	    mEndRequest;	        ///< ポーリングスレッドに終了を伝えるためのフラグ
    Thread::Mutex*          mLock;

    u32                     mOnMemoryLimitSize;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Audio
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================