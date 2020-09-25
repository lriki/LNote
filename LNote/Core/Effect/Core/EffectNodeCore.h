//=============================================================================
//�y EffectNodeCore �z
//-----------------------------------------------------------------------------
///**
//  @file       EffectNodeCore.h
//  @brief      EffectNodeCore
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include "../../Math/Random.h"
#include "../../Geometry/Matrix.h"
#include "../../Graphics/Common/RenderState.h"
#include "../../Base/HierarchyNode.h"
#include "EffectMasterAnimationTrack.h"
#include "EffectAnimationTrackGroup.h"
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

//struct EffectNodeCommonParameter
//{
//    
//
//    // �����l
//    EffectNodeCommonParameter()
//    {
//        Visible = true;
//        Name = _T( "Node" );
//        GenerateStartFramePos = 0;
//        GenerateInterval = 1;
//    }
//};

//=============================================================================
// �� EffectNodeCore �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�G�t�F�N�g�m�[�h
//*/
//=============================================================================
class EffectNodeCore
    : public Base::HierarchyNode< EffectNodeCore >
{
public:

    
public:

    EffectNodeCore();

    virtual ~EffectNodeCore();

public:

    /// ������
    void initialize();

    /// �g���g���b�N���܂߂��g���b�N�O���[�v�̐�
    int getAnimationTrackGroupNum();

    /// �g���b�N�O���[�v
    EffectAnimationTrackGroup* getAnimationTrackGroup( int idx_ );

    /// �G�~�b�^�̐ݒ�(�G�f�B�^����؂�ւ���p)
    void setEmitterCore( EmitterCoreBase* emitter_ );

    EmitterCoreBase* getEmitterCore() { return mEmitter; }

    /// ���ʃp�����[�^�̎擾
    //EffectNodeCommonParameter* getCommonParameter() { return &mCommonParameter; }

    void        setName( LRefTString name_ ) { mName = name_; mModified = true; }

    LRefTString getName() const { return mName; }

    void        setMaxGenerateCount( int count_ ) { mMaxGenerateCount = count_; mModified = true; }

    int         getMaxGenerateCount() const { return mMaxGenerateCount; }

    void        setGenerateStartFramePos( lnTime interval_ ) { mGenerateStartFramePos = interval_; mModified = true; }

    lnTime      getGenerateStartFramePos() const { return mGenerateStartFramePos; }

    void        setGenerateInterval( lnTime interval_ ) { mGenerateInterval = interval_; mModified = true; }

    lnTime      getGenerateInterval() const { return mGenerateInterval; }

    bool        isModified() const { return mModified; }

    /// ���̃m�[�h�ȉ��� Modified �t���O��ݒ肷��
    void setModifiedFlag( bool flag_ );

public:

    /// �q�m�[�h��ǉ�����
    virtual void addChild( NodeType* child_ );

    /// �q�m�[�h���O��
    virtual void removeChild( NodeType* child_ );

    /// XML �ɕۑ� (�쐬���ꂽ�v�f��Ԃ�)
    tinyxml2::XMLElement* saveToXML( tinyxml2::XMLDocument* doc_ );

    /// XML �v�f����쐬 (initialize() �Ƃ͈ꏏ�ɌĂ΂Ȃ�����)
    void loadFromXML( tinyxml2::XMLElement* elm_ );

    /// XML �e�L�X�g�ɃV���A���C�Y (�R�s�y�p)
    LRefTString serializeXML();

    /// XML �e�L�X�g����f�V���A���C�Y (�R�s�y�p) (initialize() �Ƃ͈ꏏ�ɌĂ΂Ȃ�����)
    void deserializeXML( LRefTString str );

private:

    LRenderState                    mRenderState;

    EffectAnimationTrackGroup       mCommonTrackGroupArray[ LN_EFFECT_COMMONTRACK_GROUPE_NUM ];
  
    EmitterCoreBase*                mEmitter;

    //EffectNodeCommonParameter       mCommonParameter;

    bool        mVisible;
    LRefTString mName;

    int         mMaxGenerateCount;
    lnTime      mGenerateStartFramePos;  ///< �����J�n�t���[���ʒu (�e�m�[�h��GenerateStartFramePos����̃I�t�Z�b�g)
    lnTime      mGenerateInterval;       ///< �����Ԋu (1�ȏ�ł��邱�ƁB�������� 2 �ȏ�̏ꍇ�Ɏg�p)

    //Animation::Scalar   mCommonAnimationTracks

    bool        mModified;
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