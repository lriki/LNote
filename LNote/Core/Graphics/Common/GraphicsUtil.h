//=============================================================================
//【 GraphcisUtil 】
//-----------------------------------------------------------------------------
///**
//  @file       GraphcisUtil.h
//  @brief      GraphcisUtil
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
// ■ Util クラス
//-----------------------------------------------------------------------------
///**
//  @brief      グラフィック関係のユーティリティ
//
//  @note
//              ホントに正しいアルファブレンドの計算は、ゲームで使うにはちょっと
//              時間がかかりすぎる。そのため、ここでは転送元のアルファ値優先で
//              ブレンドを行う。(dest 64、src 128 の場合、dest 64、src 191 として計算する)
//*/
//=============================================================================
class Util
{
public:

    

    /// 頂点フォーマットから頂点のサイズを求める (element_.StreamIndex == stream_index_ のみカウント)
    static lnU32 getVertexSize( const LNVertexElemenst* element_, lnU32 stream_index_ );

    /// 頂点属性ひとつ分のバイト数の取得
    static lnU32 getVertexDeclTypeSize( LNDeclType type_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      ビットマップの転送を行う
    //
    //  @param[in]  x_      : 転送先の左上 X 座標
    //  @param[in]  y_      : 転送先の左上 Y 座標
    //  @param[in]  dest_   : 転送先ビットマップ
    //  @param[in]  dest_w_ : 転送先ビットマップ全体の幅
    //  @param[in]  dest_h_ : 転送先ビットマップ全体の高さ
    //  @param[in]  sx_     : 転送元領域の左上 X 座標
    //  @param[in]  sy_     : 転送元領域の左上 Y 座標
    //  @param[in]  sw_     : 転送元領域の幅
    //  @param[in]  sh_     : 転送元領域の高さ
    //  @param[in]  src_    : 転送元ビットマップ
    //  @param[in]  src_w_  : 転送元ビットマップ全体の幅
    //  @param[in]  src_h_  : 転送元ビットマップ全体の高さ
    //  @param[in]  alpha_  : アルファブレンドの有効フラグ
    //*/
    //---------------------------------------------------------------------
    static void bitblt(
        int x_,  int y_ , u32* dest_, int dest_w_, int dest_h_,
	    int sx_, int sy_, int sw_, int sh_, u32* src_ , int src_w_ , int src_h_,
        bool alpha_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      ビットマップの転送を行う (右下クリッピング付き)
    //
    //  @param[in]  x_      : 転送先の左上 X 座標
    //  @param[in]  y_      : 転送先の左上 Y 座標
    //  @param[in]  r_      : 転送先領域の右端
    //  @param[in]  b_      : 転送先領域の下端
    //  @param[in]  dest_   : 転送先ビットマップ
    //  @param[in]  dest_w_ : 転送先ビットマップ全体の幅
    //  @param[in]  dest_h_ : 転送先ビットマップ全体の高さ
    //  @param[in]  sx_     : 転送元領域の左上 X 座標
    //  @param[in]  sy_     : 転送元領域の左上 Y 座標
    //  @param[in]  sw_     : 転送元領域の幅
    //  @param[in]  sh_     : 転送元領域の高さ
    //  @param[in]  src_    : 転送元ビットマップ
    //  @param[in]  src_w_  : 転送元ビットマップ全体の幅
    //  @param[in]  src_h_  : 転送元ビットマップ全体の高さ
    //  @param[in]  alpha_  : アルファブレンドの有効フラグ
    //*/
    //---------------------------------------------------------------------
    static void bitblt(
        int x_,  int y_ , int  r_, int  b_, u32* dest_, int dest_w_, int dest_h_,
	    int sx_, int sy_, int sw_, int sh_, u32* src_ , int src_w_ , int src_h_,
        bool alpha_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      ビットマップの転送を行う (転送元ビットマップを u8 のアルファマスクとする)
    //
    //  @param[in]  x_       : 転送先の左上 X 座標
    //  @param[in]  y_       : 転送先の左上 Y 座標
    //  @param[in]  dest_    : 転送先ビットマップ
    //  @param[in]  dest_w_  : 転送先ビットマップ全体の幅
    //  @param[in]  dest_h_  : 転送先ビットマップ全体の高さ
    //  @param[in]  sx_      : 転送元領域の左上 X 座標
    //  @param[in]  sy_      : 転送元領域の左上 Y 座標
    //  @param[in]  sw_      : 転送元領域の幅
    //  @param[in]  sh_      : 転送元領域の高さ
    //  @param[in]  src_     : 転送元ビットマップ (u8 である点に注意。これは a_level_ を 100% とした不透明度)
    //  @param[in]  a_level_ : 転送元ビットマップのアルファレベル
    //  @param[in]  color_   : 転送する色 (ARGB で指定する。アルファ値は src_ の値とブレンドされる)
    //  @param[in]  src_w_   : 転送元ビットマップ全体の幅
    //  @param[in]  src_h_   : 転送元ビットマップ全体の高さ
    //  @param[in]  alpha_   : アルファブレンドの有効フラグ
    //
    //  @par
    //              アルファ値は転送先と転送元のものを加算します。
    //
    //  @note
    //              とりあえずフォント文字の描画のために用意したもの。
    //*/
    //---------------------------------------------------------------------
    static void bitbltAlphaByte(
        int x_,  int y_ , u32* dest_, int dest_w_, int dest_h_,
	    int sx_, int sy_, int sw_, int sh_, u8* src_, u8 a_level_, u32 color_, int src_w_ , int src_h_,
        bool alpha_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      ビットマップの転送を行う (bitbltAlphaByte() の右下クリッピング付き)
    //
    //  @param[in]  x_       : 転送先の左上 X 座標
    //  @param[in]  y_       : 転送先の左上 Y 座標
    //  @param[in]  r_       : 転送先領域の右端
    //  @param[in]  b_       : 転送先領域の下端
    //  @param[in]  dest_    : 転送先ビットマップ
    //  @param[in]  dest_w_  : 転送先ビットマップ全体の幅
    //  @param[in]  dest_h_  : 転送先ビットマップ全体の高さ
    //  @param[in]  sx_      : 転送元領域の左上 X 座標
    //  @param[in]  sy_      : 転送元領域の左上 Y 座標
    //  @param[in]  sw_      : 転送元領域の幅
    //  @param[in]  sh_      : 転送元領域の高さ
    //  @param[in]  src_     : 転送元ビットマップ (u8 である点に注意。これは a_level_ を 100% とした不透明度)
    //  @param[in]  a_level_ : 転送元ビットマップのアルファレベル
    //  @param[in]  color_   : 転送する色 (ARGB で指定する。アルファ値は src_ の値とブレンドされる)
    //  @param[in]  src_w_   : 転送元ビットマップ全体の幅
    //  @param[in]  src_h_   : 転送元ビットマップ全体の高さ
    //  @param[in]  alpha_   : アルファブレンドの有効フラグ
    //
    //  @par
    //              転送先領域の右下(r_, b_)を超える場合、その部分を転送しません。
    //*/
    //---------------------------------------------------------------------
    static void bitbltAlphaByte(
        int x_,  int y_ , int  r_, int  b_, u32* dest_, int dest_w_, int dest_h_,
	    int sx_, int sy_, int sw_, int sh_, u8* src_, u8 a_level_, u32 color_, int src_w_ , int src_h_,
        bool alpha_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      テクスチャフォーマット文字列を LNSurfaceFormat に変換する
    //
    //  @note
    //              MME シェーダ用。
    //              A8R8G8B8"、"FMT_A8R8G8B8"、"D3DFMT_A8R8G8B8"のいずれの書式でも指定できる。
    //              LN_FMT_UNKNOWN 以外の値に強制的に変換する。
    //*/
    //---------------------------------------------------------------------
    static LNSurfaceFormat convertFMTStringToLNFMT( const lnChar* fmt_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      テクスチャの作成
    //
    //  @par
    //              フォーマットは A8R8G8B8、ミップマップは作成しません。
    //*/
    //---------------------------------------------------------------------
    static LNRESULT createTexture( ITexture** texture_, IGraphicsDevice* device_, u32 width_, u32 height_ );

};

//=============================================================================
// ■ LNFXShaderSource
//-----------------------------------------------------------------------------
///**
//  @brief      .lnfx の分割クラス
//
//  @note
//              各タグを #ifdef ～ #endif に置換する。
//              各シェーダコンパイル時に、以下のマクロを定義すること。
//                  LN_HLSL 1
//                  LN_GLSL_VERT 1
//                  LN_GLSL_FLAG 1
//                  (LN_GLSL_META 1)
//*/
//=============================================================================
class LNFXShaderSource
{
public:

    static const char* HLSL_MACRO;
    static const char* GLSL_VERT_MACRO;
    static const char* GLSL_FLAG_MACRO;
    static const char* GLSL_META_MACRO;

	enum ShaderSystem
	{
		SHADER_HLSL = 0,
		SHADER_GLSL,
	};

public:

	LNFXShaderSource()
	{}

public:

	void initialize(const char* buf_, ShaderSystem system_);

    const char* getSourceData() const { return mSource.c_str(); }
    int         getSourceDataSize() const { return mSource.size(); }

    const char* getGLSLMetaDataSource() const { return mGLSLMetaDataSource.c_str(); }
    int         getGLSLMetaDataSourceSize() const { return mGLSLMetaDataSource.size(); }

private:

    void _splitMetaData();

private:

	std::string mSource;
	std::string mGLSLMetaDataSource;
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