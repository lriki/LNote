//=============================================================================
//�y Model �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "ModelCore.h"
#include "ModelRigidBody.h"
#include "ModelConstraint.h"
#include "Model.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Graphics
{

//=============================================================================
// �� Model �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    Model::Model()
        : mModelCore        ( NULL )
        , mConstraintNum    ( 0 )
        , mConstraints      ( NULL )
        , mMotionController ( NULL )
        , mRigidResetCount  ( 0 )
    {
    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    Model::~Model()
    {
        SAFE_DELETE_ARRAY( mConstraints );
        LN_SAFE_RELEASE( mModelCore );

        SAFE_DELETE( mMotionController );
        
    }

    //---------------------------------------------------------------------
	// �� �t���[������������
    //---------------------------------------------------------------------
    ModelFrame* Model::findFrame( const lnChar* name_ )
    {
        lnU32 idx = mModelCore->getFrameIndexByName( name_ );
        if ( idx == 0xffffffff ) { 
	return NULL; }
        return &mModelFrames[ idx ];
    }

    //---------------------------------------------------------------------
	// �� ���ׂẴt���[���s����K�w�I�ɍX�V����
    //---------------------------------------------------------------------
    void Model::updateFrameMatrix()
    {
        mRootFrame->updateTransform( LMatrix() );

        // �e�̂Ȃ��t���[���̍X�V
        const NoParentFrameIndexArray& ary = mModelCore->getNoParentFrameIndexArray();
        if ( !ary.empty() )
        {
            NoParentFrameIndexArray::const_iterator itr = ary.begin();
            NoParentFrameIndexArray::const_iterator end = ary.end();
            for ( ; itr != end; ++itr )
            {
                mModelFrames[ (*itr) ].updateTransform( LMatrix() );
            }
        }


        if ( mRigidResetCount > 0 )
        {
            for ( lnU32 i = 0; i < mRigidBodyNum; ++i )
	        {
		        mModelRigidBodys[ i ].moveToBonePos();
	        }
            --mRigidResetCount;
        }
    }

    //---------------------------------------------------------------------
	// �� ���ׂĂ̍��̂������I�Ƀ{�[���̈ʒu�Ɉړ�������
    //---------------------------------------------------------------------
    void Model::resetRigidBody()
    {
        mRigidResetCount = 3;
        
    }

    //---------------------------------------------------------------------
	// �� 
    //---------------------------------------------------------------------
    void Model::postUpdate()
    {
        // �{�[���ʒu���킹
        //      �{�[���̃��[���h�s�񂪕K�v�B
        //      ���̊֐��̑O�� ModelFrame::mWorldMatrix ���m�肵�Ă���K�v������B

        // IK �X�V
        //      ���̒��ł� ModelFrame::mRotation �����������A
        //      ModelFrame::updateMatrixInverse() ���Ă�� mWorldMatrix ���X�V���Ă���B
    }

    //---------------------------------------------------------------------
	// �� �X�L�j���O�s��̍X�V
    //---------------------------------------------------------------------
    void Model::updateSkinningMatrix()
    {
        // �������Z���f
	    for ( lnU32 i = 0; i < mRigidBodyNum; ++i )
  	    {
		    mModelRigidBodys[ i ].updateBoneTransform();
	    }

        // �X�L�j���O�p�s��̍X�V
	    for ( lnU32 i = 0; i < mFrameNum; ++i )
	    {
		    mModelFrames[ i ].updateSkinningMatrix();
	    }
    }

    //---------------------------------------------------------------------
	// �� ���̂̈ʒu�����݂̃{�[���̈ʒu�ɂ��킹��
    //---------------------------------------------------------------------
    //void Model::resetRigidBodyPos()
    //{
	   // for ( lnU32 i = 0; i < mRigidBodyNum; ++i )
	   // {
		  //  mModelRigidBodys[ i ].moveToBonePos();
	   // }
    //}

    //---------------------------------------------------------------------
	// �� �\�t�g�E�F�A�X�L�j���O
    //---------------------------------------------------------------------
    void Model::skinning()
    {
        mModelCore->skinning( mModelFrames );
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Lumine

//=============================================================================
//								end of file
//=============================================================================