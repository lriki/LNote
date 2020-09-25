//=============================================================================
//【 GUIButton 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Shapes/GUISprite.h"
#include "../Manager.h"
#include "../VisualState/ControlTemplate.h" // 仮
#include "GUIButton.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace GUI
{

//=============================================================================
// ■ GUIButton
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GUIButton::GUIButton( Manager* manager_ )
        : GUIControl            ( manager_ )
        , mDefaultDataContext   ( NULL )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GUIButton::~GUIButton()
    {
        SAFE_DELETE( mDefaultDataContext );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIButton::initialize()
    {
        initialize(
            ControlTemplate::createDefaultButtonTemplate()->getElementTemplate() );
        //initialize( FormatConfig() );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIButton::initialize( GUIElement::Template* data_ )
    {
        mDefaultDataContext = LN_NEW GUIButtonDataContext();
        mDefaultDataContext->setText(_T("TEst"));
        this->setDataContext( mDefaultDataContext );

        this->mElementRect.w = DEFAULT_WIDTH;
        this->mElementRect.h = DEFAULT_HEIGHT;

        Template* data = (Template*)data_;

        GUIControl::initialize( data );

        
        

        // Template クラスにシリアライズ的な関数を持たせてもいいかと思ったけど、
        // なんとなく Template は単なるデータクラスにしておきたい。
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    //void GUIButton::updateDrawContents()
    //{
    //    /*
    //    LRect dest(
    //        this->mGlobalPosition.x,
    //        this->mGlobalPosition.y, 
    //        this->mElementRect.w,
    //        this->mElementRect.h );
    //    mSprite->setDestRect( dest );
    //    */
    //}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace GUI
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================