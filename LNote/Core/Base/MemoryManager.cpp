//=============================================================================
//【 MemoryManager 】
//=============================================================================

#include "stdafx.h"

#ifdef LNOTE_ENABLE_CRT_DEBUG
#else

#include <map>
#include "Hash.h"
#include "../Math/Math.h"
#include "../Thread/Mutex.h"
#include "MemoryManager.h"

#ifndef LN_MAX_PATH
#define LN_MAX_PATH ( 256 )
#endif



#if defined(LNOTE_WINDOWS)
    #define LN_WIN32_MMC_DUMP( fmt_, ... ) \
    { \
        char c[ 512 ]; \
        sprintf( c, fmt_, __VA_ARGS__ ); \
        ::OutputDebugStringA( c ); \
    }
#else
    #define LN_WIN32_MMC_DUMP( fmt_, ... ) \
    { \
        lnprintf( fmt_, __VA_ARGS__ ); \
    }
#endif

#if defined(LNOTE_MSVC)
#define LN_MMC_DUMP( fmt_, ... ) \
{ \
    switch ( mMemoryDumpDest ) \
    { \
        default: \
        case LN_MEMDIST_CONSOLE: \
            lnprintf( fmt_, __VA_ARGS__ ); \
            break; \
        case LN_MEMDIST_LOGFILE: \
            if ( mLogFile ) \
            { \
                mLogFile->write( fmt_, __VA_ARGS__ ); \
            } \
            else \
            { \
                lnprintf( fmt_, __VA_ARGS__ ); \
            } \
            break; \
        case LN_MEMDIST_WIN32DEBUG: \
            LN_WIN32_MMC_DUMP( fmt_, __VA_ARGS__ ); \
            break; \
    } \
}
#else
    #define LN_MMC_DUMP( fmt_, ... )// _ln_printf_core( fmt_, __VA_ARGS__ )
#endif

