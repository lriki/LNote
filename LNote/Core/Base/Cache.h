//=============================================================================
//【 Cache 】
//-----------------------------------------------------------------------------
///**
//  @file       Cache.h
//  @brief      Cache
//  @author     Riki
//  @note
/*              ◆ キャッシュシステム全体の注意点
                    
                    参照カウント操作用の関数亭具マクロには LN_CACHED_REFOBJ_METHODS を使うこと。
                    Midi クラスで誤って LN_REFOBJ_METHODS を書いてしまい、
                    オブジェクトが delete された後の不正な値のままキャッシュに登録された
                    ということがあった。
*/
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <iostream>
#include <map>
#include "Container/Stack.h"
//#include "Container/Map.h"
#include "SharingKey.h"
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

#define LN_CACHED_REFOBJ_METHODS                                                \
    virtual s32 getRefCount()   { return Base::ReferenceObject::getRefCount(); }\
    virtual s32 addRef()        { return Base::ReferenceObject::addRef(); }     \
    virtual s32 release()                                                       \
    {                                                                           \
        CachedObject::release();                                                \
        return Base::ReferenceObject::release();                                \
    }                                                                           \
    virtual s32 Release()                                                       \
    {                                                                           \
        CachedObject::release();                                                \
        return Base::ReferenceObject::Release();                                \
    }

