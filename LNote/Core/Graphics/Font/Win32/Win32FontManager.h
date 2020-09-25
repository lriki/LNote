//=============================================================================
//【 Win32FontManager 】
//-----------------------------------------------------------------------------
///**
//  @file       Win32FontManager.h
//  @brief      Win32FontManager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../FontManagerBase.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File { class Manager; }

namespace Graphics
{
namespace Win32
{
class Win32Font;

//=============================================================================
// ■ Win32FontManager クラス
//-----------------------------------------------------------------------------
///**
//  @brief     フォントの管理クラス
//*/
//=============================================================================
class Win32FontManager
    : public FontManagerBase
{
public:

    /// コンストラクタ
    Win32FontManager();

    ///// デストラクタ
    //virtual ~Win32FontManager();

public:

    /// 初期化
    void initialize( File::Manager* file_manager_ );

    /// 終了処理
    //void finalize();

    /// フォントファイルを追加する (ttf)
    virtual void addFontFile( const lnChar* fontname_ );

    /// フォント作成
    virtual IFont* createFont();

    /// フォントファイルを追加する
    //LNRESULT addFontFile( File::IInStream* stream_ );

    ///// フォントの作成 (フォント名が NULL の場合はデフォルトのフォントが使われる)
    //LNRESULT createFont( IFont** obj_, const lnChar* fontname_ );

    ///// フォントの作成 (デフォルトの設定から作成する)
    //LNRESULT createDefaultFont( IFont** obj_ );

    ///// デフォルトのフォントをセットする (createFont() で作成したものを渡す必要がある。デフォルトは "ＭＳ Ｐゴシック"。createFont() でコピーして使われる)
    //LNRESULT setDefaultFont( IFont* font_ );

    ///// デフォルトのフォントを取得する
    //IFont* getDefaultFont();

private:

    File::Manager*  mFileManager;

    //IFont*  mDefaultFont;
};

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