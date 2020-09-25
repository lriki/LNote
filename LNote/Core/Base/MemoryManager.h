//=============================================================================
//�y MemoryManager �z
//-----------------------------------------------------------------------------
///**
//  @file       MemoryManager.h
//  @brief      MemoryManager
//  @author     Riki
//*/
//=============================================================================

#pragma once

#ifdef LNOTE_ENABLE_CRT_DEBUG

#include <crtdbg.h>
#define LN_NEW new//( _NORMAL_BLOCK, __FILE__, __LINE__ )

#else // LNOTE_ENABLE_CRT_DEBUG

#define LN_CDECL __cdecl

namespace LNote
{
namespace Core
{

enum LNMemoryDumpDest
{
    LN_MEMDIST_CONSOLE = 0,     ///< �W���o�͂ɏo�͂��� (printf)
    LN_MEMDIST_LOGFILE,         ///< MemoryManager �ɐݒ肳��Ă��郍�O�t�@�C���ɏo�͂���
    LN_MEMDIST_WIN32DEBUG       ///< Win32 �̃f�o�b�O�֐��ŏo�͂��� (OutputDebugString)
};

namespace Base
{
class LogFile;

//=============================================================================
// �� MemoryManager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class MemoryManager
{
public:

    /// ���������̏o�͐��ݒ肷��
    static void setMemoryDumpDest( LNMemoryDumpDest d_ );

    /// ���������̏o�͐�̃��O�t�@�C����ݒ肷��
    static void setLogFile( Base::LogFile* logfile_ );
};

} // namesapce Base
} // namespace Core
} // namespace LNote

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
enum LNMMFlag
{
    LN_MM_NORMAL_BLOCK = 1,
};
typedef char LNMMChar;

// VC++ �ł͒�`���Ȃ��ƃ_���B�t�� Android �ł͒�`����ƃ_���B
// LN_CLR_DLL �� ReleaseDLL �\���Ńv���W�F�N�g�v���p�e�B�����`�����
#if defined(LNOTE_MSVC) && !defined( LN_CLR_DLL )
    void* LN_CDECL operator new( size_t size_ );
    void LN_CDECL operator delete ( void* ptr_ );
#endif

void* LN_CDECL operator new( size_t size_, LNMMFlag flag_, const LNMMChar* filename_, int line_ );
void* LN_CDECL operator new[]( size_t size_, LNMMFlag flag_, const LNMMChar*  filename_, int line_ );
void LN_CDECL operator delete ( void* ptr_, LNMMFlag flag_, const LNMMChar*, int );
void LN_CDECL operator delete[] ( void* ptr_, LNMMFlag flag_, const LNMMChar*, int );

#ifdef LNOTE_DEBUG
    #define LN_NEW new( LN_MM_NORMAL_BLOCK, __FILE__, __LINE__ )
#else
    #define LN_NEW new
#endif

#endif // LNOTE_ENABLE_CRT_DEBUG

//=============================================================================
//								end of file
//=============================================================================