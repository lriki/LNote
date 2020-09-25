//=============================================================================
//【 Types 】
//-----------------------------------------------------------------------------
///**
//  @file       Types.h
//  @brief      Types
//  @author     Riki
//  @note
//              Interface に混ぜてあったけど RenderState を別のヘッダに
//              分けたので、Include の問題を解決するために型定義もこっちに移動。
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <string>
#include <vector>

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
typedef std::vector< lnU32 > LNEffectedBoneIndices;
typedef std::vector< ln_std_tstring > LNFontNameList;

/// グラフィックス API
enum LNGraphicsAPI
{
    LN_GRAPHICSAPI_DIRECTX9 = 0,    ///< DirectX9
    LN_GRAPHICSAPI_DIRECTX11,       ///< DirectX11
    LN_GRAPHICSAPI_OPENGL,          ///< OpenGL
};

/// テクスチャの種類
enum LNTextureType
{
    LN_TEXTURETYPE_UNKNOWN = 0,     
	LN_TEXTURETYPE_NORMAL,          ///< テクスチャ
    LN_TEXTURETYPE_RENDERTARGET,    ///< レンダーターゲット
    LN_TEXTURETYPE_DEPTH,           ///< 深度バッファ

	LN_TEXTURETYPE_INVALID = 99,
};

/// サーフェイスフォーマット
enum LNSurfaceFormat
{
    LN_FMT_UNKNOWN = 0,
    LN_FMT_A8R8G8B8,            ///< 32 ビットのアルファ付きフォーマット
    LN_FMT_X8R8G8B8,            ///< 32 ビットのアルファ無しフォーマット
    LN_FMT_A16B16G16R16F,       ///< 64 ビットの浮動小数点フォーマット
    LN_FMT_A32B32G32R32F,       ///< 128 ビットの浮動小数点フォーマット
    LN_FMT_D24S8,               ///< 32 ビットの Z バッファフォーマット
};

/// メンバのデータのサイズを定義するデータ型
enum LNDeclType
{
	LN_DECL_UNKNOWN = 0,
	LN_DECL_FLOAT1,
	LN_DECL_FLOAT2,
	LN_DECL_FLOAT3,
	LN_DECL_FLOAT4,
	LN_DECL_UBYTE4, 
	LN_DECL_COLOR4,     ///< 32ビット色コード (A8R8G8B8 の順。LN_DECL_UBYTE4 の場合、エンディアンは考慮されないので注意)
	LN_DECL_SHORT2,
	LN_DECL_SHORT4,

    LN_DECL_MAX
};

/// データの使用方法
enum LNDeclUsage
{
	LN_USAGE_UNKNOWN = 0,
	LN_USAGE_POSITION,
	LN_USAGE_NORMAL,
	LN_USAGE_COLOR,
	LN_USAGE_TEXCOORD,
	LN_USAGE_PSIZE,
    LN_USAGE_BLENDINDICES,
    LN_USAGE_BLENDWEIGHT,
};

#define LN_DECL_END() { 0, LN_DECL_UNKNOWN, LN_USAGE_UNKNOWN, 0 }

/// グラフィックスパイプラインへの入力頂点データを扱う構造体
struct LNVertexElemenst
{
    u32             StreamIndex;    ///< ストリーム番号
	LNDeclType	    Type;
	LNDeclUsage	    Usage;
	u8			    UsageIndex;     ///< OpenGL で許可しているのは TEXCOORD0～8、PSIZE15 のみ。それ以外は 0
};

/// シェーダ変数のオブジェクトタイプ
enum LNShaderVariableClass
{
    LN_SVC_UNKNOWN = 0,
    LN_SVC_SCALAR,      ///< [GL] 
    LN_SVC_VECTOR,      ///< [GL] 
    LN_SVC_MATRIX,      ///< [GL] 
    LN_SVC_STRUCT,
    LN_SVC_ARRAY,       ///< TODO: DX9 ではどう認識するのかチェックしておく。今は未使用
    LN_SVC_SAMPLER,     ///< [GL] 
    LN_SVC_OBJECT,      ///< DX9のテクスチャ、サンプラ型はこれ
    LN_SVC_BUFFER
};

