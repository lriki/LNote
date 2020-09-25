﻿//=============================================================================
// FuncLibManager
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "FuncLibManager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Function
{

//=============================================================================
// Manager
//=============================================================================

    ConfigData      Manager::ConfigData;
    Framework*      Manager::Framework = NULL;
    Audio::Manager* Manager::AudioManager = NULL;

    ReleaseList< Audio::ISound > Manager::SoundList;

    Base::ExceptionManager  Manager::mExceptionManager;

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::initialize()
    {
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::initializeAudio()
    {
        Framework = LN_NEW Core::Framework();
        Framework->initializeForAudioLibrary( ConfigData );
        AudioManager = Framework->getAudioManager();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::finalize()
    {
        SoundList.clear();

        if ( Framework )
        {
            Framework->finalize();
            LN_SAFE_RELEASE( Framework );
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    LNResult Manager::procException( Base::Exception& e )
    {
        mExceptionManager.procException( e );
        return e.getErrorCode();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    LNResult Manager::getLastErrorCode()
    {
        return mExceptionManager.getLastException().getErrorCode();
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Function
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================