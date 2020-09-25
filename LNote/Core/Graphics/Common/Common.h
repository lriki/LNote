//=============================================================================
//【 Common 】
//-----------------------------------------------------------------------------
///**
//  @file       Common.h
//  @brief      Common
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Types.h"
#include "../../Geometry/Vector4.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{ 
class ITexture;
class IShader;

//=============================================================================
// ■ Color クラス
//-----------------------------------------------------------------------------
///**
//  @brief      色のクラス
//
//  @par
//              特に指定がない限り、色は 0.0 ～ 1.0 の範囲の値で扱います。
//*/
//=============================================================================
class Color
{
public:

	lnfloat red;
	lnfloat green;
	lnfloat blue;
	lnfloat alpha;

public:

    static const Color TRANSPARENCY;         ///< 透明 (0.0, 0.0, 0.0, 0.0)
    static const Color TRANSPARENCY_WHITE;   ///< 白透明 (1.0, 1.0, 1.0, 0.0)
    static const Color WHITE;                ///< 白 (1.0, 1.0, 1.0, 1.0)
    static const Color RED;                  ///< 赤 (1.0, 0.0, 0.0, 1.0)
    static const Color GREEN;                ///< 緑 (0.0, 1.0, 0.0, 1.0)
    static const Color BLUE;                 ///< 青 (0.0, 0.0, 1.0, 1.0)
    static const Color GRAY;                 ///< 灰色 (0.5, 0.5, 0.5, 1.0)

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      デフォルトコンストラクタ
    //
    //  @par		
    //              色は黒( 0, 0, 0 ) 、アルファ値は 1.0 になります。
    //*/
    //---------------------------------------------------------------------
    Color();

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ
    //
    //  @par
    //              値が範囲外の場合は 0.0 ～ 1.0 の間にクリッピングされます。
    //*/
    //---------------------------------------------------------------------
    Color( lnfloat r_, lnfloat g_, lnfloat b_, lnfloat a_ = 1.0f );

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ
    //
    //  @par
    //              32 ビットの色コード (ARGB) から作成します。
    //*/
    //---------------------------------------------------------------------
    Color( u32 color_code_, LNGraphicsAPI api_ ) { setFrom32Bit( color_code_, api_ ); }

    /// コピー
    Color( const Color& c_ )
    {
        red   = c_.red;
        green = c_.green;
        blue  = c_.blue;
        alpha = c_.alpha;
    }

    /// コピー
    Color& operator = ( const Color& c_ )
    {
        red   = c_.red;
        green = c_.green;
        blue  = c_.blue;
        alpha = c_.alpha;
        return ( *this );
    }

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      新しい値の設定
    //
    //  @par
    //              値が範囲外の場合は 0.0 ～ 1.0 の間にクリッピングされます。
    //*/
    //---------------------------------------------------------------------
    void set( lnfloat r_, lnfloat g_, lnfloat b_, lnfloat a_ = 1.0f );

    //---------------------------------------------------------------------
    ///**
    //  @brief      32 ビットの色コードから色を作成
    //
    //  @note
    //              ARGB の順で各 8 ビット
    //*/
    //---------------------------------------------------------------------
    void setFrom32Bit( u32 code_, LNGraphicsAPI api_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      32 ビットの色コードに変換
    //*/
    //---------------------------------------------------------------------
    u32 to32Bit( LNGraphicsAPI api_ ) const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      Vector4 としての参照を返す
    //*/
    //---------------------------------------------------------------------
    const LVector4& toVector4() const { return *( (LVector4*)this ); }

    //---------------------------------------------------------------------
	///**
	//	@brief		要素の内容をコンソールに出力する
	// 
	//	@param[in]	str_ : ヘッダ文字列
	//*/
	//---------------------------------------------------------------------
	void dump( const char* str_ = NULL ) const;

    /// LVector4 からの設定
    Color& operator = ( const LVector4& v_ )
    {
        red   = v_.x;
        green = v_.y;
        blue  = v_.z;
        alpha = v_.w;
        return ( *this );
    }

    /// LVector4 との乗算
    //LVector4 operator * ( const LVector4& v_ )
    //{
    //    LVector4 vec(
    //        red   * v_.x,
    //        green * v_.y,
    //        blue  * v_.z,
    //        alpha * v_.w );
    //    return vec;
    //}

    /// LVector4 への型変換
    operator LVector4() { return *reinterpret_cast< LVector4* >( this ); }
    operator const LVector4() const { return *reinterpret_cast< const LVector4* >( this ); }

public:

    bool operator == ( const Color& color_ );
};

//=============================================================================
// ■ LColor32 クラス
//-----------------------------------------------------------------------------
///**
//  @brief      32 ビットカラーのクラス
//
//  @par
//              フォーマットは ARGB
//*/
//=============================================================================
class LColor32
{
public:

