//=============================================================================
//【 PlatformContext 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../System/Manager.h"
#include "GLPlatformContext.h"

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
// ■ PlatformContext クラス
//=============================================================================

#if defined(LNOTE_GLES)
    PFNGLMAPBUFFEROESPROC   PlatformContext::m_glMapBufferOES = NULL;
    PFNGLUNMAPBUFFEROESPROC PlatformContext::m_glUnmapBufferOES = NULL;
#endif
    
    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    PlatformContext::PlatformContext()
        : mLogFile          ( NULL )
        , mSystemManager    ( NULL )
    {
#if defined(LNOTE_WINDOWS)
        mHWND = NULL;
        mHDC  = NULL;
        mGLRC = NULL;
#elif defined(LNOTE_GLES)
        mEGLDisplay = EGL_NO_DISPLAY;
        mEGLConfig  = NULL;
        mEGLContext = EGL_NO_CONTEXT;
        mEGLSurface = EGL_NO_SURFACE;
#endif
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    PlatformContext::~PlatformContext()
    {
#if defined(LNOTE_WINDOWS)
        ::wglMakeCurrent( 0, 0 );

        if ( mGLRC )
        {
            ::wglDeleteContext( mGLRC );
        }
        if ( mHDC )
        {
            ::ReleaseDC( mHWND, mHDC );
            mHWND = NULL;
            mHDC= NULL;
        }
#elif defined(LNOTE_GLES)
        
        if ( mEGLDisplay != EGL_NO_DISPLAY )
        {
            eglMakeCurrent( mEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
            
            if ( mEGLSurface )
            {
                eglDestroySurface( mEGLDisplay, mEGLSurface );
                mEGLSurface = EGL_NO_SURFACE;
            }
            
            if ( mEGLContext != EGL_NO_CONTEXT )
            {
                eglDestroyContext( mEGLDisplay, mEGLContext );
            }
            eglTerminate( mEGLDisplay );
        }
        mEGLDisplay = EGL_NO_DISPLAY;
        mEGLContext = EGL_NO_CONTEXT;
#endif
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void PlatformContext::initialize( const InitData& init_data_ )
    {
        mLogFile = init_data_.LogFile;
        mSystemManager = init_data_.SystemManager;
        
#if defined(LNOTE_WINDOWS)

        mHWND = mSystemManager->getWindow()->getWindowHandle();
        mHDC  = ::GetDC( mHWND );
        
        PIXELFORMATDESCRIPTOR pfd = {
            sizeof( PIXELFORMATDESCRIPTOR ),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32,     // color
            0, 0,   // R
            0, 0,   // G
            0, 0,   // B
            0, 0,   // A
            0, 0, 0, 0, 0,      // AC R G B A
            24,     // depth
            8,      // stencil
            0,      // aux
            PFD_MAIN_PLANE,      // layertype
            0,      // reserved
            0,      // layermask
            0,      // visiblemask
            0       // damagemask
        };
       
        int pxfm = ::ChoosePixelFormat( mHDC, &pfd );
        BOOL r   = ::SetPixelFormat( mHDC, pxfm, &pfd );
        mGLRC    = ::wglCreateContext( mHDC );

       

        ::wglMakeCurrent( mHDC, mGLRC );

        // コンテキスト作成後、wglMakeCurrent() で設定しないとエラーが返る
        //if ( glewInit() != GLEW_OK )
        //{
        //    LN_THROW_SystemCall( 0 ); 
        //}
//GLuint                          mGLVertexBuffer=0;
//        glGenBuffers( 1, &mGLVertexBuffer );
        
        //::wglMakeCurrent( 0, 0 );




#elif defined(LNOTE_GLES)
        
        m_glMapBufferOES = (PFNGLMAPBUFFEROESPROC)eglGetProcAddress( "glMapBufferOES" );
        m_glUnmapBufferOES = (PFNGLUNMAPBUFFEROESPROC)eglGetProcAddress( "glUnmapBufferOES" );
        
        EGLBoolean r;
        
        // EGLDisplayへの接続を取得
        mEGLDisplay = eglGetDisplay( EGL_DEFAULT_DISPLAY );
        LN_THROW_SystemCall( mEGLDisplay != EGL_NO_DISPLAY );
        
        // EGLDisplayへの接続を初期化
        EGLint major = 2;
        EGLint minor = 0;
        r = eglInitialize( mEGLDisplay, &major, &minor );
        LN_THROW_SystemCall( r );
        LN_LOG_WRITE( "OpenGL ES : %d.%d", major, minor );

        // EGL frame bufferの設定を取得する
        //EGL_DEPTH_SIZE, 1, //深度
        /*
        const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE };
        */
        // https://sites.google.com/site/learningopengl/eglbasics
        const EGLint attribs[] =
        {
            //EGL_LEVEL,    0,
            EGL_SURFACE_TYPE,  EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            //EGL_NATIVE_RENDERABLE, EGL_TRUE,
            //EGL_DEPTH_SIZE,   EGL_DONT_CARE,
            EGL_RED_SIZE,   8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE,  8,
            //EGL_ALPHA_SIZE, 4,
            EGL_NONE
        };
        EGLint num_configs; // 今のところ使ってないけど
        r = eglChooseConfig( mEGLDisplay, attribs, &mEGLConfig, 1, &num_configs );
        LN_THROW_SystemCall( r );
        LN_LOG_WRITE( "  num configs: %d", num_configs );

        // 設定するformatを取得する
        EGLint format;
        r = eglGetConfigAttrib( mEGLDisplay, mEGLConfig, EGL_NATIVE_VISUAL_ID, &format );
        LN_THROW_SystemCall( r );
        LN_LOG_WRITE( "  format: %d", format );

        // window bufferのサイズと設定を設定
        ANativeWindow_setBuffersGeometry( mSystemManager->getWindow()->getNativeWindow(), 0, 0, format );

        // APIをバインドする
        eglBindAPI( EGL_OPENGL_ES_API );
        
        // 新しいEGL rendering contextを作る
        EGLint ai32_context_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
        mEGLContext = eglCreateContext( mEGLDisplay, mEGLConfig, NULL, ai32_context_attribs );
        
        
        
#endif
        
        onResetDevice();
        
        //_p(mEGLDisplay );
        //_p(mEGLConfig );
        //_p(mEGLContext );
        //_p(mEGLSurface );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void PlatformContext::beginScene()
    {
#if defined(LNOTE_WINDOWS)
        //::wglMakeCurrent( mHDC, mGLRC );
#endif
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void PlatformContext::endScene()
    {
#if defined(LNOTE_WINDOWS)
        //::wglMakeCurrent( 0, 0 );
#endif
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void PlatformContext::present()
    {
#if defined(LNOTE_WINDOWS)
        ::SwapBuffers( mHDC );
#elif defined(LNOTE_ANDROID)
        eglSwapBuffers( mEGLDisplay, mEGLSurface );
#endif
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void PlatformContext::onLostDevice()
    {
#if defined(LNOTE_WINDOWS)

#elif defined(LNOTE_ANDROID)

        if ( mEGLDisplay && mEGLSurface )
        {
            eglMakeCurrent( mEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
            
            eglDestroySurface( mEGLDisplay, mEGLSurface );
            mEGLDisplay = EGL_NO_DISPLAY;
            mEGLSurface = EGL_NO_SURFACE;
        }
#endif
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void PlatformContext::onResetDevice()
    {
#if defined(LNOTE_WINDOWS)

#elif defined(LNOTE_ANDROID)

        // ディスプレイ取得
        //mEGLDisplay = mGraphicsDevice->getEGLDisplay();//eglGetDisplay( EGL_DEFAULT_DISPLAY );
        
        // サーフェイス作成
        mEGLSurface = eglCreateWindowSurface(
            mEGLDisplay,
            mEGLConfig,
            mSystemManager->getWindow()->getNativeWindow(), NULL );
        LN_THROW_SystemCall( mEGLSurface != EGL_NO_SURFACE );
        
        // コンテキストへアタッチ (複数ウィンドウつくる予定はないし、とりあえずここで)
        EGLBoolean r = eglMakeCurrent( mEGLDisplay, mEGLSurface, mEGLSurface, mEGLContext );
        LN_THROW_SystemCall( r );


#endif
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void PlatformContext::glClearDepth( GLclampf depth_ )
    {
#ifdef __gl2_h_
        glClearDepthf( depth_ );
#else
        ::glClearDepth( depth_ );
#endif
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void* PlatformContext::glMapBuffer( GLenum target_, GLuint buffer_ )
    {
#if defined(LNOTE_GLES)
        return m_glMapBufferOES( target_, buffer_ );
#else
        return ::glMapBuffer( target_, buffer_ );
#endif
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    GLboolean PlatformContext::glUnmapBuffer( GLenum target_ )
    {
#if defined(LNOTE_GLES)
        return m_glUnmapBufferOES( target_ );
#else
        return ::glUnmapBuffer( target_ );
#endif
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void PlatformContext::convertElementLNToGL( LNDeclType ln_type_, GLenum* gl_type_, GLint* size_, GLboolean* normalized_ )
    {
        static const struct _FormatType
        {
            GLenum      Type;
            GLint       Size;
            GLboolean   Normalize;
        } format_table[LN_DECL_MAX] =
        {
            { 0,                0,    GL_FALSE },   // LN_DECL_UNKNOWN
            { GL_FLOAT,         1,    GL_FALSE },   // LN_DECL_FLOAT1
            { GL_FLOAT,         2,    GL_FALSE },   // LN_DECL_FLOAT2
            { GL_FLOAT,         3,    GL_FALSE },   // LN_DECL_FLOAT3
            { GL_FLOAT,         4,    GL_FALSE },   // LN_DECL_FLOAT4
            { GL_UNSIGNED_BYTE, 4,    GL_FALSE },   // LN_DECL_UBYTE4
            { GL_UNSIGNED_BYTE, 4,    GL_FALSE },   // LN_DECL_COLOR4
            { GL_SHORT,         2,    GL_FALSE },   // LN_DECL_SHORT2
            { GL_SHORT,         4,    GL_FALSE },   // LN_DECL_SHORT4
        };

        // http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
        // GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE

        *gl_type_    = format_table[ln_type_].Type;
        *size_       = format_table[ln_type_].Size;
        *normalized_ = format_table[ln_type_].Normalize;
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