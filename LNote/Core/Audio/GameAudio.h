//=============================================================================
//�y GameAudio �z
//-----------------------------------------------------------------------------
///**
//  @file       GameAudio.h
//  @brief      GameAudio
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Geometry/Vector3.h"
#include "../Thread/Mutex.h"
#include "Interface.h"
#include "Sound.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Audio
{
class Manager;

//=============================================================================
// �� GameAudio �N���X
//-----------------------------------------------------------------------------
///**
//	@brief      �Q�[���p�̉����̉��t�������N���X
//
//  @par
//              ���̃N���X�̃C���X�^���X�� Engine::Audio::Manager �N���X����擾���܂��B
//*/
//=============================================================================
class GameAudio
    : public Base::ReferenceObject
    , public IGameAudio
{
public:

	/// �R���X�g���N�^
	GameAudio( Manager* mamager_ );

	/// �f�X�g���N�^
	~GameAudio();

    LN_REFOBJ_METHODS;

public:

    /// �Ǘ��N���X�̎擾
    Manager* getManager() const { return mManager; }

    /// BGM �����t����
	virtual LNRESULT playBGM( const lnChar* filename_, int volume_ = 100, int pitch_ = 100, int fade_time_ = 0 );

    /// �w�肳�ꂽ ISound �� BGM �Ƃ��ĉ��t����
	virtual LNRESULT playBGMFromSound( ISound* sound_, int volume_ = 100, int pitch_ = 100  , int fade_time_ = 0 );

    /// BGM �̉��t���~����
	virtual void stopBGM( int fade_time_ = 0 );

    /// BGS ( ���� ) �����t����
	virtual LNRESULT playBGS( const lnChar* filename_, int volume_ = 100, int pitch_ = 100, int fade_time_ = 0 );

    /// �w�肳�ꂽ ISound �� BGS �Ƃ��ĉ��t����
	virtual LNRESULT playBGSFromSound( ISound* sound_, int volume_ = 100, int pitch_ = 100  , int fade_time_ = 0 );

    /// BGS �̉��t���~����
	virtual void stopBGS( int fade_time_ = 0 );

    // ME ( ���ʉ��y ) �����t����
	virtual LNRESULT playME( const lnChar* filename_, int volume_ = 100, int pitch_ = 100 );

    /// �w�肳�ꂽ ISound �� ME �Ƃ��ĉ��t����
	virtual LNRESULT playMEFromSound( ISound* sound_, int volume_ = 100, int pitch_ = 100 );

    /// ME �̉��t���~����
	virtual void stopME();

    /// SE �����t����
	virtual LNRESULT playSE( const lnChar* filename_, int volume_ = 100, int pitch_ = 100 );

    /// SE �� 3D ��ԏ�ŉ��t����
    virtual LNRESULT playSE( const lnChar* filename_, const LVector3& position_, float distance, int volume_ = 100, int pitch_ = 100 );

    /// �w�肳�ꂽ ISound �� SE �Ƃ��ĉ��t����
	virtual LNRESULT playSEFromSound( ISound* sound_, int volume_ = 100, int pitch_ = 100 );

    /// �S�Ă� SE �̉��t���~����
	virtual void stopSE();

    /// ME ���t���� BGM �̃t�F�[�h���Ԃ�ݒ肷��
    virtual void setMEFadeState( int begin_, int end_ );

    /// ���� BGM �̉��t�ĊJ�t���O�̐ݒ�
    virtual void setEnableBGMRestart( bool flag_ ) { mBGMRestart = flag_; }

    /// ���� BGS �̉��t�ĊJ�t���O�̐ݒ�
	virtual void setEnableBGSRestart( bool flag_ ) { mBGSRestart = flag_; }

public:

    /// �X�V
	LNRESULT polling();
    
private:

    /// �Đ��I����������X�g�ɒǉ�����
    void _pushReleaseAtPlayEndList( ISound* sound_ );

private:

    typedef std::list< ISound* >    ReleaseAtPlayEndList;

public:

	Manager*	                mManager;
    Thread::Mutex*              mLock;
    ReleaseAtPlayEndList        mReleaseAtPlayEndList;  ///< �Đ��I�����ɉ�����鉹�����X�g
	ISound*		                mBGM;
    ISound*		                mBGS;
    ISound*                     mME;
    ln_std_tstring              mBGMName;
    ln_std_tstring              mBGSName;
    int                         mBGMVolume;
    int                         mBGMPitch;
    int                         mBGSVolume;
    int                         mBGSPitch;
    int                         mBGMFadeOutTime;
    int                         mBGMFadeInTime;
    bool                        mMEPlaying;
    bool                        mBGMRestart;
    bool                        mBGSRestart;
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