	lnU32 color;

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      デフォルトコンストラクタ
    //
    //  @par		
    //              色は黒 0xff000000 になります。
    //*/
    //---------------------------------------------------------------------
    LColor32();

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ
    //
    //  @par
    //              32 ビットの色コード (ARGB) から作成します。
    //*/
    //---------------------------------------------------------------------
    LColor32( u32 color_code_ ) { color = color_code_; }

public:

    void set( u32 color_code_ ) { color = color_code_; }

};

//=============================================================================
// ■ Tone クラス
//-----------------------------------------------------------------------------
///**
//  @brief		色調のクラス
//*/
//=============================================================================
class Tone
{
public:

	lnfloat red;
	lnfloat green;
	lnfloat blue;
	lnfloat gray;

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      デフォルトコンストラクタ
    //*/
    //---------------------------------------------------------------------
	Tone() : red( 0.0f ), green( 0.0f ), blue( 0.0f ), gray( 0.0f )
	{}

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ
    //*/
    //---------------------------------------------------------------------
	Tone( lnfloat red_, lnfloat green_, lnfloat blue_, lnfloat gray_ = 0.0f )
	{
		set( red_, green_, blue_, gray_ );
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      新しい値の設定
    //*/
    //---------------------------------------------------------------------
	void set( lnfloat red_, lnfloat green_, lnfloat blue_, lnfloat gray_ = 0.0f )
	{
		red		= ( red_   < -1.0f ) ? -1.0f : ( ( red_   > 1.0f ) ? 1.0f : red_ );
		green	= ( green_ < -1.0f ) ? -1.0f : ( ( green_ > 1.0f ) ? 1.0f : green_ );
		blue	= ( blue_  < -1.0f ) ? -1.0f : ( ( blue_  > 1.0f ) ? 1.0f : blue_ );
		gray	= ( gray_  < 0.0f ) ? 0.0f : ( ( gray_ > 1.0f ) ? 1.0f : gray_ );
	}

    //---------------------------------------------------------------------
	///**
	//	@brief		要素の内容をコンソールに出力する
	// 
	//	@param[in]	str_ : ヘッダ文字列
	//*/
	//---------------------------------------------------------------------
	void dump( const char* str_ = NULL ) const;

    /// LVector4 への型変換
    operator LVector4() { return *reinterpret_cast< LVector4* >( this ); }
    operator const LVector4() const { return *reinterpret_cast< const LVector4* >( this ); }
};


//=============================================================================
// ■ Material クラス
//-----------------------------------------------------------------------------
///**
//  @brief  マテリアルのクラス
//*/
//=============================================================================
class Material
{
public:
    
    Color   Diffuse;       ///< 物体の色
	Color   Ambient;       ///< 環境光
	Color   Specular;      ///< 光沢
	Color   Emissive;      ///< 物体の発光色 ( 光源の影響を受けない色 )
    lnfloat  Power;         ///< 光沢の強さ
    //bool     IsSphereMap;   ///< テクスチャがスフィアマップの場合 true

private:

    Graphics::ITexture*   _Texture;         ///< テクスチャ (getTexture() setTexture()でアクセスする)
    Graphics::ITexture*   _SphereTexture;   ///< スフィアテクスチャ (getSphereTexture() setSphereTexture()でアクセスする)
    Graphics::IShader*    _Shader;          ///< シェーダ (getShader() setShader()でアクセスする)

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ
    //*/
    //---------------------------------------------------------------------
	Material()
	{
		Diffuse.set( 1.0f, 1.0f, 1.0f, 1.0f );
		Ambient.set( 0.0f, 0.0f, 0.0f, 0.0f );
		Specular.set( 0.5f, 0.5f, 0.5f, 0.5f );
		Emissive.set( 0.0f, 0.0f, 0.0f, 0.0f );
		Power = 50.0f;
        _Texture = NULL;
        _SphereTexture = NULL;
        _Shader = NULL;
        //IsSphereMap = false;
	}

    ~Material();

    //---------------------------------------------------------------------
    ///**
    //  @brief      半透明要素があるかどうかを判定する
    //*/
    //---------------------------------------------------------------------
    bool isSemiTransState() const
    {
        return ( Diffuse.alpha < 1.0f );
    }

    /// テクスチャの取得
    Graphics::ITexture* getTexture() const;

    /// テクスチャの設定
    void setTexture( Graphics::ITexture* texture_ );

    /// スフィアテクスチャの取得
    Graphics::ITexture* getSphereTexture() const;

    /// スフィアテクスチャの設定
    void setSphereTexture( Graphics::ITexture* texture_ );

    /// シェーダの取得
    Graphics::IShader* getShader() const;

