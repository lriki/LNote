//=============================================================================
//【 EffectCore 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <tinyxml2/tinyxml2.h>
#include "../Manager.h"
#include "EffectNodeCore.h"
#include "EffectCore.h"

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
// ■ EffectCore クラス
//=============================================================================

    
    

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectCore::EffectCore( Manager* manager_ )
        : mManager  ( manager_ )
        , mRootNode ( NULL )
        , mLocalFPS ( 60 )
        , mModified ( false )
    {
        LN_SAFE_ADDREF( mManager );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectCore::~EffectCore()
    {
        LN_SAFE_RELEASE( mRootNode );
        LN_SAFE_RELEASE( mManager );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectCore::initialize( const lnChar* filename_ )
    {
        if ( !filename_ )
        {
            mRootNode = LN_NEW EffectNodeCore();
            mRootNode->initialize();
        }
        else
        {
            // ファイルから XML 構築
            File::IInFile* file;
            mManager->getFileManager()->createInFile( &file, filename_ );
            file->load();

            tinyxml2::XMLDocument doc;
            doc.Parse( (const char*)file->getData() );

            LN_SAFE_RELEASE( file );

            // EffectCore
            tinyxml2::XMLElement* effect_elm = doc.FirstChildElement();
            mLocalFPS = (float)effect_elm->DoubleAttribute( "LocalFPS" );

            // 子ノード
            tinyxml2::XMLElement* elm = effect_elm->FirstChildElement();
            while ( elm )
            {
                if ( _tcscmp( elm->Name(), _T( "EffectNode" ) ) == 0 )
                {
                    mRootNode = LN_NEW EffectNodeCore();
                    mRootNode->loadFromXML( elm );
                }
                elm = elm->NextSiblingElement();
            }
        }
        
    }

    


    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    //int EffectCore::getUnusedNodeIndex()
    //{

    //}

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectCore::saveToXML( const lnChar* filename_ )
    {
        tinyxml2::XMLDocument doc;
        
        tinyxml2::XMLElement* elm = doc.NewElement("Effect");
        elm->SetAttribute("LocalFPS", (double)mLocalFPS);
        doc.InsertEndChild(elm);

        elm->InsertEndChild(mRootNode->saveToXML(&doc));

        doc.SaveFile(filename_);


  //      tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
		//tinyxml2::XMLNode* element = doc->InsertEndChild( doc->NewElement( "element" ) );

		//tinyxml2::XMLElement* sub[3] = { doc->NewElement( "sub" ), doc->NewElement( "sub" ), doc->NewElement( "sub" ) };
		//for( int i=0; i<3; ++i ) {
		//	sub[i]->SetAttribute( "attrib", i );
		//}
		//element->InsertEndChild( sub[2] );
		//tinyxml2::XMLNode* comment = element->InsertFirstChild( doc->NewComment( "comment" ) );
		//element->InsertAfterChild( comment, sub[0] );
		//element->InsertAfterChild( sub[0], sub[1] );
		//sub[2]->InsertFirstChild( doc->NewText( "& Text!" ));
		//doc->Print();
		//XMLTest( "Programmatic DOM", "comment", doc->FirstChildElement( "element" )->FirstChild()->Value() );
		//XMLTest( "Programmatic DOM", "0", doc->FirstChildElement( "element" )->FirstChildElement()->Attribute( "attrib" ) );
		//XMLTest( "Programmatic DOM", 2, doc->FirstChildElement()->LastChildElement( "sub" )->IntAttribute( "attrib" ) );
		//XMLTest( "Programmatic DOM", "& Text!",
		//		 doc->FirstChildElement()->LastChildElement( "sub" )->FirstChild()->ToText()->Value() );

		//// And now deletion:
		//element->DeleteChild( sub[2] );
		//doc->DeleteNode( comment );

		//element->FirstChildElement()->SetAttribute( "attrib", true );
		//element->LastChildElement()->DeleteAttribute( "attrib" );

		//XMLTest( "Programmatic DOM", true, doc->FirstChildElement()->FirstChildElement()->BoolAttribute( "attrib" ) );
		//int value = 10;
		//int result = doc->FirstChildElement()->LastChildElement()->QueryIntAttribute( "attrib", &value );
		//XMLTest( "Programmatic DOM", result, (int)XML_NO_ATTRIBUTE );
		//XMLTest( "Programmatic DOM", value, 10 );

		//doc->Print();

		//{
		//	XMLPrinter streamer;
		//	doc->Print( &streamer );
		//	printf( "%s", streamer.CStr() );
		//}
		//{
		//	XMLPrinter streamer( 0, true );
		//	doc->Print( &streamer );
		//	XMLTest( "Compact mode", "<element><sub attrib=\"1\"/><sub/></element>", streamer.CStr(), false );
		//}
		//doc->SaveFile( "ln_pretty.xml" );
		//doc->SaveFile( "ln_compact.xml", true );
		//delete doc;
    }

#if 0
//=============================================================================
// ■ EffectCore クラス
//=============================================================================

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectCore::EffectCore( Manager* manager_ )
        : mManager  ( manager_ )
        , mRootNode ( NULL )
        , mFramePos ( 0 )
    {
        LN_SAFE_ADDREF( mManager );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectCore::~EffectCore()
    {
        LN_SAFE_RELEASE( mRootNode );
        LN_SAFE_RELEASE( mManager );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectCore::initialize()
    {
        mRootNode = LN_NEW EffectNode( this );
        //addSubset( mRootNode );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectCore::setFramePos( lnTime frame_ )
    { 
        mFramePos = frame_; 
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectCore::refreshKeyFrames()
    {
        mRootNode->refreshKeyFrames();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectCore::update()
    {
        ln_foreach( EffectNode* n, mSubsets )
        {
            n->update();
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectCore::drawSubset( lnU32 index_ )
    {
        mSubsets[ index_ ]->draw();
    }
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