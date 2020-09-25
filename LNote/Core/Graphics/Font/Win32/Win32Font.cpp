//=============================================================================
//�y Win32Font �z
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
// �� Win32Font �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
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
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    Win32Font::~Win32Font()
    {
        _dispose();
    }

    //---------------------------------------------------------------------
    // �� ������
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
    // �� �t�H���g���̐ݒ�
	//---------------------------------------------------------------------
    LNRESULT Win32Font::setName( const lnChar* fontname_ )
    {
        mFontName = fontname_;
        _tcscat( mLogFont.lfFaceName, mFontName.c_str() );
        mNeedUpdate = true;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �t�H���g�T�C�Y�̐ݒ�
    //---------------------------------------------------------------------
    void Win32Font::setSize( u32 size_ )
    {
        mFontSize = size_;
        mNeedUpdate = true;
    }

	//---------------------------------------------------------------------
    // �� �t�H���g�J���[�̐ݒ�
	//---------------------------------------------------------------------
    void Win32Font::setColor( const LColor& color_ )
    {
        mColor = color_;
        mNeedUpdate = true;
    }

   

	//---------------------------------------------------------------------
    // �� �������̐ݒ�
	//---------------------------------------------------------------------
    void Win32Font::setBold( bool flag_ )
    {
        mLogFont.lfWeight = ( ( flag_ ) ? FW_BOLD : FW_NORMAL );
        mNeedUpdate = true;
    }

	//---------------------------------------------------------------------
    // �� �C�^���b�N�̂̐ݒ�
	//---------------------------------------------------------------------
    void Win32Font::setItalic( bool flag_ )
    {
        mLogFont.lfItalic = ( ( flag_ ) ? TRUE : FALSE );
        mNeedUpdate = true;
    }

    //---------------------------------------------------------------------
    // �� �������`�悵���Ƃ��̃T�C�Y (�s�N�Z���P��) �̎擾 (char)
    //---------------------------------------------------------------------
    LNRESULT Win32Font::getTextSize( const lnChar* text_, int len_, Geometry::Rect* rect_ )
    {
        if ( len_ == 0 )
        {
            rect_->set( 0, 0, 0, 0 );
            return LN_OK;
        }

        _updateFont();

        // GetGlyphOutline ����擾����ꍇ
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
                
                // ��3�����͑��̂Ƃ���� GetGlyphOutline() �Ɠ����ɂ��Ȃ��ƃ_���B
                // �O�܂ł� GET_METRIX �݂����Ȃ̂ɂ��Ă����ǁA�u�H�v�Ƃ��uj�v�Ƃ���
                // gm.gmptGlyphOrigin.x �̒l���قȂ��Ă����߁A��������`���擾�ł��Ȃ������B
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

		    // �T�C�Y�𒲂ׂ�ꍇ�� DT_CALCRECT ���w�肷�� (�`�悵�Ȃ�)
		    DrawTextA( hdc, text_, len_, &rect, DT_CALCRECT );  

		    SelectObject( hdc, old_font );
		    //ReleaseDC( NULL, hdc );

		    rect_->set( 0, 0, rect.right / mOutlineQuality, rect.bottom / mOutlineQuality );
        }

        return LN_OK;
    }


    //---------------------------------------------------------------------
    // �� �t�H���g�����̃r�b�g�}�b�v���̎擾�J�n
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
    // �� �t�H���g�����̃r�b�g�}�b�v���̎擾�I��
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
    // �� �t�H���g�����̃r�b�g�}�b�v�����擾���� (char)
    //---------------------------------------------------------------------
    LNRESULT Win32Font::getGlyphOutline( const char* code_, LNGlyphOutlineData** data_ )
    {
        if ( !mBeganGetGlyphOutline )   { 
	return LN_ERR_INVALIDCALL; }
        if ( *code_ == '\0' )           { 
        	return LN_OK; }

        // �}���`�o�C�g�����̏ꍇ�� 2�A�V���O���o�C�g�����̏ꍇ�� 1
        int m = ( Base::StringUtil::checkMultiByteChar( code_ ) ) ? 2 : 1;
        mGlyphOutlineData.CharIdxOffset = m;

        // �G�b�W���g���ꍇ
        if ( mGlyphType == LN_GT_EDGE_NOANTI || mGlyphType == LN_GT_EDGE_ANTI )
        {
            TEXTMETRICA tm;
            GLYPHMETRICS gm;
            MAT2 mat ={ {0,1}, {0,0}, {0,0}, {0,1} };
            UINT code = ( m == 2 ? (unsigned char)code_[ 0 ] << 8 | (unsigned char)code_[ 1 ] : (unsigned char)code_[ 0 ]);
            ::GetTextMetricsA( mMemDC, &tm );
            ::GetGlyphOutlineA( mMemDC, code, GGO_METRICS, &gm, 0, 0, &mat );

            // �A�E�g���C���̕����l�����Ă҂����荶��ɕ`�悷��ꍇ�̋�`�����
            int linesize_ofs = mRealEdgeSize + 2;
            RECT char_region = {
                gm.gmptGlyphOrigin.x - linesize_ofs,
                tm.tmAscent - gm.gmptGlyphOrigin.y - linesize_ofs,
                gm.gmptGlyphOrigin.x + gm.gmBlackBoxX + linesize_ofs,
                tm.tmAscent - gm.gmptGlyphOrigin.y + gm.gmBlackBoxY + linesize_ofs
            };

            // �����̕��ƍ���
            RECT char_wh = {
                0, 0,
                ( gm.gmBlackBoxX + linesize_ofs * 2 + mOutlineQuality ) / mOutlineQuality * mOutlineQuality,
                ( gm.gmBlackBoxY + linesize_ofs * 2 + mOutlineQuality ) / mOutlineQuality * mOutlineQuality
            };


            // �w�i�͐�(�A���t�@�l�Ƃ���)�œh��Ԃ�
            RECT rc;
            rc.left   = 0;
            rc.top    = 0;
            rc.right  = mDIBBitmapWidth;
            rc.bottom = mDIBBitmapHeight;
            ::FillRect( mMemDC, &rc, mOutlineBGBrush );

            // �`�悷��
            ::SetBkMode( mMemDC, TRANSPARENT );
            ::BeginPath( mMemDC );
            ::TextOutA( mMemDC, -char_region.left, -char_region.top, code_, m );
            ::EndPath( mMemDC );
            ::StrokeAndFillPath( mMemDC );

            
            u32 tw = char_wh.right / mOutlineQuality;
            u32 th = char_wh.bottom / mOutlineQuality;
            _makeGlyphFromDIBBitmap( tw, th );

            // �`�悵���Ƃ��̋�`
            ::DrawTextA( mMemDC, code_, m, &rc, DT_CALCRECT );

            // �Ԃ��f�[�^�̍쐬
            mGlyphOutlineData.BitmapWidth   = mDIBBitmapWidth;
            mGlyphOutlineData.BitmapHeight  = mDIBBitmapHeight;
            mGlyphOutlineData.GlyphOffsetX  = gm.gmptGlyphOrigin.x / mOutlineQuality - linesize_ofs / mOutlineQuality;
            mGlyphOutlineData.GlyphOffsetY  = ( mTMAscent - gm.gmptGlyphOrigin.y ) / mOutlineQuality - linesize_ofs / mOutlineQuality;
            mGlyphOutlineData.GlyphWidth    = tw;
            mGlyphOutlineData.GlyphHeight   = th;
            mGlyphOutlineData.CellWidth     = rc.right / mOutlineQuality;
            
        }
        // ���ʂ̃A���`�G�C���A�X�t�H���g�̏ꍇ
        else if ( mGlyphType == LN_GT_NOEDGE_ANTI )
        {
            // ���p�܂��͑S�p�X�y�[�X�̏ꍇ�AGetGlyphOutline() ���琳�����f�[�^���擾�ł��Ȃ����ߎ����ŏ���
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
            // GetGlyphOutline() ����f�[�^���擾����
            else
            {
                // �}���`�o�C�g�������ǂ����𔻒f���āAGetGlyphOutline() �ɓn���������l�� code �Ɋi�[����
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
                mGlyphOutlineData.GlyphWidth   = mGlyphOutlineData.BitmapWidth;    // �Ƃ肠����
                mGlyphOutlineData.GlyphHeight  = mGlyphOutlineData.BitmapHeight;
                mGlyphOutlineData.CellWidth    = gm.gmCellIncX;
            }
        }
        // �G�b�W�Ȃ��E�A���`�G�C���A�X�Ȃ�
        else if ( mGlyphType == LN_GT_NOEDGE_NOANTI )
        {
            TEXTMETRICA tm;
            GLYPHMETRICS gm;
            MAT2 mat ={ {0,1}, {0,0}, {0,0}, {0,1} };
            UINT code = ( m == 2 ? (unsigned char)code_[ 0 ] << 8 | (unsigned char)code_[ 1 ] : (unsigned char)code_[ 0 ]);
            ::GetTextMetricsA( mMemDC, &tm );
            ::GetGlyphOutlineA( mMemDC, code, GGO_METRICS, &gm, 0, 0, &mat );

            // ����ɕ`�悷��ꍇ�̋�`�����
            RECT char_region = {
                gm.gmptGlyphOrigin.x,
                tm.tmAscent - gm.gmptGlyphOrigin.y,
                gm.gmptGlyphOrigin.x + gm.gmBlackBoxX,
                tm.tmAscent - gm.gmptGlyphOrigin.y + gm.gmBlackBoxY
            };

            // �����̕��ƍ���
            RECT char_wh = {
                0, 0,
                ( gm.gmBlackBoxX ),
                ( gm.gmBlackBoxY )
            };

            // �w�i�͐�(�A���t�@�l�Ƃ���)�œh��Ԃ�
            RECT rc;
            rc.left   = 0;
            rc.top    = 0;
            rc.right  = mDIBBitmapWidth;
            rc.bottom = mDIBBitmapHeight;
            ::FillRect( mMemDC, &rc, mOutlineBGBrush );

            // �`�悷��
            ::SetBkMode( mMemDC, TRANSPARENT );
            //::BeginPath( mMemDC );
            ::TextOutA( mMemDC, -char_region.left, -char_region.top, code_, m );
            //::EndPath( mMemDC );
            //::StrokeAndFillPath( mMemDC );

            
            u32 tw = char_wh.right;
            u32 th = char_wh.bottom;
            _makeGlyphFromDIBBitmap( tw, th );

            // �`�悵���Ƃ��̋�`
            ::DrawTextA( mMemDC, code_, m, &rc, DT_CALCRECT );

            // �Ԃ��f�[�^�̍쐬
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
    // �� �t�H���g�����̃r�b�g�}�b�v�����擾���� (wchar_t)
    //---------------------------------------------------------------------
    LNRESULT Win32Font::getGlyphOutline( const wchar_t* code_, LNGlyphOutlineData** data_ )
    {
#ifdef LNOTE_UNICODE
        if ( !mBeganGetGlyphOutline )   { 
        	return LN_ERR_INVALIDCALL; }
        if ( *code_ == L'\0' )          { 
        	return LN_OK; }

        // wchar_t �̏ꍇ�ACharIdxOffset �͏�� 1
        mGlyphOutlineData.CharIdxOffset = 1;


        // �A�E�g���C���t�H���g�̏ꍇ
        if ( isOutlineFont() )
        {
            // �w�i�͐�(�A���t�@�l�Ƃ���)�œh��Ԃ�
            RECT rc;
            rc.left   = 0;
            rc.top    = 0;
            rc.right  = mDIBBitmapWidth;
            rc.bottom = mDIBBitmapHeight;
            ::FillRect( mMemDC, &rc, mOutlineBGBrush );

            // �`�悷��
            ::SetBkMode( mMemDC, TRANSPARENT );
            ::BeginPath( mMemDC );
            ::TextOutW( mMemDC, 0, 0, code_, 1 );
            ::EndPath( mMemDC );
            ::StrokeAndFillPath( mMemDC );

            //_makeGlyphFromDIBBitmap();

            // �`�悵���Ƃ��̋�`
            ::DrawTextW( mMemDC, code_, 1, &rc, DT_CALCRECT );

            // �Ԃ��f�[�^�̍쐬
            mGlyphOutlineData.BitmapWidth   = mDIBBitmapWidth;
            mGlyphOutlineData.BitmapHeight  = mDIBBitmapHeight;
            mGlyphOutlineData.GlyphOffsetX  = 0;
            mGlyphOutlineData.GlyphOffsetY  = 0;
            mGlyphOutlineData.CellWidth     = rc.right;

        }
        // ���ʂ̃A���`�G�C���A�X�t�H���g�̏ꍇ
        else
        {
            // ���p�܂��͑S�p�X�y�[�X�̏ꍇ�AGetGlyphOutline() ���琳�����f�[�^���擾�ł��Ȃ����ߎ����ŏ���
            if ( ( *code_ == L' ' ) || ( *code_ == L'�@' ) )
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
            // GetGlyphOutline() ����f�[�^���擾����
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
    // �� �t�H���g�̃R�s�[���쐬����
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
    // �� mDIBBitmapBuffer ���� mGlyphOutlineData.Bitmap �� ARGB �r�b�g�}�b�v���쐬����
    //---------------------------------------------------------------------
    LNRESULT Win32Font::_makeGlyphFromDIBBitmap( u32 glyph_w_, u32 glyph_h_ )
    {
        u32* write_buffer = (u32*)mGlyphOutlineData.Bitmap;
        u32* write_pos;
        u32 bmp_pitch = ( mDIBBitmapWidth * 3 + 3 ) / 4 * 4; // mDIBBitmapBuffer �̉������̃o�C�g�� (21 �̂Ƃ��� 64 �Ƃ�)
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

        
        // �G�b�W�E�A���`�G�C���A�X�t��
        if ( mGlyphType == LN_GT_EDGE_ANTI )
        {
            u32 q2 = ( mOutlineQuality * mOutlineQuality );

            for ( u32 y = 0; y < glyph_h_; ++y )
            {
                for ( u32 x = 0; x < glyph_w_; ++x )
                {
                    write_pos = &write_buffer[ x + mDIBBitmapWidth * y ];

                    // quality �{�����_�𕽋ω�
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

                    // ���ߓx������ꍇ�̓G�b�W�F���̗p
                    // �s�����̏ꍇ�̓u�����h�F���̗p
                    a = 0xff - alph;
                    if ( a < 0xff )
                    {
                        a = ( a * LN_GET_A( edge_color ) ) >> 8;
                        *write_pos = ( a << 24 ) | ( edge_color & 0x00ffffff );
                    }
                    else
                    {
                        // �s����
                        r = ( (LN_GET_R( edge_color ) * edge) >> 8) + ( (LN_GET_R( fill_color ) * fill) >> 8 );
                        g = ( (LN_GET_G( edge_color ) * edge) >> 8) + ( (LN_GET_G( fill_color ) * fill) >> 8 );
                        b = ( (LN_GET_B( edge_color ) * edge) >> 8) + ( (LN_GET_B( fill_color ) * fill) >> 8 );
                        a = ( (LN_GET_A( edge_color ) * edge) >> 8) + ( (LN_GET_A( fill_color ) * fill) >> 8 );
                        *write_pos = a << 24 | r << 16 | g << 8 | b;
                    }
                }
            }
        }
        // �G�b�W�t���E�A���`�G�C���A�X�Ȃ�
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
        // �G�b�W�Ȃ��E�A���`�G�C���A�X�Ȃ�
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
    // �� ���݂̏�Ԃ���t�H���g���X�V���� (�č\�z)
    //---------------------------------------------------------------------
    LNRESULT Win32Font::_updateFont()
    {
        if ( mNeedUpdate )
        {
            // ���\�[�X���
            _dispose();

            // �G�b�W���g���ꍇ
            if ( mEdgeSize > 0 )
            {
                mGlyphType = ( mOutlineQuality > 1 ) ? LN_GT_EDGE_ANTI : LN_GT_EDGE_NOANTI;
                mRealEdgeSize = mEdgeSize * mOutlineQuality;

                // ���ۂɍ쐬����t�H���g�T�C�Y�͕i�����x�{����
                mLogFont.lfHeight = mFontSize * mOutlineQuality;

                // �t�H���g�쐬
                mFontHandle = ::CreateFontIndirect( &mLogFont );
                LN_SETERR_R_S_( ( mFontHandle ), LN_ERR_SYSTEM, "< Win32Font::_updateFont >\nCreateFontIndirect" );

                // �ȍ~�̐ݒ�̂��߁A��x�t�H���g���Z�b�g����
                HDC hdc = ::GetDC( NULL );
                HFONT old_font = (HFONT)::SelectObject( hdc, mFontHandle );

                // �e�L�X�g���g���N�X�擾�BgetGlyphOutline() �Ŏg���̂Ŋo���Ă���
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
            // ���ʂ̃A���`�G�C���A�X�̏ꍇ
            else if ( mOutlineQuality > 1 )
            {
                mGlyphType = LN_GT_NOEDGE_ANTI;

                // �T�C�Y�͂��Ƃ̒ʂ�
                mLogFont.lfHeight = mFontSize;

                // �t�H���g�쐬
                mFontHandle = ::CreateFontIndirect( &mLogFont );
                LN_SETERR_R_S_( ( mFontHandle ), LN_ERR_SYSTEM, "< Win32Font::_updateFont >\nCreateFontIndirect" );

                // �ȍ~�̐ݒ�̂��߁A��x�t�H���g���Z�b�g����
                HDC hdc = ::GetDC( NULL );
                HFONT old_font = (HFONT)::SelectObject( hdc, mFontHandle );

                // �e�L�X�g���g���N�X�擾�BgetGlyphOutline() �Ŏg���̂Ŋo���Ă���
                TEXTMETRIC tm;
                ::GetTextMetrics( hdc, &tm );
                mTMAscent = tm.tmAscent;

                mBitmapBufferSize = tm.tmMaxCharWidth * ( tm.tmHeight + 1 );
                mGlyphOutlineData.Bitmap = LN_NEW u8[ mBitmapBufferSize ];

                mGlyphOutlineData.BitmapIsARGB = false;
                ::SelectObject( hdc, old_font );
            }
            // �G�b�W���A���`�G�C���A�X���g��Ȃ�
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
    // �� ���\�[�X���
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