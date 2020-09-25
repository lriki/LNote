//=============================================================================
//【 Shader 】
//-----------------------------------------------------------------------------
///**
//  @file       Shader.h
//  @brief      Shader
//  @author     Riki
//*/
//-----------------------------------------------------------------------------
/*
    細かいメモ…
        アノテーションには行列、配列は使えず、ShaderVariable として扱われてるけど
        値のセットはできない。

        double は float と区別されている。MME シェーダで double4 とかしても認識しない。

        ベクトルは Rows が 1、Columns が次元
*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include <map>
#include "../../Base/Cache.h"
#include "../../Base/RefString.h"
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
namespace DirectX9
{
class GraphicsDevice;
class Shader;
class ShaderVariable;
class ShaderAnnotation;
class ShaderTechnique;
class ShaderPass;

typedef Base::CacheObjectManager< Shader >    ShaderCache;

typedef std::vector< ShaderVariable* >                  ShaderVariableArray;
typedef std::map< LRefTString, ShaderVariable* >        ShaderVariableMap;
typedef std::pair< LRefTString, ShaderVariable* >       ShaderVariablePair;

typedef std::vector< ShaderAnnotation* >                ShaderAnnotationArray;
typedef std::map< LRefTString, ShaderAnnotation* >      ShaderAnnotationMap;
typedef std::pair< LRefTString, ShaderAnnotation* >     ShaderAnnotationPair;

typedef std::vector< ShaderTechnique* >                 ShaderTechniqueArray;
typedef std::map< LRefTString, ShaderTechnique* >       ShaderTechniqueMap;
typedef std::pair< LRefTString, ShaderTechnique* >      ShaderTechniquePair;

typedef std::vector< ShaderPass* >                      ShaderPassArray;
typedef std::map< LRefTString, ShaderPass* >            ShaderPassMap;
typedef std::pair< LRefTString, ShaderPass* >           ShaderPassPair;

//=============================================================================
// ■ Shader クラス
//-----------------------------------------------------------------------------
///**
//  @brief      シェーダプログラムを扱うクラス
//*/
//=============================================================================
class Shader
    : public Base::ReferenceObject
    , public ShaderCache::CachedObject
    , public IShader
{
public:

    /// コンストラクタ
    Shader( GraphicsDevice* manager_ );

    /// デストラクタ
    virtual ~Shader();

    LN_CACHED_REFOBJ_METHODS;

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      初期化
    //
    //  @param[in]  code_   : データのあるバッファへのポインタ (NULL 終端文字列)
    //*/
    //---------------------------------------------------------------------
    LNRESULT initialize( const char* code_ );

public:

    /// ブール値の設定
	virtual LNRESULT setBool( const lnChar* name_, bool b_ );

    /// 整数値の設定
	virtual LNRESULT setInt( const lnChar* name_, int value_ );

    /// 実数値の設定
	virtual LNRESULT setFloat( const lnChar* name_, lnfloat value_ );

    /// ベクトルの設定
	virtual LNRESULT setVector( const lnChar* name_, const LVector4& vec_ );

    /// ベクトル配列の設定
	virtual LNRESULT setVectorArray( const lnChar* name_, const LVector4* vec_, lnU32 count_ );

    /// 行列の設定
	virtual LNRESULT setMatrix( const lnChar* name_, const LMatrix& matrix_ );

    /// 行列の配列の設定
	virtual LNRESULT setMatrixArray( const lnChar* name_, const LMatrix* matrices_, lnU32 count_ );

    /// テクスチャの設定
	virtual LNRESULT setTexture( const lnChar* name_, ITexture* texture_ );

    /// 名前によって変数を取得する
	virtual IShaderVariable* getVariableByName( const lnChar* name_ );

    /// インデックスによって変数を取得する
	virtual IShaderVariable* getVariableByIndex( u32 index_ );

    /// セマンティクスによって変数を取得する
	//virtual IShaderVariable* getVariableBySemantic( const char* name_ );
    
    /// 名前によってテクニックを取得する
	virtual IShaderTechnique* getTechniqueByName( const lnChar* name_ );

    /// インデックスによってテクニックを取得する
	virtual IShaderTechnique* getTechniqueByIndex( u32 index_ );

    /// シェーダの情報を取得する
    virtual const LNShaderDesc& getDesc() { return mDesc; }
    
    /// コンパイル結果を取得する
    virtual LNShaderCompileResult getCompileResult() { return mCompileResult; }

    /// コンパイルエラー・警告の文字列を返す
    virtual const lnChar* getCompileErrors() { return mCompileErrors.c_str(); }

public:

    GraphicsDevice* getGraphicsDevice() { return mManager; }

    /// デバイスリセット直前 GraphicsDevice から呼ばれる
    LNRESULT onLostDevice();

    /// デバイスリセット後に GraphicsDevice から呼ばれる
    LNRESULT onResetDevice();

    /// テクスチャ型一覧
    ShaderVariableArray& getTextureVariableArray() { return mTextureVariableArray; }

private:

    GraphicsDevice*         mManager;
    ID3DXEffect*            mDxEffect;
    LNShaderCompileResult   mCompileResult;
    LRefTString             mCompileErrors;
    LNShaderDesc            mDesc;
    ShaderTechniqueArray    mShaderTechniqueArray;
    ShaderTechniqueMap      mShaderTechniqueMap;
    ShaderVariableArray     mShaderVariableArray;
    ShaderVariableMap       mShaderVariableMap;
    ShaderVariableArray     mTextureVariableArray;  ///< (サンプラステート実装のための検索対象)
};

