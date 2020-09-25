//=============================================================================
//Åy RingEmitter Åz
//-----------------------------------------------------------------------------
///**
//  @file       RingEmitter.h
//  @brief      RingEmitter
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Common/SpriteRenderer.h"
#include "EmitterBase.h"

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
// Å° RingEmitterCore
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class RingEmitterCore
    : public EmitterCoreBase
{
public:

    enum TrackGroups
    {
        TRACKGROUP_ANGLE = 0,
        TRACKGROUP_OUTER_LOCATION,
        TRACKGROUP_INNER_LOCATION,
        TRACKGROUP_CENTER_RATIO,
        TRACKGROUP_OUTER_COLOR,
        TRACKGROUP_CENTER_COLOR,
        TRACKGROUP_INNER_COLOR,

        TRACKGROUP_MAX,
    };

public:

    RingEmitterCore( Manager* manager_ );

    virtual ~RingEmitterCore();

public:

    void initialize();

    int getAnimationTrackGroupNum() const { return TRACKGROUP_MAX; }
    EffectAnimationTrackGroup* getAnimationTrackGroup( int index_ ) { return &mAnimationTrackGroups[index_]; }


    void            setTextureFilename( LRefTString filename_ ) { mTextureFilename = filename_; this->mModified = true; }
    LRefTString     getTextureFilename() { return mTextureFilename; }

    void            setDivisionNum( int num_ ) { mDivisionNum = (num_ < 3) ? 3 : num_; this->mModified = true; }
    int             getDivisionNum() const { return mDivisionNum; }

public:

    virtual void beginContainerRendering();

    virtual void draw( EffectNodeInstance* instance_ );

    virtual void endContainerRendering();

private:

    LRefTString                 mTextureFilename;
    int                         mDivisionNum;
    EffectAnimationTrackGroup   mAnimationTrackGroups[TRACKGROUP_MAX];
};

#if 0

//=============================================================================
// Å° RingEmitterInstance
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class RingEmitterInstance
    : public EmitterInstanceBase
{
public:

    RingEmitterInstance( RingEmitterCore* core_ );

    virtual ~RingEmitterInstance();

public:

    virtual void refresh();

    virtual void updateDrawContents();

    virtual void draw( const LMatrix& world_mat_ );

protected:

    RingEmitterCore*    mEmitterCore;
    Graphics::ITexture* mTexture;
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