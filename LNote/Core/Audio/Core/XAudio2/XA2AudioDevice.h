//=============================================================================
//【 XA2AudioDevice 】
//-----------------------------------------------------------------------------
///**
//  @file       XA2AudioDevice.h
//  @brief      XA2AudioDevice
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <xaudio2.h>
#include <x3daudio.h>
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
namespace XAudio2
{
class AudioPlayerBase;

/// 3D オーディオの計算に必要なパラメータ
struct EmitterState
{

    

    /// コンストラクタ
    EmitterState( u32 input_channels_ );

    /// デストラクタ
    ~EmitterState();

    static const int OUTPUTCHANNELS = 8;

    /// 座標の設定
    //void setPosition( const LVector3& pos_, lnfloat ld_inv_ );

    /// X3DAUDIO_EMITTER に正しい座標を設定する
    void updateXAudioEmitter( lnfloat scale );
    
    void setDistance( lnfloat d ) { Distance = d; }

    lnfloat getDistance() { return Distance; }

public:

    LVector3        Position;
    LVector3        Velocity;
    lnfloat         Distance;        

public:

    X3DAUDIO_EMITTER		Emitter;
    X3DAUDIO_CONE			EmitterCone;
    FLOAT32*				EmitterAzimuths;
    X3DAUDIO_DSP_SETTINGS	DSPSettings;            // この中の DstChannelCount は AudioDevice::update3DState でセットされる
    FLOAT32*				MatrixCoefficients;
    
};



//=============================================================================
// ■ AudioDevice クラス
//-----------------------------------------------------------------------------
///**
//  @brief      オーディオの管理クラス
//
//  @par
//              DirectMusic も合わせて初期化します。
//
//  @note
//              XAudio2、X3DAudio のインスタンスと、3D オーディオの計算を行うだけ。
//              他の Manager みたく、内部にリストを持って他のオブジェクトを
//              管理するっていうスタイルじゃない点注意。
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
        bool                    EnableDebugFlag;    ///< XAudio2 のデバッグフラグを有効にする場合 true ( Release ビルドの場合は無効 )
        LNDirectMusicUsingMode  DMInitMode;         ///< DirectMusic を初期化する時の方法
        System::IWindow*        Window;                 ///< DirectMusic の初期化に使うウィンドウハンドルを持つウィンドウクラス

        /// デフォルト値
        Initdata()
            : LogFile           ( NULL )
            , EnableDebugFlag   ( false )
            , DMInitMode        ( LN_DMUSINGMODE_NOTUSE )
            , Window            ( NULL )
        {}
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

    /// IXAudio2 インスタンスの取得
	IXAudio2* getXAudio2() const { return mXAudio; }

    /// マスタリングボイスの取得
	IXAudio2MasteringVoice* getMasteringVoice() { return mMasteringVoice; }

    /// 3D 音源の計算をする ( AudioPlayer::polling() から呼ばれる )
    LNRESULT calcEmitterState( EmitterState* emitter_ );

    
    lnfloat getListenableDistanceInv() const { return mListenableDistanceInv; }


    /// IAudioPlayer を作成する
    virtual LNRESULT createAudioPlayer( IAudioPlayer** obj_, IAudioSource* source_, bool enable_3d_, LNSoundPlayType type_ );

    virtual LN3DSoundListener* get3DSoundListener() { return &m3DSoundListener; }

    virtual void update();
    
    /// 3D 音源のリスナー情報を設定する
    virtual LNRESULT setListenerState( const LVector3& position_, const LVector3& front_ );

    /// 3D 音源の可聴距離の設定
    //virtual void setListenableDistance( lnfloat length_ );

    /// 3D 音源の可聴距離の取得
    //virtual lnfloat getListenableDistance() { return mListenableDistance; }

    /// 3D 空間の1メートル相当の距離の設定
    virtual lnfloat setMetreUnitDistance( lnfloat d ) { return mMetreUnitDistanceInv = 1.0f / d; }

    /// IAudioPlayer をリストから外す ( 各 AudioPlayer のデストラクタから呼ばれる )
    virtual void removeAudioPlayer( IAudioPlayer* player_ );

    

private:

    

private:

    typedef void ( *DEF_X3DAudioCalculate ) ( __in const X3DAUDIO_HANDLE, __in const X3DAUDIO_LISTENER*, __in const X3DAUDIO_EMITTER*, UINT32, __inout X3DAUDIO_DSP_SETTINGS* );
    typedef Base::NodeList< AudioPlayerBase > AudioPlayerList;

private:

    IXAudio2*				mXAudio;			    ///< IXAudio2 インスタンスへのポインタ
	IXAudio2MasteringVoice*	mMasteringVoice;	    ///< IXAudio2MasteringVoice インスタンスへのポインタ
	X3DAUDIO_HANDLE			mX3DInstance;           ///< 3D オーディオの管理クラス
    u32						mDeviceChannels;        ///< アクティブなオーディオデバイスの出力チャンネル数
    

    X3DAUDIO_LISTENER	    mListenerState;
    LN3DSoundListener       m3DSoundListener;

    HMODULE                 mX3DAudioModule;        ///< "X3DAudio1_7.dll"
    DEF_X3DAudioCalculate   mMD_X3DAudioCalculate;  ///< X3DAudioCalculate()

    AudioPlayerList*        mAudioPlayerList;

    lnfloat                 mListenableDistance;
    lnfloat                 mListenableDistanceInv; ///< OuterDistance / mListenableDistance の値
    lnfloat                 mMetreUnitDistanceInv;     ///< 3D 空間の1メートル相当の距離の逆数

    //bool                    mLogWrite;
    Base::LogFile*          mLogFile;

    

    bool                    mCoInited;

public:

    LN_REFOBJ_METHODS;
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