//=============================================================================
// ■ ShaderVariable クラス
//-----------------------------------------------------------------------------
///**
//  @brief      シェーダプログラム内の変数を扱うクラス
//*/
//=============================================================================
class ShaderVariable
    : public Base::ReferenceObject
    , public IShaderVariable
{
public:

    /// コンストラクタ
    ShaderVariable( Shader* shader_, ID3DXEffect* dx_effect_, D3DXHANDLE handle_ );

    /// デストラクタ
    virtual ~ShaderVariable();

    LN_REFOBJ_METHODS;

public:

    /// ブール値の取得
	virtual LNRESULT getBool( bool* b_ );

    /// ブール値の設定
	virtual LNRESULT setBool( bool b_ );

    /// 整数値の取得
	virtual LNRESULT getInt( int* value_ );

    /// 整数値の設定
	virtual LNRESULT setInt( int value_ );

    /// 実数値の取得
    virtual LNRESULT getFloat( lnfloat* value_ );

    /// 実数値の設定
    virtual LNRESULT setFloat( lnfloat value_ );

    /// ベクトルの取得
	virtual LNRESULT getVector( LVector4* vec_ );

    /// ベクトルの設定
	virtual LNRESULT setVector( const LVector4& vec_ );

    /// ベクトル配列の取得
	virtual LNRESULT getVectorArray( LVector4* vec_, u32 count_ );
    
    /// ベクトル配列の設定
	virtual LNRESULT setVectorArray( const LVector4* vec_, u32 count_ );

    /// 行列の取得
    virtual LNRESULT getMatrix( LMatrix* matrix_ );

    /// 行列の設定
    virtual LNRESULT setMatrix( const LMatrix& matrix_ );

    /// 行列の配列の取得
	virtual LNRESULT getMatrixArray( LMatrix* matrices_, u32 count_ );

    /// 行列の配列の設定
	virtual LNRESULT setMatrixArray( const LMatrix* matrices_, u32 count_ );

    /// テクスチャの取得
	//virtual LNRESULT getTexture( ITexture** texture_ );

    /// テクスチャの設定
	virtual LNRESULT setTexture( ITexture* texture_ );

    /// テクスチャの設定
	virtual LNRESULT setTexture( ITexture* texture_, const LSamplerState& state_ );

    /// 文字列の取得
	virtual LNRESULT getString( const char** str_ );

    /// 文字列の設定
	virtual LNRESULT setString( const char* str_ );

    /// 変数名の取得
	virtual const char* getName() const;

    /// セマンティクス名の取得
	virtual const char* getSemanticName() const;

    /// 型情報の取得
    virtual const LNShaderVariableTypeDesc& getTypeDesc() const;

    /// インデックスによってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByIndex( u32 index_ );

    /// 名前によってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByName( const char* name_ );

public:

    struct TextureParam
    {
        ITexture*       Texture;
        LSamplerState   SamplerState;
    };

public:

    /// テクスチャ型であるかを判定 (LN_SVT_TEXTURE[*D])
    bool isTextureType() const { return (mTextureParam != NULL); }

    /// setTexture() で設定された情報
    const TextureParam& getTextureParam() const { return *mTextureParam; }

private:

    Shader*                     mShader;
    ID3DXEffect*                mDxEffect;
    D3DXHANDLE                  mHandle;
    LRefTString                 mName;
    LRefTString                 mSemanticName;
    ShaderAnnotationArray       mAnnotationArray;
    ShaderAnnotationMap         mAnnotationMap;
    LNShaderVariableTypeDesc    mVariableTypeDesc;
    TextureParam*               mTextureParam;      ///< (テクスチャ型の場合のみ作成)
};

