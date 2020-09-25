//=============================================================================
//【 SceneShader 】
//-----------------------------------------------------------------------------
///**
//  @file       SceneShader.h
//  @brief      SceneShader
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Interface.h"
#include "../Common.h"
#include "MMETypes.h"
#include "SceneShaderManager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
class SceneShaderTechnique;
class NodeRenderParam;
class DrawCommandContainer;

//=============================================================================
// ■ SceneShader クラス
//-----------------------------------------------------------------------------
///**
//  @brief      シーン用シェーダのクラス
//*/
//=============================================================================
class SceneShader
    : public Base::ReferenceObject
    , public SceneShaderCache::CachedObject
{
public:

    struct InitData
    {
        const void*     ShaderData;
        lnU32           ShaderDataSize;
        LRefTString     ShaderName;

    };

public:
    SceneShader( SceneShaderManager* manager_ );
    LN_CACHED_REFOBJ_METHODS;

protected:
    virtual ~SceneShader();

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

    /// 再読み込み
    LNRESULT reload();

    /// リソースの再構築
    LNRESULT refreshResource( const LVector2& view_size_ );

    /// Graphics::IShader の取得
    Graphics::IShader* getCoreShader() { return mShader; }

    /// エラー文字列の設定 (SceneShaderTechnique クラスからも呼ばれる)
    void setErrorMessage( LRefTString str_ ) { mErrorMessage = str_; }

    /// エフェクトファイルの使用目的の取得
    MMEScriptClass getScriptClass() const { return mScriptClass; }

    /// エフェクトファイルの実行タイミングの取得
    MMEScriptOrder getScriptOrder() const { return mScriptOrder; }

    /// ポストエフェクトか、ポストエフェクトの場合 true
    bool isPostEffect() const { return ( mScriptOrder == MME_SCRORDER_preprocess || mScriptOrder == MME_SCRORDER_postprocess ); }

    


public: // ▼ 内部用

    /// SceneShaderManager 取得
    SceneShaderManager* getSceneShaderManager() { return mManager; }

    /// 設定する必要があるライトの数の取得
    lnU32 getRequiredLightNum() const { return mRequiredLightNum; }

    /// 変数名に対応する MMEShaderVariable を検索する (MMETechniqueで使う。このポインタはシェーダがあるうちは不変なので、MMETechnique で使うなら initialize() で取得しておいてもOK)
    MMEShaderVariable* findShaderVariable( const lnChar* name_ );

    

    /// Scene 単位で必要なパラメータを設定する
    void updateSceneParam( const MMESceneParam& param_ );

    /// Camera 単位で必要なパラメータを設定する
    void updateCameraParam( const LNCameraSceneParam& param_ );

    /// Node 単位で必要なパラメータを設定する (ノードごとに影響するライトは異なるため、ライトもこの中)
    LNRESULT updateNodeParam( const SceneNodeContext* param_ );// const NodeRenderParam& param_ );

    /// Subset 単位で必要なパラメータを設定する
    void updateSubsetParam( const LNSubsetRenderParam& param_ );

    /// CONTROLOBJECT を要求している変数の値をすべて更新する
    void updateControlObjectVariables();

    /// 描画コマンドの作成 (テクニックがポストエフェクトの場合は何も行わない。SceneNode::makeDrawCommand() から呼ばれる)
    void makeDrawCommand( DrawCommandContainer* container_, SceneNode* object_, MMDPass pass_ );

    /// 描画コマンドの作成 ("ScriptExternal=Color" の前まで)
    void makeDrawCommandPreExternal( DrawCommandContainer* container_, SceneNode* object_, MMDPass pass_ );

    /// 描画コマンドの作成 ("ScriptExternal=Color" から終端まで)
    void makeDrawCommandPostExternal( DrawCommandContainer* container_, SceneNode* object_, MMDPass pass_ );


    /// オフスクリーンRT DefaultEffect の "self" で識別されるノードの取得 (onSettingShader() で設定)
    SceneNode* getSelfSceneNode() { return mSelfSceneNode; }

    /// 共有可能なシェーダの場合 true
    bool isShareable() const { return mShareable; }

    /// SceneNode::setShader() での先頭で呼ばれる
    void onSettingShader( SceneNode* node_ );


    /// 情報出力
    void dumpInfo( FILE* stream_ = NULL );

private:

    /// パラメータなどを実際に設定していく
    LNRESULT _build();

    /// 解放
    void _release();

    /// リソース解放
    void _releaseResource();

    /// シェーダ変数のセマンティクスを調べて対応する要求項目を返す
    MMEVariableRequest _checkVariableRequest( Graphics::IShaderVariable* var_, MMEShaderVariable* sv_ );

private:

    typedef std::vector< SceneShaderTechnique* >    SceneShaderTechniqueArray;
    typedef std::vector< OffscreenScene* >          OffscreenSceneList;

private:

    SceneShaderManager*     mManager;
    Graphics::IShader*      mShader;                ///< シェーダプログラム本体
    LRefTString             mName;

    MMEScriptOutput         mScriptOutput;          ///< STANDARDSGLOBAL の ScriptOutput (常に "color")
    MMEScriptClass          mScriptClass;           ///< エフェクトファイルの使用目的 (デフォルトは "object")
    MMEScriptOrder          mScriptOrder;           ///< エフェクトファイルの実行タイミング (デフォルトは "standard")

    MMEShaderVariableArray  mShaderVariableArray;   ///< シェーダプログラム内の変数一覧
    MMEShaderVariableMap    mShaderVariableMap;     ///< 変数名とMMEShaderVariableの対応表 (主にテクニックのスクリプトで使う)
    MMEShaderVariableArray  mControlObjectVarArray; ///< CONTROLOBJECT を要求している変数リスト
    SceneShaderTechniqueArray   mTechniqueList;         ///< テクニックの一覧
    OffscreenSceneList      mOffscreenSceneList;

    lnU32                   mRequiredLightNum;
    lnU32                   mWorldMatrixCalcMask;   ///< MMEWorldMatrixCalcFlags の組み合わせ (もしかしたら使わないかも)

    SceneNode*              mSelfSceneNode;

    LRefTString             mErrorMessage;
    bool                    mIsError;
    bool                    mShareable;
};

