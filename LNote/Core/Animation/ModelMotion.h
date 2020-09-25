//=============================================================================
//【 ModelMotion 】
//-----------------------------------------------------------------------------
///**
//  @file       ModelMotion.h
//  @brief      ModelMotion
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include "../Math/Math.h"
#include "../Geometry/Vector3.h"
#include "../Geometry/Vector4.h"
#include "../Geometry/Quaternion.h"
#include "../Geometry/SQTTransform.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{

enum LNModelAnimationType
{
    LN_MODELANIM_LINEAR = 0,
    LN_MODELANIM_VMD,
};

namespace Animation
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

// ▼ union 内ではコピーコンストラクタを持つクラスは使えないので緊急用
#pragma pack( push, 1 )

struct KeyVector3
{
    lnfloat x, y, z;

    KeyVector3& operator = ( const LVector3& v_ )
    {
        x = v_.x; y = v_.y; z = v_.z;
        return *this;
    }

    LVector3& getVector3() { return (LVector3&)*this; }
    LVector3* getVector3Ptr() { return (LVector3*)this; }
};

/*
struct KeyVector4
{
    lnfloat x, y, z, w;

    KeyVector4& operator = ( const LVector4& v_ )
    {
        x = v_.x; y = v_.y; z = v_.z; w = v_.w;
    }

    LVector4& getVector4() { return (LVector4&)*this; }
    LVector4* getVector4Ptr() { return (LVector4*)this; }
};
*/

struct KeyQuaternion
{
    lnfloat x, y, z, w;

    KeyQuaternion& operator = ( const LQuaternion& v_ )
    {
        x = v_.x; y = v_.y; z = v_.z; w = v_.w;
        return *this;
    }

    LQuaternion& getQuaternion() { return (LQuaternion&)*this; }
};

#pragma pack( pop )

//=============================================================================
// ■ ModelMotion クラス
//=============================================================================
class ModelMotion
{
public:

    /// キーフレームデータ
    struct KeyFrame
    {
	    float	    FrameNo;		// フレーム番号

        union
        {
            /// 線形補間 (X ファイル用)
            struct
            {
                KeyVector3	    Position;	    ///< 位置
	            KeyQuaternion	Rotation;	    ///< 回転
                KeyVector3      Scale;          ///< 拡大
            } Linear;

            /// VMD 用
            struct
            {
	            KeyVector3	    Position;	    // 位置
	            KeyQuaternion	Rotation;	    // 回転(クォータニオン)

	            VMDBezier	    PosXInterBezier;	// X軸移動補間
	            VMDBezier	    PosYInterBezier;	// Y軸移動補間
	            VMDBezier	    PosZInterBezier;	// Z軸移動補間
	            VMDBezier	    RotInterBezier;	    // 回転補間
            } VMD;
        };
    };

public:

    ModelMotion();

public:

    /// キーフレームタイプの設定
    void setAnimType( LNModelAnimationType type_ ) { mType = type_; }

    /// キーフレーム追加 (VMD 用)
    void addVMDKeyFrame(
        float frame_, const LVector3& pos_, const LQuaternion& rot_,
        char* interpolation_x,
	    char* interpolation_y,
	    char* interpolation_z,
	    char* interpolation_rot );

    /// キーフレームのソート
    void sortKeyFrame();

    /// 時間のスケール係数の設定 (基本的に現在のフレームレートを渡すこと)
    void setTimeTickPerSec( lnfloat tick_ ) { mTimeTick = tick_; }

    /// 時間の設定 (tick_ には
    void setTime( double time_ );

    /// 値の取得 (setTime() 後に呼ぶこと)
    const LSQTTransform& getTransform() const { return mTransform; }

private:

    typedef std::vector< KeyFrame >     KeyFrameList;

private:

    LNModelAnimationType    mType;
    KeyFrameList            mKeyFrameList;
    LSQTTransform           mTransform;
    lnfloat                 mTimeTick;

};



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Animation
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================