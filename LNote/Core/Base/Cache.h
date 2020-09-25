//=============================================================================
//�y Cache �z
//-----------------------------------------------------------------------------
///**
//  @file       Cache.h
//  @brief      Cache
//  @author     Riki
//  @note
/*              �� �L���b�V���V�X�e���S�̂̒��ӓ_
                    
                    �Q�ƃJ�E���g����p�̊֐�����}�N���ɂ� LN_CACHED_REFOBJ_METHODS ���g�����ƁB
                    Midi �N���X�Ō���� LN_REFOBJ_METHODS �������Ă��܂��A
                    �I�u�W�F�N�g�� delete ���ꂽ��̕s���Ȓl�̂܂܃L���b�V���ɓo�^���ꂽ
                    �Ƃ������Ƃ��������B
*/
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <iostream>
#include <map>
#include "Container/Stack.h"
//#include "Container/Map.h"
#include "SharingKey.h"
#include "../Thread/Mutex.h"



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Base
{

#define LN_CACHED_REFOBJ_METHODS                                                \
    virtual s32 getRefCount()   { return Base::ReferenceObject::getRefCount(); }\
    virtual s32 addRef()        { return Base::ReferenceObject::addRef(); }     \
    virtual s32 release()                                                       \
    {                                                                           \
        CachedObject::release();                                                \
        return Base::ReferenceObject::release();                                \
    }                                                                           \
    virtual s32 Release()                                                       \
    {                                                                           \
        CachedObject::release();                                                \
        return Base::ReferenceObject::Release();                                \
    }

//=============================================================================
// �� CacheObjectManager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �L���b�V���V�X�e���̊Ǘ��N���X
//
//  @note
//              �Q�ƃJ�E���g�����Ǘ��N���X�Ɍp�������Ďg���B
//
//              �Ǘ������I�u�W�F�N�g�́A����҂����X�g�ɓ����ꂽ�Ƃ���
//              �Q�ƃJ�E���g���ЂƂ�����B
//				�c�Ƃ������A�J�E���g�� 1 �ɂȂ������ɑ҂����X�g�ɓ������
//*/
//=============================================================================
template< class TYPE_ >
class CacheObjectManager
    : private NonCopyable
{
public:

    class CachedObject;
    class CacheUnusedList;

public:

    /// �R���X�g���N�^
	CacheObjectManager();

    /// �f�X�g���N�^
    virtual ~CacheObjectManager();

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      ������
    //
    //  @param[in]  capacity_ : ����҂����X�g�̃T�C�Y
    //
    //  @note
    //              capacity_ �� 0 �̏ꍇ�A�g�p���̃I�u�W�F�N�g�̋��L�̂ݍs���A
    //              ����҂��͍s��Ȃ��B
    //*/
    //---------------------------------------------------------------------
    void initialize( int capacity_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �I������
    //
    //  @note
    //              �I����ʒm���āA����ȍ~�L���b�V���ɃI�u�W�F�N�g��
    //              �ǉ����Ȃ��悤�ɂ���B
    //*/
    //---------------------------------------------------------------------
    void finalize();

    //---------------------------------------------------------------------
    ///**
    //  @brief      ����҂����X�g���N���A����
    //*/
    //---------------------------------------------------------------------
    void clear();

    //---------------------------------------------------------------------
    ///**
    //  @brief      �I�u�W�F�N�g���Ǘ��ɒǉ�����
    //
    //  @note
    //              �L�[�������ȏꍇ�͒ǉ����Ȃ��B
    //*/
    //---------------------------------------------------------------------
    void registerCachedObject( const SharingKey& key_, TYPE_* obj_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L�[�Ɉ�v����I�u�W�F�N�g����������
    //
    //  @note
    //              ���������ꍇ�͎Q�ƃJ�E���g�𑝂₵�ĕԂ��B
    //              ����҂����X�g�ɓ����Ă����ꍇ�͂��ꂩ��O���āA
    //              �Q�ƃJ�E���g�� 1 �̏�ԂŕԂ��B
    //              �L�[�������̏ꍇ�͏�� NULL ��Ԃ��B
    //*/
    //---------------------------------------------------------------------
    TYPE_* findCacheAddRef( const SharingKey& key_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L���b�V���̓��e���e�L�X�g�t�@�C���ɏo�͂���
    //*/
    //---------------------------------------------------------------------
    void dumpCache( const lnChar* filename_ );

public:

    /// �Q�ƃJ�E���g�𑝂₷ (CachedObject �̃R���X�g���N�^����Ă΂��)
    virtual s32 addRef() = 0;

    /// �Q�ƃJ�E���g�����炷 (CachedObject �̃f�X�g���N�^����Ă΂��)
    virtual s32 release() = 0;

private:

    /// CachedObject �폜���O�̃R�[���o�b�N
    void _onDeleteCachedObject( CachedObject* obj_ );

private:

    template < class CACHE_UNUSED_LIST >
    struct _mb
    {
        std::map< SharingKey, TYPE_* >  CacheUsingMap;
        CACHE_UNUSED_LIST               CacheUnusedList;
        Thread::Mutex*                  Mutex;
        bool                            IsSharingOnly;
        bool                            Finalized;
    };
    _mb< CacheUnusedList > _m;

    friend class CachedObject;
};

//=============================================================================
// �� CachedObject �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �L���b�V�������I�u�W�F�N�g�̊��
//*/
//=============================================================================
template< class TYPE_ >
class CacheObjectManager< TYPE_ >::CachedObject
{
public:

    /// �R���X�g���N�^
    CachedObject()
    {
        _m.Manager = NULL;
        _m.Using = true;
    }

    /// �f�X�g���N�^
    virtual ~CachedObject()
    {
        LN_SAFE_RELEASE( _m.Manager );
    }

public:

    /// �Q�ƃJ�E���g���擾����
    virtual s32 getRefCount() = 0;

    /// �Q�ƃJ�E���g�𑝂₷
    virtual s32 addRef() = 0;

    /// release() �̒ǉ����� (�p����ŁAReferenceObject::release() �̑O�ŌĂяo��)
    virtual s32 release()
    {
        if ( getRefCount() == 1 )
        {
            if ( _m.Manager && !_m.Manager->_m.Finalized )
            {
                _m.Manager->_onDeleteCachedObject( this );
            }
        }
       
        return 0;
    }

private:

    struct
    {
        CacheObjectManager* Manager;    ///< �Z�b�g�����̂� addCachedObject()�BNULL �ł͂Ȃ��ꍇ�A�L�[�͗L���B
        SharingKey          Key;        ///< �L�[ (addCachedObject() �ŃZ�b�g)
        bool                Using;      ///< ����҂����X�g�ɓ����Ă���Ԃ� false
    } _m;

    friend class CacheObjectManager;
};

//=============================================================================
// �� CacheUnusedList �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �L���b�V���V�X�e���̂����A���ݎg�p����Ă��Ȃ��I�u�W�F�N�g���Ǘ�����N���X
//*/
//=============================================================================
template< class TYPE_ >
class CacheObjectManager< TYPE_ >::CacheUnusedList
    : private NonCopyable
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �f�t�H���g�R���X�g���N�^
    //
    //  @note
    //              �f�t�H���g�R���X�g���N�^�ō쐬�����ꍇ��
    //              ���̂��Ƃ� setCapacity() ���Ă�ŗ̈���m�ۂ��邱�ƁB
    //*/
    //---------------------------------------------------------------------
	CacheUnusedList()
        : mNodes    ( NULL )
        , mCapacity ( 0 )
        , mSize     ( 0 )
    {}

    //---------------------------------------------------------------------
    ///**
    //  @brief      �f�X�g���N�^
    //*/
    //---------------------------------------------------------------------
    ~CacheUnusedList()
    {
        clear();
        SAFE_DELETE_ARRAY( mNodes );
    }

public:

    /// �ő�v�f���̎擾
    int getCapacity() const { return mCapacity - 1; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �̈�̊m��
    //
    //  @param[in]  capacity_ : ����҂����X�g�̃T�C�Y
    //*/
    //---------------------------------------------------------------------
    void setCapacity( int capacity_ )
    {
        clear();

        SAFE_DELETE_ARRAY( mNodes );

        if ( capacity_ == 0 )
        {
            mCapacity = 0;
            return;
        }

        mCapacity = capacity_ + 1;

        mNodes = LN_NEW Node[ mCapacity + 1 ];


        // �󂫔ԍ��X�^�b�N
	    mIndexStack.setCapacity( mCapacity );
	    for ( int i = 0; i < mCapacity; ++i )
        {
		    mIndexStack.push( i );
	    }

        // mCapacity �Ԃ͍ŏ��̗v�f�̑O���Ō�̗v�f�̌�ɂ���_�~�[�v�f
		mNodes[ mCapacity ].Next = mNodes[ mCapacity ].Prev = mCapacity;
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �N���A
    //
    //  @par
    //              ����҂����X�g�ɓ����Ă�����̂����ׂč폜���܂��B( delete �Ăяo�� )
    //*/
    //---------------------------------------------------------------------
    void clear()
    {
        if ( mNodes && mCapacity > 0 )
        {
		    Node* node = &mNodes[ mCapacity ];	// �_�~�[�m�[�h
		    u32 index = node->Next;             // �_�~�[�m�[�h�̎����J�n�C���f�b�N�X
            int i = 0;

		    while ( true )
		    {
			    // �_�~�[�m�[�h���w���Ă���ꍇ�͏I��
			    if ( index == mCapacity )
			    {
				    break;
			    }
                // �폜
                _release( mNodes[ index ].Value );
                // ����
                index = mNodes[ index ].Next;

                ++i;
		    }

            mNodeMap.clear();

            // �󂫔ԍ��X�^�b�N
	        mIndexStack.clear();
	        for ( int i = 0; i < mCapacity; ++i )
            {
		        mIndexStack.push( i );
	        }

            // �m�[�h�� new �Ƃ����Ă�킯����Ȃ��̂ŁA�_�~�[�̑O�ゾ���t���ς��Ă����΃��X�g�͋�̏�ԂɂȂ�
            mNodes[ mCapacity ].Next = mNodes[ mCapacity ].Prev = mCapacity;
            mSize = 0;
        }
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �I�u�W�F�N�g��ǉ�����
    //
    //  @note
    //              �ǉ������ƁA�Q�ƃJ�E���g���ЂƂ�����B
    //*/
    //---------------------------------------------------------------------
    void addObject( CachedObject* obj_ )
    {
        if ( obj_ && !obj_->_m.Key.isEmpty() && mCapacity > 0 )
        {
            // ���łɒǉ�����Ă��邩���ׂ�


            typename NodeMap::iterator itr = mNodeMap.find( obj_->_m.Key );
            if ( itr != mNodeMap.end() )
            {
                return;
            }

            // �L���b�V���𒲂ׂČÂ����̂�����΍폜
            _checkList();

            int idx = mIndexStack.top();
            mIndexStack.pop();

            Node* dummy     = &mNodes[ mCapacity ];	    // �_�~�[�m�[�h
            Node* prev      = &mNodes[ dummy->Prev ];	// �_�~�[�̂ЂƂO ( �Ō� )
            Node* new_node  = &mNodes[ idx ];           // ���̊Ԃɓ����m�[�h

            // new_node �����X�g�̖����ɒǉ�����
            new_node->Prev = dummy->Prev;
            new_node->Next = mCapacity;
            dummy->Prev = idx;
            prev->Next = idx;

            new_node->Value = obj_;

            mNodeMap.insert( std::pair< SharingKey, Node* >( obj_->_m.Key, new_node ) );

            // �폜�҂���
            obj_->_m.Using = false;
            obj_->addRef();

            ++mSize;
        }
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L�[�Ɉ�v����I�u�W�F�N�g�̌���
    //
    //  @note
    //              �Ԃ����I�u�W�F�N�g�̎Q�ƃJ�E���g�͏�� 1�B
    //*/
    //---------------------------------------------------------------------
    TYPE_* findObject( const SharingKey& key_ )
    {
        if ( mCapacity == 0 ) { return NULL; }

        typename NodeMap::iterator it = mNodeMap.find( key_ );
        if ( it != mNodeMap.end() )
        {
            // �폜�҂����X�g����O��
            Node* n = it->second;
            u32 idx = mNodes[ n->Next ].Prev;   // �����̃C���f�b�N�X
            mNodes[ n->Next ].Prev = n->Prev;
            mNodes[ n->Prev ].Next = n->Next;

            // �󂫃C���f�b�N�X��߂�
            mIndexStack.push( idx );

            TYPE_* v = static_cast< TYPE_* >( it->second->Value );
            
            mNodeMap.erase( it );

            --mSize;

            // �g�p���ɂ���
            v->_m.Using = true;

            return v;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L���b�V���̓��e���o�͂���
    //*/
    //---------------------------------------------------------------------
    void dump( FILE* stream_ );

private:

    /// �L���b�V���̃��X�g�𒲂ׂāA�Â�����(�擪)������ꍇ�͍폜
    void _checkList()
    {
        if ( mSize >= mCapacity - 1 )
        {
            Node* dummy     = &mNodes[ mCapacity ];	    // �_�~�[�m�[�h
			u32 front_index = dummy->Next;
			Node* front     = &mNodes[ front_index ];	// �擪�̃m�[�h���o��

            // front �����X�g����O��
            dummy->Next = front->Next;
            mNodes[ front->Next ].Prev = mCapacity;

            // �󂢂��C���f�b�N�X���X�^�b�N�ɐς�
            mIndexStack.push( front_index );

            // Map ������폜
            mNodeMap.erase( front->Value->_m.Key );

            --mSize;
            
			// �Ȃ��Ǝv�����ǁA�ꉞ front ���_�~�[����Ȃ����`�F�b�N���Ă�����
			if ( front != dummy )
			{
                _release( front->Value );
			}            
        }
    }

    /// �Q�ƃJ�E���g���ւ炷
	void _release( CachedObject* obj_ )
	{
		if ( obj_ )
        {
            obj_->release();
        }
	}

private:

    struct Node
	{
		int             Next;
		int             Prev;
        CachedObject*   Value;
	};

	typedef std::map< SharingKey, Node* >    NodeMap;

private:

    Node*           mNodes;         ///< ����҂����X�g
    int             mCapacity;      ///< �ő�v�f��
    int             mSize;          ///< ���݂̗v�f��
    Stack< int >    mIndexStack;    ///< �󂫔ԍ��X�^�b�N
    NodeMap         mNodeMap;       ///< �L�[�ƃm�[�h�̑Ή��\

};


//=============================================================================
// �� CacheObjectManager �N���X�̎���
//=============================================================================

    //---------------------------------------------------------------------
    // �� �f�t�H���g�R���X�g���N�^
    //---------------------------------------------------------------------
	template< class TYPE_ >
    CacheObjectManager< TYPE_ >::CacheObjectManager()
    {
        _m.IsSharingOnly = true;
        _m.Mutex = NULL;
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    template< class TYPE_ >
    CacheObjectManager< TYPE_ >::~CacheObjectManager()
    {
        LN_SAFE_RELEASE( _m.Mutex );
    }

    //---------------------------------------------------------------------
    // �� ������
    //---------------------------------------------------------------------
    template< class TYPE_ >
    void CacheObjectManager< TYPE_ >::initialize( int capacity_ )
    {
        LN_SAFE_RELEASE( _m.Mutex );
        _m.CacheUnusedList.setCapacity( capacity_ );
        _m.IsSharingOnly = ( capacity_ == 0 );
        _m.Finalized = false;
        _m.Mutex = LN_NEW Thread::Mutex();
    }

    //---------------------------------------------------------------------
    // �� �I������
    //---------------------------------------------------------------------
    template< class TYPE_ >
    void CacheObjectManager< TYPE_ >::finalize()
    {
        if ( !_m.Finalized )
        {
            clear();
            _m.Finalized = true;
        }
    }

    //---------------------------------------------------------------------
    // �� �I�u�W�F�N�g���Ǘ��ɒǉ�����
    //---------------------------------------------------------------------
    template< class TYPE_ >
    void CacheObjectManager< TYPE_ >::registerCachedObject( const SharingKey& key_, TYPE_* obj_ )
    {
        _m.Mutex->lock();

        if ( !key_.isEmpty() && obj_->_m.Key.isEmpty() && !_m.Finalized )
        {
            _m.CacheUsingMap.insert( std::pair< SharingKey, TYPE_* >( key_, obj_ ) );
            obj_->_m.Manager = this;
            obj_->_m.Manager->addRef();
            obj_->_m.Key = key_;
        }

        _m.Mutex->unlock();
    }

    //---------------------------------------------------------------------
    // �� �L�[�Ɉ�v����I�u�W�F�N�g����������
    //---------------------------------------------------------------------
    template< class TYPE_ >
    TYPE_* CacheObjectManager< TYPE_ >::findCacheAddRef( const SharingKey& key_ )
    {
        TYPE_* obj = NULL;

        _m.Mutex->lock();

        if ( !key_.isEmpty() && !_m.Finalized )
        {
            // �܂��͍��g���Ă镨������
            typename std::map< SharingKey, TYPE_* >::iterator itr = _m.CacheUsingMap.find( key_ );
            if ( itr != _m.CacheUsingMap.end() )
            {
                obj = itr->second;
                obj->addRef();
            }
            // �L���b�V���O����ꍇ�͍폜�҂����X�g������
            else if ( !_m.IsSharingOnly )
            {
                obj = _m.CacheUnusedList.findObject( key_ );
                if ( obj )
                {
                    // �g�p���I�u�W�F�N�g�̃}�b�v�ɓ���Ă���
                    _m.CacheUsingMap.insert( std::pair< SharingKey, TYPE_* >( obj->_m.Key, obj ) );
                }
            }
        }

        _m.Mutex->unlock();
        
        return obj;
    }

    //---------------------------------------------------------------------
    // �� ����҂����X�g���N���A����
    //---------------------------------------------------------------------
    template< class TYPE_ >
    void CacheObjectManager< TYPE_ >::clear()
    {
        _m.Mutex->lock();
        _m.CacheUnusedList.clear();
        _m.Mutex->unlock();
    }

    //---------------------------------------------------------------------
    // �� �L���b�V���̓��e���e�L�X�g�t�@�C���ɏo�͂���
    //---------------------------------------------------------------------
    template< class TYPE_ >
    void CacheObjectManager< TYPE_ >::dumpCache( const lnChar* filename_ )
    {
#if defined(LNOTE_MSVC)
        _m.Mutex->lock();

        FILE* stream = _tfopen( filename_, "wt" );

        if ( stream )
        {
            _ftprintf( stream, _T( "================================================================\n" ) );
            _ftprintf( stream, _T( "�� CacheDump\n" ) );
            _ftprintf( stream, _T( "    Type     : %s\n" ), typeid( TYPE_ ).name() );
            _ftprintf( stream, _T( "    Capacity : %d\n" ), _m.CacheUnusedList.getCapacity() );
            _ftprintf( stream, _T( "----------------------------------------------------------------\n" ) );
            _ftprintf( stream, _T( "�� �g�p����Ă���I�u�W�F�N�g ( %d �� )\n" ), _m.CacheUsingMap.size() );

            CachedObject* obj;
            typename std::map< SharingKey, TYPE_* >::iterator itr = _m.CacheUsingMap.begin();
            typename std::map< SharingKey, TYPE_* >::iterator end = _m.CacheUsingMap.end();
            for ( ; itr != end; ++itr )
            {
                obj = itr->second;

                // �|�C���^�ƎQ�ƃJ�E���g
                _ftprintf( stream, _T( "[ %p ] RefCount:%d " ), obj, obj->getRefCount() );

                // �L�[�l
                if ( obj->_m.Key.isString() )
                {
                    _ftprintf( stream, _T( "Key(string):%s\n" ), obj->_m.Key.getKeyString() );
                }
                else
                {
                    _ftprintf( stream, _T( "Key(number):%u\n" ), obj->_m.Key.getKeyHash() );
                }
            }

            // ����҂����X�g�̏o��
            _m.CacheUnusedList.dump( stream );

            _ftprintf( stream, _T( "End CacheDump\n================================================================\n" ) );


            fclose( stream );
        }

        _m.Mutex->unlock();
#endif
    }
    
    //---------------------------------------------------------------------
    // �� CachedObject �폜���O�̃R�[���o�b�N
    //---------------------------------------------------------------------
    template< class TYPE_ >
    void CacheObjectManager< TYPE_ >::_onDeleteCachedObject( CachedObject* obj_ )
    {
        
        // �g�p���̏ꍇ�A�g�p���I�u�W�F�N�g�Ή��\����O���āA
        // �L���b�V���O����ꍇ�͉���҂����X�g�ɒǉ�����
        if ( obj_->_m.Using )
        {
            _m.Mutex->lock();

            _m.CacheUsingMap.erase( obj_->_m.Key );
            if ( !_m.IsSharingOnly )
            {
                _m.CacheUnusedList.addObject( obj_ );
            }

            _m.Mutex->unlock();
        }

    }

//=============================================================================
// �� CacheUnusedList �N���X�̎���
//=============================================================================

    //---------------------------------------------------------------------
    // �� �L���b�V���̓��e���o�͂���
    //---------------------------------------------------------------------
    template< class TYPE_ >
    void CacheObjectManager< TYPE_ >::CacheUnusedList::dump( FILE* stream_ )
    {
        _ftprintf(
            stream_,
            _T( "----------------------------------------------------------------\n" )
            _T( "�� ����҂��̃I�u�W�F�N�g ( %d �� )\n" ),
            mNodeMap.size() );

        CachedObject* obj;
        typename NodeMap::iterator itr = mNodeMap.begin();
        typename NodeMap::iterator end = mNodeMap.end();
        for ( ; itr != end; ++itr )
        {
            obj = itr->second->Value;

            // �|�C���^�ƎQ�ƃJ�E���g
            _ftprintf( stream_, _T( "[ %p ] RefCount:%d " ), obj, obj->getRefCount() );

            // �L�[�l
            if ( obj->_m.Key.isString() )
            {
                _ftprintf( stream_, _T( "Key(string):%s\n" ), obj->_m.Key.getKeyString() );
            }
            else
            {
                _ftprintf( stream_, _T( "Key(number):%u\n" ), obj->_m.Key.getKeyHash() );
            }
        }
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