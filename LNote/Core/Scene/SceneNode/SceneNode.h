//=============================================================================
//【 SceneNode 】
//-----------------------------------------------------------------------------
///**
//  @file       SceneNode.h
//  @brief      SceneNode
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Game/GameTime.h"
#include "../../Graphics/Interface.h"
#include "NodeRenderParam.h"

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
// ■ SceneNode クラス
//-----------------------------------------------------------------------------
///**
//  @brief		シーンノードのクラス
//
//  @par
//              2D オブジェクトの場合、位置の z 値の範囲は 0～10000 です。
//              値が大きいほど手前に表示されます。範囲外の場合は表示されません。
//
//              優先度は、値の大きい方が手前に描画されます。(後から処理されます)
//*/
//=============================================================================
class SceneNode
    : public Base::ReferenceObject
{
public:
    /// コンストラクタ (add_scene_ は NullNode 用)
    SceneNode( SceneGraph* scene_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~SceneNode();

public:

    /// 初期化 (描画空間の設定はデフォルトシェーダの設定などですぐ使うので、継承先のinitialize() ではできるだけ先頭に書くこと)
    //      context_ に何かを渡した場合、subset_num_ は無効。
    LNRESULT initialize( lnU32 subset_num_, LNDrawCoordSpace cs_, SceneNodeContext* context_ = NULL );

    /// ノード名の取得
    const lnChar* getName() const { return mName.c_str(); }

    /// ノード名の設定
    LNRESULT setName( const lnChar* name_ );

    /// ワールド行列の設定
    void setMatrix( const LMatrix& matrix_ )        { mMatrix = matrix_; mNeedUpdateMatrix = false; }

    /// ワールド行列の取得
    const LMatrix& getMatrix() const                { return mMatrix; }

    /// 位置の設定
    void setPosition( const LVector3& pos_ )        { mPosition = pos_; mNeedUpdateMatrix = true; }

    /// 位置の取得
    const LVector3& getPosition() const             { return mPosition; }

	/// 回転角度の設定
	void setAngle( const LVector3& angle_ )         { mAngle = angle_; mNeedUpdateMatrix = true; }

    /// 回転角度の取得
	const LVector3& getAngle() const                { return mAngle; }

	/// 拡大率の設定
	void setScale( const LVector3& scale_ )         { mScale = scale_; mNeedUpdateMatrix = true; }

    /// 拡大率の取得
	const LVector3& getScale() const                { return mScale; }

    /// 原点の設定
    void setCenter( const LVector3& center_ )       { mCenter = center_; mNeedUpdateMatrix = true; }

    /// 原点の取得
    const LVector3& getCenter() const               { return mCenter; }

    /// 可視状態の設定
    void setVisible( bool flag_ )                   { mIsVisible = flag_; }

    /// 可視状態の判定
    bool isVisible() const                          { return mIsVisible; }

    /// 不透明度の設定
    void setOpacity( lnfloat opacity_, lnU32 subset_idx_ = 0 )             { mNodeRenderParam.Subset[ subset_idx_ ].Opacity = opacity_; }//mOpacity = opacity_; }

    /// 不透明度の取得
    lnfloat getOpacity( lnU32 subset_idx_ = 0 ) const                      { return mNodeRenderParam.Subset[ subset_idx_ ].Opacity; }//return mOpacity; }

    /// 乗算色の設定
    void setColorScale( const LColor& color_, lnU32 subset_idx_ = 0 )      { mNodeRenderParam.Subset[ subset_idx_ ].ColorScale = color_; }          

    /// 乗算色の取得
    const LColor& getColorScale( lnU32 subset_idx_ = 0 ) const             { return mNodeRenderParam.Subset[ subset_idx_ ].ColorScale; }

    /// ブレンドカラーの設定
    void setBlendColor( const LColor& color_, lnU32 subset_idx_ = 0 )      { mNodeRenderParam.Subset[ subset_idx_ ].BlendColor = color_; }

    /// ブレンドカラーの取得
    const LColor& getBlendColor( lnU32 subset_idx_ = 0 ) const             { return mNodeRenderParam.Subset[ subset_idx_ ].BlendColor; }

    /// 色調の設定
    void setTone( const LTone& tone_, lnU32 subset_idx_ = 0 )              { mNodeRenderParam.Subset[ subset_idx_ ].Tone = tone_; }

    /// 色調の取得
    const LTone& getTone( lnU32 subset_idx_ = 0 ) const                     { return mNodeRenderParam.Subset[ subset_idx_ ].Tone; }

    /// UV 変換行列の設定
	void setUVTransform( const LMatrix& matrix_, lnU32 subset_idx_ = 0 )    { mNodeRenderParam.Subset[ subset_idx_ ].UVTransform = matrix_; }

    /// UV 変換行列の設定
    const LMatrix& getUVTransform( lnU32 subset_idx_ = 0 )  const           { return mNodeRenderParam.Subset[ subset_idx_ ].UVTransform; }

    /// 合成方法の設定
    void setBlendMode( LNBlendMode mode_ )          { mNodeRenderParam.RenderState.BlendMode = mode_; }

    /// 合成方法の取得
    LNBlendMode getBlendMode() const                { return mNodeRenderParam.RenderState.BlendMode; }

    /// 深度テストの有効設定
    void setEnableDepthTest( bool flag_ )           { mNodeRenderParam.RenderState.DepthTest = flag_; }

    /// 深度テストの有効判定
    bool isEnableDepthTest() const                  { return mNodeRenderParam.RenderState.DepthTest; }

    /// 深度Writeの有効設定
    void setEnableDepthWrite( bool flag_ )          { mNodeRenderParam.RenderState.DepthWrite = flag_; }

    /// 深度書き込みの有効判定
    bool isEnableDepthWritet() const                { return mNodeRenderParam.RenderState.DepthWrite; }

    /// カリング方法の設定
    void setCullingMode( LNCullingMode mode_ )      { mNodeRenderParam.RenderState.Culling = mode_; }

    /// カリング方法の取得
    LNCullingMode getCullingMode() const            { return mNodeRenderParam.RenderState.Culling; }

    /// 回転順序の設定
    void setRotateOrder( LNRotOrder order_ )        { mRotOrder = order_; }

    /// 回転順序の取得
    LNRotOrder getRotateOrder() const               { return mRotOrder; }

    /// 優先度の設定
    void setPriority( s32 priority_ )               { mPriority = priority_; }

    /// 優先度の取得
    s32 getPriority() const                         { return mPriority; }
  
    /// ビルボード状態の設定
    void setBillboardType( LNBillboardType type_ )  { mBillboardType = type_; }

    /// ビルボード状態の取得
    LNBillboardType getBillboardType() const        { return mBillboardType; }

    /// 自動更新の有効設定
    void setEnableAutoUpdate( bool flag_ )          { mIsAutoUpdate = flag_; }

    /// 自動更新の有効判定
    bool isEnableAutoUpdate()                       { return mIsAutoUpdate; }

    /// シェーダの設定
    void setShader( SceneShader* shader_ );

    /// シェーダの取得
    SceneShader* getShader()                        { return mShader; }

public:

    /// ノードに関連付けられている特殊な値を取得する
    //virtual LNRESULT getItemData( const lnChar* item_name_, LMatrix* out_ );

    /// 子ノードを追加する
    LNRESULT addChild( SceneNode* node_ );

    /// 子ノードを外す
    LNRESULT removeChild( SceneNode* node_ );

    /// ワールド行列を階層的に更新する (SceneGraph::update() から呼ばれる。Context 作成もとりあえずここで)
    void updateTransformHierarchy( const LMatrix& parent_marix_, const NodeRenderParam* parent_params_ );

    /// カメラ単位の描画情報の更新
    void updateParamFromCameraAndLight( Camera* camera_ );

    /// ×優先パラメータの設定 (実体は必ず描画終了時まで存在していること)
    //      makeDrawCommand() に渡す方向でもいいと思ったけど、
    //      それだとオフスクリーンRTでの描画ノードの仕分け→階層描画
    //      に対応できなくなる。
    void setPriorityParameter( LNPriorityParameter* param_ ) { mPriorityParameter = param_; }//mNodeRenderParam.setPriorityParameter( param_ ); }

    /// 描画コマンドを階層的に作成する
    virtual void makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ );

    /// 描画する座標空間
    LNDrawCoordSpace getDrawCoordSpace() const { return mDrawCoordSpace; }

    /// 描画パラメータの取得
    NodeRenderParam* getNodeRenderParam() { return &mNodeRenderParam; }

    /// サブセット数の取得 (0 のノードは描画しない)
    lnU32 getSubsetNum() const { return mSubsetNum; }

    SceneNodeContext* getContext() { return mContext; }

    void updateContext();

public: // ▼ 継承先で実装する

    /// ノードの種類の取得
    virtual LNSceneNodeType getSceneNodeType() const = 0;

    /// フレーム更新
    virtual void update( const Game::GameTime& time_ ) { }

    /// サブセットを描画する
    virtual LNRESULT drawSubset( lnU32 index_ ) = 0;

    /// デバッグ用の描画
    virtual void drawDebug( lnU32 flags_ ) { };

private:

    /// Z ソート用の比較関数
    static bool _cmpZAndPrioritySort( const SceneNode* l_, const SceneNode* r_ );

protected:

    SceneGraph*         mSceneGraph;
    LNDrawCoordSpace    mDrawCoordSpace;
    lnU32               mSubsetNum;
    NodeRenderParam     mNodeRenderParam;

    SceneNode*          mParentNode;            ///< 親ノード
    SceneNodeArray      mChildNodeList;         ///< 子ノードのリスト

    SceneNodeContext*   mContext;

    LRefTString         mName;
    LMatrix             mMatrix;                ///< ワールド変換行列
    LMatrix             mCombinedWorldMatrix;   ///< 行列の階層更新で作成される結合済みワールド行列
    LVector3            mPosition;
    LVector3            mAngle;
    LVector3            mScale;
    LVector3            mCenter;
    LNRotOrder          mRotOrder;          ///< 回転順序
    lnS32               mPriority;          ///< 優先度
    LNBillboardType     mBillboardType;
    SceneShader*        mShader;
    lnfloat             mZDistance;
    LightArray          mApplyLights;       ///< 描画に使うライト (setShader() で必要なライト数分 resize() されている)
    
    LNPriorityParameter*     mPriorityParameter;

    bool                mIsVisible;         ///< 可視状態
    bool                mNeedUpdateMatrix;
    bool                mIsAutoUpdate;
    bool                mEnableDraw;        ///< mIsVisible が false だったり、カリングで除外された場合は false

    friend class SceneNodeContext;
  
};

