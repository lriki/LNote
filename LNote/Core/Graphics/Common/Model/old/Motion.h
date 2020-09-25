//=============================================================================
//【 Motion 】
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
// ■ VMDBezier クラス
//=============================================================================
class VMDBezier
{
public:

	void initialize( float pt_x1_, float pt_y1_, float pt_x2_, float pt_y2_ );

    /// t = 0.0～1.0
	float getInterValue( float t_ );

private:

	float getYValue( float x_, float x1_, float y1_, float x2_, float y2_ );

private:

	static const int	YVAL_DIV_NUM = 16;
	float	            mYValue[ YVAL_DIV_NUM + 1 ];
	bool	            mIsLinear;
};

//=============================================================================
// ■ MotionAnimation クラス
//=============================================================================
class MotionAnimation
{
public:

    /// キーフレームデータ
    struct KeyFrame
    {
	    float	    FrameNo;		// フレーム番号

	    LVector3	Position;	    // 位置
	    LQuaternion	Rotation;	    // 回転(クォータニオン)

	    VMDBezier	PosXInterBezier;	// X軸移動補間
	    VMDBezier	PosYInterBezier;	// Y軸移動補間
	    VMDBezier	PosZInterBezier;	// Z軸移動補間
	    VMDBezier	RotInterBezier;	    // 回転補間
    };

public:

    /// キーフレーム追加
    void addKeyFrame( const VMD_Motion* vmd_motion_ );

    /// キーフレームのソート
    void sortKeyFrame();

    /// 時間の設定 (tick_ には基本的に現在のフレームレートを渡すこと)
    void setTime( double time_, lnfloat tick_ );

    /// 値の取得 (setTime() 後に呼ぶこと)
    const LSQTTransform& getTransform() const { return mTransform; }

private:

    typedef std::vector< KeyFrame >     KeyFrameList;

private:

    KeyFrameList    mKeyFrameList;
    LSQTTransform   mTransform;

};

//=============================================================================
// ■ FaceAnimation クラス
//=============================================================================
class FaceAnimation
{
public:

    /// キーフレームデータ
    struct KeyFrame
    {
	    float	    FrameNo;    ///< フレーム番号
	    lnfloat     Rate;       ///< ブレンド率
    };

public:

    /// キーフレーム追加
    void addKeyFrame( const VMD_Face* vmd_face_ );

    /// キーフレームのソート
    void sortKeyFrame();

private:

    typedef std::vector< KeyFrame >     KeyFrameList;

private:

    KeyFrameList    mKeyFrameList;

};

//=============================================================================
// ■ VMDAnimation
//=============================================================================
// ボーンごとのキーフレームデータのリスト
//class VMDAnimation
//{
//public:
//
//    //typedef std::vector<  >
//
//public:
//    //char	szBoneName[16];			// ボーン名
//
//    //unsigned long	ulNumKeyFrames;	// キーフレーム数
//    //BoneKeyFrame	*pKeyFrames;	// キーフレームデータ配列
//
//    //MotionDataList	*pNext;
//
//    lnU32   mKeyNum;
//};

//=============================================================================
// ■ Motion クラス
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

    /// 初期化
    LNRESULT initializeVMD( const void* data_, lnU32 size_ );

    /// ボーン名に対応するアニメーションを返す
    MotionAnimation* findBoneAnimation( const lnChar* name_ );

    /// 表情名に対応するアニメーションを返す
    FaceAnimation* findFaceAnimation( const lnChar* name_ );

    /// ボーン名に対応するアニメーションの番号を返す (使わないかも)
    lnU32 findBoneAnimationIndex( const lnChar* name_ );

    /// 表情名に対応するアニメーションの番号を返す
    lnU32 findFaceAnimationIndex( const lnChar* name_ );

private:

    typedef std::vector< MotionAnimation* >     MotionAnimationList;
    typedef std::map< LRefTString, lnU32 >      AnimationIndexMap;
    typedef std::pair< LRefTString, lnU32 >     AnimationIndexPair;

    typedef std::vector< FaceAnimation* >       FaceAnimationList;
    typedef std::map< LRefTString, lnU32 >      FaceAnimationIndexMap;
    typedef std::pair< LRefTString, lnU32 >     FaceAnimationIndexPair;

private:

    double                  mMaxFrames;         ///< 最大フレーム数
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