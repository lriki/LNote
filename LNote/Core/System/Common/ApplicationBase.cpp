//=============================================================================
//【 Application 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
//#include "Window.h"
#include "ApplicationBase.h"

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
// ■ MouseCursor クラス
//-----------------------------------------------------------------------------
///**
//  @brief		マウスカーソルの表示管理
//*/
//=============================================================================
class MouseCursor
{
public:

    MouseCursor( ApplicationBase* app_ )
        : mApplication          ( app_ )
        , mMouseVisibleCount    ( 0 )
        , mMouseVisibleCountMax ( 0 )
        , mVisibleMouseCursor   ( true )
        //, mIsSystemMouseShow    ( true )
        , mRequest              ( true )
    {}

public:

    /// マウスカーソルの表示設定 (count は マウスカーソル放置時、消すまでのフレーム数。0 で常に消す)
    void setMouseCursorVisibleState( bool flag_, int count_ )
    {
        mVisibleMouseCursor   = flag_;
        mMouseVisibleCountMax = std::max( 0, count_ );

        // すぐに消す場合
        if ( !mVisibleMouseCursor && mMouseVisibleCountMax == 0 )
        {
            mRequest = false;
        }
        else
        {
            mRequest = true;
        }
    }

    /// フレーム更新
    void update()
    {
        // マウスがしばらく動かなければカーソルを消す処理
		if ( !mVisibleMouseCursor && mMouseVisibleCountMax > 0 )
		{
			if ( mMouseVisibleCount < mMouseVisibleCountMax )
			{
				++mMouseVisibleCount;
				if ( mMouseVisibleCount == mMouseVisibleCountMax )
				{
					//_showCursor( false );
                    mRequest = false;
                    
				}
			}
		}
    }

    /// 表示状態更新 (ワーカースレッドから呼ぶのはこの関数だけ)
    void updateVisibleState()
    {
        mApplication->onShowCursor( mRequest );
    }

    /// マウスカーソルの操作後に呼んで、現在の設定に応じてマウスカーソルの表示・非表示を切り替える
	void onMoveCursor( bool on_client_area_ )
	{
        // 表示しない場合はクライアント領域内でのみ表示しない
        if ( !mVisibleMouseCursor && mMouseVisibleCountMax == 0 )
        {
            if ( on_client_area_ )
            {
                mRequest = false;
            }
            else
            {
                mRequest = true;
                mMouseVisibleCount = 0;
            }
        }
        else
        {
            mRequest = true;
            mMouseVisibleCount = 0;
        }
	}

    

private:

    ApplicationBase* mApplication;
    volatile int     mMouseVisibleCount;
    volatile int     mMouseVisibleCountMax;  ///< マウスカーソル放置時、消すまでのフレーム数
    volatile bool    mVisibleMouseCursor;
    
    volatile bool    mRequest;

};

