//=============================================================================
//�y EffectAnimationTrack �z
//-----------------------------------------------------------------------------
///**
//  @file       EffectAnimationTrack.h
//  @brief      EffectAnimationTrack
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
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
// �� IAnimationTrack
//-----------------------------------------------------------------------------
///**
//  @brief		
//
//  @note
//              �L�[�̍쐬�A�ړ��Ȃǂ͂��ׂ� Managed ���ōs���B
//              �L�[�̏�Ԃ��ύX�ɂȂ邽�тɂ��ׂẴL�[�� Native �ɔ��f������`�B
//*/
//=============================================================================
class IAnimationTrack
    : public Base::ReferenceObject
{
protected:

    virtual ~IAnimationTrack() {}

public:

    /// �g���b�N�̎�ނ̎擾
    virtual AnimationTrackType getType() const = 0;

    /// �t���[���ʒu��ݒ肵�čX�V (�C�x���g�̏ꍇ�͂ЂƂO�̃t���[���ʒu����̃C�x���g����)
    virtual void update( lnTime frame_pos_ ) = 0;

    ///�����_���l�𔽉f����
    virtual void resetRand( int seed_ ) {}

    /// update() �ōX�V���ꂽ�l�̎擾 (�Ƃ肠���� ValueAnimationTrack �̂݁B����ȊO�� return 0)
    virtual lnfloat getValue() const = 0;

    /// Node �����l�ݒ�p�BValueAnimationTrack �̂ݗL�� (�L���X�g���ʓ|�Ȃ̂Łc)
    virtual void setDefaultValue( lnfloat value_ ) {}

    /// XMLElement �֕ۑ�
    virtual tinyxml2::XMLElement* serializeXMLElement( tinyxml2::XMLDocument* doc_ ) = 0;

    /// XMLElement ���畜��
    virtual void deserializeXMLElement( tinyxml2::XMLElement* elm_ ) = 0;

public:

    LRefTString     getElementName() const { return mElementName; }
    const LColor&   getElementColor() const { return mElementColor; }

protected:

    LRefTString     mElementName;
    LColor          mElementColor;
};

//=============================================================================
// �� ValueAnimationTrack
//-----------------------------------------------------------------------------
///**
//  @brief		�l�ЂƂ�
//*/
//=============================================================================
class ValueAnimationTrack
    : public Animation::Scalar
    , public IAnimationTrack
{
public:

    static const lnChar* XML_ELEMENT_NAME;
    static const lnChar* KEY_XML_ELEMENT_NAME;

public:

    ValueAnimationTrack();
    virtual ~ValueAnimationTrack();

public:

    /// ������
    void initialize( const TrackElement& element_ );

    /// �g���b�N�̎�ނ̎擾
    virtual AnimationTrackType getType() const { return ANIMTRACKTYPE_VALUE; }

    /// �t���[���ʒu��ݒ肵�čX�V
    virtual void update( lnTime frame_pos_ );

    /// �����_���l�𔽉f����
    virtual void resetRand( int seed_ );

    /// update() �ōX�V���ꂽ�l�̎擾
    virtual lnfloat getValue() const;

    /// Node �����l�ݒ�p�BValueAnimationTrack �̂ݗL�� (�L���X�g���ʓ|�Ȃ̂Łc)
    virtual void setDefaultValue( lnfloat value_ ) { Animation::Scalar::setDefaultValue( value_); }

    /// XMLElement �֕ۑ�
    virtual tinyxml2::XMLElement* serializeXMLElement( tinyxml2::XMLDocument* doc_ );

    /// XMLElement ���畜��
    virtual void deserializeXMLElement( tinyxml2::XMLElement* elm_ );

public:

    /// �L�[�ǉ� (�\�[�X�f�[�^)
    void addEffectAnimationKey( const AnimationKey& key_ ) { mAnimationKeyArray.push_back( key_ ); }

    /// AnimationKey ���X�g�̃N���A
    void clearEffectAnimationKeyArray() { mAnimationKeyArray.clear(); }

    AnimationKey* getEffectAnimationKey( int idx_ ) { return &mAnimationKeyArray[ idx_ ]; }

    int getEffectAnimationKeyNum() const { return mAnimationKeyArray.size(); }

    /// Animation::Scalar �̃L�[�������_���l�Ȃ��ōĐݒ�
    //      �G�f�B�^�ł͊�{�I�Ƀ^�C�����C���\�z���O�ɂ��̊֐��ōĐݒ���s���B
    //      �Q�[���ł��AaddEffectAnimationKey() �╜���ȂǂŃL�[���ω������ꍇ�͈�x���̊֐��ōč\�z����
    void refreshKeysNotRand();

private:

    AnimationKeyArray   mAnimationKeyArray;
};

//=============================================================================
// �� EventAnimationTrack
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class EventAnimationTrack
    : public IAnimationTrack
{
public:

    static const lnChar* XML_ELEMENT_NAME;

public:

    struct Key
    {
        LRefTString     SoundFilename;
        int             Volume;
        int             Pitch;
    };

public:

    EventAnimationTrack();
    virtual ~EventAnimationTrack();

public:

    /// ������
    void initialize( const TrackElement& element_ );

    /// �g���b�N�̎�ނ̎擾
    virtual AnimationTrackType getType() const { return ANIMTRACKTYPE_EVENT; }

    /// �t���[���ʒu��ݒ肵�čX�V
    virtual void update( lnTime frame_pos_ );

    /// update() �ōX�V���ꂽ�l�̎擾
    virtual lnfloat getValue() const { return 0; }

    /// XMLElement �֕ۑ�
    virtual tinyxml2::XMLElement* serializeXMLElement( tinyxml2::XMLDocument* doc_ );

    /// XMLElement ���畜��
    virtual void deserializeXMLElement( tinyxml2::XMLElement* elm_ );

public:

    /// �L�[�̒ǉ� (�\�[�g�͍s��Ȃ��BManaged ���Ń\�[�g��clear()���ЂƂ���addKeyFrame())
    void addKeyFrame( const Key& key_ );

    /// �L�[��S�č폜����
    void clear();
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