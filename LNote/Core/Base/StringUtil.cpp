//=============================================================================
//【 StringUtil 】
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
// ■ StringUtil クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● バッファを確保して文字列をコピーする ( マルチバイト文字列 )
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
	// ● バッファを確保して文字列をコピーする ( ワイド文字列 )
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
	// ● バッファを確保して文字列をコピーする ( マルチバイト文字列 )
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
	// ● バッファを確保して文字列をコピーする ( ワイド文字列 )
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
	// ● バッファを確保して文字列をコピーする ( 範囲指定・マルチバイト文字列 )
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
	// ● バッファを確保して文字列をコピーする ( 範囲指定・ワイド文字列 )
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
	// ● ふたつの文字列を結合した新しい文字列を作成する ( char )
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
	// ● ふたつの文字列を結合した新しい文字列を作成する ( char )
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
	// ● マルチバイト文字列をワイド文字列に変換した後の文字数取得
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
    // ● マルチバイト文字列をワイド文字列に変換
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
        // ワイド文字に変換 ( 3つめの引数は wchar での文字数 )
        errno_t err = mbstowcs_s( NULL, out_str_, size_, in_str_, _TRUNCATE );

        if ( err == ERANGE )
        {
            out_str_[ 0 ] = L'\0';
        }
#endif
        return true;
    }

	//---------------------------------------------------------------------
	// ● マルチバイト文字列をワイド文字列に変換
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
    // ● ワイド文字列をマルチバイト文字列に変換した後の文字数取得
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
            &size,		// [out] 変換された文字数
            NULL,		// [out] 変換結果のマルチバイト文字列用のバッファのアドレス
            0,			// [in]  mbstr (第二引数) バッファのサイズ (バイト数)
            wide_str_,	// [in]	 変換されるワイド文字列
            0 );		// [in]	 mbstr (第二引数)バッファに格納される最大バイト数または _TRUNCATE
        return static_cast< int >( size ) - 1;
#endif
	}
	
	//---------------------------------------------------------------------
	// ● ワイド文字列をマルチバイト文字列に変換
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
	// ● ワイド文字列をマルチバイト文字列に変換
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
	// ● マルチバイト文字を 1 として文字数をカウントする (unicode の場合はそのまま)
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
	// ● ワイルドカード('*', '?')を使って文字列を比較する
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
	// ● 文字列の比較 (大文字小文字を考慮しない)
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
	// ● 文字列の比較 (文字数指定で大文字小文字を考慮しない)
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
	// ● 文字列の置換を行う
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
		while((pos = str_->find_first_of(_T(" 　\t"))) != ln_std_tstring::npos)
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
	// ● 文字列の先頭と終端のみのホワイトスペースを削除した string を返す
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

    /* 空ポインタか? */
    if ( s == NULL ) { /* yes */
        return -1;
    }

    /* 文字列長を取得する */
    i = strlen(s);

    /* 末尾から順に空白でない位置を探す */
    while ( --i >= 0 && s[i] == ' ' ) count++;

    /* 終端ナル文字を付加する */
    s[i+1] = '\0';

    /* 先頭から順に空白でない位置を探す */
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