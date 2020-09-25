//=============================================================================
//【 GameAudio 】
//-----------------------------------------------------------------------------
///**
//  @file       GameAudio.h
//  @brief      GameAudio
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Geometry/Vector3.h"
#include "../Thread/Mutex.h"
#include "Interface.h"
#include "Sound.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Audio
{
class Manager;

//=============================================================================
// ■ GameAudio クラス
//-----------------------------------------------------------------------------
///**
//	@brief      ゲーム用の音声の演奏を扱うクラス
//
//  @par
//              このクラスのインスタンスは Engine::Audio::Manager クラスから取得します。
//*/
//=============================================================================
class GameAudio
    : public Base::ReferenceObject
    , public IGameAudio
{
public:

	/// コンストラクタ
	GameAudio( Manager* mamager_ );

	/// デストラクタ
	~GameAudio();

    LN_REFOBJ_METHODS;

public:

    /// 管理クラスの取得
    Manager* getManager() const { return mManager; }

    /// BGM を演奏する
	virtual LNRESULT playBGM( const lnChar* filename_, int volume_ = 100, int pitch_ = 100, int fade_time_ = 0 );

    /// 指定された ISound を BGM として演奏する
	virtual LNRESULT playBGMFromSound( ISound* sound_, int volume_ = 100, int pitch_ = 100  , int fade_time_ = 0 );

    /// BGM の演奏を停止する
	virtual void stopBGM( int fade_time_ = 0 );

    /// BGS ( 環境音 ) を演奏する
	virtual LNRESULT playBGS( const lnChar* filename_, int volume_ = 100, int pitch_ = 100, int fade_time_ = 0 );

    /// 指定された ISound を BGS として演奏する
	virtual LNRESULT playBGSFromSound( ISound* sound_, int volume_ = 100, int pitch_ = 100  , int fade_time_ = 0 );

    /// BGS の演奏を停止する
	virtual void stopBGS( int fade_time_ = 0 );

    // ME ( 効果音楽 ) を演奏する
	virtual LNRESULT playME( const lnChar* filename_, int volume_ = 100, int pitch_ = 100 );

    /// 指定された ISound を ME として演奏する
	virtual LNRESULT playMEFromSound( ISound* sound_, int volume_ = 100, int pitch_ = 100 );

    /// ME の演奏を停止する
	virtual void stopME();

    /// SE を演奏する
	virtual LNRESULT playSE( const lnChar* filename_, int volume_ = 100, int pitch_ = 100 );

    /// SE を 3D 空間上で演奏する
    virtual LNRESULT playSE( const lnChar* filename_, const LVector3& position_, float distance, int volume_ = 100, int pitch_ = 100 );

    /// 指定された ISound を SE として演奏する
	virtual LNRESULT playSEFromSound( ISound* sound_, int volume_ = 100, int pitch_ = 100 );

    /// 全ての SE の演奏を停止する
	virtual void stopSE();

    /// ME 演奏時の BGM のフェード時間を設定する
    virtual void setMEFadeState( int begin_, int end_ );

    /// 同名 BGM の演奏再開フラグの設定
    virtual void setEnableBGMRestart( bool flag_ ) { mBGMRestart = flag_; }

    /// 同名 BGS の演奏再開フラグの設定
	virtual void setEnableBGSRestart( bool flag_ ) { mBGSRestart = flag_; }

public:

    /// 更新
	LNRESULT polling();
    
private:

    /// 再生終了時解放リストに追加する
    void _pushReleaseAtPlayEndList( ISound* sound_ );

private:

    typedef std::list< ISound* >    ReleaseAtPlayEndList;

public:

	Manager*	                mManager;
    Thread::Mutex*              mLock;
    ReleaseAtPlayEndList        mReleaseAtPlayEndList;  ///< 再生終了時に解放する音声リスト
	ISound*		                mBGM;
    ISound*		                mBGS;
    ISound*                     mME;
    ln_std_tstring              mBGMName;
    ln_std_tstring              mBGSName;
    int                         mBGMVolume;
    int                         mBGMPitch;
    int                         mBGSVolume;
    int                         mBGSPitch;
    int                         mBGMFadeOutTime;
    int                         mBGMFadeInTime;
    bool                        mMEPlaying;
    bool                        mBGMRestart;
    bool                        mBGSRestart;
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