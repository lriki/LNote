//=============================================================================
//�y Application �z
//-----------------------------------------------------------------------------
///**
//  @file       Application.h
//  @brief      Application
//  @version    1.0
//  @date       2012/4/30
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <list>
#include "../../Thread/Thread.h"
#include "../../Thread/EventFlag.h"
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
class Win32WindowBase;

//=============================================================================
// �� Win32Application �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�ЂƂ̃A�v���P�[�V�������ŕK�v�ȋ��ʏ���
//*/
//=============================================================================
class Win32Application
    : public ApplicationBase
{
public:

    Win32Application();

	virtual ~Win32Application();

protected:

    /// ���C���E�B���h�E�����
    virtual LNRESULT onCreateMainWindow( WindowBase** window_ );

    /// �_�~�[�E�B���h�E�����
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