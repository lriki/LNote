//=============================================================================
//【 ControlTemplate 】
//-----------------------------------------------------------------------------
///**
//  @file       ControlTemplate.h
//  @brief      ControlTemplate
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Common.h"
#include "../GUIElement.h"

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
// ■ ControlTemplate
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class ControlTemplate
    : public Base::ReferenceObject
{
public:

    ControlTemplate();

public:

    /// 初期化
    void initialize();

    GUIElement::Template* getElementTemplate() const { return mTemplateData; }

public:

    GUIElement::Template* mTemplateData;    ///< インスタンスは各コントロール::Template

public:

    static GUIElement* createElement( Manager* manager_, GUIElement::Template* template_data_ );

    static ControlTemplate* createDefaultButtonTemplate();

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