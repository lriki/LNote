//=============================================================================
//【 GLUtil 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Common/GraphicsUtil.h"
#include "GLUtil.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace OpenGL
{

//=============================================================================
// ■ Util クラス
//=============================================================================

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    LNGLVertexAttribPointerData* Util::createVertexAttribData( const LNVertexElemenst* elements_ )
    {
        // 頂点宣言の数を調べる
		int elem_size = 0;
        while ( 1 )
		{
			if ( elements_[elem_size].Type == LN_DECL_UNKNOWN )
			{
				break;
			}
			++elem_size;
		}

        // 配列作成
		LNGLVertexAttribPointerData* gl_attr = LN_NEW LNGLVertexAttribPointerData[elem_size + 1];

        // レイアウト作成
        lnU32 struct_size = Graphics::Util::getVertexSize( elements_, 0 );
        lnU8 total_size = 0;
        for ( int i = 0; i < elem_size; ++i )
		{
            gl_attr[i].Usage = convertDeclUsage( elements_[i].Usage, elements_[i].UsageIndex );

            convertDeclTypeLNToGL(
                elements_[i].Type, 
                &gl_attr[i].Type,
                &gl_attr[i].Size,
                &gl_attr[i].Normalized );

            gl_attr[i].Stride = struct_size;

            gl_attr[i].ByteOffset = total_size;

            total_size += (lnU8)Graphics::Util::getVertexDeclTypeSize( elements_[i].Type );
        }

        // 終端
        gl_attr[elem_size].Size= 0;
        gl_attr[elem_size].Type = 0;
        gl_attr[elem_size].Normalized= GL_FALSE;
        gl_attr[elem_size].Stride= 0;
        gl_attr[elem_size].ByteOffset= 0;

        return gl_attr;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Util::convertDeclTypeLNToGL( LNDeclType ln_type_, GLenum* gl_type_, GLint* size_, GLboolean* normalized_ )
    {
        static const struct _FormatType
        {
            GLenum      Type;
            GLint       Size;
            GLboolean   Normalize;
        } format_table[LN_DECL_MAX] =
        {
            { 0,                0,    GL_FALSE },   // LN_DECL_UNKNOWN
            { GL_FLOAT,         1,    GL_FALSE },   // LN_DECL_FLOAT1
            { GL_FLOAT,         2,    GL_FALSE },   // LN_DECL_FLOAT2
            { GL_FLOAT,         3,    GL_FALSE },   // LN_DECL_FLOAT3
            { GL_FLOAT,         4,    GL_FALSE },   // LN_DECL_FLOAT4
            { GL_UNSIGNED_BYTE, 4,    GL_FALSE },   // LN_DECL_UBYTE4
            { GL_UNSIGNED_BYTE, 4,    GL_TRUE },   // LN_DECL_COLOR4
            { GL_SHORT,         2,    GL_FALSE },   // LN_DECL_SHORT2
            { GL_SHORT,         4,    GL_FALSE },   // LN_DECL_SHORT4
        };

        // http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
        // GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE

        *gl_type_    = format_table[ln_type_].Type;
        *size_       = format_table[ln_type_].Size;
        *normalized_ = format_table[ln_type_].Normalize;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    LNVertexDeclUsage Util::convertDeclUsage( LNDeclUsage usage_, lnU32 index_ )
    {
        switch ( usage_ )
        {
            default:
            case LN_USAGE_UNKNOWN:
                return LN_VERTEXDECLUSAGE_UNKNOWN;

            case LN_USAGE_POSITION:
                return LN_VERTEXDECLUSAGE_POSITION;

            case LN_USAGE_NORMAL:
                return LN_VERTEXDECLUSAGE_NORMAL;

            case LN_USAGE_COLOR:
                return LN_VERTEXDECLUSAGE_COLOR;

            case LN_USAGE_TEXCOORD:
            {
                switch ( index_ )
                {
                    case 0: 
                    	return LN_VERTEXDECLUSAGE_TEXCOORD0;
                    case 1: 
                    	return LN_VERTEXDECLUSAGE_TEXCOORD1;
                    case 2: 
                    	return LN_VERTEXDECLUSAGE_TEXCOORD2;
                    case 3: 
                    	return LN_VERTEXDECLUSAGE_TEXCOORD3;
                    case 4: 
                    	return LN_VERTEXDECLUSAGE_TEXCOORD4;
                    case 5: 
                    	return LN_VERTEXDECLUSAGE_TEXCOORD5;
                    case 6: 
                    	return LN_VERTEXDECLUSAGE_TEXCOORD6;
                    case 7: 
                    	return LN_VERTEXDECLUSAGE_TEXCOORD7;
                    case 8: 
                    	return LN_VERTEXDECLUSAGE_TEXCOORD8;
                }
                return LN_VERTEXDECLUSAGE_UNKNOWN;
            }

            case LN_USAGE_PSIZE:
                return LN_VERTEXDECLUSAGE_PSIZE15;

            case LN_USAGE_BLENDINDICES:
                return LN_VERTEXDECLUSAGE_BLENDWEIGHT;

            case LN_USAGE_BLENDWEIGHT:
                return LN_VERTEXDECLUSAGE_BLENDINDICES;
        }
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace OpenGL
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================