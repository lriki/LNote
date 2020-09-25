//=============================================================================
//【 MessageQueue 】
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
// ■ MessageQueue クラス
//-----------------------------------------------------------------------------
///**
//  @brief      異なるサイズの複数のデータを扱うためのキュー
//
//  @par
//              ネットワーク関係で使うのメッセージキューのために用意したクラスです。
//
//  @attention
//              現在、ひとつ分のサイズが 0 か、または 255 バイトを超える
//              データは格納できません。
//*/
//=============================================================================
class MessageQueue
    : public Core::Base::NonCopyable
{
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
    MessageQueue();

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ
    //
    //  @param[in]  capacity_     : 確保するバイト数
    //*/
    //---------------------------------------------------------------------
	explicit MessageQueue( int capacity_ );
    
	/// デストラクタ
	~MessageQueue();

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      領域の再確保
    //
    //  @param[in]  capacity_     : 確保するバイト数
    //*/
    //---------------------------------------------------------------------
	void setCapacity( int capacity_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      空かどうかを判定する
    //*/
	//---------------------------------------------------------------------
	bool empty() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      要素数の取得
    //*/
	//---------------------------------------------------------------------
	int size() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      全ての要素を削除
    //*/
    //---------------------------------------------------------------------
	void clear();

	//---------------------------------------------------------------------
    ///**
    //  @brief      先頭に追加
    //
    //  @param[in]  data_ : データのアドレス
    //  @param[in]  size_ : データのサイズ (バイト単位)
    //
    //  @par
    //              data_ の内容をそのまま memcpy() でコピーします。
    //*/
	//---------------------------------------------------------------------
	void push( void* data_, int size_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      先頭を削除
    //*/
	//---------------------------------------------------------------------
	void pop();

    //---------------------------------------------------------------------
    ///**
    //  @brief      先頭の要素の取得
    //
    //  @param[in]  data_ : データへのポインタを格納する変数へのアドレス
    //  @param[in]  size_ : データのサイズを格納する変数へのアドレス
    //*/
	//---------------------------------------------------------------------
	void top( void** data_, int* size_ ) const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      シリアライズするために必要なバッファサイズの取得
    //*/
	//---------------------------------------------------------------------
	u32 getSerializeDataSize() const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      シリアライズ
    //
    //  @param[in]  out_  : 文字列データを格納するバッファアドレス
    //
    //  @par
    //              out_ のバッファは getSerializeDataSize() で返された
    //              値分の領域がある必要があります。
    //*/
	//---------------------------------------------------------------------
	void serialize( char* out_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      デシリアライズ
    //
    //  @param[in]  data_  : シリアライズされたデータ
    //*/
    //---------------------------------------------------------------------
	void deserialize( const char* data_ );

private:

    struct
    {
	    char*   mBuffer;	        ///< 配列本体
        u32     mCapacity;          ///< 追加できるバイト数
        u32     mSize;              ///< 追加されている要素数
        u32     mBegin;             ///< 次回読み出し位置
        u32     mEnd;               ///< 次回書き込み位置
	    
    } _c;

    static const int DATA_OFFSET = 11 * 4;
};

//-------------------------------------------------------------------------

// デフォルトコンストラクタ
inline MessageQueue::MessageQueue()
{
    memset( &_c, 0, sizeof( _c ) );
}

// コンストラクタ
inline MessageQueue::MessageQueue( int capacity_ )
{
    memset( &_c, 0, sizeof( _c ) );

    _c.mCapacity = capacity_;
    _c.mBuffer = LN_NEW char[ capacity_ + 1 ];
    memset( _c.mBuffer, 0, capacity_ + 1 );
}

// デストラクタ
inline MessageQueue::~MessageQueue()
{
    clear();
    SAFE_DELETE_ARRAY( _c.mBuffer );
}

// 領域の再確保
inline void MessageQueue::setCapacity( int capacity_ )
{
    clear();
    SAFE_DELETE_ARRAY( _c.mBuffer );
    
    _c.mCapacity = capacity_;
    _c.mBuffer = LN_NEW char[ capacity_ + 1 ];
    memset( _c.mBuffer, 0, capacity_ + 1 );
}

// 空かどうかを判定する
inline bool MessageQueue::empty() const
{
	return ( _c.mSize == 0 );
}

// 要素数の取得
inline int MessageQueue::size() const
{
    return _c.mSize;
}

// 全ての要素を削除 
inline void MessageQueue::clear()
{
    _c.mSize = _c.mBegin = _c.mEnd = 0;
}

// 先頭に追加
inline void MessageQueue::push( void* data_, int size_ )
{
    LN_ASSERT_S( ( size_ > 0 ), "< MessageQueue::push >\n\nサイズが 0 のデータは追加できません。" );
    LN_ASSERT_S( ( size_ < 256 ), "< MessageQueue::push >\n\n追加できるデータのサイズは 255 以下です。\nsize = %d", size_ );
    
    // 実際に書き込むサイズ
    int dsize = sizeof( u8 ) + size_;

    // 書き込むとバッファ本体をオーバーフローする場合
    if ( _c.mEnd + dsize  > _c.mCapacity )
    {
        // 先頭に続く事を示すサイズ 0
        _c.mBuffer[ _c.mEnd ] = 0;

        // 先頭に戻す
        _c.mEnd = 0;

        

        // 戻した後で読み込み位置に追いついた場合はエラー
        if ( _c.mEnd + dsize >= _c.mBegin )
        {
            LN_ASSERT_S( 0, "< MessageQueue::push >\n\nバッファのオーバーフローが発生しました。\nこれ以上データを追加することができません。" );
        }
    }
    // 書き込み位置が読み込み位置より前にあって、書き込んだ後追い付く場合はエラー
    else if ( _c.mEnd < _c.mBegin && _c.mEnd + dsize >= _c.mBegin )
    {
        LN_ASSERT_S( 0, "< MessageQueue::push >\n\nバッファのオーバーフローが発生しました。\nこれ以上データを追加することができません。" );
    }

    // 最初の 1 バイトにはデータサイズ
	_c.mBuffer[ _c.mEnd ] = static_cast< u8 >( size_ );

    // 1 バイト飛ばしてデータ本体
    memcpy( &_c.mBuffer[ _c.mEnd + 1 ], data_, size_ );

    _c.mEnd += size_ + 1;
    if ( _c.mEnd >= _c.mCapacity )    // 超えることは無いはずだけど、一応
    {
        _c.mEnd = 0;
    }
    ++_c.mSize;
}

// 先頭を削除
inline void MessageQueue::pop()
{
    LN_ASSERT_S( ( !empty() ), "< MessageQueue::pop >\n\nqueue is empty" );
    
    _c.mBegin += _c.mBuffer[ _c.mBegin ] + 1;
    if ( _c.mBegin >= _c.mCapacity )    // 超えることは無いはずだけど、一応
    {
        _c.mBegin = 0;
    }
    // End が Begin の前にあるという事は、終端と先頭が繋がっているということ。
    // mBuffer のバッファ内での一番最後の要素の場合、そのサイズは 0 になっている。
    // また、mBuffer はダミー要素として1バイト多く確保していて、そこには必ず 0 が入ってる。
    else if ( _c.mEnd < _c.mBegin && _c.mBuffer[ _c.mBegin ] == 0 )
    {
        _c.mBegin = 0;
    }
    --_c.mSize;
}

// 先頭の要素の取得
inline void MessageQueue::top( void** data_, int* size_ ) const
{
    *size_ = _c.mBuffer[ _c.mBegin ];

    if ( *size_ == 0 )
	{
        // 1周して先頭にデータがある
        if ( _c.mEnd < _c.mBegin )
        {
            *size_ = _c.mBuffer[ 0 ];

            if ( *size_ == 0 )  // 念のため
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

// シリアライズするために必要なバッファサイズの取得
inline u32 MessageQueue::getSerializeDataSize() const
{
    return DATA_OFFSET + _c.mCapacity;
}

// シリアライズ
inline void MessageQueue::serialize( char* out_ )
{
    sprintf( out_, "%d %d %d %d ", _c.mCapacity, _c.mSize, _c.mBegin, _c.mEnd );
    memcpy( &out_[ DATA_OFFSET ], _c.mBuffer, _c.mCapacity );
}

// デシリアライズ
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