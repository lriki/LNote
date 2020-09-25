//=============================================================================
//�y Texture �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Common/BitmapTextRenderer.h"
#include "../Common/GraphicsUtil.h"
#include "../Manager.h"
#include "DX9Module.h"
#include "GraphicsDevice.h"
#include "Texture.h"

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
// �� Texture �N���X
//=============================================================================

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
	//---------------------------------------------------------------------
    Texture::Texture( GraphicsDevice* device_ )
        : mGraphicsDevice   ( device_ )
        , mDxTexture    ( NULL )
        , mDxSurface    ( NULL )
        , mFont         ( NULL )
	{
        LN_SAFE_ADDREF( mGraphicsDevice );
    }

	//---------------------------------------------------------------------
	// �� �f�X�g���N�^
	//---------------------------------------------------------------------
    Texture::~Texture()
	{
		SAFE_RELEASE( mDxSurface );
		SAFE_RELEASE( mDxTexture );
        LN_SAFE_RELEASE( mFont );
        LN_SAFE_RELEASE( mGraphicsDevice );
	}

    //---------------------------------------------------------------------
    // �� �e�N�X�`���쐬
	//---------------------------------------------------------------------
	LNRESULT Texture::initialize( u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ )
    {
        HRESULT hr;

        D3DFORMAT dx_fmt = DX9Module::convertLNFormatToDxFormat( format_ );

        // �T�C�Y�i�[
        UINT w = width_;
        UINT h = height_;
        UINT levels = levels_;
        mSize.x = static_cast< lnfloat >( width_ );
        mSize.y = static_cast< lnfloat >( height_ );

        // ���ۂɍ쐬�����ׂ��e�N�X�`���̏����擾����
        LN_DXCALL_R(
            DX9Module::getInstance()->D3DXCheckTextureRequirements(
				mGraphicsDevice->getDX9Device(),
                &w, &h,
				&levels,        // 0 �̏ꍇ�͍ő吔���i�[�����
                0,
                &dx_fmt,    
                D3DPOOL_MANAGED ) );

        mRealSize.x = static_cast< lnfloat >( w );
        mRealSize.y = static_cast< lnfloat >( h );
        mRealSizeInv.x = 1.0f / mRealSize.x;
        mRealSizeInv.y = 1.0f / mRealSize.y;

		// �e�N�X�`���쐬
		// 3�߂̈��� ( �~�b�v�}�b�v ) �́A�g��Ȃ��ꍇ�� 1 �ɂ��Ă���( 0 �ɂ���Ɖ\�Ȍ���S������� )
		LN_DXCALL_R(
            mGraphicsDevice->getDX9Device()->CreateTexture(
				w, h, levels, 0,
				dx_fmt, D3DPOOL_MANAGED, &mDxTexture, NULL ) );

		// �e�N�X�`���̃T�[�t�F�C�X���擾���Ă���
		LN_DXCALL_R( mDxTexture->GetSurfaceLevel( 0, &mDxSurface ) );

        // �f�t�H���g�̃t�H���g���Z�b�g���Ă���
        setFont( mGraphicsDevice->getManager()->getDefaultFont() );

        return LN_OK;
    }


	//---------------------------------------------------------------------
	// �� �e�N�X�`���쐬 ( �������ɓW�J���ꂽ�摜�f�[�^���� )
	//---------------------------------------------------------------------
	LNRESULT Texture::initialize( const void* data_, u32 size_, u32 color_key_, u32 levels_, LNSurfaceFormat format_ )
	{
		// ����������ǂݍ���
		D3DXIMAGE_INFO image_info;
        HRESULT hr;

        LN_DXCALL_R( DX9Module::getInstance()->D3DXGetImageInfoFromFileInMemory( data_, size_, &image_info ) );

        // �e�N�X�`���̃t�H�[�}�b�g�����߂�
        D3DFORMAT format = ( format_ == LN_FMT_UNKNOWN ) ? image_info.Format : DX9Module::getInstance()->convertLNFormatToDxFormat( format_ );
        
        LN_DXCALL_R(
            DX9Module::getInstance()->D3DXCreateTextureFromFileInMemoryEx(
			    mGraphicsDevice->getDX9Device(), 
                data_, size_,
                //512, 512, // �m�[�g OK
			    //D3DX_DEFAULT_NONPOW2, // �m�[�g NG  �f�X�N�g�b�v OK
                //D3DX_DEFAULT_NONPOW2,
                //D3DX_DEFAULT, // �m�[�g OK  �f�X�N�g�b�v NG
                //D3DX_DEFAULT,
                //0, 0,       // �m�[�g OK  �f�X�N�g�b�v NG
                //300, 300,       // �f�X�N�g�b�v OK
                image_info.Width,
                image_info.Height,
			    levels_,
                0,
			    format,
			    D3DPOOL_MANAGED,
                D3DX_FILTER_NONE,
                D3DX_DEFAULT,
			    color_key_,
			    NULL,
			    NULL,
			    &mDxTexture ) );

		// �����̎��_�� mDxTexture �̎Q�ƃJ�E���g�́u 1 �v

		// �t�@�C���̃C���[�W�̕��ƍ������L��
		mSize.x = static_cast< lnfloat >( image_info.Width );
        mSize.y = static_cast< lnfloat >( image_info.Height );

        UINT levels = levels_;  // 64 bit �Ή�
        
		// ���ۂ̃e�N�X�`���̑傫�����擾
		LN_DXCALL_R(
            DX9Module::getInstance()->D3DXCheckTextureRequirements(
			    mGraphicsDevice->getDX9Device(),
                &image_info.Width, &image_info.Height,
			    &levels, 0, NULL, D3DPOOL_MANAGED ) );

        mRealSize.x = static_cast< lnfloat >( image_info.Width );
        mRealSize.y = static_cast< lnfloat >( image_info.Height );
        mRealSizeInv.x = 1.0f / mRealSize.x;
        mRealSizeInv.y = 1.0f / mRealSize.y;

       
		// �e�N�X�`���̃T�[�t�F�C�X���擾����
		// ( �����Ńe�N�X�`���̃C���^�[�t�F�C�X�̎Q�ƃJ�E���g���ЂƂ����Ă�݂��� )
		LN_DXCALL_R( mDxTexture->GetSurfaceLevel( 0, &mDxSurface ) );

    
		// �����̎��_�� mDxSurface �̎Q�ƃJ�E���g�́u 3 �v
		// �����̎��_�� mDxTexture �̎Q�ƃJ�E���g�́u 2 �v

        // �f�t�H���g�̃t�H���g���Z�b�g���Ă���
        setFont( mGraphicsDevice->getManager()->getDefaultFont() );

		return LN_OK;
	}

    
	//---------------------------------------------------------------------
	// �� �N���A
	//---------------------------------------------------------------------
	void Texture::clear( const LColor& color_ )
	{
		D3DLOCKED_RECT locked_rect;
		HRESULT hr = mDxTexture->LockRect( 0, &locked_rect, NULL, 0 );
		u32* buf = (u32*)locked_rect.pBits;

        u32 c = color_.to32Bit( LN_GRAPHICSAPI_DIRECTX9 );

        int m = static_cast< int >( mRealSize.x * mRealSize.y );
        for ( int i = 0; i < m; ++i )
        {
            buf[ i ] = c;
        }

		mDxTexture->UnlockRect( 0 );
	}

	//---------------------------------------------------------------------
	// �� ���b�N
	//---------------------------------------------------------------------
	u32* Texture::lock()
	{
		D3DLOCKED_RECT locked_rect;
		HRESULT hr = mDxTexture->LockRect( 0, &locked_rect, NULL, 0 );
		return (u32*)locked_rect.pBits;
	}

	//---------------------------------------------------------------------
	// �� �A�����b�N
	//---------------------------------------------------------------------
	void Texture::unlock()
	{
		mDxTexture->UnlockRect( 0 );
	}

    //---------------------------------------------------------------------
	// �� ������̕`��
    //---------------------------------------------------------------------
    LNRESULT Texture::drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_, int len_ )
    {
        LNRESULT lr;

        Graphics::BitmapTextRenderer tr( LN_GRAPHICSAPI_DIRECTX9 );

        if ( mFont )
        {
            tr.setFont( mFont );
        }
        else
        {
            tr.setFont( mGraphicsDevice->getManager()->getDefaultFont() );
        }

        tr.setTargetBuffer(
            lock(), 
            static_cast< u32 >( mRealSize.x ),
            static_cast< u32 >( mRealSize.y ) );

        lr = tr.drawText( text_, rect_, align_, len_ );
        
         unlock();

        return lr;
    }

    //---------------------------------------------------------------------
	// �� �e�N�X�`���̃u���b�N�]��
    //---------------------------------------------------------------------
    LNRESULT Texture::blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ )
    {
        // �t�H�[�}�b�g�`�F�b�N���K�v


        u32* dest = lock();
        u32* src  = src_texture_->lock();

        const LVector2& src_size = src_texture_->getRealSize();

        Util::bitblt(
            x_, y_,
            static_cast< int >( mRealSize.x ),
            static_cast< int >( mRealSize.y ),
            dest,
            static_cast< int >( mRealSize.x ),
            static_cast< int >( mRealSize.y ),
            src_rect_.x, src_rect_.y,
            src_rect_.w, src_rect_.h,
            src,
            static_cast< int >( src_size.x ),
            static_cast< int >( src_size.y ),
            true );

        unlock();
        src_texture_->unlock();
        return LN_OK;
    }

