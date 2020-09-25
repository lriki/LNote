//=============================================================================
//【 EffectNodeCore 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Emitter/EmitterBase.h"
#include "EffectCore.h"
#include "EffectNodeCore.h"

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
// ■ EffectNodeCore クラス
//=============================================================================

    struct TrackElementTable
    {
        TrackElement X;
        TrackElement Y;
        TrackElement Z;
        TrackElement W;
        TrackElement Rot;
        TrackElement R;
        TrackElement G;
        TrackElement B;
        TrackElement A;
        TrackElement U;
        TrackElement V;
    };

    TrackElementTable gTrackElementTable = 
    {
        { _T( "X" ), LColor( 1, 0, 0 )          , ANIMTRACKTYPE_VALUE },
        { _T( "Y" ), LColor( 0, 1, 0 )          , ANIMTRACKTYPE_VALUE },
        { _T( "Z" ), LColor( 0, 0, 1 )          , ANIMTRACKTYPE_VALUE },
        { _T( "W" ), LColor( 0.5, 0.5, 0.5 )    , ANIMTRACKTYPE_VALUE },
        { _T( "R" ), LColor( 0.5, 0.5, 0.5 )    , ANIMTRACKTYPE_VALUE },
        { _T( "R" ), LColor( 1, 0, 0 )          , ANIMTRACKTYPE_VALUE },
        { _T( "G" ), LColor( 0, 1, 0 )          , ANIMTRACKTYPE_VALUE },
        { _T( "B" ), LColor( 0, 0, 1 )          , ANIMTRACKTYPE_VALUE },
        { _T( "A" ), LColor( 0.5, 0.5, 0.5 )    , ANIMTRACKTYPE_VALUE },
        { _T( "U" ), LColor( 1, 0, 0 )          , ANIMTRACKTYPE_VALUE },
        { _T( "V" ), LColor( 0, 1, 0 )          , ANIMTRACKTYPE_VALUE },
    };

    struct TrackGroupElementTable
    {
        TrackGroupElement Position;
        TrackGroupElement Rotation;
        TrackGroupElement Scaling;
        TrackGroupElement Color;
        TrackGroupElement TexUV;
        //TrackGroupElement Event;
    } gTrackGroupElementTable = 
    {
        { _T( "Position" ),     LColor( 0.75, 1.0, 0.75 ) },
        { _T( "Rotation" ),     LColor( 1.0, 0.75, 0.75 ) },
        { _T( "Scaling" ),      LColor( 0.75, 0.75, 1.0 ) },
        { _T( "Color" ),        LColor( 0.75, 1.0, 1.0 ) },
        { _T( "TextureUV" ),    LColor( 1.0, 1.0, 0.75 ) },
    };

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectNodeCore::EffectNodeCore()
        : mEmitter      ( NULL )
        , mModified     ( false )
    {
        mVisible = true;
        mName = _T( "Node" );
        mMaxGenerateCount = 10;
        mGenerateStartFramePos = 0;
        mGenerateInterval = 10;

        mRenderState.BlendMode  = LN_BLEND_ADD;
        mRenderState.Culling    = LN_CULL_DOUBLE;
        mRenderState.FillMode   = LN_FILL_SOLID;
        mRenderState.AlphaTest  = false;
        mRenderState.DepthTest  = false;
        mRenderState.DepthWrite = true;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectNodeCore::~EffectNodeCore()
    {
        LN_SAFE_RELEASE( mEmitter );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeCore::initialize()
    {
        TrackElement pos_elements[]  = { gTrackElementTable.X,  gTrackElementTable.Y,  gTrackElementTable.Z };
        mCommonTrackGroupArray[COMMONTRACKGROUP_POSITION].initialize( gTrackGroupElementTable.Position, 3, pos_elements );
        
        TrackElement rot_elements[]  = { gTrackElementTable.X,  gTrackElementTable.Y,  gTrackElementTable.Z,  gTrackElementTable.Rot };
        mCommonTrackGroupArray[COMMONTRACKGROUP_ROTATION].initialize( gTrackGroupElementTable.Rotation, 4, rot_elements );

        TrackElement scale_elements[]  = { gTrackElementTable.X,  gTrackElementTable.Y,  gTrackElementTable.Z };
        mCommonTrackGroupArray[COMMONTRACKGROUP_SCALING].initialize( gTrackGroupElementTable.Scaling, 3, scale_elements );
        mCommonTrackGroupArray[COMMONTRACKGROUP_SCALING].getTrack( 0 )->setDefaultValue( 1.0f );
        mCommonTrackGroupArray[COMMONTRACKGROUP_SCALING].getTrack( 1 )->setDefaultValue( 1.0f );
        mCommonTrackGroupArray[COMMONTRACKGROUP_SCALING].getTrack( 2 )->setDefaultValue( 1.0f );

        TrackElement color_elements[]  = { gTrackElementTable.R,  gTrackElementTable.G,  gTrackElementTable.B,  gTrackElementTable.A };
        mCommonTrackGroupArray[COMMONTRACKGROUP_COLOR].initialize( gTrackGroupElementTable.Color, 4, color_elements );
        mCommonTrackGroupArray[COMMONTRACKGROUP_COLOR].getTrack( 0 )->setDefaultValue( 1.0f );
        mCommonTrackGroupArray[COMMONTRACKGROUP_COLOR].getTrack( 1 )->setDefaultValue( 1.0f );
        mCommonTrackGroupArray[COMMONTRACKGROUP_COLOR].getTrack( 2 )->setDefaultValue( 1.0f );
        mCommonTrackGroupArray[COMMONTRACKGROUP_COLOR].getTrack( 3 )->setDefaultValue( 1.0f );

        TrackElement uv_elements[]  = { gTrackElementTable.U,  gTrackElementTable.V };
        mCommonTrackGroupArray[COMMONTRACKGROUP_TEXUV].initialize( gTrackGroupElementTable.TexUV, 2, uv_elements );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    int EffectNodeCore::getAnimationTrackGroupNum()
    {
        int num = LN_EFFECT_COMMONTRACK_GROUPE_NUM;
        if ( mEmitter )
        {
            num += mEmitter->getExTrackGroupNum();
        }
        return num;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectAnimationTrackGroup* EffectNodeCore::getAnimationTrackGroup( int idx_ )
    {
        if ( idx_ < LN_EFFECT_COMMONTRACK_GROUPE_NUM )
        {
            return &mCommonTrackGroupArray[idx_];
        }
        if ( mEmitter && idx_ < mEmitter->getExTrackGroupNum() )
        {
            return mEmitter->getExTrackGroup( idx_ - LN_EFFECT_COMMONTRACK_GROUPE_NUM );
        }
        return NULL;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeCore::setEmitterCore( EmitterCoreBase* emitter_ )
    {
        LN_SAFE_ADDREF( emitter_ );
        LN_SAFE_RELEASE( mEmitter );
        mEmitter = emitter_;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeCore::setModifiedFlag( bool flag_ )
    {
        mModified = flag_;

        if ( mEmitter )
        {
            mEmitter->setModifiedFlag( flag_ );
        }

        NodeList& children = this->getChildren();
        ln_foreach( EffectNodeCore* n, children )
        {
            n->setModifiedFlag( flag_ );
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeCore::addChild( NodeType* child_ )
    {
        Base::HierarchyNode< EffectNodeCore >::addChild( child_ );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeCore::removeChild( NodeType* child_ )
    {
        Base::HierarchyNode< EffectNodeCore >::removeChild( child_ );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    tinyxml2::XMLElement* EffectNodeCore::saveToXML( tinyxml2::XMLDocument* doc_ )
    {
        tinyxml2::XMLElement* elm = doc_->NewElement("EffectNode");
        elm->SetAttribute("Name", mName.c_str());
        elm->SetAttribute("MaxGenerateCount", mMaxGenerateCount);
        elm->SetAttribute("GenerateStartFramePos", mGenerateStartFramePos);
        elm->SetAttribute("GenerateInterval", mGenerateInterval);

        // トラックグループ
        for ( int i = 0; i < LN_EFFECT_COMMONTRACK_GROUPE_NUM; ++i )
        {
            elm->InsertEndChild( mCommonTrackGroupArray[i].serializeXMLElement( doc_ ) );
        }
        
        // 子ノード
        ln_foreach( EffectNodeCore* n, this->mChildren )
        {
            elm->InsertEndChild(n->saveToXML(doc_));
        }

        return elm;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeCore::loadFromXML( tinyxml2::XMLElement* elm_ )
    {
        initialize();

        mName                   = elm_->Attribute( "Name" );
        mMaxGenerateCount       = elm_->IntAttribute("MaxGenerateCount" );
        mGenerateStartFramePos  = elm_->IntAttribute("GenerateStartFramePos" );
        mGenerateInterval       = elm_->IntAttribute("GenerateInterval" );

        int track_idx = 0;
        tinyxml2::XMLElement* elm = elm_->FirstChildElement();
        while ( elm )
        {
            // 子ノード
            if ( _tcscmp( elm->Name(), _T( "EffectNode" ) ) == 0 )
            {
                EffectNodeCore* node = LN_NEW EffectNodeCore();
                node->loadFromXML( elm );
                this->addChild( node );
            }
            // トラックグループ
            else if ( _tcscmp( elm->Name(), EffectAnimationTrackGroup::XML_ELEMENT_NAME ) == 0 )
            {
                mCommonTrackGroupArray[track_idx].deserializeXMLElement( elm );
                ++track_idx;
            }
            elm = elm->NextSiblingElement();
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    LRefTString EffectNodeCore::serializeXML()
    {
        tinyxml2::XMLDocument doc;
        doc.InsertEndChild( saveToXML( &doc ) );

        FILE* fp = fopen( "_lt", "w+b" );
        if ( fp )
        {
            doc.SaveFile( fp );

            size_t size = File::getFileSize( fp );
            char* buf = LN_NEW char[ size + 1 ];

            fread( buf, 1, size, fp );
            buf[ size ] = '\0';

            LRefTString str( buf );

            SAFE_DELETE_ARRAY( buf );
            fclose( fp );
            remove( "_lt" );

            return str;
        }

        return LRefTString();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeCore::deserializeXML( LRefTString str )
    {
        tinyxml2::XMLDocument doc;
        doc.Parse( str.c_str() );
        loadFromXML( doc.FirstChildElement() );
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