/// シェーダ変数の型
enum LNShaderVariableType
{
    LN_SVT_UNKNOWN = 0,         ///<  [GL] 
    LN_SVT_ARRAY,
    LN_SVT_STRING,              ///< [DX9] [GL ※アノテーションのみ]
    LN_SVT_STRUCT,
    LN_SVT_TYPELESS_STRUCT,
    LN_SVT_BUFFER,
    LN_SVT_UNIFORMBUFFER,
    LN_SVT_ADDRESS,
    LN_SVT_PROGRAM_TYPE,
    LN_SVT_PIXELSHADER,         ///< [DX9]
    LN_SVT_VERTEXSHADER,        ///< [DX9]
    LN_SVT_TEXTURE,             ///< [DX9]
    LN_SVT_TEXTURE1D,           ///< [DX9]
    LN_SVT_TEXTURE2D,           ///< [DX9]
    LN_SVT_TEXTURE3D,           ///< [DX9]
    LN_SVT_TEXTURECUBE,         ///< [DX9]
    LN_SVT_SAMPLER,             ///< [DX9] 
    LN_SVT_SAMPLER1D,           ///< [DX9] [GL] DX9 に合わせて追加
    LN_SVT_SAMPLER2D,           ///< [DX9] [GL] 
    LN_SVT_SAMPLER3D,           ///< [DX9] [GL] 
    LN_SVT_SAMPLERCUBE,         ///< [DX9] [GL] 
    LN_SVT_ENUM,
    LN_SVT_VOID,                ///< [DX9] Cg にはないかも？
    LN_SVT_BOOL,                ///< [DX9] [GL]
    LN_SVT_CHAR,
    LN_SVT_DOUBLE,
    LN_SVT_FIXED,
    LN_SVT_FLOAT,               ///< [DX9] [GL]
    LN_SVT_HALF,
    LN_SVT_INT,                 ///< [DX9] [GL]
    LN_SVT_LONG,
    LN_SVT_SHORT,
    LN_SVT_UCHAR,
    LN_SVT_UINT,
    LN_SVT_ULONG,
    LN_SVT_USHORT,

    LN_SVT_TEXTURE_BEGIN = LN_SVT_TEXTURE,
    LN_SVT_TEXTURE_END   = LN_SVT_TEXTURECUBE,


    // Texture 系は Cg では認識しないかったかも。
    // とりあえず DirectX9 に合わせておく。
    // 変更外必要な場合は Shader_dx.cpp の
    // ShaderVariable コンストラクタをいじる感じで。
    //

    // D3DXPT_PIXELFRAGMENT、D3DXPT_VERTEXFRAGMENT はカット

    /*
    LN_SVT_VOID,      ///< void ポインタ
    LN_SVT_BOOL,
    LN_SVT_INT,
    LN_SVT_FLOAT,
    LN_SVT_STRING,
    LN_SVT_TEXTURE,
    LN_SVT_TEXTURE1D,
    LN_SVT_TEXTURE2D,
    LN_SVT_TEXTURE3D,
    LN_SVT_TEXTURECUBE,
    */


    /*
    LN_SVT_SAMPLER,
    LN_SVT_SAMPLER1D,
    LN_SVT_SAMPLER2D,
    LN_SVT_SAMPLER3D,
    LN_SVT_SAMPLERCUBE,
    LN_SVT_PIXELSHADER,
    LN_SVT_VERTEXSHADER,
    LN_SVT_PIXELFRAGMENT,
    LN_SVT_VERTEXFRAGMENT,
    LN_SVT_UNSUPPORTED,
    */

};

/// シェーダプログラムのコンパイル結果
enum LNShaderCompileResult
{
    LN_SHADERRESULT_SUCCESS = 0,   ///< 成功
    LN_SHADERRESULT_ERROR,         ///< エラーが発生している
    LN_SHADERRESULT_WARNING        ///< 警告が発生している (実行は可能)
};

