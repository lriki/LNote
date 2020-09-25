//=============================================================================
//�y EffectInstance �z
//-----------------------------------------------------------------------------
///**
//  @file       EffectInstance.h
//  @brief      EffectInstance
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Types.h"
#include "EffectNodeInstance.h"

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
// �� EffectInstance �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�G�t�F�N�g�C���X�^���X�̃��[�g
//*/
//=============================================================================
class EffectInstance
    : public Base::ReferenceObject
{
public:

    EffectInstance( Manager* manager_ );

    virtual ~EffectInstance();

public:

    /// ������
    void initialize( EffectCore* core_ );

    /// ���Ԑi�s�E�t���[���X�V�� Manager ���玩���ōs�� (�f�t�H���g�� true)
    void setEnableAutoUpdate( bool flag_ ) { mEnableAutoUpdate = flag_; }
    bool isEnableAutoUpdate() const { return mEnableAutoUpdate; }

    /// ���Ԃ̐ݒ�
    void setFrame( lnTime frame_pos_ );

    /// ���Ԃ�i�߂�
    void advanceFrame( lnTime delta_frame_ );

#if 0
    
    /// ���[���h�s��̐ݒ�
    void setTransformMatrix( const LMatrix& mat_ ) { mTransformMatrix = mat_; }

    
#endif

    /// ���݂̃t���[���ʒu�ōX�V (�����X�V�̏ꍇ�� Manager ����Ăяo�����)
    void update();

    /// �`�揀��
    void updateDrawContents();

    /// �`��
    void draw();

public:

    

    /// Modified �t���O�� true �ł��邷�ׂĂ̗v�f�ɑ΂��čč\�z���s��
    //void refresh();

private:

    Manager*                        mManager;
    EffectCore*                     mEffectCore;
    EffectNodeInstanceContainer*    mRootContainer;
    lnTime                          mCurrentFramePos;
    lnTime                          mLastFramePos;
    bool                            mEnableAutoUpdate;

    //EffectNodeInstanceGroup* mRootGroup;
    ////EffectNodeInstance* mRootNodeInstance;

    //LMatrix             mTransformMatrix;
    


    //EffectNodeDrawList      mEffectNodeDrawList;
    //EffectNodeInstanceArray mDrawNodeList;

   
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================