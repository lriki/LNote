//=============================================================================
//�y DirectMusic �z
//-----------------------------------------------------------------------------
///**
//  @file       DirectMusic.h
//  @brief      DirectMusic
//  @version    1.0
//  @date       2011/08/04
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "dmusic/dmusici.h"
#include <list>
#include "../../../Thread/Thread.h"
#include "../../../Thread/Mutex.h"
#include "../../../Thread/EventFlag.h"
#include "../../Interface.h"
#include "../AudioSource.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Audio
{
namespace XAudio2
{

//=============================================================================
// �� DirectMusicSegment �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      DirectMusic ��ł̍Đ��V�[�P���X�ЂƂ��������N���X
//
//  @par
//              �����ЂƂ����Đ����邽�߂ɕK�v�Ȉ�A�̃C���^�[�t�F�[�X��
//              �����N���X�ł��B
//              ( Performance�ASegment �����܂Ƃ߂����� )
//              IXAudio2SourceVoice �̂悤�ȃC���[�W�Ŏg����悤�ɂ��Ă���܂��B
//
//  @note
//              DirectMusic �̎d�l��A�p�t�H�[�}���X���쐬����O�ɃZ�O�����g��
//              �쐬���邱�Ƃ��ł���B�X���b�h�� DirectMusic ������������ꍇ�A
//              �������O�ɃZ�O�����g���쐬���Ă����Ύ��Ԃ̐ߖ�Ƃ��ɂȂ邩������Ȃ����ǁA
//              �Z�O�����g���p�t�H�[�}���X������ɍ쐬����ƕs����N����
//              ( �S���s�A�m�����ɂȂ�Ƃ� )
//*/
//=============================================================================
class DirectMusicSegment
    : public Base::NonCopyable
{
public:

    //---------------------------------------------------------------------
	///**
    //  @brief      �R���X�g���N�^
    //
    //  @par
    //              audio_source_ �͕ێ����Ȃ����߁A�Q�ƃJ�E���g�͕ω������܂���B
    //              �������A�R���X�g���N�^�ɓn���O�� fillBuffer() ���Ă�ł����K�v������܂�
    //*/
	//---------------------------------------------------------------------
    DirectMusicSegment( IDirectMusicPerformance8* dm_performance_, const Midi* audio_source_ );

    /// �f�X�g���N�^
    virtual ~DirectMusicSegment();

public:

    /// ���ʂ̐ݒ� ( 100.0 �` 0.0 )
    void setVolume( float volume_ );

    /// �s�b�`�̐ݒ� ( 50 �` 200 ����{�B�͈͊O���ꉞ�B )
    void setPitch( float pitch_ );

    /// �S�̂̍Đ����Ԃ̎擾 ( �~���[�W�b�N�^�C���P�� )
    u32 getTotalTime() const;

    /// �Đ��ʒu�̎擾 ( �~���[�W�b�N�^�C���P�� )
    u32 getPlayPosition() const;

    /// �Đ��ʒu�̐ݒ� ( �~���[�W�b�N�^�C���P�� )
    void setPlayPosition( u32 time_ );

    /// �Đ������𔻒� (�������Ă��邩)
    bool isPlaying() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���[�v���̐ݒ�
    //
    //  @param[in]  is_loop_ : true �̏ꍇ���[�v�Đ�����
    //  @param[in]  begin_   : ���[�v�͈͂̊J�n�ʒu ( �~���[�W�b�N�^�C���P�� )
    //  @param[in]  length_  : ���[�v�͈͂̒���( �~���[�W�b�N�^�C���P�� )
    //
    //  @par
    //              length_ �� 0 �̏ꍇ�� begin_ �̈ʒu����I�[�܂ōĐ����܂��B
    //*/
	//---------------------------------------------------------------------
	void DirectMusicSegment::setLoopState( bool is_loop_, u32 begin_, u32 length_ );

    void play();

    void stop();

private:

    IDirectMusicPerformance8*   mDMPerformance;
    IDirectMusicAudioPath*      mDMAudioPath;
    IDirectMusicSegment8*		mDMSegment;
	IDirectMusicSegmentState8*	mDMSegmentState;
    IDirectSoundBuffer8*        mDSSoundBuffer;
};

//=============================================================================
// �� DirectMusicManager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      DirectMusic �̊Ǘ��N���X
//
//  @par
//              IDirectMusic8 �C���^�[�t�F�C�X�͂ЂƂ̃A�v���P�[�V�����ɂ�
//              �ЂƂ����쐬�ł��Ȃ����߁A���̃N���X�̓V���O���g���Ŏ������Ă܂��B
//              <br>
//              http://msdn.microsoft.com/ja-jp/library/cc353890.aspx <br>
//
//  @note
//              �� DirectMusic �̉��
//                  DirectMusic ���������Ƃ��̓E�B���h�E�����݂��Ă��Ȃ���
//                  Release() �ŉ����������Ȃ� ( �����~�͂��Ȃ��B���������Ńf�b�h���b�N���Ă� )<br>
//                  ������ CoUninitialize ���Ă�ł��܂�����ɉ�����Ă������������B
//                    �E�B���h�E�͂��łɍ폜����Ă��Ă�����͉\�B
//              
//*/
//=============================================================================
class DirectMusicManager
    : private Base::NonCopyable
{
public:

    /// initialize() �ɓn���������f�[�^
    struct InitData
    {
        Base::LogFile*          LogFile;
        HWND                    WindowHandle;
        LNDirectMusicUsingMode  DMInitMode;
    };

    /// �Đ��v�������N���X�̊��
    class PlayerObject
    {
    public:

        PlayerObject(){}

        virtual ~PlayerObject();

        /// �Đ��v�����X�g�ɒǉ�����Ă����Ԃ� DirectMusic �̏����������������Ƃ��ɌĂ΂��
        virtual void onFinishDMInit( IDirectMusicPerformance8* dm_performance_ ) = 0;
    };

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      ������
    //
    //  @param[in]  init_data_ : �������f�[�^
    //
    //  @par
    //              DMInitMode �� LN_DMINITMODE_NOTUSE �̏ꍇ�̓C���X�^���X��
    //              �쐬���܂���BgetInstance() �� NULL ��Ԃ��܂��B
    //*/
	//---------------------------------------------------------------------
	static LNRESULT initialize( const InitData& init_data_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      �I������
    //*/
	//---------------------------------------------------------------------
	static void finalize();

    //---------------------------------------------------------------------
	///**
    //  @brief      �C���X�^���X�ւ̃|�C���^�̎擾
    //*/
	//---------------------------------------------------------------------
    static DirectMusicManager* getInstance() { return sInstance; }

public:

    //---------------------------------------------------------------------
	/**
    //  @brief     ���������������Ă��邩�𔻒肷��
    //*/
	//---------------------------------------------------------------------
    bool isInitialized() const { return mPerformanceInited->isTrue(); }

    //---------------------------------------------------------------------
	/**
    //  @brief      �G���[��Ԃ̎擾
    //
    //  @return     �G���[�ԍ�(�����p)�B ����ȏꍇ�� 0
    //*/
	//---------------------------------------------------------------------
    u32 getErrorState() const;

    //---------------------------------------------------------------------
	/**
    //  @brief      �Đ��v���̒ǉ�
    //
    //  @par
    //              ���ɓ����I�u�W�F�N�g���ǉ�����Ă���ꍇ�͂Ȃɂ����܂���B
    //*/
	//---------------------------------------------------------------------
	void addPlayRequest( PlayerObject* obj_ );

    //---------------------------------------------------------------------
	/**
    //  @brief      �Đ��v���̍폜
    //
    //  @par
    //              DirectMusic �̏��������ɃI�[�f�B�I�v���C���[��������ꂽ
    //              ���ɁA���̊֐��Ń��X�g����O���܂����A
    //              PlayerObject �N���X�̃f�X�g���N�^�ŌĂ΂�邽��
    //              �ʏ�͌Ăяo���K�v�͂���܂���B
    //*/
	//---------------------------------------------------------------------
	void removePlayRequest( PlayerObject* obj_ );

    //---------------------------------------------------------------------
	/**
    //  @brief      IDirectMusicPerformance8 �̍쐬
    //
    //  @par
    //              isInitialized() �ɂ���ď������̊������m�F���Ă���
    //              �Ăяo���Ă��������B
    //*/
	//---------------------------------------------------------------------
    IDirectMusicPerformance8* createDMPerformance();

    //---------------------------------------------------------------------
	/**
    //  @brief     �Đ��v�����X�g���`�F�b�N���āA�\�Ȃ�Đ�����
    //*/
	//---------------------------------------------------------------------
    void polling();

private:

    /// �R���X�g���N�^
    DirectMusicManager();

    /// �f�X�g���N�^
    ~DirectMusicManager();

    /// ������
    LNRESULT _initialize( const InitData& init_data_ );

    /// �ЂƂ߂̃p�t�H�[�}���X�̏����� ( �ʃX���b�h�ŏ���������ꍇ�͂�������Ă΂�� )
	void _initPerformance();

    /// (�Q�ƃJ�E���g�Ȃǂɕω��͂Ȃ�)
    IDirectMusicPerformance8* _createDMPerformance();

private:

    /// �ŏ��� IDirectMusicPerformance8 �̏������X���b�h
    class PerformanceInitThread
        : public Thread::ThreadObject
    {
    public:

        /// �R���X�g���N�^
        PerformanceInitThread()
		{}

        virtual void run()
        {
            CoInitializeEx( NULL, COINIT_MULTITHREADED );

			DirectMusicManager::getInstance()->_initPerformance();
			
			CoUninitialize();
        }
    };

private:

    typedef std::list< PlayerObject* > PlayRequestList;

private:

    HWND				        mWindowHandle;
	LNDirectMusicUsingMode	    mInitMode;
	IDirectSound8*		        mDirectSound;
	IDirectMusic8*		        mDirectMusic;
    IDirectMusicPerformance8*   mFirstPerformance;      ///< �������X���b�h�ɂ���č쐬���ꂽ IDirectMusicPerformance8

    PerformanceInitThread*      mInitThread;            ///< �������X���b�h
    Thread::EventFlag*          mPerformanceInited;     ///< �������ς݃t���O
    Thread::Mutex*              mLock;
    Thread::Mutex*              mListLock;

    PlayRequestList             mPlayRequestList;

    u32                         mErrorState;

    Base::LogFile*              mLogFile;

    static DirectMusicManager* sInstance;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace XAudio2
} // namespace Audio
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================