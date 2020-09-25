//=============================================================================
//【 AudioResourceManager 】
//-----------------------------------------------------------------------------
///**
//  @file       AudioResourceManager.h
//  @brief      AudioResourceManager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <map>
#include "../../Base/Cache.h"
//#include "../../Base/Container/Map.h"
#include "../../Thread/Mutex.h"
#include "../../File/Interface.h"
#include "../Interface.h"



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Audio
{
class IAudioSource;
class AudioSourceBase;
typedef Base::CacheObjectManager< AudioSourceBase >   AudioSourceCache;

//=============================================================================
// ■ ResourceManager クラス
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class ResourceManager
    : public Base::ReferenceObject
    , public IResourceManager
    , public AudioSourceCache
{
public:

	/// initialize() に渡す初期化データ
	struct InitData
	{
        Base::LogFile*      LogFile;
        u32                 AudioSourceCacheSize;
    };

public:

    /// コンストラクタ
	ResourceManager();

	/// デストラクタ
	virtual ~ResourceManager();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

    /// 終了処理
    void finalize();

    /// キーに対応するオーディオソースを検索する
    virtual LNRESULT findAudioSource( IAudioSource** obj_, LSharingKey key_ );

    /// オーディオソースの作成
    virtual LNRESULT createAudioSource( IAudioSource** obj_, File::IInStream* stream_, LSharingKey key_ );

    /// キャッシュをクリアする
    virtual void clearCache() { AudioSourceCache::clear(); }

public:

    /// オーディオソースをキャッシュに登録 (重複があった場合はなにもしない)
    //void registerCachedObject( Base::SharingKey key_, IAudioSource* audio_source_ );

private:

    Base::LogFile*      mLogFile;
    Thread::Mutex*      mLock;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Audio
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================