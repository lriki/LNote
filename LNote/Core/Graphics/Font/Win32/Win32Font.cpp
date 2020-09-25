//=============================================================================
//【 Win32Font 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../../Base/StringUtil.h"
#include "Win32Font.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace Win32
{

//=============================================================================
// ■ Win32Font クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    Win32Font::Win32Font()
        : mFontHandle           ( NULL )
        , mFontName             ( LN_DEFAULT_FONT_NAME )
        , mFontSize             ( 22 )
        , mGlyphType            ( LN_GT_NOEDGE_NOANTI )
        , mEdgeColor            ( 0, 0, 0, 1 )
        , mEdgeSize             ( 0 )
        , mRealEdgeSize         ( 0 )
        , mOutlineQuality       ( FONT_ANTI_QUALITY )
        , mMemDC                ( NULL )
        , mDIBBitmap            ( NULL )
        , mDIBBitmapBuffer      ( NULL )
        , mDIBBitmapWidth       ( 0 )
        , mDIBBitmapHeight      ( 0 )
        , mOutlinePen           ( NULL )
        , mOutlineBrush         ( NULL )
        , mOutlineBGBrush       ( NULL )
        
        , mBitmapBufferSize     ( 0 )
        , mHDC                  ( NULL )
        
        , mOldFont              ( NULL )
        
        , mNeedUpdate           ( true )
        , mBeganGetGlyphOutline ( false )
    {
        memset( &mLogFont, 0, sizeof( mLogFont ) );
        memset( &mGlyphOutlineData, 0, sizeof( mGlyphOutlineData ) );
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    Win32Font::~Win32Font()
    {
        _dispose();
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT Win32Font::initialize(
        const lnChar* fontname_,
        u32 size_,
        const LColor& color_,
        bool bold_,
        bool italic_ )
    {
        mFontName = fontname_;
        mColor = color_;

        mLogFont.lfHeight = size_ * mOutlineQuality;
		mLogFont.lfWidth = 0;
		mLogFont.lfEscapement = 0;
		mLogFont.lfOrientation = 0;
		mLogFont.lfWeight = ( ( bold_ ) ? FW_BOLD : FW_NORMAL );
		mLogFont.lfItalic = ( ( italic_ ) ? TRUE : FALSE );
		mLogFont.lfUnderline = FALSE;
		mLogFont.lfStrikeOut = FALSE;
		mLogFont.lfCharSet = SHIFTJIS_CHARSET;
		mLogFont.lfOutPrecision = OUT_TT_ONLY_PRECIS;
		mLogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		mLogFont.lfQuality = ANTIALIASED_QUALITY | PROOF_QUALITY;	// PROOF_QUALITY;
		mLogFont.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
        //wcscat_s( mLogFont.lfFaceName, LF_FACESIZE, mFontName.c_str() );
        _tcscat( mLogFont.lfFaceName, mFontName.c_str() );

        return _updateFont();
    }

    //---------------------------------------------------------------------
    // ● フォント名の設定
	//---------------------------------------------------------------------
    LNRESULT Win32Font::setName( const lnChar* fontname_ )
    {
        mFontName = fontname_;
        _tcscat( mLogFont.lfFaceName, mFontName.c_str() );
        mNeedUpdate = true;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● フォントサイズの設定
    //---------------------------------------------------------------------
    void Win32Font::setSize( u32 size_ )
    {
        mFontSize = size_;
        mNeedUpdate = true;
    }

	//---------------------------------------------------------------------
    // ● フォントカラーの設定
	//---------------------------------------------------------------------
    void Win32Font::setColor( const LColor& color_ )
    {
        mColor = color_;
        mNeedUpdate = true;
    }

   

	//---------------------------------------------------------------------
    // ● 太文字の設定
	//---------------------------------------------------------------------
    void Win32Font::setBold( bool flag_ )
    {
        mLogFont.lfWeight = ( ( flag_ ) ? FW_BOLD : FW_NORMAL );
        mNeedUpdate = true;
    }

	//---------------------------------------------------------------------
    // ● イタリック体の設定
	//---------------------------------------------------------------------
    void Win32Font::setItalic( bool flag_ )
    {
        mLogFont.lfItalic = ( ( flag_ ) ? TRUE : FALSE );
        mNeedUpdate = true;
    }

    //---------------------------------------------------------------------
    // ● 文字列を描画したときのサイズ (ピクセル単位) の取得 (char)
    //---------------------------------------------------------------------
    LNRESULT Win32Font::getTextSize( const lnChar* text_, int len_, Geometry::Rect* rect_ )
    {
        if ( len_ == 0 )
        {
            rect_->set( 0, 0, 0, 0 );
            return LN_OK;
        }

        _updateFont();

        // GetGlyphOutline から取得する場合
        if ( mGlyphType == LN_GT_NOEDGE_ANTI )
        {
            HDC hdc = GetDC( NULL );
            HFONT old_font = (HFONT)::SelectObject( hdc, mFontHandle );

            TEXTMETRICA tm;
            GLYPHMETRICS gm;
            MAT2 mat ={ {0,1}, {0,0}, {0,0}, {0,1} };
            
            ::GetTextMetrics( hdc, &tm );

            if ( len_ == -1 )
            {
                len_ = _tcslen( text_ );
            }

            rect_->w = 0;
            rect_->h = mFontSize;

            UINT code;
            for ( int i = 0; i < len_; ++i )
            {
#ifdef LNOTE_UNICODE
                code = text_[ i ];
#else
                if ( Base::StringUtil::checkMultiByteChar( &text_[ i ] ) )
                {
                    code = (unsigned char)text_[ i ] << 8 | (unsigned char)text_[ i + 1 ];
                    ++i;
                }
                else
                {
                    code = text_[ i ];
                }
#endif
                
                // 第3引数は他のところの GetGlyphOutline() と同じにしないとダメ。
                // 前までは GET_METRIX みたいなのにしてたけど、「ォ」とか「j」とかの
                // gm.gmptGlyphOrigin.x の値が異なってたため、正しい矩形を取得できなかった。
                ::GetGlyphOutline( hdc, code, GGO_GRAY4_BITMAP, &gm, 0, 0, &mat );
                rect_->w += gm.gmCellIncX;
            }

            ::SelectObject( hdc, old_font );
        }
        else
        {


            RECT rect = { 0, 0, 0, 0 };
            HDC hdc = GetDC( NULL );
		    HFONT old_font = (HFONT)SelectObject( hdc, mFontHandle );

		    // サイズを調べる場合は DT_CALCRECT を指定する (描画しない)
		    DrawTextA( hdc, text_, len_, &rect, DT_CALCRECT );  

		    SelectObject( hdc, old_font );
		    //ReleaseDC( NULL, hdc );

		    rect_->set( 0, 0, rect.right / mOutlineQuality, rect.bottom / mOutlineQuality );
        }

        return LN_OK;
    }


    //---------------------------------------------------------------------
    // ● フォント文字のビットマップ情報の取得開始
    //---------------------------------------------------------------------
    LNRESULT Win32Font::beginGetGlyphOutline()
    {
        _updateFont();


        mHDC = ::GetDC( NULL );
        mOldFont = (HFONT)::SelectObject( mHDC, mFontHandle );

        

        mBeganGetGlyphOutline = true;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● フォント文字のビットマップ情報の取得終了
    //---------------------------------------------------------------------
    LNRESULT Win32Font::endGetGlyphOutline()
    {
       


        ::SelectObject( mHDC, mOldFont );
        mHDC = NULL;
        mOldFont = NULL;

        mBeganGetGlyphOutline = false;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● フォント文字のビットマップ情報を取得する (char)
    //---------------------------------------------------------------------
    LNRESULT Win32Font::getGlyphOutline( const char* code_, LNGlyphOutlineData** data_ )
    {
        if ( !mBeganGetGlyphOutline )   { 
	return LN_ERR_INVALIDCALL; }
        if ( *code_ == '\0' )           { 
        	return LN_OK; }

        // マルチバイト文字の場合は 2、シングルバイト文字の場合は 1
        int m = ( Base::StringUtil::checkMultiByteChar( code_ ) ) ? 2 : 1;
        mGlyphOutlineData.CharIdxOffset = m;

        // エッジを使う場合
        if ( mGlyphType == LN_GT_EDGE_NOANTI || mGlyphType == LN_GT_EDGE_ANTI )
        {
            TEXTMETRICA tm;
            GLYPHMETRICS gm;
            MAT2 mat ={ {0,1}, {0,0}, {0,0}, {0,1} };
            UINT code = ( m == 2 ? (unsigned char)code_[ 0 ] << 8 | (unsigned char)code_[ 1 ] : (unsigned char)code_[ 0 ]);
            ::GetTextMetricsA( mMemDC, &tm );
            ::GetGlyphOutlineA( mMemDC, code, GGO_METRICS, &gm, 0, 0, &mat );

            // アウトラインの幅を考慮してぴったり左上に描画する場合の矩形を作る
            int linesize_ofs = mRealEdgeSize + 2;
            RECT char_region = {
                gm.gmptGlyphOrigin.x - linesize_ofs,
                tm.tmAscent - gm.gmptGlyphOrigin.y - linesize_ofs,
                gm.gmptGlyphOrigin.x + gm.gmBlackBoxX + linesize_ofs,
                tm.tmAscent - gm.gmptGlyphOrigin.y + gm.gmBlackBoxY + linesize_ofs
            };

            // 文字の幅と高さ
            RECT char_wh = {
                0, 0,
                ( gm.gmBlackBoxX + linesize_ofs * 2 + mOutlineQuality ) / mOutlineQuality * mOutlineQuality,
                ( gm.gmBlackBoxY + linesize_ofs * 2 + mOutlineQuality ) / mOutlineQuality * mOutlineQuality
            };


            // 背景は青(アルファ値とする)で塗りつぶす
            RECT rc;
            rc.left   = 0;
            rc.top    = 0;
            rc.right  = mDIBBitmapWidth;
            rc.bottom = mDIBBitmapHeight;
            ::FillRect( mMemDC, &rc, mOutlineBGBrush );

            // 描画する
            ::SetBkMode( mMemDC, TRANSPARENT );
            ::BeginPath( mMemDC );
            ::TextOutA( mMemDC, -char_region.left, -char_region.top, code_, m );
            ::EndPath( mMemDC );
            ::StrokeAndFillPath( mMemDC );

            
            u32 tw = char_wh.right / mOutlineQuality;
            u32 th = char_wh.bottom / mOutlineQuality;
            _makeGlyphFromDIBBitmap( tw, th );

            // 描画したときの矩形
            ::DrawTextA( mMemDC, code_, m, &rc, DT_CALCRECT );

            // 返すデータの作成
            mGlyphOutlineData.BitmapWidth   = mDIBBitmapWidth;
            mGlyphOutlineData.BitmapHeight  = mDIBBitmapHeight;
            mGlyphOutlineData.GlyphOffsetX  = gm.gmptGlyphOrigin.x / mOutlineQuality - linesize_ofs / mOutlineQuality;
            mGlyphOutlineData.GlyphOffsetY  = ( mTMAscent - gm.gmptGlyphOrigin.y ) / mOutlineQuality - linesize_ofs / mOutlineQuality;
            mGlyphOutlineData.GlyphWidth    = tw;
            mGlyphOutlineData.GlyphHeight   = th;
            mGlyphOutlineData.CellWidth     = rc.right / mOutlineQuality;
            
        }
        // 普通のアンチエイリアスフォントの場合
        else if ( mGlyphType == LN_GT_NOEDGE_ANTI )
        {
            // 半角または全角スペースの場合、GetGlyphOutline() から正しいデータを取得できないため自分で処理
            if ( ( *code_ == ' ' ) || ( code_[ 0 ] == -127 && code_[ 1 ] == 64 ) )
            {
                Geometry::Rect rc;
                if ( *code_ == ' ' )
                {
                    getTextSize( " ", 1, &rc );
                }
                else
                {
                    getTextSize( "  ", 2, &rc );
                }
                mGlyphOutlineData.BitmapWidth  = rc.w;
                mGlyphOutlineData.BitmapHeight = rc.h;
                mGlyphOutlineData.GlyphOffsetX = 0;
                mGlyphOutlineData.GlyphOffsetY = 0;
                mGlyphOutlineData.CellWidth    = rc.w;
                memset( mGlyphOutlineData.Bitmap, 0, mBitmapBufferSize );
            }
            // GetGlyphOutline() からデータを取得する
            else
            {
                // マルチバイト文字かどうかを判断して、GetGlyphOutline() に渡す正しい値を code に格納する
                u32 code;
                if ( m == 2 )
                {
                    code = (u8)code_[ 0 ] << 8 | (u8)code_[ 0 + 1 ];
                }
                else
                {
                    code = code_[ 0 ];
                }

                const MAT2 mat = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };

                GLYPHMETRICS gm;
                DWORD r = ::GetGlyphOutlineA(
                    mHDC, code, GGO_GRAY4_BITMAP, &gm, mBitmapBufferSize,
                    mGlyphOutlineData.Bitmap, &mat );
                LN_SETERR_R_S_( ( r != GDI_ERROR ), LN_ERR_SYSTEM, "< Win32Font::getGlyphOutline >\nGetGlyphOutline\ncode:%u", code_ );

                mGlyphOutlineData.BitmapWidth  = gm.gmBlackBoxX + ( 4 - ( gm.gmBlackBoxX % 4 ) ) % 4;
                mGlyphOutlineData.BitmapHeight = gm.gmBlackBoxY;
                mGlyphOutlineData.GlyphOffsetX = gm.gmptGlyphOrigin.x;
                mGlyphOutlineData.GlyphOffsetY = mTMAscent - gm.gmptGlyphOrigin.y;
                mGlyphOutlineData.GlyphWidth   = mGlyphOutlineData.BitmapWidth;    // とりあえず
                mGlyphOutlineData.GlyphHeight  = mGlyphOutlineData.BitmapHeight;
                mGlyphOutlineData.CellWidth    = gm.gmCellIncX;
            }
        }
        // エッジなし・アンチエイリアスなし
        else if ( mGlyphType == LN_GT_NOEDGE_NOANTI )
        {
            TEXTMETRICA tm;
            GLYPHMETRICS gm;
            MAT2 mat ={ {0,1}, {0,0}, {0,0}, {0,1} };
            UINT code = ( m == 2 ? (unsigned char)code_[ 0 ] << 8 | (unsigned char)code_[ 1 ] : (unsigned char)code_[ 0 ]);
            ::GetTextMetricsA( mMemDC, &tm );
            ::GetGlyphOutlineA( mMemDC, code, GGO_METRICS, &gm, 0, 0, &mat );

            // 左上に描画する場合の矩形を作る
            RECT char_region = {
                gm.gmptGlyphOrigin.x,
                tm.tmAscent - gm.gmptGlyphOrigin.y,
                gm.gmptGlyphOrigin.x + gm.gmBlackBoxX,
                tm.tmAscent - gm.gmptGlyphOrigin.y + gm.gmBlackBoxY
            };

            // 文字の幅と高さ
            RECT char_wh = {
                0, 0,
                ( gm.gmBlackBoxX ),
                ( gm.gmBlackBoxY )
            };

            // 背景は青(アルファ値とする)で塗りつぶす
            RECT rc;
            rc.left   = 0;
            rc.top    = 0;
            rc.right  = mDIBBitmapWidth;
            rc.bottom = mDIBBitmapHeight;
            ::FillRect( mMemDC, &rc, mOutlineBGBrush );

            // 描画する
            ::SetBkMode( mMemDC, TRANSPARENT );
            //::BeginPath( mMemDC );
            ::TextOutA( mMemDC, -char_region.left, -char_region.top, code_, m );
            //::EndPath( mMemDC );
            //::StrokeAndFillPath( mMemDC );

            
            u32 tw = char_wh.right;
            u32 th = char_wh.bottom;
            _makeGlyphFromDIBBitmap( tw, th );

            // 描画したときの矩形
            ::DrawTextA( mMemDC, code_, m, &rc, DT_CALCRECT );

            // 返すデータの作成
            mGlyphOutlineData.BitmapWidth   = mDIBBitmapWidth;
            mGlyphOutlineData.BitmapHeight  = mDIBBitmapHeight;
            mGlyphOutlineData.GlyphOffsetX  = gm.gmptGlyphOrigin.x;
            mGlyphOutlineData.GlyphOffsetY  = ( tm.tmAscent - gm.gmptGlyphOrigin.y );
            mGlyphOutlineData.GlyphWidth    = tw;
            mGlyphOutlineData.GlyphHeight   = th;
            mGlyphOutlineData.CellWidth     = rc.right;
        }

        *data_ = &mGlyphOutlineData;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● フォント文字のビットマップ情報を取得する (wchar_t)
    //---------------------------------------------------------------------
    LNRESULT Win32Font::getGlyphOutline( const wchar_t* code_, LNGlyphOutlineData** data_ )
    {
#ifdef LNOTE_UNICODE
        if ( !mBeganGetGlyphOutline )   { 
        	return LN_ERR_INVALIDCALL; }
        if ( *code_ == L'\0' )          { 
        	return LN_OK; }

        // wchar_t の場合、CharIdxOffset は常に 1
        mGlyphOutlineData.CharIdxOffset = 1;


        // アウトラインフォントの場合
        if ( isOutlineFont() )
        {
            // 背景は青(アルファ値とする)で塗りつぶす
            RECT rc;
            rc.left   = 0;
            rc.top    = 0;
            rc.right  = mDIBBitmapWidth;
            rc.bottom = mDIBBitmapHeight;
            ::FillRect( mMemDC, &rc, mOutlineBGBrush );

            // 描画する
            ::SetBkMode( mMemDC, TRANSPARENT );
            ::BeginPath( mMemDC );
            ::TextOutW( mMemDC, 0, 0, code_, 1 );
            ::EndPath( mMemDC );
            ::StrokeAndFillPath( mMemDC );

            //_makeGlyphFromDIBBitmap();

            // 描画したときの矩形
            ::DrawTextW( mMemDC, code_, 1, &rc, DT_CALCRECT );

            // 返すデータの作成
            mGlyphOutlineData.BitmapWidth   = mDIBBitmapWidth;
            mGlyphOutlineData.BitmapHeight  = mDIBBitmapHeight;
            mGlyphOutlineData.GlyphOffsetX  = 0;
            mGlyphOutlineData.GlyphOffsetY  = 0;
            mGlyphOutlineData.CellWidth     = rc.right;

        }
        // 普通のアンチエイリアスフォントの場合
        else
        {
            // 半角または全角スペースの場合、GetGlyphOutline() から正しいデータを取得できないため自分で処理
            if ( ( *code_ == L' ' ) || ( *code_ == L'　' ) )
            {
                Geometry::Rect rc;
                if ( *code_ == L' ' )
                {
                    getTextSize( L" ", 1, &rc );
                }
                else
                {
                    getTextSize( L"  ", 1, &rc );
                }
                mGlyphOutlineData.BitmapWidth  = rc.width;
                mGlyphOutlineData.BitmapHeight = rc.height;
                mGlyphOutlineData.GlyphOffsetX = 0;
                mGlyphOutlineData.GlyphOffsetY = 0;
                mGlyphOutlineData.CellWidth    = rc.width;
                memset( mGlyphOutlineData.Bitmap, 0, mBitmapBufferSize );
            }
            // GetGlyphOutline() からデータを取得する
            else
            {
                const MAT2 mat = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };

                u32 code = (u32)*code_;

                GLYPHMETRICS gm;
                DWORD r = ::GetGlyphOutlineW(
                    mHDC, code, GGO_GRAY4_BITMAP, &gm, mBitmapBufferSize,
                    mGlyphOutlineData.Bitmap, &mat );
                LN_SETERR_R_S_( ( r != GDI_ERROR ), LN_ERR_SYSTEM, "< Win32Font::getGlyphOutline >\nGetGlyphOutline\ncode:%u", code_ );

                mGlyphOutlineData.BitmapWidth  = gm.gmBlackBoxX + ( 4 - ( gm.gmBlackBoxX % 4 ) ) % 4;
                mGlyphOutlineData.BitmapHeight = gm.gmBlackBoxY;
                mGlyphOutlineData.GlyphOffsetX = gm.gmptGlyphOrigin.x;
                mGlyphOutlineData.GlyphOffsetY = mTMAscent - gm.gmptGlyphOrigin.y;
                mGlyphOutlineData.CellWidth    = gm.gmCellIncX;
            }
        }

        *data_ = &mGlyphOutlineData;
#else
        LN_PRINT_NOT_IMPL_FUNCTION;
#endif
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● フォントのコピーを作成する
    //---------------------------------------------------------------------
    LNRESULT Win32Font::deepCopy( IFont** font_ )
    {
        LNRESULT lr;
        Win32Font* new_font = LN_NEW Win32Font();
        LN_CALL_R(
            new_font->initialize(
                getName(),
                getSize(),
                getColor(),
                isBold(),
                isItalic() ) );
        new_font->setEdgeColor( getEdgeColor() );
        new_font->setEdgeSize( getEdgeSize() );
        new_font->setAntiAlias( isAntiAlias() );

        *font_ = new_font;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● mDIBBitmapBuffer から mGlyphOutlineData.Bitmap に ARGB ビットマップを作成する
    //---------------------------------------------------------------------
    LNRESULT Win32Font::_makeGlyphFromDIBBitmap( u32 glyph_w_, u32 glyph_h_ )
    {
        u32* write_buffer = (u32*)mGlyphOutlineData.Bitmap;
        u32* write_pos;
        u32 bmp_pitch = ( mDIBBitmapWidth * 3 + 3 ) / 4 * 4; // mDIBBitmapBuffer の横方向のバイト数 (21 のときは 64 とか)
        u32 alph = 0;
        u32 edge = 0;
        u32 fill = 0;
        u32 a, r, g, b;
        u32 edge_color = mEdgeColor.to32Bit( LN_GRAPHICSAPI_DIRECTX9 );
        u32 fill_color = mColor.to32Bit( LN_GRAPHICSAPI_DIRECTX9 );

        #define LN_GET_A( c_ ) ( ( c_ & 0xff000000 ) >> 24 )
        #define LN_GET_R( c_ ) ( ( c_ & 0x00ff0000 ) >> 16 )
        #define LN_GET_G( c_ ) ( ( c_ & 0x0000ff00 ) >> 8 )
        #define LN_GET_B( c_ ) ( ( c_ & 0x000000ff ) )

        
        // エッジ・アンチエイリアス付き
        if ( mGlyphType == LN_GT_EDGE_ANTI )
        {
            u32 q2 = ( mOutlineQuality * mOutlineQuality );

            for ( u32 y = 0; y < glyph_h_; ++y )
            {
                for ( u32 x = 0; x < glyph_w_; ++x )
                {
                    write_pos = &write_buffer[ x + mDIBBitmapWidth * y ];

                    // quality 倍率分点を平均化
                    alph = edge = fill = 0;
                    for ( u32 i = 0; i < mOutlineQuality; ++i )
                    {
                        for ( u32 j = 0; j < mOutlineQuality; ++j )
                        {
                            u32 jjj = (y * mOutlineQuality + i) * bmp_pitch + (x * mOutlineQuality + j);
                            //_p((y * mOutlineQuality + i) * bmp_pitch + (x * mOutlineQuality + j) * 3);
                            alph += mDIBBitmapBuffer[ (y * mOutlineQuality + i) * bmp_pitch + (x * mOutlineQuality + j) * 3 + 0 ];
                            edge += mDIBBitmapBuffer[ (y * mOutlineQuality + i) * bmp_pitch + (x * mOutlineQuality + j) * 3 + 1 ];
                            fill += mDIBBitmapBuffer[ (y * mOutlineQuality + i) * bmp_pitch + (x * mOutlineQuality + j) * 3 + 2 ];
                        }
                    }
                    alph /= q2;
                    edge /= q2;
                    fill /= q2;

                    // 透過度がある場合はエッジ色を採用
                    // 不透明の場合はブレンド色を採用
                    a = 0xff - alph;
                    if ( a < 0xff )
                    {
                        a = ( a * LN_GET_A( edge_color ) ) >> 8;
                        *write_pos = ( a << 24 ) | ( edge_color & 0x00ffffff );
                    }
                    else
                    {
                        // 不透明
                        r = ( (LN_GET_R( edge_color ) * edge) >> 8) + ( (LN_GET_R( fill_color ) * fill) >> 8 );
                        g = ( (LN_GET_G( edge_color ) * edge) >> 8) + ( (LN_GET_G( fill_color ) * fill) >> 8 );
                        b = ( (LN_GET_B( edge_color ) * edge) >> 8) + ( (LN_GET_B( fill_color ) * fill) >> 8 );
                        a = ( (LN_GET_A( edge_color ) * edge) >> 8) + ( (LN_GET_A( fill_color ) * fill) >> 8 );
                        *write_pos = a << 24 | r << 16 | g << 8 | b;
                    }
                }
            }
        }
        // エッジ付き・アンチエイリアスなし
        else if ( mGlyphType == LN_GT_EDGE_NOANTI )
        {
            for ( u32 y = 0; y < glyph_h_; ++y )
            {
                for ( u32 x = 0; x < glyph_w_; ++x )
                {
                    write_pos = &write_buffer[ x + mDIBBitmapWidth * y ];
                    
                    alph = mDIBBitmapBuffer[ y * bmp_pitch + x * 3 + 0 ];
                    edge = mDIBBitmapBuffer[ y * bmp_pitch + x * 3 + 1 ];
                    fill = mDIBBitmapBuffer[ y * bmp_pitch + x * 3 + 2 ];

                    a = 0xff - alph;
                    if ( a < 0xff )
                    {
                        a = ( a * LN_GET_A( edge_color ) ) >> 8;
                        *write_pos = ( a << 24 ) | ( edge_color & 0x00ffffff );
                    }
                    else
                    {
                        r = ( (LN_GET_R( edge_color ) * edge) >> 8) + ( (LN_GET_R( fill_color ) * fill) >> 8 );
                        g = ( (LN_GET_G( edge_color ) * edge) >> 8) + ( (LN_GET_G( fill_color ) * fill) >> 8 );
                        b = ( (LN_GET_B( edge_color ) * edge) >> 8) + ( (LN_GET_B( fill_color ) * fill) >> 8 );
                        a = ( (LN_GET_A( edge_color ) * edge) >> 8) + ( (LN_GET_A( fill_color ) * fill) >> 8 );
                        *write_pos = a << 24 | r << 16 | g << 8 | b;
                    }
                }
            }
        }
        // エッジなし・アンチエイリアスなし
        else if ( mGlyphType == LN_GT_NOEDGE_NOANTI )
        {
            for ( u32 y = 0; y < glyph_h_; ++y )
            {
                for ( u32 x = 0; x < glyph_w_; ++x )
                {
                    write_pos = &write_buffer[ x + mDIBBitmapWidth * y ];
                    
                    alph = mDIBBitmapBuffer[ y * bmp_pitch + x * 3 + 0 ];

                    a = 0xff - alph;
                    a = ( a * LN_GET_A( fill_color ) ) >> 8;
                    *write_pos = ( a << 24 ) | ( fill_color & 0x00ffffff );
                }
            }
        }

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 現在の状態からフォントを更新する (再構築)
    //---------------------------------------------------------------------
    LNRESULT Win32Font::_updateFont()
    {
        if ( mNeedUpdate )
        {
            // リソース解放
            _dispose();

            // エッジを使う場合
            if ( mEdgeSize > 0 )
            {
                mGlyphType = ( mOutlineQuality > 1 ) ? LN_GT_EDGE_ANTI : LN_GT_EDGE_NOANTI;
                mRealEdgeSize = mEdgeSize * mOutlineQuality;

                // 実際に作成するフォントサイズは品質制度倍する
                mLogFont.lfHeight = mFontSize * mOutlineQuality;

                // フォント作成
                mFontHandle = ::CreateFontIndirect( &mLogFont );
                LN_SETERR_R_S_( ( mFontHandle ), LN_ERR_SYSTEM, "< Win32Font::_updateFont >\nCreateFontIndirect" );

                // 以降の設定のため、一度フォントをセットする
                HDC hdc = ::GetDC( NULL );
                HFONT old_font = (HFONT)::SelectObject( hdc, mFontHandle );

                // テキストメトリクス取得。getGlyphOutline() で使うので覚えておく
                TEXTMETRIC tm;
                ::GetTextMetrics( hdc, &tm );
                mTMAscent = tm.tmAscent;



                mMemDC = ::CreateCompatibleDC( NULL );

                int edge_ofs = mRealEdgeSize * 2 + mOutlineQuality;

                mDIBBitmapWidth  = ( mLogFont.lfHeight   + edge_ofs + mOutlineQuality - 1 ) / mOutlineQuality * mOutlineQuality;
                mDIBBitmapHeight = ( ( mLogFont.lfHeight + 1 ) + edge_ofs + mOutlineQuality - 1 ) / mOutlineQuality * mOutlineQuality;

                mBitmapBufferSize = mDIBBitmapWidth * mDIBBitmapHeight;

                BITMAPINFO bmp_info = {};
                bmp_info.bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
                bmp_info.bmiHeader.biWidth    = mDIBBitmapWidth;
                bmp_info.bmiHeader.biHeight   = -static_cast< LONG>( mDIBBitmapHeight );
                bmp_info.bmiHeader.biPlanes   = 1;
                bmp_info.bmiHeader.biBitCount = 24;
                mDIBBitmap      = ::CreateDIBSection( NULL, &bmp_info, DIB_RGB_COLORS, (void**)&mDIBBitmapBuffer, 0, 0 );
                
                mOutlinePen     = (HPEN)::CreatePen( PS_SOLID, mRealEdgeSize, RGB( 0, 255, 0 ) );
                mOutlineBrush   = (HBRUSH)::CreateSolidBrush( RGB( 255, 0, 0 ) );
                mOutlineBGBrush = (HBRUSH)::CreateSolidBrush( RGB( 0, 0, 255 ) );
                ::SelectObject( mMemDC, mOutlinePen );
                ::SelectObject( mMemDC, mOutlineBrush );
                ::SelectObject( mMemDC, mFontHandle );
                ::SelectObject( mMemDC, mDIBBitmap );

              
                mBitmapBufferSize = ( mDIBBitmapWidth * mDIBBitmapHeight ) * sizeof( u32 );
                mGlyphOutlineData.Bitmap = LN_NEW u8[ mBitmapBufferSize ];

                mGlyphOutlineData.BitmapIsARGB = true;
                ::SelectObject( hdc, old_font );
            }
            // 普通のアンチエイリアスの場合
            else if ( mOutlineQuality > 1 )
            {
                mGlyphType = LN_GT_NOEDGE_ANTI;

                // サイズはもとの通り
                mLogFont.lfHeight = mFontSize;

                // フォント作成
                mFontHandle = ::CreateFontIndirect( &mLogFont );
                LN_SETERR_R_S_( ( mFontHandle ), LN_ERR_SYSTEM, "< Win32Font::_updateFont >\nCreateFontIndirect" );

                // 以降の設定のため、一度フォントをセットする
                HDC hdc = ::GetDC( NULL );
                HFONT old_font = (HFONT)::SelectObject( hdc, mFontHandle );

                // テキストメトリクス取得。getGlyphOutline() で使うので覚えておく
                TEXTMETRIC tm;
                ::GetTextMetrics( hdc, &tm );
                mTMAscent = tm.tmAscent;

                mBitmapBufferSize = tm.tmMaxCharWidth * ( tm.tmHeight + 1 );
                mGlyphOutlineData.Bitmap = LN_NEW u8[ mBitmapBufferSize ];

                mGlyphOutlineData.BitmapIsARGB = false;
                ::SelectObject( hdc, old_font );
            }
            // エッジもアンチエイリアスも使わない
            else
            {
                mGlyphType = LN_GT_NOEDGE_NOANTI;

                mLogFont.lfHeight = mFontSize;

                mFontHandle = ::CreateFontIndirect( &mLogFont );
                LN_SETERR_R_S_( ( mFontHandle ), LN_ERR_SYSTEM, "< Win32Font::_updateFont >\nCreateFontIndirect" );

                HDC hdc = ::GetDC( NULL );
                HFONT old_font = (HFONT)::SelectObject( hdc, mFontHandle );


                mMemDC = ::CreateCompatibleDC( NULL );

                mDIBBitmapWidth  = mLogFont.lfHeight;
                mDIBBitmapHeight = mLogFont.lfHeight;
                mBitmapBufferSize = mDIBBitmapWidth * mDIBBitmapHeight;

                BITMAPINFO bmp_info = {};
                bmp_info.bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
                bmp_info.bmiHeader.biWidth    = mDIBBitmapWidth;
                bmp_info.bmiHeader.biHeight   = -static_cast< LONG>( mDIBBitmapHeight );
                bmp_info.bmiHeader.biPlanes   = 1;
                bmp_info.bmiHeader.biBitCount = 24;
                mDIBBitmap      = ::CreateDIBSection( NULL, &bmp_info, DIB_RGB_COLORS, (void**)&mDIBBitmapBuffer, 0, 0 );
                
                mOutlineBrush   = (HBRUSH)::CreateSolidBrush( RGB( 255, 0, 0 ) );
                mOutlineBGBrush = (HBRUSH)::CreateSolidBrush( RGB( 0, 0, 255 ) );
                ::SelectObject( mMemDC, mOutlineBrush );
                ::SelectObject( mMemDC, mFontHandle );
                ::SelectObject( mMemDC, mDIBBitmap );

                mBitmapBufferSize = ( mDIBBitmapWidth * mDIBBitmapHeight ) * sizeof( u32 );
                mGlyphOutlineData.Bitmap = LN_NEW u8[ mBitmapBufferSize ];


                mGlyphOutlineData.BitmapIsARGB = true;
                ::SelectObject( hdc, old_font );
            }

            mNeedUpdate = false;
        }
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● リソース解放
    //---------------------------------------------------------------------
    void Win32Font::_dispose()
    {
        if ( mFontHandle )
        {
            ::DeleteObject( mFontHandle );
            mFontHandle = NULL;
        }
        if ( mMemDC )
        {
            ::ReleaseDC( NULL, mMemDC );
            mMemDC = NULL;
        }
        if ( mDIBBitmap )
        {
            ::DeleteObject( mDIBBitmap );
            mDIBBitmap = NULL;
        }
        if ( mOutlinePen )
        {
            ::DeleteObject( mOutlinePen );
            mOutlinePen = NULL;
        }
        if ( mOutlineBrush )
        {
            ::DeleteObject( mOutlineBrush );
            mOutlineBrush = NULL;
        }
        if ( mOutlineBGBrush )
        {
            ::DeleteObject( mOutlineBGBrush );
            mOutlineBGBrush = NULL;
        }

        
        SAFE_DELETE_ARRAY( mGlyphOutlineData.Bitmap );
        mGlyphOutlineData.BitmapWidth  = 0;
        mGlyphOutlineData.BitmapHeight = 0;
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Win32
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================