//=============================================================================
//�y EffectCore �z
//-----------------------------------------------------------------------------
///**
//  @file       EffectCore.h
//  @brief      EffectCore
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <algorithm>
#include "../Types.h"
#include "EffectAnimationTrackGroup.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Effect
{

//=============================================================================
// �� EffectCore �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�G�t�F�N�g
//*/
//=============================================================================
class EffectCore
    : public Base::ReferenceObject
{
public:

    EffectCore( Manager* manager_ );

    virtual ~EffectCore();

public:

    float   getLocalFPS() const { return mLocalFPS; }
    void    setLocalFPS( float fps_ ) { mLocalFPS = fps_; mModified = true; }

public:

    /// ������ (filename_ �� NULL �̏ꍇ�� ��̃G�t�F�N�g�ō쐬)
    void initialize( const lnChar* filename_ );

    /// Manager �̎擾
    Manager* getManager() { return mManager; }

    /// ���[�g�m�[�h�̎擾
    EffectNodeCore* getRootNode() { return mRootNode; } 

    

    /// ���g�p�m�[�h�C���f�b�N�X�̑���
    //int getUnusedNodeIndex();

    /// XML �t�@�C���ɕۑ�
    void saveToXML( const lnChar* filename_ );

private:

    Manager*            mManager;
    EffectNodeCore*     mRootNode;
    float               mLocalFPS;
    bool                mModified;
};

#if 0
class EffectNode;

//=============================================================================
// �� EffectCore �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�G�t�F�N�g
//*/
//=============================================================================
class EffectCore
    : public Base::ReferenceObject
{
public:

    EffectCore( Manager* manager_ );

    virtual ~EffectCore();

public:

    /// ������ (Manager ����Ă΂��)
    void initialize();

    /// Manager �̎擾
    Manager* getManager() { return mManager; }

    /// ���[�g�m�[�h�̎擾
    EffectNode* getRootNode() { return mRootNode; } 

    /// �t���[���ʒu�̐ݒ�
    void setFramePos( lnTime frame_ );

    /// �S�L�[�̃����_���l���Đݒ肷��
    void refreshKeyFrames();

    /// �T�u�Z�b�g���̎擾
    lnU32 getSubsetNum() const { return mSubsets.size(); }

    /// �X�V (�X�V�X���b�h����Ă΂��)
    void update();

    /// �`��
    void drawSubset( lnU32 index_ );

public:

    /// �q�m�[�h�ǉ����ɌĂ΂��
    void addSubset( EffectNode* node_ ) { mSubsets.push_back( node_ ); }

    /// �q�m�[�h�폜���ɌĂ΂��
    void removeSubset( EffectNode* node_ ) { std::remove( mSubsets.begin(), mSubsets.end(), node_ ); }

private:

    Manager*        mManager;
    EffectNode*     mRootNode;
    EffectNodeArray mSubsets;

    lnTime          mFramePos;
};
#endif
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core
} // namespace LNote

#if 0



class EffectNode
{
    void update()
    {
        if (�V�����������鎞�ԂɂȂ���)
        {
            mEffectNodeSubsetArray.push_back(new EffectNodeSubset(this));
        }

        for (mEffectNodeSubsetArray)
        {
            itr->setTime(mCurrentTime - mOffset * i);
        }
    }

    // �~�����������܂��Ă���ꍇ�͍ŏ��ɗv�f�m�ہB�u�����v�̏ꍇ�͂��̓s�x push_back
    //EffectNodeSubsetArray   mEffectNodeSubsetArray; 
    EffectNodeSubset*   mEffectNodeSubset;  // �ЂƂ���
};

class EffectNodeSubset
{
    EffectNodeSubset(EffectNode* node_)
    {
        mAttachedNode = node_;

        for(node_->Children())
        {
            mChildSubsetArray.push_back(new EffectNodeSubset(itr));
        }

        // �X�v���C�g�ȂǐÓI�Ȃ��̂� new ���Ȃ��œ����|�C���^��Ԃ��̂����肩���H
        mEmitterSubset = mAttachedNode->getEmitter()->createEmitterSubset();
    }

 /*   // �A�j���[�V�����X�V (update �ł��������B
    void setTime(time_)
    {
        
    }*/

    /// (time_ �́A�����̌v�Z�����₷�����邽�߂ɐe�̃��[�J���^�C��)
    void update()
    {
        //for(mChildSubsetArray)
        //{
        //    itr->setTime(time_);
        //}

        for (��{�}�X�^�[�g���b�N���X�g)
        {

            // �w��^�C���̑O��̃L�[���擾
            key0, key1

            // �L�[�ɗ����l�𔽉f


            // �l�̎擾

            // �L�[�ɔ��f���������l�����Ƃɖ߂�

            // 
            mCommonParamArray.PosX�Ƃ� = value;
        }

        for (�g���}�X�^�[�g���b�N���X�g)
        {
            // ����

            mEXTrackValues[i] = value;  // �i�[ (�`�掞�Ɋg���g���b�N�ɓn��)
        }
    }

    void draw()
    {
        // �����Őݒ肷��l�́A�V�F�[�_�֓]�����邽�߂� update �ł͐ݒ�ł��Ȃ����́B

        mEmitterSubset->setCommonParam(mCommonParam);

        // �g���g���b�N�̐ݒ�𔽉f
        for (mEXTrackValues)
        {
            mEmitterSubset->setValue(i, mEXTrackValues[i]);
        }
        mEmitterSubset->draw();
    }

    // �T�u�Z�b�g�̃c���[�́A�Ώۂ̃m�[�h�����[�g�Ƃ����ʂ̃c���[�ɂȂ�B
    // = ���Ȃ炸���[�g�͂ЂƂ� EffectNode�B
    // �q�͂��ׂĊǗ����邪�A�Z��͊Ǘ����Ȃ��B(�e�ɔC����)
    EffectNodeSubsetArray   mChildSubsetArray;

    EffectNode* mAttachedNode;

    EmitterSubsetBase* mEmitterSubset;

    CommonParam    mCommonParam;
    FloatArray mEXTrackValues;
};

#endif

//=============================================================================
//								end of file
//=============================================================================