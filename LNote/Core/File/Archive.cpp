//=============================================================================
//【 Archive 】
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
// ■ Archive クラス
//=============================================================================

    #pragma pack(1)
    struct LNArchiveHeader
    {
        char            ID[ 4 ];        ///< 'l' 'n' 'a' '_'
        char            VerMajor;
        char            VerMinor;
        unsigned char   KeyLength;
        char            Key[ 128 ];
        char            Padding[ 9 ];   ///< サイズが 16 の倍数になるようにするためのパディング
    };
    #pragma pack()

	//---------------------------------------------------------------------
	// ● コンストラクタ
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
	// ● デストラクタ
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
	// ● アーカイブファイルを開いてアクセスの準備をする
	//---------------------------------------------------------------------
	Archive::ErrorCode Archive::initialize(  const lnChar* directory_, const lnChar* filename_, const char* key_ )
	{
        if ( ( !mStream && mIsDummy ) || ( mStream ) )
        {
            LN_THROW_Initialized( 0 );
        }


		// 日本語ファイルを受け付けるようにする
		setlocale( LC_ALL, "" );

        mIsDummy = false;

		// ダミーの場合はフラグを立てるだけ
		if ( filename_ == NULL )
		{
			mIsDummy = true;
			return ERR_NOT_OPEN;
		}

        // 実際のファイル名を作る
        ln_std_tstring real_name = directory_;
        real_name += _T( "/" );
        real_name += filename_;

        LN_THROW_FileNotFound( File::isExist( real_name.c_str() ), real_name.c_str() );


        // 拡張子を除いた部分をコピーする
        mArchiveDirectory  = File::getExtName( filename_ );
        mArchiveDirectory += _T( "\\" );
        File::normalizePathName( &mArchiveDirectory );   // バックスラッシュ化
        
        
        //mDecoder = ( decoder_ ) ? decoder_ : _decoder;
		mKey = ( key_ ) ? key_ : "";

		mFileNum = 0;
		mIsDummy = false;

        // 拡張キーの作成
        if ( key_ )
        {
            lnByte key_buf[ KEY_SIZE ] = { 0 };
            memcpy( key_buf, key_, strlen( key_ ) );
            memset( mKeyTable, 0, sizeof( mKeyTable ) );
            Camellia_Ekeygen( KEY_SIZE, key_buf, mKeyTable );
        }

        // とりあえず一時バッファを確保しておく
        _checkTempBuffer( 5000 );

		// アーカイブファイルを開く
        mStream = _tfopen( real_name.c_str(), _T( "rb" ) );
		if ( !mStream )
		{
			return ERR_NOT_OPEN;	// 開けなかった
		}

		// ファイルサイズ取得
		fseek( mStream, 0, SEEK_END );
		long archive_size = ftell( mStream );

        // 16バイト戻ってからそれを読むとファイル数
		fseek( mStream, -16, SEEK_END );
		mFileNum = _readU32Padding16();

        // 先頭に戻ってヘッダをチェック
        fseek( mStream, 0, SEEK_SET );
        LNArchiveHeader header;
        _readPadding16( &header, sizeof( header ) );

        //_p(sizeof( header ));

        // マジックナンバーをチェック
        if ( memcmp( header.ID, "lna_", 4 ) != 0 )
		{
			fclose( mStream );
			mStream = NULL;
			return ERR_FORMAT;
		}
        // 埋め込まれたキーと一致するかチェック
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

        // Map の領域をファイル数分確保
        //mEntriesMap.setCapacity( mFileNum );

        // ファイル情報を取得していく
		u32 name_len;
		char* name_buf;
        LRefTString tn;
		Entry entry;
		for ( int i = 0; i < mFileNum; ++i )
		{
			// ファイル名の長さとファイルサイズを読む
            _readU32Padding16( &name_len, &entry.mSize );

            

			// ファイル名を読み込むバッファを確保して読みこみ
			name_buf = LN_NEW char[ name_len + 1 ];
			_readPadding16( name_buf, name_len );
			name_buf[ name_len ] = _T( '\0' );

            // ここでのファイルポインタがデータ本体の位置
			entry.mOffset = ftell( mStream );
			
            tn = mArchiveDirectory;
            tn += name_buf;

            //_p(tn.c_str());
            //_p(entry.mSize);

			mEntriesMap.insert( std::pair< LRefTString, Entry >( tn, entry ) );

			// ファイルポインタをデータサイズ分進める
			//fseek( mStream, entry.mSize + ( 16 - ( entry.mSize % 16 ) ) % 16, SEEK_CUR );
            u32 ofs = _padding16( entry.mSize );
            fseek( mStream, entry.mSize + ofs, SEEK_CUR );
            
			
			SAFE_DELETE_ARRAY( name_buf );
		}

        mLock = LN_NEW Thread::Mutex();

		return SUCCESS;
	}


	//---------------------------------------------------------------------
	// ● ファイルストリームとデータ位置までのオフセット取得
	//---------------------------------------------------------------------
	void Archive::open( const lnChar* filename_, FILE** out_stream_, int* data_offset_, int* data_size_, LNFileOpenMode mode_ )
	{
		*out_stream_ = NULL;
		*data_offset_ = 0;
		*data_size_ = 0;
		if ( mIsDummy )
		{
            FILE* fp =  _tfopen( filename_, (mode_ == LN_OPENMODE_BINARY) ? _T("rb") : _T("r") );
            
			// ファイルが無い場合
			if ( !fp )
			{
				return;
			}

			// ファイルサイズを調べて data_size_ に返す
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
	// ● アーカイブ内のファイルにアクセスするためのストリームの作成
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
	// ● アーカイブの中にファイルがあるかどうか確認する
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
	// ● 読み込み
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

            int cur_fpos = ftell( mStream );            // 例)位置が 20 の場合
            int over_fpos = ( cur_fpos % 16 );          // 4
            int read_start_pos = cur_fpos - over_fpos;  // 16
            
            // 16 バイト単位になるように戻す
            fseek( mStream, read_start_pos, SEEK_SET ); // seek 16

            int over_count = ( count_ % 16 );           // count_ が 30 の場合、14
            int r_size = count_ + over_fpos;            // count_ が 30 の場合、34
            r_size += _padding16( r_size );//( 16 - ( r_size % 16 ) ) % 16;    // 48 バイト読む

            // 必要な領域を確保
            _checkTempBuffer( r_size );

            read_size = fread( mTempBuffer, 1, r_size, mStream );

            int tmpbuf_pos = 0;     // mTempBuffer から読むバイト位置
            u32 total = 0;          // mTempBuffer から読んだバイト数の累計 (buffer_ に転送しようとしたバイト数)
            int re_count = count_;  // あと残り buffer_ に書き込めるバイト数
            lnByte tmp_buf[ 16 ];
            lnByte* write_pos = static_cast< lnByte* >( buffer_ );

            // 前方に余分に読んだものがある場合
            if ( over_fpos > 0 )
            {
                Camellia_Decrypt( KEY_SIZE, &mTempBuffer[ total ], mKeyTable, tmp_buf );
                tmpbuf_pos += 16;

                int data_size = 16 - over_fpos; // 余分を除いたデータサイズ
                memcpy( write_pos, &tmp_buf[ over_fpos ], std::min( re_count, data_size ) );   // min は count_ 以上書きこまないため
                write_pos = write_pos + data_size;
                
                total += data_size;
                re_count -= std::min( re_count, data_size );
            }

            while ( true )
            {
                // 今回の読み込みで count_ に到達する場合は終了
                // count_ が 16 丁度の場合はもう一周
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

            // 後方に余分に読んだものがある場合
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



            

            // 呼び出し側での不具合を避けるため、正常に読めている場合は count_ を読んだバイト数とする
            if ( read_size > count_ )
            {
                read_size = count_;

                // ファイルポインタを正しい位置に移動
                fseek( mStream, cur_fpos + count_, SEEK_SET );
            }



            //-------------------------------------------------
            // テキストモードの場合は改行コードを変換
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
	// ● ストリームを閉じる
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
	// ● 一時バッファのサイズチェック
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
	// ● パディングを考慮して整数を読み込む
    //---------------------------------------------------------------------
	u32 Archive::_readU32Padding16()
    {
        lnByte b[ 16 ] = { 0 };

        // 復号する場合
        if ( mKey.size() > 0 )
        {
            lnByte buf[ 16 ];
            fread( buf, 1, 16, mStream );
            Camellia_Decrypt( KEY_SIZE, buf, mKeyTable, b );
        }
        // そのまま読み込む場合
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
	// ● パディングを考慮して、整数を読み込む 
    //---------------------------------------------------------------------
	void Archive::_readU32Padding16( u32* v0_, u32* v1_ )
    {
        lnByte b[ 16 ] = { 0 };

        // 復号する場合
        if ( mKey.size() > 0 )
        {
            lnByte buf[ 16 ];
            fread( buf, 1, 16, mStream );
            Camellia_Decrypt( KEY_SIZE, buf, mKeyTable, b );
        }
        // そのまま読み込む場合
        else
        {
            fread( b, 1, 16, mStream );
        }

        // 復号する場合
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
	// ● パディングを考慮してデータを読み込む
    //---------------------------------------------------------------------
	int Archive::_readPadding16( void* buffer_, int count_ )
    {
        int read_size;

        // 復号する場合
        if ( mKey.size() > 0 )
        {
            int count_p16 = _padding16( count_ );

            // 全体のサイズを計算して、バッファ確保
            int all_size = count_ + count_p16;//( 16 - ( count_ % 16 ) ) % 16;
            _checkTempBuffer( all_size );

            // 一時バッファに読む
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

            // 仮
            //memcpy( buffer_, mTempBuffer, count_ );
        }
        // 復号しない場合は buffer_ に直接読んで、余分を進める
        else
        {
            read_size = fread( buffer_, 1, count_, mStream );
            fseek( mStream, _padding16( count_ ), SEEK_CUR );
            
            //fseek( mStream, ( 16 - ( count_ % 16 ) ) % 16, SEEK_CUR );
            
        }
        return read_size;
    }



	//---------------------------------------------------------------------
	// ● 4 バイト読み込み
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
	// ● デコード付き read
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
	// ● デフォルトのデコーダ
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
// ■ ArchiveInStream クラス
//=============================================================================


    //---------------------------------------------------------------------
	// ● コンストラクタ
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
	// ● デストラクタ
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
	// ● データの読み込み
    //---------------------------------------------------------------------
    int ArchiveInStream::read( void* buffer_, int buffer_size_, int read_size_ )
    {
        if ( read_size_ < 0 )
		{
			read_size_ = buffer_size_;
		}

        // 読み込み開始位置に移動
		long read_point = static_cast< long >( mDataOffset ) + mSeekPoint;
		fseek( mStream, read_point, SEEK_SET );

        // 復号しながら読み込む
		int read_size = mArchive->read( buffer_, read_size_, mStream, mOpenMode );

        // 読んだ分だけファイルポインタを移動
		mSeekPoint += read_size;

        return read_size;
    }

	//---------------------------------------------------------------------
	// ● ファイルポインタの設定
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