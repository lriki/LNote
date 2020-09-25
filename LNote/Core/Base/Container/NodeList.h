//=============================================================================
//【 NodeList 】
//-----------------------------------------------------------------------------
///**
//  @file       NodeList.h
//  @brief      NodeList
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
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
// ■ NodeList クラス
//-----------------------------------------------------------------------------
///**
//  @brief      
//
//  @par
//              TYPE_ にはポインタ型を指定しないようにしてください。
//              ただ、push_back() 等の追加・削除はポインタで行う点に注意。
//              (このへんわかりにくいと思うので後で変えるかも)
//
//  @note
//              実のところテンプレートにする必要は無かったりするけど、
//              イテレータ等で型の参照を返せるようにするためにテンプレートで実装。<br>
//              あと、複数のリストへの追加とか。
//
//              SceneObject みたいに、複数の親から継承させたクラスのリストとする場合、
//              push_back() 等の引数が Node* とかだと、C++ が気を利かせてくれる
//              なんかの仕様で、ポインタがずれる。
//              代わりに、Node* ではなく TYPE_* にして対応する。
//*/
//=============================================================================
template< class TYPE_ > class NodeList
    : public NonCopyable
{
public:

    /// NodeList に追加するためにはこれを継承する
    template< class NODE_LIST > 
    class _Node
    {
    public:

        /// コンストラクタ
	    _Node()
        {
            _m.Prev = _m.Next = NULL;
            _m.NodeList = NULL;
        }

        /// デストラクタ
	    virtual ~_Node()
        {
	        if ( _m.NodeList && _m.Prev && _m.Next )
	        {
		        _m.Prev->_m.Next = _m.Next;
		        _m.Next->_m.Prev = _m.Prev;
		        _m.Prev = _m.Next = NULL;
                _m.NodeList = NULL;
	        }
        }

    public:

        struct
        {
            TYPE_*      Prev;		///< ひとつ前のタスク
	        TYPE_*      Next;		///< ひとつ後ろのタスク
	        NODE_LIST*	NodeList;   ///< リストへのポインタ
        } _m;
    };

    typedef _Node< NodeList< TYPE_ > >Node;

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

        TYPE_* operator * ()
        {
            return static_cast< TYPE_* >( _m.Node );
        }

        iterator& operator ++ ()
        {
            _m.Node = _m.Node->_m.Next;
            return *this;
        }

        iterator operator ++ ( int )
        {
            iterator c( *this );
            _m.Node = _m.Node->_m.Next;
            return c;
        }

        iterator& operator -- ()
        {
            _m.Node = _m.Node->_m.Prev;
            return *this;
        }

        iterator operator -- ( int )
        {
            iterator c( *this );
            _m.Node = _m.Node->_m.Prev;
            return c;
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

        iterator( TYPE_* node_ )
        {
            _m.Node = node_;
        }

    private:

        struct _mb
        {
	        TYPE_*   Node;
        } _m;

        friend class NodeList< TYPE_ >;
    };

public:

    /// 比較関数の型
    typedef bool( *CompFunction )( const void*, const void* );

public:

    /// コンストラクタ
	NodeList();

    /// デストラクタ
	~NodeList();

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      要素数の取得
    //*/
    //---------------------------------------------------------------------
    int size() const { return _m.Size; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      ノードを末尾に追加する
    //*/
    //---------------------------------------------------------------------
    void push_back( TYPE_* node_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      ノードをリストから外す
    //*/
    //---------------------------------------------------------------------
    void remove( TYPE_* node_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      全てのノードをリストから外す
    //*/
    //---------------------------------------------------------------------
    void clear();

    //---------------------------------------------------------------------
    ///**
    //  @brief     先頭のノードの取得
    //*/
    //---------------------------------------------------------------------
    TYPE_* front() const { return static_cast< TYPE_* >( _m.Dummy->_m.Next ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief     先頭のノードををリストから外す
    //*/
    //---------------------------------------------------------------------
    void pop_front() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      先頭を示すイテレータの取得
    //*/
    //---------------------------------------------------------------------
    iterator begin() const { return iterator( _m.Dummy->_m.Next ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      終端を示すイテレータの取得
    //*/
    //---------------------------------------------------------------------
    iterator end() const { return iterator( _m.Dummy ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      イテレータが指す要素をリストから外す
    //*/
    //---------------------------------------------------------------------
    iterator erase( const iterator& it_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      リストをソートする
    //*/
	//---------------------------------------------------------------------
	void sort( CompFunction comp_function_ )
	{
		// 1個以下の場合はソートの必要ナシ
		if ( _m.Dummy->_m.Next == _m.Dummy || _m.Dummy->_m.Next->_m.Next == _m.Dummy ) { return; }

		// 比較関数設定
		_m.CompFunction = comp_function_;
		// ソート実行後、戻ってきたタスクを先頭にする
		_m.Dummy->_m.Next = _mergeSort( _m.Dummy->_m.Next );

		// ソートが終わったとき、後ろ方向 ( mNext ) は正しくつながってるけど
		// 前方向 ( mPrev ) はバラバラになっている。というわけでつなぎなおす。
		TYPE_* prev = _m.Dummy;
		TYPE_* node = _m.Dummy->_m.Next;	// 先頭(ATの次)
		for ( ; node != _m.Dummy; )
		{
			node->_m.Prev = prev;
			prev = node;
			node = node->_m.Next;
		}
		_m.Dummy->_m.Prev = prev;
	}

private:

    // マージソート実行
	TYPE_* _mergeSort( TYPE_* front_head )
	{
		// タスクの数が 0 または 1 個の場合はソートの必要ないので終了
		if ( front_head == _m.Dummy || front_head->_m.Next == _m.Dummy )
		{
			return front_head;
		}
		
		// a は先頭を指す
		TYPE_* a = front_head;

		// b は先頭のひとつ後ろを指した後、もうひとつ後ろがあればそれを指す
		TYPE_* b = front_head->_m.Next;
		if ( b != _m.Dummy )
		{
			b = b->_m.Next;
		}

		// 線形リストを中心くらいから半分に分けるため、中心位置がどこにあるのか探す
		while ( b != _m.Dummy )
		{
			// a はひとつずつ進む
			a = a->_m.Next;

			// b は可能な限りふたつずつ進む
			b = b->_m.Next;

			if ( b != _m.Dummy )
			{
				b = b->_m.Next;
			}
		}

		// a の次の要素をふたつめのリストの先頭として、リストを分割
		TYPE_* back_head = a->_m.Next;
		a->_m.Next = _m.Dummy;

		// 再帰でガンガン削ったあと、ソートしながら結合していく
		return _mergeList( _mergeSort( front_head ), _mergeSort( back_head ) );
	}

	// マージソート用・リスト結合
	TYPE_* _mergeList( TYPE_* left_node, TYPE_* right_node )
	{
		TYPE_* res_front = _m.Dummy;	// 結合後リストの先頭ノード
		TYPE_* res_back  = _m.Dummy;	// 結合後リストの終端ノード

		// ▼ 最初に先頭となる要素を決める

		// 両方のリストの中身がある
		if ( left_node != _m.Dummy && right_node != _m.Dummy )
		{
			// 先頭の要素同士を比較
			if( _m.CompFunction( left_node, right_node ) )
			{
				// 左リストからひとつ取り出して結合後リストに追加
				res_front = left_node;
				res_front->_m.Next = left_node->_m.Next;

				// 元の左リストの次の要素を指す
				left_node = left_node->_m.Next;
			}
			else
			{
				// 右リストからひとつ取り出して結合後リストに追加
				res_front = right_node;
				res_front->_m.Next = right_node->_m.Next;	// やってること同じだけど…?[9/11]

				// 元の右リストの次の要素を指す
				right_node = right_node->_m.Next;
			}
		}
		else
		{
			//LASSERT_S_( 1, LException::Fatal(), "リストのソート中の予期しないエラー\n( 高速化のために削ったところで落ちたかも。連絡お願いします。 )" );
		}
		// この部分は無くても同じかも
		//// 左側リストだけがある
		//else if ( left_node != mActiveTask )
		//{
		//	res_front = left_node;
		//	res_front->mNext = left_node;
		//	left_node = left_node->mNext;
		//}
		//// 右側リストだけがある
		//else
		//{
		//	res_front = right_node;
		//	res_front->mNext = right_node;
		//	right_node = right_node->mNext;
		//}
		//

		//if ( left_node == mActiveTask && right_node == mActiveTask )
		//{
		//	return res_front;
		//}


		// ▼ 先頭の要素が決まったので、ここから残りを比較しながら結合していく

		// とりあえず結合後リストの一番後ろも一番前を指しておく
		res_back = res_front;

		// 分割されている 2 つのリストのどちらかが空になるまでループ
		while( left_node != _m.Dummy && right_node != _m.Dummy )
		{
			// 先頭の要素同士を比較
			if( _m.CompFunction( left_node, right_node ) )
			{
				// 左リストからひとつ取り出して結合後リストに追加
				res_back->_m.Next = left_node;
				res_back = left_node;

				// 元の左リストの次の要素を指す
				left_node = left_node->_m.Next;
			}
			else
			{
				// 右リストからひとつ取り出して結合後リストに追加
				res_back->_m.Next = right_node;
				res_back = right_node;

				// 元の右リストの次の要素を指す
				right_node = right_node->_m.Next;
			}
		}
		
		// 先に片方のリストが空になるので、残された方のリストの要素を全部結合
		while ( left_node != _m.Dummy || right_node != _m.Dummy )
		{
			// 左がもう空っぽの場合
			if( left_node == _m.Dummy )
			{
				// 右に残っているものを詰めていく
				res_back->_m.Next = right_node;
				res_back = right_node;
				right_node = right_node->_m.Next;
			}
			// 右がもう空っぽの場合
			else
			{
				// 左に残っているものを詰めていく
				res_back->_m.Next = left_node;
				res_back = left_node;
				left_node = left_node->_m.Next;
			}
		}

		return res_front;
	}

private:

    template < typename COMP_FUNC >
    struct _mb
    {
        TYPE_*          Dummy;
	    COMP_FUNC    CompFunction;
        int             Size;
    };
    
    _mb< CompFunction > _m;
};

//-------------------------------------------------------------------------
// NodeList::Node

    /*
// コンストラクタ
template< class TYPE_ > inline NodeList< TYPE_ >::Node::Node()
{
    _m.Prev = _m.Next = NULL;
    _m.NodeList = NULL;
}

// デストラクタ
template< class TYPE_ > inline NodeList< TYPE_ >::Node::~Node()
{
	// リストから外す
	if ( _m.NodeList && _m.Prev && _m.Next )
	{
		_m.Prev->_m.Next = _m.Next;
		_m.Next->_m.Prev = _m.Prev;
		_m.Prev = _m.Next = NULL;
        _m.NodeList = NULL;
	}
}
    */

//-------------------------------------------------------------------------
// NodeList

// コンストラクタ
template< class TYPE_ > inline NodeList< TYPE_ >::NodeList()
{
	_m.Dummy = static_cast< TYPE_* >( LN_OPERATOR_NEW( sizeof( TYPE_ ) ) );
    //_m.Dummy = static_cast< TYPE_* >( LN_OPERATOR_NEW( sizeof( Node ) ) );    // これでもいいと思うけど、なんか不安なので
	_m.Dummy->_m.Prev = _m.Dummy;
	_m.Dummy->_m.Next = _m.Dummy;
    _m.CompFunction = NULL;
    _m.Size = 0;
}

// デストラクタ
template< class TYPE_ > inline NodeList< TYPE_ >::~NodeList()
{
    clear();
	LN_OPERATOR_DELETE( _m.Dummy );
}

// ノードを末尾に追加する
template< class TYPE_ > inline void NodeList< TYPE_ >::push_back( TYPE_* node_ )
{
	// 既に追加されている場合はなにもしない
	if ( !node_->_m.NodeList )
	{
		TYPE_* prev = _m.Dummy->_m.Prev;
		TYPE_* next = _m.Dummy;
		node_->_m.Prev = prev;
		node_->_m.Next = next;
		prev->_m.Next = next->_m.Prev = node_;
		node_->_m.NodeList = this;
        ++_m.Size;
	}
}

// ノードをリストから外す
template< class TYPE_ > inline void NodeList< TYPE_ >::remove( TYPE_* node_ )
{
	if ( node_ )
	{
		if ( node_->_m.NodeList )
		{
			node_->_m.Prev->_m.Next = node_->_m.Next;
			node_->_m.Next->_m.Prev = node_->_m.Prev;
			node_->_m.Prev = node_->_m.Next = NULL;
			node_->_m.NodeList = NULL;
            --_m.Size;
		}
	}
}

// 全てのノードをリストから外す
template< class TYPE_ > inline void NodeList< TYPE_ >::clear()
{
    TYPE_* n = _m.Dummy->_m.Next;
    TYPE_* t;
    while ( n != _m.Dummy )
    {
        t = n->_m.Next;
        n->_m.Prev = n->_m.Next = NULL;
        n->_m.NodeList = NULL;
        n = t;
    }
    _m.Dummy->_m.Prev = _m.Dummy;
	_m.Dummy->_m.Next = _m.Dummy;
    _m.Size = 0;
}

// 先頭のノードををリストから外す
template< class TYPE_ > inline void NodeList< TYPE_ >::pop_front() const
{
    TYPE_* n = _m.Dummy->_m.Next;
    if ( n != _m.Dummy )
    {
        // 前後をつなげる
        n->_m.Prev->_m.Next = n->_m.Next;
        n->_m.Next->_m.Prev = n->_m.Prev;
        // 外す
        n->_m.Prev = n->_m.Next = NULL;
        n->_m.NodeList = NULL;
        --_m.Size;
    }
}

// イテレータが指す要素をリストから外す
template< class TYPE_ > typename NodeList< TYPE_ >::iterator NodeList< TYPE_ >::erase( const iterator& it_ )
{
    TYPE_* n = it_._m.Node;
    TYPE_* next = n->_m.Next;
    // 前後をつなげる
    n->_m.Prev->_m.Next = n->_m.Next;
    n->_m.Next->_m.Prev = n->_m.Prev;
    // 外す
    n->_m.Prev = n->_m.Next = NULL;
    n->_m.NodeList = NULL;

    --_m.Size;
    return iterator( next );
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