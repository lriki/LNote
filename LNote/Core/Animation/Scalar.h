//=============================================================================
//【 Scalar 】
//-----------------------------------------------------------------------------
///**
//  @file       Scalar.h
//  @brief      Scalar
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include "../Math/Math.h"
#include "Types.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Animation
{

//=============================================================================
// ■ Scalar クラス
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class Scalar
{
public:

    // キーフレーム
    struct Key
    {
        double      FramePos;       ///< フレーム位置
        lnfloat     Value;          ///< 値
        lnfloat     Velocity;       ///< 速度
        lnfloat     Accel;          ///< 加速度
        lnfloat     LeftSlope;      ///< 左辺 (終了) 速度
        lnfloat     RightSlope;     ///< 右辺 (開始) 速度
        u8          InterMode;      ///< 次のキーとの補間方法 ( LNAnimInterMode の値 )
    
        /// 初期化 ( InterMode は LN_ANIM_INTER_LINEAR、それ以外は 0.0 で初期化 )
        Key();
    };

public:

    Scalar();

public:

    /// 時間のスケール係数の設定 (通常はフレームレートの値をそのまま渡しておく。デフォルト値は 60。setTime() で時間の値に乗算される)
    void setTimeTickPerSec( lnfloat tick_ ) { mTimeTick = tick_; }

    /// キーがひとつもないか、時間が最初のフレーム位置より前のときに返す値の設定
    void setDefaultValue( lnfloat value_ ) { mDefaultValue = value_; }

    /// キーの追加 (ソートは行わない)
    void addKeyFrame( double frame_pos_, lnfloat value_ );

    /// キーの追加 (ソートは行わない)
    void addKeyFrame( const Key& key_ );

    /// キーフレームのソート (フレーム位置の昇順)
    void sortKeyFrame();

    /// キーの数の取得
    int getKeyNum() const { return mKeyFrameList.size(); }

    /// ループ再生の有効設定
    void setEnableLoop( bool flag_ );

    /// ループ領域の設定
    void setLoopFrame( lnfloat begin_frame_, lnfloat end_frame_ );

    /// 時間の設定 (通常、秒で指定する)
    void setTime( double time_ );

    /// 時間を進める (進めた時間で setTime() が呼ばれる)
    void advanceTime( lnfloat delta_time_ );  

    /// 再生が終了しているかを判定する (キーが 0 個の時と、時間が最後のキーのフレーム位置以降の場合 true。ループ再生時は常に false)
    bool isFinished() const;

    /// 値の取得
    lnfloat getValue() const { return mValue; }

    /// キーを全て削除する
    void clear();

    /// キーにアクセスする
    Key* getKey( int idx_ );

    /// キーにアクセスする
    Key& operator [] ( int idx_ );

private:

    static int _cmpKey( const void* a_, const void* b_ );

private:

    typedef std::vector< Key >   KeyFrameList;  

private:

    KeyFrameList    mKeyFrameList;
    double          mTime;
    lnfloat         mTimeTick;
    double          mCurrentFramePos;   ///< setTime() で設定された時間のフレーム位置 (ループ範囲内)
    double          mLoopBeginPos;
    double          mLoopEndPos;
    lnfloat         mDefaultValue;
    lnfloat         mValue;
    bool            mEnableLoop;
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Animation
} // namespace Core
} // namespace LNote




// 使用法によっては全く使わないメンバ変数もある。
// そういうものはとりあえずポインタにしておいて、必要なものだけ new で確保するようにしようとか考えたけど、
// new の数が増えると作成に時間がかかってくる。この辺は前の Effect クラスで問題になったところ。
// そのため、たとえ無駄な領域が増えても、高速化のためにメンバは全部実態で作っておく。

// ランダム値付きのものはさすがにサイズ大きくなるし、エフェクトでしか使わないから、
// これは継承して実装しておく。　← 保留

// ちなみに Effect はすべてスカラーのキーにする。クォータにオンは使わない。
// ( クォータにオンを使う場合は回転の 4 つのトラックに全てキーを作るようにしないとダメとか、
//   いろいろ複雑になるので )

//=============================================================================
//                              end of file
//=============================================================================