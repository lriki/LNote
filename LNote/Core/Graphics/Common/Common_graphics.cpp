//=============================================================================
//【 Common 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Common.h"
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
// ■ Color クラス
//=============================================================================

    const Color Color::TRANSPARENCY         = Color( 0.0, 0.0, 0.0, 0.0 );
    const Color Color::TRANSPARENCY_WHITE   = Color( 1.0, 1.0, 1.0, 0.0 );
    const Color Color::WHITE                = Color( 1.0, 1.0, 1.0, 1.0 );
    const Color Color::RED                  = Color( 1.0, 0.0, 0.0, 1.0 );
    const Color Color::GREEN                = Color( 0.0, 1.0, 0.0, 1.0 );
    const Color Color::BLUE                 = Color( 0.0, 0.0, 1.0, 1.0 );
    const Color Color::GRAY                 = Color( 0.5, 0.5, 0.5, 1.0 );

    //---------------------------------------------------------------------
    // ● デフォルトコンストラクタ
    //---------------------------------------------------------------------
    Color::Color()
        : red   ( 0.0f )
	    , green ( 0.0f )
	    , blue  ( 0.0f )
	    , alpha ( 1.0f )
    {
    	}

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    Color::Color( lnfloat r_, lnfloat g_, lnfloat b_, lnfloat a_ )
    {
        red		= ( r_ < 0.0f ) ? 0.0f : ( ( r_ > 1.0f ) ? 1.0f : r_ );
		green	= ( g_ < 0.0f ) ? 0.0f : ( ( g_ > 1.0f ) ? 1.0f : g_ );
		blue	= ( b_ < 0.0f ) ? 0.0f : ( ( b_ > 1.0f ) ? 1.0f : b_ );
		alpha	= ( a_ < 0.0f ) ? 0.0f : ( ( a_ > 1.0f ) ? 1.0f : a_ );
    }

    //---------------------------------------------------------------------
    // ● 新しい値の設定
    //---------------------------------------------------------------------
    void Color::set( lnfloat r_, lnfloat g_, lnfloat b_, lnfloat a_ )
    {
        red		= ( r_ < 0.0f ) ? 0.0f : ( ( r_ > 1.0f ) ? 1.0f : r_ );
		green	= ( g_ < 0.0f ) ? 0.0f : ( ( g_ > 1.0f ) ? 1.0f : g_ );
		blue	= ( b_ < 0.0f ) ? 0.0f : ( ( b_ > 1.0f ) ? 1.0f : b_ );
		alpha	= ( a_ < 0.0f ) ? 0.0f : ( ( a_ > 1.0f ) ? 1.0f : a_ );
    }

    //---------------------------------------------------------------------
    // ● 32 ビットの色コードから色を作成
    //---------------------------------------------------------------------
    void Color::setFrom32Bit( u32 code_, LNGraphicsAPI api_ )
    {
        static const lnfloat d = 0.003921568627450980392156862745098f;
        switch ( api_ )
        {
            case LN_GRAPHICSAPI_DIRECTX9:
            case LN_GRAPHICSAPI_DIRECTX11:
                red	    = static_cast< lnfloat >( code_ & 0x000000ff ) * d;
		        code_ >>= 8;
		        green	= static_cast< lnfloat >( code_ & 0x000000ff ) * d;
		        code_ >>= 8;
		        blue    = static_cast< lnfloat >( code_ & 0x000000ff ) * d;
		        code_ >>= 8;
		        alpha	= static_cast< lnfloat >( code_ & 0x000000ff ) * d;
                break;
            case LN_GRAPHICSAPI_OPENGL:
                blue	= static_cast< lnfloat >( code_ & 0x000000ff ) * d;
		        code_ >>= 8;
		        green	= static_cast< lnfloat >( code_ & 0x000000ff ) * d;
		        code_ >>= 8;
		        red		= static_cast< lnfloat >( code_ & 0x000000ff ) * d;
		        code_ >>= 8;
		        alpha	= static_cast< lnfloat >( code_ & 0x000000ff ) * d;
                break;
            default:
                LN_THROW_NotImpl( 0 );
                break;
        }
    }

    //---------------------------------------------------------------------
    // ● 32 ビットの色コードに変換
    //---------------------------------------------------------------------
    u32 Color::to32Bit( LNGraphicsAPI api_ ) const
    {
        u32 r = static_cast< u32 >( red   * 255.0f );
		u32 g = static_cast< u32 >( green * 255.0f );
		u32 b = static_cast< u32 >( blue  * 255.0f );
		u32 a = static_cast< u32 >( alpha * 255.0f );
        switch ( api_ )
        {
            case LN_GRAPHICSAPI_DIRECTX9:
            case LN_GRAPHICSAPI_DIRECTX11:
                return ( ( ( a & 0xff ) << 24 ) | ( ( r & 0xff ) << 16 ) | ( ( g & 0xff ) << 8 ) | ( b & 0xff ) );
            case LN_GRAPHICSAPI_OPENGL:
		        return ( ( ( a & 0xff ) << 24 ) | ( ( b & 0xff ) << 16 ) | ( ( g & 0xff ) << 8 ) | ( r & 0xff ) );
            default:
                LN_THROW_NotImpl( 0 );
                return 0;
        }
    }

    //---------------------------------------------------------------------
	// ● 要素の内容をコンソールに出力する
	//---------------------------------------------------------------------
	void Color::dump( const char* str_ ) const
    {
        if ( str_ )
		{
			printf( "%s", str_ );
		}
		else
		{
			printf( "◆ Color " );
		}
		printf( "( %f, %f, %f, %f )\n", red, green, blue, alpha );
    }

    bool Color::operator == ( const Color& color_ )
    {
        return ( memcmp( this, &color_, sizeof( Color ) ) == 0 );
   //     return (
			//red == color_.red && green == color_.green &&
			//blue == color_.blue && alpha == color_.alpha );
    }

