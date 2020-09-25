//=============================================================================
//【 SharingKey 】
//-----------------------------------------------------------------------------
///**
//  @file       SharingKey.h
//  @brief      SharingKey
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../File/FileUtil.h"
#include "StringUtil.h"
#include "Hash.h"

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
// ■ SharingKey クラス
//-----------------------------------------------------------------------------
///**
//	@brief      ハッシュ値、文字列 ( char、wchar_t ) をまとめてひとつのキーとして扱うクラス
//*/
//=============================================================================
class SharingKey
{
public:

    /// 空のキー
    SharingKey()
        : mHash( 0 )
    {
        mReqType = 0;
    }

    /// ハッシュ値 (整数値)
    explicit SharingKey( u32 key_ )
        : mHash( key_ )
    {
        mReqType = ( key_ != 0 ) ? 1 : 0;
    }

    /// 文字列
    explicit SharingKey( const lnChar* key_ )
        : mHash( 0 )
        , mString( key_ )
    {
        File::normalizePathName( &mString );
        mReqType = ( mString.size() > 0 ) ? 2 : 0;
    }

    /// RefString
    explicit SharingKey( RefTString key_ )
        : mHash( 0 )
        , mString( key_.c_str() )
    {
        File::normalizePathName( &mString );
        mReqType = ( mString.size() > 0 ) ? 2 : 0;
    }

    /// デストラクタ
    virtual ~SharingKey() {}

public:

    /// キーが空か判定する
    bool isEmpty() const 
    {
        return ( mHash == 0 && mString.size() == 0 );
    }

    /// キーが文字列かを判定する
    bool isString() const
    {
        return ( mHash == 0 && mString.size() > 0 );
    }

    /// 数値を取得する
    u32 getKeyHash() const { return mHash; }

    /// 文字列を取得する
    const lnChar* getKeyString() const { return ( mString.size() > 0 ) ? mString.c_str() : NULL; }

public:

    bool operator == ( const SharingKey& key_ ) const
    {
        if ( mReqType == key_.mReqType )
        {
            if ( mHash != 0 )
            {
                return ( mHash == key_.mHash );
            }
            if ( mString.size() > 0 && key_.mString.size() > 0 )
            {
                return ( mString == key_.mString );
            }
        }
        return false;
    };

    

    bool operator < ( const SharingKey& key_ ) const
    {
        if ( mReqType == key_.mReqType )
        {
            if ( mHash != 0 )
            {
                return ( mHash < key_.mHash );
            }
            if ( mString.size() > 0 )
            {
                return ( mString < key_.mString );
            }
        }
        return mReqType < key_.mReqType;
        //return mReqType == key_.mReqType; // ハマった…

        // map,set が検索を行う時は
        //「X1 < X2」「X2 < X1」の両方が成立しなかったとき「X1==X2」とみなす
        // ってのが標準的な動作らしい
    };

private:

    u32             mHash;
    RefTString      mString;
    u32             mReqType;

public:

    unsigned getGenericHash( unsigned m_ ) const
    {
        unsigned h = 0;
        int s = mString.size();
        if ( s > 0 )
        {
            const unsigned char* p = reinterpret_cast< const unsigned char* >( mString.c_str() );
            for ( int i = 0; i < s; ++i )
            {
		        h *= 137;
		        h += p[ i ];
	        }
            h %= m_;
        }
        return h;
    }
    
};

// class SharingKey 
template<> inline unsigned GenericHash< SharingKey >::value( const SharingKey& v, unsigned m ) const
{
    return v.getGenericHash( m );
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core

const Core::Base::SharingKey    lnNullKey;
typedef Core::Base::SharingKey  LSharingKey;

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================