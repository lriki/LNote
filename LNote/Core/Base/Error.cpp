//=============================================================================
// �y Error �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <stdarg.h>
#include "StringUtil.h"
#include "LogFile.h"
#include "Error.h"
#include "../Thread/Mutex.h"

#if defined(LNOTE_DIRECTX)
#include <d3d9.h>
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Base
{

//=============================================================================
// �� Error ���W���[��
//=============================================================================

    static const int		BUFFER_SIZE = 1024;
    static const int		MBTITLE_SIZE = 128;     ///< ���b�Z�[�W�{�b�N�X�̃^�C�g���p�Ɋm�ۂ���o�b�t�@�T�C�Y
    static const int		CONTENTS_BUFFER_SIZE = 512;    ///< ���e�A�s�A�t�@�C���������̃o�b�t�@�T�C�Y
    static const int        LN_MAX_ERROR_CAP = 2;

    const lnChar*  HEADER_FORMAT_STRING =
        _T( "--------------------------------------------------------------------\n" )
        _T( "�� %s\n" )
        _T( "\n" )
        _T( "%s\n" );

    const lnChar*  HEADER_FORMAT_STRING_DX =
        _T( "--------------------------------------------------------------------\n" )
        _T( "�� %s\n" )
        _T( "\n" )
        _T( "%s\n" )
        _T( "\n" )
        _T( "%s\n" );

    const lnChar*  FORMAT_STRING =
        _T( "%s\n" )          // setLastErrorString() �Őݒ肳�ꂽ������
        //_T( "%s\n" )          // �~�h���E�F�A�����̃G���[������ ( DirectX �̃G���[������Ƃ� )
        _T( "\n" )
        _T( "file : %s\n" )
        _T( "line : %d\n" );

    LogFile*        mLogFile = NULL;                                ///< �G���[���o�͂��郍�O�t�@�C��

    lnChar         sPreFormatString[ CONTENTS_BUFFER_SIZE ]     = { 0 };    ///< setPreErrorFormat() �Őݒ肳���
	lnChar         sErrorContentsString[ CONTENTS_BUFFER_SIZE ] = { 0 };    ///< �w�b�_�A�t�b�^�ȊO�̕�����
    lnChar			sLastErrorString[ BUFFER_SIZE ]     = { 0 };    ///< �Ō�ɐݒ肳�ꂽ�G���[������ ( [0] �� '\0' �̏ꍇ�A�g���Ă��Ȃ� )

    LNRESULT        sLastLNRESULT = LN_OK;                          ///< �Ō�ɐݒ肳�ꂽ�G���[�� LNRESULT �l
    //void*           sWindowHandle = NULL;                           ///< ���b�Z�[�W�{�b�N�X�Ɗ֘A�t����E�B���h�E
    
    ErrorObserver*  mErrorObserver = NULL;

    LNErrorProcMode sErrorProcMode = LN_ERROR_PROC_ABORT;           ///< �G���[�����̓�����@
    int             sErrorNum = 0;
    bool            sEnableMinimize = false;                        ///< ���b�Z�[�W�{�b�N�X�o�͎��̃E�B���h�E�ŏ����̗L���ݒ�
    bool            sErrorHold = false;
    bool            sIsSetNecessarily = false;                      ///< Release ��Ԃł��\�����镶���񂪂���ꍇ true
    bool            sProcessed = false;                             ///< �����ς݃t���O
    
#ifdef LNOTE_DEBUG
    bool		    sIsReleaseMode = false;
#else
    bool		    sIsReleaseMode = true;
#endif



    

    

    static Thread::Mutex gMtx;    // initialize() �Ă΂Ȃ��Ă��ꉞ�g����悤�ɂ���׍H
    Thread::Mutex*       sMutex = &gMtx;//NULL;




    //---------------------------------------------------------------------
	// �� ������
	//---------------------------------------------------------------------
    bool Error::initialize( Thread::Mutex* mutex_ )
    {
        setlocale( LC_ALL, "" );

        finalize();

        

        if ( mutex_ )
        {
            sMutex = mutex_;
        }
        else
        {
            sMutex = LN_NEW Thread::Mutex();
        }

        reset();

        sErrorNum = 0;

        return ( sMutex != NULL );
    }

    //---------------------------------------------------------------------
	// �� �I������
	//---------------------------------------------------------------------
    void Error::finalize()
    {
        if ( sMutex != &gMtx )
        {
            SAFE_DELETE( sMutex );
        }
        LN_SAFE_RELEASE( mLogFile );
    }

    //---------------------------------------------------------------------
	// �� �G���[��������o�͂��郍�O�t�@�C����ݒ肷��
    //---------------------------------------------------------------------
    void Error::setLogFile( LogFile* log_file_ )
    {
        LN_SAFE_RELEASE( mLogFile );
        mLogFile = log_file_;
        LN_SAFE_ADDREF( mLogFile );
    }


    //---------------------------------------------------------------------
	// �� �G���[������̐ݒ�̒��O���� (�����w��̋��ʕ������܂Ƃ߂�����)
	//---------------------------------------------------------------------
    void Error::setPreErrorFormat( const lnChar* filename_, const int line_, const lnChar* format_, ... )
    {
        if ( !sErrorHold || sLastErrorString[ 0 ] == _T( '\0' ) )
        {
            // �t�@�C���p�X�̂����A�t�@�C�����̕������������
            int i = _tcslen( filename_ );
            for ( ; ( i > 0 ) && filename_[ i ] != _T( '\\' ) && filename_[ i ] != _T( ':' ) ; --i ){
}
            const lnChar* fn = &filename_[ i + 1 ];

            // �t�@�C�����̒��� + �s�� (20 ���܂�) + '\n' + '\0'
	        int meta_len = _tcslen( fn ) + 20 + 1 + 1;

            const lnChar* format_top;
            if ( format_[ 0 ] == _T( '#' ) )
            {
                format_top = &( format_[ 1 ] );
                sIsSetNecessarily = true;
            }
            else
            {
                format_top = &( format_[ 0 ] );
                sIsSetNecessarily = false;
            }

            // �ψ������當���������� tmp_buf �Ɋi�[����
            // �V�F�[�_�̃G���[�Ƃ��� 1000 ��]�T�Œ����邱�Ƃ�����̂œ��I�m��
            va_list args;
            va_start( args, format_ );

            // gcc �ɂ͂���ɑ�������֐����Ȃ������̂ŁA���̓R���ŁB
            int args_len = 2048;//_vsctprintf( format_, args ) + 1;
            lnChar* tmp_buf = static_cast< lnChar* >( malloc( args_len * sizeof( lnChar ) ) );
            _vstprintf_s( tmp_buf, args_len, format_top, args );
            
            // CONTENTS_BUFFER_SIZE �𒴂���ꍇ�͐؂�
            if ( args_len > CONTENTS_BUFFER_SIZE )
            {
                tmp_buf[ CONTENTS_BUFFER_SIZE - meta_len - 5 ] = _T( ' ' );
                tmp_buf[ CONTENTS_BUFFER_SIZE - meta_len - 4 ] = _T( '.' );
                tmp_buf[ CONTENTS_BUFFER_SIZE - meta_len - 3 ] = _T( '.' );
                tmp_buf[ CONTENTS_BUFFER_SIZE - meta_len - 2 ] = _T( '.' );
                tmp_buf[ CONTENTS_BUFFER_SIZE - meta_len - 1 ] = _T( '\0' );
            }

            _tcscpy( sErrorContentsString, tmp_buf );

            SAFE_FREE( tmp_buf );

            va_end( args );

            // �S�̂����������Ƃ��̒������o�b�t�@�T�C�Y�ȏ�ɂȂ�ꍇ�́Aargs_len ���k�߁A�������r���ŋ�؂�
	        if ( meta_len + args_len >= CONTENTS_BUFFER_SIZE )
	        {
		        args_len = ( CONTENTS_BUFFER_SIZE - meta_len ) - 1;
		        sErrorContentsString[ args_len ] = _T( '\0' );
	        }

            // sPreFormatString �ɏ����o��
            _stprintf_s( sPreFormatString, CONTENTS_BUFFER_SIZE, FORMAT_STRING, sErrorContentsString, fn, line_ );

        }
    }

    //---------------------------------------------------------------------
	// �� �G���[�������ݒ肷��
	//---------------------------------------------------------------------
    void Error::setLastErrorString( const LNRESULT lr_ )
    {
        sMutex->lock();

		// ���ɕ����񂪐ݒ肳��Ă���ꍇ�͂Ȃɂ����Ȃ�
		if ( !sErrorHold || sLastErrorString[ 0 ] == _T( '\0' ) )
        {
            sLastLNRESULT = lr_;

            _stprintf_s(
                sLastErrorString,
                BUFFER_SIZE,
                HEADER_FORMAT_STRING,
                _getLNErrorMessage( lr_ ),
                sPreFormatString );

            if ( sErrorNum < LN_MAX_ERROR_CAP )
            {
                LN_LOG_WRITE( _T( "�y�G���[�����z\n%s\n" ), sLastErrorString );
                ++sErrorNum;
            }
        }

        if ( sErrorProcMode == LN_ERROR_PROC_IMMABORT )
		{ 
        	*reinterpret_cast< int* >( 0 ) = 0;
		}

		sMutex->unlock();
    }

#if defined(LNOTE_DIRECTX)
    //---------------------------------------------------------------------
	// �� �G���[�������ݒ肷�� ( DirectX �p )
    //---------------------------------------------------------------------
    void Error::setLastErrorStringDx( const HRESULT hr_ )
    {
        sMutex->lock();

		if ( !sErrorHold || sLastErrorString[ 0 ] == _T( '\0' ) )
        {
            sLastLNRESULT = LN_ERR_DIRECTX;

            _stprintf_s(
                sLastErrorString,
                BUFFER_SIZE,
                HEADER_FORMAT_STRING_DX,
                _getLNErrorMessage( LN_ERR_DIRECTX ),
                _getDxErrorMessage( hr_ ),
                sPreFormatString );

            if ( sErrorNum < LN_MAX_ERROR_CAP )
            {
                LN_LOG_WRITE( _T( "�y�G���[�����z\n%s\n" ), sLastErrorString );
                ++sErrorNum;
            }
        }

        if ( sErrorProcMode == LN_ERROR_PROC_IMMABORT ) { 
        	*reinterpret_cast< int* >( 0 ) = 0; }

		sMutex->unlock();
    }
#endif

    //---------------------------------------------------------------------
	// �� �Ō�ɐݒ肳�ꂽ�G���[��������擾����
	//---------------------------------------------------------------------
    const lnChar* Error::getLastErrorString()
    {
        return sPreFormatString;
    }

    //---------------------------------------------------------------------
	// �� �G���[������̓�����@�̐ݒ�
    //---------------------------------------------------------------------
    void Error::setProcessMode( LNErrorProcMode mode_ )
    {
        sMutex->lock();
        sErrorProcMode = mode_;
        sMutex->unlock();
    }

    //---------------------------------------------------------------------
	// �� process() �ŏo�͂����G���[���b�Z�[�W�����J�ł̂��̂ɂ���
    //---------------------------------------------------------------------
    void Error::setEnableReleaseMode( bool flag_ )
    {
        sMutex->lock();
        sIsReleaseMode = flag_;
        sMutex->unlock();
    }

    //---------------------------------------------------------------------
	// �� ���b�Z�[�W�{�b�N�X�o�͎��̃E�B���h�E�ŏ����̗L���ݒ�
	//---------------------------------------------------------------------
    void Error::setEnableMinimize( bool flag_ )
    {
        sMutex->lock();
        sEnableMinimize = flag_;
        sMutex->unlock();
    }

    //---------------------------------------------------------------------
	// �� �G���[��ێ����邩�̐ݒ�
	//---------------------------------------------------------------------
    void Error::setEnableErrorHold( bool flag_ )
    {
        sMutex->lock();
        sErrorHold = flag_;
        sMutex->unlock();
    }

    //---------------------------------------------------------------------
	// �� ���b�Z�[�W��������N���A����
	//---------------------------------------------------------------------
    void Error::reset()
    {
        sMutex->lock();

		// �擪�� \0 ���ǂ����̃t���O�Ŕ��肵�Ă���̂ł���� OK
		sLastErrorString[ 0 ] = _T( '\0' );
        sLastLNRESULT = LN_OK;
        sProcessed = false;

        LN_LOG_WRITE( "�Ō�ɔ��������G���[���N���A���܂����B\n\n" );
        LN_LOG_WRITE( "�Ō�ɔ��������G���[���N���A���܂����B%d\n\n", 0 );

        LN_LOG_WRITE( _T( "�Ō�ɔ��������G���[���N���A���܂����B\n\n" ) );

		sMutex->unlock();
    }
    

    //---------------------------------------------------------------------
	// �� �G���[�������A�f�t�H���g�̏������s��
	//---------------------------------------------------------------------
    void Error::_process( LNRESULT lr_ )
    {
        sMutex->lock();

        // �G���[�������̏ꍇ�ɏ���������
        if ( !sProcessed )
        {
            // �G���[���������Ă���ꍇ
            if ( LN_FAILED( sLastLNRESULT ) )
            {
                // �K�v������΃E�B���h�E���ŏ���
                if ( sEnableMinimize && mErrorObserver )
                {
                    mErrorObserver->onShowWindow( false );
                }

                lnChar contents[ BUFFER_SIZE ];
                lnChar title[ MBTITLE_SIZE ];
                _makeOutputString( contents, BUFFER_SIZE, title, MBTITLE_SIZE );

                _showErrorMessage( contents, title );

                
                //::MessageBox( sWindowHandle, contents, title, MB_OK | MB_ICONERROR );

                // ������~������ꍇ
                if ( sErrorProcMode == LN_ERROR_PROC_ABORT )
                {
                    *reinterpret_cast< int* >( 0 ) = 0;
                }
                else if ( !sErrorHold )
                {
                    // �擪�� \0 ���ǂ����̃t���O�Ŕ��肵�Ă���̂ł���� OK
		            sLastErrorString[ 0 ] = _T( '\0' );
                    sLastLNRESULT = LN_OK;
                    sProcessed = false;
                    LN_LOG_WRITE( _T( "�Ō�ɔ��������G���[���N���A���܂����B\n\n" ) );
                }
                else if ( sErrorHold )
                {
                    // �ꎞ�p�B�A���Ăяo����h�~����B
                    // ������x�ĂԂ��߂ɂ͈�x reset() ���ĂԂ��ƁB
                    sProcessed = true;
                }
            }
            // �G���[������̓Z�b�g����Ă��Ȃ����ǈ����ŃG���[���󂯎���Ă���ꍇ
            else if ( LN_FAILED( lr_ ) )
            {
                
                lnChar contents[ BUFFER_SIZE ];
                lnChar title[ MBTITLE_SIZE ];
                _makeOutputString( contents, BUFFER_SIZE, title, MBTITLE_SIZE );
                _showErrorMessage( contents, title );
                


                // ������~������ꍇ
                if ( sErrorProcMode == LN_ERROR_PROC_ABORT )
                {
                    *reinterpret_cast< int* >( 0 ) = 0;
                }
                
                // �ꎞ�p�B�A���Ăяo����h�~����B
                // ������x�ĂԂ��߂ɂ͈�x reset() ���ĂԂ��ƁB
                sProcessed = true;
            }
        }
       
        sMutex->unlock();
    }

    //---------------------------------------------------------------------
	// �� �ǉ����t���ŕW���̃G���[�������s��
    //---------------------------------------------------------------------
    void Error::process( LNRESULT lr_, const lnChar* filename_, const int line_, const lnChar* format_, ... )
    {
        // ���������񂪂Ȃ����A���J�ł̏ꍇ�� process() �ɔC����
        if ( format_ == NULL || sIsReleaseMode )
        {
            process( lr_ );
            return;
        }

        sMutex->lock();


        lnChar output_buffer[ BUFFER_SIZE ];
        lnChar output_title[ MBTITLE_SIZE ];

        // �ψ������當���������� buf �Ɋi�[����
        va_list args;
        va_start( args, format_ );

        int args_len = 2048;//_vsctprintf( format_, args ) + 1;
        lnChar* buf = static_cast< lnChar* >( malloc( args_len * sizeof( lnChar ) ) );
        _vstprintf_s( buf, args_len, format_, args );

        va_end( args );

        // output_buffer �ɁAbuf�ƃt�@�C�����ƍs�������킹�ďo��
        int exp_len = _stprintf_s(
            output_buffer,
            BUFFER_SIZE,
            _T( "--------------------------------------------------------------------\n�� %s\n\nFile:%s\nLine:%d\n\n" ), 
            buf, filename_, line_ );

        SAFE_FREE( buf );

        // ���݂̏�Ԃ���o�͕���������
        _makeOutputString( &output_buffer[ exp_len ], BUFFER_SIZE - exp_len - 1, output_title, MBTITLE_SIZE );

        // �o��
        _showErrorMessage( output_buffer, output_title );

        // ������~������ꍇ
        if ( sErrorProcMode == LN_ERROR_PROC_ABORT )
        {
            *reinterpret_cast< int* >( 0 ) = 0;
        }
        else if ( !sErrorHold )
        {
            // �擪�� \0 ���ǂ����̃t���O�Ŕ��肵�Ă���̂ł���� OK
            sLastErrorString[ 0 ] = _T( '\0' );
            sLastLNRESULT = LN_OK;
            sProcessed = false;
            LN_LOG_WRITE( _T( "�Ō�ɔ��������G���[���N���A���܂����B\n\n" ) );
        }
        else if ( sErrorHold )
        {
        
            // �ꎞ�p�B�A���Ăяo����h�~����B
            // ������x�ĂԂ��߂ɂ͈�x reset() ���ĂԂ��ƁB
            sProcessed = true;
        }


        sMutex->unlock();
    }

    //---------------------------------------------------------------------
	// �� �G���[�������̃R�[���o�b�N�C���^�[�t�F�C�X�̐ݒ�
    //---------------------------------------------------------------------
    void Error::setErrorObserver( ErrorObserver* eo_ )
    { 
        mErrorObserver = eo_;
    }

    //---------------------------------------------------------------------
	// �� �Ō�ɐݒ肳�ꂽ LNRESULT �l�̎擾
    //---------------------------------------------------------------------
    LNRESULT Error::getLastResult()
    {
        return sLastLNRESULT;
    }

    //---------------------------------------------------------------------
	// �� ���݂̏�Ԃ���A�ŏI�I�ɏo�͂��镶������쐬����
    //---------------------------------------------------------------------
    void Error::_makeOutputString( lnChar* buffer_, int size_, lnChar* title_, int title_size_ )
    {
        // ���J�ł̏ꍇ
        if ( sIsReleaseMode )
        {
            // �����[�X�łł��\�����镶���񂪂���΁A������o�͂���
            if ( sIsSetNecessarily )
            {
                _tcsncpy( buffer_, sErrorContentsString, size_ );
            }
            // �����ꍇ�̓f�t�H���g
            else
            {
                _stprintf_s( buffer_, size_, _T( "�����G���[���������܂����B[ %d ]" ), sLastLNRESULT );
            }

            _tcscpy( title_, _T( "Error \n" ) );
        }
        // ���J�łł͂Ȃ��ꍇ
        else
        {
            _tcsncpy( buffer_, sLastErrorString, size_ );
            _tcsncpy( title_, _T( "LNote �G���[" ), title_size_ );
        }
    }

    /// ��������o�͂��� (ErrorObserver ���o�^����Ă��Ȃ���ΕW���o�͂ɏo�͂���)
    void Error::_showErrorMessage( lnChar* text_, lnChar* caption_ )
    {
        if ( mErrorObserver )
        {
            mErrorObserver->onShowMessageBox( text_, caption_ );
        }
        else
        {
            _tprintf( "--------------------------------\n[%s]\n\n%s\n--------------------------------\n", caption_, text_ );
        }
    }
	
    /*
    //---------------------------------------------------------------------
	// �� �G���[�������ݒ肷�� ( wchar_t )
    //---------------------------------------------------------------------
    void Error::_setLastErrorStringW( const wchar_t* filename_, const int line_, const wchar_t* format_, va_list args_, const wchar_t* sub_msg_ )
    {
        // �t�@�C���p�X�̂����A�t�@�C�����̕������������
        int i = wcslen( filename_ );
        for ( ; ( i > 0 ) && filename_[ i ] != L'\\' && filename_[ i ] != L':' ; --i ){}
        const wchar_t* fn = &filename_[ i + 1 ];

	    // �ψ������瓾���镶����̒����ȊO�̒���
	    int meta_len = 0;
	    meta_len += wcslen( fn );			    // �t�@�C�����̒���
	    meta_len += 20;						    // ���C�����B20 ���܂�
	    meta_len += wcslen( FORMAT_STRING_W );  // fmt ������̒���
	    meta_len += wcslen( sub_msg_ );		    // �T�u������

	    // �ψ������當���������� args_buf �Ɋi�[����
	    int args_len = _vscwprintf( format_, args_ ) + 1;
	    wchar_t* args_buf = static_cast< wchar_t* >( malloc( sizeof( wchar_t ) * args_len ) );
	    vswprintf_s( args_buf, args_len, format_, args_ );

	    // �S�̂����������Ƃ��̒������o�b�t�@�T�C�Y�ȏ�ɂȂ�ꍇ�́Aargs_len ���k�߁A�������r���ŋ�؂�
	    if ( meta_len + args_len >= BUFFER_SIZE )
	    {
		    args_len = ( BUFFER_SIZE - meta_len ) - 1;
		    args_buf[ args_len ] = '\0';
	    }

	    // �擪�� # �̏ꍇ�͂�����΂�
	    wchar_t* arg_s = ( args_buf[ 0 ] == RELEASE_OUT_KEY_W ) ? &( args_buf[ 1 ] ) : &( args_buf[ 0 ] );

	    // sLastErrorStringW �ɐݒ肷��
	    swprintf( sLastErrorStringW, FORMAT_STRING_W, sub_msg_, arg_s, fn, line_ );

        wcscpy( sLastContentsString, arg_s );

	    SAFE_FREE( args_buf );

        LN_LOG_WRITE( L"�y�G���[�����z\n%s\n", sLastErrorStringW );
        
        sNecessarilyString[ 0 ] = L'\0';

        // ���J�ł̏ꍇ
        if ( sIsReleaseMode )
        {
            if ( format_[ 0 ] == RELEASE_OUT_KEY_W )
		    {
			    // �ψ������當���������� args_buf �Ɋi�[����
			    int args_len = _vscwprintf( format_, args_ ) + 1;
			    wchar_t* args_buf = static_cast< wchar_t* >( malloc( sizeof( wchar_t ) * args_len ) );
			    vswprintf_s( args_buf, args_len, format_, args_ );

			    // �S�̂����������Ƃ��̒������o�b�t�@�T�C�Y�ȏ�ɂȂ�ꍇ�́Aargs_len ���k�߁A�������r���ŋ�؂�
			    if ( args_len >= BUFFER_SIZE )
			    {
				    args_len = BUFFER_SIZE - 1;
				    args_buf[ args_len ] = L'\0';
			    }

			    wcscpy_s( sNecessarilyString, BUFFER_SIZE, &( args_buf[ 1 ] ) );
    			
			    SAFE_FREE( args_buf );

                sNecessarilyStringIsWide = true;
		    }
        }
    }
    */

/*
    //---------------------------------------------------------------------
	// �� �G���[���� ( char )
    //---------------------------------------------------------------------
    bool Error::_processA()
    {
        bool r;

        // ���J�ł̏ꍇ
        if ( sIsReleaseMode )
        {
            char buf[ 32 ];
            sprintf( buf, "�G���[ ( %d )", -sLastLNRESULT );
            ::MessageBoxA( sWindowHandle, sLastErrorStringA, "�G���[", MB_OK | MB_ICONERROR );
            r = false;
        }
        // ���ʂɕ\��
        else
        {
            ::MessageBoxA( sWindowHandle, sLastErrorStringA, "LNote Error", MB_OK | MB_ICONERROR );
            r = false;
        }
        return false;
    }

    //---------------------------------------------------------------------
	// �� �G���[���� ( wchar_t )
    //---------------------------------------------------------------------
    bool Error::_processW()
    {
        bool r;

        // ���J�ł̏ꍇ
        if ( sIsReleaseMode )
        {
            wchar_t buf[ 32 ];
            wsprintf( buf, L"�G���[ ( %d )", -sLastLNRESULT );
            ::MessageBoxW( sWindowHandle, sLastErrorStringW, L"�G���[", MB_OK | MB_ICONERROR );
            r = false;
        }
        // ���ʂɕ\��
        else
        {
            ::MessageBoxW( sWindowHandle, sLastErrorStringW, L"LNote Error", MB_OK | MB_ICONERROR );
            r = false;
        }
        return false;
    }
*/
    //---------------------------------------------------------------------
	// �� LNRESULT �̃G���[���b�Z�[�W������̎擾 ( char )
    //---------------------------------------------------------------------
    const lnChar* Error::_getLNErrorMessage( const LNRESULT lr_ )
    {
        switch ( lr_ )
        {
            case LN_ERR_ASSERT:        
            	return _T( "�G���[���������܂���" );
            case LN_ERR_INVALIDCALL:   
            	return _T( "���\�b�h�̌Ăяo���������ł��B�����ɕs���Ȓl���n���ꂽ�\��������܂�" );
            case LN_ERR_OUTOFMEMORY:   
            	return _T( "���̑������������̂ɏ\���ȃ������̈悪����܂���" );
            //case LN_ERR_INDEX:         
            //	return _T( "�z��͈̔͊O���Q�Ƃ��悤�Ƃ��܂���" );
            case LN_ERR_NULLPOINTER:   
            	return _T( "NULL �|�C���^�ɃA�N�Z�X���悤�Ƃ��܂���" );
            case LN_ERR_FILENOTFOUND:  
            	return _T( "�t�@�C����������܂���ł���" );
            case LN_ERR_SYSTEM:        
            	return _T( "�V�X�e���@�\�̌Ăяo���ŃG���[���������܂���" );
            case LN_ERR_INITIALIZED:   
            	return _T( "�I�u�W�F�N�g�͊��ɏ���������Ă��܂�" );
            case LN_ERR_FATAL:         
            	return _T( "�v���I�ȃG���[���������܂���" );
            case LN_ERR_DIRECTX:       
            	return _T( "DirectX �̃V�X�e���ŃG���[���������܂���" );
            default:                   
            	return _T( "[ �s���ȃG���[�R�[�h ]" );
        }
    }

#if defined(LNOTE_DIRECTX)
    //---------------------------------------------------------------------
	// �� DirectX �̃G���[���b�Z�[�W������̎擾 ( char )
    //---------------------------------------------------------------------
    const lnChar* Error::_getDxErrorMessage( const HRESULT hr_ )
    {
        switch ( hr_ )
		{
            // D3DERR
			case D3DOK_NOAUTOGEN:
				return _T( "[ D3DOK_NOAUTOGEN ]\n �������R�[�h�@�܂��́A�~�b�v�}�b�v�͎�����������܂���B" );
				break;
			case D3DERR_CONFLICTINGRENDERSTATE:
				return _T( "[ D3DERR_CONFLICTINGRENDERSTATE ]\n���ݐݒ肳��Ă��郌���_�����O�X�e�[�g�͓����ɂ͎g���܂���B" );
				break;
			case D3DERR_CONFLICTINGTEXTUREFILTER:
				return _T( "[ D3DERR_CONFLICTINGTEXTUREFILTER ]\n���݂̃e�N�X�`���t�B���^�͓����ɂ͎g���܂���B" );
				break;
			case D3DERR_CONFLICTINGTEXTUREPALETTE:
				return _T( "[ D3DERR_CONFLICTINGTEXTUREPALETTE ]\n���݂̃e�N�X�`���͓����ɂ͎g���܂���B" );
				break;
			case D3DERR_DEVICELOST:
				return _T( "[ D3DERR_DEVICELOST ]\n�f�o�C�X���X�g���������Ă��ă��Z�b�g�ł��܂���B" );
				break;
			case D3DERR_DEVICENOTRESET:
				return _T( "[ D3DERR_DEVICENOTRESET ]\n�f�o�C�X���X�g���������Ă��܂������Z�b�g�ł��܂��B" );
				break;
			case D3DERR_DRIVERINTERNALERROR:
				return _T( "[ D3DERR_DRIVERINTERNALERROR ]\n�����h���C�o�G���[�ł��B" );
				break;
			case D3DERR_INVALIDCALL:
				return _T( "[ D3DERR_INVALIDCALL ]\n���\�b�h�̌Ăяo���������ł��B\n�����ɕs���Ȓl���n���ꂽ�\��������܂��B" );
				break;
			case D3DERR_INVALIDDEVICE:
				return _T( "[ D3DERR_INVALIDDEVICE ]\n�v�����ꂽ�f�o�C�X�̎�ނ������ł��B" );
				break;
			case D3DERR_MOREDATA:
				return _T( "[ D3DERR_MOREDATA ]\n�w�肳�ꂽ�o�b�t�@ �T�C�Y�ɕێ��ł���ȏ�̃f�[�^�����݂��܂��B" );
				break;
			case D3DERR_NOTAVAILABLE:
				return _T( "[ D3DERR_NOTAVAILABLE ]\n���̃f�o�C�X�́A�Ɖ�ꂽ�e�N�j�b�N���T�|�[�g���Ă��܂���B" );
				break;
			case D3DERR_NOTFOUND:
				return _T( "[ D3DERR_NOTFOUND ]\n�v�����ꂽ���ڂ�������܂���ł����B" );
				break;
			case D3DERR_OUTOFVIDEOMEMORY:
				return _T( "[ D3DERR_OUTOFVIDEOMEMORY ]\nDirect3D ���������s���̂ɏ\���ȃf�B�X�v���C������������܂���B" );
				break;
			case D3DERR_TOOMANYOPERATIONS:
				return _T( "[ D3DERR_TOOMANYOPERATIONS ]\n�f�o�C�X���T�|�[�g���Ă��鐔��葽����\n�e�N�X�`���t�B���^�����O�������v������܂����B" );
				break;
			case D3DERR_UNSUPPORTEDALPHAARG:
				return _T( "[ D3DERR_UNSUPPORTEDALPHAARG ]\n�A���t�@�`�����l���ɑ΂��Ďw�肳��Ă���\n�e�N�X�`���u�����f�B���O�������A�f�o�C�X���T�|�[�g���Ă��܂���B" );
				break;
			case D3DERR_UNSUPPORTEDALPHAOPERATION:
				return _T( "[ D3DERR_UNSUPPORTEDALPHAOPERATION ]\n�A���t�@�`�����l���ɑ΂��Ďw�肳��Ă���\n�e�N�X�`���u�����f�B���O�������A�f�o�C�X���T�|�[�g���Ă��܂���B" );
				break;
			case D3DERR_UNSUPPORTEDCOLORARG:
				return _T( "[ D3DERR_UNSUPPORTEDCOLORARG ]\n�F�l�ɑ΂��Ďw�肳��Ă���e�N�X�`���u�����f�B���O�������A\n�f�o�C�X���T�|�[�g���Ă��܂���B" );
				break;
			case D3DERR_UNSUPPORTEDCOLOROPERATION:
				return _T( "[ D3DERR_UNSUPPORTEDCOLOROPERATION ]\n�F�l�ɑ΂��Ďw�肳��Ă���e�N�X�`���u�����f�B���O�������A\n�f�o�C�X���T�|�[�g���Ă��܂���B" );
				break;
			case D3DERR_UNSUPPORTEDFACTORVALUE:
				return _T( "[ D3DERR_UNSUPPORTEDFACTORVALUE ]\n�f�o�C�X���w�肳�ꂽ�e�N�X�`���W���l���T�|�[�g���Ă��܂���B" );
				break;
			case D3DERR_UNSUPPORTEDTEXTUREFILTER:
				return _T( "[ D3DERR_UNSUPPORTEDTEXTUREFILTER ]\n�f�o�C�X���w�肳�ꂽ�e�N�X�`�� �t�B���^���T�|�[�g���Ă��܂���B" );
				break;
			case D3DERR_WRONGTEXTUREFORMAT:
				return _T( "[ D3DERR_WRONGTEXTUREFORMAT ]\n�e�N�X�`�� �T�[�t�F�C�X�̃s�N�Z�� �t�H�[�}�b�g�������ł��B" );
				break;

            // D3DXERR
            case D3DXERR_INVALIDDATA:
				return _T( "[ D3DXERR_INVALIDDATA ]" );
				break;

            // E_
			case E_FAIL:
				return _T( "[ E_FAIL ]\nDirect3D �T�u�V�X�e�����Ō����s���̃G���[���������܂����B" );
				break;
			case E_INVALIDARG:
				return _T( "[ E_INVALIDARG ]\n�����ȃp�����[�^���֐��ɓn����܂����B" );
				break;
			case E_OUTOFMEMORY:
				return _T( "[ E_OUTOFMEMORY ]\nDirect3D ���Ăяo�����������邽�߂̏\���ȃ����������蓖�Ă邱�Ƃ��ł��܂���ł����B" );
				break;
			default:
				return _T( "[ �s���ȃG���[�R�[�h ]" );
				break;
		}
    }
#endif


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================