//=============================================================================
//【 AndroidFont 】
//-----------------------------------------------------------------------------
///**
//  @file       AndroidFont.h
//  @brief      AndroidFont
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../Base/RefString.h"
#include "../../Common/Image/Image.h"
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
namespace Android
{

//=============================================================================
// ■ AndroidFont クラス
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
class AndroidFont
    : public Base::ReferenceObject
    , public IFont
{
public:

    AndroidFont();
    virtual ~AndroidFont();
    LN_REFOBJ_METHODS;

public:

    /// 初期化
    void initialize();

    /// フォント名の設定
    virtual LNRESULT setName( const lnChar* fontname_ ) { mFontName = fontname_; mRequestedUpdate = true; }

    /// フォント名の取得
    virtual const lnChar* getName() const { return mFontName.c_str(); }

    /// フォントサイズの設定
    virtual void setSize( u32 size_ ) { mFontSize = size_; mRequestedUpdate = true; }

    /// フォントサイズの取得
    virtual u32 getSize() const { return mFontSize; }

	/// フォントカラーの設定
    virtual void setColor( const LColor& color_ ) { mColor = color_; mRequestedUpdate = true; }

    /// フォントカラーの取得
    virtual const LColor& getColor() const { return mColor; }

    /// エッジカラーの設定
    virtual void setEdgeColor( const LColor& color_ ) { mEdgeColor = color_; mRequestedUpdate = true; }

    /// エッジカラーの取得
    virtual const LColor& getEdgeColor() const { return mEdgeColor; }

    /// エッジの幅の設定
	virtual void setEdgeSize( u32 size_ ) { mEdgeSize = size_; mRequestedUpdate = true; }

    /// エッジの幅の取得
	virtual u32 getEdgeSize() const { return mEdgeSize; }

	/// 太文字の設定
    virtual void setBold( bool flag_ ) { mBold = flag_; mRequestedUpdate = true; }

    /// 太文字の判定
    virtual bool isBold() const { return mBold; }

	/// イタリック体の設定
    virtual void setItalic( bool flag_ ) { mItalic = flag_; mRequestedUpdate = true; }

    /// イタリック体の判定
    virtual bool isItalic() const { return mItalic; }

    /// アンチエイリアスの有効設定
    virtual void setAntiAlias( bool flag_ ) { mAntiAlias = flag_; mRequestedUpdate = true; }

    /// アンチエイリアスの有効判定
    virtual bool isAntiAlias() const { return mAntiAlias; }

    /// 文字列を描画したときのサイズ (ピクセル単位) の取得 (char)
    virtual LNRESULT getTextSize( const lnChar* text_, int len_, LRect* rect_ );

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

    bool isOutlineFont() const { return ( mEdgeSize > 0 ); }

private:

    /// 現在の状態からフォントを更新する (再構築)
    LNRESULT _updateFont();

    /// リソース解放
    void _dispose();

private:
    
    Image               mImage;
    LNGlyphOutlineData  mGlyphOutlineData;  ///< グリフデータ
    int                 mFontID;
    LRefTString         mFontName;          ///< フォント名
    u32                 mFontSize;          ///< フォントサイズ
    LColor              mColor;             ///< フォントカラー
    LColor              mEdgeColor;         ///< エッジカラー
    u32                 mEdgeSize;
    bool                mBold;
    bool                mItalic;
    bool                mAntiAlias;
    bool                mRequestedUpdate;
};

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