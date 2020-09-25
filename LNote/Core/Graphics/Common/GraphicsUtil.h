//=============================================================================
//�y GraphcisUtil �z
//-----------------------------------------------------------------------------
///**
//  @file       GraphcisUtil.h
//  @brief      GraphcisUtil
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

//=============================================================================
// �� Util �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �O���t�B�b�N�֌W�̃��[�e�B���e�B
//
//  @note
//              �z���g�ɐ������A���t�@�u�����h�̌v�Z�́A�Q�[���Ŏg���ɂ͂������
//              ���Ԃ������肷����B���̂��߁A�����ł͓]�����̃A���t�@�l�D���
//              �u�����h���s���B(dest 64�Asrc 128 �̏ꍇ�Adest 64�Asrc 191 �Ƃ��Čv�Z����)
//*/
//=============================================================================
class Util
{
public:

    

    /// ���_�t�H�[�}�b�g���璸�_�̃T�C�Y�����߂� (element_.StreamIndex == stream_index_ �̂݃J�E���g)
    static lnU32 getVertexSize( const LNVertexElemenst* element_, lnU32 stream_index_ );

    /// ���_�����ЂƂ��̃o�C�g���̎擾
    static lnU32 getVertexDeclTypeSize( LNDeclType type_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �r�b�g�}�b�v�̓]�����s��
    //
    //  @param[in]  x_      : �]����̍��� X ���W
    //  @param[in]  y_      : �]����̍��� Y ���W
    //  @param[in]  dest_   : �]����r�b�g�}�b�v
    //  @param[in]  dest_w_ : �]����r�b�g�}�b�v�S�̂̕�
    //  @param[in]  dest_h_ : �]����r�b�g�}�b�v�S�̂̍���
    //  @param[in]  sx_     : �]�����̈�̍��� X ���W
    //  @param[in]  sy_     : �]�����̈�̍��� Y ���W
    //  @param[in]  sw_     : �]�����̈�̕�
    //  @param[in]  sh_     : �]�����̈�̍���
    //  @param[in]  src_    : �]�����r�b�g�}�b�v
    //  @param[in]  src_w_  : �]�����r�b�g�}�b�v�S�̂̕�
    //  @param[in]  src_h_  : �]�����r�b�g�}�b�v�S�̂̍���
    //  @param[in]  alpha_  : �A���t�@�u�����h�̗L���t���O
    //*/
    //---------------------------------------------------------------------
    static void bitblt(
        int x_,  int y_ , u32* dest_, int dest_w_, int dest_h_,
	    int sx_, int sy_, int sw_, int sh_, u32* src_ , int src_w_ , int src_h_,
        bool alpha_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �r�b�g�}�b�v�̓]�����s�� (�E���N���b�s���O�t��)
    //
    //  @param[in]  x_      : �]����̍��� X ���W
    //  @param[in]  y_      : �]����̍��� Y ���W
    //  @param[in]  r_      : �]����̈�̉E�[
    //  @param[in]  b_      : �]����̈�̉��[
    //  @param[in]  dest_   : �]����r�b�g�}�b�v
    //  @param[in]  dest_w_ : �]����r�b�g�}�b�v�S�̂̕�
    //  @param[in]  dest_h_ : �]����r�b�g�}�b�v�S�̂̍���
    //  @param[in]  sx_     : �]�����̈�̍��� X ���W
    //  @param[in]  sy_     : �]�����̈�̍��� Y ���W
    //  @param[in]  sw_     : �]�����̈�̕�
    //  @param[in]  sh_     : �]�����̈�̍���
    //  @param[in]  src_    : �]�����r�b�g�}�b�v
    //  @param[in]  src_w_  : �]�����r�b�g�}�b�v�S�̂̕�
    //  @param[in]  src_h_  : �]�����r�b�g�}�b�v�S�̂̍���
    //  @param[in]  alpha_  : �A���t�@�u�����h�̗L���t���O
    //*/
    //---------------------------------------------------------------------
    static void bitblt(
        int x_,  int y_ , int  r_, int  b_, u32* dest_, int dest_w_, int dest_h_,
	    int sx_, int sy_, int sw_, int sh_, u32* src_ , int src_w_ , int src_h_,
        bool alpha_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �r�b�g�}�b�v�̓]�����s�� (�]�����r�b�g�}�b�v�� u8 �̃A���t�@�}�X�N�Ƃ���)
    //
    //  @param[in]  x_       : �]����̍��� X ���W
    //  @param[in]  y_       : �]����̍��� Y ���W
    //  @param[in]  dest_    : �]����r�b�g�}�b�v
    //  @param[in]  dest_w_  : �]����r�b�g�}�b�v�S�̂̕�
    //  @param[in]  dest_h_  : �]����r�b�g�}�b�v�S�̂̍���
    //  @param[in]  sx_      : �]�����̈�̍��� X ���W
    //  @param[in]  sy_      : �]�����̈�̍��� Y ���W
    //  @param[in]  sw_      : �]�����̈�̕�
    //  @param[in]  sh_      : �]�����̈�̍���
    //  @param[in]  src_     : �]�����r�b�g�}�b�v (u8 �ł���_�ɒ��ӁB����� a_level_ �� 100% �Ƃ����s�����x)
    //  @param[in]  a_level_ : �]�����r�b�g�}�b�v�̃A���t�@���x��
    //  @param[in]  color_   : �]������F (ARGB �Ŏw�肷��B�A���t�@�l�� src_ �̒l�ƃu�����h�����)
    //  @param[in]  src_w_   : �]�����r�b�g�}�b�v�S�̂̕�
    //  @param[in]  src_h_   : �]�����r�b�g�}�b�v�S�̂̍���
    //  @param[in]  alpha_   : �A���t�@�u�����h�̗L���t���O
    //
    //  @par
    //              �A���t�@�l�͓]����Ɠ]�����̂��̂����Z���܂��B
    //
    //  @note
    //              �Ƃ肠�����t�H���g�����̕`��̂��߂ɗp�ӂ������́B
    //*/
    //---------------------------------------------------------------------
    static void bitbltAlphaByte(
        int x_,  int y_ , u32* dest_, int dest_w_, int dest_h_,
	    int sx_, int sy_, int sw_, int sh_, u8* src_, u8 a_level_, u32 color_, int src_w_ , int src_h_,
        bool alpha_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �r�b�g�}�b�v�̓]�����s�� (bitbltAlphaByte() �̉E���N���b�s���O�t��)
    //
    //  @param[in]  x_       : �]����̍��� X ���W
    //  @param[in]  y_       : �]����̍��� Y ���W
    //  @param[in]  r_       : �]����̈�̉E�[
    //  @param[in]  b_       : �]����̈�̉��[
    //  @param[in]  dest_    : �]����r�b�g�}�b�v
    //  @param[in]  dest_w_  : �]����r�b�g�}�b�v�S�̂̕�
    //  @param[in]  dest_h_  : �]����r�b�g�}�b�v�S�̂̍���
    //  @param[in]  sx_      : �]�����̈�̍��� X ���W
    //  @param[in]  sy_      : �]�����̈�̍��� Y ���W
    //  @param[in]  sw_      : �]�����̈�̕�
    //  @param[in]  sh_      : �]�����̈�̍���
    //  @param[in]  src_     : �]�����r�b�g�}�b�v (u8 �ł���_�ɒ��ӁB����� a_level_ �� 100% �Ƃ����s�����x)
    //  @param[in]  a_level_ : �]�����r�b�g�}�b�v�̃A���t�@���x��
    //  @param[in]  color_   : �]������F (ARGB �Ŏw�肷��B�A���t�@�l�� src_ �̒l�ƃu�����h�����)
    //  @param[in]  src_w_   : �]�����r�b�g�}�b�v�S�̂̕�
    //  @param[in]  src_h_   : �]�����r�b�g�}�b�v�S�̂̍���
    //  @param[in]  alpha_   : �A���t�@�u�����h�̗L���t���O
    //
    //  @par
    //              �]����̈�̉E��(r_, b_)�𒴂���ꍇ�A���̕�����]�����܂���B
    //*/
    //---------------------------------------------------------------------
    static void bitbltAlphaByte(
        int x_,  int y_ , int  r_, int  b_, u32* dest_, int dest_w_, int dest_h_,
	    int sx_, int sy_, int sw_, int sh_, u8* src_, u8 a_level_, u32 color_, int src_w_ , int src_h_,
        bool alpha_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �e�N�X�`���t�H�[�}�b�g������� LNSurfaceFormat �ɕϊ�����
    //
    //  @note
    //              MME �V�F�[�_�p�B
    //              A8R8G8B8"�A"FMT_A8R8G8B8"�A"D3DFMT_A8R8G8B8"�̂�����̏����ł��w��ł���B
    //              LN_FMT_UNKNOWN �ȊO�̒l�ɋ����I�ɕϊ�����B
    //*/
    //---------------------------------------------------------------------
    static LNSurfaceFormat convertFMTStringToLNFMT( const lnChar* fmt_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �e�N�X�`���̍쐬
    //
    //  @par
    //              �t�H�[�}�b�g�� A8R8G8B8�A�~�b�v�}�b�v�͍쐬���܂���B
    //*/
    //---------------------------------------------------------------------
    static LNRESULT createTexture( ITexture** texture_, IGraphicsDevice* device_, u32 width_, u32 height_ );

};

