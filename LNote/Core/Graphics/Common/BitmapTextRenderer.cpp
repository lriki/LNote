//=============================================================================
//【 BitmapTextRenderer 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "GraphicsUtil.h"
#include "BitmapTextRenderer.h"

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
// ■ BitmapTextRenderer クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    BitmapTextRenderer::BitmapTextRenderer( LNGraphicsAPI api_ )
        : mGraphicsAPI  ( api_ )
        , mBitmapBuffer ( NULL )
        , mWidth        ( 0 )
        , mHeight       ( 0 )
        , mFont         ( NULL )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    BitmapTextRenderer::~BitmapTextRenderer()
    {
        LN_SAFE_RELEASE( mFont );
    }

    //---------------------------------------------------------------------
    // ● 描画先となるビットマップバッファを設定する
    //---------------------------------------------------------------------
    void BitmapTextRenderer::setTargetBuffer( void* buffer_, u32 width_, u32 height_ )
    {
        mBitmapBuffer = static_cast< u32* >( buffer_ );
        mWidth = width_;
        mHeight = height_;
    }

    //---------------------------------------------------------------------
    // ● 文字列の描画に使うフォントを設定する
    //---------------------------------------------------------------------
    void BitmapTextRenderer::setFont( IFont* font_ )
    {
        LN_SAFE_RELEASE( mFont );
        mFont = font_;
        LN_SAFE_ADDREF( mFont );
    }

    //---------------------------------------------------------------------
    // ● 文字列を描画する
    //---------------------------------------------------------------------
    LNRESULT BitmapTextRenderer::drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_, int len_ )
    {
        LNRESULT lr;

     

        // ビットマップ情報の取得開始
        mFont->beginGetGlyphOutline();

        // アルファ値以外の色を取り出しておく
        //u32 font_color = mFont->getColor().to32Bit() & 0x00ffffff;

        u32 font_color = mFont->getColor().to32Bit( mGraphicsAPI );

        // 描画領域の右側と下側を決める
		int	right, bottom;
		if ( rect_.w > 0 || rect_.h > 0 )
		{
			right  = rect_.x + rect_.w;
			bottom = rect_.y + rect_.h;
			right = ( ( right > (int)mWidth ) ? mWidth : right );
			bottom = ( ( bottom > (int)mHeight ) ? mHeight : bottom );
		}
		else
		{
			right = mWidth;
			bottom = mHeight;
		}

		// 文字を描画する左上位置
		int sx = rect_.x;
		int sy = rect_.y;

        const char* begin = text_;
        const char* end   = begin;
        int i = 0;
        while ( true )
        {
            // 改行または終端 (1行の終端) が見つかったら1行分を描画する
            if ( *end == '\n' || *end == '\0' || (len_ >= 0 && i >= len_) )
            {
                

                // 今回描画する行の収まる矩形を取得
			    //Geometry::Rect line_rect = font.getTextSize( str_ary[ i ].wstr() );
    			
			    switch ( align_ )
			    {
				    // 中央揃え
				    case LN_TEXTALIGN_CENTER:
                    {
                        int r = ( rect_.x + rect_.w );  // 描画領域の右側
                        Geometry::Rect rc;
                        LN_CALL_R( mFont->getTextSize( begin, (int)( end - begin ), &rc ) );
					    sx = sx + ( ( r - sx ) / 2 ) - ( rc.w / 2 );       //sx + ( ( right - sx ) / 2 );
					    break;
                    }
				    // 右揃え
				    case LN_TEXTALIGN_RIGHT:
                    {
                        int r = ( rect_.x + rect_.w );
                        Geometry::Rect rc;
                        mFont->getTextSize( begin, (int)( end - begin ), &rc );
					    sx = r - rc.w;
					    break;
                    }
				    // 左揃え
				    default:
                    {
					    sx = rect_.x;
					    break;
                    }
			    }

                int write_len;
                LN_CALL_R( _drawLine( begin, ( end - begin ), sx, sy, right, bottom, font_color, &write_len ) );

                //len_ -= write_len;
                i += write_len;

                // 終端だったら終了
                if ( *end == '\0' || (len_ >= 0 && i >= len_) )
                {
                    break;
                }
                else
                {
                    sy += mFont->getSize();

                    ++end;
                    begin = end;
                    //end   = begin;
                }
            }
            else
            {
                ++end;
                ++i;
            }
        }

        // ビットマップ情報の取得終了
        mFont->endGetGlyphOutline();

        return LN_OK;     
    }

    //---------------------------------------------------------------------
    // ● 1行分の描画
    //---------------------------------------------------------------------
    LNRESULT BitmapTextRenderer::_drawLine( const lnChar* text_, int len_, int l_, int t_, int r_, int b_, u32 color_, int* write_len_ )
    {
        LNRESULT lr;
        LNGlyphOutlineData* glyph;

        u32 sx = l_;    // 書き込み左上 X
        u32 sy = t_;    // 書き込み左上 Y    

        int i;
        for ( i = 0; i < len_ ;  )
        {
            // ビットマップ取得
            LN_CALL_R( mFont->getGlyphOutline( &text_[ i ], &glyph ) );


            if ( glyph->BitmapIsARGB )
            {
                Util::bitblt(
                    sx + glyph->GlyphOffsetX,
                    sy + glyph->GlyphOffsetY,
                    r_, b_,
                    mBitmapBuffer, mWidth, mHeight,
                    0, 0, glyph->GlyphWidth, glyph->GlyphHeight, (u32*)glyph->Bitmap,
                    glyph->BitmapWidth, glyph->BitmapHeight, true );
            }
            else
            {
                Util::bitbltAlphaByte(
                    sx + glyph->GlyphOffsetX,
                    sy + glyph->GlyphOffsetY,
                    r_, b_,
                    mBitmapBuffer, mWidth, mHeight,
                    0, 0, glyph->GlyphWidth, glyph->GlyphHeight, glyph->Bitmap, 17,
                    color_, glyph->BitmapWidth, glyph->BitmapHeight, true );
            }

            sx += glyph->CellWidth;

            i += glyph->CharIdxOffset;


        }


        *write_len_ = i;
        
        return LN_OK;
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