/// シェーダプログラムの変数の型情報 (DX9 では型の名前はとれないみたい)
struct LNShaderVariableTypeDesc
{
    LNShaderVariableClass   Class;
    LNShaderVariableType    Type;
    lnU32                   Rows;       ///< 行の数 (行列型以外は 1)
    lnU32                   Columns;    ///< 列の数
    lnU32                   Elements;   ///< 配列の要素数 (配列でない場合は 0) TODO:OpenGLでは1…調査必要
    bool                    Shared;     ///< shared キーワードがついている
};

/// シェーダプログラムの情報
struct LNShaderDesc
{
    lnU32           ParameterNum;       ///< パラメータの数
    lnU32           TechniqueNum;       ///< テクニックの数
};


/// テキストアラインの記述
enum LNTextAlign
{
    LN_TEXTALIGN_LEFT = 0, ///< 左揃え
    LN_TEXTALIGN_CENTER,   ///< 中央揃え
    LN_TEXTALIGN_RIGHT     ///< 右揃え
};

/// フォントのグリフを表す構造体
struct LNGlyphOutlineData
{
    lnU8*   Bitmap;         ///< 1文字分のビットマップが格納されるバッファ
    lnU32   BitmapWidth;    ///< Bitmap の幅 (幅と高さは1文字分がぴっちり入るサイズ)
    lnU32   BitmapHeight;   ///< Bitmap の高さ

    lnU32   GlyphOffsetX;   ///< 描画領域の左上から、ビットマップを転送するべき座標へのXオフセット
    lnU32   GlyphOffsetY;   ///< 描画領域の左上から、ビットマップを転送するべき座標へのYオフセット
    lnU32   GlyphWidth;     ///< Bitmap 内の文字がある部分の幅
    lnU32   GlyphHeight;    ///< Bitmap 内の文字がある部分の高さ
    lnU32   CellWidth;      ///< 文字領域の幅 (次の文字を描画するときはこの値分だけピクセルを進めるようにする)

    lnU32   CharIdxOffset;  ///< IFont::getGlyphOutline() によって解析した文字がマルチバイト文字だった場合は 2、シングルバイトまたは Unicode(wchar_t)の場合は 1
    bool    BitmapIsARGB;   ///< true の場合は Bitmap を u32* にキャストして使うこと (true の場合、Bitmap は u8 ではなく u32 として確保されている)
};

/// プリミティブ
enum LNPrimitiveType
{
	LN_PRIM_TRIANGLELIST = 1,
	LN_PRIM_TRIANGLESTRIP,
	LN_PRIM_LINELIST,
    LN_PRIM_LINESTRIP,
	LN_PRIM_POINTLIST,
};

/// 合成方法
enum LNBlendMode
{
    LN_BLEND_NORMAL = 0,        ///< 通常 ( アルファ無視 )
    LN_BLEND_ALPHA,             ///< アルファブレンド
    LN_BLEND_ADD,               ///< 加算
    LN_BLEND_ADD_ALPHADISABLE,	///< 加算（アルファチャンネル無視）
    LN_BLEND_SUB,				///< 減算
    LN_BLEND_SUB_ALPHADISABLE,	///< 減算（アルファチャンネル無視）
    LN_BLEND_MUL,				///< 乗算
    LN_BLEND_SCREEN,			///< スクリーン
    LN_BLEND_REVERSE,           ///< リバース
};

/// カリング方法
enum LNCullingMode
{
    LN_CULL_FRONT = 0,          ///< 前面表示  (D3DCULL_CCW)
	LN_CULL_BACK,               ///< 裏面表示
	LN_CULL_DOUBLE              ///< 両面表示
};

/// カリング方法
/* 手右座標系スタイル (右手系の場合はD3DCULL_CWがデフォ)
enum LNCullingMode
{
    LN_CULL_BACK = 0,           ///< 裏面表示
    LN_CULL_FRONT,              ///< 前面表示
	LN_CULL_DOUBLE              ///< 両面表示
};
*/

/// 塗りつぶし方法
enum LNFillMode
{
	LN_FILL_SOLID = 0,	    ///< 面を塗りつぶす
	LN_FILL_WIRE,	        ///< ワイヤーフレーム
	LN_FILL_POINT,	        ///< 頂点
};

