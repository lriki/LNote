//=============================================================================
//【 GraphicsUtil 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "GraphicsUtil.h"

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
// ■ Util クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● 頂点フォーマットから頂点のサイズを求める
    //---------------------------------------------------------------------
    u32 Util::getVertexSize( const LNVertexElemenst* element_, u32 stream_index_ )
    {
        u32 size = 0;
        u32 i = 0;

        while ( true )
        {
            if ( element_[ i ].StreamIndex != stream_index_ || element_[ i ].Usage == LN_USAGE_UNKNOWN )
            {
                break;
            }

            size += getVertexDeclTypeSize( element_[ i ].Type );

            ++i;
        }

        return size;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    lnU32 Util::getVertexDeclTypeSize( LNDeclType type_ )
    {
        switch ( type_ )
	    {
		    case LN_DECL_FLOAT1: 
		    	return sizeof( float );
		    case LN_DECL_FLOAT2: 
		    	return sizeof( float ) * 2;
		    case LN_DECL_FLOAT3: 
		    	return sizeof( float ) * 3;
		    case LN_DECL_FLOAT4: 
		    	return sizeof( float ) * 4;
		    case LN_DECL_UBYTE4: 
		    	return sizeof( unsigned char ) * 4;
		    case LN_DECL_COLOR4: 
		    	return sizeof( unsigned char ) * 4;
		    case LN_DECL_SHORT2: 
		    	return sizeof( short ) * 2;
		    case LN_DECL_SHORT4: 
		    	return sizeof( short ) * 4;
	    }
        return 0;
    }

    //---------------------------------------------------------------------
    // ● ビットマップ転送の際の必要なループ数を計算する (ループ数が 0 以下の場合 false を返す)
    //---------------------------------------------------------------------
    bool calcBitBltLoopNum(
        int* loop_x_, int* loop_y_,
        int* x_, int* y_, int r_, int b_,
        int dest_w_, int dest_h_,
        int sx_, int sy_,
        int sw_, int sh_, int  src_w_, int src_h_ )
    {
        // 転送先の範囲に入っているかチェック
		if ( *x_ + sw_ < 0 ||
			 *y_ + sh_ < 0 ||
			 *x_ >= dest_w_ ||
			 *y_ >= dest_h_ )
		{
			return false;
		}
		// 転送元の範囲に入っているかチェック
		if ( sx_ + sw_ < 0 ||
			 sy_ + sh_ < 0 ||
			 sx_ >= src_w_ ||
			 sy_ >= src_h_ )
		{
			return false;
		}
		// ループ回数を調べていく
		*loop_x_ = sw_;
		*loop_y_ = sh_;
		if ( *x_ < 0 )
		{
			*loop_x_ += *x_;
			*x_ = 0;
		}
        if ( *x_ + *loop_x_ >= r_ )
        {
            *loop_x_ -= ( *x_ + *loop_x_ ) - r_;
        }
		else if ( *x_ + *loop_x_ >= dest_w_ )
		{
			*loop_x_ -= ( *x_ + *loop_x_ ) - dest_w_;
		}
        
		if ( *y_ < 0 )
		{
			*loop_y_ += *y_;
			*y_ = 0;
		}
        if ( *y_ + *loop_y_ >= b_ )
        {
            *loop_y_ -= ( *y_ + *loop_y_ ) - b_;
        }
		else if ( *y_ + *loop_y_ >= dest_h_ )
		{
			*loop_y_ -= ( *y_ + *loop_y_ ) - dest_h_;
		}
        return true;
    }

    //---------------------------------------------------------------------
    // ● ビットマップの転送を行う
    //---------------------------------------------------------------------
    void Util::bitblt(
        int x_,  int y_ , u32* dest_, int dest_w_, int dest_h_,
	    int sx_, int sy_, int sw_, int sh_, u32* src_ , int src_w_ , int src_h_,
        bool alpha_ )
    {
        bitblt( 
            x_,  y_ , dest_w_ - x_, dest_h_ - y_, dest_, dest_w_, dest_h_,
	        sx_, sy_, sw_, sh_, src_ , src_w_ , src_h_, alpha_ );
    }

    //---------------------------------------------------------------------
    // ● ビットマップの転送を行う (右下クリッピング付き)
    //---------------------------------------------------------------------
    void Util::bitblt(
        int x_,  int y_ , int  r_, int  b_, u32* dest_, int dest_w_, int dest_h_,
	    int sx_, int sy_, int sw_, int sh_, u32* src_ , int src_w_ , int src_h_,
        bool alpha_ )
    {
        int loop_x, loop_y;
        if ( !calcBitBltLoopNum( &loop_x, &loop_y, &x_, &y_, r_, b_, dest_w_, dest_h_, sx_, sy_, sw_, sh_, src_w_, src_h_ ) ) { 
	return; }
		
        // アルファブレンドを行う場合
        if ( alpha_ )
        {
            u32* write_pos;     // 書き込むピクセル
            u32  color;         // 書き込む色
            u32  dest_color;
            u32  dest_alpha;    // 転送先アルファ値 (0～255)
            u32  src_color;
            u32  src_alpha;     // 転送元アルファ値 (0～255)
            u32  a, r, g, b;

            for ( int y = 0; y < loop_y; ++y )
		    {
			    for ( int x = 0; x < loop_x; ++x )
			    {
                    

                    // 書き込み位置
                    write_pos = dest_ + ( ( x_ + x ) + ( ( y_ + y ) * dest_w_ ) );
                    
                    // 転送先、転送元のアルファ値
                    dest_color = *write_pos;
                    dest_alpha = ( *write_pos >> 24 ) & 0xff;
                    src_color  = *( src_ + ( ( sx_ + x ) + ( ( sy_ + y ) * src_w_ ) ) );
                    src_alpha  = ( src_color & 0xff000000 ) >> 24;


                    
                    if ( dest_alpha == 0 )
                    {
                        a = 0xff;
                    }
                    else if ( src_alpha == 0 )
                    {
                        a = 0;
                    }
                    else
                    {
                        a = src_alpha;// * ( ( color_ & 0xff000000 ) >> 24 );
                        //a >>= 8;
                    }
                  

                    r = ( ( ( dest_color & 0x00ff0000 ) >> 16 ) * ( 255 - a ) ) +
                        ( ( ( src_color  & 0x00ff0000 ) >> 16 ) * a );
                    r >>= 8;
                    r = ( r > 255 ) ? 255 : r;

                    g = ( ( ( dest_color & 0x0000ff00 ) >> 8 ) * ( 255 - a ) ) +
                        ( ( ( src_color  & 0x0000ff00 ) >> 8 ) * a );
                    g >>= 8;
                    g = ( g > 255 ) ? 255 : g;

                    b = ( ( ( dest_color & 0x000000ff ) ) * ( 255 - a ) ) +
                        ( ( ( src_color  & 0x000000ff ) ) * a );
                    b >>= 8;
                    b = ( b > 255 ) ? 255 : b;

                    // アルファ値を書き込み用のアルファ値として計算し直す
                    a = ( dest_alpha + src_alpha );
                    a = ( a > 255 ) ? 255 : a;

                    color = ( a << 24 ) |
                            ( r << 16 ) |
                            ( g << 8 ) |
                            ( b );
                
                    // 書き込む
                    *write_pos = color;
			    }
            }
        }
        // アルファブレンドを行わない場合
        else
        {
		    for ( int y = 0; y < loop_y; ++y )
		    {
			    //for ( int x = 0; x < loop_x; ++x )
			    //{
			    //	dest_[ ( x_ + x ) + ( ( y_ + y ) * dest_w_ ) ] = src_[ ( sx_ + x ) + ( ( sy_ + y ) * src_w_ ) ];
			    //}
                // 横方向まるコピ
                memcpy( dest_ + ( x_ + ( ( y_ + y ) * dest_w_ ) ), src_ + ( sx_ + ( ( sy_ + y ) * src_w_ ) ), sizeof( u32 ) * loop_x );
		    }
        }
    }

    //---------------------------------------------------------------------
    // ● ビットマップの転送を行う (転送元ビットマップを u8 のアルファマスクとする)
    //---------------------------------------------------------------------
    void Util::bitbltAlphaByte(
        int x_,  int y_ , u32* dest_, int dest_w_, int dest_h_,
	    int sx_, int sy_, int sw_, int sh_, u8* src_, u8 a_level_, u32 color_, int src_w_ , int src_h_,
        bool alpha_ )
    {
        bitbltAlphaByte(
            x_, y_, dest_w_ - x_, dest_h_ - y_, dest_, dest_w_, dest_h_, 
            sx_, sy_, sw_, sh_, src_, a_level_, color_, src_w_, src_h_, alpha_ );
    }

    //---------------------------------------------------------------------
    // ● ビットマップの転送を行う (転送元ビットマップを u8 のアルファマスクとする)
    //---------------------------------------------------------------------
    void Util::bitbltAlphaByte(
        int x_,  int y_ , int  r_, int  b_, u32* dest_, int dest_w_, int dest_h_,
	    int sx_, int sy_, int sw_, int sh_, u8* src_, u8 a_level_, u32 color_, int src_w_ , int src_h_,
        bool alpha_ )
    {
        int loop_x, loop_y;
        if ( !calcBitBltLoopNum( &loop_x, &loop_y, &x_, &y_, r_, b_, dest_w_, dest_h_, sx_, sy_, sw_, sh_, src_w_, src_h_ ) ) { 
	return; }

        // アルファブレンドを行う場合
        if ( alpha_ )
        {
            const u8 level = 255 / a_level_;    // src_ から取得した値にこれを掛けると 0～255 に変換した値になる
            u32* write_pos;     // 書き込むピクセル
            u32  color;         // 書き込む色
            u32  dest_color;
            u32  dest_alpha;    // 転送先アルファ値 (0～255)
            u32  src_alpha;     // 転送元アルファ値 (0～255)
            u32  a, r, g, b;

            for ( int y = 0; y < loop_y; ++y )
		    {
			    for ( int x = 0; x < loop_x; ++x )
			    {
                    

                    // 書き込み位置
                    write_pos = dest_ + ( ( x_ + x ) + ( ( y_ + y ) * dest_w_ ) );
                    
                    // 転送先、転送元のアルファ値
                    dest_color = *write_pos;
                    dest_alpha = ( *write_pos >> 24 ) & 0xff;
                    src_alpha  = *( src_ + ( ( sx_ + x ) + ( ( sy_ + y ) * src_w_ ) ) ) * level;

                    //// 転送元の値をブレンド用のアルファ値にする
                    //a = src_alpha * ( ( color_ & 0xff000000 ) >> 24 );
                    //a >>= 8;
                    // ↑だと、dest が 0x00000000 だった場合に src のアルファ値が 0xff ではない部分が
                    // 黒とブレンドされて黒ずんでしまう。

                    
                    if ( dest_alpha == 0 )
                    {
                        a = 0xff;
                    }
                    else if ( src_alpha == 0 )
                    {
                        a = 0;
                    }
                    else
                    {
                        a = src_alpha * ( ( color_ & 0xff000000 ) >> 24 );
                        a >>= 8;
                    }
                  

                    r = ( ( ( dest_color & 0x00ff0000 ) >> 16 ) * ( 255 - a ) ) +
                        ( ( ( color_     & 0x00ff0000 ) >> 16 ) * a );
                    r >>= 8;
                    r = ( r > 255 ) ? 255 : r;

                    g = ( ( ( dest_color & 0x0000ff00 ) >> 8 ) * ( 255 - a ) ) +
                        ( ( ( color_     & 0x0000ff00 ) >> 8 ) * a );
                    g >>= 8;
                    g = ( g > 255 ) ? 255 : g;

                    b = ( ( ( dest_color & 0x000000ff ) ) * ( 255 - a ) ) +
                        ( ( ( color_     & 0x000000ff ) ) * a );
                    b >>= 8;
                    b = ( b > 255 ) ? 255 : b;

                    // アルファ値を書き込み用のアルファ値として計算し直す
                    a = ( dest_alpha + ((src_alpha * ( ( color_ & 0xff000000 ) >> 24 ))>>8) );
                    a = ( a > 255 ) ? 255 : a;

                    color = ( a << 24 ) |
                            ( r << 16 ) |
                            ( g << 8 ) |
                            ( b );
                
                    // 書き込む
                    *write_pos = color;
			    }
            }
        }
        // アルファブレンドを行わない場合
        else
        {            
            const u8 level = 255 / a_level_;    // src_ から取得した値にこれを掛けると 0～255 に変換した値になる
            u32* write_pos;                     // 書き込むピクセル
            u32  color;                         // 書き込む色

		    for ( int y = 0; y < loop_y; ++y )
		    {
                for ( int x = 0; x < loop_x; ++x )
			    {
                    // 書き込み位置と、転送するアルファ値を取得しておく
                    write_pos = dest_ + ( ( x_ + x ) + ( ( y_ + y ) * dest_w_ ) );
                    color = ( *( src_ + ( ( sx_ + x ) + ( ( sy_ + y ) * src_w_ ) ) ) );

                    color *= level;                                     // ここで 0～255 に変換
                    color *= ( ( color_ & 0xff000000 ) >> 24 );         // color_ のアルファ値と乗算
                    color >>= 8;                                        // 乗算結果を 0～255 に変換
                    color = ( color << 24 ) | ( color_ & 0x00ffffff );  // 元の色とくっつけて書き込む色の完成

                    // ↑もうちょっと最適化できるけどあんまり見辛くなってもアレなのでとりあえずここまで
                    
                    // 書き込む
                    *write_pos = color;
                }
		    }
        }
    }

    //---------------------------------------------------------------------
    // ● テクスチャフォーマット文字列を LNSurfaceFormat に変換する
    //---------------------------------------------------------------------
    LNSurfaceFormat Util::convertFMTStringToLNFMT( const lnChar* fmt_ )
    {
#define LN_STRCMP( a_, b_ )   ( _tcsicmp( a_, _T( b_ ) ) == 0 )

        ln_std_tstring tfmt = fmt_;

        // D3DFMT_、FMT_ があれば削除
        Base::StringUtil::replaceString( &tfmt, _T( "D3DFMT_" ), _T( "" ) );
        Base::StringUtil::replaceString( &tfmt, _T( "FMT_" ), _T( "" ) );

        const lnChar* fmt = tfmt.c_str();

        // よく使うものを先に判定
        if LN_STRCMP( fmt, "A8R8G8B8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "X8R8G8B8" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "A16B16G16R16F" ) { 
        	return LN_FMT_A16B16G16R16F; }
        if LN_STRCMP( fmt, "A32B32G32R32F" ) { 
        	return LN_FMT_A16B16G16R16F; }
        if LN_STRCMP( fmt, "D24S8" ) { 
        	return LN_FMT_D24S8; }


        // バックバッファー フォーマットまたはディスプレイ フォーマット
        if LN_STRCMP( fmt, "A2R10G10B10" ) { 
        	return LN_FMT_A8R8G8B8; }
        //if LN_STRCMP( fmt, "A8R8G8B8" ) { return LN_FMT_A8R8G8B8; }
        //if LN_STRCMP( fmt, "X8R8G8B8" ) { return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "A1R5G5B5" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "X1R5G5B5" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "R5G6B5" ) { 
        	return LN_FMT_A8R8G8B8; }

        // バッファー フォーマット
        if LN_STRCMP( fmt, "D16_LOCKABLE" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "D32" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "D15S1" ) { 
        	return LN_FMT_D24S8; }
        //if LN_STRCMP( fmt, "D24S8" ) { return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "D24X8" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "D24X4S4" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "D32F_LOCKABLE" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "D24FS8" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "D32_LOCKABLE" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "S8_LOCKABLE" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "D16" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "VERTEXDATA" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "INDEX16" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "INDEX32" ) { 
        	return LN_FMT_D24S8; }
        
        // DXTn 圧縮テクスチャー フォーマット
        if LN_STRCMP( fmt, "DXT1" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "DXT2" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "DXT3" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "DXT4" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "DXT5" ) { 
        	return LN_FMT_A8R8G8B8; }

        // 浮動小数点フォーマット
        if LN_STRCMP( fmt, "R16F" ) { 
        	return LN_FMT_A16B16G16R16F; }
        if LN_STRCMP( fmt, "G16R16F" ) { 
        	return LN_FMT_A16B16G16R16F; }
        //if LN_STRCMP( fmt, "A16B16G16R16F" ) { return LN_FMT_A16B16G16R16F; }

        // FOURCC フォーマット
        if LN_STRCMP( fmt, "MULTI2_ARGB8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "G8R8_G8B8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "R8G8_B8G8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "UYVY" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "YUY2" ) { 
        	return LN_FMT_A8R8G8B8; }

        // IEEE フォーマット
        if LN_STRCMP( fmt, "R32F" ) { 
        	return LN_FMT_A16B16G16R16F; }
        if LN_STRCMP( fmt, "G32R32F" ) { 
        	return LN_FMT_A16B16G16R16F; }
        //if LN_STRCMP( fmt, "A32B32G32R32F" ) { return LN_FMT_A16B16G16R16F; }

        // ミックス フォーマット
        if LN_STRCMP( fmt, "L6V5U5" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "X8L8V8U8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "A2W10V10U10" ) { 
        	return LN_FMT_A8R8G8B8; }

        // 符号付きフォーマット
        if LN_STRCMP( fmt, "V8U8" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "Q8W8V8U8" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "V16U16" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "Q16W16V16U16" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "CxV8U8" ) { 
        	return LN_FMT_X8R8G8B8; }

        // 符号なしフォーマット
        if LN_STRCMP( fmt, "R8G8B8" ) { 
        	return LN_FMT_X8R8G8B8; }
        //if LN_STRCMP( fmt, "A8R8G8B8" ) { return LN_FMT_A8R8G8B8; }
        //if LN_STRCMP( fmt, "X8R8G8B8" ) { return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "R5G6B5" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "X1R5G5B5" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "A1R5G5B5" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "A4R4G4B4" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "R3G3B2" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "A8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "A8R3G3B2" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "X4R4G4B4" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "A2B10G10R10" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "A8B8G8R8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "X8B8G8R8" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "G16R16" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "A2R10G10B10" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "A16B16G16R16" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "A8P8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "P8" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "L8" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "L16" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "A8L8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "A4L4" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "A1" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "BINARYBUFFER" ) { 
        	return LN_FMT_X8R8G8B8; }

        return LN_FMT_X8R8G8B8;

