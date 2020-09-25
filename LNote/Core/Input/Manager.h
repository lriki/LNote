//=============================================================================
//�y Manager �z
//-----------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Geometry/Matrix.h"
#include "Interface.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Touch.h"
#include "GameController.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Input
{
class Joypad;

//=============================================================================
// �� Manager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      ���̓��W���[���̊Ǘ��N���X
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
    , public IManager
    , public System::IEventListener
{
public:

	/// �R���X�g���N�^�ɓn���������f�[�^
	struct InitData
	{
        Base::LogFile*      LogFile;
        System::IWindow*    Window;		        ///< DirectInput �̏������Ɏg���E�B���h�E�n���h��
	};

public:

	/// �R���X�g���N�^
	Manager();

	/// �f�X�g���N�^
	virtual ~Manager();

    LN_REFOBJ_METHODS;

public:

    /// ������
    LNRESULT initialize( const InitData& data_ );

    /// �I������
    void finalize();

    /// GameController �ւ̃|�C���^�̎擾
    virtual IGameController* getGameController( int index_ ) { return mGameControllers[ index_ ]; }

	/// Mouse �ւ̃|�C���^�̎擾
	virtual IMouse* getMouse() { return mMouse; }

	/// Keyboard �ւ̃|�C���^�̎擾
	virtual IKeyboard* getKeyboard() { return mKeyboard; }

	/// Joypad �ւ̃|�C���^�̎擾
	virtual IJoypad* getJoypad( int index_ );
    
    /// Touch �ւ̃|�C���^�̎擾
	virtual Touch* getTouch() { return mTouch; }

	/// �ڑ�����Ă���W���C�p�b�h�̐��̎擾
	virtual int getJoypadNum() { return 0; }

    /// IInputDevice �ւ̃|�C���^�̎擾
    virtual IInputDevice* getInputDevice();

    /// �t���[���X�V
    virtual void update();

    /// ���b�Z�[�W�����R�[���o�b�N
    virtual bool onEvent( const System::Event& args_ );

    void setMouseTransform( const LMatrix& transform_ ) { mMouseTransform = transform_; }

public:

    /// �}�E�X�ʒu�̕ϊ��s��̎擾
    const LMatrix& getMouseTransform() const { return mMouseTransform; }

    /// �}�E�X�ʒu���ݒ�ł��邩
    bool canSetMousePoint() const;

private:

    Base::LogFile*              mLogFile;
    //Win32::InputDevice*	        mInputDevice;
    IInputDevice*	            mInputDevice;
    LMatrix                     mMouseTransform;
	Mouse*			            mMouse;
	Keyboard*		            mKeyboard;
	Joypad*			            mJoypads[ LN_MAX_JOYPADS ];
    Touch*                      mTouch;
	GameController*	            mGameControllers[ LN_MAX_GAME_CONTROLLERS ];

    bool                        mCanSetMousePoint;

};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Input
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================