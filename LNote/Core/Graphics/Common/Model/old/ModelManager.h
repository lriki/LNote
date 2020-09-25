//=============================================================================
//【 ModelManager 】
//-----------------------------------------------------------------------------
///**
//  @file       ModelManager.h
//  @brief      ModelManager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../Base/Cache.h"
#include "../../../File/Interface.h"
#include "../../../File/Manager.h"
#include "../../../Physics/Manager.h"
#include "../../Interface.h"
#include "Model.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Graphics
{
class ModelCore;
class Motion;

typedef Base::CacheObjectManager< ModelCore >   ModelCoreCache;
typedef Base::CacheObjectManager< Motion >      MotionCache;

//=============================================================================
// ■ ModelManager クラス
//-----------------------------------------------------------------------------
///**
//  @brief		3D モデル管理クラス
//*/
//=============================================================================
class ModelManager
     : public Base::ReferenceObject
     , public ModelCoreCache
     //, public MotionCache
{
public:

	/// 初期化データ
	struct InitData
	{
        Base::LogFile*      LogFile;
        IGraphicsDevice*    GraphicsDevice;
        File::Manager*      FileManager;
        Physics::Manager*   PhysicsManager;
        lnU32               ModelCoreCacheSize;
	};

public:
    ModelManager();
    LN_REFOBJ_METHODS;

protected:
    virtual ~ModelManager();

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

    /// 終了処理
    void finalize();

    /// モデルファイルを読み込む
    LNRESULT createModel( Model** model_, void* data_, lnU32 size_, LSharingKey key_ );

    /// モデルファイルを読み込む
    LNRESULT createModel( Model** model_, File::IInStream* stream_, LSharingKey key_ );

    /// モデルファイルを読み込む (ファイル名指定)
    LNRESULT createModel( Model** model_, const lnChar* filename_, LSharingKey key_ );

    /// モーションファイルを読み込む
    LNRESULT createMotion( Motion** motion_, void* data_, lnU32 size_, LSharingKey key_ );

    /// モーションファイルを読み込む
    LNRESULT createMotion( Motion** motion_, File::IInStream* stream_, LSharingKey key_ );

    /// モーションファイルを読み込む (ファイル名指定)
    LNRESULT createMotion( Motion** motion_, const lnChar* filename_, LSharingKey key_ );

    /// IGraphicsDevice の取得
    IGraphicsDevice* getGraphicsDevice() { return mGraphicsDevice; }

public:

    /// 物理管理クラスの取得
    Physics::Manager* getPhysicsManager() { return mPhysicsManager; }

    /// テクスチャ作成 (ModelCore から作成時に呼ばれる。PMD、X ファイルのテクスチャ名は char なので lnChar にはしない)
    LNRESULT createTexture( ITexture** texture_, const char* filename_ );

private:

    Base::LogFile*      mLogFile;
    IGraphicsDevice*    mGraphicsDevice;
    File::Manager*      mFileManager;
    Physics::Manager*   mPhysicsManager;
    Base::TempBuffer    mTempBuffer;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Lumine

//=============================================================================
//                              end of file
//=============================================================================