//=============================================================================
//�y GameSceneManager �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "GameScene.h"
#include "../../Core/Framework/Framework.h"
#include "../../Core/Graphics/Manager.h"
#include "../../Core/Graphics/Interface.h"
#include "../../Core/Scene/Manager.h"
#include "GameSceneManager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Utility
{

//=============================================================================
// �� GameSceneManager �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    GameSceneManager::GameSceneManager()
        : mScene            ( NULL )
        , mNextScene        ( NULL )
        , mChangeRequested  ( false )
        , mEndRequested     ( false )
        , mFramework        ( NULL )
        , mShouldAddedScene ( NULL )
    {
    }

	//---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    GameSceneManager::~GameSceneManager()
    {
        finalize();
    }

    //---------------------------------------------------------------------
    // �� ������
    //---------------------------------------------------------------------
    LNRESULT GameSceneManager::initialize( const InitData& data_ )
    {
        mFramework = data_.Framework;

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �I������
    //---------------------------------------------------------------------
    void GameSceneManager::finalize()
    {
        _releaseAllScene();
        _releaseAllAddedScene();

        RegistedGameSceneMap::iterator itr = mRegistedGameSceneMap.begin();
        RegistedGameSceneMap::iterator end = mRegistedGameSceneMap.end();
        for ( ; itr != end; ++itr )
        {
            if ( itr->second )
            {
                itr->second->release();
            }
        }
        mRegistedGameSceneMap.clear();
    }


    //---------------------------------------------------------------------
    // �� �Q�[���V�[���̃C���X�^���X��o�^����
    //---------------------------------------------------------------------
    bool GameSceneManager::registerSceneInstance( GameScene* scene_ )
    {
        if ( scene_->getSceneName() == NULL )
        {
            // �x��
            return false;
        }

        std::pair< RegistedGameSceneMap::iterator, bool > res;
        mRegistedGameSceneMap.insert( RegistedGameScenePair( LSharingKey( scene_->getSceneName() ), scene_ ) );
        return res.second;
    }

    //---------------------------------------------------------------------
    // �� �ʂ̃Q�[���V�[���ɕύX����
    //---------------------------------------------------------------------
    void GameSceneManager::changeScene( GameScene* next_ )
    {
        EventMessage ev;
        ev.Command  = SMC_CHANGE;
        ev.Scene    = next_;
        mMessageQueue.push_back( ev );
    }

    //---------------------------------------------------------------------
    // �� �ʂ̃Q�[���V�[���ɕύX����
    //---------------------------------------------------------------------
    void GameSceneManager::changeScene( const lnChar* scene_name_ )
    {
        RegistedGameSceneMap::iterator itr = mRegistedGameSceneMap.find( LSharingKey( scene_name_ ) );
        if ( itr != mRegistedGameSceneMap.end() )
        {
            changeScene( itr->second );
            itr->second->addRef();
        }
    }

    //---------------------------------------------------------------------
    // �� �V�����Q�[���V�[�����v�b�V������
    //---------------------------------------------------------------------
    void GameSceneManager::pushScene( GameScene* scene_ )
    {
        EventMessage ev;
        ev.Command  = SMC_PUSH;
        ev.Scene    = scene_;
        mMessageQueue.push_back( ev );
    }

    //---------------------------------------------------------------------
    // �� �Q�[���V�[�����v�b�V������
    //---------------------------------------------------------------------
    void GameSceneManager::pushScene( const lnChar* scene_name_ )
    {
        RegistedGameSceneMap::iterator itr = mRegistedGameSceneMap.find( LSharingKey( scene_name_ ) );
        if ( itr != mRegistedGameSceneMap.end() )
        {
            pushScene( itr->second );
            itr->second->addRef();
        }
    }

    //---------------------------------------------------------------------
    // �� ���s���̃V�[�����I�����Đe�V�[���֖߂�
    //---------------------------------------------------------------------
	void GameSceneManager::returnScene( const lnChar* scene_name_ )
    {
        if ( scene_name_ )
        {
            // ���O�̈�v����V�[���܂ł̊K�w���𒲂ׂ�
            int  level = 1;
            bool found = false;
            GameScene* scene = mScene->_lngs.mParent;
            while ( scene )
            {
                if ( _tcscmp( scene->getSceneName(), scene_name_ ) != 0 )
                {
                    ++level;
                    found = true;
                }
                scene = scene->_lngs.mParent;
            }
            // ���������璲�ׂ��K�w�� return
            if ( found )
            {
                EventMessage ev;
                ev.Command  = SMC_RETURN;
                ev.Scene    = NULL;
                for ( int i = 0; i < level; ++i )
                {
                    mMessageQueue.push_back( ev );
                }
            }
        }
        else
        {
            EventMessage ev;
            ev.Command  = SMC_RETURN;
            ev.Scene    = NULL;
            mMessageQueue.push_back( ev );
        }
    }

    //---------------------------------------------------------------------
    // �� ���̃Q�[���V�[�������邩���ׂ�
    //---------------------------------------------------------------------
	bool GameSceneManager::isNextScene() const
    {
        // ���̃V�[��������A����ɂ��ꂪ���݂̃V�[���Ƃ͈Ⴄ�ꍇ�� true ��Ԃ�
		return ( mNextScene != NULL  &&  mNextScene != mScene );
    }

    //---------------------------------------------------------------------
    // �� �t���[���X�V
    //---------------------------------------------------------------------
	bool GameSceneManager::update()
    {
        while ( !mMessageQueue.empty() )
        {
            const EventMessage& ev = mMessageQueue.front();

            switch ( ev.Command )
            {
                /////////////// ���ڑJ��
                case SMC_CHANGE:
                {
                    // ���݂̑S�ẴV�[������� (onTerminate() �Ăяo��)
                    _releaseAllScene();

                    mScene = ev.Scene;
                    if ( mScene )
                    {
                        mScene->_lngs.mParent = NULL;
                        mScene->onStart();
                    }
                    break;
                }
                /////////////// �Ăяo��
                case SMC_PUSH:
                {
                    // �A�N�e�B�u�ȃV�[���̎q�Ƃ��Ēǉ�����
                    ev.Scene->_lngs.mParent = mScene;
                    mScene = ev.Scene;
                    mScene->onStart();
                    break;
                }
                /////////////// �Ăяo�����֖߂�
                case SMC_RETURN:
                {
                    GameScene* old = mScene;
                    mScene = mScene->_lngs.mParent;
                    old->onTerminate();
                    old->release();
                    break;
                }
            }

            mMessageQueue.pop_front();
        }
            
        // ���̎��_�ŃV�[�����Ȃ���ΏI��
        if ( mScene == NULL )
        {
            return false;
        }

        mScene->onUpdate();
        return true;
    }

    //---------------------------------------------------------------------
    // �� �I������
    //---------------------------------------------------------------------
	void GameSceneManager::end()
    {
        mNextScene = NULL;
		mEndRequested = true;
    }

    //---------------------------------------------------------------------
    // �� ���s���̃V�[�����K�w�\�����܂߂� release ����
    //---------------------------------------------------------------------
    void GameSceneManager::_releaseAllScene()
    {
        GameScene* scene = mScene;
        GameScene* tmp;
        while ( scene )
        {
            scene->onTerminate();

            tmp = scene;
            scene = scene->_lngs.mParent;
            tmp->release();
        }
        mScene = NULL;
    }

    //---------------------------------------------------------------------
    // �� �ǉ��҂��q�c���[��S�� release() ����
    //---------------------------------------------------------------------
    void GameSceneManager::_releaseAllAddedScene()
    {
        GameScene* scene = mShouldAddedScene;
        GameScene* tmp;
        while ( scene )
        {
            tmp = scene;
            scene = scene->_lngs.mParent;
            tmp->release();
        }
        mShouldAddedScene = NULL;  
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Utility
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================