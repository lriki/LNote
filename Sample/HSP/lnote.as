//=============================================================================
// LNote
//=============================================================================
#ifndef __lnote__
#define __lnote__

#uselib "LNote.dll"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#ifndef true
#const true 1
#endif

#ifndef false
#const false 0
#endif

#ifndef SUCCEEDED
#define SUCCEEDED(%1)   ((%1) >= 0)
#endif

#ifndef FAILED
#define FAILED(%1)      ((%1) <  0)
#endif

#define LN_MUSICTIMEBASE 768

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#func __LAudio_Init "LAudio_Init"
#define LAudio_Init _LAudio_Init
#func __LAudio_PlayBGM "LAudio_PlayBGM" str, int, int, int
#define LAudio_PlayBGM(%1, %2=100, %3=100, %4=0) _LAudio_PlayBGM %1, %2, %3, %4
#func  _LAudio_PlayBGMMem "LAudio_PlayBGMMem" var, int, int, int, int
#define LAudio_PlayBGMMem(%1, %2, %3=100, %4=100, %5=0) _LAudio_PlayBGMMem %1, %2, %3, %4, %5
#func  _LAudio_StopBGM "LAudio_StopBGM" int
#define LAudio_StopBGM(%1=0) _LAudio_StopBGM %1
#func __LAudio_PlayBGS "LAudio_PlayBGS" str, int, int, int
#define LAudio_PlayBGS(%1, %2=100, %3=100, %4=0) _LAudio_PlayBGS %1, %2, %3, %4
#func  _LAudio_PlayBGSMem "LAudio_PlayBGSMem" var, int, int, int, int
#define LAudio_PlayBGSMem(%1, %2, %3=100, %4=100, %5=0) _LAudio_PlayBGSMem %1, %2, %3, %4, %5
#func  _LAudio_StopBGS "LAudio_StopBGS" int
#define LAudio_StopBGS(%1=0) _LAudio_StopBGS %1
#func __LAudio_PlayME "LAudio_PlayME" str, int, int
#define LAudio_PlayME(%1, %2=100, %3=100) _LAudio_PlayME %1, %2, %3
#func  _LAudio_PlayMEMem "LAudio_PlayMEMem" var, int, int, int
#define LAudio_PlayMEMem(%1, %2, %3=100, %4=100) _LAudio_PlayMEMem %1, %2, %3, %4
#func  _LAudio_StopME "LAudio_StopME"
#define LAudio_StopME _LAudio_StopME
#func __LAudio_PlaySE "LAudio_PlaySE" str, int, int
#define LAudio_PlaySE(%1, %2=100, %3=100) _LAudio_PlaySE %1, %2, %3
#func __LAudio_PlaySE3D "LAudio_PlaySE3D" str, float, float, float, float, int, int
#define LAudio_PlaySE3D(%1, %2, %3, %4, %5, %6=100, %7=100) _LAudio_PlaySE3D %1, %2, %3, %4, %5, %6, %7
#func  _LAudio_PlaySEMem "LAudio_PlaySEMem" var, int, int, int
#define LAudio_PlaySEMem(%1, %2, %3=100, %4=100) _LAudio_PlaySEMem %1, %2, %3, %4
#func  _LAudio_PlaySE3DMem "LAudio_PlaySE3DMem" var, int, float, float, float, float, int, int
#define LAudio_PlaySE3DMem(%1, %2, %3, %4, %5, %6, %7=100, %8=100) _LAudio_PlaySE3DMem %1, %2, %3, %4, %5, %6, %7, %8
#func  _LAudio_StopSE "LAudio_StopSE"
#define LAudio_StopSE _LAudio_StopSE
#func  _L3DSoundListener_SetPosition "L3DSoundListener_SetPosition" float, float, float
#define L3DSoundListener_SetPosition(%1, %2, %3) _L3DSoundListener_SetPosition %1, %2, %3
#func  _L3DSoundListener_SetDirection "L3DSoundListener_SetDirection" float, float, float
#define L3DSoundListener_SetDirection(%1, %2, %3) _L3DSoundListener_SetDirection %1, %2, %3
#func  _L3DSoundListener_SetUpDirection "L3DSoundListener_SetUpDirection" float, float, float
#define L3DSoundListener_SetUpDirection(%1, %2, %3) _L3DSoundListener_SetUpDirection %1, %2, %3
#func  _LSound_CreateMem "LSound_CreateMem" var, int, int
#define LSound_CreateMem(%1, %2, %3=false) _LSound_CreateMem %1, %2, %3
#func  _LSound_Load "LSound_LoadToPtr" var, str, int, int
#define LSound_Load(%1, %2, %3=false, %4=LN_SOUNDPLAYTYPE_AUTO) _LSound_Load %1, %2, %3, %4
#func  _LSound_Release "LSound_Release" int
#define LSound_Release(%1) _LSound_Release %1
#func  _LSound_SetVolume "LSound_SetVolume" int, int
#define LSound_SetVolume(%1, %2) _LSound_SetVolume %1, %2
#func  _LSound_GetVolume "LSound_GetVolume" int
#define LSound_GetVolume(%1) _LSound_GetVolume %1
#func  _LSound_SetPitch "LSound_SetPitch" int, int
#define LSound_SetPitch(%1, %2) _LSound_SetPitch %1, %2
#func  _LSound_GetPitch "LSound_GetPitch" int
#define LSound_GetPitch(%1) _LSound_GetPitch %1
#func  _LSound_SetLoopState "LSound_SetLoopState" int, int, int, int
#define LSound_SetLoopState(%1, %2, %3=0, %4=0) _LSound_SetLoopState %1, %2, %3, %4
#func  _LSound_IsLoop "LSound_IsLoop" int
#define LSound_IsLoop(%1) _LSound_IsLoop %1
#func  _LSound_GetPlayState "LSound_GetPlayState" int
#define LSound_GetPlayState(%1) _LSound_GetPlayState %1
#func  _LSound_Play "LSound_Play" int
#define LSound_Play(%1) _LSound_Play %1
#func  _LSound_Stop "LSound_Stop" int
#define LSound_Stop(%1) _LSound_Stop %1
#func  _LSound_Pause "LSound_Pause" int, int
#define LSound_Pause(%1, %2) _LSound_Pause %1, %2
#func  _LSound_FadeVolume "LSound_FadeVolume" int, int, int, int
#define LSound_FadeVolume(%1, %2, %3, %4) _LSound_FadeVolume %1, %2, %3, %4
#func  _LSound_GetUnitsPlayed "LSound_GetUnitsPlayed" int
#define LSound_GetUnitsPlayed(%1) _LSound_GetUnitsPlayed %1
#func  _LSound_GetTotalUnits "LSound_GetTotalUnits" int
#define LSound_GetTotalUnits(%1) _LSound_GetTotalUnits %1
#func  _LSound_GetUnitsParSecond "LSound_GetUnitsParSecond" int
#define LSound_GetUnitsParSecond(%1) _LSound_GetUnitsParSecond %1
#func  _LSound_Is3DSound "LSound_Is3DSound" int
#define LSound_Is3DSound(%1) _LSound_Is3DSound %1
#func  _LSound_SetEmitterPosition "LSound_SetEmitterPosition" int, float, float, float
#define LSound_SetEmitterPosition(%1, %2, %3, %4) _LSound_SetEmitterPosition %1, %2, %3, %4
#func  _LSound_SetEmitterDistance "LSound_SetEmitterDistance" int, float
#define LSound_SetEmitterDistance(%1, %2) _LSound_SetEmitterDistance %1, %2
#func  _LConfig_SetEnableLogFile "LConfig_SetEnableLogFile" int
#define LConfig_SetEnableLogFile(%1) _LConfig_SetEnableLogFile %1
#func  _LConfig_SetEnableConsole "LConfig_SetEnableConsole" int
#define LConfig_SetEnableConsole(%1) _LConfig_SetEnableConsole %1
#func  _LConfig_SetUserWindowHandle "LConfig_SetUserWindowHandle" int
#define LConfig_SetUserWindowHandle(%1) _LConfig_SetUserWindowHandle %1
#func  _LConfig_SetDirectMusicUsingMode "LConfig_SetDirectMusicUsingMode" int
#define LConfig_SetDirectMusicUsingMode(%1) _LConfig_SetDirectMusicUsingMode %1

