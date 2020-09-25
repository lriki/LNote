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
#include <list>
#include "../Thread/EventFlag.h"
#include "Interface.h"
#include "OS.h"

#if defined(LNOTE_ANDROID)
struct android_app;
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{
class ApplicationBase;

//=============================================================================
// �� Manager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�V�X�e�� (OS ����) �Ǘ��N���X
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
    //, public IManager
{
public:

	/// �������f�[�^
	struct InitData
	{
        Base::LogFile*      LogFile;            ///< ���O���o�͂��郍�O�t�@�C���I�u�W�F�N�g
        bool			    UseConsole;         ///< [win32] �R���\�[�������蓖�Ă�ꍇ true
        bool                UseThread;          ///< [win32] ���b�Z�[�W�����ɐ�p�̃X���b�h���g���ꍇ�� true
        const lnChar*	    WindowTitleText;    ///< [win32] �E�B���h�E�^�C�g���̕�����
        lnU32               WindowWidth;        ///< �E�B���h�E�̃N���C�A���g�̈�̕�
        lnU32               WindowHeight;       ///< �E�B���h�E�̃N���C�A���g�̈�̍���
        bool			    Windowed;		    ///< [win32] �E�B���h�E���[�h�ŏ���������ꍇ true
        void*               WindowHandle;       ///< [win32] ���[�U�[��`�̃E�B���h�E�n���h�� (windows �̏ꍇ�� HWND�AX11 �� Window*�B�������AX11 �͖��Ή�)
#if defined(LNOTE_ANDROID)
        android_app*        AndroidApp;
#endif
    };

public:

    Manager();

	virtual ~Manager();

    LN_REFOBJ_METHODS;

public:

    /// ������
    LNRESULT initialize( const InitData& init_data_ );

    /// �I������
    void finalize();

	/// �E�B���h�E���擾����
    virtual IWindow* getWindow();

    /// ���b�Z�[�W����
	virtual bool processMessage();

    /// �C�x���g���X�i�[�̃A�^�b�` (�K���f�^�b�`���邱��)
    virtual void attachEventListener( IEventListener* listener_, int priority_ );

    /// �C�x���g���X�i�[�̃f�^�b�`
    virtual void detachEventListener( IEventListener* listener_ );

    /// �C�x���g�𑗐M����
    void sendEvent( Event* event_ );

    /// �C�x���g���|�X�g����
    virtual void postEvent( Event* event_ );

#if defined(LNOTE_WINDOWS)
    /// �E�B���h�E���b�Z�[�W�̏���
    virtual LRESULT WndProc( HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_, bool* handled_ );
#endif

    /// ���b�Z�[�W�R�[���o�b�N
	bool lnMsgProc( const Event& args_ );

public:

    struct ListenerEntry
    {
        IEventListener* EventListener;
        int             Priority;
    };
    typedef std::list< ListenerEntry > ListenerEntryList;

    class IsEven;

private:

    /// EventListener �\�[�g�p�̔�r�֐�
    static bool _cmpEventListener( const ListenerEntry& l_, const ListenerEntry& r_ )
    {
        return l_.Priority < r_.Priority;
    }

private:

    Base::LogFile*      mLogFile;           ///< ���O���o�͂��郍�O�t�@�C���I�u�W�F�N�g

    ApplicationBase*    mApplication;
    ListenerEntryList   mListenerEntryList;
    

    bool                mEndRequested;      ///< �I���v��
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