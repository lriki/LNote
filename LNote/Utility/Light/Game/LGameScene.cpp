//=============================================================================
//【 LGameScene 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Core/Framework/Framework.h"
#include "../Accessor.h"
#include "LGameScene.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// ■ LGameScene クラス
//=============================================================================

    // コンストラクタ
    LGameScene::LGameScene()
        : Utility::GameScene( gGameSceneManager )
    {
    }

    // シーンの変更
    void LGameScene::changeScene( const lnChar* scene_name_ )
    {
        Utility::GameScene::changeScene( scene_name_ );
    }

    // シーンの変更
    void LGameScene::changeScene( LGameScene* next_scene_ )
    {
        Utility::GameScene::changeScene( next_scene_ );
    }

    // シーンの呼び出し
    void LGameScene::callScene( const lnChar* scene_name_ )
    {
        Utility::GameScene::callScene( scene_name_ );
    }

    // シーンの呼び出し
    void LGameScene::callScene( LGameScene* next_scene_ )
    {
        Utility::GameScene::callScene( next_scene_ );
    }

    // 呼び出し元のシーン (親シーン) へ戻る
    void LGameScene::returnScene( const lnChar* parent_name_ )
    {
        Utility::GameScene::returnScene( parent_name_ );
    }

    // アクティブなシーンかを判定する
    bool LGameScene::isActiveScene() const
    {
        return Utility::GameScene::isActiveScene();
    }

    // 親ゲームシーンの取得
    LGameScene* LGameScene::getParentScene() const
    {
        return static_cast< LGameScene* >( Utility::GameScene::getParentScene() );
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================