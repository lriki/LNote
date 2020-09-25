//=============================================================================
//�y Motion �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"

#include "Motion.h"

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
// �� Motion �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    Motion::Motion()
        : mMaxFrames    ( 0.0 )
    {
    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    Motion::~Motion()
    {
        {
            MotionAnimationList::iterator itr = mMotionAnimationList.begin();
            MotionAnimationList::iterator end = mMotionAnimationList.end();
            for ( ; itr != end; ++itr )
            {
                SAFE_DELETE( *itr );
            }
        }
        {
            FaceAnimationList::iterator itr = mFaceAnimationList.begin();
            FaceAnimationList::iterator end = mFaceAnimationList.end();
            for ( ; itr != end; ++itr )
            {
                SAFE_DELETE( *itr );
            }
        }
    }

    //---------------------------------------------------------------------
	// �� ������
    //---------------------------------------------------------------------
    LNRESULT Motion::initializeVMD( const void* data_top_, lnU32 size_ )
    {
        mMotionAnimationList.clear();
        mMaxFrames = 0.0;

        const lnByte* data = static_cast< const lnByte* >( data_top_ );

        // �w�b�_�̃`�F�b�N
	    VMD_Header* vmd_header = (VMD_Header *)data;
	    if ( strncmp( vmd_header->szHeader, "Vocaloid Motion Data 0002", 30 ) != 0 )
		    return LN_ERR_FORMAT;	// �t�@�C���`�����Ⴄ

        printf("initializeVMD\n");

        data += sizeof( VMD_Header );

        //-----------------------------------------------------
        // �{�[���A�j���[�V����

	    // �{�[���̃L�[�t���[�������擾
	    unsigned long	all_bone_key_num = *((unsigned long *)data);
	    data += sizeof( unsigned long );

	    VMD_Motion* vmd_motion = (VMD_Motion *)data;

	    for ( lnU32 i = 0 ; i < all_bone_key_num ; ++i, ++vmd_motion )
	    {
            // �ő�t���[���X�V
		    if ( mMaxFrames < (float)vmd_motion->ulFrameNo )	mMaxFrames = (float)vmd_motion->ulFrameNo;	

            // �����A�j���[�V��������
            AnimationIndexMap::iterator itr = mAnimationIndexMap.find( vmd_motion->szBoneName );

            // �V�����{�[��
            if ( itr == mAnimationIndexMap.end() )
            {
                // ���O�E�C���f�b�N�X�̑Ή�
                mAnimationIndexMap.insert( AnimationIndexPair( LRefTString( vmd_motion->szBoneName ), mMotionAnimationList.size() ) );

                // �A�j���[�V�����쐬�A�L�[�ǉ�
                MotionAnimation* anim = LN_NEW MotionAnimation();
                anim->setAnimType( LN_MODELANIM_VMD );
                anim->addVMDKeyFrame(
                    static_cast< double >( vmd_motion->ulFrameNo ),
                    vmd_motion->vec3Position,
                    vmd_motion->vec4Rotation,
                    vmd_motion->cInterpolationX,
                    vmd_motion->cInterpolationY,
                    vmd_motion->cInterpolationZ,
                    vmd_motion->cInterpolationRot );
                mMotionAnimationList.push_back( anim );
            }
            // ���łɂ���{�[��
            else
            {
                mMotionAnimationList[ (itr->second) ]->addVMDKeyFrame(
                    static_cast< double >( vmd_motion->ulFrameNo ),
                    vmd_motion->vec3Position,
                    vmd_motion->vec4Rotation,
                    vmd_motion->cInterpolationX,
                    vmd_motion->cInterpolationY,
                    vmd_motion->cInterpolationZ,
                    vmd_motion->cInterpolationRot );
            }
	    }

        // �L�[�t���[�����Ƀ\�[�g
        MotionAnimationList::iterator itr = mMotionAnimationList.begin();
        MotionAnimationList::iterator end = mMotionAnimationList.end();
        for ( ; itr != end; ++itr )
        {
            (*itr)->sortKeyFrame();
        }

        data += sizeof( VMD_Motion ) * all_bone_key_num;

        //-----------------------------------------------------
	    // �\��A�j���[�V����

        unsigned long	all_face_key_num = *((unsigned long *)data);
	    data += sizeof( unsigned long );

	    // ���[�V�����f�[�^���̕\��Ƃ̃L�[�t���[�������J�E���g
	    VMD_Face* vmd_face = (VMD_Face*)data;

        for ( lnU32 i = 0 ; i < all_face_key_num ; ++i, ++vmd_face )
	    {
            // �ő�t���[���X�V
		    if ( mMaxFrames < (float)vmd_face->ulFrameNo )	mMaxFrames = (float)vmd_face->ulFrameNo;	

            // �����A�j���[�V��������
            FaceAnimationIndexMap::iterator itr = mFaceAnimationIndexMap.find( vmd_face->szFaceName );

            // �V�����{�[��
            if ( itr == mFaceAnimationIndexMap.end() )
            {
                // ���O�E�C���f�b�N�X�̑Ή�
                mFaceAnimationIndexMap.insert( FaceAnimationIndexPair( LRefTString( vmd_face->szFaceName ), mFaceAnimationList.size() ) );

                // �A�j���[�V�����쐬�A�L�[�ǉ�
                FaceAnimation* anim = LN_NEW FaceAnimation();
                anim->addKeyFrame( vmd_face->ulFrameNo, vmd_face->fFactor );
                mFaceAnimationList.push_back( anim );
            }
            // ���łɂ���{�[��
            else
            {
                mFaceAnimationList[ (itr->second) ]->addKeyFrame( vmd_face->ulFrameNo, vmd_face->fFactor );
            }
	    }

        // �L�[�t���[�����Ƀ\�[�g
        FaceAnimationList::iterator itr2 = mFaceAnimationList.begin();
        FaceAnimationList::iterator end2 = mFaceAnimationList.end();
        for ( ; itr2 != end2; ++itr2 )
        {
            (*itr2)->sortKeyFrame();
        }

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� �{�[�����ɑΉ�����A�j���[�V������Ԃ�
    //---------------------------------------------------------------------
    MotionAnimation* Motion::findBoneAnimation( const lnChar* name_ )
    {
        lnU32 i = findBoneAnimationIndex( name_ );
        if ( i != LN_INVALID_INDEX )
        {
            return mMotionAnimationList[ i ];
        }
        return NULL;
    }

    //---------------------------------------------------------------------
	// �� �\��ɑΉ�����A�j���[�V������Ԃ�
    //---------------------------------------------------------------------
    FaceAnimation* Motion::findFaceAnimation( const lnChar* name_ )
    {
        lnU32 i = findFaceAnimationIndex( name_ );
        if ( i != LN_INVALID_INDEX )
        {
            return mFaceAnimationList[ i ];
        }
        return NULL;
    }

    //---------------------------------------------------------------------
	// �� �{�[�����ɑΉ�����A�j���[�V�����̔ԍ���Ԃ�
    //---------------------------------------------------------------------
    lnU32 Motion::findBoneAnimationIndex( const lnChar* name_ )
    {
        AnimationIndexMap::iterator itr = mAnimationIndexMap.find( name_ );
        if ( itr != mAnimationIndexMap.end() )
        {
            return itr->second;
        }
        return LN_INVALID_INDEX;
    }

    //---------------------------------------------------------------------
	// �� �\��ɑΉ�����A�j���[�V�����̔ԍ���Ԃ�
    //---------------------------------------------------------------------
    lnU32 Motion::findFaceAnimationIndex( const lnChar* name_ )
    {
        FaceAnimationIndexMap::iterator itr = mFaceAnimationIndexMap.find( name_ );
        if ( itr != mFaceAnimationIndexMap.end() )
        {
            return itr->second;
        }
        return LN_INVALID_INDEX;
    }

    //---------------------------------------------------------------------
	// �� ���
    //---------------------------------------------------------------------
    //void Motion::_release()
    //{

    //}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================