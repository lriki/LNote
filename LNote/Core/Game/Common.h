//=============================================================================
//�y Common �z
//-----------------------------------------------------------------------------
///**
//  @file       Common.h
//  @brief      Common
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Game
{

class GameObject;
class GameScene;
class Component;

typedef std::vector< GameObject* >  GameObjectArray;
typedef std::vector< Component* >   ComponentArray;

} // namespace Game
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================