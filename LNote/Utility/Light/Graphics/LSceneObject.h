//=============================================================================
//【 LSceneObject3D 】
//-----------------------------------------------------------------------------
///**
//  @file       LSceneObject3D.h
//  @brief      LSceneObject3D
//  @version    1.0
//  @date       2010/10/06
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Core/Scene/Manager.h"
#include "../Math/LMath.h"
#include "Types.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// ■ LSceneObject3D クラス
//-----------------------------------------------------------------------------
///**
//  @brief     3D 空間上のオブジェクトの基底クラス
//*/
//=============================================================================
class LSceneObject3D
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      座標の設定
    //
    //  @param[in]  pos_ : 設定する座標
    //*/
    //---------------------------------------------------------------------
    void setPosition( const LVector3& pos_ ) { _lnso->setPosition( pos_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      座標の設定
    //
    //  @param[in]  x_ : 設定する X 座標
    //  @param[in]  y_ : 設定する Y 座標
    //  @param[in]  z_ : 設定する Z 座標
    //*/
    //---------------------------------------------------------------------
    void setPosition( lnfloat x_, lnfloat y_, lnfloat z_ ) { _lnso->setPosition( LVector3( x_, y_, z_ ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      座標の取得
    //*/
    //---------------------------------------------------------------------
    const LVector3& getPosition() const { return _lnso->getPosition(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      回転角度の設定 (ラジアン単位)
    //
    //  @param[in]  angle_ : 各軸の回転角度が設定されたベクトル
    //*/
    //---------------------------------------------------------------------
    void setAngle( const LVector3& angle_ ) { _lnso->setAngle( angle_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      回転角度の設定 (ラジアン単位)
    //
    //  @param[in]  x_ : X 軸周りの回転値
    //  @param[in]  y_ : Y 軸周りの回転値
    //  @param[in]  z_ : Z 軸周りの回転値
    //*/
    //---------------------------------------------------------------------
    void setAngle( lnfloat x_, lnfloat y_, lnfloat z_ ) { _lnso->setAngle( LVector3( x_, y_, z_ ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      回転角度の取得 (ラジアン単位)
    //*/
    //---------------------------------------------------------------------
    const LVector3& getAngle() const { return _lnso->getAngle(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      拡大率の設定
    //
    //  @param[in]  scale_ : 各軸方向の拡大率が格納されたベクトル
    //*/
    //---------------------------------------------------------------------
    void setScale( const LVector3& scale_ ) { _lnso->setScale( scale_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      拡大率の設定
    //
    //  @param[in]  x_ : X 方向の拡大率
    //  @param[in]  y_ : Y 方向の拡大率
    //  @param[in]  z_ : Z 方向の拡大率
    //*/
    //---------------------------------------------------------------------
    void setScale( lnfloat x_, lnfloat y_, lnfloat z_ ) { _lnso->setScale( LVector3( x_, y_, z_ ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      拡大率の設定
    //
    //  @param[in]  xyz_ : すべての軸方向に設定される拡大率
    //*/
    //---------------------------------------------------------------------
    void setScale( lnfloat xyz_ ) { _lnso->setScale( LVector3( xyz_, xyz_, xyz_ ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      拡大率の取得
    //*/
    //---------------------------------------------------------------------
    const LVector3& getScale() const { return _lnso->getScale(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      原点の設定
    //
    //  @param[in]  center_ : 設定する座標
    //
    //  @par
    //              オブジェクトの原点を、オブジェクトのローカル座標系内で指定します。<br>
    //              例えば、サイズ (10, 20) のパネルオブジェクト(Z- が正面)の左上を原点に
    //              したい場合は setCenter() に (-5, 10, 0) を設定します。
    //*/
    //---------------------------------------------------------------------
    void setCenter( const LVector3& center_ ) { _lnso->setCenter( center_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      原点の設定
    //
    //  @param[in]  x_ : 設定する X 座標
    //  @param[in]  y_ : 設定する Y 座標
    //  @param[in]  z_ : 設定する Z 座標
    //*/
    //---------------------------------------------------------------------
    void setCenter( lnfloat x_, lnfloat y_, lnfloat z_ ) { _lnso->setCenter( LVector3( x_, y_, z_ ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      原点の取得
    //*/
    //---------------------------------------------------------------------
    const LVector3& getCenter() const { return _lnso->getCenter(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      行列の設定
    //
    //  @param[in]  matrix_ : 設定する行列
    //
    //  @par
    //              内部では描画の直前に setPosition() setAngle() setScale() setCenter()
    //              で設定された値から行列を計算していますが、<br>
    //              この計算を行わず、自分自身で作成した行列で座標変換を
    //              行いたい場合に、この関数を使います。<br>
    //
    //  @attention
    //              一度単位行列以外の行列を設定すると、
    //              setPosition() setAngle() setScale() setCenter() が全て無効になり、
    //              この関数で設定された行列のみを使って座標変換を行います。<br>
    //              <br>
    //              解除する場合は、この関数に単位行列を渡してください。
    //*/
    //---------------------------------------------------------------------
    void setMatrix( const LMatrix& matrix_ ) { _lnso->setMatrix( matrix_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      行列の取得
    //
    //  @attention
    //              setPosition() setAngle() setScale() で設定した値からも
    //              行列は作成されますが、その場合は常に正しい値を返すとは限りません。<br>
    //              setMatrix() で設定した行列を取得するためだけに使うことをお勧めします。<br>
    //              <br>
    //              <br>
    //              また、setEnableBillboard() によってビルボード化されている場合、
    //              getMatrix() の使用は推奨しません。<br>
    //              これは、内部でのビルボード行列の計算の都合で
    //              意図しない値が返ることがあるためです。<br>
    //              <br>
    //              ビルボード化されている場合は、
    //              カメラに対して正面を向いた行列が返されます。<br>
    //              ただし、内部でのビュー行列との計算タイミングの関係で、
    //              一度画面を描画した後でなければ正常な値は取得できません。(描画は正常に行われます)
    //              また、複数のカメラを使っている場合は、最後に描画したときに
    //              基準となったカメラを向いた行列が返されます。
    //*/
    //---------------------------------------------------------------------
    const LMatrix& getMatrix() const { return _lnso->getMatrix(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      可視状態の設定
    //
    //  @param[in]  flag_ : 可視状態 (true:表示する、false:表示しない)
    //*/
    //---------------------------------------------------------------------
    void setVisible( bool flag_ ) { _lnso->setVisible( flag_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      可視状態の判定
    //
    //  @retval     true  : 表示中 (描画する)
    //  @retval     false : 非表示 (描画しない)
    //*/
    //---------------------------------------------------------------------
    bool isVisible() const { return _lnso->isVisible(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      不透明度の設定
    //
    //  @param[in]  opacity_ : 不透明度 (0.0〜1.0)
    //*/
    //---------------------------------------------------------------------
    void setOpacity( lnfloat opacity_ ) { _lnso->setOpacity( opacity_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      不透明度の取得
    //*/
    //---------------------------------------------------------------------
    lnfloat getOpacity() const { return _lnso->getOpacity(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      乗算色の設定
    //
    //  @param[in]  color_ : 乗算する色
    //
    //  @par
    //              描画を行う際、最終的に出力される色に乗算する値を設定します。<br>
    //              簡単な色違い表現などに使うことができます。<br>
    //              <br>
    //              乗算色のアルファ値は不透明度の値とは別に管理され、
    //              シェーダプログラム内で乗算されます。<br>
    //              例えば、乗算色のアルファ値と不透明度のどちらかが 0.0 の場合、
    //              なにも描画されなくなります。
    //*/
    //---------------------------------------------------------------------
    void setColorScale( const LColor& color_ ) { _lnso->setColorScale( color_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      乗算色の設定
    //
    //  @param[in]  r_ : R (赤) 成分 (0.0 〜 1.0)
    //  @param[in]  g_ : G (緑) 成分 (0.0 〜 1.0)
    //  @param[in]  b_ : B (青) 成分 (0.0 〜 1.0)
    //  @param[in]  a_ : A (アルファ) 成分 (0.0 〜 1.0)
    //
    //  @par
    //              描画を行う際、最終的に出力される色に乗算する値を設定します。<br>
    //              簡単な色違い表現などに使うことができます。<br>
    //              <br>
    //              乗算色のアルファ値は不透明度の値とは別に管理され、
    //              シェーダプログラム内で乗算されます。<br>
    //              例えば、乗算色のアルファ値と不透明度のどちらかが 0.0 の場合、
    //              なにも描画されなくなります。
    //*/
    //---------------------------------------------------------------------
    void setColorScale( lnfloat r_, lnfloat g_, lnfloat b_, lnfloat a_ = 1.0f ) { _lnso->setColorScale( LColor( r_, g_, b_, a_ ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      乗算色の取得
    //*/
    //---------------------------------------------------------------------
    const LColor& getColorScale() const { return _lnso->getColorScale(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      ブレンドカラーの設定
    //
    //  @param[in]  color_ : ブレンド色する色
    //
    //  @par
    //              ブレンドカラーは、描画するオブジェクトを塗りつぶす色です。<br>
    //              アルファ値がブレンドの強さとなり、
    //              1.0 のときに完全に設定した色で塗りつぶされます。<br>
    //              <br>
    //              ヒットエフェクト時のオブジェクトの発光等に使うことができます。
    //*/
    //---------------------------------------------------------------------
    void setBlendColor( const LColor& color_ ) { _lnso->setBlendColor( color_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      ブレンドカラーの設定
    //
    //  @param[in]  r_ : R (赤) 成分 (0.0 〜 1.0)
    //  @param[in]  g_ : G (緑) 成分 (0.0 〜 1.0)
    //  @param[in]  b_ : B (青) 成分 (0.0 〜 1.0)
    //  @param[in]  a_ : ブレンド量 (0.0 〜 1.0)
    //
    //  @par
    //              ブレンドカラーは、描画するオブジェクトを塗りつぶす色です。<br>
    //              アルファ値がブレンドの強さとなり、
    //              1.0 のときに完全に設定した色で塗りつぶされます。<br>
    //              <br>
    //              ヒットエフェクト時のオブジェクトの発光等に使うことができます。
    //*/
    //---------------------------------------------------------------------
    void setBlendColor( lnfloat r_, lnfloat g_, lnfloat b_, lnfloat a_ ) { _lnso->setBlendColor( LColor( r_, g_, b_, a_ ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      ブレンドカラーの取得
    //*/
    //---------------------------------------------------------------------
    const LColor& getBlendColor() const { return _lnso->getBlendColor(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      色調の設定
    //
    //  @param[in]  tone_ : 色調
    //*/
    //---------------------------------------------------------------------
    void setTone( const LTone& tone_ ) { _lnso->setTone( tone_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      色調の設定
    //
    //  @param[in]  r_    : R (赤) 成分 (-1.0 〜 1.0)
    //  @param[in]  g_    : G (緑) 成分 (-1.0 〜 1.0)
    //  @param[in]  b_    : B (青) 成分 (-1.0 〜 1.0)
    //  @param[in]  gray_ : 単色化の強さ (0.0 〜 1.0)
    //*/
    //---------------------------------------------------------------------
    void setTone( lnfloat r_, lnfloat g_, lnfloat b_, lnfloat gray_ ) { _lnso->setTone( LTone( r_, g_, b_, gray_ ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      色調の取得
    //*/
    //---------------------------------------------------------------------
    const LTone& getTone() const { return _lnso->getTone(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      合成方法の設定
    //
    //  @param[in]  mode_ : 合成方法
    //*/
    //---------------------------------------------------------------------
    void setBlendMode( LNBlendMode mode_ ) { _lnso->setBlendMode( mode_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      合成方法の取得
    //*/
    //---------------------------------------------------------------------
    LNBlendMode getBlendMode() const { return _lnso->getBlendMode(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      深度テストの有効設定
    //
    //  @param[in]  flag_ : 有効フラグ (true:深度テストを行う false:行わない)
    //*/
    //---------------------------------------------------------------------
    void setEnableDepthTest( bool flag_ ) { _lnso->setEnableDepthTest( flag_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      深度テストの有効判定
    //*/
    //---------------------------------------------------------------------
    bool isEnableDepthTest() const { return _lnso->isEnableDepthTest(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      深度書き込みの有効設定
    //
    //  @param[in]  flag_ : 有効フラグ (true:深度Writeを行う false:行わない)
    //*/
    //---------------------------------------------------------------------
    void setEnableDepthWrite( bool flag_ ) { _lnso->setEnableDepthWrite( flag_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      深度書き込みの有効判定
    //*/
    //---------------------------------------------------------------------
    bool isEnableDepthWritet() const { return _lnso->isEnableDepthWritet(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      カリング方法の設定
    //
    //  @param[in]  mode_ : カリング方法
    //*/
    //---------------------------------------------------------------------
    void setCullingMode( LNCullingMode mode_ ) { _lnso->setCullingMode( mode_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      カリング方法の取得
    //*/
    //---------------------------------------------------------------------
    LNCullingMode getCullingMode() const { return _lnso->getCullingMode(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      ビルボード化の有効設定
    //
    //  @par
    //              オブジェクトが常にカメラに対して正面を向くようにします。<br>
    //              setMatrix() で得られる行列も変化するため、
    //              使う場合は注意してください。
    //*/
    //---------------------------------------------------------------------
    void setBillboardType( LNBillboardType flag_ ) { _lnso->setBillboardType( flag_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      ビルボード化の有効判定
    //*/
    //---------------------------------------------------------------------
    LNBillboardType getBillboardType() const { return _lnso->getBillboardType(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      自動更新の有効設定
    //
    //  @par
    //              パーティクルやスキンメッシュ等の、フレーム更新処理に
    //              経過時間を指定する必要のあるオブジェクトを、
    //              自動的に時間を与えて更新するかどうかの設定です。
    //*/
    //---------------------------------------------------------------------
    void setEnableAutoUpdate( bool flag_ ) { _lnso->setEnableAutoUpdate( flag_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      自動更新の有効判定
    //*/
    //---------------------------------------------------------------------
    bool isEnableAutoUpdate() const { return _lnso->isEnableAutoUpdate(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      回転順序の設定
    //
    //  @param[in]  order_ : 回転順序
    //
    //  @par
    //              setAngle() で設定された値を使って回転を行う際の、
    //              軸の回転順序を設定します。
    //*/
    //---------------------------------------------------------------------
    void setRotateOrder( LNRotOrder order_ ) { _lnso->setRotateOrder( order_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      回転順序の取得
    //*/
    //---------------------------------------------------------------------
    LNRotOrder getRotateOrder() const { return _lnso->getRotateOrder(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      優先度の設定
    //
    //  @param[in]  priority_ : 優先度
    //
    //  @par
    //              描画時のソートの基準となる優先度を設定します。
    //              値が大きい方が後から描画されるようになります。
    //              この値はZソートよりも優先されます。
    //*/
    //---------------------------------------------------------------------
    void setPriority( s32 priority_ ) { _lnso->setPriority( priority_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      優先度の取得
    //*/
    //---------------------------------------------------------------------
    s32 getPriority() const { return _lnso->getPriority(); }

protected:

    Core::Scene::SceneNode*    _lnso;
};

//=============================================================================
// ■ LSceneObject2D クラス
//-----------------------------------------------------------------------------
///**
//  @brief     2D 空間上のオブジェクトの基底クラス
//*/
//=============================================================================
class LSceneObject2D
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      座標の設定
    //
    //  @param[in]  pos_ : 設定する座標
    //
    //  @par
    //              座標の Z 値は前後関係を表します。0.0 〜 10000.0 の間で指定します。<br>
    //              0.0 が最も手前となります。
    //*/
    //---------------------------------------------------------------------
    void setPosition( const LVector3& pos_ ) { _lnso->setPosition( pos_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      座標の設定
    //
    //  @param[in]  x_ : 設定する X 座標
    //  @param[in]  y_ : 設定する Y 座標
    //
    //  @par
    //              この関数では Z 座標は設定しません。
    //*/
    //---------------------------------------------------------------------
    void setPosition( lnfloat x_, lnfloat y_ ) { _lnso->setPosition( LVector3( x_, y_, _lnso->getPosition().z ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      座標の設定
    //
    //  @param[in]  x_ : 設定する X 座標
    //  @param[in]  y_ : 設定する Y 座標
    //  @param[in]  z_ : 設定する Z 座標
    //
    //  @par
    //              座標の Z 値は前後関係を表します。0.0 〜 10000.0 の間で指定します。<br>
    //              0.0 が最も手前となります。
    //*/
    //---------------------------------------------------------------------
    void setPosition( lnfloat x_, lnfloat y_, lnfloat z_ ) { _lnso->setPosition( LVector3( x_, y_, z_ ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      座標の取得
    //*/
    //---------------------------------------------------------------------
    const LVector3& getPosition() const { return _lnso->getPosition(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      回転角度の設定 (ラジアン単位)
    //
    //  @param[in]  r_ : 回転角度
    //
    //  @par
    //              setCenter() で設定した軸を中心に、時計回りに回転します。
    //*/
    //---------------------------------------------------------------------
    void setAngle( lnfloat r_ ) { _lnso->setAngle( LVector3( 0, 0, r_ ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      回転角度の取得 (ラジアン単位)
    //*/
    //---------------------------------------------------------------------
    const lnfloat getAngle() const { return _lnso->getAngle().z; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      拡大率の設定
    //
    //  @param[in]  scale_ : 各軸方向の拡大率が格納されたベクトル
    //*/
    //---------------------------------------------------------------------
    void setScale( const LVector2& scale_ ) { _lnso->setScale( LVector3( scale_, 1.0f ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      拡大率の設定
    //
    //  @param[in]  x_ : X 方向の拡大率
    //  @param[in]  y_ : Y 方向の拡大率
    //*/
    //---------------------------------------------------------------------
    void setScale( lnfloat x_, lnfloat y_ ) { _lnso->setScale( LVector3( x_, y_, 1.0f ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      拡大率の設定
    //
    //  @param[in]  xy_ : XY両方に設定される拡大率
    //*/
    //---------------------------------------------------------------------
    void setScale( lnfloat xy_ ) { _lnso->setScale( LVector3( xy_, xy_, 1.0f ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      拡大率の取得
    //*/
    //---------------------------------------------------------------------
    LVector2 getScale() const { return LVector2( _lnso->getScale().getXY() ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      原点の設定
    //
    //  @param[in]  center_ : 設定する座標
    //
    //  @par
    //              オブジェクトの原点を、オブジェクトのローカル座標系内で指定します。<br>
    //              例えば、サイズ (10, 20) のスプライトの中心を原点に
    //              したい場合は setCenter() に (5, 10) を設定します。
    //*/
    //---------------------------------------------------------------------
    void setCenter( const LVector2& center_ ) { _lnso->setCenter( LVector3( center_, 0 ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      原点の設定
    //
    //  @param[in]  x_ : 設定する X 座標
    //  @param[in]  y_ : 設定する Y 座標
    //*/
    //---------------------------------------------------------------------
    void setCenter( lnfloat x_, lnfloat y_ ) { _lnso->setCenter( LVector3( x_, y_, 0 ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      原点の取得
    //*/
    //---------------------------------------------------------------------
    LVector2 getCenter() const { return LVector2( _lnso->getCenter().getXY() ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      行列の設定
    //
    //  @param[in]  matrix_ : 設定する行列
    //
    //  @par
    //              内部では描画の直前に setPosition() setAngle() setScale() setCenter()
    //              で設定された値から行列を計算していますが、<br>
    //              この計算を行わず、自分自身で作成した行列で座標変換を
    //              行いたい場合に、この関数を使います。<br>
    //
    //  @attention
    //              一度単位行列以外の行列を設定すると、
    //              setPosition() setAngle() setScale() setCenter() が全て無効になり、
    //              この関数で設定された行列のみを使って座標変換を行います。<br>
    //              <br>
    //              解除する場合は、この関数に単位行列を渡してください。
    //*/
    //---------------------------------------------------------------------
    void setMatrix( const LMatrix& matrix_ ) { _lnso->setMatrix( matrix_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      行列の取得
    //
    //  @attention
    //              setPosition() setAngle() setScale() で設定した値からも
    //              行列は作成されますが、その場合は常に正しい値を返すとは限りません。<br>
    //              setMatrix() で設定した行列を取得するためだけに使うことをお勧めします。<br>
    //*/
    //---------------------------------------------------------------------
    const LMatrix& getMatrix() const { return _lnso->getMatrix(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      可視状態の設定
    //
    //  @param[in]  flag_ : 可視状態 (true:表示する、false:表示しない)
    //*/
    //---------------------------------------------------------------------
    void setVisible( bool flag_ ) { _lnso->setVisible( flag_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      可視状態の判定
    //
    //  @retval     true  : 表示中 (描画する)
    //  @retval     false : 非表示 (描画しない)
    //*/
    //---------------------------------------------------------------------
    bool isVisible() const { return _lnso->isVisible(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      不透明度の設定
    //
    //  @param[in]  opacity_ : 不透明度 (0.0〜1.0)
    //*/
    //---------------------------------------------------------------------
    void setOpacity( lnfloat opacity_ ) { _lnso->setOpacity( opacity_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      不透明度の取得
    //*/
    //---------------------------------------------------------------------
    lnfloat getOpacity() const { return _lnso->getOpacity(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      乗算色の設定
    //
    //  @param[in]  color_ : 乗算する色
    //
    //  @par
    //              描画を行う際、最終的に出力される色に乗算する値を設定します。<br>
    //              簡単な色違い表現などに使うことができます。<br>
    //              <br>
    //              乗算色のアルファ値は不透明度の値とは別に管理され、
    //              シェーダプログラム内で乗算されます。<br>
    //              例えば、乗算色のアルファ値と不透明度のどちらかが 0.0 の場合、
    //              なにも描画されなくなります。
    //*/
    //---------------------------------------------------------------------
    void setColorScale( const LColor& color_ ) { _lnso->setColorScale( color_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      乗算色の設定
    //
    //  @param[in]  r_ : R (赤) 成分 (0.0 〜 1.0)
    //  @param[in]  g_ : G (緑) 成分 (0.0 〜 1.0)
    //  @param[in]  b_ : B (青) 成分 (0.0 〜 1.0)
    //  @param[in]  a_ : A (アルファ) 成分 (0.0 〜 1.0)
    //
    //  @par
    //              描画を行う際、最終的に出力される色に乗算する値を設定します。<br>
    //              簡単な色違い表現などに使うことができます。<br>
    //              <br>
    //              乗算色のアルファ値は不透明度の値とは別に管理され、
    //              シェーダプログラム内で乗算されます。<br>
    //              例えば、乗算色のアルファ値と不透明度のどちらかが 0.0 の場合、
    //              なにも描画されなくなります。
    //*/
    //---------------------------------------------------------------------
    void setColorScale( lnfloat r_, lnfloat g_, lnfloat b_, lnfloat a_ = 1.0f ) { _lnso->setColorScale( LColor( r_, g_, b_, a_ ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      乗算色の取得
    //*/
    //---------------------------------------------------------------------
    const LColor& getColorScale() const { return _lnso->getColorScale(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      ブレンドカラーの設定
    //
    //  @param[in]  color_ : ブレンド色する色
    //
    //  @par
    //              ブレンドカラーは、描画するオブジェクトを塗りつぶす色です。<br>
    //              アルファ値がブレンドの強さとなり、
    //              1.0 のときに完全に設定した色で塗りつぶされます。<br>
    //              <br>
    //              ヒットエフェクト時のオブジェクトの発光等に使うことができます。
    //*/
    //---------------------------------------------------------------------
    void setBlendColor( const LColor& color_ ) { _lnso->setBlendColor( color_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      ブレンドカラーの設定
    //
    //  @param[in]  r_ : R (赤) 成分 (0.0 〜 1.0)
    //  @param[in]  g_ : G (緑) 成分 (0.0 〜 1.0)
    //  @param[in]  b_ : B (青) 成分 (0.0 〜 1.0)
    //  @param[in]  a_ : ブレンド量 (0.0 〜 1.0)
    //
    //  @par
    //              ブレンドカラーは、描画するオブジェクトを塗りつぶす色です。<br>
    //              アルファ値がブレンドの強さとなり、
    //              1.0 のときに完全に設定した色で塗りつぶされます。<br>
    //              <br>
    //              ヒットエフェクト時のオブジェクトの発光等に使うことができます。
    //*/
    //---------------------------------------------------------------------
    void setBlendColor( lnfloat r_, lnfloat g_, lnfloat b_, lnfloat a_ ) { _lnso->setBlendColor( LColor( r_, g_, b_, a_ ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      ブレンドカラーの取得
    //*/
    //---------------------------------------------------------------------
    const LColor& getBlendColor() const { return _lnso->getBlendColor(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      色調の設定
    //
    //  @param[in]  tone_ : 色調
    //*/
    //---------------------------------------------------------------------
    void setTone( const LTone& tone_ ) { _lnso->setTone( tone_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      色調の設定
    //
    //  @param[in]  r_    : R (赤) 成分 (-1.0 〜 1.0)
    //  @param[in]  g_    : G (緑) 成分 (-1.0 〜 1.0)
    //  @param[in]  b_    : B (青) 成分 (-1.0 〜 1.0)
    //  @param[in]  gray_ : 単色化の強さ (0.0 〜 1.0)
    //*/
    //---------------------------------------------------------------------
    void setTone( lnfloat r_, lnfloat g_, lnfloat b_, lnfloat gray_ ) { _lnso->setTone( LTone( r_, g_, b_, gray_ ) ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      色調の取得
    //*/
    //---------------------------------------------------------------------
    const LTone& getTone() const { return _lnso->getTone(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      合成方法の設定
    //
    //  @param[in]  mode_ : 合成方法
    //*/
    //---------------------------------------------------------------------
    void setBlendMode( LNBlendMode mode_ ) { _lnso->setBlendMode( mode_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      合成方法の取得
    //*/
    //---------------------------------------------------------------------
    LNBlendMode getBlendMode() const { return _lnso->getBlendMode(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      深度テストの有効設定
    //
    //  @param[in]  flag_ : 有効フラグ (true:深度テストを行う false:行わない)
    //*/
    //---------------------------------------------------------------------
    void setEnableDepthTest( bool flag_ ) { _lnso->setEnableDepthTest( flag_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      深度テストの有効判定
    //*/
    //---------------------------------------------------------------------
    bool isEnableDepthTest() const { return _lnso->isEnableDepthTest(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      深度書き込みの有効設定
    //
    //  @param[in]  flag_ : 有効フラグ (true:深度Writeを行う false:行わない)
    //*/
    //---------------------------------------------------------------------
    void setEnableDepthWrite( bool flag_ ) { _lnso->setEnableDepthWrite( flag_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      深度書き込みの有効判定
    //*/
    //---------------------------------------------------------------------
    bool isEnableDepthWritet() const { return _lnso->isEnableDepthWritet(); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      優先度の設定
    //
    //  @param[in]  priority_ : 優先度
    //
    //  @par
    //              描画時のソートの基準となる優先度を設定します。
    //              値が大きい方が後から描画されるようになります。
    //              この値はZソートよりも優先されます。
    //*/
    //---------------------------------------------------------------------
    void setPriority( s32 priority_ ) { _lnso->setPriority( priority_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      優先度の取得
    //*/
    //---------------------------------------------------------------------
    s32 getPriority() const { return _lnso->getPriority(); }

protected:

    Core::Scene::SceneNode*    _lnso;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================