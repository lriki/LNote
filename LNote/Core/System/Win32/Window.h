//=============================================================================
//�y Window �z
//-----------------------------------------------------------------------------
///**
//  @file       Window.h
//  @brief      Window
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <string>
#include "../Interface.h"
#include "../Common/WindowBase.h"

#if defined(LNOTE_WINDOWS)

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{
class Win32Application;

//=============================================================================
// �� WindowBase �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      ���C�u�����ŕK�v�ȃE�B���h�E���b�Z�[�W����������v���V�[�W�������������
//*/
//=============================================================================
class Win32WindowBase
    : public Base::ReferenceObject
    , public WindowBase
{
public:

    Win32WindowBase( Win32Application* app_ )
        : mApplication  ( app_ )
        , mIsActive     ( false )
    {}

    virtual ~Win32WindowBase() {}

    LN_REFOBJ_METHODS;

public:

    /// �I������
    virtual void finalize() {}

    /// �_�~�[�E�B���h�E���𔻒肷��
    virtual bool isDummy() = 0;

    /// �E�B���h�E�̃A�N�e�B�u��Ԃ𔻒肷��
    virtual bool isActive() { return mIsActive; }

    /// �v���V�[�W���֐� (�A�N�e�B�u��Ԃ̕ω����������A�K�v�ȃ��b�Z�[�W������Application �ɃC�x���g�𑗂�)
    virtual LRESULT WndProc( HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_ );

protected:

    Win32Application*   mApplication;
    bool                mIsActive;      ///< Manager �o�R�Őݒ肳���
};

//=============================================================================
// �� Window �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�E�B���h�E�������N���X
//*/
//=============================================================================
class Window
    : public Win32WindowBase
{
public:

	/// �������f�[�^
	struct InitData
	{
        Base::LogFile*          LogFile;        ///< ���O���o�͂��郍�O�t�@�C���I�u�W�F�N�g
		const lnChar*	        TitleText;		///< �E�B���h�E�^�C�g���̕�����
		int				        Width;			///< �N���C�A���g�̈�̕�
		int				        Height;			///< �N���C�A���g�̈�̍���
		bool			        Windowed;		///< �E�B���h�E���[�h�ŏ���������ꍇ true
		//EventListener*          EventListener;	///< �C�x���g�������s���N���X�C���X�^���X�ւ̃|�C���^�܂��� NULL
        unsigned                MainThreadID;   ///< processMessageThread() ���Ăяo���X���b�h�̎��ʎq�܂��� 0
        const lnChar*	        WinClassName;   ///< �E�B���h�E�N���X�̖��O �܂��� NULL ( NULL �̏ꍇ�AL"_LNote_" ���g���� )
        int                     IconResourceID; ///< �^�C�g���o�[�̃A�C�R�����ʎq (IDI_ICON1 ��)
	
        /// �f�t�H���g�l
        InitData()          
            : LogFile           ( NULL )
            , TitleText         ( _T( "default" ) )
            , Width             ( 640 )
            , Height            ( 480 )
            , Windowed          ( true )
            //, EventListener ( NULL )
            , MainThreadID      ( 0 )
            , WinClassName      ( NULL )
            , IconResourceID    ( 0 )
        {}
    };

public:

    Window( Win32Application* manager_ );

	virtual ~Window();

public:

    /// ������
    LNRESULT initialize( const InitData& init_data_ );

    /// �I������
    virtual void finalize();

    /// �N���C�A���g�̈�̃T�C�Y���擾����
    virtual const Geometry::Size& getSize() { return mClientSize; }

    /// �E�B���h�E�̉���Ԃ�ݒ肷��
    virtual void setVisible( bool flag_ );

    /// �t���X�N���[���̗L���ݒ�
	virtual void setEnableFullScreen( bool flag_ );

    /// �t���X�N���[����Ԃ��𔻒肷��
    virtual bool isFullScreen() { return mFullScreen; }

    /// �E�B���h�E�̃A�N�e�B�u��Ԃ𔻒肷��
    virtual bool isActive() { return mIsActive; }

    /// �^�C�g���o�[�̕�����̌��ɒǉ��ŕ\�����镶�����ݒ肷��
	virtual void setWindowTextFooter( const lnChar* format_, ... );

#if defined(LNOTE_WINDOWS)

    /// �E�B���h�E�n���h���̎擾
    virtual HWND getWindowHandle() { return mWindowHandle; }

#endif

    /// �_�~�[�E�B���h�E���𔻒肷��
    virtual bool isDummy() { return false; }

private:

    /// �E�B���h�E�T�C�Y��ύX����
    void _resize( bool fullscreen_ );

    /// �Ăяo�����v���V�[�W���֐� (�E�B���h�E���쐬�����X���b�h���ŌĂ΂��)
	static LRESULT CALLBACK _staticMsgProc( HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_ );

    /// �v���V�[�W���֐� ()
    virtual LRESULT WndProc( HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_ );

private:

	static const lnChar*	WINDOW_CLASS_NAME;				///< �E�B���h�E�N���X��	  "_LNote_"
	static const lnChar*	PROP_WINPROC;					///< SetProp() �ɓn�����O "_LNoteProp_"
	static const int		MAX_WINDOW_TEXT = 256;			///< �E�B���h�E�^�C�g��������̍ő啶����
	static const int		MAX_WINDOW_TEXT_FOOTER = 64;	///< �E�B���h�E�^�C�g��������̃t�b�^���̍ő啶����

private:

    Win32Application*           mManager;
    Base::LogFile*              mLogFile;           ///< ���O���o�͂��郍�O�t�@�C���I�u�W�F�N�g
    ln_std_tstring    		    mTitleText;			///< �E�B���h�E�^�C�g���̕�����    
    Geometry::Size				mClientSize;        ///< �N���C�A���g�̈�̑傫��
    int                         mOrginalWidth;      ///< initialize() �܂��� setSize() �Őݒ肳�ꂽ�N���C�A���g�̈�̕�
    int                         mOrginalHeight;     ///< initialize() �܂��� setSize() �Őݒ肳�ꂽ�N���C�A���g�̈�̍���
	HWND				        mWindowHandle;		///< �E�B���h�E�n���h��
	RECT				        mWindowRect;		///< �E�B���h�E�̈�
    HACCEL                      mAccelerators;      ///< �A�N�Z�����[�^ (Alt+Enter �̌x�����΍�̂��߂Ɏg���Ă�)
    unsigned                    mMainThreadID;      ///< �}���`�X���b�h�����̂Ƃ��Ɏg���A���C���X���b�h�̎��ʎq
    bool                        mFullScreen;        ///< �t���X�N���[�����[�h ( �p�̃X�^�C�� ) �̏ꍇ true
    bool				        mIsActive;			///< �E�B���h�E���A�N�e�B�u�̏ꍇ true

};

//=============================================================================
// �� DummyWindow �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �E�B���h�E�n���h�������b�v����N���X
//*/
//=============================================================================
class DummyWindow
    : public Win32WindowBase
{
public:

    DummyWindow( Win32Application* app_, void* hwnd_ )
        : Win32WindowBase    ( app_ )
        , mWindowHandle     ( hwnd_ )
    {}

    virtual ~DummyWindow() {}

public:

    /// �N���C�A���g�̈�̃T�C�Y���擾����
	virtual const Geometry::Size& getSize()
    {
        // �R���X�g���N�^�� mClientSize �Ɋi�[���Ă����Ă������Ǝv�������ǁA
        // �t���X�N���[�������ŃE�B���h�E�T�C�Y���ς�������̑Ή����ʓ|�����Ȃ̂ł��̂܂܁B
#if defined(LNOTE_WINDOWS)
        RECT rc;
        ::GetClientRect( getWindowHandle(), &rc );
        mClientSize.set( rc.right, rc.bottom );
        return mClientSize;
        
#elif defined(LNOTE_X11)
        Display* d = XOpenDisplay( NULL );
        Window root;
        int x, y;
		unsigned int width, height;
		unsigned int border;
		unsigned int depth;
        XGetGeometry(
            d, getWindowHandle(), &root, &x, &y,
            &width, &height, &border, &depth );
        return mClientSize.set( width, height );
#endif
    }

    virtual void setVisible( bool flag_ ) {}

    virtual void setEnableFullScreen( bool flag_ ) {}

    virtual bool isFullScreen() { return false; }

    virtual bool isActive() { return mIsActive; }

    virtual void setWindowTextFooter( const lnChar* format_, ... ) {}

    virtual HWND getWindowHandle() { return static_cast< HWND >( mWindowHandle ); }

    /// �_�~�[�E�B���h�E���𔻒肷��
    virtual bool isDummy() { return true; }

private:

    void*			mWindowHandle;
    Geometry::Size	mClientSize;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace System
} // namespace Core
} // namespace LNote

#endif // LNOTE_WINDOWS

//=============================================================================
//								end of file
//=============================================================================