//=============================================================================
//�y LFramework �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Core/Framework/Framework.h"
#include "LFramework.h"
#include "LConfig.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// �� LFramework �N���X
//=============================================================================

	// �C���X�^���X�̎擾
	LFramework LFramework::getInstance()
	{
		LFramework o;
		o._impl = gFramework;
		return o;
	}

    // �t���[���X�V�񐔂̎擾
    u32 LFramework::getFrameCount() const
    {
        return _impl->getGameTime().getTotalFrameCount();
    }

    // �Q�[���^�C���̎擾
    LGameTime LFramework::getGameTime() const
    {
        return _impl->getGameTime();
    }

    // update() �ōs���������w�肷��
    //void LFramework::setAutoUpdateFlags( u32 flags_ )
    //{
    //    _impl->setAutoUpdateFlags( flags_ );
    //}

    // �t���[���X�V
	bool LFramework::update()
	{
        if ( gGameSceneManager->getActiveGameScene() != NULL ) { return true; }
		return _impl->update();
	}

    // �o�^����Ă���Q�[���V�[�����w�肵�Ď��s�J�n
    void LFramework::run( const lnChar* scene_name_ )
    {
        gGameSceneManager->changeScene( scene_name_ );

        do
        {
            gGameSceneManager->update();
            //_preUpdate();

            //mGameManager->update();

            //if ( !_postUpdate() )
            //{
            //    return;
            //}
        } while ( _impl->update() );

        /*
        _impl->run( "Scene_Test" );
        */
    }

    // �Q�[���V�[����^���Ď��s�J�n
    void LFramework::run( LGameScene* scene_ )
    {
        gGameSceneManager->changeScene( scene_ );

        do
        {
            gGameSceneManager->update();

        } while ( _impl->update() );
    } 

    // �Q�[���V�[�������s�����ǂ����𔻒肷��
    bool LFramework::isRunning() const
    {
        return ( gGameSceneManager->getActiveGameScene() != NULL );
    }

    // ���C�u�����ɏI����v������
    void LFramework::exit()
    {
        _impl->exit();
    }

    // �Q�[���V�[���̃C���X�^���X��o�^����
    void LFramework::registerSceneInstance( LGameScene* scene_ )
    {
        gGameSceneManager->registerSceneInstance( scene_ );
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================