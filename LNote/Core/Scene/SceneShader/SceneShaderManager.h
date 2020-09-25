//=============================================================================
//【 SceneShaderManager 】
//-----------------------------------------------------------------------------
///**
//  @file       SceneShaderManager.h
//  @brief      SceneShaderManager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <algorithm>
#include "../../Base/STLUtil.h"
#include "../../Base/Cache.h"
#include "../../System/Interface.h"
#include "../../File/Manager.h"
#include "../../Graphics/Interface.h"
#include "../../Graphics/Manager.h"
#include "../../Game/GameTime.h"
#include "../Common.h"
#include "MMETypes.h"

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
class OffscreenScene;
class DrawCommandContainer;
typedef Base::CacheObjectManager< SceneShader >    SceneShaderCache;

//=============================================================================
// ■ SceneShaderManager クラス
//-----------------------------------------------------------------------------
///**
//  @brief      シーン用シェーダのクラス
//*/
//=============================================================================
class SceneShaderManager
    : public Base::ReferenceObject
    , public SceneShaderCache
{
public:

    /// 初期化データ
    struct InitData
    {
        Base::LogFile*      LogFile;
        File::Manager*      FileManager;
        Graphics::Manager*  GraphicsManager;
        lnU32               ShaderCacheSize;
        bool                UseSemantics;       ///< true の場合、シェーダ変数の検索にセマンティクス名を使う (false の場合は名前)
    };

public:
    SceneShaderManager( SceneGraph* scene_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~SceneShaderManager();

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

    /// 終了処理
    void finalize();

    /// シェーダの作成
    //LNRESULT createSceneShader( SceneShader** shader_, const void* data_, u32 size_, const lnChar* name_ );

    /// シェーダの作成 (ファイル名指定・ファイル名をキーとして辞書登録)
    LNRESULT createSceneShader( SceneShader** shader_, const lnChar* filename_ );

    /// 解放が必要なシェーダがないかチェックする
    void updateShaderList();

    /// 全シェーダにシーン単位で設定するべきデータを設定する
    void updateAllShaderGlobalParam( const Game::GameTime& game_time_ );

    /// 全ェーダにカメラ単位で設定するべきデータを設定する
    void updateAllShaderCameraParam( const LNCameraSceneParam& param_ );

    /// すべてのオフスクリーンRTの描画コマンドを作成する
    void makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ );

    /// (SceneGraph::addNode() から呼ばれる)
    void addSceneNodeToOffscreenScene( SceneNode* node_ );

    /// (SceneGraph::onDeleteNode() から呼ばれる)
    void removeSceneNodeFromOffscreenScene( SceneNode* node_ );

public:

    /// メッセージ処理 (SceneGraph から呼ばれる)
    LNRESULT onEvent( const System::Event& args_ );

    SceneGraph* getScene() { return mScene; }

    /// セマンティクスの文字列とIDの対応表の取得
    const MMESemanticsMap& getSemanticsMap() { return mMMESemanticsMap; }

    /// アノテーションの文字列とIDの対応表
    const MMEAnnotationMap& getAnnotationMap() { return mMMEAnnotationMap; }

    /// (SceneShader 内から呼ばれる)
    LNRESULT createTexture(
        Graphics::ITexture** texture_,
        MMEVariableRequest req_,
        const lnChar* resource_name_,
        lnU32 width_,
        lnU32 height_,
        lnU32 depth_,
        lnU32 miplevels_,
        LNSurfaceFormat format_ );

    /// GraphicsManager の取得
    Graphics::Manager* getGraphicsManager() { return mGraphicsManager; }

    /// グラフィックスデバイスの取得
    Graphics::IGraphicsDevice* getGraphicsDevice() { return mGraphicsDevice; }

    /// (OffscreenScene のコンストラクタから呼ばれる)
    void addOffscreenScene( OffscreenScene* ofs_ ) { mOffscreenSceneList.push_back( ofs_ ); }

    /// (OffscreenScene のデストラクタから呼ばれる)
    void removeOffscreenScene( OffscreenScene* ofs_ ) { Base::STLUtil::remove( mOffscreenSceneList, ofs_ ); }

    /// SceneShader::updateNodeParam() で行列・ベクトル配列をセットするときに使うので公開
    Base::TempBuffer* getTempBuffer() { return &mTempBuffer; }

 
private:

    /// シェーダの作成
    LNRESULT _createSceneShader( SceneShader** shader_, const void* data_, u32 size_, const lnChar* name_ );

    /// シェーダの作成 (入力ストリームから)
    LNRESULT _createSceneShader( SceneShader** shader_, File::IInStream* stream_, const lnChar* name_ );  

private:

    typedef std::vector< SceneShader* >     SceneShaderList;
    typedef std::vector< OffscreenScene* >  OffscreenSceneList;

private:

    Base::LogFile*              mLogFile;
    SceneGraph*                 mScene;
    File::Manager*              mFileManager;
    Graphics::Manager*          mGraphicsManager;
    Graphics::IGraphicsDevice*  mGraphicsDevice;

    MMESemanticsMap             mMMESemanticsMap;   ///< セマンティクスの文字列とIDの対応表 (セマンティクスを使わない場合は変数名) 
    MMEAnnotationMap            mMMEAnnotationMap;  ///< アノテーションの文字列とIDの対応表

    SceneShaderList             mSceneShaderList;
    OffscreenSceneList          mOffscreenSceneList;

    double                      mPlayStartTime;
    MMESceneParam               mMMESceneParam;     ///< Scene 単位のシェーダ設定パラメータ (updateAllShaderGlobalParam() で設定)
   
    Base::TempBuffer            mTempBuffer;        ///< 


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