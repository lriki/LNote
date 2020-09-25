//=============================================================================
//【 Common 】
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
#include "../Geometry/DrawingShapes.h"
#include "../Graphics/Common/GeometryRenderer.h"
#include "../Graphics/Manager.h"
#include "../Graphics/Interface.h"
#include "../Game/GameTime.h"
#include "GUIEvent.h"
#include "Binding/INotifyPropertyChanged.h"
#include "Binding/BindingValue.h"

namespace LNote
{
namespace Core
{
namespace GUI
{

class GUIElement;
class GUIControl;
class GUIDesktop;
class GUIShape;
class GUISprite;

class BindingSourceValue;
class GUIButtonDataContext;

typedef std::vector<GUIElement*> GUIElementArray;

enum ElementType
{
    ELEMENTTYPE_UNKNOWN = 0,
    ELEMENTTYPE_WINDOW,
    ELEMENTTYPE_BUTTON,

    ELEMENTTYPE_SPRITE,
    ELEMENTTYPE_TEXTBLOCK,
};

/// GUI コントロールの枠の描画方法
enum FrameImagePlace
{
    FRAMEPLACE_IMAGE_SCALING = 0, ///< イメージをそのままコントロールのサイズに拡大する
    FRAMEPLACE_FRAME_SCALING,     ///< 枠の上下左右を拡大する
    FRAMEPLACE_FRAME_TILING,      ///< 枠の上下左右をタイル状に配置する
};

/// コントロールの背景の生成方法
enum FrameBackgroundFormat
{
    FRAMEBACKGROUNDFORMAT_NONE = 0,          ///< 描画しない
    FRAMEBACKGROUNDFORMAT_SCALING,           ///< イメージを拡大する
    FRAMEBACKGROUNDFORMAT_TILING,            ///< イメージをタイル状に配置する
};

/// 枠の生成方法
struct FrameFormatConfig
{
    LRect           ImageSrcRect;   ///< ソースイメージがある領域
    FrameImagePlace Format;         ///< 枠の作成方法
};

/// 背景の生成方法
struct FrameBackgroundFormatConfig
{
    LRect                   ImageSrcRect;   ///< ソースイメージがある領域
    FrameBackgroundFormat   Format;         ///< 作成方法
};

} // namespace GUI
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================