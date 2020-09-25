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

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{
class X11Application;
    
//=============================================================================
// �� X11WindowBase �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class X11WindowBase
    : public Base::ReferenceObject
    , public WindowBase
{
public:

    X11WindowBase( X11Application* app_ )
        : mApplication  ( app_ )
        , mIsActive     ( false )
    {}

    virtual ~X11WindowBase() {}

    //LN_REFOBJ_METHODS;

public:

    /// �I������
    virtual void finalize() {}

    /// �_�~�[�E�B���h�E���𔻒肷��
    virtual bool isDummy() = 0;

    /// �E�B���h�E�̃A�N�e�B�u��Ԃ𔻒肷��
    virtual bool isActive() { return mIsActive; }

    /// �N���[�X�{�^���������ꂽ�Ƃ��ɌĂяo����A���̃E�B���h�E�Ɗ֘A�t����ꂽ�N���[�Y�{�^�����𔻒肷��
    virtual bool checkCloseButtonOfThis( Atom a1_, Atom a2_ ) { return false; }

protected:

    X11Application* mApplication;
    bool            mIsActive;      ///< Manager �o�R�Őݒ肳���
};

//=============================================================================
// �� X11Window �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �E�B���h�E�������N���X
//*/
//=============================================================================
class X11Window
    : public X11WindowBase
{
public:

    /// �������f�[�^
    struct InitData
    {
        Base::LogFile*      LogFile;        ///< ���O���o�͂��郍�O�t�@�C���I�u�W�F�N�g
        const lnChar*	    TitleText;		///< �E�B���h�E�^�C�g���̕�����
        int				    Width;			///< �N���C�A���g�̈�̕�
        int				    Height;			///< �N���C�A���g�̈�̍���
        bool			    Windowed;		///< �E�B���h�E���[�h�ŏ���������ꍇ true

        /// �f�t�H���g�l
        InitData()          
            : LogFile           ( NULL )
            , TitleText         ( LNTEXT( "default" ) )
            , Width             ( 640 )
            , Height            ( 480 )
            , Windowed          ( true )
        {}
    };

public:

    X11Window( X11Application* manager_, Display* display_ );

	virtual ~X11Window();

    LN_REFOBJ_METHODS;

public:

    /// ������
    LNRESULT initialize( const InitData& init_data_ );

    /// �I������
    void finalize();

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
    
    /// �_�~�[�E�B���h�E���𔻒肷��
    virtual bool isDummy() { return false; }

    /// �N���[�X�{�^���������ꂽ�Ƃ��ɌĂяo����A���̃E�B���h�E�Ɗ֘A�t����ꂽ�N���[�Y�{�^�����𔻒肷��
    virtual bool checkCloseButtonOfThis( Atom a1_, Atom a2_ ) { return ( a1_ == mAtomForCloseButton1 && a2_ == mAtomForCloseButton2 ); }

private:

    /// �E�B���h�E�T�C�Y��ύX����
    void _resize( bool fullscreen_ );

private:
	static const int		MAX_WINDOW_TEXT = 256;			///< �E�B���h�E�^�C�g��������̍ő啶����
	static const int		MAX_WINDOW_TEXT_FOOTER = 64;	///< �E�B���h�E�^�C�g��������̃t�b�^���̍ő啶����

private:

    X11Application*             mManager;
    Base::LogFile*              mLogFile;           ///< ���O���o�͂��郍�O�t�@�C���I�u�W�F�N�g
    ln_std_tstring    		    mTitleText;			///< �E�B���h�E�^�C�g���̕�����
    Geometry::Size				mClientSize;        ///< ���݂̃N���C�A���g�̈�̃T�C�Y
    int                         mOrginalWidth;      ///< initialize() �܂��� setSize() �Őݒ肳�ꂽ�N���C�A���g�̈�̕�
    int                         mOrginalHeight;     ///< initialize() �܂��� setSize() �Őݒ肳�ꂽ�N���C�A���g�̈�̍���
    
    
    Display*                    mXDisplay;
    int                         mXDefaultScreen;
    Window                      mXRootWindow;
    Window                      mXWindow;
    XSetWindowAttributes        mWindowAttributes;
    
    bool                        mFullScreen;        ///< �t���X�N���[�����[�h ( �p�̃X�^�C�� ) �̏ꍇ true
    

    Atom        mAtomForCloseButton1;
    Atom        mAtomForCloseButton2;

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