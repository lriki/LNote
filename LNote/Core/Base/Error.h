//=============================================================================
// �y Error �z
//-----------------------------------------------------------------------------
///**
//  @file       Error.h
//  @brief      Error
//  @author     Riki
//*/
//=============================================================================

#pragma once

#include "Common.h"

// __FILE__ �� TCHAR �� LN_FILE
#define LN_PREFILE( x )	_T( x )
#define LN_FILE	        LN_PREFILE( __FILE__ )

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
    

    enum LNErrorProcMode
    {
        LN_ERROR_PROC_CONTINUE = 0,         ///< ���b�Z�[�W�{�b�N�X�o�͌�A�p������
        //LN_ERROR_PROC_CONTINUE_CLEAR = 0,   ///< ���b�Z�[�W�{�b�N�X�o�͌�A���������G���[���N���A���Čp������
        LN_ERROR_PROC_ABORT,                ///< ���b�Z�[�W�{�b�N�X�o�͌�A������~����
        LN_ERROR_PROC_IMMABORT,             ///< �G���[�����������u�Ԃɓ�����~���� (���C�u���������̃f�o�b�O�p)
    };
}

// HRESULT �g���������������ǁA�����Œ�`�����̎g���������X�b�L������Ǝv���B
// HRESULT ���� �Ⴆ��0���Z�Ȃ񂩂�DISP_E_DIVBYZERO�Ƃ�������Ȃ��̎g��Ȃ��ƂȂ�Ȃ��݂��������B
// ���ƁA�Ȃ񂩓����悤�ȈӖ��̒l���������������C������B

//-------------------------------------------------------------------------
// ���J�p�G���[�����}�N��

#define LN_ASSERT_S( r_, format_, ... )	    { if (!(r_)) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, ( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorString( LN_ERR_ASSERT ); LNErrMD::process( LN_ERR_ASSERT ); } }

#ifdef LNOTE_DEBUG
    #define LN_ASSERT( r_, format_, ... )   { if (!(r_)) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, ( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorString( LN_ERR_ASSERT ); LNErrMD::process( LN_ERR_ASSERT ); } }
#else
    #define LN_ASSERT( r_, format_, ... )
#endif


//-------------------------------------------------------------------------
// LNRESULT �`�F�b�N�n�}�N��

#define LN_SUCCEEDED( lr_ )                         (((LNRESULT)(lr_)) >= 0)
#define LN_FAILED( lr_ )                            ( ((LNRESULT)(lr_)) < 0 )
#define LN_EVENT_HANDLED( lr_ )                     ( lr_ < 0 || lr_ == LN_HANDLED )

// LNRESULT ��Ԃ��֐��̌��ʂ����̂܂ܕԂ��ꍇ�Ɏg��
#define LN_CALL_R( x_ )                             { lr = ( x_ ); if( LN_FAILED( lr ) ) { return lr; } }
#define LN_CALL_GOTO( label_, x_ )                  { lr = ( x_ ); if( LN_FAILED( lr ) ) { goto label_; } }

// �G���[�����t���֐��Ăяo��
#define LN_PCALL_S( x_ )                            { LNErrMD::process( x_ ); }
#define LN_PCALLF_S( x_, format_, ... )             { LNErrMD::process( x_, _T( __FILE__ ),  __LINE__, format_, __VA_ARGS__ ); }

#ifdef LNOTE_DEBUG
    #define LN_PCALL( x_ )                          { LNErrMD::process( x_ ); }
    #define LN_PCALLF( x_, format_, ... )           { LNErrMD::process( x_, _T( __FILE__ ),  __LINE__, format_, __VA_ARGS__ ); }
#else
    #define LN_PCALL( x_ )                          { LNErrMD::process( x_ ); }
    #define LN_PCALLF( x_, format_, ... )           { LNErrMD::process( x_ ); }
#endif

//-------------------------------------------------------------------------
// �����p�̃G���[�ݒ�}�N��



