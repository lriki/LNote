//=============================================================================
//【 CommandLineArguments 】
//-----------------------------------------------------------------------------
///**
//  @file       CommandLineArguments.h
//  @brief      CommandLineArguments
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <string>
#include <map>
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
// ■ CommandLineArguments クラス
//-----------------------------------------------------------------------------
///**
//  @brief      コマンドライン引数
//
//  @note
//              フォーマットは "名前=値" か "名前"。
//              値を省略している場合、getValue() は空文字列("") を返す。
//*/
//=============================================================================
class CommandLineArguments
{
public:

	CommandLineArguments()
	{}

	~CommandLineArguments()
	{}

public:

    // main() の引数から作成
    void create( int argc_, char *argv_[] )
    {
        for ( int i = 0; i < argc_; ++i )
        {
            std::vector< ln_std_tstring > pair = StringUtil::split( argv_[i], "=" );

            if (pair.size() == 1)
            {
                mValueMap.insert( ValueMapPair( pair[0], "" ) );
            }
            else
            {
                mValueMap.insert( ValueMapPair( pair[0], pair[1] ) );
            }
        }
    }

#if defined(LNOTE_WINDOWS)
    /// Win32 アプリケーションのコマンドライン引数から作成
	void createWin32()
    {
        char* s = ::GetCommandLineA();
        std::vector< ln_std_tstring > args = StringUtil::split( s, " " );
        ::LocalFree( s );

        // EXE パスの " を削除
        if ( args[0][0] == _T( '\"' ) )
        {
            ln_std_tstring exe_path( args[0], 1, args[0].size() - 2 );
            args[0] = exe_path;
        }

        ln_foreach( ln_std_tstring& a, args )
        {
            std::vector< ln_std_tstring > pair = StringUtil::split( a, "=" );

            if (pair.size() == 1)
            {
                mValueMap.insert( ValueMapPair( pair[0], "" ) );
            }
            else
            {
                mValueMap.insert( ValueMapPair( pair[0], pair[1] ) );
            }
        }

#if 0
	    // 件数を調べる
	    int argc = 0;
	    for ( ; *c; ++argc )
	    {
		    // 先頭や途中や最後に使われている空白をスキップする
		    while ( *c == ' ' ) { ++c; }

		    // コマンドライン最後の空白だった場合，ループから抜ける
		    if ( !( *c ) ) { break; }

		    // "" で囲まれた文字列を処理
		    if ( *c =='\"' )
		    {
			    // " をスキップ
			    ++c;

			    // 終わりの " を探す
			    while ( *c && *c != '\"' ) { ++c; }

			    // この時点で c は " を指しているので、次の文字に移動
			    ++c;
		    }
		    // スペースで囲まれた文字列を処理
		    else
		    {
			    // 次の空白まで移動
			    while ( *c && *c != ' ' ) { ++c; }
		    }
	    }

        

	    c = GetCommandLineA();
	    char** argv = static_cast< char** >( malloc( sizeof( char* ) * argc ) );
	    int i = 0;


        MessageBoxA(0, c, 0, MB_OK);
        char* a = "0";
        a[0] += argc;
        MessageBoxA(0, a, 0, MB_OK);

	    while ( *c )
	    {
		    // 先頭や途中や最後に使われている空白をスキップする
		    while ( *c == ' ' ) { ++c; }

		    // コマンドライン最後の空白だった場合，ループから抜ける
		    if ( !( *c ) ) { break; }

		    // "" で囲まれた文字列を処理
		    if ( *c =='\"' )
		    {
			    // " をスキップ
			    ++c;

			    argv[ i ] = c;
			    ++i;

			    // 終わりの " を探す
			    while ( *c && *c != '\"' ) { ++c; }

			    *c = '\0';	// " を \0 に変えて区切る

			    ++c;
		    }
		    // スペースで囲まれた文字列を処理
		    else
		    {
			    // 次の空白まで移動
			    while ( *c && *c != ' ' ) { ++c; }
		    }
	    }

        create( argc, argv );

	    SAFE_FREE( argv );
#endif
    }
#endif

    bool isExist( const lnChar* key_ ) const
    {
        return isExist(ln_std_tstring(key_));
    }

    /// 値の存在チェック
    bool isExist( const ln_std_tstring& key_ ) const
    {
        ValueMap::const_iterator itr = mValueMap.find( key_ );
        if ( itr != mValueMap.end() )
        {
            return true;
        }
        return false;
    }

    /// 値の検索
    const ln_std_tstring& getValue( const ln_std_tstring& key_ ) const
    {
        ValueMap::const_iterator itr = mValueMap.find( key_ );
        if ( itr != mValueMap.end() )
        {
            return (*itr).second;
        }
        return mDummy;
    }

private:

    typedef std::map< ln_std_tstring, ln_std_tstring >    ValueMap;
    typedef std::pair< ln_std_tstring, ln_std_tstring >   ValueMapPair;

private:

	ValueMap        mValueMap;
    ln_std_tstring  mDummy;
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================