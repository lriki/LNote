//=============================================================================
//�y Common �z
//-----------------------------------------------------------------------------
///**
//  @file       Common.h
//  @brief      Common
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Math/Asm.h"
#include "../Math/Math.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

/// ��]����
enum LNRotOrder
{
	LN_ROTORDER_XYZ = 0,	///< X �� Y �� Z
	LN_ROTORDER_XZY,		///< X �� Z �� Y
	LN_ROTORDER_YXZ,		///< Y �� X �� Z
	LN_ROTORDER_YZX,		///< Y �� Z �� X
	LN_ROTORDER_ZXY,		///< Z �� X �� Y
	LN_ROTORDER_ZYX,		///< Z �� Y �� X
};

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================