//=============================================================================
//�y Joypad �z
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../System/OS.h"
#include "XInputModule.h"
#include "JoypadDevice.h"

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

// �W���C�X�e�B�b�N�� GUID
GUID GUIDDIJOYSTICK[8] =
{
//	Data1		Data2	Data3	Data4
	0xa36d02e0,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02e1,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02e2,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02f4,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02f5,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02e3,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02e4,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02f2,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
} ;

// �W���C�X�e�B�b�N�̃f�o�C�X�I�u�W�F�N�g�f�[�^
DIOBJECTDATAFORMAT C_ODFDIJOYSTICK[44] =
{
//	pguid				dwOfs	dwType		dwFlags
	&GUIDDIJOYSTICK[0],	0x00,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[1],	0x04,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[2],	0x08,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[3],	0x0c,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[4],	0x10,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[5],	0x14,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[6],	0x18,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[6],	0x1c,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[7],	0x20,	0x80ffff10,	0x000,
	&GUIDDIJOYSTICK[7],	0x24,	0x80ffff10,	0x000,
	&GUIDDIJOYSTICK[7],	0x28,	0x80ffff10,	0x000,
	&GUIDDIJOYSTICK[7],	0x2c,	0x80ffff10,	0x000,
	NULL,				0x30,	0x80ffff0c,	0x000,
	NULL,				0x31,	0x80ffff0c,	0x000,
	NULL,				0x32,	0x80ffff0c,	0x000,
	NULL,				0x33,	0x80ffff0c,	0x000,
	NULL,				0x34,	0x80ffff0c,	0x000,
	NULL,				0x35,	0x80ffff0c,	0x000,
	NULL,				0x36,	0x80ffff0c,	0x000,
	NULL,				0x37,	0x80ffff0c,	0x000,
	NULL,				0x38,	0x80ffff0c,	0x000,
	NULL,				0x39,	0x80ffff0c,	0x000,
	NULL,				0x3a,	0x80ffff0c,	0x000,
	NULL,				0x3b,	0x80ffff0c,	0x000,
	NULL,				0x3c,	0x80ffff0c,	0x000,
	NULL,				0x3d,	0x80ffff0c,	0x000,
	NULL,				0x3e,	0x80ffff0c,	0x000,
	NULL,				0x3f,	0x80ffff0c,	0x000,
	NULL,				0x40,	0x80ffff0c,	0x000,
	NULL,				0x41,	0x80ffff0c,	0x000,
	NULL,				0x42,	0x80ffff0c,	0x000,
	NULL,				0x43,	0x80ffff0c,	0x000,
	NULL,				0x44,	0x80ffff0c,	0x000,
	NULL,				0x45,	0x80ffff0c,	0x000,
	NULL,				0x46,	0x80ffff0c,	0x000,
	NULL,				0x47,	0x80ffff0c,	0x000,
	NULL,				0x48,	0x80ffff0c,	0x000,
	NULL,				0x49,	0x80ffff0c,	0x000,
	NULL,				0x4a,	0x80ffff0c,	0x000,
	NULL,				0x4b,	0x80ffff0c,	0x000,
	NULL,				0x4c,	0x80ffff0c,	0x000,
	NULL,				0x4d,	0x80ffff0c,	0x000,
	NULL,				0x4e,	0x80ffff0c,	0x000,
	NULL,				0x4f,	0x80ffff0c,	0x000,
};

// �W���C�X�e�B�b�N�̃f�o�C�X�f�[�^�t�H�[�}�b�g
const DIDATAFORMAT c_dfDIJoystick =
{
	24, 	// dwSize
	16,		// dwObjSize
	0x01,	// dwFlags
	80,		// dwDataSize
	44,		// dwNumObjs
	C_ODFDIJOYSTICK, // rgodf
};

