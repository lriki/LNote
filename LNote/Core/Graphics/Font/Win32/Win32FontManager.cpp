//=============================================================================
//【 Win32FontManager 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../../File/Manager.h"
#include "Win32Font.h"
#include "Win32FontManager.h"

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
// ■ Win32FontManager クラス
//=============================================================================


    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    Win32FontManager::Win32FontManager()
        : mFileManager  ( NULL )
    {
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    //Win32FontManager::~Win32FontManager()
    //{
    //}

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    void Win32FontManager::initialize( File::Manager* file_manager_ )
    {
        mFileManager = file_manager_;
        FontManagerBase::initialize();
    }

    //---------------------------------------------------------------------
    // ● 終了処理
    //---------------------------------------------------------------------
    //void Win32FontManager::finalize()
    //{
    //    LN_SAFE_RELEASE( mDefaultFont );
    //}

    //---------------------------------------------------------------------
    // ● フォントファイルを追加する
    //---------------------------------------------------------------------
    void Win32FontManager::addFontFile( const lnChar* fontname_ )
    {
        File::IInFile* stream;
        mFileManager->createInFile( &stream, fontname_ );

        stream->load();

        //int size =  stream->getSize();
        //lnByte* buf = LN_NEW lnByte[ size ];
        //stream->read( buf, size );

        DWORD num;
        DESIGNVECTOR v;
        ZeroMemory( &v, sizeof( v ) );
        HANDLE hfont = ::AddFontMemResourceEx( stream->getData(), stream->getSize(), &v, &num );
        LN_THROW_SystemCall( hfont );
    }


    //---------------------------------------------------------------------
    // ● フォントの作成
    //---------------------------------------------------------------------
    IFont* Win32FontManager::createFont()
    {
        Win32Font* font = LN_NEW Win32Font();
        font->initialize( LN_DEFAULT_FONT_NAME, 22, LColor::GRAY, false, false );
        return font;
    }

    
    //LNRESULT Win32FontManager::createFont( IFont** obj_, const lnChar* fontname_ )
    //{
    //    LNRESULT lr;
    //    
    //    *obj_ = NULL;

    //    const lnChar* fontname = fontname_;
    //    u32 size;
    //    LColor color;
    //    bool bold;
    //    bool italic;

    //    // デフォルトのフォントがある場合はその設定を使う
    //    if ( mDefaultFont )
    //    {
    //        if ( !fontname ) { fontname = mDefaultFont->getName(); }
    //        size     = mDefaultFont->getSize();
    //        color    = mDefaultFont->getColor();
    //        bold     = mDefaultFont->isBold();
    //        italic   = mDefaultFont->isItalic();
    //    }
    //    // フォント名もデフォルトのフォントもない場合はデフォルトの設定
    //    else
    //    {
    //        if ( !fontname ) { fontname = LN_DEFAULT_FONT_NAME; }
    //        size     = 22;
    //        color    = LColor( 0.5f, 0.5f, 0.5f, 1.0f );
    //        bold     = false;
    //        italic   = false;
    //    }

    //    // フォント作成
    //    font = LN_NEW Win32Font();
    //    LN_CALL_R( font->initialize( fontname, size, color, bold, italic ) );

    //    *obj_ = font;
    //    return LN_OK;
    //}

 

    //---------------------------------------------------------------------
    // ● フォントの作成 (デフォルトの設定から作成する)
    //---------------------------------------------------------------------
    //LNRESULT Win32FontManager::createDefaultFont( IFont** obj_ )
    //{
    //    return createFont( obj_, (const char*)NULL );
    //}

    //---------------------------------------------------------------------
    // ● デフォルトのフォントをセットする
    //---------------------------------------------------------------------
    //LNRESULT Win32FontManager::setDefaultFont( IFont* font_ )
    //{
    //    LN_SAFE_RELEASE( mDefaultFont );
    //    mDefaultFont = dynamic_cast< Win32Font* >( font_ );
    //    LN_SAFE_ADDREF( mDefaultFont );
    //    return LN_OK;
    //}

    //---------------------------------------------------------------------
    // ● デフォルトのフォントを取得する
    //---------------------------------------------------------------------
    //IFont* Win32FontManager::getDefaultFont()
    //{ 
    //    return mDefaultFont;
    //}


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