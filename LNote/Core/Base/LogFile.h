//=============================================================================
// �y LogFile �z
//-----------------------------------------------------------------------------
///**
//  @file       LogFile.h
//  @brief      LogFile
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "ReferenceObject.h"

//-------------------------------------------------------------------------
// macro
//-------------------------------------------------------------------------

#if defined(LNOTE_GNUC)
    // ���O�o�� ( mLogFile �������o�Ɏ��N���X�p�B���C�h�����񌓗p )
    #define LN_LOG_WRITE( format_, ... )    { if ( mLogFile ) { mLogFile->write( ( format_ ), ## __VA_ARGS__ ); } }

    #define LN_LOG_INIT_BEGIN \
        mLogFile = init_data_.LogFile; \
        LN_SAFE_ADDREF( mLogFile ); \
        LN_LOG_WRITE( "%s ���������J�n���܂�...", __FUNCTION__ );

    #define LN_LOG_INIT_END \
        LN_LOG_WRITE( "%s ���������������܂����B", __FUNCTION__ );

    #define LN_LOG_FIN_BEGIN \
        LN_LOG_WRITE( "%s �I���������J�n���܂�...", __FUNCTION__ );

    #define LN_LOG_FIN_END \
        LN_LOG_WRITE( "%s �I���������������܂����B", __FUNCTION__ ); \
        LN_SAFE_RELEASE( mLogFile );

#else
    // ���O�o�� ( mLogFile �������o�Ɏ��N���X�p�B���C�h�����񌓗p )
    #define LN_LOG_WRITE( format_, ... )    { if ( mLogFile ) { mLogFile->write( ( format_ ), __VA_ARGS__ ); } }

    #define LN_LOG_INIT_BEGIN \
        mLogFile = init_data_.LogFile; \
        LN_SAFE_ADDREF( mLogFile ); \
        LN_LOG_WRITE( __FUNCTION__" ���������J�n���܂�..." );

    #define LN_LOG_INIT_END \
        LN_LOG_WRITE( __FUNCTION__" ���������������܂����B" );

    #define LN_LOG_FIN_BEGIN \
        LN_LOG_WRITE( __FUNCTION__" �I���������J�n���܂�..." );

    #define LN_LOG_FIN_END \
        LN_LOG_WRITE( __FUNCTION__" �I���������������܂����B" ); \
        LN_SAFE_RELEASE( mLogFile );

#endif
/*
#define LN_LOG_INIT_BEGIN( format_, ... ) \
    mLogFile = init_data_.LogFile; \
    LN_SAFE_ADDREF( mLogFile ); \
    LN_LOG_WRITE( format_ " ���������J�n���܂�...", __VA_ARGS__ );

#define LN_LOG_INIT_END( format_, ... ) \
    LN_LOG_WRITE( format_ " ���������������܂����B", __VA_ARGS__ );

#define LN_LOG_FIN_BEGIN( format_, ... ) \
    LN_LOG_WRITE( format_ " �I���������J�n���܂�...", __VA_ARGS__ );

#define LN_LOG_FIN_END( format_, ... ) \
    LN_LOG_WRITE( format_ " �I���������������܂����B", __VA_ARGS__ ); \
    LN_SAFE_RELEASE( mLogFile );
*/

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Thread { class Mutex; }

namespace Base
{

//=============================================================================
// �� LogFile �N���X
//-----------------------------------------------------------------------------
///**
//	@brief      ���O�t�@�C���̃N���X
//
//  @par
//              �ꉞ�X���b�h�Z�[�t�ł��B
//*/
//=============================================================================
class LogFile
    : public ReferenceObject
{
public:

    /// �R���X�g���N�^
    LogFile();

    /// �f�X�g���N�^ ( �t�@�C�����J����Ă���Ε��� )
    virtual ~LogFile();

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      ������
    //
    //  @param[in]  log_name_ : ���O�t�@�C���̖��O
    //  @param[in]  log_dir_  : ���O�t�@�C�����쐬����f�B���N�g�� (NULL �̏ꍇ�̓J�����g�f�B���N�g��)
    //
    //  @retval     true  : ����
    //  @retval     false : ���s
    //
    //  @par
    //              ���݁ALinux �ł͏�ɃJ�����g�f�B���N�g���ɍ쐬����܂��B
	//*/
    //---------------------------------------------------------------------
    bool initialize( const lnChar* log_name_, const lnChar* log_dir_ = NULL );

    //---------------------------------------------------------------------
	///**
    //  @brief      �t�@�C���ɒǉ����[�h�ŏ������� ( char )
	//*/
    //---------------------------------------------------------------------
    bool write( const char *format_, ... );

    /// �t�@�C���ɒǉ����[�h�ŏ������� ( wchar_t )
    bool write( const wchar_t *format_, ... );

private:

    static const int        TEMP_BUFFER_SIZE = 2048;

    lnChar                  mLogFilePath[ LN_MAX_PATH ];
    Thread::Mutex*          mMutex;
    unsigned int            mLogStartTime;
    wchar_t                 mTempBuffer[ TEMP_BUFFER_SIZE ];    ///< char�Awchar_t ���p
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================