//=============================================================================
// �� LNFXShaderSource
//-----------------------------------------------------------------------------
///**
//  @brief      .lnfx �̕����N���X
//
//  @note
//              �e�^�O�� #ifdef �` #endif �ɒu������B
//              �e�V�F�[�_�R���p�C�����ɁA�ȉ��̃}�N�����`���邱�ƁB
//                  LN_HLSL 1
//                  LN_GLSL_VERT 1
//                  LN_GLSL_FLAG 1
//                  (LN_GLSL_META 1)
//*/
//=============================================================================
class LNFXShaderSource
{
public:

    static const char* HLSL_MACRO;
    static const char* GLSL_VERT_MACRO;
    static const char* GLSL_FLAG_MACRO;
    static const char* GLSL_META_MACRO;

	enum ShaderSystem
	{
		SHADER_HLSL = 0,
		SHADER_GLSL,
	};

public:

	LNFXShaderSource()
	{}

public:

	void initialize(const char* buf_, ShaderSystem system_);

    const char* getSourceData() const { return mSource.c_str(); }
    int         getSourceDataSize() const { return mSource.size(); }

    const char* getGLSLMetaDataSource() const { return mGLSLMetaDataSource.c_str(); }
    int         getGLSLMetaDataSourceSize() const { return mGLSLMetaDataSource.size(); }

private:

    void _splitMetaData();

private:

	std::string mSource;
	std::string mGLSLMetaDataSource;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================