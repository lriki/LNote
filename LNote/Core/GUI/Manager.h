//=============================================================================
//【 Manager 】
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
// ■ Manager
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

    /// 初期化
    void initialize( const InitData& data_ );

    /// 終了処理
    void finalize();

    /// デスクトップの取得
    GUIDesktop* getDesktop() { return mDesktop; }

    /// デフォルトのスキンテクスチャの取得
    Graphics::ITexture* getDefaultSkinTexture() { return mDefaultSkinTexture; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      コントロールにフォーカスを渡す
    //*/
    //---------------------------------------------------------------------
    void setFocusControl( GUIElement* element_ );

    /// フォーカスを持っているコントロールを返す (無い場合は Desktop を返す)
    GUIElement* getFocusElement();

    //---------------------------------------------------------------------
    ///**
    //  @brief      マウス位置のコントロールを検索し、保持する
    //  @note
    //              マウス位置のコントロールを mMouseHoverElement に格納する。
    //              mMouseHoverElement が違うコントロールに移った場合、
    //              古い方には LN_GUIEV_MOUSE_LEAVE、
    //              新しい方には LN_GUIEV_MOUSE_ENTER を直ちに送信する。
    //              ※フレーム更新で全要素の update() 後で処理すること。
    //*/
    //---------------------------------------------------------------------
    void updateMouseHover();

    //---------------------------------------------------------------------
    ///**
    //  @brief      全てのコントロールのフレーム更新
    //*/
    //---------------------------------------------------------------------
    void update( const Core::Game::GameTime& game_time_ );

    /// イベント処理
    bool doEvent( const System::Event& e_ );


    void updateDrawContents();
    void render();

public:

    Graphics::Manager* getGraphicsManager() { return mGraphicsManager; }
    Graphics::GeometryRenderer* getGeometryRenderer() { return mGraphicsManager->getGeometryRenderer(); }

private:

    Graphics::Manager*  mGraphicsManager;

    GUIElement*         mFocusElement;          ///< GUIシステム全体でフォーカスを持っているコントロール
    GUIElement*         mMouseHoverElement;     ///< マウスの位置にあるコントロール
    LPoint              mMousePosition;         ///< onEvent() で設定される、変換行列適用済みのマウス位置
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