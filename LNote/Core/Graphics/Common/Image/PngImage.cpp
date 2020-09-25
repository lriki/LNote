//=============================================================================
//【 PngImage 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "PngImage.h"

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
// ■ PngImage
//=============================================================================

    #define PNG_BYTES_TO_CHECK ( 4 )	// png 識別用の、ファイルの先頭バイト数
    
    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    PngImage::PngImage()
        : mPngStruct        ( NULL )
        , mPngInfo          ( NULL )
        , mImageData        ( NULL )
        , mWidth            ( 0 )
        , mHeight           ( 0 )
        //, mBitDepth         ( 0 )
        //, mColorType        ( 0 )
        //, mInterlaceType    ( 0 )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    PngImage::~PngImage()
    {
        SAFE_DELETE_ARRAY( mImageData );

        if ( mPngStruct )
        {
            png_destroy_read_struct( &mPngStruct, &mPngInfo, NULL );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    bool PngImage::initialize( void* data_, lnU32 size_, bool swap_height_ )
    {
		png_byte	sig[PNG_BYTES_TO_CHECK];
		size_t		size;
		int			res;

        // サイズチェック
        if ( size_ < PNG_BYTES_TO_CHECK ) return false;

		// データが png かどうか調べる
		res = png_check_sig( (png_byte*)data_, PNG_BYTES_TO_CHECK ); 
		if ( !res ) return false;

		// png_struct構造体を確保・初期化する
		mPngStruct = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
		if ( !mPngStruct ) return false;

		// png_info構造体を確保・初期化する
		mPngInfo = png_create_info_struct( mPngStruct );
		if ( ! mPngInfo ) return false;

        if (setjmp(png_jmpbuf(mPngStruct))) return false;

        // png データ、読み込みコールバック設定
        PngData png_data;
        png_data.Data   = (unsigned char*)data_;
        png_data.Offset = 0;
        png_data.Length = size_;
        png_set_read_fn( mPngStruct, (void *)&png_data, _readPngData );

		// シグネチャの確認で読み飛ばしたバイト数を知らせる
		//png_set_sig_bytes( mPngStruct, PNG_BYTES_TO_CHECK );

        // PNGファイルのヘッダ情報を読み込む
        png_read_png( mPngStruct, mPngInfo, PNG_TRANSFORM_EXPAND, NULL );
		//png_read_info( mPngStruct, mPngInfo );

		// IHDRチャンク情報を取得する
		//png_get_IHDR(
  //          mPngStruct, mPngInfo, &mWidth, &mHeight,
		//	&mBitDepth, &mColorType, &mInterlaceType, NULL, NULL);

        mWidth  = mPngInfo->width;
        mHeight = mPngInfo->height;

        

        // 必ず1色 8 ビットで
        if ( mPngInfo->bit_depth != 8 ) return false;

        // パレットモードは非対応
        if ( mPngInfo->color_type & PNG_COLOR_MASK_PALETTE ) return false;

        //unsigned int row_bytes = png_get_rowbytes( mPngStruct, mPngInfo );
        //mImageData = (unsigned char*) malloc( row_bytes * mHeight );

        //png_bytepp row_pointers = png_get_rows( mPngStruct, mPngInfo );

        //for (int i = 0; i < mHeight; i++) {
        //    memcpy(mImageData+(row_bytes * (i)), row_pointers[i], row_bytes);
        //}

        //printf("%x\n", *((u32*)mImageData));
        //printf("%x\n", *(((u32*)mImageData) + 1));


        //-----------------------------------------------------
        // ビットマップ格納

        // 横幅のバイト数 (例えば ABGR の時は 横幅 * 4)
        unsigned int row_bytes = png_get_rowbytes( mPngStruct, mPngInfo );
        
        png_bytepp row_pointers = png_get_rows( mPngStruct, mPngInfo );

        mImageData = LN_NEW lnByte[mHeight * mWidth * 4];

        lnU32 sign = ( swap_height_ ) ? -1 : 1;
        lnU32 unit = ( swap_height_ ) ? mHeight - 1 : 0;

        // ABGR (ピクセルフォーマットは AA BB GG RR)
        if ( mPngInfo->color_type == PNG_COLOR_TYPE_RGB_ALPHA && mPngInfo->pixel_depth == 32 )
        {
            for ( lnU32 h = 0; h < mHeight; ++h )
                memcpy( mImageData + (row_bytes * (unit + (sign * h))), row_pointers[h], row_bytes );
        }
        // BGR (Byte に分けたピクセルフォーマットは RR GG BB)
        else if ( mPngInfo->color_type == PNG_COLOR_TYPE_RGB && mPngInfo->pixel_depth == 24 )
        {
            lnU32* image = (lnU32*)mImageData;
            lnByte* row;
            lnByte* b;
            for ( lnU32 y = 0; y < mHeight; ++y )
            {
                row = row_pointers[unit + (sign * y)];
                for ( lnU32 x = 0; x < mWidth; ++x )
                {
                    b = &row[x * 3];
                    image[x + mWidth * y] = 
                        0xff000000           |
                        ( *( b + 0 ) )       |
                        ( *( b + 1 ) << 8 )  |
                        ( *( b + 2 ) << 16 );
                }
            }
        }
        // Gray
        else if ( mPngInfo->color_type == PNG_COLOR_TYPE_GRAY && mPngInfo->pixel_depth == 8 )
        {
            lnU32* image = (lnU32*)mImageData;
            lnByte* row;
            lnByte c;
            for ( lnU32 y = 0; y < mHeight; ++y )
            {
                row = row_pointers[unit + (sign * y)];
                for ( lnU32 x = 0; x < mWidth; ++x )
                {
                    c = row[x];
                    image[x + mWidth * y] =
                        0xff000000  |
                        ( c )       |
                        ( c << 8 )  |
                        ( c << 16 );
                }
            }
        }
        else
        {
            return false;
        }
        


		//PNG_COLOR_TYPE_GRAY// (ビット深度 1, 2, 4, 8, 16)
		//PNG_COLOR_TYPE_GRAY_ALPHA// (ビット深度 8, 16)
		//PNG_COLOR_TYPE_PALETTE// (ビット深度 1, 2, 4, 8)
		//PNG_COLOR_TYPE_RGB// (ビット深度 8, 16)
		//PNG_COLOR_TYPE_RGB_ALPHA// (ビット深度 8, 16)

		//PNG_COLOR_MASK_PALETTE
		//PNG_COLOR_MASK_COLOR
		//PNG_COLOR_MASK_ALPHA

		//if(png_get_valid(pPng, pInfo, PNG_INFO_tRNS))
		//	png_set_expand(pPng);
		//if(ColorType == PNG_COLOR_TYPE_PALETTE)
		//	png_set_expand(pPng);
		//if(ColorType == PNG_COLOR_TYPE_GRAY && bpp < 8)
		//	png_set_expand(pPng);
		//if(bpp > 8)
		//	png_set_strip_16(pPng);
		//if(ColorType == PNG_COLOR_TYPE_GRAY)
		//	png_set_gray_to_rgb(pPng);
		
		return true;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void PngImage::_convertToColor32()
    {
        

        //if ( mPngInfo->pixel_depth )
        //{
        //}

        /*
        // データ本体読み込み
        unsigned int row_bytes = png_get_rowbytes( mPngStruct, mPngInfo );
        lnByte* src_data = LN_NEW lnByte[mHeight * row_bytes];
        png_read_image( mPngStruct, &mImageData );

        mImageData = LN_NEW lnByte[mHeight * row_bytes];

        lnU32 

        // アルファチャンネルが含まれている場合
		if ( mColorType & PNG_COLOR_MASK_ALPHA )
		{
            
            for ()

			// 一色 4 バイトとして取得
			color = mImage[ y ][ ( 4 * x ) + 3 ];
			color = ( color << 8 ) + mImage[ y ][ ( 4 * x ) ];
			color = ( color << 8 ) + mImage[ y ][ ( 4 * x ) + 1 ];
			color = ( color << 8 ) + mImage[ y ][ ( 4 * x ) + 2 ];
		}

        SAFE_DELETE_ARRAY( src_data );
        */
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void PngImage::_readPngData( png_structp png_ptr_, png_bytep data_, png_size_t length_ )
    {
        PngData* buffer = (PngData*)png_get_io_ptr( png_ptr_ );

        if ( buffer->Offset + length_ <= buffer->Length )
        {
            memcpy( data_, buffer->Data + buffer->Offset, length_ );
            buffer->Offset += length_;
        }
        else
        {
            printf("buffer->Offset:%d length_:%d buffer->Length:%d", buffer->Offset, length_, buffer->Length);
            png_error( png_ptr_, "_readPngData failed" );
        }
    }


//=============================================================================
// ■ Image
//=============================================================================

    /// 読み込みコールバックに渡すデータ
    struct PngData
    {
        unsigned char* Data;
        unsigned long  Length;
        unsigned long  Offset;
    };

    /// 読み込みコールバック
    static void _readPngData( png_structp png_ptr_, png_bytep data_, png_size_t length_ )
    {
        PngData* buffer = (PngData*)png_get_io_ptr( png_ptr_ );

        if ( buffer->Offset + length_ <= buffer->Length )
        {
            memcpy( data_, buffer->Data + buffer->Offset, length_ );
            buffer->Offset += length_;
        }
        else
        {
            png_error( png_ptr_, "_readPngData failed" );
        }
    }



    struct PngStreamUserData
    {
        File::IInStream*    Stream;
    };
    static void _readPngDataFromStream( png_structp png_ptr_, png_bytep data_, png_size_t length_ )
    {
        PngStreamUserData* user_data = (PngStreamUserData*)png_get_io_ptr( png_ptr_ );

        int read_size = user_data->Stream->read(data_, length_);
        if (read_size != length_)
        {
            png_error( png_ptr_, "_readPngDataFromStream failed" );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    bool Image::getImageInfo( File::IInStream* stream_ )
    {
        // 注意・・・
        png_structp     mPngStruct;
	    png_infop       mPngInfo;

        PngStreamUserData user_data;
        user_data.Stream = stream_;

        mPngStruct = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
		if ( !mPngStruct ) goto ERR_EXIT;

        mPngInfo = png_create_info_struct( mPngStruct );
		if ( !mPngInfo ) goto ERR_EXIT;

        if (setjmp(png_jmpbuf(mPngStruct))) goto ERR_EXIT;

        png_set_read_fn( mPngStruct, (void *)&user_data, _readPngDataFromStream );

        png_read_png( mPngStruct, mPngInfo, PNG_TRANSFORM_EXPAND, NULL );

        this->Width  = mPngInfo->width;
        this->Height = mPngInfo->height;

        // 終了処理
        if ( mPngStruct )
        {
            png_destroy_read_struct( &mPngStruct, &mPngInfo, NULL );
        }
        return true;

    ERR_EXIT:

        this->Width = 0;
        this->Height = 0;
        SAFE_DELETE( ImageData );
        if ( mPngStruct )
        {
            png_destroy_read_struct( &mPngStruct, &mPngInfo, NULL );
        }
		return false;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    bool Image::createPngFromBytes( void* data_, lnU32 size_, bool swap_height_ )
    {
        png_structp     mPngStruct;
	    png_infop       mPngInfo;

        png_byte	sig[PNG_BYTES_TO_CHECK];
		size_t		size;
		int			res;
        
        PngData png_data;
        png_data.Data   = (unsigned char*)data_;
        png_data.Offset = 0;
        png_data.Length = size_;

        // サイズチェック
        if ( size_ < PNG_BYTES_TO_CHECK ) goto ERR_EXIT;

		// データが png かどうか調べる
		res = png_check_sig( (png_byte*)data_, PNG_BYTES_TO_CHECK ); 
		if ( !res ) goto ERR_EXIT;

		// png_struct構造体を確保・初期化する
		mPngStruct = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
		if ( !mPngStruct ) goto ERR_EXIT;

		// png_info構造体を確保・初期化する
		mPngInfo = png_create_info_struct( mPngStruct );
		if ( !mPngInfo ) goto ERR_EXIT;

        if (setjmp(png_jmpbuf(mPngStruct))) goto ERR_EXIT;

        // png データ、読み込みコールバック設定
        png_set_read_fn( mPngStruct, (void *)&png_data, _readPngData );

		// シグネチャの確認で読み飛ばしたバイト数を知らせる
		//png_set_sig_bytes( mPngStruct, PNG_BYTES_TO_CHECK );

        // PNGファイルのヘッダ情報を読み込む
        png_read_png( mPngStruct, mPngInfo, PNG_TRANSFORM_EXPAND, NULL );

        this->Width  = mPngInfo->width;
        this->Height = mPngInfo->height;

        // 必ず1色 8 ビットで
        if ( mPngInfo->bit_depth != 8 ) goto ERR_EXIT;

        // パレットモードは非対応
        if ( mPngInfo->color_type & PNG_COLOR_MASK_PALETTE ) goto ERR_EXIT;

        //-----------------------------------------------------
        // ビットマップ格納
        
        // gcc コンパイラでは同スコープ内で変数宣言以前に goto がある場合エラーになるので、その対策でひとつネスト
        {
            // 横幅のバイト数 (例えば ABGR の時は 横幅 * 4)
            unsigned int row_bytes = png_get_rowbytes( mPngStruct, mPngInfo );
            
            png_bytepp row_pointers = png_get_rows( mPngStruct, mPngInfo );

            lnByte* image_data;
            lnU32 data_size = this->Height * this->Width * 4;
            if ( this->ImageDataSize < data_size )
            {
                image_data = LN_NEW lnByte[data_size];
                this->ImageData = (lnU32*)image_data;
                this->ImageDataSize = data_size;
            }
            else
            {
                image_data = (lnByte*)this->ImageData;
            }

            lnU32 sign = ( swap_height_ ) ? -1 : 1;
            lnU32 unit = ( swap_height_ ) ? this->Height - 1 : 0;

            // ABGR (ピクセルフォーマットは AA BB GG RR)
            if ( mPngInfo->color_type == PNG_COLOR_TYPE_RGB_ALPHA && mPngInfo->pixel_depth == 32 )
            {
                for ( lnU32 h = 0; h < this->Height; ++h )
                    memcpy( image_data + (row_bytes * (unit + (sign * h))), row_pointers[h], row_bytes );
            }
            // BGR (Byte に分けたピクセルフォーマットは RR GG BB)
            else if ( mPngInfo->color_type == PNG_COLOR_TYPE_RGB && mPngInfo->pixel_depth == 24 )
            {
                lnU32* image = (lnU32*)image_data;
                lnByte* row;
                lnByte* b;
                for ( lnU32 y = 0; y < this->Height; ++y )
                {
                    row = row_pointers[unit + (sign * y)];
                    for ( lnU32 x = 0; x < this->Width; ++x )
                    {
                        b = &row[x * 3];
                        image[x + this->Width * y] = 
                            0xff000000           |
                            ( *( b + 0 ) )       |
                            ( *( b + 1 ) << 8 )  |
                            ( *( b + 2 ) << 16 );
                    }
                }
            }
            // Gray
            else if ( mPngInfo->color_type == PNG_COLOR_TYPE_GRAY && mPngInfo->pixel_depth == 8 )
            {
                lnU32* image = (lnU32*)image_data;
                lnByte* row;
                lnByte c;
                for ( lnU32 y = 0; y < this->Height; ++y )
                {
                    row = row_pointers[unit + (sign * y)];
                    for ( lnU32 x = 0; x < this->Width; ++x )
                    {
                        c = row[x];
                        image[x + this->Width * y] =
                            0xff000000  |
                            ( c )       |
                            ( c << 8 )  |
                            ( c << 16 );
                    }
                }
            }
            else
            {
                goto ERR_EXIT;
            }
        }

        
        // 終了処理
        if ( mPngStruct )
        {
            png_destroy_read_struct( &mPngStruct, &mPngInfo, NULL );
        }
        return true;

    ERR_EXIT:

        this->Width = 0;
        this->Height = 0;
        SAFE_DELETE( ImageData );
        if ( mPngStruct )
        {
            png_destroy_read_struct( &mPngStruct, &mPngInfo, NULL );
        }
		return false;
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================