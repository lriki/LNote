//=============================================================================
//�y MemoryManager �z
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
// �� MemoryManagerCore �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class MemoryManagerCore
{
public:

    //---------------------------------------------------------------------
	// �� �C���X�^���X�擾
	//---------------------------------------------------------------------
	static MemoryManagerCore& getInstance()
	{
		static MemoryManagerCore instance;
		return instance;
	}

    /// ���������̏o�͐��ݒ肷��
    void setMemoryDumpDest( LNMemoryDumpDest d_ ) { mMemoryDumpDest = d_; }

    /// ���������̏o�͐�̃��O�t�@�C����ݒ肷��
    void setLogFile( Base::LogFile* logfile_ )
    {
        LN_SAFE_RELEASE( mLogFile );
        mLogFile = logfile_;
        LN_SAFE_ADDREF( mLogFile );
    }

    //---------------------------------------------------------------------
	// �� �������m�ہE�m�[�h�쐬
	//---------------------------------------------------------------------
	void* allocate( size_t size_, const LNMMChar* filename_, int line_ )
	{
        // �S�̃T�C�Y
        lnU32 all_size = EXTRA_SIZE + size_;

        // �������m�ہE�w�b�_�ݒ�
        NodeHeader* header = (NodeHeader*)malloc( all_size );
        header->Filename = filename_;
        header->Line = line_;
        header->Size = size_;
        header->HeaderMagic = HEADER_MAGIC;

        // �t�b�^�ݒ�
        NodeFooter* footer = (NodeFooter*)( ((lnByte*)header) + ( all_size - FOOTER_SIZE ) );
        footer->FooterMagic = FOOTER_MAGIC;

        // ������m�[�h�����X�g�̈�ԑO�ƂЂƂ߂̊Ԃɒǉ�����
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
	// �� ���������
	//---------------------------------------------------------------------
	void deallocate( void* ptr_ )
	{
		if ( ptr_ )
		{
            NodeHeader* header = (NodeHeader*)( ((lnByte*)ptr_) - HEADER_SIZE );
            lnU32 all_size = EXTRA_SIZE + header->Size;
            NodeFooter* footer = (NodeFooter*)( ((lnByte*)header) + ( all_size - FOOTER_SIZE ) );

            // �j��`�F�b�N
			if ( header->HeaderMagic != HEADER_MAGIC || footer->FooterMagic != FOOTER_MAGIC )
			{
                LN_MMC_DUMP( "< LNote: Herp Error > file:%s line:%d\n", header->Filename, header->Line );

                //DebugBreak
			}

			// �C���X�^���X���Ǘ����Ă���\���̂����X�g����O���ĉ������
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
	// �� �R���X�g���N�^
	//---------------------------------------------------------------------
	MemoryManagerCore()
        : mInstanceList         ( NULL )
        , mLogFile              ( NULL )
        , mMemoryDumpDest       ( LN_MEMDIST_CONSOLE )
        , mUsingMemorySize      ( 0 )
        , mMaxUsedMemorySize    ( 0 )
	{
		// ���X�g�擪�̃_�~�[�v�f�����
		mInstanceList = (NodeHeader*)malloc( sizeof( NodeHeader ) );
		mInstanceList->Prev = mInstanceList;
		mInstanceList->Next = mInstanceList;
#if defined(LNOTE_WINDOWS)
        mMemoryDumpDest = LN_MEMDIST_WIN32DEBUG;
#endif
	}

	//---------------------------------------------------------------------
	// �� �f�X�g���N�^
	//---------------------------------------------------------------------
	~MemoryManagerCore()
	{
		// ���X�g�̒��Ŏc���Ă�v�f���m�F���Ă���
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
    		
			    // ���̗v�f���L��
			    next = node->Next;
			    // ���݂̗v�f�����
			    free( node );
			    // ���̗v�f���Z�b�g
			    node = next;
		    }

            LN_MMC_DUMP( "\n< LNote: Object dump complete. Total of %u bytes. >\n----------------------------------------------------------------\n\n", total );
        }

		// �Ō�ɐ擪�̗v�f�����
		free( mInstanceList );

        LN_MMC_DUMP( "LNote UsingMemorySize:%ld\n", mUsingMemorySize );
        LN_MMC_DUMP( "LNote MaxUsedMemorySize:%ld\n", mMaxUsedMemorySize );

        LN_SAFE_RELEASE( mLogFile );
	}

	// �R�s�[�֎~
	MemoryManagerCore( const MemoryManagerCore& );
	void operator = ( const MemoryManagerCore& );

private:

	// new 1�񕪂̃������u���b�N+�����Ǘ�����\���� (���̍\���̂̌��Ƀf�[�^�{�́A�t�b�^�Ƒ���)
	struct NodeHeader
	{
        const LNMMChar* Filename;
		int		        Line;	    // �s��
		NodeHeader*	    Prev;       // �O�̗v�f
		NodeHeader*	    Next;	    // ���̗v�f
        size_t	        Size;       // �f�[�^�̃o�C�g��
        lnU32           HeaderMagic;
	};

    struct NodeFooter
	{
        lnU32           FooterMagic;
	};

	NodeHeader* mInstanceList;	// ���X�g�擪�̃_�~�[�ւ̃|�C���^
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
// �� MemoryManager �N���X
//=============================================================================

    // ���������̏o�͐��ݒ肷��
    void MemoryManager::setMemoryDumpDest( LNMemoryDumpDest d_ )
    {
        MemoryManagerCore::getInstance().setMemoryDumpDest( d_ );
    }

    // ���������̏o�͐�̃��O�t�@�C����ݒ肷��
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