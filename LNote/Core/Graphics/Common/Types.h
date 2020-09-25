//=============================================================================
//�y Types �z
//-----------------------------------------------------------------------------
///**
//  @file       Types.h
//  @brief      Types
//  @author     Riki
//  @note
//              Interface �ɍ����Ă��������� RenderState ��ʂ̃w�b�_��
//              �������̂ŁAInclude �̖����������邽�߂Ɍ^��`���������Ɉړ��B
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <string>
#include <vector>

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
typedef std::vector< lnU32 > LNEffectedBoneIndices;
typedef std::vector< ln_std_tstring > LNFontNameList;

/// �O���t�B�b�N�X API
enum LNGraphicsAPI
{
    LN_GRAPHICSAPI_DIRECTX9 = 0,    ///< DirectX9
    LN_GRAPHICSAPI_DIRECTX11,       ///< DirectX11
    LN_GRAPHICSAPI_OPENGL,          ///< OpenGL
};

/// �e�N�X�`���̎��
enum LNTextureType
{
    LN_TEXTURETYPE_UNKNOWN = 0,     
	LN_TEXTURETYPE_NORMAL,          ///< �e�N�X�`��
    LN_TEXTURETYPE_RENDERTARGET,    ///< �����_�[�^�[�Q�b�g
    LN_TEXTURETYPE_DEPTH,           ///< �[�x�o�b�t�@

	LN_TEXTURETYPE_INVALID = 99,
};

/// �T�[�t�F�C�X�t�H�[�}�b�g
enum LNSurfaceFormat
{
    LN_FMT_UNKNOWN = 0,
    LN_FMT_A8R8G8B8,            ///< 32 �r�b�g�̃A���t�@�t���t�H�[�}�b�g
    LN_FMT_X8R8G8B8,            ///< 32 �r�b�g�̃A���t�@�����t�H�[�}�b�g
    LN_FMT_A16B16G16R16F,       ///< 64 �r�b�g�̕��������_�t�H�[�}�b�g
    LN_FMT_A32B32G32R32F,       ///< 128 �r�b�g�̕��������_�t�H�[�}�b�g
    LN_FMT_D24S8,               ///< 32 �r�b�g�� Z �o�b�t�@�t�H�[�}�b�g
};

/// �����o�̃f�[�^�̃T�C�Y���`����f�[�^�^
enum LNDeclType
{
	LN_DECL_UNKNOWN = 0,
	LN_DECL_FLOAT1,
	LN_DECL_FLOAT2,
	LN_DECL_FLOAT3,
	LN_DECL_FLOAT4,
	LN_DECL_UBYTE4, 
	LN_DECL_COLOR4,     ///< 32�r�b�g�F�R�[�h (A8R8G8B8 �̏��BLN_DECL_UBYTE4 �̏ꍇ�A�G���f�B�A���͍l������Ȃ��̂Œ���)
	LN_DECL_SHORT2,
	LN_DECL_SHORT4,

    LN_DECL_MAX
};

/// �f�[�^�̎g�p���@
enum LNDeclUsage
{
	LN_USAGE_UNKNOWN = 0,
	LN_USAGE_POSITION,
	LN_USAGE_NORMAL,
	LN_USAGE_COLOR,
	LN_USAGE_TEXCOORD,
	LN_USAGE_PSIZE,
    LN_USAGE_BLENDINDICES,
    LN_USAGE_BLENDWEIGHT,
};

#define LN_DECL_END() { 0, LN_DECL_UNKNOWN, LN_USAGE_UNKNOWN, 0 }

/// �O���t�B�b�N�X�p�C�v���C���ւ̓��͒��_�f�[�^�������\����
struct LNVertexElemenst
{
    u32             StreamIndex;    ///< �X�g���[���ԍ�
	LNDeclType	    Type;
	LNDeclUsage	    Usage;
	u8			    UsageIndex;     ///< OpenGL �ŋ����Ă���̂� TEXCOORD0�`8�APSIZE15 �̂݁B����ȊO�� 0
};

/// �V�F�[�_�ϐ��̃I�u�W�F�N�g�^�C�v
enum LNShaderVariableClass
{
    LN_SVC_UNKNOWN = 0,
    LN_SVC_SCALAR,      ///< [GL] 
    LN_SVC_VECTOR,      ///< [GL] 
    LN_SVC_MATRIX,      ///< [GL] 
    LN_SVC_STRUCT,
    LN_SVC_ARRAY,       ///< TODO: DX9 �ł͂ǂ��F������̂��`�F�b�N���Ă����B���͖��g�p
    LN_SVC_SAMPLER,     ///< [GL] 
    LN_SVC_OBJECT,      ///< DX9�̃e�N�X�`���A�T���v���^�͂���
    LN_SVC_BUFFER
};

