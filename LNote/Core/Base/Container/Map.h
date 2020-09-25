//=============================================================================
//【 Map 】
//-----------------------------------------------------------------------------
///**
//  @file       Map.h
//  @brief      Map
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
// ■ Map クラス
//-----------------------------------------------------------------------------
///**
//  @brief
//
//  @par
//              同一キーのものは追加できません。
//*/
//=============================================================================
template< class KEY_, class TYPE_ > class Map
    : public NonCopyable
{
public:

    class iterator
    {
    public:

        class pair
        {
        public:

            pair( KEY_& k_, TYPE_& v_ )
                : first ( k_ )
                , second( v_ )
            {}

            KEY_&    first;
            TYPE_&   second;
        };



        iterator()
            : _pa( _m.Keys[ 0 ], _m.Values[ 0 ] )
        {
            memset( &_m, 0, sizeof( _m ) );
        }

        iterator( const iterator& obj_ )
            : _pa( obj_._m.Keys[ obj_._m.Pos ], obj_._m.Values[ obj_._m.Pos ] )
        {
            _m.Keys = obj_._m.Keys;
            _m.Values = obj_._m.Values;
            _m.Pos = obj_._m.Pos;
        }

        iterator& operator = ( const iterator& obj_ )
        {
            _m.Keys = obj_._m.Keys;
            _m.Values = obj_._m.Values;
            _m.Pos = obj_._m.Pos;

            new ( &_pa ) pair( _m.Keys[ _m.Pos ], _m.Values[ _m.Pos ] );

            return (*this);
        }

        //TYPE_& operator * () const
        //{
        //    return _m.Values[ _m.Pos ];
        //}

        //TYPE_& second() const
        //{
        //    return _m.Values[ _m.Pos ];
        //}

        pair* operator -> ()
        {
            //new ( &_m.Pair ) pair( _m.Keys[ _m.Pos ], _m.Values[ _m.Pos ] );
            return &_pa;//pair( &_m.Keys[ _m.Pos ], &_m.Values[ _m.Pos ] );//&_m.Pair;
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

        iterator( KEY_* keys_, TYPE_* values_, int pos_ )
            : _pa( keys_[ pos_ ], values_[ pos_ ] )
        {
            _m.Keys = keys_;
            _m.Values = values_;
            _m.Pos = pos_;
        }

    private:

        struct
        {
            KEY_*   Keys;
            TYPE_*  Values;
            int     Pos;
        } _m;

        pair    _pa;

        friend class Map< KEY_, TYPE_ >;
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
	Map()
        : mNexts        ( NULL )
        , mKeys         ( NULL )
        , mValues       ( NULL )
        , mCapacity     ( 0 )
        , mSize         ( 0 )
        , mTableSize    ( 0 )
    {}

	//---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ
    //
    //  @param[in]  capacity_ : 格納できる要素数
    //*/
    //---------------------------------------------------------------------
	explicit Map( int capacity_, int table_size_ = 0 )
        : mNexts        ( NULL )
        , mKeys         ( NULL )
        , mValues       ( NULL )
        , mCapacity     ( 0 )
        , mSize         ( 0 )
        , mTableSize    ( 0 )
    {
        setCapacity( capacity_, table_size_ );
    }

    /// デストラクタ
	~Map()
    {
        if ( mNexts )
        {
		    int pos = mNexts[ mCapacity ];
            // ノードがある限りループ
		    while ( pos < mCapacity + mTableSize )
            {
			    if ( pos < mCapacity )
                {
				    mKeys[ pos ].~KEY_();
				    mValues[ pos ].~TYPE_();
			    }
			    pos = mNexts[ pos ];
		    }
		    LN_OPERATOR_DELETE( mKeys );
		    LN_OPERATOR_DELETE( mValues );
		    SAFE_DELETE_ARRAY( mNexts );
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
    void setCapacity( int capacity_, int table_size_ = 0 )
    {
	    // まずはクリア
	    if ( mCapacity > 0 )
        {
		    mIndexStack.clear();
		    clear();
		    LN_OPERATOR_DELETE( mValues );
		    LN_OPERATOR_DELETE( mKeys );
		    SAFE_DELETE_ARRAY( mNexts );
	    }

	    mCapacity = capacity_;
	    if ( capacity_ <= 0 )
        {
		    return;
	    }

        // table_size_ が 0 の場合は自動で capacity_ 以上の素数を設定
	    if ( table_size_ == 0 )
        { 
		    table_size_ = capacity_;
	    }

	    table_size_ = PrimeNumber::next( table_size_ );
	    if ( table_size_ < 3 )
        {
		    table_size_ = 3; // 最低で3
	    }
	    mTableSize = table_size_;

	    // 次の要素を示す配列 (テーブルサイズ分だけダミーを足すことに注意)
        // 要素数は、例えば mCapacity が 5 だったら mTableSize も 5 で、全体で 10 になる
	    mNexts = LN_NEW int[ mCapacity + mTableSize ];

	    // 値配列確保
	    mKeys = static_cast< KEY_* >( OPERATOR_NEW( sizeof( KEY_ ) * mCapacity ) );
	    mValues = static_cast< TYPE_* >( OPERATOR_NEW( sizeof( TYPE_ ) * mCapacity ) );

	    // 空き番号スタック
	    mIndexStack.setCapacity( mCapacity );
	    for ( int i = 0; i < mCapacity; ++i )
        {
		    mIndexStack.push( i ); // 空き番号リストに詰めていく
	    }
	    // mCapacity+0からmCapacity+mTableSize-1番はダミー要素。各列の先頭の前に位置する
	    for ( int i = 0; i < mTableSize; ++i )
        {
		    mNexts[ mCapacity + i ] = mCapacity + i + 1;    // 自分の位置の次を指す
	    }
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      全ての要素を削除 ( デストラクタ呼び出し )
    //*/
    //---------------------------------------------------------------------
    void clear()
    {
	    int prev = mCapacity;           // ヘッド
	    int p = mNexts[ mCapacity ];    // ダミーヘッド (mNexts[ mCapacity ] は先頭要素のインデックス)
        // mCapacity + mTableSizeは存在しない。そこまではダミーとして存在する
	    while ( p < mCapacity + mTableSize )
        {  
		    if ( p < mCapacity ){
			    mNexts[ prev ] = mNexts[ p ]; // つなぎ変えて
			    mKeys[ p ].~KEY_();
			    mValues[ p ].~TYPE_();
			    mIndexStack.push( p );  // 空きスタックにプッシュ
		    }
		    prev = p;
		    p = mNexts[ p ];
	    }
	    mSize = 0;
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      追加されている要素数の取得
    //*/
    //---------------------------------------------------------------------
    int size() const
    {
        return mSize;
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      要素を追加する
    //
    //  @retval     -1 : 追加できなかった (キーの重複か、サイズオーバー)
    //
    //  @par
    //              同じキーのが既に追加されている場合は何もしません。
    //*/
    //---------------------------------------------------------------------
    int insert( const KEY_& k, const TYPE_& v )
    {
        if ( mSize >= mCapacity )
        {
            return -1;
        }

	    // ハッシュ関数算出
	    //int h = H().value( k, mTableSize );
        int h = GenericHash< KEY_ >().value( k, mTableSize );
	    //検索
	    int p = mNexts[ mCapacity + h ];
	    while ( p < mCapacity )
        {
		    //if ( H().isEqual( mKeys[ p ], k ) )
            if ( GenericHash< KEY_ >().isEqual( mKeys[ p ], k ) )
            {
			    break;
		    }
		    p = mNexts[ p ];
	    }

        // 検索で見つからなかったので新しく追加する
	    if ( p >= mCapacity )
        {
		    // 空き場所を取得
		    int new_pos = mIndexStack.top();
		    mIndexStack.pop();

		    // テーブル先頭に挿入
		    int head = mCapacity + h;
		    mNexts[ new_pos ] = mNexts[ head ]; // 頭ダミーの次をセット
		    new ( &mKeys[ new_pos ] ) KEY_( k );
		    new ( &mValues[ new_pos ] ) TYPE_( v );
		    mNexts[ head ] = new_pos; // 前のリンクを新ノードにつなぐ
		    ++mSize;
		    return new_pos;
	    }
        // 既に追加されていた
        else
        {
		    return -1;//mCapacity + mTableSize;
	    }
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      キーに一致する要素を検索する
    //*/
    //---------------------------------------------------------------------
    iterator find( const KEY_& k_ ) const
    {
	    //int h = H().value( k, mTableSize );
        int h = GenericHash< KEY_ >().value( k_, mTableSize );

	    int p = mNexts[ mCapacity + h ];
	    while ( p < mCapacity )
        {
		    if ( GenericHash< KEY_ >().isEqual( mKeys[ p ], k_ ) )
            {
			    return iterator( mKeys, mValues, p );
		    }
		    p = mNexts[ p ];
	    }
        // 見つからなかった場合は末尾を示すイテレータを返す
	    return end();
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      終端を示すイテレータを返す
    //*/
    //---------------------------------------------------------------------
    iterator end() const
    {
        return iterator( mKeys, mValues, mCapacity + mTableSize );
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      要素を削除する
    //*/
    //---------------------------------------------------------------------
    void erase( iterator it_ )
    {
	    int h = it_._m.Pos;
	    while ( h < mCapacity )
        {
		    h = mNexts[ h ];
	    }
	    h -= mCapacity;
	    h -= 1;

        int prev = mCapacity + h;
	    int p = mNexts[ prev ];
	    while ( p < mCapacity )
        {
		    if ( p == it_._m.Pos )
            {
			    mNexts[ prev ] = mNexts[ p ];
			    mKeys[ p ].~KEY_();
			    mValues[ p ].~TYPE_();
			    mIndexStack.push( p );
			    --mSize;
			    return;// true;
		    }
		    prev = p;
		    p = mNexts[ p ];
	    }
	    return;// false;
    }

public:

    int*            mNexts;
	KEY_*           mKeys;
	TYPE_*          mValues;
	Stack< int >    mIndexStack;
	int             mCapacity;
	int             mSize;
	int             mTableSize;
};


//=============================================================================
// ■ LLRBMap クラス
//-----------------------------------------------------------------------------
///**
//  @brief      LLRBツリー(Left-Leaning Red-Black Tree) を使った map
//*/
//=============================================================================
template < typename KEY_, typename TYPE_ > class LLRBMap
{
public:

    /// コンストラクタ
    LLRBMap()
        : mRoot ( NULL )
        , mSize ( 0 )
    {
        mDummy = static_cast< Node* >( LN_OPERATOR_NEW( sizeof( Node ) ) );
    }

    /// デストラクタ
    ~LLRBMap()
    {
        clear();
        LN_OPERATOR_DELETE( mDummy );
    }

    class iterator;

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      空かどうかを判定する
    //*/
    //---------------------------------------------------------------------
    bool empty() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      追加されている要素数の取得
    //*/
    //---------------------------------------------------------------------
    int size() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      すべての要素を削除する
    //*/
    //---------------------------------------------------------------------
    void clear();
    
    //---------------------------------------------------------------------
    ///**
    //  @brief      要素を追加する
    //
    //  @par
    //              同じキーのが既に追加されている場合は値を上書きします。( 変更するかも )
    //*/
    //---------------------------------------------------------------------
    void insert( const KEY_& key_, const TYPE_& value_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      キーに一致する要素を検索する
    //*/
    //---------------------------------------------------------------------
    iterator find( const KEY_& key_ ) const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      要素を削除する
    //*/
    //---------------------------------------------------------------------
    void erase( const KEY_& key_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      キーに対応する値が存在するかを判定する
    //*/
    //---------------------------------------------------------------------
    bool containsKey( const KEY_& key_ ) const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      終端を示すイテレータを返す
    //*/
    //---------------------------------------------------------------------
    iterator end();

private:

    struct Node;

public:

    /// イテレータ
    class iterator
    {
    public:

        class pair
        {
        public:

            pair( KEY_& k_, TYPE_& v_ )
                : first ( k_ )
                , second( v_ )
            {}

            KEY_&    first;
            TYPE_&   second;
        };



        iterator()
            : _n    ( NULL )
            , _pa   ( _n->Key, _n->Value )
        {
        }

        iterator( const iterator& obj_ )
            : _n    ( obj_._n )
            , _pa   ( obj_._n->Key, obj_._n->Value )
        {
        }

        iterator& operator = ( const iterator& obj_ )
        {
            _n = obj_._n;
            new ( &_pa ) pair( _n->Key, _n->Value );//, obj_._n->Value );

            return (*this);
        }

        //TYPE_& operator * () const
        //{
        //    return _m.Values[ _m.Pos ];
        //}

        //TYPE_& second() const
        //{
        //    return _m.Values[ _m.Pos ];
        //}



        pair* operator -> ()
        {
            return &_pa;
        }

        iterator& operator ++ ()
        {
            //_m.Pos = _m.Nodes[ _m.Pos ].Next;
            Node*   _n;
            
            //if ( _n->Left ) { _n = _n->Left };

            return *this;
        }

        //iterator operator ++ ( int )
        //{
        //    iterator c( *this );
        //    _m.Pos = _m.Nodes[ _m.Pos ].Next;
        //    return c;
        //}

        iterator& operator -- ()
        {
            //_m.Pos = _m.Nodes[ _m.Pos ].Prev;
            return *this;
        }

        //iterator operator -- ( int )
        //{
        //    iterator c( *this );
        //    _m.Pos = _m.Nodes[ _m.Pos ].Prev;
        //    return c;
        //}

        bool operator == ( const iterator& obj_ ) const
        {
            return ( _n == obj_._n );
        }

        bool operator != ( const iterator& obj_ ) const
        {
            return ( _n != obj_._n );
        }

    private:

        iterator( Node* node_ )
            : _n    ( node_ )
            , _pa   ( node_->Key, node_->Value )
        { }

    private:

        Node*   _n;
        pair    _pa;

        friend class LLRBMap< KEY_, TYPE_ >;
    };

private:

    struct Node
    {
        KEY_    Key;
        TYPE_   Value;
        Node*   Left;
        Node*   Right;
        bool    Color;

        /// コンストラクタ
        Node( const KEY_& key_, const TYPE_& val_ )
            : Key   ( key_ )
            , Value ( val_ )
            , Left  ( NULL )
            , Right ( NULL )
            , Color ( RED )
        {}
    };



    

private:

    static const bool RED = true;
    static const bool BLACK = false;

    Node*   mDummy;     ///< イテレータで終端を示すダミーノード
    Node*   mRoot;
    int     mSize;

private:

    /// 追加
    Node* _put( Node* h_, const KEY_& key_, const TYPE_& value_ )
    {
        // ノードが無ければ新しく作って返す
        if ( !h_ )
        {
            return LN_NEW Node( key_, value_ );
        }

        // h_ のキーと一致していれば、それに新しい値を代入する ( 必要に応じてカットしておくこと )
        if ( key_ == h_->Key )
        {
            h_->Value = value_;
        }
        // key_ ( 新しく追加する方 ) が小さければ左 
        else if ( key_ < h_->Key )
        {
            h_->Left = _put( h_->Left, key_, value_ );
        }
        // それ以外 ( 新しく追加する方が大きい ) は右
        else
        {
            h_->Right = _put( h_->Right, key_, value_ );
        }

        // 子の状態に応じて回転・入れ替え操作を行う
        if ( _isRed( h_->Right ) && !_isRed( h_->Left ) )       { h_ = _rotateLeft( h_ ); }
        if ( _isRed( h_->Left )  && _isRed( h_->Left->Left ) )  { h_ = _rotateRight( h_ ); }
        if ( _isRed( h_->Left )  && _isRed( h_->Right ) )       { _flipColors( h_ ); }

        return h_;
    }

    /// ノードが赤か判定
    bool _isRed( const Node* node_ ) const
    {
        if ( !node_ ) { return false; }
        return ( node_->Color == RED );
    }

    /// 左回転
    Node* _rotateLeft( Node* h_ )
    {
        Node* x = h_->Right;
        h_->Right = x->Left;
        x->Left  = h_;
        x->Color = h_->Color;
        h_->Color = RED;
        return x;
    }

    /// 右回転
    Node* _rotateRight( Node* h_ )
    {
        Node* x = h_->Left;
        h_->Left = x->Right;
        x->Right = h_;
        x->Color = h_->Color;
        h_->Color = RED;
        return x;
    }

    /// 色の入れ替え
    void _flipColors( Node* h_ )
    {
        h_->Color = !h_->Color;
        h_->Left->Color = !h_->Left->Color;
        h_->Right->Color = !h_->Right->Color;
    }

    /// 検索
    //TYPE_* _get( Node* x_, const KEY_& key_ ) const
    Node* _get( Node* x_, const KEY_& key_ ) const
    {
        while( x_ )
        {
            // 見つかった
            if ( key_ == x_->Key )
            {
                return x_;//&x_->Value;
            }
            // key_ が小さければ左へ
            else if ( key_ < x_->Key )
            {
                x_ = x_->Left;
            }
            // key_ が大きければ右へ
            else
            {
                x_ = x_->Right;
            }
        }
        return NULL;
    }


    /// 削除
    Node* _erase( Node* h_, const KEY_& key_ )
    {
        if ( key_ < h_->Key )
        {
            // 左側が 黒-NULL
            if ( !_isRed( h_->Left ) && !_isRed( h_->Left->Left ) ) // 要素 1 でキー一致しない削除しようとしたらここで止まった
            {
                h_ = _moveRedLeft( h_ );
            }
            h_->Left = _erase( h_->Left, key_ );
        }
        else
        {
            if ( _isRed( h_->Left ) )
            {
                h_ = _rotateRight( h_ );
            }

            // 一致して、右側が無かった場合 (葉) は NULL
            if ( key_ == h_->Key && !h_->Right )
            {
                delete h_;
                return NULL;
            }

            if ( !_isRed( h_->Right ) && !_isRed( h_->Right->Left ) )
            {
                h_ = _moveRedRight( h_ );
            }

            // 一致 ( ここでは右側の子がある )
            if ( key_ == h_->Key )
            {
                // h_ 以下の一番小さい値のキーを探す
                KEY_* min_key = &_min( h_->Right )->Key;

                // ノードを削除し、そのノードの位置に右の子を持ってくる
                h_->Value = _get( h_->Right, *min_key )->Value;
                h_->Key = *min_key;

                h_->Right = _deleteMin( h_->Right );

            }
            else
            {
                h_->Right = _erase( h_->Right, key_ );
            }
        }
        return _fixUp( h_ );
    }

    /// h_ 以下の一番小さいノードのキーを探す
    Node* _min( Node* h_ )
    {
        if ( !h_->Left ) { return h_; }
        return _min( h_->Left );
    }


    Node* _deleteMin( Node* h_ )
    {
        if ( !h_->Left )
        {
            delete h_;
            return NULL;
        }

        if ( !_isRed( h_->Left ) && !_isRed( h_->Left->Left ) )
        {
            h_ = _moveRedLeft( h_ );
        }
        h_->Left = _deleteMin( h_->Left );
        return _fixUp( h_ );
    }

   
    Node* _moveRedLeft( Node* h_ )
    {
        _flipColors( h_ );

        if ( _isRed( h_->Right->Left ) ) 
        {
            h_->Right = _rotateRight( h_->Right );
            h_ = _rotateLeft( h_ );
            _flipColors( h_ );
        }
        return h_;
    }

    Node* _moveRedRight( Node* h_ )
    {
        _flipColors( h_ );
        if ( _isRed( h_->Left->Left ) )
        {
            h_ = _rotateRight( h_ );
            _flipColors( h_ );
        }
        return h_;
    }

    Node* _fixUp( Node* h_ )
    {
        if ( _isRed( h_->Right ) )
        {
            h_ = _rotateLeft( h_ );
        }
        if ( _isRed( h_->Left ) && _isRed( h_->Left->Left ) )
        {
            h_ = _rotateRight( h_ );
        }
        if ( _isRed( h_->Left ) && _isRed( h_->Right ) )
        {
            _flipColors( h_ );
        }
        return h_;
    }

    /// 全て削除 ( ルートノードを渡す )
    void _clear( Node* node_ )
    {
        if ( node_->Left )
        {
            _clear( node_->Left );
        }
        if ( node_->Right )
        {
            _clear( node_->Right );
        }
        delete node_;
    }
};

//---------------------------------------------------------------------
// 空かどうかを判定する
template< typename KEY_, typename TYPE_ > inline bool LLRBMap< KEY_, TYPE_ >::empty() const
{
    return ( !mRoot );
}

// 追加されている要素数の取得
template< typename KEY_, typename TYPE_ > inline int LLRBMap< KEY_, TYPE_ >::size() const
{
    return mSize;
}

// すべての要素を削除する
template< typename KEY_, typename TYPE_ > inline void LLRBMap< KEY_, TYPE_ >::clear()
{
    if ( mRoot )
    {
        _clear( mRoot );
    }
}

// 要素を追加する
template< typename KEY_, typename TYPE_ > inline void LLRBMap< KEY_, TYPE_ >::insert( const KEY_& key_, const TYPE_& value_ )
{
    mRoot = _put( mRoot, key_, value_ );
    if ( mRoot )
    {
        mRoot->Color = BLACK;
    }
}

// キーに一致する要素を検索する
template< typename KEY_, typename TYPE_ > inline typename LLRBMap< KEY_, TYPE_ >::iterator LLRBMap< KEY_, TYPE_ >::find( const KEY_& key_ ) const
{
    if ( mRoot && mSize > 0 )
    {
        Node* n = _get( mRoot, key_ );
        return iterator( ( n ) ? n : mDummy );
    }
    return iterator( mDummy );
}

// 要素を削除する
template< typename KEY_, typename TYPE_ > inline void LLRBMap< KEY_, TYPE_ >::erase( const KEY_& key_ )
{
    if ( containsKey( key_ ) )
    {
        mRoot = _erase( mRoot, key_ );
        // 新しくルートになったものは黒にする
        if ( mRoot )
        {
            mRoot->Color = BLACK;
        }
    }
}

// キーに対応する値が存在するかを判定する
template< typename KEY_, typename TYPE_ > inline bool LLRBMap< KEY_, TYPE_ >::containsKey( const KEY_& key_ ) const
{
    return ( _get( mRoot, key_ ) != NULL );
}

// 終端を示すイテレータを返す
template< typename KEY_, typename TYPE_ > inline typename LLRBMap< KEY_, TYPE_ >::iterator LLRBMap< KEY_, TYPE_ >::end()
{
    return iterator( mDummy );
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