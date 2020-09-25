//=============================================================================
//y GUISprite z
//-----------------------------------------------------------------------------
///**
//  @file       GUISprite.h
//  @brief      GUISprite
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
// ¡ GUISprite
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class GUISprite
    : public GUIShape
{
public:

    GUISprite( Manager* manager_ );
    virtual ~GUISprite();

public:

    //void setDestRect( const LRect& rect_ ) { mDestRect = rect_; }

    void setSrcRect( const LRect& rect_ ) { mSrcRect = rect_; }

    void setPixelOffset( int x_, int y_ ) { mPixelOffsetX = x_; mPixelOffsetY = y_; }

    //void setTexture( Graphics::ITexture* texture_ ) { LN_REFOBJ_SET( mTexture, texture_ ); }

    /// (NULL ‚Ìê‡‚Í GeometryRenderer ‚Å•`‰æ)
    void setSpriteRenderer( Graphics::SpriteRenderer* sprite_renderer_ ) { mSpriteRenderer = sprite_renderer_; }

public:

    virtual void initialize( GUIElement::Template* data_ );

    virtual void updateDrawContents();

    virtual void draw();

private:

    LRect                       mDestRect;
    LRect                       mSrcRect;
    int                         mPixelOffsetX;
    int                         mPixelOffsetY;
    //Graphics::ITexture*         mTexture;
    Graphics::SpriteRenderer*   mSpriteRenderer;

public:

    struct Template
        : public GUIShape::Template
    {
        Template() { this->Type = ELEMENTTYPE_SPRITE; }

        LRect   SrcRect;
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