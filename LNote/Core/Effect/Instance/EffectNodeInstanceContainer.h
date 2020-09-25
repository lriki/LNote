//=============================================================================
//【 EffectNodeInstanceContainer 】
//-----------------------------------------------------------------------------
///**
//  @file       EffectNodeInstanceContainer.h
//  @brief      EffectNodeInstanceContainer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Types.h"
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
// ■ EffectNodeInstanceContainer
//-----------------------------------------------------------------------------
///**
//  @brief      グループの集合
//  @note
//              データクラスの EffectNodeCore に 1対1 で対応する
//*/
//=============================================================================
class EffectNodeInstanceContainer
    : public Base::ReferenceObject
{
public:

    EffectNodeInstanceContainer( Manager* manager_ );

    virtual ~EffectNodeInstanceContainer();

public:

    /// 初期化
    void initialize( EffectNodeCore* node_core_, EffectInstance* instance_ );

    /// エフェクトインスタンスの取得
    EffectInstance* getEffectInstance() { return mEffectInstance; }

    /// 子コンテナリストの取得
    EffectNodeInstanceContainerArray& getChildrenContainer() { return mChildrenContainerArray; }

    /// グループの作成 (ルートコンテナ、ノードインスタンス生成時に呼ばれる)
    EffectNodeInstanceGroup* createNodeInstanceGroup();

    /// 全ての要素を強制的に削除する (~EffectNodeInstanceContainer() から呼ぶときは false)
    void removeForcibly( bool recursive );

    /// 全ての Instance を破棄する (破棄フラグを立てる。removeForcibly() から呼ぶときは false)
    void destroyAllInstance( bool recursive_ );

    /// 無効なグループをすべて削除する
    void removeInvalidGroups();

    /// フレーム更新
    void update( lnTime delta_frame_ );

    /// 描画準備
    void updateDrawContents();

    /// 描画
    void draw();

private:

    Manager*                            mManager;
    EffectNodeCore*                     mEffectNodeCore;
    EffectInstance*                     mEffectInstance;
    EffectNodeInstanceContainerArray    mChildrenContainerArray;
    EffectNodeInstanceGroupArray        mNodeInstanceGroupArray;
    NodeInstanceGroupDrawList           mNodeInstanceGroupDrawList;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================