//=============================================================================
// �� JoypadDevice �N���X
//=============================================================================

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    JoypadDevice::JoypadDevice()
		: mDevice		        ( NULL )
        , mDeviceEffect         ( NULL )
        , mVibrationStartTime   ( 0 )
        , mVibrationTime        ( 0 )
        , mPlayRange            ( LN_JOYPAD_PLAY_RANGE )
		, mPOVState		        ( -1 )
	{
		memset( mButtonState, 0, sizeof( mButtonState ) );
		memset( mAxisState, 0, sizeof( mAxisState ) );
	}

	//---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
	JoypadDevice::~JoypadDevice()
	{
		dispose();


	}

    //---------------------------------------------------------------------
    // �� ������
    //---------------------------------------------------------------------
    LNRESULT JoypadDevice::initialize( IDirectInputDevice8* device_, HWND hwnd_, int xinput_no_, XInputModule* xinput_, bool forcefeedback_ )
	{
        dispose();

        HRESULT hr;

        mDevice = device_;
        mXInputNo = xinput_no_;
        mXInputModule = xinput_;


        memset( mButtonState, 0, sizeof( mButtonState ) );
		memset( mAxisState, 0, sizeof( mAxisState ) );

        if ( !isXInputDevice() )
        {
		    // �W���C�X�e�B�b�N�Ƃ��ăf�[�^�t�H�[�}�b�g��ݒ�
		    //hr = mDevice->SetDataFormat( &c_dfDIJoystick2 );	// �����Ƃ����ȏ�񂪂ق����Ƃ�
		    LN_DXCALL_R( mDevice->SetDataFormat( &c_dfDIJoystick ) );

		    // �t�H�A�O���E���h�E�r�����[�h
		    LN_DXCALL_R( mDevice->SetCooperativeLevel( hwnd_, DISCL_FOREGROUND | DISCL_EXCLUSIVE ) );

		    // �R�[���o�b�N�֐����g���Ċe���̃��[�h��ݒ�
		    LN_DXCALL_R( mDevice->EnumObjects( _enumAxesCallback, this, DIDFT_AXIS ) );

            if ( forcefeedback_ )
            {
                // �G�t�F�N�g�����ݒ�
                ZeroMemory( &mDIPeriodic, sizeof( mDIPeriodic ) );
                mDIPeriodic.dwMagnitude     = DI_FFNOMINALMAX; // �G�t�F�N�g�̋���(0�`10,000)
                mDIPeriodic.lOffset         = 0;
                mDIPeriodic.dwPhase         = 0;
                mDIPeriodic.dwPeriod        = (DWORD)( 1.5 * DI_SECONDS ); // �G�t�F�N�g�̊Ԋu

                // �G���x���[�v
                ZeroMemory( &mDIEnvelope, sizeof( mDIEnvelope ) );
                mDIEnvelope.dwSize			= sizeof(DIENVELOPE);
			    mDIEnvelope.dwAttackLevel	= 0;
			    mDIEnvelope.dwAttackTime    = (DWORD) (0.5 * DI_SECONDS);
			    mDIEnvelope.dwFadeLevel		= 0;
			    mDIEnvelope.dwFadeTime		= (DWORD) (1.0 * DI_SECONDS);

                // �G�t�F�N�g��
                mDIAxes[ 0 ] = DIJOFS_X;
                mDIAxes[ 1 ] = DIJOFS_Y;

                // �G�t�F�N�g����
                mDIDirection[ 0 ] = 0;
                mDIDirection[ 1 ] = 0;

                // �U���G�t�F�N�g�ݒ�
                ZeroMemory( &mDIEffect, sizeof( mDIEffect ) );
                mDIEffect.dwSize = sizeof( mDIEffect );
                mDIEffect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;//DIEFF_POLAR | DIEFF_OBJECTOFFSETS;
                mDIEffect.dwDuration = INFINITE; // �G�t�F�N�g�p������
                mDIEffect.dwSamplePeriod = 0;
                mDIEffect.dwGain = DI_FFNOMINALMAX;
                mDIEffect.dwTriggerButton = DIEB_NOTRIGGER;
                mDIEffect.dwTriggerRepeatInterval = 0;
                mDIEffect.cAxes = 2;
                mDIEffect.rgdwAxes = mDIAxes;
                mDIEffect.rglDirection = mDIDirection;
                mDIEffect.lpEnvelope = &mDIEnvelope; // �G���x���[�v�\����
                mDIEffect.cbTypeSpecificParams = sizeof( mDIPeriodic ); // �G�t�F�N�g�����\���̂̃T�C�Y
                mDIEffect.lpvTypeSpecificParams = &mDIPeriodic; // �G�t�F�N�g�����\����

                // �G�t�F�N�g����
                LN_DXCALL_R( mDevice->CreateEffect( GUID_Sine, &mDIEffect, &mDeviceEffect, NULL ) );

            }
            

            mDevice->Acquire();

        }

        return LN_OK;
	}

    //---------------------------------------------------------------------
    // �� ���
    //---------------------------------------------------------------------
	void JoypadDevice::dispose()
    {
        if ( mDeviceEffect )
        {
            mDeviceEffect->Unload();
            SAFE_RELEASE( mDeviceEffect );
        }

        // ���͂��~���ăW���C�X�e�B�b�N�f�o�C�X�����
		if ( mDevice )
		{
			mDevice->Unacquire();
            SAFE_RELEASE( mDevice );
		}
    }

    //---------------------------------------------------------------------
    // �� �{�^����Ԃ̎擾
    //---------------------------------------------------------------------
	void JoypadDevice::getButtonState( u8* buf_ ) const
	{
		memcpy( buf_, mButtonState, MAX_BUTTON_NUM );
	}

    //---------------------------------------------------------------------
    // �� ���̏�Ԃ̎擾
    //---------------------------------------------------------------------
	void JoypadDevice::getAxisState( float* axes_ ) const
	{
		memcpy( axes_, mAxisState, sizeof( mAxisState ) );
	}

    //---------------------------------------------------------------------
    // �� �W���C�p�b�h�̐U�����J�n����
    //---------------------------------------------------------------------
    void JoypadDevice::startVibration( int power_, int time_ )
    {
        if ( isXInputDevice() )
        {
            XINPUT_VIBRATION vibration;
            vibration.wLeftMotorSpeed  = power_ * 65535 / 1000;
            vibration.wRightMotorSpeed = power_ * 65535 / 1000;
            mXInputModule->XInputSetState( mXInputNo, &vibration );
        }
        else
        {
            if ( mDeviceEffect )
            {
                mDIPeriodic.dwMagnitude = 10 * std::max( 0, std::min( power_, 1000 ) );

                if ( time_ > 0 )
                {
                    mDIPeriodic.dwPeriod = std::max( 0, time_ ) * 10000; // ���ʂ� 1000 �����ǁA�Ȃ񂩓r���ŐU������u�キ�Ȃ�
                    //mDIEffect.dwDuration = mDIPeriodic.dwPeriod = time_ * 1000;
                }

                mDeviceEffect->SetParameters( &mDIEffect, DIEP_TYPESPECIFICPARAMS | DIEP_START );
                mDeviceEffect->Start( 1, 0 );
            }
        }

        if ( time_ > 0 )
        {
            mVibrationTime = time_;
            mVibrationStartTime = System::getSystemTime();
        }
        else
        {
            mVibrationTime = 0;
            mVibrationStartTime = 0;
        }
    }

    //---------------------------------------------------------------------
    // �� �W���C�p�b�h�̐U�����~����
    //---------------------------------------------------------------------
    void JoypadDevice::stopVibration()
    {
        if ( isXInputDevice() )
        {
            XINPUT_VIBRATION vibration;
            vibration.wLeftMotorSpeed  = 0;
            vibration.wRightMotorSpeed = 0;
            mXInputModule->XInputSetState( mXInputNo, &vibration );
        }
        else
        {
            if ( mDeviceEffect )
            {
                mDeviceEffect->Stop();
                mVibrationStartTime = 0;
                mVibrationTime = 0;
            }
        }
    }

    //---------------------------------------------------------------------
    // �� �t���[���X�V
    //---------------------------------------------------------------------
	LNRESULT JoypadDevice::update()
	{
        LNRESULT hr;
        bool connected = false;

        // XInput
        if ( isXInputDevice() )
        {
            XINPUT_STATE state;
            if ( mXInputModule->XInputGetState( mXInputNo, &state ) == ERROR_SUCCESS )
		    {
                // �{�^��
                mButtonState[ 0 ] = ( state.Gamepad.wButtons & XINPUT_GAMEPAD_A ) != 0;
                mButtonState[ 1 ] = ( state.Gamepad.wButtons & XINPUT_GAMEPAD_B ) != 0;
                mButtonState[ 2 ] = ( state.Gamepad.wButtons & XINPUT_GAMEPAD_X ) != 0;
                mButtonState[ 3 ] = ( state.Gamepad.wButtons & XINPUT_GAMEPAD_Y ) != 0;
                mButtonState[ 4 ] = ( state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ) != 0;
                mButtonState[ 5 ] = ( state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ) != 0;
                mButtonState[ 6 ] = ( state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK ) != 0;
                mButtonState[ 7 ] = ( state.Gamepad.wButtons & XINPUT_GAMEPAD_START ) != 0;
                mButtonState[ 8 ] = ( state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB ) != 0;
                mButtonState[ 9 ] = ( state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB ) != 0;

                // ��
                lnfloat inv_s16 = 1.0f / 32767.f;
                lnfloat inv_u8  = 1.0f / 255.f;
                mAxisState[ 0 ] = inv_s16 * static_cast< lnfloat >( state.Gamepad.sThumbLX );
			    mAxisState[ 1 ] = inv_s16 * static_cast< lnfloat >( state.Gamepad.sThumbLY );
			    mAxisState[ 2 ] = inv_s16 * static_cast< lnfloat >( state.Gamepad.sThumbRX );
			    mAxisState[ 3 ] = inv_s16 * static_cast< lnfloat >( state.Gamepad.sThumbRY );
                mAxisState[ 4 ] = inv_u8  * static_cast< lnfloat >( state.Gamepad.bLeftTrigger );
			    mAxisState[ 5 ] = inv_u8  * static_cast< lnfloat >( state.Gamepad.bRightTrigger );
                mAxisState[ 6 ] = 0.0f;
                mAxisState[ 7 ] = 0.0f;

                // ���͈̔͂� -32768 �` 32767�B32767.f �Ŋ��邾������ -������ 1.0f ���������Ȃ�̂ŁA���̕ӂ̏C��
                for ( int i = 0; i < 4; ++i )
                {
                    mAxisState[ i ] = std::max( mAxisState[ i ], -1.0f );
                }

                // POV
                mPOVState =
                    ( state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT  ) ? POV_LEFT : 0  |
                    ( state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ) ? POV_RIGHT : 0 |
                    ( state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP    ) ? POV_UP : 0    |
                    ( state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN  ) ? POV_DOWN : 0;
   
                /*
                for ( int i = 0; i < 10; ++i )
                {
                    printf( "%d ", mButtonState[ i ] );
                }
                printf( "b:%x  pov:%x\n", state.Gamepad.wButtons, mPOVState );

                for ( int i = 0; i < 6; ++i )
                {
                    printf( "%f ", mAxisState[ i ] );
                }
                printf( "\n" );
                */

                connected = true;
            }
        }
        // DirectInput
        else
        {
            DIJOYSTATE state;
            hr = mDevice->GetDeviceState( sizeof( DIJOYSTATE ), &state );
            if ( SUCCEEDED( hr ) )
		    {
                // �{�^��
			    for ( int i = 0; i < LN_MAX_JOYPAD_BUTTONS; ++i )
			    {
				    mButtonState[ i ] = ( state.rgbButtons[ i ] & 0x80 ) != 0x00;
			    }

                // ��
                static const float r_range = 1.0f / AXIS_RANGE;
                mAxisState[ 0 ] = r_range * state.lX;
			    mAxisState[ 1 ] = r_range * state.lY;
			    mAxisState[ 2 ] = r_range * state.lZ;
			    mAxisState[ 3 ] = r_range * state.lRz;
                mAxisState[ 4 ] = r_range * state.lRx;
			    mAxisState[ 5 ] = r_range * state.lRy;
                mAxisState[ 6 ] = 0.0f;
                mAxisState[ 7 ] = 0.0f;

                // POV

                u32 pov = state.rgdwPOV[ 0 ];
                int dx, dy;

              
                if ( LOWORD( pov ) != 0xffff )
                {
 

			        if ( pov > 0 && pov < 18000 ) { 
	dx = 1; }	        // �E
			        else if ( pov > 18000 ) { 
			        	dx = -1; }		        // ��
			        else { 
			        	dx = 0; }

			        if ( pov > 9000 && pov < 27000 ) { 
			        	dy = 1; }	    // ��
			        else if ( pov < 9000 || pov > 27000  ) { 
			        	dy = -1; }	// ��
			        else { 
			        	dy = 0; }
                }
                else
                {
                    dx = dy = 0;
                }

                mPOVState =
                    ( ( dx ==  1 ) ? POV_RIGHT : 0 ) |
                    ( ( dx == -1 ) ? POV_LEFT : 0 )  |
                    ( ( dy ==  1 ) ? POV_DOWN : 0 )  |
                    ( ( dy == -1 ) ? POV_UP : 0 );


  

                /*
                printf( "%f %f %f %f %f %f\n",
                    mAxisState[ 0 ],
			        mAxisState[ 1 ],
			        mAxisState[ 2 ],
			        mAxisState[ 3 ],
                    mAxisState[ 4 ],
			        mAxisState[ 5 ] );
                */

                connected = true;
            }
            else
            {
                mDevice->Acquire();
                memset( mButtonState, 0, sizeof( mButtonState ) );
			    memset( mAxisState, 0, sizeof( mAxisState ) );
                mPOVState = -1;
            }
        }

        // ���̗V�ђׂ�
		for ( int i = 0; i < MAX_AXIS_NUM; ++i )
		{
            if ( abs( mAxisState[ i ] ) < mPlayRange )
            {
                mAxisState[ i ] = 0.0f;
            }
            
		}

        // �U���I���̊Ď�
        if ( mVibrationStartTime > 0 )
        {
            if ( System::getSystemTime() >= mVibrationStartTime + mVibrationTime )
            {
                stopVibration();
            }
        }

        return LN_OK;
	}

    //---------------------------------------------------------------------
	// �� �W���C�X�e�B�b�N�̎���񋓂���֐�
    //---------------------------------------------------------------------
    BOOL CALLBACK JoypadDevice::_enumAxesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi_, LPVOID this_ )
	{
		JoypadDevice* self = static_cast< JoypadDevice* >( this_ );

		// ���̒l�͈̔͂�ݒ�
		DIPROPRANGE diprg;
		ZeroMemory( &diprg, sizeof( diprg ) );
		diprg.diph.dwSize		= sizeof( diprg );
		diprg.diph.dwHeaderSize	= sizeof( diprg.diph ); 
		diprg.diph.dwObj		= lpddoi_->dwType;
		diprg.diph.dwHow		= DIPH_BYID;
		diprg.lMin				= -AXIS_RANGE;
		diprg.lMax				= AXIS_RANGE;
		HRESULT hr = self->mDevice->SetProperty( DIPROP_RANGE, &diprg.diph );
		if ( FAILED( hr ) ) { 
			return DIENUM_STOP; }

		return DIENUM_CONTINUE;
	}


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