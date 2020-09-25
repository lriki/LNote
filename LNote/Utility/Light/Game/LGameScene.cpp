//=============================================================================
//�y LGameScene �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Core/Framework/Framework.h"
#include "../Accessor.h"
#include "LGameScene.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// �� LGameScene �N���X
//=============================================================================

    // �R���X�g���N�^
    LGameScene::LGameScene()
        : Utility::GameScene( gGameSceneManager )
    {
    }

    // �V�[���̕ύX
    void LGameScene::changeScene( const lnChar* scene_name_ )
    {
        Utility::GameScene::changeScene( scene_name_ );
    }

    // �V�[���̕ύX
    void LGameScene::changeScene( LGameScene* next_scene_ )
    {
        Utility::GameScene::changeScene( next_scene_ );
    }

    // �V�[���̌Ăяo��
    void LGameScene::callScene( const lnChar* scene_name_ )
    {
        Utility::GameScene::callScene( scene_name_ );
    }

    // �V�[���̌Ăяo��
    void LGameScene::callScene( LGameScene* next_scene_ )
    {
        Utility::GameScene::callScene( next_scene_ );
    }

    // �Ăяo�����̃V�[�� (�e�V�[��) �֖߂�
    void LGameScene::returnScene( const lnChar* parent_name_ )
    {
        Utility::GameScene::returnScene( parent_name_ );
    }

    // �A�N�e�B�u�ȃV�[�����𔻒肷��
    bool LGameScene::isActiveScene() const
    {
        return Utility::GameScene::isActiveScene();
    }

    // �e�Q�[���V�[���̎擾
    LGameScene* LGameScene::getParentScene() const
    {
        return static_cast< LGameScene* >( Utility::GameScene::getParentScene() );
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================