//=============================================================================
//【 GameSceneManager 】
//-----------------------------------------------------------------------------
///**
//  @file       GameSceneManager.h
//  @brief      GameSceneManager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Common.h"
#include <map>
#include <string>
#include <deque>

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Game
{

//=============================================================================
// ■ Manager
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

    Manager();

public:

    /// 初期化
    void initialize(Core::Framework* framework_);

    /// 終了処理
    void finalize();


    //---------------------------------------------------------------------
    ///**
    //  @brief      実行
    //*/
    //---------------------------------------------------------------------
    void run(GameScene* first_scene_);

    //---------------------------------------------------------------------
    ///**
    //  @brief      ゲームシーンの変更
    //  @attention
    //              渡されたゲームシーンは、終了した (onTerminate() が呼ばれた直後) に
    //              release() されますが、gotoGameScene() では addRef() されません。
    //              これは gotoGameScene(new TitleScene()) のように
    //              シーン変更を簡潔に記述できるようにするためです。
    //*/
    //---------------------------------------------------------------------
    void gotoGameScene(GameScene* next_scene_);

    //---------------------------------------------------------------------
    ///**
    //  @brief      ゲームシーンの呼び出し
    //  @par
    //              指定したシーンを子シーンとして呼び出します。
    //              元のシーンに戻るには、returnGameScene() を使用します。
    //  @attention
    //              gotoGameScene() と同様に、参照カウントはインクリメントしません。
    //*/
    //---------------------------------------------------------------------
    void callGameScene(GameScene* next_scene_);

    //---------------------------------------------------------------------
    ///**
    //  @brief      呼び出し元のゲームシーン (親ゲームシーン) へ戻る
    //*/
    //---------------------------------------------------------------------
    void returnGameScene();

    //---------------------------------------------------------------------
    ///**
    //  @brief      ゲーム終了
    //  @par
    //              ゲーム終了を Core::Framework に伝えます。
    //*/
    //---------------------------------------------------------------------
    void exit();

    //---------------------------------------------------------------------
    ///**
    //  @brief      次のゲームシーンがあるか(変更の必要があるか)調べる
    //*/
    //---------------------------------------------------------------------
	//bool isNextScene() const;


    //---------------------------------------------------------------------
    ///**
    //  @brief      フレーム更新
    //  @retval     true  : 実行中
    //  @retval     false : シーンを変更する必要がある、または終了した
    //*/
    //---------------------------------------------------------------------
	bool update();

    /// 現在のシーンの取得
    GameScene* getCurrentGameScene() { return mScene; }





    /// (Ruby 用暫定)
    void setActiveGameScene(GameScene* scene_, const lnChar* name_);

    //---------------------------------------------------------------------
    ///**
    //  @brief      シーン名に対してユニークなIDを割り当てる
    //  @note
    //              アクティブなシーンを識別するためのID(単純な登録順)を作成し、
    //              登録する。既に登録された名前がある場合は登録しない。
    //              (Ruby対応のために暫定的に用意)
    //*/
    //---------------------------------------------------------------------
    int registerActiveSceneKeyName(const lnChar* name_);

	void setEnableRefCountOperate( bool flag ) { mEnableRefCountOperate = flag; }

public:

    Core::Framework* getFramework() { return mFramework; }

private:

    /// 実行中のシーンを階層構造も含めて release する (gotoGameScene() のために使う。全てのシーンのonTerminate() が呼ばれる。順番は子→親)
    void _releaseAllScene();

private:

    enum EventCommand
    {
        SMC_NONE = 0,       ///< 定義なし
        SMC_CHANGE,         ///< 直接遷移
        SMC_PUSH,           ///< 呼び出し
        SMC_RETURN,         ///< 呼び出し元へ戻る
/*
        SMC_CHANGE_LOADING,
        SMC_PUSH_LOADING
        */
    };

    struct EventMessage
    {
        EventCommand    Command;
        GameScene*      Scene;
    };

    typedef std::map<ln_std_tstring, int>           SceneKeyNameMap;
    typedef std::pair<ln_std_tstring, int>          SceneKeyNamePair;

private:

    Core::Framework*    mFramework;
    GameScene*	        mScene;			    ///< 現在のゲームシーン
	GameScene*	        mNextScene;		    ///< 移動先のゲームシーン
    bool                mChangeRequested;   ///< シーン変更要求
	bool		        mEndRequested;	    ///< 終了要請

    GameScene*          mShouldAddedScene;  ///< 次のフレームの開始時に子として追加するシーン (同時に複数追加する場合は一番子を指す。追加した後はすぐに NULL にしておく)
    std::deque< EventMessage >  mMessageQueue;

    SceneKeyNameMap mSceneKeyNameMap;
    //GameScene*      mActiveScene;
    int             mActiveSceneID;

	bool			mEnableRefCountOperate;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Game
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================