//=============================================================================
//【 Light 】
//-----------------------------------------------------------------------------
///**
//  @file       Light.h
//  @brief      Light
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Geometry/Geometry.h"
#include "../SceneGraph.h"
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

//=============================================================================
// ■ Light クラス
//-----------------------------------------------------------------------------
///**
//  @brief      ライトを扱うクラス
//*/
//=============================================================================
class Light
    : public SceneNode//Base::ReferenceObject
{
public:
    Light( SceneGraph* scene_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~Light();

public:

    /// 初期化
    LNRESULT initialize();

    /// 種類の取得
    LNLightType getType() const { return mType; }

    /// 種類の設定 (後で initlaize() に移動する)
    void setType( LNLightType type_ ) { mType = type_; }

    /// ライトの有効、無効を判定する
    bool isEnable() const { return mEnable; }

    /// ライトの有効、無効を設定する
    void setEnable( bool f_ ) { mEnable = f_; }

    /// 位置の設定
    //void setPosition( const LVector3& position_ ) { mPosition = position_; mNeedUpdateMatrices = true; }

    /// 位置の取得
    //const LVector3& getPosition() const { return mPosition; }

    /// 向きの設定
    void setDirection( const LVector3& direction_ ) { mDirection = direction_; mNeedUpdateMatrices = true; }

    /// 向きの取得
    const LVector3& getDirection() const { return mDirection; }

    /// ディフューズカラーの設定
    void setDiffuseColor( const LColor& color_ ) { mDiffuse = color_; }

    /// ディフューズカラーの取得
    const LColor& getDiffuseColor() const { return mDiffuse; }

    /// アンビエントカラーの設定
    void setAmbientColor( const LColor& color_ ) { mAmbient = color_; }

    /// アンビエントカラーの取得
    const LColor& getAmbientColor() const { return mAmbient; }

    /// スペキュラカラーの設定
    void setSpecularColor( const LColor& color_ ) { mSpecular = color_; }

    /// スペキュラカラーの取得
    const LColor& getSpecularColor() const { return mSpecular; }

    /// ライティング優先度の設定 (適用するライトの列挙時、距離より優先する値 (大きい方が優先))
    void setLightingPriority( lnS32 priority_ ) { mLightingPriority = priority_; }

    /// ライティング優先度の取得
    lnU32 getLightingPriority() const { return mLightingPriority; }

    /// 有効距離の設定
    void setRange( lnfloat range_ ) { mRange = range_; }
    
    /// 有効距離の取得 (ライト列挙で、ポイント、スポットのときに視錐台に入るかの判定で使う)
    lnfloat getRange() const { return mRange; }


    /// スポットライトの内側コーン角度の設定 (ラジアン単位)
    void setSpotInAngle( lnfloat angle_ ) { mSpotInAngle = angle_; }

    /// スポットライトの内側コーン角度の取得 (ラジアン単位)
    lnfloat getSpotInAngle() const { return mSpotInAngle; }

    /// スポットライトの外側コーン角度の設定 (ラジアン単位)
    void setSpotOutAngle( lnfloat angle_ ) { mSpotOutAngle = angle_; }

    /// スポットライトの外側コーン角度の取得 (ラジアン単位)
    lnfloat getSpotOutAngle() const { return mSpotOutAngle; }

    /// シャドウの暗さの設定 (0.0～1.0)
    //void setShadowStrength( lnfloat strength_ ) { mShadowStrength = strength_; }

    /// シャドウの暗さの取得
    //lnfloat getShadowStrength() const { return mShadowStrength; }

    /// シャドウバッファを作成する
    //LNRESULT createShadowBuffer( lnU32 width_, lnU32 height_ );

    /// シャドウバッファとなるレンダーターゲットの取得
    //Graphics::ITexture* getShadowBuffer() const { return mShadowBuffer; }

    /// シャドウバッファとなるレンダーターゲットの設定 (参照カウントが変化します)
    //void setShadowBuffer( Core::Graphics::ITexture* tareget_ );

    
    /// ビュー行列の取得
    const LMatrix& getViewMatrix() const { _updateMatrices(); return mViewMatrix; }

    /// プロジェクション行列の取得
    const LMatrix& getProjectionMatrix() const { _updateMatrices(); return mProjectionMatrix; }

    /// ビュー行列とプロジェクション行列の積の取得
    const LMatrix& getViewProjectionMatrix() const { _updateMatrices(); return mViewProjMatrix; }

    void updateContext();

    LightContext* getLightContext() { return mLightContext; }

public:

    ///// 現在の状態からビュー行列を作成して out_ に格納する
    //void getViewMatrix( LMatrix* out_, const LVector3& up_ = LVector3( 0, 1, 0 ) );

    ///// 現在の状態からプロジェクション行列を作成して out_ に格納する
    //void getProjectionMatrix( LMatrix* out_, lnfloat fov_y_, lnfloat near_, lnfloat far_ );

    ///// フレーム更新 (SceneBase::update() から呼ばれる)
    //void update();


public:

    /// ノードの種類の取得
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_LIGHT; }

    /// フレーム更新
    virtual void update( const Game::GameTime& time_ ) { }

    /// サブセットを描画する
    virtual LNRESULT drawSubset( lnU32 index_ ) { return LN_OK; }

    /// デバッグ用の描画
    virtual void drawDebug( lnU32 flags_ );

protected:

    /// mNeedUpdateMatrices が true の場合、行列計算を行う
    void _updateMatrices() const;

protected:

    //SceneGraph*             mScene;

    LNLightType             mType;              ///< ライトの種類
    //LVector3                    mPosition;          ///< 位置
    LVector3                mDirection;         ///< 向き

    LColor                  mDiffuse;           ///< ディフューズカラー
    LColor                  mAmbient;           ///< アンビエントカラー
    LColor                  mSpecular;          ///< スペキュラカラー

    lnS32                   mLightingPriority;

    lnfloat                 mRange;             ///< 有効距離
    lnfloat                 mSpotInAngle;       ///< 内側のコーン角度 (ラジアン単位)
    lnfloat                 mSpotOutAngle;      ///< 外側のコーン角度 (ラジアン単位)

    //lnfloat                 mShadowStrength;    ///< シャドウの暗さ (0.0～1.0)
    //Graphics::ITexture*     mShadowBuffer;

    mutable LMatrix         mViewMatrix;        ///< ビュー行列
    mutable LMatrix         mProjectionMatrix;  ///< プロジェクション行列
    mutable LMatrix         mViewProjMatrix;    ///< ビュー行列とプロジェクション行列の積

    mutable LViewFrustum    mViewFrustum;       ///< シャドウバッファを使うときの視錐台
    
    LightContext*           mLightContext;

    bool                    mEnable;            ///< ライトが有効かどうかのフラグ
    mutable bool            mNeedUpdateMatrices;


    /*
    Intensity
	    光の明るさ。スポット/ポイントでは 1、ディレクショナルでは 0.5 デフォ。(unity)
    */

    friend class LightContext;
};

