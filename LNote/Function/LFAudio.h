//=============================================================================
//�y LFAudio �z
//-----------------------------------------------------------------------------
///**
//  @file       LFAudio.h
//  @brief      LFAudio
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "LFCommon.h"

LNOTE_FUNC_NAMESPACE_BEGIN

//-------------------------------------------------------------------------
/**
@defgroup   group_lib_audio Audio
@{
*/
//-------------------------------------------------------------------------

//=============================================================================
// LAudio
//-----------------------------------------------------------------------------
///**
//  @defgroup   group_laudio LAudio - �����Ǘ��y�уQ�[���I�[�f�B�I�@�\
//  @brief      
//  @{
//*/
//=============================================================================

    //---------------------------------------------------------------------
	///**
    //  @brief      BGM �����t����
    //  @param[in]  filename   : �t�@�C����
    //  @param[in]  volume     : �{�����[�� (0 �` 100)
    //  @param[in]  pitch      : �s�b�` (50 �` 200)
    //  @param[in]  fade_time  : �t�F�[�h�C���ɂ����鎞�� (�~���b)
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------[{hsp override=true}]
    HRESULT LAudio_PlayBGM( const TCHAR* filename, int volume = 100, int pitch = 100, int fade_time = 0 );

    //---------------------------------------------------------------------
	///**
    //  @brief      ��������̉����f�[�^���� BGM �����t����
    //  @param[in]  data        : ��������̉����f�[�^�ւ̃|�C���^
    //  @param[in]  data_size   : �f�[�^�T�C�Y (�o�C�g�P��)
    //  @param[in]  volume      : �{�����[�� (0 �` 100)
    //  @param[in]  pitch       : �s�b�` (50 �` 200)
    //  @param[in]  fade_time   : �t�F�[�h�C���ɂ����鎞�� (�~���b)
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT LAudio_PlayBGMMem( const void* data, int data_size, int volume = 100, int pitch = 100, int fade_time = 0 );

    //---------------------------------------------------------------------
	///**
    //  @brief      BGM �̉��t���~����
    //  @param[in]  fade_time : �t�F�[�h�A�E�g�ɂ����鎞�� (�~���b)
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
	HRESULT LAudio_StopBGM( int fade_time = 0 );

    //---------------------------------------------------------------------
	///**
    //  @brief      BGS �����t����
    //  @param[in]  filename   : �t�@�C����
    //  @param[in]  volume     : �{�����[�� (0 �` 100)
    //  @param[in]  pitch      : �s�b�` (50 �` 200)
    //  @param[in]  fade_time  : �t�F�[�h�C���ɂ����鎞�� (�~���b)
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------[{hsp override=true}]
    HRESULT LAudio_PlayBGS( const TCHAR* filename, int volume = 100, int pitch = 100, int fade_time = 0 );

    //---------------------------------------------------------------------
	///**
    //  @brief      ��������̉����f�[�^���� BGS �����t����
    //  @param[in]  data        : ��������̉����f�[�^�ւ̃|�C���^
    //  @param[in]  data_size   : �f�[�^�T�C�Y (�o�C�g�P��)
    //  @param[in]  volume      : �{�����[�� (0 �` 100)
    //  @param[in]  pitch       : �s�b�` (50 �` 200)
    //  @param[in]  fade_time   : �t�F�[�h�C���ɂ����鎞�� (�~���b)
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT LAudio_PlayBGSMem( const void* data, int data_size, int volume = 100, int pitch = 100, int fade_time = 0 );

    //---------------------------------------------------------------------
	///**
    //  @brief      BGS �̉��t���~����
    //  @param[in]  fade_time : �t�F�[�h�A�E�g�ɂ����鎞�� (�~���b)
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
	HRESULT LAudio_StopBGS( int fade_time = 0 );

    //---------------------------------------------------------------------
	///**
    //  @brief      ME �����t����
    //  @param[in]  filename   : �t�@�C����
    //  @param[in]  volume     : �{�����[�� (0 �` 100)
    //  @param[in]  pitch      : �s�b�` (50 �` 200)
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------[{hsp override=true}]
    HRESULT LAudio_PlayME( const TCHAR* filename, int volume = 100, int pitch = 100 );

    //---------------------------------------------------------------------
	///**
    //  @brief      ��������̉����f�[�^���� ME �����t����
    //  @param[in]  data        : ��������̉����f�[�^�ւ̃|�C���^
    //  @param[in]  data_size   : �f�[�^�T�C�Y (�o�C�g�P��)
    //  @param[in]  volume      : �{�����[�� (0 �` 100)
    //  @param[in]  pitch       : �s�b�` (50 �` 200)
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT LAudio_PlayMEMem( const void* data, int data_size, int volume = 100, int pitch = 100 );

    //---------------------------------------------------------------------
	///**
    //  @brief      ME �̉��t���~����
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
	HRESULT LAudio_StopME();

    //---------------------------------------------------------------------
	///**
    //  @brief      SE �����t����
    //  @param[in]  filename   : �t�@�C����
    //  @param[in]  volume     : �{�����[�� (0 �` 100)
    //  @param[in]  pitch      : �s�b�` (50 �` 200)
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------[{hsp override=true}]
    HRESULT LAudio_PlaySE( const TCHAR* filename, int volume = 100, int pitch = 100 );

    //---------------------------------------------------------------------
	///**
    //  @brief      SE �����t���� (3D �T�E���h)
    //  @param[in]  filename    : �t�@�C����
    //  @param[in]  x, y, z     : 3D ���W
    //  @param[in]  distance    : ��������
    //  @param[in]  volume      : �{�����[�� (0 �` 100)
    //  @param[in]  pitch       : �s�b�` (50 �` 200)
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------[{hsp override=true}]
    HRESULT LAudio_PlaySE3D( const TCHAR* filename, float x, float y, float z, float distance, int volume = 100, int pitch = 100 );

    //---------------------------------------------------------------------
	///**
    //  @brief      ��������̉����f�[�^���� SE �����t����
    //  @param[in]  data        : ��������̉����f�[�^�ւ̃|�C���^
    //  @param[in]  data_size   : �f�[�^�T�C�Y (�o�C�g�P��)
    //  @param[in]  volume      : �{�����[�� (0 �` 100)
    //  @param[in]  pitch       : �s�b�` (50 �` 200)
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT LAudio_PlaySEMem( const void* data, int data_size, int volume = 100, int pitch = 100 );

    //---------------------------------------------------------------------
	///**
    //  @brief      ��������̉����f�[�^���� SE �����t���� (3D �T�E���h)
    //  @param[in]  data        : ��������̉����f�[�^�ւ̃|�C���^
    //  @param[in]  data_size   : �f�[�^�T�C�Y (�o�C�g�P��)
    //  @param[in]  x, y, z     : 3D ���W
    //  @param[in]  distance    : ��������
    //  @param[in]  volume      : �{�����[�� (0 �` 100)
    //  @param[in]  pitch       : �s�b�` (50 �` 200)
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT LAudio_PlaySE3DMem( const void* data, int data_size, float x, float y, float z, float distance, int volume = 100, int pitch = 100 );

    //---------------------------------------------------------------------
	///**
    //  @brief      ���ׂĂ� SE �̉��t���~����
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
	HRESULT LAudio_StopSE();

    //---------------------------------------------------------------------
	///**
    //  @brief      3D ��Ԃ�1���[�g�������̋����̐ݒ�
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
	HRESULT LAudio_SetMetreUnitDistance( float distance );

