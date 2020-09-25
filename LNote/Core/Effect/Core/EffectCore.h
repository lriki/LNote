//=============================================================================
//【 EffectCore 】
//-----------------------------------------------------------------------------
///**
//  @file       EffectCore.h
//  @brief      EffectCore
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <algorithm>
#include "../Types.h"
#include "EffectAnimationTrackGroup.h"

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
// ■ EffectCore クラス
//-----------------------------------------------------------------------------
///**
//  @brief		エフェクト
//*/
//=============================================================================
class EffectCore
    : public Base::ReferenceObject
{
public:

    EffectCore( Manager* manager_ );

    virtual ~EffectCore();

public:

    float   getLocalFPS() const { return mLocalFPS; }
    void    setLocalFPS( float fps_ ) { mLocalFPS = fps_; mModified = true; }

public:

    /// 初期化 (filename_ が NULL の場合は 空のエフェクトで作成)
    void initialize( const lnChar* filename_ );

    /// Manager の取得
    Manager* getManager() { return mManager; }

    /// ルートノードの取得
    EffectNodeCore* getRootNode() { return mRootNode; } 

    

    /// 未使用ノードインデックスの走査
    //int getUnusedNodeIndex();

    /// XML ファイルに保存
    void saveToXML( const lnChar* filename_ );

private:

    Manager*            mManager;
    EffectNodeCore*     mRootNode;
    float               mLocalFPS;
    bool                mModified;
};

#if 0
class EffectNode;

//=============================================================================
// ■ EffectCore クラス
//-----------------------------------------------------------------------------
///**
//  @brief		エフェクト
//*/
//=============================================================================
class EffectCore
    : public Base::ReferenceObject
{
public:

    EffectCore( Manager* manager_ );

    virtual ~EffectCore();

public:

    /// 初期化 (Manager から呼ばれる)
    void initialize();

    /// Manager の取得
    Manager* getManager() { return mManager; }

    /// ルートノードの取得
    EffectNode* getRootNode() { return mRootNode; } 

    /// フレーム位置の設定
    void setFramePos( lnTime frame_ );

    /// 全キーのランダム値を再設定する
    void refreshKeyFrames();

    /// サブセット数の取得
    lnU32 getSubsetNum() const { return mSubsets.size(); }

    /// 更新 (更新スレッドから呼ばれる)
    void update();

    /// 描画
    void drawSubset( lnU32 index_ );

public:

    /// 子ノード追加時に呼ばれる
    void addSubset( EffectNode* node_ ) { mSubsets.push_back( node_ ); }

    /// 子ノード削除時に呼ばれる
    void removeSubset( EffectNode* node_ ) { std::remove( mSubsets.begin(), mSubsets.end(), node_ ); }

private:

    Manager*        mManager;
    EffectNode*     mRootNode;
    EffectNodeArray mSubsets;

    lnTime          mFramePos;
};
#endif
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core
} // namespace LNote

#if 0



class EffectNode
{
    void update()
    {
        if (新しく生成する時間になった)
        {
            mEffectNodeSubsetArray.push_back(new EffectNodeSubset(this));
        }

        for (mEffectNodeSubsetArray)
        {
            itr->setTime(mCurrentTime - mOffset * i);
        }
    }

    // ×生成数が決まっている場合は最初に要素確保。「無限」の場合はその都度 push_back
    //EffectNodeSubsetArray   mEffectNodeSubsetArray; 
    EffectNodeSubset*   mEffectNodeSubset;  // ひとつだけ
};

class EffectNodeSubset
{
    EffectNodeSubset(EffectNode* node_)
    {
        mAttachedNode = node_;

        for(node_->Children())
        {
            mChildSubsetArray.push_back(new EffectNodeSubset(itr));
        }

        // スプライトなど静的なものは new しないで同じポインタを返すのもありかも？
        mEmitterSubset = mAttachedNode->getEmitter()->createEmitterSubset();
    }

 /*   // アニメーション更新 (update でいいかも。
    void setTime(time_)
    {
        
    }*/

    /// (time_ は、生成の計算をしやすくするために親のローカルタイム)
    void update()
    {
        //for(mChildSubsetArray)
        //{
        //    itr->setTime(time_);
        //}

        for (基本マスタートラックリスト)
        {

            // 指定タイムの前後のキーを取得
            key0, key1

            // キーに乱数値を反映


            // 値の取得

            // キーに反映した乱数値をもとに戻す

            // 
            mCommonParamArray.PosXとか = value;
        }

        for (拡張マスタートラックリスト)
        {
            // 同上

            mEXTrackValues[i] = value;  // 格納 (描画時に拡張トラックに渡す)
        }
    }

    void draw()
    {
        // ここで設定する値は、シェーダへ転送するために update では設定できないもの。

        mEmitterSubset->setCommonParam(mCommonParam);

        // 拡張トラックの設定を反映
        for (mEXTrackValues)
        {
            mEmitterSubset->setValue(i, mEXTrackValues[i]);
        }
        mEmitterSubset->draw();
    }

    // サブセットのツリーは、対象のノードをルートとした別のツリーになる。
    // = かならずルートはひとつの EffectNode。
    // 子はすべて管理するが、兄弟は管理しない。(親に任せる)
    EffectNodeSubsetArray   mChildSubsetArray;

    EffectNode* mAttachedNode;

    EmitterSubsetBase* mEmitterSubset;

    CommonParam    mCommonParam;
    FloatArray mEXTrackValues;
};

#endif

//=============================================================================
//								end of file
//=============================================================================