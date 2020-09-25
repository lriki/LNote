//=============================================================================
//【 Resource 】
//=============================================================================

#include "stdafx.h"
#include "LNResource.h"

// 配列の要素数を返すマクロ
#define ARRAY_SIZE_OF( ary_ )     ( sizeof( ary_ ) / sizeof( ary_[ 0 ] ) )

namespace LNote
{
namespace Core
{
namespace Resource
{

/// シェーダリソース
namespace Shader
{
    /// Pane マスクなしトランジション
    const unsigned char BasicPane_Transition[] =
	{
		#include "Shader/BasicPane_Transition.lnfx.h"
	};
    const unsigned int BasicPane_Transition_Size = ARRAY_SIZE_OF( BasicPane_Transition );

    /// Pane マスク付きトランジション
    const unsigned char BasicPane_TransitionWithMask[] =
	{
		#include "Shader/BasicPane_TransitionWithMask.lnfx.h"
	};
    const unsigned int BasicPane_TransitionWithMask_Size = ARRAY_SIZE_OF( BasicPane_TransitionWithMask );

    /// Pane ブラ―
    const unsigned char BasicPane_BufferingBlar[] =
	{
		#include "Shader/BasicPane_BufferingBlar.lnfx.h"
	};
    const unsigned int BasicPane_BufferingBlar_Size = ARRAY_SIZE_OF( BasicPane_BufferingBlar );
    
	/// Pane PostDraw
    const unsigned char BasicPane_PostDraw[] =
	{
		#include "Shader/BasicPane_PostDraw.lnfx.h"
	};
    const unsigned int BasicPane_PostDraw_Size = ARRAY_SIZE_OF( BasicPane_PostDraw );
    

    /// 基本形状描画用
    const unsigned char GeometryRenderer[] =
	{
		#include "Shader/GeometryRenderer.lnfx.h"
	};
    const unsigned int  GeometryRenderer_Size = ARRAY_SIZE_OF( GeometryRenderer );

    /// スプライト描画用
    const unsigned char SpriteRenderer[] =
	{
		#include "Shader/SpriteRenderer.lnfx.h"
	};
    const unsigned int  SpriteRenderer_Size = ARRAY_SIZE_OF( SpriteRenderer );

    /// PointParticle
    const unsigned char PointParticle[] =
	{
		#include "Shader/PointParticle.lnfx.h"
	};
    const unsigned int PointParticle_Size = ARRAY_SIZE_OF( PointParticle );

    /// Tilemap
    const unsigned char Tilemap[] =
	{
		#include "Shader/Tilemap.lnfx.h"
	};
    const unsigned int Tilemap_Size = ARRAY_SIZE_OF( Tilemap );


    /// SceneShader 2D 基本
    extern const unsigned char SSBasic2D[] =
	{
		#include "Shader/SSBasic2D.lnfx.h"
	};
    extern const unsigned int SSBasic2D_Size = ARRAY_SIZE_OF( SSBasic2D );

    /// SceneShader 3D 基本
    extern const unsigned char SSBasic3D[] =
	{
		#include "Shader/SSBasic3D.lnfx.h"
	};
    extern const unsigned int SSBasic3D_Size = ARRAY_SIZE_OF( SSBasic3D );

    /// SSBasic3DSkinning
    extern const unsigned char SSBasic3DSkinning[] =
	{
		#include "Shader/SSBasic3DSkinning.lnfx.h"
	};
    extern const unsigned int SSBasic3DSkinning_Size = ARRAY_SIZE_OF( SSBasic3DSkinning );

    /// SceneShader ライティングなし
    const unsigned char SSNoLighting[] =
	{
		#include "Shader/SSNoLighting.lnfx.h"
	};
    const unsigned int  SSNoLighting_Size = ARRAY_SIZE_OF( SSNoLighting );
    
}

/// 画像リソース
namespace Image
{
    /// DebugFont 用フォントテクスチャ
    const unsigned char DebugFont[] =
	{
		#include "Image/DebugFont.png.h"
	};
    const unsigned int DebugFont_Size = ARRAY_SIZE_OF( DebugFont );

