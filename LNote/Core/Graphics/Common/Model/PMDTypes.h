//=============================================================================
//�y PMDTypes �z
//-----------------------------------------------------------------------------
///**
//  @file       PMDTypes.h
//  @brief      PMDTypes
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../Geometry/Vector2.h"
#include "../../../Geometry/Vector3.h"
#include "../../../Geometry/Vector4.h"
#include "../../../Geometry/Matrix.h"
#include "../../../Geometry/Quaternion.h"
#include "../Types.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

#pragma pack( push, 1 )

//-------------------------------------------------------------
// PMD

// �t�@�C���w�b�_
struct PMD_Header
{
	char	szMagic[3];		// "Pmd"
	float	fVersion;		// PMD�o�[�W�����ԍ�
	char	szName[20];		// ���f����
	char	szComment[256];	// �R�����g(���쌠�\���Ȃ�)
};

// ���_�f�[�^
struct PMD_Vertex
{
	LVector3    vec3Pos;	// ���W
	LVector3    vec3Normal;	// �@���x�N�g��
	LVector2    uvTex;		// �e�N�X�`�����W

	unsigned short	unBoneNo[2];	// �{�[���ԍ�
	unsigned char	cbWeight;		// �u�����h�̏d�� (0�`100��)
	unsigned char	cbEdge;			// �G�b�W�t���O
};

// �}�e���A���f�[�^
struct PMD_Material
{
	LVector4    col4Diffuse;
	float		fShininess;
	LVector3    col3Specular,
				col3Ambient;

	unsigned short	unknown;
	unsigned long	ulNumIndices;			// ���̍ގ��ɑΉ����钸�_��
	char			szTextureFileName[20];	// �e�N�X�`���t�@�C����
};

// �{�[���f�[�^
struct PMD_Bone
{
	char	szName[20];			// �{�[���� (0x00 �I�[�C�]���� 0xFD)
	short	nParentNo;			// �e�{�[���ԍ� (�Ȃ���� -1)
	short	nChildNo;			// �q�{�[���ԍ�

	unsigned char	cbKind;		// �{�[���̎��
	unsigned short	unIKTarget;	// IK���̃^�[�Q�b�g�{�[��

	LVector3		vec3Position;	// ���f�����_����̈ʒu
};
/*
kind
	0x00�@�ʏ�{�[��(��]�̂�)
	0x01�@�ʏ�{�[��(�ړ��\)
	0x02�@IK�{�[��(�ړ��\)
	0x03�@�����Ȃ�(�I����)
	0x04�@IK�{�[��(��]�̂�)
	0x05�@��]�Ǐ](��]�̂�)
	0x06�@IK�ڑ���
	0x07�@�����Ȃ�(�I��s��)
	0x08�@�Ђ˂�
	0x09�@��]�^��
*/

// IK�f�[�^
struct PMD_IK
{
	short			nTargetNo;	// IK�^�[�Q�b�g�{�[���ԍ�
	short			nEffNo;		// IK��[�{�[���ԍ�

	unsigned char	cbNumLink;	// IK���\������{�[���̐�

	unsigned short	unCount;
	float			fFact;

	unsigned short	punLinkNo[1];// IK���\������{�[���̔z��
};

// �\��_
struct PMD_FaceVtx
{
	unsigned long	ulIndex;
	LVector3	    vec3Pos;
};

// �\��f�[�^
struct PMD_Face
{
	char			szName[20];		// �\� (0x00 �I�[�C�]���� 0xFD)

	unsigned long	ulNumVertices;	// �\��_��
	unsigned char	cbType;			// ����

	PMD_FaceVtx		pVertices[1];	// �\��_�f�[�^
};

// ���̃f�[�^
struct PMD_RigidBody
{
	char			szName[20];		// ���̖�

	unsigned short	unBoneIndex;	// �֘A�{�[���ԍ�
	unsigned char	cbColGroupIndex;// �Փ˃O���[�v
	unsigned short	unColGroupMask;	// �Փ˃O���[�v�}�X�N

	unsigned char	cbShapeType;	// �`��  0:�� 1:�� 2:�J�v�Z��

	float			fWidth;			// ���a(��)
	float			fHeight;		// ����
	float			fDepth;			// ���s

	LVector3	    vec3Position;	// �ʒu(�{�[������)
	LVector3	    vec3Rotation;	// ��](radian)

