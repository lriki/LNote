//=============================================================================
//�y Archive �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <string>
#include <Dependencies/camellia/camellia.h>
#include "../Base/StringUtil.h"
#include "FileUtil.h"
#include "Archive.h"

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
// �� Archive �N���X
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
	Archive::Archive()
		: mStream	        ( NULL )
		, mFileNum	        ( 0 )
		, mIsDummy	        ( false )
        , mTempBuffer       ( NULL )
        , mTempBufferSize   ( NULL )
        , mLock             ( NULL )
	{

	}

	//---------------------------------------------------------------------
	// �� �f�X�g���N�^
	//---------------------------------------------------------------------
	Archive::~Archive()
	{
		if ( mStream )
		{
			fclose( mStream );
			mStream = NULL;

            mEntriesMap.clear();
		}
        SAFE_DELETE_ARRAY( mTempBuffer );
        LN_SAFE_RELEASE( mLock );
	}

	//---------------------------------------------------------------------
	// �� �A�[�J�C�u�t�@�C�����J���ăA�N�Z�X�̏���������
	//---------------------------------------------------------------------
	Archive::ErrorCode Archive::initialize(  const lnChar* directory_, const lnChar* filename_, const char* key_ )
	{
        if ( ( !mStream && mIsDummy ) || ( mStream ) )
        {
            LN_THROW_Initialized( 0 );
        }


		// ���{��t�@�C�����󂯕t����悤�ɂ���
		setlocale( LC_ALL, "" );

        mIsDummy = false;

		// �_�~�[�̏ꍇ�̓t���O�𗧂Ă邾��
		if ( filename_ == NULL )
		{
			mIsDummy = true;
			return ERR_NOT_OPEN;
		}

        // ���ۂ̃t�@�C���������
        ln_std_tstring real_name = directory_;
        real_name += _T( "/" );
        real_name += filename_;

        LN_THROW_FileNotFound( File::isExist( real_name.c_str() ), real_name.c_str() );


        // �g���q���������������R�s�[����
        mArchiveDirectory  = File::getExtName( filename_ );
        mArchiveDirectory += _T( "\\" );
        File::normalizePathName( &mArchiveDirectory );   // �o�b�N�X���b�V����
        
        
        //mDecoder = ( decoder_ ) ? decoder_ : _decoder;
		mKey = ( key_ ) ? key_ : "";

		mFileNum = 0;
		mIsDummy = false;

        // �g���L�[�̍쐬
        if ( key_ )
        {
            lnByte key_buf[ KEY_SIZE ] = { 0 };
            memcpy( key_buf, key_, strlen( key_ ) );
            memset( mKeyTable, 0, sizeof( mKeyTable ) );
            Camellia_Ekeygen( KEY_SIZE, key_buf, mKeyTable );
        }

        // �Ƃ肠�����ꎞ�o�b�t�@���m�ۂ��Ă���
        _checkTempBuffer( 5000 );

		// �A�[�J�C�u�t�@�C�����J��
        mStream = _tfopen( real_name.c_str(), _T( "rb" ) );
		if ( !mStream )
		{
			return ERR_NOT_OPEN;	// �J���Ȃ�����
		}

		// �t�@�C���T�C�Y�擾
		fseek( mStream, 0, SEEK_END );
		long archive_size = ftell( mStream );

        // 16�o�C�g�߂��Ă��炻���ǂނƃt�@�C����
		fseek( mStream, -16, SEEK_END );
		mFileNum = _readU32Padding16();

        // �擪�ɖ߂��ăw�b�_���`�F�b�N
        fseek( mStream, 0, SEEK_SET );
        LNArchiveHeader header;
        _readPadding16( &header, sizeof( header ) );

        //_p(sizeof( header ));

        // �}�W�b�N�i���o�[���`�F�b�N
        if ( memcmp( header.ID, "lna_", 4 ) != 0 )
		{
			fclose( mStream );
			mStream = NULL;
			return ERR_FORMAT;
		}
        // ���ߍ��܂ꂽ�L�[�ƈ�v���邩�`�F�b�N
        LRefTString file_key( header.Key, header.KeyLength );
        if ( mKey != file_key )
		{
            fclose( mStream );
            mStream = NULL;
			return ERR_KEY;
		}

        //_p( (int)header.VerMajor );
        //_p( (int)header.VerMinor );
        //_p( file_key.c_str() );

        // Map �̗̈���t�@�C�������m��
        //mEntriesMap.setCapacity( mFileNum );

        // �t�@�C�������擾���Ă���
		u32 name_len;
		char* name_buf;
        LRefTString tn;
		Entry entry;
		for ( int i = 0; i < mFileNum; ++i )
		{
			// �t�@�C�����̒����ƃt�@�C���T�C�Y��ǂ�
            _readU32Padding16( &name_len, &entry.mSize );

            

			// �t�@�C������ǂݍ��ރo�b�t�@���m�ۂ��ēǂ݂���
			name_buf = LN_NEW char[ name_len + 1 ];
			_readPadding16( name_buf, name_len );
			name_buf[ name_len ] = _T( '\0' );

            // �����ł̃t�@�C���|�C���^���f�[�^�{�̂̈ʒu
			entry.mOffset = ftell( mStream );
			
            tn = mArchiveDirectory;
            tn += name_buf;

            //_p(tn.c_str());
            //_p(entry.mSize);

			mEntriesMap.insert( std::pair< LRefTString, Entry >( tn, entry ) );

			// �t�@�C���|�C���^���f�[�^�T�C�Y���i�߂�
			//fseek( mStream, entry.mSize + ( 16 - ( entry.mSize % 16 ) ) % 16, SEEK_CUR );
            u32 ofs = _padding16( entry.mSize );
            fseek( mStream, entry.mSize + ofs, SEEK_CUR );
            
			
			SAFE_DELETE_ARRAY( name_buf );
		}

        mLock = LN_NEW Thread::Mutex();

		return SUCCESS;
	}


	//---------------------------------------------------------------------
	// �� �t�@�C���X�g���[���ƃf�[�^�ʒu�܂ł̃I�t�Z�b�g�擾
	//---------------------------------------------------------------------
	void Archive::open( const lnChar* filename_, FILE** out_stream_, int* data_offset_, int* data_size_, LNFileOpenMode mode_ )
	{
		*out_stream_ = NULL;
		*data_offset_ = 0;
		*data_size_ = 0;
		if ( mIsDummy )
		{
            FILE* fp =  _tfopen( filename_, (mode_ == LN_OPENMODE_BINARY) ? _T("rb") : _T("r") );
            
			// �t�@�C���������ꍇ
			if ( !fp )
			{
				return;
			}

			// �t�@�C���T�C�Y�𒲂ׂ� data_size_ �ɕԂ�
			fseek( fp, 0, SEEK_END );
			*data_size_ = ftell( fp );
			fseek( fp, 0, SEEK_SET );

            *out_stream_ = fp;
		}
		else
		{
			if ( mFileNum == 0 )
			{
				return;
			}
			else
			{
				//std::string name = filename_;
                Base::RefString name = filename_;
                File::normalizePathName( &name );
                /*
				for ( int i = 0; i < name.size(); ++i )
				{
					if ( name[ i ] == '/' )
					{
						name[ i ] = '\\';
					}
				}
                */
				EntriesMap::iterator it;
				it = mEntriesMap.find( name );
				if ( it != mEntriesMap.end() )
				{
					*out_stream_ = mStream;
					Entry entry = it->second;
					*data_offset_ = entry.mOffset;
					*data_size_ = entry.mSize;
				}
			}
		}
	}


    //---------------------------------------------------------------------
	// �� �A�[�J�C�u���̃t�@�C���ɃA�N�Z�X���邽�߂̃X�g���[���̍쐬
    //---------------------------------------------------------------------
    void Archive::createStream( IInStream** stream_, const lnChar* filename_, LNFileOpenMode mode_ )
    {
        *stream_ = NULL;

        FILE* out_stream;
        int data_offset;
        int data_size;
        open( filename_, &out_stream, &data_offset, &data_size, mode_ );
        if ( !out_stream )
        {
            return;
        }

        *stream_ = LN_NEW ArchiveInStream( this, out_stream, data_offset, data_size, mode_, ( out_stream != mStream ) );
    }

	//---------------------------------------------------------------------
	// �� �A�[�J�C�u�̒��Ƀt�@�C�������邩�ǂ����m�F����
	//---------------------------------------------------------------------
	bool Archive::isExist( const char* filename_ )
	{
		if ( mIsDummy )
		{
			FILE* stream = fopen( filename_, "rb" );
			if ( stream )
			{
				fclose( stream );
				return true;
			}
			return false;
		}
		else
		{
			if ( mFileNum == 0 )
			{
				return false;
			}
			else
			{
				/*std::string name = filename_;
				for ( size_t i = 0; i < name.size(); ++i )
				{
					if ( name[ i ] == '/' )
					{
						name[ i ] = '\\';
					}
				}*/

                LRefString name = filename_;
                File::normalizePathName( &name );

				EntriesMap::iterator it;
				it = mEntriesMap.find( name );
				if ( it == mEntriesMap.end() )
				{
					return false;
				}
			}
			return true;
		}
	}

	//---------------------------------------------------------------------
	// �� �ǂݍ���
	//---------------------------------------------------------------------
	u32 Archive::read( void* buffer_, u32 count_, FILE* stream_, LNFileOpenMode mode_ )
	{
		u32 read_size = 0;
		if ( mIsDummy )
		{
			read_size = fread( buffer_, 1, count_, stream_ );
		}
		else
		{
            mLock->lock();

            int cur_fpos = ftell( mStream );            // ��)�ʒu�� 20 �̏ꍇ
            int over_fpos = ( cur_fpos % 16 );          // 4
            int read_start_pos = cur_fpos - over_fpos;  // 16
            
            // 16 �o�C�g�P�ʂɂȂ�悤�ɖ߂�
            fseek( mStream, read_start_pos, SEEK_SET ); // seek 16

            int over_count = ( count_ % 16 );           // count_ �� 30 �̏ꍇ�A14
            int r_size = count_ + over_fpos;            // count_ �� 30 �̏ꍇ�A34
            r_size += _padding16( r_size );//( 16 - ( r_size % 16 ) ) % 16;    // 48 �o�C�g�ǂ�

            // �K�v�ȗ̈���m��
            _checkTempBuffer( r_size );

            read_size = fread( mTempBuffer, 1, r_size, mStream );

            int tmpbuf_pos = 0;     // mTempBuffer ����ǂރo�C�g�ʒu
            u32 total = 0;          // mTempBuffer ����ǂ񂾃o�C�g���̗݌v (buffer_ �ɓ]�����悤�Ƃ����o�C�g��)
            int re_count = count_;  // ���Ǝc�� buffer_ �ɏ������߂�o�C�g��
            lnByte tmp_buf[ 16 ];
            lnByte* write_pos = static_cast< lnByte* >( buffer_ );

            // �O���ɗ]���ɓǂ񂾂��̂�����ꍇ
            if ( over_fpos > 0 )
            {
                Camellia_Decrypt( KEY_SIZE, &mTempBuffer[ total ], mKeyTable, tmp_buf );
                tmpbuf_pos += 16;

                int data_size = 16 - over_fpos; // �]�����������f�[�^�T�C�Y
                memcpy( write_pos, &tmp_buf[ over_fpos ], std::min( re_count, data_size ) );   // min �� count_ �ȏ㏑�����܂Ȃ�����
                write_pos = write_pos + data_size;
                
                total += data_size;
                re_count -= std::min( re_count, data_size );
            }

            while ( true )
            {
                // ����̓ǂݍ��݂� count_ �ɓ��B����ꍇ�͏I��
                // count_ �� 16 ���x�̏ꍇ�͂������
                if ( total + 16 >= count_ )
                {
                    break;
                }

                Camellia_Decrypt( KEY_SIZE, &mTempBuffer[ tmpbuf_pos ], mKeyTable, write_pos ); 
                tmpbuf_pos += 16;
                write_pos += 16;
                total += 16;
                re_count -= 16;
            }

            // ����ɗ]���ɓǂ񂾂��̂�����ꍇ
            if ( over_count > 0 )
            {
                //_p(tmpbuf_pos);
                //_p(total);
                //_p(count_);
                //_p(over_count);
                Camellia_Decrypt( KEY_SIZE, &mTempBuffer[ tmpbuf_pos ], mKeyTable, tmp_buf );
                memcpy( write_pos, tmp_buf, std::min( re_count, over_count ) );

                //total += 16;
            }
            else
            {
                Camellia_Decrypt( KEY_SIZE, &mTempBuffer[ tmpbuf_pos ], mKeyTable, tmp_buf );
                memcpy( write_pos, tmp_buf, re_count );
            }



            

            // �Ăяo�����ł̕s�������邽�߁A����ɓǂ߂Ă���ꍇ�� count_ ��ǂ񂾃o�C�g���Ƃ���
            if ( read_size > count_ )
            {
                read_size = count_;

                // �t�@�C���|�C���^�𐳂����ʒu�Ɉړ�
                fseek( mStream, cur_fpos + count_, SEEK_SET );
            }



            //-------------------------------------------------
            // �e�L�X�g���[�h�̏ꍇ�͉��s�R�[�h��ϊ�
            if (mode_ == LN_OPENMODE_TEXT)
            {
                int cr_cnt = getFileSize((char*)buffer_, read_size);
                std::string str((char*)buffer_, read_size);
                Base::StringUtil::removeChar(&str, 13);
                read_size -= cr_cnt;
            }
            
            mLock->unlock();
		}
		return read_size;
	}

	//---------------------------------------------------------------------
	// �� �X�g���[�������
	//---------------------------------------------------------------------
	void Archive::close( FILE* stream_ )
	{
		if ( mIsDummy )
		{
			if ( stream_ )
			{
				fclose( stream_ );
			}
		}
	}

    //---------------------------------------------------------------------
	// �� �ꎞ�o�b�t�@�̃T�C�Y�`�F�b�N
    //---------------------------------------------------------------------
    void Archive::_checkTempBuffer( u32 request_size_ )
    {
        if ( request_size_ > mTempBufferSize )
        {
            mTempBufferSize = request_size_;
            SAFE_DELETE_ARRAY( mTempBuffer );
            mTempBuffer = LN_NEW lnByte[ mTempBufferSize ];
        }
    }

    //---------------------------------------------------------------------
	// �� �p�f�B���O���l�����Đ�����ǂݍ���
    //---------------------------------------------------------------------
	u32 Archive::_readU32Padding16()
    {
        lnByte b[ 16 ] = { 0 };

        // ��������ꍇ
        if ( mKey.size() > 0 )
        {
            lnByte buf[ 16 ];
            fread( buf, 1, 16, mStream );
            Camellia_Decrypt( KEY_SIZE, buf, mKeyTable, b );
        }
        // ���̂܂ܓǂݍ��ޏꍇ
        else
        {
            fread( b, 1, 16, mStream );
        }

        if ( Base::isLittleEndian() )
        {
            return *((u32*)b);
        }
        else
        {
            u32 r = *((u32*)b);
            r = (((r) >> 24) +
                (((r) & 0x00ff0000) >> 8) +
                (((r) & 0x0000ff00) << 8) +
                ((r) << 24));
            return r;
        }
    }

    //---------------------------------------------------------------------
	// �� �p�f�B���O���l�����āA������ǂݍ��� 
    //---------------------------------------------------------------------
	void Archive::_readU32Padding16( u32* v0_, u32* v1_ )
    {
        lnByte b[ 16 ] = { 0 };

        // ��������ꍇ
        if ( mKey.size() > 0 )
        {
            lnByte buf[ 16 ];
            fread( buf, 1, 16, mStream );
            Camellia_Decrypt( KEY_SIZE, buf, mKeyTable, b );
        }
        // ���̂܂ܓǂݍ��ޏꍇ
        else
        {
            fread( b, 1, 16, mStream );
        }

        // ��������ꍇ
        if ( 0 )
        {
        }

        if ( Base::isLittleEndian() )
        {
            *v0_ = *((u32*)(b + 0));
            *v1_ = *((u32*)(b + 4));
        }
        else
        {
            u32 r = *((u32*)b);
            *v0_ =  (((r) >> 24) +
                    (((r) & 0x00ff0000) >> 8) +
                    (((r) & 0x0000ff00) << 8) +
                    ((r) << 24));

            r = *((u32*)(b + 4));
            *v1_ =  (((r) >> 24) +
                    (((r) & 0x00ff0000) >> 8) +
                    (((r) & 0x0000ff00) << 8) +
                    ((r) << 24));
        }
    }

    //---------------------------------------------------------------------
	// �� �p�f�B���O���l�����ăf�[�^��ǂݍ���
    //---------------------------------------------------------------------
	int Archive::_readPadding16( void* buffer_, int count_ )
    {
        int read_size;

        // ��������ꍇ
        if ( mKey.size() > 0 )
        {
            int count_p16 = _padding16( count_ );

            // �S�̂̃T�C�Y���v�Z���āA�o�b�t�@�m��
            int all_size = count_ + count_p16;//( 16 - ( count_ % 16 ) ) % 16;
            _checkTempBuffer( all_size );

            // �ꎞ�o�b�t�@�ɓǂ�
            read_size = fread( mTempBuffer, 1, all_size, mStream );

            int over_size = count_p16;//( 16 - ( count_ % 16 ) ) % 16;
            int i = 0;
            int total = 0;
            
            lnByte* buf = static_cast< lnByte* >( buffer_ );
            while ( true )
            {
                if ( total + 16 >= count_ )
                {
                    lnByte read_data[ 16 ];
                    Camellia_Decrypt( KEY_SIZE, &mTempBuffer[ total ], mKeyTable, read_data );
                    memcpy( &buf[ i * 16 ], read_data, 16 - over_size );
                    break;
                }
                else
                {
                    Camellia_Decrypt( KEY_SIZE, &mTempBuffer[ total ], mKeyTable, &buf[ total ] );
                }
                ++i;
                total += 16;
            }

            // ��
            //memcpy( buffer_, mTempBuffer, count_ );
        }
        // �������Ȃ��ꍇ�� buffer_ �ɒ��ړǂ�ŁA�]����i�߂�
        else
        {
            read_size = fread( buffer_, 1, count_, mStream );
            fseek( mStream, _padding16( count_ ), SEEK_CUR );
            
            //fseek( mStream, ( 16 - ( count_ % 16 ) ) % 16, SEEK_CUR );
            
        }
        return read_size;
    }



	//---------------------------------------------------------------------
	// �� 4 �o�C�g�ǂݍ���
	//---------------------------------------------------------------------
	//u32 Archive::_readU32()
	//{
	//	u8 buffer[ 4 ];
	//	//fread( b, 1, 4, mStream );
	//	_readWithDecord( buffer, 4 );
	//	u32 r = buffer[ 0 ];
	//	r |= ( buffer[ 1 ] << 8 );
	//	r |= ( buffer[ 2 ] << 16 );
	//	r |= ( buffer[ 3 ] << 24 );
	//	return r;
	//}

	//---------------------------------------------------------------------
	// �� �f�R�[�h�t�� read
	//---------------------------------------------------------------------
	//u32 Archive::_readWithDecord( const void* buffer_, u32 count_ )
	//{
	//	char* b = (char*)buffer_;
	//	u32 read_size = 0;
	//	read_size = fread( b, 1, count_, mStream );

	//	if ( mDecoder && mKey.size() > 0 )
	//	{
	//		DecodeArgs args;
	//		args.Data		= b;
	//		args.Size		= read_size;
	//		args.Offset		= ftell( mStream ) - count_;
	//		args.Key		= mKey.c_str();
	//		args.KeyLength	= mKey.size();
	//		mDecoder( args );
	//	}
	//	
	//	return read_size;
	//}

    //---------------------------------------------------------------------
	// �� �f�t�H���g�̃f�R�[�_
    //---------------------------------------------------------------------
	//void Archive::_decoder( DecodeArgs& args_ )
 //   {
 //       u32 idx = args_.Offset % args_.KeyLength;

	//    for ( u32 i = 0; i < args_.Size; ++i )
	//    {
	//	    args_.Data[ i ] ^= ( args_.Key[ idx ] + idx );

	//	    idx = ( ++idx ) % args_.KeyLength;
	//    }
 //   }

