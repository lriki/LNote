//=============================================================================
//�y Manager �z
//-----------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Common.h"

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
// �� Manager
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

    struct InitData
    {
        Graphics::Manager*  GraphicsManager;
        LSize               VirtualScreenSize;
    };

public:

    Manager();

public:

    /// ������
    void initialize( const InitData& data_ );

    /// �I������
    void finalize();

    /// �f�X�N�g�b�v�̎擾
    GUIDesktop* getDesktop() { return mDesktop; }

    /// �f�t�H���g�̃X�L���e�N�X�`���̎擾
    Graphics::ITexture* getDefaultSkinTexture() { return mDefaultSkinTexture; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���g���[���Ƀt�H�[�J�X��n��
    //*/
    //---------------------------------------------------------------------
    void setFocusControl( GUIElement* element_ );

    /// �t�H�[�J�X�������Ă���R���g���[����Ԃ� (�����ꍇ�� Desktop ��Ԃ�)
    GUIElement* getFocusElement();

    //---------------------------------------------------------------------
    ///**
    //  @brief      �}�E�X�ʒu�̃R���g���[�����������A�ێ�����
    //  @note
    //              �}�E�X�ʒu�̃R���g���[���� mMouseHoverElement �Ɋi�[����B
    //              mMouseHoverElement ���Ⴄ�R���g���[���Ɉڂ����ꍇ�A
    //              �Â����ɂ� LN_GUIEV_MOUSE_LEAVE�A
    //              �V�������ɂ� LN_GUIEV_MOUSE_ENTER �𒼂��ɑ��M����B
    //              ���t���[���X�V�őS�v�f�� update() ��ŏ������邱�ƁB
    //*/
    //---------------------------------------------------------------------
    void updateMouseHover();

    //---------------------------------------------------------------------
    ///**
    //  @brief      �S�ẴR���g���[���̃t���[���X�V
    //*/
    //---------------------------------------------------------------------
    void update( const Core::Game::GameTime& game_time_ );

    /// �C�x���g����
    bool doEvent( const System::Event& e_ );


    void updateDrawContents();
    void render();

public:

    Graphics::Manager* getGraphicsManager() { return mGraphicsManager; }
    Graphics::GeometryRenderer* getGeometryRenderer() { return mGraphicsManager->getGeometryRenderer(); }

private:

    Graphics::Manager*  mGraphicsManager;

    GUIElement*         mFocusElement;          ///< GUI�V�X�e���S�̂Ńt�H�[�J�X�������Ă���R���g���[��
    GUIElement*         mMouseHoverElement;     ///< �}�E�X�̈ʒu�ɂ���R���g���[��
    LPoint              mMousePosition;         ///< onEvent() �Őݒ肳���A�ϊ��s��K�p�ς݂̃}�E�X�ʒu
    GUIElement*         mCaptureElement;

    GUIDesktop*         mDesktop;

    LSize               mVirtualScreenSize;
    LMatrix             mMousePosTransform;

    Graphics::ITexture* mDefaultSkinTexture;
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