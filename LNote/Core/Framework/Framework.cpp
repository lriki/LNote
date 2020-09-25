//=============================================================================
//�y Framework �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <locale.h>
#include "../Thread/Thread.h"
#include "../Task/Manager.h"
#include "../System/Manager.h"
#include "../File/Manager.h"
#include "../Input/Manager.h"
#include "../Audio/Manager.h"
#include "../Physics/Manager.h"
#include "../Graphics/Manager.h"
#include "../Effect/Manager.h"
#include "../Scene/Manager.h"
#include "../Scene/SceneGraph.h"
#include "../GUI/Manager.h"
#include "../Game/GameSceneManager.h"
#include "Framework.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{

//=============================================================================
// �� Framework �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    Framework::Framework()
        : mLogFile          ( NULL )
        , mTaskManager      ( NULL )
        , mSystemManager    ( NULL )
        , mFileManager      ( NULL )
        , mInputManager     ( NULL )
        , mAudioManager     ( NULL )
        , mPhysicsManager   ( NULL )
        , mGraphicsManager  ( NULL )
        , mEffectManager    ( NULL )
        , mSceneManager     ( NULL )
        , mGUIManager       ( NULL )
        , mGameManager      ( NULL )
        , mFPSController    ( NULL )
        , mUpdateMode       ( LN_UPDATEMODE_ACTIVE_ONLY )
        , mInitialized      ( false )
        , mEndRequested     ( false )
    {
    }

	//---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
	Framework::~Framework()
    {
        finalize();

        SAFE_DELETE( mFPSController );
       
        LN_SAFE_RELEASE( mGameManager );
        LN_SAFE_RELEASE( mGUIManager );
        LN_SAFE_RELEASE( mSceneManager );
        LN_SAFE_RELEASE( mEffectManager );
        LN_SAFE_RELEASE( mGraphicsManager );
        LN_SAFE_RELEASE( mPhysicsManager );
        LN_SAFE_RELEASE( mAudioManager );
        LN_SAFE_RELEASE( mInputManager );
        LN_SAFE_RELEASE( mFileManager );
        LN_SAFE_RELEASE( mSystemManager );
        LN_SAFE_RELEASE( mTaskManager );
        
        LN_SAFE_RELEASE( mLogFile );
        
        Base::Error::finalize();
        Base::ExceptionManager::finalize();
    }



    //---------------------------------------------------------------------
	// �� ������
    //---------------------------------------------------------------------
    LNRESULT Framework::initialize( const ConfigData& init_data_ )
    {
        if ( mInitialized ) { return LN_ERR_INITIALIZED; }

        mUpdateMode = init_data_.UpdateMode;
        mConfigData = init_data_;

        // wchar_t �œ��{����g����悤�ɂ���
        setlocale( LC_ALL, "" );
        //setlocale( LC_ALL, "ja_JP.UTF-8" );

        // �G���[�������W���[��������
        Base::Error::initialize();
        
        // ���O�t�@�C��
        if ( init_data_.EnableMakeLogFile )
        {
            mLogFile = LN_NEW Base::LogFile();
            mLogFile->initialize( _T( "lnlog.txt" ), init_data_.LogSaveDir );
            Base::Error::setLogFile( mLogFile );
        }

        // ��O�Ǘ�������
        Base::ExceptionManager::initialize( mLogFile );

        mFPSController = LN_NEW Game::FPSController();
        mFPSController->setFrameRate( init_data_.FrameRate );
        mFPSController->setEnableFPSTest( true );

        mInitialized = true;
      
        if ( init_data_.AllInitialize )
        {
            initializeTaskManager();
            initializeSystemManager();
            initializeFileManager();
            initializePhysicsManager();
            
            initializeInputManager();
            initializeAudioManager();

/*
            if (init_data_.GraphicsAPI == LN_GRAPHICSAPI_OPENGL)
            {
                // �`��^�X�N
                mTaskManager->setMasterTask(
                    Base::createDelegate( mSceneManager, &Scene::Manager::renderDefaultScene ),
                    Base::createDelegate( this, &Framework::initializeGraphicsManager ),
                    LN_TASKPHASE_MASTER_RENDER );
            }
            else
            */
            {
                initializeGraphicsManager();
            }

            initializeEffectManager();
            initializeSceneManager();


            GUI::Manager::InitData gui_data;
            gui_data.GraphicsManager    = mGraphicsManager;
            gui_data.VirtualScreenSize  = mConfigData.BackbufferSize;
            mGUIManager = LN_NEW GUI::Manager();
            mGUIManager->initialize( gui_data );

            // TODO:GUI�֘A�t���b��
            mSceneManager->getDefaultSceneGraph()->getDefaultPane()->setGUIDesktop(mGUIManager->getDesktop());

            mGameManager = LN_NEW Game::Manager();
            mGameManager->initialize(this);

            //initializeTaskManager();
            //if ( init_data_.ClassInitializeFlag & LN_CLASS_SYSTEM ) initializeSystemManager();
            //if ( init_data_.ClassInitializeFlag & LN_CLASS_FILE ) initializeFileManager();
            //if ( init_data_.ClassInitializeFlag & LN_CLASS_INPUT ) initializeInputManager();
            //if ( init_data_.ClassInitializeFlag & LN_CLASS_AUDIO ) initializeAudioManager();
            //if ( init_data_.ClassInitializeFlag & LN_CLASS_PHYSICS ) initializePhysicsManager();
            //if ( init_data_.ClassInitializeFlag & LN_CLASS_GRAPHICS ) initializeGraphicsManager();
            //if ( init_data_.ClassInitializeFlag & LN_CLASS_EFFECT ) initializeEffectManager();
            //if ( init_data_.ClassInitializeFlag & LN_CLASS_SCENE ) initializeSceneManager();

            // �G�t�F�N�g�X�V�^�X�N
            mTaskManager->pushConstTask(
                Base::createDelegate( mEffectManager, &Effect::Manager::update ),
                LN_EXTERNAL_SUB_TASK_EXECUTE_PHASE );   // �����Ԃ����������s

            // �����t�F�[�Y�Ŏ��s����^�X�N
            mTaskManager->pushConstTask(
                Base::createDelegate( this, &Framework::_onUpdateSyncPhase ),
                LN_TASKPHASE_UPDATE_RENDERING_CONTENT );

            //if (init_data_.GraphicsAPI != LN_GRAPHICSAPI_OPENGL)
            {
                // �`��^�X�N
                mTaskManager->setMasterTask(
                    Base::createDelegate( mSceneManager, &Scene::Manager::renderDefaultScene ),
                    Base::Delegate00(),//Base::createDelegate( this, &Framework::initializeGraphicsManager ),
                    LN_TASKPHASE_MASTER_RENDER );
            }
        }
        
        _onResetDevice();

        return LN_OK;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Framework::initializeForEditor( const ConfigData& init_data_ )
    {
        mUpdateMode = init_data_.UpdateMode;
        mConfigData = init_data_;

        setlocale( LC_ALL, "" );

        Base::Error::initialize();
        
        if ( init_data_.EnableMakeLogFile )
        {
            mLogFile = LN_NEW Base::LogFile();
            mLogFile->initialize( _T( "lnlog.txt" ), init_data_.LogSaveDir );
            Base::Error::setLogFile( mLogFile );
        }

        Base::ExceptionManager::initialize( mLogFile );

        mFPSController = LN_NEW Game::FPSController();
        mFPSController->setFrameRate( init_data_.FrameRate );
        mFPSController->setEnableFPSTest( true );

        //-----------------------------------------------------
        //

        initializeSystemManager();
        initializeFileManager();
        initializePhysicsManager();
        initializeAudioManager();

        initializeGraphicsManager();
        initializeEffectManager();
        initializeSceneManager();

        _onResetDevice();

        mInitialized = true;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Framework::initializeForAudioLibrary( const ConfigData& init_data )
    {
        mUpdateMode = init_data.UpdateMode;
        mConfigData = init_data;

        setlocale( LC_ALL, "" );

        Base::Error::initialize();
        
        if ( init_data.EnableMakeLogFile )
        {
            mLogFile = LN_NEW Base::LogFile();
            mLogFile->initialize( _T( "lnlog.txt" ), init_data.LogSaveDir );
            Base::Error::setLogFile( mLogFile );
        }

        Base::ExceptionManager::initialize( mLogFile );

        //-----------------------------------------------------
        //

        //initializeSystemManager();
        initializeFileManager();
        initializePhysicsManager();
        initializeAudioManager();

        mInitialized = true;
    }

    //---------------------------------------------------------------------
	// �� �I������
    //---------------------------------------------------------------------
    void Framework::finalize()
    {
        //onLostDevice();

        if ( mTaskManager )
        {
            mTaskManager->finalize();   // �S�ẴX���b�h���~�����邽�߁A�ŏ��ɏ���
        }
        if ( mGameManager )
        {
            mGameManager->finalize();
        }
        if ( mGUIManager )
        {
            mGUIManager->finalize();
        }
        if ( mSceneManager )
        {
            mSceneManager->finalize();
        }
        if ( mEffectManager )
        {
            mEffectManager->finalize();
        }
        if ( mGraphicsManager )
        {
            mGraphicsManager->finalize();
        }
        if ( mPhysicsManager )
        {
            mPhysicsManager->finalize();
        }
        if ( mAudioManager )
        {
            mAudioManager->finalize();
        }
        if ( mInputManager )
        {
            mInputManager->finalize();
        }
        if ( mFileManager )
        {
            mFileManager->finalize();
        }
        if ( mSystemManager )
        {
            mSystemManager->detachEventListener( this );
            mSystemManager->finalize();
        }
        
        
        Base::Error::setErrorObserver( NULL );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void  Framework::initializeTaskManager()
    {
        if ( mTaskManager ) return;

        mTaskManager = LN_NEW Task::Manager();
        mTaskManager->initialize(this);
    }

    //---------------------------------------------------------------------
	// �� �V�X�e�� (OS����̊Ǘ��N���X) �̏�����
    //---------------------------------------------------------------------
    LNRESULT Framework::initializeSystemManager()
    {
        if ( mSystemManager ) return LN_ERR_INITIALIZED;
        System::Manager::InitData data;
        data.LogFile            = mLogFile;
        data.UseConsole         = mConfigData.EnableConsole;
        data.UseThread          = ( mUpdateMode == LN_UPDATEMODE_THREADED );
        data.WindowTitleText    = mConfigData.WindowTitle;
        data.WindowWidth        = mConfigData.WindowWidth;
        data.WindowHeight       = mConfigData.WindowHeight;
        data.Windowed           = !mConfigData.EnableFullscreen;
        data.WindowHandle       = mConfigData.WindowHandle;
#if defined(LNOTE_ANDROID)
        data.AndroidApp         = mConfigData.AndroidApp;
#endif
        mSystemManager  = LN_NEW System::Manager();
        LNRESULT lr = mSystemManager->initialize( data );

        mSystemManager->attachEventListener( this, LN_DEFAULT_EL_PRIORITY_FRAMEWORK );

        return lr;
    }

    //---------------------------------------------------------------------
	// �� �t�@�C���Ǘ��N���X�̏�����
    //---------------------------------------------------------------------
    void Framework::initializeFileManager()
    {
        LN_THROW_Initialized( !mFileManager );

        File::Manager::InitData data;
        data.LogFile        = mLogFile;
        data.ArchiveList    = mConfigData.ArchiveList;
        data.AccessMode     = mConfigData.FileAccessMode;
        mFileManager = LN_NEW File::Manager();
        mFileManager->initialize( data );
    }

    //---------------------------------------------------------------------
	// �� ���͊Ǘ��N���X�̏�����
    //---------------------------------------------------------------------
    LNRESULT Framework::initializeInputManager()
    {
        if ( mInputManager ) return LN_ERR_INITIALIZED;

        LNRESULT lr;

        // ���[�U�[��`�̃E�B���h�E�n���h�����ݒ肳��Ă��āA�V�X�e���Ǘ��N���X�����������̏ꍇ
        if ( mConfigData.WindowHandle && !mSystemManager )
        {
            lr = initializeSystemManager();
            if ( LN_FAILED( lr ) )
            {
                return lr;
            }
        }

        Input::Manager::InitData data;
        data.LogFile    = mLogFile;
        data.Window     = mSystemManager->getWindow();

        mInputManager = LN_NEW Input::Manager();
        LN_CALL_R( mInputManager->initialize( data ) );

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� �I�[�f�B�I�Ǘ��N���X�̏�����
    //---------------------------------------------------------------------
    LNRESULT Framework::initializeAudioManager()
    {
        if ( mAudioManager ) return LN_ERR_INITIALIZED;

        LNRESULT lr;

        if ( !mFileManager )
        {
            initializeFileManager();
        }

        // ���[�U�[��`�̃E�B���h�E�n���h�����ݒ肳��Ă��āA�V�X�e���Ǘ��N���X�����������̏ꍇ
        if ( mConfigData.WindowHandle && !mSystemManager )
        {
            lr = initializeSystemManager();
            if ( LN_FAILED( lr ) )
            {
                return lr;
            }
        }

        Audio::Manager::InitData data;
        data.LogFile                = mLogFile;
        data.FileIOManager          = mFileManager;
        data.AudioSourceCacheSize   = mConfigData.SoundCacheSize;
        data.DMInitMode             = mConfigData.DMInitMode;
        data.Window                 = ( mSystemManager ) ? mSystemManager->getWindow() : NULL;

        mAudioManager = LN_NEW Audio::Manager();
        return mAudioManager->initialize( data );
    }

    //---------------------------------------------------------------------
	// �� �������Z�Ǘ��N���X�̏�����
    //---------------------------------------------------------------------
    LNRESULT Framework::initializePhysicsManager()
    {
        if ( mPhysicsManager ) return LN_ERR_INITIALIZED;
#if !defined(LNOTE_ANDROID)
        Physics::Manager::InitData data;
        data.LogFile            = mLogFile;

        mPhysicsManager = LN_NEW Physics::Manager();
        return mPhysicsManager->initialize( data );
#endif
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� �O���t�B�b�N�X�Ǘ��N���X�̏�����
    //---------------------------------------------------------------------
    void Framework::initializeGraphicsManager()
    {
        if ( mGraphicsManager ) return;

        if ( !mFileManager )
        {
            initializeFileManager();
        }

        if ( !mSystemManager ) //mConfigData.WindowHandle && 
        {
            LNRESULT lr = initializeSystemManager();
            if ( LN_FAILED( lr ) )
            {
                return;
            }
        }

        if ( !mPhysicsManager )
        {
            LNRESULT lr = initializePhysicsManager();
            if ( LN_FAILED( lr ) )
            {
                return;
            }
        }

        Graphics::Manager::InitData data;
        data.LogFile            = mLogFile;
        data.FileManager        = mFileManager;
        data.PhysicsManager     = mPhysicsManager;
        data.SystemManager      = mSystemManager;
        data.TaskManager        = mTaskManager;
        //data.MainWindow         = ( mSystemManager ) ? mSystemManager->getWindow() : NULL;
        data.GraphicsAPI        = mConfigData.GraphicsAPI;
        data.CanvasSize         = mConfigData.BackbufferSize;
        data.EnableFPUPreserve  = mConfigData.EnableFPUPreserve;
        data.FontFilenameList   = mConfigData.FontFilenameList;

        mGraphicsManager = LN_NEW Graphics::Manager();
        mGraphicsManager->initialize( data );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Framework::initializeEffectManager()
    {
        Effect::Manager::InitData data;
        data.LogFile            = mLogFile;
        data.FileManager        = mFileManager;
        data.GraphicsManager    = mGraphicsManager;
        data.EffectCacheSize    = 32;
        data.MaxSpriteNum       = 2048;

        mEffectManager = LN_NEW Effect::Manager();
        mEffectManager->initialize( data );
    }

    //---------------------------------------------------------------------
	// �� �V�[���Ǘ��N���X�̏�����
    //---------------------------------------------------------------------
    LNRESULT Framework::initializeSceneManager()
    {
        if ( mSceneManager ) return LN_ERR_INITIALIZED;

        if ( !mFileManager )
        {
            initializeFileManager();
        }

        if ( !mGraphicsManager )
        {
            initializeGraphicsManager();
        }

        Scene::Manager::InitData data;
        data.LogFile            = mLogFile;
        data.FileManager        = mFileManager;
        data.GraphicsManager    = mGraphicsManager;
        data.EffectManager      = mEffectManager;
        data.PhysicsManager     = mPhysicsManager;
        data.VirtualViewSize    = LVector2( (lnfloat)mConfigData.WindowWidth, (lnfloat)mConfigData.WindowHeight );
        data.ShaderCacheSize    = 32;
        data.EnableDrawThread   = false;//true;

        mSceneManager = LN_NEW Scene::Manager();
        return mSceneManager->initialize( data );
    }

    //---------------------------------------------------------------------
	// �� �t���[���X�V
    //---------------------------------------------------------------------
    bool Framework::update()
    {
        _preUpdate();
        
        return _postUpdate();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Framework::updateAsLibrary()
    {
        mFPSController->processForMeasure();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Framework::resetDelayTime()
    {
        mFPSController->refreshSystemDelay( System::getSystemTime() );
    }

    //---------------------------------------------------------------------
	// �� ���b�Z�[�W�����R�[���o�b�N
    //---------------------------------------------------------------------
    bool Framework::onEvent( const System::Event& args_ )
    {
        LNRESULT lr;

        if ( mInputManager )
        {
            lr = mInputManager->onEvent( args_ );
            if ( LN_EVENT_HANDLED( lr ) )
            {
                return true;
            }
        }

        if ( mGUIManager )
        {
            if ( mGUIManager->doEvent( args_ ) ) return true;
        }

        if ( mSceneManager )
        {
            lr = mSceneManager->onEvent( args_ );
            if ( LN_EVENT_HANDLED( lr ) )
            {
                return true;
            }
        }

        switch ( args_.Type )
        {
            /////////////////////////////////////////////// Alt + Enter
            case LNEV_ALTENTER:
            {
                System::IWindow* main_window = mSystemManager->getWindow();

                mTaskManager->waitMasterTaskWait();
                mGraphicsManager->getGraphicsDevice()->onLostDevice();
                mGraphicsManager->getGraphicsDevice()->onResetDevice();

                if ( mConfigData.ForbidAutoFullscreen )
                {
                    bool b = main_window->isFullScreen();
                    if ( b )
                    {
                        main_window->setEnableFullScreen( false );
                    }
                    else
                    {
                        main_window->setEnableFullScreen( true );
                    }

                    if ( mGraphicsManager )
                    {
                        mGraphicsManager->getGraphicsDevice()->getDefaultCanvas()->setWindowSize( main_window->getSize() );
                           
                        /*
                        if ( b )
                        {
                            mGraphicsManager->getGraphicsDevice()->getDefaultCanvas()->setEnableFullScreen( false );
                        }
                        else
                        {
                            mGraphicsManager->getGraphicsDevice()->getDefaultCanvas()->setEnableFullScreen( true );
                        }
                        */
                    }

                    //LNEvent ev;
                    //ev.Type = LNEV_DEVICE_LOST;
                    //mSystemManager->postEvent( ev );

                    //ev.Type = LNEV_DEVICE_RESET;
                    //mSystemManager->postEvent( ev );

                    return true;
                }
                break;
            }

            /////////////////////////////////////////////// �E�B���h�E���A�N�e�B�u�ɂȂ���
            case LNEV_APP_ACTIVATE:
            {
                if ( mFPSController )
                {
                    mFPSController->refreshSystemDelay( System::getSystemTime() );
                }
                break;
            }

            /////////////////////////////////////////////// Android �̏���������
            //case LNEV_ANDROID_INIT_WINDOW:
            //{
            //    
            //    return true;
            //}

            /////////////////// Android APP_CMD_PAUSE (�f�o�C�X���X�g)
            case LNEV_ANDROID_PAUSED:
            {
                _onLostDevice();
                mGraphicsManager->getGraphicsDevice()->onLostDevice();
                return true;
            }

            /////////////////// Android APP_CMD_RESUME (�f�o�C�X���Z�b�g)
            case LNEV_ANDROID_RESUMED:
            {
                mGraphicsManager->getGraphicsDevice()->onResetDevice();
                _onResetDevice();
                return true;
            }
        }

        return false;
    }
   
    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Framework::OnFrameWait()
    {
        if ( (mConfigData.ClassUpdateFlag & LN_CLASS_FRAMEWORK) && mFPSController )
        {
            mFPSController->process();
        }
        else
        {
            Thread::sleep(16);  // �O�̂���
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Framework::OnUpdateDrawContents()
    {
        // �G�t�F�N�g�`�惊�X�g�X�V
        mEffectManager->updateDrawContents();

        // �`����X�V
        mSceneManager->updateDefaultSceneRenderContent();

        // �f�o�C�X���X�g�̊m�F
        checkDeviceState();

        mGraphicsManager->getDebugText()->commitText();
    }
	

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Framework::_onLostDevice()
    {
		// LNEV_DEVICE_LOST
		mSystemManager->sendEvent(System::createEvent<System::Event>(LNEV_DEVICE_LOST));

        //if ( mGraphicsManager )
        //{
        //    mGraphicsManager->getGraphicsDevice()->onLostDevice();
        //}
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Framework::_onResetDevice()
    {
		

        //if ( mGraphicsManager )
        //{
        //    mGraphicsManager->getGraphicsDevice()->onResetDevice();
        //}
        if ( mInputManager && mGraphicsManager )
        {
            LMatrix m = mGraphicsManager->getGraphicsDevice()->getDefaultCanvas()->getScreenTransform();
            mInputManager->setMouseTransform( m );
        }

		// LNEV_DEVICE_RESET
		mSystemManager->sendEvent(System::createEvent<System::Event>(LNEV_DEVICE_RESET));
    }

    //---------------------------------------------------------------------
    // �� �Q�[���{�̂̍X�V�O�ɏ�������ׂ�����
    //---------------------------------------------------------------------
    void Framework::_preUpdate()
    {
       
        
    }

    //---------------------------------------------------------------------
    // �� �Q�[���{�̂̍X�V��ɏ�������ׂ�����
    //---------------------------------------------------------------------
    bool Framework::_postUpdate()
    {
        bool r = false;

        //if ( (mConfigData.ClassUpdateFlag & LN_CLASS_EFFECT) && mEffectManager )
        //{
        //    mEffectManager->update();
        //}

        

        if ( mEffectManager )
        {
            mEffectManager->setGameTime( mFPSController->getGameTime() );
        }

        if ( (mConfigData.ClassUpdateFlag & LN_CLASS_SCENE) && mSceneManager )
        {

            mSceneManager->updateAndRender( mFPSController->getGameTime() );

            //mSceneManager->renderDefaultScene();
            /*
            Graphics::IRenderer* r = mGraphicsManager->getGraphicsDevice()->getRenderer();

            r->setRenderTarget( NULL, 0 );
            r->beginScene();
            mSceneManager->renderDefaultScene();
            r->endScene();
            mGraphicsManager->getGraphicsDevice()->getDefaultCanvas()->present();
            */
        }
        
        if ( mPhysicsManager )
        {
            mPhysicsManager->update( mFPSController->getGameTime().getElapsedGameTime() );
        }
        
        if ( mGUIManager )
        {
            mGUIManager->update( mFPSController->getGameTime() );
        }

        if ( mTaskManager )
        {
            mTaskManager->beginMakeCommand();
            mTaskManager->endMakeCommand();
            mTaskManager->execute();
        }

        

		

        if ( (mConfigData.ClassUpdateFlag & LN_CLASS_SYSTEM) && mSystemManager )
        {
            // �E�B���h�E���A�N�e�B�u�̂Ƃ������X�V����ꍇ�ŃE�B���h�E����A�N�e�B�u�̏ꍇ�́A���b�Z�[�W�������������đҋ@����
            if ( mUpdateMode == LN_UPDATEMODE_ACTIVE_ONLY )
            {
                while ( !mSystemManager->getWindow()->isActive() )
		        {
			        if ( !mSystemManager->processMessage() || mEndRequested )
                    {
                        return false;
                    }
                    Thread::sleep( 32 );    // �������[�v�̕��׌y��
		        }
            }

            // FPS �l���E�B���h�E�^�C�g���ɕ\������ꍇ
		    if ( mConfigData.EnableShowFPS && mFPSController )
		    {
			    // 1 �b���̃^�C�~���O�Őݒ肷��
			    if ( mFPSController->isRoundSec() )
			    {
                    mSystemManager->getWindow()->setWindowTextFooter( _T( " - fps:%.1f cap:%.1f" ), mFPSController->getFPS(), mFPSController->getCapacityFPS() );
                }
		    }

            r = mSystemManager->processMessage();

			if ( (mConfigData.ClassUpdateFlag & LN_CLASS_INPUT) && mInputManager )
			{
				mInputManager->update();
			}

            return ( mEndRequested ) ? false : r;
        }
        else
        {
            return !mEndRequested;
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Framework::_onUpdateSyncPhase()
    {
        
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Framework::checkDeviceState()
    {
        if ( mGraphicsManager->getGraphicsDevice()->isDeviceLost() )
        {
            _onLostDevice();
            mGraphicsManager->getGraphicsDevice()->resetDevice();
            _onResetDevice();
        }
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

// Framework �C���X�^���X�̍쐬
Framework* LNOTE_STDCALL createFramework()
{
    return LN_NEW Framework();
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================
