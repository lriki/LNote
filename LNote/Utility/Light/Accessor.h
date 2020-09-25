//=============================================================================
//【 Accessor 】
//-----------------------------------------------------------------------------
///**
//  @file       Accessor.h
//  @brief      Accessor
//  @version    1.0
//  @date       2011/08/20
//  @par
//              L プレフィックスの付く参照クラスのうち、Framework::Manager 
//              ひとつに対してひとつだけ存在するクラスへのアクセスを簡単にするための
//              グローバル変数を宣言しています。
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

// ※ここに書き加えたらドキュメントにも追加しておくこと

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