/// グラフィックスパイプラインへの入力頂点データを扱う構造体
struct LNGraphicsCache
{
    u32             StreamIndex;    ///< ストリーム番号
	LNDeclType	    Type;
	LNDeclUsage	    Usage;
	u8			    UsageIndex;
};

/// テクスチャフィルタ
enum LNTextureFilter
{
    LN_TEXFILTER_NONE = 0,
    LN_TEXFILTER_POINT,
    LN_TEXFILTER_LINEAR,
};

/// テクスチャアドレッシング
enum LNTextureAddress
{
    LN_TEXFILTER_REPEAT = 0,    ///< 繰り返し
    LN_TEXFILTER_CLAMP,         ///< 境界色が引き延ばされる
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

/// 3D 空間での基準方向を表す値
enum LNAxisAlignedDir
{
    LN_AADIR_X = 0,   ///< X+ 方向 (右向き)
	LN_AADIR_Y,       ///< Y+ 方向 (上向き)
	LN_AADIR_Z,       ///< Z+ 方向 (奥向き)
    LN_AADIR_RX,      ///< X- 方向 (左向き)
	LN_AADIR_RY,      ///< Y- 方向 (下向き)
	LN_AADIR_RZ,      ///< Z- 方向 (手前向き)
};

/// スプライトのソート方法
enum LNSpriteSortMode
{
    LN_SPRITESORT_NONE                = 0x00,   ///< ソートしない
    LN_SPRITESORT_DEPTH_BACK_TO_FRONT = 0x01,   ///< Z値が大きいものが先に描画されるようにソートする (アルファブレンド使用時)
    LN_SPRITESORT_DEPTH_FRONT_TO_BACK = 0x02,   ///< Z値が小さいものが先に描画されるようにソートする (LN_SPRITESORT_DEPTH_BACK_TO_FRONT と同時には使えない)
    LN_SPRITESORT_TEXTURE             = 0x04    ///< テクスチャを優先してソートする (同じテクスチャを持つスプライトが多数あるとき用。ただし、アルファブレンドが有効な場合は描画が不自然になることがある)
};

/// ビルボードの計算方法
enum LNBillboardType
{
    LN_BILLBOARD_NOT = 0,   ///< ビルボードの計算を行わない
    LN_BILLBOARD_FRONT,     ///< カメラ (ビュー行列) に対して正面を向く
    LN_BILLBOARD_ROT_Y,     ///< Y 軸回転のみ行う
};

/// メッシュの属性
struct LNMeshAttribute
{
    u32     MaterialIndex;  ///< 対応するマテリアル番号
    u32     StartIndex;     ///< 開始インデックス
    u32     PrimitiveNum;   ///< 描画プリミティブ数

    LNEffectedBoneIndices   BoneIndices;    ///< (いらないかも)GPU スキニングにおいて、この属性のサブセットが影響を受けるボーン番号の配列
};

/// グリッドを表示するかどうかの指定
enum LNGridVisible
{
    LN_GRID_XZ      = 0x01,     ///< XZ 平面に描画する
    LN_GRID_XY      = 0x02,     ///< XY 平面に描画する
    LN_GRID_YZ      = 0x04,     ///< YZ 平面に描画する
    LN_GRID_AXIS    = 0x08      ///< 軸を描画する
};

/// グリッドの描画範囲
enum LNGridPlaceRange
{
    LN_GRIDPLACE_PLUS = 0,      ///< +方向にだけ描画する
    LN_GRIDPLACE_MINUS,         ///< -方向にだけ描画する
    LN_GRIDPLACE_PLUS_MINUS     ///< +-両方に描画する
};

/// モデル生成の指定
enum LNModelFlag
{
    LN_SKINNINGTYPE_SOFTWARE = 1,   ///< ソフトウェアスキニング用のモデル (動的な頂点、インデックスバッファを作成)
    //LN_SKINNINGTYPE_HARDWARE,       ///< 頂点テクスチャを使う GPU スキニング用のモデル (静的な頂点、インデックスバッファを作成)
    //LN_SKINNINGTYPE_MATRIX_PALETTE    ///< 行列パレット (DirectX のサンプルのもの)

    LN_MODELFLAG_COLLISION = 0x02,			///< 物理演算のメッシュボディとして扱うメッシュを生成する
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================