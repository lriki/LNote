//=============================================================================
//�y AllocateHierarchy �z
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
// �� AllocateHierarchy
//=============================================================================

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
	//---------------------------------------------------------------------
	AllocateHierarchy::AllocateHierarchy()
        : mFrameNum         ( 0 )
        , mAllMaterialNum   ( 0 )
	{

	}

	//---------------------------------------------------------------------
	// �� �t���[���쐬�R�[���o�b�N
	//---------------------------------------------------------------------
	HRESULT AllocateHierarchy::CreateFrame( const char* name_, D3DXFRAME** out_new_frame_ )
	{
		HRESULT hr = S_OK;

        // �܂��͋�̃t���[�����쐬����
		DerivedD3DXFrame* dx_frame = LN_NEW DerivedD3DXFrame();
		if ( dx_frame == NULL )
		{
			hr = E_OUTOFMEMORY;
			goto ERR_EXIT;
		}

        // �t���[���̖��O���t���[���Ɋi�[
		hr = _allocateName( name_, &dx_frame->Name );
		if ( FAILED( hr ) ) { 
			goto ERR_EXIT; }

		// �o���オ�����t���[����Ԃ�
		*out_new_frame_ = dx_frame;

		// �ԍ�������
		dx_frame->Index = mFrameNum;
		++mFrameNum;

        mDerivedD3DXFrameArray.push_back( dx_frame );
		return S_OK;

	ERR_EXIT:
		DestroyFrame( dx_frame );
		return hr;
	}

	//---------------------------------------------------------------------
	// �� ���b�V���R���e�i�쐬�R�[���o�b�N
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

        // �p�b�`�E�v���O���b�V�u���b�V���͈���Ȃ�
		if ( dx_mesh_data_->Type != D3DXMESHTYPE_MESH ) { 
	goto ERR_EXIT; }

        // FVF�݊��łȂ����b�V���͈���Ȃ�
		if ( dx_mesh_data_->pMesh->GetFVF() == 0 ) { 
			goto ERR_EXIT; }
		
		// ���b�V���f�[�^�\���̒��� LPD3DXMESH �ւ̃|�C���^�𓾂�
		ID3DXMesh* dx_mesh = dx_mesh_data_->pMesh;

        // ���b�V���R���e�i���쐬����
		DerivedD3DXMeshContainer* mesh_container = LN_NEW DerivedD3DXMeshContainer();
		if ( !mesh_container )
		{
			hr = E_OUTOFMEMORY;
			goto ERR_EXIT;
		}

		// �����Ŏ󂯎�������b�V���R���e�i�̖��O��ۑ�
		hr = _allocateName( name_, &mesh_container->Name );
		if ( FAILED( hr ) ) goto ERR_EXIT;

        // ���b�V���ɖ@���E���_�F�����邩���ׂāA������Βǉ�����
        mesh_container->MeshData.Type = D3DXMESHTYPE_MESH;
		bool converted;
        ID3DXMesh* new_mesh;
		hr = _checkMeshFormat( dx_mesh, &converted, &new_mesh );
		if ( FAILED( hr ) ) goto ERR_EXIT;
		// �@���܂��͒��_�F���ǉ�����Ȃ������ꍇ
		if ( !converted )
		{
            mesh_container->MeshData.pMesh = dx_mesh;
			dx_mesh->AddRef();
		}
        else
        {
            mesh_container->MeshData.pMesh = new_mesh;
        }
		
        // �����Ŏ󂯎�����}�e���A���̏������b�V���R���e�i�Ɋi�[
		hr = _registMaterialData( dx_mesh->GetNumFaces(), adjacency_, material_num_, dx_materials, mesh_container );
		if ( FAILED( hr ) ) { 
			goto ERR_EXIT; }
		
        // �X�L�j���O�̏�񂪂���ꍇ
		if ( dx_skin_info_ != NULL )
		{
			// ���̃��b�V�����R���e�i�Ɋi�[���Ċo���Ă���
			mesh_container->OriginalMesh = dx_mesh;
			dx_mesh->AddRef();

			// �X�L�j���O�����R���e�i�ɓo�^����
			_registSkinInfo( dx_skin_info_, mesh_container );
		}

        // �쐬�������b�V���R���e�i�̃|�C���^��n��
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
	// �� �t���[������R�[���o�b�N
	//---------------------------------------------------------------------
	HRESULT AllocateHierarchy::DestroyFrame( D3DXFRAME* frame_to_free_ )
	{
        SAFE_DELETE_ARRAY( frame_to_free_->Name );
		SAFE_DELETE( frame_to_free_ );
		return S_OK;
	}

	//---------------------------------------------------------------------
	// �� ���b�V���R���e�i����R�[���o�b�N
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
	// �� ���������m�ۂ��ĕ�������i�[���� ( �t���[�����̖��O�i�[�p )
	//-------------------------------------------------------------------------
	HRESULT AllocateHierarchy::_allocateName( const char* name_, char** out_name_ )
	{
		if ( name_ != NULL )
		{
			u32 size = strlen( name_ ) + 1;
			*out_name_ = LN_NEW char[ size ];
			if ( *out_name_ == NULL )
			{
				// �������m�ێ��s
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
	// �� ���b�V���ɖ@���E���_�F�����邩�ǂ������ׂāA������΃��b�V�������Ȃ���
	//---------------------------------------------------------------------
	HRESULT AllocateHierarchy::_checkMeshFormat( ID3DXMesh* dx_mesh_, bool* converted_, ID3DXMesh** out_mesh_ )
	{
		//ID3DXMesh* dx_mesh = *dx_mesh_;
        *out_mesh_ = NULL;
		DWORD fvf = 0;
		*converted_ = false;

        // �@��
		if ( ( dx_mesh_->GetFVF() & D3DFVF_NORMAL ) == 0 )
		{
			fvf |= D3DFVF_NORMAL;
		}
        // ���_�J���[
		if ( ( dx_mesh_->GetFVF() & D3DFVF_DIFFUSE ) == 0 )
		{
			fvf |= D3DFVF_DIFFUSE;
		}
        // �e�N�X�`�����W
		if ( ( dx_mesh_->GetFVF() & D3DFVF_TEX1 ) == 0 )
		{
			fvf |= D3DFVF_TEX1;
		}

		// �ǉ����� FVF ������ꍇ
		if ( fvf != 0 )
		{
			// �f�o�C�X�ւ̃|�C���^���擾
			IDirect3DDevice9* dx_device;
			dx_mesh_->GetDevice( &dx_device );

			// �@���t���̃��b�V���𕡐�����
			HRESULT hr = dx_mesh_->CloneMeshFVF(
				dx_mesh_->GetOptions(), dx_mesh_->GetFVF() | fvf, 
				dx_device, out_mesh_ );
			if ( FAILED( hr  ) ) return hr;

			// �@�����v�Z����
			DirectX9::DX9Module::getInstance()->D3DXComputeNormals( *out_mesh_, NULL );

			*converted_ = true;
			dx_device->Release();
		}

		return S_OK;
	}

    //---------------------------------------------------------------------
	// �� �}�e���A�����E�אڏ����R���e�i�ɓo�^����
	//---------------------------------------------------------------------
	HRESULT AllocateHierarchy::_registMaterialData(
		lnU32 face_num_,
		const DWORD* adjacency_,
		lnU32 material_num_,
		const D3DXMATERIAL* materials_,
		DerivedD3DXMeshContainer* out_ )
	{
        // �}�e���A���� (�Œ�1�쐬)
        out_->NumMaterials = (((1) > (material_num_)) ? (1) : (material_num_));

        mAllMaterialNum += out_->NumMaterials;

		// �������m��
        out_->pAdjacency = LN_NEW DWORD[ face_num_ * 3 ];
        out_->pMaterials = LN_NEW D3DXMATERIAL[ out_->NumMaterials ];
        if( ( out_->pAdjacency == NULL ) || ( out_->pMaterials == NULL ) )
        {
            return E_OUTOFMEMORY;
        }

        // �אڏ����R�s�[
        memcpy( out_->pAdjacency, adjacency_, sizeof( DWORD ) * face_num_ * 3 );

        // �󂯎�����}�e���A�����R�s�[����
		if ( material_num_ > 0 )
		{
            // �܂��͊ۂ��ƃR�s�[
            memcpy( out_->pMaterials, materials_, sizeof( D3DXMATERIAL ) * material_num_ );

            // �e�N�X�`������ۑ�����
            for ( lnU32 i = 0; i < material_num_; ++i )
            {
                // �A���r�G���g�ɂ̓f�B�t���[�Y���g��
				//out_->pMaterials[ i ].Ambient = out_->pMaterials[ i ].Diffuse;

                if ( out_->pMaterials[ i ].pTextureFilename != NULL )
                {
                    out_->TextureNames.push_back( LRefTString( out_->pMaterials[ i ].pTextureFilename ) );
                    out_->pMaterials[ i ].pTextureFilename = NULL;
                }
                else
                {
                    // �e�N�X�`�������Ȃ��ꍇ�ł��A��̕���������Ă���
                    // �������Ȃ��ƁA�}�e���A�����ƃe�N�X�`������������Ȃ��Ȃ�
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
	// �� �X�L�j���O�����R���e�i�ɓo�^����
	//---------------------------------------------------------------------
	HRESULT AllocateHierarchy::_registSkinInfo( ID3DXSkinInfo* dx_skin_info_, DerivedD3DXMeshContainer* out_ )
	{
		// �X�L�������i�[
		out_->pSkinInfo = dx_skin_info_;
		dx_skin_info_->AddRef();

		// ���̂̃��[�J����Ԃ���{�[����Ԃɒ��_���ړ����邽�߂̃I�t�Z�b�g�s��p�̔z���p�ӂ���
		lnU32 bone_num_ = dx_skin_info_->GetNumBones();
		out_->BoneOffsetMatrices = LN_NEW D3DXMATRIX[ bone_num_ ];
		if ( !out_->BoneOffsetMatrices ) { 
			return E_OUTOFMEMORY; }

		// ���ꂼ��̃{�[���̃I�t�Z�b�g�s������o���ĕۑ�����
		for ( lnU32 i = 0; i < bone_num_; ++i )
		{
			out_->BoneOffsetMatrices[ i ] = *( dx_skin_info_->GetBoneOffsetMatrix( i ) );
		}

		// �X�L�j���O�ɓs���̂悢��ʓI�ȃX�L�j���O���ƍ��W�ϊ��𐶐�����
		return _generateSkinnedMesh( out_ );
	}

    //---------------------------------------------------------------------
	// �� �X�L�����b�V�����쐬����
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

            // �������擾
            hr = out_->MeshData.pMesh->GetAttributeTable( NULL, &out_->NumAttributeGroups );
            if( FAILED( hr ) )
                goto ERR_EXIT;

            // �����̃������m��
            //delete[] pMeshContainer->pAttributeTable;
            SAFE_DELETE_ARRAY( out_->pAttributeTable );
            out_->pAttributeTable = LN_NEW D3DXATTRIBUTERANGE[ out_->NumAttributeGroups ];
            if( out_->pAttributeTable == NULL )
            {
                hr = E_OUTOFMEMORY;
                goto ERR_EXIT;
            }

            // �������R�s�[
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
			// ��x�Ɏg���{�[���s��̐�����
			u32 max_matrices = 26;
			out_->mPaletteEntriesNum = min( max_matrices, out_->pSkinInfo->GetNumBones() );

			// ���b�V���œK���̎�ނ����߂�
			// ( DirectX �̃T���v���ł́Ahlsl 1.1 ���O�̃o�[�W�����ł�
			//   �\�t�g�E�F�A���������˂�悤�ɂ��Ă邯�ǁA����̓J�b�g )
			DWORD Flags = D3DXMESHOPT_VERTEXCACHE | D3DXMESH_MANAGED;	// �L���b�V���q�b�g���A�b�v�̂��߂ɖʏ��ԕ��בւ� + MANAGED
			
			// ���̃��b�V���͂��̊֐��I����͎g��Ȃ��̂ŁA
			// �X�L�����b�V���쐬��ɉ�������悤�ɎQ�ƃJ�E���g�����炵�Ă���
			SAFE_RELEASE( out_->MeshData.pMesh );

			// ���_�P�ʂ̃u�����h�̏d�݂ƃC���f�b�N�X�ƃ{�[���̑g�ݍ��킹�e�[�u����K�p�����V�������b�V�������
			hr = out_->pSkinInfo->ConvertToIndexedBlendedMesh(
				out_->mOrigMesh,
				Flags,
				out_->mPaletteEntriesNum,
				out_->pAdjacency,
				NULL, NULL, NULL,
				&out_->mInflNum,			// �{�[���̉e���� 1 �ʂ�����̍ő吔
				&out_->mAttributeGroupsNum,	// �{�[���̑g�ݍ��킹�e�[�u���Ɋ܂܂��{�[���̐��ւ̃|�C���^
				&out_->mBoneCombinationBuf,	// �{�[���̑g�ݍ��킹�e�[�u���ւ̃|�C���^
				&out_->MeshData.pMesh );
			if ( FAILED( hr ) ) { 
				goto ERR_EXIT; }


			// �@�������邩�ǂ������ׂ� ( �����ꍇ�͎��� FVF �ϊ���ɖ@�����Čv�Z����t���O�� true �� )
			bool req_compute_normals = ( ( out_->MeshData.pMesh->GetFVF() & D3DFVF_NORMAL ) == 0 );

			// ���_�F�����邩�ǂ������ׂ�
			bool use_vertex_color = ( ( out_->MeshData.pMesh->GetFVF() & D3DFVF_DIFFUSE ) != 0 );

			// �K�v������΍œK�� FVF �ɕϊ�
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
					// �ϊ��O�ɖ@�����Ȃ������ꍇ�͖@�����v�Z����
					if ( req_compute_normals )
					{
						D3DXComputeNormals( dx_mesh, NULL );
					}
					// �ϊ��O�ɒ��_�F���Ȃ������ꍇ�͔��Ŗ��߂�
					//if ( !use_vertex_color )
					//{
					//	//LVertex
					//	//LockVertexBuffer
					//	printf( "use_vertex_color\n" );
					//}

					// �ϊ��O�ɒ��_�F���Ȃ������ꍇ�͔��Ŗ��߂�
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
			
			// ���b�V���̒��_�錾���擾
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
			

			// �ŏ��� D3DDECLUSAGE_BLENDINDICES �� D3DDECLTYPE_D3DCOLOR �Ƃ��Ĉ����݂�����
			// ( ���ꂪ�Ȃ��ƃ��f��������� )
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

			// �{�[���̍ő吔���o���Ă���
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