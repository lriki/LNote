//=============================================================================
//【 LGameScene 】
//-----------------------------------------------------------------------------
///**
//  @file       LGameScene.h
//  @brief      LGameScene
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../GameScene/GameScene.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// ■ LGameScene クラス
//-----------------------------------------------------------------------------
///**
//  @brief     タイトル画面、バトル画面等、ひとつのゲームシーンを表すクラスの基底
//*/
//=============================================================================
class LGameScene
    : public Utility::GameScene
{
public:

    /// コンストラクタ
	LGameScene();

protected:

	/// デストラクタ
    virtual ~LGameScene() {}

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      シーン名の取得
    //*/
    //---------------------------------------------------------------------
	virtual const lnChar* getSceneName() { return NULL; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      開始処理
    //*/
    //---------------------------------------------------------------------
	virtual void onStart() {}

    //---------------------------------------------------------------------
    ///**
    //  @brief      フレーム更新
    //*/
    //---------------------------------------------------------------------
	virtual void onUpdate() {}

    //---------------------------------------------------------------------
    ///**
    //  @brief      終了処理
    //*/
    //---------------------------------------------------------------------
	virtual void onTerminate() {}

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      登録されたシーン名を指定してシーンを変更する
    //
    //  @par
    //              不具合防止のため、この関数を呼び出したシーンがアクティブではない場合は
    //              変更を行いません。
    //*/
    //---------------------------------------------------------------------
    void changeScene( const lnChar* scene_name_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      シーンの変更
    //
    //  @par
    //              不具合防止のため、この関数を呼び出したシーンがアクティブではない場合は
    //              変更を行いません。
    //*/
    //---------------------------------------------------------------------
    void changeScene( LGameScene* next_scene_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      登録されたシーン名を指定してシーンを呼び出す
    //
    //  @par
    //              不具合防止のため、この関数を呼び出したシーンがアクティブではない場合は
    //              変更を行いません。
    //*/
    //---------------------------------------------------------------------
    void callScene( const lnChar* scene_name_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      シーンの呼び出し
    //
    //  @par
    //              指定したシーンを子シーンとして呼び出します。
    //              元のシーンに戻るには、子シーン側の returnScene() を呼びます。
    //*/
    //---------------------------------------------------------------------
    void callScene( LGameScene* next_scene_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      呼び出し元のシーン (親シーン) へ戻る
    //*/
    //---------------------------------------------------------------------
    void returnScene( const lnChar* parent_name_ = NULL );

    //---------------------------------------------------------------------
    ///**
    //  @brief      アクティブなシーンかを判定する
    //*/
    //---------------------------------------------------------------------
    bool isActiveScene() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      親ゲームシーンの取得
    //*/
    //---------------------------------------------------------------------
    LGameScene* getParentScene() const;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================