/// �V�F�[�_�ϐ��̌^
enum LNShaderVariableType
{
    LN_SVT_UNKNOWN = 0,         ///<  [GL] 
    LN_SVT_ARRAY,
    LN_SVT_STRING,              ///< [DX9] [GL ���A�m�e�[�V�����̂�]
    LN_SVT_STRUCT,
    LN_SVT_TYPELESS_STRUCT,
    LN_SVT_BUFFER,
    LN_SVT_UNIFORMBUFFER,
    LN_SVT_ADDRESS,
    LN_SVT_PROGRAM_TYPE,
    LN_SVT_PIXELSHADER,         ///< [DX9]
    LN_SVT_VERTEXSHADER,        ///< [DX9]
    LN_SVT_TEXTURE,             ///< [DX9]
    LN_SVT_TEXTURE1D,           ///< [DX9]
    LN_SVT_TEXTURE2D,           ///< [DX9]
    LN_SVT_TEXTURE3D,           ///< [DX9]
    LN_SVT_TEXTURECUBE,         ///< [DX9]
    LN_SVT_SAMPLER,             ///< [DX9] 
    LN_SVT_SAMPLER1D,           ///< [DX9] [GL] DX9 �ɍ��킹�Ēǉ�
    LN_SVT_SAMPLER2D,           ///< [DX9] [GL] 
    LN_SVT_SAMPLER3D,           ///< [DX9] [GL] 
    LN_SVT_SAMPLERCUBE,         ///< [DX9] [GL] 
    LN_SVT_ENUM,
    LN_SVT_VOID,                ///< [DX9] Cg �ɂ͂Ȃ������H
    LN_SVT_BOOL,                ///< [DX9] [GL]
    LN_SVT_CHAR,
    LN_SVT_DOUBLE,
    LN_SVT_FIXED,
    LN_SVT_FLOAT,               ///< [DX9] [GL]
    LN_SVT_HALF,
    LN_SVT_INT,                 ///< [DX9] [GL]
    LN_SVT_LONG,
    LN_SVT_SHORT,
    LN_SVT_UCHAR,
    LN_SVT_UINT,
    LN_SVT_ULONG,
    LN_SVT_USHORT,

    LN_SVT_TEXTURE_BEGIN = LN_SVT_TEXTURE,
    LN_SVT_TEXTURE_END   = LN_SVT_TEXTURECUBE,


    // Texture �n�� Cg �ł͔F�����Ȃ������������B
    // �Ƃ肠���� DirectX9 �ɍ��킹�Ă����B
    // �ύX�O�K�v�ȏꍇ�� Shader_dx.cpp ��
    // ShaderVariable �R���X�g���N�^�������銴���ŁB
    //

    // D3DXPT_PIXELFRAGMENT�AD3DXPT_VERTEXFRAGMENT �̓J�b�g

    /*
    LN_SVT_VOID,      ///< void �|�C���^
    LN_SVT_BOOL,
    LN_SVT_INT,
    LN_SVT_FLOAT,
    LN_SVT_STRING,
    LN_SVT_TEXTURE,
    LN_SVT_TEXTURE1D,
    LN_SVT_TEXTURE2D,
    LN_SVT_TEXTURE3D,
    LN_SVT_TEXTURECUBE,
    */


    /*
    LN_SVT_SAMPLER,
    LN_SVT_SAMPLER1D,
    LN_SVT_SAMPLER2D,
    LN_SVT_SAMPLER3D,
    LN_SVT_SAMPLERCUBE,
    LN_SVT_PIXELSHADER,
    LN_SVT_VERTEXSHADER,
    LN_SVT_PIXELFRAGMENT,
    LN_SVT_VERTEXFRAGMENT,
    LN_SVT_UNSUPPORTED,
    */

};

/// �V�F�[�_�v���O�����̃R���p�C������
enum LNShaderCompileResult
{
    LN_SHADERRESULT_SUCCESS = 0,   ///< ����
    LN_SHADERRESULT_ERROR,         ///< �G���[���������Ă���
    LN_SHADERRESULT_WARNING        ///< �x�����������Ă��� (���s�͉\)
};

/// �V�F�[�_�v���O�����̕ϐ��̌^��� (DX9 �ł͌^�̖��O�͂Ƃ�Ȃ��݂���)
struct LNShaderVariableTypeDesc
{
    LNShaderVariableClass   Class;
    LNShaderVariableType    Type;
    lnU32                   Rows;       ///< �s�̐� (�s��^�ȊO�� 1)
    lnU32                   Columns;    ///< ��̐�
    lnU32                   Elements;   ///< �z��̗v�f�� (�z��łȂ��ꍇ�� 0) TODO:OpenGL�ł�1�c�����K�v
    bool                    Shared;     ///< shared �L�[���[�h�����Ă���
};