#const LN_OK 0
#const LN_FALSE 1
#const LN_HANDLED 2
#const LN_ERR_ASSERT -1
#const LN_ERR_INVALIDCALL -2
#const LN_ERR_OUTOFMEMORY -3
#const LN_ERR_INDEX -4
#const LN_ERR_NULLPOINTER -5
#const LN_ERR_FILENOTFOUND -6
#const LN_ERR_SYSTEM -7
#const LN_ERR_INITIALIZED -9
#const LN_ERR_NOINITIALIZE -10
#const LN_ERR_FORMAT -11
#const LN_ERR_NONIMPLEMENTED -12
#const LN_ERR_FATAL -15
#const LN_ERR_DIRECTX -99
#const LN_ERR_OPENGL -100
#const LN_ERR_COM LN_ERR_DIRECTX
#const LN_SOUNDPLAYTYPE_AUTO 0
#const LN_SOUNDPLAYTYPE_ONMEMORY 1
#const LN_SOUNDPLAYTYPE_STREAMING 2
#const LN_SOUNDPLAYTYPE_MIDI 3
#const LN_SOUNDPLAYTYPE_UNKNOWN 0xffff
#const LN_SOUNDPLAYSTATE_STOP 0
#const LN_SOUNDPLAYSTATE_PLAYING 1
#const LN_SOUNDPLAYSTATE_PAUSING 2
#const LN_SOUNDFADE_CONTINUE 0
#const LN_SOUNDFADE_STOP 1
#const LN_SOUNDFADE_STOP_RESET 2
#const LN_SOUNDFADE_PAUSE 3
#const LN_SOUNDFADE_PAUSE_RESET 4
#const LN_DMUSINGMODE_NOTUSE 0
#const LN_DMUSINGMODE_USE 1
#const LN_DMUSINGMODE_THREAD_WAIT 2
#const LN_DMUSINGMODE_THREAD_REQUEST 3

