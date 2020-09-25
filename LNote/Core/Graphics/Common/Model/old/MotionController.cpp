//=============================================================================
//�y MotionController �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "ModelFrame.h"
#include "ModelFace.h"
#include "MotionController.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Graphics
{

//=============================================================================
// �� MotionController �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    MotionController::MotionController( ModelFrame* frames_, lnU32 frame_num_, ModelFace* faces_, lnU32 face_num_ )
        : mModelFrameNum    ( frame_num_ )
        , mModelFrames      ( frames_ )
        , mModelFaceNum     ( face_num_ )
        , mModelFaces       ( faces_ )
    {
        for ( int i = 0; i < LN_MAX_ATTACH_MOTIONS; ++i )
        {
            mAttachedMotionDataArray[ i ].initialize( mModelFrameNum );
        }
    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    MotionController::~MotionController()
    {
        for ( int i = 0; i < LN_MAX_ATTACH_MOTIONS; ++i )
        {
            LN_SAFE_RELEASE( mAttachedMotionDataArray[ i ].AttachedMotion );
        }
    }

    //---------------------------------------------------------------------
	// �� ���[�V�����̃A�^�b�`
    //---------------------------------------------------------------------
    void MotionController::attachMotion( lnU32 slot_idx_, Motion* motion_ )
    {
        LN_SAFE_RELEASE( mAttachedMotionDataArray[ slot_idx_ ].AttachedMotion );
        mAttachedMotionDataArray[ slot_idx_ ].AttachedMotion = motion_;
        LN_SAFE_ADDREF( mAttachedMotionDataArray[ slot_idx_ ].AttachedMotion );

        // �{�[�����ɑΉ�����A�j���[�V��������������
        for ( lnU32 i = 0; i < mModelFrameNum; ++i )
        {
            mAttachedMotionDataArray[ slot_idx_ ].BoneAnimations[ i ] =
                motion_->findBoneAnimation( mModelFrames[ i ].getName().c_str() );

            //_p(mModelFrames[ i ].getName().c_str());
            //_p(mAttachedMotionDataArray[ slot_idx_ ].BoneAnimations[ i ]);
        }

        // �\��ɑΉ�����A�j���[�V��������������
        for ( lnU32 i = 0; i < mModelFaceNum; ++i )
        {
            mAttachedMotionDataArray[ slot_idx_ ].FaceAnimations[ i ] =
                motion_->findFaceAnimation( mModelFaces[ i ].getName().c_str() );
        }

        // ���ԏ�����
        mAttachedMotionDataArray[ slot_idx_ ].Time = 0.0;
    }

    //---------------------------------------------------------------------
	// �� ���Ԃ�i�߂�A�{�[���s����X�V���� (�b)
    //---------------------------------------------------------------------
    void MotionController::advanceTime( lnfloat elapsed_time_ )
    {
        // ���[�V�����̃A�^�b�`����Ă���g���b�N�̎��Ԃ�i�߂�
        for ( int i = 0; i < LN_MAX_ATTACH_MOTIONS; ++i )
        {
            if ( mAttachedMotionDataArray[ i ].AttachedMotion )
            {
                mAttachedMotionDataArray[ i ].Time += elapsed_time_;
            }
        }


        for ( int i = 0; i < LN_MAX_ATTACH_MOTIONS; ++i )
        {
            if ( mAttachedMotionDataArray[ i ].AttachedMotion )
            {
                for ( lnU32 frame_idx = 0; frame_idx < mModelFrameNum; ++frame_idx )
                {
                    if ( mAttachedMotionDataArray[ i ].BoneAnimations[ frame_idx ] )
                    {
                        mAttachedMotionDataArray[ i ].BoneAnimations[ frame_idx ]->setTime(
                            mAttachedMotionDataArray[ i ].Time, 60.0f );

                        *(mModelFrames[ frame_idx ].getLocalTransformPtr()) =
                            mAttachedMotionDataArray[ i ].BoneAnimations[ frame_idx ]->getTransform();
                    }
                }
            }
        }
    }


  

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Lumine

//=============================================================================
//								end of file
//=============================================================================