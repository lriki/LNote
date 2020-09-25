//=============================================================================
//【 AllocateHierarchy 】
//-----------------------------------------------------------------------------
///**
//  @file       AllocateHierarchy.h
//  @brief      AllocateHierarchy
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include <string>

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

struct DerivedD3DXFrame
    : public D3DXFRAME
{
	lnU32       Index;     ///< 連番

    DerivedD3DXFrame()
    {
        Name             = NULL;
        D3DXMatrixIdentity( &TransformationMatrix );
		pMeshContainer	 = NULL;
		pFrameSibling	 = NULL;
		pFrameFirstChild = NULL;

        Index = 0;
    }
};


struct DerivedD3DXMeshContainer
    : public D3DXMESHCONTAINER
{
    std::vector< LRefTString >  TextureNames;       ///< テクスチャ名 (マテリアル配列と対応)
    ID3DXMesh*		            OriginalMesh;       ///< 元のメッシュ (念のため用意。スキニング方法ごとのメッシュ変換やソフトウェアスキニングで使う )
    D3DXMATRIX*		            BoneOffsetMatrices;	///< ボーンオフセット行列の配列 ( 行列更新する時の元になる初期姿勢 )

    //DWORD NumAttributeGroups;
    //LPD3DXATTRIBUTERANGE pAttributeTable;

    DerivedD3DXMeshContainer()
	{
        Name = NULL;
        MeshData.pMesh = NULL;
        pMaterials = NULL;
        pEffects = NULL;
        NumMaterials = 0;
        pAdjacency = NULL;
        pSkinInfo = NULL;

		OriginalMesh = NULL;
        BoneOffsetMatrices = NULL;

        //NumAttributeGroups = 0;
        //pAttributeTable = NULL;
	}
};

typedef std::vector< DerivedD3DXFrame* >            DerivedD3DXFrameArray;
typedef std::vector< DerivedD3DXMeshContainer* >    DerivedD3DXMeshContainerArray;

//=============================================================================
// ■ AllocateHierarchy
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class AllocateHierarchy
    : public ID3DXAllocateHierarchy
{
public:

	////!< マテリアルとファイル名を覚えておくための構造体
	//struct MaterialEntry
	//{
	//	LMaterial	material;
	//	std::string	texture_name;

	//	bool operator == ( const MaterialEntry& obj_ )
	//	{
	//		if ( texture_name == obj_.texture_name )
	//		{
	//			return ( material == obj_.material );
	//		}
	//		return false;
	//	}
	//};

	////!< ↑の構造体の配列
	//typedef std::vector< MaterialEntry > MaterialArray;

public:

	/// コンストラクタ
	AllocateHierarchy();

public:
	
	/// フレーム作成コールバック ( CreateFrame() )
	STDMETHOD( CreateFrame )( const char* name_, D3DXFRAME** out_new_frame_ );

	/// メッシュコンテナ作成コールバック ( CreateMeshContainer() )
	STDMETHOD( CreateMeshContainer )(
		const char* name_,
		const D3DXMESHDATA* dx_mesh_data_,
        const D3DXMATERIAL* dx_materials,
		const D3DXEFFECTINSTANCE* dx_effect_instances,
		DWORD material_num_,
        const DWORD* adjacency_,
		ID3DXSkinInfo* dx_skin_info_,
        D3DXMESHCONTAINER** new_mesh_container_ );

	/// フレーム解放コールバック( DestroyFrame() )
	STDMETHOD( DestroyFrame )( D3DXFRAME* frame_to_free_ );

	/// メッシュコンテナ解放コールバック( DestroyMeshContainer() )
	STDMETHOD( DestroyMeshContainer )( D3DXMESHCONTAINER* mesh_container_base_ );

public:

    /// 全フレームの配列の取得
    DerivedD3DXFrameArray& getDerivedD3DXFrameArray() { return mDerivedD3DXFrameArray; }

    /// 全メッシュコンテナの配列の取得
    DerivedD3DXMeshContainerArray& getDerivedD3DXMeshContainerArray() { return mDerivedD3DXMeshContainerArray; }

    /// 全メッシュコンテナのマテリアル数の合計
    lnU32 getAllMaterialNum() const { return mAllMaterialNum; }

private:

	/// メモリを確保して文字列を格納する (フレーム等の名前格納用)
	HRESULT _allocateName( const char* name_, char** out_name_ );

    /// メッシュに法線・頂点色があるかどうか調べて、無ければメッシュを作りなおす
	HRESULT _checkMeshFormat( ID3DXMesh* dx_mesh_, bool* converted_, ID3DXMesh** out_mesh_ );
	
    /// マテリアル情報・隣接情報をコンテナに登録する
	HRESULT _registMaterialData(
		lnU32 face_num_,
		const DWORD* adjacency_,
		lnU32 material_num_,
		const D3DXMATERIAL* materials_,
		DerivedD3DXMeshContainer* out_ );

    /// スキニング情報をコンテナに登録する
	HRESULT _registSkinInfo( ID3DXSkinInfo* dx_skin_info_, DerivedD3DXMeshContainer* out_ );

    /// スキンメッシュを作成する
	HRESULT _generateSkinnedMesh( DerivedD3DXMeshContainer* out_ );

private:

    lnU32	    mFrameNum;      ///< フレームの数 (DerivedD3DXFrame に連番を付けるために使う)
    DerivedD3DXFrameArray           mDerivedD3DXFrameArray;
    DerivedD3DXMeshContainerArray   mDerivedD3DXMeshContainerArray;

    lnU32       mAllMaterialNum;
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