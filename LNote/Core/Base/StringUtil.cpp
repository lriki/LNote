//=============================================================================
//�y StringUtil �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <algorithm>
#include "StringUtil.h"

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
// �� StringUtil �N���X
//=============================================================================

	//---------------------------------------------------------------------
	// �� �o�b�t�@���m�ۂ��ĕ�������R�s�[���� ( �}���`�o�C�g������ )
	//---------------------------------------------------------------------
	char* StringUtil::copyLength( const char* src_str_, int length_ )
	{
        if ( src_str_ )
        {
		    if ( length_ < 0 )
		    {
			    length_ = strlen( src_str_ );
		    }
		    char* str = LN_NEW char[ length_ + 1 ];
		    memcpy( str, src_str_, sizeof( char ) * length_ );
		    str[ length_ ] = '\0';
		    return str;
        }
        else
        {
            char* str = LN_NEW char[ 1 ];
            *str = '\0';
            return str;
        }
	}

	//---------------------------------------------------------------------
	// �� �o�b�t�@���m�ۂ��ĕ�������R�s�[���� ( ���C�h������ )
	//---------------------------------------------------------------------
	wchar_t* StringUtil::copyLength( const wchar_t* src_str_, int length_ )
	{
        if ( src_str_ )
        {
		    if ( length_ < 0 )
		    {
			    length_ = wcslen( src_str_ );
		    }
		    wchar_t* str = LN_NEW wchar_t[ length_ + 1 ];
		    memcpy( str, src_str_, sizeof( wchar_t ) * length_ );
		    str[ length_ ] = L'\0';
		    return str;
        }
        else
        {
            wchar_t* str = LN_NEW wchar_t[ 1 ];
            *str = L'\0';
            return str;
        }
	}

	//---------------------------------------------------------------------
	// �� �o�b�t�@���m�ۂ��ĕ�������R�s�[���� ( �}���`�o�C�g������ )
	//---------------------------------------------------------------------
	char* StringUtil::copyLength( const char* src_str_, int begin_, int length_ )
	{
		if ( begin_ >= static_cast< int >( strlen( src_str_ ) ) )
		{
			length_ = 0;
		}
		return copyLength( src_str_ + begin_, length_ );
	}

	//---------------------------------------------------------------------
	// �� �o�b�t�@���m�ۂ��ĕ�������R�s�[���� ( ���C�h������ )
	//---------------------------------------------------------------------
	wchar_t* StringUtil::copyLength( const wchar_t* src_str_, int begin_, int length_ )
	{
		if ( begin_ >= static_cast< int >( wcslen( src_str_ ) ) )
		{
			length_ = 0;
		}
		return copyLength( src_str_ + begin_, length_ );
	}

	//---------------------------------------------------------------------
	// �� �o�b�t�@���m�ۂ��ĕ�������R�s�[���� ( �͈͎w��E�}���`�o�C�g������ )
	//---------------------------------------------------------------------
	char* StringUtil::copyRange( const char* src_str_, int begin_, int end_ )
	{
		int len = ( end_ - begin_ ) + 1;
		if ( len < 0 )
		{
			len = 0;
		}
		char* str = LN_NEW char[ len + 1 ];
		memcpy( str, &( src_str_[ begin_ ] ), sizeof( char ) + len );
		str[ len ] = '\0';
		return str;
	}

	//---------------------------------------------------------------------
	// �� �o�b�t�@���m�ۂ��ĕ�������R�s�[���� ( �͈͎w��E���C�h������ )
	//---------------------------------------------------------------------
	wchar_t* StringUtil::copyRange( const wchar_t* src_str_, int begin_, int end_ )
	{
		int len = ( end_ - begin_ ) + 1;
		if ( len < 0 )
		{
			len = 0;
		}
		wchar_t* str = LN_NEW wchar_t[ len + 1 ];
		memcpy( str, &( src_str_[ begin_ ] ), sizeof( wchar_t ) + len );
		str[ len ] = L'\0';
		return str;
	}

    //---------------------------------------------------------------------
	// �� �ӂ��̕���������������V������������쐬���� ( char )
    //---------------------------------------------------------------------
    char* StringUtil::merge( const char* src1_, const char* src2_ )
    {
        int len1 = strlen( src1_ );
        char* new_str = LN_NEW char[ len1 + strlen( src2_ ) + 1 ];
        strcpy( new_str, src1_ );
        strcpy( ( new_str + len1 ), src2_ );
        return new_str;
    }

    //---------------------------------------------------------------------
	// �� �ӂ��̕���������������V������������쐬���� ( char )
    //---------------------------------------------------------------------
    wchar_t* StringUtil::merge( const wchar_t* src1_, const wchar_t* src2_ )
    {
        int len1 = wcslen( src1_ );
        wchar_t* new_str = LN_NEW wchar_t[ len1 + wcslen( src2_ ) + 1 ];
        wcscpy( new_str, src1_ );
        wcscpy( ( new_str + len1 ), src2_ );
        return new_str;
    }

	//---------------------------------------------------------------------
	// �� �}���`�o�C�g����������C�h������ɕϊ�������̕������擾
	//---------------------------------------------------------------------
	int StringUtil::getMultiToWideLength( const char* multi_str_ )
	{
	    if ( !multi_str_ )
        {
            return -1;
        }
	    
#if defined(LNOTE_GNUC)
        mbstate_t state;
        ::memset( &state, 0, sizeof( state ) );
        return static_cast< int >( mbsrtowcs( NULL, &multi_str_, 0, &state ) );
#else
        size_t size = 0;
        mbstowcs_s( &size, NULL, 0, multi_str_, 0 );
        return static_cast< int >( size ) - 1;
#endif
    }

    //---------------------------------------------------------------------
    // �� �}���`�o�C�g����������C�h������ɕϊ�
    //---------------------------------------------------------------------
    bool StringUtil::convMultiToWide( wchar_t* out_str_, const int size_, const char* in_str_ )
    {
        if ( !out_str_ || size_ <= 0 || !in_str_ )
        {
            return false;
        }
#if defined(LNOTE_GNUC)
        mbstate_t state;
        ::memset( &state, 0, sizeof( state ) );
        mbsrtowcs( out_str_, &in_str_, size_, &state );
#else
        // ���C�h�����ɕϊ� ( 3�߂̈����� wchar �ł̕����� )
        errno_t err = mbstowcs_s( NULL, out_str_, size_, in_str_, _TRUNCATE );

        if ( err == ERANGE )
        {
            out_str_[ 0 ] = L'\0';
        }
#endif
        return true;
    }

	//---------------------------------------------------------------------
	// �� �}���`�o�C�g����������C�h������ɕϊ�
	//---------------------------------------------------------------------
	wchar_t* StringUtil::convMultiToWide( const char* multi_str_ )
	{
        if ( !multi_str_ )
        {
            return NULL;
        }

		int len = StringUtil::getMultiToWideLength( multi_str_ ) + 1;
		wchar_t* str = LN_NEW wchar_t[ len ];
		StringUtil::convMultiToWide( str, len, multi_str_ );
		return str;
	}

    //---------------------------------------------------------------------
    // �� ���C�h��������}���`�o�C�g������ɕϊ�������̕������擾
    //---------------------------------------------------------------------
    int StringUtil::getWideToMultiLength( const wchar_t* wide_str_ )
    {
        if ( !wide_str_ )
        {
            return -1;
        }
#if defined(LNOTE_GNUC)
        mbstate_t state;
        ::memset( &state, 0, sizeof( state ) );
        return static_cast< int >( wcsrtombs( NULL, &wide_str_, 0, &state ) );
#else
        size_t size = 0;
        wcstombs_s(
            &size,		// [out] �ϊ����ꂽ������
            NULL,		// [out] �ϊ����ʂ̃}���`�o�C�g������p�̃o�b�t�@�̃A�h���X
            0,			// [in]  mbstr (������) �o�b�t�@�̃T�C�Y (�o�C�g��)
            wide_str_,	// [in]	 �ϊ�����郏�C�h������
            0 );		// [in]	 mbstr (������)�o�b�t�@�Ɋi�[�����ő�o�C�g���܂��� _TRUNCATE
        return static_cast< int >( size ) - 1;
#endif
	}
	
	//---------------------------------------------------------------------
	// �� ���C�h��������}���`�o�C�g������ɕϊ�
	//---------------------------------------------------------------------
	bool StringUtil::convWideToMulti( char* out_str_, int size_, const wchar_t* in_str_ )
	{
        if ( !out_str_ || size_ <= 0 || !in_str_ )
        {
            return false;
        }
#if defined(LNOTE_GNUC)
	    mbstate_t state;
        ::memset( &state, 0, sizeof( state ) );
        wcsrtombs( out_str_, &in_str_, size_, &state );
        return true;
#else
        errno_t err = wcstombs_s( NULL, out_str_, size_, in_str_, _TRUNCATE );

        if ( err == ERANGE )
        {
            out_str_[ 0 ] = '\0';
        }
        return true;
#endif
	}

	//---------------------------------------------------------------------
	// �� ���C�h��������}���`�o�C�g������ɕϊ�
	//---------------------------------------------------------------------
	char* StringUtil::convWideToMulti( const wchar_t* wide_str_ )
	{
        if ( !wide_str_ ) { 
	return NULL; }

		int len = StringUtil::getWideToMultiLength( wide_str_ ) + 1;
		char* str = LN_NEW char[ len ];
		StringUtil::convWideToMulti( str, len, wide_str_ );
		return str;
	}

    
    //---------------------------------------------------------------------
	// �� �}���`�o�C�g������ 1 �Ƃ��ĕ��������J�E���g���� (unicode �̏ꍇ�͂��̂܂�)
    //---------------------------------------------------------------------
    int StringUtil::strlenMB( const lnChar* str_ )
    {
#ifdef LNOTE_UNICODE
        return _tcslen( str_ );
#else
        //int max_len = _tcslen( str_ );
        int count = 0;
        //int i = 0;
        //for ( ; i < max_len; )
        while ( *str_ )
        {
            if ( checkMultiByteChar( str_ ) )
            {
                str_ += 2;
                //i += 2;
            }
            else
            {
                ++str_;
                //++i
            }
            ++count;
        }
        return count;
#endif
    }

    //---------------------------------------------------------------------
	// �� ���C���h�J�[�h('*', '?')���g���ĕ�������r����
    //---------------------------------------------------------------------
    bool StringUtil::match( const lnChar* ptn_, const lnChar* str_ )
    {
        switch( *ptn_ )
	    {
		    case _T( '\0' ):
			    return _T( '\0' ) == *str_;
		    case _T( '*' ):
			    return match( ptn_ + 1, str_ )
			        || ( ( _T( '\0' ) != *str_ ) && match( ptn_, str_ + 1 ) );
		    case _T( '?' ):
			    return ( _T( '\0' ) != *str_ )
			        && match( ptn_ + 1, str_ + 1 );
		    default:
			    return ( (unsigned char)*ptn_ == (unsigned char)*str_ )
			        && match( ptn_ + 1, str_ + 1 );
	    }
    }
    
    //---------------------------------------------------------------------
	// �� ������̔�r (�啶�����������l�����Ȃ�)
    //---------------------------------------------------------------------
    bool StringUtil::strcmpI( const char* str1_, const char* str2_ )
    {
#if defined(LNOTE_MSVC)
        return stricmp( str1_, str2_ ) == 0;
#else
        return strcasecmp( str1_, str2_ ) == 0;
#endif
    }

    //---------------------------------------------------------------------
	// �� ������̔�r (�������w��ő啶�����������l�����Ȃ�)
    //---------------------------------------------------------------------
    bool StringUtil::strcmpNI( const char* str1_, const char* str2_, int n_ )
    {
        for ( int i = 0; ( (*str1_) && (*str2_) && (i < n_) ); ++i )
        {
            if ( toupper( *str1_ ) != toupper( *str2_ ) )
            {
                return false;
            }
            ++str1_;
            ++str2_;
        }
        return true;
    }


    //---------------------------------------------------------------------
	// �� ������̒u�����s��
    //---------------------------------------------------------------------
    void StringUtil::replaceString( std::string* str_, const char* from_, const char* to_ )
    {
        std::string::size_type pos = 0;
        int from_length = strlen( from_ );
        int to_length   = strlen( to_ );

        while ( pos = str_->find( from_, pos ), pos != std::string::npos )
        {
            str_->replace( pos, from_length, to_ );
            pos += to_length;
        }
    }
