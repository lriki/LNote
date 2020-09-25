//=============================================================================
//�y EffectNodeInstanceContainer �z
//-----------------------------------------------------------------------------
///**
//  @file       EffectNodeInstanceContainer.h
//  @brief      EffectNodeInstanceContainer
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
// �� EffectNodeInstanceContainer
//-----------------------------------------------------------------------------
///**
//  @brief      �O���[�v�̏W��
//  @note
//              �f�[�^�N���X�� EffectNodeCore �� 1��1 �őΉ�����
//*/
//=============================================================================
class EffectNodeInstanceContainer
    : public Base::ReferenceObject
{
public:

    EffectNodeInstanceContainer( Manager* manager_ );

    virtual ~EffectNodeInstanceContainer();

public:

    /// ������
    void initialize( EffectNodeCore* node_core_, EffectInstance* instance_ );

    /// �G�t�F�N�g�C���X�^���X�̎擾
    EffectInstance* getEffectInstance() { return mEffectInstance; }

    /// �q�R���e�i���X�g�̎擾
    EffectNodeInstanceContainerArray& getChildrenContainer() { return mChildrenContainerArray; }

    /// �O���[�v�̍쐬 (���[�g�R���e�i�A�m�[�h�C���X�^���X�������ɌĂ΂��)
    EffectNodeInstanceGroup* createNodeInstanceGroup();

    /// �S�Ă̗v�f�������I�ɍ폜���� (~EffectNodeInstanceContainer() ����ĂԂƂ��� false)
    void removeForcibly( bool recursive );

    /// �S�Ă� Instance ��j������ (�j���t���O�𗧂Ă�BremoveForcibly() ����ĂԂƂ��� false)
    void destroyAllInstance( bool recursive_ );

    /// �����ȃO���[�v�����ׂč폜����
    void removeInvalidGroups();

    /// �t���[���X�V
    void update( lnTime delta_frame_ );

    /// �`�揀��
    void updateDrawContents();

    /// �`��
    void draw();

private:

    Manager*                            mManager;
    EffectNodeCore*                     mEffectNodeCore;
    EffectInstance*                     mEffectInstance;
    EffectNodeInstanceContainerArray    mChildrenContainerArray;
    EffectNodeInstanceGroupArray        mNodeInstanceGroupArray;
    NodeInstanceGroupDrawList           mNodeInstanceGroupDrawList;
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