//=============================================================================
// ■ SceneShaderUtil
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class SceneShaderUtil
{
public:

    /// テクスチャリソースの生成情報
    struct TextureResourceData
    {
        LRefTString         ResourceName;
        LVector3            Dimensions;
        LVector2            ViewportRatio;
        int                 Miplevels;
        LNSurfaceFormat     Format;

        // アニメーションテクスチャ用
        lnfloat             Offset;
        lnfloat             Speed;
        LRefTString         SeekVariable;

        // オフスクリーンRT用
        LVector4            ClearColor;
        lnfloat             ClearDepth;
        bool                AntiAlias;
        LRefTString         Description;
        LRefTString         DefaultEffect;
    };

    

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      シェーダ変数のセマンティクスを調べて対応する要求項目を返す
    //
    //  @param[out] script_output_  : STANDARDSGLOBAL の ScriptOutput の値を格納する変数のアドレス (常に "color")
    //  @param[out] script_class_   : エフェクトファイルの使用目的 (デフォルトで "object")
    //  @param[out] script_order_   : エフェクトファイルの実行タイミング (デフォルトは "standard")            
    //  @param[out] is_controllobject_ : 有効な CONTROLOBJECT セマンティクスの場合、true が格納される
    //
    //  @note
    //              script_output_、script_class_、script_order_ は STANDARDSGLOBAL が
    //              ある場合のみ設定される。この関数の呼び出し側で sv_->Request == MME_VARREQ_STANDARDSGLOBAL と
    //              チェック後、変数の中身を確認すること。
    //*/
    //---------------------------------------------------------------------
    static void checkVariableRequest(
        Graphics::IShaderVariable* var_,    // const化したい
        const MMESemanticsMap& semantics_map_,
        MMEShaderVariable* sv_,
        MMEScriptOutput* script_output_,
        MMEScriptClass* script_class_,
        MMEScriptOrder* script_order_,
        bool* is_controllobject_ );

    /// 変数の "Object" アノテーションを調べて、"Light" の場合は fales、それ以外("Camera") は true を返す
    static bool checkAnnotationCameraOrLight( Graphics::IShaderVariable* var_ );

    /// 変数の "Object" アノテーションを調べて、"Geometry" の場合は 1、"Light" の場合は 2、それ以外は 0 を返す
    static int checkAnnotationGeometryOrLight( Graphics::IShaderVariable* var_ );

    /// 名前を指定してアノテーションを検索する (大文字小文字の区別無し。IShaderVariable からは大小小文字区別ありでしか取れないので用意)
    static Graphics::IShaderVariable* getAnnotationByName( Graphics::IShaderVariable* var_, const lnChar* name_ );

    /// 要求項目から、計算する必要のある行列マスクのビット列を作成して返す
    static lnU32 checkMatrixMask( MMEVariableRequest req_ );

    /// テクスチャリソースを作成する必要のある要求かを判定する
    static bool needCreateTexture( MMEVariableRequest req_ ) { return ( MME_VARREQ_TEXTURE <= req_ && req_ <= MME_VARREQ_OFFSCREENRENDERTARGET ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      テクスチャリソースの情報を返す
    //  
    //  @return     テクスチャを作る必要がある場合 (変数のセマンティクスがテクスチャ系) true を返す
    //*/
    //---------------------------------------------------------------------
    static bool checkTextureResourceData(
        MMEShaderVariable* sv_,  // これも const にしたい…
        const MMEAnnotationMap& annotation_map_,
        TextureResourceData* data_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      テクスチャリソースの情報から、テクスチャサイズを返す
    //
    //  @param[in]  viewport_size_ : シーンの仮想スクリーンサイズ
    //*/
    //---------------------------------------------------------------------
    static void checkTextureSize(
        int* width_,
        int* height_,
        const LVector3& dimensions_,
        const LVector2& viewport_ratio_,
        const LVector2& viewport_size_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      描画サブセットを示す文字列から、サブセット番号の配列を作成する
    //
    //  @note
    //              '-' による範囲指定で「x番以降すべて」を描画する場合、
    //              配列の終端には LN_INVALID_INDEX が格納される。
    //*/
    //---------------------------------------------------------------------
    static void makeSubsetIndexArray( SubsetIndexArray* list_, const lnChar* subset_text_ );

private:

    static const int MAX_SEMANTICS_NAME_LEN = 256;

    struct SubsetIndexOrder
    {
        lnU32   Index;
        int     RangeState;    ///< 範囲の指定 (0=なし 1=つぎのSubsetIndexの値まで 2=サブセット数の最大まで)
    };

    typedef std::list< SubsetIndexOrder > SubsetIndexOrderList;

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