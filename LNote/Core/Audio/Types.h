//=============================================================================
//�y Types �z
//-----------------------------------------------------------------------------
///**
//  @file       Types.h
//  @brief      Types
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------
namespace LNote
{
struct LNWaveFormat
{
    lnU16       FormatTag;
    lnU16       Channels;
    lnU32       SamplesPerSec;
    lnU32       AvgBytesPerSec;
    lnU16       BlockAlign;
    lnU16       BitsPerSample;
    lnU16       EXSize;
};

/// �����t�@�C���t�H�[�}�b�g
enum LNAudioSourceFormat
{
	LN_AUDIOSOURCE_UNKNOWN = 0,		///< �s���ȃt�@�C��
	LN_AUDIOSOURCE_WAVE,			///< WAVE
	LN_AUDIOSOURCE_MP3,				///< MP3
	LN_AUDIOSOURCE_OGG,				///< OGG
	LN_AUDIOSOURCE_MIDI				///< MIDI
};

/// �Đ����@ ( �I�[�f�B�I�v���C���[�̎�� )
enum LNSoundPlayType
{
	LN_SOUNDPLAYTYPE_AUTO = 0,		///< �����I�� ( �f�t�H���g�ł̓f�R�[�h��̃T�C�Y�� 10000 �o�C�g�ȏ�ɂȂ�ꍇ�̓X�g���[�~���O�A�����łȂ��ꍇ�̓I���������Đ��ɂȂ�܂� )
	LN_SOUNDPLAYTYPE_ONMEMORY,		///< �I��������
	LN_SOUNDPLAYTYPE_STREAMING,		///< �X�g���[�~���O
	LN_SOUNDPLAYTYPE_MIDI,  		///< SMF

    LN_SOUNDPLAYTYPE_UNKNOWN = 0xffff,	///< �s���ȍĐ����@ ( ��Ŏ����I���Ƃ܂Ƃ߂Ă����Ă����������H )
};

/// DirectMusic �̏��������@
enum LNDirectMusicUsingMode
{
    LN_DMUSINGMODE_NOTUSE = 0,       ///< DirectMusic ���g�p���Ȃ� ( LConfig ����ݒ肷�邽�߂ɗp�� )

	LN_DMUSINGMODE_USE,		        ///< �ʏ�
	LN_DMUSINGMODE_THREAD_WAIT,		///< �ʃX���b�h�ŏ��������āA�Đ����ɖ������̏ꍇ�͑҂�
	LN_DMUSINGMODE_THREAD_REQUEST,	///< �ʃX���b�h�ŏ��������āA�Đ����ɖ������̏ꍇ�͍Đ���\�񂷂�
};

/// �����̍Đ����
enum LNSoundPlayState
{
    LN_SOUNDPLAYSTATE_STOP = 0,     ///< ��~��
    LN_SOUNDPLAYSTATE_PLAYING,      ///< �Đ���
    LN_SOUNDPLAYSTATE_PAUSING       ///< �ꎞ��~��
};

/// ���ʃt�F�[�h�������̓���̋L�q
enum LNSoundFadeState
{
    LN_SOUNDFADE_CONTINUE = 0,	    ///< �p��(�Ȃɂ����Ȃ�)
	LN_SOUNDFADE_STOP,		        ///< ��~���� (stop())
	LN_SOUNDFADE_STOP_RESET,	    ///< ��~���āA���ʂ����̒l�ɖ߂�
	LN_SOUNDFADE_PAUSE,			    ///< �ꎞ��~���� (pause( true ))
	LN_SOUNDFADE_PAUSE_RESET,	    ///< �ꎞ��~���āA���ʂ����̒l�ɖ߂�
};

/// �O���[�v�̋L�q
enum LNAudioPlayerGroupFlag
{
	LN_AUDIOPLAYER_GROUP_BGM        = 0x0002,	///< BGM �O���[�v�ɑ�����t���O
	LN_AUDIOPLAYER_GROUP_BGS        = 0x0004,	///< BGS �O���[�v�ɑ�����t���O
	LN_AUDIOPLAYER_GROUP_SE         = 0x0008,	///< SE  �O���[�v�ɑ�����t���O
    LN_AUDIOPLAYER_GROUP_1          = 0x0010,   ///< �O���[�v 1 �ɑ�����t���O
    LN_AUDIOPLAYER_GROUP_2          = 0x0020,   ///< �O���[�v 2 �ɑ�����t���O
    LN_AUDIOPLAYER_GROUP_3          = 0x0040,   ///< �O���[�v 3 �ɑ�����t���O
    LN_AUDIOPLAYER_GROUP_4          = 0x0080,   ///< �O���[�v 4 �ɑ�����t���O
};

/// �����̍Đ����
struct LNAudioPlayState
{

    u32                 TotalUnits;     ///< �S�T���v���� ( Midi �̏ꍇ�̓~���[�W�b�N�^�C���P�� ( �l�������ЂƂ��� 768 ) �܂��AMidi �̏ꍇ��1�x�ł� play() �ōĐ����Ă��Ȃ��Ǝ擾�ł��Ȃ� )
    u32                 CurrentUnits;   ///< ���݂̍Đ��T���v����
    lnU32               SamplesPerSec;  ///< �T���v�����O���g�� (MIDI �̏ꍇ�� LN_MUSICTIMEBASE)
    LNSoundPlayState    SoundPlayState;
};

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================