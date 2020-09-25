//=============================================================================
//�y GLGraphicsDevice �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../System/Manager.h"
#include "../../File/Manager.h"
#include "LNGL/LNGL.h"
#include "GLRenderer.h"
#include "GLCanvas.h"
#include "GLVertexBuffer.h"
#include "GLIndexBuffer.h"
#include "GLTexture.h"
#include "GLShaderManager.h"
#include "GLShader.h"
#include "GLGraphicsDevice.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace OpenGL
{

//=============================================================================
// �� GraphicsDevice �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    GraphicsDevice::GraphicsDevice()
        : mLogFile              ( NULL )
        , mSystemManager        ( NULL )
        , mFileManager          ( NULL )
        , mRenderer             ( NULL )
        , mShaderManager        ( NULL )
        //, mPlatformContext      ( NULL )
        , mDefaultCanvas        ( NULL )
        , mCurrentShaderrPass   ( NULL )
        , mIsDeviceLost         ( false )
    {
    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    GraphicsDevice::~GraphicsDevice()
    {
        /*SAFE_DELETE( mPlatformContext );*/

        LNGL::finalize();
    }

    //---------------------------------------------------------------------
	// �� ������
    //---------------------------------------------------------------------
    void GraphicsDevice::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        LNGL::InitData lngl_data;
        lngl_data.LogFile       = mLogFile;
        lngl_data.SystemManager = init_data_.SystemManager;
        LNGL::initialize(lngl_data);
        


        mManager        = init_data_.Manager;
        mFileManager    = init_data_.FileManager;


        
        //-----------------------------------------------------
        //


        // �L���b�V���T�C�Y��ݒ�
        TextureCache::initialize( init_data_.TextureCacheSize );

        // �f�t�H���g�̃L�����o�X (�R���X�g���N�^�Ń��X�g�ɓ������)
        DefaultCanvas::InitData c_data;
        //c_data.PlatformContext  = mPlatformContext;
        c_data.Window           = init_data_.SystemManager->getWindow();
        c_data.CanvasSize       = init_data_.CanvasSize;
        mDefaultCanvas = LN_NEW DefaultCanvas( this );
        mDefaultCanvas->initialize( c_data );

        // �`��N���X�̍쐬
        Renderer::InitData rend_data;
        rend_data.LogFile           = mLogFile;
        rend_data.GraphicsDevice    = this;
        //rend_data.PlatformContext   = mPlatformContext;
        mRenderer = LN_NEW Renderer();
        mRenderer->initialize( rend_data );

        // �V�F�[�_�Ǘ��N���X
        ShaderManager::InitData sm_data;
        sm_data.LogFile             = mLogFile;
        sm_data.GraphicsDevice      = this;
        sm_data.ShaderCacheSize     = init_data_.ShaderCacheSize;
        mShaderManager = LN_NEW ShaderManager();
        mShaderManager->initialize( sm_data );



        

        //mIsDeviceLost = true;   // ���Z�b�g�����s�����邽�߁A��x true �ɂ���
        //onResetDevice();

        LN_LOG_INIT_END;
    }


    //---------------------------------------------------------------------
	// �� �I������
    //---------------------------------------------------------------------
    void GraphicsDevice::finalize()
    {
        LN_LOG_FIN_BEGIN;

        onLostDevice();

        LN_SAFE_RELEASE( mCurrentShaderrPass );

        
        LN_SAFE_RELEASE( mRenderer );

        // ~Renderer() �� onLostDevice() �Ŏg�p���Ă���̂�Renderer �̌�ŊJ������
        LN_SAFE_RELEASE( mDefaultCanvas );  

        if ( mShaderManager )
        {
            mShaderManager->finalize();
            LN_SAFE_RELEASE( mShaderManager );
        }

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
        *obj_ = NULL;
            
        VertexBuffer* vb = LN_NEW VertexBuffer();
        vb->initialize( this, elements_, vertex_num_, data_, is_dynamic_ );

        mVertexBufferList.push_back(vb);
        *obj_ = vb;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �C���f�b�N�X�o�b�t�@�̍쐬
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createIndexBuffer( IIndexBuffer** obj_, u32 index_num_, const u16* data_, bool is_dynamic_, bool is_16bit_ )
    {
        *obj_ = NULL;

        IndexBuffer* ib = LN_NEW IndexBuffer();
        ib->initialize( this, index_num_, data_, is_dynamic_, is_16bit_ );

        mIndexBufferList.push_back(ib);
        *obj_ = ib;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �e�N�X�`���̍쐬
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createTexture( ITexture** obj_, u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ )
    {
        *obj_ = NULL;
        Texture* tex = LN_NEW Texture( this );
        tex->initialize( width_, height_, levels_, format_ );
        *obj_ = tex;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �e�N�X�`���̍쐬 ( �������ɓW�J���ꂽ�摜�f�[�^���� )
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createTexture( ITexture** obj_, const void* data_, u32 size_, u32 color_key_, u32 levels_, LNSurfaceFormat format_, LSharingKey key_ )
    {
#if 0
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
#endif
        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �e�N�X�`���̍쐬 (���̓X�g���[������)
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createTexture( ITexture** obj_, File::IInStream* stream_, u32 color_key_, u32 levels_, LNSurfaceFormat format_, LSharingKey key_ )
    {
#if 0
        u32 size = stream_->getSize();
        *obj_ = NULL;

        lnByte* buf = mTempBuffer.lock( size );
        stream_->read( buf, size );

        LNRESULT lr = createTexture( obj_, buf, size, color_key_, levels_, format_, key_ );

        mTempBuffer.unlock();
        return lr;
#endif
        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }
    
    //---------------------------------------------------------------------
    // �� �e�N�X�`���̍쐬 (�t�@�C������)
    //---------------------------------------------------------------------
    void GraphicsDevice::createTexture( ITexture** obj_, const lnChar* filename_, u32 color_key_, u32 levels_, LNSurfaceFormat format_, LSharingKey key_ )
    {
        *obj_ = NULL;
        Texture* tex = LN_NEW Texture( this );
        tex->initialize( filename_, color_key_, levels_, format_ );
        *obj_ = tex;

        mTextureList.push_back(tex);
    }

    //---------------------------------------------------------------------
    // �� �����_�[�^�[�Q�b�g�e�N�X�`���̍쐬
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createRenderTarget( ITexture** obj_, u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ )
    {
        *obj_ = NULL;
        RenderTargetTexture* tex = LN_NEW RenderTargetTexture( this );
        tex->initialize( width_, height_, levels_, format_ );
        mRenderTargetList.push_back( tex );
        *obj_ = tex;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �[�x�o�b�t�@�̍쐬
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::createDepthBuffer( ITexture** obj_, u32 width_, u32 height_, LNSurfaceFormat format_ )
    {
        *obj_ = NULL;
        DepthBuffer* tex = LN_NEW DepthBuffer( this );
        tex->initialize( width_, height_, format_ );
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
        return LN_OK;
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
        return lr;
#endif
        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }
	
    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GraphicsDevice::createShader( IShader** obj_, const lnChar* filename_, LSharingKey key_ )
    {
        *obj_ = mShaderManager->createShader( filename_, key_ );
    }

    //---------------------------------------------------------------------
    // �� �L���b�V���̃N���A
    //---------------------------------------------------------------------
    void GraphicsDevice::clearCache( bool texture_, bool shader_ )
    {
        if ( texture_ ) TextureCache::clear();
        if ( shader_  ) mShaderManager->clearCache();
    }
    
    //---------------------------------------------------------------------
    // �� �f�o�C�X���Z�b�g�O����
    //---------------------------------------------------------------------
    void GraphicsDevice::onLostDevice()
    {
        if (!mIsDeviceLost)
        {
            LN_LOG_WRITE(_T("GraphicsDevice::onLostDevice() begin"));

            ln_foreach(VertexBuffer* t, mVertexBufferList)
            {
                t->onLostDevice();
            }
            ln_foreach(IndexBuffer* t, mIndexBufferList)
            {
                t->onLostDevice();
            }
            ln_foreach(Texture* t, mTextureList)
            {
                t->onLostDevice();
            }
            ln_foreach(RenderTargetTexture* t, mRenderTargetList)
            {
                t->onLostDevice();
            }
            ln_foreach(DepthBuffer* t, mDepthBufferList)
            {
                t->onLostDevice();
            }          

            if (mShaderManager) mShaderManager->onLostDevice();
            if (mRenderer) mRenderer->onLostDevice();
            if (mDefaultCanvas) mDefaultCanvas->onLostDevice();
            mIsDeviceLost = true;

            LN_LOG_WRITE(_T("GraphicsDevice::onLostDevice() end"));
        }
    }

    //---------------------------------------------------------------------
    // �� �f�o�C�X���Z�b�g�㏈��
    //---------------------------------------------------------------------
    void GraphicsDevice::onResetDevice()
    {
        if (mIsDeviceLost)
        {
            LN_LOG_WRITE(_T("GraphicsDevice::onResetDevice() begin"));

            mDefaultCanvas->onResetDevice();
            mRenderer->onResetDevice();
            mShaderManager->onResetDevice();

            ln_foreach(DepthBuffer* t, mDepthBufferList)
            {
                t->onResetDevice();
            } 
            ln_foreach(RenderTargetTexture* t, mRenderTargetList)
            {
                t->onResetDevice();
            }
            ln_foreach(Texture* t, mTextureList)
            {
                t->onResetDevice();
            }
            ln_foreach(IndexBuffer* t, mIndexBufferList)
            {
                t->onResetDevice();
            }
            ln_foreach(VertexBuffer* t, mVertexBufferList)
            {
                t->onResetDevice();
            }
            
            // Renderer::setRenderTarget() �� mDefaultCanvas ���g���Ă�Ƃ��̓s���ŁA
            // mDefaultCanvas�AmRenderer �̏��������I�������ɐݒ肵�Ă����K�v������
            //mDefaultCanvas->activate();

            mIsDeviceLost = true;

            LN_LOG_WRITE(_T("GraphicsDevice::onResetDevice() end"));
        }
    }


    //---------------------------------------------------------------------
    // �� �f�o�C�X�̃��Z�b�g
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::resetDevice( bool fullscreen_, const Geometry::Size& window_bb_size_ )
    {
#if 0
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
            LN_DXCALL_R( _onLostDevice() );

            // �f�o�C�X�̃��Z�b�g
            LN_DXCALL_R( mDxDevice->Reset( &mPresentParameters ) );

            LN_DXCALL_R( _onResetDevice() );
        }

        return LN_OK;
#endif
        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GraphicsDevice::commitChangesGLResource()
    {
        ln_foreach(VertexBuffer* t, mVertexBufferList)
        {
            t->commitChanges();
        }
        ln_foreach(IndexBuffer* t, mIndexBufferList)
        {
            t->commitChanges();
        }
        ln_foreach(Texture* t, mTextureList)
        {
            t->commitChanges();
        }
        ln_foreach(RenderTargetTexture* t, mRenderTargetList)
        {
            t->commitChanges();
        }
        ln_foreach(DepthBuffer* t, mDepthBufferList)
        {
            t->commitChanges();
        }     
        mShaderManager->commitChangesGLResource();
    }

    //---------------------------------------------------------------------
    // �� �f�o�C�X�̐��\�`�F�b�N
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::_checkDeviceInformation()
    {

        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }

 
    //---------------------------------------------------------------------
    // �� �f�o�C�X���Z�b�g�O����
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::_onLostDevice()
    {

        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �f�o�C�X���Z�b�g�㏈��
    //---------------------------------------------------------------------
    LNRESULT GraphicsDevice::_onResetDevice()
    {
#if 0
        LNRESULT lr;

        ln_foreach( Shader* t, mShaderList )
        {
            LN_CALL_R( t->onResetDevice() );
        }

        ln_foreach( DepthBuffer* t, mDepthBufferList )
        {
            LN_CALL_R( t->onResetDevice() );
        }

        ln_foreach( RenderTargetTexture* t, mRenderTargetList )
        {
            LN_CALL_R( t->onResetDevice() );
        }
       
        ln_foreach( CanvasBase* t, mCanvasList )
        {
            LN_CALL_R( t->onResetDevice() );
        }

        // �r���[�|�[�g�T�C�Y�̃��Z�b�g�Ƀo�b�N�o�b�t�@�T�C�Y���g���Ă��邽�߁A
        // �f�t�H���g�L�����o�X�� onResetDevice() �̌�łȂ���
        // �o�b�N�o�b�t�@�T�C�Y���Â��܂܂ɂȂ��Ă��܂�
        LN_CALL_R( mRenderer->onResetDevice() );

        // TODO ���Z�b�g��R�[���o�b�N�Ăяo��
        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
#endif
        LN_PRINT_NOT_IMPL_FUNCTION;
        
        
        
        
        
        
        return LN_OK;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GraphicsDevice::setCurrentShaderrPass( ShaderPass* pass_ )
    {
        if ( pass_ != mCurrentShaderrPass )
        {
            LN_SAFE_RELEASE( mCurrentShaderrPass );
            mCurrentShaderrPass = pass_;
            LN_SAFE_ADDREF( mCurrentShaderrPass );
        }
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace OpenGL
} // namespace Graphics
} // namespace Core

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================