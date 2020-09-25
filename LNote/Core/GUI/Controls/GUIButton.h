//=============================================================================
//Åy GUIButton Åz
//-----------------------------------------------------------------------------
///**
//  @file       GUIButton.h
//  @brief      GUIButton
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Common.h"
#include "GUIControl.h"

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
// Å° GUIButtonDataContext
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class GUIButtonDataContext
    : public BasicNotifyPropertyChanged
{
public:

    void        setText( const lnChar* text_ ) { mText = text_; onPropertyChanged( _T("Text") ); }
    LRefTString getText() const { return mText; }

    void        setImageName( const lnChar* name_ ) { mImageName = name_; onPropertyChanged( _T("ImageName") ); }
    LRefTString getImageName() const { return mText; }

public:

    virtual const lnChar* getPropertyValue( const LRefTString& property_name_ )
    {
        if ( property_name_ == _T("Text") )      return mText.c_str();
        if ( property_name_ == _T("ImageName") ) return mImageName.c_str();
        return NULL;
    }

    virtual void onPropertyApplied( const LRefTString& property_name_, const BindingSourceValue& value_ )
    {
        if ( property_name_ == _T("Text") )      mText = value_.getString();
        if ( property_name_ == _T("ImageName") ) mText = value_.getString();
    }

private:

    LRefTString mText;
    LRefTString mImageName;
};

//=============================================================================
// Å° GUIButton
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class GUIButton
    : public GUIControl
{
public:

    struct FormatConfig
    {
        Graphics::ITexture*         SkinTexture;        ///< (NULL ÇÃèÍçáÇÕÉfÉtÉHÉãÉgÇÃÉXÉLÉìÇégóp)
        FrameFormatConfig           FrameFormat;        ///< ògÇÃê∂ê¨èÓïÒ
        FrameBackgroundFormatConfig BackgroundFormat;   ///< îwåiÇÃê∂ê¨èÓïÒ

        FormatConfig()
        {
            SkinTexture = NULL;
            FrameFormat.ImageSrcRect.set( 0, 0, 32, 32 );
            FrameFormat.Format = FRAMEPLACE_IMAGE_SCALING;
            BackgroundFormat.ImageSrcRect.set( 0, 32, 32, 32 );
            BackgroundFormat.Format = FRAMEBACKGROUNDFORMAT_SCALING;
        };
    };

    static const int DEFAULT_WIDTH = 64;
    static const int DEFAULT_HEIGHT = 24;

public:

    GUIButton( Manager* manager_ );
    virtual ~GUIButton();

public:

    /// èâä˙âª
    void initialize();

    /// èâä˙âª
    //void initialize( const FormatConfig& config_ );

    void        setText( const lnChar* text_ ) { mDefaultDataContext->setText( text_ ); }
    //LRefTString getText() const { return mText; }

    //void        setImageName( const lnChar* name_ ) { mImageName = name_; onPropertyChanged( _T("ImageName") ); }
    //LRefTString getImageName() const { return mText; }

public:

    virtual void initialize( GUIElement::Template* data_ );

    //virtual void updateDrawContents();

public:

    GUIButtonDataContext*   mDefaultDataContext;
    FormatConfig            mFormatConfig;

public:

    struct Template
        : public GUIControl::Template
    {
        Template() { this->Type = ELEMENTTYPE_BUTTON; }
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