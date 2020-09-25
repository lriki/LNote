
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