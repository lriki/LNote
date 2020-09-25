//=============================================================================
//【 GUIControl 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Base/STLUtil.h"
#include "../VisualState/ControlTemplate.h"
#include "../GUIElement.h"
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
// ■ GUIControl
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GUIControl::GUIControl( Manager* manager_ )
        : GUIElement            ( manager_ )
        , mActiveTabOrderIndex  ( -1 )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GUIControl::~GUIControl()
    {
        // 削除リストの作成
        //      Element のデストラクタで呼ばれる mParent->removeControl() は、
        //      内部で mChildList.remove() を呼んでいる。
        //      このため、普通に mChildList をイテレートして解放すると、イテレータのつながりが
        //      おかしくなってエラーとなってしまう。
        //      そのため、一度別の配列にコピーしてから削除している。
	    GUIElementArray delete_controls;
	    delete_controls.reserve( mChildList.size() );
	    GUIElementArray::iterator itr = mChildList.begin();
	    GUIElementArray::iterator end = mChildList.end();
	    for ( ; itr != end; ++itr )
	    {
		    delete_controls.push_back( *itr );
	    }

	    // 削除リストの中身をdelete
	    GUIElementArray::iterator itr_del = delete_controls.begin();
	    GUIElementArray::iterator end_del = delete_controls.end();
	    for ( ; itr_del != end_del; ++itr_del )
	    {
		    if ( *itr_del != this )
		    {
                removeControl( *itr_del );
		    }
	    }

        // 内部要素も同様に削除
        GUIElementArray delete_internals;
        delete_internals.reserve( mInternalChildren.size() );
        ln_foreach( GUIElement* e, mInternalChildren )
        {
            delete_internals.push_back( e );
        }
        ln_foreach( GUIElement* e, delete_internals )
        {
            if ( e != this ) removeInternalElement( e );
        }
    }


    
    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIControl::initialize()
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIControl::addControl( GUIElement* element_ )
    {
        // すでに何かの子になっている場合はエラー
	    LN_THROW_InvalidCall( !element_->mParent, "Already been added." );
        LN_THROW_InvalidCall( !element_->mIsInternalElement, "element is internal" );

	    // 子をリストに追加
	    mChildList.push_back( element_ );
        LN_SAFE_ADDREF( element_ );
        element_->mParent = this;

	    // フォーカスを持てる場合はタブオーダーリストに追加する
	    if ( element_->canFocus() )
	    {
		    mTabOrderArray.push_back( element_ );
            mActiveTabOrderIndex = mTabOrderArray.size() - 1;
	    }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIControl::removeControl( GUIElement* element_ )
    {
        // このコントロールの子ではない場合エラー
	    LN_THROW_InvalidCall( element_->mParent == this, "Element is not a child." );
        LN_THROW_InvalidCall( !element_->mIsInternalElement, "element is internal" );

        // 子のリストから外す
        Base::STLUtil::remove( mChildList, element_ );
        element_->mParent = NULL;

	    // 現在フォーカスを持っているコントロールを focus に代入
	    GUIElement* focus = ( mActiveTabOrderIndex == -1 ) ? NULL : mTabOrderArray[ mActiveTabOrderIndex ];

	    // タブオーダーリストを再構築
	    GUIElementArray new_array;
	    new_array.reserve( mChildList.size() );
	    GUIElementArray::iterator itr = mTabOrderArray.begin();
	    GUIElementArray::iterator end = mTabOrderArray.end();
	    for ( ; itr != end; ++itr )
	    {
		    if ( element_ != *itr )
		    {
			    new_array.push_back( *itr );
		    }
	    }
	    mTabOrderArray = new_array;

	    // フォーカス持ってたコントロールが残っているならインデックス探す
	    mActiveTabOrderIndex = -1;
	    if ( element_ != focus )
	    {
		    s32 count = static_cast< s32 >( mTabOrderArray.size() );
		    for ( s32 i = 0; i < count; ++i )
		    {
			    if ( mTabOrderArray[ i ] == focus )
			    {
				    mActiveTabOrderIndex = i;
				    break;
			    }
		    }
	    }

	    LN_SAFE_RELEASE( element_ );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GUIElement* GUIControl::getMouseHoverElement( const LPoint& mouse_pos_ )
    {
        // InternalChildren
        {
	        GUIElementArray::reverse_iterator itr = mInternalChildren.rbegin();
	        GUIElementArray::reverse_iterator end = mInternalChildren.rend();
	        for ( ; itr != end; ++itr )
	        {
		        if ( (*itr)->isVisible() )
                {
                    GUIElement* elm = (*itr)->getMouseHoverElement( mouse_pos_ );
                    if ( elm ) return elm;
                }
	        }
        }

        // 全ての子と判定
        {
	        GUIElementArray::reverse_iterator itr = mChildList.rbegin();
	        GUIElementArray::reverse_iterator end = mChildList.rend();
	        for ( ; itr != end; ++itr )
	        {
		        if ( (*itr)->isVisible() )
                {
                    GUIElement* elm = (*itr)->getMouseHoverElement( mouse_pos_ );
                    if ( elm ) return elm;
                }
	        }
        }

        // 見つからなかった場合は自分自身と判定
        return ( GUIElement::getMouseHoverElement( mouse_pos_ ) ) ? this : NULL;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIControl::updateCoordinate( const LRect& parent_client_rect_ )
    {
        GUIElement::updateCoordinate( parent_client_rect_ );

        // コントロールサイズと同じ
        mGlobalClientRect.x = this->mGlobalPosition.x;
        mGlobalClientRect.y = this->mGlobalPosition.y;
        mGlobalClientRect.w = this->mElementRect.w;
        mGlobalClientRect.h = this->mElementRect.h;

        // 子の処理
	    GUIElementArray::iterator itr = mChildList.begin();
	    GUIElementArray::iterator end = mChildList.end();
	    for ( ; itr != end; ++itr )
	    {
            if ( (*itr)->isVisible() )
		    {
                (*itr)->updateCoordinate( parent_client_rect_ );
		    }
	    }

        ln_foreach(GUIElement* elm, mInternalChildren)
        {
            if ( elm->isVisible() ) elm->updateCoordinate( parent_client_rect_ );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIControl::setActiveElement( GUIElement* child_element_ )
    {
        if ( child_element_ )
        {
	        // リストの最後につなぎかえる
	        Base::STLUtil::remove( mChildList, child_element_ );
	        mChildList.push_back( child_element_ );

	        // 子供のタブオーダーのインデックスを探す
	        mActiveTabOrderIndex = -1;
	        s32 count = static_cast< s32 >( mTabOrderArray.size() );
	        for ( s32 i = 0; i < count; ++i )
	        {
		        if ( mTabOrderArray[ i ] == child_element_ )
		        {
			        mActiveTabOrderIndex = i;
			        break;
		        }
	        }
        }
        else
        {
            mActiveTabOrderIndex = -1;
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GUIElement* GUIControl::getActiveElement()
    {
        if ( mActiveTabOrderIndex != -1 )
	    {
		    return mTabOrderArray[ mActiveTabOrderIndex ];
	    }
	    return NULL;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIControl::setDataContext( INotifyPropertyChanged* object_ )
    {
        GUIElement::setDataContext( object_ );

        ln_foreach( GUIElement* e, mInternalChildren )
        {
            e->setDataContext( object_ );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIControl::initialize( GUIElement::Template* data_ )
    {
        Template* data = (Template*)data_;

        GUIElement::initialize( data );

        ln_foreach( GUIElement::Template* et, data->InternalChildren )
        {
            GUIElement* e = ControlTemplate::createElement( this->mManager, et );
            e->setDataContext( this->mDataContext );
            e->initialize( et );
            addInternalElement( e );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIControl::updateDrawContents()
    {
        GUIElement::updateDrawContents();

        ln_foreach(GUIElement* elm, mChildList)
        {
            if ( elm->isVisible() ) elm->updateDrawContents();
        }

        ln_foreach(GUIElement* elm, mInternalChildren)
        {
            if ( elm->isVisible() ) elm->updateDrawContents();
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIControl::draw()
    {
        GUIElement::draw();

        ln_foreach(GUIElement* elm, mChildList)
        {
            if ( elm->isVisible() ) elm->draw();
        }

        ln_foreach(GUIElement* elm, mInternalChildren)
        {
            if ( elm->isVisible() ) elm->draw();
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIControl::addInternalElement( GUIElement* element_ )
    {
	    LN_THROW_InvalidCall( !element_->mParent, "Already been added." );

	    mInternalChildren.push_back( element_ );
        element_->mParent = this;
        LN_SAFE_ADDREF( element_ );
        element_->mIsInternalElement = true;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIControl::removeInternalElement( GUIElement* element_ )
    {
	    LN_THROW_InvalidCall( element_->mParent == this, "Element is not a child." );

        Base::STLUtil::remove( mInternalChildren, element_ );
        element_->mParent = NULL;
        element_->mIsInternalElement = false;
        LN_SAFE_RELEASE( element_ );

        // ※ Parent を NULL にしてから release しないと、~Element() で無限再起になる
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