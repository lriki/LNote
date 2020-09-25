//=============================================================================
//【 Accessor 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Accessor.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Accessor
{

LFramework		Framework;
LMouse			Mouse;
LKeyboard		Keyboard;
LGameController	Input;
LGameAudio      Audio;
LGraphics       Graphics;
LCamera         Camera;
//LDebugFont      DebugFont;
//LDebugFont      ldout;

void initializeLNGlobalAccessor()
{
    // グローバル変数にちゃんとしたインスタンスを持たせていく

    Framework = LFramework::getInstance();

    LInput input = LInput::getInstance();
    if ( input != nil )
    {
        Mouse       = input.getMouse();
        Keyboard    = input.getKeyboard();
        Input       = input.getGameController( 0 );
    }

    Audio       = LGameAudio::getInstance();
    Graphics    = LGraphics::getInstance();
    Camera      = LCamera::getDefaultCamera();
    //DebugFont   = LDebugFont::getInstance();
    //ldout       = LDebugFont::getInstance();
}

void finalizeLNGlobalAccessor()
{
    Framework = LFramework();
    Mouse = LMouse();
    Keyboard = LKeyboard();
    Input = LGameController();
    Audio = LGameAudio();
    Graphics = LGraphics();
    Camera = LCamera();
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Accessor
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================