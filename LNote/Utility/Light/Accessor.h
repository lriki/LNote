//=============================================================================
//�y Accessor �z
//-----------------------------------------------------------------------------
///**
//  @file       Accessor.h
//  @brief      Accessor
//  @version    1.0
//  @date       2011/08/20
//  @par
//              L �v���t�B�b�N�X�̕t���Q�ƃN���X�̂����AFramework::Manager 
//              �ЂƂɑ΂��ĂЂƂ������݂���N���X�ւ̃A�N�Z�X���ȒP�ɂ��邽�߂�
//              �O���[�o���ϐ���錾���Ă��܂��B
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "UtilCommon.h"
#include "Framework/LFramework.h"
#include "Input/LInput.h"
#include "Input/LMouse.h"
#include "Input/LKeyboard.h"
#include "Input/LGameController.h"
#include "Audio/LGameAudio.h"
#include "Graphics/LGraphics.h"
#include "Graphics/LCamera.h"
#include "Graphics/LDebugFont.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Accessor
{

// �������ɏ�����������h�L�������g�ɂ��ǉ����Ă�������

extern LFramework		Framework;
extern LMouse			Mouse;
extern LKeyboard		Keyboard;
extern LGameController	Input;
extern LGameAudio       Audio;
extern LGraphics        Graphics;
extern LCamera          Camera;
//extern LDebugFont       DebugFont;
//extern LDebugFont       ldout;

//#ifndef LNOTE_PRIVATE_HIDE

void initializeLNGlobalAccessor();
void finalizeLNGlobalAccessor();

//#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Accessor
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================