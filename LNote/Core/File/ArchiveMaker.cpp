//=============================================================================
//�y ArchiveMaker �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <string>
#include <Dependencies/camellia/camellia.h>
#include "FileUtil.h"
#include "ArchiveMaker.h"

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
//=============================================================================

    #pragma pack(1)
    struct LNArchiveHeader
    {
        char            ID[ 4 ];        ///< 'l' 'n' 'a' '_'
        char            VerMajor;
        char            VerMinor;
        unsigned char   KeyLength;
        char            Key[ 128 ];
        char            Padding[ 9 ];   ///< �T�C�Y�� 16 �̔{���ɂȂ�悤�ɂ��邽�߂̃p�f�B���O
    };
    #pragma pack()

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
	//---------------------------------------------------------------------
	ArchiveMaker::ArchiveMaker()
		: mStream	( NULL )
		, mFileNum	( 0 )
	{
	}

	//---------------------------------------------------------------------
	// �� �f�X�g���N�^
	//---------------------------------------------------------------------
	ArchiveMaker::~ArchiveMaker()
	{
		close();
	}

	//---------------------------------------------------------------------
	// �� �V�����A�[�J�C�u�t�@�C�����J��
	//---------------------------------------------------------------------
	bool ArchiveMaker::open( const lnChar* filename_, const char* key_ )
	{
		mEncryptionKey = ( key_ ) ? key_ : "";
		mFileNum = 0;
        memset( mKeyTable, 0, sizeof( mKeyTable ) );

        // �g���L�[�̍쐬
        if ( key_ )
        {
            lnByte key_buf[ KEY_SIZE ] = { 0 };
            memcpy( key_buf, key_, strlen( key_ ) );
            memset( mKeyTable, 0, sizeof( mKeyTable ) );
            Camellia_Ekeygen( KEY_SIZE, key_buf, mKeyTable );
        }

        mStream = _tfopen( filename_, ( "wb" ) );
		if ( mStream )
		{
            LNArchiveHeader header;
            header.ID[0] = 'l';
            header.ID[1] = 'n';
            header.ID[2] = 'a';
            header.ID[3] = '_';
            header.VerMajor = VER_MAJOR;
            header.VerMinor = VER_MINOR;
            header.KeyLength = mEncryptionKey.size();
            _tcsncpy( header.Key, mEncryptionKey.c_str(), 128 );
            memset( header.Padding, 0, sizeof( header.Padding ) );

            _writePadding16( &header, sizeof( header ) );
            //fwrite( &header, sizeof( header ), 1, mStream );

            return true;
        }




        /*
        mEncoder = ( encoder_ ) ? encoder_ : _encoder;

		mStream = fopen( filename_, "wb" );
		if ( mStream )
		{
			// �}�W�b�N�i���o�[
			char magic[ 5 ] = "lna_";
			_write( magic, 4 );

			// �L�[�̃T�C�Y
			u32 size = mEncryptionKey.size();
			_write( &size, 4 );

			// �L�[
            Base::RefString tk = mEncryptionKey.c_str();
			_write( tk.c_str(), tk.size() );

            return true;
		}
        */
        return false;
	}

	//---------------------------------------------------------------------
	// �� �A�[�J�C�u�t�@�C�������
	//---------------------------------------------------------------------
	void ArchiveMaker::close()
	{
		if ( mStream )
		{
			// ��ԍŌ�Ƀt�@�C���̐�����������
            _writeU32Padding16( mFileNum, 0 );

            /*
            fwrite( &header, sizeof( header ), 1, mStream );
			_write( &mFileNum, sizeof( u32 ) );
            */

			mEncryptionKey = "";
			mFileNum = 0;
			fclose( mStream );
			mStream = NULL;
		}
	}

	//---------------------------------------------------------------------
	// �� �t�@�C����ǉ�����
	//---------------------------------------------------------------------
	bool ArchiveMaker::addFile( const lnChar* filename_, const lnChar* access_name_ )
	{
		FILE* stream = _tfopen( filename_, ( "rb" ) );
		if ( stream )
		{
            // �A�N�Z�X�p�̖��O���Ȃ���΁A�t�@�C���������Ɏg��
            if ( !access_name_ )
            {
                access_name_ = filename_;
            }

            // �A�N�Z�X�p�t�@�C�����̃X���b�V�����o�b�N�X���b�V����
			ln_std_tstring filename = access_name_;
			for ( size_t i = 0; i < filename.size(); ++i )
			{
				if ( filename[ i ] == ( '/' ) )
				{
					filename[ i ] = ( '\\' );
				}
			}

            //-------------------------------------------------
            // �t�@�C�����̒����ƃt�@�C���̃T�C�Y����������
			u32 name_size = filename.size();
			
		    fseek( stream, 0, SEEK_END );
		    u32 file_size = ftell( stream );

            _writeU32Padding16( name_size, file_size );
            

            //-------------------------------------------------
            // �t�@�C�����ƃt�@�C�����e����������

            // �t�@�C�������������� (�I�[NULL�̓i�V)
            _writePadding16( filename.c_str(), name_size );

            // �T�C�Y���̃��������m�ۂ��Ēǉ�������e��ǂݍ���
		    char* buffer = LN_NEW char[ file_size ];
		    fseek( stream, 0, SEEK_SET );
		    fread( buffer, sizeof( char ), file_size, stream );

		    // ���e����������
		    _writePadding16( buffer, file_size );

		    SAFE_DELETE_ARRAY( buffer );

            fclose( stream );

            ++mFileNum;

			

            /*
            if ( !access_name_ )
            {
                access_name_ = filename_;
            }
			// �A�N�Z�X�p�t�@�C�����̃X���b�V�����o�b�N�X���b�V����
			std::string filename = access_name_;
           
			for ( size_t i = 0; i < filename.size(); ++i )
			{
				if ( filename[ i ] == '/' )
				{
					filename[ i ] = '\\';
				}
			}

			// �t�@�C�����̒�������������
			u32 size = filename.size();
			u32 ts = size;
			_write( &size, sizeof( u32 ) );

			// �t�@�C�������������� (�I�[NULL�̓i�V)
			_write( filename.c_str(), ts );

			_addFile( stream );

			fclose( stream );
            */

            return true;
		}
        return false;
	}

    //---------------------------------------------------------------------
	// �� �p�f�B���O���l�����āA�J���Ă���A�[�J�C�u�t�@�C���Ƀf�[�^����������
    //---------------------------------------------------------------------
	void ArchiveMaker::_writePadding16( const void* data_, u32 size_ )
    {
        u32 ps = ( 16 - ( size_ % 16 ) ) % 16;
        char padding[ 16 ];
        memset( padding, ps, sizeof( padding ) );

        // �Í�������ꍇ
        if ( mEncryptionKey.size() > 0 )
        {
            int i = 0;
            u32 total = 0;
            const lnByte* data = static_cast< const lnByte* >( data_ );
            lnByte src_data[ 16 ];
            lnByte write_data[ 16 ];
            while ( true )
            {
                if ( total + 16 > size_ )
                {
                    memcpy( src_data, &data[ i * 16 ], 16 - ps );
                    memcpy( &src_data[ 16 - ps ], padding, ps );
                }
                else
                {
                    memcpy( src_data, &data[ i * 16 ], 16 );
                }
                Camellia_Encrypt( KEY_SIZE, src_data, mKeyTable, write_data );
                fwrite( write_data, 1, 16, mStream );
                ++i;
                total += 16;
                if ( total >= size_ )
                {
                    break;
                }
            }
        }
        else
        {
            fwrite( data_,   1, size_, mStream );
            fwrite( padding, 1, ps, mStream );
        }
    }

    //---------------------------------------------------------------------
	// �� �p�f�B���O���l�����āA��������������
    //---------------------------------------------------------------------
	void ArchiveMaker::_writeU32Padding16( u32 v0_, u32 v1_ )
    {
        lnByte b[ 16 ] = { 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8 };
        if ( Base::isLittleEndian() )
        {
            *((u32*)(b + 0)) = v0_;
            *((u32*)(b + 4)) = v1_;
        }
        else
        {
            *( b + 0 ) = static_cast< lnU8 >(   v0_ & 0x000000ff );
            *( b + 1 ) = static_cast< lnU8 >( ( v0_ & 0x0000ff00 ) >> 8 );
            *( b + 2 ) = static_cast< lnU8 >( ( v0_ & 0x00ff0000 ) >> 16 );
            *( b + 3 ) = static_cast< lnU8 >( ( v0_ & 0xff000000 ) >> 24 );

            *( b + 4 ) = static_cast< lnU8 >(   v1_ & 0x000000ff );
            *( b + 5 ) = static_cast< lnU8 >( ( v1_ & 0x0000ff00 ) >> 8 );
            *( b + 6 ) = static_cast< lnU8 >( ( v1_ & 0x00ff0000 ) >> 16 );
            *( b + 7 ) = static_cast< lnU8 >( ( v1_ & 0xff000000 ) >> 24 );
        }

        lnByte buf[ 16 ];
        Camellia_Encrypt( KEY_SIZE, b, mKeyTable, buf );
        fwrite( &buf, sizeof( b ), 1, mStream );
    }

	//---------------------------------------------------------------------
	// �� �J���Ă���t�@�C���Ƀf�[�^����������
	//---------------------------------------------------------------------
	void ArchiveMaker::_write( const void* data_, u32 size_ )
	{
        /*
		if ( mEncoder && mEncryptionKey.size() > 0 )
		{
            EncodeArgs args;
            args.Data       = (char*)data_;
            args.Size       = size_;
            args.Offset     = ftell( mStream );
            //args.BlockLen   = ;
            args.Key        = mEncryptionKey.c_str();
            args.KeyLength  = mEncryptionKey.size();

			mEncoder( args );
		}
		fwrite( data_, sizeof( char ), size_, mStream );
        */
	}

	//---------------------------------------------------------------------
	// �� �ǉ�����t�@�C���̃X�g���[�����󂯂Ƃ��āA���ۂɒǉ�����
	//---------------------------------------------------------------------
	void ArchiveMaker::_addFile( FILE* stream_ )
	{
        

        /*
		// �ǉ�����t�@�C���̃T�C�Y�𒲂ׂď�������
		fseek( stream_, 0, SEEK_END );
		u32 file_size = ftell( stream_ );
		u32 ts = file_size;
		_write( &ts, sizeof( u32 ) );

		// �T�C�Y���̃��������m�ۂ��Ēǉ�������e��ǂݍ���
		char* buffer = NEW char[ file_size ];
		fseek( stream_, 0, SEEK_SET );
		fread( buffer, sizeof( char ), file_size, stream_ );

		// ���e����������
		_write( buffer, file_size );

		SAFE_DELETE_ARRAY( buffer );

		++mFileNum;
        */

	}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace File
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================