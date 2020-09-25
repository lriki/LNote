//=============================================================================
//【 EffectAnimationTrack 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
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
// ■ ValueAnimationTrack
//=============================================================================

    const lnChar* ValueAnimationTrack::XML_ELEMENT_NAME     = _T( "ValueAnimationTrack" );
    const lnChar* ValueAnimationTrack::KEY_XML_ELEMENT_NAME = _T( "ValueAnimationKey" );

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    ValueAnimationTrack::ValueAnimationTrack()
    {
        Animation::Scalar::setTimeTickPerSec( 1 );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    ValueAnimationTrack::~ValueAnimationTrack()
    {
        
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void ValueAnimationTrack::initialize( const TrackElement& element_ )
    {
        this->mElementName = element_.Name;
        this->mElementColor = element_.Color;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void ValueAnimationTrack::update( lnTime frame_pos_ )
    {
        Animation::Scalar::setTime( frame_pos_ );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void ValueAnimationTrack::resetRand( int seed_ )
    {
        LRandom random( seed_ );

        int key_count = this->getKeyNum();
        for ( int i = 0; i < key_count; ++i )
        {
            Key* k = this->getKey( i );
            AnimationKey& ek = mAnimationKeyArray[i];

            //k->FramePos    = ;
            k->Value       = random.getFloatRange( ek.Value, ek.ValueRand );
            k->Velocity    = random.getFloatRange( ek.Velocity, ek.VelocityRand );
            k->Accel       = random.getFloatRange( ek.Accel, ek.AccelRand );
            k->LeftSlope   = random.getFloatRange( ek.LeftSlope, ek.LeftSlopeRand );
            k->RightSlope  = random.getFloatRange( ek.RightSlope, ek.RightSlopeRand );
            //k->InterMode   = ;
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    lnfloat ValueAnimationTrack::getValue() const
    {
        return Animation::Scalar::getValue();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    tinyxml2::XMLElement* ValueAnimationTrack::serializeXMLElement( tinyxml2::XMLDocument* doc_ )
    {
        tinyxml2::XMLElement* elm = doc_->NewElement( XML_ELEMENT_NAME );

        // キーフレーム
        ln_foreach( AnimationKey& k, mAnimationKeyArray )
        {
            tinyxml2::XMLElement* key_elm = doc_->NewElement( KEY_XML_ELEMENT_NAME );
            key_elm->SetAttribute( "FramePos",      k.FramePos );
            key_elm->SetAttribute( "Value",         k.Value );
            key_elm->SetAttribute( "ValueRand",     k.ValueRand );
            key_elm->SetAttribute( "Velocity",      k.Velocity );
            key_elm->SetAttribute( "VelocityRand",  k.VelocityRand );
            key_elm->SetAttribute( "Accel",         k.Accel );
            key_elm->SetAttribute( "AccelRand",     k.AccelRand );
            key_elm->SetAttribute( "LeftSlope",     k.LeftSlope );
            key_elm->SetAttribute( "LeftSlopeRand", k.LeftSlopeRand );
            key_elm->SetAttribute( "RightSlope",    k.RightSlope );
            key_elm->SetAttribute( "RightSlopeRand",k.RightSlopeRand );
            key_elm->SetAttribute( "InterMode",     (int)k.InterMode );

            elm->InsertEndChild( key_elm );
        }
        /*
        int n = this->getKeyNum();
        for ( int i = 0; i < n; ++i )
        {
            Key* key = this->getKey( i );
           
            tinyxml2::XMLElement* key_elm = doc_->NewElement( KEY_XML_ELEMENT_NAME );
            key_elm->SetAttribute( "FramePos",  key->FramePos );
            key_elm->SetAttribute( "Value",     key->Value );
            key_elm->SetAttribute( "Velocity",  key->Velocity );
            key_elm->SetAttribute( "Accel",     key->Accel );
            key_elm->SetAttribute( "LeftSlope", key->LeftSlope );
            key_elm->SetAttribute( "RightSlope",key->RightSlope );
            key_elm->SetAttribute( "InterMode", (int)key->InterMode );

            elm->InsertEndChild( key_elm );
        }
        */

        return elm;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void ValueAnimationTrack::deserializeXMLElement( tinyxml2::XMLElement* elm_ )
    {
        clearEffectAnimationKeyArray();

        tinyxml2::XMLElement* elm = elm_->FirstChildElement();
        while ( elm )
        {
            // キーフレーム
            //if ( _tcscmp( elm->Name(), KEY_XML_ELEMENT_NAME ) == 0 )  // 高速化。1種類しかないので…
            {
                AnimationKey k;
                k.FramePos          = elm->DoubleAttribute( "FramePos" );
                k.Value             = elm->FloatAttribute( "Value" );
                k.ValueRand         = elm->FloatAttribute( "ValueRand" );
                k.Velocity          = elm->FloatAttribute( "Velocity" );
                k.VelocityRand      = elm->FloatAttribute( "VelocityRand" );
                k.Accel             = elm->FloatAttribute( "Accel" );
                k.AccelRand         = elm->FloatAttribute( "AccelRand" );
                k.LeftSlope         = elm->FloatAttribute( "LeftSlope" );
                k.LeftSlopeRand     = elm->FloatAttribute( "LeftSlopeRand" );
                k.RightSlope        = elm->FloatAttribute( "RightSlope" );
                k.RightSlopeRand    = elm->FloatAttribute( "RightSlopeRand" );
                k.InterMode         = (LNAnimInterMode)elm->IntAttribute( "InterMode" );

                addEffectAnimationKey( k );
            }
            
            elm = elm->NextSiblingElement();
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void ValueAnimationTrack::refreshKeysNotRand()
    {
        this->clear();

        ln_foreach( AnimationKey& k, mAnimationKeyArray )
        {
            Key key;
            key.FramePos    = k.FramePos;
            key.Value       = k.Value;
            key.Velocity    = k.Velocity;
            key.Accel       = k.Accel;
            key.LeftSlope   = k.LeftSlope;
            key.RightSlope  = k.RightSlope;
            key.InterMode   = k.InterMode;

            this->addKeyFrame( key );
        }
    }


//=============================================================================
// ■ EventAnimationTrack
//=============================================================================

    const lnChar* EventAnimationTrack::XML_ELEMENT_NAME = _T( "EventAnimationTrack" );

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EventAnimationTrack::EventAnimationTrack()
    {
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EventAnimationTrack::~EventAnimationTrack()
    {
        
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EventAnimationTrack::initialize( const TrackElement& element_ )
    {
        this->mElementName = element_.Name;
        this->mElementColor = element_.Color;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EventAnimationTrack::update( lnTime frame_pos_ )
    {
        
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    tinyxml2::XMLElement* EventAnimationTrack::serializeXMLElement( tinyxml2::XMLDocument* doc_ )
    {
        LN_PRINT_NOT_IMPL_FUNCTION
            return NULL;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EventAnimationTrack::deserializeXMLElement( tinyxml2::XMLElement* elm_ )
    {
        LN_PRINT_NOT_IMPL_FUNCTION
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EventAnimationTrack::addKeyFrame( const Key& key_ )
    {
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EventAnimationTrack::clear()
    {
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