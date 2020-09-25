//=============================================================================
// GameAudio_Basic
//-----------------------------------------------------------------------------
//  音声ファイルを BGM として演奏します
//=============================================================================

#include <lnote.h>

void main()
{
    HRESULT hr = LNote_InitAudio();
    if ( FAILED( hr ) ) return;

    LAudio_PlayBGM( "../../Media/ln28ti.ogg" );

    getchar();

    LNote_End();
}

//=============================================================================
//
//=============================================================================