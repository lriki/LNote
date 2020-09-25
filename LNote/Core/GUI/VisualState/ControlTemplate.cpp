//=============================================================================
//y ControlTemplate z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Controls/GUIButton.h"
#include "../Shapes/GUISprite.h"
#include "../Shapes/GUITextBlock.h"
#include "ControlTemplate.h"

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
// ¡ ControlTemplate
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    ControlTemplate::ControlTemplate()
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ControlTemplate::initialize()
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GUIElement* ControlTemplate::createElement( Manager* manager_, GUIElement::Template* template_data_ )
    {
        GUIElement* element = NULL;

        switch ( template_data_->Type )
        {
            case ELEMENTTYPE_BUTTON:
            {
                element = LN_NEW GUIButton( manager_ );
                break;
            }

            case ELEMENTTYPE_SPRITE:
            {
                element = LN_NEW GUISprite( manager_ );
                break;
            }
            case ELEMENTTYPE_TEXTBLOCK:
            {
                element = LN_NEW GUITextBlock( manager_ );
                break;
            }
        }

        if ( !element ) printf("unregistered Element\n");

        return element;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    ControlTemplate* ControlTemplate::createDefaultButtonTemplate()
    {
        GUIButton::Template* root = LN_NEW GUIButton::Template();

        // ”wŒi
        GUISprite::Template* sprite_tmpl = LN_NEW GUISprite::Template();
        sprite_tmpl->SrcRect.set( 0, 0, 32, 32 );
        root->InternalChildren.push_back( sprite_tmpl );

        // ƒeƒLƒXƒg
        GUITextBlock::Template* text_tmpl = LN_NEW GUITextBlock::Template();
        text_tmpl->Text = _T("{Binding Text}"); // GUIButton(DataContext)::Text;
        root->InternalChildren.push_back( text_tmpl );

        GUITextBlock::Template* text2_tmpl = LN_NEW GUITextBlock::Template();
        text2_tmpl->ElementRect.set( 32, 32, 32, 32 );
        text2_tmpl->Text = _T("lnote");
        root->InternalChildren.push_back( text2_tmpl );

        ControlTemplate* t = LN_NEW ControlTemplate();
        t->mTemplateData = root;
        return t;
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace GUI
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================