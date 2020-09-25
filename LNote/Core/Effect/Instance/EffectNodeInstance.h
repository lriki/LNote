//=============================================================================
//【 EffectNodeInstance 】
//-----------------------------------------------------------------------------
///**
//  @file       EffectNodeInstance.h
//  @brief      EffectNodeInstance
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
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
typedef Base::NodeList< EffectNodeInstance >    EffectNodeDrawList;

//=============================================================================
// ■ EffectNodeInstance クラス
//-----------------------------------------------------------------------------
///**
//  @brief		エフェクトノード
//*/
//=============================================================================
class EffectNodeInstance
    : public Base::ReferenceObject
#if LN_EFFECT_USING_NODE_LIST
    , public NodeInstanceDrawList::Node
#endif
{
public:

    enum State
    {
	    STATE_ACTIVE = 0,
	    STATE_REMOVING,
	    //STATE_REMOVED,
    };

    struct DrawContext
    {
        lnTime  LocalFramePos;
        LMatrix WorldMatrix;
    };

public:

    EffectNodeInstance();

    ~EffectNodeInstance();

public:

    /// 初期化
    void initialize( EffectNodeCore* core_, EffectNodeInstanceContainer* container_ );

    /// フレーム更新
    void update( lnTime delta_frame_ );

    /// 描画準備
    void updateDrawContents();

    /// 描画
    void draw();

    /// 削除
    void destroy() { mState = STATE_REMOVING; }

    /// 状態の取得
    State getState() const { return mState; }

    /// Emitter に伝える乱数シード
    int getRandSeed() const { return (int)this; }

    /// 描画データ
    DrawContext* getDrawContext() { return &mDrawContext; }

private:

    EffectNodeCore*                 mEffectNodeCore;
    EffectNodeInstanceContainer*    mContainer;
    EffectNodeInstanceGroupArray    mChildrenContainsGroupArray;    ///< update() から作成される子 Instance を格納するグループのリスト
    State                           mState;
    lnTime                          mLocalFramePos;
    lnTime                          mLivedFrame;        ///< 生存フレーム数 (無限の場合は DBL_MAX)
    LMatrix                         mWorldMatrix;       ///< 結合済み行列
    DrawContext                     mDrawContext;
    



#if 0
    /// [描画から呼ばれる]
    const LMatrix& getWorldMatrix() { return mWorldMatrixForDraw; }

    /// フレーム位置の設定(frame_pos_ はグループのローカルタイム)
    void update( lnTime frame_pos_, const LMatrix& parent_matrix_ );

    /// 描画リストを作成する
    void updateDrawList( EffectNodeDrawList* draw_node_list_ );

    /// このノード以下の Modified フラグが true であるすべての要素に対して再構築を行う
    void refresh();

    
    /// エミッタインスタンスの取得
    //EmitterInstanceBase* getEmitterInstance() { return mEmitterInstance; }

private:

    typedef std::vector< EffectNodeInstanceGroup* > EffectNodeInstanceGroupArray;

private:

    EffectNodeCore*                 mEffectNodeCore;
    EffectNodeInstanceGroupArray    mEffectNodeInstanceGroupArray;  ///< 子ノードの数分の要素を持つ
    //EmitterInstanceBase*            mEmitterInstance;
    int                             mRandSeed;

    lnTime              mGroupFramePosOffset;         ///< グループ内の生成オフセット
    lnTime              mLocalFramePos;  ///< ローカルのフレーム位置(開始位置によっては負の値にもなる)
    LMatrix             mWorldMatrix;       ///< 結合済み行列

    LMatrix             mWorldMatrixForDraw;       ///< 結合済み行列 (DrawContent)

#endif
};

#if 0

struct EffectNodeCommonParameter
{
    bool        Visible;
    LRefTString Name;
    lnfloat     GenerateStartFramePos;  ///< 生成開始フレーム位置
    lnfloat     GenerateInterval;       ///< 生成間隔 (1以上であること。生成数が 2 以上の場合に使用)

    // 初期値
    EffectNodeCommonParameter()
    {
        Visible = true;
        Name = _T( "Node" );
        GenerateStartFramePos = 0;
        GenerateInterval = 1;
    }
};

//=============================================================================
// ■ EffectNode クラス
//-----------------------------------------------------------------------------
///**
//  @brief		エフェクトノード
//*/
//=============================================================================
class EffectNode
    : public Base::HierarchyNode< EffectNode >
{
public:

    EffectNode( EffectCore* core_ );

    virtual ~EffectNode();

public:

    /// 初期化 (内部 から呼ばれる)
    void initialize();

    /// EffectCore の取得
    EffectCore* getEffectCore() { return mEffectCore; }

    /// エミッタの設定
    void setEmitter( EmitterBase* emitter_ );

    /// マスタトラックの取得
    EffectMasterAnimationTrack* getMasterTrack( LNEffectCommonTrack track_ ) { return &mMasterCommonAnimationTrackArray[ (int)track_ ]; }

    /// [必要なくなるかも？]全キーのランダム値を再設定する (階層更新。初期化時の負荷が気になるようなら、初回のupdate() で呼んでもいいかも(別スレッドで更新する場合))
    void refreshKeyFrames();

    /// 更新 (Core から配列イテレートによって呼ばれる。かつ、更新スレッド)
    void update();

    /// 描画
    void draw();

public:

    /// 子ノードを追加する
    virtual void addChild( NodeType* child_ );

    /// 子ノードを外す
    virtual void removeChild( NodeType* child_ );

private:

    EffectCore*                     mEffectCore;

    LRenderState                    mRenderState;
    MasterCommonAnimationTrackArray mMasterCommonAnimationTrackArray;   ///< キーフレームのマスタデータ (refreshKeyFrames() で これを元に Emitter にキーを構築する)
    //AnimationTracks     mAnimationTracks;

    LMatrix	                        mTransformMatrix;
	LMatrix	                        mCombinedTransformMatrix;

    EmitterBase*                    mEmitter;

    EffectNodeCommonParameter       mCommonParameter;
};

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