//=============================================================================
//�y Misc �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Misc.h"
#include "../Thread/Mutex.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Base
{

    TempBuffer::TempBuffer()
        : mBuffer   ( NULL )
        , mSize     ( 0 )
        , mMutex    ( LN_NEW Thread::Mutex() )
    {
    	}

    TempBuffer::~TempBuffer()
    {
        LN_SAFE_RELEASE( mMutex );
        SAFE_DELETE_ARRAY( mBuffer );
        mSize = 0;
    }

    // size_ ���̗̈���m�ۂ��āA�擪�A�h���X��Ԃ� (���Ɋm�ۂ���Ă���ꍇ�͊m�ۂ��Ȃ�)
    lnByte* TempBuffer::request( u32 size_ )
    {
        if ( size_ > mSize )
        {
            SAFE_DELETE_ARRAY( mBuffer );
            mSize = size_;
            mBuffer = LN_NEW lnByte[ mSize + 1 ];
        }
        mBuffer[ size_ ] = (lnByte)NULL;
        return mBuffer;
    }

    // �r������t�� requestBuffer()
    lnByte* TempBuffer::lock( u32 size_ )
    {
        mMutex->lock();
        return request( size_ );
    }

    // lookBuffer() �ɑ΂���A�����b�N
    void TempBuffer::unlock()
    {
        mMutex->unlock();
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