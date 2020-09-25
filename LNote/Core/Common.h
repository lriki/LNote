//=============================================================================
//【 Common 】
//-----------------------------------------------------------------------------
///**
//  @file       Common.h
//  @brief      Common
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-----------------------------------------------------------------------------
// flags

// Debug ビルドフラグ
#if defined( DEBUG ) || defined( _DEBUG )
#   define LNOTE_DEBUG
#endif

// プラットフォーム
#if defined(__ANDROID__)  // Android
#   define LNOTE_ANDROID
#   define LNOTE_GNUC
#   define LNOTE_OPENGL
#   define LNOTE_GLES
#elif defined( __WIN32__ ) || defined( _WIN32 )// || defined( __CYGWIN__ )
#   define LNOTE_WINDOWS
#   define LNOTE_MSVC
#   define LNOTE_DIRECTX
#   define LNOTE_OPENGL
#else
#   define LNOTE_GNUC
#   define LNOTE_X11
#   define LNOTE_OPENGL
#endif

// マルチバイト文字セット使用の強制
#if defined(LNOTE_SPECIFIED_CHARCODESET_MBS)
#   undef UNICODE
#   undef _UNICODE
#   ifndef _MBCS
#       define _MBCS
#   endif
// UNICODE 文字セット使用の強制
#elif defined(LNOTE_SPECIFIED_CHARCODESET_UNICODE)
#   ifndef UNICODE
#       define UNICODE
#   endif
#   ifndef _UNICODE
#       define _UNICODE
#   endif
#   undef _MBCS
#endif

// LNOTE_UNICODE 使用フラグ
#if defined( UNICODE ) || defined( _UNICODE )
#   define LNOTE_UNICODE
#else
#   define LNOTE_MBS
#endif

// 警告無視
#if defined(LNOTE_DISABLE_WARNING_4996) && (_MSC_VER >= 1200)
#   pragma warning( disable:4996 )
#endif

//-------------------------------------------------------------------------
// constant

#define LNOTE_NAME         _T( "LNote" )

// バージョン (上位 4 バイトがメジャー、下位 4 バイトがマイナー)
#define LNOTE_VERSION      0x00000001
#define LNOTE_VERSION_STR  _T( "0.1a" )

#if defined(LNOTE_WINDOWS)
    #define LN_MAX_PATH     ( 260 )         // 260 (windows の MAX_PATH)
#else
    #define LN_MAX_PATH     ( 255 )         // UNIX 系 OS のファイル名最大長
#endif
#define LN_MUSICTIMEBASE    ( 768 )         // Midi 用ミュージックタイムのベース値
#define LN_INFINITE_TIME    ( 0xFFFFFFFF )  // Infinite timeout

static const unsigned long LN_INVALID_INDEX = 0xffffffff;

//-------------------------------------------------------------------------
// include

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

// tchar.h の扱い
#if defined(LNOTE_WINDOWS)
#   if defined(LNOTE_INCLUDE_INTERNAL_TCHAR)
#       include "Base/TChar.h"     // _T() のために先に include
#   else
#       include <tchar.h>
#   endif
#else
#   include "Base/TChar.h"
#endif

#if defined(LNOTE_INCLUDE_ENV_DEPEND_HEADER)
#   if defined(LNOTE_WINDOWS)
#       define INITGUID
#       include <winsock2.h>
#       include <mmreg.h>
#       include <msacm.h>
#       include <crtdbg.h>
#       include <locale.h>     // for Framework::init
#       include <numeric>
#       define NOMINMAX
#       include <windows.h>
#       undef min
#       undef max
#       undef NOMINMAX
#       include <process.h>
#   if defined(LNOTE_DIRECTX)
#       include <d3d9.h>
#       include <d3dx9.h>
#       include <xaudio2.h>
#       include <x3daudio.h>
#       define DIRECTINPUT_VERSION 0x0800
#       include <dinput.h>
#       include <xinput.h>
#       define LNOTE_INCLUDED_DIRECTX_HEADER (1)
#   endif
#   if defined(LNOTE_OPENGL)
#       include "../Dependencies/glee/GLee.h"
#   endif
#   elif defined(LNOTE_ANDROID)
#       include <stdint.h>
#       include <unistd.h>
#       include <ctype.h>
#       include <pthread.h>
#       include <jni.h>
#       include <android/log.h>
#       include <GLES2/gl2.h>      // for GLenum
#   else   // LNOTE_WINDOWS (LNOTE_X11)
#       include <stdint.h>
#       include <unistd.h>
#       include <pthread.h>
#       include <X11/Xlib.h>
#       include <X11/Xutil.h>
#       include <X11/Xlocale.h>
#       include <X11/keysym.h>
#   endif
#else
#   if defined(LNOTE_WINDOWS)
#       define NOMINMAX
#       include <windows.h>
#       undef min
#       undef max
#       undef NOMINMAX
#   endif
#endif

