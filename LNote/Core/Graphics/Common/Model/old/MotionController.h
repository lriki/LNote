//=============================================================================
//【 MotionController 】
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
/// アタッチできるモーションの最大数
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
    MotionAnimation**   BoneAnimations;///< (対応モーションが無いときはその要素が NULL。要素数はフレーム数(MotionController::mFrameNum)と同じ)
    FaceAnimation**     FaceAnimations;
    
    double              Time;          ///< 経過時間 (1.0 = 1秒)
    
    
    //lnU32*      MotionIndices;     ///< (対応モーションが無いときは LN_INVALID_INDEX。要素数はフレーム数(MotionController::mFrameNum)と同じ)

    //bool*       BindingEnables;     ///< フレームにモーションを適用するかのフラグ (下半身にのみ適用する等…)
};

//=============================================================================
// ■ MotionController クラス
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

    /// モーションのアタッチ
    void attachMotion( lnU32 slot_idx_, Motion* motion_ );

    /// 時間を進め、ボーン行列を更新する (秒)
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