    /// GUI スキンテクスチャ
    const unsigned char GUISkin[] =
	{
		#include "Image/GUISkin.png.h"
	};
    const unsigned int GUISkin_Size = ARRAY_SIZE_OF( GUISkin );
}



//=============================================================================
// ■ HardCodeResourceArchive
//=============================================================================

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HardCodeResourceArchive::HardCodeResourceArchive()
    {

    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    File::IInStream* HardCodeResourceArchive::createStream( const lnChar* filename_ )
    {
        File::IInStream* st = NULL;

        
        if ( _tcsicmp( filename_, _T( "LNResource\\Shader\\GeometryRenderer.lnfx" ) ) == 0 )
        {
            File::createInStreamFromMemoryUnManaged( &st, Shader::GeometryRenderer, Shader::GeometryRenderer_Size );
        }
        else if ( _tcsicmp( filename_, _T( "LNResource\\Shader\\SpriteRenderer.lnfx" ) ) == 0 )
        {
            File::createInStreamFromMemoryUnManaged( &st, Shader::SpriteRenderer, Shader::SpriteRenderer_Size );
        }
        else if ( _tcsicmp( filename_, _T( "LNResource\\Shader\\SSBasic2D.lnfx" ) ) == 0 )
        {
            File::createInStreamFromMemoryUnManaged( &st, Shader::SSBasic2D, Shader::SSBasic2D_Size );
        }
        else if ( _tcsicmp( filename_, _T( "LNResource\\Shader\\SSBasic3D.lnfx" ) ) == 0 )
        {
            File::createInStreamFromMemoryUnManaged( &st, Shader::SSBasic3D, Shader::SSBasic3D_Size );
        }
        else if ( _tcsicmp( filename_, _T( "LNResource\\Shader\\SSBasic3DSkinning.lnfx" ) ) == 0 )
        {
            File::createInStreamFromMemoryUnManaged( &st, Shader::SSBasic3DSkinning, Shader::SSBasic3DSkinning_Size );
        }
        else if ( _tcsicmp( filename_, _T( "LNResource\\Shader\\SSNoLighting.lnfx" ) ) == 0 )
        {
            File::createInStreamFromMemoryUnManaged( &st, Shader::SSNoLighting, Shader::SSNoLighting_Size );
        }
        


        //-----------------------------------------------------
        // Shader
        if ( _tcsicmp( filename_, _T( "LNResource\\Shader\\BasicPane_Transition.lnfx" ) ) == 0 )
        {
            File::createInStreamFromMemoryUnManaged( &st, Shader::BasicPane_Transition, Shader::BasicPane_Transition_Size );
        }
        else if ( _tcsicmp( filename_, _T( "LNResource\\Shader\\BasicPane_TransitionWithMask.lnfx" ) ) == 0 )
        {
            File::createInStreamFromMemoryUnManaged( &st, Shader::BasicPane_TransitionWithMask, Shader::BasicPane_TransitionWithMask_Size );
        }
        else if ( _tcsicmp( filename_, _T( "LNResource\\Shader\\BasicPane_BufferingBlar.lnfx" ) ) == 0 )
        {
            File::createInStreamFromMemoryUnManaged( &st, Shader::BasicPane_BufferingBlar, Shader::BasicPane_BufferingBlar_Size );
        }
		else if ( _tcsicmp( filename_, _T( "LNResource\\Shader\\BasicPane_PostDraw.lnfx" ) ) == 0 )
        {
            File::createInStreamFromMemoryUnManaged( &st, Shader::BasicPane_PostDraw, Shader::BasicPane_PostDraw_Size );
        }
        else if ( _tcsicmp( filename_, _T( "LNResource\\Shader\\PointParticle.lnfx" ) ) == 0 )
        {
            File::createInStreamFromMemoryUnManaged( &st, Shader::PointParticle, Shader::PointParticle_Size );
        }
        else if ( _tcsicmp( filename_, _T( "LNResource\\Shader\\Tilemap.lnfx" ) ) == 0 )
        {
            File::createInStreamFromMemoryUnManaged( &st, Shader::Tilemap, Shader::Tilemap_Size );
        }

        //-----------------------------------------------------
        // Image
        else if ( _tcsicmp( filename_, _T( "LNResource\\Image\\DebugFont.png" ) ) == 0 )
        {
            File::createInStreamFromMemoryUnManaged( &st, Image::DebugFont, Image::DebugFont_Size );
        }
        else if ( _tcsicmp( filename_, _T( "LNResource\\Image\\GUISkin.png" ) ) == 0 )
        {
            File::createInStreamFromMemoryUnManaged( &st, Image::GUISkin, Image::GUISkin_Size );
        }

        return st;
    }


} // namespace Resource
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================