//=============================================================================
//【 RingEmitter 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "RingEmitter.h"

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
// ■ RingEmitterCore
//=============================================================================

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    RingEmitterCore::RingEmitterCore( Manager* manager_ )
        : EmitterCoreBase   ( manager_ )
        , mDivisionNum( 16 )
    {
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    RingEmitterCore::~RingEmitterCore()
    {
        printf( "~RingEmitterCore()\n" );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void RingEmitterCore::initialize()
    {
        // 角度
        TrackGroupElement angle_group_elm = 
        {
            _T( "RingAngle" ), LColor( 0.75, 1.0, 0.75 ),
        };
        TrackElement angle_ele[] =
        {
            { _T("Angle"), LColor( 1, 0, 0 ), ANIMTRACKTYPE_VALUE }
        };
        mAnimationTrackGroups[TRACKGROUP_ANGLE].initialize( angle_group_elm, 1, angle_ele );
        mAnimationTrackGroups[TRACKGROUP_ANGLE].getTrack( 0 )->setDefaultValue( 360 );

        // 外輪
        TrackGroupElement outer_loc_group_elm = 
        {
            _T( "RingOuter" ), LColor( 0.75, 1.0, 0.75 ),
        };
        TrackElement outer_loc_ele[] =
        {
            { _T("OuterRadius"), LColor( 1, 0, 0 ), ANIMTRACKTYPE_VALUE },
            { _T("OuterHeight"), LColor( 0, 1, 0 ), ANIMTRACKTYPE_VALUE }
        };
        mAnimationTrackGroups[TRACKGROUP_OUTER_LOCATION].initialize( outer_loc_group_elm, 2, outer_loc_ele );
        mAnimationTrackGroups[TRACKGROUP_OUTER_LOCATION].getTrack( 0 )->setDefaultValue( 1.0 );
        mAnimationTrackGroups[TRACKGROUP_OUTER_LOCATION].getTrack( 1 )->setDefaultValue( 0.0 );

        // 内輪
        TrackGroupElement inner_loc_group_elm = 
        {
            _T( "RingInner" ), LColor( 1.0, 0.75, 0.75 ),
        };
        TrackElement inner_loc_ele[] =
        {
            { _T("InnerRadius"), LColor( 1, 0, 0 ), ANIMTRACKTYPE_VALUE },
            { _T("InnerHeight"), LColor( 0, 1, 0 ), ANIMTRACKTYPE_VALUE }
        };
        mAnimationTrackGroups[TRACKGROUP_INNER_LOCATION].initialize( inner_loc_group_elm, 2, inner_loc_ele );
        mAnimationTrackGroups[TRACKGROUP_INNER_LOCATION].getTrack( 0 )->setDefaultValue( 0.75 );
        mAnimationTrackGroups[TRACKGROUP_INNER_LOCATION].getTrack( 1 )->setDefaultValue( 0.0 );

        // 中心比率
        TrackGroupElement center_ratio_group_elm = 
        {
            _T( "CenterRatio" ), LColor( 0.75, 0.75, 1.0 ),
        };
        TrackElement center_ratio_ele[] =
        {
            { _T("CenterRatio"), LColor( 1, 0, 0 ), ANIMTRACKTYPE_VALUE }
        };
        mAnimationTrackGroups[TRACKGROUP_CENTER_RATIO].initialize( center_ratio_group_elm, 1, center_ratio_ele );
        mAnimationTrackGroups[TRACKGROUP_CENTER_RATIO].getTrack( 0 )->setDefaultValue( 0.5 );

        // 外輪色
        TrackGroupElement outer_color_group_elm = 
        {
            _T( "RingOuterColor" ), LColor( 0.75, 1.0, 1.0 ),
        };
        TrackElement outer_color_ele[] =
        {
            { _T("R"), LColor( 1, 0, 0 ), ANIMTRACKTYPE_VALUE },
            { _T("G"), LColor( 0, 1, 0 ), ANIMTRACKTYPE_VALUE },
            { _T("B"), LColor( 0, 0, 1 ), ANIMTRACKTYPE_VALUE },
            { _T("A"), LColor( 0.5, 0.5, 0.5 ), ANIMTRACKTYPE_VALUE }
        };
        mAnimationTrackGroups[TRACKGROUP_OUTER_COLOR].initialize( outer_color_group_elm, 4, outer_color_ele );
        mAnimationTrackGroups[TRACKGROUP_OUTER_COLOR].getTrack( 0 )->setDefaultValue( 1.0 );
        mAnimationTrackGroups[TRACKGROUP_OUTER_COLOR].getTrack( 1 )->setDefaultValue( 1.0 );
        mAnimationTrackGroups[TRACKGROUP_OUTER_COLOR].getTrack( 2 )->setDefaultValue( 1.0 );
        mAnimationTrackGroups[TRACKGROUP_OUTER_COLOR].getTrack( 3 )->setDefaultValue( 0.0 );

        // 中心色
        TrackGroupElement center_color_group_elm = 
        {
            _T( "RingCenterColor" ), LColor( 1.0, 1.0, 0.75 ),
        };
        TrackElement center_color_ele[] =
        {
            { _T("R"), LColor( 1, 0, 0 ), ANIMTRACKTYPE_VALUE },
            { _T("G"), LColor( 0, 1, 0 ), ANIMTRACKTYPE_VALUE },
            { _T("B"), LColor( 0, 0, 1 ), ANIMTRACKTYPE_VALUE },
            { _T("A"), LColor( 0.5, 0.5, 0.5 ), ANIMTRACKTYPE_VALUE }
        };
        mAnimationTrackGroups[TRACKGROUP_CENTER_COLOR].initialize( center_color_group_elm, 4, center_color_ele );
        mAnimationTrackGroups[TRACKGROUP_CENTER_COLOR].getTrack( 0 )->setDefaultValue( 1.0 );
        mAnimationTrackGroups[TRACKGROUP_CENTER_COLOR].getTrack( 1 )->setDefaultValue( 1.0 );
        mAnimationTrackGroups[TRACKGROUP_CENTER_COLOR].getTrack( 2 )->setDefaultValue( 1.0 );
        mAnimationTrackGroups[TRACKGROUP_CENTER_COLOR].getTrack( 3 )->setDefaultValue( 1.0 );


        // 内輪色
        TrackGroupElement inner_color_group_elm = 
        {
            _T( "RingInnerColor" ), LColor( 1.0, 0.75, 1.0 ),
        };
        TrackElement inner_color_ele[] =
        {
            { _T("R"), LColor( 1, 0, 0 ), ANIMTRACKTYPE_VALUE },
            { _T("G"), LColor( 0, 1, 0 ), ANIMTRACKTYPE_VALUE },
            { _T("B"), LColor( 0, 0, 1 ), ANIMTRACKTYPE_VALUE },
            { _T("A"), LColor( 0.5, 0.5, 0.5 ), ANIMTRACKTYPE_VALUE }
        };
        mAnimationTrackGroups[TRACKGROUP_INNER_COLOR].initialize( inner_color_group_elm, 4, inner_color_ele );
        mAnimationTrackGroups[TRACKGROUP_INNER_COLOR].getTrack( 0 )->setDefaultValue( 1.0 );
        mAnimationTrackGroups[TRACKGROUP_INNER_COLOR].getTrack( 1 )->setDefaultValue( 1.0 );
        mAnimationTrackGroups[TRACKGROUP_INNER_COLOR].getTrack( 2 )->setDefaultValue( 1.0 );
        mAnimationTrackGroups[TRACKGROUP_INNER_COLOR].getTrack( 3 )->setDefaultValue( 0.0 );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void RingEmitterCore::beginContainerRendering()
    {
        this->mManager->getRingRenderer()->begin();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void RingEmitterCore::draw( EffectNodeInstance* instance_ )
    {
        EffectNodeInstance::DrawContext* context = instance_->getDrawContext();

        for ( int i = 0; i < TRACKGROUP_MAX; ++i )
        {
            mAnimationTrackGroups[i].resetRand( instance_->getRandSeed() );
            mAnimationTrackGroups[i].update( context->LocalFramePos );
        }

        Graphics::RingRenderer::DrawingParamater param;
        param.Transform = context->WorldMatrix;
        param.DivisionCount = mDivisionNum;
        param.ViewingAngle = mAnimationTrackGroups[TRACKGROUP_ANGLE].getValue( 0 );
        param.OuterRadius = mAnimationTrackGroups[TRACKGROUP_OUTER_LOCATION].getValue( 0 );
        param.OuterHeight = mAnimationTrackGroups[TRACKGROUP_OUTER_LOCATION].getValue( 1 );
        param.InnerRadius = mAnimationTrackGroups[TRACKGROUP_INNER_LOCATION].getValue( 0 );
        param.InnerHeight = mAnimationTrackGroups[TRACKGROUP_INNER_LOCATION].getValue( 1 );
        param.CenterRatio = mAnimationTrackGroups[TRACKGROUP_CENTER_RATIO].getValue( 0 );
        param.OuterColor.set(
            mAnimationTrackGroups[TRACKGROUP_OUTER_COLOR].getValue( 0 ),
            mAnimationTrackGroups[TRACKGROUP_OUTER_COLOR].getValue( 1 ),
            mAnimationTrackGroups[TRACKGROUP_OUTER_COLOR].getValue( 2 ),
            mAnimationTrackGroups[TRACKGROUP_OUTER_COLOR].getValue( 3 ) );
        param.CenterColor.set(
            mAnimationTrackGroups[TRACKGROUP_CENTER_COLOR].getValue( 0 ),
            mAnimationTrackGroups[TRACKGROUP_CENTER_COLOR].getValue( 1 ),
            mAnimationTrackGroups[TRACKGROUP_CENTER_COLOR].getValue( 2 ),
            mAnimationTrackGroups[TRACKGROUP_CENTER_COLOR].getValue( 3 ) );
        param.InnerColor.set(
            mAnimationTrackGroups[TRACKGROUP_INNER_COLOR].getValue( 0 ),
            mAnimationTrackGroups[TRACKGROUP_INNER_COLOR].getValue( 1 ),
            mAnimationTrackGroups[TRACKGROUP_INNER_COLOR].getValue( 2 ),
            mAnimationTrackGroups[TRACKGROUP_INNER_COLOR].getValue( 3 ) );
        param.UV = LRectF( 0, 0, 1, 1 );
        param.BillboardType = LN_BILLBOARD_NOT;

        this->mManager->getRingRenderer()->drawRequest3D( param );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void RingEmitterCore::endContainerRendering()
    {
        this->mManager->getRingRenderer()->end();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    //EmitterInstanceBase* RingEmitterCore::createInstance()
    //{
    //    RingEmitterInstance* e = LN_NEW RingEmitterInstance( this );
    //    e->refresh();
    //    return e;
    //}


//=============================================================================
// ■ RingEmitterInstance
//=============================================================================
#if 0
    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    RingEmitterInstance::RingEmitterInstance( RingEmitterCore* core_ )
        : mEmitterCore  ( core_ )
        , mTexture      ( NULL )
    {
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    RingEmitterInstance::~RingEmitterInstance()
    {
        LN_SAFE_RELEASE( mTexture );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void RingEmitterInstance::refresh()
    {
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void RingEmitterInstance::updateDrawContents()
    {

    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void RingEmitterInstance::draw( const LMatrix& world_mat_ )
    {
        _p("RingEmitterInstance::draw");
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