//=============================================================================
//【 Graphics 】
//-----------------------------------------------------------------------------
///**
//  @file       Graphics.h
//  @brief      Graphics
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Base/SharingKey.h"
#include "../Geometry/Vector2.h"
#include "../Geometry/Vector3.h"
#include "../Geometry/Vector4.h"
#include "../Geometry/Matrix.h"
#include "../Geometry/DrawingShapes.h"
#include "../System/Interface.h"
#include "../File/Interface.h"
#include "Common/Common.h"
#include "Common/RenderState.h"
#include "Common/VertexTypes.h"

#if defined(LNOTE_DIRECTX)
    struct IDirect3DDevice9;
#endif

#if defined(LNOTE_OPENGL)
    typedef unsigned int GLuint;
#endif

#define LN_DEFAULT_FONT_NAME    _T( "ＭＳ Ｐゴシック" )

/// マルチレンダリングターゲットの最大数
static const int LN_MAX_MULTI_RENDERTARGETS = 4;

/// サンプラステージの最大数
static const int LN_MAX_SAMPLER_STAGE_INDEX = 8;

//-------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------

//#define LNEV_CANVAS_SIZE_CHANGED    1001    ///< キャンバスサイズ変更イベント

//-------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
class IGraphicsDevice;
class IResourceManager;
class IRenderer;
class ICanvas;
class IVertexBuffer;
class IIndexBuffer;
class ITexture;
class IFont;
class IShader;
class IShaderVariable;
class IShaderTechnique;
class IShaderPass;

class GeometryRenderer;
class SpriteRenderer;
class Image;

//=============================================================================
// ■ IGraphicsDevice クラス
//-----------------------------------------------------------------------------
///**
//  @brief      グラフィックデバイスのインターフェイス
//
//  @par
//              デバイスリセットは必ず
//*/
//=============================================================================
class IGraphicsDevice
    : public Base::Interface
{
public:

    /// 使用中の GraphicsAPI の取得
    virtual LNGraphicsAPI getGraphicsAPI() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      描画クラスの取得
    //*/
    //---------------------------------------------------------------------
    virtual IRenderer* getRenderer() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      デフォルトのバックバッファを示すキャンバスの取得
    //*/
    //---------------------------------------------------------------------
    virtual ICanvas* getDefaultCanvas() = 0;

    /// 頂点バッファの作成
    virtual LNRESULT createVertexBuffer( IVertexBuffer** obj_, LNVertexElemenst* elements_, u32 vertex_num_, const void* data_, bool is_dynamic_ = false ) = 0;

    /// インデックスバッファの作成
    virtual LNRESULT createIndexBuffer( IIndexBuffer** obj_, u32 index_num_, const u16* data_, bool is_dynamic_ = false, bool is_16bit_ = true ) = 0;

    // テクスチャの作成
    virtual LNRESULT createTexture( ITexture** obj_, u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ ) = 0;

    ///// テクスチャの作成 (入力ストリームから)
    //virtual LNRESULT createTexture( ITexture** obj_, File::IInStream* stream_, u32 color_key_ = 0, u32 levels_ = 1, LNSurfaceFormat format_ = LN_FMT_UNKNOWN, LSharingKey key_ = lnNullKey ) = 0;

    /// テクスチャの作成 (ファイルから)
    virtual void createTexture( ITexture** obj_, const lnChar* filename_, u32 color_key_ = 0, u32 levels_ = 1, LNSurfaceFormat format_ = LN_FMT_UNKNOWN, LSharingKey key_ = lnNullKey ) = 0;

    /// レンダーターゲットテクスチャの作成
    virtual LNRESULT createRenderTarget( ITexture** obj_, u32 width_, u32 height_, u32 levels_ = 1, LNSurfaceFormat format_ = LN_FMT_A8R8G8B8 ) = 0;

    /// 深度バッファの作成
    virtual LNRESULT createDepthBuffer( ITexture** obj_, u32 width_, u32 height_, LNSurfaceFormat format_ ) = 0;

    ///// シェーダの作成 (埋め込みリソースから作成するときはストリーム化するよりもこの方が手っ取り早いので一緒に公開)
    //virtual LNRESULT createShader( IShader** obj_, const void* data_, u32 size_, LSharingKey key_ = lnNullKey ) = 0;

    ///// シェーダの作成 (入力ストリームから)
    //virtual LNRESULT createShader( IShader** obj_, File::IInStream* stream_, LSharingKey key_ = lnNullKey ) = 0;

    /// シェーダの作成 (ファイルから)
    virtual void createShader( IShader** obj_, const lnChar* filename_, LSharingKey key_ = lnNullKey ) = 0;

    /// キャッシュのクリア
    virtual void clearCache( bool texture_, bool shader_ ) = 0;

    /// デバイスリセットの必要がある場合は true
    virtual bool isDeviceLost() = 0;

    /// デバイスリセット前処理
    virtual void onLostDevice() = 0;

    /// デバイスリセット後処理
    virtual void onResetDevice() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      デバイスリセット
    //
    //  @note
    //              基本的に内部用。
    //              デバイスリセットは必ず
    //              onLostDevice() → resetDevice() → onResetDevice()
    //              の順で行うこと。
    //              関数を分けているのは、ライブラリ外部でリセットを行う場合の対応。
    //              その場合は onLostDevice()、onResetDevice() を適切なタイミングで呼ぶ。
    //              (基本的にリセット処理は Graphics 以外のモジュールでもハンドリング
    //              しないとならないので、Framework で isDeviceLost() からチェックを行う必要がある)
    //              ライブラリ外部のデバイスを使用している場合は常に false を返す。
    //
    //              デバイスリセットは Canvas::present() で
    //*/
    //---------------------------------------------------------------------
    virtual void resetDevice() = 0;

#if defined(LNOTE_DIRECTX)

    /// IDirect3DDevice9 へのポインタの取得
    virtual IDirect3DDevice9* getDX9Device() = 0;

#endif

protected:

    virtual ~IGraphicsDevice() {};
};

