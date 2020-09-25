//=============================================================================
//【 EffectNodeInstance 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Emitter/EmitterBase.h"
#include "../Core/EffectCore.h"
#include "EffectNodeInstanceContainer.h"
#include "EffectNodeInstanceGroup.h"
#include "EffectNodeInstance.h"

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
// ■ EffectNodeInstance クラス
//=============================================================================

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectNodeInstance::EffectNodeInstance()
        : mEffectNodeCore       ( NULL )
        , mState                ( STATE_ACTIVE )
        , mLocalFramePos        ( 0 )
        , mLivedFrame           ( 0 )
        //, mEmitterInstance      ( NULL )
        //, mGroupFramePosOffset  ( 0 )
        //, mLocalFramePos        ( 0 )
    {
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectNodeInstance::~EffectNodeInstance()
    {
        //SAFE_DELETE( mEmitterInstance );

        //ln_foreach( EffectNodeInstanceGroup* group, mEffectNodeInstanceGroupArray )
        //{
        //    LN_SAFE_RELEASE( group );
        //}
        //mEffectNodeInstanceGroupArray.clear();

        //LN_SAFE_RELEASE( mEffectNodeCore );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstance::initialize(  EffectNodeCore* core_, EffectNodeInstanceContainer* container_ )// EffectNodeCore* core_, lnTime group_frame_offset_, const LMatrix& parent_matrix_ )
    {
        mEffectNodeCore = core_;
        mContainer = container_;

        // このインスタンスが生成する子インスタンスを格納するグループを作成する
        EffectNodeInstanceContainerArray& children = mContainer->getChildrenContainer();
        mChildrenContainsGroupArray.reserve( children.size() );
        ln_foreach( EffectNodeInstanceContainer* c, children )
        {
            EffectNodeInstanceGroup* g = c->createNodeInstanceGroup();
            mChildrenContainsGroupArray.push_back( g );
        }

        mState = STATE_ACTIVE;

        // 仮　無限
        mLivedFrame = DBL_MAX;

        update( 0 );

        /*
        LN_SAFE_ADDREF( mEffectNodeCore );

        mGroupFramePosOffset = group_frame_offset_;

        // グループ
        EffectNodeCore::NodeList& children = mEffectNodeCore->getChildren();
        int child_num = children.size();
        for ( int i = 0; i < child_num; ++i )
        {
            EffectNodeInstanceGroup* g = LN_NEW EffectNodeInstanceGroup();
            g->initlaize( children[i], parent_matrix_ );
            mEffectNodeInstanceGroupArray.push_back( g );
        }

        // エミッタ
        //EmitterCoreBase* emitter_core = mEffectNodeCore->getEmitterCore();
        //if ( emitter_core )
        //{
        //    mEmitterInstance = emitter_core->createInstance();
        //}

        //update( 0, parent_matrix_ );
        */
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstance::update( lnTime delta_frame_ )
    {
        if ( mState != STATE_ACTIVE ) return;

        

        mWorldMatrix.identity();

        // スケーリング
        {
            EffectAnimationTrackGroup* track_group =
                mEffectNodeCore->getAnimationTrackGroup( COMMONTRACKGROUP_SCALING );
            track_group->resetRand( (int)this );
            track_group->update( mLocalFramePos );
            mWorldMatrix.scaling(
                track_group->getValue( 0 ),
                track_group->getValue( 1 ),
                track_group->getValue( 2 ) );

            //_p(mLocalFramePos);
            //_p(track_group->getValue( 0 ));
        }

        mLocalFramePos += delta_frame_;

        if ( mLocalFramePos <= mLivedFrame )
        {
            // 子の生成処理
            EffectNodeCore::NodeList& core_children = mEffectNodeCore->getChildren();
            int core_children_count = core_children.size();
            for ( int i = 0; i < core_children_count; ++i )
		    {
                EffectNodeCore* child = core_children[i];

                // 今回生成するべき数を計算
                int living_time = mLocalFramePos - mEffectNodeCore->getGenerateStartFramePos();
                int living_time_p = living_time + delta_frame_;

                if ( living_time < 0 ) continue;

                int generation_count = (int)( living_time / mEffectNodeCore->getGenerateInterval() );
			    int generation_count_p = (int)( living_time_p / mEffectNodeCore->getGenerateInterval() );
    			
			    // 最初の1フレーム
			    if( living_time < 1.0f )
			    {
				    generation_count_p += 1;
			    }
			    else
			    {
				    generation_count += 1;
				    generation_count_p += 1;
			    }

                generation_count = LMath::min( generation_count, mEffectNodeCore->getMaxGenerateCount() );
			    generation_count_p = LMath::min( generation_count_p, mEffectNodeCore->getMaxGenerateCount() );

                for ( int j = generation_count; j < generation_count_p; j++ )
                {
                    EffectNodeInstance* instance = mChildrenContainsGroupArray[i]->createNodeInstance();
                    if ( !instance ) break;
                }
            }
        }

    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstance::updateDrawContents()
    {
        mDrawContext.LocalFramePos = mLocalFramePos;
        mDrawContext.WorldMatrix = mWorldMatrix;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstance::draw()
    {
        EmitterCoreBase* emitter = mEffectNodeCore->getEmitterCore();
        if ( emitter )
        {
            emitter->draw( this );
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    //void EffectNodeInstance::update( lnTime frame_pos_, const LMatrix& parent_matrix_ )
    //{
    //    mLocalFramePos = frame_pos_ - mGroupFramePosOffset;


    //    

    //    mWorldMatrix = parent_matrix_;

    //    EffectAnimationTrackGroup* track_group;
    //    //track = mEffectNodeCore->getAnimationTrackGroup( LN_EFFECT_COMMONTRACKGROUP_SCALING )->getTrack( 0 );
    //    //track->setTime( mLocalFramePos );

    //    // スケーリング
    //    track_group = mEffectNodeCore->getAnimationTrackGroup( COMMONTRACKGROUP_SCALING );
    //    track_group->update( mLocalFramePos );
    //    mWorldMatrix.scaling(
    //        track_group->getValue( 0 ),
    //        track_group->getValue( 1 ),
    //        track_group->getValue( 2 ) );

    //    // 子の処理
    //    ln_foreach( EffectNodeInstanceGroup* group, mEffectNodeInstanceGroupArray )
    //    {
    //        group->update( mLocalFramePos, mWorldMatrix );
    //    }
    //}

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    //void EffectNodeInstance::updateDrawList( EffectNodeDrawList* draw_node_list_ )
    //{
    //    mEmitterInstance->updateDrawContents();
    //    draw_node_list_->push_back( this );

    //    ln_foreach( EffectNodeInstanceGroup* group, mEffectNodeInstanceGroupArray )
    //    {
    //        group->updateDrawList( draw_node_list_ );
    //    }

    //    mWorldMatrixForDraw = mWorldMatrix;
    //}

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    //void EffectNodeInstance::refresh()
    //{
    //    mEmitterInstance->refresh();

    //    ln_foreach( EffectNodeInstanceGroup* group, mEffectNodeInstanceGroupArray )
    //    {
    //        group->refresh();
    //    }
    //}

    

#if 0
//=============================================================================
// ■ EffectNode クラス
//=============================================================================

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectNode::EffectNode( EffectCore* core_ )
        : mEffectCore   ( core_ )
        , mEmitter      ( NULL )
    {
        mRenderState.BlendMode  = LN_BLEND_ADD;
        mRenderState.Culling    = LN_CULL_DOUBLE;
        mRenderState.FillMode   = LN_FILL_SOLID;
        mRenderState.AlphaTest  = false;
        mRenderState.DepthTest  = false;
        mRenderState.DepthWrite = true;

        mMasterCommonAnimationTrackArray.resize( LN_EFFECT_MAX_COMMON_TRACKS );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectNode::~EffectNode()
    {
        LN_SAFE_RELEASE( mEmitter );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNode::initialize()
    {
        
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNode::setEmitter( EmitterBase* emitter_ )
    {
        LN_SAFE_ADDREF( emitter_ );
        LN_SAFE_RELEASE( mEmitter );
        mEmitter = emitter_;

        if ( mEmitter )
        {
            mEmitter->refreshCommonState( mCommonParameter );
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNode::refreshKeyFrames()
    {
        if ( mEmitter )
        {
            mEmitter->refreshKeyFrames( mMasterCommonAnimationTrackArray );
        }

        ln_foreach( EffectNode* n, this->mChildren )
        {
            n->refreshKeyFrames();
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNode::update()
    {
        //ln_foreach( EffectNode* n, mSubsets )
        //{
        //    n->update();
        //}
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNode::draw()
    {
        if ( mEmitter )
        {
            mEmitter->draw();
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNode::addChild( NodeType* child_ )
    {
        Base::HierarchyNode< EffectNode >::addChild( child_ );
        mEffectCore->addSubset( child_ );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNode::removeChild( NodeType* child_ )
    {
        Base::HierarchyNode< EffectNode >::removeChild( child_ );
        mEffectCore->removeSubset( child_ );
    }
#endif
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================