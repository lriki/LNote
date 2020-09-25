//=============================================================================
//【 List 】
//-----------------------------------------------------------------------------
///**
//  @file       List.h
//  @brief      List
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <new>
#include "Stack.h"

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
// ■ List クラス
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
template< typename TYPE_ > class List
    : public NonCopyable
{
private:

    struct Node
	{
		int Next;
		int Prev;
	};

public:

    /// イテレータ
    class iterator
    {
    public:

        iterator()
        {
            memset( &_m, 0, sizeof( _m ) );
        }

        iterator( const iterator& obj_ )
        {
            _m.Values = obj_._m.Values;
            _m.Nodes = obj_._m.Nodes;
            _m.Pos = obj_._m.Pos;
        }

        iterator& operator = ( const iterator& obj_ )
        {
            _m.Values = obj_._m.Values;
            _m.Nodes = obj_._m.Nodes;
            _m.Pos = obj_._m.Pos;
            return (*this);
        }

        TYPE_& operator * ()    // const はつけない
        {
            return _m.Values[ _m.Pos ];
        }

        TYPE_* operator -> ()
        {
            return &_m.Values[ _m.Pos ];
        }

        const TYPE_* operator -> () const
        {
            return &_m.Values[ _m.Pos ];
        }

        iterator& operator ++ ()
        {
            _m.Pos = _m.Nodes[ _m.Pos ].Next;
            return *this;
        }

        iterator operator ++ ( int )
        {
            iterator c( *this );
            _m.Pos = _m.Nodes[ _m.Pos ].Next;
            return c;
        }

        iterator& operator -- ()
        {
            _m.Pos = _m.Nodes[ _m.Pos ].Prev;
            return *this;
        }

        iterator operator -- ( int )
        {
            iterator c( *this );
            _m.Pos = _m.Nodes[ _m.Pos ].Prev;
            return c;
        }

        bool operator == ( const iterator& obj_ ) const
        {
            return ( _m.Pos == obj_._m.Pos );
        }

        bool operator != ( const iterator& obj_ ) const
        {
            return ( _m.Pos != obj_._m.Pos );
        }

    private:

        iterator( TYPE_* values_, Node* nodes_, int pos_ )
        {
            _m.Values = values_;
            _m.Nodes = nodes_;
            _m.Pos = pos_;
        }

    private:

        struct
        {
            TYPE_*  Values;		///< 配列本体
	        Node*   Nodes;
            int     Pos;
        } _m;

        friend class List< TYPE_ >;
    };

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      デフォルトコンストラクタ
    //
    //  @par
    //              デフォルトコンストラクタで作成した場合は
    //              そのあとに setCapacity() を呼んで領域を確保してください。
    //*/
    //---------------------------------------------------------------------
    List()
    {
        _m.Values = NULL;
        _m.Nodes = NULL;
        _m.Capacity = 0;
        _m.Size = 0;
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ
    //
    //  @param[in]  capacity_ : 格納できる要素数
    //*/
    //---------------------------------------------------------------------
	explicit List( int capacity_ )
	{
        _m.Values = NULL;
        _m.Nodes = NULL;
        _m.Capacity = 0;
        _m.Size = 0;
		setCapacity( capacity_ );
	}

    /// デストラクタ
	~List()
	{
		if ( _m.Nodes )
		{
			int pos = _m.Nodes[ _m.Capacity ].Next;
			// ダミーでない限りループ
			while ( pos < _m.Capacity )
			{
				// デストラクタ呼び出し
				_m.Values[ pos ].~TYPE_();
				// 次の要素へ
				pos = _m.Nodes[ pos ].Next;
			}
			LN_OPERATOR_DELETE( _m.Values );
			SAFE_DELETE_ARRAY( _m.Nodes );
		}
	}

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      領域の確保
    //
    //  @param[in]  capacity_ : 格納できる要素数
    //*/
    //---------------------------------------------------------------------
	void setCapacity( int capacity_ )
	{
		// まずはクリア
		if ( _m.Capacity > 0 )
		{
            _m.IndexStack.clear();
			clear();
			LN_OPERATOR_DELETE( _m.Values );
			SAFE_DELETE_ARRAY( _m.Nodes );
		}

		if ( capacity_ <= 0 ) { return; }
		_m.Capacity = capacity_;

		_m.Nodes = LN_NEW Node[ _m.Capacity + 1 ];

		// 必要な分のメモリ確保
		_m.Values = static_cast< TYPE_* >( LN_OPERATOR_NEW( sizeof( TYPE_ ) * _m.Capacity ) );


		// 空き番号スタックを作成して番号を全部詰め込む
        _m.IndexStack.setCapacity( _m.Capacity );
		for ( int i = 0; i < _m.Capacity; ++i )
		{
			_m.IndexStack.push( i );
		}

		// mCapacity 番は最初の要素の前かつ最後の要素の後にあるダミー要素
		_m.Nodes[ _m.Capacity ].Next = _m.Nodes[ _m.Capacity ].Prev = _m.Capacity;
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      全ての要素を削除 ( デストラクタ呼び出し )
    //*/
    //---------------------------------------------------------------------
	void clear()
	{
		int pos = _m.Nodes[ _m.Capacity ].Next;
		while ( pos < _m.Capacity )
		{
			_m.Values[ pos ].~TYPE_();
			pos = _m.Nodes[ pos ].Next;
		}
		_m.Nodes[ _m.Capacity ].Next = _m.Capacity;
		_m.Nodes[ _m.Capacity ].Prev = _m.Capacity;
		_m.Size = 0;
		// スタック詰めなおし
		_m.IndexStack.clear();
		for ( int i = 0; i < _m.Capacity; ++i )
		{
			_m.IndexStack.push( i );
		}
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      要素数の取得
    //*/
    //---------------------------------------------------------------------
    u32 size() const
	{
        return _m.Size;
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      要素を末尾に追加する
    //*/
    //---------------------------------------------------------------------
    int push_back( const TYPE_& value_ )
	{
		if ( _m.Size < _m.Capacity )
		{
			// 空き場所を取得
			int new_pos = _m.IndexStack.top();
			_m.IndexStack.pop();

			// ダミーを取り出す
			Node* dummy = &_m.Nodes[ _m.Capacity ];

			// ダミーのひとつ前を取り出す
			Node* prev = &_m.Nodes[ dummy->Prev ];

			// new_pos のノードをリストにつなぐ
			Node* e = &_m.Nodes[ new_pos ];
			e->Prev = dummy->Prev;
			e->Next = _m.Capacity;
			dummy->Prev = new_pos;
			prev->Next = new_pos;

			++_m.Size;

			// コピーコンストラクタ呼び出し
			::new ( &_m.Values[ new_pos ] ) TYPE_( value_ );

			return new_pos;
		}

        LN_SETERR_R_S( LN_ERR_ASSERT, "< Core::Base::List::push_back() >\n\nこれ以上要素を追加できません。\nCapacity:%d", _m.Capacity );

		return -1;
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      先頭の要素の取得
    //*/
    //---------------------------------------------------------------------
    TYPE_& front() const
	{
        return _m.Values[ _m.Nodes[ _m.Capacity ].Next ];
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      先頭の要素の削除
    //*/
    //---------------------------------------------------------------------
    void pop_front()
	{
        pop( _m.Nodes[ _m.Capacity ].Next );
    }

    //---------------------------------------------------------------------
	// 指定位置の要素を削除
	//---------------------------------------------------------------------
	void pop( int index_ )
	{
		// 指定場所の要素をリストから外す
		Node* cur = &_m.Nodes[ index_ ];
		Node* next = &_m.Nodes[ cur->Next ];
		Node* prev = &_m.Nodes[ cur->Prev ];
		prev->Next = cur->Next;
		next->Prev = cur->Prev;

		// デストラクタ呼び出し
		_m.Values[ index_ ].~TYPE_();
		
		// スタックに戻す
		_m.IndexStack.push( index_ );
		--_m.Size;
	}

   

    //---------------------------------------------------------------------
    ///**
    //  @brief      先頭を示すイテレータの取得
    //*/
    //---------------------------------------------------------------------
    iterator begin() const
    {
        return iterator( _m.Values, _m.Nodes, _m.Nodes[ _m.Capacity ].Next );
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      終端を示すイテレータの取得
    //*/
    //---------------------------------------------------------------------
    iterator end() const
    {
        return iterator( _m.Values, _m.Nodes, _m.Capacity );
    }



private:

    struct
    {
        TYPE_*			Values;		///< 配列本体
	    Node*			Nodes;
        Stack< int >    IndexStack;
        int				Capacity;
	    int				Size;
    } _m;

    friend class iterator;
};


//=============================================================================
// ■ LowList クラス
//-----------------------------------------------------------------------------
///**
//  @brief      std::list が使えないときの代用品。最低限の機能しか持たせてありません。
//*/
//=============================================================================
template< typename TYPE_ > class LowList : public NonCopyable
{
private:

    struct Node
    {
        Node*   Prev;
        Node*   Next;
        TYPE_*  Value;
    };

public:

    /// コンストラクタ
    LowList()
        : mDummyNode    ( NULL )
        , mSize         ( 0 )
    {
        mDummyNode = LN_NEW Node;
        mDummyNode->Next = mDummyNode->Prev = mDummyNode;
    }

    /// デストラクタ
    ~LowList()
    {
        Node* n = mDummyNode->Next;
        Node* t;
        while ( n != mDummyNode )
        {
            t = n->Next;

            n->Value->~TYPE_();
            LN_OPERATOR_DELETE( n->Value );
            SAFE_DELETE( n );

            n = t;
        }
        SAFE_DELETE( mDummyNode );
    }

    /// イテレータ
    class iterator
    {
    public:

        iterator()
        {
            memset( &_m, 0, sizeof( _m ) );
        }

        iterator( const iterator& obj_ )
        {
            _m.Node = obj_._m.Node;
        }

        iterator& operator = ( const iterator& obj_ )
        {
            _m.Node = obj_._m.Node;
            return (*this);
        }

        //TYPE_* operator * ()
        TYPE_& operator * ()
        {
            return *_m.Node->Value;//static_cast< TYPE_* >( _m.Node->Value );
        }

        TYPE_* operator -> ()
        {
            return static_cast< TYPE_* >( _m.Node->Value );
        }

        iterator& operator ++ ()
        {
            _m.Node = _m.Node->Next;
            return *this;
        }

        iterator& operator -- ()
        {
            _m.Node = _m.Node->Prev;
            return *this;
        }

        bool operator == ( const iterator& obj_ ) const
        {
            return ( _m.Node == obj_._m.Node );
        }

        bool operator != ( const iterator& obj_ ) const
        {
            return ( _m.Node != obj_._m.Node );
        }

    private:

        iterator( Node* node_ )
        {
            _m.Node = node_;
        }

    private:

        struct
        {
	        Node*   Node;
        } _m;

        friend class LowList< TYPE_ >;
    };



    /// const イテレータ
    class const_iterator
    {
    public:

        const_iterator()
        {
            memset( &_m, 0, sizeof( _m ) );
        }

        const_iterator( const iterator& obj_ )
        {
            _m.Node = obj_._m.Node;
        }

        const_iterator& operator = ( const const_iterator& obj_ )
        {
            _m.Node = obj_._m.Node;
            return (*this);
        }

        TYPE_* operator * () const
        {
            return static_cast< TYPE_* >( _m.Node->Value );
        }

        TYPE_* operator -> () const
        {
            return static_cast< TYPE_* >( _m.Node->Value );
        }

        const_iterator& operator ++ ()
        {
            _m.Node = _m.Node->Next;
            return *this;
        }

        const_iterator& operator -- ()
        {
            _m.Node = _m.Node->Prev;
            return *this;
        }

        bool operator == ( const const_iterator& obj_ ) const
        {
            return ( _m.Node == obj_._m.Node );
        }

        bool operator != ( const const_iterator& obj_ ) const
        {
            return ( _m.Node != obj_._m.Node );
        }

    private:

        const_iterator( Node* node_ )
        {
            _m.Node = node_;
        }

    private:

        struct
        {
	        Node*   Node;
        } _m;

        friend class LowList< TYPE_ >;
    };

public:

    /// 追加されている要素数の取得
    u32 size() const
    {
        return mSize;
    }

    /// 要素の追加
    void push_back( const TYPE_& val_ )
    {
        Node* new_node = LN_NEW Node;
        new_node->Value = static_cast< TYPE_* >( LN_OPERATOR_NEW( sizeof( TYPE_ ) ) );
        new ( new_node->Value ) TYPE_( val_ );

        Node* last = mDummyNode->Prev;

        last->Next       = new_node;
        mDummyNode->Prev = new_node;
        new_node->Prev   = last;
        new_node->Next   = mDummyNode;

        ++mSize;
    }

    /// 先頭の要素の取得
    TYPE_& front() const
    {
        return *( mDummyNode->Next->Value );
    }

    /// 先頭の要素の削除
    void pop_front()
	{
        Node* old = mDummyNode->Next;
        Node* f = old->Next;

        f->Prev = mDummyNode;
        mDummyNode->Next = f;

        old->Value->~TYPE_();
        LN_OPERATOR_DELETE( old->Value );
        SAFE_DELETE( old );

        --mSize;
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      一致する要素を削除する ( 最初に見つかったひとつだけ )
    //*/
    //---------------------------------------------------------------------
    void remove( const TYPE_& val_ )
    {
        Node* n = mDummyNode->Next;
        Node* t;
        while ( n != mDummyNode )
        {
            t = n->Next;

            if ( *(n->Value) == val_ )
            {
                n->Value->~TYPE_();
                OPERATOR_DELETE( n->Value );
                SAFE_DELETE( n );
                return;
            }

            n = t;
        }
        //SAFE_DELETE( mDummyNode );
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      先頭を示すイテレータの取得
    //*/
    //---------------------------------------------------------------------
    iterator begin() { return iterator( mDummyNode->Next ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      終端を示すイテレータの取得
    //*/
    //---------------------------------------------------------------------
    iterator end() { return iterator( mDummyNode ); }

    const_iterator begin() const { return const_iterator( mDummyNode->Next ); }
    const_iterator end() const { return const_iterator( mDummyNode ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      イテレータが指す要素をリストから外す
    //*/
    //---------------------------------------------------------------------
    iterator erase( const iterator& it_ )
    {
        Node* n = it_._m.Node;

        Node* next = n->Next;

        // 前後をつなげる
        n->Prev->Next = n->Next;
        n->Next->Prev = n->Prev;

        // 削除
        n->Value->~TYPE_();
        LN_OPERATOR_DELETE( n->Value );
        SAFE_DELETE( n );

        --mSize;

        return iterator( next );
    }


private:

    Node*    mDummyNode;     ///< 終端を表すノード
    u32      mSize;
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