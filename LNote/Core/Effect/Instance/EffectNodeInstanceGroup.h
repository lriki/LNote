//=============================================================================
//【 EffectNodeInstanceGroup 】
//-----------------------------------------------------------------------------
///**
//  @file       EffectNodeInstanceGroup.h
//  @brief      EffectNodeInstanceGroup
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include <list>
#include "../Types.h"

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
//-----------------------------------------------------------------------------
///**
//  @brief		エフェクトグループ。「生成数」パラメータの対応。
//
//  @note
//              座標変換行列などは複製ノードごとに持つべきメンバ。
//              対して、グループは生成数などの共通パラメータを管理。
//
//              イメージは EffectNodeCore → EffectNodeInstanceGroup。
//              EffectNodeCore → EffectNodeInstance じゃないので注意。
//*/
//=============================================================================
class EffectNodeInstanceGroup
    : public Base::ReferenceObject
#if LN_EFFECT_USING_NODE_LIST
    , public NodeInstanceGroupDrawList::Node
#endif
{
public:

    EffectNodeInstanceGroup();

    ~EffectNodeInstanceGroup();

public:

    /// 初期化
    void initlaize( EffectNodeCore* core_, EffectNodeInstanceContainer* container_ );

    /// EffectNodeInstance 作成
    EffectNodeInstance* createNodeInstance();

    /// 子 EffectNodeInstance リスト
    EffectNodeInstanceArray& getChildNodeInstanceArray() { return mNodeInstanceArray; }

    /// 全ての要素を強制的に削除する
    void removeForcibly();

    /// 全ての Instance を破棄する (破棄フラグを立てる)
    void destroyAllInstance();

    /// 無効なグループかをチェックする
    bool checkInvalid();

    /// 無効な NodeInstance をすべて削除する
    void removeInvalidInstances();

    /// フレーム更新
    void update( lnTime delta_frame_ );

    /// 描画準備
    void updateDrawContents();

    void draw();

private:

    EffectNodeCore*                 mEffectNodeCore;
    EffectNodeInstanceContainer*    mContainer; ///< このグループを保持しているコンテナ
    EffectNodeInstanceArray         mNodeInstanceArray;
    NodeInstanceDrawList            mNodeInstanceDrawList;
    //EffectNodeInstanceList          mNodeInstanceAddedList;

#if 0
    /// 生成数などの状態をリセット
    void reset();

    /// フレーム位置の設定(親ノードのローカルタイムを渡すこと。ルートであればグローバルタイム)
    void update( lnTime frame_pos_, const LMatrix& parent_matrix_ );

    /// 描画リストを作成する
    void updateDrawList( EffectNodeDrawList* draw_node_list_ );

    /// このノード以下のデータクラスの Modified フラグが true であるすべての要素に対して再構築を行う
    void refresh();

    

private:

    /// 解放処理
    void _dispose();

private:

    
    typedef std::list< EffectNodeInstance* >    EffectNodeInstanceList;
    typedef std::vector< EmitterInstanceBase* > EmitterInstanceArray;

private:

    //EffectInstance*         mEffectInstance;
    EffectNodeCore*         mEffectNodeCore;
    EffectNodeInstanceArray mEffectNodeInstanceArray;
    //EffectNodeInstanceList  mAddedNodeList;             ///< update() 内で作成されたノードが一時的に格納される
    int                     mGenerateCount;

    lnTime                  mLocalFramePos;             ///< 0 は最初の生成位置
    lnTime                  mLastLocalFramePos;
    
    EmitterInstanceArray    mDrawEmitterInstanceList;

#endif
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