/** @} */

//=============================================================================
// L3DSoundListener
//-----------------------------------------------------------------------------
///**
//  @defgroup   group_l3dsoundlistener L3DSoundListener - 3D�������X�i�[
//  @brief      
//  @{
//*/
//=============================================================================

    //---------------------------------------------------------------------
	///**
    //  @brief      ���X�i�[�̈ʒu�̐ݒ�
    //  @param[in]  x, y, z    : ���W
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT L3DSoundListener_SetPosition( float x, float y, float z );

    //---------------------------------------------------------------------
	///**
    //  @brief      ���X�i�[�̐��ʕ����̐ݒ�
    //  @param[in]  x, y, z    : ����
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT L3DSoundListener_SetDirection( float x, float y, float z );

    //---------------------------------------------------------------------
	///**
    //  @brief      ���X�i�[�̏�����̐ݒ�
    //  @param[in]  x, y, z    : ���� (���ʕ����Ƃ͒����ł��邱��)
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT L3DSoundListener_SetUpDirection( float x, float y, float z );

    //---------------------------------------------------------------------
	///**
    //  @brief      ���X�i�[�̑��x�̐ݒ�
    //  @param[in]  x, y, z    : ���x
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT L3DSoundListener_Velocity( float x, float y, float z );
    
/** @} */

