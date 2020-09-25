//=============================================================================
//【 DirectMusic 】
//-----------------------------------------------------------------------------
///**
//  @file       DirectMusic.h
//  @brief      DirectMusic
//  @version    1.0
//  @date       2011/08/04
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "dmusic/dmusici.h"
#include <list>
#include "../../../Thread/Thread.h"
#include "../../../Thread/Mutex.h"
#include "../../../Thread/EventFlag.h"
#include "../../Interface.h"
#include "../AudioSource.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Audio
{
namespace XAudio2
{

//=============================================================================
// ■ DirectMusicSegment クラス
//-----------------------------------------------------------------------------
///**
//  @brief      DirectMusic 上での再生シーケンスひとつ分を扱うクラス
//
//  @par
//              音声ひとつ分を再生するために必要な一連のインターフェースを
//              扱うクラスです。
//              ( Performance、Segment 等をまとめたもの )
//              IXAudio2SourceVoice のようなイメージで使えるようにしてあります。
//
//  @note
//              DirectMusic の仕様上、パフォーマンスを作成する前にセグメントを
//              作成することができる。スレッドで DirectMusic を初期化する場合、
//              初期化前にセグメントを作成しておけば時間の節約とかになるかもしれないけど、
//              セグメントをパフォーマンスよりも先に作成すると不具合が起こる
//              ( 全部ピアノ音源になるとか )
//*/
//=============================================================================
class DirectMusicSegment
    : public Base::NonCopyable
{
public:

    //---------------------------------------------------------------------
	///**
    //  @brief      コンストラクタ
    //
    //  @par
    //              audio_source_ は保持しないため、参照カウントは変化させません。
    //              ただし、コンストラクタに渡す前に fillBuffer() を呼んでおく必要があります
    //*/
	//---------------------------------------------------------------------
    DirectMusicSegment( IDirectMusicPerformance8* dm_performance_, const Midi* audio_source_ );

    /// デストラクタ
    virtual ~DirectMusicSegment();

public:

    /// 音量の設定 ( 100.0 ～ 0.0 )
    void setVolume( float volume_ );

    /// ピッチの設定 ( 50 ～ 200 が基本。範囲外も一応可。 )
    void setPitch( float pitch_ );

    /// 全体の再生時間の取得 ( ミュージックタイム単位 )
    u32 getTotalTime() const;

    /// 再生位置の取得 ( ミュージックタイム単位 )
    u32 getPlayPosition() const;

    /// 再生位置の設定 ( ミュージックタイム単位 )
    void setPlayPosition( u32 time_ );

    /// 再生中かを判定 (音が鳴っているか)
    bool isPlaying() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ループ情報の設定
    //
    //  @param[in]  is_loop_ : true の場合ループ再生する
    //  @param[in]  begin_   : ループ範囲の開始位置 ( ミュージックタイム単位 )
    //  @param[in]  length_  : ループ範囲の長さ( ミュージックタイム単位 )
    //
    //  @par
    //              length_ が 0 の場合は begin_ の位置から終端まで再生します。
    //*/
	//---------------------------------------------------------------------
	void DirectMusicSegment::setLoopState( bool is_loop_, u32 begin_, u32 length_ );

    void play();

    void stop();

private:

    IDirectMusicPerformance8*   mDMPerformance;
    IDirectMusicAudioPath*      mDMAudioPath;
    IDirectMusicSegment8*		mDMSegment;
	IDirectMusicSegmentState8*	mDMSegmentState;
    IDirectSoundBuffer8*        mDSSoundBuffer;
};

//=============================================================================
// ■ DirectMusicManager クラス
//-----------------------------------------------------------------------------
///**
//  @brief      DirectMusic の管理クラス
//
//  @par
//              IDirectMusic8 インターフェイスはひとつのアプリケーションにつき
//              ひとつしか作成できないため、このクラスはシングルトンで実装してます。
//              <br>
//              http://msdn.microsoft.com/ja-jp/library/cc353890.aspx <br>
//
//  @note
//              ◆ DirectMusic の解放
//                  DirectMusic を解放するときはウィンドウが存在していないと
//                  Release() で応答が無くなる ( 動作停止はしない。多分内部でデッドロックしてる )<br>
//                  ↑実は CoUninitialize を呼んでしまった後に解放してただけだった。
//                    ウィンドウはすでに削除されていても解放は可能。
//              
//*/
//=============================================================================
class DirectMusicManager
    : private Base::NonCopyable
{
public:

    /// initialize() に渡す初期化データ
    struct InitData
    {
        Base::LogFile*          LogFile;
        HWND                    WindowHandle;
        LNDirectMusicUsingMode  DMInitMode;
    };

    /// 再生要求を持つクラスの基底
    class PlayerObject
    {
    public:

        PlayerObject(){}

        virtual ~PlayerObject();

        /// 再生要求リストに追加されている状態で DirectMusic の初期化が完了したときに呼ばれる
        virtual void onFinishDMInit( IDirectMusicPerformance8* dm_performance_ ) = 0;
    };

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      初期化
    //
    //  @param[in]  init_data_ : 初期化データ
    //
    //  @par
    //              DMInitMode が LN_DMINITMODE_NOTUSE の場合はインスタンスを
    //              作成しません。getInstance() は NULL を返します。
    //*/
	//---------------------------------------------------------------------
	static LNRESULT initialize( const InitData& init_data_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      終了処理
    //*/
	//---------------------------------------------------------------------
	static void finalize();

    //---------------------------------------------------------------------
	///**
    //  @brief      インスタンスへのポインタの取得
    //*/
	//---------------------------------------------------------------------
    static DirectMusicManager* getInstance() { return sInstance; }

public:

    //---------------------------------------------------------------------
	/**
    //  @brief     初期化が完了しているかを判定する
    //*/
	//---------------------------------------------------------------------
    bool isInitialized() const { return mPerformanceInited->isTrue(); }

    //---------------------------------------------------------------------
	/**
    //  @brief      エラー状態の取得
    //
    //  @return     エラー番号(内部用)。 正常な場合は 0
    //*/
	//---------------------------------------------------------------------
    u32 getErrorState() const;

    //---------------------------------------------------------------------
	/**
    //  @brief      再生要求の追加
    //
    //  @par
    //              既に同じオブジェクトが追加されている場合はなにもしません。
    //*/
	//---------------------------------------------------------------------
	void addPlayRequest( PlayerObject* obj_ );

    //---------------------------------------------------------------------
	/**
    //  @brief      再生要求の削除
    //
    //  @par
    //              DirectMusic の初期化中にオーディオプレイヤーが解放された
    //              時に、この関数でリストから外しますが、
    //              PlayerObject クラスのデストラクタで呼ばれるため
    //              通常は呼び出す必要はありません。
    //*/
	//---------------------------------------------------------------------
	void removePlayRequest( PlayerObject* obj_ );

    //---------------------------------------------------------------------
	/**
    //  @brief      IDirectMusicPerformance8 の作成
    //
    //  @par
    //              isInitialized() によって初期化の完了を確認してから
    //              呼び出してください。
    //*/
	//---------------------------------------------------------------------
    IDirectMusicPerformance8* createDMPerformance();

    //---------------------------------------------------------------------
	/**
    //  @brief     再生要求リストをチェックして、可能なら再生する
    //*/
	//---------------------------------------------------------------------
    void polling();

private:

    /// コンストラクタ
    DirectMusicManager();

    /// デストラクタ
    ~DirectMusicManager();

    /// 初期化
    LNRESULT _initialize( const InitData& init_data_ );

    /// ひとつめのパフォーマンスの初期化 ( 別スレッドで初期化する場合はそこから呼ばれる )
	void _initPerformance();

    /// (参照カウントなどに変化はなし)
    IDirectMusicPerformance8* _createDMPerformance();

private:

    /// 最初の IDirectMusicPerformance8 の初期化スレッド
    class PerformanceInitThread
        : public Thread::ThreadObject
    {
    public:

        /// コンストラクタ
        PerformanceInitThread()
		{}

        virtual void run()
        {
            CoInitializeEx( NULL, COINIT_MULTITHREADED );

			DirectMusicManager::getInstance()->_initPerformance();
			
			CoUninitialize();
        }
    };

private:

    typedef std::list< PlayerObject* > PlayRequestList;

private:

    HWND				        mWindowHandle;
	LNDirectMusicUsingMode	    mInitMode;
	IDirectSound8*		        mDirectSound;
	IDirectMusic8*		        mDirectMusic;
    IDirectMusicPerformance8*   mFirstPerformance;      ///< 初期化スレッドによって作成された IDirectMusicPerformance8

    PerformanceInitThread*      mInitThread;            ///< 初期化スレッド
    Thread::EventFlag*          mPerformanceInited;     ///< 初期化済みフラグ
    Thread::Mutex*              mLock;
    Thread::Mutex*              mListLock;

    PlayRequestList             mPlayRequestList;

    u32                         mErrorState;

    Base::LogFile*              mLogFile;

    static DirectMusicManager* sInstance;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace XAudio2
} // namespace Audio
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================