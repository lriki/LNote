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

/*
[��ƃ��� 12/7/31]
    Model�` �n�̃N���X�͂قڊ����B
    ���f���̋��L�ɂ��Ă̓`�F�b�N���K�v�B

    �A�j���[�V�����͂Ƃ肠�����Đ��ł�����Ă����̏�ԁB

*/

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../Geometry/Vector4.h"
#include "ModelFrame.h"
#include "MotionController.h"
#include "ModelCore.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

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
    Model( ModelManager* manager_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~Model();

public:

    /// �X�L�����b�V�����𔻒�
    bool isSkinMesh() const { return mModelCore->isSkinMesh(); }

    /// �t���[�����̎擾
    lnU32 getFrameNum() const { return mFrameNum; }

    /// �C���f�b�N�X���w�肵�ăt���[�����擾����
    ModelFrame* getFrame( lnU32 index_ ) { return &mModelFrames[ index_ ]; }

    /// �t���[������������
    ModelFrame* findFrame( const lnChar* name_ );

    /// �}�e���A���̎擾
    LMaterial* getMaterial( lnU32 idx_ ) { return &mMaterials[ idx_ ]; }

    /// �T�u�Z�b�g���̎擾 (������)
    lnU32 getSubsetNum() const { return mAttrNum; }

    /// �����̎擾
    LNMeshAttribute* getAttribute( lnU32 idx_ ) { return &mAttributes[ idx_ ]; }

    /// ���[�V�����R���g���[���̎擾
    MotionController* getMotionController() { return mMotionController; }

    /// �R���W�������b�V���̎擾
    const CollisionMesh& getCollisionMesh() { return mModelCore->getCollisionMesh(); }

	/// �������Z�̗L���ݒ� (�ݒ��A���� updateSkinningMatrix() ����L��)
	void setEnablePhysics( bool flag );

	/// �������Z�̗L������
	bool isEnablePhysics() const { return mEnablePhysics; }


    /// ���ׂĂ̍��̂������I�Ƀ{�[���̈ʒu�Ɉړ�������
    void resetRigidBody();


    /// ���ׂẴt���[���s����K�w�I�ɍX�V����
    void updateFrameMatrix(bool ik_ = true);

    /// ���̂̈ʒu�C���AIK �X�V
    void postUpdate( lnfloat elapsed_time_ );

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

    /// �\�t�g�E�F�A�X�L�j���O
    void skinning();


    /// �T�u�Z�b�g�̕`��
    LNRESULT drawSubset( lnU32 idx_ );


private:

    typedef std::vector< lnU32 > NoParentFrameIndexArray;

private:

    ModelManager*           mManager;
    IGraphicsDevice*        mGraphicsDevice;    ///< ������� Renderer?
    ModelCore*              mModelCore;         ///< (ModelCore ����������A���̎��Q�ƃJ�E���g+1)

    lnU32                   mAttrNum;
    LMaterial*              mMaterials;
    LNMeshAttribute*        mAttributes;

    lnU32                   mFrameNum;
    ModelFrame*             mModelFrames;
    ModelFrame*             mRootFrame;     ///< (ModelCore �����������)
    NoParentFrameIndexArray mNoParentFrameIndexArray;   ///< ���[�g�t���[���ȊO�̐e�̂Ȃ��t���[���̃C���f�b�N�X���X�g

    lnU32                   mRigidBodyNum;
    ModelRigidBody*         mModelRigidBodys;

    lnU32                   mConstraintNum;
    ModelConstraint**       mConstraints;

    MotionController*       mMotionController;

    lnU8                    mRigidResetCount;
	bool					mEnablePhysics;

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
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================