//=============================================================================
// ■ ApplicationBase クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    ApplicationBase::ApplicationBase() 
        : mLogFile          ( NULL )
		, mManager			( NULL )
        , mMainWindow       ( NULL )
        , mMouseCursor      ( NULL )
        , mFinishedInit     ( false )
        , mEndRequest       ( true )
        , mResultInit       ( LN_OK )
        , mThreaded         ( false )
        , mConsoleAlloced   ( false )
        , mIsSystemMouseShow( true )
    {

    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    ApplicationBase::~ApplicationBase()
    {

    }

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    LNRESULT ApplicationBase::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        mFinishedInit.setFalse();

        LNRESULT lr;

        mInitData = init_data_;
		mManager = mInitData.SystemManager;

        // ユーザー定義のウィンドウハンドルがある場合はダミーウィンドウを作る
        if ( init_data_.WindowHandle )
        {
            //LN_LOG_WRITE( "ユーザー定義のウィンドウハンドルを使用します。" );
            LN_LOG_WRITE( L"    UserHWND : %p", init_data_.WindowHandle );
            
            mMainWindow = onCreateDummyWindow( init_data_.WindowHandle );
        }
        // ウィンドウハンドルを受け取っていない場合は普通に作る
        else
        {
            mThreaded = mInitData.UseThread;
            LN_LOG_WRITE( L"    Threaded : %d", mThreaded );

            if ( mThreaded )
            {
                this->start();

                // 初期化が終わるまで待機する
                mFinishedInit.wait();

                if ( LN_FAILED( mResultInit ) )
                {
                    return mResultInit;
                }
            }
            else
            {
                LN_CALL_R( _initialize() );
            }
        }

        // コンソールを使う場合
        if ( init_data_.UseConsole )
	    {
            onAllocConsole();
        }

        mMouseCursor = LN_NEW MouseCursor( this );

        LN_LOG_INIT_END;
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 終了処理
    //---------------------------------------------------------------------
    void ApplicationBase::finalize()
    {
        LN_LOG_FIN_BEGIN;

        if ( mThreaded )
        {
            mEndRequest.setTrue();
            this->wait();
        }
        else
        {
            _finalize();
        }

        onFreeConsole();

        ln_foreach( Event* e, mEventQueue )
        {
            SAFE_DELETE( e );
        }
        mEventQueue.clear();

        SAFE_DELETE( mMouseCursor );

        LN_LOG_FIN_END;
    }


    //---------------------------------------------------------------------
	// ● マウスカーソルの表示設定 (flag_ が false の場合、count だけ待ってから消す。0ですぐ消す)
    //---------------------------------------------------------------------
    void ApplicationBase::setMouseCursorVisibleState( bool flag_, int count_ )
    {
        if ( mMouseCursor ) mMouseCursor->setMouseCursorVisibleState( flag_, count_ );
    }

    //---------------------------------------------------------------------
	// ● マウスが移動したときに呼び出す
    //---------------------------------------------------------------------
    void ApplicationBase::onMoveCursor( bool on_client_area_ )
    {
        if ( mMouseCursor ) mMouseCursor->onMoveCursor( on_client_area_ );
    }

    //---------------------------------------------------------------------
	// ● 更新
    //---------------------------------------------------------------------
    void ApplicationBase::update()
    {
        if ( !mThreaded )
        {
            _update();
        }
        if ( mMouseCursor ) mMouseCursor->update();
    }

    //---------------------------------------------------------------------
	// ● イベントメッセージをポストする
    //---------------------------------------------------------------------
    void ApplicationBase::postMessage( Event* event_ )
    {
        if ( event_ )
        {
            mLock.lock();
            mEventQueue.push_back( event_ );
            mLock.unlock();
        }
    }

    //---------------------------------------------------------------------
	// ● 処理するべきイベントメッセージがあるかを確認する (true=ある)
    //---------------------------------------------------------------------
    bool ApplicationBase::peekMessage()
    {
        return ( !mEventQueue.empty() );
    }

    //---------------------------------------------------------------------
	// ● 処理するべきイベントメッセージを取得する (取得した場合、キューから削除する )
    //---------------------------------------------------------------------
    const Event* ApplicationBase::getMessage()
    {
        mLock.lock();

        const Event* e = mEventQueue.front();
        mEventQueue.pop_front();

        mLock.unlock();
        return e;
    }

    //---------------------------------------------------------------------
	// ● スレッド関数
    //---------------------------------------------------------------------
    void ApplicationBase::run()
    {
        _initialize();
        while ( !mEndRequest.isTrue() )
        {
            _update();
            Thread::sleep( 16 );
        }
        _finalize();
    }


    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    LNRESULT ApplicationBase::_initialize()
    {
        mEndRequest.setTrue();
        mResultInit = LN_OK;

        mResultInit = onCreateMainWindow( &mMainWindow );

        // 初期化終了
        mFinishedInit.setTrue();

        if ( LN_SUCCEEDED( mResultInit ) )
        {
            mEndRequest.setFalse();
        }
        return mResultInit;
    }

    //---------------------------------------------------------------------
	// ● 終了処理
    //---------------------------------------------------------------------
    void ApplicationBase::_finalize()
    {
        if ( mMainWindow )
        {
            onFinalizeWindow( mMainWindow );
            mMainWindow = NULL;
        }
    }

    //---------------------------------------------------------------------
	// ● 更新
    //---------------------------------------------------------------------
    void ApplicationBase::_update()
    {
        onProcMessage();
        if ( mMouseCursor ) mMouseCursor->updateVisibleState();
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