//=============================================================================
//【 EffectNodeInstanceContainer 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Manager.h"
#include "../Core/EffectNodeCore.h"
#include "../Emitter/EmitterBase.h"
#include "EffectNodeInstanceGroup.h"
#include "EffectNodeInstanceContainer.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Effect
{

//=============================================================================
// ■ EffectNodeInstanceContainer
//=============================================================================

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectNodeInstanceContainer::EffectNodeInstanceContainer( Manager* manager_ )
        : mManager          ( manager_ )
        , mEffectNodeCore   ( NULL )
        , mEffectInstance   ( NULL )
    {
        LN_SAFE_ADDREF( mManager );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectNodeInstanceContainer::~EffectNodeInstanceContainer()
    {
        ln_foreach( EffectNodeInstanceGroup* g, mNodeInstanceGroupDrawList )
        {
            g->release();
        }
        mNodeInstanceGroupDrawList.clear();

        removeForcibly( false );


        ln_foreach( EffectNodeInstanceContainer* c, mChildrenContainerArray )
        {
            LN_SAFE_RELEASE( c );
        }
        mChildrenContainerArray.clear();

        LN_SAFE_RELEASE( mManager );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstanceContainer::initialize( EffectNodeCore* node_core_, EffectInstance* instance_ )
    {
        mEffectNodeCore = node_core_;
        mEffectInstance = instance_;

        // 子コンテナを再帰的に作成
        EffectNodeCore::NodeList& list = mEffectNodeCore->getChildren();
        ln_foreach( EffectNodeCore* n, list )
        {
            EffectNodeInstanceContainer* c = LN_NEW EffectNodeInstanceContainer( this->mManager );
            c->initialize( n, instance_ );
            mChildrenContainerArray.push_back( c );
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectNodeInstanceGroup* EffectNodeInstanceContainer::createNodeInstanceGroup()
    {
        EffectNodeInstanceGroup* group = LN_NEW EffectNodeInstanceGroup();
        group->initlaize( mEffectNodeCore, this );
        mNodeInstanceGroupArray.push_back( group );
        return group;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstanceContainer::removeForcibly( bool recursive )
    {
        destroyAllInstance( false );

        ln_foreach( EffectNodeInstanceGroup* g, mNodeInstanceGroupArray )
        {
            g->removeForcibly();
        }
	    removeInvalidGroups();

        if ( recursive )
	    {
            ln_foreach( EffectNodeInstanceContainer* c, mChildrenContainerArray )
            {
                c->removeForcibly( recursive );
            }
	    }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstanceContainer::destroyAllInstance( bool recursive_ )
    {
        ln_foreach( EffectNodeInstanceGroup* g, mNodeInstanceGroupArray )
        {
            g->destroyAllInstance();
        }

	    if ( recursive_ )
	    {
            ln_foreach( EffectNodeInstanceContainer* c, mChildrenContainerArray )
            {
                c->destroyAllInstance( recursive_ );
            }
	    }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstanceContainer::removeInvalidGroups()
    {
        EffectNodeInstanceGroupArray::iterator itr = mNodeInstanceGroupArray.begin();
        while ( itr != mNodeInstanceGroupArray.end() )
        {
            if ( (*itr)->checkInvalid() )
            {
                LN_SAFE_RELEASE( *itr );
                itr = mNodeInstanceGroupArray.erase( itr );
            }
            else
            {
                ++itr;
            }
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstanceContainer::update( lnTime delta_frame_ )
    {
        ln_foreach( EffectNodeInstanceGroup* g, mNodeInstanceGroupArray )
        {
            g->update( delta_frame_);
        }

        //removeInvalidGroups();

        ln_foreach( EffectNodeInstanceContainer* c, mChildrenContainerArray )
        {
            c->update( delta_frame_ );
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstanceContainer::updateDrawContents()
    {
        ln_foreach( EffectNodeInstanceGroup* g, mNodeInstanceGroupDrawList )
        {
            g->release();
        }
        mNodeInstanceGroupDrawList.clear();

        removeInvalidGroups();

        // 子グループ
        ln_foreach( EffectNodeInstanceGroup* g, mNodeInstanceGroupArray )
        {
            g->updateDrawContents();
            mNodeInstanceGroupDrawList.push_back( g );
            g->addRef();
        }

        // 子コンテナ
        ln_foreach( EffectNodeInstanceContainer* c, mChildrenContainerArray )
        {
            c->updateDrawContents();
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstanceContainer::draw()
    {
        EmitterCoreBase* emitter = mEffectNodeCore->getEmitterCore();
        if ( emitter )
        {
            emitter->beginContainerRendering();

            ln_foreach( EffectNodeInstanceGroup* g, mNodeInstanceGroupDrawList )
            {
                emitter->beginGroupRendering();

                g->draw();

                //EffectNodeInstanceArray& children = g->getChildNodeInstanceArray();
                //ln_foreach( EffectNodeInstance* i, children )
                //{
                //    i->draw();
                //}

                emitter->endGroupRendering();
            }

            emitter->endContainerRendering();
        }

        ln_foreach( EffectNodeInstanceContainer* c, mChildrenContainerArray )
        {
            c->draw();
        }
    }

   

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================