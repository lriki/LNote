//=============================================================================
//�y Mouse �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Manager.h"
#include "Mouse.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Input
{

//=============================================================================
// �� Mouse �N���X
//=============================================================================

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
	//---------------------------------------------------------------------
	Mouse::Mouse( Manager* manager_ )
		: mManager				( manager_ )
		, mInputDevice			( manager_->getInputDevice() )
		, mFirstRepeatInterval	( 20 )
		, mRemRepeatInterval	( 5 )
        , mIsCenterFix          ( false )

		, mWheelState			( 0 )
		//, mMouseCursorMode		( LN_MOUSEMODE_MOVER_VISIBLE )
		, mMouseVisibleCount	( 0 )
		, mIsSystemMouseShow	( true )
	{
		memset( mButtonStatus, 0, sizeof( mButtonStatus ) );
	}

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^ 
    //---------------------------------------------------------------------
    Mouse::~Mouse()
    {
    }

	//---------------------------------------------------------------------
	// �� �{�^�������݉�����Ă��邩�𔻒肷��
	//---------------------------------------------------------------------
	bool Mouse::isPress( LNMouseButton button_ )
	{
		if ( button_ < 0 || button_ >= MAX_BUTTON_NUM ) { 
	return false; }
		return ( mButtonStatus[ button_ ] > 0 );
	}

	//---------------------------------------------------------------------
	// �� �{�^�����V���������ꂽ���𔻒肷��
	//---------------------------------------------------------------------
	bool Mouse::isOnTrigger( LNMouseButton button_ )
	{
		if ( button_ < 0 || button_ >= MAX_BUTTON_NUM ) { 
	return false; }
		return ( mButtonStatus[ button_ ] == 1 );
	}

	//---------------------------------------------------------------------
	// �� �{�^���������ꂽ���𔻒肷��
	//---------------------------------------------------------------------
	bool Mouse::isOffTrigger( LNMouseButton button_ )
	{
		if ( button_ < 0 || button_ >= MAX_BUTTON_NUM ) { 
	return false; }
		return ( mButtonStatus[ button_ ] == -1 );
	}

	//---------------------------------------------------------------------
	// �� �{�^�����V���������ꂽ & ���s�[�g��Ԃ𔻒肷��
	//---------------------------------------------------------------------
	bool Mouse::isRepeat( LNMouseButton button_ )
	{
		if ( button_ < 0 || button_ >= MAX_BUTTON_NUM ){ 
	return 0; }
		int state = mButtonStatus[ button_ ];
		return ( ( state == 1 )  ||  ( state > mFirstRepeatInterval && state % mRemRepeatInterval == 0 ) );
	}



	//---------------------------------------------------------------------
	// �� ���s�[�g�Ԋu�̐ݒ�
	//---------------------------------------------------------------------
	void Mouse::setRepeatInterval( u32 first_, u32 interval_ )
	{
		mFirstRepeatInterval = first_;
		mRemRepeatInterval = interval_;
	}

	//---------------------------------------------------------------------
	// �� �t���[���X�V
	//---------------------------------------------------------------------
	void Mouse::update()
	{
        const LNMouseDeviceState& state = mInputDevice->getMouseState();

		for ( int i = 0; i < MAX_BUTTON_NUM; ++i )
		{
			if ( state.Buttons[ i ] )
			{
				++mButtonStatus[ i ];
				
				if ( mButtonStatus[ i ] > mRemRepeatInterval + mFirstRepeatInterval )
				{
					mButtonStatus[ i ] = mFirstRepeatInterval;
				}
			}
			else
			{
				if ( mButtonStatus[ i ] > 0 )
				{
					mButtonStatus[ i ] = -1;
				}
				else
				{
					mButtonStatus[ i ] = 0;
				}
			}
		}

	    mPosition.x = static_cast< lnfloat >( state.X );
	    mPosition.y = static_cast< lnfloat >( state.Y );
        mPosition.transform( mManager->getMouseTransform() );

        // ��ʒ����ɃJ�[�\�����Œ肷��ꍇ
		if ( mIsCenterFix )
		{
            if ( mManager->canSetMousePoint() )
            {
                mDistance.x = mPosition.x - mLastPosition.x;
		        mDistance.y = mPosition.y - mLastPosition.y;

			    mInputDevice->setMouseCursorCenter();

                const Geometry::Size& size = mInputDevice->getWindow()->getSize();
                mLastPosition.x = static_cast< lnfloat >( size.w / 2 );
                mLastPosition.y = static_cast< lnfloat >( size.h / 2 );
            }
            else
            {
                mDistance.x = 0;
                mDistance.y = 0;
            }
		}
        else
        {
		    mDistance.x = mPosition.x - mLastPosition.x;
		    mDistance.y = mPosition.y - mLastPosition.y;
            mLastPosition = mPosition;
        }

        mWheelState = state.Wheel;
		
	}

#if 0

	//---------------------------------------------------------------------
	// �� �����炩�̃}�E�X���상�b�Z�[�W������ꍇ�ɌĂ�
	//---------------------------------------------------------------------
	void Mouse::_onOperatedMouse( bool is_client_ )
	{
		_postMoveCursor( is_client_ );
	}

	//---------------------------------------------------------------------
	// �� �}�E�X�J�[�\���̕\����Ԃ�ݒ肷��
	//---------------------------------------------------------------------
	void Mouse::_showCursor( bool flag_ )
	{
		if ( flag_ )
		{
			mMouseVisibleCount = 0;
		}

		// �}�E�X�J�[�\���̕\���͓����ŎQ�ƃJ�E���g�̂悤�Ȃ��̂ŊǗ�����Ă���B
		// http://msdn.microsoft.com/ja-jp/library/cc364876.aspx
		// ���̂��߁A����Ȋ����� if �����܂��Ă����Ȃ��ƁA�J�E���g�������������肵�ĕs����o��B
		if ( flag_ != mIsSystemMouseShow )
		{
			if ( flag_ )
			{
				::ShowCursor( TRUE );
			}
			else
			{
				mMouseVisibleCount = MOUSE_CURSOR_COUNT_MAX;
				::ShowCursor( FALSE );
			}
			mIsSystemMouseShow = flag_;
		}
	}

	//---------------------------------------------------------------------
	// �� �}�E�X�J�[�\���̑����ɌĂ�ŁA���݂̐ݒ�ɉ����ă}�E�X�J�[�\���̕\���E��\����؂�ւ���
	//---------------------------------------------------------------------
	void Mouse::_postMoveCursor( bool is_client_ )
	{
		// ��ɕ\�����Ȃ��ȊO�́A��x�\��������
		if ( mMouseCursorMode != LN_MOUSEMODE_INVISIBLE && mMouseCursorMode != LN_MOUSEMODE_CENTER_FIX )
		{
			_showCursor( true );
		}
		else
		{
			// �\�����Ȃ��ꍇ�̓N���C�A���g�̈���ł̂ݕ\�����Ȃ�
			if ( is_client_ || ( mMouseCursorMode == LN_MOUSEMODE_CENTER_FIX && mInputDevice->isActive() ) )
			{
				_showCursor( false );
			}
			else
			{
				_showCursor( true );
			}
		}
	}
#endif
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Input
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================