//=============================================================================
// ■ IRenderer クラス
//-----------------------------------------------------------------------------
///**
//  @brief     描画とそのステート管理を行うインターフェイス
//*/
//=============================================================================
class IRenderer
    : public Base::Interface
{
public:

    /// レンダリングステートの設定
    virtual LNRESULT setRenderState( const LRenderState& state_ ) = 0;

    /// レンダリングターゲットの取得 (参照カウントに変化はナシ)
    virtual ITexture* getRenderTarget( u32 index_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      レンダリングターゲットの設定
    //
    //  @par
    //              index_ 0 に設定した場合、設定されたテクスチャサイズで
    //              ビューポートをリセットします。<br>
    //              index_ が 0 の場合に texture_ に NULL を指定すると、
    //              デフォルトのバックバッファをレンダリングターゲットとして設定します。<br>
    //              
    //  @attention
    //              0 番に NULL が設定された場合、深度バッファもデフォルトのものが設定されます。
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT setRenderTarget( u32 index_, ITexture* texture_, bool reset_ = false ) = 0;

    /// 深度バッファの取得 (参照カウントに変化はナシ)
    virtual ITexture* getDepthBuffer() = 0;

    /// 深度バッファの設定
    virtual LNRESULT setDepthBuffer( ITexture* texture_ ) = 0;

    /// ビューポート矩形の取得
    virtual const Geometry::Rect& getViewport() const = 0;

	/// ビューポート矩形の設定
	virtual LNRESULT setViewport( const Geometry::Rect& viewport_rect_ ) = 0;

    /// シザー矩形の取得
    virtual const Geometry::Rect& getScissor() const = 0;

    /// シザー矩形の設定 (使用しない場合、要素がすべて 0 か、ビューポートと等しい rect_ を渡す)
	virtual LNRESULT setScissor( const Geometry::Rect& rect_ ) = 0;

    /// 頂点バッファの設定
	virtual LNRESULT setVertexBuffer( IVertexBuffer* vertex_buffer_, bool reset_ = false ) = 0;

    /// インデックスバッファの設定
	virtual LNRESULT setIndexBuffer( IIndexBuffer* index_buffer_, bool reset_ = false ) = 0;

    /// 設定されているカラーバッファ、深度バッファをクリアする
	virtual LNRESULT clear( bool target_, bool depth_, u32 color_, lnfloat z_ = 1.0f ) = 0;

    /// プリミティブ描画
	virtual LNRESULT drawPrimitive( LNPrimitiveType primitive_, u32 start_vertex_, u32 primitive_count_ ) = 0;

    /// プリミティブ描画 (インデックス付き。頂点、インデックスの両方のバッファのdynamic、static が一致している必要がある)
	virtual LNRESULT drawPrimitiveIndexed( LNPrimitiveType primitive_, u32 start_index_, u32 primitive_count_ ) = 0;

    /// シーン開始
	virtual LNRESULT beginScene() = 0;

	/// シーン終了
	virtual LNRESULT endScene() = 0;
   
protected:

    virtual ~IRenderer() {}
};

//=============================================================================
// ■ ICanvas クラス
//-----------------------------------------------------------------------------
///**
//  @brief     バックバッファと仮想スクリーンのインターフェイス
//*/
//=============================================================================
class ICanvas
    : public Base::Interface
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      キャンバスのサイズを取得する
    //*/
    //---------------------------------------------------------------------
    virtual const Geometry::Size& getSize() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      キャンバスサイズを変更する (setWindowSize() にも同時に設定される)
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT resize( u32 width_, u32 height_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      仮想バックバッファの取得
    //
    //  @note
    //              WPF アプリ等との連携のために用意したものだけど、
    //              DirectX9 で動作が怪しいので後でチェックしておく。
    //              [12/12/02] WPF 側はフォームホストにしたからコレは必要ない。
    //*/
    //---------------------------------------------------------------------
    virtual ITexture* getBackbuffer() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      仮想バックバッファへ描画できる状態にする
    //
    //  @par
    //              ゲーム画面を描画できる状態にします。
    //
    //              内部的には、getBackbuffer() で取得できるレンダリングターゲットを
    //              描画先として設定します。
    //              IRenderer::setRenderTarget( 0, NULL ); で設定される
    //              レンダリングターゲットとは異なります。
    //              (ただ、現状DirectX9はPresent()で転送先を指定しているので同じ)
    //
    //              OpenGL ではバックバッファを設定後、仮想スクリーン領域の
    //              ビューポートを設定します。
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT activate() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      バックバッファの内容をウィンドウへ転送する
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT present() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      描画先のウィンドウサイズを設定する
	//
	//	@note
	//				レターボックスサイズ計算に使用する値。
	//				バックバッファサイズは変更しないので注意。
    //*/
    //---------------------------------------------------------------------
    virtual void setWindowSize( const Geometry::Size& size_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      キャンバス→ウィンドウへ転送する際の座標変換行列の取得
    //
    //  @note
    //              GUI 等でマウス座標の位置を合わせるために用意したもの。
    //*/
    //---------------------------------------------------------------------
    virtual const LMatrix& getScreenTransform() const = 0;

#if defined(LNOTE_WINDOWS)
	/// present() の転送先ウィンドウのウィンドウハンドルの設定
	virtual void setTargetWindowHandle(HWND hwnd) = 0;
#endif

protected:

    virtual ~ICanvas() {}
};

//=============================================================================
// ■ IVertexBuffer クラス
//-----------------------------------------------------------------------------
///**
//  @brief     頂点バッファのインターフェイス
//*/
//=============================================================================
class IVertexBuffer
    : public Base::Interface
{
public:

    /// 頂点数の取得
	virtual u32 getVertexNum() const = 0;

	/// 頂点ひとつ分のサイズの取得
	virtual u32 getVertexStride() const = 0;

	/// 動的な頂点かを判定する	
	virtual bool isDynamic() const = 0;

	/// ロック
	virtual void* lock() = 0;

	/// アンロック
	virtual void unlock() = 0;

protected:

    virtual ~IVertexBuffer() {};
};

//=============================================================================
// ■ IIndexBuffer クラス
//-----------------------------------------------------------------------------
///**
//  @brief     インデックスバッファのインターフェイス
//*/
//=============================================================================
class IIndexBuffer
    : public Base::Interface
{
public:

    /// インデックス数の取得
    virtual u32 getIndexNum() const = 0;

    /// インデックスひとつ分のサイズの取得 (2 または 4)
	virtual lnU32 getIndexStride() const = 0;

	/// 動的なインデックスバッファかを判定する	
	virtual bool isDynamic() const = 0;

	/// ロック
	virtual void* lock() = 0;

	/// アンロック
	virtual void unlock() = 0;

protected:

    virtual ~IIndexBuffer() {};
};

//=============================================================================
// ■ ITexture クラス
//-----------------------------------------------------------------------------
///**
//  @brief     テクスチャのインターフェイス
//*/
//=============================================================================
class ITexture
    : public Base::Interface
{
public:

	/// テクスチャの種類の取得
	virtual LNTextureType getType() const = 0;

	/// サイズの取得
    virtual const LVector2& getSize() const = 0;

	/// 実際のサイズの取得
	virtual const LVector2& getRealSize( bool is_reciprocal_ = false ) const = 0;

	/// クリア
	virtual void clear( const LColor& color_ = LColor::TRANSPARENCY ) = 0;

	/// ロック
	virtual u32* lock() = 0;

	/// アンロック
	virtual void unlock() = 0;

    /// drawText() での文字列描画に使用するフォントの設定 (レンダリングターゲット、深度バッファの場合は無効)
    virtual void setFont( IFont* font_ ) = 0;

    /// drawText() での文字列描画に使用するフォントの取得
    virtual IFont* getFont() const = 0;

    /// 文字列の描画
    virtual LNRESULT drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_ = LN_TEXTALIGN_LEFT, int len_ = -1 ) = 0;

    /// テクスチャのブロック転送 (現在、普通のテクスチャ (RT、深度バッファではない) にのみ有効)
    virtual LNRESULT blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ ) = 0;

    /// ファイルから作成したときのファイル名
    virtual const lnChar* getFilename() const = 0;

public:

#if defined(LNOTE_DIRECTX)
    virtual void* getIDirect3DTexture9() = 0;
    virtual void* getIDirect3DSurface9() = 0;
#endif

protected:

    virtual ~ITexture() {}
};