    /// シェーダの設定
    void setShader( Graphics::IShader* texture_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンソールに表示
    //*/
    //---------------------------------------------------------------------
	void dump( const char* str_ = NULL ) const
	{
        if ( str_ )
		{
			printf( "%s\n", str_ );
		}
		else
		{
			printf( "◆ LMaterial " );
		}
		printf( "Diffuse  : %f, %f, %f, %f\n", Diffuse.red, Diffuse.green, Diffuse.blue, Diffuse.alpha );
		printf( "Ambient  : %f, %f, %f, %f\n", Ambient.red, Ambient.green, Ambient.blue, Ambient.alpha );
		printf( "Specular : %f, %f, %f, %f\n", Specular.red, Specular.green, Specular.blue, Specular.alpha );
		printf( "Emissive : %f, %f, %f, %f\n", Emissive.red, Emissive.green, Emissive.blue, Emissive.alpha );
		printf( "Power    : %f\n", Power );
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      比較
    //*/
    //---------------------------------------------------------------------
	bool operator == ( const Material& material_ )
	{
		return (
			Diffuse  == material_.Diffuse  && Ambient  == material_.Ambient &&
			Specular == material_.Specular && Emissive == material_.Emissive &&
            Power    == material_.Power );
	}

    /// コピーコンストラクタ
    Material( const Material& material_ );

    /// 代入
    Material& operator = ( const Material& material_ );

#ifdef LNOTE_INCLUDED_DIRECTX_HEADER

    //---------------------------------------------------------------------
    ///**
    //  @brief      D3DMATERIAL9 から設定
    //*/
    //---------------------------------------------------------------------
	void setFromDxMaterial( const D3DMATERIAL9& dx_material_ )
	{
		Diffuse.red	   = dx_material_.Diffuse.r;
		Diffuse.green  = dx_material_.Diffuse.g;
		Diffuse.blue   = dx_material_.Diffuse.b;
		Diffuse.alpha  = dx_material_.Diffuse.a;
		Ambient.red	   = dx_material_.Ambient.r;
		Ambient.green  = dx_material_.Ambient.g;
		Ambient.blue   = dx_material_.Ambient.b;
		Ambient.alpha  = dx_material_.Ambient.a;
		Specular.red   = dx_material_.Specular.r;
		Specular.green = dx_material_.Specular.g;
		Specular.blue  = dx_material_.Specular.b;
		Specular.alpha = dx_material_.Specular.a;
		Emissive.red   = dx_material_.Emissive.r;
		Emissive.green = dx_material_.Emissive.g;
		Emissive.blue  = dx_material_.Emissive.b;
		Emissive.alpha = dx_material_.Emissive.a;
		Power = dx_material_.Power;
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      D3DMATERIAL9 に変換
    //*/
    //---------------------------------------------------------------------
	D3DMATERIAL9 toDxMaterial() const
	{
		D3DMATERIAL9 dx_material;
		dx_material.Diffuse.r  = Diffuse.red;
		dx_material.Diffuse.g  = Diffuse.green;
		dx_material.Diffuse.b  = Diffuse.blue;
		dx_material.Diffuse.a  = Diffuse.alpha;
		dx_material.Ambient.r  = Ambient.red;
		dx_material.Ambient.g  = Ambient.green;
		dx_material.Ambient.b  = Ambient.blue;
		dx_material.Ambient.a  = Ambient.alpha;
		dx_material.Specular.r = Specular.red;
		dx_material.Specular.g = Specular.green;
		dx_material.Specular.b = Specular.blue;
		dx_material.Specular.a = Specular.alpha;
		dx_material.Emissive.r = Emissive.red;
		dx_material.Emissive.g = Emissive.green;
		dx_material.Emissive.b = Emissive.blue;
		dx_material.Emissive.a = Emissive.alpha;
		dx_material.Power = Power;
		return dx_material;
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      D3DMATERIAL9 をコンソールに表示
    //*/
    //---------------------------------------------------------------------
	static void dumpDxMaterial( const D3DMATERIAL9& dx_material_ )
	{
		printf( "D3DMATERIAL9----------------------------\n" );
		printf( "diffuse  : %f, %f, %f, %f\n", dx_material_.Diffuse.r, dx_material_.Diffuse.g, dx_material_.Diffuse.b, dx_material_.Diffuse.a );
		printf( "ambient  : %f, %f, %f, %f\n", dx_material_.Ambient.r, dx_material_.Ambient.g, dx_material_.Ambient.b, dx_material_.Ambient.a );
		printf( "specular : %f, %f, %f, %f\n", dx_material_.Specular.r, dx_material_.Specular.g, dx_material_.Specular.b, dx_material_.Specular.a );
		printf( "emissive : %f, %f, %f, %f\n", dx_material_.Emissive.r, dx_material_.Emissive.g, dx_material_.Emissive.b, dx_material_.Emissive.a );
		printf( "power    : %f\n", dx_material_.Power );
		printf( "----------------------------------------\n" );
	}

#endif // LNOTE_DIRECTX
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core

// 仮
typedef Core::Graphics::Color		LColor;
typedef Core::Graphics::Tone		LTone;
typedef Core::Graphics::Material	LMaterial;

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================