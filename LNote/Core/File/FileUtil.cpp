//=============================================================================
//【 FileUtil 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Base/StringUtil.h"
#include "Interface.h"
#include "StreamObject.h"
#include "FileUtil.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File
{


//---------------------------------------------------------------------
// ● 数値をリトルエンディアンとして書き込む
//---------------------------------------------------------------------
void writeU32Little( FILE* stream_, u32 v_ )
{
    if ( Base::isLittleEndian() )
    {
        fwrite( &v_, sizeof( v_ ), 1, stream_ );
    }
    else
    {
        u8 b[ 4 ];
        *( b + 0 ) =   v_ & 0x000000ff;
        *( b + 1 ) = ( v_ & 0x0000ff00 ) >> 8;
        *( b + 2 ) = static_cast< lnU8 >( ( v_ & 0x00ff0000 ) >> 16 );
        *( b + 3 ) = ( v_ & 0xff000000 ) >> 24;
        fwrite( &b, sizeof( b ), 1, stream_ );
    }
}


//---------------------------------------------------------------------
// ● ファイルパスの拡張子を取り除いたものを返す ( char )
//---------------------------------------------------------------------
/*
char* getExtName( const char* path_ )
{
    int pos = strlen( path_ );

    int dot_pos = pos;

    for ( ; pos >= 0; --pos )
    {
        if ( path_[ pos ] == '.' )
        {
            dot_pos = pos;
            break;
        }
        else if ( path_[ pos ] == '\\' || path_[ pos ] == '/' )
        {
            pos = 0;
            break;
        }
    }

    // 最後まで見つからなかった場合
	if ( pos == -1 )
	{
        return Base::StringUtil::copyLength( path_, -1 );
	}

    return Base::StringUtil::copyLength( path_, dot_pos );
}
*/

//---------------------------------------------------------------------
// ● ファイルパスの中からディレクトリパスの部分を取り出す ( char )
//---------------------------------------------------------------------
/*
char* getDirectoryPath( const char* path_ )
{
    int pos = strlen( path_ );

	// 後ろから前に調べて、最初に \\ か / が見つかるところを探す
	for ( ; pos >= 0; --pos )
	{
		if ( path_[ pos ] == '\\' || path_[ pos ] == '/' )
		{
			break;
		}
	}

	// 最後まで見つからなかった場合
	if ( pos == -1 )
	{
        char* out = LN_NEW char[ 1 ];
        out[ 0 ] = '\0';
		return out;
	}

    return Base::StringUtil::copyLength( path_, pos );
}
*/


//---------------------------------------------------------------------
// ● ファイルパスからディレクトリパスとファイルパスを返す
//---------------------------------------------------------------------
bool getDirPathAndFilePath( const char* path_, char* dir_, char* name_ )
{
    int i = 0;
    int dir_last = -1;      // ディレクトリ名の終端 ( 見つかればいずれかの区切り文字の位置を指す )
    while ( path_[ i ] )
    {
        if ( Base::StringUtil::checkMultiByteChar( &path_[ i ] ) )
        {
            i += 2;
        }
        else
        {
            if ( path_[ i ] == '\\' || path_[ i ] == '/' || path_[ i ] == '\0' || path_[ i ] == ':' )
            {
                dir_last = i;
            }
            ++i;
        }
    }

    // ディレクトリパス格納
    if ( dir_ )
    {
        // ディレクトリ部分が見つからなかった場合は空
        if ( dir_last == -1 )
        {
            dir_[ 0 ] = '\0';
        }
        else
        {
            strncpy( dir_, path_, dir_last );
		    dir_[ dir_last ] = '\0';
        }
    }

    // ファイル名格納
    if ( name_ )   
    {
        // ディレクトリ部分が見つからなかった場合は全部コピー
        if ( dir_last == -1 )
        {
            strcpy( name_, path_ );
        }
        else
        {
            strcpy( name_, &path_[ dir_last + 1 ] );
        }
    }
    return true;
}

//---------------------------------------------------------------------
// ● ファイルパスからディレクトリパスとファイルパスを返す ( wchar_t )
//---------------------------------------------------------------------
bool getDirPathAndFilePath( const wchar_t* path_, wchar_t* dir_, wchar_t* name_ )
{
    int i = 0;
    int dir_last = -1; 
    while ( path_[ i ] )
    {
        if ( path_[ i ] == L'\\' || path_[ i ] == L'/' || path_[ i ] == L'\0' || path_[ i ] == L':' )
        {
            dir_last = i;
        }
        ++i;
    }

    if ( dir_ )
    {
        if ( dir_last == -1 )
        {
            dir_[ 0 ] = L'\0';
        }
        else
        {
            wcsncpy( dir_, path_, dir_last );
		    dir_[ dir_last ] = L'\0';
        }
    }

    if ( name_ )   
    {
        if ( dir_last == -1 )
        {
            wcscpy( name_, path_ );
        }
        else
        {
            wcscpy( name_, &path_[ dir_last + 1 ] );
        }
    }
    return true;
}

//---------------------------------------------------------------------
// ● ファイルポインタの移動計算
//---------------------------------------------------------------------
int setSeekPoint( int now_point_, int max_size_, int offset_, int origin_ )
{
    int new_point = now_point_;
    switch ( origin_ )
	{
		case SEEK_CUR:
			new_point += offset_;
            break;

		case SEEK_END:
			new_point = max_size_ + offset_;
			break;

		default :
			new_point = offset_;
			break;
	}

	if ( new_point < 0 ) { 
		new_point = 0; }
	if ( new_point > max_size_ ) { 
		new_point = max_size_; }

    return new_point;
}




//---------------------------------------------------------------------
// ● メモリ上に展開されたデータから入力ストリームを作成する
//---------------------------------------------------------------------
void createInStreamFromMemoryManaged( IInStream** stream_, const void* buffer_, int size_ )
{
    *stream_ = NULL;

    Base::FinallyReleasePtr< InManagedMemoryStream > st( LN_NEW InManagedMemoryStream() );
    st->initialize( buffer_, size_ );

    *stream_ = st.returnObject();
}

//---------------------------------------------------------------------
// ● メモリ上に展開されたデータから入力ストリームを作成する
//---------------------------------------------------------------------
void createInStreamFromMemoryUnManaged( IInStream** stream_, const void* buffer_, int size_ )
{
    *stream_ = NULL;

    Base::FinallyReleasePtr< InUnManagedMemoryStream > st( LN_NEW InUnManagedMemoryStream() );
    st->initialize( buffer_, size_ );

    *stream_ = st.returnObject();
}

//---------------------------------------------------------------------
// ● 数値 ( 2バイト ) の読み込み
//---------------------------------------------------------------------
u16 readU16( IInStream* stream_ )
{
	u8 buffer[ 2 ];
	stream_->read( buffer, 2 );

	u16 r = buffer[ 0 ];
	r |= ( buffer[ 1 ] << 8 );
	return r;
}

//---------------------------------------------------------------------
// ● 数値 ( 4バイト ) の読み込み
//---------------------------------------------------------------------
u32 readU32( IInStream* stream_ )
{
	u8 buffer[ 4 ];
	stream_->read( buffer, 4, 4 );
	u32 r = buffer[ 0 ];
	r |= ( buffer[ 1 ] << 8 );
	r |= ( buffer[ 2 ] << 16 );
	r |= ( buffer[ 3 ] << 24 );
	return r;
}

//---------------------------------------------------------------------
// ● 数値 ( 2バイト ) の読み込み ( ビッグエンディアンから )
//---------------------------------------------------------------------
u16 readU16Big( IInStream* stream_ )
{
	u8 buffer[ 2 ];
	stream_->read( buffer, 2, 2 );
	u16 ret = 0;
	for ( int i = 0; i < 2; i ++ )
	{
		ret = ret << 8;
		ret |= ( (u8*)&buffer )[ i ];
	}
	return ret;
}

//---------------------------------------------------------------------
// ● 数値 ( 4バイト ) の読み込み ( ビッグエンディアンから )
//---------------------------------------------------------------------
u32 readU32Big( IInStream* stream_ )
{
	u8 buffer[ 4 ];
	stream_->read( buffer, 4, 4 );
	u32 ret = 0;
	for ( int i = 0; i < 4; i ++ )
	{
		ret = ret << 8;
		ret |= ( (u8*)&buffer )[ i ];
	}
	return ret;
}

//---------------------------------------------------------------------
// ● ファイルの存在をチェックする
//---------------------------------------------------------------------
bool isExist( const lnChar* path_ )
{
    if ( !path_ ) { 
	return false; }

	FILE* fp = _tfopen( path_, _T( "rb" ) );
	if ( !fp ) { 
		return false; }
	fclose( fp );

	return true;
}

//---------------------------------------------------------------------
// ● ファイルパスの拡張子を取り除いたものを返す
//---------------------------------------------------------------------
LRefTString getExtName( const lnChar* path_ )
{
    int pos = strlen( path_ );

    int dot_pos = pos;

    for ( ; pos >= 0; --pos )
    {
        if ( path_[ pos ] == _T( '.' ) )
        {
            dot_pos = pos;
            break;
        }
        else if ( path_[ pos ] == _T( '\\' ) || path_[ pos ] == _T( '/' ) )
        {
            pos = 0;
            break;
        }
    }

    // 最後まで見つからなかった場合
	if ( pos == -1 )
	{
        return LRefTString( path_, -1 );
        //return Base::StringUtil::copyLength();
	}

    return LRefTString( path_, dot_pos );
    //return Base::StringUtil::copyLength( path_, dot_pos );
}

//---------------------------------------------------------------------
// ● ファイルパスの中からディレクトリパスの部分を取り出す
//---------------------------------------------------------------------
LRefTString getDirectoryPath( const lnChar* path_ )
{
    int pos = strlen( path_ );

	// 後ろから前に調べて、最初に \\ か / が見つかるところを探す
	for ( ; pos >= 0; --pos )
	{
		if ( path_[ pos ] == _T( '\\' ) || path_[ pos ] == _T( '/' ) )
		{
			break;
		}
	}

	// 最後まで見つからなかった場合
	if ( pos == -1 )
	{
        return LRefTString();
        /*
        char* out = NEW char[ 1 ];
        out[ 0 ] = _T( '\0' );
		return out;
        */
	}

    return LRefTString( path_, pos );
    //return Base::StringUtil::copyLength( path_, pos );
}

//---------------------------------------------------------------------
// ● ファイルパスの中から拡張子を含むファイル名の部分を返す
//---------------------------------------------------------------------
LRefTString getFileName( const lnChar* path_ )
{
    int pos, len;
    pos = len = _tcslen( path_ );

    // 後ろから前に調べて、最初に \\ か / が見つかるところを探す
	for ( ; pos >= 0; --pos )
	{
		if ( path_[ pos ] == _T( '\\' ) || path_[ pos ] == _T( '/' ) )
		{
            ++pos;
			break;
		}
	}
    if ( pos < 0 )
    {
        pos = 0;
    }

    return LRefTString( &path_[ pos ], len - pos );
}

//---------------------------------------------------------------------
// ● できるだけ単純なファイルパスを作成する
//---------------------------------------------------------------------
void makeFileName( ln_std_tstring* out_, const lnChar* path_ )
{
    lnChar path[ LN_MAX_PATH ];
    _tcscpy( path, path_ );

    int path_len = _tcslen( path_ );
    int e = path_len;
    int last_dot = 0;
    int step = 0;
    int cnt = 0;

    for ( ; e > 2;  )
    {
        switch ( step )
        {
            // 最初の "../" 検索
            case 0:
            {
                if ( path_[ e - 2 ] == _T( '.' ) && path_[ e - 1 ] == _T( '.' ) && path_[ e ] == _T( '\\' ) )
                {
                    last_dot = e - 2;   // ../ の 先頭の . を指す
                    e -= 3;             // 次の / を指す
                    cnt = 1;
                    step = 1;
                    continue;
                }
                --e;
                break;
            }

            // "../" の連続数カウント
            case 1:
            {
                if ( path_[ e - 2 ] == _T( '.' ) && path_[ e - 1 ] == _T( '.' ) && path_[ e ] == _T( '\\' ) )
                {
                    last_dot = e - 2;   // ../ の 先頭の . を指す
                    e -= 3;             // 次の / を指す
                    ++cnt;
                    continue;
                }
                else
                {
                    //printf( "連続数カウント終了 %d\n", cnt );
                    path[ e ] = _T( '\0' );
                    step = 2;
                    continue;
                }
                break;
            }

            // カットする部分を \0 で埋めていく
            case 2:
            {
                // cnt が残っているけど "../" が見つかった
                if ( path_[ e - 2 ] == _T( '.' ) && path_[ e - 1 ] == _T( '.' ) && path_[ e ] == _T( '\\' ) )
                {
                    e -= 3; // 次の / を指す
                    cnt = 1;
                    step = 1;
                    continue;
                }
                else
                {
                    --e;
                    if ( path_[ e ] != _T( '\\' ) )
                    {
                        path[ e ] = _T( '\0' );
                    }
                    // フォルダ・ファイル名の後ろの / が見つかった
                    else
                    {
                        

                        path[ last_dot + 0 ] = _T( '\0' );
                        path[ last_dot + 1 ] = _T( '\0' );
                        path[ last_dot + 2 ] = _T( '\0' );

                        last_dot = last_dot + 3;

                        --cnt;
                        if ( cnt == 0 )
                        {
                            step = 0;
                            continue;
                        }

                        // / を消すが、"../" の場合は消さない
                        if ( path[ e - 1 ] != _T( '.' ) )
                        {
                            path[ e ] = _T( '\0' );
                        }
                    }
                }
                
                break;
            }
        }
    }

    if ( step == 2 )
    {
        for ( ;e > 0; )
        {
            --e;
            if ( path_[ e ] != _T( '\\' ) )
            {
                path[ e ] = _T( '\0' );
            }
        }

        path[ last_dot + 0 ] = _T( '\0' );
        path[ last_dot + 1 ] = _T( '\0' );
        path[ last_dot + 2 ] = _T( '\0' );
    }

    (*out_) = path;

    // 一度でも変更があった場合
    if ( _tcslen( path ) != path_len )
    {
        int i = _tcslen( path );    // \0 を指す
        for ( ;i < path_len; )
        {
            for ( ; path[ i ] == _T( '\0' ) && i < path_len; )
            {
                ++i;
            }
            (*out_) += &path[ i ];
            i += _tcslen( &path[ i ] );
        }
    }
}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	bool isFullPath( const lnChar* path )
	{
		if (*path == _T('/') || *path == _T('\\')) return true;

		for (; *path; ++path)
		{
			if (*path == _T(':')) return true;
		}

		return false;
	}

#if 0

    //---------------------------------------------------------------------
	// ● ファイルパスの拡張子を取り除いたものを返す ( char )
	//---------------------------------------------------------------------
    std::string getExtName( const char* path_ )
    {
        int pos = strlen( path_ );

        int dot_pos = pos;

        for ( ; pos >= 0; --pos )
        {
            if ( path_[ pos ] == '.' )
            {
                dot_pos = pos;
                break;
            }
            else if ( path_[ pos ] == '\\' || path_[ pos ] == '/' )
            {
                pos = 0;
                break;
            }
        }

        // 最後まで見つからなかった場合
		if ( pos == -1 )
		{
			return std::string( path_ );
		}

        return std::string( path_, dot_pos );
    }

    //---------------------------------------------------------------------
	// ● ファイルパスの中からディレクトリパスの部分を取り出す ( char )
	//---------------------------------------------------------------------
    std::string getDirectoryPath( const char* path_ )
    {
        int pos = strlen( path_ );

		// 後ろから前に調べて、最初に \\ か / が見つかるところを探す
		for ( ; pos >= 0; --pos )
		{
			if ( path_[ pos ] == '\\' || path_[ pos ] == '/' )
			{
				break;
			}
		}

		// 最後まで見つからなかった場合
		if ( pos == -1 )
		{
			return std::string();
		}

		return std::string( path_, pos );
    }

    //---------------------------------------------------------------------
	// ● ファイルパスの中からディレクトリパスの部分を取り出す
	//---------------------------------------------------------------------
    std::string getDirectoryPath( const std::string& path_ )
    {
        int pos = path_.size();

		// 後ろから前に調べて、最初に \\ か / が見つかるところを探す
		for ( ; pos >= 0; --pos )
		{
			if ( path_[ pos ] == '\\' || path_[ pos ] == '/' )
			{
				break;
			}
		}

		// 最後まで見つからなかった場合
		if ( pos == -1 )
		{
			return std::string();
		}

		return std::string( path_.c_str(), pos );
    }

#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace File
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================