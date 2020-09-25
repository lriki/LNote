//=============================================================================
//【 LFFramework 】
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "FuncLibManager.h"
#include "LFStdAfx.h"
#include "LFFramework.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
LNOTE_FUNC_NAMESPACE_BEGIN

//=============================================================================
// LConfig
//=============================================================================

    //---------------------------------------------------------------------
    // 
    //---------------------------------------------------------------------
    void LConfig_SetEnableLogFile( bool flag )
    {
        Manager::ConfigData.EnableMakeLogFile = flag;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LConfig_SetEnableConsole( bool flag )
    {
        Manager::ConfigData.EnableConsole = flag;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LConfig_SetUserWindowHandle( void* window_handle )
    {
        Manager::ConfigData.WindowHandle = window_handle;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LConfig_SetDirectMusicUsingMode( LNDirectMusicUsingMode mode )
    {
        Manager::ConfigData.DMInitMode = mode;
    }

//=============================================================================
// LNote
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    HRESULT LNote_InitAudio()
    {
        LN_FUNC_CALL_THROW( Manager::initializeAudio() );
    }

    //---------------------------------------------------------------------
    // 
    //---------------------------------------------------------------------
    void LNote_End()
    {
        Manager::finalize();
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
LNOTE_FUNC_NAMESPACE_END

#if 0

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
LNOTE_FUNC_NAMESPACE_BEGIN


//=============================================================================
// ■ LFramework
//=============================================================================

ConfigData gConfigData;
lnU32 gComponentInitFlag = LN_USECOMP_ALL;
Framework* gFramework = NULL;

LNote::Core::Graphics::IGraphicsDevice* gGraphicsDevice = NULL;
LNote::Core::Graphics::IRenderer* gRenderer = NULL;

//---------------------------------------------------------------------
// ライブラリの初期化
//---------------------------------------------------------------------
LNRESULT Lumine_Init()
{
    LNRESULT lr;

    Framework* fw = createFramework();

    try
    {

        LN_CALL_R( fw->initialize( gConfigData ) );

        if ( gComponentInitFlag & LN_USECOMP_FILE )
        {
            fw->initializeFileManager();
        }
        if ( gComponentInitFlag & LN_USECOMP_SYSTEM )
        {
            LN_CALL_R( fw->initializeSystemManager() );
        }
        if ( gComponentInitFlag & LN_USECOMP_INPUT )
        {
            LN_CALL_R( fw->initializeInputManager() );
        }
        if ( gComponentInitFlag & LN_USECOMP_AUDIO )
        {
            LN_CALL_R( fw->initializeAudioManager() );
        }
        if ( gComponentInitFlag & LN_USECOMP_PHYSICS )
        {
            LN_CALL_R( fw->initializePhysicsManager() );
        }
        if ( gComponentInitFlag & LN_USECOMP_GRAPHICS )
        {
            fw->initializeGraphicsManager();
            gGraphicsDevice = fw->getGraphicsManager()->getGraphicsDevice();
            gRenderer       = fw->getGraphicsManager()->getGraphicsDevice()->getRenderer();
        }
        
        if ( gComponentInitFlag & LN_USECOMP_GUI )
        {
            LN_PRINT_NOT_IMPL_FUNCTION
        }
        if ( gComponentInitFlag & LN_USECOMP_SCENE )
        {
            LN_CALL_R( fw->initializeSceneManager() );
        }
        if ( gComponentInitFlag & LN_USECOMP_GAME )
        {
            LN_PRINT_NOT_IMPL_FUNCTION
        }
    }
    catch ( Core::Base::Exception e )
    {
        fw->finalize();
        fw->release();
        return e.getErrorCode();
    }

    gFramework = fw;
    return LN_OK;
}

//---------------------------------------------------------------------
// ライブラリの終了処理
//---------------------------------------------------------------------
void Lumine_End()
{
    if ( gFramework )
    {
        gFramework->finalize();
        LN_SAFE_RELEASE( gFramework );
    }
}

//---------------------------------------------------------------------
// ライブラリ全体の更新処理
//---------------------------------------------------------------------
bool Lumine_Update()
{
    return gFramework->update();
}

//---------------------------------------------------------------------
// Framework へのポインタの取得
//---------------------------------------------------------------------
LNote::Core::Framework* Lumine_GetFramework()
{
    return gFramework;
}

//=============================================================================
// ■ LConfig
//=============================================================================
   
//---------------------------------------------------------------------
// 初期化する機能の選択 (デフォルトではすべて初期化する)
//---------------------------------------------------------------------
void LConfig_SetComponentInitFlag( int flags_ )
{
    gComponentInitFlag = flags_;
}

//---------------------------------------------------------------------
// ライブラリ全般の基本的な初期化情報の設定
//---------------------------------------------------------------------
void LConfig_SetFrameworkData(
    bool            enable_make_logfile_,
    LNUpdateMode    update_mode_,
    bool            forbid_auto_fullscreen_,
    int             frame_rate_ )
{
    gConfigData.EnableMakeLogFile = enable_make_logfile_;
    gConfigData.UpdateMode = update_mode_;
    gConfigData.ForbidAutoFullscreen = forbid_auto_fullscreen_;
    gConfigData.FrameRate = frame_rate_;
}

//---------------------------------------------------------------------
// OS関係の基本的な初期化情報の設定
//---------------------------------------------------------------------
void LConfig_SetSystemData(
    const lnChar*   window_title_,
    int             window_width_,
    int             window_height_,
    void*           window_handle_ )
{
    gConfigData.WindowTitle = window_title_;
    gConfigData.WindowWidth = window_width_;
    gConfigData.WindowHeight = window_height_;
    gConfigData.WindowHandle = window_handle_;
}

//---------------------------------------------------------------------
// アーカイブファイルの追加
//---------------------------------------------------------------------
void LConfig_AddArchive( const lnChar* directory_, const lnChar* filename_, const char* key_ )
{
    gConfigData.addArchive( directory_, filename_, key_ );
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

LNOTE_FUNC_NAMESPACE_END

#endif

//=============================================================================
//                              end of file
//=============================================================================