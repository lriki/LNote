//=============================================================================
//�y CommandLineArguments �z
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
// �� CommandLineArguments �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �R�}���h���C������
//
//  @note
//              �t�H�[�}�b�g�� "���O=�l" �� "���O"�B
//              �l���ȗ����Ă���ꍇ�AgetValue() �͋󕶎���("") ��Ԃ��B
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

    // main() �̈�������쐬
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
    /// Win32 �A�v���P�[�V�����̃R�}���h���C����������쐬
	void createWin32()
    {
        char* s = ::GetCommandLineA();
        std::vector< ln_std_tstring > args = StringUtil::split( s, " " );
        ::LocalFree( s );

        // EXE �p�X�� " ���폜
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
	    // �����𒲂ׂ�
	    int argc = 0;
	    for ( ; *c; ++argc )
	    {
		    // �擪��r����Ō�Ɏg���Ă���󔒂��X�L�b�v����
		    while ( *c == ' ' ) { ++c; }

		    // �R�}���h���C���Ō�̋󔒂������ꍇ�C���[�v���甲����
		    if ( !( *c ) ) { break; }

		    // "" �ň͂܂ꂽ�����������
		    if ( *c =='\"' )
		    {
			    // " ���X�L�b�v
			    ++c;

			    // �I���� " ��T��
			    while ( *c && *c != '\"' ) { ++c; }

			    // ���̎��_�� c �� " ���w���Ă���̂ŁA���̕����Ɉړ�
			    ++c;
		    }
		    // �X�y�[�X�ň͂܂ꂽ�����������
		    else
		    {
			    // ���̋󔒂܂ňړ�
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
		    // �擪��r����Ō�Ɏg���Ă���󔒂��X�L�b�v����
		    while ( *c == ' ' ) { ++c; }

		    // �R�}���h���C���Ō�̋󔒂������ꍇ�C���[�v���甲����
		    if ( !( *c ) ) { break; }

		    // "" �ň͂܂ꂽ�����������
		    if ( *c =='\"' )
		    {
			    // " ���X�L�b�v
			    ++c;

			    argv[ i ] = c;
			    ++i;

			    // �I���� " ��T��
			    while ( *c && *c != '\"' ) { ++c; }

			    *c = '\0';	// " �� \0 �ɕς��ċ�؂�

			    ++c;
		    }
		    // �X�y�[�X�ň͂܂ꂽ�����������
		    else
		    {
			    // ���̋󔒂܂ňړ�
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

    /// �l�̑��݃`�F�b�N
    bool isExist( const ln_std_tstring& key_ ) const
    {
        ValueMap::const_iterator itr = mValueMap.find( key_ );
        if ( itr != mValueMap.end() )
        {
            return true;
        }
        return false;
    }

    /// �l�̌���
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