//=============================================================================
// LSound
//-----------------------------------------------------------------------------
///**
//  @defgroup   group_lsound LSound - �����@�\
//  @brief      
//  @{
//*/
//=============================================================================

    //---------------------------------------------------------------------
	///**
    //  @brief      �t�@�C�����特�����쐬����
    //  @param[in]  filename    : �t�@�C����
    //  @param[in]  enable_3d   : true �̏ꍇ�A3D �����Ƃ��č쐬����
    //  @param[in]  play_type   : �����̍Đ����@
    //  @return     �T�E���h�n���h��
    //*/
    //---------------------------------------------------------------------[{hsp disable},{ruby a=1 b=2}]
    LNHandle LSound_Load( const TCHAR* filename, bool enable_3d = false, LNSoundPlayType play_type = LN_SOUNDPLAYTYPE_AUTO );

    //---------------------------------------------------------------------
	///**
    //  @brief      ��������̃f�[�^���特�����쐬����
    //  @param[in]  data        : ��������̉����f�[�^�ւ̃|�C���^
    //  @param[in]  data_size   : �f�[�^�T�C�Y (�o�C�g�P��)
    //  @param[in]  enable_3d   : true �̏ꍇ�A3D �����Ƃ��č쐬����
    //  @return     �T�E���h�n���h��
    //*/
    //---------------------------------------------------------------------
    LNHandle LSound_CreateMem( const void* data, int data_size, bool enable_3d = false );

    //---------------------------------------------------------------------
	///**
    //  @brief      �t�@�C�����特�����쐬����
    //  @param[out] out_handle  : �T�E���h�n���h�����i�[����ϐ��̃|�C���^
    //  @param[in]  filename    : �t�@�C����
    //  @param[in]  enable_3d   : true �̏ꍇ�A3D �����Ƃ��č쐬����
    //  @param[in]  play_type   : �����̍Đ����@
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------[{hsp name=LSound_Load}]
    HRESULT LSound_LoadToPtr( LNHandle* out_handle, const TCHAR* filename, bool enable_3d = false, LNSoundPlayType play_type = LN_SOUNDPLAYTYPE_AUTO );

    //---------------------------------------------------------------------
	///**
    //  @brief      �������폜����
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //*/
    //---------------------------------------------------------------------
    void LSound_Release( LNHandle sound_handle );

    //---------------------------------------------------------------------
	///**
    //  @brief      �{�����[���̐ݒ�
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @param[in]  volume          : �{�����[�� (0�`100)
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT LSound_SetVolume( LNHandle sound_handle, int volume );

    //---------------------------------------------------------------------
	///**
    //  @brief      �{�����[���̎擾
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @return     �{�����[��
    //*/
    //---------------------------------------------------------------------
    int LSound_GetVolume( LNHandle sound_handle );

    //---------------------------------------------------------------------
	///**
    //  @brief      �s�b�`�̐ݒ�
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @param[in]  pitch           : �s�b�` (50�`200)
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT LSound_SetPitch( LNHandle sound_handle, int pitch );

    //---------------------------------------------------------------------
	///**
    //  @brief      �s�b�`�̎擾
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @return     �s�b�`
    //*/
    //---------------------------------------------------------------------
    int LSound_GetPitch( LNHandle sound_handle );

    //---------------------------------------------------------------------
	///**
    //  @brief      ���[�v�Đ��̗L���ݒ�
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @param[in]  flag            : true = ���[�v�Đ����� / false = ���Ȃ�
    //  @param[in]  begin           : ���[�v�̈�̐擪�ʒu (�T���v�����P��)
    //  @param[in]  length          : ���[�v�̈撷�� (�T���v�����P��)
    //  @return     ���U���g�R�[�h
    //  @par
    //              begin �� length �� 0 ���w�肷��ƁA�S�̂����[�v�̈�Ƃ��Đݒ肵�܂��B
    //*/
    //---------------------------------------------------------------------
    HRESULT LSound_SetLoopState( LNHandle sound_handle, bool loop, int begin = 0, int length = 0 );

    //---------------------------------------------------------------------
	///**
    //  @brief      ���[�v�Đ����L�����𒲂ׂ�
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @return     true = ���[�v�Đ����� / false = ���[�v�Đ����Ȃ�
    //*/
    //---------------------------------------------------------------------
    bool LSound_IsLoop( LNHandle sound_handle );

    //---------------------------------------------------------------------
	///**
    //  @brief      �T�E���h�̍Đ���Ԃ��擾����
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @return     LN_SOUNDPLAYSTATE_STOP    : ��~��
    //              LN_SOUNDPLAYSTATE_PLAYING : �Đ���
    //              LN_SOUNDPLAYSTATE_PAUSING : �ꎞ��~��
    //*/
    //---------------------------------------------------------------------
    LNSoundPlayState LSound_GetPlayState( LNHandle sound_handle );

    //---------------------------------------------------------------------
	///**
    //  @brief      �������Đ�����
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT LSound_Play( LNHandle sound_handle );

    //---------------------------------------------------------------------
	///**
    //  @brief      �Đ����~����
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT LSound_Stop( LNHandle sound_handle );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �ꎞ��~
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @param[in]  pause           : true = �ꎞ��~ / false = �ꎞ��~����
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
	HRESULT LSound_Pause( LNHandle sound_handle, bool pause );

    //---------------------------------------------------------------------
	///**
    //  @brief      ���ʂ̃t�F�[�h
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @param[in]  target_volume   : �ύX��̉���
    //  @param[in]  time            : �t�F�[�h�ɂ����鎞�� (�~���b)
    //  @param[in]  fade_state      : �t�F�[�h������̓���̎w��
    //                              @arg LN_SOUNDFADE_CONTINUE : �p��
    //                              @arg LN_SOUNDFADE_STOP : ��~����
    //                              @arg LN_SOUNDFADE_STOP_RESET : ��~���āA���ʂ����̒l�ɖ߂�
    //                              @arg LN_SOUNDFADE_PAUSE : �ꎞ��~����
    //                              @arg LN_SOUNDFADE_PAUSE_RESET : �ꎞ��~���āA���ʂ����̒l�ɖ߂�
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT LSound_FadeVolume( LNHandle sound_handle, int target_volume, int time, LNSoundFadeState fade_state );

    //---------------------------------------------------------------------
	///**
    //  @brief      �Đ������T���v�����̎擾
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @return     �Đ������T���v����
    //*/
    //---------------------------------------------------------------------
    int LSound_GetUnitsPlayed( LNHandle sound_handle );

    //---------------------------------------------------------------------
	///**
    //  @brief      �����f�[�^�S�̂̃T���v�����̎擾
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @return     �����f�[�^�S�̂̃T���v����
    //*/
    //---------------------------------------------------------------------
    int LSound_GetTotalUnits( LNHandle sound_handle );

    //---------------------------------------------------------------------
	///**
    //  @brief      �T���v�����O���g���̎擾
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @return     �T���v�����O���g��
    //*/
    //---------------------------------------------------------------------
    int LSound_GetUnitsParSecond( LNHandle sound_handle );

    //---------------------------------------------------------------------
    ///**
    //  @brief		3D �������𒲂ׂ�
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @return     true=3D���� / false=3D�����ł͂Ȃ�
    //*/
    //---------------------------------------------------------------------
    bool LSound_Is3DSound( LNHandle sound_handle );

    //---------------------------------------------------------------------
    ///**
    //  @brief		3D �����Ƃ��Ă̈ʒu��ݒ肷��
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @param[in]  x, y ,z         : 3D ���W
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT LSound_SetEmitterPosition( int sound_handle, float x, float y, float z );

    //---------------------------------------------------------------------
    ///**
    //  @brief		3D �����Ƃ��Ă̑��x��ݒ肷��
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @param[in]  x, y, z         : ���x
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT LSound_SetEmitterVelocity( int sound_handle, float x, float y, float z );

    //---------------------------------------------------------------------
    ///**
    //  @brief		3D �����̌������� (�������Ȃ��Ȃ鋗��) ��ݒ肷��
    //  @param[in]  sound_handle    : �T�E���h�n���h��
    //  @param[in]  distance        : ����
    //  @return     ���U���g�R�[�h
    //*/
    //---------------------------------------------------------------------
    HRESULT LSound_SetEmitterDistance( int sound_handle, float distance );

/** @} */

//-------------------------------------------------------------------------
/** @} */
//-------------------------------------------------------------------------

LNOTE_FUNC_NAMESPACE_END

//=============================================================================
//                              end of file
//=============================================================================