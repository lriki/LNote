//=============================================================================
//【 SLESAudioPlayer 】
//-----------------------------------------------------------------------------
///**
//  @file       SLESAudioPlayer.h
//  @brief      SLESAudioPlayer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "SLESAudioDevice.h"
#include "../../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Audio
{
namespace SLES
{

//=============================================================================
// ■ AudioPlayerBase クラス
//-----------------------------------------------------------------------------
///**
//  @brief      音声の再生を扱うクラスの基底
//*/
//=============================================================================
class AudioPlayerBase
    : public IAudioPlayer
    , public Base::ReferenceObject
    , public Base::NodeList< AudioPlayerBase >::Node
{
public:

    /// IXAudio2SourceVoice の取得
    //virtual IXAudio2SourceVoice* getXAudio2SourceVoice() const = 0;

    /// 3D 音源の情報の取得
    virtual EmitterState* getEmitterState() const = 0;

    LN_REFOBJ_METHODS;
};

//=============================================================================
// ■ OnMemory クラス
//-----------------------------------------------------------------------------
///**
//  @brief      オンメモリデータの再生を扱うクラス
//*/
//=============================================================================
class OnMemory
    : public AudioPlayerBase
{
public:

	/// コンストラクタ
	OnMemory( AudioDevice* manager_ );

protected:

	/// デストラクタ
	virtual ~OnMemory();

public:

	/// 初期化
	virtual LNRESULT initialize( IAudioSource* audio_source_, bool enable_3d_ = false );

    /// 設定されているオーディオソースの取得 ( 今のところ参照カウントは増やさない )
    virtual IAudioSource* getAudioSource() const { return mAudioSource; }

	/// 音量の取得
	virtual int getVolume() const;

	/// 音量の設定
	LNRESULT setVolume( int volume_ );

	/// ピッチの取得
	virtual int getPitch() const;

	/// ピッチの設定
	LNRESULT setPitch( int pitch_ );

	/// 再生中判定
	virtual bool isPlaying() const;

	/// 一時停止中判定
	virtual bool isPausing() const;

    /// 再生したサンプル数の取得
    virtual u64 getPlayedSamples() const;

    /// ループ再生の有効設定
    virtual void loop( bool loop_ ) { mIsLoop = loop_; }

    /// ループ再生が有効かを判定する
    virtual bool isLoop() { return mIsLoop; }

	/// ループ情報の設定
	virtual LNRESULT setLoopState( u32 loop_begin_, u32 loop_length_ );

	/// 再生
	virtual LNRESULT play();

	/// 停止
	virtual LNRESULT stop();

	/// 一時停止
	virtual LNRESULT pause( bool is_pause_ );

	/// ポーリング更新
	virtual bool polling();

    /// 3D 音源かを判定する
    virtual bool is3DSound() { return ( mEmitterState != NULL ); }

    /// 3D 音源としての位置を設定する
    virtual LNRESULT setPosition( const LVector3& pos_ );

    /// 3D 音源としての位置を取得する
    virtual const LVector3& getPosition() { return mEmitterState->Position; }

public:

    /// IXAudio2SourceVoice の取得
    //virtual IXAudio2SourceVoice* getXAudio2SourceVoice() const { return mSourceVoice; }

    /// 3D 音源の情報の取得
    virtual EmitterState* getEmitterState() const { return mEmitterState; }
    
private:
    
    /// Play イベント処理
    static void _slPlayCallback( SLPlayItf caller_, void* context_, SLuint32 event_ );

private:

    AudioDevice*                mManager;       ///< 管理クラス
    IAudioSource*		        mAudioSource;
    
    SLObjectItf                     mSLOutputMixObject;
    SLObjectItf                     mSLPlayerObject;
    SLPlayItf                       mSLPlayerPlay;
    SLAndroidSimpleBufferQueueItf   mSLPlayerBufferQueue;
    SLVolumeItf                     mSLPlayerVolume;
    SLPlaybackRateItf               mSLPlaybackRate;
    
    EmitterState*               mEmitterState;  ///< 3D 音源の情報 (3D音源として初期化された場合に作成される)

	
	float					    mVolume;		///< 音量 ( 0 ～ 100 )
	float					    mPitch;			///< ピッチ ( 50 ～ 200 )

	u32						    mLoopBegin;		///< ループされる領域の最初のサンプル
	u32						    mLoopLength;	///< ループ領域の長さ (サンプル数単位)

	volatile bool		        mIsPlaying;
	volatile bool			    mIsPausing;
	volatile bool			    mIsLoop;
    volatile bool			    mPlayEnd;
};



//=============================================================================
// ■ Streaming クラス
//-----------------------------------------------------------------------------
/**
//  @brief      ストリーミングでの再生を扱うクラス
//*/
//=============================================================================
class Streaming
    : public AudioPlayerBase
{
public:

	/// コンストラクタ
	Streaming( AudioDevice* manager_ );

protected:

	/// デストラクタ
	virtual ~Streaming();

public:

    /// 初期化
	virtual LNRESULT initialize( IAudioSource* audio_source_, bool enable_3d_ = false );

    /// 設定されているオーディオソースの取得 ( 今のところ参照カウントは増やさない )
    virtual IAudioSource* getAudioSource() const { return mAudioSource; }

	/// 音量の取得
	virtual int getVolume() const;

	/// 音量の設定
	virtual LNRESULT setVolume( int volume_ );

	/// ピッチの取得
	virtual int getPitch() const;

	/// ピッチの設定
	virtual LNRESULT setPitch( int pitch_ );

	/// 再生中かを判定する
	virtual bool isPlaying() const;

	/// 一時停止中かを判定する
	virtual bool isPausing() const;

    /// 再生したサンプル数の取得
    virtual u64 getPlayedSamples() const;

    /// ループ再生の有効設定
    virtual void loop( bool loop_ ) { mIsLoop = loop_; }

    /// ループ再生が有効かを判定する
    virtual bool isLoop() { return mIsLoop; }

	/// ループ情報の設定
	virtual LNRESULT setLoopState( u32 loop_begin_, u32 loop_length_ );

	/// 再生
	virtual LNRESULT play();

	/// 停止
	virtual LNRESULT stop();

	/// 一時停止
	virtual LNRESULT pause( bool is_pause_ );

	/// ポーリング更新
	virtual bool polling();

    /// 3D 音源かを判定する
    virtual bool is3DSound() { return false; }

    /// 3D 音源としての位置を設定する
    virtual LNRESULT setPosition( const LVector3& pos_ );

    /// 3D 音源としての位置を取得する
    virtual const LVector3& getPosition() { return LVector3::ZERO; }
    
public:

    /// 3D 音源の情報の取得
    virtual EmitterState* getEmitterState() const { return NULL; }

private:

	/// セカンダリに新しいデータを書き込んで、再生キューに追加する
	LNRESULT _addNextBuffer();
    
    /// Play イベント処理
    static void _slPlayCallback( SLPlayItf caller_, void* context_, SLuint32 event_ );
    
private:

    AudioDevice*                    mManager;       ///< 管理クラス
    
    SLObjectItf                     mSLOutputMixObject;
    SLObjectItf                     mSLPlayerObject;
    SLPlayItf                       mSLPlayerPlay;
    SLAndroidSimpleBufferQueueItf   mSLPlayerBufferQueue;
    SLVolumeItf                     mSLPlayerVolume;
    SLPlaybackRateItf               mSLPlaybackRate;

    Audio::IAudioSource*             mAudioSource;

	float					mVolume;		///< 音量 ( 0 ～ 100 )
	float					mPitch;			///< ピッチ ( 50 ～ 200 )

	u32						mLoopBegin;		///< ループされる領域の最初のサンプル
	u32						mLoopLength;	///< ループ領域の長さ (サンプル数単位)
    //u32                     mEndSample;     ///< 終端サンプル ( ループ終端 )

	u8*						mPrimaryAudioData;		///< ダブルバッファリング用プライマリ
	u8*						mSecondaryAudioData;	///< ダブルバッファリング用セカンダリ
	u32						mAudioDataBufferSize;	///< プライマリバッファとセランダリバッファのサイズ ( 1 秒分 )
	u32						mReadCursor;

    u32                     mLoopBeginBlockOffset;
    u32                     mCursorBeginPos;
    u32                     mCursorEndPos;

    u32                     mWriteSampleNum;

	bool					mIsPlaying;
	bool					mIsPausing;
	bool					mIsLoop;
	bool					mEOF;			///< ファイルの終端まで再生したフラグ
    
    volatile bool           mPlayEnd;
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace SLES
} // namespace Audio
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================