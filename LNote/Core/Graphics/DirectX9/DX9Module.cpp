//=============================================================================
//【 DX9Module 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "DX9Module.h"

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

//=============================================================================
// ■ DX9Module クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    DX9Module::~DX9Module()
    {
        if ( mD3Dx9Module )
        {
            ::FreeLibrary( mD3Dx9Module );
        }

        if ( mD3D9Module )
        {
            ::FreeLibrary( mD3D9Module );
        }
    }

    //---------------------------------------------------------------------
    // ● インスタンスの取得
    //---------------------------------------------------------------------
    DX9Module* DX9Module::getInstance()
    {
        static DX9Module instance;
        return &instance;
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT DX9Module::initialize()
    {
        if ( !mD3Dx9Module )
        {
            // モジュール読み込み
            mD3D9Module = ::LoadLibrary( _T( "d3d9.dll" ) );
            LN_SETERR_R_S_( ( mD3D9Module ), LN_ERR_SYSTEM,
                "#DirectX の DLL の読み込みに失敗しました。\n"
                "DirectX が正しくインストールされていない可能性があります。" );

            mDirect3DCreate9 = reinterpret_cast< MD_Direct3DCreate9 >( ::GetProcAddress( mD3D9Module, "Direct3DCreate9" ) );

            // モジュール読み込み
            char name[ 64 ] = "";
	        sprintf( name, "d3dx9_%d.dll", D3DX_SDK_VERSION );
            mD3Dx9Module = ::LoadLibraryA( name );
            LN_SETERR_R_S_( ( mD3Dx9Module ), LN_ERR_SYSTEM,
                "#DirectX の DLL の読み込みに失敗しました。\n"
                "DirectX が正しくインストールされていない可能性があります。" );

            mD3DXCreateTextureFromFileInMemoryEx = reinterpret_cast< MD_D3DXCreateTextureFromFileInMemoryEx >( ::GetProcAddress( mD3Dx9Module, "D3DXCreateTextureFromFileInMemoryEx" ) );

            mD3DXGetImageInfoFromFileInMemory = reinterpret_cast< MD_D3DXGetImageInfoFromFileInMemory >( ::GetProcAddress( mD3Dx9Module, "D3DXGetImageInfoFromFileInMemory" ) );

            mD3DXCheckTextureRequirements = reinterpret_cast< MD_D3DXCheckTextureRequirements >( ::GetProcAddress( mD3Dx9Module, "D3DXCheckTextureRequirements" ) );

            mD3DXCreateEffect = reinterpret_cast< MD_D3DXCreateEffect >( ::GetProcAddress( mD3Dx9Module, "D3DXCreateEffect" ) );

            mD3DXLoadMeshFromXInMemory = reinterpret_cast< MD_D3DXLoadMeshFromXInMemory >( ::GetProcAddress( mD3Dx9Module, "D3DXLoadMeshFromXInMemory" ) );

            mD3DXCreateFontIndirect = reinterpret_cast< MD_D3DXCreateFontIndirect >( ::GetProcAddress( mD3Dx9Module, "D3DXCreateFontIndirectW" ) );
               
            mD3DXDeclaratorFromFVF = reinterpret_cast< MD_D3DXDeclaratorFromFVF >( ::GetProcAddress( mD3Dx9Module, "D3DXDeclaratorFromFVF" ) );

            mD3DXLoadMeshHierarchyFromXInMemory = reinterpret_cast< MD_D3DXLoadMeshHierarchyFromXInMemory >( ::GetProcAddress( mD3Dx9Module, "D3DXLoadMeshHierarchyFromXInMemory" ) );
        
            mD3DXFrameDestroy = reinterpret_cast< MD_D3DXFrameDestroy >( ::GetProcAddress( mD3Dx9Module, "D3DXFrameDestroy" ) );

            mD3DXComputeNormals = reinterpret_cast< MD_D3DXComputeNormals >( ::GetProcAddress( mD3Dx9Module, "D3DXComputeNormals" ) );
        }
        return LN_OK;
    }

    // Direct3DCreate9
    IDirect3D9* DX9Module::Direct3DCreate9( UINT SDKVersion )
    {
        return mDirect3DCreate9( SDKVersion );
    }
    
    // D3DXCreateTextureFromFileInMemoryEx
    HRESULT DX9Module::D3DXCreateTextureFromFileInMemoryEx( LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture )
    {
        return mD3DXCreateTextureFromFileInMemoryEx(
				pDevice,
				pSrcData,
				SrcDataSize,
				Width,
				Height,
				MipLevels,
				Usage,
				Format,
				Pool,
				Filter,
				MipFilter,
				ColorKey,
				pSrcInfo,
				pPalette,
				ppTexture );
    }

    // D3DXGetImageInfoFromFileInMemory
    HRESULT DX9Module::D3DXGetImageInfoFromFileInMemory( LPCVOID pSrcData, UINT SrcDataSize, D3DXIMAGE_INFO* pSrcInfo )
    {
        return mD3DXGetImageInfoFromFileInMemory(
            pSrcData,
            SrcDataSize,
            pSrcInfo );
    }

    // D3DXCheckTextureRequirements
    HRESULT DX9Module::D3DXCheckTextureRequirements( LPDIRECT3DDEVICE9 pDevice, UINT *pWidth, UINT *pHeight, UINT *pNumMipLevels, DWORD Usage, D3DFORMAT *pFormat, D3DPOOL Pool )
    {
        return mD3DXCheckTextureRequirements(
                pDevice,
                pWidth,
                pHeight,
                pNumMipLevels,
                Usage,
                pFormat,
                Pool );
    }

    // D3DXCreateEffect
    HRESULT DX9Module::D3DXCreateEffect( LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataLen, CONST D3DXMACRO* pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXEFFECT* ppEffect, LPD3DXBUFFER *ppCompilationErrors )
    {
        return mD3DXCreateEffect(
            pDevice,
            pSrcData,
            SrcDataLen,
            pDefines,
            pInclude,
            Flags,
            pPool,
            ppEffect,
            ppCompilationErrors );
    }

    // D3DXLoadMeshFromXInMemory
    HRESULT DX9Module::D3DXLoadMeshFromXInMemory( LPCVOID Memory, DWORD SizeOfMemory, DWORD Options, LPDIRECT3DDEVICE9 pD3DDevice, LPD3DXBUFFER * ppAdjacency, LPD3DXBUFFER * ppMaterials, LPD3DXBUFFER * ppEffectInstances, DWORD * pNumMaterials, LPD3DXMESH * ppMesh )
    {
        return mD3DXLoadMeshFromXInMemory(
            Memory,
            SizeOfMemory,
            Options,
            pD3DDevice,
            ppAdjacency,
            ppMaterials,
            ppEffectInstances,
            pNumMaterials,
            ppMesh );
    }

    // D3DXCreateFontIndirect
    HRESULT DX9Module::D3DXCreateFontIndirect( LPDIRECT3DDEVICE9 pDevice, CONST D3DXFONT_DESC * pDesc, LPD3DXFONT * ppFont )
    {
        return mD3DXCreateFontIndirect(
            pDevice,
            pDesc,
            ppFont );
    }

    // D3DXDeclaratorFromFVF
    HRESULT DX9Module::D3DXDeclaratorFromFVF( DWORD FVF, D3DVERTEXELEMENT9 pDeclarator[MAX_FVF_DECL_SIZE] )
    {
        return mD3DXDeclaratorFromFVF(
            FVF,
            pDeclarator );
    }

    // D3DXLoadMeshHierarchyFromXInMemory
    HRESULT DX9Module::D3DXLoadMeshHierarchyFromXInMemory( LPCVOID pMemory, DWORD SizeOfMemory, DWORD MeshOptions, LPDIRECT3DDEVICE9 pDevice, LPD3DXALLOCATEHIERARCHY pAlloc, LPD3DXLOADUSERDATA pUserDataLoader, LPD3DXFRAME* ppFrameHeirarchy, LPD3DXANIMATIONCONTROLLER* ppAnimController )
    {
        return mD3DXLoadMeshHierarchyFromXInMemory(
            pMemory,
            SizeOfMemory,
            MeshOptions,
            pDevice,
            pAlloc,
            pUserDataLoader,
            ppFrameHeirarchy,
            ppAnimController );
    }

    /// D3DXFrameDestroy
    HRESULT DX9Module::D3DXFrameDestroy( LPD3DXFRAME pFrameRoot, LPD3DXALLOCATEHIERARCHY pAlloc )
    {
        return mD3DXFrameDestroy(
            pFrameRoot,
            pAlloc
            );
    }

    /// D3DXComputeNormals
    HRESULT DX9Module::D3DXComputeNormals( LPD3DXBASEMESH pMesh, const DWORD *pAdjacency )
    {
        return mD3DXComputeNormals(
            pMesh,
            pAdjacency );
    }

    //---------------------------------------------------------------------
    // ● D3DFORMAT の文字列表現の取得
    //---------------------------------------------------------------------
    const lnChar* DX9Module::getDxFormatString( D3DFORMAT Format )
    {
	    switch ( Format )
	    {
	    case D3DFMT_UNKNOWN:			
	    	return _T( "UNKNOWN" );
	    case D3DFMT_R8G8B8:				
	    	return _T( "R8G8B8" );
	    case D3DFMT_A8R8G8B8:			
	    	return _T( "A8R8G8B8" );
	    case D3DFMT_X8R8G8B8:			
	    	return _T( "X8R8G8B8" );
	    case D3DFMT_R5G6B5:				
	    	return _T( "R5G6B5" );
	    case D3DFMT_X1R5G5B5:			
	    	return _T( "X1R5G5B5" );
	    case D3DFMT_A1R5G5B5:			
	    	return _T( "A1R5G5B5" );
	    case D3DFMT_A4R4G4B4:			
	    	return _T( "A4R4G4B4" );
	    case D3DFMT_R3G3B2:				
	    	return _T( "R3G3B2" );
	    case D3DFMT_A8:					
	    	return _T( "A8" );
	    case D3DFMT_A8R3G3B2:			
	    	return _T( "A8R3G3B2" );
	    case D3DFMT_X4R4G4B4:			
	    	return _T( "X4R4G4B4" );
	    case D3DFMT_A2B10G10R10:		
	    	return _T( "A2B10G10R10" );
	    case D3DFMT_A8B8G8R8:			
	    	return _T( "A8B8G8R8" );
	    case D3DFMT_X8B8G8R8:			
	    	return _T( "X8B8G8R8" );
	    case D3DFMT_G16R16:				
	    	return _T( "G16R16" );
	    case D3DFMT_A2R10G10B10:		
	    	return _T( "A2R10G10B10" );
	    case D3DFMT_A16B16G16R16:		
	    	return _T( "A16B16G16R16" );
	    case D3DFMT_A8P8:				
	    	return _T( "A8P8" );
	    case D3DFMT_P8:					
	    	return _T( "P8" );
	    case D3DFMT_L8:					
	    	return _T( "L8" );
	    case D3DFMT_L16:				
	    	return _T( "L16" );
	    case D3DFMT_A8L8:				
	    	return _T( "A8L8" );
	    case D3DFMT_A4L4:				
	    	return _T( "A4L4" );
	    case D3DFMT_V8U8:				
	    	return _T( "V8U8" );
	    case D3DFMT_Q8W8V8U8:			
	    	return _T( "Q8W8V8U8" );
	    case D3DFMT_V16U16:				
	    	return _T( "V16U16" );
	    case D3DFMT_Q16W16V16U16:		
	    	return _T( "Q16W16V16U16" );
	    case D3DFMT_CxV8U8:				
	    	return _T( "CxV8U8" );
	    case D3DFMT_L6V5U5:				
	    	return _T( "L6V5U5" );
	    case D3DFMT_X8L8V8U8:			
	    	return _T( "X8L8V8U8" );
	    case D3DFMT_A2W10V10U10:		
	    	return _T( "A2W10V10U10" );
	    case D3DFMT_G8R8_G8B8:			
	    	return _T( "G8R8_G8B8" );
	    case D3DFMT_R8G8_B8G8:			
	    	return _T( "R8G8_B8G8" );
	    case D3DFMT_DXT1:				
	    	return _T( "DXT1" );
	    case D3DFMT_DXT2:				
	    	return _T( "DXT2" );
	    case D3DFMT_DXT3:				
	    	return _T( "DXT3" );
	    case D3DFMT_DXT4:				
	    	return _T( "DXT4" );
	    case D3DFMT_DXT5:				
	    	return _T( "DXT5" );
	    case D3DFMT_UYVY:				
	    	return _T( "UYVY" );
	    case D3DFMT_YUY2:				
	    	return _T( "YUY2" );
	    case D3DFMT_D16_LOCKABLE:		
	    	return _T( "D16_LOCKABLE" );
	    case D3DFMT_D32:				
	    	return _T( "D32" );
	    case D3DFMT_D15S1:				
	    	return _T( "D15S1" );
	    case D3DFMT_D24S8:				
	    	return _T( "D24S8" );
	    case D3DFMT_D24X8:				
	    	return _T( "D24X8" );
	    case D3DFMT_D24X4S4:			
	    	return _T( "D24X4S4" );
	    case D3DFMT_D32F_LOCKABLE:		
	    	return _T( "D32F_LOCKABLE" );
	    case D3DFMT_D24FS8:				
	    	return _T( "D24FS8" );
	    case D3DFMT_D16:				
	    	return _T( "D16" );
	    case D3DFMT_VERTEXDATA:			
	    	return _T( "VERTEXDATA" );
	    case D3DFMT_INDEX16:			
	    	return _T( "INDEX16" );
	    case D3DFMT_INDEX32:			
	    	return _T( "INDEX32" );
	    case D3DFMT_R16F:				
	    	return _T( "R16F" );
	    case D3DFMT_G16R16F:			
	    	return _T( "G16R16F" );
	    case D3DFMT_A16B16G16R16F:		
	    	return _T( "A16B16G16R16F" );
	    case D3DFMT_R32F:				
	    	return _T( "R32F" );
	    case D3DFMT_G32R32F:			
	    	return _T( "G32R32F" );
	    case D3DFMT_A32B32G32R32F:		
	    	return _T( "A32B32G32R32F" );
	    }

	    return _T( "-" );
    }

    // VertexElemenst から D3DVERTEXELEMENT9 の要素に変換
	void DX9Module::convertElementLNToDX( LNVertexElemenst* element_, u8* type_, u8* offset_, u8* usage_ )
	{
		*type_ = 0;
		*offset_ = 0;
		*usage_ = 0;

		switch ( element_->Type )
		{
			case LN_DECL_FLOAT1:
				*type_ = D3DDECLTYPE_FLOAT1;
				*offset_ = sizeof( float );
				break;

			case LN_DECL_FLOAT2:
				*type_ = D3DDECLTYPE_FLOAT2;
				*offset_ = sizeof( float ) * 2;
				break;

			case LN_DECL_FLOAT3:
				*type_ = D3DDECLTYPE_FLOAT3;
				*offset_ = sizeof( float ) * 3;
				break;

			case LN_DECL_FLOAT4:
				*type_ = D3DDECLTYPE_FLOAT4;
				*offset_ = sizeof( float ) * 4;
				break;

			case LN_DECL_UBYTE4:
				*type_ = D3DDECLTYPE_UBYTE4;
				*offset_ = sizeof( unsigned char ) * 4;
				break;

			case LN_DECL_COLOR4:
				*type_ = D3DDECLTYPE_D3DCOLOR;
				*offset_ = sizeof( unsigned char ) * 4;
				break;

			case LN_DECL_SHORT2:
				*type_ = D3DDECLTYPE_SHORT2;
				*offset_ = sizeof( short ) * 2;
				break;

			case LN_DECL_SHORT4:
				*type_ = D3DDECLTYPE_SHORT4;
				*offset_ = sizeof( short ) * 4;
				break;
		}

		switch ( element_->Usage )
		{
			case LN_USAGE_POSITION:
				*usage_ = D3DDECLUSAGE_POSITION;
				break;

			case LN_USAGE_NORMAL:
				*usage_ = D3DDECLUSAGE_NORMAL;
				break;

			case LN_USAGE_COLOR:
				*usage_ = D3DDECLUSAGE_COLOR;
				break;

			case LN_USAGE_TEXCOORD:
				*usage_ = D3DDECLUSAGE_TEXCOORD;
				break;

			case LN_USAGE_PSIZE:
				*usage_ = D3DDECLUSAGE_PSIZE;
				break;

            case LN_USAGE_BLENDINDICES:
				*usage_ = D3DDECLUSAGE_BLENDINDICES;
				break;

            case LN_USAGE_BLENDWEIGHT:
				*usage_ = D3DDECLUSAGE_BLENDWEIGHT;
				break;
		}
	}

    //---------------------------------------------------------------------
    // ● D3DVERTEXELEMENT9 の要素から VertexElemenst の要素に変換
    //---------------------------------------------------------------------
    LNVertexElemenst* DX9Module::convertElementArrayDXToLN( D3DVERTEXELEMENT9* dx_elm_ )
    {
        // 要素数チェック
        u32 num = 0;
        while ( true )
        {
            if ( dx_elm_[ num ].Stream == 0xff && dx_elm_[ num ].Type == D3DDECLTYPE_UNUSED )
            {
                break;
            }
            ++num;
        }
        ++num;

        LNVertexElemenst* ln_elm = LN_NEW LNVertexElemenst[ num ];
        
        for ( u32 i = 0; i < num; ++i )
        {
            ln_elm[ i ].StreamIndex = dx_elm_[ i ].Stream;
            ln_elm[ i ].UsageIndex  = dx_elm_[ i ].UsageIndex;
            
            switch ( dx_elm_[ i ].Type )
		    {
                case D3DDECLTYPE_FLOAT1:
                    ln_elm[ i ].Type = LN_DECL_FLOAT1;
				    break;

			    case D3DDECLTYPE_FLOAT2:
				    ln_elm[ i ].Type = LN_DECL_FLOAT2;
				    break;

			    case D3DDECLTYPE_FLOAT3:
				    ln_elm[ i ].Type = LN_DECL_FLOAT3;
				    break;

			    case D3DDECLTYPE_FLOAT4:
				    ln_elm[ i ].Type = LN_DECL_FLOAT4;
				    break;

			    case D3DDECLTYPE_UBYTE4:
				    ln_elm[ i ].Type = LN_DECL_UBYTE4;
				    break;

			    case D3DDECLTYPE_D3DCOLOR:
				    ln_elm[ i ].Type = LN_DECL_COLOR4;
				    break;

			    case D3DDECLTYPE_SHORT2:
				    ln_elm[ i ].Type = LN_DECL_SHORT2;
				    break;

			    case D3DDECLTYPE_SHORT4:
                    ln_elm[ i ].Type = LN_DECL_SHORT4;
				    break;

                default:
                    ln_elm[ i ].Type = LN_DECL_UNKNOWN;
                    break;
            };

            switch ( dx_elm_[ i ].Usage )
		    {
                case D3DDECLUSAGE_POSITION:
				    ln_elm[ i ].Usage = LN_USAGE_POSITION;
				    break;

			    case D3DDECLUSAGE_NORMAL:
				    ln_elm[ i ].Usage = LN_USAGE_NORMAL;
				    break;

			    case D3DDECLUSAGE_COLOR:
				    ln_elm[ i ].Usage = LN_USAGE_COLOR;
				    break;

			    case D3DDECLUSAGE_TEXCOORD:
				    ln_elm[ i ].Usage = LN_USAGE_TEXCOORD;
				    break;

			    case D3DDECLUSAGE_PSIZE:
				    ln_elm[ i ].Usage = LN_USAGE_PSIZE;
				    break;

                case D3DDECLUSAGE_BLENDINDICES:
				    ln_elm[ i ].Usage = LN_USAGE_BLENDINDICES;
				    break;

                case D3DDECLUSAGE_BLENDWEIGHT:
				    ln_elm[ i ].Usage = LN_USAGE_BLENDWEIGHT;
				    break;

                default:
                    ln_elm[ i ].Usage = LN_USAGE_UNKNOWN;
				    break;
            };
        }

        return ln_elm;
    }

    //---------------------------------------------------------------------
    // ● LNSurfaceFormat から D3DFORMAT に変換
    //---------------------------------------------------------------------
    D3DFORMAT DX9Module::convertLNFormatToDxFormat( LNSurfaceFormat format_ )
    {
        switch ( format_ )
        {
            case LN_FMT_A8R8G8B8:       
            	return D3DFMT_A8R8G8B8;
            case LN_FMT_X8R8G8B8:       
            	return D3DFMT_X8R8G8B8;
            case LN_FMT_A16B16G16R16F:  
            	return D3DFMT_A16B16G16R16F;
            case LN_FMT_A32B32G32R32F:  
            	return D3DFMT_A32B32G32R32F;
            case LN_FMT_D24S8:          
            	return D3DFMT_D24S8;
        }

        return D3DFMT_UNKNOWN;
    }

    //---------------------------------------------------------------------
    // ● D3DFORMAT から LNSurfaceFormat に変換
    //---------------------------------------------------------------------
    LNSurfaceFormat DX9Module::convertFormatDxToLN( D3DFORMAT dx_format_ )
    {
        switch ( dx_format_ )
        {
            case D3DFMT_A8B8G8R8:       
            	return LN_FMT_A8R8G8B8;
            case D3DFMT_X8B8G8R8:       
            	return LN_FMT_X8R8G8B8;
            case D3DFMT_A16B16G16R16F:  
            	return LN_FMT_A16B16G16R16F;
            case D3DFMT_D24S8:          
            	return LN_FMT_D24S8;
        }

        return LN_FMT_UNKNOWN;
    }

    //---------------------------------------------------------------------
    // ● X ファイルから LightNote 用のオブジェクトを作成する
    //---------------------------------------------------------------------
    //LNRESULT DX9Module::createMeshFromXInMemory( const void* data_, u32 size_, IGraphicsDevice* device_, Material** materials_, char** tex_names_, u32* material_num_, IVertexBuffer** vb_, IIndexBuffer** ib_ )
    //{

    //    return LN_OK;
    //}

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