//-------------------------------------------------------------------------
// macro

#ifndef LN_OPERATOR_NEW
#   define LN_OPERATOR_NEW( x ) ::operator new( x )
#endif

#ifndef LN_OPERATOR_DELETE
#   define LN_OPERATOR_DELETE( x ) { ::operator delete( x ); ( x ) = 0; }
#endif

#ifndef SAFE_FREE
#   define SAFE_FREE( p ) { if ( p ) { free( ( p ) ); ( p ) = NULL; } }
#endif

#ifndef SAFE_DELETE
#   define SAFE_DELETE( p ) { if ( p ) { delete ( p ); ( p ) = NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#   define SAFE_DELETE_ARRAY( p ) { if ( p ) { delete[] ( p ); ( p ) = NULL; } }
#endif

#ifndef SAFE_ADDREF
#   define SAFE_ADDREF( p ) { if ( p ) { ( p )->AddRef(); } }
#endif

#ifndef SAFE_RELEASE
#   define SAFE_RELEASE( p ) { if ( p ) { ( p )->Release(); ( p ) = NULL; } }
#endif

#ifndef LN_SAFE_ADDREF
#   define LN_SAFE_ADDREF( p ) { if ( p ) { ( p )->addRef(); } }
#endif

#ifndef LN_SAFE_RELEASE
#   define LN_SAFE_RELEASE( p ) { if ( p ) { ( p )->release(); ( p ) = NULL; } }
#endif

#if defined(LNOTE_WINDOWS)
#   define LNOTE_STDCALL __stdcall 
#elif defined(LNOTE_ANDROID)
#   define LNOTE_STDCALL
#endif

// CLR クラスライブラリ
#ifdef __cplusplus_cli
#   define LN_CLR_STRUCT( x )  public x struct
#else
#   define LN_CLR_STRUCT( x )  x
#endif

// 関数名マクロ
#if defined(LNOTE_MSVC)
#   define LNOTE_FUNC_MACRO   __FUNCTION__
#else
#   define LNOTE_FUNC_MACRO   __FUNC__
#endif

#define lnToF( x ) ( static_cast< lfloat >( x ) )	// 実数へ
#define lnToI( x ) ( static_cast< int >( x ) )		// 整数へ
#define lnToRev( x ) ( 1.0f / lnToF( x ) )			// 逆数

// 未実装関数
#define LN_PRINT_NOT_IMPL_FUNCTION printf( "%s is not implemented\n", LNOTE_FUNC_MACRO );

#if defined(LNOTE_ANDROID)
#   define lnprintf(...)       __android_log_print( ANDROID_LOG_DEBUG, LNOTE_NAME, __VA_ARGS__ )
#   define lnwprintf(...)      __android_log_print( ANDROID_LOG_DEBUG, LNOTE_NAME, __VA_ARGS__ )
#else
#   define lnprintf            printf
#   define lnwprintf           wprintf
#endif

// _p
#if !defined(LNOTE_DISABLE_SIMPLE_TYPEDEF)
#   ifndef _p
        inline void _ln_printf_( const char* name_, const char* v_ )		{ lnprintf( "[ %s = %s ]\n", name_, v_ ); }
        inline void _ln_printf_( const char* name_, unsigned char v_ )		{ lnprintf( "[ %s = %c ]\n", name_, v_ ); }
        inline void _ln_printf_( const char* name_, short v_ )				{ lnprintf( "[ %s = %d ]\n", name_, v_ ); }
        inline void _ln_printf_( const char* name_, unsigned short v_ )		{ lnprintf( "[ %s = %u ]\n", name_, v_ ); }
        inline void _ln_printf_( const char* name_, int v_ )				{ lnprintf( "[ %s = %d ]\n", name_, v_ ); }
        inline void _ln_printf_( const char* name_, unsigned int v_ )		{ lnprintf( "[ %s = %u ]\n", name_, v_ ); }
        inline void _ln_printf_( const char* name_, long v_ )				{ lnprintf( "[ %s = %ld ]\n", name_, v_ ); }
        inline void _ln_printf_( const char* name_, unsigned long v_ )		{ lnprintf( "[ %s = %lu ]\n", name_, v_ ); }
        inline void _ln_printf_( const char* name_, unsigned long long v_ ) { lnprintf( "[ %s = %llu ]\n", name_, v_ ); }
        inline void _ln_printf_( const char* name_, float v_ )				{ lnprintf( "[ %s = %f ]\n", name_, v_ ); }
        inline void _ln_printf_( const char* name_, double v_ )				{ lnprintf( "[ %s = %lf ]\n", name_, v_ ); }
        inline void _ln_printf_( const char* name_, bool v_ )				{ lnprintf( "[ %s = %s ]\n", name_, ( v_ ) ? "true" : "false" ); }
        inline void _ln_printf_( const char* name_, void* v_ )				{ lnprintf( "[ %s = %p ]\n", name_, v_ ); }
        inline void _ln_printf_( const char* name_, const void* v_ )		{ lnprintf( "[ %s = %p ]\n", name_, v_ ); }
        
        inline void _ln_printf_( const wchar_t* name_, const wchar_t* v_ )	{ lnwprintf( L"[ %s = %s ]\n", name_, v_ ); }
        inline void _ln_printf_( const wchar_t* name_, const wchar_t  v_ )	{ lnwprintf( L"[ %s = %c ]\n", name_, v_ ); }

        // 変数名と値をコンソールに出力するマクロ
