//=============================================================================
//【 GUIElement 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Manager.h"
#include "GUIElement.h"
#include "Controls/GUIControl.h"

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
// ■ GUIElement
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GUIElement::GUIElement( Manager* manager_ )
        : mManager              ( manager_ )
        , mParent               ( NULL )
        , mDataContext          ( NULL )
        , mSkinTexture          ( NULL ) 
        , mVisible              ( true )
        , mEnable               ( true )
        , mTempActive           ( false )
        , mIsInternalElement    ( false )
    {
        LN_SAFE_ADDREF( mManager );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GUIElement::~GUIElement()
    {
        // GUI システム全体で自分がフォーカスを持っている場合は外す
	    if ( hasFocus() )
	    {
		    mManager->setFocusControl( NULL );
	    }

        // 親がいる場合は親のリストから削除
	    if ( mParent )
	    {
		    if ( mIsInternalElement ) 
                mParent->removeInternalElement( this );
            else
                mParent->removeControl( this );
	    }

        if ( mDataContext ) mDataContext->PropertyChanged -= this;

        LN_SAFE_RELEASE( mSkinTexture );
        LN_SAFE_RELEASE( mManager );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIElement::initialize()
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIElement::setRect( const LRect& rect_ )
    {
        mElementRect = rect_;
        mBoundingRect = rect_;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    bool GUIElement::isActive() const
    {
        return ( mParent && this == mParent->getActiveElement() ) || ( hasFocus() );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    bool GUIElement::hasFocus() const
    {
        return ( this == mManager->getFocusElement() );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIElement::focus()
    {
        mManager->setFocusControl( this );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIElement::setDataContext( INotifyPropertyChanged* object_ )
    {
        if ( mDataContext ) mDataContext->PropertyChanged -= this;
        mDataContext = object_;
        if ( mDataContext ) mDataContext->PropertyChanged += this;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIElement::initialize( Template* data_ )
    {
        mElementRect    = data_->ElementRect;
        mMargin         = data_->Margin;
        LN_REFOBJ_SET( mSkinTexture, data_->SkinTexture );

        if ( !mSkinTexture )
        {
            mSkinTexture = this->mManager->getDefaultSkinTexture();
            LN_SAFE_ADDREF( mSkinTexture );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIElement::updateCoordinate( const LRect& parent_client_rect_ )
    {
        mGlobalPosition.x = parent_client_rect_.x + mElementRect.x;
        mGlobalPosition.y = parent_client_rect_.y + mElementRect.y;

        updateCollider();
        
        mBoundingRect.clip( parent_client_rect_ );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIElement::updateCollider()
    {
        mBoundingRect.x = mGlobalPosition.x;
        mBoundingRect.y = mGlobalPosition.y;
        mBoundingRect.w = mElementRect.w;
        mBoundingRect.h = mElementRect.h;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    bool GUIElement::checkMouseHover( const LPoint& mouse_pos )
    {
        if (!mVisible) return false;
        return mBoundingRect.contains( mouse_pos );
    }

    ///---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GUIElement* GUIElement::getMouseHoverElement( const LPoint& mouse_pos )
    {
        return ( checkMouseHover( mouse_pos ) ) ? this : NULL;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIElement::beginCheckActivate()
    {
        mTempActive = isActive();

        // 親があれば、階層をさかのぼりながらコンテナ内のアクティブ状態を外していく
        if ( mParent )
        {
            if ( this == mParent->getActiveElement() )
            {
                mParent->setActiveElement( NULL );
            }

            mParent->beginCheckActivate();
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIElement::endCheckActivate( bool active_ )
    {
        // アクティブになる場合、階層をさかのぼってアクティブにしていく
        if ( active_ && mParent )//hasFocus() )
        {
            mParent->setActiveElement( this );
            mParent->endCheckActivate( active_ );
        }

        // begin で記憶した状態と異なる場合はイベント発生
        bool ac = isActive();
        if ( mTempActive != ac )
        {
            if ( ac )
            {
                doEvent( Event( LN_GUIEV_GOT_FOCUS ) );
            }
            else
            {
                doEvent( Event( LN_GUIEV_LOST_FOCUS ) );
            }
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