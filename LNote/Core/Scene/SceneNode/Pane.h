//=============================================================================
//【 Pane 】
//-----------------------------------------------------------------------------
///**
//  @file       Pane.h
//  @brief      Pane
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Common/ScreenEffect.h"
#include "../../GUI/Controls/GUIDesktop.h"
#include "SceneNode.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
class PanePostDrawContext;

//=============================================================================
// ■ Pane クラス
//-----------------------------------------------------------------------------
///**
//  @brief		
//
//  @note
//              3D、2D、GUI をまとめる。
//              [12/12/12] SceneGraph::makeDrawCommand() にルートだけ渡せればいろいろ楽なので Parent - Children 化。
//*/
//=============================================================================
class Pane
    : public Base::ReferenceObject
    //: public SceneNode
{
public:

    /// コンストラクタ
    Pane( SceneGraph* scene_ );

    /// デストラクタ
    virtual ~Pane();

public:

    /// 初期化
    void initialize( const Geometry::Size& size_ );//, bool is_3d_ );

	void setViewSize( const Geometry::Size& size );

	const LVector2& getViewSize() const { return mViewSize; }

    void setCamera( Camera* camera_3d_, Camera* camera_2d_ );

    void getCamera( Camera** camera_3d_, Camera** camera_2d_ ) { *camera_3d_ = mCamera3D; *camera_2d_ = mCamera2D; }

    void setRootNode( SceneNode* root_3d_, SceneNode* root_2d_ );

    void getRootNode( SceneNode** root_3d_, SceneNode** root_2d_ );

    /// (参照カウント++)
    void setGUIDesktop( GUI::GUIDesktop* desktop_ );

    /// 特殊効果の使用設定
    void setEffectMode( LNScreenEffectMode mode_ );

    void addChildPane( Pane* pane_ );

    void removeChildPane( Pane* pane_ );

	/// 背景色の設定 (特殊効果有効時に使用する。LColor::TRANSPARENCY を設定するとクリアしない)
	void setBackgroundColor( const LColor& color_ ) { mBackgroundColor = color_; }

public:

    /// 色調の設定
    void setTone( const LTone& tone_, double duration_ ) { mScreenEffect->setTone( tone_, duration_ ); }

    /// トランジションの準備として現在の画面を固定する
    void freeze() { mScreenEffect->freeze(); }

    /// トランジション実行
    void transition( double duration_, Graphics::ITexture* mask_, int vague_ = 40 ) { mScreenEffect->transition( duration_, mask_, vague_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      ブラー実行
    //
    //  @param[in]  duration_     : 適用する時間 (秒)
    //  @param[in]  power_        : ブラーの強さ (ブラーイメージを描画する時のアルファ値。0.0～1.0 で指定)
    //  @param[in]  scale_        : ブラーの大きさ (長さ)
    //  @param[in]  center_       : ブラーの中心座標
    //  @param[in]  center_is_3d_ : center_ の座標情報が示す座標空間 (true の場合、3D 空間上の点)
    // 
    //  @par
    //              
    //              duration_ が 0 より大きい場合、値分の時間をかけて<br>
    //              徐々に power_ を 0 に戻します。
    //              <br>
    //              duration_ が 0 の場合は強さを自動で変化させません。<br>
    //              元に戻すためには、もう一度この関数を呼んで power_ を 0 にする必要があります。<br>
    //              <br>
    //              <br>
    //              scale_ は元のスクリーンイメージに対して、1フレーム当たりどの程度拡大するかを指定します。<br>
    //              1.0 でスクリーンと等倍 (変化なし)、2.0 でスクリーンの2倍の大きさとなります。<br>
    //              scale_ が 1.0 の場合、ブラーは放射状に拡散しません。<br>
    //              通常は 1.0～1.1 の範囲で指定するのが無難です。<br>
    //              また、1.0 より小さい値を指定すると内側向きのブラーになります。<br>
    //              <br>
    //              <br>
    //              center_is_3d_ は、center_ が 2D 空間上の座標か、3D 空間上の座標かを区別するために指定します。<br>
    //              <br>
    //              false が指定されている場合、center_ の値がそのままブラーの中心座標になります。<br>
    //              中心座標はレイヤーの左上を原点(0, 0)として、ピクセル単位で指定します。<br>
    //              <br>
    //              true が指定されている場合、center_ を3D空間上の点として、このレイヤーにセットされているカメラから見た
    //              スクリーン座標上の点を中心とします。<br>
    //              <br>    
    //*/
    //---------------------------------------------------------------------
    void blur(
        double duration_,
        lnfloat power_,
        lnfloat scale_,
        const LVector3& center_,
        bool center_is_3d_ );

    /// フレーム更新 (トランジション、ブラ―)
    void update( const Game::GameTime& game_time_ );

    /// ブラー適用中かを判定する
    bool isBlurEffect() const { return mScreenEffect->isBlurEffect(); }

    /// エフェクト適用中かを判定する
    bool isEffect() const { return mScreenEffect->isEffect(); }

public:

    void makeDrawCommand( DrawCommandContainer* c_ );

    /// (SceneGraph::update() から呼ばれる)
    void updateContext();

    PaneContext* getPaneContext() { return mPaneContext; }

    Graphics::ScreenEffect* getScreenEffectRenderer() { return mScreenEffect; }

	bool onEvent( const System::Event& e );

private:

	/// 特殊効果リソース作成
    void _createEffectResource();

    /// 特殊効果リソース解放
    void _disposeEffectResource();

private:

    SceneGraph*				mSceneGraph;
    LVector2				mViewSize;
    Camera*					mCamera3D;
    Camera*					mCamera2D;
    SceneNode*				mRootNode3D;
    SceneNode*				mRootNode2D;
    GUI::GUIDesktop*		mGUIDesktop;

    PaneArray				mChildPaneArray;
    PaneContext*			mPaneContext;
	PanePostDrawContext*	mPanePostDrawContext;

    Graphics::ScreenEffect* mScreenEffect;
    LColor				    mBackgroundColor;
    LNScreenEffectMode      mEffectMode;

    friend class PaneContext;
	friend class PanePostDrawContext;
};

//=============================================================================
// ■ PaneContext
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class PaneContext
    : public DrawingContext
{
public:

    PaneContext();
    ~PaneContext();
    
public:

    void initialize( Pane* pane_ );

    void updateContext();

    const LVector2& getViewSize() const { return mViewSize; }

    void getCameraContext( CameraContext** camera_context_3d_, CameraContext** camera_context_2d_ );

public:

    virtual void draw(
        Graphics::IRenderer* renderer_,
        Graphics::GeometryRenderer* geometry_renderer_ );

private:

    Pane*               mPane;
    LVector2            mViewSize;
    Camera*             mCamera3D;
    Camera*             mCamera2D;
    GUI::GUIDesktop*    mGUIDesktop;
    LNScreenEffectMode  mEffectMode;
};

//=============================================================================
// ■ PanePostDrawContext
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class PanePostDrawContext
    : public DrawingContext
{
public:

    PanePostDrawContext();
    ~PanePostDrawContext();
    
public:

    void initialize( Pane* pane_ );

    void updateContext();

public:

    virtual void draw(
        Graphics::IRenderer* renderer_,
        Graphics::GeometryRenderer* geometry_renderer_ );

private:

    Pane*   mPane;

    // ブラ―の後に描画を設けないと、「画面全体への効果」はできない。
    //  →ブラ―は「直前の画面全体」を「半透明」で描画するため。
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================