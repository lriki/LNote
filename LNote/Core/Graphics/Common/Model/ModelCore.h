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
#include "../../../Geometry/Vector2.h"
#include "../../../Geometry/Vector3.h"
#include "../../../Geometry/Vector4.h"
#include "../../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{



class Model;
class ModelManager;
class ModelFrame;
class ModelBone;
class ModelFace;
class ModelIK;
class ModelIKLimitter;
class ModelRigidBody;
class ModelConstraint;


struct CollisionMesh
{
    lnU32       VertexCount;
    LVector3*   CollisionVertexBuffer;
    lnU32       IndexCount;
    lnU32       IndexStride;            ///< 2 or 4
    void*       CollisionIndexBuffer;
};



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

    /// �X�L�����b�V�����𔻒�
    bool isSkinMesh() const { return mIsSkinMesh; }

    /// PMD ���f���ǂݍ��� (�V�����쐬���� model_ �Ƀ|�C���^��Ԃ�)
    /// flags_ �� LNModelFlag �̑g�ݍ��킹
    LNRESULT loadPMD( const void* data_, lnU32 size_, lnU32 flags_, Model** model_ );

    /// X �t�@�C���ǂݍ���
    void loadXFile( const void* data_top_, lnU32 size_, lnU32 flags_, Model** model_ );

    /// ���O����t���[���ԍ����擾���� (������Ȃ��ꍇ�� LN_INVALID_INDEX)
    lnU32 getFrameIndexByName( const lnChar* name_ );

    /// �\�t�g�E�F�A�X�L�j���O
    void skinning( ModelFrame* frames_ );

    /// IK ���~�b�^�̎擾
    ModelIKLimitter* getIKLimitter() { return mIKLimitter; }

    /// �R���W�������b�V���̎擾
    const CollisionMesh& getCollisionMesh() { return *mCollisionMesh; }

private:

    /// ���f���f�[�^���
    void _release();

    /// �X�t�B�A�}�b�v�p�̃e�N�X�`�������𔻒肷��
    bool _isSphereMapTexName( const char* tex_name_ ) const;

    void _createCollisionBuffers();

private:

    typedef std::map< LRefTString, lnU32 >   FrameIndexMap;
    typedef std::pair< LRefTString, lnU32 >  FrameIndexPair;
    

private:

    ModelManager*           mManager;
    IGraphicsDevice*        mGraphicsDevice;    // �g���� load �ł��������烁���o�Ɏ����Ȃ��Ă�������
    LRefTString             mModelName;

    // ���L����
    IVertexBuffer*          mVertexBuffer;
    IIndexBuffer*           mIndexBuffer;
    LVector3*               mOrgVertexPositions;    ///< (�\�t�g�E�F�A�X�L�j���O�p)
    LVector3*               mOrgVertexNormals;      ///< (�\�t�g�E�F�A�X�L�j���O�p)
    CollisionMesh*          mCollisionMesh;
    

    FrameIndexMap           mFrameIndexMap;     ///< �t���[�����ɑΉ�����t���[���ԍ��\

    // ���L����
    lnU32                   mIKNum;
    ModelIK*                mIKArray;
    ModelIKLimitter*        mIKLimitter;

    // ���L����
    lnU32                   mFaceNum;
    ModelFace*              mModelFaces;

    bool                    mIsSkinMesh;

    friend class Model;
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