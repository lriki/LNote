//=============================================================================
//【 SLESAudioDevice 】
//-----------------------------------------------------------------------------
///**
//  @file       SLESAudioDevice.h
//  @brief      SLESAudioDevice
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "../../../Base/Container/NodeList.h"
#include "../../../System/Interface.h"
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
class AudioPlayerBase;

/// 3D オーディオの計算に必要なパラメータ
struct EmitterState
{
    LVector3                Position;           // setPosition() で設定された仮想座標
    
    /*
    X3DAUDIO_EMITTER		Emitter;
    X3DAUDIO_CONE			EmitterCone;
    FLOAT32*				EmitterAzimuths;
    X3DAUDIO_DSP_SETTINGS	DSPSettings;            // この中の DstChannelCount は AudioDevice::update3DState でセットされる
    FLOAT32*				MatrixCoefficients;
    */

    /// コンストラクタ
    EmitterState( u32 input_channels_ );

    /// デストラクタ
    ~EmitterState();

    static const int OUTPUTCHANNELS = 8;

    /// 座標の設定
    void setPosition( const LVector3& pos_, lnfloat ld_inv_ );

    /// X3DAUDIO_EMITTER に正しい座標を設定する
    void updatePosition( lnfloat ld_inv_ );
    
};



//=============================================================================
// ■ AudioDevice クラス
//-----------------------------------------------------------------------------
///**
//  @brief      オーディオの管理クラス
//*/
//=============================================================================
class AudioDevice
    : public IAudioDevice
    , public Base::ReferenceObject
{
public:

    /// initialize() に渡す初期化データ
    struct Initdata
    {
        Base::LogFile*          LogFile;
    };

public:

    /// コンストラクタ
    AudioDevice();

protected:

    /// デストラクタ
    virtual ~AudioDevice();

public:

    /// 初期化
    LNRESULT initialize( const Initdata& init_data_ );

    /// 終了処理 ( デストラクタでも呼ばれます )
    void finalize();

    /// IAudioPlayer を作成する
    virtual LNRESULT createAudioPlayer( IAudioPlayer** obj_, IAudioSource* source_, bool enable_3d_, LNAudioPlayType type_ );

    /// 3D 音源のリスナー情報を設定する
    virtual LNRESULT setListenerState( const LVector3& position_, const LVector3& front_ );

    /// 3D 音源の可聴距離の設定
    virtual void setListenableDistance( lnfloat length_ );

    /// 3D 音源の可聴距離の取得
    virtual lnfloat getListenableDistance() { return mListenableDistance; }

    /// IAudioPlayer をリストから外す ( 各 AudioPlayer のデストラクタから呼ばれる )
    virtual void removeAudioPlayer( IAudioPlayer* player_ );


    SLEngineItf getSLEngineEngine() { return mSLEngineEngine; }
   
private:

   typedef Base::NodeList< AudioPlayerBase > AudioPlayerList;

private:
    
    SLObjectItf mSLEngineObject;
    SLEngineItf mSLEngineEngine;
    
   
    AudioPlayerList*        mAudioPlayerList;

    lnfloat                 mListenableDistance;
    Base::LogFile*          mLogFile;

public:

    LN_REFOBJ_METHODS;
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