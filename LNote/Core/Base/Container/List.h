//=============================================================================
//�y List �z
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
// �� List �N���X
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

    /// �C�e���[�^
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

        TYPE_& operator * ()    // const �͂��Ȃ�
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
            TYPE_*  Values;		///< �z��{��
	        Node*   Nodes;
            int     Pos;
        } _m;

        friend class List< TYPE_ >;
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
    List()
    {
        _m.Values = NULL;
        _m.Nodes = NULL;
        _m.Capacity = 0;
        _m.Size = 0;
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���X�g���N�^
    //
    //  @param[in]  capacity_ : �i�[�ł���v�f��
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

    /// �f�X�g���N�^
	~List()
	{
		if ( _m.Nodes )
		{
			int pos = _m.Nodes[ _m.Capacity ].Next;
			// �_�~�[�łȂ����胋�[�v
			while ( pos < _m.Capacity )
			{
				// �f�X�g���N�^�Ăяo��
				_m.Values[ pos ].~TYPE_();
				// ���̗v�f��
				pos = _m.Nodes[ pos ].Next;
			}
			LN_OPERATOR_DELETE( _m.Values );
			SAFE_DELETE_ARRAY( _m.Nodes );
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
	void setCapacity( int capacity_ )
	{
		// �܂��̓N���A
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

		// �K�v�ȕ��̃������m��
		_m.Values = static_cast< TYPE_* >( LN_OPERATOR_NEW( sizeof( TYPE_ ) * _m.Capacity ) );


		// �󂫔ԍ��X�^�b�N���쐬���Ĕԍ���S���l�ߍ���
        _m.IndexStack.setCapacity( _m.Capacity );
		for ( int i = 0; i < _m.Capacity; ++i )
		{
			_m.IndexStack.push( i );
		}

		// mCapacity �Ԃ͍ŏ��̗v�f�̑O���Ō�̗v�f�̌�ɂ���_�~�[�v�f
		_m.Nodes[ _m.Capacity ].Next = _m.Nodes[ _m.Capacity ].Prev = _m.Capacity;
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      �S�Ă̗v�f���폜 ( �f�X�g���N�^�Ăяo�� )
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
		// �X�^�b�N�l�߂Ȃ���
		_m.IndexStack.clear();
		for ( int i = 0; i < _m.Capacity; ++i )
		{
			_m.IndexStack.push( i );
		}
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      �v�f���̎擾
    //*/
    //---------------------------------------------------------------------
    u32 size() const
	{
        return _m.Size;
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �v�f�𖖔��ɒǉ�����
    //*/
    //---------------------------------------------------------------------
    int push_back( const TYPE_& value_ )
	{
		if ( _m.Size < _m.Capacity )
		{
			// �󂫏ꏊ���擾
			int new_pos = _m.IndexStack.top();
			_m.IndexStack.pop();

			// �_�~�[�����o��
			Node* dummy = &_m.Nodes[ _m.Capacity ];

			// �_�~�[�̂ЂƂO�����o��
			Node* prev = &_m.Nodes[ dummy->Prev ];

			// new_pos �̃m�[�h�����X�g�ɂȂ�
			Node* e = &_m.Nodes[ new_pos ];
			e->Prev = dummy->Prev;
			e->Next = _m.Capacity;
			dummy->Prev = new_pos;
			prev->Next = new_pos;

			++_m.Size;

			// �R�s�[�R���X�g���N�^�Ăяo��
			::new ( &_m.Values[ new_pos ] ) TYPE_( value_ );

			return new_pos;
		}

        LN_SETERR_R_S( LN_ERR_ASSERT, "< Core::Base::List::push_back() >\n\n����ȏ�v�f��ǉ��ł��܂���B\nCapacity:%d", _m.Capacity );

		return -1;
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      �擪�̗v�f�̎擾
    //*/
    //---------------------------------------------------------------------
    TYPE_& front() const
	{
        return _m.Values[ _m.Nodes[ _m.Capacity ].Next ];
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �擪�̗v�f�̍폜
    //*/
    //---------------------------------------------------------------------
    void pop_front()
	{
        pop( _m.Nodes[ _m.Capacity ].Next );
    }

    //---------------------------------------------------------------------
	// �w��ʒu�̗v�f���폜
	//---------------------------------------------------------------------
	void pop( int index_ )
	{
		// �w��ꏊ�̗v�f�����X�g����O��
		Node* cur = &_m.Nodes[ index_ ];
		Node* next = &_m.Nodes[ cur->Next ];
		Node* prev = &_m.Nodes[ cur->Prev ];
		prev->Next = cur->Next;
		next->Prev = cur->Prev;

		// �f�X�g���N�^�Ăяo��
		_m.Values[ index_ ].~TYPE_();
		
		// �X�^�b�N�ɖ߂�
		_m.IndexStack.push( index_ );
		--_m.Size;
	}

   

    //---------------------------------------------------------------------
    ///**
    //  @brief      �擪�������C�e���[�^�̎擾
    //*/
    //---------------------------------------------------------------------
    iterator begin() const
    {
        return iterator( _m.Values, _m.Nodes, _m.Nodes[ _m.Capacity ].Next );
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �I�[�������C�e���[�^�̎擾
    //*/
    //---------------------------------------------------------------------
    iterator end() const
    {
        return iterator( _m.Values, _m.Nodes, _m.Capacity );
    }



private:

    struct
    {
        TYPE_*			Values;		///< �z��{��
	    Node*			Nodes;
        Stack< int >    IndexStack;
        int				Capacity;
	    int				Size;
    } _m;

    friend class iterator;
};


//=============================================================================
// �� LowList �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      std::list ���g���Ȃ��Ƃ��̑�p�i�B�Œ���̋@�\�����������Ă���܂���B
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

    /// �R���X�g���N�^
    LowList()
        : mDummyNode    ( NULL )
        , mSize         ( 0 )
    {
        mDummyNode = LN_NEW Node;
        mDummyNode->Next = mDummyNode->Prev = mDummyNode;
    }

    /// �f�X�g���N�^
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

    /// �C�e���[�^
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



    /// const �C�e���[�^
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

    /// �ǉ�����Ă���v�f���̎擾
    u32 size() const
    {
        return mSize;
    }

    /// �v�f�̒ǉ�
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

    /// �擪�̗v�f�̎擾
    TYPE_& front() const
    {
        return *( mDummyNode->Next->Value );
    }

    /// �擪�̗v�f�̍폜
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
    //  @brief      ��v����v�f���폜���� ( �ŏ��Ɍ��������ЂƂ��� )
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
    //  @brief      �擪�������C�e���[�^�̎擾
    //*/
    //---------------------------------------------------------------------
    iterator begin() { return iterator( mDummyNode->Next ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �I�[�������C�e���[�^�̎擾
    //*/
    //---------------------------------------------------------------------
    iterator end() { return iterator( mDummyNode ); }

    const_iterator begin() const { return const_iterator( mDummyNode->Next ); }
    const_iterator end() const { return const_iterator( mDummyNode ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �C�e���[�^���w���v�f�����X�g����O��
    //*/
    //---------------------------------------------------------------------
    iterator erase( const iterator& it_ )
    {
        Node* n = it_._m.Node;

        Node* next = n->Next;

        // �O����Ȃ���
        n->Prev->Next = n->Next;
        n->Next->Prev = n->Prev;

        // �폜
        n->Value->~TYPE_();
        LN_OPERATOR_DELETE( n->Value );
        SAFE_DELETE( n );

        --mSize;

        return iterator( next );
    }


private:

    Node*    mDummyNode;     ///< �I�[��\���m�[�h
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