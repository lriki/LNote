//=============================================================================
//�y AudioSource �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../File/FileUtil.h"
#include "AudioUtil.h"
#include "AudioResourceManager.h"
#include "AudioSource.h"

#ifndef WAVE_FORMAT_PCM
    #define WAVE_FORMAT_PCM     1
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Audio
{

//=============================================================================
// �� AudioSourceBase �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    AudioSourceBase::AudioSourceBase( ResourceManager* manager_ )
        : mManager  ( manager_ )
    {
        LN_SAFE_ADDREF( mManager );
    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    AudioSourceBase::~AudioSourceBase()
    {
        LN_SAFE_RELEASE( mManager );
    }


//=============================================================================
// �� Wave �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
	//---------------------------------------------------------------------
	Wave::Wave( ResourceManager* manager_ )
        : AudioSourceBase           ( manager_ )
        , mInFile                   ( NULL )
        , mSourceDataSize			( 0 )
		, mDataOffset				( 0 )
		, mPCMDataSize				( 0 )
		, mTotalTime				( 0 )
		, mOnmemoryPCMBuffer		( NULL )
		, mOnmemoryPCMBufferSize	( 0 )
        , mTotalSamples             ( 0 )
	{
        memset( &mWaveFormat, 0, sizeof( mWaveFormat ) );
    }

	//---------------------------------------------------------------------
	// �� �f�X�g���N�^
	//---------------------------------------------------------------------
	Wave::~Wave()
	{
		SAFE_DELETE_ARRAY( mOnmemoryPCMBuffer );
        LN_SAFE_RELEASE( mInFile );
	}

	//---------------------------------------------------------------------
	// �� �I�[�f�B�I�t�@�C���Ƃ��Ĉ����X�g���[����ݒ肷��
	//---------------------------------------------------------------------
    LNRESULT Wave::setStream( File::IInStream* stream_ )
	{
		mInFile = stream_;
        mInFile->addRef();

		// �O�̂��߃t�@�C���|�C���^��擪�ɖ߂�
		mInFile->seek( 0 );

		// �w�b�_�ǂݍ���ŔO�̂��߃t�H�[�}�b�g�`�F�b�N
		WaveFileHeader rh;
		mInFile->read( &rh, sizeof( WaveFileHeader ) );
		if ( memcmp( &rh.RIFF, "RIFF", 4 ) != 0 ||
			 memcmp( &rh.WaveHeader, "WAVE", 4 ) != 0 ||
			 memcmp( &rh.FmtChunk, "fmt ", 4 ) != 0 )
		{
		    LN_THROW_FileFormat( 0 );
		}


        mWaveFormat.FormatTag		= File::readU16( mInFile );
		mWaveFormat.Channels		= File::readU16( mInFile );
		mWaveFormat.SamplesPerSec	= File::readU32( mInFile );
		mWaveFormat.AvgBytesPerSec	= File::readU32( mInFile );
		mWaveFormat.BlockAlign		= File::readU16( mInFile );
		mWaveFormat.BitsPerSample	= File::readU16( mInFile );

		// �g�������̂���t�@�C���̏ꍇ
		if ( mWaveFormat.FormatTag != WAVE_FORMAT_PCM )
		{
			mWaveFormat.EXSize = File::readU16( mInFile );
			// �g�������ǂݍ���
			//...
            //mInFile->seek( mWaveFormat.cbSize, SEEK_CUR );

            LN_THROW_FileFormat( 0, "Wave file extensions are not supported.\nmWaveFormat.FormatTag : %d", mWaveFormat.FormatTag );
		}
		else
		{
			mWaveFormat.EXSize = 0;
		}
		
		u8 chunk[ 4 ];
		u32 len;

		// 'data' �`�����N��T��
		// ( �g�������֌W�Ȃ��A�Ƃ肠�����f�[�^��ǂݐi�߂� data ��T�� )
		while ( mInFile->read( chunk, 4 ) == 4 )
		{
			// �f�[�^�T�C�Y�擾 ( data �`�����N�̎��� 4 �o�C�g���f�[�^�T�C�Y )
			len = File::readU32( mInFile );

			// 'data' �`�����N���������Ă���ꍇ�͔�����
			if ( memcmp( chunk, "data", 4 ) == 0 ) { 
				break; }

            if ( mInFile->getPosition() == mInFile->getSize() )
			{
			    LN_THROW_FileFormat( 0, "not found 'data' chunk." );
            }

			mInFile->seek( len, SEEK_CUR );
		}

		// �t�@�C���|�C���^�̌��݈ʒu ( data �`�����N���̃f�[�^�ʒu ) ���L��
        mDataOffset = mInFile->getPosition();

		// ���f�[�^�̃T�C�Y�� data �`�����N���̃f�[�^�̃T�C�Y
		mOnmemoryPCMBufferSize = mPCMDataSize = len;

		mSourceDataSize = mOnmemoryPCMBufferSize;


		// �S�̂̍Đ����Ԃ��v�Z����
		double t = static_cast< double >( mPCMDataSize ) / ( static_cast< double >( mWaveFormat.AvgBytesPerSec ) * 0.001 );
		mTotalTime = static_cast< u32 >( t );

        // �S�̂̍Đ��T���v����
        mTotalSamples = mOnmemoryPCMBufferSize / ( mWaveFormat.BitsPerSample / 8 ) / mWaveFormat.Channels; 

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� �I���������Đ��p�ɑS�Ẵf�[�^��ǂݍ���
	//---------------------------------------------------------------------
	LNRESULT Wave::fillBufferAndReleaseStream()
	{
		if ( mOnmemoryPCMBuffer == NULL )
		{
			// �I���������Đ�����Ƃ��ɕK�v�ȃo�b�t�@�̃T�C�Y�͌��f�[�^�̃T�C�Y�Ɠ���
			mOnmemoryPCMBufferSize = mPCMDataSize;

			// �������m��
			mOnmemoryPCMBuffer = LN_NEW u8[ mOnmemoryPCMBufferSize ];

			// ���݂̃V�[�N�ʒu���o���Ă���
            int old_seek = mInFile->getPosition();

			// �t�@�C���|�C���^���f�[�^������ꏊ�̐擪�ɃZ�b�g
			mInFile->seek( mDataOffset );

			// �S���ǂݍ���
			int size = mInFile->read( mOnmemoryPCMBuffer, mOnmemoryPCMBufferSize );

			// �ǂݍ��񂾃T�C�Y���ςȏꍇ�̓G���[
		    LN_THROW_InvalidCall( ( size == mOnmemoryPCMBufferSize ), "read file size is incorrect.\nThere is a possibility that the file is corrupted." );
		   
			// �V�[�N�ʒu�����ɖ߂�
			mInFile->seek( old_seek );

            LN_SAFE_RELEASE( mInFile );
		}
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� �f�[�^�� buffer_ �֓ǂݍ���
	//---------------------------------------------------------------------
	LNRESULT Wave::read( void* buffer_, u32 buffer_size_, u32* read_size_, u32* write_size_ )
	{
        if ( !buffer_ || !( buffer_size_ > 0 ) || !( read_size_ ) || !( write_size_ ) ) { 
	return LN_ERR_NULLPOINTER; }
		//LN_SETERR_R_S_( ( buffer_ ),      LN_ERR_ASSERT, "< Audio::Wave::read >\n 'buffer_' is NULL" );
		//LN_SETERR_R_S_( ( buffer_size_ > 0 ),    LN_ERR_ASSERT, "< Audio::Wave::read >\n 'buffer_size_' is 0" );
		//LN_SETERR_R_S_( ( read_size_ ),   LN_ERR_ASSERT, "< Audio::Wave::read >\n 'read_size_' is NULL" );
		//LN_SETERR_R_S_( ( write_size_ ),  LN_ERR_ASSERT, "< Audio::Wave::read >\n 'write_size_' is NULL" );

		// �ǂݍ��ރT�C�Y
		u32 read_size = buffer_size_;
		// �\�[�X�̃T�C�Y�𒴂��Ă���ꍇ�̓\�[�X�T�C�Y���ǂ�
		if ( buffer_size_ > mPCMDataSize )
		{
			read_size = mPCMDataSize;
		}

		int size = mInFile->read( buffer_, read_size );

		// ���f�[�^����ǂݍ��񂾃T�C�Y�ƁAbuffer_ �֏������񂾃T�C�Y�͓���
		*read_size_ = static_cast< u32 >( size );
		*write_size_ = static_cast< u32 >( size );

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� �t�@�C���|�C���^�ړ�
	//---------------------------------------------------------------------
	LNRESULT Wave::seek( u32 offset_ )
	{
		if ( offset_ < mPCMDataSize )
		{
			mInFile->seek( mDataOffset + offset_ );
		}
        return LN_OK;
	}



//=============================================================================
// �� Audio::Source::MP3 �N���X
//=============================================================================
#if defined(LNOTE_MSVC)

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
	//---------------------------------------------------------------------
	MP3::MP3( ResourceManager* manager_ )
        : AudioSourceBase               ( manager_ )
        , mInFile                       ( NULL )
        , mMP3WaveFormat				( NULL )
		, mACMStreamHandle				( NULL )
		, mSourceDataSize				( 0 )
		, mDataOffset					( 0 )
		, mID3vTagFieldSize				( 0 )
		, mOnmemoryPCMBuffer			( NULL )
		, mOnmemoryPCMBufferSize		( 0 )
		, mStreamingPCMBufferSize		( 0 )
		, mMP3SourceBufferParSec		( NULL )
		, mMP3SourceBufferSizeParSec	( 0 )
		, mTotalTime					( 0 )
        , mTotalSamples                 ( 0 )
		, mResetFlag					( true )
	{
		mMP3WaveFormat = LN_NEW MPEGLAYER3WAVEFORMAT;
		mACMStreamHandle = LN_NEW HACMSTREAM;
		HACMSTREAM* acm = (HACMSTREAM*)mACMStreamHandle;
		*acm = 0;
	}

	//---------------------------------------------------------------------
	// �� �f�X�g���N�^
	//---------------------------------------------------------------------
	MP3::~MP3()
	{
		HACMSTREAM* acm = (HACMSTREAM*)mACMStreamHandle;
		if ( *acm )
		{
			acmStreamClose( *acm, 0 );
		}

		SAFE_DELETE( mMP3WaveFormat );
		SAFE_DELETE( mACMStreamHandle );
		SAFE_DELETE_ARRAY( mMP3SourceBufferParSec );
		SAFE_DELETE_ARRAY( mOnmemoryPCMBuffer );
        LN_SAFE_RELEASE( mInFile );

        //// �L���b�V���ɒǉ�����Ă��Ȃ��ꍇ�� Manager �ւ̎Q�Ƃ������Ă���̂ŉ��
        //if ( !isCaching() )
        //{
        //    LN_SAFE_RELEASE( mManager );
        //}
	}

	//---------------------------------------------------------------------
	// �� �I�[�f�B�I�t�@�C���Ƃ��Ĉ����X�g���[����ݒ肷��
	//---------------------------------------------------------------------
	LNRESULT MP3::setStream( File::IInStream* stream_ )
	{
        LN_SETERR_R_S_( ( !mInFile ), LN_ERR_ASSERT, "< Audio::MP3::setStream >\n�����̃X�g���[����ݒ肵�悤�Ƃ��܂����B" );

        LNRESULT lr;

		mInFile = stream_;
        mInFile->addRef();

		// �t�@�C���|�C���^��擪�ɖ߂�
		mInFile->seek( 0 );

		mDataOffset = 0;

		// ID3v �m�F
        LN_CALL_R( _checkId3v() );
		
		// PCM �t�H�[�}�b�g�擾
        LN_CALL_R( _getPCMFormat() );

		// mp3 �� PCM �Ƀf�R�[�h�������� wave �t�H�[�}�b�g���擾
		MPEGLAYER3WAVEFORMAT* mp3_format = mMP3WaveFormat;
        WAVEFORMATEX wav_fmt_ex;
        wav_fmt_ex.wFormatTag = WAVE_FORMAT_PCM;
		acmFormatSuggest( NULL, &mp3_format->wfx, &wav_fmt_ex, sizeof( WAVEFORMATEX ), ACM_FORMATSUGGESTF_WFORMATTAG );
        
		// ACM �ϊ��X�g���[�����J�� ( mp3 �� wave )
		HACMSTREAM* acm = mACMStreamHandle;//(HACMSTREAM*)&mACMStreamHandle;
		acmStreamOpen( acm, NULL, &mp3_format->wfx, &wav_fmt_ex, NULL, 0, 0, 0 );

        AudioUtil::convWAVEFORMATEXToLNWaveFormat( wav_fmt_ex, &mWaveFormat );

		// �S�̂�ϊ��������� PCM �T�C�Y�� mOnmemoryPCMBufferSize �Ɋi�[
        DWORD pcm_size = 0;
		acmStreamSize( *acm, mSourceDataSize, &pcm_size, ACM_STREAMSIZEF_SOURCE );
        mOnmemoryPCMBufferSize = pcm_size;

		// 1 �b���� mp3 �f�[�^��ϊ��������́A�œK�ȓ]���� PCM �o�b�t�@�T�C�Y���擾����
		DWORD wave_buffer_size;
		acmStreamSize( *acm, mp3_format->wfx.nAvgBytesPerSec, &wave_buffer_size, ACM_STREAMSIZEF_SOURCE );
		mStreamingPCMBufferSize = wave_buffer_size;

		//HACMSTREAM acm2;
		//MMRESULT m = acmStreamOpen( &acm2, NULL, &mp3_format->wfx, &mWaveFormat, NULL, 0, 0, 0 );
		//LASSERT_STRONG_RETURN( (m!=0), false, "acmStreamOpen"  );
		//acmStreamSize( *acm, mMP3BufferSize, &mMP3BufferSize, ACM_STREAMSIZEF_SOURCE );
		//acmStreamClose( acm2, 0 );


		// �X�g���[�~���O�Đ��p�̃o�b�t�@�̃T�C�Y���擾 ( mp3_format->wfx.nAvgBytesPerSec �͉��̃T�C�Y )
		mMP3SourceBufferSizeParSec = mp3_format->wfx.nAvgBytesPerSec;

		// 1 �b���� mp3 �f�[�^�̃T�C�Y���A���������m��
		mMP3SourceBufferParSec = LN_NEW u8[ mMP3SourceBufferSizeParSec ];

		// �S�̂̍Đ����Ԃ��v�Z����
		double t = static_cast< double >( mOnmemoryPCMBufferSize ) / ( static_cast< double >( mWaveFormat.AvgBytesPerSec ) * 0.001 );
		mTotalTime = static_cast< u32 >( t );

        // �S�̂̍Đ��T���v����
        mTotalSamples = mOnmemoryPCMBufferSize / ( mWaveFormat.BitsPerSample / 8 ) / mWaveFormat.Channels;

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� �I���������Đ��p�ɑS�Ẵf�[�^��ǂݍ���
	//---------------------------------------------------------------------
	LNRESULT MP3::fillBufferAndReleaseStream()
	{
		if ( mOnmemoryPCMBuffer == NULL )
		{
			MMRESULT mr;
			HACMSTREAM* acm = (HACMSTREAM*)mACMStreamHandle;

			// mp3 �f�[�^�S�̂��i�[����o�b�t�@���쐬���ēǂݍ���
			u8* mp3_buffer = (u8*)malloc( mSourceDataSize );
			mInFile->seek( mDataOffset, SEEK_SET );
			mInFile->read( mp3_buffer, mSourceDataSize );

			// �S�̂�ϊ��������� PCM �T�C�Y�� mPCMSize �Ɋi�[
            DWORD pcm_size = 0;
			acmStreamSize( *acm, mSourceDataSize, &pcm_size, ACM_STREAMSIZEF_SOURCE );
            mOnmemoryPCMBufferSize = pcm_size;

			// �擾�����T�C�Y�Ńo�b�t�@�m��
			mOnmemoryPCMBuffer = LN_NEW u8[ mOnmemoryPCMBufferSize ];

			// ACM �w�b�_�ɕϊ��o�b�t�@�ݒ�
			ACMSTREAMHEADER ash;
			ZeroMemory( &ash, sizeof( ACMSTREAMHEADER ) );
			ash.cbStruct    = sizeof( ACMSTREAMHEADER );
			ash.pbSrc       = mp3_buffer;
			ash.cbSrcLength = mSourceDataSize;
			ash.pbDst       = (LPBYTE)mOnmemoryPCMBuffer;
			ash.cbDstLength = mOnmemoryPCMBufferSize;

			// �R���o�[�g���s
			mr = acmStreamPrepareHeader( *acm, &ash, 0 );
            LN_SETERR_R_S_( ( mr == 0 ), LN_ERR_SYSTEM, "< Audio::MP3::fillBufferAndReleaseStream >\n::acmStreamPrepareHeader" );
			mr = acmStreamConvert( *acm, &ash, 0 );
			LN_SETERR_R_S_( ( mr == 0 ), LN_ERR_SYSTEM, "< Audio::MP3::fillBufferAndReleaseStream >\n::acmStreamConvert" );
			mr = acmStreamUnprepareHeader( *acm, &ash, 0 );
			LN_SETERR_R_S_( ( mr == 0 ), LN_ERR_SYSTEM, "< Audio::MP3::fillBufferAndReleaseStream >\n::acmStreamUnprepareHeader" );

			// ���ۂ� PCM �o�b�t�@�ɏ������񂾃f�[�^�T�C�Y���L������
			mOnmemoryPCMBufferSize = ash.cbDstLengthUsed;

			// �ꎞ�o�b�t�@���
			free( mp3_buffer );
			mp3_buffer = NULL;

            LN_SAFE_RELEASE( mInFile );
		}

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� �f�[�^���f�R�[�h���Abuffer_ �ɏ�������
	//---------------------------------------------------------------------
	LNRESULT MP3::read( void* buffer_, u32 buffer_size_, u32* read_size_, u32* write_size_ )
	{
		ZeroMemory( buffer_, buffer_size_ );
		// �t�@�C������f�[�^�ǂݍ���
		u32 read_size = mInFile->read( mMP3SourceBufferParSec, mMP3SourceBufferSizeParSec, mMP3SourceBufferSizeParSec );

		DWORD src_length = mMP3SourceBufferSizeParSec;

		// ���ۂɓǂݍ��񂾃T�C�Y���A�ǂނׂ��T�C�Y���������������ꍇ
		if ( read_size < mMP3SourceBufferSizeParSec )
		{
			// �Ƃ肠�����A�ǂݍ��߂��T�C�Y���R���o�[�g����
			src_length = read_size;
		}

		// ACM �w�b�_�ɕϊ��o�b�t�@�ݒ�
		ACMSTREAMHEADER ash;
		ZeroMemory( &ash, sizeof( ACMSTREAMHEADER ) );
		ash.cbStruct    = sizeof( ACMSTREAMHEADER );
		ash.pbSrc       = mMP3SourceBufferParSec;
		ash.cbSrcLength = src_length;
		ash.pbDst       = (LPBYTE)buffer_;
		ash.cbDstLength = buffer_size_;

		// �R���o�[�g���s
		HACMSTREAM* acm = (HACMSTREAM*)mACMStreamHandle;
		acmStreamPrepareHeader( *acm, &ash, 0 );

		DWORD acm_flag = ( mResetFlag == true ) ? ACM_STREAMCONVERTF_START : ACM_STREAMCONVERTF_BLOCKALIGN;
		acmStreamConvert( *acm, &ash, acm_flag );
		
		acmStreamUnprepareHeader( *acm, &ash, 0 );

		// �R���o�[�g�������ʁA���ۂɎg�����̈��Ԃ�
		*read_size_ = ash.cbSrcLengthUsed;
		*write_size_ = ash.cbDstLengthUsed;

		mResetFlag = false;

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� �t�@�C���|�C���^�ړ�
	//---------------------------------------------------------------------
	LNRESULT MP3::seek( u32 offset_ )
	{
		mInFile->seek( mDataOffset + offset_, SEEK_SET );
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� �ǂ̃t�H�[�}�b�g�� mp3 �����ׂāA�f�[�^�܂ł̃I�t�Z�b�g��^�O�t�B�[���h�̃T�C�Y�������o�Ɋi�[
	//---------------------------------------------------------------------
	LNRESULT MP3::_checkId3v()
	{
		// �Ƃ肠�����ŏ��ɁA�t�@�C���T�C�Y�� mp3 �f�[�^�S�̂̃T�C�Y�Ƃ���
        mSourceDataSize = mInFile->getSize();

		// �Ƃ肠���� ID3v2 �Ƃ��ăw�b�_������ǂݍ���
		ID3v2Header header;
		mInFile->read( &header, sizeof( ID3v2Header ) );

		// Id3v2 �`���̏ꍇ
		if ( header.ID[ 0 ] == 'I' && header.ID[ 1 ] == 'D' && header.ID[ 2 ] == '3' )
		{
			// �^�O�T�C�Y�擾
			mID3vTagFieldSize  = ( ( header.Size[0] << 21 ) | ( header.Size[1] << 14 ) | ( header.Size[2] << 7 ) | ( header.Size[3] ) ) + 10; 
			
			// �����f�[�^������ʒu�́A�^�O�̏ꏊ�̎�����Ƃ���
			mDataOffset = mID3vTagFieldSize;

			// �����f�[�^�{�̂̃T�C�Y�́A�t�@�C���S�̂̃T�C�Y����^�O�̕�������������
			mSourceDataSize -= mID3vTagFieldSize;
		}
		// Id3v2 �`���ȊO ( Id3v1 ) �̏ꍇ
		else
		{
			// �^�O�Ȃ��@�f�[�^������ꏊ�̓t�@�C���̐擪����
			mID3vTagFieldSize = 0;
			mDataOffset = 0;

			// �t�@�C���I�[���� 128 �o�C�g�߂����Ƃ���𒲂ׂ�
			u8 data[ 3 ];
			mInFile->seek( -128, SEEK_END );
			mInFile->read( data, 3, 3 );

            //printf( "%c %c %c %c\n", data[ 0 ], data[ 1 ], data[ 2 ], data[ 3 ] );

			// 'TAG' ����������
			if ( data[ 0 ] == 'T' && data[ 1 ] == 'A' && data[ 2 ] == 'G' )
			{
				// mp3 �f�[�^�����̃T�C�Y�́A�S�̂���^�O�̕�������������
				mSourceDataSize -= 128;
			}
		}
		// �t�@�C���|�C���^��擪�ɖ߂��Ă���
		mInFile->seek( 0 );
        
        return LN_OK;
	}


	//---------------------------------------------------------------------
	// �� mp3 �� PCM �t�H�[�}�b�g�𒲂ׂă����o�Ɋi�[����
	//---------------------------------------------------------------------
	LNRESULT MP3::_getPCMFormat()
	{
		BYTE  index;
		BYTE  version;
		BYTE  channel;
		BYTE  padding;
		WORD  wBlockSize;
		DWORD dwBitRate;
		DWORD dwSampleRate;
		DWORD dwBitTableLayer3[][ 16 ] = {
			{ 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0 },
			{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0 }
		};
		DWORD dwSampleTable[][ 3 ] = {
			{ 44100, 48000, 32000 },
			{ 22050, 24000, 16000 }
		};

		// �t���[���w�b�_������ǂݍ���
		u8 data[ 4 ];
		mInFile->seek( mID3vTagFieldSize, SEEK_SET );
		mInFile->read( data, 4 );

		// data ���t���[���w�b�_���w���Ă��邩���ׂ�
		if ( data[ 0 ] != 0xff || data[ 1 ] >> 5 != 0x07 )
		{
            // �擪�ɂȂ���΃K���K���i�߂Ȃ���T��
            int rs;
            int ends = mInFile->getSize();
            while ( true )
            {
                rs = mInFile->read( data, 4 );
                if ( rs != 4 )
                {
                    LN_SETERR_R_S( LN_ERR_ASSERT, "< Audio::MP3::_getPCMFormat >\nmp3 �t�@�C���̃t���[���w�b�_��������܂���ł����B( 1 )" );
                }
                if ( data[ 0 ] == 0xff && data[ 1 ] >> 5 == 0x07 )
                {
                    break;
                }
                mInFile->seek( -3, SEEK_CUR );
            }
		}
		
		// MP3 �̃o�[�W�����́H
		switch ( data[ 1 ] >> 3 & 0x03 )
		{
			case 3:
				version = 1;
				break;

			case 2:
				version = 2;
				break;

			default:
                LN_SETERR_R_S( LN_ERR_ASSERT, "< Audio::MP3::_getPCMFormat >\nmp3 �t�@�C���̃t���[���w�b�_��������܂���ł����B( 2 )" );
				break;
		}

		// ���C���[ 3 �H
		if ( ( data[1] >> 1 & 0x03 ) != 1 )
		{
            LN_SETERR_R_S( LN_ERR_ASSERT, "< Audio::MP3::_getPCMFormat >\n ( ( data[1] >> 1 & 0x03 ) != 1 )" );
		}

		// �e�[�u���Œ�`�����r�b�g���[�g�̂����A���Ă͂܂���̂�I��
		index     = data[ 2 ] >> 4;
		dwBitRate = dwBitTableLayer3[ version - 1 ][ index ];

		
		// �����悤�ɁA�T���v�����O���[�g��I��
		index        = data[ 2 ] >> 2 & 0x03;
		dwSampleRate = dwSampleTable[ version - 1 ][ index ];

		// �p�f�B���O�̎擾
		padding = data[ 2 ] >> 1 & 0x01;

		// �`�����l�����̎擾
		channel = ( ( data[ 3 ] >> 6 ) == 3 ) ? 1 : 2;

		wBlockSize = (WORD)((1152 * dwBitRate * 1000 / dwSampleRate) / 8) + padding;
		
		// MPEGLAYER3WAVEFORMAT �\���̂ɂ��낢��i�[����
		MPEGLAYER3WAVEFORMAT* format = mMP3WaveFormat;//(MPEGLAYER3WAVEFORMAT*)&mMP3WaveFormat;

		format->wfx.wFormatTag      = WAVE_FORMAT_MPEGLAYER3;
		format->wfx.nChannels       = channel;
		format->wfx.nSamplesPerSec  = dwSampleRate;
		format->wfx.nAvgBytesPerSec = (dwBitRate * 1000) / 8;
		format->wfx.nBlockAlign     = 1;
		format->wfx.wBitsPerSample  = 0;
		format->wfx.cbSize          = MPEGLAYER3_WFX_EXTRA_BYTES;

		format->wID             = MPEGLAYER3_ID_MPEG;
		format->fdwFlags        = padding ? MPEGLAYER3_FLAG_PADDING_ON : MPEGLAYER3_FLAG_PADDING_OFF;
		format->nBlockSize      = wBlockSize;
		format->nFramesPerBlock = 1;
		format->nCodecDelay     = 0x571;

        return LN_OK;
	}

#endif

//=============================================================================
// �� Ogg �N���X
//=============================================================================

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
	//---------------------------------------------------------------------
	Ogg::Ogg( ResourceManager* manager_ )
        : AudioSourceBase           ( manager_ )
        , mInFile                   ( NULL )
        , mSourceDataSize			( 0 )
		, mOnmemoryPCMBuffer		( NULL )
		, mOnmemoryPCMBufferSize	( 0 )
		, mTotalTime				( 0 )
        , mTotalSamples             ( 0 )
        , mLoopStart                ( 0 )
        , mLoopLength               ( 0 )
	{
        memset( &mWaveFormat, 0, sizeof( mWaveFormat ) );
    }

	//---------------------------------------------------------------------
	// �� �f�X�g���N�^
	//---------------------------------------------------------------------
	Ogg::~Ogg()
	{
        SAFE_DELETE_ARRAY( mOnmemoryPCMBuffer );
		ov_clear( &mOggVorbisFile );
        LN_SAFE_RELEASE( mInFile );
	}

	//---------------------------------------------------------------------
	// �� �I�[�f�B�I�t�@�C���Ƃ��Ĉ����X�g���[����ݒ肷��
	//---------------------------------------------------------------------
	LNRESULT Ogg::setStream( File::IInStream* stream_ )
	{
	    LN_THROW_InvalidCall( ( !mInFile ) );
        //LN_SETERR_R_S_( ( !mInFile ), LN_ERR_ASSERT, "< Audio::Ogg::setStream >\n�����̃X�g���[����ݒ肵�悤�Ƃ��܂����B" );

		mInFile = stream_;
        mInFile->addRef();

		mInFile->seek( 0 );

		// �R�[���o�b�N�֐��̔z����쐬
		ov_callbacks callbacks = {
			readOggCallback,
			seekOggCallback,
			closeOggCallback,
			tellOggCallback };

		// �R�[���o�b�N���g���� Ogg �t�@�C���I�[�v��
		int err = ov_open_callbacks( mInFile, &mOggVorbisFile , 0, 0, callbacks );

		// �G���[�����������ꍇ
		if ( err != 0 )
		{
			const char* es = "";
			switch( err )
			{
				case OV_EREAD:       
					es = "OV_EREAD"; break;
				case OV_ENOTVORBIS:  
					es = "OV_ENOTVORBIS"; break;
				case OV_EVERSION:    
					es = "OV_EVERSION"; break;
				case OV_EBADHEADER:  
					es = "OV_EBADHEADER"; break;
				case OV_EFAULT:      
					es = "OV_EFAULT"; break;
			}
		    LN_THROW_FileFormat( 0, "ov_open_callbacks %s\n", es );
            //LN_SETERR_R_S( LN_ERR_ASSERT, "< Audio::Ogg::setStream >\n );
		}

		// Ogg �t�@�C���̏��擾
		vorbis_info* ogg_info = ov_info( &mOggVorbisFile, -1 );

		int bits_per_sample = WORD_BITS;	// �Ƃ肠���� 16bit �Œ�
		mWaveFormat.FormatTag		= 1;	// PCM
		mWaveFormat.Channels		= ogg_info->channels;
		mWaveFormat.SamplesPerSec	= ogg_info->rate;
		mWaveFormat.AvgBytesPerSec	= ogg_info->rate * bits_per_sample / 8 * ogg_info->channels;
		mWaveFormat.BlockAlign		= bits_per_sample / 8 * ogg_info->channels;
		mWaveFormat.BitsPerSample	= bits_per_sample;//readU16( file_ );
        mWaveFormat.EXSize          = 0;

		ogg_int64_t total = ov_pcm_total( &mOggVorbisFile, -1 );
	    LN_THROW_FileFormat( ( total != OV_EINVAL ), "ov_pcm_total %d\n", total );
        //LN_SETERR_R_S_( , LN_ERR_ASSERT, "< Audio::Ogg::setStream >\no" );
		
		// �I���������ɓW�J���鎞�ɕK�v�� PCM �T�C�Y
		mOnmemoryPCMBufferSize = static_cast< u32 >( total ) * WORD_SIZE * mWaveFormat.Channels;		// 2 �� 16bit �Ȃ̂�

		//printf( "(�v�`�F�b�N�I�I)mOnmemoryPCMBufferSize: %d\n", mOnmemoryPCMBufferSize );

		mSourceDataSize = mOnmemoryPCMBufferSize;

		// �O�̂��ߐ擪�ɃV�[�N
		ov_time_seek( &mOggVorbisFile, 0.0 );

        // �Đ�����
		double t = static_cast< double >( mOnmemoryPCMBufferSize ) / ( static_cast< double >( mWaveFormat.AvgBytesPerSec ) * 0.001 );
		mTotalTime = static_cast< u32 >( t );

        // �S�̂̍Đ��T���v����
        mTotalSamples = mOnmemoryPCMBufferSize / ( mWaveFormat.BitsPerSample / 8 ) / mWaveFormat.Channels;


        // �t�@�C���ɖ��ߍ��܂�Ă��� "LOOPSTART" "LOOPLENGTH" �R�����g��T��
        vorbis_comment* ogg_comment = ov_comment( &mOggVorbisFile, -1 );
        char* c;
        char buf[ 20 ];
        for ( int i = 0; i < ogg_comment->comments; ++i )
        {
            if ( ogg_comment->comment_lengths[ i ] >= 9 )
            {
                c = ogg_comment->user_comments[ i ];

                if ( memcmp( c, "LOOPSTART", 9 ) == 0 )
                {
                    memset( buf, 0, sizeof( buf ) );
                    memcpy( buf, ( c + 10 ), ( ogg_comment->comment_lengths[ i ] - 10 ) );
                    mLoopStart = static_cast< u32 >( atoi( buf ) );
                }
                else if ( memcmp( c, "LOOPLENGTH", 10 ) == 0 )
                {
                    memset( buf, 0, sizeof( buf ) );
                    memcpy( buf, ( c + 11 ), ( ogg_comment->comment_lengths[ i ] - 11 ) );
                    mLoopLength = static_cast< u32 >( atoi( buf ) );
                }
            }
        }

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� �S�Ẵf�[�^�� mOnmemoryPCMBuffer �֓ǂݍ���
	//---------------------------------------------------------------------
	LNRESULT Ogg::fillBufferAndReleaseStream()
	{
		if ( mOnmemoryPCMBuffer == NULL && mOnmemoryPCMBufferSize > 0 )
		{
			mOnmemoryPCMBuffer = LN_NEW u8[ mOnmemoryPCMBufferSize ];

			long read_size;
			long point = 0;
			u32 total = 0;
			char* temp_buffer[ 4096 ];
			int bitstream;

			// �t�@�C���̐擪�ɃV�[�N
			ov_time_seek( &mOggVorbisFile, 0.0 );

			// �S���ǂݍ���
			while ( 1 )
			{
				// �s�蒷�`4096 �o�C�g���ǂ�ł���
				read_size = ov_read( &mOggVorbisFile, (char*)temp_buffer, 4096, 0, WORD_SIZE, 1, &bitstream );

				// �o�b�t�@�ɃR�s�[
				memcpy( &mOnmemoryPCMBuffer[ point ], temp_buffer, read_size );

				total += read_size;

				// �Ō�܂œǂ񂾏ꍇ�͏I��
				if ( read_size == EOF || read_size == 0 ) { 
					break; }

				point += read_size;
			}

            LN_SAFE_RELEASE( mInFile );
		}
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� �f�[�^�� buffer_ �֓ǂݍ���
	//---------------------------------------------------------------------
	LNRESULT Ogg::read( void* buffer_, u32 buffer_size_, u32* read_size_, u32* write_size_ )
	{
		// 0 �N���A
		memset( buffer_, 0, buffer_size_ );
		*read_size_ = 0;

		long size = 0;
		int request_size = 4096;
		int bitstream = 0;
		u32 com_size = 0;
		char* buffer = (char*)buffer_;

		while ( 1 )
		{
			size = ov_read( &mOggVorbisFile, (char*)( buffer + com_size ), request_size, 0, WORD_SIZE, 1, &bitstream );
		
			// �t�@�C���G���h
			if ( size == 0 )
			{
				break;
			}

			com_size += size;

			// �o�b�t�@��S�����߂��ꍇ
			if ( com_size >= buffer_size_ )
			{
				break;
			}

			// �o�b�t�@�̎c�肪 4096 �����̏ꍇ
			if ( buffer_size_ - com_size < 4096 )
			{
				// ���ɓǂރo�C�g���́A�o�b�t�@�̎c��̈敪����
				request_size = buffer_size_ - com_size;
			}
		}

		*read_size_ = com_size;
		*write_size_ = com_size;

        return LN_OK;
	}

    

	//---------------------------------------------------------------------
	// �� �t�@�C���|�C���^�ړ�
	//---------------------------------------------------------------------
	LNRESULT Ogg::seek( u32 offset_ )
	{
		ov_pcm_seek( &mOggVorbisFile, ( offset_ / ( WORD_SIZE * mWaveFormat.Channels ) ) );
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� read �R�[���o�b�N
	//---------------------------------------------------------------------
	size_t Ogg::readOggCallback( void* buffer_, size_t element_size_, size_t count_, void* stream_ )
	{
        File::IInStream* file = (File::IInStream*)stream_;
		size_t read_size = file->read( buffer_, element_size_ * count_, element_size_ * count_ );
		return read_size;
	}

	//---------------------------------------------------------------------
	// �� seek �R�[���o�b�N
	//---------------------------------------------------------------------
	int Ogg::seekOggCallback( void* stream_, ogg_int64_t offset_, int whence_ )
	{
		if ( stream_ == NULL ) { 
	return -1; } // �ُ�̎��� 0 �ȊO�̒l��Ԃ�
		File::IInStream* file = (File::IInStream*)stream_;
		file->seek( static_cast< int >( offset_ ), whence_ );
		return 0;
	}

	//---------------------------------------------------------------------
	// �� close �R�[���o�b�N
	//---------------------------------------------------------------------
	int Ogg::closeOggCallback( void* stream_ )
	{
		return 0;	// InFile �̃f�X�g���N�^�ŕ���̂ŁA�����ł͉������Ȃ��B
	}

	//---------------------------------------------------------------------
	// �� tell �R�[���o�b�N
	//---------------------------------------------------------------------
	long Ogg::tellOggCallback( void* stream_ )
	{
		File::IInStream* file = (File::IInStream*)stream_;
		return file->getPosition();
	}

#if defined(LNOTE_MSVC)
//=============================================================================
// �� Midi �N���X
//=============================================================================

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    Midi::Midi( ResourceManager* manager_ )
        : AudioSourceBase   ( manager_ )
        , mInFile           ( NULL )
        , mSourceData       ( NULL )
        , mSourceDataSize   ( 0 )
        , mCC111Time        ( 0 )
        , mBaseTime         ( 0 )
        , mVolumeNormalize  ( false )
    {
    }

	//---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
	Midi::~Midi()
    {
        SAFE_DELETE_ARRAY( mSourceData );
        LN_SAFE_RELEASE( mInFile );
    }

    //---------------------------------------------------------------------
	// �� �I�[�f�B�I�t�@�C���Ƃ��Ĉ����X�g���[����ݒ肷��
    //---------------------------------------------------------------------
	LNRESULT Midi::setStream( File::IInStream* stream_ )
    {
        LN_SETERR_R_S_( ( !mInFile ), LN_ERR_ASSERT, "< Audio::Midi::setStream >\n�����̃X�g���[����ݒ肵�悤�Ƃ��܂����B" );

		mInFile = stream_;
        mInFile->addRef();

        mVolumeEntryList.clear();

        _searchData();

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� �����Ƀo�b�t�@���m�ۂ��AMidi �f�[�^��S�ēǂݍ���
    //---------------------------------------------------------------------
	LNRESULT Midi::fillBufferAndReleaseStream()
    {
        if ( !mSourceData )
        {
            mSourceDataSize = mInFile->getSize();
            mSourceData = LN_NEW char[ mSourceDataSize ];

            mInFile->seek( 0 );
            mInFile->read( mSourceData, mSourceDataSize );

            
            if ( mVolumeNormalize )
			{
				std::list< VolumeEntry >::iterator it;
                std::list< VolumeEntry >::iterator end = mVolumeEntryList.end();

				// �{�����[���̍ő�l��T��
				u32 max_volume = 0;
				for ( it = mVolumeEntryList.begin(); it != end; ++it )
				{
					if ( it->mVolume > max_volume )
					{
						max_volume = it->mVolume;
					}
				}

                // �ő�l�� 127 �ɂ��邽�߂̒l���v�Z
				int sub = 127 - max_volume;

                // �S�Ẳ��ʒl�ɉ��Z
				for ( it = mVolumeEntryList.begin(); it != end; ++it )
				{
					mSourceData[ it->mPosition ] += sub;
				}

				mVolumeEntryList.clear();
			}

            LN_SAFE_RELEASE( mInFile );
        }

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� midi �f�[�^�̒��̕K�v�ȃf�[�^���`�F�b�N����
    //---------------------------------------------------------------------
	void Midi::_searchData()
    {
        // �t�@�C���|�C���^��擪�ɖ߂��Ă���
		mInFile->seek( 0 );

		// Midi �t�@�C���̃w�b�_�ǂݍ���
		MidiHeader header;
		mInFile->read( &( header.mChunktype ), 4 );
        header.mLength      = File::readU32Big( mInFile );
		header.mFormat      = File::readU16Big( mInFile );
		header.mNumtrack    = File::readU16Big( mInFile );
		header.mTimebase    = File::readU16Big( mInFile );

		// �x�[�X�^�C���i�[
		mBaseTime = header.mTimebase;

        mCC111Time = 0;
        u32 cc111time = 0;

		// �g���b�N�̐��������[�v���āAcc111 �ƃ{�����[���`�F���W��T��
		for ( int i = 0; i < header.mNumtrack; ++i )
		{
            _searchTrack( &cc111time );

            if ( mCC111Time == 0 )
            {
                mCC111Time = cc111time;
            }

			/*if ( !_searchTrackCC111( &mCC111Time ) )
			{
				return;
			}*/
		}
    }

	//---------------------------------------------------------------------
	// �� �f���^�^�C���̓ǂݍ���
    //---------------------------------------------------------------------
	u32 Midi::_readDelta()
    {
        u8 t;
		u32 dtime = 0;
		for ( int i = 0; i < sizeof( u32 ); ++i )
		{
			mInFile->read( &t, sizeof( u8 ) );
			dtime = ( dtime << 7 ) | ( t & 0x7f );

			// MSB�������Ă��Ȃ��Ȃ�΁A���̃o�C�g�̓f���^�^�C���ł͂Ȃ��̂Ŕ�����
			if ( !( t & 0x80 ) ) { 
				break; }
		}
		return dtime;
    }

	//---------------------------------------------------------------------
	// �� �g���b�N�̑{��
    //---------------------------------------------------------------------
	bool Midi::_searchTrack( u32* cc111_time_ )
    {
        // �g���b�N�`�����N�̃`�F�b�N
		u8 chunk[ 4 ];
		mInFile->read( chunk, 4 );
		if ( memcmp( chunk, "MTrk", 4 ) != 0 ) { 
			return false; }

		// �g���b�N�̒����ǂݍ���
		u32   track_length;
		track_length = File::readU32Big( mInFile );

		u8 prev_state = 0; // �ЂƂO�̃C�x���g�̃X�e�[�^�X�o�C�g���L������ϐ�
		u8 state;
		u8 data1;
		u8 data2;
		u32 track_time = 0;

		while ( 1 )
		{
			// �f���^�^�C����ǂݍ���
			track_time += _readDelta();

			// �X�e�[�^�X�o�C�g��ǂݍ���
			mInFile->read( &state, sizeof( u8 ) );
			
			// �����j���O�X�e�[�^�X�̏ꍇ
			if ( !( state & 0x80 ) )
			{
				// ��O�̃C�x���g�̃X�e�[�^�X�o�C�g����
				state = prev_state;
				// �t�@�C���|�C���^����߂�
				mInFile->seek( -1, SEEK_CUR );
			}
			
			// �X�e�[�^�X�o�C�g����ɂǂ̃C�x���g������
			switch ( state & 0xf0 )
			{
				// �f�[�^�o�C�g�� 2 �o�C�g�̃C�x���g
				case 0x80:	// �m�[�g�I�t
				case 0x90:	// �m�[�g�I��
				case 0xA0:	// �|���t�H�j�b�N�E�L�[�E�v���b�V��
				case 0xE0:	// �s�b�`�x���h
					mInFile->seek( 2, SEEK_CUR );
					break;

				case 0xB0:	// �R���g���[���`�F���W
					mInFile->read( &data1, sizeof( u8 ) );
					mInFile->read( &data2, sizeof( u8 ) );

					// cc111
					if ( data1 == 0x6F )
					{
						*cc111_time_ = track_time;
					}
					// �{�����[���`�F���W
					else if ( data1 == 0x07 )
					{
						// �f�[�^�̈ʒu�ƃ{�����[�����L���[�ɓ���ĕۑ�
						VolumeEntry entry;
						entry.mPosition = mInFile->getPosition() - 1;
						entry.mVolume = data2;
						mVolumeEntryList.push_back( entry );
						//printf("�{�����[���`�F���W %d\n", data2);
					}
					break;

				// �f�[�^�o�C�g�� 1 �o�C�g�̃C�x���g
				case 0xC0:	// �v���O�����`�F���W
				case 0xD0:	// �`�����l���v���b�V��
					mInFile->seek( 1, SEEK_CUR );
					break;
				
				// �f�[�^�o�C�g���ϒ��̃C�x���g
				case 0xF0:
					// SysEx�C�x���g
					if ( state == 0xF0 )
					{
						int data_length = 0;
						// �f�[�^���ǂݍ���
						mInFile->read( &data_length, sizeof( u8 ) );
						mInFile->seek( data_length, SEEK_CUR );
					}
					// ���^�C�x���g
					else if ( state == 0xFF )
					{
						u8 type;

						// type�̎擾
						mInFile->read( &type, sizeof( u8 ) );

						u32 data_length = -1;

						// type �ʂɃf�[�^�o�C�g�̒������擾
						switch ( type )
						{
							case 0x00: 
								data_length = 2; break;
							case 0x01:
							case 0x02:
							case 0x03:
							case 0x04:
							case 0x05:
							case 0x06:
							case 0x07: 
								break;
							case 0x20: 
								data_length = 1; break; 
							case 0x21: 
								data_length = 1; break; 
							case 0x2F: 
								data_length = 0; break; // �G���h�I�u�g���b�N
							case 0x51: 
								data_length = 3; break; // �Z�b�g�e���|
							case 0x54: 
								data_length = 5; break;
							case 0x58: 
								data_length = 4; break;
							case 0x59: 
								data_length = 2; break;
							case 0x7F: 
								break;

							default:
								//LASSERT_S_( 1, LException::Format(), "< LAudioSourceMIDI::_searchCC111FromTrack >\n���݂��Ȃ����^�C�x���g�ł��B\n%x", type );
								return false;

						}
						
						u32 temp = data_length;

						// �f�[�^�����Œ�̏ꍇ
						if ( data_length != -1 )
						{
							data_length = _readDelta();
							if ( data_length != temp )
							{
								//LASSERT_S_( 1, LException::Format(), "< LAudioSourceMIDI::_searchCC111FromTrack >\n�Œ蒷���^�C�x���g�̃f�[�^�����s���ł��B\n%d != %d\n", data_length, temp );
								return false;
							}
						}
						else
						{
							// �C�ӂ̃f�[�^�����擾
							data_length = _readDelta(); 
						}

						mInFile->seek( data_length, SEEK_CUR );

						// �g���b�N�̏I�[�����������ꍇ�͏I��
						if ( type == 0x2F )
						{
							return true;
						}
					}

					break;

				default:
					//LASSERT_S_( 1, LException::Format(), "< LAudioSourceMIDI::_searchCC111FromTrack >\n�X�e�[�^�X�o�C�g���s���ł��B%u\n", ( state & 0xf0 ) );
					return false;

			}
			// ���̃C�x���g���O�̃C�x���g�̃X�e�[�^�X�o�C�g���m�F�ł���悤�ɕۑ�����
			prev_state = state;
		}
		return true;
    }

#endif // LNOTE_MSVC

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Audio
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================