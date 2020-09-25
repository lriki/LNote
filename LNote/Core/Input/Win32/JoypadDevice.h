//=============================================================================
//�y Joypad �z
//-----------------------------------------------------------------------------
///**
//  @file       Joypad.h
//  @brief      Joypad
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Input
{
namespace Win32
{
class XInputModule;

//=============================================================================
// �� JoypadDevice �N���X
//-----------------------------------------------------------------------------
/**
// @brief      �W���C�p�b�h�������N���X
//*/
//=============================================================================
class JoypadDevice
{
public:

	/// �X�e�B�b�N�̏�Ԃ�\���\����
	struct AnalogState
	{
		s32	X1;
		s32	Y1;
		s32	X2;
		s32	Y2;
	};

public:

	/// �R���X�g���N�^
	JoypadDevice();

	/// �f�X�g���N�^
	~JoypadDevice();

public:

    /// ������ (xinput_no_ �� XInput �p�R���g���[���̊��蓖�Ĕԍ��BXInput �ȊO�̂��̂� -1 ��n������)
	LNRESULT initialize( IDirectInputDevice8* device_, HWND hwnd_, int xinput_no_, XInputModule* xinput_, bool forcefeedback_ );

    /// ���
	void dispose();

    /// �{�^����Ԃ̎擾
    bool getButtonState( u32 button_ ) const { return mButtonState[ button_ ] != 0; }

    /// �{�^����Ԃ̎擾
	void getButtonState( u8* buf_ ) const;

    /// �����R���g���[���̏�Ԃ̎擾
	s32 getPOVState() const { return mPOVState; }

    /// ���̏�Ԃ̎擾 (MAX_AXIS_NUM ��)
	void getAxisState( float* axes_ ) const;

    /// ���̏�Ԃ̎擾
    float getAxisState( u32 axis_ ) const { return mAxisState[ axis_ ]; }

    /// XInput �f�o�C�X���𔻒肷��
    bool isXInputDevice() const { return mXInputNo >= 0; }

    /// �W���C�p�b�h�̐U�����J�n����
    void startVibration( int power_, int time_ );

    /// �W���C�p�b�h�̐U�����~����
    void stopVibration();

    /// �t���[���X�V
	LNRESULT update();

private:

    /// �W���C�X�e�B�b�N�̎���񋓂���֐�
	static BOOL CALLBACK _enumAxesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi_, LPVOID this_ );

public:

	static const int AXIS_RANGE     = 1000;		///< ���͈̔� ( -1000�`1000 )
    static const int MAX_BUTTON_NUM = LN_MAX_JOYPAD_BUTTONS;	///< �{�^���̐�
	static const int MAX_AXIS_NUM   = LN_MAX_JOYPAD_AXIS;	    ///< ���̐�

private:

    Base::LogFile*          mLogFile;
    XInputModule*           mXInputModule;
    IDirectInputDevice8*	mDevice;

    IDirectInputEffect*     mDeviceEffect;
    DWORD                   mDIAxes[ 2 ];
    LONG                    mDIDirection[ 2 ];
    DIPERIODIC              mDIPeriodic;
    DIENVELOPE              mDIEnvelope;
    DIEFFECT                mDIEffect;
    lnU32                   mVibrationStartTime;
    lnU32                   mVibrationTime;

    float                   mPlayRange;

	u8		                mButtonState[ MAX_BUTTON_NUM ];	///< �{�^���̏��
	u32		                mPOVState;						///< POV ��� (POVDir �̑g�ݍ��킹)
	float	                mAxisState[ MAX_AXIS_NUM ];		///< ���̏��
    int                     mXInputNo;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Win32
} // namespace Input
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================