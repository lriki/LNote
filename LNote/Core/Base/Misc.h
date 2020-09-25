//=============================================================================
//�y Misc �z
//-----------------------------------------------------------------------------
///**
//  @file       Misc.h
//  @brief      Misc
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Thread{ class Mutex; }

namespace Base
{

//=============================================================================
// �� NonCopyable �N���X
//-----------------------------------------------------------------------------
///**
//	@brief      �R�s�[���֎~����N���X�� private �p��������
//*/
//=============================================================================
class NonCopyable
{
protected:

    NonCopyable() {}
    virtual ~NonCopyable() {}

private:

    NonCopyable( NonCopyable const& );
    NonCopyable& operator=( NonCopyable const& );
};

//=============================================================================
// �� TempBuffer �N���X
//-----------------------------------------------------------------------------
///**
//	@brief      
//*/
//=============================================================================
class TempBuffer
{
public:

    TempBuffer();

    ~TempBuffer();

public:

    /// size_ ���̗̈���m�ۂ��āA�擪�A�h���X��Ԃ� (���Ɋm�ۂ���Ă���ꍇ�͊m�ۂ��Ȃ�)
    lnByte* request( u32 size_ );

    /// �r������t�� requestBuffer()
    lnByte* lock( u32 size_ );

    /// lookBuffer() �ɑ΂���A�����b�N
    void unlock();

    /// �m�ۂ����������̈�擾
    lnByte* getBuffer() { return mBuffer; }

private:

    lnByte* mBuffer;
    lnU32   mSize;
    Thread::Mutex* mMutex;
};

//=============================================================================
// �� TempBufferLock �N���X
//-----------------------------------------------------------------------------
///**
//	@brief      
//*/
//=============================================================================
class TempBufferLock
{
public:

    /// �R���X�g���N�^
    TempBufferLock( TempBuffer& target_obj_, lnU32 request_size_ )
        : mObject( target_obj_ )
    {
        mObject.lock( request_size_ );
    }

    /// �f�X�g���N�^
    ~TempBufferLock()
    {
        mObject.unlock();
    }

    /// �m�ۂ����������̈�擾
    lnByte* getBuffer() { return mObject.getBuffer(); }

private:

    TempBuffer& mObject;
};

//=============================================================================
// �� FinallyDeletePtr �N���X
//-----------------------------------------------------------------------------
///**
//	@brief      finally { delete obj; } �̑�p�N���X
//
//  @note
//              TYPE_ �̓|�C���^�^�ł��邱�ƁB
//*/
//=============================================================================
template < class TYPE_ >
class FinallyDeletePtr
    : Base::NonCopyable
{
public:

    FinallyDeletePtr( TYPE_ obj_ ) : mObject( obj_ ) { }

    ~FinallyDeletePtr() { SAFE_DELETE( mObject ); }

    TYPE_& operator * () { return *mObject; }

    TYPE_ operator -> () { return mObject; }

    TYPE_ returnObject()
    {
        TYPE_ obj = mObject;
        mObject = NULL;
        return obj;
    }

private:

    TYPE_  mObject;
};

//=============================================================================
// �� FinallyReleasePtr �N���X
//-----------------------------------------------------------------------------
///**
//	@brief      finally { obj->release; } �̑�p�N���X
//*/
//=============================================================================
template < class T >
class FinallyReleasePtr
    : Base::NonCopyable
{
public:

    FinallyReleasePtr() : p( NULL ) { }

    explicit FinallyReleasePtr( T* obj ) : p( obj ) { }

    ~FinallyReleasePtr() { LN_SAFE_RELEASE( p ); }

    operator T*() const { return p; }

    T** operator&() { return &p; }

    T& operator * () { return *p; }

    T* operator -> () { return p; }

    T* getPtr() { return p; }

    T* returnObject()
    {
        T* t = p;
        p = NULL;
        return t;
    }

private:

    T*  p;
};

#if 0
template < class TYPE_ >
class FinallyReleasePtr
    : Base::NonCopyable
{
public:

    FinallyReleasePtr( TYPE_ obj_ ) : mObject( obj_ ) { }

    ~FinallyReleasePtr() { LN_SAFE_RELEASE( mObject ); }

    TYPE_& operator * () { return *mObject; }

    TYPE_ operator -> () { return mObject; }

    TYPE_ getObjectPtr() { return mObject; }

    TYPE_ returnObject()
    {
        TYPE_ obj = mObject;
        mObject = NULL;
        return obj;
    }

private:

    TYPE_  mObject;
};
#endif

//=============================================================================
// �� 
//=============================================================================

/// ���g���G���f�B�A���������`�F�b�N����
inline bool isLittleEndian()
{
    static unsigned short x = 1;   // 0x0001
    return ( *(unsigned char *)&x ) != 0;
}

/// �����̌����𒲂ׂ�
inline int getNumberDigits( int x_ ) { int i = 0; while( x_ != 0 ) { ++i; x_/=10; } return i; }

/// �����̌����𒲂ׂ�
inline int getNumberDigits( u32 x_ ) { int i = 0; while( x_  > 0 ) { ++i; x_/=10; } return i; }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================