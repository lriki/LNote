//=============================================================================
//【 Gateway 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../../Core/Thread/EventFlag.h"
#include "../../../Core/Thread/Thread.h"
#include "../../../Core/System/Manager.h"
#include "../../../Core/Framework/Framework.h"
#include "../../GameScene/GameSceneManager.h"
#include "../Accessor.h"
#include "Gateway.h"



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
    static bool gInited = false;

    //---------------------------------------------------------------------
    // ● 初期化
	//---------------------------------------------------------------------
	int initialize()
    {
        if ( gInited ) { return -1; }

        // wchar_t で日本語を使えるようにする
        //setlocale( LC_ALL, "" );

        // エラー処理モジュール初期化
        //Base::Error::initialize();

        gInited = true;

        return 0;
    }

    //---------------------------------------------------------------------
    // ●終了処理
	//---------------------------------------------------------------------
	void finalize()
    {
        // 終了処理
        gFramework->finalize();
        //Base::Error::finalize();

        LN_SAFE_RELEASE( gFramework );

        gInited = false;
    }
   
    //---------------------------------------------------------------------
    // ●
	//---------------------------------------------------------------------
	int run( MD_lnMain ln_main_, MD_lnConfig ln_config_ )
    {
        if ( !ln_main_ ) { return -1; }

        int r = -1;
        LNRESULT lr;

        // Framework の初期化データを作る
        Core::ConfigData* init_data = LN_NEW Core::ConfigData();
        
        // LConfig と lnConfig を使って初期化データ設定
        LConfig config( init_data );
        if ( ln_config_ )
        {
            ln_config_( config );
        }
        /*
        if ( init_data->Failed )
        {
            SAFE_DELETE( init_data );
		    return -1;
        }
        */

        initialize();

        // Framework を初期化する
        //Framework* fw = Framework::getInstance();
        gFramework = LN_NEW Core::Framework();
        lr = gFramework->initialize( *init_data );
        LN_PCALL( lr );

        // 設定はもう使わないので解放
        SAFE_DELETE( init_data );

        gGameSceneManager = LN_NEW Utility::GameSceneManager();
        Utility::GameSceneManager::InitData sdata;
        sdata.LogFile = NULL;
        sdata.Framework = gFramework;
        gGameSceneManager->initialize( sdata );

        if ( LN_SUCCEEDED( lr ) && lr != LN_FALSE )
        {
            // Accessor 初期化
            Accessor::initializeLNGlobalAccessor();

            // main 呼び出し
            r = ln_main_();

            Accessor::finalizeLNGlobalAccessor();
           
            // 終了処理
            //gFramework->finalize();
        }

        gGameSceneManager->finalize();
        LN_SAFE_RELEASE( gGameSceneManager );
        
        finalize();

        return r;
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================