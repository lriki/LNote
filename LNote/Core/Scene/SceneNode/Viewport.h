//=============================================================================
//【 Viewport 】
//-----------------------------------------------------------------------------
///**
//  @file       Viewport.h
//  @brief      Viewport
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Animation/FadeValue.h"
#include "../../Graphics/Common/Model/Model.h"
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
// ■ Viewport クラス
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class Viewport
    : public SceneNode
{
public:

    /// コンストラクタ
    Viewport( SceneGraph* scene_ );

    /// デストラクタ
    virtual ~Viewport();

public:

    /// 初期化
    LNRESULT initialize( const Geometry::Size& size_ );

    

    /// プリプロセス・ポストプロセス用のシェーダを追加する (priority_ は値が大きい方が先に処理される。ScriptOrder に preprocess または postprocess が指定されているシェーダのみ追加可能)
    void addPostEffectShader( SceneShader* shader_, int priority_ );

    /// プリプロセス・ポストプロセス用のシェーダを外す
    void removePostEffectShader( SceneShader* shader_ );

    /// 色調の設定
    void setTone( const LTone& tone_, double duration_ );

    /// エフェクト適用中かを判定する
    bool isEffect() const { return mTone.isFinished(); }

public:

    /// 描画コマンドを階層的に作成する
    virtual void makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ );

    /// ノードの種類の取得
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_VIEWPORT; }

    /// フレーム更新
    virtual void update( const Game::GameTime& time_ );

    /// サブセットを描画する
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    

private:

    struct PPShader
    {
        int          Priority;
        SceneShader* Shader;

        bool operator() ( const PPShader& e_ ) const { return e_.Shader == Shader; }
    };

    typedef std::vector< PPShader >    PPShaderArray;

    /// ソート用の比較関数 (降順)
    static bool _cmpPPShader( const PPShader& l_, const PPShader& r_ ) { return l_.Priority >= r_.Priority; }

private:

    PPShaderArray                       mPPShaderArray;
    Animation::FadeValue< LVector4, lnfloat >    mTone;
    LVector2                            mViewSize;

    

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