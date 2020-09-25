//=============================================================================
//y GUITextBlock z
//-----------------------------------------------------------------------------
///**
//  @file       GUITextBlock.h
//  @brief      GUITextBlock
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Common.h"
#include "GUIShape.h"

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
// ¡ GUITextBlock
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class GUITextBlock
    : public GUIShape
{
public:

    GUITextBlock( Manager* manager_ );
    virtual ~GUITextBlock();

public:

    /// ‰Šú‰»
    void initialize();

    void            setText( const lnChar* text_ );
    const lnChar*   getText() const { return mText.c_str(); }//mText.getString( this->mDataContext ); }

public:

    virtual void initialize( GUIElement::Template* data_ );

    /// ƒCƒxƒ“ƒgˆ—
    virtual bool doEvent( const Event& e_ );

    /// •`‰æî•ñ‚Ìì¬
    virtual void updateDrawContents();

    /// •`‰æ
    virtual void draw();

    virtual void onContextPropertyChanged( const lnChar* property_name_ );

private:

    LRefTString         mText;
    BindableValue       mTextBinding;
    Graphics::ITexture* mTexture;
    Graphics::IFont*    mFont;

public:

    struct Template
        : public GUIShape::Template
    {
        LRefTString     Text;

        Template() { this->Type = ELEMENTTYPE_TEXTBLOCK; }
    };
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace GUI
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================