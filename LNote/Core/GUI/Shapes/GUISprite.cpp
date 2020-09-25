//=============================================================================
//y GUISprite z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Manager.h"
#include "GUISprite.h"

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
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GUISprite::GUISprite( Manager* manager_ )
        : GUIShape          ( manager_ )
        , mPixelOffsetX     ( 0 )
        , mPixelOffsetY     ( 0 )
        //, mTexture          ( NULL )
        , mSpriteRenderer   ( NULL )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GUISprite::~GUISprite()
    {
        //LN_SAFE_RELEASE( mTexture );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUISprite::initialize( GUIElement::Template* data_ )
    {
         Template* data = (Template*)data_;

         GUIShape::initialize( data );

         mSrcRect = data->SrcRect;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUISprite::updateDrawContents()
    {
        GUIShape::updateDrawContents();

        mDestRect.set(
            this->mGlobalPosition.x,
            this->mGlobalPosition.y, 
            this->mElementRect.w,
            this->mElementRect.h );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUISprite::draw()
    {
        if ( !this->mSkinTexture || mDestRect.getSize().isAnyZero() ) return;

        if ( mSpriteRenderer )
        {
        }
        else
        {
            LRectF dest_rect( mDestRect );
            dest_rect.offset( mPixelOffsetX, mPixelOffsetY );

            const LVector2& rs = this->mSkinTexture->getRealSize( true );
            LRectF src_rect(
                (lnfloat)mSrcRect.x * rs.x,
                (lnfloat)mSrcRect.y * rs.y,
                (lnfloat)mSrcRect.w * rs.x,
                (lnfloat)mSrcRect.h * rs.y );

            Graphics::GeometryRenderer* gr = this->mManager->getGeometryRenderer();
            gr->setMatrix( LMatrix::IDENTITY );
            gr->setTexture( this->mSkinTexture );
            gr->begin();
            gr->drawRect( dest_rect, src_rect, 0xff000000 );
            gr->end();
        }
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