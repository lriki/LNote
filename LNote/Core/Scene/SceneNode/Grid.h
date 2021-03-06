//=============================================================================
//【 Grid 】
//-----------------------------------------------------------------------------
///**
//  @file       Grid.h
//  @brief      Grid
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Common/Grid.h"
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
// ■ Grid クラス
//-----------------------------------------------------------------------------
///**
//  @brief		グリッド
//*/
//=============================================================================
class Grid
    : public SceneNode
{
public:

    /// コンストラクタ
    Grid( SceneGraph* scene_ );

    /// デストラクタ
    virtual ~Grid();

public:

    /// 初期化
    void initialize();

public:

    /// ノードの種類の取得
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_GRID; }

    /// サブセットを描画する
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    Graphics::Grid* mGrid;
    
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