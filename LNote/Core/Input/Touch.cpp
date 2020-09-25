//=============================================================================
//�y Touch �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Manager.h"
#include "Touch.h"

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
// �� Touch �N���X
//=============================================================================

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
	//---------------------------------------------------------------------
	Touch::Touch( Manager* manager_ )
		: mManager				( manager_ )
		, mInputDevice			( manager_->getInputDevice() )
		, mFirstRepeatInterval	( 20 )
		, mRemRepeatInterval	( 5 )
	{
		memset( mPressCounts, 0, sizeof( mPressCounts ) );
	}

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^ 
    //---------------------------------------------------------------------
    Touch::~Touch()
    {
    }

	//---------------------------------------------------------------------
	// �� �{�^�������݉�����Ă��邩�𔻒肷��
	//---------------------------------------------------------------------
	bool Touch::isPress( lnU32 id_ )
	{
		if ( id_ < 0 || id_ >= LN_MAX_TOUCH_DATA ) { 
	return false; }
		return ( mPressCounts[ id_ ] > 0 );
	}

	//---------------------------------------------------------------------
	// �� �{�^�����V���������ꂽ���𔻒肷��
	//---------------------------------------------------------------------
	bool Touch::isOnTrigger( lnU32 id_ )
	{
		if ( id_ < 0 || id_ >= LN_MAX_TOUCH_DATA ) { 
	return false; }
		return ( mPressCounts[ id_ ] == 1 );
	}

	//---------------------------------------------------------------------
	// �� �{�^���������ꂽ���𔻒肷��
	//---------------------------------------------------------------------
	bool Touch::isOffTrigger( lnU32 id_ )
	{
		if ( id_ < 0 || id_ >= LN_MAX_TOUCH_DATA ) { 
	return false; }
		return ( mPressCounts[ id_ ] == -1 );
	}

	//---------------------------------------------------------------------
	// �� �{�^�����V���������ꂽ & ���s�[�g��Ԃ𔻒肷��
	//---------------------------------------------------------------------
	bool Touch::isRepeat( lnU32 id_ )
	{
		if ( id_ < 0 || id_ >= LN_MAX_TOUCH_DATA ){ 
	return 0; }
		int state = mPressCounts[ id_ ];
		return ( ( state == 1 )  ||  ( state > mFirstRepeatInterval && state % mRemRepeatInterval == 0 ) );
	}


	//---------------------------------------------------------------------
	// �� ���s�[�g�Ԋu�̐ݒ�
	//---------------------------------------------------------------------
	void Touch::setRepeatInterval( u32 first_, u32 interval_ )
	{
		mFirstRepeatInterval = first_;
		mRemRepeatInterval = interval_;
	}

	//---------------------------------------------------------------------
	// �� �t���[���X�V
	//---------------------------------------------------------------------
	void Touch::update()
	{
        const LNMouseDeviceState& mouse = mInputDevice->getMouseState();

	    mInputDevice->getTouchState( 0, &mTouchState[0] );
	    mInputDevice->getTouchState( 1, &mTouchState[1] );
 
		for ( int i = 0; i < LN_MAX_TOUCH_DATA; ++i )
		{
            if ( mTouchState[ i ].Touch || ( i == 0 && mouse.Buttons[0] ) )
			{
				++mPressCounts[ i ];
				
				if ( mPressCounts[ i ] > mRemRepeatInterval + mFirstRepeatInterval )
				{
					mPressCounts[ i ] = mFirstRepeatInterval;
				}
			}
			else
			{
				if ( mPressCounts[ i ] > 0 )
				{
					mPressCounts[ i ] = -1;
				}
				else
				{
					mPressCounts[ i ] = 0;
				}
			}
		    
            if ( mTouchState[ i ].Touch )
            {
                mPosition[i].x = static_cast< lnfloat >( mTouchState[i].X );
	            mPosition[i].y = static_cast< lnfloat >( mTouchState[i].Y ); 
            }
            else if ( i == 0 && mouse.Buttons[0] )
            {
                mPosition[i].x = static_cast< lnfloat >( mouse.X );
	            mPosition[i].y = static_cast< lnfloat >( mouse.Y ); 
            }

            mPosition[i].transform( mManager->getMouseTransform() );
    		    
	        mDistance[i].x = mPosition[i].x - mLastPosition[i].x;
	        mDistance[i].y = mPosition[i].y - mLastPosition[i].y;
            mLastPosition[i] = mPosition[i];
		}
	}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Input
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================