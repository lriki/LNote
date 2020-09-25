//=============================================================================
//�y EffectNodeInstanceGroup �z
//-----------------------------------------------------------------------------
///**
//  @file       EffectNodeInstanceGroup.h
//  @brief      EffectNodeInstanceGroup
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include <list>
#include "../Types.h"

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
// �� EffectNodeInstanceGroup �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�G�t�F�N�g�O���[�v�B�u�������v�p�����[�^�̑Ή��B
//
//  @note
//              ���W�ϊ��s��Ȃǂ͕����m�[�h���ƂɎ��ׂ������o�B
//              �΂��āA�O���[�v�͐������Ȃǂ̋��ʃp�����[�^���Ǘ��B
//
//              �C���[�W�� EffectNodeCore �� EffectNodeInstanceGroup�B
//              EffectNodeCore �� EffectNodeInstance ����Ȃ��̂Œ��ӁB
//*/
//=============================================================================
class EffectNodeInstanceGroup
    : public Base::ReferenceObject
#if LN_EFFECT_USING_NODE_LIST
    , public NodeInstanceGroupDrawList::Node
#endif
{
public:

    EffectNodeInstanceGroup();

    ~EffectNodeInstanceGroup();

public:

    /// ������
    void initlaize( EffectNodeCore* core_, EffectNodeInstanceContainer* container_ );

    /// EffectNodeInstance �쐬
    EffectNodeInstance* createNodeInstance();

    /// �q EffectNodeInstance ���X�g
    EffectNodeInstanceArray& getChildNodeInstanceArray() { return mNodeInstanceArray; }

    /// �S�Ă̗v�f�������I�ɍ폜����
    void removeForcibly();

    /// �S�Ă� Instance ��j������ (�j���t���O�𗧂Ă�)
    void destroyAllInstance();

    /// �����ȃO���[�v�����`�F�b�N����
    bool checkInvalid();

    /// ������ NodeInstance �����ׂč폜����
    void removeInvalidInstances();

    /// �t���[���X�V
    void update( lnTime delta_frame_ );

    /// �`�揀��
    void updateDrawContents();

    void draw();

private:

    EffectNodeCore*                 mEffectNodeCore;
    EffectNodeInstanceContainer*    mContainer; ///< ���̃O���[�v��ێ����Ă���R���e�i
    EffectNodeInstanceArray         mNodeInstanceArray;
    NodeInstanceDrawList            mNodeInstanceDrawList;
    //EffectNodeInstanceList          mNodeInstanceAddedList;

#if 0
    /// �������Ȃǂ̏�Ԃ����Z�b�g
    void reset();

    /// �t���[���ʒu�̐ݒ�(�e�m�[�h�̃��[�J���^�C����n�����ƁB���[�g�ł���΃O���[�o���^�C��)
    void update( lnTime frame_pos_, const LMatrix& parent_matrix_ );

    /// �`�惊�X�g���쐬����
    void updateDrawList( EffectNodeDrawList* draw_node_list_ );

    /// ���̃m�[�h�ȉ��̃f�[�^�N���X�� Modified �t���O�� true �ł��邷�ׂĂ̗v�f�ɑ΂��čč\�z���s��
    void refresh();

    

private:

    /// �������
    void _dispose();

private:

    
    typedef std::list< EffectNodeInstance* >    EffectNodeInstanceList;
    typedef std::vector< EmitterInstanceBase* > EmitterInstanceArray;

private:

    //EffectInstance*         mEffectInstance;
    EffectNodeCore*         mEffectNodeCore;
    EffectNodeInstanceArray mEffectNodeInstanceArray;
    //EffectNodeInstanceList  mAddedNodeList;             ///< update() ���ō쐬���ꂽ�m�[�h���ꎞ�I�Ɋi�[�����
    int                     mGenerateCount;

    lnTime                  mLocalFramePos;             ///< 0 �͍ŏ��̐����ʒu
    lnTime                  mLastLocalFramePos;
    
    EmitterInstanceArray    mDrawEmitterInstanceList;

#endif
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