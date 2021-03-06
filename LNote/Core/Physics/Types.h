//=============================================================================
//【 Types 】
//-----------------------------------------------------------------------------
///**
//  @file       Types.h
//  @brief      Types
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

struct LNTriangleMeshBodyInfo
{
	lnU32           VertexCount;    ///< 頂点数
	const LVector3* VertexTable;	///< 頂点データテーブル
	lnU32           IndexCount;	    ///< インデックス数
    lnU32           IndexStride;    ///< インデックスのストライド (2 or 4)
	const void*     IndexTable;	    ///< インデックスデータテーブル
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

namespace Core
{
namespace Physics
{
class IDebugRenderer;
class BodyBase;
class RigidBody;
class Box;
class Plane;
class JointBase;
class TriMesh;


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Physics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================