/// �V�F�[�_�v���O�����̏��
struct LNShaderDesc
{
    lnU32           ParameterNum;       ///< �p�����[�^�̐�
    lnU32           TechniqueNum;       ///< �e�N�j�b�N�̐�
};


/// �e�L�X�g�A���C���̋L�q
enum LNTextAlign
{
    LN_TEXTALIGN_LEFT = 0, ///< ������
    LN_TEXTALIGN_CENTER,   ///< ��������
    LN_TEXTALIGN_RIGHT     ///< �E����
};

/// �t�H���g�̃O���t��\���\����
struct LNGlyphOutlineData
{
    lnU8*   Bitmap;         ///< 1�������̃r�b�g�}�b�v���i�[�����o�b�t�@
    lnU32   BitmapWidth;    ///< Bitmap �̕� (���ƍ�����1���������҂��������T�C�Y)
    lnU32   BitmapHeight;   ///< Bitmap �̍���

    lnU32   GlyphOffsetX;   ///< �`��̈�̍��ォ��A�r�b�g�}�b�v��]������ׂ����W�ւ�X�I�t�Z�b�g
    lnU32   GlyphOffsetY;   ///< �`��̈�̍��ォ��A�r�b�g�}�b�v��]������ׂ����W�ւ�Y�I�t�Z�b�g
    lnU32   GlyphWidth;     ///< Bitmap ���̕��������镔���̕�
    lnU32   GlyphHeight;    ///< Bitmap ���̕��������镔���̍���
    lnU32   CellWidth;      ///< �����̈�̕� (���̕�����`�悷��Ƃ��͂��̒l�������s�N�Z����i�߂�悤�ɂ���)

    lnU32   CharIdxOffset;  ///< IFont::getGlyphOutline() �ɂ���ĉ�͂����������}���`�o�C�g�����������ꍇ�� 2�A�V���O���o�C�g�܂��� Unicode(wchar_t)�̏ꍇ�� 1
    bool    BitmapIsARGB;   ///< true �̏ꍇ�� Bitmap �� u32* �ɃL���X�g���Ďg������ (true �̏ꍇ�ABitmap �� u8 �ł͂Ȃ� u32 �Ƃ��Ċm�ۂ���Ă���)
};

/// �v���~�e�B�u
enum LNPrimitiveType
{
	LN_PRIM_TRIANGLELIST = 1,
	LN_PRIM_TRIANGLESTRIP,
	LN_PRIM_LINELIST,
    LN_PRIM_LINESTRIP,
	LN_PRIM_POINTLIST,
};

/// �������@
enum LNBlendMode
{
    LN_BLEND_NORMAL = 0,        ///< �ʏ� ( �A���t�@���� )
    LN_BLEND_ALPHA,             ///< �A���t�@�u�����h
    LN_BLEND_ADD,               ///< ���Z
    LN_BLEND_ADD_ALPHADISABLE,	///< ���Z�i�A���t�@�`�����l�������j
    LN_BLEND_SUB,				///< ���Z
    LN_BLEND_SUB_ALPHADISABLE,	///< ���Z�i�A���t�@�`�����l�������j
    LN_BLEND_MUL,				///< ��Z
    LN_BLEND_SCREEN,			///< �X�N���[��
    LN_BLEND_REVERSE,           ///< ���o�[�X
};

/// �J�����O���@
enum LNCullingMode
{
    LN_CULL_FRONT = 0,          ///< �O�ʕ\��  (D3DCULL_CCW)
	LN_CULL_BACK,               ///< ���ʕ\��
	LN_CULL_DOUBLE              ///< ���ʕ\��
};

/// �J�����O���@
/* ��E���W�n�X�^�C�� (�E��n�̏ꍇ��D3DCULL_CW���f�t�H)
enum LNCullingMode
{
    LN_CULL_BACK = 0,           ///< ���ʕ\��
    LN_CULL_FRONT,              ///< �O�ʕ\��
	LN_CULL_DOUBLE              ///< ���ʕ\��
};
*/

/// �h��Ԃ����@
enum LNFillMode
{
	LN_FILL_SOLID = 0,	    ///< �ʂ�h��Ԃ�
	LN_FILL_WIRE,	        ///< ���C���[�t���[��
	LN_FILL_POINT,	        ///< ���_
};

/// �O���t�B�b�N�X�p�C�v���C���ւ̓��͒��_�f�[�^�������\����
struct LNGraphicsCache
{
    u32             StreamIndex;    ///< �X�g���[���ԍ�
	LNDeclType	    Type;
	LNDeclUsage	    Usage;
	u8			    UsageIndex;
};

