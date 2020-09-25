//=============================================================================
//【 Manager 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Thread/Thread.h"
#include "../Thread/EventFlag.h"
#include "../Thread/Mutex.h"
#include "Manager.h"

#if defined(LNOTE_WINDOWS)
    #include "Win32/SystemUtil.h"
    #include "Win32/Application.h"
#elif defined(LNOTE_X11)
    #include "X11/Application.h"
#elif defined(LNOTE_ANDROID)
    #include "../../AndroidMain/native_app_glue.h"
    #include <android/native_window.h>
    #include "Android/AndroidWindow.h"
    #include "Android/AndroidApplication.h"
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{


//=============================================================================
// ■ Manager クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    Manager::Manager()
        : mLogFile      ( NULL )
        , mApplication  ( NULL )
        , mEndRequested ( true )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
	Manager::~Manager()
    {
    }


    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    LNRESULT Manager::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        LNRESULT lr;

#if defined(LNOTE_WINDOWS)
        Win32Application::InitData data;
        data.LogFile        = init_data_.LogFile;
		data.SystemManager  = this;
        data.UseConsole     = init_data_.UseConsole;
        data.UseThread      = init_data_.UseThread;
        data.WindowHandle   = init_data_.WindowHandle;
        data.TitleText      = init_data_.WindowTitleText;
        data.Width          = init_data_.WindowWidth;
        data.Height         = init_data_.WindowHeight;
        data.Windowed       = init_data_.Windowed;
        mApplication = LN_NEW Win32Application();
        LN_CALL_R( mApplication->initialize( data ) );
#elif defined(LNOTE_X11)
        X11Application::InitData data;
        data.LogFile        = init_data_.LogFile;
        data.UseConsole     = init_data_.UseConsole;
        data.UseThread      = init_data_.UseThread;
        data.WindowHandle   = init_data_.WindowHandle;
        data.TitleText      = init_data_.WindowTitleText;
        data.Width          = init_data_.WindowWidth;
        data.Height         = init_data_.WindowHeight;
        data.Windowed       = init_data_.Windowed;
        mApplication = LN_NEW X11Application();
        LN_CALL_R( mApplication->initialize( data ) );
#elif defined(LNOTE_ANDROID)
        AndroidApplication::InitData data;
        data.SystemManager  = this;
        data.Width          = init_data_.WindowWidth;
        data.Height         = init_data_.WindowHeight;
        data.AndroidApp     = init_data_.AndroidApp;
        AndroidApplication* app = LN_NEW AndroidApplication();
        app->initialize( data );
        mApplication = app;
#endif

        mApplication->getMainWindow()->setVisible( true );

        mApplication->setMouseCursorVisibleState( false, 60 );

        mEndRequested = false;

        LN_LOG_INIT_END;
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 終了処理
    //---------------------------------------------------------------------
    void Manager::finalize()
    {
        LN_LOG_FIN_BEGIN;

        if ( mApplication )
        {
            mApplication->finalize();
            LN_SAFE_RELEASE( mApplication );
        }

        LN_LOG_FIN_END;
    }

	//---------------------------------------------------------------------
	// ● ウィンドウを取得する
    //---------------------------------------------------------------------
    IWindow* Manager::getWindow()
    {
        return mApplication->getMainWindow();
    }

    //---------------------------------------------------------------------
	// ● メッセージ処理
    //---------------------------------------------------------------------
    bool Manager::processMessage()
    {
        mApplication->update();

        bool r = ( !mEndRequested );
        const Event* ev;
        while ( mApplication->peekMessage() )
        {
            ev = mApplication->getMessage();
            
            if ( ev->Type == LNEV_QUIT )
            {
                r = false;
            }
            else
            {
                lnMsgProc( *ev );
            }

            SAFE_DELETE( ev );
        }
        return r;
    }

    //---------------------------------------------------------------------
	// ● イベントリスナーのアタッチ
    //---------------------------------------------------------------------
    void Manager::attachEventListener( IEventListener* listener_, int priority_ )
    {
        ListenerEntry e;
        e.EventListener = listener_;
        e.Priority = priority_;
        mListenerEntryList.push_back( e );
        mListenerEntryList.sort( _cmpEventListener );
    }

    // 削除用の関数オブジェクト
    class Manager::IsEven
    {
    public:

        IEventListener* mListener;

    public:

        IsEven( IEventListener* listener_ ) : mListener( listener_ ) {}

        bool operator()( const ListenerEntry& e_ ) const
        {
            return e_.EventListener == mListener;
        }
    };

    //---------------------------------------------------------------------
	// ● イベントリスナーのデタッチ
    //---------------------------------------------------------------------
    void Manager::detachEventListener( IEventListener* listener_ )
    {
        mListenerEntryList.remove_if( IsEven( listener_ ) );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::sendEvent( Event* event_ )
    {
        if (event_)
        {
            lnMsgProc( *event_ );
            delete event_;
        }
    }

    //---------------------------------------------------------------------
	// ● イベントをポストする
    //---------------------------------------------------------------------
    void Manager::postEvent( Event* event_ )
    {
        mApplication->postMessage( event_ );
    }


#if defined(LNOTE_WINDOWS)

    /// ウィンドウメッセージの処理
    LRESULT Manager::WndProc( HWND hwnd_, UINT msg_, WPARAM wparam_, LPARAM lparam_, bool* handled_ )
    {
		ListenerEntryList::iterator itr = mListenerEntryList.begin();
        ListenerEntryList::iterator end = mListenerEntryList.end();
        for ( ; itr != end; ++itr )
        {
			LRESULT r = (*itr).EventListener->WndProc( hwnd_, msg_, wparam_, lparam_, handled_ );
            if ( *handled_ )
            {
                return r;
            }
        }
        return 0;
    }

#endif

    //---------------------------------------------------------------------
	// ● メッセージコールバック
    //---------------------------------------------------------------------
	bool Manager::lnMsgProc( const Event& args_ )
    {
        bool r;
        ListenerEntryList::iterator itr = mListenerEntryList.begin();
        ListenerEntryList::iterator end = mListenerEntryList.end();
        for ( ; itr != end; ++itr )
        {
            r = (*itr).EventListener->onEvent( args_ );
            if ( r )
            {
                return true;
            }
        }
        

        switch ( args_.Type )
        {
            
            /*
            /////////////////////////////////////////////// マウスが移動した
            case LNEV_MOUSE_MOVE:
            {
                
                if ( 0 <= args_.Mouse.X && 0 <= args_.Mouse.Y )
                {
                    mMouseCursor->onMoveCursor( true );
                }
                else
                {
                    mMouseCursor->onMoveCursor( false );
                }
                break;
            }*/

            /////////////////////////////////////////////// 終了する場合
            case LNEV_CLOSE:
            {
                mEndRequested = true;
                mApplication->postMessage( createEvent< Event >( LNEV_QUIT ) );
                break;
            }

            default:
                break;

        }

        return false;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace System
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================