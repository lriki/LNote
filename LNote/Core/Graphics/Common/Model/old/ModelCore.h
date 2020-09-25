//=============================================================================
//�y ModelCore �z
//-----------------------------------------------------------------------------
///**
//  @file       ModelCore.h
//  @brief      ModelCore
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <map>
#include "../../../Base/RefString.h"
#include "../../../Math/Vector2.h"
#include "../../../Math/Vector3.h"
#include "../../../Math/Vector4.h"
#include "../../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Graphics
{



class Model;
class ModelManager;
class ModelFrame;
class ModelBone;
class ModelFace;
class ModelIK;
class ModelRigidBody;
class ModelConstraint;

typedef std::vector< lnU32 > NoParentFrameIndexArray;

//=============================================================================
// �� ModelCore �N���X
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class ModelCore
    : public Base::ReferenceObject
{
public:
    ModelCore( ModelManager* manager_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~ModelCore();

public:

    /// PMD ���f���ǂݍ��� (�V�����쐬���� model_ �Ƀ|�C���^��Ԃ�)
    LNRESULT loadPMD( const void* data_, lnU32 size_, bool sw_skinning_, Model** model_ );

    /// �}�e���A���z��̎擾
    LMaterial* getMaterials() { return mMaterials; };

    /// �����O���[�v���̎擾 (�T�u�Z�b�g��)
    lnU32 getAttrGroupNum() const { return mAttrNum; }  // ���̓e�X�g�p���Ă��Ƃ� mAttrNum �ŁB

    LNMeshAttributeRange* getAttributes() { return mAttributeRanges; }

    /// �T�u�Z�b�g�̕`��
    LNRESULT drawSubset( lnU32 idx_ );

    /// �t���[���z��̎擾 (ModelFrames ����g��)
    ModelFrame* getModelFrames() { return mModelFrames; }
    //ModelFrame* getModelFrame( lnU32 idx_ ) { return &mModelFrames[ idx_ ]; } // �T�C�Y���s���ł��ƌ�����̂łƂ肠����

    /// ���̂̈ʒu�C���AIK �X�V (���[�V�����X�V��ɌĂ�)
    void correction( lnfloat elapsed_time_ );



    /// ���O����t���[�����擾���� (Model �N���X�Ɏ�������ׂ�)
    //ModelFrame* getFrameByName( const lnChar* name_ );

    /// ���O����t���[���ԍ����擾���� (������Ȃ��ꍇ�� 0xffffffff)
    lnU32 getFrameIndexByName( const lnChar* name_ );

    /// �\�t�g�E�F�A�X�L�j���O
    void skinning( ModelFrame* frames_ );

    const NoParentFrameIndexArray& getNoParentFrameIndexArray() const { return mNoParentFrameIndexArray; }

private:

    /// ���f���f�[�^���
    void _release();

    /// �X�t�B�A�}�b�v�p�̃e�N�X�`�������𔻒肷��
    bool _isSphereMapTexName( const char* tex_name_ ) const;

private:

    typedef std::map< LRefTString, lnU32 >   FrameIndexMap;
    typedef std::pair< LRefTString, lnU32 >  FrameIndexPair;
    

private:

    ModelManager*           mManager;
    IGraphicsDevice*        mGraphicsDevice;
    LRefTString             mModelName;

    IVertexBuffer*          mVertexBuffer;
    IIndexBuffer*           mIndexBuffer;
    LVector3*               mOrgVertexPositions;    ///< (�\�t�g�E�F�A�X�L�j���O�p)
    LVector3*               mOrgVertexNormals;      ///< (�\�t�g�E�F�A�X�L�j���O�p)
    

    lnU32                   mAttrNum;
    LMaterial*              mMaterials;
    LNMeshAttributeRange*   mAttributeRanges;

    lnU32                   mBoneNum;
    ModelFrame*             mModelFrames;   // Model ��
    FrameIndexMap           mFrameIndexMap; ///< (����� ModelCore �Ɏ������Ă�����OK)

    lnU32                   mIKNum;
    ModelIK*                mIKArray;

    lnU32                   mFaceNum;
    ModelFace*              mModelFaces;

    lnU32                   mRigidBodyNum;
    ModelRigidBody*         mModelRigidBodys;

    NoParentFrameIndexArray mNoParentFrameIndexArray;   ///< ���[�g�t���[���ȊO�̐e�̂Ȃ��t���[���̃C���f�b�N�X���X�g

};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Lumine

//=============================================================================
//								end of file
//=============================================================================