//=============================================================================
//【 DX9Module 】
//-----------------------------------------------------------------------------
///**
//  @file       DX9Module.h
//  @brief      DX9Module
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <d3dx9.h>
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

//=============================================================================
// ■ DX9Module クラス
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
class DX9Module
    : private Base::NonCopyable
{
private:

    DX9Module() { mD3Dx9Module = NULL; }

    ~DX9Module();

public:

    /// インスタンスの取得
    static DX9Module* getInstance();

    /// 初期化
    LNRESULT initialize();

    /// Direct3DCreate9
    IDirect3D9* Direct3DCreate9( UINT SDKVersion );

    /// D3DXCreateTextureFromFileInMemoryEx
    HRESULT D3DXCreateTextureFromFileInMemoryEx( LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture );
	
    /// D3DXGetImageInfoFromFileInMemory
    HRESULT D3DXGetImageInfoFromFileInMemory( LPCVOID pSrcData, UINT SrcDataSize, D3DXIMAGE_INFO* pSrcInfo );

    /// D3DXCheckTextureRequirements
    HRESULT D3DXCheckTextureRequirements( LPDIRECT3DDEVICE9 pDevice, UINT *pWidth, UINT *pHeight, UINT *pNumMipLevels, DWORD Usage, D3DFORMAT *pFormat, D3DPOOL Pool );

    /// D3DXCreateEffect
    HRESULT D3DXCreateEffect( LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataLen, CONST D3DXMACRO* pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXEFFECT* ppEffect, LPD3DXBUFFER *ppCompilationErrors );

    /// D3DXLoadMeshFromXInMemory
    HRESULT D3DXLoadMeshFromXInMemory( LPCVOID Memory, DWORD SizeOfMemory, DWORD Options, LPDIRECT3DDEVICE9 pD3DDevice, LPD3DXBUFFER * ppAdjacency, LPD3DXBUFFER * ppMaterials, LPD3DXBUFFER * ppEffectInstances, DWORD * pNumMaterials, LPD3DXMESH * ppMesh );

    /// D3DXCreateFontIndirect
    HRESULT D3DXCreateFontIndirect( LPDIRECT3DDEVICE9 pDevice, CONST D3DXFONT_DESC * pDesc, LPD3DXFONT * ppFont );

    /// D3DXDeclaratorFromFVF
    HRESULT D3DXDeclaratorFromFVF( DWORD FVF, D3DVERTEXELEMENT9 pDeclarator[MAX_FVF_DECL_SIZE] );

    /// D3DXLoadMeshHierarchyFromXInMemory
    HRESULT D3DXLoadMeshHierarchyFromXInMemory( LPCVOID pMemory, DWORD SizeOfMemory, DWORD MeshOptions, LPDIRECT3DDEVICE9 pDevice, LPD3DXALLOCATEHIERARCHY pAlloc, LPD3DXLOADUSERDATA pUserDataLoader, LPD3DXFRAME* ppFrameHeirarchy, LPD3DXANIMATIONCONTROLLER* ppAnimController );
    
    /// D3DXFrameDestroy
    HRESULT D3DXFrameDestroy( LPD3DXFRAME pFrameRoot, LPD3DXALLOCATEHIERARCHY pAlloc );

    /// D3DXComputeNormals
    HRESULT D3DXComputeNormals( LPD3DXBASEMESH pMesh, const DWORD *pAdjacency );

public:

    /// D3DFORMAT の文字列表現の取得
    static const lnChar* getDxFormatString( D3DFORMAT Format );

    /// VertexElemenst から D3DVERTEXELEMENT9 の要素に変換
	static void convertElementLNToDX( LNVertexElemenst* element_, u8* type_, u8* offset_, u8* usage_ );

    /// D3DVERTEXELEMENT9 の要素から VertexElemenst の要素に変換 (要 delete[])
	static LNVertexElemenst* convertElementArrayDXToLN( D3DVERTEXELEMENT9* dx_elm_ );

    /// LNSurfaceFormat から D3DFORMAT に変換
    static D3DFORMAT convertLNFormatToDxFormat( LNSurfaceFormat format_ );

    /// D3DFORMAT から LNSurfaceFormat に変換
    static LNSurfaceFormat convertFormatDxToLN( D3DFORMAT dx_format_ );

    /// X ファイルからライブラリ用のオブジェクトを作成する
    //static LNRESULT createMeshFromXInMemory( const void* data_, u32 size_, IGraphicsDevice* device_, Material** materials_, char** tex_names_, u32* material_num_, IVertexBuffer** vb_, IIndexBuffer** ib_ );


private:

    typedef IDirect3D9 * ( WINAPI *MD_Direct3DCreate9 )( UINT SDKVersion );

    typedef HRESULT ( WINAPI *MD_D3DXCreateTextureFromFileInMemoryEx )(
		LPDIRECT3DDEVICE9         pDevice,
		LPCVOID                   pSrcData,
		UINT                      SrcDataSize,
		UINT                      Width,
		UINT                      Height,
		UINT                      MipLevels,
		DWORD                     Usage,
		D3DFORMAT                 Format,
		D3DPOOL                   Pool,
		DWORD                     Filter,
		DWORD                     MipFilter,
		D3DCOLOR                  ColorKey,
		D3DXIMAGE_INFO*           pSrcInfo,
		PALETTEENTRY*             pPalette,
		LPDIRECT3DTEXTURE9*       ppTexture);

    typedef HRESULT ( WINAPI *MD_D3DXGetImageInfoFromFileInMemory )(
        LPCVOID pSrcData,
        UINT SrcDataSize,
        D3DXIMAGE_INFO* pSrcInfo );

    typedef HRESULT ( WINAPI *MD_D3DXCheckTextureRequirements )(
        LPDIRECT3DDEVICE9 pDevice,
        UINT *pWidth,
        UINT *pHeight,
        UINT *pNumMipLevels,
        DWORD Usage,
        D3DFORMAT *pFormat,
        D3DPOOL Pool );

    typedef HRESULT ( WINAPI *MD_D3DXCreateEffect )(
        LPDIRECT3DDEVICE9 pDevice,
        LPCVOID pSrcData,
        UINT SrcDataLen,
        CONST D3DXMACRO* pDefines,
        LPD3DXINCLUDE pInclude,
        DWORD Flags,
        LPD3DXEFFECTPOOL pPool,
        LPD3DXEFFECT* ppEffect,
        LPD3DXBUFFER *ppCompilationErrors );

    typedef HRESULT ( WINAPI *MD_D3DXLoadMeshFromXInMemory )(
        LPCVOID Memory,
        DWORD SizeOfMemory,
        DWORD Options,
        LPDIRECT3DDEVICE9 pD3DDevice,
        LPD3DXBUFFER * ppAdjacency,
        LPD3DXBUFFER * ppMaterials,
        LPD3DXBUFFER * ppEffectInstances,
        DWORD * pNumMaterials,
        LPD3DXMESH * ppMesh );

    typedef HRESULT ( WINAPI *MD_D3DXCreateFontIndirect )(
        LPDIRECT3DDEVICE9 pDevice,
        CONST D3DXFONT_DESC * pDesc,
        LPD3DXFONT * ppFont );

    typedef HRESULT ( WINAPI *MD_D3DXDeclaratorFromFVF )(
        DWORD FVF,
        D3DVERTEXELEMENT9 pDeclarator[MAX_FVF_DECL_SIZE] );

    typedef HRESULT ( WINAPI *MD_D3DXLoadMeshHierarchyFromXInMemory )(
        LPCVOID pMemory,
        DWORD SizeOfMemory,
        DWORD MeshOptions,
        LPDIRECT3DDEVICE9 pDevice,
        LPD3DXALLOCATEHIERARCHY pAlloc,
        LPD3DXLOADUSERDATA pUserDataLoader,
        LPD3DXFRAME* ppFrameHeirarchy,
        LPD3DXANIMATIONCONTROLLER* ppAnimController );

    typedef HRESULT ( WINAPI *MD_D3DXFrameDestroy )(
        LPD3DXFRAME pFrameRoot,
        LPD3DXALLOCATEHIERARCHY pAlloc );

    typedef HRESULT ( WINAPI *MD_D3DXComputeNormals )(
        LPD3DXBASEMESH pMesh,
        const DWORD *pAdjacency );

private:

    HMODULE  mD3D9Module;       ///< "d3d9.dll"
    HMODULE  mD3Dx9Module;      ///< "d3dx9.dll"
    MD_Direct3DCreate9                       mDirect3DCreate9;
    MD_D3DXCreateTextureFromFileInMemoryEx   mD3DXCreateTextureFromFileInMemoryEx;
    MD_D3DXGetImageInfoFromFileInMemory      mD3DXGetImageInfoFromFileInMemory;
    MD_D3DXCheckTextureRequirements          mD3DXCheckTextureRequirements;
    MD_D3DXCreateEffect                      mD3DXCreateEffect;
    MD_D3DXLoadMeshFromXInMemory             mD3DXLoadMeshFromXInMemory;
    MD_D3DXCreateFontIndirect                mD3DXCreateFontIndirect;
    MD_D3DXDeclaratorFromFVF                 mD3DXDeclaratorFromFVF;
    MD_D3DXLoadMeshHierarchyFromXInMemory    mD3DXLoadMeshHierarchyFromXInMemory;
    MD_D3DXFrameDestroy                      mD3DXFrameDestroy;
    MD_D3DXComputeNormals                    mD3DXComputeNormals;
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