//=============================================================================
//�y Framework �z
//-----------------------------------------------------------------------------
///**
//  @file       Framework.h
//  @brief      Framework
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "ConfigData.h"
#include "../System/Interface.h"
#include "../Game/FPSController.h"
#include "../Task/ITaskExecuteHandler.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{

//=============================================================================
// �� Framework �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		���C�u�����S�̂̊Ǘ��N���X
//*/
//=============================================================================
class Framework
    : public Base::ReferenceObject
    , public System::IEventListener
    , public Task::ITaskExecuteHandler
{
public:

    LN_REFOBJ_METHODS;

    Framework();

protected:

	virtual ~Framework();

public:

    /// ������
    LNRESULT initialize( const ConfigData& data_ );

    /// �G�f�B�^�c�[���p������
    void initializeForEditor( const ConfigData& data_ );

    /// �������C�u�����p������
    void initializeForAudioLibrary( const ConfigData& data_ );


    /// �I������
    void finalize();

    /// �^�X�N�Ǘ��N���X�̏�����
    void initializeTaskManager();

    /// �V�X�e���Ǘ��N���X�̏�����
    LNRESULT initializeSystemManager();

    /// �t�@�C���Ǘ��N���X�̏�����
    void initializeFileManager();

    /// ���͊Ǘ��N���X�̏�����
    LNRESULT initializeInputManager();

    /// �I�[�f�B�I�Ǘ��N���X�̏�����
    LNRESULT initializeAudioManager();

    /// �������Z�Ǘ��N���X�̏�����
    LNRESULT initializePhysicsManager();
    
    /// �O���t�B�b�N�X�Ǘ��N���X�̏�����
    void initializeGraphicsManager();

    /// �G�t�F�N�g�Ǘ��N���X�̏�����
    void initializeEffectManager();

    /// �V�[���Ǘ��N���X�̏�����
    LNRESULT initializeSceneManager();

    /// �V�X�e���Ǘ��N���X�̎擾
    System::Manager* getSystemManager() { return mSystemManager; }

    /// �t�@�C���Ǘ��N���X�̎擾
    File::Manager* getFileManager() { return mFileManager; }

    /// ���͊Ǘ��N���X�̎擾
    Input::Manager* getInputManager() { return mInputManager; }

    /// �I�[�f�B�I�Ǘ��N���X�̎擾
    Audio::Manager* getAudioManager() { return mAudioManager; }

    /// �������Z�Ǘ��N���X�̎擾
    Physics::Manager* getPhysicsManager() { return mPhysicsManager; }

    /// �O���t�B�b�N�X�Ǘ��N���X�̎擾
    Graphics::Manager* getGraphicsManager() { return mGraphicsManager; }

    /// �G�t�F�N�g�Ǘ��N���X�̎擾
    Effect::Manager* getEffectManager() { return mEffectManager; }

    /// �V�[���Ǘ��N���X�̎擾
    Scene::Manager* getSceneManager() { return mSceneManager; }

    /// GUI �Ǘ��N���X�̎擾
    GUI::Manager* getGUIManager() { return mGUIManager; }

    /// �Q�[���V�[���Ǘ��N���X�̎擾
    Game::Manager* getGameManager() { return mGameManager; }
    

    /// �Q�[���^�C���̎擾
    const Game::GameTime& getGameTime() const { return mFPSController->getGameTime(); }

    /// FPS ����N���X�̎擾
    Game::FPSController* getFPSController() { return mFPSController; }

    // ���C�u�����S�̂ɏI����ʒm����
    void exit() { mEndRequested = true; }

    /// �t���[���X�V
    bool update();

    /// �G�f�B�^�A���邢�͑g�ݍ��ݗp�̃t���[���X�V
    void updateAsLibrary();

    /// ���Ԃ������鏈���̌�ɌĂяo�����ƂŁAFPS �����A�Q�[���^�C���ւ̉e�������Z�b�g����
    void resetDelayTime();

    /// �f�o�C�X���Z�b�g���K�v�ł����onLostDevice() �� ���Z�b�g onResetDevice() �ƌĂяo�� (�����t�F�[�Y�Ŏ��s�BManaged Wrapper �Ŏg���̂� public ��)
    void checkDeviceState();

public:

    /// ���b�Z�[�W�����R�[���o�b�N
    virtual bool onEvent( const System::Event& args_ );

    /// ���C���X���b�h�̃t���[���҂��������s��
    virtual void OnFrameWait();

    /// �`����̍\�z���s��
    virtual void OnUpdateDrawContents();

private:

    /// �f�o�C�X���Z�b�g�O����
	//		_onLostDevice()�A_onResetDevice() �� Graphics �ȊO�ɔ�����`���邽�߂̏����B
    void _onLostDevice();

    /// �f�o�C�X���Z�b�g�㏈��
    void _onResetDevice();

    /// �Q�[���{�̂̍X�V�O�ɏ�������ׂ�����
    void _preUpdate();

    /// �Q�[���{�̂̍X�V��ɏ�������ׂ�����
    bool _postUpdate();

    /// �����t�F�[�Y�̏���
    void _onUpdateSyncPhase();

private:

    Base::LogFile*      mLogFile;           ///< ���O���o�͂��郍�O�t�@�C���I�u�W�F�N�g
    ConfigData          mConfigData;        ///< ���������

    Task::Manager*      mTaskManager;
    System::Manager*    mSystemManager;
    File::Manager*      mFileManager;
    Input::Manager*     mInputManager;
    Audio::Manager*     mAudioManager;
    Physics::Manager*   mPhysicsManager;
    Graphics::Manager*  mGraphicsManager;
    Effect::Manager*    mEffectManager;
    Scene::Manager*     mSceneManager;
    GUI::Manager*       mGUIManager;
    Game::Manager*      mGameManager;

    Game::FPSController*    mFPSController;

    LNUpdateMode        mUpdateMode;
    bool                mInitialized;
    bool                mEndRequested;
};

//---------------------------------------------------------------------
///**
//  @brief      Framework �C���X�^���X�̍쐬
//*/
//---------------------------------------------------------------------
Framework* LNOTE_STDCALL createFramework();

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================