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
#include "../Base/Cache.h"
#include "../Thread/Thread.h"
#include "../Thread/Barrier.h"
#include "../Graphics/Common/SpriteRenderer.h"
#include "../Graphics/Common/RingRenderer.h"
#include "../Graphics/Manager.h"
#include "../Game/GameTime.h"
#include "Instance/EffectInstance.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Effect
{
typedef Base::CacheObjectManager< EffectCore >     EffectCoreCache;

//=============================================================================
// �� Manager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�G�t�F�N�g�̊Ǘ��N���X
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
    , public EffectCoreCache
{
public:

	/// �������f�[�^
	struct InitData
	{
        Base::LogFile*      LogFile;
        File::Manager*      FileManager;
        Graphics::Manager*  GraphicsManager;
        lnU32               EffectCacheSize;
        lnU32               MaxSpriteNum;
    };

public:

    Manager();
    LN_REFOBJ_METHODS;

protected:

	virtual ~Manager();

public:

    /// ������
    void initialize( const InitData& init_data_ );

    /// �I������
    void finalize();

    /// �ێ����Ă���e�`��N���X�ɍs���ݒ肷�� (SceneGraph �Ɗ֘A�t�����Ă���ꍇ�͕K�v�ɉ����ČĂ΂��)
    void setViewProjection( const LMatrix& view_, const LMatrix& proj_, bool is_3d_pahse_ );

    /// ��̃G�t�F�N�g���쐬 (���[�g�m�[�h�̂ݍ쐬)
    //Effect* createEffect();

    /// ��̃G�t�F�N�g�R�A�̍쐬 (���[�g�m�[�h�̂ݍ쐬)
    //EffectCore* createEmptyEffectCore();

    /// �G�t�F�N�g�V�X�e���Ɗ֘A�t�����Ă��� 3D �X�v���C�g�`��N���X�̎擾
    Graphics::SpriteRenderer* getSprite3DRenderer() { return mSprite3DRenderer; }

    /// �G�t�F�N�g�V�X�e���Ɗ֘A�t�����Ă��� 2D �X�v���C�g�`��N���X�̎擾
    Graphics::SpriteRenderer* getSprite2DRenderer() { return mSprite2DRenderer; }

    Graphics::RingRenderer* getRingRenderer() { return mRingRenderer; }

    /// ���Ԃ̃X�P�[���W���̐ݒ� (�ʏ�̓t���[�����[�g�̒l�����̂܂ܓn���Ă����B�f�t�H���g�l�� 60�BsetTime() �Ŏ��Ԃ̒l�ɏ�Z�����)
    void setTimeTickPerSec( lnfloat tick_ ) { mTimeTick = tick_; }

    /// ���Ԃ̃X�P�[���W���̎擾
    lnfloat getTimeTickPerSec() const { return mTimeTick; }

    /// ���Ԃ�i�߂� (�ʏ�A�b�P�ʂœn��)
    //void advanceTime( lnTime delta_time_ );

    /// �Q�[���^�C���̐ݒ� (��{�I�ɖ����[�v����)
    void setGameTime( const Game::GameTime& game_time_ );

    /// [�X�V�^�X�N] �e�G�t�F�N�g���X�V����
    void update();

    /// [�`����\�z�^�X�N] �`�惊�X�g���X�V���� (update() �̌�ŌĂԂ���)
    void updateDrawContents();


    ///// �G�t�F�N�g�`��J�n
    //void beginDraw();

    ///// �G�t�F�N�g�`��I��
    //void endDraw();

    /// �o�b�`�`�� (3D ���)
    void drawBatch3D();

    /// �o�b�`�`�� (2D ���)
    void drawBatch2D();

public:

    File::Manager* getFileManager() { return mFileManager; }

    Graphics::Manager* getGraphicsManager() { return mGraphicsManager; }

    /// (EffectInstance �̃R���X�g���N�^����Ă΂��)
    void addEffectInstance( EffectInstance* effect_ );

    /// (EffectInstance �̃f�X�g���N�^����Ă΂��)
    void removeEffectInstance( EffectInstance* effect_ );

    /// ���ׂĂ̗v�f�̏C���ς݃t���O�ݒ肷��
    //void setAllModifiedFlag( bool flag_ );

private:

    /// �e�G�t�F�N�g���X�V����
    void _update();

private:

    typedef std::vector< EffectInstance* >  EffectInstanceArray;
    typedef std::list< EffectInstance* >    EffectInstanceList;

    /// ���s�X���b�h
    class UpdateThread
        : public Thread::ThreadObject
    {
    public:

        /// �R���X�g���N�^
        UpdateThread( Manager* manager_ ) : mManager( manager_ ) {}

        /// �X���b�h�֐�
        void run();

    private:

        Manager*    mManager;
    };

private:

    Base::LogFile*              mLogFile;
    File::Manager*              mFileManager;
    Graphics::Manager*          mGraphicsManager;
    Graphics::SpriteRenderer*   mSprite3DRenderer;
    Graphics::SpriteRenderer*   mSprite2DRenderer;
    Graphics::RingRenderer*     mRingRenderer;

    EffectInstanceArray         mEffectInstanceArray;
    EffectInstanceList          mAddedEffectInstanceList;
    EffectInstanceArray         mDrawingInstanceArray;

    //Thread::Barrier*            mASyncBarrier;
    //Thread::EventFlag*          mASyncUpdating;
    //Thread::EventFlag*          mEndRequested;
    //UpdateThread*               mUpdateThread;

    //lnTime          mTime;
    Game::GameTime  mGameTime;
    lnfloat         mTimeTick;

    friend class UpdateThread;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================