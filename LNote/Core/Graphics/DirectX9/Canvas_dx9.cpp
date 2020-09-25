//=============================================================================
//�y Canvas �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "GraphicsDevice.h"
#include "Canvas.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace DirectX9
{

//=============================================================================
// �� CanvasBase �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    CanvasBase::CanvasBase( GraphicsDevice* device_ )
    {
        mGraphicsDevice = device_;
        mGraphicsDevice->addCanvas( this );
        LN_SAFE_ADDREF( mGraphicsDevice );
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    CanvasBase::~CanvasBase()
    {
        if ( mGraphicsDevice )
        {
            mGraphicsDevice->removeCanvas( this );
            LN_SAFE_RELEASE( mGraphicsDevice );
        }
    }

//=============================================================================
// �� DefaultCanvas �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    DefaultCanvas::DefaultCanvas( GraphicsDevice* device_ )
        : CanvasBase            ( device_ )
        , mDxDevice             ( NULL )
        , mBackbuffer           ( NULL )
        , mDepthBuffer          ( NULL )
        , mBackgroundColorCode  ( 0 )
		, mTargetWindowHandle	( NULL )
    {
        
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    DefaultCanvas::~DefaultCanvas()
    {
        LN_SAFE_RELEASE( mBackbuffer );
        LN_SAFE_RELEASE( mDepthBuffer );
    }

    //---------------------------------------------------------------------
    // �� ������
    //---------------------------------------------------------------------
    LNRESULT DefaultCanvas::initialize( const Geometry::Size& size_ )
    {
        LNRESULT lr;

        mDxDevice       = mGraphicsDevice->getDX9Device();

        mBackbuffer = LN_NEW SystemCreatedTexture();

        

        mCanvasSize = size_;

        setWindowSize( size_ );
        
        // �����_�����O�^�[�Q�b�g����Ă����K�v�����邪�A
        // ���͉��z�t���X�N���[������ōs���̂ňꉞOK

        return onResetDevice();
    }

    //---------------------------------------------------------------------
    // �� ���z�X�N���[���T�C�Y��ύX����
    //---------------------------------------------------------------------
    LNRESULT DefaultCanvas::resize( u32 width_, u32 height_ )
    {
        mCanvasSize.set( width_, height_ );

		// �\�t�g�I�Ƀf�o�C�X���X�g��Ԃɂ��āA
		// ����̃��X�g�`�F�b�N���Ƀo�b�N�o�b�t�@����蒼��
        mGraphicsDevice->postDeviceLostEvent();

        setWindowSize( mCanvasSize );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �o�b�N�o�b�t�@�̃����_�����O�^�[�Q�b�g�A�[�x�o�b�t�@��ݒ肷��
    //---------------------------------------------------------------------
    LNRESULT DefaultCanvas::activate()
    {
        LNRESULT lr;

        LN_CALL_R( mGraphicsDevice->getRenderer()->setRenderTarget( 0, mBackbuffer, true ) );
        LN_CALL_R( mGraphicsDevice->getRenderer()->setDepthBuffer( mDepthBuffer ) );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �o�b�N�o�b�t�@�̓��e��]������
    //---------------------------------------------------------------------
    LNRESULT DefaultCanvas::present()
    {
        if ( mGraphicsDevice->isDeviceLost() ) return LN_OK;

        LNRESULT lr;

        // ������RT ���o�b�N�o�b�t�@�ɖ߂��Ă���
        LN_CALL_R( activate() );


        //HRESULT hr = mDxDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffff00ff, 0, 0x00000000 );
        HRESULT hr = mDxDevice->Present( NULL, &mViewDestRect, mTargetWindowHandle, NULL );
        // hr = mDxDevice->Present( NULL, NULL, NULL, NULL );

        // �f�o�C�X���X�g��Ԃ̂Ƃ��͕��A����
        if ( hr == D3DERR_DEVICELOST )
        {
            // �ȑO���̂ւ�� while �Ńt���O�����ɖ߂�̂��`�F�b�N���Ă����ǁA
            // �Ⴆ�΃E�B���h�E����A�N�e�B�u�ɂȂ����̂Ńf�o�C�X���X�g�����Ƃ������ꍇ�A
            // ������x�A�N�e�B�u�ɂ���ɂ̓��b�Z�[�W�������K�v�B
            // �ł��Awhile �ŉ񂵂Ă邾�����Ꮘ�����ł��Ȃ��āA���ʖ������[�v�Ɋׂ��Ă��B

            hr = mDxDevice->TestCooperativeLevel();

		    switch ( hr )
		    {
			    // �f�o�C�X���X�g
		        case D3DERR_DEVICELOST:
			        //::SleepEx( 1000, true );      
			        break;
			    // �f�o�C�X���X�g�F���Z�b�g�\���
		        case D3DERR_DEVICENOTRESET:
                    mGraphicsDevice->postDeviceLostEvent();
			        //LN_CALL_R( mGraphicsDevice->resetDevice( mGraphicsDevice->isFullscreen(), mCanvasSize ) );
			        break;

                default:
                    return LN_ERR_FATAL;
		    }
            
        }

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �`���̃E�B���h�E�T�C�Y��ݒ肷��
    //---------------------------------------------------------------------
    void DefaultCanvas::setWindowSize( const Geometry::Size& size_ )
    { 
        if ( size_.w <= 0 || size_.h <= 0 ) return;

        mWindowSize = size_;

        lnfloat sw = static_cast< lnfloat >( mCanvasSize.w );   // �]����
	    lnfloat sh = static_cast< lnfloat >( mCanvasSize.h );
        lnfloat dw = static_cast< lnfloat >( mWindowSize.w );   // �]����
	    lnfloat dh = static_cast< lnfloat >( mWindowSize.h );

	    lnfloat new_x, new_y;
	    lnfloat new_w, new_h;

        lnfloat ratio_w;
        lnfloat ratio_h;

	    // �o�b�N�o�b�t�@�T�C�Y�ƃX�N���[���T�C�Y�������ꍇ
	    if ( sw == dw && sh == dh )
	    {
		    // ���̂܂ܐݒ�
		    new_x = 0;
		    new_y = 0;
		    new_w = sw;
		    new_h = sh;
            ratio_w = 1.0f;
            ratio_h = 1.0f;
	    }
	    else
	    {
		    // ���݂̃X�N���[���T�C�Y(�f�t�H���g�r���[�|�[�g�̃T�C�Y)�Ɖ�ʃT�C�Y�̔䗦�v�Z
		    ratio_w = dw / sw;
		    ratio_h = dh / sh;

		    // �c�����ɍ��킹�� ( ���E���]�� )
		    if ( ratio_w > ratio_h )
		    {
			    new_w = static_cast< lnfloat >( sw * ratio_h );
			    new_h = static_cast< lnfloat >( dh );
			    new_x = static_cast< lnfloat >( ( dw / 2 ) - ( new_w / 2 ) );
			    new_y = 0;
		    }
		    //�������ɂ��킹��
		    else
		    {
			    new_w = static_cast< lnfloat >( dw );
			    new_h = static_cast< lnfloat >( sh * ratio_w );
			    new_x = 0;
			    new_y = static_cast< lnfloat >( ( dh / 2 ) - ( new_h / 2 ) );
		    }
	    }

        //mViewSrcRect.set( new_x, new_y, new_w, new_h );
        ::SetRect(
            &mViewDestRect,
            static_cast< int >( new_x ),
            static_cast< int >( new_y ),
            static_cast< int >( new_x + new_w ),
            static_cast< int >( new_y + new_h ) );

        mScreenTransformMatrix.identity();
        mScreenTransformMatrix.translation( -new_x, -new_y, 0 );
        mScreenTransformMatrix.scaling( 1.0f / ( new_w / sw ), 1.0f / ( new_h / sh ), 1 );
    }

    //---------------------------------------------------------------------
    // �� �f�o�C�X���Z�b�g���O GraphicsDevice ����Ă΂��
    //---------------------------------------------------------------------
    LNRESULT DefaultCanvas::onLostDevice()
    {
        LN_SAFE_RELEASE( mDepthBuffer );
        return mBackbuffer->setTextureAndSurface( NULL, NULL );
    }

    //---------------------------------------------------------------------
    // �� �f�o�C�X���Z�b�g��� GraphicsDevice ����Ă΂��
    //---------------------------------------------------------------------
    LNRESULT DefaultCanvas::onResetDevice()
    {
        // Framework �Ƃ̘A�g��A2���΂�邱�Ƃ�����̂�
        onLostDevice();

        HRESULT hr;

        IDirect3DSurface9* surface_;
        LN_DXCALL_R( mDxDevice->GetRenderTarget( 0, &surface_ ) );

        LNRESULT lr = mBackbuffer->setTextureAndSurface( NULL, surface_ );  // surface_ �̎Q�ƃJ�E���g��+1
    
        SAFE_RELEASE( surface_ );

        LN_CALL_R( mGraphicsDevice->createDepthBuffer( 
            &mDepthBuffer, mCanvasSize.w, mCanvasSize.h, LN_FMT_D24S8 ) );

        return lr;
    }



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace DirectX9
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================