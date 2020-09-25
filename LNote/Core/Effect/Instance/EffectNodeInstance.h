//=============================================================================
//�y EffectNodeInstance �z
//-----------------------------------------------------------------------------
///**
//  @file       EffectNodeInstance.h
//  @brief      EffectNodeInstance
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
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
typedef Base::NodeList< EffectNodeInstance >    EffectNodeDrawList;

//=============================================================================
// �� EffectNodeInstance �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�G�t�F�N�g�m�[�h
//*/
//=============================================================================
class EffectNodeInstance
    : public Base::ReferenceObject
#if LN_EFFECT_USING_NODE_LIST
    , public NodeInstanceDrawList::Node
#endif
{
public:

    enum State
    {
	    STATE_ACTIVE = 0,
	    STATE_REMOVING,
	    //STATE_REMOVED,
    };

    struct DrawContext
    {
        lnTime  LocalFramePos;
        LMatrix WorldMatrix;
    };

public:

    EffectNodeInstance();

    ~EffectNodeInstance();

public:

    /// ������
    void initialize( EffectNodeCore* core_, EffectNodeInstanceContainer* container_ );

    /// �t���[���X�V
    void update( lnTime delta_frame_ );

    /// �`�揀��
    void updateDrawContents();

    /// �`��
    void draw();

    /// �폜
    void destroy() { mState = STATE_REMOVING; }

    /// ��Ԃ̎擾
    State getState() const { return mState; }

    /// Emitter �ɓ`���闐���V�[�h
    int getRandSeed() const { return (int)this; }

    /// �`��f�[�^
    DrawContext* getDrawContext() { return &mDrawContext; }

private:

    EffectNodeCore*                 mEffectNodeCore;
    EffectNodeInstanceContainer*    mContainer;
    EffectNodeInstanceGroupArray    mChildrenContainsGroupArray;    ///< update() ����쐬�����q Instance ���i�[����O���[�v�̃��X�g
    State                           mState;
    lnTime                          mLocalFramePos;
    lnTime                          mLivedFrame;        ///< �����t���[���� (�����̏ꍇ�� DBL_MAX)
    LMatrix                         mWorldMatrix;       ///< �����ςݍs��
    DrawContext                     mDrawContext;
    



#if 0
    /// [�`�悩��Ă΂��]
    const LMatrix& getWorldMatrix() { return mWorldMatrixForDraw; }

    /// �t���[���ʒu�̐ݒ�(frame_pos_ �̓O���[�v�̃��[�J���^�C��)
    void update( lnTime frame_pos_, const LMatrix& parent_matrix_ );

    /// �`�惊�X�g���쐬����
    void updateDrawList( EffectNodeDrawList* draw_node_list_ );

    /// ���̃m�[�h�ȉ��� Modified �t���O�� true �ł��邷�ׂĂ̗v�f�ɑ΂��čč\�z���s��
    void refresh();

    
    /// �G�~�b�^�C���X�^���X�̎擾
    //EmitterInstanceBase* getEmitterInstance() { return mEmitterInstance; }

private:

    typedef std::vector< EffectNodeInstanceGroup* > EffectNodeInstanceGroupArray;

private:

    EffectNodeCore*                 mEffectNodeCore;
    EffectNodeInstanceGroupArray    mEffectNodeInstanceGroupArray;  ///< �q�m�[�h�̐����̗v�f������
    //EmitterInstanceBase*            mEmitterInstance;
    int                             mRandSeed;

    lnTime              mGroupFramePosOffset;         ///< �O���[�v���̐����I�t�Z�b�g
    lnTime              mLocalFramePos;  ///< ���[�J���̃t���[���ʒu(�J�n�ʒu�ɂ���Ă͕��̒l�ɂ��Ȃ�)
    LMatrix             mWorldMatrix;       ///< �����ςݍs��

    LMatrix             mWorldMatrixForDraw;       ///< �����ςݍs�� (DrawContent)

#endif
};

#if 0

struct EffectNodeCommonParameter
{
    bool        Visible;
    LRefTString Name;
    lnfloat     GenerateStartFramePos;  ///< �����J�n�t���[���ʒu
    lnfloat     GenerateInterval;       ///< �����Ԋu (1�ȏ�ł��邱�ƁB�������� 2 �ȏ�̏ꍇ�Ɏg�p)

    // �����l
    EffectNodeCommonParameter()
    {
        Visible = true;
        Name = _T( "Node" );
        GenerateStartFramePos = 0;
        GenerateInterval = 1;
    }
};

//=============================================================================
// �� EffectNode �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�G�t�F�N�g�m�[�h
//*/
//=============================================================================
class EffectNode
    : public Base::HierarchyNode< EffectNode >
{
public:

    EffectNode( EffectCore* core_ );

    virtual ~EffectNode();

public:

    /// ������ (���� ����Ă΂��)
    void initialize();

    /// EffectCore �̎擾
    EffectCore* getEffectCore() { return mEffectCore; }

    /// �G�~�b�^�̐ݒ�
    void setEmitter( EmitterBase* emitter_ );

    /// �}�X�^�g���b�N�̎擾
    EffectMasterAnimationTrack* getMasterTrack( LNEffectCommonTrack track_ ) { return &mMasterCommonAnimationTrackArray[ (int)track_ ]; }

    /// [�K�v�Ȃ��Ȃ邩���H]�S�L�[�̃����_���l���Đݒ肷�� (�K�w�X�V�B���������̕��ׂ��C�ɂȂ�悤�Ȃ�A�����update() �ŌĂ�ł���������(�ʃX���b�h�ōX�V����ꍇ))
    void refreshKeyFrames();

    /// �X�V (Core ����z��C�e���[�g�ɂ���ČĂ΂��B���A�X�V�X���b�h)
    void update();

    /// �`��
    void draw();

public:

    /// �q�m�[�h��ǉ�����
    virtual void addChild( NodeType* child_ );

    /// �q�m�[�h���O��
    virtual void removeChild( NodeType* child_ );

private:

    EffectCore*                     mEffectCore;

    LRenderState                    mRenderState;
    MasterCommonAnimationTrackArray mMasterCommonAnimationTrackArray;   ///< �L�[�t���[���̃}�X�^�f�[�^ (refreshKeyFrames() �� ��������� Emitter �ɃL�[���\�z����)
    //AnimationTracks     mAnimationTracks;

    LMatrix	                        mTransformMatrix;
	LMatrix	                        mCombinedTransformMatrix;

    EmitterBase*                    mEmitter;

    EffectNodeCommonParameter       mCommonParameter;
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