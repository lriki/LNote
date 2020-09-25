//=============================================================================
//【 ReferenceObject 】
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
// ■ ReferenceObject クラス
//-----------------------------------------------------------------------------
///**
//  @brief      参照カウントを持つクラスの基底
//*/
//=============================================================================
class ReferenceObject
    : private NonCopyable
{
public:

	/// コンストラクタ
	ReferenceObject()
		: mReferenceCount( 1 )
	{}

protected:

	/// デストラクタ
	virtual ~ReferenceObject() {}

public:

	/// 参照カウントの取得
	virtual s32 getRefCount()
    {
        return mReferenceCount.get();
    }

	/// 参照カウントをひとつ増やす
	virtual s32 addRef()
    {
        return mReferenceCount.increment();
    }

	/// 参照カウントをひとつ減らす
	virtual s32 release()
	{
        int count = mReferenceCount.decrement();
		if ( count <= 0 )
		{
			 delete this;
		}
        return count;
	}

    /// SAFE_RELEASE() マクロ対応版 release()
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

    Thread::Atomic		mReferenceCount;	///< 参照カウント
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

/// ReferenceObject の継承先で定義するべき関数をまとめたマクロ
#define LN_REFOBJ_METHODS                                                   \
    virtual s32 getRefCount()   { return ReferenceObject::getRefCount(); }  \
	virtual s32 addRef()        { return ReferenceObject::addRef(); }       \
    virtual s32 release()       { return ReferenceObject::release(); }

/// a_ に b_ を格納
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