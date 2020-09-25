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
#include "../Geometry/Vector2.h"
#include "../Thread/EventFlag.h"
#include "../System/Interface.h"
#include "../Game/GameTime.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
class SceneGraph;
class SceneNode;

//=============================================================================
// �� Manager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�V�[���̊Ǘ��N���X
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

	/// �������f�[�^
	struct InitData
	{
        Base::LogFile*      LogFile;
        File::Manager*      FileManager;
        Graphics::Manager*  GraphicsManager;
        Effect::Manager*    EffectManager;      ///< �f�t�H���g�̃V�[���ɐݒ肳��� EffectManage
        Physics::Manager*   PhysicsManager;     ///< �f�o�b�O�`��p
        LVector2            VirtualViewSize;    ///< �J������2D�v���W�F�N�V�����A3D��2D�̍��W�ϊ��A�V�F�[�_��VIEWPIXELSIZE
        lnU32               ShaderCacheSize;
        bool                EnableDrawThread;
    };

public:
    Manager();
    LN_REFOBJ_METHODS;

protected:
	virtual ~Manager();

public:

    /// ������
    LNRESULT initialize( const InitData& init_data_ );

    /// �I������
    void finalize();

    /// �f�t�H���g�̃V�[�����擾����
	SceneGraph* getDefaultSceneGraph() { return mDefaultScene; }

    /// �`�� (beginScene endScene present �̈�A�̗�����s��)
    LNRESULT updateAndRender( const Game::GameTime& time_ );

    /// �`��ɐ�p�̃X���b�h���g���ꍇ�Atrue ��n��
    void enableMulticoreOptimization( bool f_ ) { mMulticoreOptimization = f_; }


    /// [�`��O�����^�X�N] ������X�V�A�`��R�}���h�\�z�Ȃ�
    void updateDefaultSceneRenderContent();

    /// [�`��^�X�N] �f�t�H���g�̃V�[����`�悷��
    void renderDefaultScene();

    /// ���b�Z�[�W���� (Framework ����Ă΂��)
    LNRESULT onEvent( const System::Event& args_ );

private:

    /// �`�� (�`��X���b�h���g���ꍇ�� DrawThread �N���X����Ă΂��)
	LNRESULT _render();

private:

    class DrawThread;
    friend class DrawThread;

private:

    Base::LogFile*      mLogFile;
    Graphics::Manager*  mGraphicsManager;
    SceneGraph*         mDefaultScene;
    Effect::Manager*    mEffectManager;     ///< �}���`�R�A�œK���̏����̂��߁B���Ԃ�����Ƃ��A������ TaskManager �Ŏ���������

    DrawThread*         mDrawThread;
    Thread::EventFlag*  mEndFlag;
    Thread::EventFlag*  mDrawEnd;
    Thread::EventFlag*  mDrawing;

    Game::GameTime      mGameTime;
    bool                mMulticoreOptimization;
    Thread::EventFlag   mRenderReady;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================