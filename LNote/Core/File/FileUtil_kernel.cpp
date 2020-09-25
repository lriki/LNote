//=============================================================================
//【 FileUtil 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Base/StringUtil.h"
#include "FileUtil.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Kernel
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
        *( b + 2 ) = ( v_ & 0x00ff0000 ) >> 16;
        *( b + 3 ) = ( v_ & 0xff000000 ) >> 24;
        fwrite( &b, sizeof( b ), 1, stream_ );
    }
}

//---------------------------------------------------------------------
// ● ファイルの存在をチェックする
//---------------------------------------------------------------------
bool isExist( const char* path_ )
{
    if ( !path_ ) { 
	return false; }

	FILE* fp = fopen( path_, "rb" );
	if ( !fp ) { 
		return false; }
	fclose( fp );

	return true;
}

//---------------------------------------------------------------------
// ● ファイルの存在をチェックする ( wchar_t 用 )
//---------------------------------------------------------------------
bool isExist( const wchar_t* path_ )
{
    if ( !path_ ) { 
	return false; }

	FILE* fp = _wfopen( path_, L"rb" );
	if ( !fp ) { 
		return false; }
	fclose( fp );

	return true;
}

//---------------------------------------------------------------------
// ● ファイルパスの拡張子を取り除いたものを返す ( char )
//---------------------------------------------------------------------
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

//---------------------------------------------------------------------
// ● ファイルパスの中からディレクトリパスの部分を取り出す ( char )
//---------------------------------------------------------------------
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


//#ifdef _STRING_
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
} // namespace Kernel
} // namespace Lumine

//=============================================================================
//								end of file
//=============================================================================