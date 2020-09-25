//=============================================================================
//【 DummyNode 】
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
// ■ DummyNode クラス
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

    /// 初期化
    LNRESULT initialize( bool axis_drawing_ = false );

    /// ノード単位の描画パラメータ更新
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

    /// サブセットを描画する
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