//=============================================================================
//�y ModelIK �z
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

/// IK�v�Z���̉�]�������N���X
class IKRotationAxisLimit
{
public:

    /// �R���X�g���N�^
    IKRotationAxisLimit()
    {
        mLimits[0] = mLimits[1] = mLimits[2] = false;
    }

    /// �R���X�g���N�^
    IKRotationAxisLimit( bool x_, bool y_, bool z_ )
    {
        mLimits[0] = x_;
        mLimits[1] = y_;
        mLimits[2] = z_;
    }

public:

    /// �e���̐����̐ݒ�
    void setLimits( bool x_, bool y_, bool z_ )
    {
        mLimits[0] = x_;
        mLimits[1] = y_;
        mLimits[2] = z_;
    }

    /// ��]�������̓K�p
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

/// ��]�����N���X
class IKRotationLimit
{
public:

    /// �R���X�g���N�^
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

    /// ���������p�x��Ԃ�
    bool adjust( lnfloat* value_, int index_ )
    {
        lnfloat min = mMinRot[index_];
        lnfloat max = mMaxRot[index_];

        //return value_;

        //_p(value_);

        //�p�x���t�Ȃ����ւ��Ă���
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


        // -PI �Ŋ��S�ɂЂ����Ȃ������

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

public: // ������Ƃ߂�ǂ��̂�

    lnfloat mMaxRot[3];         ///< �ő��]
    lnfloat mMinRot[3];         ///< �ŏ���]
    bool    mMirror[3];         ///< �p�x�̔��˒����@�\�g�p�t���O (IK��CCD�\���u�͑���IK���������ɂȂ銴���̉����o���Ă���̂ŁA���˂��Ă��Ə�肭�������ۂ�)
    lnfloat mRestitution[3];    ///< �p�x�̔��˒����̔����W��
    //lnfloat mStickness[3];      ///< �p���x��"�S��"�W���BIK�̃\���u�̉ߒ��ŉ���"���"�̂�h�����߂ɐݒ�
};

/// IK �����N���X
class ModelIKLimitter
{
public:

    /// PMD �W���̐ݒ�
    ModelIKLimitter()
    {
        // ��]������
        //mIKRotationAxisLimits.insert(
        //    IKRotationAxisLimitPair( _T( "����" ), IKRotationAxisLimit( false, true, false ) ) );
        //mIKRotationAxisLimits.insert(
        //    IKRotationAxisLimitPair( _T( "�E��" ), IKRotationAxisLimit( false, true, false ) ) );

        // ��]�p�x����
        IKRotationLimit lim1;
        lim1.mMinRot[0] = -LMath::PI;
        lim1.mMaxRot[0] = -LMath::DegToRad( 3.0f );//-0.002f;////3�x���炢������݂��Ă��Ə�肭�����B
        //lim1.mMinRot[0] = LMath::DegToRad( 3.0f );    // �E����W�n
        //lim1.mMaxRot[0] = LMath::PI;
        lim1.mMinRot[1] = 0;
        lim1.mMaxRot[1] = 0;
        lim1.mMinRot[2] = 0;
        lim1.mMaxRot[2] = 0;
        lim1.mMirror[0] = true;
        lim1.mRestitution[0] = 0.99f;
        mIKRotationLimits.insert( IKRotationLimitPair( _T( "���Ђ�" ), lim1 ) );

        IKRotationLimit lim2;
        lim2.mMinRot[0] = -LMath::PI;
        lim2.mMaxRot[0] = -LMath::DegToRad( 3.0f );//-0.002f;//
        //lim2.mMinRot[0] = LMath::DegToRad( 3.0f );    // �E����W�n
        //lim2.mMaxRot[0] = LMath::PI;
        lim2.mMinRot[1] = 0;
        lim2.mMaxRot[1] = 0;
        lim2.mMinRot[2] = 0;
        lim2.mMaxRot[2] = 0;
        lim2.mMirror[0] = true;
        lim2.mRestitution[0] = 0.99f;
        mIKRotationLimits.insert( IKRotationLimitPair( _T( "�E�Ђ�" ), lim2 ) );
    }

public:

    /// ��]�������̓K�p
    void adjustAxis( const lnChar* bone_name_, LVector3* target_axis_ )
    {
        IKRotationAxisLimits::iterator itr = mIKRotationAxisLimits.find( bone_name_ );
        if ( itr != mIKRotationAxisLimits.end() )
        {
            itr->second.adjust( target_axis_ );
        }
    }

    /// ��]�p�x�����̓K�p
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
// �� ModelIK �N���X
//-----------------------------------------------------------------------------
///**
//  @brief
//
//  @note
//              IK �f�[�^�͋��L���Ďg����B
//              ���̂��߁AIK �^�[�Q�b�g�{�[���̓|�C���^�ł͂Ȃ��C���f�b�N�X�Ŏ����Ă����B
//*/
//=============================================================================
class ModelIK
{
public:

    ModelIK();

    virtual ~ModelIK();

public:

    /// ������
    void initializePMD( ModelCore* model_core_, const PMD_IK* ik_data_, ModelFrame* frames_ );

    /// �K�p (���[�V�����K�p��A�{�[���̃��[���h�s�񂪊K�w�X�V���ꂽ��ɌĂ�)
    void update( ModelFrame* frames_ );

    /// ModelCore ���̔�r�֐�����Ă΂��
    inline short getSortVal() { return mSortVal; }

private:

    /// ���
    void _release();

    /// �{�[���̉�]�p�x�𐧌�����
    void _limitAngle( LQuaternion* pvec_out_, const LQuaternion& pvec_src_ );

private:

    ModelCore*  mModelCore;

    lnU32       mTargetBoneIdx;	///< IK�^�[�Q�b�g�{�[��
    lnU32       mEffBoneIdx;	///< IK��[�{�[�� (�G�t�F�N�^)

    lnU16	    mCalcCount;     ///< �ċA���Z��
    float       mFact;          ///< IK �e���x
    short		mSortVal;       ///< IK �f�[�^�\�[�g�p�̊�l

    lnU8	    mLinkNum;	    ///< IK���\������{�[���̐�
    lnU32*		mBoneIndexList; ///< IK���\������{�[���ԍ��̔z�� Child

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