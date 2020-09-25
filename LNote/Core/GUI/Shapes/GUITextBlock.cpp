//=============================================================================
//【 GUITextBlock 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Manager.h"
#include "GUITextBlock.h"

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
// ■ GUITextBlock
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GUITextBlock::GUITextBlock( Manager* manager_ )
        : GUIShape      ( manager_ )
        , mTextBinding  ( this, BINDINGVALUETYPE_STRING )
        , mTexture      ( NULL )
        , mFont         ( NULL )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GUITextBlock::~GUITextBlock()
    {
        LN_SAFE_RELEASE( mTexture );
        LN_SAFE_RELEASE( mFont );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUITextBlock::initialize()
    {
        mFont = this->mManager->getGraphicsManager()->getDefaultFont();
        LN_SAFE_ADDREF( mFont );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUITextBlock::setText( const lnChar* text_ )
    {
        const lnChar* text = text_;

        LRect rect;
        mFont->getTextSize( text, -1, &rect );
        if ( rect.w <= 0 || rect.h <= 0 ) return;

        bool is_remake = false;
        if ( !mTexture )
        {
            is_remake = true;
        }
        else
        {
            // テクスチャサイズが文字列よりも小さいかチェック
            const LVector2& size = mTexture->getSize();
            if ( size.x < rect.w || size.y < rect.w )
            {
                is_remake = true;
            }
        }

        if ( is_remake )
        {
            LN_SAFE_RELEASE( mTexture );

            this->mManager->getGraphicsManager()->getGraphicsDevice()->createTexture(
                &mTexture, rect.w, rect.h, 1, LN_FMT_A8R8G8B8 );
        }

        mTexture->drawText( text, LRect(0, 0, 0, 0) );

        this->mElementRect.w = rect.w;
        this->mElementRect.h = rect.h;

        mTextBinding.sendString( this->mDataContext, text_ );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUITextBlock::initialize( GUIElement::Template* data_ )
    {
        Template* data = (Template*)data_;
        GUIShape::initialize( data );
        mTextBinding.setFormat( data->Text.c_str() );

        mFont = this->mManager->getGraphicsManager()->getDefaultFont();
        LN_SAFE_ADDREF( mFont );

        setText( mTextBinding.getString( this->mDataContext ) );//data->Text.c_str() );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    bool GUITextBlock::doEvent( const Event& e_ )
    {
        switch ( e_.Type )
        {
            case LN_GUIEV_MOUSE_LEAVE:
                setText(_T("Leave"));
                break;
            case LN_GUIEV_MOUSE_ENTER:
                setText(_T("Enter"));
                break;
        }

        return false;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUITextBlock::updateDrawContents()
    {
        GUIShape::updateDrawContents();
        //printf("GUITextBlock::updateDrawContents()\n");
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUITextBlock::draw()
    {
        if ( !mTexture ) return;

        const LVector2& size = mTexture->getSize();
        LRectF dest_rect(
            this->mGlobalPosition.x,
            this->mGlobalPosition.y,
            size.x, size.y );
        LRectF src_rect(
            0, 0, 1, 1 );

        Graphics::GeometryRenderer* r = this->mManager->getGeometryRenderer();
        r->setMatrix( LMatrix::IDENTITY );
        r->setTexture( mTexture );
        r->begin();
        r->drawRect( dest_rect, src_rect, 0xffffffff );
        r->end();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUITextBlock::onContextPropertyChanged( const lnChar* property_name_ )
    {
        // この関数内では OneWayToSource、TwoWay を禁止しなければならない

        if ( mTextBinding.checkBindingSource( property_name_ ) )
        {
            setText( mTextBinding.getString( this->mDataContext ) );
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