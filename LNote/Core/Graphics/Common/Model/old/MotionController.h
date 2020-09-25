//=============================================================================
//�y MotionController �z
//-----------------------------------------------------------------------------
///**
//  @file       MotionController.h
//  @brief      MotionController
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Motion.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
/// �A�^�b�`�ł��郂�[�V�����̍ő吔
static const int LN_MAX_ATTACH_MOTIONS = 4;

namespace Graphics
{

class ModelFace;



class AttachedMotionData
{
public:

    AttachedMotionData()
        : AttachedMotion    ( NULL )
        , BoneAnimations    ( NULL )
        , FaceAnimations    ( NULL )
        , Time              ( 0.0 )
    {}

    ~AttachedMotionData()
    {
        SAFE_DELETE_ARRAY( BoneAnimations );
        SAFE_DELETE_ARRAY( FaceAnimations );
    }

    void initialize( lnU32 frame_num_ )
    {
        BoneAnimations = LN_NEW MotionAnimation*[ frame_num_ ];
        FaceAnimations = LN_NEW FaceAnimation*[ frame_num_ ];
        //MotionIndices = LN_NEW lnU32[ frame_num_ ];
    }

public:

    Motion*             AttachedMotion;
    MotionAnimation**   BoneAnimations;///< (�Ή����[�V�����������Ƃ��͂��̗v�f�� NULL�B�v�f���̓t���[����(MotionController::mFrameNum)�Ɠ���)
    FaceAnimation**     FaceAnimations;
    
    double              Time;          ///< �o�ߎ��� (1.0 = 1�b)
    
    
    //lnU32*      MotionIndices;     ///< (�Ή����[�V�����������Ƃ��� LN_INVALID_INDEX�B�v�f���̓t���[����(MotionController::mFrameNum)�Ɠ���)

    //bool*       BindingEnables;     ///< �t���[���Ƀ��[�V������K�p���邩�̃t���O (�����g�ɂ̂ݓK�p���铙�c)
};

//=============================================================================
// �� MotionController �N���X
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class MotionController
{

public:

    MotionController( ModelFrame* frames_, lnU32 frame_num_, ModelFace* faces_, lnU32 face_num_ );

    virtual ~MotionController();

public:

    /// ���[�V�����̃A�^�b�`
    void attachMotion( lnU32 slot_idx_, Motion* motion_ );

    /// ���Ԃ�i�߁A�{�[���s����X�V���� (�b)
    void advanceTime( lnfloat elapsed_time_ );

private:

    lnU32       mModelFrameNum;
    ModelFrame* mModelFrames;

    lnU32       mModelFaceNum;
    ModelFace*  mModelFaces;

    AttachedMotionData  mAttachedMotionDataArray[ LN_MAX_ATTACH_MOTIONS ];
    
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Lumine

//=============================================================================
//								end of file
//=============================================================================