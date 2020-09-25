//=============================================================================
//�y AudioResourceManager �z
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
// �� ResourceManager �N���X
//=============================================================================


    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    ResourceManager::ResourceManager()
        : mLogFile          ( NULL )
    {
    }


	//---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    ResourceManager::~ResourceManager()
    {
        

        LN_SAFE_RELEASE( mLock );

        
    }


    //---------------------------------------------------------------------
    // �� ������
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
    // �� �I������
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
    // �� �L�[�ɑΉ�����I�[�f�B�I�\�[�X����������
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
    // �� �I�[�f�B�I�\�[�X�̍쐬
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
				//LN_SETERR_S( LN_ERR_ASSERT, "< ResourceManager::createAudioSource >\n�s���ȃI�[�f�B�I�t�@�C���ł��B" );
                lr = LN_ERR_ASSERT;
                goto ERR_EXIT;
                break;
        }

		// �V�����쐬�ł����ꍇ
		if ( audio_source )
		{
			// ���̓t�@�C����ݒ肷��
			LN_CALL_GOTO( ERR_EXIT, audio_source->setStream( stream_ ) );

            // MP3 �� MIDI �ȊO�͓o�^
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
    // �� �I�[�f�B�I�\�[�X���L���b�V���ɓo�^ (�d�����������ꍇ�͂Ȃɂ����Ȃ�)
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