#undef LN_STRCMP
    }

   /*








符号付きフォーマットのデータは、正でも負でもかまいません。符号付きフォーマットでは、(U)、(V)、(W)、および (Q) のデータを組み合わせて使用します。

符号付きフォーマット フラグ	値	フォーマット


符号なしフォーマットのデータは、正でなければなりません。符号なしフォーマットでは、赤 (R)、緑 (G)、青 (B)、アルファ (A)、輝度 (L)、パレット (P) のデータを組み合わせて使用します。パレット データは、カラー パレットのインデックス作成に使用されるので、カラー インデックス データとも呼ばれます。

符号付きフォーマット フラグ	値	フォーマット
その他

このフラグは、未定義のフォーマットに使用されます。

その他のフラグ	値	フォーマット
D3DFMT_UNKNOWN
*/

    //---------------------------------------------------------------------
    // ● テクスチャの作成
    //---------------------------------------------------------------------
    LNRESULT Util::createTexture( ITexture** texture_, IGraphicsDevice* device_, u32 width_, u32 height_ )
    {
        return device_->createTexture(
            texture_, width_, height_, 1, LN_FMT_A8R8G8B8 );
    }


  
//=============================================================================
// ■ LNFXShaderSource
//=============================================================================

struct LNFXTag
{
	const char* TagName;
	const char*	DefineStr;
};

