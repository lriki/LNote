//=============================================================================
//【 OffscreenScene 】
//-----------------------------------------------------------------------------
///**
//  @file       OffscreenScene.h
//  @brief      OffscreenScene
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Common.h"
#include "../SceneNode/NodeRenderParam.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
class SceneGraph;
class SceneShader;

//=============================================================================
// ■ OffscreenScene クラス
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class OffscreenScene
    : public Base::ReferenceObject
{
public:

    struct InitData
    {
        LVector4            ClearColor;
        lnfloat             ClearDepth;
        LRefTString         Description;
        LRefTString         DefaultEffectStrings;
        Graphics::ITexture* RTTexture;              ///< addRef
        //bool                Antialias;
    };

public:

    /// コンストラクタ
    OffscreenScene( SceneShaderManager* manager_, SceneShader* shader_ );

    /// デストラクタ
    ~OffscreenScene();

public:
   
    /// 初期化 (アンチエイリアス未対応)
    LNRESULT initialize( const InitData& init_data_ );

    /// レンダリングターゲットクリアする色の設定
    void setClearColor( const LVector4& color_ );

    /// レンダリングターゲットクリアする色の取得 (カラーコード)
    u32 getClearColor() const { return mClearColor; }

    /// 深度ターゲットクリアするZ値の設定
    void setClearDepth( lnfloat depth_ ) { mClearDepth = depth_; }

    /// 深度ターゲットクリアするZ値の取得
    lnfloat getClearDepth() const { return mClearDepth; }

    /// エフェクトファイルの説明文の設定
    void setDescription( LRefTString desc_ ) { mDescription = desc_; }

    /// エフェクトファイルの説明文の取得
    LRefTString getDescription() const { return mDescription; }

    /// エフェクトファイルの割り当て方法の設定 (DefaultEffect アノテーション)
    void setDefaultEffectStrings( LRefTString effects_ ) { mDefaultEffectStrings = effects_; }

    /// エフェクトファイルの割り当て方法の取得
    //DefaultEffectStrings getDefaultEffectStrings() { return mDefaultEffectStrings; }

    /// ノードをDefaultEffect の指定と一致する場合に追加する (新しく作成されたノードに対してはすべて一度これを呼ぶ)
    void addSceneNode( SceneNode* node_ );

    /// ノードを外す (SceneShaderManager::removeSceneNodeFromOffscreenScene() から呼ばれる)
    void removeSceneNode( SceneNode* node_ );

    /// シーンに登録されているすべてのノードで addSceneNode() を呼ぶ (makeDrawCommand で読んでもいいかも)
    void refreshAffectNodes();

    /// DefaultEffect に "self=" が含まれている
    bool isThereSelf() const { return mIsThereSelf; }

    /// すべてのオフスクリーンRTの描画コマンドを作成する
    void makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ );

private:

    /// エフェクトファイルの割り当て更新
    //      必要なシェーダがまだ Manager に登録されていない場合は新しく作成する。
    //      これだけなら初期化のときにすればいいと思ったけど、
    //      このクラスの initialzie() はシェーダの初期化から呼ばれる。
    //      ということは、Manager::createShader() が再帰的に呼ばれることになってしまい、
    //      なんか不安。(実際、createShader() では FileManager の排他処理を行ってる)
    //      そんなわけでちょっと面倒かもだけどシェーダを作り終わった後、
    //      初期化後処理っていう形で検索・作成をしていく。
    //
    //      [12/8/12] initialize() は SceneShader::refreshResource() から呼び出すようにしたので、
    //      別途これを呼ぶ必要は今のところなし。
    LNRESULT _refreshEffects();

    // オフスクリーン RT 内で作成したシェーダを解放する
    void _releaseEffects();

private:

    enum EntryType
    {
        ENTRY_EFFECT = 0,   ///< エフェクトファイル
        ENTRY_NONE,         ///< エフェクトなし
        ENTRY_HIDE          ///< 非表示
    };

    struct DefaultEffectEntry
    {
        EntryType       Type;
        LRefTString     ObjectName;     ///< (ワイルドカード指定可能 * ?)
        LRefTString     EffectName;     ///< (ワイルドカード指定不可)
        SceneShader*    Shader;         ///< EffectName で作成したシェーダ
    };

    struct SceneNodeEntry
    {
        SceneNode*          Node;   ///< 描画するノード
        LNPriorityParameter PriorityParameter;  ///< 描画コマンドに渡す
        //SceneShader*    Shader; ///< 適用する優先シェーダ
        //bool            Hide;
    };

private:

    typedef std::vector< DefaultEffectEntry >   DefaultEffectEntryList;
    typedef std::vector< SceneNodeEntry >       SceneNodeEntryList;
    
private:

    SceneShaderManager*     mManager;
    SceneShader*            mSceneShader;
    //SceneGraph*             mScene;
    lnU32                   mClearColor;
    lnfloat                 mClearDepth;
    LRefTString             mDescription;
    LRefTString             mDefaultEffectStrings;
    Graphics::ITexture*     mRTTexture;

    SceneNodeEntryList      mSceneNodeEntryArray;            ///< このオフスクリーンRTで描画するノードの一覧

    DefaultEffectEntryList  mDefaultEffectEntryList;
    LRefTString             mErrorMessage;

    bool                    mIsThereSelf;
    bool                    mNeedRefreshAffectNodes;
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