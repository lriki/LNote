//=============================================================================
//�y NodeList �z
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
// �� NodeList �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      
//
//  @par
//              TYPE_ �ɂ̓|�C���^�^���w�肵�Ȃ��悤�ɂ��Ă��������B
//              �����Apush_back() ���̒ǉ��E�폜�̓|�C���^�ōs���_�ɒ��ӁB
//              (���̂ւ�킩��ɂ����Ǝv���̂Ō�ŕς��邩��)
//
//  @note
//              ���̂Ƃ���e���v���[�g�ɂ���K�v�͖��������肷�邯�ǁA
//              �C�e���[�^���Ō^�̎Q�Ƃ�Ԃ���悤�ɂ��邽�߂Ƀe���v���[�g�Ŏ����B<br>
//              ���ƁA�����̃��X�g�ւ̒ǉ��Ƃ��B
//
//              SceneObject �݂����ɁA�����̐e����p���������N���X�̃��X�g�Ƃ���ꍇ�A
//              push_back() ���̈����� Node* �Ƃ����ƁAC++ ���C�𗘂����Ă����
//              �Ȃ񂩂̎d�l�ŁA�|�C���^�������B
//              ����ɁANode* �ł͂Ȃ� TYPE_* �ɂ��đΉ�����B
//*/
//=============================================================================
template< class TYPE_ > class NodeList
    : public NonCopyable
{
public:

    /// NodeList �ɒǉ����邽�߂ɂ͂�����p������
    template< class NODE_LIST > 
    class _Node
    {
    public:

        /// �R���X�g���N�^
	    _Node()
        {
            _m.Prev = _m.Next = NULL;
            _m.NodeList = NULL;
        }

        /// �f�X�g���N�^
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
            TYPE_*      Prev;		///< �ЂƂO�̃^�X�N
	        TYPE_*      Next;		///< �ЂƂ��̃^�X�N
	        NODE_LIST*	NodeList;   ///< ���X�g�ւ̃|�C���^
        } _m;
    };

    typedef _Node< NodeList< TYPE_ > >Node;

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

    /// ��r�֐��̌^
    typedef bool( *CompFunction )( const void*, const void* );

