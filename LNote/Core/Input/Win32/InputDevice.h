//=============================================================================
//�y InputDevice �z
//-----------------------------------------------------------------------------
///**
//  @file       InputDevice.h
//  @brief      InputDevice
//  @version    1.0
//  @date       2012/2/16
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <deque>
#define DIRECTINPUT_VERSION 0x0800
#include "XInputModule.h"
#include <dinput.h>
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
class JoypadDevice;

//=============================================================================
// �� InputDevice �N���X
//-----------------------------------------------------------------------------
///**
// @brief      DirectInput ����� XInput
//*/
//=============================================================================
class InputDevice
    : public Base::ReferenceObject
    , public IInputDevice
{
public:

    /// initialize() �ɓn���������f�[�^
	struct InitData
	{

        System::IWindow*    Window;     ///< DirectInput �̏������Ɏg���E�B���h�E
	};

public:

	/// �R���X�g���N�^
	InputDevice();

	/// �f�X�g���N�^
	virtual ~InputDevice();

    LN_REFOBJ_METHODS;

public:

    /// ������
	LNRESULT initialize( const InitData& init_data_ );

    /// �ڑ��ς݂̃W���C�p�b�h���̎擾
    virtual int getJoypadNum() { return mJoypadNum; }

    /// �}�E�X�̏�Ԃ̎擾
    virtual const LNMouseDeviceState& getMouseState() { return mMouseState; }

    /// �L�[�{�[�h�̏�Ԃ̎擾
    virtual bool getKeyState( LNVirtualKeyCode vkey_ );

    /// �L�[�{�[�h�̃o�b�t�@�����O�f�[�^�̎擾
    virtual LNVirtualKeyCode getKeyBufferState();

    /// �W���C�p�b�h�̏�Ԃ̎擾
	virtual void getJoypadState( int joypad_id_, LNJoypadDeviceState* state_ );

    /// �W���C�p�b�h�̐U�����J�n����
	virtual void startVibration( int joypad_id_, int power_, int time_ );

    /// �W���C�p�b�h�̐U�����~����
	virtual void stopVibration( int joypad_id_ );

    /// �^�b�`���̎擾
    virtual void getTouchState( int touch_id_, LNTouchDeviceState* state_ ) { state_->Touch = false; }

    /// ���z�{�^���̏�Ԃ̎擾
	virtual float getVirtualButtonState( u32 button_, bool keyboard_, bool mouse_, int joypad_id_ );

    /// �}�E�X���W�̐ݒ�
	virtual void setMousePoint( const LVector2& point_ );

    /// �}�E�X�J�[�\���̈ʒu���N���C�A���g�̈�̒����ɐݒ肷��
    virtual void setMouseCursorCenter();

    /// ���͏��̍X�V
	virtual LNRESULT update();

    /// �f�o�C�X�̍ăZ�b�g�A�b�v
	virtual LNRESULT refresh();

    /// �֘A�t�����Ă��� IWindow �̎擾
    virtual System::IWindow* getWindow() { return mWindow; }

    /// �C�x���g���� (Manager::onEvent() ����Ă΂��)
    virtual bool onEvent( const System::Event& args_ );

private:

    /// ���
    void _release();

    /// �W���C�p�b�h�̉��
    void _releaseJoypads();

    /// �}�E�X�f�o�C�X�̏�����
    LNRESULT _initMouse();

private:

    /// �W���C�X�e�B�b�N��񋓂���R�[���o�b�N�֐�
	static BOOL CALLBACK _enumJoysticksCallback( const DIDEVICEINSTANCE* instance_, VOID* this_ );

	/// �W���C�X�e�B�b�N�̃f�o�C�X�I�u�W�F�N�g���쐬����
	void _createJoysticksDevice( const DIDEVICEINSTANCE* instance_ );

    /// Xbox �R���g���[�����𔻒肷��
    static BOOL _isXInputDevice( const GUID* pGuidProductFromDirectInput );

private:

    typedef std::deque< LNVirtualKeyCode >     KeyBuffer;

private:

    System::IWindow*            mWindow;
    HWND                        mWindowHandle;
    IDirectInput8*              mDirectInput;
    XInputModule*               mXInputModule;

    bool                        mKeyStatus[ LN_MAX_KEYS ];
    KeyBuffer                   mKeyBuffer;

    IDirectInputDevice8*	    mMouseDevice;				///< �}�E�X�f�o�C�X
	LNMouseDeviceState	        mMouseState;				///< �}�E�X�̏��

    JoypadDevice*               mJoypads[ LN_MAX_JOYPADS ];
    int                         mJoypadNum;
    int                         mXInputNo;

    
    bool                        mCoInited;
	bool						mEnableKeyBuffering;
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