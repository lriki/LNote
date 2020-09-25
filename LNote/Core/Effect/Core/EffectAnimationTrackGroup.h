//=============================================================================
//�y EffectAnimationTrackGroup �z
//-----------------------------------------------------------------------------
///**
//  @file       EffectAnimationTrackGroup.h
//  @brief      EffectAnimationTrackGroup
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Types.h"
#include "EffectAnimationTrack.h"

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
// �� EffectAnimationTrackGroup
//-----------------------------------------------------------------------------
///**
//  @brief		Vector
//*/
//=============================================================================
class EffectAnimationTrackGroup
    : public Base::ReferenceObject
{
public:

    static const lnChar* XML_ELEMENT_NAME;

public:

    EffectAnimationTrackGroup();

    virtual ~EffectAnimationTrackGroup();

public:

    /// ������ (element_num_ �g���b�N��)
    void initialize( const TrackGroupElement& elm_, int element_num_, const TrackElement* elements_ );

    int getTrackNum() const { return mAnimationTrackArray.size(); }

    IAnimationTrack* getTrack( int idx_ ) { return mAnimationTrackArray[idx_]; }

    //LRefTString getTrackName( int idx_ ) { return mTrackNameArray[idx_]; }

    //const LColor& getTrackColor( int idx_ ) { return mColors[idx_]; }

    /// ���ׂẴg���b�N�Ɏ��Ԃ�ݒ肷��
    void update( lnTime time_ );

    /// ���ׂẴg���b�N�̃����_���l�𔽉f����
    void resetRand( int seed_ );

    /// �g���b�N�̒l���擾����
    lnfloat getValue( int idx_ ) { return mAnimationTrackArray[idx_]->getValue(); }

    /// XML �ɕۑ� (�쐬���ꂽ�v�f��Ԃ�)
    tinyxml2::XMLElement* serializeXMLElement( tinyxml2::XMLDocument* doc_ );

    /// XML �v�f����쐬 (initialize() �Ƃ͈ꏏ�ɌĂ΂Ȃ�����)
    void deserializeXMLElement( tinyxml2::XMLElement* elm_ );


    LRefTString     getElementName() const { return mElementName; }
    const LColor&   getElementColor() const { return mElementColor; }

private:

    void _dispose();

private:

    AnimationTrackArray mAnimationTrackArray;
    //StringArray         mTrackNameArray;
    //LColor*             mColors;

    LRefTString     mElementName;
    LColor          mElementColor;
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