//=============================================================================
//�y Model �z
//-----------------------------------------------------------------------------
///**
//  @file       Model.h
//  @brief      Model
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../Math/Vector4.h"
#include "ModelFrame.h"
#include "MotionController.h"
#include "ModelCore.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Graphics
{
/*
    �� ���[�U�[�� HowTo
        
    �� ����r�𑊎�̕����Ɍ�����
        
        �m�[�h�c���[�Ƀ��[�V������K�p
            ��
        head = model.getFrame("��");
        mat = head.getWorldMatrix();
        - mat �𑊎�Ɍ����� -
        head.setWorldMatrix( mat, 0 );  // ��2�����͍X�V����[��


    �� ������ HowTo

    �� HLSL �ŃX�L�j���O�ł���`�ɂ���
        
        HLSL �̍ő�{�[������ 26 ���炢�B
        ���ɂ����W�X�g���g���Ƃ���������ƌ���B
        ID3DXSkinInfo::ConvertToIndexedBlendedMesh ���g���ĕϊ��B
        �ő�{�[���������̊֐��ɓn�����ƂŁA���b�V�����K�؂ɕϊ������B

    �� �������b�V���R���e�i�̑Ή�

        ���_�t�H�[�}�b�g���Ⴄ�ꍇ�ɁA�����̃��b�V���R���e�i���g���邱�Ƃ�����B
        ���_�t�H�[�}�b�g�͋����I�ɕϊ����邵�A�ő��ɂȂ����Ƃ�����Ή����Ȃ��Ă������Ǝv�����ǁc�B
        �Ƃ肠�������̂Ƃ���͖����ŁB

    �� ����̃t���[���ȉ��݂̂Ƀ��[�V������K�p�E�u�����h����


    �� ���̑�����

        DirectX�T���v����DXLib �ł́A�t���[���̒��Ƀ��b�V��(�R���e�i)���i�[����Ă��āA
        ���b�V���������Ă��Ȃ��t���[�������݂���B
        �������A���̃��C�u�����ł�ModelMesh���ɂ��ׂĂ�z��Ŏ����Ƃɂ���B

        DirectX�T���v���ł̓��b�V���R���e�i��NumInfl�Ƃ��������o��
        1�ʂ�����̉e���{�[���� (�ő�4) ���i�[���� HLSL ���ŏ�Z�񐔂����炷�H�v���Ă����ǁA
        ���̃��C�u�����ł�4�ŌŒ肷��B
        (���[�U�[���V�F�[�_���J�X�^�}�C�Y����Ƃ��A�]�v�Ȃ��ƍl���Ȃ��Ă������悤��)

*/

//=============================================================================
// �� Model �N���X
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class Model
    : public Base::ReferenceObject
{
public:
    Model();
    LN_REFOBJ_METHODS;

protected:
    virtual ~Model();

public:

    /// �t���[�����̎擾
    lnU32 getFrameNum() const { return mFrameNum; }

    /// �C���f�b�N�X���w�肵�ăt���[�����擾����
    ModelFrame* getFrame( lnU32 index_ ) { return &mModelFrames[ index_ ]; }

    /// �t���[������������
    ModelFrame* findFrame( const lnChar* name_ );

    /// ���[�V�����R���g���[���̎擾
    MotionController* getMotionController() { return mMotionController; }

    /// ���ׂẴt���[���s����K�w�I�ɍX�V����
    void updateFrameMatrix();

    /// ���ׂĂ̍��̂������I�Ƀ{�[���̈ʒu�Ɉړ�������
    void resetRigidBody();


    void postUpdate();

    /// �X�L�j���O�s��̍X�V
    void updateSkinningMatrix();

    // �A�j���[�V�����X�V
    // updateFrameMatrix()
    // �C�ӂ̃{�[���X�V
    // postUpdate()
    // �����X�V
    // updateSkinningMatrix()
    // �`��
    //      �̏��ŏ������s��

    // ���̂̈ʒu�����݂̃{�[���̈ʒu�ɂ��킹��
    //void resetRigidBodyPos();

    /// �\�t�g�E�F�A�X�L�j���O
    void skinning();




public:

    ModelCore* mModelCore;     ///< (ModelCore ����������A���̎��Q�ƃJ�E���g+1)

private:

    lnU32               mFrameNum;
    ModelFrame*         mModelFrames;
    ModelFrame*         mRootFrame;     ///< (ModelCore �����������)

    lnU32               mRigidBodyNum;
    ModelRigidBody*     mModelRigidBodys;

    lnU32               mConstraintNum;
    ModelConstraint*    mConstraints;

    MotionController*   mMotionController;

    lnU8                mRigidResetCount;             

    friend class ModelCore;
};

#if 0

/// ���b�V���̑����l
struct LNMeshAttributeRange
{
    u32     AttribID;           ///< �����e�[�u���̎��ʎq
    u32     StartIndex;         ///< �J�n�C���f�b�N�X
    u32     PrimitiveNum;       ///< �`��v���~�e�B�u��
};


struct LNBoneCombination
{
    lnU32   MaterialAttribId;   ///< mMaterials�AmTextures �̔ԍ� (HLSL �X�L�j���O�ł̓T�u�Z�b�g�ԍ��ł͂Ȃ��̂Œ��ӁI)
    lnU32*  BoneIds;            ///< �ő� 26 ���炢�́BUINT_MAX �͕K�v�Ȃ����Ƃ�\���B
};

class Model
{

private:

    Graphics::Material*     mMaterials;       ///< 
    Graphics::ITexture**    mTextures;

    lnU32                   mBoneNum;
    LMatrix*                mBoneMatrices;
};

class ModelCore
{
private:

    std::map< ln_std_tstring, lnU32 >   mFrameNameMap;  ///< 
};

// DirectX �̃��b�V���R���e�i�ɂ�����B
class ModelMesh
{
private:

    Graphics::IVertexBuffer*    mVertexBuffer;
    Graphics::IIndexBuffer*     mIndexBuffer;

    lnU32                       mAttrGroupNum;          ///< (HLSL �X�L�j���O�ł́A�`��T�u�Z�b�g�ԍ������˂�)
    LNMeshAttributeRange*       mMeshAttrs;             ///< ���b�V���T�u�Z�b�g
    LNBoneCombination*          mBoneCombinations;

    lnU32                       mAttrNum;
    Graphics::Material*         mOrginalMaterials;       ///< �I���W�i���̃}�e���A�� (���L���f�����쐬����Ƃ��A�R�s�[���Ďg��)
    Graphics::ITexture**        mOrginalTextures;

    lnU32                       mBoneNum;
    LMatrix*                    mOrginalBoneMatrices;
};

class ModelFrame
{
private:

    
};




/// ���[�V�����ЂƂ� (MMD �Ȃ� VMD �t�@�C���ЂƂ�)
class Motion
{
private:

    // �{�[���Ώۂ̃L�[�t���[���f�[�^
    struct KeyFrame
    {
	    lnU32	    FrameNo;		// �t���[���ԍ�

	    LVector3	Position;	    // �ʒu
	    LQuaternion	Rotation;	    // ��](�N�H�[�^�j�I��)

	    cVMDBezier	clPosXInterBezier;	// X���ړ����
	    cVMDBezier	clPosYInterBezier;	// Y���ړ����
	    cVMDBezier	clPosZInterBezier;	// Z���ړ����
	    cVMDBezier	clRotInterBezier;	// ��]���
    };

    // �{�[�����Ƃ̃L�[�t���[���f�[�^�̃��X�g
    struct MotionDataList
    {
	    char	szBoneName[16];			// �{�[����

	    unsigned long	ulNumKeyFrames;	// �L�[�t���[����
	    BoneKeyFrame	*pKeyFrames;	// �L�[�t���[���f�[�^�z��

	    MotionDataList	*pNext;
    };
};


class MotionController
{
public:

    /// ���[�V��������������
    MotionTrack* findMotionTrack( const lnChar* name_ );

private:
};


// �{�[���A�J����?�Ȃǂ̂ЂƂ��̃��[�V����
class MotionTrack
{
public:

    /// �u�����h�ςݍs��̎擾
    const LMatrix& getTransformMatrix();

private:
};
#endif


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Lumine

//=============================================================================
//								end of file
//=============================================================================