//=============================================================================
// �� ArchiveInStream �N���X
//=============================================================================


    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    ArchiveInStream::ArchiveInStream( Archive* archive_, FILE* stream_, u32 data_offset_, u32 data_size_, LNFileOpenMode mode_, bool is_dir_ )
        : mArchive          ( archive_ )
        , mStream           ( stream_ )
        , mDataOffset       ( data_offset_ )
        , mDataSize         ( data_size_ )
        , mSeekPoint        ( 0 )
        , mOpenMode         ( mode_ )
        , mIsReadDirectory  ( is_dir_ )
    {
        LN_SAFE_ADDREF( mArchive );
    }

	//---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    ArchiveInStream::~ArchiveInStream()
    {
        if ( mIsReadDirectory && mStream )
        {
            mArchive->close( mStream );
        }

        LN_SAFE_RELEASE( mArchive );
    }

	//---------------------------------------------------------------------
	// �� �f�[�^�̓ǂݍ���
    //---------------------------------------------------------------------
    int ArchiveInStream::read( void* buffer_, int buffer_size_, int read_size_ )
    {
        if ( read_size_ < 0 )
		{
			read_size_ = buffer_size_;
		}

        // �ǂݍ��݊J�n�ʒu�Ɉړ�
		long read_point = static_cast< long >( mDataOffset ) + mSeekPoint;
		fseek( mStream, read_point, SEEK_SET );

        // �������Ȃ���ǂݍ���
		int read_size = mArchive->read( buffer_, read_size_, mStream, mOpenMode );

        // �ǂ񂾕������t�@�C���|�C���^���ړ�
		mSeekPoint += read_size;

        return read_size;
    }

	//---------------------------------------------------------------------
	// �� �t�@�C���|�C���^�̐ݒ�
    //---------------------------------------------------------------------
    void ArchiveInStream::seek( int offset_, int origin_ )
    {
        switch ( origin_ )
		{
			case SEEK_CUR:
				mSeekPoint += offset_;
				break;

			case SEEK_END:
				mSeekPoint = mDataSize + offset_;
				break;

			default :
				mSeekPoint = offset_;
				break;
		}

		if ( mSeekPoint < 0 ) { 
			mSeekPoint = 0; }
		if ( mSeekPoint > mDataSize ) { 
			mSeekPoint = mDataSize; }
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