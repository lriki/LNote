//=============================================================================
//【 GLIndexBuffer 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "LNGL/LNGL.h"
#include "GLGraphicsDevice.h"
#include "GLIndexBuffer.h"

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
// ■ IndexBuffer クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    IndexBuffer::IndexBuffer()
        : mGraphicsDevice   ( NULL )
        , mGLIndexBuffer    ( NULL )
        , mBufferData       ( NULL )
        , mIndexNum         ( 0 )
        , mIndexStride      ( 0 )
        , mUsage            ( 0 )
        , mIsDynamic        ( false )
    {
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    IndexBuffer::~IndexBuffer()
    {
        onDisposeGLResource();

        if (mGraphicsDevice)
        {
            mGraphicsDevice->removeIndexBuffer(this);
        }
        SAFE_DELETE_ARRAY( mBufferData );
        LN_SAFE_RELEASE( mGraphicsDevice );
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT IndexBuffer::initialize( GraphicsDevice* device_, int index_num_, const void* data_, bool is_dynamic_, bool is_16bit_ )
    {
        mGraphicsDevice = device_;
        mGraphicsDevice->addRef();
        mIndexNum = index_num_;

        mIndexStride = ( is_16bit_ ) ? 2 : 4;

        mBufferData = LN_NEW lnByte[mIndexStride * mIndexNum];
        if (data_)
        {
            memcpy(mBufferData, data_, mIndexStride * mIndexNum);
        }
        
        if ( is_dynamic_ )
        {
            mIsDynamic = true;
            mUsage = GL_DYNAMIC_DRAW;
        }
        else
        {
            mIsDynamic = false;
            mUsage = GL_STATIC_DRAW;
        }
        
        return LN_OK;
    }

	//---------------------------------------------------------------------
    // ● ロック
    //---------------------------------------------------------------------
	void* IndexBuffer::lock()
    {
        return mBufferData;
    }

	//---------------------------------------------------------------------
    // ● アンロック
    //---------------------------------------------------------------------
	void IndexBuffer::unlock()
    {
        requestCommitGLResource();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void IndexBuffer::onCommitGLResource()
    {
        if (!mGLIndexBuffer)
        {
            LNGL::glGenBuffers( 1, &mGLIndexBuffer );
            LNGL::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mGLIndexBuffer );
            LNGL::glBufferData( GL_ELEMENT_ARRAY_BUFFER, mIndexStride * mIndexNum, mBufferData, mUsage );
            LNGL::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

            //glGenBuffers( 1, &mGLIndexBuffer );
            //LN_CHECK_GLERROR();

            //glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mGLIndexBuffer );
            //LN_CHECK_GLERROR();
            //
            //glBufferData( GL_ELEMENT_ARRAY_BUFFER, mIndexStride * mIndexNum, mBufferData, mUsage ); // data_ が NULL のときはバッファのみ確保される
            //LN_CHECK_GLERROR();

            //glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
            //LN_CHECK_GLERROR();
        }
        else if (mBufferData)
        {
            LNGL::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mGLIndexBuffer );
            LNGL::glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, mIndexStride * mIndexNum, mBufferData);
            LNGL::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

#if 0
            LNGL::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mGLIndexBuffer );
            void* b = LNGL::glMapBuffer( GL_ELEMENT_ARRAY_BUFFER, LNGL_WRITE_ONLY );

            memcpy(b, mBufferData, mIndexStride * mIndexNum);

            LNGL::glUnmapBuffer( GL_ELEMENT_ARRAY_BUFFER );
            LNGL::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
#endif

            //glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mGLIndexBuffer );
            //LN_CHECK_GLERROR();

            //void* b = PlatformContext::glMapBuffer( GL_ELEMENT_ARRAY_BUFFER, LNGL_WRITE_ONLY );
            //LN_CHECK_GLERROR();

            //memcpy(b, mBufferData, mIndexStride * mIndexNum);

            //PlatformContext::glUnmapBuffer( GL_ELEMENT_ARRAY_BUFFER );
            //LN_CHECK_GLERROR();

            //glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
            //LN_CHECK_GLERROR();
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void IndexBuffer::onDisposeGLResource()
    {
        if ( mGLIndexBuffer )
        {
            LNGL::glDeleteBuffers( 1, &mGLIndexBuffer );
            //glGetError();
            mGLIndexBuffer = NULL;
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