namespace LNote
{
namespace Core
{
namespace Base
{

//=============================================================================
// ■ MemoryManagerCore クラス
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class MemoryManagerCore
{
public:

    //---------------------------------------------------------------------
	// ● インスタンス取得
	//---------------------------------------------------------------------
	static MemoryManagerCore& getInstance()
	{
		static MemoryManagerCore instance;
		return instance;
	}

    /// メモリ情報の出力先を設定する
    void setMemoryDumpDest( LNMemoryDumpDest d_ ) { mMemoryDumpDest = d_; }

    /// メモリ情報の出力先のログファイルを設定する
    void setLogFile( Base::LogFile* logfile_ )
    {
        LN_SAFE_RELEASE( mLogFile );
        mLogFile = logfile_;
        LN_SAFE_ADDREF( mLogFile );
    }

    //---------------------------------------------------------------------
	// ● メモリ確保・ノード作成
	//---------------------------------------------------------------------
	void* allocate( size_t size_, const LNMMChar* filename_, int line_ )
	{
        // 全体サイズ
        lnU32 all_size = EXTRA_SIZE + size_;

        // メモリ確保・ヘッダ設定
        NodeHeader* header = (NodeHeader*)malloc( all_size );
        header->Filename = filename_;
        header->Line = line_;
        header->Size = size_;
        header->HeaderMagic = HEADER_MAGIC;

        // フッタ設定
        NodeFooter* footer = (NodeFooter*)( ((lnByte*)header) + ( all_size - FOOTER_SIZE ) );
        footer->FooterMagic = FOOTER_MAGIC;

        // 作ったノードをリストの一番前とひとつめの間に追加する
        mMutex.lock();
		header->Prev = mInstanceList;
		header->Next = mInstanceList->Next;
		mInstanceList->Next->Prev = header;
		mInstanceList->Next = header;
        mMutex.unlock();

        mUsingMemorySize += all_size;
        if ( mMaxUsedMemorySize < mUsingMemorySize )
        {
            mMaxUsedMemorySize = mUsingMemorySize;
        }

		return ((lnByte*)header) + HEADER_SIZE;
	}

	//---------------------------------------------------------------------
	// ● メモリ解放
	//---------------------------------------------------------------------
	void deallocate( void* ptr_ )
	{
		if ( ptr_ )
		{
            NodeHeader* header = (NodeHeader*)( ((lnByte*)ptr_) - HEADER_SIZE );
            lnU32 all_size = EXTRA_SIZE + header->Size;
            NodeFooter* footer = (NodeFooter*)( ((lnByte*)header) + ( all_size - FOOTER_SIZE ) );

            // 破壊チェック
			if ( header->HeaderMagic != HEADER_MAGIC || footer->FooterMagic != FOOTER_MAGIC )
			{
                LN_MMC_DUMP( "< LNote: Herp Error > file:%s line:%d\n", header->Filename, header->Line );

                //DebugBreak
			}

			// インスタンスを管理している構造体をリストから外して解放する
            mMutex.lock();
			header->Prev->Next = header->Next;
			header->Next->Prev = header->Prev;
            mMutex.unlock();
			free( header );

            mUsingMemorySize -= all_size;
		}
	}


private:

	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	MemoryManagerCore()
        : mInstanceList         ( NULL )
        , mLogFile              ( NULL )
        , mMemoryDumpDest       ( LN_MEMDIST_CONSOLE )
        , mUsingMemorySize      ( 0 )
        , mMaxUsedMemorySize    ( 0 )
	{
		// リスト先頭のダミー要素を作る
		mInstanceList = (NodeHeader*)malloc( sizeof( NodeHeader ) );
		mInstanceList->Prev = mInstanceList;
		mInstanceList->Next = mInstanceList;
#if defined(LNOTE_WINDOWS)
        mMemoryDumpDest = LN_MEMDIST_WIN32DEBUG;
#endif
	}

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	~MemoryManagerCore()
	{
		// リストの中で残ってる要素を確認していく
		NodeHeader* next;
		NodeHeader* node = mInstanceList->Next;

        if ( node != mInstanceList )
        {
            LN_MMC_DUMP( "\n----------------------------------------------------------------\n< LNote: Detected memory leaks! >\nDumping objects ->\n\n" );
            
            lnU32 total = 0;

		    for( ; node != mInstanceList; )
		    {
                lnByte* data = ((lnByte*)node) + HEADER_SIZE;
                lnU32 n = LMath::min( node->Size, 16u );
                
                LN_MMC_DUMP( "%s(%d) : \n[ %p ] %u bytes < ", node->Filename, node->Line, data, node->Size );
                for ( lnU32 i = 0; i < n; ++i )
                {
                    LN_MMC_DUMP( "%c ", data[ i ] );
                }
                LN_MMC_DUMP( "> " );
                for ( lnU32 i = 0; i < n; ++i )
                {
                    LN_MMC_DUMP( "%x ", data[ i ] );
                }
                LN_MMC_DUMP( "\n\n" );

                total += node->Size;
    		
			    // 次の要素を記憶
			    next = node->Next;
			    // 現在の要素を解放
			    free( node );
			    // 次の要素をセット
			    node = next;
		    }

            LN_MMC_DUMP( "\n< LNote: Object dump complete. Total of %u bytes. >\n----------------------------------------------------------------\n\n", total );
        }

		// 最後に先頭の要素を解放
		free( mInstanceList );

        LN_MMC_DUMP( "LNote UsingMemorySize:%ld\n", mUsingMemorySize );
        LN_MMC_DUMP( "LNote MaxUsedMemorySize:%ld\n", mMaxUsedMemorySize );

        LN_SAFE_RELEASE( mLogFile );
	}

	// コピー禁止
	MemoryManagerCore( const MemoryManagerCore& );
	void operator = ( const MemoryManagerCore& );

private:

	// new 1回分のメモリブロック+αを管理する構造体 (この構造体の後ろにデータ本体、フッタと続く)
	struct NodeHeader
	{
        const LNMMChar* Filename;
		int		        Line;	    // 行数
		NodeHeader*	    Prev;       // 前の要素
		NodeHeader*	    Next;	    // 次の要素
        size_t	        Size;       // データのバイト数
        lnU32           HeaderMagic;
	};

    struct NodeFooter
	{
        lnU32           FooterMagic;
	};

	NodeHeader* mInstanceList;	// リスト先頭のダミーへのポインタ
    Thread::Mutex   mMutex;
    Base::LogFile*  mLogFile;
    LNMemoryDumpDest        mMemoryDumpDest;
    lnS64                   mUsingMemorySize;
    lnS64                   mMaxUsedMemorySize;
    

    /// 
	static const size_t HEADER_SIZE = sizeof( NodeHeader );
	static const size_t FOOTER_SIZE = sizeof( NodeFooter );

    static const size_t EXTRA_SIZE = HEADER_SIZE + FOOTER_SIZE;

    static const lnU32 HEADER_MAGIC = 123456789;
	static const lnU32 FOOTER_MAGIC = 987654321;
};

//=============================================================================
// ■ MemoryManager クラス
//=============================================================================