//-----------------------------------------------------------------------------
// internal
//-----------------------------------------------------------------------------
#func LAudio_End onexit "LAudio_End"
#func LFile_Exist "LFile_Exist" str

//-----------------------------------------------------------------------------
// module
//-----------------------------------------------------------------------------
#module "LNote"

// LFile_ExistDirectoryOrEXE(stat: 0=なし 1=ディレクトリに存在(優先) 2=EXEに存在)
#deffunc LFile_ExistDirectoryOrEXE str p1
	LFile_Exist@ p1
	if stat != 0 : return 1
	exist p1
	if strsize != -1 : return 2
	return 0

// LAudio_Init
#deffunc _LAudio_Init
	_LConfig_SetUserWindowHandle@ hwnd
	__LAudio_Init@
	return

// LAudio_PlayBGM
#deffunc _LAudio_PlayBGM str p1, int p2, int p3, int p4
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlayBGMMem@ buf, strsize, p2, p3, p4
	} else : if stat == 1 {
		__LAudio_PlayBGM@ p1, p2, p3, p4
	}
	return

// LAudio_PlayBGS
#deffunc _LAudio_PlayBGS str p1, int p2, int p3, int p4
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlayBGSMem@ buf, strsize, p2, p3, p4
	} else : if stat == 1 {
		__LAudio_PlayBGS@ p1, p2, p3, p4
	}
	return

// LAudio_PlayME
#deffunc _LAudio_PlayME str p1, int p2, int p3
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlayMEMem@ buf, strsize, p2, p3
	} else : if stat == 1 {
		__LAudio_PlayME@ p1, p2, p3
	}
	return

// LAudio_PlaySE
#deffunc _LAudio_PlaySE str p1, int p2, int p3
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlaySEMem@ buf, strsize, p2, p3
	} else : if stat == 1 {
		__LAudio_PlaySE@ p1, p2, p3
	}
	return

// LAudio_PlaySE3D
#deffunc _LAudio_PlaySE3D str p1, double p2, double p3, double p4, double p5, int p6, int p7
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlaySE3DMem@ buf, strsize, p2, p3, p4, p5, p6, p7
	} else : if stat == 1 {
		__LAudio_PlaySE3D@ p1, p2, p3, p4, p5, p6, p7
	}
	return
	
#global

#endif
//=============================================================================
//
//=============================================================================