//=============================================================================
//【 Sound 】
//-----------------------------------------------------------------------------
///**
//  @file       Sound.h
//  @brief      Sound
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Base/Container/NodeList.h"
#include "../Geometry/Vector3.h"
#include "../Animation/FadeValue.h"
#include "Interface.h"

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
// ■ Sound クラス
//-----------------------------------------------------------------------------
///**
//  @brief      音声とその再生を扱うクラス
//*/
//=============================================================================
class Sound
    : public Base::ReferenceObject
    , public ISound
    , public Base::NodeList< Sound >::Node
{
public:

    /// コンストラクタ ( IAudioPlayer は Manager での作成後すぐに設定することを想定しているため、参照カウントは増やさない。デストラクタで release() )
    Sound( Manager* manager_, IAudioPlayer* player_ );

	/// デストラクタ
	virtual ~Sound();

    LN_REFOBJ_METHODS;

public:

	/// 音量の取得
	virtual int getVolume();

	/// 音量の設定
	virtual void setVolume( int volume_ );

	/// ピッチの取得
	virtual int getPitch();

	/// ピッチの設定
	virtual void setPitch( int pitch_ );

    /// サウンドの状態を取得する
    virtual void getState( LNAudioPlayState* state_ );

    /// ループ再生の有効設定
    virtual void loop( bool loop_ );

    /// ループ再生が有効かを判定する
    virtual bool isLoop();

    /// ループ情報の設定
	virtual void setLoopState( u32 begin_, u32 length_ );

    /// 再生
	virtual LNRESULT play();

    /// 停止
	virtual void stop();

    /// 一時停止
	virtual void pause( bool is_pause_ );

    /// 音量のフェード
	virtual LNRESULT fadeVolume( int target_volume_, int time_, LNSoundFadeState state_ );

    /// 3D 音源かを判定する
    virtual bool is3DSound();

    /// 3D 音源としての位置を設定する
    virtual LNRESULT setPosition( const LVector3& pos_ );

    /// 3D 音源としての位置を取得する
    virtual const LVector3& getPosition();

    /// 速度の設定
    virtual void setVelocity( const LVector3& v );

    /// 減衰距離の設定
    virtual void setMaxDistance( lnfloat distance );

	virtual void setGroupFlag( u32 flag_ );

    virtual u32 getGroupFlag();

    /// 内部に持っている音声再生クラスの取得
    virtual IAudioPlayer* getAudioPlayer() { return mAudioPlayer; }

    virtual bool update( float elapsed_time_ );

    //virtual u32 getPlayerFlags() const { return mFlags; }

    ///// LNAudioPlayerFlag の値
    //virtual void setPlayerFlags( u32 flags_ ) { mFlags = flags_; }


private:

    Manager*                        mManager;
    IAudioPlayer*	                mAudioPlayer;
    Thread::Mutex*                  mLock;

    int 					        mOrginalVolume;	        ///< setVolume() で設定される、元の音量
    LNSoundFadeState		        mFadeState;
    Animation::FadeValue< int >     mFadeValue;
    
    u32                             mFlags;                 ///< LNAudioPlayerFlag の値
    u32                             mGroupFlags;
    bool					        mIsFading;		        ///< フェードイン、またはフェードアウト中フラグ
    bool                            mOnlyRefFromManager;    ///< Manager::deleteAudioPlayer() から操作される
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