//=============================================================================
// ■ Tone クラス
//=============================================================================

    // コンソールに出力する
	void Tone::dump( const char* str_ ) const
	{
		if ( str_ )
		{
			printf( "%s", str_ );
		}
		else
		{
			printf( "◆ Tone " );
		}
		printf( "( %f, %f, %f, %f )\n", red, green, blue, gray );
	}

//=============================================================================
// ■ Material クラス
//=============================================================================

    /// デストラクタ
    Material::~Material()
    {
        LN_SAFE_RELEASE( _Texture );
        LN_SAFE_RELEASE( _SphereTexture );
        LN_SAFE_RELEASE( _Shader );
    }

    /// テクスチャの取得
    Graphics::ITexture* Material::getTexture() const
    {
        return _Texture;
    }

    /// テクスチャの設定
    void Material::setTexture( Graphics::ITexture* texture_ )
    {
        LN_SAFE_ADDREF( texture_ );
        LN_SAFE_RELEASE( _Texture );
        _Texture = texture_;
    }

    /// スフィアテクスチャの取得
    Graphics::ITexture* Material::getSphereTexture() const
    {
        return _SphereTexture;
    }

    /// スフィアテクスチャの設定
    void Material::setSphereTexture( Graphics::ITexture* texture_ )
    {
        LN_SAFE_ADDREF( texture_ );
        LN_SAFE_RELEASE( _SphereTexture );
        _SphereTexture = texture_;    
    }

    /// シェーダの取得
    Graphics::IShader* Material::getShader() const
    {
        return _Shader;
    }

    /// シェーダの設定
    void Material::setShader( Graphics::IShader* shader_ )
    {
        LN_SAFE_ADDREF( shader_ );
        LN_SAFE_RELEASE( _Shader );
        _Shader = shader_;
    }

    /// コピーコンストラクタ
    Material::Material( const Material& material_ )
    {
        Diffuse = material_.Diffuse;
		Ambient = material_.Ambient;
		Specular = material_.Specular;
		Emissive = material_.Emissive;
		Power = material_.Power;
        setTexture( material_._Texture );
        setSphereTexture( material_._SphereTexture );
        setShader( material_._Shader );
    }

    /// 代入
    Material& Material::operator = ( const Material& material_ )
    {
        Diffuse = material_.Diffuse;
		Ambient = material_.Ambient;
		Specular = material_.Specular;
		Emissive = material_.Emissive;
		Power = material_.Power;
        setTexture( material_._Texture );
        setSphereTexture( material_._SphereTexture );
        setShader( material_._Shader );
        return *this;
    }
} // namespace Graphics
//=============================================================================
// ■ LRenderState クラス
//=============================================================================

    const LRenderState LRenderState::BUFFER_COPY_STATE =
        LRenderState(
            LN_BLEND_NORMAL,
            LN_CULL_FRONT,
            LN_FILL_SOLID,
            false, false, false, false );

    const LRenderState LRenderState::SMOOTH_ALPHA =
        LRenderState(
            LN_BLEND_ALPHA,
            LN_CULL_FRONT,
            LN_FILL_SOLID,
            false, false, false, false );
    
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------


} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================