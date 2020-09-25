//=============================================================================
//�y ReferenceObject �z
//-----------------------------------------------------------------------------
///**
//  @file       ReferenceObject.h
//  @brief      ReferenceObject
//  @version    Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Misc.h"
#include "../Thread/Atomic.h"

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
// �� ReferenceObject �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �Q�ƃJ�E���g�����N���X�̊��
//*/
//=============================================================================
class ReferenceObject
    : private NonCopyable
{
public:

	/// �R���X�g���N�^
	ReferenceObject()
		: mReferenceCount( 1 )
	{}

protected:

	/// �f�X�g���N�^
	virtual ~ReferenceObject() {}

public:

	/// �Q�ƃJ�E���g�̎擾
	virtual s32 getRefCount()
    {
        return mReferenceCount.get();
    }

	/// �Q�ƃJ�E���g���ЂƂ��₷
	virtual s32 addRef()
    {
        return mReferenceCount.increment();
    }

	/// �Q�ƃJ�E���g���ЂƂ��炷
	virtual s32 release()
	{
        int count = mReferenceCount.decrement();
		if ( count <= 0 )
		{
			 delete this;
		}
        return count;
	}

    /// SAFE_RELEASE() �}�N���Ή��� release()
	virtual s32 Release()
	{
        int count = mReferenceCount.decrement();
		if ( count <= 0 )
		{
			 delete this;
		}
        return count;
    }

private:

    Thread::Atomic		mReferenceCount;	///< �Q�ƃJ�E���g
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

/// ReferenceObject �̌p����Œ�`����ׂ��֐����܂Ƃ߂��}�N��
#define LN_REFOBJ_METHODS                                                   \
    virtual s32 getRefCount()   { return ReferenceObject::getRefCount(); }  \
	virtual s32 addRef()        { return ReferenceObject::addRef(); }       \
    virtual s32 release()       { return ReferenceObject::release(); }

/// a_ �� b_ ���i�[
#define LN_REFOBJ_SET( a_, b_ ) \
    LN_SAFE_ADDREF( b_ ); \
    LN_SAFE_RELEASE( a_ ); \
    a_ = b_;

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================