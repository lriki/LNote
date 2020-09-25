//=============================================================================
//�y SharingKey �z
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
// �� SharingKey �N���X
//-----------------------------------------------------------------------------
///**
//	@brief      �n�b�V���l�A������ ( char�Awchar_t ) ���܂Ƃ߂ĂЂƂ̃L�[�Ƃ��Ĉ����N���X
//*/
//=============================================================================
class SharingKey
{
public:

    /// ��̃L�[
    SharingKey()
        : mHash( 0 )
    {
        mReqType = 0;
    }

    /// �n�b�V���l (�����l)
    explicit SharingKey( u32 key_ )
        : mHash( key_ )
    {
        mReqType = ( key_ != 0 ) ? 1 : 0;
    }

    /// ������
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

    /// �f�X�g���N�^
    virtual ~SharingKey() {}

public:

    /// �L�[���󂩔��肷��
    bool isEmpty() const 
    {
        return ( mHash == 0 && mString.size() == 0 );
    }

    /// �L�[�������񂩂𔻒肷��
    bool isString() const
    {
        return ( mHash == 0 && mString.size() > 0 );
    }

    /// ���l���擾����
    u32 getKeyHash() const { return mHash; }

    /// ��������擾����
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
        //return mReqType == key_.mReqType; // �n�}�����c

        // map,set ���������s������
        //�uX1 < X2�v�uX2 < X1�v�̗������������Ȃ������Ƃ��uX1==X2�v�Ƃ݂Ȃ�
        // ���Ă̂��W���I�ȓ���炵��
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