//=============================================================================
//�y DummyNode �z
//-----------------------------------------------------------------------------
///**
//  @file       DummyNode.h
//  @brief      DummyNode
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "SceneNode.h"



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
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class DummyNode
    : public SceneNode
{
public:
    DummyNode( SceneGraph* scene_ )
        : SceneNode( scene_ )
    {}

protected:
    virtual ~DummyNode() {}

public:

    /// ������
    LNRESULT initialize( bool axis_drawing_ = false );

    /// �m�[�h�P�ʂ̕`��p�����[�^�X�V
    //virtual LNRESULT updateNodeRenderParam( Camera* camera_, const LNSubsetRenderParam* parent_params_ )
    //{
    //    mZDistance = 0.0f;

    //    SceneNodeList::iterator itr = mChildNodeList.begin();
	   // SceneNodeList::iterator end = mChildNodeList.end();
	   // for ( ;itr != end; ++itr )
	   // {
		  //  (*itr)->updateNodeRenderParam( camera_, NULL );
	   // }

    //    mChildNodeList.sort( _cmpZSort );

    //    return LN_OK;
    //}

public:

    /// �T�u�Z�b�g��`�悷��
    virtual LNRESULT drawSubset( u32 index_ );

private:

    bool    mAxisDrawing;

};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================