//=============================================================================
// ■ SceneNodeContext
//-----------------------------------------------------------------------------
///**
//  @brief
//  @note
//              Context の使用手順…
//                  1. SceneNodeContext を継承したクラスを定義
//                  2. updateContext() で保存するメンバ変数をコピーする
//                  3. SceneNode::initialize() に、派生コンテキストのポインタを渡す
//                  ※ 解放処理は必要なし
//*/
//=============================================================================
class SceneNodeContext
    //: public Base::ReferenceObject
{
public:

    SceneNodeContext();
    virtual ~SceneNodeContext();
    
public:

    void initialize( SceneNode* node_, lnU32 subset_num_ );

    /// [同期フェーズ]
    virtual void updateContext( SceneNodeContext* parent_context_ );

    /// [同期フェーズ] (とりあえず updateContext() から呼んでいる)
    virtual void releaseAllResource();

    /// [描画スレッド]
    void setPriorityParameter( LNPriorityParameter* param_ ) { mPriorityParameter = param_; }

    /// [描画スレッド]
    virtual void OnSetActivePaneContext( SceneGraphContext* scene_context_, PaneContext* pane_context_, CameraContext* camera_context_ );

    /// [描画スレッド] Node 単位のパラメータをシェーダに設定する時に呼び出される
    virtual void onSetNodeParameter( SceneShader* shader_ ) {}

    /// [描画スレッド] (今は Model のみで使用中。ほかのも順に、これで描画するようにしたい)
    virtual void drawSubset( lnU32 index_ ) {}

public:

    lnU32 getSubsetNum() const { return mSubsetNum; }

    SceneShader* getShader() { return mShader; }

    const LMatrix& getCombinedWorldMatrix() const { return mCombinedWorldMatrix; }

    // out_ に値を格納した場合は true を返す
    bool getObjectMatrix( LMatrix* out_, MMEVariableRequest req_, int light_idx_ ) const;

    // SceneShader::updateNodeParam() 用
    const LightContextArray&    getAffectLightArray() const { return mAffectLightContextArray; }

    // SceneShader::updateSubsetParam() 用
    const LNSubsetRenderParam&  getSubsetRenderParam( int index_ ) const { return mSubsetRenderParams[index_]; }

    const LRenderState&         getRenderState() const { return mRenderState; }

    CameraContext* getCameraContext() { return mAffectCameraContext; }

private:

    static bool _cmpZAndPrioritySort( const SceneNodeContext* l_, const SceneNodeContext* r_ );

protected:

    SceneNode*              mSceneNode;
    SceneNodeContextArray   mChildSceneNodeContextArray;
    lnU32                   mSubsetNum;
    LNSubsetRenderParam*    mSubsetRenderParams;
    LRenderState            mRenderState;
    LNPriorityParameter*    mPriorityParameter;
    LMatrix                 mCombinedWorldMatrix;   ///< 行列の階層更新で作成される結合済みワールド行列
    lnS32                   mPriority;
    LNBillboardType         mBillboardType;
    SceneShader*            mShader;
    lnfloat                 mZDistance;
    LightContextArray       mAffectLightContextArray;           ///< 描画に使うライト
    CameraContext*          mAffectCameraContext;
    bool                    mIsVisible;

    
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