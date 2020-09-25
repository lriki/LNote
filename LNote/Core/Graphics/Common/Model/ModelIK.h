//=============================================================================
//【 ModelIK 】
//-----------------------------------------------------------------------------
///**
//  @file       ModelIK.h
//  @brief      ModelIK
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <map>
#include "../../../Base/RefString.h"
#include "PMDTypes.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
class ModelFrame;

/// IK計算時の回転軸制限クラス
class IKRotationAxisLimit
{
public:

    /// コンストラクタ
    IKRotationAxisLimit()
    {
        mLimits[0] = mLimits[1] = mLimits[2] = false;
    }

    /// コンストラクタ
    IKRotationAxisLimit( bool x_, bool y_, bool z_ )
    {
        mLimits[0] = x_;
        mLimits[1] = y_;
        mLimits[2] = z_;
    }

public:

    /// 各軸の制限の設定
    void setLimits( bool x_, bool y_, bool z_ )
    {
        mLimits[0] = x_;
        mLimits[1] = y_;
        mLimits[2] = z_;
    }

    /// 回転軸制限の適用
    void adjust( LVector3* axis_ )
    {
        if ( mLimits[0] )
        {
            axis_->x = 0.0f;
        }
        if ( mLimits[1] )
        {
            axis_->y = 0.0f;
        }
        if ( mLimits[2] )
        {
            axis_->z = 0.0f;
        }
    }

private:
    
    bool    mLimits[3];
};

/// 回転制限クラス
class IKRotationLimit
{
public:

    /// コンストラクタ
    IKRotationLimit()
    {
        for (int i = 0; i < 3; i++)
        {
            mMaxRot[i] = LMath::PI;
            mMinRot[i] = -LMath::PI;
            mMirror[i] = false;
            mRestitution[i] = 0.5f;
            //mStickness[i] = 0.0f;
        }
    }

public:

    /// 制限した角度を返す
    bool adjust( lnfloat* value_, int index_ )
    {
        lnfloat min = mMinRot[index_];
        lnfloat max = mMaxRot[index_];

        //return value_;

        //_p(value_);

        //角度が逆なら入れ替えておく
        if ( min > max )
        {
            mMinRot[index_] = max;
            mMaxRot[index_] = min;
            min = mMinRot[index_];
            max = mMaxRot[index_];
        }

/*
        if ( max < value_ )
        {
            if ( mMirror[index_] )
                return max * (1.0f + mRestitution[index_]) - value_ * mRestitution[index_];
            else
                return max;
        }
        else if ( min > value_ )
        {
            if ( mMirror[index_] )
                return min * (1.0f + mRestitution[index_]) - value_ * mRestitution[index_];
            else
                return min;
        }
        else
        {
            return value_;
        }

         */


        // -PI で完全にひざを曲げた状態

        if ( max < *value_ )
        {
            if ( mMirror[index_] )
                *value_ = max * (1.0f + mRestitution[index_]) - *value_ * mRestitution[index_];
            else
                *value_ = max;
            return true;
        }
        else if ( min > *value_ )
        {
            if ( mMirror[index_] )
                *value_ = min * (1.0f + mRestitution[index_]) - *value_ * mRestitution[index_];
            else
                *value_ = min;
            return true;
        }
        else
        {
            //*value_ = value_;
            return false;
        }
       


    }

public: // ちょっとめんどいので

    lnfloat mMaxRot[3];         ///< 最大回転
    lnfloat mMinRot[3];         ///< 最小回転
    bool    mMirror[3];         ///< 角度の反射調整機能使用フラグ (IKのCCDソルブは足のIKが＜が＞になる感じの解を出してくるので、反射してやると上手くいくっぽい)
    lnfloat mRestitution[3];    ///< 角度の反射調整の反発係数
    //lnfloat mStickness[3];      ///< 角速度の"粘性"係数。IKのソルブの過程で解が"飛ぶ"のを防ぐために設定
};

/// IK 制限クラス
class ModelIKLimitter
{
public:

