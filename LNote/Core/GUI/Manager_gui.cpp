//=============================================================================
//�y Manager �z
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
// �� Manager
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
			// ���݃t�H�[�J�X�������Ă���R���g���[���Ɠ������A
			// �t�H�[�J�X�����ĂȂ��R���g���[���̏ꍇ�͏I��
			if ( ( element_ == mFocusElement ) || ( !element_->canFocus() ) ) { return; }
		}

        GUIElement* prev_focus = mFocusElement;

        if ( mFocusElement )    { mFocusElement->beginCheckActivate(); }
        if ( element_ )         { element_->beginCheckActivate(); }

        // ���݃t�H�[�J�X�������Ă���R���g���[���Ƃ��ċL��
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

		// TODO:IME���̃C�x���g����������
		//if ( m_pIme != NULL )
		//{
		//	if ( m_pIme->OnMouseHoverCheck( m_MousePosition, &mMouseHoverControl ) )
		//	{
		//		goto EXIT;
		//	}
		//}

		// �ʏ�̃E�B���h�E�̃C�x���g����������
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

        // �}�E�X�ʒu�̃R���g���[��������
        updateMouseHover();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    bool Manager::doEvent( const System::Event& e_ )
    {
        //-----------------------------------------------------
        // �}�E�X���상�b�Z�[�W�̏ꍇ
		if ( LNEV_MOUSE_BEGIN <= e_.Type && e_.Type <= LNEV_MOUSE_END )
		{
            const System::MouseEvent& e = (const System::MouseEvent&)e_;

            LVector3 pos( (lnfloat)e.X, (lnfloat)e.Y, 0 );

            // GUI �̍��W��Ԃɕϊ�
            pos.transform( mMousePosTransform );

            // ��ʂ̊O�ɂ͏o���Ȃ��悤�ɂ���
            pos.x = (lnfloat)LMath::limit( e.X, 0, mVirtualScreenSize.w );
            pos.y = (lnfloat)LMath::limit( e.Y, 0, mVirtualScreenSize.h );

            /*
            // Core::System::Window �Ɏ�������ƁA�ЂƂO�̍��W���o���Ă����Ȃ��Ƃ����Ȃ�����
            // ���̊��ɑg�ݍ��ނ̂��ʓ|�ɂȂ�B
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
            
            // checkMouseHover() ���Ŏg�����W���o���Ă���
            mMousePosition.x = (int)pos.x;
			mMousePosition.y = (int)pos.y;

            // �L���v�`�����Ă���R���g���[��������ꍇ�͂���ɑ���
            //      �Ƃ肠�������̌`�ł��������B
            //      D&D���󂯕t���鑤�͋����\���̂��߂� MouseMove ���ׂȂ��Ƃ����Ȃ��Ƃ�
            //      �v�������ǁADragEnter DragLeave �Ń`�F�b�N����΂�����B
            if ( mCaptureElement )
            {
                if ( mCaptureElement->doEvent( gui_e ) ) return true;
            }

			// �}�E�X�ʒu�̃R���g���[���𒲂ׂāA����� mMouseHoverControl �Ɋi�[����
			updateMouseHover();

            if ( mMouseHoverElement )
            {
			    // �}�E�X�{�^���������ꂽ���̃��b�Z�[�W�̏ꍇ�A
                // �R���g���[�����t�H�[�J�X�������Ă��Ȃ��ꍇ�̓t�H�[�J�X����������
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