public:

    /// �R���X�g���N�^
	NodeList();

    /// �f�X�g���N�^
	~NodeList();

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �v�f���̎擾
    //*/
    //---------------------------------------------------------------------
    int size() const { return _m.Size; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �m�[�h�𖖔��ɒǉ�����
    //*/
    //---------------------------------------------------------------------
    void push_back( TYPE_* node_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �m�[�h�����X�g����O��
    //*/
    //---------------------------------------------------------------------
    void remove( TYPE_* node_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �S�Ẵm�[�h�����X�g����O��
    //*/
    //---------------------------------------------------------------------
    void clear();

    //---------------------------------------------------------------------
    ///**
    //  @brief     �擪�̃m�[�h�̎擾
    //*/
    //---------------------------------------------------------------------
    TYPE_* front() const { return static_cast< TYPE_* >( _m.Dummy->_m.Next ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief     �擪�̃m�[�h�������X�g����O��
    //*/
    //---------------------------------------------------------------------
    void pop_front() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �擪�������C�e���[�^�̎擾
    //*/
    //---------------------------------------------------------------------
    iterator begin() const { return iterator( _m.Dummy->_m.Next ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �I�[�������C�e���[�^�̎擾
    //*/
    //---------------------------------------------------------------------
    iterator end() const { return iterator( _m.Dummy ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �C�e���[�^���w���v�f�����X�g����O��
    //*/
    //---------------------------------------------------------------------
    iterator erase( const iterator& it_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���X�g���\�[�g����
    //*/
	//---------------------------------------------------------------------
	void sort( CompFunction comp_function_ )
	{
		// 1�ȉ��̏ꍇ�̓\�[�g�̕K�v�i�V
		if ( _m.Dummy->_m.Next == _m.Dummy || _m.Dummy->_m.Next->_m.Next == _m.Dummy ) { return; }

		// ��r�֐��ݒ�
		_m.CompFunction = comp_function_;
		// �\�[�g���s��A�߂��Ă����^�X�N��擪�ɂ���
		_m.Dummy->_m.Next = _mergeSort( _m.Dummy->_m.Next );

		// �\�[�g���I������Ƃ��A������ ( mNext ) �͐������Ȃ����Ă邯��
		// �O���� ( mPrev ) �̓o���o���ɂȂ��Ă���B�Ƃ����킯�łȂ��Ȃ����B
		TYPE_* prev = _m.Dummy;
		TYPE_* node = _m.Dummy->_m.Next;	// �擪(AT�̎�)
		for ( ; node != _m.Dummy; )
		{
			node->_m.Prev = prev;
			prev = node;
			node = node->_m.Next;
		}
		_m.Dummy->_m.Prev = prev;
	}

private:

    // �}�[�W�\�[�g���s
	TYPE_* _mergeSort( TYPE_* front_head )
	{
		// �^�X�N�̐��� 0 �܂��� 1 �̏ꍇ�̓\�[�g�̕K�v�Ȃ��̂ŏI��
		if ( front_head == _m.Dummy || front_head->_m.Next == _m.Dummy )
		{
			return front_head;
		}
		
		// a �͐擪���w��
		TYPE_* a = front_head;

		// b �͐擪�̂ЂƂ����w������A�����ЂƂ�낪����΂�����w��
		TYPE_* b = front_head->_m.Next;
		if ( b != _m.Dummy )
		{
			b = b->_m.Next;
		}

		// ���`���X�g�𒆐S���炢���甼���ɕ����邽�߁A���S�ʒu���ǂ��ɂ���̂��T��
		while ( b != _m.Dummy )
		{
			// a �͂ЂƂ��i��
			a = a->_m.Next;

			// b �͉\�Ȍ���ӂ����i��
			b = b->_m.Next;

			if ( b != _m.Dummy )
			{
				b = b->_m.Next;
			}
		}

		// a �̎��̗v�f���ӂ��߂̃��X�g�̐擪�Ƃ��āA���X�g�𕪊�
		TYPE_* back_head = a->_m.Next;
		a->_m.Next = _m.Dummy;

		// �ċA�ŃK���K����������ƁA�\�[�g���Ȃ��猋�����Ă���
		return _mergeList( _mergeSort( front_head ), _mergeSort( back_head ) );
	}

	// �}�[�W�\�[�g�p�E���X�g����
	TYPE_* _mergeList( TYPE_* left_node, TYPE_* right_node )
	{
		TYPE_* res_front = _m.Dummy;	// �����ナ�X�g�̐擪�m�[�h
		TYPE_* res_back  = _m.Dummy;	// �����ナ�X�g�̏I�[�m�[�h

		// �� �ŏ��ɐ擪�ƂȂ�v�f�����߂�

		// �����̃��X�g�̒��g������
		if ( left_node != _m.Dummy && right_node != _m.Dummy )
		{
			// �擪�̗v�f���m���r
			if( _m.CompFunction( left_node, right_node ) )
			{
				// �����X�g����ЂƂ��o���Č����ナ�X�g�ɒǉ�
				res_front = left_node;
				res_front->_m.Next = left_node->_m.Next;

				// ���̍����X�g�̎��̗v�f���w��
				left_node = left_node->_m.Next;
			}
			else
			{
				// �E���X�g����ЂƂ��o���Č����ナ�X�g�ɒǉ�
				res_front = right_node;
				res_front->_m.Next = right_node->_m.Next;	// ����Ă邱�Ɠ��������ǁc?[9/11]

				// ���̉E���X�g�̎��̗v�f���w��
				right_node = right_node->_m.Next;
			}
		}
		else
		{
			//LASSERT_S_( 1, LException::Fatal(), "���X�g�̃\�[�g���̗\�����Ȃ��G���[\n( �������̂��߂ɍ�����Ƃ���ŗ����������B�A�����肢���܂��B )" );
		}
		// ���̕����͖����Ă���������
		//// �������X�g����������
		//else if ( left_node != mActiveTask )
		//{
		//	res_front = left_node;
		//	res_front->mNext = left_node;
		//	left_node = left_node->mNext;
		//}
		//// �E�����X�g����������
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


		// �� �擪�̗v�f�����܂����̂ŁA��������c����r���Ȃ��猋�����Ă���

		// �Ƃ肠���������ナ�X�g�̈�Ԍ�����ԑO���w���Ă���
		res_back = res_front;

		// ��������Ă��� 2 �̃��X�g�̂ǂ��炩����ɂȂ�܂Ń��[�v
		while( left_node != _m.Dummy && right_node != _m.Dummy )
		{
			// �擪�̗v�f���m���r
			if( _m.CompFunction( left_node, right_node ) )
			{
				// �����X�g����ЂƂ��o���Č����ナ�X�g�ɒǉ�
				res_back->_m.Next = left_node;
				res_back = left_node;

				// ���̍����X�g�̎��̗v�f���w��
				left_node = left_node->_m.Next;
			}
			else
			{
				// �E���X�g����ЂƂ��o���Č����ナ�X�g�ɒǉ�
				res_back->_m.Next = right_node;
				res_back = right_node;

				// ���̉E���X�g�̎��̗v�f���w��
				right_node = right_node->_m.Next;
			}
		}
		
		// ��ɕЕ��̃��X�g����ɂȂ�̂ŁA�c���ꂽ���̃��X�g�̗v�f��S������
		while ( left_node != _m.Dummy || right_node != _m.Dummy )
		{
			// ������������ۂ̏ꍇ
			if( left_node == _m.Dummy )
			{
				// �E�Ɏc���Ă�����̂��l�߂Ă���
				res_back->_m.Next = right_node;
				res_back = right_node;
				right_node = right_node->_m.Next;
			}
			// �E����������ۂ̏ꍇ
			else
			{
				// ���Ɏc���Ă�����̂��l�߂Ă���
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
// �R���X�g���N�^
template< class TYPE_ > inline NodeList< TYPE_ >::Node::Node()
{
    _m.Prev = _m.Next = NULL;
    _m.NodeList = NULL;
}

// �f�X�g���N�^
template< class TYPE_ > inline NodeList< TYPE_ >::Node::~Node()
{
	// ���X�g����O��
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

// �R���X�g���N�^
template< class TYPE_ > inline NodeList< TYPE_ >::NodeList()
{
	_m.Dummy = static_cast< TYPE_* >( LN_OPERATOR_NEW( sizeof( TYPE_ ) ) );
    //_m.Dummy = static_cast< TYPE_* >( LN_OPERATOR_NEW( sizeof( Node ) ) );    // ����ł������Ǝv�����ǁA�Ȃ񂩕s���Ȃ̂�
	_m.Dummy->_m.Prev = _m.Dummy;
	_m.Dummy->_m.Next = _m.Dummy;
    _m.CompFunction = NULL;
    _m.Size = 0;
}

// �f�X�g���N�^
template< class TYPE_ > inline NodeList< TYPE_ >::~NodeList()
{
    clear();
	LN_OPERATOR_DELETE( _m.Dummy );
}

// �m�[�h�𖖔��ɒǉ�����
template< class TYPE_ > inline void NodeList< TYPE_ >::push_back( TYPE_* node_ )
{
	// ���ɒǉ�����Ă���ꍇ�͂Ȃɂ����Ȃ�
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

// �m�[�h�����X�g����O��
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

// �S�Ẵm�[�h�����X�g����O��
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

// �擪�̃m�[�h�������X�g����O��
template< class TYPE_ > inline void NodeList< TYPE_ >::pop_front() const
{
    TYPE_* n = _m.Dummy->_m.Next;
    if ( n != _m.Dummy )
    {
        // �O����Ȃ���
        n->_m.Prev->_m.Next = n->_m.Next;
        n->_m.Next->_m.Prev = n->_m.Prev;
        // �O��
        n->_m.Prev = n->_m.Next = NULL;
        n->_m.NodeList = NULL;
        --_m.Size;
    }
}

// �C�e���[�^���w���v�f�����X�g����O��
template< class TYPE_ > typename NodeList< TYPE_ >::iterator NodeList< TYPE_ >::erase( const iterator& it_ )
{
    TYPE_* n = it_._m.Node;
    TYPE_* next = n->_m.Next;
    // �O����Ȃ���
    n->_m.Prev->_m.Next = n->_m.Next;
    n->_m.Next->_m.Prev = n->_m.Prev;
    // �O��
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