#define LN_SETERR_S( lr_, format_, ... )	        {                  LNErrMD::setPreErrorFormat( LN_FILE, __LINE__, _T( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorString( lr_ ); }
#define LN_SETERR_R_S( lr_, format_, ... )	        {                  LNErrMD::setPreErrorFormat( LN_FILE, __LINE__, _T( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorString( lr_ ); return lr_; }
#define LN_SETERR_R_S_( r_, lr_, format_, ... )     { if ( !( r_ ) ) { LNErrMD::setPreErrorFormat( LN_FILE, __LINE__, _T( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorString( lr_ ); return lr_; } }

#ifdef LNOTE_DEBUG
    #define LN_SETERR_R_( r_, lr_, format_, ... )   { if ( !( r_ ) ) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, ( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorString( lr_ ); return lr_; } }
#else
    #define LN_SETERR_R_( r_, lr_, format_, ... )
#endif

// �w�b�_������̐ݒ� (�t�@�C�����Ȃ�)
// LN_FAILED() �ŃG���[������s���Ă���ݒ肷�邱�ƁB
//#define LN_SETERR_HEADER( format_, ... )            { }//LNErrMD::setHeaderString( format_, __VA_ARGS__ ); }
//#define LN_SETERR_GOTO_S_( r_, label_, lr_, format_, ... )  { if ( !( r_ ) ) { LNErrMD::setPreErrorFormat( LN_FILE, __LINE__, _T( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorString( lr_ ); goto label_; } }


//-------------------------------------------------------------------------
// DirectX �p�̃G���[�ݒ�}�N�� (fn_ : �Ăяo�����֐���)

#define LN_SETDXERR_S( hr_, format_, ... )          {              if ( FAILED( hr_ ) ) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, ( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorStringDx( hr_ ); } }
#define LN_SETDXERR_R_S( hr_, format_, ... )        {              if ( FAILED( hr_ ) ) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, ( format_ ), __VA_ARGS__ ); LNErrMD::setLastErrorStringDx( hr_ ); return LN_ERR_DIRECTX; } }
#define LN_DXCALL_S( x_, fn_ )                      { hr = ( x_ ); if ( FAILED( hr  ) ) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, "< %s >\n%s", fn_, #x_ ); LNErrMD::setLastErrorStringDx( hr  ); } }
#define LN_DXCALL_R_S( x_, fn_ )                    { hr = ( x_ ); if ( FAILED( hr  ) ) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, "< %s >\n%s", fn_, #x_ ); LNErrMD::setLastErrorStringDx( hr  ); return LN_ERR_DIRECTX; } }

#ifdef LNOTE_DEBUG
    #define LN_DXCALL_R( x_ )                       { hr = ( x_ ); if ( FAILED( hr  ) ) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, "< %s >\n%s", __FUNCTION__, #x_ ); LNErrMD::setLastErrorStringDx( hr ); return LN_ERR_DIRECTX; } }
    #define LN_DXCALLF_R( x_, format_, ... )        { hr = ( x_ ); if ( FAILED( hr  ) ) { LNErrMD::setPreErrorFormat( _T( __FILE__ ), __LINE__, "< %s >\n%s\n\n"format_, __FUNCTION__, #x_, __VA_ARGS__ ); LNErrMD::setLastErrorStringDx( hr ); return LN_ERR_DIRECTX; } }
#else
    #define LN_DXCALL_R( x_ )                       { hr = ( x_ ); if ( FAILED( hr  ) ) { return LN_ERR_DIRECTX; } }
    #define LN_DXCALLF_R( x_, format_, ... )        { hr = ( x_ ); }
#endif


//-------------------------------------------------------------------------
// �G���[�����p�}�N��

////#define LN_ERRPROC_S()                              { LNErrMD::process(); }

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

class LogFile;
class ErrorObserver;

