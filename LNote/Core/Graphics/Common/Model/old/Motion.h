//=============================================================================
//�y Motion �z
//-----------------------------------------------------------------------------
///**
//  @file       Motion.h
//  @brief      Motion
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include <map>
#include "../../../Base/RefString.h"
#include "../../../Math/Vector3.h"
#include "../../../Math/Quaternion.h"
#include "../../../Math/SQTTransform.h"
#include "PMDTypes.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Graphics
{

//=============================================================================
// �� VMDBezier �N���X
//=============================================================================
class VMDBezier
{
public:

	void initialize( float pt_x1_, float pt_y1_, float pt_x2_, float pt_y2_ );

    /// t = 0.0�`1.0
	float getInterValue( float t_ );

private:

	float getYValue( float x_, float x1_, float y1_, float x2_, float y2_ );

private:

	static const int	YVAL_DIV_NUM = 16;
	float	            mYValue[ YVAL_DIV_NUM + 1 ];
	bool	            mIsLinear;
};

//=============================================================================
// �� MotionAnimation �N���X
//=============================================================================
class MotionAnimation
{
public:

    /// �L�[�t���[���f�[�^
    struct KeyFrame
    {
	    float	    FrameNo;		// �t���[���ԍ�

	    LVector3	Position;	    // �ʒu
	    LQuaternion	Rotation;	    // ��](�N�H�[�^�j�I��)

	    VMDBezier	PosXInterBezier;	// X���ړ����
	    VMDBezier	PosYInterBezier;	// Y���ړ����
	    VMDBezier	PosZInterBezier;	// Z���ړ����
	    VMDBezier	RotInterBezier;	    // ��]���
    };

public:

    /// �L�[�t���[���ǉ�
    void addKeyFrame( const VMD_Motion* vmd_motion_ );

    /// �L�[�t���[���̃\�[�g
    void sortKeyFrame();

    /// ���Ԃ̐ݒ� (tick_ �ɂ͊�{�I�Ɍ��݂̃t���[�����[�g��n������)
    void setTime( double time_, lnfloat tick_ );

    /// �l�̎擾 (setTime() ��ɌĂԂ���)
    const LSQTTransform& getTransform() const { return mTransform; }

private:

    typedef std::vector< KeyFrame >     KeyFrameList;

private:

    KeyFrameList    mKeyFrameList;
    LSQTTransform   mTransform;

};

//=============================================================================
// �� FaceAnimation �N���X
//=============================================================================
class FaceAnimation
{
public:

    /// �L�[�t���[���f�[�^
    struct KeyFrame
    {
	    float	    FrameNo;    ///< �t���[���ԍ�
	    lnfloat     Rate;       ///< �u�����h��
    };

public:

    /// �L�[�t���[���ǉ�
    void addKeyFrame( const VMD_Face* vmd_face_ );

    /// �L�[�t���[���̃\�[�g
    void sortKeyFrame();

private:

    typedef std::vector< KeyFrame >     KeyFrameList;

private:

    KeyFrameList    mKeyFrameList;

};

//=============================================================================
// �� VMDAnimation
//=============================================================================
// �{�[�����Ƃ̃L�[�t���[���f�[�^�̃��X�g
//class VMDAnimation
//{
//public:
//
//    //typedef std::vector<  >
//
//public:
//    //char	szBoneName[16];			// �{�[����
//
//    //unsigned long	ulNumKeyFrames;	// �L�[�t���[����
//    //BoneKeyFrame	*pKeyFrames;	// �L�[�t���[���f�[�^�z��
//
//    //MotionDataList	*pNext;
//
//    lnU32   mKeyNum;
//};

//=============================================================================
// �� Motion �N���X
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class Motion
    : public Base::ReferenceObject
{
public:
    Motion();
    LN_REFOBJ_METHODS;

protected:
    virtual ~Motion();

public:

    /// ������
    LNRESULT initializeVMD( const void* data_, lnU32 size_ );

    /// �{�[�����ɑΉ�����A�j���[�V������Ԃ�
    MotionAnimation* findBoneAnimation( const lnChar* name_ );

    /// �\��ɑΉ�����A�j���[�V������Ԃ�
    FaceAnimation* findFaceAnimation( const lnChar* name_ );

    /// �{�[�����ɑΉ�����A�j���[�V�����̔ԍ���Ԃ� (�g��Ȃ�����)
    lnU32 findBoneAnimationIndex( const lnChar* name_ );

    /// �\��ɑΉ�����A�j���[�V�����̔ԍ���Ԃ�
    lnU32 findFaceAnimationIndex( const lnChar* name_ );

private:

    typedef std::vector< MotionAnimation* >     MotionAnimationList;
    typedef std::map< LRefTString, lnU32 >      AnimationIndexMap;
    typedef std::pair< LRefTString, lnU32 >     AnimationIndexPair;

    typedef std::vector< FaceAnimation* >       FaceAnimationList;
    typedef std::map< LRefTString, lnU32 >      FaceAnimationIndexMap;
    typedef std::pair< LRefTString, lnU32 >     FaceAnimationIndexPair;

private:

    double                  mMaxFrames;         ///< �ő�t���[����
    AnimationIndexMap       mAnimationIndexMap;
    MotionAnimationList     mMotionAnimationList;

    FaceAnimationIndexMap   mFaceAnimationIndexMap;
    FaceAnimationList       mFaceAnimationList;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Lumine

//=============================================================================
//								end of file
//=============================================================================