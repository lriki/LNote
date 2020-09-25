//=============================================================================
//【 GUIElement 】
//-----------------------------------------------------------------------------
///**
//  @file       GUIElement.h
//  @brief      GUIElement
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
// ■ GUIElement
//-----------------------------------------------------------------------------
///**
//  @brief      
//  @note
//              Rect と Margin について…
//                  Rect はローカル内での座標&サイズ。
//                  親コントロールのサイズはMargin によって描画領域が
//                  Rect サイズよりも小さくなっても、Rect サイズは維持される。
//                  Margin は水平、垂直アラインのためのオフセット。
//*/
//=============================================================================
class GUIElement
    : public Base::ReferenceObject
    , public BindingTargetElement
{
public:

    struct Template;

public:

    GUIElement( Manager* manager_ );

    virtual ~GUIElement();

public:

    /// 初期化
    void initialize();
    Manager* getManager() const { return mManager; }

    void                setRect( const LRect& rect_ );
    const LRect&        getRect() const { return mElementRect; }

    void                setMargin( const LThickness& margin_ ) { mMargin = margin_; }
    const LThickness&   getMargin() const { return mMargin; }

    void                setVisible( bool flag_ ) { mVisible = flag_; }
    bool                isVisible() const { return mVisible; }

    void                setEnable( bool flag_ ) { mEnable = flag_; }
    bool                isEnable() const { return mEnable; }

    bool                isActive() const;   ///< 同一階層の子要素内でアクティブかを判定 (タブオーダーを持っているか、またはフォーカスを持っているか)
    bool                hasFocus() const;

    void                focus();

    virtual void        setDataContext( INotifyPropertyChanged* object_ );
    INotifyPropertyChanged* getDataContext() const { return mDataContext; }

public:

    /// テンプレートから生成
    virtual void initialize( Template* data_ );

    /// 位置情報の更新 (parent_client_rect_ はグローバル座標)
    virtual void updateCoordinate( const LRect& parent_client_rect_ );

    /// マウス位置との当たり判定に使用するコライダの更新 (デフォルトでは mBoundingRect を更新)
    virtual void updateCollider();

    /// マウス位置と重なっているかを判定する (mouse_pos_ はグローバル座標)
    virtual bool checkMouseHover( const LPoint& mouse_pos );

    /// マウス位置と重なっているかを判定する (子も含めて、重なっているコントロールを返す)
    ///     ここでは checkMouseHover() が true を返す場合、this を返している。
    ///     これは GUIControl での再帰的な処理をシンプルに書くための措置。
    virtual GUIElement* getMouseHoverElement( const LPoint& mouse_pos );

    /// フォーカスを持てるコントロールの場合は true を返す (サブクラスでオーバーライド)
    virtual bool canFocus() const { return false; }

    /// イベント処理 (イベントを処理したことを示すには true を返す)
    virtual bool doEvent( const Event& e_ ) { return false; }

    /// アクティブ状態のチェック開始 (Manager::setFocusControl() から呼ばれる)
    virtual void beginCheckActivate();

    /// アクティブ状態のチェック終了 (Manager::setFocusControl() から呼ばれる)
    virtual void endCheckActivate( bool active_ );

    /// 描画情報の作成
    virtual void updateDrawContents() {}

    /// 描画
    virtual void draw() {}

public:

    virtual void onContextPropertyChanged( const lnChar* property_name_ ) {}

protected:

    Manager*                mManager;
    GUIControl*             mParent;
    INotifyPropertyChanged* mDataContext;
    LRect                   mElementRect;
    LThickness              mMargin;
    LPoint                  mGlobalPosition;    ///< グローバル座標 (updateCoordinate() で作成される)
    LRect                   mBoundingRect;      ///< マウスとの当たり判定に使う矩形 (グローバル空間)
    Graphics::ITexture*     mSkinTexture;
    bool                    mVisible;
    bool                    mEnable;
    bool                    mTempActive;
    bool                    mIsInternalElement;

    friend class GUIControl;

public:

    /// どっちかっていうと style?
    struct Template
    {
        ElementType         Type;           ///< 復元するクラスの判定に使用する。継承先で設定すること。

        LRect               ElementRect;
        LThickness          Margin;
        Graphics::ITexture* SkinTexture;    // 名前の方がいいかも

        TemplateBindableValue   Width;
        TemplateBindableValue   Height;
        TemplateBindableValue   _Margin;
        TemplateBindableValue   _SkinTexture;

        Template()
        {
            Type = ELEMENTTYPE_UNKNOWN,
            ElementRect.set( 0, 0, 32, 32 );
            Margin.set( 0, 0, 0, 0 );
            SkinTexture = NULL;

            Width.initialize( 64 );
            Height.initialize( 32 );
            _Margin.initialize( LVector4( 0, 0, 0, 0 ) );
            _SkinTexture.initialize( _T("") );
        }


        virtual void serialize( const LRefTString& attr, const LRefTString& value )
        {
            
        }
    };

    typedef std::vector< Template* > TemplateArray;
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