//=============================================================================
// ■ LightContext
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class LightContext
{
public:

    static LightContext DUMMY;

public:

    LightContext();

public:

    void initialize( Light* light_ );

    void updateContext();

public:

    bool isEnable() const { return mEnable; }

    LNLightType getType() const { return mType; }

    const LVector3& getPosition() const { return mPosition; }

    const LVector3& getDirection() const { return mPosition; }

    const LColor&   getDiffuseColor() const { return mDiffuse; }
    const LColor&   getAmbientColor() const { return mAmbient; }
    const LColor&   getSpecularColor() const { return mSpecular; }

    lnS32 getLightingPriority() const { return mLightingPriority; }

    lnfloat getRange() const { return mRange; }

    const LMatrix& getViewMatrix() const { return mViewMatrix; }

    const LMatrix& getViewProjectionMatrix() const { return mViewProjMatrix; }

    // ソート用の一時データ
    lnfloat DistanceForSort;

private:

    Light*          mLight;
    LNLightType     mType;
    LVector3        mPosition;
    LVector3        mDirection;
    LColor          mDiffuse;
    LColor          mAmbient;
    LColor          mSpecular;
    lnS32           mLightingPriority;
    lnfloat         mRange;
    lnfloat         mSpotInAngle;
    lnfloat         mSpotOutAngle;
    LMatrix         mViewMatrix;
    LMatrix         mProjectionMatrix;
    LMatrix         mViewProjMatrix;
    bool            mEnable;
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