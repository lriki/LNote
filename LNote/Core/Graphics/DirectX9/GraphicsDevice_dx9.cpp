//=============================================================================
//�y GraphicsDevice �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../File/Manager.h"
#include "DX9Module.h"
#include "Renderer.h"
#include "Canvas.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "GraphicsDevice.h"

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
// �� GraphicsDevice �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    GraphicsDevice::GraphicsDevice()
        : mLogFile                  ( NULL )
        , mRenderer                 ( NULL )
        , mDirect3D                 ( NULL )
        , mDxDevice                 ( NULL )
        , mMainWindow               ( NULL )
        , mFileManager              ( NULL )
        , mCurrentMinFilter         ( D3DTEXF_POINT )
        , mCurrentMagFilter         ( D3DTEXF_POINT )
        , mCurrentMipFilter         ( D3DTEXF_NONE )
        , mBestMinFilter            ( D3DTEXF_POINT )
        , mBestMagFilter            ( D3DTEXF_POINT )
        , mBestMipFilter            ( D3DTEXF_NONE )
        , mMSAAQualityWindowed      ( 0 )
        , mMSAAQualityFullScreen    ( 0 )
        , mDeviceType               ( D3DDEVTYPE_HAL )
        , mDefaultCanvas            ( NULL )
        , mIsDeviceLost             ( true )
        , mIsFullScreen             ( false )
        , mEnableFPUPreserve        ( false )
        , mDeviceLostEventSemaphore ( 1, 1 )
    {
    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    GraphicsDevice::~GraphicsDevice()
    {
        SAFE_RELEASE( mDxDevice );
        SAFE_RELEASE( mDirect3D );
        LN_SAFE_RELEASE( mMainWindow );
        LN_SAFE_RELEASE( mSystemManager );
    }

    //---------------------------------------------------------------------
	// �� ������
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        LNRESULT lr;

        mManager            = init_data_.Manager;
        mEnableFPUPreserve  = init_data_.EnableFPUPreserve;
        mSystemManager      = init_data_.SystemManager;
        mMainWindow         = init_data_.SystemManager->getWindow();
        mFileManager        = init_data_.FileManager;
        LN_SAFE_ADDREF( mSystemManager );
        LN_SAFE_ADDREF( mMainWindow );

        // DLL �ǂݍ���
        LN_CALL_R( DX9Module::getInstance()->initialize() );

        // Direct3D �쐬
        mDirect3D = DX9Module::getInstance()->Direct3DCreate9( D3D_SDK_VERSION );
	    LN_SETERR_R_S_( ( mDirect3D ), LN_ERR_SYSTEM,
            "#Direct3D �̏������Ɏ��s���܂����B\n"
            "DirectX ���������C���X�g�[������Ă��Ȃ����A\n"
            "���g���̃r�f�I�J�[�h�̃h���C�o�[���Ή����Ă��Ȃ��\��������܂��B" );

        // �f�o�C�X�̐��\�`�F�b�N   
        LN_CALL_R( _checkDeviceInformation() );

        // ��{�I�ɕω��̂Ȃ��v���[���e�[�V�����p�����[�^�̐ݒ�
		// ( PresentationInterval �� D3DPRESENT_INTERVAL_IMMEDIATE �ȊO�̏ꍇ�A
		//   DirectShow ���g���ē���Đ��������ɉ�ʂ��\������Ȃ��Ȃ邱�Ƃ����� )
		ZeroMemory( &mPresentParameters, sizeof( D3DPRESENT_PARAMETERS ) );
		mPresentParameters.BackBufferCount				= 1;
		mPresentParameters.EnableAutoDepthStencil		= FALSE;//TRUE;//					// ����͂���Ȃ��B�s��o������ǂ��Ă���
		mPresentParameters.AutoDepthStencilFormat		= D3DFMT_D24S8;//D3DFMT_UNKNOWN;//
		mPresentParameters.SwapEffect					= D3DSWAPEFFECT_DISCARD;	// �}���`�T���v�����O����ɂ̓R���炵��
		mPresentParameters.PresentationInterval			= ( init_data_.EnableVSyncWait ) ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;//D3DPRESENT_INTERVAL_IMMEDIATE(�A�_�v�^�̍X�V�Ԋu����); // 	//D3DPRESENT_INTERVAL_DEFAULT(�A�_�v�^�̍X�V�Ԋu�ɍ��킹��);	// ��ʂ̍X�V�Ԋu
		mPresentParameters.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
        mPresentParameters.hDeviceWindow				= mMainWindow->getWindowHandle();
		//gPresentParameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

        // �f�o�C�X�쐬
        LN_CALL_R( resetDevice( init_data_.FullScreen, mMainWindow->getSize() ) );

        

        // �L���b�V���T�C�Y��ݒ�
        TextureCache::initialize( init_data_.TextureCacheSize );
        ShaderCache::initialize( init_data_.ShaderCacheSize ); 

        // �f�t�H���g�̃L�����o�X (�R���X�g���N�^�Ń��X�g�ɓ������)
        mDefaultCanvas = LN_NEW DefaultCanvas( this );
        LN_CALL_R( mDefaultCanvas->initialize( mMainWindow->getSize() ) );

        // �`��N���X�̍쐬
        Renderer::InitData rend_data;
        rend_data.LogFile           = mLogFile;
        rend_data.GraphicsDevice    = this;
        rend_data.DxCaps            = &mDxCaps;
        mRenderer = LN_NEW Renderer();
        LN_CALL_R( mRenderer->initialize( rend_data ) );

        // Renderer::setRenderTarget() �� mDefaultCanvas ���g���Ă�Ƃ��̓s���ŁA
        // ��L��̃N���X�̏��������I�������ɐݒ肵�Ă����K�v������
        LN_CALL_R( mDefaultCanvas->activate() );

        LN_LOG_INIT_END;
        return LN_OK;
    }


    //---------------------------------------------------------------------
	// �� �I������
    //---------------------------------------------------------------------
    void GraphicsDevice::finalize()
    {
        LN_LOG_FIN_BEGIN;

        LN_SAFE_RELEASE( mDefaultCanvas );
        LN_SAFE_RELEASE( mRenderer );

		

        ShaderCache::finalize();
        TextureCache::finalize();

		
        
        LN_LOG_FIN_END;
    }

    //---------------------------------------------------------------------
    // �� �`��N���X�̎擾
    //---------------------------------------------------------------------
    IRenderer* GraphicsDevice::getRenderer()
    {
        return mRenderer;
    }

    //---------------------------------------------------------------------
    // �� �f�t�H���g�̃o�b�N�o�b�t�@�������L�����o�X�̎擾
    //---------------------------------------------------------------------
    ICanvas* GraphicsDevice::getDefaultCanvas()
    {
        return mDefaultCanvas;
    }

    //---------------------------------------------------------------------
    // �� ���_�o�b�t�@�̍쐬
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createVertexBuffer( IVertexBuffer** obj_, LNVertexElemenst* elements_, u32 vertex_num_, const void* data_, bool is_dynamic_ )
    {
        LNRESULT lr;
        *obj_ = NULL;
            
        VertexBuffer* vb = LN_NEW VertexBuffer();
        LN_CALL_R( vb->initialize( this, elements_, vertex_num_, data_, is_dynamic_ ) );

        *obj_ = vb;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �C���f�b�N�X�o�b�t�@�̍쐬
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createIndexBuffer( IIndexBuffer** obj_, u32 index_num_, const u16* data_, bool is_dynamic_, bool is_16bit_ )
    {
        LNRESULT lr;
        *obj_ = NULL;

        IndexBuffer* ib = LN_NEW IndexBuffer();
        LN_CALL_R( ib->initialize( this, index_num_, data_, is_dynamic_, is_16bit_ ) );

        *obj_ = ib;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �e�N�X�`���̍쐬
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createTexture( ITexture** obj_, u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ )
    {
        LNRESULT lr;
        *obj_ = NULL;

        Texture* tex = LN_NEW Texture( this );
        LN_CALL_R( tex->initialize( width_, height_, levels_, format_ ) );

        *obj_ = tex;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �e�N�X�`���̍쐬 ( �������ɓW�J���ꂽ�摜�f�[�^���� )
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createTexture( ITexture** obj_, const void* data_, u32 size_, u32 color_key_, u32 levels_, LNSurfaceFormat format_, LSharingKey key_ )
    {
        LNRESULT lr;
        *obj_ = NULL;

        // �L���b�V��������
        *obj_ = TextureCache::findCacheAddRef( key_ );
        if ( *obj_ )
        {
            return LN_OK;
        }
        
        // �V�������
        Texture* texture = LN_NEW Texture( this );
        LN_CALL_R( texture->initialize( data_, size_, color_key_, levels_, format_ ) );

        // �L���b�V���Ǘ��N���X�ɒǉ�
        TextureCache::registerCachedObject( key_, texture );
        
        *obj_ = texture;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �e�N�X�`���̍쐬 (���̓X�g���[������)
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createTexture( ITexture** obj_, File::IInStream* stream_, u32 color_key_, u32 levels_, LNSurfaceFormat format_, LSharingKey key_ )
    {
        u32 size = stream_->getSize();
        *obj_ = NULL;

        // TODO:TempBufferLock

        lnByte* buf = mTempBuffer.lock( size );
        stream_->read( buf, size );

        LNRESULT lr = createTexture( obj_, buf, size, color_key_, levels_, format_, key_ );

        mTempBuffer.unlock();
        return lr;
    }

    //---------------------------------------------------------------------
    // �� �e�N�X�`���̍쐬 (�t�@�C������)
    //---------------------------------------------------------------------
    void GraphicsDevice::createTexture( ITexture** obj_, const lnChar* filename_, u32 color_key_, u32 levels_, LNSurfaceFormat format_, LSharingKey key_ )
    {
        // �t�@�C���쐬
        File::IInFile* file = NULL;
        mFileManager->createInFile( &file, filename_ );
        Base::FinallyReleasePtr< File::IInFile > file_ptr( file );

        createTexture( obj_, file, color_key_, levels_, format_, key_ );
    }

    //---------------------------------------------------------------------
    // �� �����_�[�^�[�Q�b�g�e�N�X�`���̍쐬
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createRenderTarget( ITexture** obj_, u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ )
    {
        LNRESULT lr;
        *obj_ = NULL;

        RenderTargetTexture* tex = LN_NEW RenderTargetTexture( this );
        LN_CALL_R( tex->initialize( width_, height_, levels_, format_ ) );

        mRenderTargetList.push_back( tex );

        *obj_ = tex;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �[�x�o�b�t�@�̍쐬
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createDepthBuffer( ITexture** obj_, u32 width_, u32 height_, LNSurfaceFormat format_ )
    {
        LNRESULT lr;
        *obj_ = NULL;

        DepthBuffer* tex = LN_NEW DepthBuffer( this );
        LN_CALL_R( tex->initialize( width_, height_, format_ ) );

        mDepthBufferList.push_back( tex );

        *obj_ = tex;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �V�F�[�_�̍쐬
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createShader( IShader** obj_, const void* data_, u32 size_, LSharingKey key_ )
    {
#if 0
        LNRESULT lr;
        *obj_ = NULL;

        // �L���b�V��������
        *obj_ = ShaderCache::findCacheAddRef( key_ );
        if ( *obj_ )
        {
            return LN_OK;
        }
        
        // �V�������
        Shader* shader = LN_NEW Shader( this );
        LN_CALL_R( shader->initialize( static_cast< const char* >( data_ ) ) );

        // �L���b�V���Ǘ��N���X�ɒǉ�
        ShaderCache::registerCachedObject( key_, shader );

        mShaderList.push_back( shader );
        
        *obj_ = shader;
#endif
        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �V�F�[�_�̍쐬 ( ���̓X�g���[������ )
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createShader( IShader** obj_, File::IInStream* stream_, LSharingKey key_ )
    {
#if 0
        u32 size = stream_->getSize();
        *obj_ = NULL;

        lnByte* buf = mTempBuffer.lock( size + 1 );

        // mTempBuffer �ɑS���ǂݍ���
        stream_->read( buf, size );

        // Cg �ɓn���Ƃ��͏I�[ NULL ���K�v
        buf[ size ] = '\0';

        LNRESULT lr = createShader( obj_, buf, size, key_ );

        mTempBuffer.unlock();
#endif
        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }
	
    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GraphicsDevice::createShader( IShader** obj_, const lnChar* filename_, LSharingKey key_ )
    {
        // �L���b�V��������
        Shader* shader = ShaderCache::findCacheAddRef( key_ );
        if ( shader )
        {
            *obj_ = shader;
            return;
            //return shader;
        }

        // �t�@�C���쐬
        File::IInFile* file = NULL;
        mFileManager->createInFile( &file, filename_ );
        Base::FinallyReleasePtr< File::IInFile > file_ptr( file );

        // mTempBuffer �ɑS���ǂݍ���
        lnU32 size = file->getSize();

        Base::TempBufferLock tlock( mTempBuffer, size + 1 );
        char* buf = (char*)tlock.getBuffer();
        file->read( buf, size );
        buf[ size ] = '\0';

        // Shader �쐬
        Base::FinallyReleasePtr< Shader > shader_ptr( LN_NEW Shader( this ) );
        shader_ptr->initialize( buf );
        if ( shader_ptr->getCompileResult() == LN_SHADERRESULT_ERROR )
        {
			ln_std_tstring str = filename_;
			str += _T("\n");
			str += shader_ptr->getCompileErrors();
			LN_THROW_InvalidCall( 0, str.c_str() );
        }
        

        // �L���b�V���Ǘ��ɒǉ�
        ShaderCache::registerCachedObject( key_, shader_ptr );

        mShaderList.push_back( shader_ptr );

        *obj_ = shader_ptr.returnObject();
        return;
        //return shader_ptr.returnObject();

        //LN_PRINT_NOT_IMPL_FUNCTION;
        //*obj_ = mShaderManager->createShader( filename_, key_ );
    }

    //---------------------------------------------------------------------
    // �� �L���b�V���̃N���A
    //---------------------------------------------------------------------
    void GraphicsDevice::clearCache( bool texture_, bool shader_ )
    {
        if ( texture_ ) TextureCache::clear();
        if ( shader_  ) ShaderCache::clear();
    }

    //---------------------------------------------------------------------
    // �� �f�o�C�X���Z�b�g�O����
    //---------------------------------------------------------------------
    void GraphicsDevice::onLostDevice()
    {
        LN_PCALL( mRenderer->onLostDevice() );
        
        ln_foreach( Shader* t, mShaderList )
        {
            LN_PCALL( t->onLostDevice() );
        }

        ln_foreach( DepthBuffer* t, mDepthBufferList )
        {
            LN_PCALL( t->onLostDevice() );
        }

        ln_foreach( RenderTargetTexture* t, mRenderTargetList )
        {
            LN_PCALL( t->onLostDevice() );
        }

        ln_foreach( CanvasBase* t, mCanvasList )
        {
            LN_PCALL( t->onLostDevice() );
        }
    }

    //---------------------------------------------------------------------
    // �� �f�o�C�X���Z�b�g�㏈��
    //---------------------------------------------------------------------
    void GraphicsDevice::onResetDevice()
    {
        ln_foreach( CanvasBase* t, mCanvasList )
        {
            LN_PCALL( t->onResetDevice() );
        }
        
        ln_foreach( RenderTargetTexture* t, mRenderTargetList )
        {
            LN_PCALL( t->onResetDevice() );
        }
       
        ln_foreach( DepthBuffer* t, mDepthBufferList )
        {
            LN_PCALL( t->onResetDevice() );
        }
        
        ln_foreach( Shader* t, mShaderList )
        {
            LN_PCALL( t->onResetDevice() );
        }

        for ( int i = 0; i < LN_MAX_SAMPLER_STAGE_INDEX; ++i )
        {
            setSamplerState( i, mSamplerStatus[i], true );
        }

        // �r���[�|�[�g�T�C�Y�̃��Z�b�g�Ƀo�b�N�o�b�t�@�T�C�Y���g���Ă��邽�߁A
        // �f�t�H���g�L�����o�X�� onResetDevice() �̌�łȂ���
        // �o�b�N�o�b�t�@�T�C�Y���Â��܂܂ɂȂ��Ă��܂�
        LN_PCALL( mRenderer->onResetDevice() );
    }

    //---------------------------------------------------------------------
    // �� �f�o�C�X�̃��Z�b�g
    //---------------------------------------------------------------------
    void GraphicsDevice::resetDevice()
    {
        LN_PCALL(
            resetDevice( mIsFullScreen, mDefaultCanvas->getSize() ));
    }

    //---------------------------------------------------------------------
    // �� �f�o�C�X�̃��Z�b�g
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::resetDevice( bool fullscreen_, const Geometry::Size& window_bb_size_ )
    {
        HRESULT hr;
        LNRESULT lr;

        // �v���[���e�[�V�����p�����[�^�ݒ�
        mIsFullScreen = ( canFullScreen() ) ? fullscreen_ : false;
		_setPresentParameters( window_bb_size_ );

        // �܂��f�o�C�X���쐬����Ă��Ȃ���ΐV�K�쐬
        if ( !mDxDevice )
        {
            // D3D�f�o�C�X�̐���
            DWORD fpu_precision = ( mEnableFPUPreserve ) ? D3DCREATE_FPU_PRESERVE : 0;
            LN_DXCALL_R(
                mDirect3D->CreateDevice(
				    D3DADAPTER_DEFAULT,
				    mDeviceType,
				    mPresentParameters.hDeviceWindow,
				    fpu_precision | D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,//D3DCREATE_SOFTWARE_VERTEXPROCESSING, | D3DCREATE_MULTITHREADED
				    &mPresentParameters,
				    &mDxDevice ) );
        }
        // ���ɍ쐬����Ă���ꍇ�̓��Z�b�g
        else
        {
            onLostDevice();

            // �f�o�C�X�̃��Z�b�g
            LN_DXCALL_R( mDxDevice->Reset( &mPresentParameters ) );

        //    IDirect3DSurface9* rp;
        //mDxDevice->GetRenderTarget(0, &rp);
        //_p(rp->Release());

        //D3DSURFACE_DESC rrd;
        //rp->GetDesc(&rrd);

            onResetDevice();
        }

        mIsDeviceLost.setFalse();

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // 
    //---------------------------------------------------------------------
    void GraphicsDevice::postDeviceLostEvent()
    {
        if ( !mIsDeviceLost.isTrue() && mDeviceLostEventSemaphore.decrease( 1 ) )
        {
            mIsDeviceLost.setTrue();

            //System::Event* ev = LN_NEW System::Event();
            //ev->Type = LNEV_DEVICE_LOST;
            //mSystemManager->postEvent( ev );

            mDeviceLostEventSemaphore.increase();
        }
    }

    //---------------------------------------------------------------------
    // 
    //---------------------------------------------------------------------
    void GraphicsDevice::setSamplerState( int index_, const LSamplerState& state_, bool reset_ )
    {
        const DWORD filter_table[] = 
        {
            D3DTEXF_NONE,
            D3DTEXF_POINT,
            D3DTEXF_LINEAR,
        };

        const DWORD address_table[] = 
        {
            D3DTADDRESS_WRAP,
            D3DTADDRESS_CLAMP,
        };

        // MagFilter
        if ( state_.MagFilter != mSamplerStatus[index_].MagFilter || reset_ )
        {
            mDxDevice->SetSamplerState( index_, D3DSAMP_MAGFILTER, filter_table[state_.MagFilter] );
            mSamplerStatus[index_].MagFilter = state_.MagFilter;
        }
        // MinFilter
        if ( state_.MinFilter != mSamplerStatus[index_].MinFilter || reset_ )
        {
            mDxDevice->SetSamplerState( index_, D3DSAMP_MINFILTER, filter_table[state_.MinFilter] );
            mSamplerStatus[index_].MinFilter = state_.MinFilter;
        }
        // MipFilter
        if ( state_.MipFilter!= mSamplerStatus[index_].MipFilter || reset_ )
        {
            mDxDevice->SetSamplerState( index_, D3DSAMP_MIPFILTER, filter_table[state_.MipFilter] );
            mSamplerStatus[index_].MipFilter = state_.MipFilter;
        }

        // AddressU
        if ( state_.AddressU!= mSamplerStatus[index_].AddressU || reset_ )
        {
            mDxDevice->SetSamplerState( index_, D3DSAMP_ADDRESSU, address_table[state_.AddressU] );
            mSamplerStatus[index_].AddressU = state_.AddressU;
        }
        // AddressV
        if ( state_.AddressV!= mSamplerStatus[index_].AddressV || reset_ )
        {
            mDxDevice->SetSamplerState( index_, D3DSAMP_ADDRESSV, address_table[state_.AddressV] );
            mSamplerStatus[index_].AddressV = state_.AddressV;
        }
    }

    //---------------------------------------------------------------------
    // �� �f�o�C�X�̐��\�`�F�b�N
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::_checkDeviceInformation()
    {
        // �g����f�o�C�X�̎�ނ𒲂ׂ�
		HRESULT hr = mDirect3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &mDxCaps );
        if ( SUCCEEDED( hr ) )
		{
			mDeviceType = D3DDEVTYPE_HAL;
		}
		else
		{
			hr = mDirect3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, &mDxCaps );
			if ( SUCCEEDED( hr ) )
			{
				mDeviceType = D3DDEVTYPE_REF;
			}
			else
			{
				hr = mDirect3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_SW, &mDxCaps );
				if ( SUCCEEDED( hr ) )
				{
					mDeviceType = D3DDEVTYPE_SW;
				}
				else
				{
					LN_SETDXERR_R_S( hr, "#�g�p�ł���O���t�B�b�N�f�o�C�X��������܂���ł����B" );
				}
			}
		}

        // �f�B�X�v���C�̏����擾
		LN_DXCALL_R( mDirect3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &mDxDisplayMode ) );

        // �A���`�G�C���A�X����ꍇ�̕i��
		// �E�B���h�E���[�h�̂Ƃ��́H
		hr = mDirect3D->CheckDeviceMultiSampleType(
			D3DADAPTER_DEFAULT,  mDeviceType, 
            mDxDisplayMode.Format, TRUE,
			D3DMULTISAMPLE_NONMASKABLE,
			&mMSAAQualityWindowed );
		if ( FAILED( hr ) )
		{
			mMSAAQualityWindowed = 0;
		}
		// �t���X�N���[���̂Ƃ��́H
		hr = mDirect3D->CheckDeviceMultiSampleType(
			D3DADAPTER_DEFAULT,  mDeviceType, 
			mDxDisplayMode.Format, FALSE,
			D3DMULTISAMPLE_NONMASKABLE,
			&mMSAAQualityFullScreen );
		if ( FAILED( hr ) )
		{
			mMSAAQualityFullScreen = 0;
		}

        // ��Ԃ������ۂ��e�N�X�`���t�B���^�擾 ( ���Ƃł����ƒǉ����Ă��� )
		DWORD tfcaps = mDxCaps.TextureFilterCaps;
		// �k��
		if ( tfcaps & D3DPTFILTERCAPS_MIPFLINEAR )
		{
			mBestMipFilter = D3DTEXF_LINEAR;
		}
		else if ( tfcaps & D3DPTFILTERCAPS_MIPFPOINT )
		{
			mBestMipFilter = D3DTEXF_POINT;
		}
		// �g��
		if ( tfcaps & D3DPTFILTERCAPS_MAGFANISOTROPIC )
		{
			mBestMagFilter = D3DTEXF_ANISOTROPIC;
		}
		else if ( tfcaps & D3DPTFILTERCAPS_MAGFLINEAR )
		{
			mBestMagFilter = D3DTEXF_LINEAR;
		}
		// �~�b�v�}�b�v
		if ( tfcaps & D3DPTFILTERCAPS_MINFANISOTROPIC )
		{
			mBestMinFilter = D3DTEXF_ANISOTROPIC;
		}
		else if ( tfcaps & D3DPTFILTERCAPS_MINFLINEAR )
		{
			mBestMinFilter = D3DTEXF_LINEAR;
		}

        LN_LOG_WRITE( "�O���t�B�b�N�f�o�C�X�̏����擾���܂�..." );
        LN_LOG_WRITE( "    �X�N���[���̕�             : %u", mDxDisplayMode.Width );
        LN_LOG_WRITE( "    �X�N���[���̍���           : %u", mDxDisplayMode.Height );
        LN_LOG_WRITE( "    ���t���b�V�����[�g         : %u", mDxDisplayMode.RefreshRate );
        LN_LOG_WRITE( "    �f�B�X�v���C�t�H�[�}�b�g   : %s", DX9Module::getDxFormatString( mDxDisplayMode.Format ) );
        LN_LOG_WRITE( "    �e�N�X�`��" );
        LN_LOG_WRITE( "        �ő剡��               : %u", mDxCaps.MaxTextureWidth );
        LN_LOG_WRITE( "        �ő�c��               : %u", mDxCaps.MaxTextureHeight );
        LN_LOG_WRITE( "        2 �̗ݏ搧��           : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_POW2 ? "����" : "�Ȃ�" );
        LN_LOG_WRITE( "        2 �̗ݏ搧���̌������ : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL ? "����" : "�Ȃ�" );
        LN_LOG_WRITE( "        �����`����             : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY ? "����" : "�Ȃ�" );
        LN_LOG_WRITE( "        ���I�e�N�X�`��         : %s", mDxCaps.Caps2 & D3DCAPS2_DYNAMICTEXTURES ? "��" : "�~" );
        LN_LOG_WRITE( "    �L���[�u�e�N�X�`��         : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP ? "��" : "�~" );
        LN_LOG_WRITE( "        2 �̗ݏ搧��           : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP_POW2 ? "����" : "�Ȃ�" );
        LN_LOG_WRITE( "    �{�����[���e�N�X�`��       : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP ? "��" : "�~" );
        LN_LOG_WRITE( "        2 �̗ݏ搧��           : %s", mDxCaps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP_POW2 ? "����" : "�Ȃ�" );
        LN_LOG_WRITE( "        �ި�ݼޮݍő�l        : %u", mDxCaps.MaxVolumeExtent );
        LN_LOG_WRITE( "    �ٕ����̎���               : %u", mDxCaps.MaxAnisotropy );
        LN_LOG_WRITE( "    �ٕ����t�B���^�g��         : %s", mDxCaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC ? "��" : "�~" );
        LN_LOG_WRITE( "    �ٕ����t�B���^�k��         : %s", mDxCaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC ? "��" : "�~" );
        LN_LOG_WRITE( "    �ő�v���~�e�B�u��         : %u", mDxCaps.MaxPrimitiveCount );
        LN_LOG_WRITE( "    �ő�C���f�b�N�X��         : %u", mDxCaps.MaxVertexIndex );
        LN_LOG_WRITE( "    ���_�V�F�[�_�o�[�W����     : %u.%u", D3DSHADER_VERSION_MAJOR(mDxCaps.VertexShaderVersion), D3DSHADER_VERSION_MINOR(mDxCaps.VertexShaderVersion) );
        LN_LOG_WRITE( "    �s�N�Z���V�F�[�_�o�[�W���� : %u.%u", D3DSHADER_VERSION_MAJOR(mDxCaps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(mDxCaps.PixelShaderVersion) );
        LN_LOG_WRITE( "    �}���`�����_�[�^�[�Q�b�g�� : %u", mDxCaps.NumSimultaneousRTs );
        LN_LOG_WRITE( "    MSAA ���x��(Window)        : %u", mMSAAQualityWindowed );
        LN_LOG_WRITE( "    MSAA ���x��(FullScreen)    : %u", mMSAAQualityFullScreen );

        // �V�F�[�_�̃o�[�W�����`�F�b�N
		LN_SETERR_R_S_( ( mDxCaps.VertexShaderVersion >= D3DVS_VERSION( 2, 0 ) ), LN_ERR_FATAL, "#���g���̊��͒��_�V�F�[�_2.0�ɑΉ����Ă��܂���B" );
		LN_SETERR_R_S_( ( mDxCaps.PixelShaderVersion  >= D3DPS_VERSION( 2, 0 ) ), LN_ERR_FATAL, "#���g���̊��̓s�N�Z���V�F�[�_2.0�ɑΉ����Ă��܂���B" );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �X�N���[�����[�h ( mIsFullScreen ) �ɉ����� mPresentParameters �ݒ�
	//---------------------------------------------------------------------
	void GraphicsDevice::_setPresentParameters( const Geometry::Size& window_bb_size_ )
	{
		// �t���X�N���[�����[�h�̏ꍇ
		if ( mIsFullScreen == true )
		{
            mPresentParameters.Windowed = TRUE;
			mPresentParameters.BackBufferWidth  = mDxDisplayMode.Width; 
			mPresentParameters.BackBufferHeight = mDxDisplayMode.Height;
			mPresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;//D3DFMT_X8B8G8R8;//
			if ( false )//( mMSAAQualityWindowed > 0 )
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
				mPresentParameters.MultiSampleQuality = mMSAAQualityWindowed - 1;
			}
			else
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
				mPresentParameters.MultiSampleQuality = 0;
			}
            /*
			mPresentParameters.Windowed = FALSE;
			mPresentParameters.BackBufferWidth  = mDxDisplayMode.Width; 
            mPresentParameters.BackBufferHeight = mDxDisplayMode.Height;
            mPresentParameters.BackBufferFormat = mDxDisplayMode.Format;
			// �i�����x���� 0 ���傫���ꍇ
			if ( false )//( mMSAAQualityFullScreen > 0 )
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
				mPresentParameters.MultiSampleQuality = mMSAAQualityFullScreen - 1;
			}
			else
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
				mPresentParameters.MultiSampleQuality = 0;
			}
            */
		}
		// �E�B���h�E���[�h�̏ꍇ
		else
		{
			mPresentParameters.Windowed = TRUE;
            mPresentParameters.BackBufferWidth  = window_bb_size_.w; 
            mPresentParameters.BackBufferHeight = window_bb_size_.h;
			mPresentParameters.BackBufferFormat = D3DFMT_X8R8G8B8;//D3DFMT_UNKNOWN;//D3DFMT_X8R8G8B8;//
			if ( false )//( mMSAAQualityWindowed > 0 )
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
				mPresentParameters.MultiSampleQuality = mMSAAQualityWindowed - 1;
			}
			else
			{
				mPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
				mPresentParameters.MultiSampleQuality = 0;
			}
		}
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