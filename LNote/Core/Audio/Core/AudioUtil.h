//=============================================================================
//�y AudioUtil �z
//-----------------------------------------------------------------------------
///**
//  @file       AudioUtil.h
//  @brief      AudioUtil
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File { class IInStream; }

namespace Audio
{
class IAudioSource;

//=============================================================================
// �� AudioUtil �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �������[�e�B���e�B
//*/
//=============================================================================
class AudioUtil
{
public:

#if defined(LNOTE_WINDOWS)

    //---------------------------------------------------------------------
    ///**
    //  @brief      LNWaveFormat ���� WAVEFORMATEX �ւ̕ϊ�
    //*/
    //---------------------------------------------------------------------
    static void convLNWaveFormatToWAVEFORMATEX( const LNWaveFormat& ln_fmt_, WAVEFORMATEX* wav_fmt_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      WAVEFORMATEX ���� LNWaveFormat �ւ̕ϊ�
    //*/
    //---------------------------------------------------------------------
    static void convWAVEFORMATEXToLNWaveFormat( const WAVEFORMATEX& wav_fmt_, LNWaveFormat* ln_fmt_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      WAVEFORMATEX �\���̂��R���\�[����ʂɕ\��
    //
    //  @param[in]  stream_ : WAVEFORMATEX
    //	@param[in]	str_    : �w�b�_������
    //*/
    //---------------------------------------------------------------------
    static void dumpWAVEFORMATEX( const WAVEFORMATEX& wav_fmt_, const char* str_ = NULL );

#endif

    //---------------------------------------------------------------------
    ///**
    //  @brief      �t�H�[�}�b�g�`�F�b�N
    //
    //  @param[in]  stream_ : ���̓X�g���[��
    //
    //  @par
    //              �����鉹���t�@�C�����ǂ����𒲂ׂāA���̎�ނ�Ԃ��܂��B<br>
    //              ���ׂ���A�t�@�C���|�C���^�̓t�@�C���̐擪�ɖ߂�܂��B
    //*/
    //---------------------------------------------------------------------
    static LNAudioSourceFormat checkFormat( File::IInStream* stream_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �v�����Ă��� type_ �� IAudioSouce ���琳���� type ��Ԃ�
    //
    //  @param[in]  type_       : �v���Đ����@
    //  @param[in]  source_     : �`�F�b�N�Ɏg���I�[�f�B�I�\�[�X
    //  @param[in]  limit_size_ : �I���������ƃX�g���[�~���O�̋��E�T�C�Y
    //
    //  @par
    //              LN_SOUNDPLAYTYPE_AUTO ���v�����ꂽ�ꍇ�A�I�[�f�B�I�\�[�X�̃T�C�Y��
    //              limit_size_ �̃T�C�Y�𒴂���� LN_SOUNDPLAYTYPE_STREAMING ��Ԃ��܂��B
    //*/
    //---------------------------------------------------------------------
    static LNSoundPlayType checkAudioPlayType( LNSoundPlayType type_, IAudioSource* audio_source_, u32 limit_size_ );
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