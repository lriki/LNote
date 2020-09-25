//=============================================================================
//�y DummyNode �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../SceneGraph.h"
#include "DummyNode.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

//=============================================================================
// �� DummyNode �N���X
//=============================================================================

    /// ������
    LNRESULT DummyNode::initialize( bool axis_drawing_ )
    {
        mAxisDrawing = axis_drawing_;
        SceneNode::initialize( 1, LN_DRAWCOORD_UNKNOWN );

        if ( mAxisDrawing )
        {
            setShader( this->mSceneGraph->getDefaultShader( LN_DEFSHADER_NOLIGHTING ) );
        }

        return LN_OK;
    }

    // �T�u�Z�b�g��`�悷��
    LNRESULT DummyNode::drawSubset( u32 index_ )
    {
        if ( mAxisDrawing )
        {
            this->mSceneGraph->getGraphicsManager()->getGeometryRenderer()->drawTransformAxis( 10 );
        }
        return LN_OK;
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================