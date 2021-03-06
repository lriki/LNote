//=============================================================================
//【 ModelComponent 】
//-----------------------------------------------------------------------------
///**
//  @file       ModelComponent.h
//  @brief      ModelComponent
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../File/ASyncProcessObject.h"
#include "../../Scene/SceneNode/Model.h"
#include "GameComponent.h"

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
// ■ ModelComponent
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class ModelComponent
    : public Component
    , public File::ASyncProcessObject
{
public:

    ModelComponent( Manager* manager );

    virtual ~ModelComponent();

public:

    /// ファイル名の設定
    void setFileName( const lnChar* filename );

public:

    /// (GameObject から呼ばれる)
    void initialize( GameObject* object );

    virtual bool operator()() { return true; }

private:

    void _release();

    void _startLoad();

private:

    ln_std_tstring  mFileName;

    Scene::Model*   mModel;
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