//=============================================================================
// ■ IFont クラス
//-----------------------------------------------------------------------------
///**
//  @brief      フォントのインターフェイス
//*/
//=============================================================================
class IFont
    : public Base::Interface
{
public:

    /// フォント名の設定
	virtual LNRESULT setName( const lnChar* fontname_ ) = 0;

    /// フォント名の取得
	virtual const lnChar* getName() const = 0;

    /// フォントサイズの設定
	virtual void setSize( u32 size_ ) = 0;

    /// フォントサイズの取得
	virtual u32 getSize() const = 0;

	/// フォントカラーの設定
	virtual void setColor( const LColor& color_ ) = 0;

    /// フォントカラーの取得
	virtual const LColor& getColor() const = 0;

    /// エッジカラーの設定
	virtual void setEdgeColor( const LColor& color_ ) = 0;

    /// エッジカラーの取得
	virtual const LColor& getEdgeColor() const = 0;

    /// エッジの幅の設定 (0 でエッジ無効)
	virtual void setEdgeSize( u32 size_ ) = 0;

    /// エッジの幅の取得
	virtual u32 getEdgeSize() const = 0;

	/// 太文字の設定
	virtual void setBold( bool flag_ ) = 0;

    /// 太文字の判定
	virtual bool isBold() const = 0;

	/// イタリック体の設定
	virtual void setItalic( bool flag_ ) = 0;

    /// イタリック体の判定
	virtual bool isItalic() const = 0;

    /// アンチエイリアスの有効設定
	virtual void setAntiAlias( bool flag_ ) = 0;

    /// アンチエイリアスの有効判定
	virtual bool isAntiAlias() const = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      文字列を描画したときのサイズ (ピクセル単位) の取得
    //
    //  @param[in]  text_ : チェックする文字列
    //  @param[in]  len_  : 文字数 (-1 を指定すると終端までチェックする)
    //  @param[out] rect_ : 結果を格納する変数へのポインタ
    //
    //  @par
    //              結果の幅と高さは rect_ の width、heiht に格納されます。
    //              現在、rect_ の x、y 要素には常に 0 が格納されます。
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT getTextSize( const lnChar* text_, int len_, Geometry::Rect* rect_ ) = 0;


    //---------------------------------------------------------------------
    ///**
    //  @brief      フォント文字のビットマップ情報の取得開始
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT beginGetGlyphOutline() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      フォント文字のビットマップ情報の取得終了
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT endGetGlyphOutline() = 0;

  
    //---------------------------------------------------------------------
    ///**
    //  @brief      フォント文字のビットマップ情報を取得する (char)
    //
    //  @param[in]  code_ : データを取得する文字へのポインタ
    //  @param[out] data_ : グリフデータ構造体のポインタを格納する変数へのアドレス
    //
    //  @retval     LN_ERR_INVALIDCALL : beginGetGlyphOutline() が呼ばれていない
    //
    //  @par
    //              code_ にマルチバイト文字を渡す場合、以下の式で変換してから
    //              渡してください。<br>
    //              code = (u8)c[ 0 ] << 8 | (u8)c[ 1 ];<br>
    //              <br>
    //              この関数は beginGetGlyphOutline() ～ endGetGlyphOutline() の間で
    //              呼び出す必要があります。
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT getGlyphOutline( const char* code_, LNGlyphOutlineData** data_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      フォント文字のビットマップ情報を取得する (wchar_t)
    //
    //  @param[in]  code_ : データを取得する文字へのポインタ
    //  @param[out] data_ : グリフデータ構造体のポインタを格納する変数へのアドレス
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT getGlyphOutline( const wchar_t* code_, LNGlyphOutlineData** data_ ) = 0;
  
    /// フォントのコピーを作成する
    virtual LNRESULT deepCopy( IFont** font_ ) = 0;

protected:

    virtual ~IFont() {}
};

//=============================================================================
// ■ IShader クラス
//-----------------------------------------------------------------------------
///**
//  @brief      シェーダプログラムのインターフェイス
//*/
//=============================================================================
class IShader
    : public Base::Interface
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      名前によって変数を取得する
    //
    //  @param[in]  name_ : 変数の名前
    //
    //  @retval     NULL : 見つからなかった
    //*/
    //---------------------------------------------------------------------
	virtual IShaderVariable* getVariableByName( const char* name_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      インデックスによって変数を取得する
    //
    //  @param[in]  name_ : テクニックの名前
    //
    //  @retval     NULL : 見つからなかった
    //*/
    //---------------------------------------------------------------------
	virtual IShaderVariable* getVariableByIndex( u32 index_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief     セマンティクスによって変数を取得する
    //
    //  @param[in]  name_ : 変数の名前
    //
    //  @retval     NULL : 見つからなかった
    //*/
    //---------------------------------------------------------------------
	//virtual IShaderVariable* getVariableBySemantic( const char* name_ ) = 0;
    
    //---------------------------------------------------------------------
    ///**
    //  @brief      名前によってテクニックを取得する
    //
    //  @param[in]  name_ : テクニックの名前
    //
    //  @retval     NULL : 見つからなかった
    //
    //  @attention
    //              GLSL ではテクニックはひとつだけで常に同じものが返ります。
    //*/
    //---------------------------------------------------------------------
	virtual IShaderTechnique* getTechniqueByName( const char* name_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      インデックスによってテクニックを取得する
    //
    //  @param[in]  name_ : テクニックの名前
    //
    //  @retval     NULL : 見つからなかった
    //
    //  @attention
    //              GLSL ではテクニックはひとつだけで常に同じものが返ります。
    //*/
    //---------------------------------------------------------------------
	virtual IShaderTechnique* getTechniqueByIndex( lnU32 index_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      シェーダの情報を取得する
    //*/
    //---------------------------------------------------------------------
	virtual const LNShaderDesc& getDesc() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンパイル結果を取得する
    //*/
    //---------------------------------------------------------------------
	virtual LNShaderCompileResult getCompileResult() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンパイルエラー・警告の文字列を返す
    //*/
    //---------------------------------------------------------------------
	virtual const lnChar* getCompileErrors() = 0;

protected:

    virtual ~IShader() {}
};



//=============================================================================
// ■ IShaderVariable クラス
//-----------------------------------------------------------------------------
///**
//  @brief      シェーダプログラム内の変数のインターフェイス
//
//  @attention
//              行列配列の設定と取得は、シェーダ側の型が float4x4 以外の
//              大きさの場合(float4x3 float3x3 等)、作業領域が必要になります。
//              この領域確保は内部で処理されますが、あくまで補助的なものなので
//              あまり効率は良くありません。<br>
//              (具体的には、より大きい領域が必要になったときに再確保します。
//              以降はシェーダ自体が解放されるまで、その領域が残ったままになります。)<br>
//              4x4 以外の大きさの行列型は、スキニング用の姿勢行列が大量に必要な時など、
//              どうしても必要な時だけ使うようにしてください。<br>
//              <br>
//              また、アノテーションも IShaderVariable として取得しますが、
//              アノテーションへの値の設定、ネストされたアノテーションの取得はできません。
//              <br>
//              GLSL のアノテーションサポートは bool int float vec4 string です。
//*/
//=============================================================================
class IShaderVariable
    : public Base::Interface
{
public:

    /// ブール値の取得
	virtual LNRESULT getBool( bool* b_ ) = 0;

    /// ブール値の設定
	virtual LNRESULT setBool( bool b_ ) = 0;

    /// 整数値の取得
	virtual LNRESULT getInt( int* value_ ) = 0;

    /// 整数値の設定
	virtual LNRESULT setInt( int value_ ) = 0;

    /// 実数値の取得
    virtual LNRESULT getFloat( lnfloat* value_ ) = 0;

    /// 実数値の設定
    virtual LNRESULT setFloat( lnfloat value_ ) = 0;

    /// ベクトルの取得
	virtual LNRESULT getVector( LVector4* vec_ ) = 0;

    /// ベクトルの設定
	virtual LNRESULT setVector( const LVector4& vec_ ) = 0;

    /// ベクトル配列の取得
	virtual LNRESULT getVectorArray( LVector4* vec_, lnU32 count_ ) = 0;
    
    /// ベクトル配列の設定
	virtual LNRESULT setVectorArray( const LVector4* vec_, lnU32 count_ ) = 0;

    /// 行列の取得
    virtual LNRESULT getMatrix( LMatrix* matrix_ ) = 0;

    /// 行列の設定
    virtual LNRESULT setMatrix( const LMatrix& matrix_ ) = 0;

    /// 行列の配列の取得
	virtual LNRESULT getMatrixArray( LMatrix* matrices_, u32 count_ ) = 0;

    /// 行列の配列の設定
	virtual LNRESULT setMatrixArray( const LMatrix* matrices_, u32 count_ ) = 0;

    /// テクスチャの設定 (サンプラステートは更新しない)
	virtual LNRESULT setTexture( ITexture* texture_ ) = 0;

    /// テクスチャの設定
	virtual LNRESULT setTexture( ITexture* texture_, const LSamplerState& state_ ) = 0;

    /// 文字列の取得 (OpenGL 非対応)
	virtual LNRESULT getString( const char** str_ ) = 0;

    /// 文字列の設定 (OpenGL 非対応)
	virtual LNRESULT setString( const char* str_ ) = 0;


    /// 変数名の取得
	virtual const char* getName() const = 0;

    /// セマンティクス名の取得 (ない場合は NULL)
	virtual const char* getSemanticName() const = 0;

    /// 型情報の取得
    virtual const LNShaderVariableTypeDesc& getTypeDesc() const = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      インデックスによってアノテーションを取得する
    //
    //  @param[in]  index_ : 0 から始まるインデックス
    //
    //*/
    //---------------------------------------------------------------------
	virtual IShaderVariable* getAnnotationByIndex( lnU32 index_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      名前によってアノテーションを取得する
    //
    //  @param[in]  name_ : アノテーションの名前
    //*/
    //---------------------------------------------------------------------
	virtual IShaderVariable* getAnnotationByName( const char* name_ ) = 0;

protected:

    virtual ~IShaderVariable() {}
};

//=============================================================================
// ■ IShaderTechnique クラス
//-----------------------------------------------------------------------------
///**
//  @brief      シェーダプログラム内のテクニックのインターフェイス
//*/
//=============================================================================
class IShaderTechnique
    : public Base::Interface
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      名前によってアノテーションを取得する
    //
    //  @param[in]  name_ : アノテーションの名前
    //*/
    //---------------------------------------------------------------------
	virtual IShaderVariable* getAnnotationByName( const char* name_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      インデックスによってアノテーションを取得する
    //
    //  @param[in]  index_ : 0 から始まるインデックス
    //*/
    //---------------------------------------------------------------------
	virtual IShaderVariable* getAnnotationByIndex( lnU32 index_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      テクニックの名前を取得する
    //*/
    //---------------------------------------------------------------------
    virtual const char* getName() const = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      テクニック内のパスの数を取得する
    //*/
    //---------------------------------------------------------------------
	virtual u32 getPasses() const = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      名前によってパスを取得する
    //
    //  @param[in]  name_ : パスの名前
    //*/
    //---------------------------------------------------------------------
	virtual IShaderPass* getPassByName( const char* name_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      インデックスによってパスを取得する
    //
    //  @param[in]  index_ : 0 から始まるインデックス
    //*/
    //---------------------------------------------------------------------
	virtual IShaderPass* getPassByIndex( lnU32 index_ ) = 0;

protected:

    virtual ~IShaderTechnique() {}
};


//=============================================================================
// ■ IShaderPass クラス
//-----------------------------------------------------------------------------
///**
//  @brief      シェーダプログラム内のパスのインターフェイス
//*/
//=============================================================================
class IShaderPass
    : public Base::Interface
{
public:

    /// パスの名前を取得する
    virtual const lnChar* getName() const = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      名前によってアノテーションを取得する
    //
    //  @param[in]  name_ : アノテーションの名前
    //*/
    //---------------------------------------------------------------------
	virtual IShaderVariable* getAnnotationByName( const lnChar* name_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      インデックスによってアノテーションを取得する
    //
    //  @param[in]  index_ : 0 から始まるインデックス
    //*/
    //---------------------------------------------------------------------
	virtual IShaderVariable* getAnnotationByIndex( lnU32 index_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      パスの適用を開始する
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT begin() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      パスの適用を終了する
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT end() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      パスの実行中に変化したステートをデバイスに送信する
    //
    //  @pattention
    //              CgFX の場合、テクスチャの設定は送信されないので注意してください。
    //*/
    //---------------------------------------------------------------------
	virtual void commit() = 0;

protected:

    virtual ~IShaderPass() {}
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