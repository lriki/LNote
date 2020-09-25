//=============================================================================
//【 Resource 】
//=============================================================================

#pragma once

#include "../File/StreamObject.h"

namespace LNote
{
namespace Core
{
namespace Resource
{

/// 文字列リソース
namespace String
{
    static const lnChar* ERR_InvalidCallException   = _T( "メソッドの呼び出しが無効です。引数に不正な値が渡された可能性があります。" );
    static const lnChar* ERR_OutOfMemoryException   = _T( "操作を完了するのに十分なメモリ領域がありません。" );
    static const lnChar* ERR_FileNotFoundException  = _T( "ファイルが存在しないか、アクセスできないファイルです。" );
    static const lnChar* ERR_SystemCallException    = _T( "システム機能の呼び出しでエラーが発生しました。" );
    static const lnChar* ERR_FileFormatException    = _T( "対応していないファイルフォーマットです。" );
    static const lnChar* ERR_NotImplementedException= _T( "未実装、または未対応の機能が呼び出されました。" );
    static const lnChar* ERR_InitializedException   = _T( "オブジェクトは既に初期化されています。" );
    static const lnChar* ERR_FatalException         = _T( "致命的なエラーが発生しました。" );
    static const lnChar* ERR_ComException           = _T( "HRESULT にエラーが返されました。" );
    static const lnChar* ERR_OpenGLException        = _T( "OpenGL でエラーが返されました。" );

    static const lnChar* ERR_FileArchive1           = _T( "#アーカイブファイルを開くことができませんでした。" );
    static const lnChar* ERR_FileArchive2           = _T( "#不正なアーカイブファイルを開こうとしました。" );
    static const lnChar* ERR_FileArchive3           = _T( "#アーカイブにを開くためのキーが不正です。" );

    static const lnChar* ERR_SceneViewport          = _T( "指定されたシェーダはプリプロセスまたはポストプロセス用ではありません。" );
    static const lnChar* ERR_SceneShader_onSettingShader    = _T( "共有できないシェーダが設定されようとしました。" );
    static const lnChar* ERR_SceneShader_build1     = _T( "オブジェクトを描画するエフェクトに \"Draw=Buffer\" コマンドが見つかりました。" );
    static const lnChar* ERR_SceneShader_build2     = _T( "スクリーンバッファを描画するエフェクトに \"Draw=Geometry\" コマンドが見つかりました。" );
    static const lnChar* ERR_SceneShader_build3     = _T( "ポストエフェクト用のエフェクトに \"ScriptExternal=Color\" コマンドのないテクニックが見つかりました。" );
    static const lnChar* ERR_SceneShader_build4     = _T( "ポストエフェクト用ではないエフェクトに \"ScriptExternal=Color\" コマンドのあるテクニックが見つかりました。" );
}

/// シェーダプログラムリソース
//namespace Shader
//{
//    /// 基本形状描画用
//    extern const unsigned char GeometryRenderer[];
//    extern const unsigned int  GeometryRenderer_Size;
//
//    /// スプライト描画用
//    extern const unsigned char SpriteRenderer[];
//    extern const unsigned int  SpriteRenderer_Size;
//
//    /// SceneShader 2D 基本
//    extern const unsigned char SSBasic2D[];
//    extern const unsigned int  SSBasic2D_Size;
//
//    /// SceneShader 3D 基本
//    extern const unsigned char SSBasic3D[];
//    extern const unsigned int  SSBasic3D_Size;
//
//    /// SceneShader ライティングなし
//    extern const unsigned char SSNoLighting[];
//    extern const unsigned int  SSNoLighting_Size;
//
//    
//}

///// OpenGL シェーダ
//namespace GLSL
//{
//    /// 基本形状描画用
//    extern const unsigned char GeometryRenderer[];
//    extern const unsigned int  GeometryRenderer_Size;
//}

/// 画像リソース
namespace Image
{
   
}

/// 埋め込みリソースをストリームとして返すクラス
class HardCodeResourceArchive
    : public File::IArchive
{
public:

    HardCodeResourceArchive();

public:

    /// 入力ストリームの作成
    virtual File::IInStream* createStream( const lnChar* filename_ );
};


} // namespace Resource
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================