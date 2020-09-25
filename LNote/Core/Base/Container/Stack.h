//=============================================================================
//�y Stack �z
//-----------------------------------------------------------------------------
///**
//  @file       Stack.h
//  @brief      Stack
//  @author     Riki
//*/
//=============================================================================

#pragma once

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
// �� Stack �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �T�C�Y�Œ�̃X�^�b�N�N���X
//
//  @par
//              STL ���g���� lib ����������� VisualC++ �̃o�[�W�����ɂ����
//              �����N�ł��Ȃ����Ƃ����邽�߁A�����p�ӂ��Ă���܂��B<br>
//              ���̂��߂���͊�{�I�ɓ����p�ł��B
//              <br>
//              �����ASTL �̂��̂������{(��҂̊��ł�7�{���炢)�����̂�
//              �g�����͂��邩������܂���B
//*/
//=============================================================================
template< typename TYPE_ > class Stack
    : public NonCopyable
{
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
    Stack();

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���X�g���N�^
    //
    //  @param[in]  capacity_ : �i�[�ł���v�f��
    //*/
    //---------------------------------------------------------------------
	explicit Stack( int capacity_ );
    
	/// �f�X�g���N�^
	~Stack();

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �̈�̍Ċm��
    //
    //  @param[in]  capacity_ : �i�[�ł���v�f��
    //*/
    //---------------------------------------------------------------------
	void setCapacity( int capacity_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �󂩂ǂ����𔻒肷��
    //*/
	//---------------------------------------------------------------------
	bool empty() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �v�f���̎擾
    //*/
	//---------------------------------------------------------------------
	int size() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �S�Ă̗v�f���폜 ( �f�X�g���N�^�Ăяo�� )
    //*/
    //---------------------------------------------------------------------
	void clear();

	//---------------------------------------------------------------------
    ///**
    //  @brief      �擪�ɒǉ� ( �R�s�[�R���X�g���N�^�Ăяo�� )
    //
    //  @retval     true  : ����
    //  @retval     false : ����ȏ�ǉ��ł��Ȃ�
    //*/
	//---------------------------------------------------------------------
	bool push( const TYPE_& value_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �擪���폜 ( �f�X�g���N�^�Ăяo�� )
    //*/
	//---------------------------------------------------------------------
	void pop();

    //---------------------------------------------------------------------
    ///**
    //  @brief      �擪�̗v�f�̎擾
    //*/
	//---------------------------------------------------------------------
	TYPE_& top() const;

private:

    struct
    {
	    TYPE_*	mValues;	///< �z��{��
	    int		mSize;		///< ���݂̗v�f��
	    int		mCapacity;	///< �v�f�̍ő吔
	    int		mBack;		///< �I�[�B����̂ЂƂO�܂ł��v�f������Ƃ���
    } _c;
};

//-------------------------------------------------------------------------

// �f�t�H���g�R���X�g���N�^
template< class TYPE_ > inline Stack< TYPE_ >::Stack()
{
    memset( &_c, 0, sizeof( _c ) );
}

// �R���X�g���N�^
template< class TYPE_ > inline Stack< TYPE_ >::Stack( int capacity_ )
{
    memset( &_c, 0, sizeof( _c ) );

	if ( capacity_ <= 0 ) { return; }
	_c.mCapacity = capacity_;
	_c.mValues = static_cast< TYPE_* >( OPERATOR_NEW( sizeof( TYPE_ ) * _c.mCapacity ) );
}

// �f�X�g���N�^
template< class TYPE_ > inline Stack< TYPE_ >::~Stack()
{
	clear();
	LN_OPERATOR_DELETE( _c.mValues );
}

// �̈�̍Ċm��
template< class TYPE_ > inline void Stack< TYPE_ >::setCapacity( int capacity_ )
{
	clear();
	LN_OPERATOR_DELETE( _c.mValues );
	_c.mCapacity = 0;
	if ( capacity_ <= 0 ) { return; }
	_c.mCapacity = capacity_;
	_c.mValues = static_cast< TYPE_* >( LN_OPERATOR_NEW( sizeof( TYPE_ ) * _c.mCapacity ) );
}

// �󂩂ǂ����𔻒肷��
template< class TYPE_ > inline bool Stack< TYPE_ >::empty() const
{
	return ( _c.mSize == 0 );
}

// �v�f���̎擾
template< class TYPE_ > inline int Stack< TYPE_ >::size() const
{
    return _c.mSize;
}

// �S�Ă̗v�f���폜 ( �f�X�g���N�^�Ăяo�� )
template< class TYPE_ > inline void Stack< TYPE_ >::clear()
{
	if ( _c.mValues )
	{
		for ( int i = 0; i < _c.mBack; ++i )
		{
			// �f�X�g���N�^�Ăяo��
			_c.mValues[ i ].~TYPE_();
		}
		_c.mBack = _c.mSize = 0;
	}
}

// �擪�ɒǉ� ( �R�s�[�R���X�g���N�^�Ăяo�� )
template< class TYPE_ > inline bool Stack< TYPE_ >::push( const TYPE_& value_ )
{
	if ( _c.mSize >= _c.mCapacity )
	{
		// �x���\��
		return false;
	}

	// mBack �̈ʒu�ɑ΂��ăR�s�[�R���X�g���N�^�Ăяo��
    ::new ( &_c.mValues[ _c.mBack ] ) TYPE_( value_ );
	++_c.mBack;
	++_c.mSize;

    return true;
}

// �擪���폜 ( �f�X�g���N�^�Ăяo�� )
template< class TYPE_ > inline void Stack< TYPE_ >::pop()
{
	if ( _c.mSize <= 0 )
	{
		return;
	}

	--_c.mBack;
	// �f�X�g���N�^�Ăяo��
	_c.mValues[ _c.mBack ].~TYPE_();
	--_c.mSize;
}

// �擪�̗v�f�̎擾
template< class TYPE_ > inline TYPE_& Stack< TYPE_ >::top() const
{
	return _c.mValues[ _c.mBack - 1 ];
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