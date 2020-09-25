//=============================================================================
//【 GLVertexBuffer 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "LNGL/LNGL.h"
#include "GLGraphicsDevice.h"
#include "GLVertexBuffer.h"

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
// ■ VertexBuffer クラス
//=============================================================================


    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    VertexBuffer::VertexBuffer()
        : mGraphicsDevice   ( NULL )
        , mGLVertexBuffer   ( 0 )
        , mLNGLAttribData   ( NULL )
        , mBufferData       ( NULL )
        , mVertexNum        ( 0 )
        , mVertexStride     ( 0 )
        , mUsage            ( 0 )
        , mIsDynamic        ( false )
    {
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    VertexBuffer::~VertexBuffer()
    {
        onDisposeGLResource();

        if (mGraphicsDevice)
        {
            mGraphicsDevice->removeVertexBuffer(this);
        }
        SAFE_DELETE_ARRAY( mBufferData );
		SAFE_DELETE_ARRAY( mLNGLAttribData );
        LN_SAFE_RELEASE( mGraphicsDevice );
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT VertexBuffer::initialize( GraphicsDevice* device_, LNVertexElemenst* elements_, lnU32 vertex_num_, const void* data_, bool is_dynamic_ )
    {
        mGraphicsDevice = device_;
        mGraphicsDevice->addRef();
        mVertexNum = vertex_num_;

        mLNGLAttribData = Util::createVertexAttribData( elements_ );

        // 頂点ひとつ分のサイズ
		mVertexStride = mLNGLAttribData[0].Stride;

        mBufferData = LN_NEW lnByte[mVertexStride * mVertexNum];
        if (data_)
        {
            memcpy(mBufferData, data_, mVertexStride * mVertexNum);
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
	void* VertexBuffer::lock()
	{
        return mBufferData;
	}

	//---------------------------------------------------------------------
    // ● アンロック
	//---------------------------------------------------------------------
	void VertexBuffer::unlock()
	{
        requestCommitGLResource();
	}

    //---------------------------------------------------------------------
    //
	//---------------------------------------------------------------------
    void VertexBuffer::onCommitGLResource()
    {
        if (!mGLVertexBuffer)
        {
            LNGL::glGenBuffers( 1, &mGLVertexBuffer );
            LNGL::glBindBuffer( GL_ARRAY_BUFFER, mGLVertexBuffer );
            LNGL::glBufferData( GL_ARRAY_BUFFER, mVertexStride * mVertexNum, mBufferData, mUsage ); 
            LNGL::glBindBuffer( GL_ARRAY_BUFFER, 0 );

            //glGenBuffers( 1, &mGLVertexBuffer );
            //LN_CHECK_GLERROR();
            //
            //glBindBuffer( GL_ARRAY_BUFFER, mGLVertexBuffer );
            //LN_CHECK_GLERROR();
            //
            //glBufferData( GL_ARRAY_BUFFER, mVertexStride * mVertexNum, mBufferData, mUsage ); // data_ が NULL のときはバッファのみ確保される
            //LN_CHECK_GLERROR();

            //glBindBuffer( GL_ARRAY_BUFFER, 0 );
            //LN_CHECK_GLERROR();
        }
        else if (mBufferData)
        {
            LNGL::glBindBuffer( GL_ARRAY_BUFFER, mGLVertexBuffer );
            LNGL::glBufferSubData( GL_ARRAY_BUFFER, 0, mVertexStride * mVertexNum, mBufferData);
            LNGL::glBindBuffer( GL_ARRAY_BUFFER, 0 );
#if 0
            LNGL::glBindBuffer( GL_ARRAY_BUFFER, mGLVertexBuffer );
            void* b = LNGL::glMapBuffer( GL_ARRAY_BUFFER, LNGL_WRITE_ONLY );

            memcpy(b, mBufferData, mVertexStride * mVertexNum);

            LNGL::glUnmapBuffer( GL_ARRAY_BUFFER );
            LNGL::glBindBuffer( GL_ARRAY_BUFFER, 0 );
#endif
            //glBindBuffer( GL_ARRAY_BUFFER, mGLVertexBuffer );
            //LN_CHECK_GLERROR();

            //void* b = PlatformContext::glMapBuffer( GL_ARRAY_BUFFER, LNGL_WRITE_ONLY );
            //LN_CHECK_GLERROR();

            //memcpy(b, mBufferData, mVertexStride * mVertexNum);

            //PlatformContext::glUnmapBuffer( GL_ARRAY_BUFFER );
            //LN_CHECK_GLERROR();

            //glBindBuffer( GL_ARRAY_BUFFER, 0 );
            //LN_CHECK_GLERROR();
        }
    }

    //---------------------------------------------------------------------
    //
	//---------------------------------------------------------------------
    void VertexBuffer::onDisposeGLResource()
    {
        if ( mGLVertexBuffer )
        {
            LNGL::glDeleteBuffers( 1, &mGLVertexBuffer );
            //LN_CHECK_GLERROR();
            mGLVertexBuffer = 0;
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