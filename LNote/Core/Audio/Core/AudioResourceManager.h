//=============================================================================
//�y AudioResourceManager �z
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
// �� ResourceManager �N���X
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

	/// initialize() �ɓn���������f�[�^
	struct InitData
	{
        Base::LogFile*      LogFile;
        u32                 AudioSourceCacheSize;
    };

public:

    /// �R���X�g���N�^
	ResourceManager();

	/// �f�X�g���N�^
	virtual ~ResourceManager();

    LN_REFOBJ_METHODS;

public:

    /// ������
    LNRESULT initialize( const InitData& init_data_ );

    /// �I������
    void finalize();

    /// �L�[�ɑΉ�����I�[�f�B�I�\�[�X����������
    virtual LNRESULT findAudioSource( IAudioSource** obj_, LSharingKey key_ );

    /// �I�[�f�B�I�\�[�X�̍쐬
    virtual LNRESULT createAudioSource( IAudioSource** obj_, File::IInStream* stream_, LSharingKey key_ );

    /// �L���b�V�����N���A����
    virtual void clearCache() { AudioSourceCache::clear(); }

public:

    /// �I�[�f�B�I�\�[�X���L���b�V���ɓo�^ (�d�����������ꍇ�͂Ȃɂ����Ȃ�)
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