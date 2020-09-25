//=============================================================================
//�y Map �z
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
// �� Map �N���X
//-----------------------------------------------------------------------------
///**
//  @brief
//
//  @par
//              ����L�[�̂��̂͒ǉ��ł��܂���B
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
    //  @brief      �f�t�H���g�R���X�g���N�^
    //
    //  @par
    //              �f�t�H���g�R���X�g���N�^�ō쐬�����ꍇ��
    //              ���̂��Ƃ� setCapacity() ���Ă�ŗ̈���m�ۂ��Ă��������B
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
    //  @brief      �R���X�g���N�^
    //
    //  @param[in]  capacity_ : �i�[�ł���v�f��
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

    /// �f�X�g���N�^
	~Map()
    {
        if ( mNexts )
        {
		    int pos = mNexts[ mCapacity ];
            // �m�[�h��������胋�[�v
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
    //  @brief      �̈�̊m��
    //
    //  @param[in]  capacity_ : �i�[�ł���v�f��
    //*/
    //---------------------------------------------------------------------
    void setCapacity( int capacity_, int table_size_ = 0 )
    {
	    // �܂��̓N���A
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

        // table_size_ �� 0 �̏ꍇ�͎����� capacity_ �ȏ�̑f����ݒ�
	    if ( table_size_ == 0 )
        { 
		    table_size_ = capacity_;
	    }

	    table_size_ = PrimeNumber::next( table_size_ );
	    if ( table_size_ < 3 )
        {
		    table_size_ = 3; // �Œ��3
	    }
	    mTableSize = table_size_;

	    // ���̗v�f�������z�� (�e�[�u���T�C�Y�������_�~�[�𑫂����Ƃɒ���)
        // �v�f���́A�Ⴆ�� mCapacity �� 5 �������� mTableSize �� 5 �ŁA�S�̂� 10 �ɂȂ�
	    mNexts = LN_NEW int[ mCapacity + mTableSize ];

	    // �l�z��m��
	    mKeys = static_cast< KEY_* >( OPERATOR_NEW( sizeof( KEY_ ) * mCapacity ) );
	    mValues = static_cast< TYPE_* >( OPERATOR_NEW( sizeof( TYPE_ ) * mCapacity ) );

	    // �󂫔ԍ��X�^�b�N
	    mIndexStack.setCapacity( mCapacity );
	    for ( int i = 0; i < mCapacity; ++i )
        {
		    mIndexStack.push( i ); // �󂫔ԍ����X�g�ɋl�߂Ă���
	    }
	    // mCapacity+0����mCapacity+mTableSize-1�Ԃ̓_�~�[�v�f�B�e��̐擪�̑O�Ɉʒu����
	    for ( int i = 0; i < mTableSize; ++i )
        {
		    mNexts[ mCapacity + i ] = mCapacity + i + 1;    // �����̈ʒu�̎����w��
	    }
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �S�Ă̗v�f���폜 ( �f�X�g���N�^�Ăяo�� )
    //*/
    //---------------------------------------------------------------------
    void clear()
    {
	    int prev = mCapacity;           // �w�b�h
	    int p = mNexts[ mCapacity ];    // �_�~�[�w�b�h (mNexts[ mCapacity ] �͐擪�v�f�̃C���f�b�N�X)
        // mCapacity + mTableSize�͑��݂��Ȃ��B�����܂ł̓_�~�[�Ƃ��đ��݂���
	    while ( p < mCapacity + mTableSize )
        {  
		    if ( p < mCapacity ){
			    mNexts[ prev ] = mNexts[ p ]; // �Ȃ��ς���
			    mKeys[ p ].~KEY_();
			    mValues[ p ].~TYPE_();
			    mIndexStack.push( p );  // �󂫃X�^�b�N�Ƀv�b�V��
		    }
		    prev = p;
		    p = mNexts[ p ];
	    }
	    mSize = 0;
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �ǉ�����Ă���v�f���̎擾
    //*/
    //---------------------------------------------------------------------
    int size() const
    {
        return mSize;
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �v�f��ǉ�����
    //
    //  @retval     -1 : �ǉ��ł��Ȃ����� (�L�[�̏d�����A�T�C�Y�I�[�o�[)
    //
    //  @par
    //              �����L�[�̂����ɒǉ�����Ă���ꍇ�͉������܂���B
    //*/
    //---------------------------------------------------------------------
    int insert( const KEY_& k, const TYPE_& v )
    {
        if ( mSize >= mCapacity )
        {
            return -1;
        }

	    // �n�b�V���֐��Z�o
	    //int h = H().value( k, mTableSize );
        int h = GenericHash< KEY_ >().value( k, mTableSize );
	    //����
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

        // �����Ō�����Ȃ������̂ŐV�����ǉ�����
	    if ( p >= mCapacity )
        {
		    // �󂫏ꏊ���擾
		    int new_pos = mIndexStack.top();
		    mIndexStack.pop();

		    // �e�[�u���擪�ɑ}��
		    int head = mCapacity + h;
		    mNexts[ new_pos ] = mNexts[ head ]; // ���_�~�[�̎����Z�b�g
		    new ( &mKeys[ new_pos ] ) KEY_( k );
		    new ( &mValues[ new_pos ] ) TYPE_( v );
		    mNexts[ head ] = new_pos; // �O�̃����N��V�m�[�h�ɂȂ�
		    ++mSize;
		    return new_pos;
	    }
        // ���ɒǉ�����Ă���
        else
        {
		    return -1;//mCapacity + mTableSize;
	    }
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L�[�Ɉ�v����v�f����������
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
        // ������Ȃ������ꍇ�͖����������C�e���[�^��Ԃ�
	    return end();
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �I�[�������C�e���[�^��Ԃ�
    //*/
    //---------------------------------------------------------------------
    iterator end() const
    {
        return iterator( mKeys, mValues, mCapacity + mTableSize );
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �v�f���폜����
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
// �� LLRBMap �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      LLRB�c���[(Left-Leaning Red-Black Tree) ���g���� map
//*/
//=============================================================================
template < typename KEY_, typename TYPE_ > class LLRBMap
{
public:

    /// �R���X�g���N�^
    LLRBMap()
        : mRoot ( NULL )
        , mSize ( 0 )
    {
        mDummy = static_cast< Node* >( LN_OPERATOR_NEW( sizeof( Node ) ) );
    }

    /// �f�X�g���N�^
    ~LLRBMap()
    {
        clear();
        LN_OPERATOR_DELETE( mDummy );
    }

    class iterator;

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �󂩂ǂ����𔻒肷��
    //*/
    //---------------------------------------------------------------------
    bool empty() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �ǉ�����Ă���v�f���̎擾
    //*/
    //---------------------------------------------------------------------
    int size() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���ׂĂ̗v�f���폜����
    //*/
    //---------------------------------------------------------------------
    void clear();
    
    //---------------------------------------------------------------------
    ///**
    //  @brief      �v�f��ǉ�����
    //
    //  @par
    //              �����L�[�̂����ɒǉ�����Ă���ꍇ�͒l���㏑�����܂��B( �ύX���邩�� )
    //*/
    //---------------------------------------------------------------------
    void insert( const KEY_& key_, const TYPE_& value_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L�[�Ɉ�v����v�f����������
    //*/
    //---------------------------------------------------------------------
    iterator find( const KEY_& key_ ) const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �v�f���폜����
    //*/
    //---------------------------------------------------------------------
    void erase( const KEY_& key_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L�[�ɑΉ�����l�����݂��邩�𔻒肷��
    //*/
    //---------------------------------------------------------------------
    bool containsKey( const KEY_& key_ ) const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �I�[�������C�e���[�^��Ԃ�
    //*/
    //---------------------------------------------------------------------
    iterator end();

private:

    struct Node;

public:

    /// �C�e���[�^
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

        /// �R���X�g���N�^
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

    Node*   mDummy;     ///< �C�e���[�^�ŏI�[�������_�~�[�m�[�h
    Node*   mRoot;
    int     mSize;

private:

    /// �ǉ�
    Node* _put( Node* h_, const KEY_& key_, const TYPE_& value_ )
    {
        // �m�[�h��������ΐV��������ĕԂ�
        if ( !h_ )
        {
            return LN_NEW Node( key_, value_ );
        }

        // h_ �̃L�[�ƈ�v���Ă���΁A����ɐV�����l�������� ( �K�v�ɉ����ăJ�b�g���Ă������� )
        if ( key_ == h_->Key )
        {
            h_->Value = value_;
        }
        // key_ ( �V�����ǉ������ ) ����������΍� 
        else if ( key_ < h_->Key )
        {
            h_->Left = _put( h_->Left, key_, value_ );
        }
        // ����ȊO ( �V�����ǉ���������傫�� ) �͉E
        else
        {
            h_->Right = _put( h_->Right, key_, value_ );
        }

        // �q�̏�Ԃɉ����ĉ�]�E����ւ�������s��
        if ( _isRed( h_->Right ) && !_isRed( h_->Left ) )       { h_ = _rotateLeft( h_ ); }
        if ( _isRed( h_->Left )  && _isRed( h_->Left->Left ) )  { h_ = _rotateRight( h_ ); }
        if ( _isRed( h_->Left )  && _isRed( h_->Right ) )       { _flipColors( h_ ); }

        return h_;
    }

    /// �m�[�h���Ԃ�����
    bool _isRed( const Node* node_ ) const
    {
        if ( !node_ ) { return false; }
        return ( node_->Color == RED );
    }

    /// ����]
    Node* _rotateLeft( Node* h_ )
    {
        Node* x = h_->Right;
        h_->Right = x->Left;
        x->Left  = h_;
        x->Color = h_->Color;
        h_->Color = RED;
        return x;
    }

    /// �E��]
    Node* _rotateRight( Node* h_ )
    {
        Node* x = h_->Left;
        h_->Left = x->Right;
        x->Right = h_;
        x->Color = h_->Color;
        h_->Color = RED;
        return x;
    }

    /// �F�̓���ւ�
    void _flipColors( Node* h_ )
    {
        h_->Color = !h_->Color;
        h_->Left->Color = !h_->Left->Color;
        h_->Right->Color = !h_->Right->Color;
    }

    /// ����
    //TYPE_* _get( Node* x_, const KEY_& key_ ) const
    Node* _get( Node* x_, const KEY_& key_ ) const
    {
        while( x_ )
        {
            // ��������
            if ( key_ == x_->Key )
            {
                return x_;//&x_->Value;
            }
            // key_ ����������΍���
            else if ( key_ < x_->Key )
            {
                x_ = x_->Left;
            }
            // key_ ���傫����ΉE��
            else
            {
                x_ = x_->Right;
            }
        }
        return NULL;
    }


    /// �폜
    Node* _erase( Node* h_, const KEY_& key_ )
    {
        if ( key_ < h_->Key )
        {
            // ������ ��-NULL
            if ( !_isRed( h_->Left ) && !_isRed( h_->Left->Left ) ) // �v�f 1 �ŃL�[��v���Ȃ��폜���悤�Ƃ����炱���Ŏ~�܂���
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

            // ��v���āA�E�������������ꍇ (�t) �� NULL
            if ( key_ == h_->Key && !h_->Right )
            {
                delete h_;
                return NULL;
            }

            if ( !_isRed( h_->Right ) && !_isRed( h_->Right->Left ) )
            {
                h_ = _moveRedRight( h_ );
            }

            // ��v ( �����ł͉E���̎q������ )
            if ( key_ == h_->Key )
            {
                // h_ �ȉ��̈�ԏ������l�̃L�[��T��
                KEY_* min_key = &_min( h_->Right )->Key;

                // �m�[�h���폜���A���̃m�[�h�̈ʒu�ɉE�̎q�������Ă���
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

    /// h_ �ȉ��̈�ԏ������m�[�h�̃L�[��T��
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

    /// �S�č폜 ( ���[�g�m�[�h��n�� )
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
// �󂩂ǂ����𔻒肷��
template< typename KEY_, typename TYPE_ > inline bool LLRBMap< KEY_, TYPE_ >::empty() const
{
    return ( !mRoot );
}

// �ǉ�����Ă���v�f���̎擾
template< typename KEY_, typename TYPE_ > inline int LLRBMap< KEY_, TYPE_ >::size() const
{
    return mSize;
}

// ���ׂĂ̗v�f���폜����
template< typename KEY_, typename TYPE_ > inline void LLRBMap< KEY_, TYPE_ >::clear()
{
    if ( mRoot )
    {
        _clear( mRoot );
    }
}

// �v�f��ǉ�����
template< typename KEY_, typename TYPE_ > inline void LLRBMap< KEY_, TYPE_ >::insert( const KEY_& key_, const TYPE_& value_ )
{
    mRoot = _put( mRoot, key_, value_ );
    if ( mRoot )
    {
        mRoot->Color = BLACK;
    }
}

// �L�[�Ɉ�v����v�f����������
template< typename KEY_, typename TYPE_ > inline typename LLRBMap< KEY_, TYPE_ >::iterator LLRBMap< KEY_, TYPE_ >::find( const KEY_& key_ ) const
{
    if ( mRoot && mSize > 0 )
    {
        Node* n = _get( mRoot, key_ );
        return iterator( ( n ) ? n : mDummy );
    }
    return iterator( mDummy );
}

// �v�f���폜����
template< typename KEY_, typename TYPE_ > inline void LLRBMap< KEY_, TYPE_ >::erase( const KEY_& key_ )
{
    if ( containsKey( key_ ) )
    {
        mRoot = _erase( mRoot, key_ );
        // �V�������[�g�ɂȂ������͍̂��ɂ���
        if ( mRoot )
        {
            mRoot->Color = BLACK;
        }
    }
}

// �L�[�ɑΉ�����l�����݂��邩�𔻒肷��
template< typename KEY_, typename TYPE_ > inline bool LLRBMap< KEY_, TYPE_ >::containsKey( const KEY_& key_ ) const
{
    return ( _get( mRoot, key_ ) != NULL );
}

// �I�[�������C�e���[�^��Ԃ�
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