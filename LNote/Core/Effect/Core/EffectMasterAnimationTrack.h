//=============================================================================
//【 EffectMasterAnimationTrack 】
//-----------------------------------------------------------------------------
///**
//  @file       EffectMasterAnimationTrack.h
//  @brief      EffectMasterAnimationTrack
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
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
// ■ EffectMasterAnimationTrack クラス
//-----------------------------------------------------------------------------
///**
//  @brief		
//
//  @note
//              このクラス自体はキーを保持するだけ。
//*/
//=============================================================================
class EffectMasterAnimationTrack
{
public:

    EffectMasterAnimationTrack() {}

    virtual ~EffectMasterAnimationTrack() {}

public:

    /// キーの追加 (追加と同時にソート)
    void addKey( const AnimationKey& key_ );

private:

    AnimationKeyArray mAnimationKeyArray;
};

typedef std::vector< EffectMasterAnimationTrack >    MasterCommonAnimationTrackArray;

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================