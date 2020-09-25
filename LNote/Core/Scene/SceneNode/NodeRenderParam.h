//=============================================================================
//【 NodeRenderParam 】
//-----------------------------------------------------------------------------
///**
//  @file       NodeRenderParam.h
//  @brief      NodeRenderParam
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Common.h"
#include "../SceneShader/MMETypes.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
class SceneShader;
class SceneNode;
class Camera;

/// 優先パラメータまとめ
struct LNPriorityParameter
{
    SceneShader*    Shader;     ///< NULL の場合、getShader() で SceneNode::mShader が使われる
    bool            Hide;       ///< true の場合、isVisible() が常に false。false で SceneNode::mVisible で可視が決まる

    /// 初期値
    LNPriorityParameter()
        : Shader    ( NULL )
        , Hide      ( false )
    {}

    LNPriorityParameter( bool hide_ )
        : Shader    ( NULL )
        , Hide      ( hide_ )
    {}

    static LNPriorityParameter HIDE_PARAM;

    // Hide は、オフスクリーンRT からの可視指定や、視錐台に入っているかどうかで設定する。
};

//=============================================================================
// ■ NodeRenderParam クラス
//-----------------------------------------------------------------------------
///**
//  @brief		シェーダに送るデータをまとめたもの            
//*/
//-----------------------------------------------------------------------------
/*
    これまではオブジェクト単位とサブセット単位で構造体を
    分けてたけど、ひとつにまとめた方がわかりやすいと思ったので用意。
    また、マルチスレッド対応の為のコピーデータも兼ねる。

    描画スレッド有効の場合、各メンバのメモリ領域は new で作成される。
    その場合、set～ で設定された値は全てこの領域にコピーされ、
    get～ はこのメモリのポインタを返す。

    描画スレッド無効の場合、get～は set～ で設定されたポインタを
    そのまま返す。


    ※ DirectX のサンプルとかは結合済みボーン行列配列には
    ワールド変換済みのものを渡してる。
    ただ、ミラーのシェーダとかはオブジェクトのワールド行列を
    加工して適応する方が簡単になりそうだし、なにより MME 対応の
    シェーダプログラムに大きな変更を加えなくてもそこそこ使える
    ようになると思う。
    そんなわけで、ココではオブジェクトのワールド行列と、
    ローカルでのボーンの結合行列を分けて格納し、それぞれシェーダに渡す。 

    [12/8/5]
    なんか公式のマニュアルと違うけど、スキニング用のシェーダに関しては
    頂点シェーダでテクスチャのフェッチができるみたいなので
    それを利用する方向で組んでみる。
*/
//=============================================================================
class NodeRenderParam
{
public:
    NodeRenderParam();

    ~NodeRenderParam();

public: // 公開メンバ変数

    LNSubsetRenderParam*    Subset;
    LRenderState            RenderState;
    
public:

    /// 初期化
    LNRESULT initialize( SceneNode* node_, lnU32 subset_num_ );

    /// このパラメータを持つノードの取得
    SceneNode* getSceneNode() { return mSceneNode; }

    /// 全体の可視判定 (OffscreenRT からの優先パラメータ考慮済み。描画時は Node の getShader() ではなくこちらを使う)
    //SceneShader* getShader() { return ( mPriorityParameter.Shader ) ? mPriorityParameter.Shader : mSceneShader; }

    /// 全体の可視判定 (OffscreenRT からの優先パラメータ考慮済み。描画時は Node の isVisible() ではなくこちらを使う)
    bool isVisible() const { return ( mPriorityParameter.Hide ) ? false : mIsVisible; }

    /// 実際に描画時に使う情報の更新 (RenderState もここ。 parent_params_ が NULL の場合は親から受け継がない)
    void updateSubsetRenderParam( const NodeRenderParam* parent_params_ );

    /// 実際に描画時に使う情報の取得
    const LNSubsetRenderParam& getCombinedSubsetRenderParam( lnU32 idx_ ) const { return mCombinedSubsetRenderParams[ idx_ ]; }

    /// 実際に描画時に使うレンダーステートの取得
    const LRenderState& getCommittedRenderState() const { return mCommittedRenderState; }

    /// 描画に使う行列情報等の設定
    void setAffectElements( const LMatrix* world_, const Camera* camera_, const LightArray* lights_, SceneShader* shader_, bool visible_ );

    /// 優先パラメータの設定
    //void setPriorityParameter( const LNPriorityParameter& param_ ) { mPriorityParameter = param_; }

    /// シェーダ変数の要求に対応する行列(WORLD が含まれているもの)を返す (無効なものには単位行列を返す)
    //      light_idx_ : ライトが関係する行列が要求されたとき、そのインデックス (-1 で常にデフォルトライト)
    const LMatrix* getObjectMatrix( MMEVariableRequest req_, int light_idx_ ) const;

    /// 影響ライトの配列を取得する
    const LightArray& getAffectLightArray() const { return *mAffectLightArray; }



private:
    
    SceneNode*              mSceneNode;     ///< このクラスと関連付けられているノード
    lnU32                   mSubsetNum;
    SceneShader*            mSceneShader;
    LNSubsetRenderParam*    mCombinedSubsetRenderParams;
    LRenderState            mCommittedRenderState;
    LNPriorityParameter     mPriorityParameter;

    const LMatrix*          mAffectWorldMatrix;
    const Camera*           mAffectCamera;
    const LightArray*       mAffectLightArray;
    
    mutable LMatrix         mTempMatrix;
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