//=============================================================================
// �� RenderTargetTexture �N���X
//=============================================================================

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    RenderTargetTexture::RenderTargetTexture( GraphicsDevice* device_ )
        : mGraphicsDevice   ( device_ )
        , mDxTexture        ( NULL )
	    , mDxSurface        ( NULL )
	    , mDxSystemSurface  ( NULL )
        , mMipLevel         ( 1 )
        , mFormat           ( LN_FMT_A8R8G8B8 )
    {
        LN_SAFE_ADDREF( mGraphicsDevice );
    }

	//---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
	RenderTargetTexture::~RenderTargetTexture()
    {
        _dispose();

        if ( mGraphicsDevice )
        {
            mGraphicsDevice->removeRenderTarget( this );
        }
        LN_SAFE_RELEASE( mGraphicsDevice );
    }

    //---------------------------------------------------------------------
    // �� �����_�[�^�[�Q�b�g�e�N�X�`���̍쐬
    //---------------------------------------------------------------------
	LNRESULT RenderTargetTexture::initialize( u32 width_, u32 height_, u32 level_, LNSurfaceFormat format_ )
    {
        mSize.set( static_cast< lnfloat >( width_ ), static_cast< lnfloat >( height_ ) );
	    mMipLevel = level_;
        mFormat = format_;
	
        return _create();
    }

    //---------------------------------------------------------------------
    // �� ���b�N
    //---------------------------------------------------------------------
	u32* RenderTargetTexture::lock()
    {
        IDirect3DDevice9*   dx_device = mGraphicsDevice->getDX9Device();
        D3DSURFACE_DESC     desc;
		D3DLOCKED_RECT	    locked_rect;

		// �����_�[�e�N�X�`���̃t�H�[�}�b�g���擾
		HRESULT hr = mDxTexture->GetLevelDesc( 0, &desc );
		//LDXASSERT_S( hr, "< LTextureImplRenderTarget::lock >\nIDirect3DTexture9::GetLevelDesc" );

		// �����_�[�e�N�X�`���Ɠ����t�H�[�}�b�g�̃T�[�t�F�C�X���V�X�e���������Ɋm��
		hr = dx_device->CreateOffscreenPlainSurface(
			desc.Width, desc.Height, desc.Format,
			D3DPOOL_SYSTEMMEM, &mDxSystemSurface, NULL );
		//LDXASSERT_S( hr, "< LTextureImplRenderTarget::lock >\nIDirect3DDevice9::CreateOffscreenPlainSurface" );

		// �����_�[�^�Q�b�g�̃T�[�t�F�C�X���V�X�e���������ɓ]��
		hr = dx_device->GetRenderTargetData( mDxSurface, mDxSystemSurface );
		mDxSystemSurface->GetDesc( &desc );
		//LDXASSERT_S( hr, "< LTextureImplRenderTarget::lock >\nIDirect3DDevice9::GetRenderTargetData" );

		hr = mDxSystemSurface->LockRect( &locked_rect, NULL, 0 );
		if ( FAILED( hr ) )
		{
			SAFE_RELEASE( mDxSystemSurface );
			//LDXASSERT_S( hr, "< LTextureImplRenderTarget::lock >\nIDirect3DSurface9::LockRect" );
			return NULL;
		}

		return (u32*)locked_rect.pBits;
    }

    //---------------------------------------------------------------------
    // �� �A�����b�N
    //---------------------------------------------------------------------
	void RenderTargetTexture::unlock()
    {
        if ( mDxSystemSurface )
		{
            IDirect3DDevice9* dx_device = mGraphicsDevice->getDX9Device();

			mDxSystemSurface->UnlockRect();
			dx_device->UpdateSurface( mDxSystemSurface, NULL, mDxSurface, NULL );
			SAFE_RELEASE( mDxSystemSurface );
		}
    }

    //---------------------------------------------------------------------
    // �� �f�o�C�X���Z�b�g���O�̃R�[���o�b�N
    //---------------------------------------------------------------------
    LNRESULT RenderTargetTexture::onLostDevice()
    {
        
        _dispose();
        return LN_OK;
    }

	//---------------------------------------------------------------------
    // �� �f�o�C�X���Z�b�g����̃R�[���o�b�N
    //---------------------------------------------------------------------
    LNRESULT RenderTargetTexture::onResetDevice()
    {
        onLostDevice();
        return _create();
    }

    //---------------------------------------------------------------------
    // �� ���ۂɍ쐬����
    //---------------------------------------------------------------------
    LNRESULT RenderTargetTexture::_create()
    {
		HRESULT hr;
        IDirect3DDevice9* dx_device = mGraphicsDevice->getDX9Device();

        D3DFORMAT dx_fmt = DX9Module::convertLNFormatToDxFormat( mFormat );
        switch ( dx_fmt )
        {
            case D3DFMT_A8B8G8R8: dx_fmt = D3DFMT_A8R8G8B8; break;
            case D3DFMT_X8B8G8R8: dx_fmt = D3DFMT_X8R8G8B8; break;
        }

        // �T�C�Y�i�[
        UINT w = static_cast< UINT >( mSize.x );
        UINT h = static_cast< UINT >( mSize.y );
        UINT levels = mMipLevel;
       /*
        // ���ۂɍ쐬�����ׂ��e�N�X�`���̏����擾����
        LN_DXCALL_R(
            GraphicsUtil::D3DXCheckTextureRequirements(
				dx_device,
                &w, &h,
				&levels,        // 0 �̏ꍇ�͍ő吔���i�[�����
                D3DUSAGE_RENDERTARGET,
                &dx_fmt,    
                D3DPOOL_MANAGED ),
            "Texture::initialize" );
            

        mSize.dump();

        // ���ۂ̒l�i�[
        mRealSize.x = static_cast< lnfloat >( w );
        mRealSize.y = static_cast< lnfloat >( h );
        mMipLevel = levels;
        mFormat = GraphicsUtil::convertFormatDxToLN( dx_fmt );

        mRealSize.dump();
        */

        LN_DXCALLF_R(
            dx_device->CreateTexture(
				w, h,
				levels,
				D3DUSAGE_RENDERTARGET,	// �����_�����O�^�[�Q�b�g�Ƃ��č쐬
                dx_fmt,
				D3DPOOL_DEFAULT,		// �����_�����O�^�[�Q�b�g�� D3DPOOL_DEFAULT
				&mDxTexture,
				NULL ),
            "w=%d h=%d levels=%d dx_fmt=%d\n\n�~�b�v�}�b�v���x���� 1 �ȊO�Ńe�N�X�`���T�C�Y�� 2 �̗ݏ�ł͂Ȃ��ꍇ�A���̃G���[���������邱�Ƃ�����܂��B",
            w, h, levels, dx_fmt );

        // �T�C�Y�`�F�b�N
        D3DSURFACE_DESC desc;
        mDxTexture->GetLevelDesc( 0, &desc );
        //_p( desc.Width );
        //_p( desc.Height );
        mRealSize.x = static_cast< lnfloat >( desc.Width );
        mRealSize.y = static_cast< lnfloat >( desc.Height );
        mRealSizeInv.x = 1.0f / mRealSize.x;
        mRealSizeInv.y = 1.0f / mRealSize.y;

		// �����_�����O�^�[�Q�b�g�̃T�[�t�F�C�X���擾����
		LN_DXCALL_R( mDxTexture->GetSurfaceLevel( 0, &mDxSurface ) );

		// ���Ȃ݂� D3DFMT_R8G8B8 �̓����_�����O�^�[�Q�b�g�ɂ͎g���Ȃ�
     
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �������
    //---------------------------------------------------------------------
    void RenderTargetTexture::_dispose()
    {
        SAFE_RELEASE( mDxSurface );
		SAFE_RELEASE( mDxSystemSurface );
		SAFE_RELEASE( mDxTexture );
    }

//=============================================================================
// �� DepthBuffer �N���X
//=============================================================================

	//---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    DepthBuffer::DepthBuffer( GraphicsDevice* device_ )
        : mGraphicsDevice   ( device_ )
        , mDxSurface        ( NULL )
        , mFormat           ( LN_FMT_D24S8 )
    {
        LN_SAFE_ADDREF( mGraphicsDevice );
    }

	//---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    DepthBuffer::~DepthBuffer()
    {
        _dispose();

        if ( mGraphicsDevice )
        {
            mGraphicsDevice->removeDepthBuffer( this );
        }
        LN_SAFE_RELEASE( mGraphicsDevice );
    }

    //---------------------------------------------------------------------
    // �� �[�x�o�b�t�@�̍쐬
    //---------------------------------------------------------------------
	LNRESULT DepthBuffer::initialize( u32 width_, u32 height_, LNSurfaceFormat format_ )
    {
        mSize.set( static_cast< lnfloat >( width_ ), static_cast< lnfloat >( height_ ) );
        mFormat = format_;
        return _create();
    }

    //---------------------------------------------------------------------
    // �� �f�o�C�X���Z�b�g���O�̃R�[���o�b�N
    //---------------------------------------------------------------------
    LNRESULT DepthBuffer::onLostDevice()
    {
        _dispose();
        return LN_OK;
    }

	//---------------------------------------------------------------------
    // �� �f�o�C�X���Z�b�g����̃R�[���o�b�N
    //---------------------------------------------------------------------
    LNRESULT DepthBuffer::onResetDevice()
    {
        onLostDevice();
        return _create();
    }

    //---------------------------------------------------------------------
    // �� ���ۂɍ쐬����
    //---------------------------------------------------------------------
    LNRESULT DepthBuffer::_create()
    {
        HRESULT hr;
        IDirect3DDevice9* dx_device = mGraphicsDevice->getDX9Device();

        LN_DXCALL_R(
		    dx_device->CreateDepthStencilSurface(
                static_cast< UINT >( mSize.x ),
				static_cast< UINT >( mSize.y ),
				DX9Module::convertLNFormatToDxFormat( mFormat ),
				D3DMULTISAMPLE_NONE,	// ���x�ȕ�ԕ��@�̐ݒ�
				0,						// �摜�̕i�����x���̐ݒ�
				TRUE,					// �[�x�o�b�t�@�̐�����@�̎w��
				&mDxSurface,
				NULL ) );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �������
    //---------------------------------------------------------------------
    void DepthBuffer::_dispose()
    {
        SAFE_RELEASE( mDxSurface );
    }

//=============================================================================
// �� SystemCreatedTexture �N���X
//=============================================================================

	//---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
	SystemCreatedTexture::SystemCreatedTexture()
        : mDxTexture        ( NULL )
        , mDxSurface        ( NULL )
        //, mFont             ( NULL )
    {
    }

	//---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
	SystemCreatedTexture::~SystemCreatedTexture()
    {
        SAFE_RELEASE( mDxTexture );
        SAFE_RELEASE( mDxSurface );
        //SAFE_RELEASE( mDxSystemSurface );
    }

    //---------------------------------------------------------------------
    // �� �e�N�X�`���A�T�[�t�F�C�X�̃Z�b�g
    //---------------------------------------------------------------------
    LNRESULT SystemCreatedTexture::setTextureAndSurface( IDirect3DTexture9* texture_, IDirect3DSurface9* surface_ )
    {
        SAFE_ADDREF( texture_ );
        SAFE_ADDREF( surface_ );

        SAFE_RELEASE( mDxTexture );
        SAFE_RELEASE( mDxSurface );

        mDxTexture = texture_;
        mDxSurface = surface_;

        

        if ( mDxSurface )
        {
            D3DSURFACE_DESC desc;
            mDxSurface->GetDesc( &desc );

            //DX9Module::convertFormatDxToLN

            mRealSize.x = mSize.x = static_cast< lnfloat >( desc.Width );
            mRealSize.y = mSize.y = static_cast< lnfloat >( desc.Height );
            mRealSizeInv.x = 1.0f / mRealSize.x;
            mRealSizeInv.y = 1.0f / mRealSize.y;
        }

        return LN_OK;
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