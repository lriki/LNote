//=============================================================================
//【 Model 】
//-----------------------------------------------------------------------------
///**
//  @file       Model.h
//  @brief      Model
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
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
// ■ Model クラス
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class Model
    : public SceneNode
{
public:

    /// コンストラクタ
    Model( SceneGraph* scene_ );

    /// デストラクタ
    virtual ~Model();

public:

    /// 初期化 (×同フォルダに同名の.fxファイルがある場合はそれを読み込んで設定する)
	//		flags_ は LNModelFlag の組み合わせ
    LNRESULT initialize( const lnChar* filename_, lnU32 flags_ = 0 );

    Graphics::Model* getModel() { return mModel; }

public:

    /// ノードの種類の取得
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_MODEL; }

    /// フレーム更新
    virtual void update( const Game::GameTime& time_ );

    /// サブセットを描画する
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    Graphics::Model*    mModel;
    Graphics::ITexture* mSkinningBoneTexture;
    
    class DrawingContext;
    friend class DrawingContext;
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