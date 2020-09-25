//=============================================================================
//�y Model �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "ModelCore.h"
#include "ModelIK.h"
#include "ModelRigidBody.h"
#include "ModelConstraint.h"
#include "Model.h"

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
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    Model::Model( ModelManager* manager_ )
        : mManager          ( manager_ )
        , mGraphicsDevice   ( manager_->getGraphicsDevice() )
        , mModelCore        ( NULL )
        , mAttrNum          ( 0 )
        , mMaterials        ( NULL )
        , mAttributes       ( NULL )
        , mFrameNum         ( 0 )
        , mModelFrames      ( NULL )
        , mRootFrame        ( NULL )
        , mRigidBodyNum     ( 0 )
        , mModelRigidBodys  ( NULL )
        , mConstraintNum    ( 0 )
        , mConstraints      ( NULL )
        , mMotionController ( NULL )
        , mRigidResetCount  ( 0 )
		, mEnablePhysics	( true )
    {
    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    Model::~Model()
    {
        for ( lnU32 i = 0; i < mConstraintNum; ++i )
        {
            LN_SAFE_RELEASE( mConstraints[ i ] );
        }
        SAFE_DELETE_ARRAY( mConstraints );

        SAFE_DELETE_ARRAY( mMaterials );
        SAFE_DELETE_ARRAY( mAttributes );
        SAFE_DELETE_ARRAY( mModelFrames );
        SAFE_DELETE_ARRAY( mModelRigidBodys );

        
        LN_SAFE_RELEASE( mModelCore );

        
 
        SAFE_DELETE( mMotionController );
        
    }

    //---------------------------------------------------------------------
	// �� �t���[������������
    //---------------------------------------------------------------------
    ModelFrame* Model::findFrame( const lnChar* name_ )
    {
        lnU32 idx = mModelCore->getFrameIndexByName( name_ );
        if ( idx == 0xffffffff ) return NULL;
        return &mModelFrames[ idx ];
    }

	//---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
	void Model::setEnablePhysics( bool flag )
	{
		mEnablePhysics = flag;
		//updateSkinningMatrix();
	}

    //---------------------------------------------------------------------
	// �� ���ׂĂ̍��̂������I�Ƀ{�[���̈ʒu�Ɉړ�������
    //---------------------------------------------------------------------
    void Model::resetRigidBody()
    {
        mRigidResetCount = 3;
    }

    //---------------------------------------------------------------------
	// �� ���ׂẴt���[���s����K�w�I�ɍX�V����
    //---------------------------------------------------------------------
    void Model::updateFrameMatrix(bool ik_)
    {
        if ( mRootFrame )
        {
            mRootFrame->updateTransform( LMatrix() );
        }

        // �e�̂Ȃ��t���[���̍X�V
        const NoParentFrameIndexArray& ary = mNoParentFrameIndexArray;
        if ( !ary.empty() && ik_ )
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
	// �� 
    //---------------------------------------------------------------------
    void Model::postUpdate( lnfloat elapsed_time_ )
    {
        // �{�[���ʒu���킹 (���̂��ړ�������)
        //      �{�[���̃��[���h�s�񂪕K�v�B
        //      ���̊֐��̑O�� ModelFrame::mWorldMatrix ���m�肵�Ă���K�v������B
        for ( lnU32 i = 0 ; i < mRigidBodyNum ; ++i )
	    {
		    mModelRigidBodys[ i ].fixPosition( elapsed_time_ ); // ���Ƃ��Ƃ� elapsed_frame_
	    }

        // IK �X�V
        //      ���̒��ł� ModelFrame::mRotation �����������A
        //      ModelFrame::updateMatrixInverse() ���Ă�� mWorldMatrix ���X�V���Ă���B
        for ( lnU32 i = 0 ; i < mModelCore->mIKNum ; ++i )
	    {
		    mModelCore->mIKArray[ i ].update( mModelFrames ); // mModelFrames�͉�
	    }
    }

    //---------------------------------------------------------------------
	// �� �X�L�j���O�s��̍X�V
    //---------------------------------------------------------------------
    void Model::updateSkinningMatrix()
    {
		if ( mEnablePhysics )
		{
			// �������Z���f (�{�[���̈ʒu�����̂ɍ��킹��)
			for ( lnU32 i = 0; i < mRigidBodyNum; ++i )
  			{
				mModelRigidBodys[ i ].updateBoneTransform();
			}
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
        if ( mModelFrames )
        {
            mModelCore->skinning( mModelFrames );
        }
    }

    //---------------------------------------------------------------------
	// �� �T�u�Z�b�g�̕`��
    //---------------------------------------------------------------------
    LNRESULT Model::drawSubset( lnU32 idx_ )
    {
        IRenderer* r = mGraphicsDevice->getRenderer();
        r->setVertexBuffer( mModelCore->mVertexBuffer );
        r->setIndexBuffer( mModelCore->mIndexBuffer );
        r->drawPrimitiveIndexed(
            LN_PRIM_TRIANGLELIST, mAttributes[ idx_ ].StartIndex, mAttributes[ idx_ ].PrimitiveNum );
        return LN_OK;
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