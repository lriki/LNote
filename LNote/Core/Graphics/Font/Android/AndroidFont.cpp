//=============================================================================
//【 AndroidFont 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../../../AndroidMain/JavaGlue.h"
#include "../../../Base/StringUtil.h"
#include "AndroidFont.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace Android
{

//=============================================================================
// ■ AndroidFont クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    AndroidFont::AndroidFont()
        : mFontID           ( 0 )
        , mFontSize         ( 22 )
        , mColor            ( LColor::WHITE )
        , mEdgeColor        ( LColor::GRAY )
        , mEdgeSize         ( NULL )
        , mBold             ( false )
        , mItalic           ( false )
        , mAntiAlias        ( false )
        , mRequestedUpdate  ( true )
    {
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    AndroidFont::~AndroidFont()
    {
        _dispose();
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    void AndroidFont::initialize()
    {
    }

    //---------------------------------------------------------------------
    // ● 文字列を描画したときのサイズ (ピクセル単位) の取得 (char)
    //---------------------------------------------------------------------
    LNRESULT AndroidFont::getTextSize( const lnChar* text_, int len_, LRect* rect_ )
    {
        if ( len_ == 0 )
        {
            rect_->set( 0, 0, 0, 0 );
            return LN_OK;
        }

        _updateFont();
        
        rect_->set(
            0, 0,
            LNote::JavaGlue::getFontTextWidth( mFontID, text_ ),
            LNote::JavaGlue::getFontTextHeight( mFontID, text_ ) );
        
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● フォント文字のビットマップ情報の取得開始
    //---------------------------------------------------------------------
    LNRESULT AndroidFont::beginGetGlyphOutline()
    {
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● フォント文字のビットマップ情報の取得終了
    //---------------------------------------------------------------------
    LNRESULT AndroidFont::endGetGlyphOutline()
    {
        return LN_OK;
    }
    
    
    ////////////////////////////////////////////////////////////////////////////////
//
//  UTF-8:文字のバイト数を得る
//
int     MBCodeGetUTF8ByteLen
(
    const char    *s,             // (i)  文字列(UTF-8であると想定)
    int     *u8len          // (o)  UTF-8のバイト数
                            //      0:終端(0)またはBOMまたはUTF-8でない
                            //      1:ASCII
                            //      2-4:UTF-8コード長
)
{
    int             len;
    unsigned char   c[ 4 ];

    if((s == NULL)|| (u8len == NULL))
    {
        return -1;
    }

    *u8len = 0;
    len = strlen(s);
    if (len == 0)
    {
        return -1;
    }

    c[ 0 ] = s[ 0 ];
    if ((c[ 0 ] >= 0x01) && (c[ 0 ] <= 0x7F))
    {
        *u8len = 1;                     // 1バイト ASCII : 0x00～0x7F
    }
    else if (len >= 2)
    {
        c[ 1 ] = s[ 1 ];
        if (   ((c[ 0 ] >= 0xC0) && (c[ 0 ] <= 0xDF))
            && ((c[ 1 ] >= 0x80) && (c[ 1 ] <= 0xBF)))
        {
            *u8len = 2;                 // 2バイト
        }
        else if (len >= 3)
        {
            c[ 2 ] = s[ 2 ];
            if ((c[ 0 ] == 0xEF) && (c[ 1 ] == 0xBB) && (c[ 2 ] == 0xBF))
            {
                *u8len = 0;             // BOM 0xEF 0xBB 0xBF
            }
            else
            if (   ((c[ 0 ] >= 0xE0) && (c[ 0 ] <= 0xEF))
                && ((c[ 1 ] >= 0x80) && (c[ 1 ] <= 0xBF))
                && ((c[ 2 ] >= 0x80) && (c[ 2 ] <= 0xBF)))
            {
                *u8len = 3;             // 3バイト
            }
            else
            if (len >= 4)
            {
                c[ 3 ] = s[ 3 ];
                if (   ((c[ 0 ] >= 0xF0) && (c[ 0 ] <= 0xF7))
                    && ((c[ 1 ] >= 0x80) && (c[ 1 ] <= 0xBF))
                    && ((c[ 2 ] >= 0x80) && (c[ 2 ] <= 0xBF))
                    && ((c[ 3 ] >= 0x80) && (c[ 3 ] <= 0xBF)))
                {
                    *u8len = 4;         // 4バイト
                }
            }
        }
    }
    return 0;
}


    //---------------------------------------------------------------------
    // ● フォント文字のビットマップ情報を取得する (char)
    //---------------------------------------------------------------------
    LNRESULT AndroidFont::getGlyphOutline( const char* code_, LNGlyphOutlineData** data_ )
    {
        if ( *code_ == '\0' ) { 
	return LN_OK; }
        
        _updateFont();
        
        //_p(code_);
        
        int len;
        MBCodeGetUTF8ByteLen( code_, &len );
        //_p(len);
        
        char text[5];
        int i;
        for ( i = 0; i < len; ++i )
        {
            text[i] = code_[i];
        }
        text[i] = '\0';
        mGlyphOutlineData.CharIdxOffset = len;
        /*
        if ( Base::StringUtil::checkMultiByteChar( code_ ) )
        {
            text[0] = code_[0];
            text[1] = code_[1];
            text[2] = '\0';
            mGlyphOutlineData.CharIdxOffset = 2;
        }
        else
        {
            text[0] = code_[0];
            text[1] = '\0';
            mGlyphOutlineData.CharIdxOffset = 1;
        }
        */
        
        //_p(mGlyphOutlineData.CharIdxOffset);
        //_p(text);
     
        LNote::JavaGlue::getFontBitmap( mFontID, text, &mImage );

        mGlyphOutlineData.Bitmap        = (lnU8*)mImage.ImageData;
        mGlyphOutlineData.BitmapWidth   = mImage.Width;
        mGlyphOutlineData.BitmapHeight  = mImage.Height;
        mGlyphOutlineData.GlyphOffsetX  = 0;
        mGlyphOutlineData.GlyphOffsetY  = 0;
        mGlyphOutlineData.GlyphWidth    = mImage.Width;
        mGlyphOutlineData.GlyphHeight   = mImage.Height;
        mGlyphOutlineData.CellWidth     = mImage.Width;
        //mGlyphOutlineData.CharIdxOffset
        mGlyphOutlineData.BitmapIsARGB = true;
        
        _p(mGlyphOutlineData.Bitmap);

        *data_ = &mGlyphOutlineData;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● フォント文字のビットマップ情報を取得する (wchar_t)
    //---------------------------------------------------------------------
    LNRESULT AndroidFont::getGlyphOutline( const wchar_t* code_, LNGlyphOutlineData** data_ )
    {
        LN_THROW_NotImpl( 0 );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● フォントのコピーを作成する
    //---------------------------------------------------------------------
    LNRESULT AndroidFont::deepCopy( IFont** font_ )
    {
        AndroidFont* new_font = LN_NEW AndroidFont();
        new_font->initialize();
        new_font->setName( getName() );
        new_font->setSize( getSize() );
        new_font->setColor( getColor() );
        new_font->setBold( isBold() );
        new_font->setItalic( isItalic() );
        new_font->setEdgeColor( getEdgeColor() );
        new_font->setEdgeSize( getEdgeSize() );
        new_font->setAntiAlias( isAntiAlias() );

        *font_ = new_font;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 現在の状態からフォントを更新する (再構築)
    //---------------------------------------------------------------------
    LNRESULT AndroidFont::_updateFont()
    {
        if ( mRequestedUpdate )
        {
            _dispose();

            mFontID = LNote::JavaGlue::createFont();
            _p(mFontID);
            LNote::JavaGlue::updateFont( mFontID, mFontName.c_str(), mBold, mItalic, mFontSize, mAntiAlias );

            mRequestedUpdate = false;
        }
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● リソース解放
    //---------------------------------------------------------------------
    void AndroidFont::_dispose()
    {
        if ( mFontID )
        {
            LNote::JavaGlue::deleteFont( mFontID );
            mFontID = NULL;
        }
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Android
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================