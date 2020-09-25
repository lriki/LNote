//=============================================================================
//�y AllocateHierarchy �z
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
	lnU32       Index;     ///< �A��

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
    std::vector< LRefTString >  TextureNames;       ///< �e�N�X�`���� (�}�e���A���z��ƑΉ�)
    ID3DXMesh*		            OriginalMesh;       ///< ���̃��b�V�� (�O�̂��ߗp�ӁB�X�L�j���O���@���Ƃ̃��b�V���ϊ���\�t�g�E�F�A�X�L�j���O�Ŏg�� )
    D3DXMATRIX*		            BoneOffsetMatrices;	///< �{�[���I�t�Z�b�g�s��̔z�� ( �s��X�V���鎞�̌��ɂȂ鏉���p�� )

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
// �� AllocateHierarchy
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class AllocateHierarchy
    : public ID3DXAllocateHierarchy
{
public:

	////!< �}�e���A���ƃt�@�C�������o���Ă������߂̍\����
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

	////!< ���̍\���̂̔z��
	//typedef std::vector< MaterialEntry > MaterialArray;

public:

	/// �R���X�g���N�^
	AllocateHierarchy();

public:
	
	/// �t���[���쐬�R�[���o�b�N ( CreateFrame() )
	STDMETHOD( CreateFrame )( const char* name_, D3DXFRAME** out_new_frame_ );

	/// ���b�V���R���e�i�쐬�R�[���o�b�N ( CreateMeshContainer() )
	STDMETHOD( CreateMeshContainer )(
		const char* name_,
		const D3DXMESHDATA* dx_mesh_data_,
        const D3DXMATERIAL* dx_materials,
		const D3DXEFFECTINSTANCE* dx_effect_instances,
		DWORD material_num_,
        const DWORD* adjacency_,
		ID3DXSkinInfo* dx_skin_info_,
        D3DXMESHCONTAINER** new_mesh_container_ );

	/// �t���[������R�[���o�b�N( DestroyFrame() )
	STDMETHOD( DestroyFrame )( D3DXFRAME* frame_to_free_ );

	/// ���b�V���R���e�i����R�[���o�b�N( DestroyMeshContainer() )
	STDMETHOD( DestroyMeshContainer )( D3DXMESHCONTAINER* mesh_container_base_ );

public:

    /// �S�t���[���̔z��̎擾
    DerivedD3DXFrameArray& getDerivedD3DXFrameArray() { return mDerivedD3DXFrameArray; }

    /// �S���b�V���R���e�i�̔z��̎擾
    DerivedD3DXMeshContainerArray& getDerivedD3DXMeshContainerArray() { return mDerivedD3DXMeshContainerArray; }

    /// �S���b�V���R���e�i�̃}�e���A�����̍��v
    lnU32 getAllMaterialNum() const { return mAllMaterialNum; }

private:

	/// ���������m�ۂ��ĕ�������i�[���� (�t���[�����̖��O�i�[�p)
	HRESULT _allocateName( const char* name_, char** out_name_ );

    /// ���b�V���ɖ@���E���_�F�����邩�ǂ������ׂāA������΃��b�V�������Ȃ���
	HRESULT _checkMeshFormat( ID3DXMesh* dx_mesh_, bool* converted_, ID3DXMesh** out_mesh_ );
	
    /// �}�e���A�����E�אڏ����R���e�i�ɓo�^����
	HRESULT _registMaterialData(
		lnU32 face_num_,
		const DWORD* adjacency_,
		lnU32 material_num_,
		const D3DXMATERIAL* materials_,
		DerivedD3DXMeshContainer* out_ );

    /// �X�L�j���O�����R���e�i�ɓo�^����
	HRESULT _registSkinInfo( ID3DXSkinInfo* dx_skin_info_, DerivedD3DXMeshContainer* out_ );

    /// �X�L�����b�V�����쐬����
	HRESULT _generateSkinnedMesh( DerivedD3DXMeshContainer* out_ );

private:

    lnU32	    mFrameNum;      ///< �t���[���̐� (DerivedD3DXFrame �ɘA�Ԃ�t���邽�߂Ɏg��)
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