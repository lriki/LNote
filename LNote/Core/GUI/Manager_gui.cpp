//=============================================================================
//【 Manager 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Common.h"
#include "GUIElement.h"
#include "Controls/GUIDeskTop.h"
#include "Manager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace GUI
{

//=============================================================================
// ■ Manager
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    Manager::Manager()
        : mGraphicsManager      ( NULL )
        , mFocusElement         ( NULL )
        , mMouseHoverElement    ( NULL )
        , mCaptureElement       ( NULL )
        , mDesktop              ( NULL )
        , mDefaultSkinTexture   ( NULL )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::initialize( const InitData& data_ )
    {
        mGraphicsManager = data_.GraphicsManager;
        mVirtualScreenSize = data_.VirtualScreenSize;

        const lnChar* filename = _T( "LNResource\\Image\\GUISkin.png" );
        mGraphicsManager->getGraphicsDevice()->createTexture(
            &mDefaultSkinTexture, filename, 0, 1, LN_FMT_A8R8G8B8, LSharingKey( filename ) );
        

        mDesktop = LN_NEW GUIDesktop( this );
        mDesktop->initialize();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::finalize()
    {
        LN_SAFE_RELEASE( mDesktop );
        LN_SAFE_RELEASE( mDefaultSkinTexture );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::setFocusControl( GUIElement* element_ )
    {
        if ( element_ )
		{
			// 現在フォーカスを持っているコントロールと同じか、
			// フォーカスを持てないコントロールの場合は終了
			if ( ( element_ == mFocusElement ) || ( !element_->canFocus() ) ) { return; }
		}

        GUIElement* prev_focus = mFocusElement;

        if ( mFocusElement )    { mFocusElement->beginCheckActivate(); }
        if ( element_ )         { element_->beginCheckActivate(); }

        // 現在フォーカスを持っているコントロールとして記憶
		mFocusElement = element_;

        if ( prev_focus )       { prev_focus->endCheckActivate( false ); }
        if ( mFocusElement )    { mFocusElement->endCheckActivate( true ); }
    }

    //---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	GUIElement* Manager::getFocusElement()
	{
		if ( mFocusElement )
		{
			return mFocusElement;
		}
		return mDesktop;
	}

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::updateMouseHover()
    {
        GUIElement* old = mMouseHoverElement;

		// TODO:IME側のイベントを処理する
		//if ( m_pIme != NULL )
		//{
		//	if ( m_pIme->OnMouseHoverCheck( m_MousePosition, &mMouseHoverControl ) )
		//	{
		//		goto EXIT;
		//	}
		//}

		// 通常のウィンドウのイベントを処理する
		if ( mDesktop )
		{
			mMouseHoverElement = mDesktop->getMouseHoverElement( mMousePosition );
			if ( mMouseHoverElement )
			{
				goto EXIT;
			}
		}

		mMouseHoverElement = NULL;

	EXIT:

		if ( mMouseHoverElement != old )
		{
            Event e;
            e.Mouse.Button = LN_MOUSE_NONE;
            e.Mouse.X      = mMousePosition.x;
            e.Mouse.Y      = mMousePosition.y;
            e.Mouse.Delta  = 0;

			if ( old )
			{
                e.Type = LN_GUIEV_MOUSE_LEAVE;
                old->doEvent( e );
			}
			if ( mMouseHoverElement )
			{
                e.Type = LN_GUIEV_MOUSE_ENTER;
                mMouseHoverElement->doEvent( e );
			}
		}
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Manager::update( const Core::Game::GameTime& game_time_ )
    {
        //LRect screen( 0, 0, mVirtualScreenSize.w, mVirtualScreenSize.h );
        //mDesktop->updateCoordinate( screen );

        // マウス位置のコントロールを検索
        updateMouseHover();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    bool Manager::doEvent( const System::Event& e_ )
    {
        //-----------------------------------------------------
        // マウス操作メッセージの場合
		if ( LNEV_MOUSE_BEGIN <= e_.Type && e_.Type <= LNEV_MOUSE_END )
		{
            const System::MouseEvent& e = (const System::MouseEvent&)e_;

            LVector3 pos( (lnfloat)e.X, (lnfloat)e.Y, 0 );

            // GUI の座標空間に変換
            pos.transform( mMousePosTransform );

            // 画面の外には出さないようにする
            pos.x = (lnfloat)LMath::limit( e.X, 0, mVirtualScreenSize.w );
            pos.y = (lnfloat)LMath::limit( e.Y, 0, mVirtualScreenSize.h );

            /*
            // Core::System::Window に持たせると、ひとつ前の座標を覚えておかないといけないから
            // 他の環境に組み込むのが面倒になる。
            const_cast< System::MouseEvent& >( e ).X = (int)pos.x;
            const_cast< System::MouseEvent& >( e ).Y = (int)pos.y;
            const_cast< System::MouseEvent& >( e ).MoveX = ( (int)pos.x - mMousePosition.x );
            const_cast< System::MouseEvent& >( e ).MoveY = ( (int)pos.y - mMousePosition.y );
            */

            Event gui_e( (LNGUIEventType)e_.Type );
            gui_e.Mouse.Button  = e.Button;
            gui_e.Mouse.Delta   = e.Delta;
            gui_e.Mouse.X       = (int)pos.x;
            gui_e.Mouse.Y       = (int)pos.y;
            gui_e.Mouse.MoveX   = gui_e.Mouse.X - mMousePosition.x;
            gui_e.Mouse.MoveY   = gui_e.Mouse.Y - mMousePosition.y;
            
            // checkMouseHover() 等で使う座標を覚えておく
            mMousePosition.x = (int)pos.x;
			mMousePosition.y = (int)pos.y;

            // キャプチャしているコントロールがある場合はそれに送る
            //      とりあえずこの形でいいかも。
            //      D&Dを受け付ける側は強調表示のために MouseMove 調べないといけないとか
            //      思ったけど、DragEnter DragLeave でチェックすればいける。
            if ( mCaptureElement )
            {
                if ( mCaptureElement->doEvent( gui_e ) ) return true;
            }

			// マウス位置のコントロールを調べて、あれば mMouseHoverControl に格納する
			updateMouseHover();

            if ( mMouseHoverElement )
            {
			    // マウスボタンが押された時のメッセージの場合、
                // コントロールがフォーカスを持っていない場合はフォーカスを持たせる
			    if ( e_.Type == LNEV_MOUSE_DOWN )
			    {
				    if ( !mMouseHoverElement->hasFocus() )
				    {
					    mMouseHoverElement->focus();
				    }
			    }

                if ( mMouseHoverElement->doEvent( gui_e ) ) return true;
            }
		}

        return false;
    }

    ////---------------------------------------------------------------------
    ////
    ////---------------------------------------------------------------------
    //void Manager::updateDrawContents()
    //{
    //    mDesktop->updateDrawContents();
    //}

    ////---------------------------------------------------------------------
    ////
    ////---------------------------------------------------------------------
    //void Manager::render()
    //{
    //    mDesktop->draw();
    //}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace GUI
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================