//=============================================================================
// ■ ShaderAnnotation クラス
//-----------------------------------------------------------------------------
///**
//  @brief      シェーダプログラムのアノテーションのクラス
//*/
//=============================================================================
class ShaderAnnotation
    : public Base::ReferenceObject
    , public IShaderVariable
{
public:

    /// コンストラクタ
    ShaderAnnotation( Shader* shader_, ID3DXEffect* dx_effect_, D3DXHANDLE handle_ );

    /// デストラクタ
    virtual ~ShaderAnnotation() {}

    LN_REFOBJ_METHODS;

public:

    /// ブール値の取得
	virtual LNRESULT getBool( bool* b_ );

    /// ブール値の設定
    virtual LNRESULT setBool( bool b_ ) { return LN_OK; }

    /// 整数値の取得
	virtual LNRESULT getInt( int* value_ );

    /// 整数値の設定
	virtual LNRESULT setInt( int value_ ) { return LN_OK; }

    /// 実数値の取得
    virtual LNRESULT getFloat( lnfloat* value_ );

    /// 実数値の設定
    virtual LNRESULT setFloat( lnfloat value_ ) { return LN_OK; }

    /// ベクトルの取得
	virtual LNRESULT getVector( LVector4* vec_ );

    /// ベクトルの設定
	virtual LNRESULT setVector( const LVector4& vec_ ) { return LN_OK; }

    /// ベクトル配列の取得
	virtual LNRESULT getVectorArray( LVector4* vec_, u32 count_ );
    
    /// ベクトル配列の設定
	virtual LNRESULT setVectorArray( const LVector4* vec_, u32 count_ ) { return LN_OK; }

    /// 行列の取得
    virtual LNRESULT getMatrix( LMatrix* matrix_ );

    /// 行列の設定
    virtual LNRESULT setMatrix( const LMatrix& matrix_ ) { return LN_OK; }

    /// 行列の配列の取得
	virtual LNRESULT getMatrixArray( LMatrix* matrices_, u32 count_ );

    /// 行列の配列の設定
	virtual LNRESULT setMatrixArray( const LMatrix* matrices_, u32 count_ ) { return LN_OK; }

    /// テクスチャの取得
    //virtual LNRESULT getTexture( ITexture** texture_ ) {}

    /// テクスチャの設定
    virtual LNRESULT setTexture( ITexture* texture_ ) { return LN_OK; }

    /// テクスチャの設定
	virtual LNRESULT setTexture( ITexture* texture_, const LSamplerState& state_ ) { return LN_OK; }

    /// 文字列の取得
    virtual LNRESULT getString( const lnChar** str_ );

    /// 文字列の設定
	virtual LNRESULT setString( const lnChar* str_ ) { return LN_OK; }

    /// 変数名の取得
    virtual const lnChar* getName() const { return mName.c_str(); }

    /// セマンティクス名の取得
    virtual const lnChar* getSemanticName() const { return NULL; }

    /// 型情報の取得
    virtual const LNShaderVariableTypeDesc& getTypeDesc() const { return mVariableTypeDesc; }

    /// インデックスによってアノテーションを取得する
    virtual IShaderVariable* getAnnotationByIndex( u32 index_ ) { return NULL; }

    /// 名前によってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByName( const char* name_ ) { return NULL; }

private:

    Shader*                     mShader;
    ID3DXEffect*                mDxEffect;
    D3DXHANDLE                  mHandle;
    LRefTString                 mName;
    LNShaderVariableTypeDesc    mVariableTypeDesc;
    LRefTString                 mString;            ///< String 型のアノテーションの場合の文字列値 (wchar_t 取得時の高速化用)
};