//=============================================================================
// �� Error ���W���[��
//-----------------------------------------------------------------------------
///**
//	@brief      �G���[�Ɋւ�������������W���[��
//
//  @par
//              �� �g����<br>
//                  �Einitialize() �ŏ�����<br>
//                  �E�G���[�p�}�N�� ( LN_SETERR �� ) �ŃG���[��ݒ�<br>
//                  �Eprocess() �ŃG���[����<br>
//                  �Efinalize() �ŏI��<br>
//                  <br>
//                  ( �}�N���g��Ȃ��Œ��� setLastErrorStringA �Ƃ��Ă�ł��������ǁA�}�N���g�������������ƊȒP�ł� )<br>
//              <br>
//              ��{�I�ɂ��̃��W���[���� LNRESULT �֌W�̃G���[�������܂��B<br>
//              ���̂��߁A���̃��W���[���̊֐��� LNRESULT �̃G���[��Ԃ��Ă�
//              ����܂�Ӗ����Ȃ��C������̂ł����ł� bool �Ő���/���s��
//              �Ԃ��Ă��܂��B
//*/
//=============================================================================
class Error
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      ������
    //
    //  @param[in]  log_file_ : ���O���o�͂���Ƃ��Ɏg�����O�N���X
    //  @param[in]  mutex_    : �X���b�h�Z�[�t�Ŏ�������Ƃ��̃~���[�e�b�N�X
    //
    //  @retval     true  : ����
    //  @retval     false : ���s
    //
    //  @par
    //              �G���[�������W���[�������������܂��B
    //              �ʏ�A�v���O�����̐擪�ň�x�����Ăяo���܂��B<br><br>
    //              
    //              log_file_ �� NULL ���w�肷��ƃ��O���o�͂��܂���B<br><br>
    //              
    //              mutex_ ���ȗ����ꂽ�ꍇ�̓f�t�H���g�̃~���[�e�b�N�X���g���܂��B
    //*/
    //---------------------------------------------------------------------
    static bool initialize( Thread::Mutex* mutex_ = NULL );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �I������
    //*/
    //---------------------------------------------------------------------
    static void finalize();

    //---------------------------------------------------------------------
    ///**
    //  @brief      �G���[��������o�͂��郍�O�t�@�C����ݒ肷�� (�Q�ƃJ�E���g���ЂƂ�����)
    //*/
    //---------------------------------------------------------------------
    static void setLogFile( LogFile* log_file_ );


    //---------------------------------------------------------------------
    ///**
    //  @brief      �G���[������̐ݒ�̒��O����
    //
    //  @note
    //              �����w��̋��ʕ������܂Ƃ߂����́B
    ///             setLastErrorString() ���̒��O�ɌĂяo���B
    //*/
    //---------------------------------------------------------------------
    static void setPreErrorFormat( const lnChar* filename_, const int line_, const lnChar* format_, ... );


    //---------------------------------------------------------------------
    ///**
    //  @brief      �G���[�������ݒ肷��
    //*/
    //---------------------------------------------------------------------
    static void setLastErrorString( const LNRESULT lr_ );


#if LNOTE_INCLUDED_DIRECTX_HEADER
    //---------------------------------------------------------------------
    ///**
    //  @brief      �G���[�������ݒ肷�� (HRESULT �p)
    //*/
    //---------------------------------------------------------------------
    static void setLastErrorStringDx( const HRESULT hr_ );
