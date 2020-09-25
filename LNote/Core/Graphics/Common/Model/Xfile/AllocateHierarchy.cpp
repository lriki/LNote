//=============================================================================
//【 AllocateHierarchy 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../../../Math/Math.h"
#include "../../../DirectX9/DX9Module.h"
#include "AllocateHierarchy.h"

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

//=============================================================================
// ■ AllocateHierarchy
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	AllocateHierarchy::AllocateHierarchy()
        : mFrameNum         ( 0 )
        , mAllMaterialNum   ( 0 )
	{

	}

	//---------------------------------------------------------------------
	// ● フレーム作成コールバック
	//---------------------------------------------------------------------
	HRESULT AllocateHierarchy::CreateFrame( const char* name_, D3DXFRAME** out_new_frame_ )
	{
		HRESULT hr = S_OK;

        // まずは空のフレームを作成する
		DerivedD3DXFrame* dx_frame = LN_NEW DerivedD3DXFrame();
		if ( dx_frame == NULL )
		{
			hr = E_OUTOFMEMORY;
			goto ERR_EXIT;
		}

        // フレームの名前をフレームに格納
		hr = _allocateName( name_, &dx_frame->Name );
		if ( FAILED( hr ) ) { 
			goto ERR_EXIT; }

		// 出来上がったフレームを返す
		*out_new_frame_ = dx_frame;

		// 番号をつける
		dx_frame->Index = mFrameNum;
		++mFrameNum;

        mDerivedD3DXFrameArray.push_back( dx_frame );
		return S_OK;

	ERR_EXIT:
		DestroyFrame( dx_frame );
		return hr;
	}

	//---------------------------------------------------------------------
	// ● メッシュコンテナ作成コールバック
	//---------------------------------------------------------------------
	HRESULT AllocateHierarchy::CreateMeshContainer(
		const char* name_,
		const D3DXMESHDATA* dx_mesh_data_,
        const D3DXMATERIAL* dx_materials,
		const D3DXEFFECTINSTANCE* dx_effect_instances,
		DWORD material_num_,
        const DWORD* adjacency_,
		ID3DXSkinInfo* dx_skin_info_,
        D3DXMESHCONTAINER** new_mesh_container_ )
	{
        HRESULT hr = S_OK;

        // パッチ・プログレッシブメッシュは扱わない
		if ( dx_mesh_data_->Type != D3DXMESHTYPE_MESH ) { 
	goto ERR_EXIT; }

        // FVF互換でないメッシュは扱わない
		if ( dx_mesh_data_->pMesh->GetFVF() == 0 ) { 
			goto ERR_EXIT; }
		
		// メッシュデータ構造体中の LPD3DXMESH へのポインタを得る
		ID3DXMesh* dx_mesh = dx_mesh_data_->pMesh;

        // メッシュコンテナを作成する
		DerivedD3DXMeshContainer* mesh_container = LN_NEW DerivedD3DXMeshContainer();
		if ( !mesh_container )
		{
			hr = E_OUTOFMEMORY;
			goto ERR_EXIT;
		}

		// 引数で受け取ったメッシュコンテナの名前を保存
		hr = _allocateName( name_, &mesh_container->Name );
		if ( FAILED( hr ) ) goto ERR_EXIT;

        // メッシュに法線・頂点色があるか調べて、無ければ追加する
        mesh_container->MeshData.Type = D3DXMESHTYPE_MESH;
		bool converted;
        ID3DXMesh* new_mesh;
		hr = _checkMeshFormat( dx_mesh, &converted, &new_mesh );
		if ( FAILED( hr ) ) goto ERR_EXIT;
		// 法線または頂点色が追加されなかった場合
		if ( !converted )
		{
            mesh_container->MeshData.pMesh = dx_mesh;
			dx_mesh->AddRef();
		}
        else
        {
            mesh_container->MeshData.pMesh = new_mesh;
        }
		
        // 引数で受け取ったマテリアルの情報をメッシュコンテナに格納
		hr = _registMaterialData( dx_mesh->GetNumFaces(), adjacency_, material_num_, dx_materials, mesh_container );
		if ( FAILED( hr ) ) { 
			goto ERR_EXIT; }
		
        // スキニングの情報がある場合
		if ( dx_skin_info_ != NULL )
		{
			// 元のメッシュをコンテナに格納して覚えておく
			mesh_container->OriginalMesh = dx_mesh;
			dx_mesh->AddRef();

			// スキニング情報をコンテナに登録する
			_registSkinInfo( dx_skin_info_, mesh_container );
		}

        // 作成したメッシュコンテナのポインタを渡す
        mDerivedD3DXMeshContainerArray.push_back( mesh_container );
		*new_mesh_container_ = mesh_container;
		mesh_container = NULL;

		return S_OK;

    ERR_EXIT:

		if ( mesh_container )
		{
			DestroyMeshContainer( mesh_container );
		}
		return hr;
	}

	//---------------------------------------------------------------------
	// ● フレーム解放コールバック
	//---------------------------------------------------------------------
	HRESULT AllocateHierarchy::DestroyFrame( D3DXFRAME* frame_to_free_ )
	{
        SAFE_DELETE_ARRAY( frame_to_free_->Name );
		SAFE_DELETE( frame_to_free_ );
		return S_OK;
	}

	//---------------------------------------------------------------------
	// ● メッシュコンテナ解放コールバック
	//---------------------------------------------------------------------
	HRESULT AllocateHierarchy::DestroyMeshContainer( D3DXMESHCONTAINER* mesh_container_base_ )
	{
        DerivedD3DXMeshContainer* mesh_container = (DerivedD3DXMeshContainer*)mesh_container_base_;

        SAFE_DELETE_ARRAY( mesh_container->Name );
        SAFE_DELETE_ARRAY( mesh_container->pAdjacency );
        SAFE_DELETE_ARRAY( mesh_container->pMaterials );

        SAFE_RELEASE( mesh_container->OriginalMesh );
        SAFE_DELETE_ARRAY( mesh_container->BoneOffsetMatrices );

        //SAFE_DELETE_ARRAY( mesh_container->pAttributeTable );

        SAFE_RELEASE( mesh_container->MeshData.pMesh );
        SAFE_RELEASE( mesh_container->pSkinInfo );

        SAFE_DELETE( mesh_container );
		return S_OK;
	}

    //-------------------------------------------------------------------------
	// ● メモリを確保して文字列を格納する ( フレーム等の名前格納用 )
	//-------------------------------------------------------------------------
	HRESULT AllocateHierarchy::_allocateName( const char* name_, char** out_name_ )
	{
		if ( name_ != NULL )
		{
			u32 size = strlen( name_ ) + 1;
			*out_name_ = LN_NEW char[ size ];
			if ( *out_name_ == NULL )
			{
				// メモリ確保失敗
				return E_OUTOFMEMORY;
			}
			memcpy( *out_name_, name_, size * sizeof(char) );
		}
		else
		{
			*out_name_ = NULL;
		}
		return S_OK;
	}

    //---------------------------------------------------------------------
	// ● メッシュに法線・頂点色があるかどうか調べて、無ければメッシュを作りなおす
	//---------------------------------------------------------------------
	HRESULT AllocateHierarchy::_checkMeshFormat( ID3DXMesh* dx_mesh_, bool* converted_, ID3DXMesh** out_mesh_ )
	{
		//ID3DXMesh* dx_mesh = *dx_mesh_;
        *out_mesh_ = NULL;
		DWORD fvf = 0;
		*converted_ = false;

        // 法線
		if ( ( dx_mesh_->GetFVF() & D3DFVF_NORMAL ) == 0 )
		{
			fvf |= D3DFVF_NORMAL;
		}
        // 頂点カラー
		if ( ( dx_mesh_->GetFVF() & D3DFVF_DIFFUSE ) == 0 )
		{
			fvf |= D3DFVF_DIFFUSE;
		}
        // テクスチャ座標
		if ( ( dx_mesh_->GetFVF() & D3DFVF_TEX1 ) == 0 )
		{
			fvf |= D3DFVF_TEX1;
		}

		// 追加する FVF がある場合
		if ( fvf != 0 )
		{
			// デバイスへのポインタを取得
			IDirect3DDevice9* dx_device;
			dx_mesh_->GetDevice( &dx_device );

			// 法線付きのメッシュを複製する
			HRESULT hr = dx_mesh_->CloneMeshFVF(
				dx_mesh_->GetOptions(), dx_mesh_->GetFVF() | fvf, 
				dx_device, out_mesh_ );
			if ( FAILED( hr  ) ) return hr;

			// 法線を計算する
			DirectX9::DX9Module::getInstance()->D3DXComputeNormals( *out_mesh_, NULL );

			*converted_ = true;
			dx_device->Release();
		}

		return S_OK;
	}

    //---------------------------------------------------------------------
	// ● マテリアル情報・隣接情報をコンテナに登録する
	//---------------------------------------------------------------------
	HRESULT AllocateHierarchy::_registMaterialData(
		lnU32 face_num_,
		const DWORD* adjacency_,
		lnU32 material_num_,
		const D3DXMATERIAL* materials_,
		DerivedD3DXMeshContainer* out_ )
	{
        // マテリアル数 (最低1つ作成)
        out_->NumMaterials = (((1) > (material_num_)) ? (1) : (material_num_));

        mAllMaterialNum += out_->NumMaterials;

		// メモリ確保
        out_->pAdjacency = LN_NEW DWORD[ face_num_ * 3 ];
        out_->pMaterials = LN_NEW D3DXMATERIAL[ out_->NumMaterials ];
        if( ( out_->pAdjacency == NULL ) || ( out_->pMaterials == NULL ) )
        {
            return E_OUTOFMEMORY;
        }

        // 隣接情報をコピー
        memcpy( out_->pAdjacency, adjacency_, sizeof( DWORD ) * face_num_ * 3 );

        // 受け取ったマテリアルをコピーする
		if ( material_num_ > 0 )
		{
            // まずは丸ごとコピー
            memcpy( out_->pMaterials, materials_, sizeof( D3DXMATERIAL ) * material_num_ );

            // テクスチャ名を保存する
            for ( lnU32 i = 0; i < material_num_; ++i )
            {
                // アンビエントにはディフューズを使う
				//out_->pMaterials[ i ].Ambient = out_->pMaterials[ i ].Diffuse;

                if ( out_->pMaterials[ i ].pTextureFilename != NULL )
                {
                    out_->TextureNames.push_back( LRefTString( out_->pMaterials[ i ].pTextureFilename ) );
                    out_->pMaterials[ i ].pTextureFilename = NULL;
                }
                else
                {
                    // テクスチャ名がない場合でも、空の文字列を入れておく
                    // そうしないと、マテリアル数とテクスチャ名数が合わなくなる
                    out_->TextureNames.push_back( LRefTString() );
                }
            }
		}
        else
        {
            out_->pMaterials[0].pTextureFilename = NULL;
            memset( &out_->pMaterials[0].MatD3D, 0, sizeof( D3DMATERIAL9 ) );
            out_->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
            out_->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
            out_->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
            out_->pMaterials[0].MatD3D.Specular = out_->pMaterials[0].MatD3D.Diffuse;
        }
		return S_OK;
	}

    //---------------------------------------------------------------------
	// ● スキニング情報をコンテナに登録する
	//---------------------------------------------------------------------
	HRESULT AllocateHierarchy::_registSkinInfo( ID3DXSkinInfo* dx_skin_info_, DerivedD3DXMeshContainer* out_ )
	{
		// スキン情報を格納
		out_->pSkinInfo = dx_skin_info_;
		dx_skin_info_->AddRef();

		// 物体のローカル空間からボーン空間に頂点を移動するためのオフセット行列用の配列を用意する
		lnU32 bone_num_ = dx_skin_info_->GetNumBones();
		out_->BoneOffsetMatrices = LN_NEW D3DXMATRIX[ bone_num_ ];
		if ( !out_->BoneOffsetMatrices ) { 
			return E_OUTOFMEMORY; }

		// それぞれのボーンのオフセット行列を取り出して保存する
		for ( lnU32 i = 0; i < bone_num_; ++i )
		{
			out_->BoneOffsetMatrices[ i ] = *( dx_skin_info_->GetBoneOffsetMatrix( i ) );
		}

		// スキニングに都合のよい一般的なスキニング情報と座標変換を生成する
		return _generateSkinnedMesh( out_ );
	}

    //---------------------------------------------------------------------
	// ● スキンメッシュを作成する
	//---------------------------------------------------------------------
	HRESULT AllocateHierarchy::_generateSkinnedMesh( DerivedD3DXMeshContainer* out_ )
	{
		HRESULT hr = S_OK;

		IDirect3DDevice9* dx_device;
		out_->MeshData.pMesh->GetDevice( &dx_device );
		dx_device->Release();

		//SAFE_RELEASE( out_->MeshData.pMesh );
		//SAFE_RELEASE( out_->mBoneCombinationBuf );


        // if software skinning selected, use GenerateSkinnedMesh to create a mesh that can be used with UpdateSkinnedMesh
        //if( g_SkinningMethod == SOFTWARE )
        if ( 1 )
        {
            /*
            hr = out_->OriginalMesh->CloneMeshFVF(
                D3DXMESH_MANAGED, out_->OriginalMesh->GetFVF(),
                dx_device, &out_->MeshData.pMesh );
            if( FAILED( hr ) )
                goto ERR_EXIT;

            // 属性数取得
            hr = out_->MeshData.pMesh->GetAttributeTable( NULL, &out_->NumAttributeGroups );
            if( FAILED( hr ) )
                goto ERR_EXIT;

            // 属性のメモリ確保
            //delete[] pMeshContainer->pAttributeTable;
            SAFE_DELETE_ARRAY( out_->pAttributeTable );
            out_->pAttributeTable = LN_NEW D3DXATTRIBUTERANGE[ out_->NumAttributeGroups ];
            if( out_->pAttributeTable == NULL )
            {
                hr = E_OUTOFMEMORY;
                goto ERR_EXIT;
            }

            // 属性をコピー
            hr = out_->MeshData.pMesh->GetAttributeTable( out_->pAttributeTable, NULL );
            if( FAILED( hr ) )
                goto ERR_EXIT;
                */

            //_p( out_->OriginalMesh->GetFVF() );

            //_p(out_->pSkinInfo->GetBoneName(0));

            //_p(out_->pSkinInfo->GetNumBones());

            /*
            // allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
            if( g_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones() )
            {
                g_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

                // Allocate space for blend matrices
                delete[] g_pBoneMatrices;
                g_pBoneMatrices = new D3DXMATRIXA16[g_NumBoneMatricesMax];
                if( g_pBoneMatrices == NULL )
                {
                    hr = E_OUTOFMEMORY;
                    goto ERR_EXIT;
                }
            }
            */
        }












#if 0



		//if( ( g_SkinningMethod == D3DINDEXEDVS ) || ( g_SkinningMethod == D3DINDEXEDHLSLVS ) )
		if ( 1 )
		{
			// Get palette size
			// First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
			// (96 - 9) /3 i.e. Maximum constant count - used constants 
			// 一度に使うボーン行列の数かも
			u32 max_matrices = 26;
			out_->mPaletteEntriesNum = min( max_matrices, out_->pSkinInfo->GetNumBones() );

			// メッシュ最適化の種類を決める
			// ( DirectX のサンプルでは、hlsl 1.1 より前のバージョンでは
			//   ソフトウェア処理も兼ねるようにしてるけど、今回はカット )
			DWORD Flags = D3DXMESHOPT_VERTEXCACHE | D3DXMESH_MANAGED;	// キャッシュヒット率アップのために面順番並べ替え + MANAGED
			
			// 元のメッシュはこの関数終了後は使わないので、
			// スキンメッシュ作成後に解放されるように参照カウントを減らしておく
			SAFE_RELEASE( out_->MeshData.pMesh );

			// 頂点単位のブレンドの重みとインデックスとボーンの組み合わせテーブルを適用した新しいメッシュを作る
			hr = out_->pSkinInfo->ConvertToIndexedBlendedMesh(
				out_->mOrigMesh,
				Flags,
				out_->mPaletteEntriesNum,
				out_->pAdjacency,
				NULL, NULL, NULL,
				&out_->mInflNum,			// ボーンの影響の 1 面あたりの最大数
				&out_->mAttributeGroupsNum,	// ボーンの組み合わせテーブルに含まれるボーンの数へのポインタ
				&out_->mBoneCombinationBuf,	// ボーンの組み合わせテーブルへのポインタ
				&out_->MeshData.pMesh );
			if ( FAILED( hr ) ) { 
				goto ERR_EXIT; }


			// 法線があるかどうか調べる ( 無い場合は次の FVF 変換後に法線を再計算するフラグを true に )
			bool req_compute_normals = ( ( out_->MeshData.pMesh->GetFVF() & D3DFVF_NORMAL ) == 0 );

			// 頂点色があるかどうか調べる
			bool use_vertex_color = ( ( out_->MeshData.pMesh->GetFVF() & D3DFVF_DIFFUSE ) != 0 );

			// 必要があれば最適な FVF に変換
			DWORD new_fvf =
				( out_->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK ) |
				D3DFVF_NORMAL |
				D3DFVF_DIFFUSE |
				D3DFVF_TEX1 |
				D3DFVF_LASTBETA_UBYTE4;
			if ( new_fvf != out_->MeshData.pMesh->GetFVF() )
			{
				ID3DXMesh* dx_mesh;
				hr = out_->MeshData.pMesh->CloneMeshFVF(
					out_->MeshData.pMesh->GetOptions(),
					new_fvf, dx_device, &dx_mesh );

				if ( SUCCEEDED( hr ) )
				{
					// 変換前に法線がなかった場合は法線を計算する
					if ( req_compute_normals )
					{
						D3DXComputeNormals( dx_mesh, NULL );
					}
					// 変換前に頂点色がなかった場合は白で埋める
					//if ( !use_vertex_color )
					//{
					//	//LVertex
					//	//LockVertexBuffer
					//	printf( "use_vertex_color\n" );
					//}

					// 変換前に頂点色がなかった場合は白で埋める
					if ( !use_vertex_color )
					{
						IDirect3DVertexBuffer9* vb;
						out_->MeshData.pMesh->GetVertexBuffer( &vb );

						LVertex* v;
						u32 num = out_->MeshData.pMesh->GetNumVertices();
						//hr = t_mesh->LockVertexBuffer( 0, (void**)&v );
						vb->Lock( 0, 0, (void**)&v, 0 );

						for ( u32 i = 0; i < num; ++i )
						{
							v[ i ].mColor = 0xffffffff;
						}
						//t_mesh->UnlockVertexBuffer();
						vb->Unlock();
						SAFE_RELEASE( vb );
					}


					out_->MeshData.pMesh->Release();
					out_->MeshData.pMesh = dx_mesh;
					dx_mesh = NULL;
				}
			}
			
			// メッシュの頂点宣言を取得
			D3DVERTEXELEMENT9 decl[ MAX_FVF_DECL_SIZE ];
			
			hr = out_->MeshData.pMesh->GetDeclaration( decl );
			if ( FAILED( hr ) ) { 
				goto ERR_EXIT; }

			/*
			printf( "--------------------------------------\n" );
			int ii = 0;
			while ( 1 )
			{
				if ( decl[ ii ].Stream == 0xff ) { break; }

				printf( "-------------------\n" );
				wprintf( L"Stream:%d Offset:%d Type:%s Method:%s Usage:%s UsageIndex:%d\n",
					decl[ ii ].Stream,
					decl[ ii ].Offset,
					DXUTTraceD3DDECLTYPEtoString( decl[ ii ].Type ),
					DXUTTraceD3DDECLMETHODtoString( decl[ ii ].Method ),
					DXUTTraceD3DDECLUSAGEtoString( decl[ ii ].Usage ),
					decl[ ii ].UsageIndex );
				printf( "-------------------\n" );

				++ii;
			}
			printf( "--------------------------------------\n" );
			*/
			

			// 最初の D3DDECLUSAGE_BLENDINDICES は D3DDECLTYPE_D3DCOLOR として扱うみたいな
			// ( これがないとモデルが崩れる )
			D3DVERTEXELEMENT9* cur_decl = decl;
			while ( cur_decl->Stream != 0xff )
			{
				if ( ( cur_decl->Usage == D3DDECLUSAGE_BLENDINDICES ) && ( cur_decl->UsageIndex == 0 ) )
				{
					cur_decl->Type = D3DDECLTYPE_D3DCOLOR;
				}
				++cur_decl;
			}

			hr = out_->MeshData.pMesh->UpdateSemantics( decl );
			if ( FAILED( hr ) ) { 
				goto ERR_EXIT; }

			// ボーンの最大数を覚えておく
			if ( mBoneMatricesMaxNum < out_->pSkinInfo->GetNumBones() )
			{
				mBoneMatricesMaxNum = out_->pSkinInfo->GetNumBones();
			}
		}
#endif

	ERR_EXIT:
		return hr;
	}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================