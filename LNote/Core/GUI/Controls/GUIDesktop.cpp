//=============================================================================
//�y GUIDesktop �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Manager.h"
#include "GUIDesktop.h"

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
// �� GUIDesktop
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GUIDesktop::GUIDesktop( Manager* manager_ )
        : GUIControl    ( manager_ )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIDesktop::initialize()
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIDesktop::updateCoordinate( const LRect& parent_client_rect_ )
    {
        mTargetViewSize.w = parent_client_rect_.w;
        mTargetViewSize.h = parent_client_rect_.h;
        this->mElementRect.w = parent_client_rect_.w;
        this->mElementRect.h = parent_client_rect_.h;
        GUIControl::updateCoordinate( parent_client_rect_ );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GUIDesktop::draw()
    {
        this->mManager->getGeometryRenderer()->setViewSize(
            LVector2( (lnfloat)mTargetViewSize.w, (lnfloat)mTargetViewSize.h ) );

        GUIControl::draw();
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