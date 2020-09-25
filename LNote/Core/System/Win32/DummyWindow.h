//=============================================================================
//�y DummyWindow �z
//-----------------------------------------------------------------------------
///**
//  @file       DummyWindow.h
//  @brief      DummyWindow
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{
class Manager;

//=============================================================================
// �� DummyWindow �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �E�B���h�E�n���h�������b�v����N���X
//*/
//=============================================================================
class DummyWindow
    : public Base::ReferenceObject
    , public IWindow
{
public:

    DummyWindow( void* hwnd_ )
        : mWindowHandle ( hwnd_ )
        , mIsActive     ( true )
    {}

    virtual ~DummyWindow() {}

    LN_REFOBJ_METHODS;

public:

    //

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

    /// �E�B���h�E�̉���Ԃ�ݒ肷��
    virtual void setVisible( bool flag_ ) {}

    /// �t���X�N���[���̗L���ݒ�
    virtual void setEnableFullScreen( bool flag_ ) {}

    /// �t���X�N���[����Ԃ��𔻒肷��
    virtual bool isFullScreen() { return false; }

    /// �E�B���h�E�̃A�N�e�B�u��Ԃ𔻒肷��
    virtual bool isActive() { return mIsActive; }

    /// �^�C�g���o�[�̕�����̌��ɒǉ��ŕ\�����镶�����ݒ肷��
    virtual void setWindowTextFooter( const lnChar* format_, ... ) {}

#if defined(LNOTE_WINDOWS)
    /// �E�B���h�E�n���h���̎擾
    virtual HWND getWindowHandle() { return static_cast< HWND >( mWindowHandle ); }
#elif defined(LNOTE_X11)
    /// �E�B���h�E�n���h���̎擾
    virtual ::Window* getWindowHandle() { return static_cast< ::Window* >( mWindowHandle ); }
#endif

private:

    void*			mWindowHandle;
    Geometry::Size	mClientSize;
    bool			mIsActive;      ///< Manager �o�R�Őݒ肳���

    friend class Manager;
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