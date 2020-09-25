//=============================================================================
//【 VertexTypes 】
//-----------------------------------------------------------------------------
///**
//  @file       VertexTypes.h
//  @brief      VertexTypes
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
namespace Core
{
namespace Graphics
{
namespace Vertex
{

struct PosColor
{
    LVector3	Position;   ///< 座標
    lnU32       Color;      ///< 頂点カラー
 
    /// 頂点レイアウト
    static LNVertexElemenst* elements()
    {
	    static LNVertexElemenst elements[] = {
		    { 0, LN_DECL_FLOAT3, LN_USAGE_POSITION, 0 },
            { 0, LN_DECL_COLOR4, LN_USAGE_COLOR,    0 },
		      LN_DECL_END() };
	    return elements;
    }
};

struct PosColorUV
{
    LVector3	Position;   ///< 座標
    lnU32       Color;      ///< 頂点カラー
    LVector2	TexUV;      ///< テクスチャ座標
    
    /// 頂点レイアウト
    static LNVertexElemenst* elements()
    {
        static LNVertexElemenst elements[] = {
	        { 0, LN_DECL_FLOAT3, LN_USAGE_POSITION, 0 },
            { 0, LN_DECL_COLOR4, LN_USAGE_COLOR,    0 },
            { 0, LN_DECL_FLOAT2, LN_USAGE_TEXCOORD, 0 },
	          LN_DECL_END() };
        return elements;
    }
};

/// テクスチャ座標付き頂点
struct PosUV
{
    LVector3	Position;   ///< 座標
    LVector2	TexUV;      ///< テクスチャ座標
    
    /// 頂点レイアウト
    static LNVertexElemenst* elements()
    {
	    static LNVertexElemenst elements[] = {
		    { 0, LN_DECL_FLOAT3, LN_USAGE_POSITION, 0 },
            { 0, LN_DECL_FLOAT2, LN_USAGE_TEXCOORD, 0 },
		      LN_DECL_END() };
	    return elements;
    }
};


} // namespace Vertex
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================