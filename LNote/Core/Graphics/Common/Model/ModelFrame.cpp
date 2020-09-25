//=============================================================================
//�y ModelFrame �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "ModelCore.h"
#include "ModelFrame.h"

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
// �� ModelFrame �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    ModelFrame::ModelFrame()
        : mModelCore        ( NULL )
        , mIndex            ( 0 )
        , mParent           ( NULL )
        , mIKLimitAngle     ( false )
    {
    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    ModelFrame::~ModelFrame()
    {
    }

    //---------------------------------------------------------------------
	// �� ���[���h�s����K�w�I�ɍX�V����
    //---------------------------------------------------------------------
    void ModelFrame::updateTransform( const LMatrix& parent_mat_ )
    {
        // ���[�J���ȕϊ��Ƀ��[���h��K�p
	    mWorldMatrix = mLocalTransform;
        mWorldMatrix.translation( mOffset );
        mWorldMatrix *= parent_mat_;

	    // �q���̌v�Z
#if 1
        ModelFrameList::iterator itr = mChildList.begin();
	    ModelFrameList::iterator end = mChildList.end();
	    for ( ; itr != end; ++itr )
        {
            (*itr)->updateTransform( mWorldMatrix );
        }
#else
	    //ModelFrameList::iterator itr = mChildList.begin();
	    //ModelFrameList::iterator end = mChildList.end();
	    //for ( ; itr != end; ++itr )
        int size = mChildList.size();
        if (size)
        {
            ModelFrame** fs = &mChildList[0];
            for (int i = 0; i < size; ++i, ++fs)
	        {
                (*fs)->updateTransform( mWorldMatrix );

		        //(*itr)->updateTransform( mWorldMatrix );
	        }
        }
#endif
    }

    //---------------------------------------------------------------------
	// �� �t���[���̃��[���h�s���ݒ肷��
    //---------------------------------------------------------------------
    void ModelFrame::setWorldMatrix( const LMatrix& matrix_, lnU32 update_depth_ )
    {
        mWorldMatrix = matrix_;

	    if ( update_depth_ > 0 )
	    {
		    --update_depth_;

		    // �q���̌v�Z
		    ModelFrameList::iterator itr = mChildList.begin();
		    ModelFrameList::iterator end = mChildList.end();
		    for ( ; itr != end; ++itr )
		    {
                LMatrix mat = (*itr)->mLocalTransform;
                mat.translation( mOffset ); // TODO �����ł����̂��`�F�b�N�����邩��
                mat *= mWorldMatrix;
			    (*itr)->setWorldMatrix( mat, update_depth_ );
		    }
	    }
    }

    //---------------------------------------------------------------------
	// �� �X�L�j���O�p�s��̍X�V
    //---------------------------------------------------------------------
    void ModelFrame::updateSkinningMatrix()
    {
        mSkinningMatrix =  mInvTransform;
        mSkinningMatrix *= mWorldMatrix;
    }



    //---------------------------------------------------------------------
	// �� ������
    //---------------------------------------------------------------------
    void ModelFrame::initializePMD( ModelCore* model_core_, const PMD_Bone* bone_data_, lnU32 index_, ModelFrame* frames_ )
    {
        mModelCore = model_core_;
        mIndex = index_;

        // �{�[�����̃R�s�[ (�{�[������ 20 �ŁA�I�[���Ȃ������ꍇ�̑Ώ�)
        char name[ 21 ];
        strncpy( name, bone_data_->szName, 20 );
	    name[ 20 ] = '\0';
        mName = name;

	    // �ʒu�̃R�s�[
	    mOrgPosition = bone_data_->vec3Position;

        
	    // �e�{�[���̐ݒ�
	    if ( bone_data_->nParentNo != -1 )
	    {
            mParent = &frames_[ bone_data_->nParentNo ];
            mOffset = mOrgPosition - mParent->mOrgPosition;
	    }
        // �e�Ȃ�
	    else
	    {		    
		    mOffset = mOrgPosition;
	    }

	    // �q�{�[���̐ݒ�
	    //if( pPMDBoneData->nChildNo != -1 )
	    //{
		   // m_pChildBone = (cPMDBone *)&(pBoneArray[pPMDBoneData->nChildNo]);
	    //}

        /*
            �����p�� = �X�L�j���O���Ȃ��Ă������p���B
            �{�[���͍ŏ�����I�t�Z�b�g�������Ă邯�ǁA
            ������X�L�j���O�ɓK�p����Ǝp��������Ă��܂��B
            ���̂��߁A�����I�t�Z�b�g��ł������������K�v�B���ꂪ���̍s��B

            ���܂܂ňӎ����ĂȂ��������ǁAID3DXSkinInfo::GetBoneOffsetMatrix() ��
            �擾�ł���s�� (SkinnedMesh�T���v���� D3DXMESHCONTAINER_DERIVED::pBoneOffsetMatrices) ��
            ����ɂ�������̂��ۂ��B
            �T���v���ł��`��̒��O�ɑΏۃ{�[���s��ɂ��̍s�����Z���Ă���B
        */
        LMatrix::translation(
            &mInvTransform,
            -mOrgPosition.x,
            -mOrgPosition.y,
            -mOrgPosition.z );	 

	    mIKLimitAngle = false;

	    // �e�ϐ��̏����l��ݒ�
	    reset();
    }

    //---------------------------------------------------------------------
	// �� PMD�������㏈�� (�q���X�g�쐬�A�I�t�Z�b�g�Čv�Z)
    //---------------------------------------------------------------------
    void ModelFrame::postInitializePMD()
    {
        if ( mParent )
        {
            mOffset = mOrgPosition - mParent->mOrgPosition;
            mParent->mChildList.push_back( this );
        }
    }

    //---------------------------------------------------------------------
	// �� �e�ϐ��̏����l��ݒ�
    //---------------------------------------------------------------------
    void ModelFrame::reset()
    {
        mLocalTransform = LSQTTransform::IDENTITY;
        mLocalTransform.Translation = LVector3::ZERO;//mOffset;///mOrgPosition;//
        //mWorldMatrix = mLocalTransform;
        updateWorldMatrixFromLocalAndParent();

    }

    //---------------------------------------------------------------------
	// �� mLocalTransform �� mParent->mWorldMatrix ���� mWorldMatrix �����
    //---------------------------------------------------------------------
    void ModelFrame::updateWorldMatrixFromLocalAndParent()
    {
        mWorldMatrix = mLocalTransform;
        mWorldMatrix.translation( mOffset );
        if ( mParent ) mWorldMatrix *= mParent->mWorldMatrix;
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