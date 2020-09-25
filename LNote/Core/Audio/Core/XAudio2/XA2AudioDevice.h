//=============================================================================
//�y XA2AudioDevice �z
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

/// 3D �I�[�f�B�I�̌v�Z�ɕK�v�ȃp�����[�^
struct EmitterState
{

    

    /// �R���X�g���N�^
    EmitterState( u32 input_channels_ );

    /// �f�X�g���N�^
    ~EmitterState();

    static const int OUTPUTCHANNELS = 8;

    /// ���W�̐ݒ�
    //void setPosition( const LVector3& pos_, lnfloat ld_inv_ );

    /// X3DAUDIO_EMITTER �ɐ��������W��ݒ肷��
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
    X3DAUDIO_DSP_SETTINGS	DSPSettings;            // ���̒��� DstChannelCount �� AudioDevice::update3DState �ŃZ�b�g�����
    FLOAT32*				MatrixCoefficients;
    
};



//=============================================================================
// �� AudioDevice �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �I�[�f�B�I�̊Ǘ��N���X
//
//  @par
//              DirectMusic �����킹�ď��������܂��B
//
//  @note
//              XAudio2�AX3DAudio �̃C���X�^���X�ƁA3D �I�[�f�B�I�̌v�Z���s�������B
//              ���� Manager �݂����A�����Ƀ��X�g�������đ��̃I�u�W�F�N�g��
//              �Ǘ�������Ă����X�^�C������Ȃ��_���ӁB
//*/
//=============================================================================
class AudioDevice
    : public IAudioDevice
    , public Base::ReferenceObject
{
public:

    /// initialize() �ɓn���������f�[�^
    struct Initdata
    {
        Base::LogFile*          LogFile;
        bool                    EnableDebugFlag;    ///< XAudio2 �̃f�o�b�O�t���O��L���ɂ���ꍇ true ( Release �r���h�̏ꍇ�͖��� )
        LNDirectMusicUsingMode  DMInitMode;         ///< DirectMusic �����������鎞�̕��@
        System::IWindow*        Window;                 ///< DirectMusic �̏������Ɏg���E�B���h�E�n���h�������E�B���h�E�N���X

        /// �f�t�H���g�l
        Initdata()
            : LogFile           ( NULL )
            , EnableDebugFlag   ( false )
            , DMInitMode        ( LN_DMUSINGMODE_NOTUSE )
            , Window            ( NULL )
        {}
    };

    

public:

    

    /// �R���X�g���N�^
    AudioDevice();

protected:

    /// �f�X�g���N�^
    virtual ~AudioDevice();

public:

    /// ������
    LNRESULT initialize( const Initdata& init_data_ );

    /// �I������ ( �f�X�g���N�^�ł��Ă΂�܂� )
    void finalize();

    /// IXAudio2 �C���X�^���X�̎擾
	IXAudio2* getXAudio2() const { return mXAudio; }

    /// �}�X�^�����O�{�C�X�̎擾
	IXAudio2MasteringVoice* getMasteringVoice() { return mMasteringVoice; }

    /// 3D �����̌v�Z������ ( AudioPlayer::polling() ����Ă΂�� )
    LNRESULT calcEmitterState( EmitterState* emitter_ );

    
    lnfloat getListenableDistanceInv() const { return mListenableDistanceInv; }


    /// IAudioPlayer ���쐬����
    virtual LNRESULT createAudioPlayer( IAudioPlayer** obj_, IAudioSource* source_, bool enable_3d_, LNSoundPlayType type_ );

    virtual LN3DSoundListener* get3DSoundListener() { return &m3DSoundListener; }

    virtual void update();
    
    /// 3D �����̃��X�i�[����ݒ肷��
    virtual LNRESULT setListenerState( const LVector3& position_, const LVector3& front_ );

    /// 3D �����̉������̐ݒ�
    //virtual void setListenableDistance( lnfloat length_ );

    /// 3D �����̉������̎擾
    //virtual lnfloat getListenableDistance() { return mListenableDistance; }

    /// 3D ��Ԃ�1���[�g�������̋����̐ݒ�
    virtual lnfloat setMetreUnitDistance( lnfloat d ) { return mMetreUnitDistanceInv = 1.0f / d; }

    /// IAudioPlayer �����X�g����O�� ( �e AudioPlayer �̃f�X�g���N�^����Ă΂�� )
    virtual void removeAudioPlayer( IAudioPlayer* player_ );

    

private:

    

private:

    typedef void ( *DEF_X3DAudioCalculate ) ( __in const X3DAUDIO_HANDLE, __in const X3DAUDIO_LISTENER*, __in const X3DAUDIO_EMITTER*, UINT32, __inout X3DAUDIO_DSP_SETTINGS* );
    typedef Base::NodeList< AudioPlayerBase > AudioPlayerList;

private:

    IXAudio2*				mXAudio;			    ///< IXAudio2 �C���X�^���X�ւ̃|�C���^
	IXAudio2MasteringVoice*	mMasteringVoice;	    ///< IXAudio2MasteringVoice �C���X�^���X�ւ̃|�C���^
	X3DAUDIO_HANDLE			mX3DInstance;           ///< 3D �I�[�f�B�I�̊Ǘ��N���X
    u32						mDeviceChannels;        ///< �A�N�e�B�u�ȃI�[�f�B�I�f�o�C�X�̏o�̓`�����l����
    

    X3DAUDIO_LISTENER	    mListenerState;
    LN3DSoundListener       m3DSoundListener;

    HMODULE                 mX3DAudioModule;        ///< "X3DAudio1_7.dll"
    DEF_X3DAudioCalculate   mMD_X3DAudioCalculate;  ///< X3DAudioCalculate()

    AudioPlayerList*        mAudioPlayerList;

    lnfloat                 mListenableDistance;
    lnfloat                 mListenableDistanceInv; ///< OuterDistance / mListenableDistance �̒l
    lnfloat                 mMetreUnitDistanceInv;     ///< 3D ��Ԃ�1���[�g�������̋����̋t��

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