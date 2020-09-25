//=============================================================================
//�y Manager �z
//-----------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Base/Container/NodeList.h"
#include "../Thread/Thread.h"
#include "../Thread/EventFlag.h"
#include "../Thread/Mutex.h"
#include "../File/Interface.h"
#include "../System/Interface.h"
#include "Core/AudioResourceManager.h"
#include "Sound.h"
#include "GameAudio.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File { class Manager; }

namespace Audio
{
//class ISound;
class GameAudio;

//=============================================================================
// �� Manager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �I�[�f�B�I�@�\�̊Ǘ��N���X
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
    , public IManager
{
public:

	/// initialize() �ɓn���������f�[�^
	struct InitData
	{
        Base::LogFile*              LogFile;
        File::Manager*              FileIOManager;          ///< �t�@�C������̓ǂݍ��݂Ɏg���t�@�C���Ǘ��N���X
        u32                         AudioSourceCacheSize;   ///< �L���b�V���T�C�Y (�t�@�C����)
        LNDirectMusicUsingMode      DMInitMode;             ///< DirectMusic �̏��������@
        System::IWindow*            Window;                 ///< DirectMusic �̏������Ɏg���E�B���h�E�n���h�������E�B���h�E�N���X

        /// �f�t�H���g�l
        InitData()
            : LogFile               ( NULL )
            , FileIOManager         ( NULL )
            , AudioSourceCacheSize  ( 16 )
            , DMInitMode            ( LN_DMUSINGMODE_NOTUSE )
            , Window                ( NULL )
        {}
    };

public:

    /// �R���X�g���N�^
	Manager();

	/// �f�X�g���N�^
	virtual ~Manager();

    LN_REFOBJ_METHODS;

public:

    /// ������
    LNRESULT initialize( const InitData& init_data_ );

    /// �I������ ( GameAudio �̉�� )
    void finalize();

    /// GameAudio �N���X�̎擾
    virtual IGameAudio* getGameAudio() { return mGameAudio; }

    /// �f�o�C�X�N���X�̎擾
    virtual IAudioDevice* getAudioDevice() { return mAudioDevice; }

    /// ���\�[�X�Ǘ��N���X�̎擾
    virtual IResourceManager* getResourceManager() { return mResourceManager; }

    /// ISound �̍쐬 ( stream_ = NULL �ŃL�[���g�������������s�� )
    virtual LNRESULT createSound( ISound** sound_, File::IInStream* stream_, LNSoundPlayType type_, bool enable_3d_, LSharingKey key_ );

    /// ISound �̍쐬 ( �t�@�C�����w�� )
    virtual LNRESULT createSound( ISound** sound_, const char* filename_, LNSoundPlayType type_, bool enable_3d_ );

    /// ISound �̍쐬 ( IAudioSource �w�� )
    virtual LNRESULT createSound( ISound** sound_, IAudioSource* source_, LNSoundPlayType type_, bool enable_3d_ );

    /// �O���[�v�̒�~
    virtual void stopGroup( u32 group_ );

public:

    /// Sound �̃f�X�g���N�^����Ă΂��
    void removeSound( Sound* sound_ );

    /// �t���[���X�V ( �X���b�h���g���ă|�[�����O���Ă���ꍇ�͂���������Ă΂�� )
    void update( float elapsed_time_ );

    /// �I���v�������Ă��邩�𔻒肷��
    bool isEndRequest() const { return mEndRequest->isTrue(); }


private:

    /// �|�[�����O�X���b�h�N���X
    class PollingThread
        : public Thread::ThreadObject
	{
	public:

		/// �R���X�g���N�^
		PollingThread( Manager* manager_ )
            : mManager  ( manager_ )
        {}

		/// �f�X�g���N�^
		~PollingThread()
		{
			// �Đ��I����҂�
			// ( ���̃f�X�g���N�^�́AManager �̃f�X�g���N�^����Ă΂��B
			//   ���̍ہAManager ���ł� delete �̒��O�� mEndRequest �� true �ɂ���B)
			wait();
			mManager = NULL;
		}

		/// �X���b�h�֐�
		virtual void run()
		{
#if defined(LNOTE_DIRECTX)
            // COM ������
            HRESULT hr = ::CoInitializeEx( NULL, COINIT_MULTITHREADED );
#endif
			// �I���v��������܂ŉ񂵑�����
			while( !mManager->isEndRequest() )
			{
                mManager->update( 0.020f );
                Thread::sleep( 20 );
			}
#if defined(LNOTE_DIRECTX)
            ::CoUninitialize();
#endif
		}

	private:

		Manager* mManager;		///< Audio::Manager �̃C���X�^���X�ւ̃|�C���^�B�R���X�g���N�^�Ŏ󂯎��
	};

private:

    typedef Base::NodeList< Sound > SoundList;

private:

    Base::LogFile*          mLogFile;
    IAudioDevice*           mAudioDevice;
    ResourceManager*        mResourceManager;
    SoundList               mSoundList;

    GameAudio*              mGameAudio;
    File::Manager*          mFileIOManager;

    PollingThread*	        mThread;		        ///< �|�[�����O�X���b�h�N���X
    Thread::EventFlag*	    mEndRequest;	        ///< �|�[�����O�X���b�h�ɏI����`���邽�߂̃t���O
    Thread::Mutex*          mLock;

    u32                     mOnMemoryLimitSize;
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