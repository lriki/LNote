//=============================================================================
//�y VertexTypes �z
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
    LVector3	Position;   ///< ���W
    lnU32       Color;      ///< ���_�J���[
 
    /// ���_���C�A�E�g
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
    LVector3	Position;   ///< ���W
    lnU32       Color;      ///< ���_�J���[
    LVector2	TexUV;      ///< �e�N�X�`�����W
    
    /// ���_���C�A�E�g
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

/// �e�N�X�`�����W�t�����_
struct PosUV
{
    LVector3	Position;   ///< ���W
    LVector2	TexUV;      ///< �e�N�X�`�����W
    
    /// ���_���C�A�E�g
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