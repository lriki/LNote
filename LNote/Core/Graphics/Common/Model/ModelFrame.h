//=============================================================================
//�y ModelFrame �z
//-----------------------------------------------------------------------------
///**
//  @file       ModelFrame.h
//  @brief      ModelFrame
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include "../../../Base/RefString.h"
#include "../../../Geometry/SQTTransform.h"
#include "PMDTypes.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
class ModelCore;
class ModelIK;


//=============================================================================
// �� ModelFrame �N���X
//-----------------------------------------------------------------------------
///**
//  @brief
//
//  @note
//              PMD �̃{�[���ɂ�����B�ǂ����Ă����O�ō����������Ȃ�ς��邩���B
//
//              ModelCore ���L���A�R�s�[����ׂ��f�[�^�Ƃ����łȂ��f�[�^�����݂��Ă���
//              �����������������������ǁA�킩��₷�����Ă����̂��厖�B
//              ����Ȃ킯�ŁA�t���[���z�񂲂ƑS���R�s�[����B
//*/
//=============================================================================
class ModelFrame
{
public:

    ModelFrame();

    virtual ~ModelFrame();

public:

    /// �e�t���[�����擾����
    ModelFrame* getParent() { return mParent; }

    /// ���[���h�s����K�w�I�ɍX�V���� (IK ���A�e�̂Ȃ��{�[���̓��[�g�� updateTransform() ���Ă�ł��X�V����Ȃ��̂Œ���)
    void updateTransform( const LMatrix& parent_mat_ );

    /// �t���[���̃��[���h�s���ݒ肷��
    void setWorldMatrix( const LMatrix& matrix_, lnU32 update_depth_ );

    /// �X�L�j���O�p�s��̍X�V
    void updateSkinningMatrix();

public:

    // �ȉ��͓����p

    /// ������ (frames_ �͒P���ɐe�ւ̃|�C���^���擾����̂Ɏg���B���g�͂ł��ĂȂ���OK)
    void initializePMD( ModelCore* model_core_, const PMD_Bone* bone_data_, lnU32 index_, ModelFrame* frames_ );

    /// PMD�������㏈�� (�q���X�g�쐬�A�I�t�Z�b�g�Čv�Z)
    void postInitializePMD();

    /// �e�ϐ��̏����l��ݒ�
    void reset();

    /// ���O�̎擾
    const LRefTString& getName() const { return mName; }

    /// IK �p�x�����̗L���ݒ� (ModelIK::initializePMD() ����Ă΂��)
    void setIKLimitEnable( bool f_ ) { mIKLimitAngle = f_; }

    /// IK �p�x�����̗L������
    bool isIKLimit() const { return mIKLimitAngle; }


    LSQTTransform* getLocalTransformPtr() { return &mLocalTransform; }

    LMatrix* getWorldMatrixPtr() { return &mWorldMatrix; }


    const LMatrix& getWorldMatrix() const { return mWorldMatrix; }

    const LMatrix& getSkinningMatrix() const { return mSkinningMatrix; }

    /// mLocalTransform �� mParent->mWorldMatrix ���� mWorldMatrix �����
    void updateWorldMatrixFromLocalAndParent();

    lnU32 getIndex() const { return mIndex; }

private:

    typedef std::vector< ModelFrame* >  ModelFrameList;

private:

    ModelCore*          mModelCore;

    lnU32               mIndex;
    LRefTString         mName;
    ModelFrame*         mParent;    ///< �e
    ModelFrameList	    mChildList; ///< �q�ꗗ

    LVector3            mOrgPosition;   ///< ���f�����_����̈ʒu
    LVector3		    mOffset;        ///< �emOrgPosition - mOrgPosition (initializePMD�ApostInitializePMD �ōČv�Z)
    LMatrix			    mInvTransform;	///< �����l�̃{�[�������_�Ɉړ�������悤�ȍs�� (�ړ������̂�)
    bool			    mIKLimitAngle;	///< IK���Ɋp�x���������邩�ǂ���

    // �{���͈ȉ��������R�s�[���Ďg��
 //   LVector3			mPosition;          // ARTK_MMD �ł̓��[�V�����͂���� mRotation �ɏ�����Ă���
	//LQuaternion			mRotation;          // ModelIK ��������ڏ����������� (ARTK_MMD �ł̓��[���h���͍l�����ĂȂ��݂����c)
    LSQTTransform       mLocalTransform;   // ���[�V�������������ނ̂͂����������Ǝv�����ǁc�BSelene �ł͂����ɏ�������ł�B
    LMatrix             mWorldMatrix;       ///< �����ςݍs�� (setWorldMatrix() �ōX�V���ꂽ���[���h(�O���[�o��)�s��BModelRigidBody::updateBoneTransform() �����������������)
    LMatrix             mSkinningMatrix;    ///< ���̍s��ŃX�L�j���O���� (updateSkinningMatrix() �ō쐬�BmInvTransform * mWorldMatrix)

    friend class ModelIK;
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