#endif

    //---------------------------------------------------------------------
    ///**
    //  @brief      �Ō�ɐݒ肳�ꂽ�G���[��������擾����
    //*/
    //---------------------------------------------------------------------
    static const lnChar* getLastErrorString();



    //---------------------------------------------------------------------
    ///**
    //  @brief      �G���[�����̓�����@�̐ݒ�
    //
    //  @par
    //              �f�t�H���g�� LN_ERROR_PROC_ABORT �ł��B
    //*/
    //---------------------------------------------------------------------
    static void setProcessMode( LNErrorProcMode mode_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      process() �ŏo�͂����G���[���b�Z�[�W�����J�ł̂��̂ɂ���
    //
    //  @par
    //              �f�t�H���g�̒l�́ADebug �r���h�ł� false�A
    //              Release �r���h�ł� true �ł��B
    //*/
    //---------------------------------------------------------------------
    static void setEnableReleaseMode( bool flag_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���b�Z�[�W�{�b�N�X�o�͎��̃E�B���h�E�ŏ����̗L���ݒ�
    //*/
    //---------------------------------------------------------------------
    static void setEnableMinimize( bool flag_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �G���[��ێ����邩�̐ݒ� (�f�t�H���g�� false�Btrue �̏ꍇ�� reset() �ŃN���A���Ȃ��ƐV�����G���[���o�͂���Ȃ�)
    //*/
    //---------------------------------------------------------------------
    static void setEnableErrorHold( bool flag_ );


    //---------------------------------------------------------------------
    ///**
    //  @brief      �ݒ肳��Ă���G���[���N���A����
    //
    //  @par
    //              �N���A���Ȃ��ꍇ�AsetLastErrorString() ���Ă΂�Ă��V����
    //              ���b�Z�[�W�������ݒ肵�܂���B		
    //              (��������i�[����o�b�t�@�͓����ł͐ÓI�̈�Ɋm�ۂ���邽�߁A
    //              ���̊֐����Ă΂Ȃ��ƃ��������[�N����Ƃ������͂���܂���B)
    //*/
    //---------------------------------------------------------------------
    static void reset();

    //---------------------------------------------------------------------
    ///**
    //  @brief      �W���̃G���[�������s��
    //
    //  @param[in]  lr_ : setLastErrorString() ���ŃG���[�����񂪐ݒ肳��Ă��Ȃ��ꍇ�Ɏg�p����� (����`)
    //
    //  @par
    //              �G���[���ݒ肳��Ă���ꍇ�A����̏������s���܂��B<br>
    //              <br>
    //              process() ��A���ŌĂяo���Ă��A2��ڈȍ~�͉������܂���B<br>
    //              reset() ���ĂԂ��Ƃł�����x�G���[��ݒ肵�A�Ăяo�����Ƃ��ł��܂��B
    //*/
    //---------------------------------------------------------------------
    static void _process( LNRESULT lr_ );

    static inline void process( LNRESULT lr_ )
    {
        if ( LN_FAILED( lr_ ) ) { _process( lr_ ); }
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �ǉ����t���ŕW���̃G���[�������s��
    //*/
    //---------------------------------------------------------------------
    static void process( LNRESULT lr_, const lnChar* filename_, const int line_, const lnChar* format_, ... );

    /// �G���[�������̃R�[���o�b�N�C���^�[�t�F�C�X�̐ݒ�
    static void setErrorObserver( ErrorObserver* eo_ );

    /// �Ō�ɐݒ肳�ꂽ LNRESULT �l�̎擾
    static LNRESULT getLastResult();

private:

    /// ���݂̏�Ԃ���A�ŏI�I�ɏo�͂��镶������쐬����
    static void _makeOutputString( lnChar* buffer_, int size_, lnChar* title_, int title_size_ );

    /// ��������o�͂��� (ErrorObserver ���o�^����Ă��Ȃ���ΕW���o�͂ɏo�͂���)
    static void _showErrorMessage( lnChar* text_, lnChar* caption_ );

    /// LNRESULT �̃G���[���b�Z�[�W������̎擾
    static const lnChar* _getLNErrorMessage( const LNRESULT lr_ );

#if LNOTE_INCLUDED_DIRECTX_HEADER
    /// DirectX �̃G���[���b�Z�[�W������̎擾
    static const lnChar* _getDxErrorMessage( const HRESULT hr_ );
#endif
};


//=============================================================================
// �� ErrorObserver �N���X
//-----------------------------------------------------------------------------
///**
//	@brief      �G���[�������̃R�[���o�b�N�C���^�[�t�F�C�X
//*/
//=============================================================================
class ErrorObserver
{
public:

    /// �E�B���h�E�̔�\���܂��͍ŏ������K�v�Ȏ��ɌĂ΂��
    virtual void onShowWindow( bool show_ ) = 0;

    /// ���b�Z�[�W�{�b�N�X��\�����鎞�ɌĂ΂��
    virtual void onShowMessageBox( const lnChar* text_, const lnChar* title_ ) = 0;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

// �}�N�����p
typedef LNote::Core::Base::Error LNErrMD;

//=============================================================================
//								end of file
//=============================================================================