//=============================================================================
//�y ArchiveMaker �z
//-----------------------------------------------------------------------------
///**
//  @file       ArchiveMaker.h
//  @brief      ArchiveMaker
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Base/RefString.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File
{

//=============================================================================
// �� ArchiveMaker �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �A�[�J�C�u�t�@�C������邽�߂̃N���X
//*/
//=============================================================================
class ArchiveMaker
    : public Base::NonCopyable
{
public:

	/// �R���X�g���N�^
	ArchiveMaker();

	/// �f�X�g���N�^
	virtual ~ArchiveMaker();

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief		�V�����A�[�J�C�u�t�@�C�����J��
    //
    //  @param[in]  filename_ : �V�����쐬����A�[�J�C�u�t�@�C���̖��O
    //  @param[in]  key_      : �Í����Ɏg�������� ( \0 �I�[ )
    //
    //  @retval     true  : ����
    //  @retval     false : �t�@�C�����J�����Ƃ��ł��Ȃ�����
    //
    //  @par
    //              �t�@�C�����������ݐ�p�ŊJ���܂��B
    //              ( fopen() �� "wb" ���w�肵����ԁB�����ꍇ�͐V�K�쐬 )<br><br>
    //*/
    //---------------------------------------------------------------------
    bool open( const lnChar* filename_, const char* key_ = NULL );
    //void open( const wchar_t* filename_, const char* key_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief		�A�[�J�C�u�t�@�C�������
    //
    //  @par
    //              �ǉ����ꂽ�t�@�C���̐�����������ŁA�t�@�C���X�g���[������܂��B
    //              �J����Ă��Ȃ��ꍇ�͂Ȃɂ����܂���B<br>
    //              �f�X�g���N�^�ł��Ăяo����܂��B(std::fstream �Ɠ����d�l)
    //*/
    //---------------------------------------------------------------------
    void close();

    //---------------------------------------------------------------------
    ///**
    //  @brief		�t�@�C����ǉ�����
    //
    //  @param[in]  filename_    : �ǉ�����t�@�C���̖��O
    //  @param[in]  access_name_ : �v���O��������A�N�Z�X���鎞�̖��O
    //
    //  @retval     true  : ����
    //  @retval     false : �t�@�C�����J�����Ƃ��ł��Ȃ�����
    //
    //  @par
    //              filename_ �̃t�@�C�����A�[�J�C�u�ɒǉ����܂��B<br>
    //              access_name_ ���w�肵���ꍇ�A�t�@�C���ɕʖ���������Œǉ����܂��B
    //              Archive �N���X�� open() �ɂ���ăA�[�J�C�u���̃t�@�C���ɃA�N�Z�X����ꍇ�A
    //              ���̖��O�ŃA�N�Z�X����悤�ɂ��܂��B<br>
    //              access_name_ ���ȗ������ꍇ�� filename_ �����̂܂܎g���܂��B
    //*/
    //---------------------------------------------------------------------
    bool addFile( const lnChar* filename_, const lnChar* access_name_ = NULL );

private:

    /// �p�f�B���O���l�����āA�J���Ă���A�[�J�C�u�t�@�C���Ƀf�[�^����������
	void _writePadding16( const void* data_, u32 size_ );

    /// �p�f�B���O���l�����āA�������������� (�t�@�C���������A�t�@�C���T�C�Y�p)
	void _writeU32Padding16( u32 v0_, u32 v1_ );

	/// �J���Ă���A�[�J�C�u�t�@�C���Ƀf�[�^����������
	void _write( const void* data_, u32 size_ );

	/// �ǉ�����t�@�C���̃X�g���[�����󂯂Ƃ��āA���ۂɒǉ�����
	void _addFile( FILE* stream_ );


private:

    static const int VER_MAJOR = 1;
    static const int VER_MINOR = 0;

    static const int KEY_SIZE  = 128;

	FILE*			        mStream;			///< �A�[�J�C�u�t�@�C���̃t�@�C���X�g���[��
    Base::RefString	        mEncryptionKey;		///< �Í����Ɏg���L�[
	u32				        mFileNum;			///< �t�@�C����
    lnByte                  mKeyTable[ 256 ];

    int                     mBlockLength;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace File
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================