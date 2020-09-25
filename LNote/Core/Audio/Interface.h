//=============================================================================
//�y Audio �z
//-----------------------------------------------------------------------------
///**
//  @file       Audio.h
//  @brief      Audio
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Base/SharingKey.h"
#include "../Geometry/Vector3.h"
#include "../File/Interface.h"
#include "Types.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Audio
{
class ISound;
class IGameAudio;
class IAudioDevice;
class IResourceManager;
class IAudioSource;
class IAudioPlayer;

/// 3D �T�E���h���X�i�[
struct LN3DSoundListener
{
    LVector3    Position;
    LVector3    Direction;
    LVector3    UpDirection;
    LVector3    Velocity;

    LN3DSoundListener()
        : Direction( 0, 0, 1.0f )
        , UpDirection( 0, 1.0f, 0 )
    {}
};

//=============================================================================
// �� IManager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �I�[�f�B�I�@�\�̊Ǘ��C���^�[�t�F�C�X
//*/
//=============================================================================
class IManager
    : public Base::Interface
{
public:

    /// GameAudio �N���X�̎擾
    virtual IGameAudio* getGameAudio() = 0;

    /// �f�o�C�X�N���X�̎擾
    virtual IAudioDevice* getAudioDevice() = 0;

    /// ���\�[�X�Ǘ��N���X�̎擾
    virtual IResourceManager* getResourceManager() = 0;

    /// ISound �̍쐬 (stream_ = NULL �ŃL�[���g�������������s��)
    virtual LNRESULT createSound( ISound** sound_, File::IInStream* stream_, LNSoundPlayType type_, bool enable_3d_, Base::SharingKey key_ ) = 0;

    /// ISound �̍쐬 (�t�@�C�����w��)
    virtual LNRESULT createSound( ISound** sound_, const lnChar* filename_, LNSoundPlayType type_, bool enable_3d_ ) = 0;

    /// ISound �̍쐬 (IAudioSource �w��)
    virtual LNRESULT createSound( ISound** sound_, IAudioSource* source_, LNSoundPlayType type_, bool enable_3d_ ) = 0;

    /// �O���[�v�̒�~
    virtual void stopGroup( u32 group_ ) = 0;

protected:

    virtual ~IManager() {}
};

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
class IGameAudio
    : public Base::Interface
{
public:

    //---------------------------------------------------------------------
	///**
    //  @brief      BGM �����t����
    //
    //  @param[in]  filename_  : �t�@�C����
    //  @param[in]  volume_    : ����   (  0 �` 100 )
    //  @param[in]  pitch_     : �s�b�` ( 50 �` 200 )
    //  @param[in]  fade_time_ : �t�F�[�h�C���ɂ����鎞�� ( �~���b )
    //
    //  @par
    //              filename_ �̉����t�@�C����ǂݍ���� BGM �Ƃ��ĉ��t���܂��B     <br>
    //              BGM �͂ЂƂ������t���邱�Ƃ��ł��A���[�v�Đ����܂��B          <br>
    //              ���� BGM �����t����Ă���ꍇ�͒�~���āA�V����BGM�����t���܂��B<br>
    //                                                                              <br>
    //              fade_time_ �̓t�F�[�h�C���ɂ����鎞�Ԃł��B                     <br>
    //              ���� BGM �̉��t���Ƀt�F�[�h�C�����悤�Ƃ����ꍇ�A               <br>
    //              ���t���� BGM �ƃN���X�t�F�[�h���Ȃ��牉�t���J�n���܂��B         <br>
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT playBGM( const lnChar* filename_, int volume_ = 100, int pitch_ = 100, int fade_time_ = 0 ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      BGM �̉��t���~����
    //
    //  @param[in]  fade_time_ : �t�F�[�h�A�E�g�ɂ����鎞�� ( �~���b )
    //*/
    //---------------------------------------------------------------------
	virtual void stopBGM( int fade_time_ = 0 ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      BGS ( ���� ) �����t����
    //
    //  @param[in]  filename_  : �t�@�C����
    //  @param[in]  volume_    : ����   (  0 �` 100 )
    //  @param[in]  pitch_     : �s�b�` ( 50 �` 200 )
    //  @param[in]  fade_time_ : �t�F�[�h�C���ɂ����鎞�� ( �~���b )
    //
    //  @par
    //              BGM �Ɠ������@�ōĐ�����܂��B( �ЂƂ������t�A���[�v�Đ� )    <br>
    //              BGS �� BGM �Ɠ����ɉ��t���邱�Ƃ��ł��AME ( ���ʉ��y ) �ɂ��   <br>
    //              �e�����󂯂��A��ɉ��t����܂��B
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT playBGS( const lnChar* filename_, int volume_ = 100, int pitch_ = 100, int fade_time_ = 0 ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      BGS �̉��t���~����
    //
    //  @param[in]  fade_time_ : �t�F�[�h�A�E�g�ɂ����鎞�� ( �~���b )
    //*/
    //---------------------------------------------------------------------
	virtual void stopBGS( int fade_time_ = 0 ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ME ( ���ʉ��y ) �����t����
    //
    //  @param[in]  filename_  : �t�@�C����
    //  @param[in]  volume_    : ����   (  0 �` 100 )
    //  @param[in]  pitch_     : �s�b�` ( 50 �` 200 )
    //
    //  @par
    //              filename_ �̉����t�@�C����ǂݍ���� ME �Ƃ��ĉ��t���܂��B      <br>
    //              ���x���A�b�v��A�C�e���擾���̃t�@���t�@�[�����̉��t�Ɏg���܂��B<br>
    //              ME �͂ЂƂ������t����A���[�v�Đ��͂��܂���B
    //              ���� ME �����t����Ă���ꍇ�A����͒�~����܂��B             <br>
    //                                                                              <br>
    //              ME �̉��t�J�n���� BGM �����t����Ă����ꍇ�� BGM �� 
    //              500 �~���b�����Ĉꎞ��~�����A���̌�� ME �̉��t���J�n���܂��B  <br>
    //              ���̏ꍇ�AME �̉��t�I����� BGM �̉��t���ĊJ���A
    //              2000 �~���b������ BGM ���t�F�[�h�C�������܂��B                  <br>
    //                                                                              <br>
    //              ���̃t�F�[�h�C���A�t�F�[�h�A�E�g�ɂ����鎞�Ԃ�
    //              setMEFadeState() �ɂ���Đݒ肷�邱�Ƃ��ł��܂��B
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT playME( const lnChar* filename_, int volume_ = 100, int pitch_ = 100 ) = 0;
    
    //---------------------------------------------------------------------
    ///**
    //  @brief      ME �̉��t���~����
    //
    //  @par
    //              BGM �̈ꎞ��~���� ME ���~�����ꍇ�� playME() �̋L�q��
    //              �����悤�� BGM ���t�F�[�h�C�������Ȃ���ĊJ�����܂��B
    //*/
    //---------------------------------------------------------------------
	virtual void stopME() = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      SE �����t����
    //
    //  @param[in]  filename_  : �t�@�C����
    //  @param[in]  volume_    : ����   (  0 �` 100 )
    //  @param[in]  pitch_     : �s�b�` ( 50 �` 200 )
    //
    //  @par
    //              filename_ �̉����t�@�C����ǂݍ���� SE �Ƃ��ĉ��t���܂��B  <br>
    //              SE �͓����ɕ����Đ����邱�Ƃ��ł��A�������������t�����ꍇ��
    //              �d�˂�������ĉ��t����܂��B                                <br>
    //                                                                          <br>
    //              �܂��ASE �͕K���I���������ōĐ�����܂��B
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT playSE( const lnChar* filename_, int volume_ = 100, int pitch_ = 100 ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      SE �� 3D ��ԏ�ŉ��t����
    //
    //  @param[in]  filename_  : �t�@�C����
    //  @param[in]  position_  : �����̈ʒu
    //  @param[in]  volume_    : ����   (  0 �` 100 )
    //  @param[in]  pitch_     : �s�b�` ( 50 �` 200 )
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT playSE( const lnChar* filename_, const LVector3& position_, float distance, int volume_ = 100, int pitch_ = 100 ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �S�Ă� SE �̉��t���~����
    //*/
    //---------------------------------------------------------------------
	virtual void stopSE() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ME ���t���� BGM �̃t�F�[�h���Ԃ�ݒ肷��
    //
    //  @param[in]  begin_ : ME ���t�J�n���� BGM �̃t�F�[�h�A�E�g���� ( �~���b )
    //  @param[in]  end_   : ME ���t�I����� BGM �̃t�F�[�h�C������ ( �~���b )
    //
    //  @par
    //              -1 ���w�肵�����ڂ͒l��ύX���܂���B
    //*/
    //---------------------------------------------------------------------
    virtual void setMEFadeState( int begin_, int end_ ) = 0;


    //---------------------------------------------------------------------
	///**
    //  @brief      �w�肳�ꂽ ISound �� BGM �Ƃ��ĉ��t����
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT playBGMFromSound( ISound* sound_, int volume_ = 100, int pitch_ = 100  , int fade_time_ = 0 ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      �w�肳�ꂽ ISound �� BGS �Ƃ��ĉ��t����
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT playBGSFromSound( ISound* sound_, int volume_ = 100, int pitch_ = 100  , int fade_time_ = 0 ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      �w�肳�ꂽ ISound �� ME �Ƃ��ĉ��t����
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT playMEFromSound( ISound* sound_, int volume_ = 100, int pitch_ = 100 ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      �w�肳�ꂽ ISound �� SE �Ƃ��ĉ��t����
    //
    //  @par
    //              �����̃R�s�[���쐬���āA��������t���܂��B
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT playSEFromSound( ISound* sound_, int volume_ = 100, int pitch_ = 100 ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      ���� BGM �̉��t�ĊJ�t���O�̐ݒ�
    //
    //  @par
    //              playBGM �Ō��݉��t���� BGM �Ɠ����t�@�C���� (�܂��̓T�E���h) ���A
    //              �����{�����[���A�s�b�`�ŉ��t���悤�Ƃ����Ƃ��ɁA���̂܂܉��t�𑱂��邩�A
    //              �Ȃ̐擪�ɖ߂��ĉ��t���Ȃ�������ݒ肵�܂��B
    //              �f�t�H���g�� false (���t�𑱂���) �ł��B
    //*/
    //---------------------------------------------------------------------
	virtual void setEnableBGMRestart( bool flag_ ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      ���� BGS �̉��t�ĊJ�t���O�̐ݒ�
    //*/
    //---------------------------------------------------------------------
	virtual void setEnableBGSRestart( bool flag_ ) = 0;

protected:

    virtual ~IGameAudio() {}
};

//=============================================================================
// �� ISound �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �����Ƃ��̍Đ��������N���X�̃C���^�[�t�F�[�X
//
//  @par
//              IAudioPlayer �� IAudioSource �̃��b�p�[�ł��B
//*/
//=============================================================================
class ISound
    : public Base::Interface
{
public:

	/// ���ʂ̎擾
	virtual int getVolume() = 0;

	/// ���ʂ̐ݒ�
	virtual void setVolume( int volume_ ) = 0;

	/// �s�b�`�̎擾
	virtual int getPitch() = 0;

	/// �s�b�`�̐ݒ�
	virtual void setPitch( int pitch_ ) = 0;

	///// �Đ������𔻒肷��
	//virtual bool isPlaying() = 0;

	///// �ꎞ��~�����𔻒肷��
	//virtual bool isPausing() = 0;

    /// �T�E���h�̏�Ԃ��擾����
    virtual void getState( LNAudioPlayState* state_ ) = 0;

    /// ���[�v�Đ��̗L���ݒ�
    virtual void loop( bool loop_ ) = 0;

    /// ���[�v�Đ����L�����𔻒肷��
    virtual bool isLoop() = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief		���[�v�̈�̐ݒ�
    //
    //  @param[in]  begin_  : ���[�v�̈�̐擪�ʒu ( �T���v�����P�� )
    //  @param[in]  length_ : ���[�v�̈撷�� ( �T���v�����P�� )
    //
    //  @par
    //              begin_ �� length_ �� 0 ��ݒ肷��ƑS�̂����[�v�̈�ɂȂ�܂��B
    //*/
    //---------------------------------------------------------------------
	virtual void setLoopState( u32 begin_, u32 length_ ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      �Đ�
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT play() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ��~
    //*/
    //---------------------------------------------------------------------
	virtual void stop() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �ꎞ��~
    //
    //  @param[in]  is_pause_ : true �̏ꍇ�ꎞ��~���� (��������ɂ� false ��n���܂�)
    //*/
    //---------------------------------------------------------------------
	virtual void pause( bool is_pause_ ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      ���ʂ̃t�F�[�h
    //
    //  @param[in]  target_volume_ : �ύX��̉���
    //  @param[in]  time_          : �t�F�[�h�ɂ����鎞�� (�~���b)
    //  @param[in]  state_         : �t�F�[�h������̓���̎w��					
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT fadeVolume( int target_volume_, int time_, LNSoundFadeState state_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief		3D �������𔻒肷��
    //*/
    //---------------------------------------------------------------------
    virtual bool is3DSound() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief		3D �����Ƃ��Ă̈ʒu��ݒ肷��
    //
    //  @param[in]  pos_ : �ʒu
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT setPosition( const LVector3& pos_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief		3D �����Ƃ��Ă̈ʒu���擾����
    //*/
    //---------------------------------------------------------------------
    virtual const LVector3& getPosition() = 0;

    /// ���x�̐ݒ�
    virtual void setVelocity( const LVector3& pos_ ) = 0;

    /// ���������̐ݒ�
    virtual void setMaxDistance( lnfloat distance ) = 0;

    /// �����Ɏ����Ă��鉹���Đ��N���X�̎擾
    virtual IAudioPlayer* getAudioPlayer() = 0;

	virtual void setGroupFlag( u32 flag_ ) = 0;

    virtual u32 getGroupFlag() = 0;

protected:

    virtual ~ISound() {};
};



//=============================================================================
// �� IAudioDevice �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �I�[�f�B�I�̊Ǘ��N���X�̃C���^�[�t�F�[�X
//*/
//=============================================================================
class IAudioDevice
    : public Base::Interface
{
public:

    /// IAudioPlayer ���쐬���� (type_ �� LN_SOUNDPLAYTYPE_AUTO �͎w��ł��Ȃ��̂Œ���)
    virtual LNRESULT createAudioPlayer( IAudioPlayer** obj_, IAudioSource* source_, bool enable_3d_, LNSoundPlayType type_ ) = 0;

    /// 3D�T�E���h���X�i�[�̎擾
    virtual LN3DSoundListener* get3DSoundListener() = 0;

    /// �X�V (�X�V�X���b�h����Ă΂��)
    virtual void update() = 0;

    /// 3D ��Ԃ�1���[�g�������̋����̐ݒ�
    virtual lnfloat setMetreUnitDistance( lnfloat d ) = 0;

protected:

    virtual ~IAudioDevice() {};

};

//=============================================================================
// �� IResourceManager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �������\�[�X�Ǘ��̃C���^�[�t�F�C�X
//*/
//=============================================================================
class IResourceManager
    : public Base::Interface
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L�[�ɑΉ�����I�[�f�B�I�\�[�X����������
    //
    //  @param[out] obj_ : ���������I�[�f�B�I�\�[�X�̃|�C���^���i�[����ϐ��̃A�h���X ( ������Ȃ������ꍇ�� NULL ���i�[����܂� )
    //  @param[in]  key_ : �����Ɏg���L�[�N���X
    //
    //  @par
    //              ���������ꍇ�͎Q�ƃJ�E���g���ЂƂ��₵�ĕԂ��܂��B
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT findAudioSource( IAudioSource** obj_, Base::SharingKey key_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �I�[�f�B�I�\�[�X�̍쐬
    //
    //  @param[out] obj_    : �쐬�����I�[�f�B�I�\�[�X�̃|�C���^���i�[����ϐ��̃A�h���X
    //  @param[in]  stream_ : ���̓X�g���[��
    //  @param[in]  key_    : �����f�[�^�����L���鎞�Ɏg���L�[
    //
    //  @par
    //              �쐬�ł����ꍇ�Astream_ �� AudioSource �ɂ���ĎQ�Ƃ���A
    //              �Q�ƃJ�E���g���ЂƂ����܂��B
    //              <br>
    //              ����ɍ쐬�ł����ꍇ�Astream_ �̃t�@�C���|�C���^�͐擪�ɖ߂�܂��B<br>
    //
    //  @attention
    //              �L�[���w�肷��ꍇ�͕K�� findAudioSource() �ŁA���łɑ��݂��Ă��邩
    //              �`�F�b�N���Ă��������B�d������������ƃ��������[�N���܂��B
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT createAudioSource( IAudioSource** obj_, File::IInStream* stream_, Base::SharingKey key_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L���b�V�����N���A����
    //*/
    //---------------------------------------------------------------------
    virtual void clearCache() = 0;

protected:

    virtual ~IResourceManager() {}
};

//=============================================================================
// �� IAudioPlayer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �����Đ��N���X�̃C���^�[�t�F�[�X
//*/
//=============================================================================
class IAudioPlayer
    : public Base::Interface
{
public:

    /// IAudioSource �̎擾
	virtual IAudioSource* getAudioSource() const = 0;

	/// ���ʂ̎擾
	virtual int getVolume() const = 0;

	/// ���ʂ̐ݒ�
	virtual LNRESULT setVolume( int volume_ ) = 0;

	/// �s�b�`�̎擾
	virtual int getPitch() const = 0;

	/// �s�b�`�̐ݒ�
	virtual LNRESULT setPitch( int pitch_ ) = 0;

    /// �Đ������T���v�����̎擾 ( Midi �̏ꍇ�̓~���[�W�b�N�^�C�� )
    virtual u64 getPlayedSamples() const = 0;

    /// �Đ������� (�폜�\��)
	virtual bool isPlaying() const = 0;

	// �ꎞ��~������ (�폜�\��)
	virtual bool isPausing() const = 0;

    /// �Đ���Ԃ̎擾
    virtual LNSoundPlayState getPlayState() const = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      ���[�v�Đ��̗L���ݒ�
    //
    //  @param[in]  loop_ : ���[�v�Đ�����ꍇ�Atrue ��ݒ肷��
    //*/
    //---------------------------------------------------------------------
	virtual void loop( bool loop_ ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      ���[�v�Đ����L�����𔻒肷��
    //*/
    //---------------------------------------------------------------------
	virtual bool isLoop() = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      ���[�v���̐ݒ�
    //
    //  @param[in]  loop_begin_  : ���[�v�����̈�̍ŏ��̃T���v��
    //  @param[in]  loop_length_ : ���[�v�̈�̒��� (�T���v�����P��)
    //   
    //  @par
    //              �S�̂����[�v�Đ�����ꍇ�� loop_begin_ �� loop_length_ ��
    //              0 ��ݒ肵�Ă��������B
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT setLoopState( u32 loop_begin_, u32 loop_length_ ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief		�Đ�
    //
    //  @note
    //              �E�擪����J�n			<br>
    //              �E�|�[�Y����			<br>
    //              �E�t�F�[�h��ԏ�����	<br>
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT play() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ��~
    // 
    //  @note
    //              �E�|�[�Y����
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT stop() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief		�ꎞ��~
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT pause( bool is_pause_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �|�[�����O�X�V
    // 
    //  @par
    //              �Đ��I��(��~)���Ă���ꍇ�� true ��Ԃ��܂��B
    //              ���[�v�Đ��̏ꍇ�͏�� true ��Ԃ��܂��B
    //*/
    //---------------------------------------------------------------------
	virtual bool polling() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief		3D �������𔻒肷��
    //*/
    //---------------------------------------------------------------------
    virtual bool is3DSound() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief		3D �����Ƃ��Ă̈ʒu��ݒ肷��
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT setPosition( const LVector3& pos_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief		3D �����Ƃ��Ă̈ʒu���擾����
    //*/
    //---------------------------------------------------------------------
    virtual const LVector3& getPosition() = 0;

    /// ���x�̐ݒ�
    virtual void setVelocity( const LVector3& v ) = 0;

    /// 3D �����̉e���͈� (���̓͂�����) �̐ݒ�
    virtual void setEmitterDistance( lnfloat distance_ ) = 0;

    /// 3D �����̉e���͈� (���̓͂�����) �̎擾
    virtual lnfloat getEmitterDistance() const = 0;

public:

	virtual ~IAudioPlayer() {}
};


//=============================================================================
// �� IAudioSource �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �����f�[�^�̃C���^�[�t�F�C�X
//*/
//=============================================================================
class IAudioSource
    : public Base::Interface
{
public:

	/// �t�@�C���t�H�[�}�b�g�̎擾
	virtual LNAudioSourceFormat getSourceFormat() const = 0;

	/// PCM �t�H�[�}�b�g�̎擾
	virtual const LNWaveFormat* getWaveFormat() const = 0;

	/// ���f�[�^�̃T�C�Y�̎擾 ( �X�g���[�~���O�Đ��ł̏I�����蓙�Ŏg�� )
	virtual u32 getSourceDataSize() const = 0;

	/// �S�̂̍Đ����Ԃ̎擾 ( �~���b ��Ŗ����Ȃ邩�� )
	virtual u32 getTotalTime() const = 0;

    /// �S�̂̃T���v�����̎擾 ( Midi �̏ꍇ�̓~���[�W�b�N�^�C���P�� )
	virtual u32 getTotalUnits() const = 0;

	/// �I���������Đ��p�̃o�b�t�@�̐擪�A�h���X�擾 ( fillBufferAndReleaseStream() ���Ă�ł��Ȃ��ꍇ�� NULL )
	virtual u8* getOnmemoryPCMBuffer() const = 0;

	/// �I���������Đ����̑S�o�b�t�@�T�C�Y�̎擾
	virtual u32 getOnmemoryPCMBufferSize() const = 0;

    //---------------------------------------------------------------------
	///**
    //	@brief      1 �b������̃o�C�g���擾
    //
    //	@note
    //              �ʏ�� PCM �t�H�[�}�b�g����擾�ł��邯�ǁAMP3 �̏ꍇ��
    //              API �̓s��(?)��A�f�R�[�h�ɍœK�� 1 �b���̃T�C�Y�́A
    //              ���ʂ�PCM�̂���Ƃ͈قȂ�B
    //              ���̂��߁A�����ƃ`�F�b�N�ł���悤�ɂ��̃��\�b�h��p�ӁB
    //              ���܂̂Ƃ���� MP3 �Ɍ������b�����ǁAgetWaveFormat() ��
    //              �擾�����l���� 1 �b���̃T�C�Y���v�Z����ƃo�O�̂Œ��ӁB
    //*/
    //---------------------------------------------------------------------
	virtual u32 getBytesPerSec() const = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���[�v�J�n�ʒu�ƏI���ʒu�̎擾
    //
    //  @par
    //              Midi �t�@�C���̏ꍇ�͍ŏ��� CC111 �ʒu�̃f���^�^�C���ƃx�[�X�^�C��
    //*/
    //---------------------------------------------------------------------
	virtual void getLoopState( u32* begin_, u32* length_ ) const = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      �I�[�f�B�I�t�@�C���Ƃ��Ĉ����X�g���[����ݒ肷��
    //
    //  @par
    //              �󂯎�����X�g���[���͎Q�ƃJ�E���g���ЂƂ����A
    //              �C���X�^���X���������邩 fillBuffer() ���Ă΂��܂ŕێ�����܂��B
    //              (������ꂽ�Ƃ��ɎQ�ƃJ�E���g���f�N�������g)
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT setStream( File::IInStream* stream_ ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      �I���������Đ��p�ɑS�Ẵf�[�^��ǂݍ���
    //
    //  @par
    //              �f�R�[�h��� PCM �f�[�^�T�C�Y���̃o�b�t�@������Ŋm�ۂ��A
    //              �����ɑS�Ẵf�[�^��ǂݍ��݂܂��B
    //              �f�[�^�y�уT�C�Y�� getOnmemoryPCMBuffer()�A
    //              getOnmemoryPCMBufferSize() �Ŏ擾���Ă��������B<br>
    //              <br>
    //              ������Ă΂�Ă��A���łɃo�b�t�@���m�ۂ���Ă���ꍇ��
    //              �Ȃɂ����܂���B<br>
    //              <br>
    //              �ďo����A�X�g���[���͉������A���̃I�[�f�B�I�\�[�X��
    //              �X�g���[�~���O�Đ��ɂ͎g�p�ł��Ȃ��Ȃ�܂��B<br>
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT fillBufferAndReleaseStream() = 0;

    /// fillBufferAndReleaseStream() �X���b�h�Z�[�t
    virtual void fillBufferSafe() = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      �f�[�^���f�R�[�h���Abuffer_ �ɏ�������
    //
    //  @param[out] buffer_         : PCM �f�[�^���������ރo�b�t�@
    //  @param[in]  buffer_size_    : buffer_ �̃T�C�Y ( �o�C�g�P�� )
    //  @param[out] read_size_      : �\�[�X�f�[�^����ǂݍ��񂾃f�[�^�T�C�Y
    //  @param[out] write_size_     : ���ۂ� buffer_ �ɏ������񂾃T�C�Y
    //
    //  @par
    //              �ł��邾�� buffer_size_ �𖞂����悤�Ƀf�[�^���f�R�[�h���A
    //              buffer_ �������݂܂��B
    //              �ʏ�Abuffer_size_ �� getBytesPerSec() �Ɠ����l�ł��B<br>
    //              <br>
    //              read_size_ �̓f�R�[�h�ׂ̈Ƀ\�[�X����ǂݍ��񂾃f�[�^�T�C�Y�ł��B
    //              �ʏ�Amp3 ���̈��k�t�H�[�}�b�g�ł� write_size_ �����������l�ɂȂ�܂��B
    //              ���݂̃t�@�C���|�C���^�� read_size_ �̒l�𑫂����l���A
    //              ����̓ǂݍ��݈ʒu�ƂȂ�܂��B<br>
    //              <br>
    //              write_size_ �́A�ʏ�� buffer_size_ �Ɠ����l�ł����A
    //              �t�@�C���I�[�Ȃǂł� buffer_size_ �����������l ( �����f�[�^������Ƃ���܂� )
    //              �ɂȂ�܂��B	
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT read( void* buffer_, u32 buffer_size_, u32* read_size_, u32* write_size_ ) = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      �t�@�C���|�C���^�ړ�
    //
    //  @param[in]  offset_ : �ړ��� ( �o�C�g�P�� )
    //
    //  @par
    //              �\�[�X�f�[�^�̐擪���� offset_ ���ړ������Ƃ���ɐݒ肵�܂��B
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT seek( u32 offset_ ) = 0;

    /// seek + reed + �X���b�h�Z�[�t
    virtual void readSafe( void* buffer_, u32 buffer_size_, u32 offset_, u32* read_size_, u32* write_size_ ) = 0;


    //---------------------------------------------------------------------
	///**
    //  @brief      �f�R�[�h��Ԃ̃��Z�b�g
    //
    //  @note
    //              �V�����Đ����J�n����O�ɌĂԂ��ƁB
    //              mp3 �̃\�[�X�N���X�Ŏ��������B
    //*/
    //---------------------------------------------------------------------
	virtual void reset() = 0;

protected:

	virtual ~IAudioSource() {}
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