//=============================================================================
//【 EffectAnimationTrackGroup 】
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
// ■ EffectAnimationTrackGroup
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

    /// 初期化 (element_num_ トラック数)
    void initialize( const TrackGroupElement& elm_, int element_num_, const TrackElement* elements_ );

    int getTrackNum() const { return mAnimationTrackArray.size(); }

    IAnimationTrack* getTrack( int idx_ ) { return mAnimationTrackArray[idx_]; }

    //LRefTString getTrackName( int idx_ ) { return mTrackNameArray[idx_]; }

    //const LColor& getTrackColor( int idx_ ) { return mColors[idx_]; }

    /// すべてのトラックに時間を設定する
    void update( lnTime time_ );

    /// すべてのトラックのランダム値を反映する
    void resetRand( int seed_ );

    /// トラックの値を取得する
    lnfloat getValue( int idx_ ) { return mAnimationTrackArray[idx_]->getValue(); }

    /// XML に保存 (作成された要素を返す)
    tinyxml2::XMLElement* serializeXMLElement( tinyxml2::XMLDocument* doc_ );

    /// XML 要素から作成 (initialize() とは一緒に呼ばないこと)
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