//=============================================================================
//�y GUIControl �z
//-----------------------------------------------------------------------------
///**
//  @file       GUIControl.h
//  @brief      GUIControl
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
// �� GUIControl
//-----------------------------------------------------------------------------
///**
//  @brief      
//  @note
//              InternalChildren �̗v�f�̓t�H�[�J�X�Ɋւ��鏈���͍s��Ȃ��B
//*/
//=============================================================================
class GUIControl
    : public GUIElement
{
public:

    GUIControl( Manager* manager_ );
    virtual ~GUIControl();

public:

    /// ������
    void initialize();

    /// �R���g���[�����q�Ƃ��Ēǉ����� (�Q�ƃJ�E���g++)
    void addControl( GUIElement* element_ );

    /// �w�肳�ꂽ�R���g���[�����q�̏ꍇ�A���̃R���e�i����O�� (�K�v������� GUIElement �̃f�X�g���N�^����Ă΂��)
    void removeControl( GUIElement* element_ );

    /// child_element_ ���q�R���g���[�����A�N�e�B�u�ɂ��� (�^�u�I�[�_�[���������Aelement_ �� Z �I�[�_�[����Ԏ�O�ɂ���BNULL�ŉ���)
    void setActiveElement( GUIElement* child_element_ );

    /// ���̃R���e�i���̎q�R���g���[���̂����A�A�N�e�B�u�Ȃ��̂�Ԃ�
    GUIElement* getActiveElement();

public:

    virtual void setDataContext( INotifyPropertyChanged* object_ );

    virtual void initialize( GUIElement::Template* data_ );

    virtual GUIElement* getMouseHoverElement( const LPoint& mouse_pos );

    /// �ʒu���̍X�V
    virtual void updateCoordinate( const LRect& parent_client_rect_ );

    /// �`����̍쐬
    virtual void updateDrawContents();

    /// �`��
    virtual void draw();

protected:

    void addInternalElement( GUIElement* element_ );

    void removeInternalElement( GUIElement* element_ );

private:

    GUIElementArray     mChildList;
    GUIElementArray	    mTabOrderArray;		    ///< �^�u�I�[�_�[
	s32				    mActiveTabOrderIndex;   ///< �A�N�e�B�u�R���g���[���� mTabOrderArray ���ł̃C���f�b�N�X
    LRect               mGlobalClientRect;      ///< �O���[�o�����W�n�ϊ��ς݂̃N���C�A���g�̈�
    LRect               mScissorRect;           ///< �`��Ɏg���V�U�[�̈�
    GUIElementArray     mInternalChildren;      ///< ��{�I�ɕ`��v�f���i�[���� (clearAllControls �̉e�����󂯂Ȃ��悤�ɂ��邽��)

    friend class GUIElement;

public:

    struct Template
        : public GUIElement::Template
    {
         TemplateArray  InternalChildren;
    };
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