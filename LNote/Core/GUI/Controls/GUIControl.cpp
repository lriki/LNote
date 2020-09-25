//=============================================================================
//�y GUIControl �z
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
// �� GUIControl
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
        // �폜���X�g�̍쐬
        //      Element �̃f�X�g���N�^�ŌĂ΂�� mParent->removeControl() �́A
        //      ������ mChildList.remove() ���Ă�ł���B
        //      ���̂��߁A���ʂ� mChildList ���C�e���[�g���ĉ������ƁA�C�e���[�^�̂Ȃ��肪
        //      ���������Ȃ��ăG���[�ƂȂ��Ă��܂��B
        //      ���̂��߁A��x�ʂ̔z��ɃR�s�[���Ă���폜���Ă���B
	    GUIElementArray delete_controls;
	    delete_controls.reserve( mChildList.size() );
	    GUIElementArray::iterator itr = mChildList.begin();
	    GUIElementArray::iterator end = mChildList.end();
	    for ( ; itr != end; ++itr )
	    {
		    delete_controls.push_back( *itr );
	    }

	    // �폜���X�g�̒��g��delete
	    GUIElementArray::iterator itr_del = delete_controls.begin();
	    GUIElementArray::iterator end_del = delete_controls.end();
	    for ( ; itr_del != end_del; ++itr_del )
	    {
		    if ( *itr_del != this )
		    {
                removeControl( *itr_del );
		    }
	    }

        // �����v�f�����l�ɍ폜
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
        // ���łɉ����̎q�ɂȂ��Ă���ꍇ�̓G���[
	    LN_THROW_InvalidCall( !element_->mParent, "Already been added." );
        LN_THROW_InvalidCall( !element_->mIsInternalElement, "element is internal" );

	    // �q�����X�g�ɒǉ�
	    mChildList.push_back( element_ );
        LN_SAFE_ADDREF( element_ );
        element_->mParent = this;

	    // �t�H�[�J�X�����Ă�ꍇ�̓^�u�I�[�_�[���X�g�ɒǉ�����
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
        // ���̃R���g���[���̎q�ł͂Ȃ��ꍇ�G���[
	    LN_THROW_InvalidCall( element_->mParent == this, "Element is not a child." );
        LN_THROW_InvalidCall( !element_->mIsInternalElement, "element is internal" );

        // �q�̃��X�g����O��
        Base::STLUtil::remove( mChildList, element_ );
        element_->mParent = NULL;

	    // ���݃t�H�[�J�X�������Ă���R���g���[���� focus �ɑ��
	    GUIElement* focus = ( mActiveTabOrderIndex == -1 ) ? NULL : mTabOrderArray[ mActiveTabOrderIndex ];

	    // �^�u�I�[�_�[���X�g���č\�z
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

	    // �t�H�[�J�X�����Ă��R���g���[�����c���Ă���Ȃ�C���f�b�N�X�T��
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

        // �S�Ă̎q�Ɣ���
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

        // ������Ȃ������ꍇ�͎������g�Ɣ���
        return ( GUIElement::getMouseHoverElement( mouse_pos_ ) ) ? this : NULL;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIControl::updateCoordinate( const LRect& parent_client_rect_ )
    {
        GUIElement::updateCoordinate( parent_client_rect_ );

        // �R���g���[���T�C�Y�Ɠ���
        mGlobalClientRect.x = this->mGlobalPosition.x;
        mGlobalClientRect.y = this->mGlobalPosition.y;
        mGlobalClientRect.w = this->mElementRect.w;
        mGlobalClientRect.h = this->mElementRect.h;

        // �q�̏���
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
	        // ���X�g�̍Ō�ɂȂ�������
	        Base::STLUtil::remove( mChildList, child_element_ );
	        mChildList.push_back( child_element_ );

	        // �q���̃^�u�I�[�_�[�̃C���f�b�N�X��T��
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

        // �� Parent �� NULL �ɂ��Ă��� release ���Ȃ��ƁA~Element() �Ŗ����ċN�ɂȂ�
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