//=============================================================================
//�y Win32FontManager �z
//-----------------------------------------------------------------------------
///**
//  @file       Win32FontManager.h
//  @brief      Win32FontManager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../FontManagerBase.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File { class Manager; }

namespace Graphics
{
namespace Win32
{
class Win32Font;

//=============================================================================
// �� Win32FontManager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief     �t�H���g�̊Ǘ��N���X
//*/
//=============================================================================
class Win32FontManager
    : public FontManagerBase
{
public:

    /// �R���X�g���N�^
    Win32FontManager();

    ///// �f�X�g���N�^
    //virtual ~Win32FontManager();

public:

    /// ������
    void initialize( File::Manager* file_manager_ );

    /// �I������
    //void finalize();

    /// �t�H���g�t�@�C����ǉ����� (ttf)
    virtual void addFontFile( const lnChar* fontname_ );

    /// �t�H���g�쐬
    virtual IFont* createFont();

    /// �t�H���g�t�@�C����ǉ�����
    //LNRESULT addFontFile( File::IInStream* stream_ );

    ///// �t�H���g�̍쐬 (�t�H���g���� NULL �̏ꍇ�̓f�t�H���g�̃t�H���g���g����)
    //LNRESULT createFont( IFont** obj_, const lnChar* fontname_ );

    ///// �t�H���g�̍쐬 (�f�t�H���g�̐ݒ肩��쐬����)
    //LNRESULT createDefaultFont( IFont** obj_ );

    ///// �f�t�H���g�̃t�H���g���Z�b�g���� (createFont() �ō쐬�������̂�n���K�v������B�f�t�H���g�� "�l�r �o�S�V�b�N"�BcreateFont() �ŃR�s�[���Ďg����)
    //LNRESULT setDefaultFont( IFont* font_ );

    ///// �f�t�H���g�̃t�H���g���擾����
    //IFont* getDefaultFont();

private:

    File::Manager*  mFileManager;

    //IFont*  mDefaultFont;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Win32
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================