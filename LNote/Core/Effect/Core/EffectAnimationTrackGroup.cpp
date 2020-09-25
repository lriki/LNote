//=============================================================================
//【 EffectAnimationTrackGroup 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"

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
// ■ EffectAnimationTrackGroup
//=============================================================================

    const lnChar* EffectAnimationTrackGroup::XML_ELEMENT_NAME = _T( "AnimationTrackGrupe" );

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectAnimationTrackGroup::EffectAnimationTrackGroup()
    {
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectAnimationTrackGroup::~EffectAnimationTrackGroup()
    {
        _dispose();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectAnimationTrackGroup::initialize( const TrackGroupElement& elm_, int element_num_, const TrackElement* elements_ )
    {
        mElementName = elm_.Name;
        mElementColor = elm_.Color;

        for ( int i = 0; i < element_num_; ++i )
        {
            // float トラック
            if ( elements_[i].Type == ANIMTRACKTYPE_VALUE )
            {
                ValueAnimationTrack* track = LN_NEW ValueAnimationTrack();
                track->initialize( elements_[i] );
                track->setTimeTickPerSec( 1.0f );
                mAnimationTrackArray.push_back( track );
            }
            // イベントトラック
            else
            {
                EventAnimationTrack* track = LN_NEW EventAnimationTrack();
                track->initialize( elements_[i] );
                mAnimationTrackArray.push_back( track );
            }
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectAnimationTrackGroup::update( lnTime time_ )
    {
        ln_foreach( IAnimationTrack* track, mAnimationTrackArray )
        {
            track->update( time_ );
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectAnimationTrackGroup::resetRand( int seed_ )
    {
        ln_foreach( IAnimationTrack* track, mAnimationTrackArray )
        {
            track->resetRand( seed_ );
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    tinyxml2::XMLElement* EffectAnimationTrackGroup::serializeXMLElement( tinyxml2::XMLDocument* doc_ )
    {
        tinyxml2::XMLElement* elm = doc_->NewElement( XML_ELEMENT_NAME );

        // トラック
        ln_foreach( IAnimationTrack* t, mAnimationTrackArray )
        {
            elm->InsertEndChild( t->serializeXMLElement( doc_ ) );
        }
        return elm;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectAnimationTrackGroup::deserializeXMLElement( tinyxml2::XMLElement* elm_ )
    {
        
        int i = 0;
        tinyxml2::XMLElement* elm = elm_->FirstChildElement();
        while ( elm )
        {
            // ValueAnimationTrack
            if ( _tcscmp( elm->Name(), ValueAnimationTrack::XML_ELEMENT_NAME ) == 0 )
            {
                ValueAnimationTrack* t = (ValueAnimationTrack*)mAnimationTrackArray[i];
                t->deserializeXMLElement( elm );
                ++i;

                //ValueAnimationTrack* t = LN_NEW ValueAnimationTrack();
                //t->deserializeXMLElement( elm );
                //mAnimationTrackArray.push_back( t );
            }
            // EventAnimationTrack
            else if ( _tcscmp( elm->Name(), EventAnimationTrack::XML_ELEMENT_NAME ) == 0 )
            {
                EventAnimationTrack* t = (EventAnimationTrack*)mAnimationTrackArray[i];
                t->deserializeXMLElement( elm );
                ++i;

                //EventAnimationTrack* t = LN_NEW EventAnimationTrack();
                //t->deserializeXMLElement( elm );
                //mAnimationTrackArray.push_back( t );
            }

            elm = elm->NextSiblingElement();
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectAnimationTrackGroup::_dispose()
    {
        ln_foreach( IAnimationTrack* at, mAnimationTrackArray )
        {
            LN_SAFE_RELEASE( at );
        }
        mAnimationTrackArray.clear();
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================