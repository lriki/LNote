//=============================================================================
//�y Texture �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Common/Image/Image.h"
#include "../Common/BitmapTextRenderer.h"
#include "../Common/GraphicsUtil.h"
#include "../Manager.h"
#include "LNGL/LNGL.h"
#include "GLUtil.h"
#include "GLGraphicsDevice.h"
#include "GLTexture.h"

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
// �� Texture �N���X
//=============================================================================

	//---------------------------------------------------------------------
	// �� �R���X�g���N�^
	//---------------------------------------------------------------------
    Texture::Texture( GraphicsDevice* device_ )
        : mGraphicsDevice   ( device_ )
        , mGLTexture        ( 0 )
        , mMemTexture       ( NULL )
        , mMipLevel         ( 0 )
        , mFormat           ( LN_FMT_A8R8G8B8 )
        , mFont             ( NULL )
        , mColorKey         ( 0 )
	{
        LN_SAFE_ADDREF( mGraphicsDevice );
    }

	//---------------------------------------------------------------------
	// �� �f�X�g���N�^
	//---------------------------------------------------------------------
    Texture::~Texture()
	{
        onDisposeGLResource();

        if (mGraphicsDevice)
        {
            mGraphicsDevice->removeTexture(this);
        }
		
        SAFE_DELETE_ARRAY( mMemTexture );
        LN_SAFE_RELEASE( mFont );
        LN_SAFE_RELEASE( mGraphicsDevice );
	}

    //---------------------------------------------------------------------
    // �� �e�N�X�`���쐬
	//---------------------------------------------------------------------
	void Texture::initialize( lnU32 width_, lnU32 height_, lnU32 levels_, LNSurfaceFormat format_ )
    {
        mSize.x         = static_cast< lnfloat >( width_ );
        mSize.y         = static_cast< lnfloat >( height_ );
        mRealSize.x     = LMath::pow2Paddiing( mSize.x );
        mRealSize.y     = LMath::pow2Paddiing( mSize.y );
        mRealSizeInv.x  = 1.0f / mRealSize.x;
        mRealSizeInv.y  = 1.0f / mRealSize.y;
        mFormat         = format_;
        mMipLevel       = ( levels_ == 0 ) ? 4 : levels_;// TODO:DirectX ���� 0 �̏ꍇ�͑S���x���쐬���邯�ǁA��������Ƃ�������߂�ǂ��Ȃ̂� 
        mMipLevel       = ( levels_ == 1 ) ? 0 : levels_;


        int w = static_cast< GLsizei >( mRealSize.x );
        int h = static_cast< GLsizei >( mRealSize.y );

        mMemTexture = LN_NEW lnU32[w * h];

        memset( mMemTexture, 0xff, sizeof( lnU32 ) * w * h );

        // �f�t�H���g�̃t�H���g���Z�b�g���Ă���
        setFont( mGraphicsDevice->getManager()->getDefaultFont() );
    }

    //---------------------------------------------------------------------
    //
	//---------------------------------------------------------------------
	void Texture::initialize( const lnChar* filename_, u32 color_key_, u32 levels_, LNSurfaceFormat format_ )
    {
        
        mColorKey       = color_key_;
        mFormat         = format_;
        mMipLevel       = ( levels_ == 0 ) ? 4 : levels_;
        mMipLevel       = ( levels_ == 1 ) ? 0 : levels_;

        mFilename = mGraphicsDevice->getFileManager()->getAbsolutePath( filename_ );


        // �T�C�Y�𒲂ׂ邽�߃t�@�C�����J��
        File::IInFile* file = NULL;
        mGraphicsDevice->getFileManager()->createInFile( &file, mFilename.c_str() );

        Image image;
        image.getImageInfo(file);

        mSize.x         = static_cast< lnfloat >( image.Width );
        mSize.y         = static_cast< lnfloat >( image.Height );
        mRealSize.x     = LMath::pow2Paddiing( mSize.x );
        mRealSize.y     = LMath::pow2Paddiing( mSize.y );
        mRealSizeInv.x  = 1.0f / mRealSize.x;
        mRealSizeInv.y  = 1.0f / mRealSize.y;

        LN_SAFE_RELEASE(file);
    }

	//---------------------------------------------------------------------
	// �� �e�N�X�`���쐬 ( �������ɓW�J���ꂽ�摜�f�[�^���� )
	//---------------------------------------------------------------------
    /*
	LNRESULT Texture::initialize( const void* data_, u32 size_, u32 color_key_, u32 levels_, LNSurfaceFormat format_ )
	{
		
	}
    */

    
	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void Texture::clear( const LColor& color_ )
	{
		u32* buf = lock();
        if ( buf )
        {
            u32 c = color_.to32Bit( LN_GRAPHICSAPI_OPENGL );

            int m = static_cast< int >( mRealSize.x * mRealSize.y );
            for ( int i = 0; i < m; ++i )
            {
                buf[ i ] = c;
            }

		    unlock();
        }
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	u32* Texture::lock()
	{
        return mMemTexture;
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void Texture::unlock()
	{
        GLResource::requestCommitGLResource();
        //_refresh();
	}

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    LNRESULT Texture::drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_, int len_ )
    {
        LNRESULT lr;

        Graphics::BitmapTextRenderer tr( LN_GRAPHICSAPI_OPENGL );

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
	//
    //---------------------------------------------------------------------
    LNRESULT Texture::blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ )
    {
        // TODO:�t�H�[�}�b�g�`�F�b�N���K�v


        u32* dest = lock();
        u32* src  = src_texture_->lock();

        const LVector2& src_size = src_texture_->getRealSize();

        Graphics::Util::bitblt(
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

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Texture::onCommitGLResource()
    {
        if ( !mGLTexture )
        {
            LNGL::glGenTextures( 1, &mGLTexture );
            //LN_CHECK_GLERROR();

            LNGL::glBindTexture( GL_TEXTURE_2D, mGLTexture );
            //LN_CHECK_GLERROR();

            // �t�@�C�����w��ō쐬����Ă���ꍇ
            if ( !mFilename.empty() )
            {
                // �t�@�C�����J��
                File::Manager* fm = mGraphicsDevice->getFileManager(); 
                File::IInFile* file = NULL;
                fm->createInFile( &file, mFilename.c_str() );
                
                // �ꎞ�o�b�t�@�ɑS���ǂݍ���
                lnU32 size = file->getSize();
                void* buf = (void*)mGraphicsDevice->getTempBuffer().lock( size + 1 );
                file->read( buf, size );
                
                // �C���[�W�f�[�^�쐬
                //IImage* image = IImage::createImage( buf, size, true );

                Image image;
                image.createFromBytes( buf, size, true );
                
                

                //mSize.cdump();
                //mRealSize.cdump();
                //mRealSizeInv.cdump();

                // �e�N�X�`���쐬
                LNGL::glTexImage2D(
                    GL_TEXTURE_2D,
                    mMipLevel,
                    GL_RGBA,
                    static_cast< GLsizei >( mRealSize.x ),
                    static_cast< GLsizei >( mRealSize.y ),
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    NULL );
                //LN_CHECK_GLERROR();

                LNGL::glTexSubImage2D(
                    GL_TEXTURE_2D,
 	                mMipLevel,
 	                0,
 	                static_cast< GLsizei >( mRealSize.y ) - image.Height,
 	                image.Width,
 	                image.Height,
 	                GL_RGBA,
 	                GL_UNSIGNED_BYTE,
 	                image.ImageData );
                //LN_CHECK_GLERROR();
                

                // �I��
                mGraphicsDevice->getTempBuffer().unlock();
                LN_SAFE_RELEASE( file );
            }
            // �T�C�Y�w��ō쐬����Ă���ꍇ
            else if ( mMemTexture )
            {
                LNGL::glTexImage2D(
                    GL_TEXTURE_2D,
                    mMipLevel,
                    GL_RGBA,
                    static_cast< GLsizei >( mRealSize.x ),
                    static_cast< GLsizei >( mRealSize.y ),
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    NULL );
                //LN_CHECK_GLERROR();
            }

            LNGL::glBindTexture( GL_TEXTURE_2D, 0 );
            //LN_CHECK_GLERROR();
        }
       
        // �R�~�b�g
        if ( mMemTexture )
        {
            LNGL::glBindTexture( GL_TEXTURE_2D, mGLTexture );
            //LN_CHECK_GLERROR();

            LNGL::glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
            //LN_CHECK_GLERROR();

            LNGL::glTexSubImage2D(
                GL_TEXTURE_2D,
                mMipLevel,
                0,
                0,
                static_cast< GLsizei >( mRealSize.x ),
                static_cast< GLsizei >( mRealSize.y ),
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                mMemTexture );
            //LN_CHECK_GLERROR();

            LNGL::glBindTexture( GL_TEXTURE_2D, 0 );
            //LN_CHECK_GLERROR();
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Texture::onDisposeGLResource()
    {
        if ( mGLTexture )
        {
            LNGL::glDeleteTextures( 1, &mGLTexture );
            //glGetError();
            mGLTexture = NULL;
        }
    }


//=============================================================================
// �� RenderTargetTexture �N���X
//=============================================================================

	//---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    RenderTargetTexture::RenderTargetTexture( GraphicsDevice* device_ )
        : mGraphicsDevice   ( device_ )
        , mGLTexture        ( NULL )
        , mMipLevel         ( 1 )
        , mFormat           ( LN_FMT_A8R8G8B8 )
    {
        LN_SAFE_ADDREF( mGraphicsDevice );
    }

	//---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
	RenderTargetTexture::~RenderTargetTexture()
    {
        onDisposeGLResource();

        if ( mGraphicsDevice )
        {
            mGraphicsDevice->removeRenderTarget( this );
        }
        LN_SAFE_RELEASE( mGraphicsDevice );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
	void RenderTargetTexture::initialize( u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ )
    {
        mSize.x         = static_cast< lnfloat >( width_ );
        mSize.y         = static_cast< lnfloat >( height_ );

        if ( mGraphicsDevice->IsPowerOfTwoBufferTexture() )
        {
            mRealSize.x = LMath::pow2Paddiing( mSize.x );
            mRealSize.y = LMath::pow2Paddiing( mSize.y );
        }
        else
        {
            mRealSize.x = mSize.x;
            mRealSize.y = mSize.y;
        }

        mRealSizeInv.x  = 1.0f / mRealSize.x;
        mRealSizeInv.y  = 1.0f / mRealSize.y;
        mFormat         = format_;
        mMipLevel       = ( levels_ == 0 ) ? 4 : levels_;
        mMipLevel       = ( levels_ == 1 ) ? 0 : levels_;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void RenderTargetTexture::onCommitGLResource()
    {
        if (!mGLTexture)
        {
            LNGL::glGenTextures( 1, &mGLTexture );
            //LN_CHECK_GLERROR();
            
            LNGL::glBindTexture( GL_TEXTURE_2D, mGLTexture );
            //LN_CHECK_GLERROR();

            LNGL::glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
            //LN_CHECK_GLERROR();

	        LNGL::glTexImage2D(
                GL_TEXTURE_2D,
                mMipLevel,
                GL_RGBA,
                static_cast< GLsizei >( mRealSize.x ),
                static_cast< GLsizei >( mRealSize.y ),
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                NULL );
            //LN_CHECK_GLERROR();

            // glTexParameteri() ������w�肵�Ȃ���, �e�N�X�`��������Ɏg�p�ł��Ȃ��ꍇ������
            LNGL::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );  //LN_CHECK_GLERROR();
            LNGL::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );  //LN_CHECK_GLERROR();
            LNGL::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );     //LN_CHECK_GLERROR();
            LNGL::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );     //LN_CHECK_GLERROR();
            //LNGL::glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE);

            LNGL::glBindTexture( GL_TEXTURE_2D, 0 );
            //LN_CHECK_GLERROR();
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void RenderTargetTexture::onDisposeGLResource()
    {
        if ( mGLTexture )
        {
            LNGL::glDeleteTextures( 1, &mGLTexture );
            //glGetError();
            mGLTexture = NULL;
        }
    }
	

//=============================================================================
// �� DepthBuffer �N���X
//=============================================================================

	//---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    DepthBuffer::DepthBuffer( GraphicsDevice* device_ )
        : mGraphicsDevice   ( device_ )
        , mGLTexture        ( NULL )
        , mFormat           ( LN_FMT_D24S8 )
    {
        LN_SAFE_ADDREF( mGraphicsDevice );
    }

	//---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    DepthBuffer::~DepthBuffer()
    {
        onLostDevice();

        if ( mGraphicsDevice )
        {
            mGraphicsDevice->removeDepthBuffer( this );
        }
        LN_SAFE_RELEASE( mGraphicsDevice );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
	void DepthBuffer::initialize( u32 width_, u32 height_, LNSurfaceFormat format_ )
    {
        mSize.x         = static_cast< lnfloat >( width_ );
        mSize.y         = static_cast< lnfloat >( height_ );

        if ( mGraphicsDevice->IsPowerOfTwoBufferTexture() )
        {
            mRealSize.x = LMath::pow2Paddiing( mSize.x );
            mRealSize.y = LMath::pow2Paddiing( mSize.y );
        }
        else
        {
            mRealSize.x = mSize.x;
            mRealSize.y = mSize.y;
        }

        mRealSizeInv.x  = 1.0f / mRealSize.x;
        mRealSizeInv.y  = 1.0f / mRealSize.y;
        mFormat         = format_;

        onResetDevice();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void DepthBuffer::onCommitGLResource()
    {
        if (!mGLTexture)
        {
#if 1
            LNGL::glGenRenderbuffers( 1, &mGLTexture );
            //glGenRenderbuffersEXT( 1, &mGLTexture );
            //LN_CHECK_GLERROR();

            LNGL::glBindRenderbuffer( GL_RENDERBUFFER, mGLTexture );
            //glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, mGLTexture );
            //LN_CHECK_GLERROR();
       
            LNGL::glRenderbufferStorage(
            //glRenderbufferStorageEXT(
                GL_RENDERBUFFER,//GL_RENDERBUFFER_EXT,//
                GL_DEPTH_COMPONENT16,//GL_DEPTH_COMPONENT,//
                static_cast< GLsizei >( mRealSize.x ),
                static_cast< GLsizei >( mRealSize.y ) );
            //LN_CHECK_GLERROR();
     
            LNGL::glBindRenderbuffer( GL_RENDERBUFFER, 0 );
            //glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, 0 );
            //LN_CHECK_GLERROR();
#else
            // �f�v�X�e�N�X�`���̐�����DepthAttachment�ւ̊֘A�t��
LNGL::glGenTextures(1, &mGLTexture);
LNGL::glBindTexture(GL_TEXTURE_2D, mGLTexture);
LNGL::glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, static_cast< GLsizei >( mRealSize.x ), static_cast< GLsizei >( mRealSize.y ), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);

LNGL::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
LNGL::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
LNGL::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
LNGL::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#endif
            /*
            
            */
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void DepthBuffer::onDisposeGLResource()
    {
        if ( mGLTexture )
        {
            LNGL::glDeleteRenderbuffers( 1, &mGLTexture );
            //glDeleteRenderbuffersEXT( 1, &mGLTexture );
            //glGetError();
            mGLTexture = NULL;
        }
    }

#if 0
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
        SAFE_RELEASE( mDxTexture );
        SAFE_RELEASE( mDxSurface );

        mDxTexture = texture_;
        mDxSurface = surface_;

        SAFE_ADDREF( mDxTexture );
        SAFE_ADDREF( mDxSurface );

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

#endif

} // namespace OpenGL
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================