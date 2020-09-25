//=============================================================================
//【 EffectNodeInstanceGroup 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Core/EffectNodeCore.h"
#include "EffectNodeInstance.h"
#include "EffectNodeInstanceGroup.h"

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
// ■ EffectNodeInstanceGroup クラス
//=============================================================================

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectNodeInstanceGroup::EffectNodeInstanceGroup()// EffectInstance* effect_ )
        //: mEffectInstance   ( effect_ )
        : mEffectNodeCore   ( NULL )
        , mContainer        ( NULL )
        //, mGenerateCount    ( 0 )
        //, mLocalFramePos    ( 0 )
        //, mLastLocalFramePos    ( 0 )
    {
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectNodeInstanceGroup::~EffectNodeInstanceGroup()
    {
        ln_foreach( EffectNodeInstance* i, mNodeInstanceDrawList )
        {
            i->release();
        }
        mNodeInstanceDrawList.clear();

        //_dispose();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstanceGroup::initlaize( EffectNodeCore* core_, EffectNodeInstanceContainer* container_ )
    {
        mEffectNodeCore = core_;
        mContainer = container_;

        //reset();

        //update( 0, parent_matrix_ );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectNodeInstance* EffectNodeInstanceGroup::createNodeInstance()
    {
        EffectNodeInstance* instance = LN_NEW EffectNodeInstance();
        instance->initialize( mEffectNodeCore, mContainer );
        //mNodeInstanceAddedList.push_back();
        mNodeInstanceArray.push_back( instance );
        return instance;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstanceGroup::removeForcibly()
    {
        destroyAllInstance();
	    removeInvalidInstances();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstanceGroup::destroyAllInstance()
    {
        ln_foreach( EffectNodeInstance* i, mNodeInstanceArray )
        {
            i->destroy();
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    bool EffectNodeInstanceGroup::checkInvalid()
    {
        if ( mNodeInstanceArray.empty() ) return true;

        ln_foreach( EffectNodeInstance* i, mNodeInstanceArray )
        {
            if ( i->getState() == EffectNodeInstance::STATE_ACTIVE ) return false;
        }

        return true;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstanceGroup::removeInvalidInstances()
    {
        EffectNodeInstanceArray::iterator itr = mNodeInstanceArray.begin();
        while ( itr != mNodeInstanceArray.end() )
        {
            if ( (*itr)->getState() == EffectNodeInstance::STATE_REMOVING )
            {
                LN_SAFE_RELEASE( *itr );
                itr = mNodeInstanceArray.erase( itr );
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
    void EffectNodeInstanceGroup::update( lnTime delta_frame_ )
    {
        //removeInvalidInstances();

        ln_foreach( EffectNodeInstance* i, mNodeInstanceArray )
        {
            i->update( delta_frame_ );
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstanceGroup::updateDrawContents()
    {
        ln_foreach( EffectNodeInstance* i, mNodeInstanceDrawList )
        {
            i->release();
        }
        mNodeInstanceDrawList.clear();

        ln_foreach( EffectNodeInstance* i, mNodeInstanceArray )
        {
            i->updateDrawContents();
            mNodeInstanceDrawList.push_back( i );
            i->addRef();
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstanceGroup::draw()
    {
        ln_foreach( EffectNodeInstance* i, mNodeInstanceDrawList )
        {
            i->draw();
        }
    }

#if 0
    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstanceGroup::reset()
    {
        _dispose();

        mGenerateCount = 0;

        // 初回(0フレーム目)生成の準備
        mLocalFramePos = -1;
        mLastLocalFramePos = -1;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstanceGroup::update( lnTime frame_pos_, const LMatrix& parent_matrix_ )
    {
        // データクラスに変更がある場合は再構築
        if ( mEffectNodeCore->isModified() )
        {
            refresh();
        }

        mLocalFramePos = frame_pos_ - mEffectNodeCore->getGenerateStartFramePos();

        // 生成処理
        if ( mLocalFramePos >= 0 )
        {
            lnTime delta_time = mLocalFramePos - mLastLocalFramePos;

            // + 方向に動いた場合 ( 0 だと生成されないので注意
            if ( delta_time > 0 )
            {
                // 次に生成するべき時間
                lnTime next_create_frame =
                    mLastLocalFramePos +
                    ( mEffectNodeCore->getGenerateInterval() - fmod( mLastLocalFramePos, mEffectNodeCore->getGenerateInterval() ) );

                // 新しく設定された時間までに生成する必要のあるノードを生成
                while ( next_create_frame <= mLocalFramePos && mGenerateCount < mEffectNodeCore->getMaxGenerateCount() )
                {
                    EffectNodeInstance* new_node = LN_NEW EffectNodeInstance();
                    new_node->initialize( mEffectNodeCore, next_create_frame, parent_matrix_ );
                    //mAddedNodeList.push_back( new_node );
                    mEffectNodeInstanceArray.push_back( new_node );

                    next_create_frame += mEffectNodeCore->getGenerateInterval();
                    ++mGenerateCount;
                }


                // 今回生成するべき数
                //int created_num = diff / mEffectNodeCore->getCommonParameter()->GenerateInterval;

            }
            
        }

        // 更新処理
        ln_foreach( EffectNodeInstance* n, mEffectNodeInstanceArray )
        {
            n->update( frame_pos_, parent_matrix_ );
        }


        mLastLocalFramePos = mLocalFramePos;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    //void EffectNodeInstanceGroup::updateDrawList( EffectNodeDrawList* draw_node_list_ )
    //{
    //    //mDrawEmitterInstanceList.clear();
    //    ln_foreach( EffectNodeInstance* n, mEffectNodeInstanceArray )
    //    {
    //        mDrawEmitterInstanceList.push_back( n->getEmitterInstance() );
    //        n->updateDrawList( draw_node_list_ );
    //    }
    //}

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    //void EffectNodeInstanceGroup::refresh()
    //{
    //    ln_foreach( EffectNodeInstance* n, mEffectNodeInstanceArray )
    //    {
    //        mDrawEmitterInstanceList.push_back( n->getEmitterInstance() );
    //        n->refresh();
    //    }
    //}

    

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectNodeInstanceGroup::_dispose()
    {
        //ln_foreach( EffectNodeInstance* n, mAddedNodeList )
        //{
        //    LN_SAFE_RELEASE( n );
        //}
        //mAddedNodeList.clear();

        ln_foreach( EffectNodeInstance* n, mEffectNodeInstanceArray )
        {
            LN_SAFE_RELEASE( n );
        }
        mEffectNodeInstanceArray.clear();

    }

#endif


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