static LNFXTag gLNFXTag[] =
{
	{ "@hlsl",		    "#ifdef LN_HLSL" },
	{ "@endhlsl",	    "#endif" },
	{ "@glslvert",	    "#ifdef LN_GLSL_VERT" },
	{ "@endglslvert",   "#endif" },
	{ "@glslflag",	    "#ifdef LN_GLSL_FLAG" },
	{ "@endglslflag",   "#endif" },
	{ "@glslmeta",		"#ifdef LN_GLSL_META" },
	{ "@endglslmeta",   "#endif" }
};

static const char* GLSL_META_DATA_BEGIN = gLNFXTag[6].DefineStr;
static const char* GLSL_META_DATA_END   = gLNFXTag[7].DefineStr;


const char* LNFXShaderSource::HLSL_MACRO      = "LN_HLSL";
const char* LNFXShaderSource::GLSL_VERT_MACRO = "LN_GLSL_VERT";
const char* LNFXShaderSource::GLSL_FLAG_MACRO = "LN_GLSL_FLAG";
const char* LNFXShaderSource::GLSL_META_MACRO = "LN_GLSL_META";


    void LNFXShaderSource::initialize(const char* buf_, ShaderSystem system_)
    {
	    mSource = buf_;

	    for (int i = 0; i < 8; ++i)
	    {
            Base::StringUtil::replaceString(&mSource, gLNFXTag[i].TagName, gLNFXTag[i].DefineStr);
	    }

	    //printf("---------------------------\n");
        //printf("%s\n", mSource.c_str());

        _splitMetaData();

		if (system_ == SHADER_GLSL)
		{
			Base::StringUtil::replaceString(&mSource, "\"", " ");
		}
	    
    /*
	    int begin_str_len = strlen(GLSL_META_DATA_BEGIN);
	    mGLSLMetaDataSource = std::string(
		    mSource.c_str(),
		    meta_begin_idx + begin_str_len,
		    meta_end_idx - meta_begin_idx - begin_str_len );

        // 暫定CRLF対策
        if (mGLSLMetaDataSource[0] == 13 && mGLSLMetaDataSource[1] == 10)
        {
            mGLSLMetaDataSource = "";
        }
    */

       /* printf("---------------------------\n");
	    printf("%s\n", mGLSLMetaDataSource.c_str());

        printf("---------------------------\n");*/
    }

    void LNFXShaderSource::_splitMetaData()
    {
        // "#if LN_GLSL_META" を探す
        std::string::size_type meta_begin_idx = mSource.find( GLSL_META_DATA_BEGIN );
        if ( meta_begin_idx == std::string::npos )
        {
            return;
        }

	    // "#endif" を探す
        std::string::size_type meta_end_idx = mSource.find( GLSL_META_DATA_END, meta_begin_idx );
        if ( meta_end_idx == std::string::npos )
        {
            return;
        }

        mGLSLMetaDataSource = std::string(mSource.c_str(), meta_begin_idx, meta_end_idx - meta_begin_idx);
        Base::StringUtil::replaceString(&mGLSLMetaDataSource, gLNFXTag[6].DefineStr, "\n");

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