    // メモリ情報の出力先を設定する
    void MemoryManager::setMemoryDumpDest( LNMemoryDumpDest d_ )
    {
        MemoryManagerCore::getInstance().setMemoryDumpDest( d_ );
    }

    // メモリ情報の出力先のログファイルを設定する
    void MemoryManager::setLogFile( Base::LogFile* logfile_ )
    {
        MemoryManagerCore::getInstance().setLogFile( logfile_ );
    }

} // namesapce Base
} // namespace Core
} // namespace LNote

#ifdef LNOTE_ENABLE_CRT_DEBUG

//void* LN_CDECL operator new( size_t size_ )
//{
//    return LNote::Core::Base::MemoryManagerCore::getInstance().allocate( size_, "unknown file", 0 );
//}
//void LN_CDECL operator delete ( void* ptr_ )
//{
//    LNote::Core::Base::MemoryManagerCore::getInstance().deallocate( ptr_ );
//}

void* LN_CDECL operator new( size_t size_, LNMMFlag flag_, const LNMMChar* filename_, int line_ )
{
    return ::operator new(size_, _NORMAL_BLOCK, filename_, line_);
}

void* LN_CDECL operator new[]( size_t size_, LNMMFlag flag_, const LNMMChar*  filename_, int line_ )
{
    return LNote::Core::Base::MemoryManagerCore::getInstance().allocate( size_, filename_, line_ );
}

void LN_CDECL operator delete ( void* ptr_, LNMMFlag flag_, const LNMMChar*, int )
{
    LNote::Core::Base::MemoryManagerCore::getInstance().deallocate( ptr_ );
}

void LN_CDECL operator delete[] ( void* ptr_, LNMMFlag flag_, const LNMMChar*, int )
{
    LNote::Core::Base::MemoryManagerCore::getInstance().deallocate( ptr_ );
}

#else

#if defined(LNOTE_MSVC) && !defined( LN_CLR_DLL )
void* LN_CDECL operator new( size_t size_ )
{
    return LNote::Core::Base::MemoryManagerCore::getInstance().allocate( size_, "unknown file", 0 );
}
void LN_CDECL operator delete ( void* ptr_ )
{
    LNote::Core::Base::MemoryManagerCore::getInstance().deallocate( ptr_ );
}
#endif

void* LN_CDECL operator new( size_t size_, LNMMFlag flag_, const LNMMChar* filename_, int line_ )
{
    return LNote::Core::Base::MemoryManagerCore::getInstance().allocate( size_, filename_, line_ );
}

void* LN_CDECL operator new[]( size_t size_, LNMMFlag flag_, const LNMMChar*  filename_, int line_ )
{
    return LNote::Core::Base::MemoryManagerCore::getInstance().allocate( size_, filename_, line_ );
}

void LN_CDECL operator delete ( void* ptr_, LNMMFlag flag_, const LNMMChar*, int )
{
    LNote::Core::Base::MemoryManagerCore::getInstance().deallocate( ptr_ );
}

void LN_CDECL operator delete[] ( void* ptr_, LNMMFlag flag_, const LNMMChar*, int )
{
    LNote::Core::Base::MemoryManagerCore::getInstance().deallocate( ptr_ );
}

#endif
#endif

//=============================================================================
//								end of file
//=============================================================================