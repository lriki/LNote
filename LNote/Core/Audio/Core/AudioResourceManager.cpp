//=============================================================================
//【 AudioResourceManager 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "AudioUtil.h"
#include "AudioSource.h"
#include "AudioResourceManager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Audio
{

//=============================================================================
// ■ ResourceManager クラス
//=============================================================================


    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    ResourceManager::ResourceManager()
        : mLogFile          ( NULL )
    {
    }


	//---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    ResourceManager::~ResourceManager()
    {
        

        LN_SAFE_RELEASE( mLock );

        
    }


    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT ResourceManager::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        AudioSourceCache::initialize( init_data_.AudioSourceCacheSize );

        mLock = LN_NEW Thread::Mutex();

        LN_LOG_INIT_END;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 終了処理
    //---------------------------------------------------------------------
    void ResourceManager::finalize()
    {
		LN_LOG_FIN_BEGIN;
		_p(this->getRefCount());
        AudioSourceCache::finalize();
		_p(this->getRefCount());
		LN_LOG_FIN_END;
    }

    //---------------------------------------------------------------------
    // ● キーに対応するオーディオソースを検索する
    //---------------------------------------------------------------------
    LNRESULT ResourceManager::findAudioSource( IAudioSource** obj_, Base::SharingKey key_ )
    {
        *obj_ = NULL;
        IAudioSource* source = NULL;

        mLock->lock();

        source = AudioSourceCache::findCacheAddRef( key_ );

        mLock->unlock();

        /*
        if ( source )
        {
            printf( "%p:%d\n", source, source->getRefCount() );
        }
        else
        {
            _p(source);
        }
        */

        *obj_ = source;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● オーディオソースの作成
    //---------------------------------------------------------------------
    LNRESULT ResourceManager::createAudioSource( IAudioSource** obj_, File::IInStream* stream_, LSharingKey key_ )
    {
        LNRESULT lr = LN_OK;
        AudioSourceBase* audio_source = NULL;

        LNAudioSourceFormat format = AudioUtil::checkFormat( stream_ );

        mLock->lock();

        *obj_ = NULL;

		switch( format )
		{
			// Wave
			case LN_AUDIOSOURCE_WAVE:
                audio_source = LN_NEW Wave( this );
				break;
#if defined(LNOTE_MSVC)
			// MP3
			case LN_AUDIOSOURCE_MP3:
                audio_source = LN_NEW MP3( this );
				break;
#endif
			// OGG
			case LN_AUDIOSOURCE_OGG:
                audio_source = LN_NEW Ogg( this );
				break;
#if defined(LNOTE_MSVC)
			// MIDI
			case LN_AUDIOSOURCE_MIDI:
                audio_source = LN_NEW Midi( this );
				break;
#endif
			default:
		        LN_THROW_FileFormat( 0, "Unknown Audio File" );
				//LN_SETERR_S( LN_ERR_ASSERT, "< ResourceManager::createAudioSource >\n不明なオーディオファイルです。" );
                lr = LN_ERR_ASSERT;
                goto ERR_EXIT;
                break;
        }

		// 新しく作成できた場合
		if ( audio_source )
		{
			// 入力ファイルを設定する
			LN_CALL_GOTO( ERR_EXIT, audio_source->setStream( stream_ ) );

            // MP3 と MIDI 以外は登録
            if ( format != LN_AUDIOSOURCE_MP3 && format != LN_AUDIOSOURCE_MIDI )
            {
                AudioSourceCache::registerCachedObject( key_, audio_source );
            }

		}

        stream_->seek( 0 );
        
        *obj_ = audio_source;

    ERR_EXIT:
        mLock->unlock();
		return lr;
    }

    //---------------------------------------------------------------------
    // ● オーディオソースをキャッシュに登録 (重複があった場合はなにもしない)
    //---------------------------------------------------------------------
    //void ResourceManager::registerCachedObject( Base::SharingKey key_, IAudioSource* audio_source_ )
    //{
    //    mLock->lock();
    //    AudioSourceBase* as = dynamic_cast< AudioSourceBase* >( audio_source_ );
    //    if ( as )
    //    {
    //        AudioSourceCache::registerCachedObject( key_, as );
    //    }
    //    mLock->unlock();
    //}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Audio
} // namespace Core

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================