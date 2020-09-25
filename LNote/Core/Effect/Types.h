//=============================================================================
//【 Types 】
//-----------------------------------------------------------------------------
///**
//  @file       Types.h
//  @brief      Types
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include "../Base/Container/NodeList.h"
#include "../Math/Random.h"
#include "../Animation/Scalar.h"
#include "../Graphics/Common/Common.h"

#define LN_EFFECT_USING_NODE_LIST 1

namespace tinyxml2
{
    class XMLElement;
    class XMLDocument;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
enum LNEffectCommonTrack
{
    LN_EFFECT_TRACK_POSITION_X = 0,
    LN_EFFECT_TRACK_POSITION_Y,
    LN_EFFECT_TRACK_POSITION_Z,

    LN_EFFECT_TRACK_ANGLE_X,
    LN_EFFECT_TRACK_ANGLE_Y,
    LN_EFFECT_TRACK_ANGLE_Z,
    LN_EFFECT_TRACK_ANGLE_R,

    LN_EFFECT_TRACK_SCALE_X,
    LN_EFFECT_TRACK_SCALE_Y,
    LN_EFFECT_TRACK_SCALE_Z,

    LN_EFFECT_TRACK_COLOR_R,
    LN_EFFECT_TRACK_COLOR_G,
    LN_EFFECT_TRACK_COLOR_B,
    LN_EFFECT_TRACK_COLOR_A,

    LN_EFFECT_TRACK_TEXTURE_U,
    LN_EFFECT_TRACK_TEXTURE_V,

    LN_EFFECT_MAX_COMMON_TRACKS,
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Effect
{
class EffectCore;
class EffectNodeCore;
class IAnimationTrack;
class EffectAnimationTrackGroup;
class EffectInstance;
class EffectNodeInstance;
class EffectNodeInstanceContainer;
class EffectNodeInstanceGroup;
class EmitterCoreBase;
class EmitterInstanceBase;

enum AnimationTrackType
{
    ANIMTRACKTYPE_VALUE = 0,
    ANIMTRACKTYPE_EVENT,
};

/// (ランダム値の指定は 中央値 - ±幅  10, 10 だと 5～15)
struct AnimationKey
{
    double          FramePos;           ///< フレーム位置

    lnfloat         Value;              ///< 値
    lnfloat         ValueRand;          ///< Value ランダム幅

    lnfloat         Velocity;           ///< 速度
    lnfloat         VelocityRand;       ///< Velocity ランダム幅

    lnfloat         Accel;              ///< 加速度
    lnfloat         AccelRand;          ///< Accel ランダム幅

    lnfloat         LeftSlope;          ///< 左辺 (終了) 速度
    lnfloat         LeftSlopeRand;      ///< LeftSlope ランダム幅

    lnfloat         RightSlope;         ///< 右辺 (開始) 速度
    lnfloat         RightSlopeRand;     ///< RightSlope ランダム幅

    LNAnimInterMode InterMode;          ///< 次のキーとの補間方法 ()

    /// 初期値
    AnimationKey()
        : FramePos          ( 0 )
        , Value             ( 0 )
        , ValueRand         ( 0 )
        , Velocity          ( 0 )
        , VelocityRand      ( 0 )
        , Accel             ( 0 )
        , AccelRand         ( 0 )
        , LeftSlope         ( 0 )
        , LeftSlopeRand     ( 0 )
        , RightSlope        ( 0 )
        , RightSlopeRand    ( 0 )
        , InterMode         ( LN_ANIM_INTER_CATMULLROM )
    {}
};

struct TrackElement
{
    LRefTString         Name;
    LColor              Color;
    AnimationTrackType  Type;
};

struct TrackGroupElement
{
    LRefTString         Name;
    LColor              Color;
};

enum CommonTracks
{
    COMMONTRACKS_POSITION_X = 0,
    COMMONTRACKS_POSITION_Y,
    COMMONTRACKS_POSITION_Z,

    COMMONTRACKS_ROTATION_X,
    COMMONTRACKS_ROTATION_Y,
    COMMONTRACKS_ROTATION_Z,
    COMMONTRACKS_ROTATION_R,

    COMMONTRACKS_SCALING_X,
    COMMONTRACKS_SCALING_Y,
    COMMONTRACKS_SCALING_Z,

    COMMONTRACKS_COLOR_R,
    COMMONTRACKS_COLOR_G,
    COMMONTRACKS_COLOR_B,
    COMMONTRACKS_COLOR_A,

    COMMONTRACKS_TEXUV_X,
    COMMONTRACKS_TEXUV_Y,

    COMMONTRACKS_MAX,
};

enum CommonTrackGroup
{
    COMMONTRACKGROUP_POSITION = 0,
    COMMONTRACKGROUP_ROTATION,
    COMMONTRACKGROUP_SCALING,
    COMMONTRACKGROUP_COLOR,
    COMMONTRACKGROUP_TEXUV,
};




static const int LN_EFFECT_COMMONTRACK_GROUPE_NUM = 5;

//typedef Animation::Scalar               AnimationTrack;
typedef std::vector< IAnimationTrack* >  AnimationTrackArray;
typedef std::vector< EffectAnimationTrackGroup* >   AnimationTrackGroupArray;

typedef std::vector< LRefTString >      StringArray;

typedef std::vector< EmitterInstanceBase* >       EmitterInstanceArray;
typedef std::vector< AnimationKey >             AnimationKeyArray;
//typedef std::vector< CommonAnimationKeyArray >  MasterCommonAnimationTrackArray;
typedef std::vector< EffectNodeCore* >              EffectNodeCoreArray;






typedef std::vector< EffectNodeInstanceContainer* > EffectNodeInstanceContainerArray;
typedef std::vector< EffectNodeInstanceGroup* >     EffectNodeInstanceGroupArray;
typedef std::vector< EffectNodeInstance* >          EffectNodeInstanceArray;

#if LN_EFFECT_USING_NODE_LIST
typedef Base::NodeList< EffectNodeInstanceGroup >   NodeInstanceGroupDrawList;
typedef Base::NodeList< EffectNodeInstance >        NodeInstanceDrawList;
#else
typedef std::vector< EffectNodeInstanceGroup* >       NodeInstanceGroupDrawList;
typedef std::vector< EffectNodeInstance* >            NodeInstanceDrawList;
#endif

typedef double lnTime;

/*
// EffectNode 内の Animation::Scalar は、ランダム無しの値をエディタで使うためのもの。ゲーム中では必要ない。
struct AnimationTracks
{
    Animation::Scalar   PositionX;
    Animation::Scalar   PositionY;
    Animation::Scalar   PositionZ;

    Animation::Scalar   RotationX;
    Animation::Scalar   RotationY;
    Animation::Scalar   RotationZ;
    Animation::Scalar   RotationR;

    Animation::Scalar   ScaleX;
    Animation::Scalar   ScaleY;
    Animation::Scalar   ScaleZ;

    Animation::Scalar   ColorR;
    Animation::Scalar   ColorG;
    Animation::Scalar   ColorB;
    Animation::Scalar   ColorA;

    Animation::Scalar   TextureU;
    Animation::Scalar   TextureV;

    // 以下、値を保存するために含める (もし妥協するならエミッタごとにunionでもOK)
    Animation::Scalar   RingInner;
    Animation::Scalar   RingOuter;
    Animation::Scalar   RingCenterRatio;
};
*/

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================