	float			fMass;			// ����
	float			fLinearDamping;	// �ړ���
	float			fAngularDamping;// ��]��
	float			fRestitution;	// ������
	float			fFriction;		// ���C��

	unsigned char	cbRigidBodyType;// �^�C�v 0:Bone�Ǐ] 1:�������Z 2:�������Z(Bone�ʒu����)
};

// �R���X�g���C���g(�W���C���g)�f�[�^
struct PMD_Constraint
{
	char			szName[20];		// �R���X�g���C���g��

	unsigned long	ulRigidA;		// ����A
	unsigned long	ulRigidB;		// ����B

	LVector3	    vec3Position;	// �ʒu(���f�����_���S)
	LVector3	    vec3Rotation;	// ��](radian)

	LVector3	    vec3PosLimitL;	// �ړ�����1
	LVector3	    vec3PosLimitU;	// �ړ�����2

	LVector3	    vec3RotLimitL;	// ��]����1
	LVector3	    vec3RotLimitU;	// ��]����2

	LVector3	    vec3SpringPos;	// �΂ˈړ�
	LVector3	    vec3SpringRot;	// �΂ˉ�]
};


//-------------------------------------------------------------
// VMD

// �t�@�C���w�b�_
struct VMD_Header
{
	char	szHeader[30];			// "Vocaloid Motion Data 0002"
	char	szModelName[20];		// �Ώۃ��f����
};

// ���[�V�����f�[�^
struct VMD_Motion
{
	char	szBoneName[15];			// �{�[����

	unsigned long	ulFrameNo;		// �t���[���ԍ�

	LVector3	vec3Position;			// �ʒu
	LQuaternion	vec4Rotation;			// ��](�N�H�[�^�j�I��)

	char	cInterpolationX[16];	// ��ԏ�� X���ړ�
	char	cInterpolationY[16];	// ��ԏ�� Y���ړ�
	char	cInterpolationZ[16];	// ��ԏ�� Z���ړ�
	char	cInterpolationRot[16];	// ��ԏ�� ��]
};

// �\��f�[�^
struct VMD_Face
{
	char	szFaceName[15];		// �\�

	unsigned long	ulFrameNo;	// �t���[���ԍ�

	float	fFactor;			// �u�����h��
};




namespace Vertex
{

class PMDVertex
{
public:

    LVector3	Position;           ///< ���W
    LVector3	Normal;             ///< �@��
    LVector2    TexUV;                 ///< �e�N�X�`�����W
    lnU32       Color;              ///< ���_�J���[ (���C�u�����̎d�l�ɍ��킹�邽�߂ɗp�ӁB���ׂ� 0xffffffff)
    lnU8        BlendIndices[ 4 ];  ///< �{�[���C���f�b�N�X (PMD �� 2 ���������ǁA�Ƃ肠���� 4 �o�C�g�ɂ܂Ƃ߂邽��)
    lnfloat     BlendWeights[ 4 ];  ///< �{�[���u�����h�� (0.0�`1.0�B�t�@�C������擾����̂� [0] �����ŁA[2]��[3]�͎g��Ȃ� (���0) )
    
    /// ���_���C�A�E�g
    static LNVertexElemenst* elements()
    {
	    static LNVertexElemenst elements[] = {
		    { 0, LN_DECL_FLOAT3, LN_USAGE_POSITION     , 0 },
            { 0, LN_DECL_FLOAT3, LN_USAGE_NORMAL       , 0 },
            { 0, LN_DECL_FLOAT2, LN_USAGE_TEXCOORD     , 0 },
            { 0, LN_DECL_COLOR4, LN_USAGE_COLOR        , 0 },
            { 0, LN_DECL_COLOR4, LN_USAGE_BLENDINDICES , 0 },
            { 0, LN_DECL_FLOAT4, LN_USAGE_BLENDWEIGHT  , 0 },
		      LN_DECL_END() };
	    return elements;
    }
};

class BasicXFileVertex
{
public:

    LVector3	Position;           ///< ���W
    LVector3	Normal;             ///< �@��
    lnU32       Color;              ///< ���_�J���[ (���C�u�����̎d�l�ɍ��킹�邽�߂ɗp�ӁB���ׂ� 0xffffffff)
    LVector2    TexUV;              ///< �e�N�X�`�����W
};

}

#pragma pack( pop )


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================
