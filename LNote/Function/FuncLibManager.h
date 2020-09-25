//=============================================================================
// FuncLibManager
//-----------------------------------------------------------------------------
///**
//  @file       FuncLibManager.h
//  @brief      FuncLibManager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "LFCommon.h"
#include "../Core/Framework/Framework.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Function
{

template < class T >
class ReleaseList
{
public:

    ~ReleaseList() { clear(); }

public:

    void add( T* p ) { if ( p ) mList.push_back( p ); }

    void release( T* p ) { mList.remove( p ); LN_SAFE_RELEASE( p ); }

    void clear()
    {
        ln_foreach(T* p, mList)
        {
            p->release();
        }
        mList.clear();
    }

private:

    std::list< T* > mList;
};

//=============================================================================
// Manager
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class Manager
{
public:

    /// ������
    static void initialize();

    /// �����@�\�̂ݏ�����
    static void initializeAudio();

    /// �I������
    static void finalize();

    /// ��O����
    static LNResult procException( Base::Exception& e );

    /// �Ō�ɔ���������O�R�[�h�̎擾
    static LNResult getLastErrorCode();

public:

    static ConfigData       ConfigData;
    static Framework*       Framework;
    static Audio::Manager*  AudioManager;

public:

    static ReleaseList< Audio::ISound > SoundList;

private:

    static Base::ExceptionManager   mExceptionManager;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Function
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================