/// �e�N�X�`���t�B���^
enum LNTextureFilter
{
    LN_TEXFILTER_NONE = 0,
    LN_TEXFILTER_POINT,
    LN_TEXFILTER_LINEAR,
};

/// �e�N�X�`���A�h���b�V���O
enum LNTextureAddress
{
    LN_TEXFILTER_REPEAT = 0,    ///< �J��Ԃ�
    LN_TEXFILTER_CLAMP,         ///< ���E�F���������΂����
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

/// 3D ��Ԃł̊������\���l
enum LNAxisAlignedDir
{
    LN_AADIR_X = 0,   ///< X+ ���� (�E����)
	LN_AADIR_Y,       ///< Y+ ���� (�����)
	LN_AADIR_Z,       ///< Z+ ���� (������)
    LN_AADIR_RX,      ///< X- ���� (������)
	LN_AADIR_RY,      ///< Y- ���� (������)
	LN_AADIR_RZ,      ///< Z- ���� (��O����)
};

/// �X�v���C�g�̃\�[�g���@
enum LNSpriteSortMode
{
    LN_SPRITESORT_NONE                = 0x00,   ///< �\�[�g���Ȃ�
    LN_SPRITESORT_DEPTH_BACK_TO_FRONT = 0x01,   ///< Z�l���傫�����̂���ɕ`�悳���悤�Ƀ\�[�g���� (�A���t�@�u�����h�g�p��)
    LN_SPRITESORT_DEPTH_FRONT_TO_BACK = 0x02,   ///< Z�l�����������̂���ɕ`�悳���悤�Ƀ\�[�g���� (LN_SPRITESORT_DEPTH_BACK_TO_FRONT �Ɠ����ɂ͎g���Ȃ�)
    LN_SPRITESORT_TEXTURE             = 0x04    ///< �e�N�X�`����D�悵�ă\�[�g���� (�����e�N�X�`�������X�v���C�g����������Ƃ��p�B�������A�A���t�@�u�����h���L���ȏꍇ�͕`�悪�s���R�ɂȂ邱�Ƃ�����)
};

/// �r���{�[�h�̌v�Z���@
enum LNBillboardType
{
    LN_BILLBOARD_NOT = 0,   ///< �r���{�[�h�̌v�Z���s��Ȃ�
    LN_BILLBOARD_FRONT,     ///< �J���� (�r���[�s��) �ɑ΂��Đ��ʂ�����
    LN_BILLBOARD_ROT_Y,     ///< Y ����]�̂ݍs��
};

/// ���b�V���̑���
struct LNMeshAttribute
{
    u32     MaterialIndex;  ///< �Ή�����}�e���A���ԍ�
    u32     StartIndex;     ///< �J�n�C���f�b�N�X
    u32     PrimitiveNum;   ///< �`��v���~�e�B�u��

    LNEffectedBoneIndices   BoneIndices;    ///< (����Ȃ�����)GPU �X�L�j���O�ɂ����āA���̑����̃T�u�Z�b�g���e�����󂯂�{�[���ԍ��̔z��
};

/// �O���b�h��\�����邩�ǂ����̎w��
enum LNGridVisible
{
    LN_GRID_XZ      = 0x01,     ///< XZ ���ʂɕ`�悷��
    LN_GRID_XY      = 0x02,     ///< XY ���ʂɕ`�悷��
    LN_GRID_YZ      = 0x04,     ///< YZ ���ʂɕ`�悷��
    LN_GRID_AXIS    = 0x08      ///< ����`�悷��
};

/// �O���b�h�̕`��͈�
enum LNGridPlaceRange
{
    LN_GRIDPLACE_PLUS = 0,      ///< +�����ɂ����`�悷��
    LN_GRIDPLACE_MINUS,         ///< -�����ɂ����`�悷��
    LN_GRIDPLACE_PLUS_MINUS     ///< +-�����ɕ`�悷��
};

/// ���f�������̎w��
enum LNModelFlag
{
    LN_SKINNINGTYPE_SOFTWARE = 1,   ///< �\�t�g�E�F�A�X�L�j���O�p�̃��f�� (���I�Ȓ��_�A�C���f�b�N�X�o�b�t�@���쐬)
    //LN_SKINNINGTYPE_HARDWARE,       ///< ���_�e�N�X�`�����g�� GPU �X�L�j���O�p�̃��f�� (�ÓI�Ȓ��_�A�C���f�b�N�X�o�b�t�@���쐬)
    //LN_SKINNINGTYPE_MATRIX_PALETTE    ///< �s��p���b�g (DirectX �̃T���v���̂���)

    LN_MODELFLAG_COLLISION = 0x02,			///< �������Z�̃��b�V���{�f�B�Ƃ��Ĉ������b�V���𐶐�����
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================