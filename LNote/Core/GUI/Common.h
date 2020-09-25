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

/// GUI �R���g���[���̘g�̕`����@
enum FrameImagePlace
{
    FRAMEPLACE_IMAGE_SCALING = 0, ///< �C���[�W�����̂܂܃R���g���[���̃T�C�Y�Ɋg�傷��
    FRAMEPLACE_FRAME_SCALING,     ///< �g�̏㉺���E���g�傷��
    FRAMEPLACE_FRAME_TILING,      ///< �g�̏㉺���E���^�C����ɔz�u����
};

/// �R���g���[���̔w�i�̐������@
enum FrameBackgroundFormat
{
    FRAMEBACKGROUNDFORMAT_NONE = 0,          ///< �`�悵�Ȃ�
    FRAMEBACKGROUNDFORMAT_SCALING,           ///< �C���[�W���g�傷��
    FRAMEBACKGROUNDFORMAT_TILING,            ///< �C���[�W���^�C����ɔz�u����
};

/// �g�̐������@
struct FrameFormatConfig
{
    LRect           ImageSrcRect;   ///< �\�[�X�C���[�W������̈�
    FrameImagePlace Format;         ///< �g�̍쐬���@
};

/// �w�i�̐������@
struct FrameBackgroundFormatConfig
{
    LRect                   ImageSrcRect;   ///< �\�[�X�C���[�W������̈�
    FrameBackgroundFormat   Format;         ///< �쐬���@
};

} // namespace GUI
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================