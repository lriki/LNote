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
#include "../Thread/Thread.h"
#include "../Thread/EventFlag.h"
#include "../Interface.h"
#include "../Common/ApplicationBase.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{
class X11WindowBase;

//=============================================================================
// �� X11Application �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�ЂƂ̃A�v���P�[�V�������ŕK�v�ȋ��ʏ���
//*/
//=============================================================================
class X11Application
    : public ApplicationBase
{
public:

    X11Application();

	virtual ~X11Application();

protected:

    /// ���C���E�B���h�E�����
    virtual LNRESULT onCreateMainWindow( WindowBase** window_ );

    /// �_�~�[�E�B���h�E����� (Manager::initialize() �ɃE�B���h�E�n���h�����n���ꂽ�Ƃ��� onCreateMainWindow() �̑���ɌĂ΂��)
    virtual WindowBase* onCreateDummyWindow( void* window_handle_ );

    /// �E�B���h�E�� finalize ���Ăяo���ĎQ�ƃJ�E���g���ЂƂ��炷
    virtual void onFinalizeWindow( WindowBase* window_ );

    /// �R���\�[�����J��
    virtual void onAllocConsole();

    /// �R���\�[�������
    virtual void onFreeConsole();

    /// ���b�Z�[�W����
    virtual void onProcMessage();

    /// �}�E�X�J�[�\���̕\����Ԃ�ݒ肷��
	virtual void onShowCursor( bool flag_ );

private:

    Display*        mXDisplay;
    X11WindowBase*  mX11MainWindow;
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