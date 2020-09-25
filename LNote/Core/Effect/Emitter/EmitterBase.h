//=============================================================================
//【 EmitterBase 】
//-----------------------------------------------------------------------------
///**
//  @file       EmitterBase.h
//  @brief      EmitterBase
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Interface.h"
#include "../Types.h"
#include "../Manager.h"
#include "../Core/EffectNodeCore.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Effect
{
class EmitterInstanceBase;

//=============================================================================
// ■ EmitterCoreBase クラス
//-----------------------------------------------------------------------------
///**
//  @brief		エフェクトエミッタの基底
//*/
//=============================================================================
class EmitterCoreBase
    : public Base::ReferenceObject
{
public:

    EmitterCoreBase( Manager* manager_ );

    virtual ~EmitterCoreBase() {}

public:

    /// Modified フラグを設定する
    void setModifiedFlag( bool flag_ ) { mModified = flag_; }

    /// 拡張トラック数の取得
    virtual int getExTrackGroupNum() const { return 0; }

    /// 拡張トラックグループの取得
    virtual EffectAnimationTrackGroup* getExTrackGroup( int idx_ ) { return NULL; }

    /// コンテナの描画開始
    virtual void beginContainerRendering() {}

    /// グループの描画開始
    virtual void beginGroupRendering() {}

    /// 描画
    virtual void draw( EffectNodeInstance* instance_ ) {}

    /// グループの描画終了
    virtual void endGroupRendering() {}

    /// コンテナの描画終了
    virtual void endContainerRendering() {}


    /// エミッタインスタンスクラスの生成 (生成数パラメータによる自動生成のため)
    //virtual EmitterInstanceBase* createInstance() = 0;

protected:

    Manager*    mManager;
    bool        mModified;
};

#if 0

//=============================================================================
// ■ EmitterInstanceBase クラス
//-----------------------------------------------------------------------------
///**
//  @brief		エミッタインスタンスの基底
//*/
//=============================================================================
class EmitterInstanceBase
    : public Base::ReferenceObject
{
public:

    EmitterInstanceBase()
    {}

    virtual ~EmitterInstanceBase() {}

public:

    /// データクラスの Modified フラグが true であれば再構築を行う
    virtual void refresh() {}

    /// 描画情報の更新
    virtual void updateDrawContents() {}

    /// 描画
    virtual void draw( const LMatrix& world_mat_ ) {}

protected:

};

#endif

#if 0
//=============================================================================
// ■ EmitterBase クラス
//-----------------------------------------------------------------------------
///**
//  @brief		エフェクトエミッタの基底
//*/
//=============================================================================
class EmitterBase
    : public Base::ReferenceObject
{
public:

    EmitterBase( EffectNode* node_ )
        : mEffectNode( node_ )
    { }

    virtual ~EmitterBase() { }

public:

    /// 共通ステートの再設定 (生成数情報の再設定などを行う)
    void refreshCommonState( const EffectNodeCommonParameter& param_ );

    /// 全キーのランダム値を再設定する
    void refreshKeyFrames( const MasterCommonAnimationTrackArray& master_tracks_ );

    //void update(  );

public:

    /// エミッタ描画クラスの生成 (生成数パラメータによる自動生成のため)
    virtual EmitterSubsetBase* createSubset() = 0;

    /// 描画
    virtual void draw();

protected:

    EffectNode*         mEffectNode;
    EmitterSubsetArray  mEmitterSubsetArray;
};

//=============================================================================
// ■ EmitterSubsetBase クラス
//-----------------------------------------------------------------------------
///**
//  @brief		エフェクトエミッタ描画クラスの基底
//*/
//=============================================================================
class EmitterSubsetBase
{
public:

    /// 全キーのランダム値を再設定する
    void refreshKeyFrames( const MasterCommonAnimationTrackArray& master_tracks_ );

    /// 描画
    virtual void draw() = 0;
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