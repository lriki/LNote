//=============================================================================
// BuildSetting
//=============================================================================

#pragma once

//-----------------------------------------------------------------------------
//

// �V���v���Ȏ��ʎq�̌^�⃆�[�e�B���e�B�̔��`�ݒ�
//      ���̃��C�u�����ƏՓ˂������ȃ}�N���A�^�A�֐����`���Ȃ��悤�ɂ���B
//      �i_p�A_wp�j
//#define LNOTE_DISABLE_SIMPLE_TYPEDEF

// �C�����C���A�Z���u���̎g�p�ݒ�
#define LNOTE_ENABLE_INLINE_ASM

// warning 4996 �𖳎�����
#define LNOTE_DISABLE_WARNING_4996

// Common.h ���� include ����t�@�C���̐ݒ�
//		ReferenceObject.h ���A��v�ȃN���X�ł͂قڕK���g�p������̂̂� include ����B
//#define LNOTE_COMMON_INCLUDE_REQUIRED_FILES_ONRY

// �f�o�b�O�p�������Ǘ����@�̐ݒ�
//		LN_NEW �̒�`�� CRT �̃������Ǘ����g�p����B
//#define LNOTE_ENABLE_CRT_DEBUG


//-----------------------------------------------------------------------------
// �ȉ��͊�{�I�Ƀ��C�u�����r���h���ɂ������삷�����

// ���ˑ��w�b�_�t�@�C���� include �ݒ�
//      ��{�I�� lib �̃r���h���̂ݗL���ɂ���B
#define LNOTE_INCLUDE_ENV_DEPEND_HEADER

// Base/tchar.h �� include �ݒ�
//      Base/tchar.h �̃f�o�b�O�p�BWin32 ���ł̂ݗL���B
//      Win32 �ȊO�ł͂��̐ݒ�ɂ�����炸 Base/tchar.h �� include �����B
#define LNOTE_INCLUDE_INTERNAL_TCHAR

// �����R�[�h�Z�b�g���A�v���W�F�N�g�̐ݒ肩��ł͂Ȃ��\�[�X�R�[�h����w�肷��t���O
//      �����R�[�h�Z�b�g��ύX���鎞�A�S�Ẵv���W�F�N�g���ɐݒ肵�����Ȃ��Ă�
//      �ȒP�ɕύX�ł���悤�ɂ��邽�ߗp�ӂ������́B
#define LNOTE_SPECIFIED_CHARCODESET_MBS

// UNICODE �����Z�b�g�g�p�̋���
//#define LNOTE_SPECIFIED_CHARCODESET_UNICODE

// Win32 ���ŃX���b�h�̐����� AfxBeginThread() ���g�p����
#define LNOTE_ENABLE_WIN32_RUNTIME_THREAD

//=============================================================================
//
//=============================================================================