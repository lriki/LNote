//=============================================================================
//【 SceneShaderTechnique 】
//-----------------------------------------------------------------------------
///**
//  @file       SceneShaderTechnique.h
//  @brief      SceneShaderTechnique
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Interface.h"
#include "../DrawCommand.h"
#include "MMETypes.h"
#include "SceneShader.h"

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
class DrawCommandContainer;

/// スクリプトのコマンド種類
enum MMEScriptCommandType
{
    MME_SCRIPTCOM_UNKNOWN = 0,

    MME_SCRIPTCOM_RenderColorTarget,
    MME_SCRIPTCOM_RenderDepthStencilTarget,

    MME_SCRIPTCOM_ClearSetColor,
    MME_SCRIPTCOM_ClearSetDepth,

    MME_SCRIPTCOM_Clear_Color,              ///< Clear=Color
    MME_SCRIPTCOM_Clear_Depth,              ///< Clear=Depth

    MME_SCRIPTCOM_ScriptExternal_Color,     ///< ScriptExternal=Color

    MME_SCRIPTCOM_Pass,                     ///< Pass=(パス名) (実際には使ってないけど一応)

    MME_SCRIPTCOM_BeginPass,                ///< パスの開始
    MME_SCRIPTCOM_EndPass,                  ///< パスの終了

    MME_SCRIPTCOM_LoopByCount,              ///< LoopByCount=(パラメータ名)
    MME_SCRIPTCOM_LoopEnd,                  ///< LoopEnd=
    MME_SCRIPTCOM_LoopGetIndex,             ///< LoopGetIndex=(パラメータ名)

    MME_SCRIPTCOM_Draw_Geometry,            ///< Draw=Geometry
    MME_SCRIPTCOM_Draw_Buffer,              ///< Draw=Buffer

    MME_SCRIPTCOM_RenderPort,               ///< RenderPort=*  未対応
};


/// スクリプトのコマンドひとつ分
struct MMEScriptCommand
{
    MMEScriptCommandType Type;      ///< コマンドの種類

    union
    {
        struct
        {
            MMEShaderVariable*          Variable;   ///< RT を持つシェーダ変数
            u32                         Index;      ///< 設定するインデックス
        };
        //struct
        //{
        //    u32                         Color;      ///< クリアカラー
        //    lnfloat                     Depth;      ///< クリア深度値
        //};
        struct
        {
            Graphics::IShaderVariable*  CoreVariable;       ///< クリアカラー、深度用
        };
        struct
        {
            Graphics::IShaderPass*      Pass;       ///< 実行パス
        };
        struct
        {
            int                         LoopCount;  ///< ループ回数
        };
        struct
        {
            Graphics::IShaderVariable*  LoopVar;    ///< ループ回数を格納する変数
        };
    };
};

typedef std::vector< MMEScriptCommand >   MMEScriptCommandList;

//=============================================================================
// ■ SceneShaderTechnique クラス
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class SceneShaderTechnique
{
public:

    SceneShaderTechnique();

    virtual ~SceneShaderTechnique();

public:

    /// 初期化
    LNRESULT initialize( SceneShader* shader_, Graphics::IShaderTechnique* tech_ );

    /// 描画パスを取得する
    MMDPass getMMDPass() const { return mMMDPass; }

    /// 描画コマンドの作成
    void makeDrawCommand( DrawCommandContainer* container_, SceneNode* object_ );

    /// 描画コマンドの作成 ("ScriptExternal=Color" の前まで)
    void makeDrawCommandPreExternal( DrawCommandContainer* container_, SceneNode* object_ );

    /// 描画コマンドの作成 ("ScriptExternal=Color" から終端まで)
    void makeDrawCommandPostExternal( DrawCommandContainer* container_, SceneNode* object_ );

    /// スクリプトで "ScriptExternal=Color" があるかどうかを判定する
    bool isPostProcess() const { return ( mCommandExistFlags & MME_SCEXIST_ScriptExternal_Color ); }

    /// 特定のコマンドの存在を確認するためのフラグ
    lnU32 getCommandExistFlags() const { return mCommandExistFlags; }

private:

    /// スクリプト文字列の変数を受け取って mScriptCommandList にコマンドを追加していく
    //      pass_ : パス内のスクリプトを解析する場合、そのパスを渡す (Draw="Geometry" の実行に渡すpass)
    LNRESULT _lexScriptCommandString( Graphics::IShaderVariable* var_, Graphics::IShaderPass* pass_ );

    /// ひとつ分の描画コマンドを作って container_ に追加
    LNRESULT _makeDrawCommand( DrawCommandContainer* container_, MMEScriptCommand* command_, SceneNode* object_ );


private:

    SceneShader*                mSceneShader;
    Graphics::IShader*          mCoreShader;
    Graphics::IShaderTechnique* mTechnique;
    MMDPass                     mMMDPass;
    lnU32                       mFlags;      ///< MMETechEnableFlag の組み合わせ

    SubsetIndexArray            mSubsetIndexArray;

    MMEScriptCommandList            mScriptCommandList;     
    MMEScriptCommandList::iterator  mScriptExternalIt;      ///< mScriptCommandList 内の"ScriptExternal=Color" のコマンドの位置を指すイテレータ
    lnU32                           mCommandExistFlags;     ///< MMEScriptCommandExistFlag の組み合わせ
    
    lnU32                       mNextClearColor;
    lnfloat                     mNextClearDepth;

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