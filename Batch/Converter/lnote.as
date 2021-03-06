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
#func  _LAudio_SetMetreUnitDistance "LAudio_SetMetreUnitDistance" float
#define LAudio_SetMetreUnitDistance(%1) _LAudio_SetMetreUnitDistance %1
#func  _L3DSoundListener_SetPosition "L3DSoundListener_SetPosition" float, float, float
#define L3DSoundListener_SetPosition(%1, %2, %3) _L3DSoundListener_SetPosition %1, %2, %3
#func  _L3DSoundListener_SetDirection "L3DSoundListener_SetDirection" float, float, float
#define L3DSoundListener_SetDirection(%1, %2, %3) _L3DSoundListener_SetDirection %1, %2, %3
#func  _L3DSoundListener_SetUpDirection "L3DSoundListener_SetUpDirection" float, float, float
#define L3DSoundListener_SetUpDirection(%1, %2, %3) _L3DSoundListener_SetUpDirection %1, %2, %3
#func  _L3DSoundListener_Velocity "L3DSoundListener_Velocity" float, float, float
#define L3DSoundListener_Velocity(%1, %2, %3) _L3DSoundListener_Velocity %1, %2, %3
#func  _LSound_CreateMem "LSound_CreateMem" var, int, int
#define LSound_CreateMem(%1, %2, %3=false) _LSound_CreateMem %1, %2, %3
#func  _LSound_Create "LSound_CreateToPtr" var, str, int, int
#define LSound_Create(%1, %2, %3=false, %4=LN_SOUNDPLAYTYPE_AUTO) _LSound_Create %1, %2, %3, %4
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
#func  _LSound_SetEmitterVelocity "LSound_SetEmitterVelocity" int, float, float, float
#define LSound_SetEmitterVelocity(%1, %2, %3, %4) _LSound_SetEmitterVelocity %1, %2, %3, %4
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
#func __LNote_InitAudio "LNote_InitAudio"
#define LNote_InitAudio _LNote_InitAudio
#func  _LNote_End "LNote_End"
#define LNote_End _LNote_End

#const LN_OK 0
#const LN_FALSE 1
#const LN_HANDLED 0x00042002
#const LN_ERR_ASSERT 0x80042001
#const LN_ERR_INVALIDCALL 0x80042002
#const LN_ERR_OUTOFMEMORY 0x80042003
#const LN_ERR_INDEX LN_ERR_INVALIDCALL
#const LN_ERR_NULLPOINTER 0x80042005
#const LN_ERR_FILENOTFOUND 0x80042006
#const LN_ERR_SYSTEM 0x80042007
#const LN_ERR_INITIALIZED 0x80042009
#const LN_ERR_NOINITIALIZE 0x8004200a
#const LN_ERR_FORMAT 0x8004200b
#const LN_ERR_NOTIMPL 0x8004200c
#const LN_ERR_FATAL 0x8004200d
#const LN_ERR_DIRECTX 0x8004200e
#const LN_ERR_OPENGL 0x8004200f
#const LN_ERR_COM LN_ERR_DIRECTX

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

// LNote_InitAudio
#deffunc _LNote_InitAudio
	//LException_SetEnableWrappedException@ 1
	_LConfig_SetUserWindowHandle@ hwnd
	__LNote_InitAudio@
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