#       define _p( _var_ )  _ln_printf_( #_var_, ( _var_ ) )
#       define _pw( _var_ )	_ln_printf_( _ln_to_w_( #_var_ ), ( _var_ ) );
#   endif
#endif // LNOTE_DISABLE_SIMPLE_TYPEDEF


//-------------------------------------------------------------------------
// typedef

namespace LNote
{
    /// NULL ポインタを表す値
    const class lnNullPtr
    {
    public:
        lnNullPtr() {}
	    //template< typename TYPE >
	    //operator TYPE*() const { return 0; }
	    operator void*() { return 0; }

    } lnNull;
    
    const lnNullPtr             nil;

#if defined(LNOTE_GNUC)
    typedef int8_t              lnS8;
    typedef int16_t             lnS16;
    typedef int32_t             lnS32;
    typedef int64_t             lnS64;
    typedef uint8_t             lnU8;
    typedef uint16_t            lnU16;
    typedef uint32_t            lnU32;
    typedef uint64_t            lnU64;
#else
    typedef	signed char			lnS8;
    typedef	signed short		lnS16;
    typedef	signed long			lnS32;
    typedef	signed long long	lnS64;
    typedef unsigned char		lnU8;
    typedef unsigned short		lnU16;
    typedef unsigned long		lnU32;
    typedef unsigned __int64    lnU64;    // typedef unsigned long long	u64;
    typedef unsigned int		lnUint;
#endif

    typedef	unsigned char	    lnByte;
    typedef float               lnFloat;
    typedef int                 LNRESULT; 
    typedef LNRESULT            LNResult;
    typedef int                 LNHandle;
    //typedef int                 lnHandle; 
    #define lnfloat				float	    // 旧バージョン。後で置換

#ifndef LNOTE_DISABLE_SIMPLE_TYPEDEF
    typedef	lnS8                s8;
    typedef	lnS16               s16;
    typedef	lnS32               s32;
    typedef	lnS64               s64;
    typedef lnU8                u8;
    typedef lnU16               u16;
    typedef lnU32               u32;
    typedef lnU64               u64;
    typedef float				f32;
    typedef double				f64;
    typedef unsigned int		uint;
#endif // LNOTE_DISABLE_SIMPLE_TYPEDEF

#ifdef LNOTE_UNICODE
    #define lnChar              wchar_t
    #define lnString            std::wstring
    #define ln_std_tstring      std::wstring
#else
    #define lnChar              char
    #define lnString            std::string
    #define ln_std_tstring      std::string     // 主に内部用
#endif

}

//-------------------------------------------------------------------------
// Declaration

namespace LNote
{
	namespace Core
	{
        class Framework;

		namespace Task
		{
			class Manager;
		}
		namespace System
		{
			class Manager;
		}
		namespace File
		{
			class Manager;
		}
		namespace Input
		{
			class Manager;
		}
		namespace Audio
		{
			class Manager;
		}
		namespace Physics
		{
			class Manager;
		}
		namespace Graphics
		{
			class Manager;
		}
		namespace Effect
		{
			class Manager;
		}
		namespace Scene
		{
			class Manager;
		}
        namespace GUI
        {
            class Manager;
        }
        namespace Game
		{
			class Manager;
		}
	}
}

//-------------------------------------------------------------------------
// Base include

// 必須のみ
#if defined(LNOTE_COMMON_INCLUDE_REQUIRED_FILES_ONRY)
#	include "Base/Foreach.h"
#	include "Base/MemoryManager.h"
#	include "Base/ReferenceObject.h"
#	include "Base/Exception.h"
#else
#	include "Base/Foreach.h"
#	include "Base/MemoryManager.h"
#	include "Base/ReferenceObject.h"
#	include "Base/RefString.h"
#	include "Base/LogFile.h"
#	include "Base/Interface.h"
#	include "Base/Error.h"
#	include "Base/Exception.h"
#endif

//=============================================================================
//                              end of file
//=============================================================================