//=============================================================================
// ■ CacheObjectManager クラス
//-----------------------------------------------------------------------------
///**
//  @brief      キャッシュシステムの管理クラス
//
//  @note
//              参照カウントを持つ管理クラスに継承させて使う。
//
//              管理されるオブジェクトは、解放待ちリストに入れられたときに
//              参照カウントがひとつ増える。
//				…というより、カウントが 1 になった時に待ちリストに入れられる
//*/
//=============================================================================
template< class TYPE_ >
class CacheObjectManager
    : private NonCopyable
{
public:

    class CachedObject;
    class CacheUnusedList;

public:

    /// コンストラクタ
	CacheObjectManager();

    /// デストラクタ
    virtual ~CacheObjectManager();

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      初期化
    //
    //  @param[in]  capacity_ : 解放待ちリストのサイズ
    //
    //  @note
    //              capacity_ が 0 の場合、使用中のオブジェクトの共有のみ行い、
    //              解放待ちは行わない。
    //*/
    //---------------------------------------------------------------------
    void initialize( int capacity_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      終了処理
    //
    //  @note
    //              終了を通知して、これ以降キャッシュにオブジェクトを
    //              追加しないようにする。
    //*/
    //---------------------------------------------------------------------
    void finalize();

    //---------------------------------------------------------------------
    ///**
    //  @brief      解放待ちリストをクリアする
    //*/
    //---------------------------------------------------------------------
    void clear();

    //---------------------------------------------------------------------
    ///**
    //  @brief      オブジェクトを管理に追加する
    //
    //  @note
    //              キーが無効な場合は追加しない。
    //*/
    //---------------------------------------------------------------------
    void registerCachedObject( const SharingKey& key_, TYPE_* obj_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      キーに一致するオブジェクトを検索する
    //
    //  @note
    //              見つかった場合は参照カウントを増やして返す。
    //              解放待ちリストに入っていた場合はそれから外して、
    //              参照カウントが 1 の状態で返す。
    //              キーが無効の場合は常に NULL を返す。
    //*/
    //---------------------------------------------------------------------
    TYPE_* findCacheAddRef( const SharingKey& key_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      キャッシュの内容をテキストファイルに出力する
    //*/
    //---------------------------------------------------------------------
    void dumpCache( const lnChar* filename_ );

public:

    /// 参照カウントを増やす (CachedObject のコンストラクタから呼ばれる)
    virtual s32 addRef() = 0;

    /// 参照カウントを減らす (CachedObject のデストラクタから呼ばれる)
    virtual s32 release() = 0;

private:

    /// CachedObject 削除直前のコールバック
    void _onDeleteCachedObject( CachedObject* obj_ );

private:

    template < class CACHE_UNUSED_LIST >
    struct _mb
    {
        std::map< SharingKey, TYPE_* >  CacheUsingMap;
        CACHE_UNUSED_LIST               CacheUnusedList;
        Thread::Mutex*                  Mutex;
        bool                            IsSharingOnly;
        bool                            Finalized;
    };
    _mb< CacheUnusedList > _m;

    friend class CachedObject;
};

//=============================================================================
// ■ CachedObject クラス
//-----------------------------------------------------------------------------
///**
//  @brief      キャッシュされるオブジェクトの基底
//*/
//=============================================================================
template< class TYPE_ >
class CacheObjectManager< TYPE_ >::CachedObject
{
public:

    /// コンストラクタ
    CachedObject()
    {
        _m.Manager = NULL;
        _m.Using = true;
    }

    /// デストラクタ
    virtual ~CachedObject()
    {
        LN_SAFE_RELEASE( _m.Manager );
    }

public:

    /// 参照カウントを取得する
    virtual s32 getRefCount() = 0;

    /// 参照カウントを増やす
    virtual s32 addRef() = 0;

    /// release() の追加処理 (継承先で、ReferenceObject::release() の前で呼び出す)
    virtual s32 release()
    {
        if ( getRefCount() == 1 )
        {
            if ( _m.Manager && !_m.Manager->_m.Finalized )
            {
                _m.Manager->_onDeleteCachedObject( this );
            }
        }
       
        return 0;
    }

private:

    struct
    {
        CacheObjectManager* Manager;    ///< セットされるのは addCachedObject()。NULL ではない場合、キーは有効。
        SharingKey          Key;        ///< キー (addCachedObject() でセット)
        bool                Using;      ///< 解放待ちリストに入っている間は false
    } _m;

    friend class CacheObjectManager;
};

//=============================================================================
// ■ CacheUnusedList クラス
//-----------------------------------------------------------------------------
///**
//  @brief      キャッシュシステムのうち、現在使用されていないオブジェクトを管理するクラス
//*/
//=============================================================================
template< class TYPE_ >
class CacheObjectManager< TYPE_ >::CacheUnusedList
    : private NonCopyable
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      デフォルトコンストラクタ
    //
    //  @note
    //              デフォルトコンストラクタで作成した場合は
    //              そのあとに setCapacity() を呼んで領域を確保すること。
    //*/
    //---------------------------------------------------------------------
	CacheUnusedList()
        : mNodes    ( NULL )
        , mCapacity ( 0 )
        , mSize     ( 0 )
    {}

    //---------------------------------------------------------------------
    ///**
    //  @brief      デストラクタ
    //*/
    //---------------------------------------------------------------------
    ~CacheUnusedList()
    {
        clear();
        SAFE_DELETE_ARRAY( mNodes );
    }

public:

    /// 最大要素数の取得
    int getCapacity() const { return mCapacity - 1; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      領域の確保
    //
    //  @param[in]  capacity_ : 解放待ちリストのサイズ
    //*/
    //---------------------------------------------------------------------
    void setCapacity( int capacity_ )
    {
        clear();

        SAFE_DELETE_ARRAY( mNodes );

        if ( capacity_ == 0 )
        {
            mCapacity = 0;
            return;
        }

        mCapacity = capacity_ + 1;

        mNodes = LN_NEW Node[ mCapacity + 1 ];


        // 空き番号スタック
	    mIndexStack.setCapacity( mCapacity );
	    for ( int i = 0; i < mCapacity; ++i )
        {
		    mIndexStack.push( i );
	    }

        // mCapacity 番は最初の要素の前かつ最後の要素の後にあるダミー要素
		mNodes[ mCapacity ].Next = mNodes[ mCapacity ].Prev = mCapacity;
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      クリア
    //
    //  @par
    //              解放待ちリストに入っているものをすべて削除します。( delete 呼び出し )
    //*/
    //---------------------------------------------------------------------
    void clear()
    {
        if ( mNodes && mCapacity > 0 )
        {
		    Node* node = &mNodes[ mCapacity ];	// ダミーノード
		    u32 index = node->Next;             // ダミーノードの次が開始インデックス
            int i = 0;

		    while ( true )
		    {
			    // ダミーノードを指している場合は終了
			    if ( index == mCapacity )
			    {
				    break;
			    }
                // 削除
                _release( mNodes[ index ].Value );
                // 次へ
                index = mNodes[ index ].Next;

                ++i;
		    }

            mNodeMap.clear();

            // 空き番号スタック
	        mIndexStack.clear();
	        for ( int i = 0; i < mCapacity; ++i )
            {
		        mIndexStack.push( i );
	        }

            // ノードは new とかしてるわけじゃないので、ダミーの前後だけ付け変えておけばリストは空の状態になる
            mNodes[ mCapacity ].Next = mNodes[ mCapacity ].Prev = mCapacity;
            mSize = 0;
        }
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      オブジェクトを追加する
    //
    //  @note
    //              追加されると、参照カウントがひとつ増える。
    //*/
    //---------------------------------------------------------------------
    void addObject( CachedObject* obj_ )
    {
        if ( obj_ && !obj_->_m.Key.isEmpty() && mCapacity > 0 )
        {
            // すでに追加されているか調べる


            typename NodeMap::iterator itr = mNodeMap.find( obj_->_m.Key );
            if ( itr != mNodeMap.end() )
            {
                return;
            }

            // キャッシュを調べて古いものがあれば削除
            _checkList();

            int idx = mIndexStack.top();
            mIndexStack.pop();

            Node* dummy     = &mNodes[ mCapacity ];	    // ダミーノード
            Node* prev      = &mNodes[ dummy->Prev ];	// ダミーのひとつ前 ( 最後 )
            Node* new_node  = &mNodes[ idx ];           // その間に入れるノード

            // new_node をリストの末尾に追加する
            new_node->Prev = dummy->Prev;
            new_node->Next = mCapacity;
            dummy->Prev = idx;
            prev->Next = idx;

            new_node->Value = obj_;

            mNodeMap.insert( std::pair< SharingKey, Node* >( obj_->_m.Key, new_node ) );

            // 削除待ち中
            obj_->_m.Using = false;
            obj_->addRef();

            ++mSize;
        }
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      キーに一致するオブジェクトの検索
    //
    //  @note
    //              返されるオブジェクトの参照カウントは常に 1。
    //*/
    //---------------------------------------------------------------------
    TYPE_* findObject( const SharingKey& key_ )
    {
        if ( mCapacity == 0 ) { return NULL; }

        typename NodeMap::iterator it = mNodeMap.find( key_ );
        if ( it != mNodeMap.end() )
        {
            // 削除待ちリストから外す
            Node* n = it->second;
            u32 idx = mNodes[ n->Next ].Prev;   // 自分のインデックス
            mNodes[ n->Next ].Prev = n->Prev;
            mNodes[ n->Prev ].Next = n->Next;

            // 空きインデックスを戻す
            mIndexStack.push( idx );

            TYPE_* v = static_cast< TYPE_* >( it->second->Value );
            
            mNodeMap.erase( it );

            --mSize;

            // 使用中にする
            v->_m.Using = true;

            return v;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      キャッシュの内容を出力する
    //*/
    //---------------------------------------------------------------------
    void dump( FILE* stream_ );

private:

    /// キャッシュのリストを調べて、古いもの(先頭)がある場合は削除
    void _checkList()
    {
        if ( mSize >= mCapacity - 1 )
        {
            Node* dummy     = &mNodes[ mCapacity ];	    // ダミーノード
			u32 front_index = dummy->Next;
			Node* front     = &mNodes[ front_index ];	// 先頭のノード取り出し

            // front をリストから外す
            dummy->Next = front->Next;
            mNodes[ front->Next ].Prev = mCapacity;

            // 空いたインデックスをスタックに積む
            mIndexStack.push( front_index );

            // Map からも削除
            mNodeMap.erase( front->Value->_m.Key );

            --mSize;
            
			// ないと思うけど、一応 front がダミーじゃないかチェックしてから解放
			if ( front != dummy )
			{
                _release( front->Value );
			}            
        }
    }

    /// 参照カウントをへらす
	void _release( CachedObject* obj_ )
	{
		if ( obj_ )
        {
            obj_->release();
        }
	}

private:

    struct Node
	{
		int             Next;
		int             Prev;
        CachedObject*   Value;
	};

	typedef std::map< SharingKey, Node* >    NodeMap;

private:

    Node*           mNodes;         ///< 解放待ちリスト
    int             mCapacity;      ///< 最大要素数
    int             mSize;          ///< 現在の要素数
    Stack< int >    mIndexStack;    ///< 空き番号スタック
    NodeMap         mNodeMap;       ///< キーとノードの対応表

};


//=============================================================================
// ■ CacheObjectManager クラスの実装
//=============================================================================

    //---------------------------------------------------------------------
    // ● デフォルトコンストラクタ
    //---------------------------------------------------------------------
	template< class TYPE_ >
    CacheObjectManager< TYPE_ >::CacheObjectManager()
    {
        _m.IsSharingOnly = true;
        _m.Mutex = NULL;
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    template< class TYPE_ >
    CacheObjectManager< TYPE_ >::~CacheObjectManager()
    {
        LN_SAFE_RELEASE( _m.Mutex );
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    template< class TYPE_ >
    void CacheObjectManager< TYPE_ >::initialize( int capacity_ )
    {
        LN_SAFE_RELEASE( _m.Mutex );
        _m.CacheUnusedList.setCapacity( capacity_ );
        _m.IsSharingOnly = ( capacity_ == 0 );
        _m.Finalized = false;
        _m.Mutex = LN_NEW Thread::Mutex();
    }

    //---------------------------------------------------------------------
    // ● 終了処理
    //---------------------------------------------------------------------
    template< class TYPE_ >
    void CacheObjectManager< TYPE_ >::finalize()
    {
        if ( !_m.Finalized )
        {
            clear();
            _m.Finalized = true;
        }
    }

    //---------------------------------------------------------------------
    // ● オブジェクトを管理に追加する
    //---------------------------------------------------------------------
    template< class TYPE_ >
    void CacheObjectManager< TYPE_ >::registerCachedObject( const SharingKey& key_, TYPE_* obj_ )
    {
        _m.Mutex->lock();

        if ( !key_.isEmpty() && obj_->_m.Key.isEmpty() && !_m.Finalized )
        {
            _m.CacheUsingMap.insert( std::pair< SharingKey, TYPE_* >( key_, obj_ ) );
            obj_->_m.Manager = this;
            obj_->_m.Manager->addRef();
            obj_->_m.Key = key_;
        }

        _m.Mutex->unlock();
    }

    //---------------------------------------------------------------------
    // ● キーに一致するオブジェクトを検索する
    //---------------------------------------------------------------------
    template< class TYPE_ >
    TYPE_* CacheObjectManager< TYPE_ >::findCacheAddRef( const SharingKey& key_ )
    {
        TYPE_* obj = NULL;

        _m.Mutex->lock();

        if ( !key_.isEmpty() && !_m.Finalized )
        {
            // まずは今使ってる物を検索
            typename std::map< SharingKey, TYPE_* >::iterator itr = _m.CacheUsingMap.find( key_ );
            if ( itr != _m.CacheUsingMap.end() )
            {
                obj = itr->second;
                obj->addRef();
            }
            // キャッシングする場合は削除待ちリストも検索
            else if ( !_m.IsSharingOnly )
            {
                obj = _m.CacheUnusedList.findObject( key_ );
                if ( obj )
                {
                    // 使用中オブジェクトのマップに入れておく
                    _m.CacheUsingMap.insert( std::pair< SharingKey, TYPE_* >( obj->_m.Key, obj ) );
                }
            }
        }

        _m.Mutex->unlock();
        
        return obj;
    }

    //---------------------------------------------------------------------
    // ● 解放待ちリストをクリアする
    //---------------------------------------------------------------------
    template< class TYPE_ >
    void CacheObjectManager< TYPE_ >::clear()
    {
        _m.Mutex->lock();
        _m.CacheUnusedList.clear();
        _m.Mutex->unlock();
    }

    //---------------------------------------------------------------------
    // ● キャッシュの内容をテキストファイルに出力する
    //---------------------------------------------------------------------
    template< class TYPE_ >
    void CacheObjectManager< TYPE_ >::dumpCache( const lnChar* filename_ )
    {
#if defined(LNOTE_MSVC)
        _m.Mutex->lock();

        FILE* stream = _tfopen( filename_, "wt" );

        if ( stream )
        {
            _ftprintf( stream, _T( "================================================================\n" ) );
            _ftprintf( stream, _T( "■ CacheDump\n" ) );
            _ftprintf( stream, _T( "    Type     : %s\n" ), typeid( TYPE_ ).name() );
            _ftprintf( stream, _T( "    Capacity : %d\n" ), _m.CacheUnusedList.getCapacity() );
            _ftprintf( stream, _T( "----------------------------------------------------------------\n" ) );
            _ftprintf( stream, _T( "▼ 使用されているオブジェクト ( %d 個 )\n" ), _m.CacheUsingMap.size() );

            CachedObject* obj;
            typename std::map< SharingKey, TYPE_* >::iterator itr = _m.CacheUsingMap.begin();
            typename std::map< SharingKey, TYPE_* >::iterator end = _m.CacheUsingMap.end();
            for ( ; itr != end; ++itr )
            {
                obj = itr->second;

                // ポインタと参照カウント
                _ftprintf( stream, _T( "[ %p ] RefCount:%d " ), obj, obj->getRefCount() );

                // キー値
                if ( obj->_m.Key.isString() )
                {
                    _ftprintf( stream, _T( "Key(string):%s\n" ), obj->_m.Key.getKeyString() );
                }
                else
                {
                    _ftprintf( stream, _T( "Key(number):%u\n" ), obj->_m.Key.getKeyHash() );
                }
            }

            // 解放待ちリストの出力
            _m.CacheUnusedList.dump( stream );

            _ftprintf( stream, _T( "End CacheDump\n================================================================\n" ) );


            fclose( stream );
        }

        _m.Mutex->unlock();
#endif
    }
    
    //---------------------------------------------------------------------
    // ● CachedObject 削除直前のコールバック
    //---------------------------------------------------------------------
    template< class TYPE_ >
    void CacheObjectManager< TYPE_ >::_onDeleteCachedObject( CachedObject* obj_ )
    {
        
        // 使用中の場合、使用中オブジェクト対応表から外して、
        // キャッシングする場合は解放待ちリストに追加する
        if ( obj_->_m.Using )
        {
            _m.Mutex->lock();

            _m.CacheUsingMap.erase( obj_->_m.Key );
            if ( !_m.IsSharingOnly )
            {
                _m.CacheUnusedList.addObject( obj_ );
            }

            _m.Mutex->unlock();
        }

    }

//=============================================================================
// ■ CacheUnusedList クラスの実装
//=============================================================================

    //---------------------------------------------------------------------
    // ● キャッシュの内容を出力する
    //---------------------------------------------------------------------
    template< class TYPE_ >
    void CacheObjectManager< TYPE_ >::CacheUnusedList::dump( FILE* stream_ )
    {
        _ftprintf(
            stream_,
            _T( "----------------------------------------------------------------\n" )
            _T( "▼ 解放待ちのオブジェクト ( %d 個 )\n" ),
            mNodeMap.size() );

        CachedObject* obj;
        typename NodeMap::iterator itr = mNodeMap.begin();
        typename NodeMap::iterator end = mNodeMap.end();
        for ( ; itr != end; ++itr )
        {
            obj = itr->second->Value;

            // ポインタと参照カウント
            _ftprintf( stream_, _T( "[ %p ] RefCount:%d " ), obj, obj->getRefCount() );

            // キー値
            if ( obj->_m.Key.isString() )
            {
                _ftprintf( stream_, _T( "Key(string):%s\n" ), obj->_m.Key.getKeyString() );
            }
            else
            {
                _ftprintf( stream_, _T( "Key(number):%u\n" ), obj->_m.Key.getKeyHash() );
            }
        }
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================