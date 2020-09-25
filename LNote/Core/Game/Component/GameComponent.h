//=============================================================================
//�y GameComponent �z
//-----------------------------------------------------------------------------
///**
//  @file       GameComponent.h
//  @brief      GameComponent
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Common.h"
#include "../GameSceneManager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Game
{

//=============================================================================
// �� Component
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class Component
    : public Base::ReferenceObject
{
public:

    Component( Manager* manager );

    virtual ~Component();

public:

    /// (GameObject ����Ă΂��)
    void initialize( GameObject* object );

protected:

    Manager*    mManager;
    GameObject* mGameObject;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Game
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================