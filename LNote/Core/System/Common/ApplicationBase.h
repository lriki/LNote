//=============================================================================
//�y Application �z
//-----------------------------------------------------------------------------
///**
//  @file       Application.h
//  @brief      Application
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <list>
#include "../../Thread/Mutex.h"
#include "../../Thread/Thread.h"
#include "../../Thread/EventFlag.h"
#include "../Interface.h"
#include "WindowBase.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{
class MouseCursor;

#define LN_SYSTEMTHREADMSG_MAGIC 123456789


//=============================================================================
// �� ApplicationBase �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�A�v���P�[�V�����̊�ꏈ�� (���b�Z�[�W�����A�}�E�X�J�[�\������)
//*/
//=============================================================================
class ApplicationBase
    : public Base::ReferenceObject
    , public Thread::ThreadObject
{
public:

	/// �������f�[�^
	struct InitData
	{
        Base::LogFile*      LogFile;            ///< ���O���o�͂��郍�O�t�@�C���I�u�W�F�N�g
		Manager*			SystemManager;
        bool			    UseConsole;         ///< �R���\�[�������蓖�Ă�ꍇ true
        bool                UseThread;          ///< ���b�Z�[�W�����ɐ�p�̃X���b�h���g���ꍇ�� true
        void*               WindowHandle;       ///< ���[�U�[��`�̃E�B���h�E�n���h�� (windows �̏ꍇ�� HWND�AX11 �� Window*�B�������AX11 �͖��Ή�)
    
        const lnChar*	    TitleText;		///< �E�B���h�E�^�C�g���̕�����
		int				    Width;			///< �N���C�A���g�̈�̕�
		int				    Height;			///< �N���C�A���g�̈�̍���
		bool			    Windowed;		///< �E�B���h�E���[�h�ŏ���������ꍇ true
    };

public:

    ApplicationBase();

	virtual ~ApplicationBase();

public:

    /// ������
    LNRESULT initialize( const InitData& init_data_ );

    /// �I������
    void finalize();

    /// �E�B���h�E���擾����
    IWindow* getMainWindow() { return mMainWindow; }

    /// �}�E�X�J�[�\���̕\���ݒ� (flag_ �� false �̏ꍇ�Acount �����҂��Ă�������B0�ł�������)
    void setMouseCursorVisibleState( bool flag_, int count_ );

    /// �}�E�X���ړ������Ƃ��ɌĂяo��
    void onMoveCursor( bool on_client_area_ );

    /// �X�V
    void update();

    /// �C�x���g���b�Z�[�W���|�X�g����
    void postMessage( Event* event_ );

    /// ��������ׂ��C�x���g���b�Z�[�W�����邩���m�F���� (true=����)
    bool peekMessage();

    /// ��������ׂ��C�x���g���b�Z�[�W���擾���� (�擾�����ꍇ�A�L���[����폜���� )
    const Event* getMessage();

    /// �X���b�h�֐�
    virtual void run();

protected:

    /// ���C���E�B���h�E�����
    virtual LNRESULT onCreateMainWindow( WindowBase** window_ ) = 0;

    /// �_�~�[�E�B���h�E�����
    virtual WindowBase* onCreateDummyWindow( void* window_handle_ ) = 0;

    /// �E�B���h�E�� finalize ���Ăяo���ĎQ�ƃJ�E���g���ЂƂ��炷
    virtual void onFinalizeWindow( WindowBase* window_ ) = 0;

    /// �R���\�[�����J��
    virtual void onAllocConsole() = 0;

    /// �R���\�[�������
    virtual void onFreeConsole() = 0;

    /// ���b�Z�[�W���� (�}�E�X���ړ������Ƃ��� onMoveCursor() ���ĂԂ���)
    virtual void onProcMessage() = 0;

    /// �}�E�X�J�[�\���̕\����Ԃ�ݒ肷��
	virtual void onShowCursor( bool flag_ ) = 0;

private:

    /// ������
    LNRESULT _initialize();

    /// �I������
    void _finalize();

    /// �X�V
    void _update();

protected:

    typedef std::list< Event* >    EventQueue;      // const Event* �ɂ���� delete �ł��Ȃ��̂Œ���

protected:

    Base::LogFile*      mLogFile;           ///< ���O���o�͂��郍�O�t�@�C���I�u�W�F�N�g
	Manager*			mManager;
    InitData            mInitData;
    WindowBase*         mMainWindow;		///< �A�v���P�[�V�����̃��C���E�B���h�E

    EventQueue          mEventQueue;

    MouseCursor*        mMouseCursor;

    Thread::EventFlag   mFinishedInit;
    Thread::EventFlag   mEndRequest;
    Thread::Mutex       mLock;
    LNRESULT            mResultInit;
    bool                mThreaded;

    bool                mConsoleAlloced;    ///< �R���\�[�������蓖�Ă��ꍇ true
    bool                mIsSystemMouseShow;

    friend class MouseCursor;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace System
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================