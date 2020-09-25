//=============================================================================
//�y TChar �z
//-----------------------------------------------------------------------------
///**
//  @file       TChar.h
//  @brief      TChar
//  @author     Riki
//*/
//=============================================================================

#pragma once

#ifndef _INC_TCHAR

#include <stdio.h>
#include <wchar.h>

#if defined(LNOTE_WINDOWS)
    #define snprintf  sprintf_s
    #define ln_wfopen _wfopen
#endif

#if defined(LNOTE_GNUC)
// gcc �� _wfopne ���Ȃ��̂�
inline FILE* ln_wfopen( const wchar_t* filename_, const wchar_t* mode_ )
{
    char        path[ LN_MAX_PATH + 10 ];  // 10 �͂Ȃ�ƂȂ��O�̂���
    char        mode[ 10 ];
    int         len;
    mbstate_t   mbstate;
    memset ((void*)&mbstate, 0, sizeof( mbstate ) );

    len = wcslen( filename_ );
    wcsrtombs( path, &filename_, len + 1, &mbstate );
    if (errno == EILSEQ)
    {
        return NULL;
    }

    len = wcslen( mode_ );
    wcsrtombs( mode, &mode_, len + 1, &mbstate );
    if (errno == EILSEQ)
    {
        return NULL;
    }

    FILE* fp = fopen( path, mode );
    return fp;
    
}

// �����w�蕶������̕�������Ԃ�
#define _vscprintf( fmt_, args_ )   vsnprintf( NULL, 0, fmt_, args_ )
#define _vscwprintf( fmt_, args_ )  vwprintf( fmt_, args_ );    // �ŏI��i�c

#endif


#ifdef LNOTE_UNICODE
//-------------------------------------------------------------------------
// wchar_t
//-------------------------------------------------------------------------

#define __T(x)          L ## x

#define _tprintf        wprintf
#define _stprintf_s     swprintf

#define _tstoi          _wtoi

#define _tcslen         wcslen
#define _tcscat         wcscat
#define _tcscpy         wcscpy
#define _tcsncpy        wcsncpy
#define _tcscmp         wcscmp

#define _tfopen         ln_wfopen
#define _ftprintf       fwprintf

// ���w�蕶������̕�������Ԃ�(GNUC�Ԃ́��Œ�`)
#define _vsctprintf     _vscwprintf

// �����t���o�͂���������
#define _vstprintf_s    vswprintf   // vswnprintf �̂悤��NULL, 0 ��n���ĕ��������J�E���g�ł��Ȃ��̂Œ���



#else
//-------------------------------------------------------------------------
// char
//-------------------------------------------------------------------------

#if defined(LNOTE_MSVC)
    #define _ln_stricmp_ stricmp
#else
    #define _ln_stricmp_ strcasecmp
#endif

#define __T(x)          x

#define _tprintf        printf
#define _stprintf_s     snprintf

#define _tstoi          atoi
#define _tcstod         strtod

#define _tcslen         strlen
#define _tcscat         strcat
#define _tcscpy         strcpy
#define _tcsncpy        strncpy
#define _tcscmp         strcmp
#define _tcsicmp        _ln_stricmp_

#define _tfopen         fopen
#define _ftprintf       fprintf

// ���w�蕶������̕�������Ԃ�(GNUC�Ԃ́��Œ�`)
#define _vsctprintf     _vscprintf

/// �����t���o�͂���������
#define _vstprintf_s    vsnprintf


//#define _vsctprintf( format_, args_ )   vsnprintf( NULL, 0, format_, args_ )

#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

#define _T(x)           __T(x)
#define _TEXT(x)        __T(x)

#endif

//=============================================================================
//                              end of file
//=============================================================================