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


/// �������܂��͍X�V����@�\�̎w��
enum LNClassFlag
{
    LN_CLASS_FRAMEWORK  = 0x0001,
    LN_CLASS_FILE       = 0x0002,
    LN_CLASS_SYSTEM     = 0x0004,
    LN_CLASS_INPUT      = 0x0008,
    LN_CLASS_PHYSICS    = 0x0010,
    LN_CLASS_GRAPHICS   = 0x0020,
    LN_CLASS_EFFECT     = 0x0040,
    LN_CLASS_SCENE      = 0x0080,
    LN_CLASS_AUDIO      = 0x0100,
    LN_CLASS_GAME       = 0x0200,
    LN_CLASS_GUI        = 0x0400,

    LN_CLASS_ALL        = 0xffff

    /*
    LN_INIT_FRAMEWORK   = 0x0001,   ///< Framework ���̋@�\������������ ( FPSController�AGameTime )
    LN_INIT_FILE        = 0x0002,   ///< File ���W���[��������������
    LN_INIT_SYSTEM      = 0x0004,   ///< System ���W���[��������������
    LN_INIT_INPUT       = 0x0008,   ///< Input ���W���[��������������
    LN_INIT_PHYSICS     = 0x0010,   ///< Physics ���W���[��������������
    LN_INIT_GRAPHICS    = 0x0020,   ///< Graphice ���W���[��������������
    LN_INIT_EFFECT      = 0x0040,   ///< Effect ���W���[��������������
    LN_INIT_SCENE       = 0x0080,   ///< Scene ���W���[��������������
    LN_INIT_AUDIO       = 0x0100,   ///< Audio ���W���[��������������
    LN_INIT_GAME        = 0x0200,   ///< Game ���W���[��������������
    LN_INIT_GUI         = 0x0400,   ///< GUI ���W���[��������������

    LN_INIT_ALL         = 0xffff    ///< ���ׂẴt���O�̑g�ݍ��킹
    */
    /*
        LN_INIT_FRAMEWORK | LN_INIT_FILE      | LN_INIT_SYSTEM    |
        LN_INIT_INPUT     | LN_INIT_PHYSICS   | LN_INIT_GRAPHICS  | LN_INIT_EFFECT |
        LN_INIT_SCENE     | LN_INIT_AUDIO     | LN_INIT_GAME      | LN_INIT_GUI
        */
    
};

/// �t���[�����[�N�̍X�V���@
enum LNUpdateMode
{
    LN_UPDATEMODE_ACTIVE_ONLY = 0,  ///< �Q�[���E�B���h�E���A�N�e�B�u�̂Ƃ��̂ݍX�V����
    LN_UPDATEMODE_ALWAYS,           ///< �A�N�e�B�u��Ԃɂ�����炸�X�V����
    LN_UPDATEMODE_THREADED          ///< OS �̃��b�Z�[�W�ɂ�����炸�A��ɍX�V���� (�^�C�g���o�[���h���b�O�����X�V)
};

/// �f�t�H���g�ŃZ�b�g�����C�x���g���X�i�[�̗D��x
enum LNDefaultELPriority
{
    LN_DEFAULT_EL_PRIORITY_GUI          = 100,
    LN_DEFAULT_EL_PRIORITY_INPUT        = -1000,
    LN_DEFAULT_EL_PRIORITY_FRAMEWORK    = -1001
};

/// �f�t�H���g�̃^�X�N���s�t�F�[�Y
enum LNDefaultTaskPhase
{
    LN_TASKPHASE_UPDATE_RENDERING_CONTENT = 5,  ///< �`����̍\�z
    LN_TASKPHASE_MASTER_RENDER = 5,             ///< ���C���̕`��^�X�N (���� 5 �̃t�F�[�Y�œ������Ƃ�)
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Core

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================