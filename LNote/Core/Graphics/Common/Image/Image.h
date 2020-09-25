//=============================================================================
//【 Image 】
//-----------------------------------------------------------------------------
///**
//  @file       Image.h
//  @brief      Image
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../File/StreamObject.h"

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
// ■ IImage クラス
//-----------------------------------------------------------------------------
///**
//  @brief		
//
//  @note
//              取得できるイメージデータはすべて32ビット。
//              オーダーは AA BB GG RR
//*/
////=============================================================================
//class IImage
//{
//public:
//
//    /// インスタンスの作成 (swap_height_ = true で上下を反転する)
//    static IImage* createImage( void* data_, lnU32 size_, bool swap_height_ );
//
//public:
//
//    virtual ~IImage() { }
//
//    /// 横幅の取得
//    virtual lnU32 getWidth() const = 0;
//
//    /// 高さの取得
//    virtual lnU32 getHeight() const = 0;
//
//    /// イメージデータの取得
//    virtual void* getImageData() = 0;
//};



class Image
{
public:

    lnU32   Width;
    lnU32   Height;
    lnU32*  ImageData;
    lnU32   ImageDataSize;

public:

    /// Width、Height に値を格納する
    bool getImageInfo( File::IInStream* stream_ );

    /// バイトデータから作成 (自動判定)
    bool createFromBytes( void* data_, lnU32 size_, bool swap_height_ );

    /// png データから作成 (ImageData にバッファが確保されていてサイズが十分であればそこに書き込む)
    bool createPngFromBytes( void* data_, lnU32 size_, bool swap_height_ );

public:

    Image() : Width( 0 ), Height( 0 ), ImageData( NULL ), ImageDataSize( 0 ) {}
    ~Image() { SAFE_DELETE( ImageData ); }
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