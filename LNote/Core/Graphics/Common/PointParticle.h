//=============================================================================
//【 PointParticle 】
//-----------------------------------------------------------------------------
///**
//  @file       PointParticle.h
//  @brief      PointParticle
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"
#include "PointParticleParameter.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

//=============================================================================
// ■ PointParticle クラス
//-----------------------------------------------------------------------------
///**
//  @brief		ポイントスプライトによるパーティクルのクラス
//
//  @note
//              生成時に負荷がすごくかかる可能性がある。その分描画は高速。
//              フィールド背景の街灯の炎など、フィールド生成時にまとめて作る、
//              ある程度静的なエフェクト用になるかも。
//*/
//=============================================================================
class PointParticle
    : public Base::ReferenceObject
{
public:

    /// コンストラクタ
    PointParticle();

    /// デストラクタ
    ~PointParticle();

public:

    /// 初期化
    void initialize( IGraphicsDevice* device_, const PointParticleParameter& data_ );

    /// 時間のスケール係数の設定 (通常はフレームレートの値をそのまま渡しておく。デフォルト値は 60。更新時、時間の値に乗算される)
    void setTimeTickPerSec( lnfloat tick_ ) { mTimeTick = tick_; }

    /// テクスチャの設定
    void setTexture( ITexture* texture_ );

    /// 動的な頂点バッファとして作成されているかを判定する
    bool isDynamic() { return mIsDynamic; }

    /// 時間を進める
    void advanceTime( lnfloat delta_time_ );

    /// 再生時間の設定 (時間を直接指定する。ツールなどで使う)
    void setTime( lnfloat time_ );

    /// 現在の時間を取得する
    lnfloat getTime();

    /// 頂点を更新する (advanceTime()、setTime() の後にこれを呼ぶ)
    LNRESULT update( const LMatrix& world_matrix_ );

    /// 描画 (GeometryRenderer::drawPointParticle() からも呼ばれる)
    LNRESULT draw();

private:

    /// 頂点バッファを埋める
    void _createVertexData( const PointParticleParameter& data_ );

private:

    IRenderer*      mRenderer;
    IVertexBuffer*  mVertexBuffer;		///< 頂点バッファ
    LVector3*       mOriginalPositions;
    LVector3*       mCreatedPositions;

    lnU32           mParticleNum;       ///< 実体のパーティクル数 (残像以外)
    lnU32           mAfterImageLevel;

    lnU32           mOneLoopFrameNum;

    lnfloat         mTimeTick;
    lnfloat         mTime;
    lnfloat         mLastTime;

    ITexture*       mTexture;
    LVector4        mGlobalAccel;
    lnfloat         mGravityPower;
    bool            mIsSequential;
    bool            mIsParticleLoop;
    bool            mIsDynamic;

    friend class GeometryRenderer;
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================