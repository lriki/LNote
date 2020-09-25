//=============================================================================
//Åy Asm Åz
//-----------------------------------------------------------------------------
///**
//  @file       Asm.h
//  @brief      Asm
//  @author     Riki
//  @note
//              â∆ÇÃÇ’ÇøÉmÅ[ÉgPCÇæÇ∆ sincos égÇ¡ÇƒëÂëÃ 1.5 Å` 1.8 î{Ç≠ÇÁÇ¢ëÅÇ≠Ç»Ç¡ÇΩÅB
//*/
//=============================================================================

#pragma once

#include <math.h>

namespace LNote
{
namespace Core
{
namespace Asm
{

/// sqrt
inline float sqrt( float r_ )
{
#ifdef LNOTE_ENABLE_INLINE_ASM
	__asm
	{
		fld		r_
		fsqrt
		fstp	r_
	}

	return r_;

    //#error "LNOTE_USE_INLINE_ASM"
#else
    return ::sqrt( r_ );

    //#error "LNOTE_USE_INLINE_ASM_non"
#endif
}

/// sincos
inline void sincos( float r_, float* sin_, float* cos_ )
{
#ifdef LNOTE_ENABLE_INLINE_ASM
	float ts, tc;

	__asm
	{
		fld		r_
		fsincos
		fstp	tc
		fstp	ts
	}

	*sin_ = ts;
	*cos_ = tc;
#else
	*sin_ = sin( r_ );
	*cos_ = cos( r_ );
#endif
}

} // namespace Asm
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================