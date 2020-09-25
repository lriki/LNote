//=============================================================================
//�y MessageQueue �z
//-----------------------------------------------------------------------------
///**
//  @file       MessageQueue.h
//  @brief      MessageQueue
//  @version    1.0
//  @date       2011/11/11
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <strstream>

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Utility
{

//=============================================================================
// �� MessageQueue �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �قȂ�T�C�Y�̕����̃f�[�^���������߂̃L���[
//
//  @par
//              �l�b�g���[�N�֌W�Ŏg���̃��b�Z�[�W�L���[�̂��߂ɗp�ӂ����N���X�ł��B
//
//  @attention
//              ���݁A�ЂƂ��̃T�C�Y�� 0 ���A�܂��� 255 �o�C�g�𒴂���
//              �f�[�^�͊i�[�ł��܂���B
//*/
//=============================================================================
class MessageQueue
    : public Core::Base::NonCopyable
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
    MessageQueue();

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���X�g���N�^
    //
    //  @param[in]  capacity_     : �m�ۂ���o�C�g��
    //*/
    //---------------------------------------------------------------------
	explicit MessageQueue( int capacity_ );
    
	/// �f�X�g���N�^
	~MessageQueue();

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �̈�̍Ċm��
    //
    //  @param[in]  capacity_     : �m�ۂ���o�C�g��
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
    //  @brief      �S�Ă̗v�f���폜
    //*/
    //---------------------------------------------------------------------
	void clear();

	//---------------------------------------------------------------------
    ///**
    //  @brief      �擪�ɒǉ�
    //
    //  @param[in]  data_ : �f�[�^�̃A�h���X
    //  @param[in]  size_ : �f�[�^�̃T�C�Y (�o�C�g�P��)
    //
    //  @par
    //              data_ �̓��e�����̂܂� memcpy() �ŃR�s�[���܂��B
    //*/
	//---------------------------------------------------------------------
	void push( void* data_, int size_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �擪���폜
    //*/
	//---------------------------------------------------------------------
	void pop();

    //---------------------------------------------------------------------
    ///**
    //  @brief      �擪�̗v�f�̎擾
    //
    //  @param[in]  data_ : �f�[�^�ւ̃|�C���^���i�[����ϐ��ւ̃A�h���X
    //  @param[in]  size_ : �f�[�^�̃T�C�Y���i�[����ϐ��ւ̃A�h���X
    //*/
	//---------------------------------------------------------------------
	void top( void** data_, int* size_ ) const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �V���A���C�Y���邽�߂ɕK�v�ȃo�b�t�@�T�C�Y�̎擾
    //*/
	//---------------------------------------------------------------------
	u32 getSerializeDataSize() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �V���A���C�Y
    //
    //  @param[in]  out_  : ������f�[�^���i�[����o�b�t�@�A�h���X
    //
    //  @par
    //              out_ �̃o�b�t�@�� getSerializeDataSize() �ŕԂ��ꂽ
    //              �l���̗̈悪����K�v������܂��B
    //*/
	//---------------------------------------------------------------------
	void serialize( char* out_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �f�V���A���C�Y
    //
    //  @param[in]  data_  : �V���A���C�Y���ꂽ�f�[�^
    //*/
    //---------------------------------------------------------------------
	void deserialize( const char* data_ );

private:

    struct
    {
	    char*   mBuffer;	        ///< �z��{��
        u32     mCapacity;          ///< �ǉ��ł���o�C�g��
        u32     mSize;              ///< �ǉ�����Ă���v�f��
        u32     mBegin;             ///< ����ǂݏo���ʒu
        u32     mEnd;               ///< ���񏑂����݈ʒu
	    
    } _c;

    static const int DATA_OFFSET = 11 * 4;
};

//-------------------------------------------------------------------------

// �f�t�H���g�R���X�g���N�^
inline MessageQueue::MessageQueue()
{
    memset( &_c, 0, sizeof( _c ) );
}

// �R���X�g���N�^
inline MessageQueue::MessageQueue( int capacity_ )
{
    memset( &_c, 0, sizeof( _c ) );

    _c.mCapacity = capacity_;
    _c.mBuffer = LN_NEW char[ capacity_ + 1 ];
    memset( _c.mBuffer, 0, capacity_ + 1 );
}

// �f�X�g���N�^
inline MessageQueue::~MessageQueue()
{
    clear();
    SAFE_DELETE_ARRAY( _c.mBuffer );
}

// �̈�̍Ċm��
inline void MessageQueue::setCapacity( int capacity_ )
{
    clear();
    SAFE_DELETE_ARRAY( _c.mBuffer );
    
    _c.mCapacity = capacity_;
    _c.mBuffer = LN_NEW char[ capacity_ + 1 ];
    memset( _c.mBuffer, 0, capacity_ + 1 );
}

// �󂩂ǂ����𔻒肷��
inline bool MessageQueue::empty() const
{
	return ( _c.mSize == 0 );
}

// �v�f���̎擾
inline int MessageQueue::size() const
{
    return _c.mSize;
}

// �S�Ă̗v�f���폜 
inline void MessageQueue::clear()
{
    _c.mSize = _c.mBegin = _c.mEnd = 0;
}

// �擪�ɒǉ�
inline void MessageQueue::push( void* data_, int size_ )
{
    LN_ASSERT_S( ( size_ > 0 ), "< MessageQueue::push >\n\n�T�C�Y�� 0 �̃f�[�^�͒ǉ��ł��܂���B" );
    LN_ASSERT_S( ( size_ < 256 ), "< MessageQueue::push >\n\n�ǉ��ł���f�[�^�̃T�C�Y�� 255 �ȉ��ł��B\nsize = %d", size_ );
    
    // ���ۂɏ������ރT�C�Y
    int dsize = sizeof( u8 ) + size_;

    // �������ނƃo�b�t�@�{�̂��I�[�o�[�t���[����ꍇ
    if ( _c.mEnd + dsize  > _c.mCapacity )
    {
        // �擪�ɑ������������T�C�Y 0
        _c.mBuffer[ _c.mEnd ] = 0;

        // �擪�ɖ߂�
        _c.mEnd = 0;

        

        // �߂�����œǂݍ��݈ʒu�ɒǂ������ꍇ�̓G���[
        if ( _c.mEnd + dsize >= _c.mBegin )
        {
            LN_ASSERT_S( 0, "< MessageQueue::push >\n\n�o�b�t�@�̃I�[�o�[�t���[���������܂����B\n����ȏ�f�[�^��ǉ����邱�Ƃ��ł��܂���B" );
        }
    }
    // �������݈ʒu���ǂݍ��݈ʒu���O�ɂ����āA�������񂾌�ǂ��t���ꍇ�̓G���[
    else if ( _c.mEnd < _c.mBegin && _c.mEnd + dsize >= _c.mBegin )
    {
        LN_ASSERT_S( 0, "< MessageQueue::push >\n\n�o�b�t�@�̃I�[�o�[�t���[���������܂����B\n����ȏ�f�[�^��ǉ����邱�Ƃ��ł��܂���B" );
    }

    // �ŏ��� 1 �o�C�g�ɂ̓f�[�^�T�C�Y
	_c.mBuffer[ _c.mEnd ] = static_cast< u8 >( size_ );

    // 1 �o�C�g��΂��ăf�[�^�{��
    memcpy( &_c.mBuffer[ _c.mEnd + 1 ], data_, size_ );

    _c.mEnd += size_ + 1;
    if ( _c.mEnd >= _c.mCapacity )    // �����邱�Ƃ͖����͂������ǁA�ꉞ
    {
        _c.mEnd = 0;
    }
    ++_c.mSize;
}

// �擪���폜
inline void MessageQueue::pop()
{
    LN_ASSERT_S( ( !empty() ), "< MessageQueue::pop >\n\nqueue is empty" );
    
    _c.mBegin += _c.mBuffer[ _c.mBegin ] + 1;
    if ( _c.mBegin >= _c.mCapacity )    // �����邱�Ƃ͖����͂������ǁA�ꉞ
    {
        _c.mBegin = 0;
    }
    // End �� Begin �̑O�ɂ���Ƃ������́A�I�[�Ɛ擪���q�����Ă���Ƃ������ƁB
    // mBuffer �̃o�b�t�@���ł̈�ԍŌ�̗v�f�̏ꍇ�A���̃T�C�Y�� 0 �ɂȂ��Ă���B
    // �܂��AmBuffer �̓_�~�[�v�f�Ƃ���1�o�C�g�����m�ۂ��Ă��āA�����ɂ͕K�� 0 �������Ă�B
    else if ( _c.mEnd < _c.mBegin && _c.mBuffer[ _c.mBegin ] == 0 )
    {
        _c.mBegin = 0;
    }
    --_c.mSize;
}

// �擪�̗v�f�̎擾
inline void MessageQueue::top( void** data_, int* size_ ) const
{
    *size_ = _c.mBuffer[ _c.mBegin ];

    if ( *size_ == 0 )
	{
        // 1�����Đ擪�Ƀf�[�^������
        if ( _c.mEnd < _c.mBegin )
        {
            *size_ = _c.mBuffer[ 0 ];

            if ( *size_ == 0 )  // �O�̂���
            {
                *data_ = NULL;
            }
            else
            {
                *data_ = &( _c.mBuffer[ 1 ] );
            }
        }
        else
        {
		    *data_ = NULL;
        }
	}
    else
    {
		*data_ = &( _c.mBuffer[ _c.mBegin + 1 ] );
    }
}

// �V���A���C�Y���邽�߂ɕK�v�ȃo�b�t�@�T�C�Y�̎擾
inline u32 MessageQueue::getSerializeDataSize() const
{
    return DATA_OFFSET + _c.mCapacity;
}

// �V���A���C�Y
inline void MessageQueue::serialize( char* out_ )
{
    sprintf( out_, "%d %d %d %d ", _c.mCapacity, _c.mSize, _c.mBegin, _c.mEnd );
    memcpy( &out_[ DATA_OFFSET ], _c.mBuffer, _c.mCapacity );
}

// �f�V���A���C�Y
inline void MessageQueue::deserialize( const char* data_ )
{
    u32 cap = _c.mCapacity;
    sscanf( data_, "%d %d %d %d", _c.mCapacity, _c.mSize, _c.mBegin, _c.mEnd );

    if ( _c.mCapacity > cap )
    {
        SAFE_DELETE_ARRAY( _c.mBuffer );
        _c.mBuffer = LN_NEW char[ _c.mCapacity ];
    }
    memcpy( _c.mBuffer, data_, _c.mCapacity );
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Utility
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================