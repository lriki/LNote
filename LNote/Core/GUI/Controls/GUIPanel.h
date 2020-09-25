//=============================================================================
//【 GUIPanel 】
//-----------------------------------------------------------------------------
///**
//  @file       GUIPanel.h
//  @brief      GUIPanel
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Common.h"
#include "GUIControl.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace GUI
{

//=============================================================================
// ■ GUIPanel
//-----------------------------------------------------------------------------
///**
//  @brief      子オブジェクトの配置および整列を行う
//  @note
//              
//*/
//=============================================================================
class GUIPanel
    : public GUIControl
{
public:

    GUIPanel( Manager* manager_ );

public:

    /// 初期化
    void initialize();

};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace GUI
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================