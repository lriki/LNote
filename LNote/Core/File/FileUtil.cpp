//=============================================================================
//�y FileUtil �z
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
// �� ���l�����g���G���f�B�A���Ƃ��ď�������
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
// �� �t�@�C���p�X�̊g���q����菜�������̂�Ԃ� ( char )
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

    // �Ō�܂Ō�����Ȃ������ꍇ
	if ( pos == -1 )
	{
        return Base::StringUtil::copyLength( path_, -1 );
	}

    return Base::StringUtil::copyLength( path_, dot_pos );
}
*/

//---------------------------------------------------------------------
// �� �t�@�C���p�X�̒�����f�B���N�g���p�X�̕��������o�� ( char )
//---------------------------------------------------------------------
/*
char* getDirectoryPath( const char* path_ )
{
    int pos = strlen( path_ );

	// ��납��O�ɒ��ׂāA�ŏ��� \\ �� / ��������Ƃ����T��
	for ( ; pos >= 0; --pos )
	{
		if ( path_[ pos ] == '\\' || path_[ pos ] == '/' )
		{
			break;
		}
	}

	// �Ō�܂Ō�����Ȃ������ꍇ
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
// �� �t�@�C���p�X����f�B���N�g���p�X�ƃt�@�C���p�X��Ԃ�
//---------------------------------------------------------------------
bool getDirPathAndFilePath( const char* path_, char* dir_, char* name_ )
{
    int i = 0;
    int dir_last = -1;      // �f�B���N�g�����̏I�[ ( ������΂����ꂩ�̋�؂蕶���̈ʒu���w�� )
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

    // �f�B���N�g���p�X�i�[
    if ( dir_ )
    {
        // �f�B���N�g��������������Ȃ������ꍇ�͋�
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

    // �t�@�C�����i�[
    if ( name_ )   
    {
        // �f�B���N�g��������������Ȃ������ꍇ�͑S���R�s�[
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
// �� �t�@�C���p�X����f�B���N�g���p�X�ƃt�@�C���p�X��Ԃ� ( wchar_t )
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
// �� �t�@�C���|�C���^�̈ړ��v�Z
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
// �� ��������ɓW�J���ꂽ�f�[�^������̓X�g���[�����쐬����
//---------------------------------------------------------------------
void createInStreamFromMemoryManaged( IInStream** stream_, const void* buffer_, int size_ )
{
    *stream_ = NULL;

    Base::FinallyReleasePtr< InManagedMemoryStream > st( LN_NEW InManagedMemoryStream() );
    st->initialize( buffer_, size_ );

    *stream_ = st.returnObject();
}

//---------------------------------------------------------------------
// �� ��������ɓW�J���ꂽ�f�[�^������̓X�g���[�����쐬����
//---------------------------------------------------------------------
void createInStreamFromMemoryUnManaged( IInStream** stream_, const void* buffer_, int size_ )
{
    *stream_ = NULL;

    Base::FinallyReleasePtr< InUnManagedMemoryStream > st( LN_NEW InUnManagedMemoryStream() );
    st->initialize( buffer_, size_ );

    *stream_ = st.returnObject();
}

//---------------------------------------------------------------------
// �� ���l ( 2�o�C�g ) �̓ǂݍ���
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
// �� ���l ( 4�o�C�g ) �̓ǂݍ���
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
// �� ���l ( 2�o�C�g ) �̓ǂݍ��� ( �r�b�O�G���f�B�A������ )
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
// �� ���l ( 4�o�C�g ) �̓ǂݍ��� ( �r�b�O�G���f�B�A������ )
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
// �� �t�@�C���̑��݂��`�F�b�N����
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
// �� �t�@�C���p�X�̊g���q����菜�������̂�Ԃ�
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

    // �Ō�܂Ō�����Ȃ������ꍇ
	if ( pos == -1 )
	{
        return LRefTString( path_, -1 );
        //return Base::StringUtil::copyLength();
	}

    return LRefTString( path_, dot_pos );
    //return Base::StringUtil::copyLength( path_, dot_pos );
}

//---------------------------------------------------------------------
// �� �t�@�C���p�X�̒�����f�B���N�g���p�X�̕��������o��
//---------------------------------------------------------------------
LRefTString getDirectoryPath( const lnChar* path_ )
{
    int pos = strlen( path_ );

	// ��납��O�ɒ��ׂāA�ŏ��� \\ �� / ��������Ƃ����T��
	for ( ; pos >= 0; --pos )
	{
		if ( path_[ pos ] == _T( '\\' ) || path_[ pos ] == _T( '/' ) )
		{
			break;
		}
	}

	// �Ō�܂Ō�����Ȃ������ꍇ
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
// �� �t�@�C���p�X�̒�����g���q���܂ރt�@�C�����̕�����Ԃ�
//---------------------------------------------------------------------
LRefTString getFileName( const lnChar* path_ )
{
    int pos, len;
    pos = len = _tcslen( path_ );

    // ��납��O�ɒ��ׂāA�ŏ��� \\ �� / ��������Ƃ����T��
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
// �� �ł��邾���P���ȃt�@�C���p�X���쐬����
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
            // �ŏ��� "../" ����
            case 0:
            {
                if ( path_[ e - 2 ] == _T( '.' ) && path_[ e - 1 ] == _T( '.' ) && path_[ e ] == _T( '\\' ) )
                {
                    last_dot = e - 2;   // ../ �� �擪�� . ���w��
                    e -= 3;             // ���� / ���w��
                    cnt = 1;
                    step = 1;
                    continue;
                }
                --e;
                break;
            }

            // "../" �̘A�����J�E���g
            case 1:
            {
                if ( path_[ e - 2 ] == _T( '.' ) && path_[ e - 1 ] == _T( '.' ) && path_[ e ] == _T( '\\' ) )
                {
                    last_dot = e - 2;   // ../ �� �擪�� . ���w��
                    e -= 3;             // ���� / ���w��
                    ++cnt;
                    continue;
                }
                else
                {
                    //printf( "�A�����J�E���g�I�� %d\n", cnt );
                    path[ e ] = _T( '\0' );
                    step = 2;
                    continue;
                }
                break;
            }

            // �J�b�g���镔���� \0 �Ŗ��߂Ă���
            case 2:
            {
                // cnt ���c���Ă��邯�� "../" ����������
                if ( path_[ e - 2 ] == _T( '.' ) && path_[ e - 1 ] == _T( '.' ) && path_[ e ] == _T( '\\' ) )
                {
                    e -= 3; // ���� / ���w��
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
                    // �t�H���_�E�t�@�C�����̌��� / ����������
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

                        // / ���������A"../" �̏ꍇ�͏����Ȃ�
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

    // ��x�ł��ύX���������ꍇ
    if ( _tcslen( path ) != path_len )
    {
        int i = _tcslen( path );    // \0 ���w��
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
	// �� �t�@�C���p�X�̊g���q����菜�������̂�Ԃ� ( char )
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

        // �Ō�܂Ō�����Ȃ������ꍇ
		if ( pos == -1 )
		{
			return std::string( path_ );
		}

        return std::string( path_, dot_pos );
    }

    //---------------------------------------------------------------------
	// �� �t�@�C���p�X�̒�����f�B���N�g���p�X�̕��������o�� ( char )
	//---------------------------------------------------------------------
    std::string getDirectoryPath( const char* path_ )
    {
        int pos = strlen( path_ );

		// ��납��O�ɒ��ׂāA�ŏ��� \\ �� / ��������Ƃ����T��
		for ( ; pos >= 0; --pos )
		{
			if ( path_[ pos ] == '\\' || path_[ pos ] == '/' )
			{
				break;
			}
		}

		// �Ō�܂Ō�����Ȃ������ꍇ
		if ( pos == -1 )
		{
			return std::string();
		}

		return std::string( path_, pos );
    }

    //---------------------------------------------------------------------
	// �� �t�@�C���p�X�̒�����f�B���N�g���p�X�̕��������o��
	//---------------------------------------------------------------------
    std::string getDirectoryPath( const std::string& path_ )
    {
        int pos = path_.size();

		// ��납��O�ɒ��ׂāA�ŏ��� \\ �� / ��������Ƃ����T��
		for ( ; pos >= 0; --pos )
		{
			if ( path_[ pos ] == '\\' || path_[ pos ] == '/' )
			{
				break;
			}
		}

		// �Ō�܂Ō�����Ȃ������ꍇ
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