#if LNOTE_WCHAR_T_SUPPORT
    void StringUtil::replaceString( std::wstring* str_, const wchar_t* from_, const wchar_t* to_ )
    {
        std::wstring::size_type pos = 0;
        int from_length = wcslen( from_ );
        int to_length   = wcslen( to_ );

        while ( pos = str_->find( from_, pos ), pos != std::wstring::npos )
        {
            str_->replace( pos, from_length, to_ );
            pos += to_length;
        }
    }
#endif
    /*
    void StringUtil::replaceString( ln_std_tstring* str_, const lnChar* from_, const lnChar* to_ )
    {
        ln_std_tstring::size_type pos = 0;
        int from_length = _tcslen( from_ );
        int to_length   = _tcslen( to_ );

        while ( pos = str_->find( from_, pos ), pos != ln_std_tstring::npos )
        {
            str_->replace( pos, from_length, to_ );
            pos += to_length;
        }
    }
    */

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void StringUtil::replaceSpace( ln_std_tstring* out_, const lnChar* in_, const lnChar* to_ )
    {
        ln_std_tstring out = in_;
        ln_std_tstring::size_type pos = 0;
        int to_length   = _tcslen( to_ );
        
        while ( pos < out.size() )
        {
            if ( isspace( out[pos] ) )
            {
                int len = getSpaceLength( &out[pos] );
                out.replace( pos, len, to_ );
                pos += to_length;
            }
            else
            {
                ++pos;
            }
        }

        *out_ = out;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void StringUtil::deleteSpace( ln_std_tstring* str_ )
	{
		size_t pos;
		while((pos = str_->find_first_of(_T(" �@\t"))) != ln_std_tstring::npos)
		{
			str_->erase(pos, 1);
		}
	}

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    int StringUtil::getSpaceLength( const lnChar* str_ )
    {
        int count = 0;
        while ( *str_ != _T('\0') && isspace( *str_ ) )
        {
            ++count;
            ++str_;
        }
        return count;
    }

    //---------------------------------------------------------------------
	// �� ������̐擪�ƏI�[�݂̂̃z���C�g�X�y�[�X���폜���� string ��Ԃ�
    //---------------------------------------------------------------------
    void StringUtil::trimHFSpace( ln_std_tstring* out_, const ln_std_tstring& in_ )
    {
        const lnChar* s = in_.c_str();
        int end = in_.size();
        int count = 0;
        while ( --end >= 0 && s[ end ] == _T( ' ' ) )
        {
            ++count;
        }

        int begin = 0;
        while ( s[ begin ] != _T( '\0' ) && s[ begin ] == _T( ' ' ) )
        {
            ++begin;
        }

        *out_ = ln_std_tstring( s, begin, end + 1 );
    }
#if 0
    int Trim(char *s) {
    int i;
    int count = 0;

    /* ��|�C���^��? */
    if ( s == NULL ) { /* yes */
        return -1;
    }

    /* �����񒷂��擾���� */
    i = strlen(s);

    /* �������珇�ɋ󔒂łȂ��ʒu��T�� */
    while ( --i >= 0 && s[i] == ' ' ) count++;

    /* �I�[�i��������t������ */
    s[i+1] = '\0';

    /* �擪���珇�ɋ󔒂łȂ��ʒu��T�� */
    i = 0;
    while ( s[i] != '\0' && s[i] == ' ' ) i++;
    strcpy(s, &s[i]);

    return i + count;
}
#endif

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    bool StringUtil::trimRange( ln_std_tstring* out_, const lnChar* in_, lnChar begin_, lnChar end_ )
    {
        const lnChar* c = in_;

        int begin_pos = 0;
        while ( true )
        {
            if ( *c == begin_ )
            {
                break;
            }
            else if ( *c == _T('\0') )
            {
                return false;
            }
            ++c;
            ++begin_pos;
        }

        int end_pos = begin_pos;
        ++c;
        ++end_pos;
        while ( true )
        {
            if ( *c == end_ )
            {
                break;
            }
            else if ( *c == _T('\0') )
            {
                return false;
            }
            ++c;
            ++end_pos;
        }

        *out_ = ln_std_tstring( in_, begin_pos + 1, end_pos - begin_pos - 1 );
        return true;
    }
    
    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void StringUtil::split( std::vector< ln_std_tstring >* out_, const lnChar* str_, const lnChar* delim_ )
    {
        *out_ = split( str_, delim_ );
    }

    class RemoveCharPred
    {
    public:
        RemoveCharPred(lnChar code_)
            : mCode(code_)
        {}
        bool operator()(lnChar c) const
        {
            return c == mCode;
        }
    public:
        lnChar mCode;
    };

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void StringUtil::removeChar( ln_std_tstring* str_, lnChar code_ )
    {
        str_->erase(std::remove_if(str_->begin(), str_->end(), RemoveCharPred(code_)), str_->end());
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    int StringUtil::getNewLineCount( const lnChar* text_ )
    {
        int count = 0;
        for (; *text_; ++text_)
        {
            if (*text_ == _T('\n')) ++count;
        }
        return count;
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================