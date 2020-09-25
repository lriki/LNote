//=============================================================================
//�y EmitterBase �z
//-----------------------------------------------------------------------------
///**
//  @file       EmitterBase.h
//  @brief      EmitterBase
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Interface.h"
#include "../Types.h"
#include "../Manager.h"
#include "../Core/EffectNodeCore.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Effect
{
class EmitterInstanceBase;

//=============================================================================
// �� EmitterCoreBase �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�G�t�F�N�g�G�~�b�^�̊��
//*/
//=============================================================================
class EmitterCoreBase
    : public Base::ReferenceObject
{
public:

    EmitterCoreBase( Manager* manager_ );

    virtual ~EmitterCoreBase() {}

public:

    /// Modified �t���O��ݒ肷��
    void setModifiedFlag( bool flag_ ) { mModified = flag_; }

    /// �g���g���b�N���̎擾
    virtual int getExTrackGroupNum() const { return 0; }

    /// �g���g���b�N�O���[�v�̎擾
    virtual EffectAnimationTrackGroup* getExTrackGroup( int idx_ ) { return NULL; }

    /// �R���e�i�̕`��J�n
    virtual void beginContainerRendering() {}

    /// �O���[�v�̕`��J�n
    virtual void beginGroupRendering() {}

    /// �`��
    virtual void draw( EffectNodeInstance* instance_ ) {}

    /// �O���[�v�̕`��I��
    virtual void endGroupRendering() {}

    /// �R���e�i�̕`��I��
    virtual void endContainerRendering() {}


    /// �G�~�b�^�C���X�^���X�N���X�̐��� (�������p�����[�^�ɂ�鎩�������̂���)
    //virtual EmitterInstanceBase* createInstance() = 0;

protected:

    Manager*    mManager;
    bool        mModified;
};

#if 0

//=============================================================================
// �� EmitterInstanceBase �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�G�~�b�^�C���X�^���X�̊��
//*/
//=============================================================================
class EmitterInstanceBase
    : public Base::ReferenceObject
{
public:

    EmitterInstanceBase()
    {}

    virtual ~EmitterInstanceBase() {}

public:

    /// �f�[�^�N���X�� Modified �t���O�� true �ł���΍č\�z���s��
    virtual void refresh() {}

    /// �`����̍X�V
    virtual void updateDrawContents() {}

    /// �`��
    virtual void draw( const LMatrix& world_mat_ ) {}

protected:

};

#endif

#if 0
//=============================================================================
// �� EmitterBase �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�G�t�F�N�g�G�~�b�^�̊��
//*/
//=============================================================================
class EmitterBase
    : public Base::ReferenceObject
{
public:

    EmitterBase( EffectNode* node_ )
        : mEffectNode( node_ )
    { }

    virtual ~EmitterBase() { }

public:

    /// ���ʃX�e�[�g�̍Đݒ� (���������̍Đݒ�Ȃǂ��s��)
    void refreshCommonState( const EffectNodeCommonParameter& param_ );

    /// �S�L�[�̃����_���l���Đݒ肷��
    void refreshKeyFrames( const MasterCommonAnimationTrackArray& master_tracks_ );

    //void update(  );

public:

    /// �G�~�b�^�`��N���X�̐��� (�������p�����[�^�ɂ�鎩�������̂���)
    virtual EmitterSubsetBase* createSubset() = 0;

    /// �`��
    virtual void draw();

protected:

    EffectNode*         mEffectNode;
    EmitterSubsetArray  mEmitterSubsetArray;
};

//=============================================================================
// �� EmitterSubsetBase �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�G�t�F�N�g�G�~�b�^�`��N���X�̊��
//*/
//=============================================================================
class EmitterSubsetBase
{
public:

    /// �S�L�[�̃����_���l���Đݒ肷��
    void refreshKeyFrames( const MasterCommonAnimationTrackArray& master_tracks_ );

    /// �`��
    virtual void draw() = 0;
};
#endif
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================