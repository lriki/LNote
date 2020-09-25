//=============================================================================
//【 GUIControl 】
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
// ■ GUIControl
//-----------------------------------------------------------------------------
///**
//  @brief      
//  @note
//              InternalChildren の要素はフォーカスに関する処理は行わない。
//*/
//=============================================================================
class GUIControl
    : public GUIElement
{
public:

    GUIControl( Manager* manager_ );
    virtual ~GUIControl();

public:

    /// 初期化
    void initialize();

    /// コントロールを子として追加する (参照カウント++)
    void addControl( GUIElement* element_ );

    /// 指定されたコントロールが子の場合、このコンテナから外す (必要があれば GUIElement のデストラクタから呼ばれる)
    void removeControl( GUIElement* element_ );

    /// child_element_ を子コントロール内アクティブにする (タブオーダーを持たせ、element_ の Z オーダーを一番手前にする。NULLで解除)
    void setActiveElement( GUIElement* child_element_ );

    /// このコンテナ内の子コントロールのうち、アクティブなものを返す
    GUIElement* getActiveElement();

public:

    virtual void setDataContext( INotifyPropertyChanged* object_ );

    virtual void initialize( GUIElement::Template* data_ );

    virtual GUIElement* getMouseHoverElement( const LPoint& mouse_pos );

    /// 位置情報の更新
    virtual void updateCoordinate( const LRect& parent_client_rect_ );

    /// 描画情報の作成
    virtual void updateDrawContents();

    /// 描画
    virtual void draw();

protected:

    void addInternalElement( GUIElement* element_ );

    void removeInternalElement( GUIElement* element_ );

private:

    GUIElementArray     mChildList;
    GUIElementArray	    mTabOrderArray;		    ///< タブオーダー
	s32				    mActiveTabOrderIndex;   ///< アクティブコントロールの mTabOrderArray 内でのインデックス
    LRect               mGlobalClientRect;      ///< グローバル座標系変換済みのクライアント領域
    LRect               mScissorRect;           ///< 描画に使うシザー領域
    GUIElementArray     mInternalChildren;      ///< 基本的に描画要素を格納する (clearAllControls の影響を受けないようにするため)

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