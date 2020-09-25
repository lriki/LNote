//=============================================================================
//【 EffectInstance 】
//-----------------------------------------------------------------------------
///**
//  @file       EffectInstance.h
//  @brief      EffectInstance
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
// ■ EffectInstance クラス
//-----------------------------------------------------------------------------
///**
//  @brief		エフェクトインスタンスのルート
//*/
//=============================================================================
class EffectInstance
    : public Base::ReferenceObject
{
public:

    EffectInstance( Manager* manager_ );

    virtual ~EffectInstance();

public:

    /// 初期化
    void initialize( EffectCore* core_ );

    /// 時間進行・フレーム更新を Manager から自動で行う (デフォルトは true)
    void setEnableAutoUpdate( bool flag_ ) { mEnableAutoUpdate = flag_; }
    bool isEnableAutoUpdate() const { return mEnableAutoUpdate; }

    /// 時間の設定
    void setFrame( lnTime frame_pos_ );

    /// 時間を進める
    void advanceFrame( lnTime delta_frame_ );

#if 0
    
    /// ワールド行列の設定
    void setTransformMatrix( const LMatrix& mat_ ) { mTransformMatrix = mat_; }

    
#endif

    /// 現在のフレーム位置で更新 (自動更新の場合は Manager から呼び出される)
    void update();

    /// 描画準備
    void updateDrawContents();

    /// 描画
    void draw();

public:

    

    /// Modified フラグが true であるすべての要素に対して再構築を行う
    //void refresh();

private:

    Manager*                        mManager;
    EffectCore*                     mEffectCore;
    EffectNodeInstanceContainer*    mRootContainer;
    lnTime                          mCurrentFramePos;
    lnTime                          mLastFramePos;
    bool                            mEnableAutoUpdate;

    //EffectNodeInstanceGroup* mRootGroup;
    ////EffectNodeInstance* mRootNodeInstance;

    //LMatrix             mTransformMatrix;
    


    //EffectNodeDrawList      mEffectNodeDrawList;
    //EffectNodeInstanceArray mDrawNodeList;

   
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