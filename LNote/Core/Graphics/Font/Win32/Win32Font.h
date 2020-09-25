//=============================================================================
//【 Win32Font 】
//-----------------------------------------------------------------------------
///**
//  @file       Win32Font.h
//  @brief      Win32Font
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../Base/RefString.h"
#include "../../Interface.h"

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
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
class Win32Font
    : public Base::ReferenceObject
    , public IFont
{
public:

    /// コンストラクタ
    Win32Font();

    /// デストラクタ
    virtual ~Win32Font();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    LNRESULT initialize(
        const lnChar* fontname_,
        u32 size_,
        const LColor& color_,
        bool bold_,
        bool italic_ );

    /// フォント名の設定
	virtual LNRESULT setName( const lnChar* fontname_ );

    /// フォント名の取得
    virtual const lnChar* getName() const { return mFontName.c_str(); }

    /// フォントサイズの設定
    virtual void setSize( u32 size_ );

    /// フォントサイズの取得
    virtual u32 getSize() const { return mFontSize; }

	/// フォントカラーの設定
	virtual void setColor( const LColor& color_ );

    /// フォントカラーの取得
    virtual const LColor& getColor() const { return mColor; }

    /// エッジカラーの設定
    virtual void setEdgeColor( const LColor& color_ ) { mEdgeColor = color_; mNeedUpdate = true; }

    /// エッジカラーの取得
    virtual const LColor& getEdgeColor() const { return mEdgeColor; }

    /// エッジの幅の設定
	virtual void setEdgeSize( u32 size_ ) { mEdgeSize = size_; mNeedUpdate = true; }

    /// エッジの幅の取得
	virtual u32 getEdgeSize() const { return mEdgeSize; }

	/// 太文字の設定
	virtual void setBold( bool flag_ );

    /// 太文字の判定
    virtual bool isBold() const { return ( mLogFont.lfWeight == FW_BOLD ); }

	/// イタリック体の設定
	virtual void setItalic( bool flag_ );

    /// イタリック体の判定
    virtual bool isItalic() const { return ( mLogFont.lfItalic != FALSE ); }

    /// アンチエイリアスの有効設定
    virtual void setAntiAlias( bool flag_ ) { mOutlineQuality = ( flag_ ) ? FONT_ANTI_QUALITY : 1; mNeedUpdate = true; }

    /// アンチエイリアスの有効判定
    virtual bool isAntiAlias() const { return ( mOutlineQuality != 1 ); }

    /// 文字列を描画したときのサイズ (ピクセル単位) の取得 (char)
    virtual LNRESULT getTextSize( const lnChar* text_, int len_, Geometry::Rect* rect_ );

    /// フォント文字のビットマップ情報の取得開始
    virtual LNRESULT beginGetGlyphOutline();

    /// フォント文字のビットマップ情報の取得終了
    virtual LNRESULT endGetGlyphOutline();

    /// フォント文字のビットマップ情報を取得する (char)
    virtual LNRESULT getGlyphOutline( const char* code_, LNGlyphOutlineData** data_ );

    /// フォント文字のビットマップ情報を取得する (wchar_t)
    virtual LNRESULT getGlyphOutline( const wchar_t* code_, LNGlyphOutlineData** data_ );

    /// フォントのコピーを作成する
    virtual LNRESULT deepCopy( IFont** font_ );
   
public:

    /// フォントハンドルの取得
    HFONT getFontHandle() const { return mFontHandle; }

    bool isOutlineFont() const { return ( mEdgeSize > 0 ); }

private:

    /// mDIBBitmapBuffer から mGlyphOutlineData.Bitmap に ARGB ビットマップを作成する
    //      glyph_w_, glyph_h_ : 文字部分のサイズ
    LNRESULT _makeGlyphFromDIBBitmap( u32 glyph_w_, u32 glyph_h_ );

    /// 現在の状態からフォントを更新する (再構築)
    LNRESULT _updateFont();

    /// リソース解放
    void _dispose();

private:

    /// _updateFont() で作成されたフォントの種類
    enum GlyphType
    {
        LN_GT_NOEDGE_NOANTI = 0,
        LN_GT_EDGE_NOANTI,
        LN_GT_NOEDGE_ANTI,
        LN_GT_EDGE_ANTI
    };

private:

    static const int FONT_ANTI_QUALITY = 4;

    HFONT	                mFontHandle;	    ///< フォントハンドル
    LOGFONT	                mLogFont;           ///< フォントの属性
    LRefTString             mFontName;          ///< フォント名

    u32                     mFontSize;
    LColor                  mColor;             ///< フォントカラー
    
    GlyphType               mGlyphType;

    LColor                  mEdgeColor;         ///< エッジカラー
    u32                     mEdgeSize;
    u32                     mRealEdgeSize;
    u32                     mOutlineQuality;    ///< 最低 1
    HDC                     mMemDC;
    HBITMAP                 mDIBBitmap;
    u8*                     mDIBBitmapBuffer;
    u32                     mDIBBitmapWidth;
    u32                     mDIBBitmapHeight;
    HPEN                    mOutlinePen;
    HBRUSH                  mOutlineBrush;
    HBRUSH                  mOutlineBGBrush;    ///< 青ブラシ

  

    LNGlyphOutlineData      mGlyphOutlineData;  ///< グリフデータ
    u32                     mBitmapBufferSize;
    u32                     mTMAscent;
    HDC                     mHDC;
    

    HFONT                   mOldFont;
    
    
    bool                    mNeedUpdate;
    bool                    mBeganGetGlyphOutline;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace DirectX9
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================