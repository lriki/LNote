//=============================================================================
//【 PngImage 】
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
// ■ PngImage クラス
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

    /// 初期化
    bool initialize( void* data_, lnU32 size_, bool swap_height_ );

    /// 横幅の取得
    virtual lnU32 getWidth() const { return mWidth; }

    /// 高さの取得
    virtual lnU32 getHeight() const { return mHeight; }

    /// イメージデータの取得
    virtual void* getImageData() { return mImageData; }

private:

    /// 32ビットカラーに統一する
    void _convertToColor32();

private:

    /// 読み込みコールバックに渡すデータ
    struct PngData
    {
        unsigned char* Data;
        unsigned long  Length;
        unsigned long  Offset;
    };

    /// 読み込みコールバック
    static void _readPngData( png_structp png_ptr_, png_bytep data_, png_size_t length_ );

private:

    png_structp     mPngStruct;
	png_infop       mPngInfo;
    unsigned char*	mImageData;
	png_uint_32     mWidth;
	png_uint_32     mHeight;
	//int             mBitDepth;		// 画像のチャンネル１個分のビット深度 ( 有効値は 1, 2, 4, 8, 16 )
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