//=============================================================================
// ■ ShaderTechnique クラス
//-----------------------------------------------------------------------------
///**
//  @brief      シェーダプログラム内のテクニックを扱うクラス
//*/
//=============================================================================
class ShaderTechnique
    : public Base::ReferenceObject
    , public IShaderTechnique
{
public:

    /// コンストラクタ
    ShaderTechnique( Shader* shader_, ID3DXEffect* dx_effect_, D3DXHANDLE tech_ );

    /// デストラクタ
    virtual ~ShaderTechnique();

    LN_REFOBJ_METHODS;

public:

    /// 名前によってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByName( const lnChar* name_ );

    /// インデックスによってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByIndex( u32 index_ );

    /// テクニックの名前を取得する
    virtual const lnChar* getName() const { return mName.c_str(); }

    /// テクニック内のパスの数を取得する
	virtual u32 getPasses() const { return mShaderPassArray.size(); }

    /// 名前によってパスを取得する
    virtual IShaderPass* getPassByName( const lnChar* name_ );

    /// インデックスによってパスを取得する
	virtual IShaderPass* getPassByIndex( lnU32 index_ );

private:

    Shader*                 mShader;
    ID3DXEffect*            mDxEffect;
    D3DXHANDLE              mTechnique;
    u32                     mPasses;
    LRefTString             mName;
   
    ShaderAnnotationArray   mAnnotationArray;
    ShaderAnnotationMap     mAnnotationMap;

    ShaderPassArray         mShaderPassArray;
    ShaderPassMap           mShaderPassMap;
};

//=============================================================================
// ■ ShaderPass クラス
//-----------------------------------------------------------------------------
///**
//  @brief      シェーダプログラム内のパスを扱うクラス
//*/
//=============================================================================
class ShaderPass
    : public Base::ReferenceObject
    , public IShaderPass
{
public:

    /// コンストラクタ
    ShaderPass( Shader* shader_, ID3DXEffect* dx_effect_, D3DXHANDLE handle_, u32 pass_index_, D3DXHANDLE tech_ );

    /// デストラクタ
    virtual ~ShaderPass();

    LN_REFOBJ_METHODS;

public:

    /// パスの名前を取得する
    virtual const lnChar* getName() const { return mName.c_str(); }

    /// 名前によってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByName( const lnChar* name_ );

    /// インデックスによってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByIndex( u32 index_ );

    /// パスの適用を開始する
	virtual LNRESULT begin();

    /// パスの適用を終了する
	virtual LNRESULT end();

    /// パスの実行中に変化したステートをデバイスに送信する
	virtual void commit();

private:

    Shader*                 mShader;
    ID3DXEffect*            mDxEffect;
    D3DXHANDLE              mHandle;
    D3DXHANDLE              mTechnique;
    u32                     mPassIndex;
    LRefTString             mName;
    ShaderAnnotationArray   mAnnotationArray;
    ShaderAnnotationMap     mAnnotationMap;
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