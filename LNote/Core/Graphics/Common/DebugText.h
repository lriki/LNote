//=============================================================================
//【 DebugText 】
//-----------------------------------------------------------------------------
///**
//  @file       DebugText.h
//  @brief      DebugText
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"

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
// ■ DebugText クラス
//-----------------------------------------------------------------------------
///**
//  @brief		デバッグ用の文字列を描画するためのクラス
//*/
//=============================================================================
class DebugText
    : public Base::ReferenceObject
{
public:
    DebugText();

protected:
    virtual ~DebugText();

public:

    /// 初期化
    void initialize( IGraphicsDevice* device_, SpriteRenderer* renderer_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      表示位置の設定
    //*/		
	//---------------------------------------------------------------------
	void setPosition( int x_, int y_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      文字色の設定
    //*/		
	//---------------------------------------------------------------------
    void setColor( const LColor& color_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      文字列を出力する
    //
    //  @par
    //              文字列は内部でバッファに溜められ、次回の draw() で
    //              全て描画されます。
    //*/		
	//---------------------------------------------------------------------
	LNRESULT printf( const char* format_, ... );

    DebugText& operator << ( const char* c_ );
	DebugText& operator << ( unsigned char v_ );
	DebugText& operator << ( short v_ );
	DebugText& operator << ( unsigned short v_ );
	DebugText& operator << ( int v_ );
	DebugText& operator << ( unsigned int v_ );
    DebugText& operator << ( long v_ );
	DebugText& operator << ( unsigned long v_ );
	DebugText& operator << ( float v_ );
	DebugText& operator << ( double v_ );
	DebugText& operator << ( bool v_ );
	DebugText& operator << ( void* p_ );

    /// (マルチスレッド対策。同期フェーズで呼ぶこと)
    void commitText() { mDrawingText = mText; mText = _T(""); }

    //---------------------------------------------------------------------
	///**
    //  @brief      文字列を描画する
    //*/		
	//---------------------------------------------------------------------
	void draw();

public:

    static const int TEMP_BUFFER_SIZE = 128;    ///< 一時バッファの大きさ

private:

    IGraphicsDevice*    mGraphicsDevice;
    SpriteRenderer*     mRenderer;
    ITexture*           mFontTexture;

    Geometry::Rect      mDrawArea;
    LColor              mColor;		                    ///< 文字の色

    char mTempBuffer[ TEMP_BUFFER_SIZE ];	    ///< 作業用の一時バッファ
    ln_std_tstring mText;					    ///< 表示する文字列 ( draw() 後にクリア )
    ln_std_tstring mDrawingText;			    ///< マルチスレッド対策
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