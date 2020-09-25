//=============================================================================
//【 XA2AudioPlayer 】
//-----------------------------------------------------------------------------
///**
//  @file       XA2AudioPlayer.h
//  @brief      XA2AudioPlayer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "DirectMusic.h"
#include "XA2AudioDevice.h"
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
namespace XAudio2
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
    virtual IXAudio2SourceVoice* getXAudio2SourceVoice() const = 0;

    /// 3D 音源の情報の取得
    virtual EmitterState* getEmitterState() const = 0;

    LN_REFOBJ_METHODS();
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

    /// 再生状態の取得
    virtual LNSoundPlayState getPlayState() const { return (mIsPausing) ? LN_SOUNDPLAYSTATE_PAUSING : ( (mIsPlaying) ? LN_SOUNDPLAYSTATE_PLAYING : LN_SOUNDPLAYSTATE_STOP ); }

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

    /// 速度の設定
    virtual void setVelocity( const LVector3& pos_ );

    /// 3D 音源の影響範囲 (音の届く距離) の設定
    virtual void setEmitterDistance( lnfloat distance_ ) { mEmitterState->setDistance( distance_ ); }

    /// 3D 音源の影響範囲 (音の届く距離) の取得
    virtual lnfloat getEmitterDistance() const { return mEmitterState->getDistance(); }

public:

    /// IXAudio2SourceVoice の取得
    virtual IXAudio2SourceVoice* getXAudio2SourceVoice() const { return mSourceVoice; }

    /// 3D 音源の情報の取得
    virtual EmitterState* getEmitterState() const { return mEmitterState; }

private:

    AudioDevice*                mManager;       ///< 管理クラス
    IAudioSource*		        mAudioSource;
	IXAudio2SourceVoice*	    mSourceVoice;
    EmitterState*               mEmitterState;  ///< 3D 音源の情報 (3D音源として初期化された場合に作成される)

	
	float					    mVolume;		///< 音量 ( 0 ～ 100 )
	float					    mPitch;			///< ピッチ ( 50 ～ 200 )

	u32						    mLoopBegin;		///< ループされる領域の最初のサンプル
	u32						    mLoopLength;	///< ループ領域の長さ (サンプル数単位)

	bool					    mIsPlaying;
	bool					    mIsPausing;
	bool					    mIsLoop;
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

        /// 再生状態の取得
    virtual LNSoundPlayState getPlayState() const { return (mIsPausing) ? LN_SOUNDPLAYSTATE_PAUSING : ( (mIsPlaying) ? LN_SOUNDPLAYSTATE_PLAYING : LN_SOUNDPLAYSTATE_STOP ); }

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

    /// 速度の設定
    virtual void setVelocity( const LVector3& pos_ );
    
    /// 3D 音源の影響範囲 (音の届く距離) の設定
    virtual void setEmitterDistance( lnfloat distance_ ) { mEmitterState->setDistance( distance_ ); }

    /// 3D 音源の影響範囲 (音の届く距離) の取得
    virtual lnfloat getEmitterDistance() const { return mEmitterState->getDistance(); }

public:

    /// IXAudio2SourceVoice の取得
    virtual IXAudio2SourceVoice* getXAudio2SourceVoice() const { return mSourceVoice; }

    /// 3D 音源の情報の取得
    virtual EmitterState* getEmitterState() const { return mEmitterState; }

private:

	/// セカンダリに新しいデータを書き込んで、再生キューに追加する
	LNRESULT _addNextBuffer();

private:

    AudioDevice*                   mManager;       ///< 管理クラス
    Audio::IAudioSource*		mAudioSource;
	IXAudio2SourceVoice*	    mSourceVoice;
    EmitterState*  mEmitterState;

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
};


//=============================================================================
// ■ MidiPlayer クラス
//-----------------------------------------------------------------------------
///**
//  @brief      Midi データの再生を扱うクラス
//
//  @par
//              DirectMusic を別スレッドで初期化する場合、フェードイン・アウト時間の
//              つじつまが合わなくなるということがあります。
//              これは「DirectMusic 初期化中は音を鳴らすことができないため、
//              再生中ではない」という判定が入り、停止中はフェード処理がされないために起こります。
//              <br>
//              これを修正し、Midi の演奏が開始される時間に合わせて正しい音量を
//              設定するという事は可能でしたが、実際にこの不具合に遭遇するような事態というのは
//              ユーザーの方が「ゲーム開始時からフェードイン再生したい」というものがほとんどです。
//              これを修正した場合、開始時に中途半端な音量から演奏を開始することになり、
//              演出としてあまり良くないと考えたため、現段階ではこれは仕様とします。
//*/
//=============================================================================
class MidiPlayer
    : public AudioPlayerBase
    , public DirectMusicManager::PlayerObject
{
public:

	/// コンストラクタ
	MidiPlayer( AudioDevice* manager_ );

protected:

	/// デストラクタ
	virtual ~MidiPlayer();

public:

    /// 初期化 ( xaudio2_ は無効。普通は NULL を渡しておく )
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

    /// 再生状態の取得
    virtual LNSoundPlayState getPlayState() const { return (mIsPausing) ? LN_SOUNDPLAYSTATE_PAUSING : ( (mIsPlaying) ? LN_SOUNDPLAYSTATE_PLAYING : LN_SOUNDPLAYSTATE_STOP ); }

    /// 再生したサンプル数の取得 ( Midi の場合はミュージックタイム )
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

    /// 再生要求リストに追加されている状態で DirectMusic の初期化が完了したときに呼ばれる
    virtual void onFinishDMInit( IDirectMusicPerformance8* dm_performance_ );

    /// 3D 音源かを判定する
    virtual bool is3DSound() { return false; }

    /// 3D 音源としての位置を設定する
    virtual LNRESULT setPosition( const LVector3& pos_ ) { return LN_OK; }

    /// 3D 音源としての位置を取得する
    virtual const LVector3& getPosition() { static const LVector3 pos( 0, 0, 0 ); return pos; }

    /// 速度の設定
    virtual void setVelocity( const LVector3& v ) {}

    /// 3D 音源の影響範囲 (音の届く距離) の設定
    virtual void setEmitterDistance( lnfloat distance_ ) {}

    /// 3D 音源の影響範囲 (音の届く距離) の取得
    virtual lnfloat getEmitterDistance() const { return 0; }

    /// 臨時用。全体の再生時間の取得
    u32 getTotalTime() const;

public:

    /// IXAudio2SourceVoice の取得
    virtual IXAudio2SourceVoice* getXAudio2SourceVoice() const { return NULL; }

    /// 3D 音源の情報の取得
    virtual EmitterState* getEmitterState() const { return NULL; }

private:

    /// 実際に再生する
    LNRESULT _play();

private:

    AudioDevice*                   mManager;       ///< 管理クラス
    Midi*		                    mAudioSource;   ///< Midi データ
    DirectMusicSegment*             mSegment;       ///< DirectMusic の再生管理
	
	float					        mVolume;		///< 音量 ( 0 ～ 100 )
	float					        mPitch;			///< ピッチ ( 50 ～ 200 )

	u32						        mLoopBegin;		///< ループされる領域の先頭 ( ミュージックタイム単位 )
	u32						        mLoopLength;	///< ループ領域の長さ ( ミュージックタイム単位 )
    u32                             mPauseTime;

	bool					        mIsPlaying;
	bool					        mIsPausing;
	bool					        mIsLoop;
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