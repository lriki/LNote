//=============================================================================
//�y Common �z
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
// �� Color �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �F�̃N���X
//
//  @par
//              ���Ɏw�肪�Ȃ�����A�F�� 0.0 �` 1.0 �͈̔͂̒l�ň����܂��B
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

    static const Color TRANSPARENCY;         ///< ���� (0.0, 0.0, 0.0, 0.0)
    static const Color TRANSPARENCY_WHITE;   ///< ������ (1.0, 1.0, 1.0, 0.0)
    static const Color WHITE;                ///< �� (1.0, 1.0, 1.0, 1.0)
    static const Color RED;                  ///< �� (1.0, 0.0, 0.0, 1.0)
    static const Color GREEN;                ///< �� (0.0, 1.0, 0.0, 1.0)
    static const Color BLUE;                 ///< �� (0.0, 0.0, 1.0, 1.0)
    static const Color GRAY;                 ///< �D�F (0.5, 0.5, 0.5, 1.0)

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �f�t�H���g�R���X�g���N�^
    //
    //  @par		
    //              �F�͍�( 0, 0, 0 ) �A�A���t�@�l�� 1.0 �ɂȂ�܂��B
    //*/
    //---------------------------------------------------------------------
    Color();

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���X�g���N�^
    //
    //  @par
    //              �l���͈͊O�̏ꍇ�� 0.0 �` 1.0 �̊ԂɃN���b�s���O����܂��B
    //*/
    //---------------------------------------------------------------------
    Color( lnfloat r_, lnfloat g_, lnfloat b_, lnfloat a_ = 1.0f );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���X�g���N�^
    //
    //  @par
    //              32 �r�b�g�̐F�R�[�h (ARGB) ����쐬���܂��B
    //*/
    //---------------------------------------------------------------------
    Color( u32 color_code_, LNGraphicsAPI api_ ) { setFrom32Bit( color_code_, api_ ); }

    /// �R�s�[
    Color( const Color& c_ )
    {
        red   = c_.red;
        green = c_.green;
        blue  = c_.blue;
        alpha = c_.alpha;
    }

    /// �R�s�[
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
    //  @brief      �V�����l�̐ݒ�
    //
    //  @par
    //              �l���͈͊O�̏ꍇ�� 0.0 �` 1.0 �̊ԂɃN���b�s���O����܂��B
    //*/
    //---------------------------------------------------------------------
    void set( lnfloat r_, lnfloat g_, lnfloat b_, lnfloat a_ = 1.0f );

    //---------------------------------------------------------------------
    ///**
    //  @brief      32 �r�b�g�̐F�R�[�h����F���쐬
    //
    //  @note
    //              ARGB �̏��Ŋe 8 �r�b�g
    //*/
    //---------------------------------------------------------------------
    void setFrom32Bit( u32 code_, LNGraphicsAPI api_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      32 �r�b�g�̐F�R�[�h�ɕϊ�
    //*/
    //---------------------------------------------------------------------
    u32 to32Bit( LNGraphicsAPI api_ ) const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      Vector4 �Ƃ��Ă̎Q�Ƃ�Ԃ�
    //*/
    //---------------------------------------------------------------------
    const LVector4& toVector4() const { return *( (LVector4*)this ); }

    //---------------------------------------------------------------------
	///**
	//	@brief		�v�f�̓��e���R���\�[���ɏo�͂���
	// 
	//	@param[in]	str_ : �w�b�_������
	//*/
	//---------------------------------------------------------------------
	void dump( const char* str_ = NULL ) const;

    /// LVector4 ����̐ݒ�
    Color& operator = ( const LVector4& v_ )
    {
        red   = v_.x;
        green = v_.y;
        blue  = v_.z;
        alpha = v_.w;
        return ( *this );
    }

    /// LVector4 �Ƃ̏�Z
    //LVector4 operator * ( const LVector4& v_ )
    //{
    //    LVector4 vec(
    //        red   * v_.x,
    //        green * v_.y,
    //        blue  * v_.z,
    //        alpha * v_.w );
    //    return vec;
    //}

    /// LVector4 �ւ̌^�ϊ�
    operator LVector4() { return *reinterpret_cast< LVector4* >( this ); }
    operator const LVector4() const { return *reinterpret_cast< const LVector4* >( this ); }

public:

    bool operator == ( const Color& color_ );
};

//=============================================================================
// �� LColor32 �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      32 �r�b�g�J���[�̃N���X
//
//  @par
//              �t�H�[�}�b�g�� ARGB
//*/
//=============================================================================
class LColor32
{
public:

	lnU32 color;

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �f�t�H���g�R���X�g���N�^
    //
    //  @par		
    //              �F�͍� 0xff000000 �ɂȂ�܂��B
    //*/
    //---------------------------------------------------------------------
    LColor32();

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���X�g���N�^
    //
    //  @par
    //              32 �r�b�g�̐F�R�[�h (ARGB) ����쐬���܂��B
    //*/
    //---------------------------------------------------------------------
    LColor32( u32 color_code_ ) { color = color_code_; }

public:

    void set( u32 color_code_ ) { color = color_code_; }

};

//=============================================================================
// �� Tone �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�F���̃N���X
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
    //  @brief      �f�t�H���g�R���X�g���N�^
    //*/
    //---------------------------------------------------------------------
	Tone() : red( 0.0f ), green( 0.0f ), blue( 0.0f ), gray( 0.0f )
	{}

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���X�g���N�^
    //*/
    //---------------------------------------------------------------------
	Tone( lnfloat red_, lnfloat green_, lnfloat blue_, lnfloat gray_ = 0.0f )
	{
		set( red_, green_, blue_, gray_ );
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      �V�����l�̐ݒ�
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
	//	@brief		�v�f�̓��e���R���\�[���ɏo�͂���
	// 
	//	@param[in]	str_ : �w�b�_������
	//*/
	//---------------------------------------------------------------------
	void dump( const char* str_ = NULL ) const;

    /// LVector4 �ւ̌^�ϊ�
    operator LVector4() { return *reinterpret_cast< LVector4* >( this ); }
    operator const LVector4() const { return *reinterpret_cast< const LVector4* >( this ); }
};


//=============================================================================
// �� Material �N���X
//-----------------------------------------------------------------------------
///**
//  @brief  �}�e���A���̃N���X
//*/
//=============================================================================
class Material
{
public:
    
    Color   Diffuse;       ///< ���̂̐F
	Color   Ambient;       ///< ����
	Color   Specular;      ///< ����
	Color   Emissive;      ///< ���̂̔����F ( �����̉e�����󂯂Ȃ��F )
    lnfloat  Power;         ///< ����̋���
    //bool     IsSphereMap;   ///< �e�N�X�`�����X�t�B�A�}�b�v�̏ꍇ true

private:

    Graphics::ITexture*   _Texture;         ///< �e�N�X�`�� (getTexture() setTexture()�ŃA�N�Z�X����)
    Graphics::ITexture*   _SphereTexture;   ///< �X�t�B�A�e�N�X�`�� (getSphereTexture() setSphereTexture()�ŃA�N�Z�X����)
    Graphics::IShader*    _Shader;          ///< �V�F�[�_ (getShader() setShader()�ŃA�N�Z�X����)

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���X�g���N�^
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
    //  @brief      �������v�f�����邩�ǂ����𔻒肷��
    //*/
    //---------------------------------------------------------------------
    bool isSemiTransState() const
    {
        return ( Diffuse.alpha < 1.0f );
    }

    /// �e�N�X�`���̎擾
    Graphics::ITexture* getTexture() const;

    /// �e�N�X�`���̐ݒ�
    void setTexture( Graphics::ITexture* texture_ );

    /// �X�t�B�A�e�N�X�`���̎擾
    Graphics::ITexture* getSphereTexture() const;

    /// �X�t�B�A�e�N�X�`���̐ݒ�
    void setSphereTexture( Graphics::ITexture* texture_ );

    /// �V�F�[�_�̎擾
    Graphics::IShader* getShader() const;

    /// �V�F�[�_�̐ݒ�
    void setShader( Graphics::IShader* texture_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���\�[���ɕ\��
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
			printf( "�� LMaterial " );
		}
		printf( "Diffuse  : %f, %f, %f, %f\n", Diffuse.red, Diffuse.green, Diffuse.blue, Diffuse.alpha );
		printf( "Ambient  : %f, %f, %f, %f\n", Ambient.red, Ambient.green, Ambient.blue, Ambient.alpha );
		printf( "Specular : %f, %f, %f, %f\n", Specular.red, Specular.green, Specular.blue, Specular.alpha );
		printf( "Emissive : %f, %f, %f, %f\n", Emissive.red, Emissive.green, Emissive.blue, Emissive.alpha );
		printf( "Power    : %f\n", Power );
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      ��r
    //*/
    //---------------------------------------------------------------------
	bool operator == ( const Material& material_ )
	{
		return (
			Diffuse  == material_.Diffuse  && Ambient  == material_.Ambient &&
			Specular == material_.Specular && Emissive == material_.Emissive &&
            Power    == material_.Power );
	}

    /// �R�s�[�R���X�g���N�^
    Material( const Material& material_ );

    /// ���
    Material& operator = ( const Material& material_ );

#ifdef LNOTE_INCLUDED_DIRECTX_HEADER

    //---------------------------------------------------------------------
    ///**
    //  @brief      D3DMATERIAL9 ����ݒ�
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
    //  @brief      D3DMATERIAL9 �ɕϊ�
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
    //  @brief      D3DMATERIAL9 ���R���\�[���ɕ\��
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

// ��
typedef Core::Graphics::Color		LColor;
typedef Core::Graphics::Tone		LTone;
typedef Core::Graphics::Material	LMaterial;

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================