    /// PMD 標準の設定
    ModelIKLimitter()
    {
        // 回転軸制限
        //mIKRotationAxisLimits.insert(
        //    IKRotationAxisLimitPair( _T( "左足" ), IKRotationAxisLimit( false, true, false ) ) );
        //mIKRotationAxisLimits.insert(
        //    IKRotationAxisLimitPair( _T( "右足" ), IKRotationAxisLimit( false, true, false ) ) );

        // 回転角度制限
        IKRotationLimit lim1;
        lim1.mMinRot[0] = -LMath::PI;
        lim1.mMaxRot[0] = -LMath::DegToRad( 3.0f );//-0.002f;////3度ぐらい制限を設けてやると上手くいく。
        //lim1.mMinRot[0] = LMath::DegToRad( 3.0f );    // 右手座標系
        //lim1.mMaxRot[0] = LMath::PI;
        lim1.mMinRot[1] = 0;
        lim1.mMaxRot[1] = 0;
        lim1.mMinRot[2] = 0;
        lim1.mMaxRot[2] = 0;
        lim1.mMirror[0] = true;
        lim1.mRestitution[0] = 0.99f;
        mIKRotationLimits.insert( IKRotationLimitPair( _T( "左ひざ" ), lim1 ) );

        IKRotationLimit lim2;
        lim2.mMinRot[0] = -LMath::PI;
        lim2.mMaxRot[0] = -LMath::DegToRad( 3.0f );//-0.002f;//
        //lim2.mMinRot[0] = LMath::DegToRad( 3.0f );    // 右手座標系
        //lim2.mMaxRot[0] = LMath::PI;
        lim2.mMinRot[1] = 0;
        lim2.mMaxRot[1] = 0;
        lim2.mMinRot[2] = 0;
        lim2.mMaxRot[2] = 0;
        lim2.mMirror[0] = true;
        lim2.mRestitution[0] = 0.99f;
        mIKRotationLimits.insert( IKRotationLimitPair( _T( "右ひざ" ), lim2 ) );
    }

public:

    /// 回転軸制限の適用
    void adjustAxis( const lnChar* bone_name_, LVector3* target_axis_ )
    {
        IKRotationAxisLimits::iterator itr = mIKRotationAxisLimits.find( bone_name_ );
        if ( itr != mIKRotationAxisLimits.end() )
        {
            itr->second.adjust( target_axis_ );
        }
    }

    /// 回転角度制限の適用
    bool adjustRotation( ModelFrame* bone_ );

private:

    typedef std::map< LRefTString, IKRotationAxisLimit >    IKRotationAxisLimits;
    typedef std::pair< LRefTString, IKRotationAxisLimit >   IKRotationAxisLimitPair;

    typedef std::map< LRefTString, IKRotationLimit >        IKRotationLimits;
    typedef std::pair< LRefTString, IKRotationLimit >       IKRotationLimitPair;

private:

    IKRotationAxisLimits    mIKRotationAxisLimits;
    IKRotationLimits        mIKRotationLimits;
};

//=============================================================================
// ■ ModelIK クラス
//-----------------------------------------------------------------------------
///**
//  @brief
//
//  @note
//              IK データは共有して使われる。
//              そのため、IK ターゲットボーンはポインタではなくインデックスで持っておく。
//*/
//=============================================================================
class ModelIK
{
public:

    ModelIK();

    virtual ~ModelIK();

public:

    /// 初期化
    void initializePMD( ModelCore* model_core_, const PMD_IK* ik_data_, ModelFrame* frames_ );

    /// 適用 (モーション適用後、ボーンのワールド行列が階層更新された後に呼ぶ)
    void update( ModelFrame* frames_ );

    /// ModelCore 内の比較関数から呼ばれる
    inline short getSortVal() { return mSortVal; }

private:

    /// 解放
    void _release();

    /// ボーンの回転角度を制限する
    void _limitAngle( LQuaternion* pvec_out_, const LQuaternion& pvec_src_ );

private:

    ModelCore*  mModelCore;

    lnU32       mTargetBoneIdx;	///< IKターゲットボーン
    lnU32       mEffBoneIdx;	///< IK先端ボーン (エフェクタ)

    lnU16	    mCalcCount;     ///< 再帰演算回数
    float       mFact;          ///< IK 影響度
    short		mSortVal;       ///< IK データソート用の基準値

    lnU8	    mLinkNum;	    ///< IKを構成するボーンの数
    lnU32*		mBoneIndexList; ///< IKを構成するボーン番号の配列 Child

};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================