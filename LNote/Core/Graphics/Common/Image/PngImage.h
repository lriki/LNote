//=============================================================================
//�y PngImage �z
//-----------------------------------------------------------------------------
///**
//  @file       PngImage.h
//  @brief      PngImage
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "libpng/png.h"
#include "libpng/pnginfo.h"
#include "Image.h"

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
// �� PngImage �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class PngImage
{
public:

    PngImage();

    virtual ~PngImage();

public:

    /// ������
    bool initialize( void* data_, lnU32 size_, bool swap_height_ );

    /// �����̎擾
    virtual lnU32 getWidth() const { return mWidth; }

    /// �����̎擾
    virtual lnU32 getHeight() const { return mHeight; }

    /// �C���[�W�f�[�^�̎擾
    virtual void* getImageData() { return mImageData; }

private:

    /// 32�r�b�g�J���[�ɓ��ꂷ��
    void _convertToColor32();

private:

    /// �ǂݍ��݃R�[���o�b�N�ɓn���f�[�^
    struct PngData
    {
        unsigned char* Data;
        unsigned long  Length;
        unsigned long  Offset;
    };

    /// �ǂݍ��݃R�[���o�b�N
    static void _readPngData( png_structp png_ptr_, png_bytep data_, png_size_t length_ );

private:

    png_structp     mPngStruct;
	png_infop       mPngInfo;
    unsigned char*	mImageData;
	png_uint_32     mWidth;
	png_uint_32     mHeight;
	//int             mBitDepth;		// �摜�̃`�����l���P���̃r�b�g�[�x ( �L���l�� 1, 2, 4, 8, 16 )
	//int             mColorType;
	//int             mInterlaceType;
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