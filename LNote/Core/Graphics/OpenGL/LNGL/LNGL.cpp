//=============================================================================
//【 LNGL 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "LNGL.h"
#include "../GLPlatformContext.h"

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
// ■ LNGL
//=============================================================================

#define LN_CHECK_GLERROR_D()

    enum LNGLFunc
    {
        LNGL_INITIALIZE = 0,
        LNGL_FINALIZE,
        LNGL_beginScene,
        LNGL_endScene,
        LNGL_present,
        LNGL_glGenBuffers,
        LNGL_glBindBuffer,
        LNGL_glBufferData,
        LNGL_glBufferSubData,
        LNGL_glMapBuffer,
        LNGL_glUnmapBuffer,
        LNGL_glDeleteBuffers,

        LNGL_glEnable,
        LNGL_glDisable,
        LNGL_glViewport,
        LNGL_glScissor,
        //LNGL_glClearColor,
        //LNGL_glClearDepth,
        //LNGL_glClear,
        LNGL_lnClear,
        LNGL_glVertexAttribPointer,
        LNGL_glEnableVertexAttribArray,
        LNGL_glDrawArrays,
        LNGL_glDrawElements,
        LNGL_glDrawRangeElements,
        LNGL_glGenFramebuffers,
        LNGL_glDeleteFramebuffers,
        LNGL_glBindFramebuffer,
        LNGL_glFramebufferTexture2D,
        LNGL_glFramebufferRenderbuffer,
        LNGL_glFrontFace,
        //LNGL_glBlendEquation,
        //LNGL_glBlendFunc,
        //LNGL_glCullFace,
        //LNGL_glPolygonMode,
        //LNGL_glDepthMask,
        LNGL_lnSetRenderState,

        LNGL_glGenTextures,
        LNGL_glBindTexture,
        LNGL_glTexImage2D,
        LNGL_glTexSubImage2D,
        LNGL_glPixelStorei,
        LNGL_glDeleteTextures,
        LNGL_glTexParameteri,
        LNGL_glGenRenderbuffers,
        LNGL_glBindRenderbuffer,
        LNGL_glRenderbufferStorage,
        LNGL_glDeleteRenderbuffers,
        LNGL_glActiveTexture,

        LNGL_glDeleteProgram,
        LNGL_glGetProgramiv,
        /*LNGL_glDeleteShader,
        LNGL_glDeleteProgram,
        LNGL_glGetProgramiv,
        LNGL_glCreateShader,
        LNGL_glCreateProgram,
        LNGL_glAttachShader,
        LNGL_glLinkProgram,
        LNGL_glGetProgramInfoLog,
        LNGL_glShaderSource,
        LNGL_glCompileShader,
        LNGL_glGetShaderiv,
        LNGL_glGetShaderInfoLog,*/
        LNGL_lnBuildShader,
        LNGL_glGetActiveUniform,
        LNGL_glGetUniformLocation,
        LNGL_glUseProgram,

        LNGL_glUniform1i,
        LNGL_glUniform1f,
        LNGL_glUniform2f,
        LNGL_glUniform3f,
        LNGL_glUniform4f,
        LNGL_glUniform4fv,
        LNGL_glUniformMatrix4fv,
        LNGL_glGetActiveAttrib,
        LNGL_glGetAttribLocation,
    };

    struct LNGLInitializeArgs { lnByte Func; Base::LogFile* LogFile; System::Manager* SystemManager; };

    struct Args_glGenBuffers { lnByte Func; GLsizei n; GLuint* buffers; }; 

    struct Args_glBindBuffer
    {
        lnByte Func;
        GLenum target;
        GLuint buffer;
    };

    struct Args_glBufferData
    {
        lnByte Func;
        GLenum target;
        GLsizeiptr size;
        const GLvoid* data;
        GLenum usage;
    };

    struct Args_glBufferSubData { lnByte Func; GLenum target; GLintptr offset; GLsizeiptr size; const GLvoid* data; };

    struct Args_glMapBuffer { lnByte Func; GLenum target; GLenum access; };

    struct Args_glUnmapBuffer { lnByte Func; GLenum target; };
    
    struct Args_glDeleteBuffers { lnByte Func; GLsizei n; const GLuint* buffers; }; 

    //-------------------------
    struct Args_glEnable { lnByte Func; GLenum cap; };
    typedef Args_glEnable Args_glDisable;

    struct Args_glViewport { lnByte Func; GLint x; GLint y; GLsizei width; GLsizei height; };
    typedef Args_glViewport Args_glScissor;

    //struct Args_glClearColor { lnByte Func; GLfloat red; GLfloat green; GLfloat blue; GLfloat alpha; };

    //struct Args_glClearDepth { lnByte Func; GLclampf depth; };

    //struct Args_glClear { lnByte Func; GLbitfield mask; };

    struct Args_lnClear { lnByte Func; bool target; bool depth; const LColor* color; lnfloat z; };

    struct Args_glVertexAttribPointer { lnByte Func; GLuint index; GLint size; GLenum type; GLboolean normalized; GLsizei stride; const GLvoid* pointer; };

    struct Args_glEnableVertexAttribArray { lnByte Func; GLuint index; };

    struct Args_glDrawArrays { lnByte Func; GLenum mode; GLint first; GLsizei count; };

    struct Args_glDrawElements { lnByte Func; GLenum mode; GLsizei count; GLenum type; const GLvoid* indices; };

    struct Args_glDrawRangeElements { lnByte Func; GLenum mode; GLuint start; GLuint end; GLsizei count; GLenum type; const GLvoid* indices; };

    struct Args_glGenFramebuffers { lnByte Func; GLsizei n; GLuint* ids; };

    struct Args_glDeleteFramebuffers { lnByte Func; GLsizei n; GLuint *framebuffers; };

    struct Args_glBindFramebuffer { lnByte Func; GLenum target; GLuint framebuffer; };

    struct Args_glFramebufferTexture2D { lnByte Func; GLenum target; GLenum attachment; GLenum textarget; GLuint texture; GLint level; };

    struct Args_glFramebufferRenderbuffer { lnByte Func; GLenum target; GLenum attachment; GLenum renderbuffertarget; GLuint renderbuffer; };

    struct Args_glFrontFace { lnByte Func; GLenum mode; };
    //typedef Args_glFrontFace Args_glBlendEquation;
    //typedef Args_glFrontFace Args_glCullFace;

    //struct Args_glBlendFunc { lnByte Func; GLenum sfactor; GLenum dfactor; };

    //struct Args_glPolygonMode { lnByte Func; GLenum face; GLenum mode; };

    //struct Args_glDepthMask { lnByte Func; GLboolean flag; };

    struct Args_lnSetRenderState { lnByte Func; const LRenderState* state; };

    //-------------------------
    struct Args_glGenTextures { lnByte Func; GLsizei n; GLuint* textures; };

    struct Args_glBindTexture { lnByte Func; GLenum target; GLuint texture; };

    struct Args_glTexImage2D { lnByte Func; GLenum target; GLint level; GLint internalFormat; GLsizei width; GLsizei height; GLint border; GLenum format; GLenum type; const GLvoid* data; };

    struct Args_glTexSubImage2D { lnByte Func; GLenum target; GLint level; GLint xoffset; GLint yoffset; GLsizei width; GLsizei height; GLenum format; GLenum type; const GLvoid* data; };

    struct Args_glPixelStorei { lnByte Func; GLenum pname; GLint param; };

    struct Args_glDeleteTextures { lnByte Func; GLsizei n; const GLuint* textures; };

    struct Args_glTexParameteri { lnByte Func; GLenum target; GLenum pname; GLint param; };

    struct Args_glGenRenderbuffers { lnByte Func; GLsizei n; GLuint* renderbuffers; };

    struct Args_glBindRenderbuffer { lnByte Func; GLenum target; GLuint renderbuffer; };

    struct Args_glRenderbufferStorage { lnByte Func; GLenum target; GLenum internalformat; GLsizei width; GLsizei height; };

    struct Args_glDeleteRenderbuffers { lnByte Func; GLsizei n; GLuint* renderbuffers; };

    struct Args_glActiveTexture { lnByte Func; GLenum texture; };

    //-------------------------
    struct Args_glDeleteProgram { lnByte Func; GLuint program; };

    struct Args_glGetProgramiv { lnByte Func; GLuint program; GLenum pname; GLint *params; };
    

    /*struct Args_glDeleteShader { lnByte Func; GLuint shader; };

    
    typedef Args_glDeleteProgram Args_glLinkProgram;

    

    struct Args_glCreateShader { lnByte Func; GLenum shaderType; };

    struct Args_glAttachShader { lnByte Func; GLuint program, GLuint shader; };

    struct Args_glGetProgramInfoLog { lnByte Func; GLuint program; GLsizei maxLength; GLsizei* length; GLchar* infoLog; };

    struct Args_glShaderSource { lnByte Func; GLuint shader, GLsizei count, const GLchar** string, const GLint* length; };
*/
    struct Args_lnBuildShader { lnByte Func; const LNGL::ShaderBuildArgs* args; GLenum* program; LNShaderCompileResult* result; LRefTString* log; };

    struct Args_glGetActiveUniform { lnByte Func; GLuint program; GLuint index; GLsizei bufSize; GLsizei* length; GLint* size; GLenum* type; GLchar* name; };

    struct Args_glGetUniformLocation { lnByte Func; GLuint program; const GLchar* name; };

    struct Args_glUseProgram { lnByte Func; GLuint program; };

    //-------------------------
    struct Args_glUniform1i { lnByte Func; GLint location; GLint v0; };

    struct Args_glUniform4f { lnByte Func; GLint location; GLfloat v0; GLfloat v1; GLfloat v2; GLfloat v3; };
    typedef Args_glUniform4f Args_glUniform3f;
    typedef Args_glUniform4f Args_glUniform2f;
    typedef Args_glUniform4f Args_glUniform1f;

    struct Args_glUniform4fv { lnByte Func; GLint location; GLsizei count; const GLfloat* value; };

    struct Args_glUniformMatrix4fv { lnByte Func; GLint location; GLsizei count; GLboolean transpose; const GLfloat* value; };

    struct Args_glGetActiveAttrib { lnByte Func; GLuint program; GLuint index; GLsizei bufSize; GLsizei* length; GLint* size; GLenum* type; GLchar* name; };

    struct Args_glGetAttribLocation { lnByte Func; GLuint program; const GLchar* name; };


// type 型のポインタ変数 a に代入
#define GET_ARGS(type) type* a = (type*)buf;

// return
#define PUSH_RETURN_VAL(type, val) \
    *((type*)LNGL::mMessageBuffer.request(sizeof(type))) = val;

//// return
//#define POP_RETURN_VAL(type) \
//    *((type*)LNGL::mMessageBuffer.getBuffer());

    OpenGLThread*        LNGL::mOpenGLThread = NULL;
    Thread::EventFlag    LNGL::mStandbyGLThread(false);
    Thread::EventFlag    LNGL::mRunningNotify(false);
    Thread::EventFlag    LNGL::mReturnNotify(false);
    Thread::Mutex        LNGL::mMutex;
    Base::TempBuffer     LNGL::mMessageBuffer;

    class OpenGLThread
        : public Thread::ThreadObject
    {
    private:

        Base::LogFile*      mLogFile;
        PlatformContext*    mPlatformContext;

    public:

        OpenGLThread()
        {}

        void run()
        {
            try
            {
                while (true)
                {
                    LNGL::mStandbyGLThread.setTrue();
                    LNGL::mRunningNotify.wait();
                    LNGL::mRunningNotify.setFalse();

                    lnByte* buf = LNGL::mMessageBuffer.getBuffer();

                    
                    switch (*buf)
                    {
    //---------------------------------------------------------------------
    // initialize()
    //---------------------------------------------------------------------
    case LNGL_INITIALIZE:
    {
        LNGLInitializeArgs* args = (LNGLInitializeArgs*)buf;

        mLogFile = args->LogFile;

        PlatformContext::InitData data;
        data.LogFile        = mLogFile;
        data.SystemManager  = args->SystemManager;
        mPlatformContext = LN_NEW PlatformContext();
        mPlatformContext->initialize( data );


        //-----------------------------------------------------
        //

        int n = 0;

        // シェーダーで利用できる、頂点属性の最大数
        glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &n );
        LN_LOG_WRITE( "  GL_MAX_VERTEX_ATTRIBS : %d", n );
        LN_CHECK_GLERROR_D();

        // varying 変数で利用できる、ベクトルの最大数
        glGetIntegerv( GL_MAX_VARYING_VECTORS, &n );
        LN_LOG_WRITE( "  GL_MAX_VARYING_VECTORS : %d", n );
        LN_CHECK_GLERROR_D();

        // バーテックスシェーダーで利用できる、uniform(ベクトル)の最大数
        glGetIntegerv( GL_MAX_VERTEX_UNIFORM_VECTORS, &n );
        LN_LOG_WRITE( "  GL_MAX_VERTEX_UNIFORM_VECTORS : %d", n );
        LN_CHECK_GLERROR_D();

        // フラグメントシェーダーで利用できる、uniform(ベクトル)の最大数
        glGetIntegerv( GL_MAX_FRAGMENT_UNIFORM_VECTORS, &n );
        LN_LOG_WRITE( "  GL_MAX_FRAGMENT_UNIFORM_VECTORS : %d", n );
        LN_CHECK_GLERROR_D();

        // バーテックスシェーダーで利用できる、テクスチャユニットの最大数
        glGetIntegerv( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &n );
        LN_LOG_WRITE( "  GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS : %d", n );
        LN_CHECK_GLERROR_D();

        // フラグメントシェーダーで利用できる、テクスチャユニットの最大数
        glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &n );
        LN_LOG_WRITE( "  GL_MAX_TEXTURE_IMAGE_UNITS : %d", n );
        LN_CHECK_GLERROR_D();

        // (?) 固定機能で使用可能な、テクスチャユニットの最大数
        //      GLSL 使用のため固定機能は必要なし。
        //      というか、GLES2だとエラーを返す。
        //glGetIntegerv( GL_MAX_TEXTURE_UNITS, &n );
        //LN_CHECK_GLERROR_D();
        //LN_LOG_WRITE( "  GL_MAX_TEXTURE_UNITS : %d", n );

        // テクスチャの最大の幅と高さ
        glGetIntegerv( GL_MAX_TEXTURE_SIZE, &n );
        LN_LOG_WRITE( "  GL_MAX_TEXTURE_SIZE : %d", n );
        LN_CHECK_GLERROR_D();

        // キューブテクスチャの最大幅と高さ
        glGetIntegerv( GL_MAX_CUBE_MAP_TEXTURE_SIZE, &n );
        LN_LOG_WRITE( "  GL_MAX_CUBE_MAP_TEXTURE_SIZE : %d", n );
        LN_CHECK_GLERROR_D();

#if !defined(LNOTE_GLES)
        // 3D テクスチャの最大幅と高さ
        glGetIntegerv( GL_MAX_3D_TEXTURE_SIZE, &n );
        LN_LOG_WRITE( "  GL_MAX_3D_TEXTURE_SIZE : %d", n );
        LN_CHECK_GLERROR_D();
#endif
        // レンダーバッファの最大幅と高さ
        glGetIntegerv( GL_MAX_RENDERBUFFER_SIZE, &n );
        LN_LOG_WRITE( "  GL_MAX_RENDERBUFFER_SIZE : %d", n );
        LN_CHECK_GLERROR_D();

        // ビューポートの最大幅と高さ
        //  ※なぜか変数領域が破壊されているとかエラーが出るので、
        //    今のところは レンダーバッファ の最大サイズを使う。
        //    （エラーは関数を出たときに出る。ログ出力するときは
        //      値が正しく、GL_MAX_RENDERBUFFER_SIZE と等しい）
        //n = 0;
        //glGetIntegerv( GL_MAX_VIEWPORT_DIMS, &n );
        //LN_LOG_WRITE( "  GL_MAX_VIEWPORT_DIMS : %d", n );
        //LN_CHECK_GLERROR_D();

#if defined(LNOTE_GLES)
        // OES 使ってないのでこのまま
        //glGetIntegerv( GL_FRAMEBUFFER_BINDING_OES, &n );
        //LN_CHECK_GLERROR_D();
        //LN_LOG_WRITE( "  GL_FRAMEBUFFER_BINDING_OES : %d", n );
#endif

        

        break;
    }
    //---------------------------------------------------------------------
    // finalize()
    //---------------------------------------------------------------------
    case LNGL_FINALIZE:
    {
        SAFE_DELETE( mPlatformContext );
        LNGL::mReturnNotify.setTrue();
        return; // end
    }
    //---------------------------------------------------------------------
    // LNGL_beginScene()
    //---------------------------------------------------------------------
    case LNGL_beginScene:
    {
        mPlatformContext->beginScene();
        break;
    }
    //---------------------------------------------------------------------
    // LNGL_endScene()
    //---------------------------------------------------------------------
    case LNGL_endScene:
    {
        mPlatformContext->endScene();
        break;
    }
    //---------------------------------------------------------------------
    // present()
    //---------------------------------------------------------------------
    case LNGL_present:
    {
        mPlatformContext->present();
        break;
    }
    //---------------------------------------------------------------------
    // glGenBuffers()
    //---------------------------------------------------------------------
    case LNGL_glGenBuffers:
    {
        GET_ARGS(Args_glGenBuffers);
        glGenBuffers(a->n, a->buffers);
        LN_CHECK_GLERROR_D();
        break;
    }
    //---------------------------------------------------------------------
    // glBindBuffer()
    //---------------------------------------------------------------------
    case LNGL_glBindBuffer:
    {
        GET_ARGS(Args_glBindBuffer);
        glBindBuffer(a->target, a->buffer);
        LN_CHECK_GLERROR_D();
        break;
    }
    //---------------------------------------------------------------------
    // glBufferData()
    //---------------------------------------------------------------------
    case LNGL_glBufferData:
    {
        GET_ARGS(Args_glBufferData);
        glBufferData(a->target, a->size, a->data, a->usage);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glBufferSubData:
    {
        GET_ARGS(Args_glBufferSubData);
        glBufferSubData(a->target, a->offset, a->size, a->data);
        LN_CHECK_GLERROR_D();
        break;
    }
    //---------------------------------------------------------------------
    // glMapBuffer()
    //---------------------------------------------------------------------
    case LNGL_glMapBuffer:
    {
        GET_ARGS(Args_glMapBuffer);
        void* v = mPlatformContext->glMapBuffer(a->target, a->access);
        LN_CHECK_GLERROR_D();
        PUSH_RETURN_VAL(void*, v);
        break;
    }
    //---------------------------------------------------------------------
    // glUnmapBuffer()
    //---------------------------------------------------------------------
    case LNGL_glUnmapBuffer:
    {
        GET_ARGS(Args_glUnmapBuffer);
        GLboolean v = mPlatformContext->glUnmapBuffer(a->target);
        LN_CHECK_GLERROR_D();
        PUSH_RETURN_VAL(GLboolean, v);
        break;
    }
    //---------------------------------------------------------------------
    // glDeleteBuffers()
    //---------------------------------------------------------------------
    case LNGL_glDeleteBuffers:
    {
        GET_ARGS(Args_glDeleteBuffers);
        glDeleteBuffers(a->n, a->buffers);
        LN_CHECK_GLERROR_D();
        break;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    case LNGL_glEnable:
    {
        GET_ARGS(Args_glEnable);
        glEnable(a->cap);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glDisable:
    {
        GET_ARGS(Args_glDisable);
        glDisable(a->cap);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glViewport:
    {
        GET_ARGS(Args_glViewport);
        glViewport(a->x, a->y, a->width, a->height);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glScissor:
    {
        GET_ARGS(Args_glScissor);
        glScissor(a->x, a->y, a->width, a->height);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_lnClear:
    {
        GET_ARGS(Args_lnClear);
        glDepthMask(GL_TRUE);   // これがないと Depth が正常にクリアされない
        LN_CHECK_GLERROR_D();
        glClearColor(a->color->red, a->color->green, a->color->blue, a->color->alpha );
        LN_CHECK_GLERROR_D();
        //glClearDepth(a->z);
        mPlatformContext->glClearDepth(a->z);
        LN_CHECK_GLERROR_D();
        //glClearStencil( 0 );
        //LN_CHECK_GLERROR_D();
        glClear(
            ( ( a->target ) ? GL_COLOR_BUFFER_BIT : 0 ) | 
            ( ( a->depth  ) ? GL_DEPTH_BUFFER_BIT : 0 ) );// | GL_STENCIL_BUFFER_BIT  );
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glVertexAttribPointer:
    {
        GET_ARGS(Args_glVertexAttribPointer);
        glVertexAttribPointer(a->index, a->size, a->type, a->normalized, a->stride, a->pointer);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glEnableVertexAttribArray:
    {
        GET_ARGS(Args_glEnableVertexAttribArray);
        glEnableVertexAttribArray(a->index);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glDrawArrays:
    {
        GET_ARGS(Args_glDrawArrays);
        glDrawArrays(a->mode, a->first, a->count);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glDrawElements:
    {
        GET_ARGS(Args_glDrawElements);
        glDrawElements(a->mode, a->count, a->type, a->indices);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glDrawRangeElements:
    {
        GET_ARGS(Args_glDrawRangeElements);
#if !defined(LNOTE_GLES)
        glDrawRangeElements(a->mode, a->start, a->end, a->count, a->type, a->indices);
        //if (a->type == GL_UNSIGNED_SHORT)
        //{
        //    glDrawElements(a->mode, a->count, GL_UNSIGNED_SHORT, (GLvoid*) (sizeof(GLushort) * a->start));
        //}
        //else
        //{
        //    glDrawElements(a->mode, a->count, GL_UNSIGNED_INT, (GLvoid*) (sizeof(GLuint) * a->start));
        //}
        LN_CHECK_GLERROR_D();
#endif
        break;
    }
    case LNGL_glGenFramebuffers:
    {
        GET_ARGS(Args_glGenFramebuffers);
        glGenFramebuffers(a->n, a->ids);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glDeleteFramebuffers:
    {
        GET_ARGS(Args_glDeleteFramebuffers);
        glDeleteFramebuffers(a->n, a->framebuffers);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glBindFramebuffer:
    {
        GET_ARGS(Args_glBindFramebuffer);
        glBindFramebuffer(a->target, a->framebuffer);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glFramebufferTexture2D:
    {
        GET_ARGS(Args_glFramebufferTexture2D);
        glFramebufferTexture2D(a->target, a->attachment, a->textarget, a->texture, a->level);
        LN_CHECK_GLERROR_D();
        // ここで例外を throw する場合…
        //      ・デフォルトのフレームバッファに対してRT、Depth バッファをセットしようとしていないか。
        //      ・RT、Depth を間違えて設定していないか。
        break;
    }
    case LNGL_glFramebufferRenderbuffer:
    {
        GET_ARGS(Args_glFramebufferRenderbuffer);
        glFramebufferRenderbuffer(a->target, a->attachment, a->renderbuffertarget, a->renderbuffer);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glFrontFace:
    {
        GET_ARGS(Args_glFrontFace);
        glFrontFace(a->mode);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_lnSetRenderState:
    {
        GET_ARGS(Args_lnSetRenderState);

        // 合成方法
        switch ( a->state->BlendMode )
	    {
	        case LN_BLEND_NORMAL:
                glDisable( GL_BLEND );                  LN_CHECK_GLERROR_D();
	            glBlendEquation( GL_FUNC_ADD );         LN_CHECK_GLERROR_D();
	            glBlendFunc( GL_ONE, GL_ZERO );         LN_CHECK_GLERROR_D();
	            //glAlphaFunc( GL_GREATER, 0.5f );        LN_CHECK_GLERROR_D();
		        break;
	        case LN_BLEND_ALPHA:
	            glEnable( GL_BLEND );                                   LN_CHECK_GLERROR_D();
	            glBlendEquation( GL_FUNC_ADD );                         LN_CHECK_GLERROR_D();
	            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );    LN_CHECK_GLERROR_D();
	            //glAlphaFunc( GL_GREATER, 0.01f );                       LN_CHECK_GLERROR_D();
                break;
	        case LN_BLEND_ADD:
	            glEnable( GL_BLEND );                   LN_CHECK_GLERROR_D();
	            glBlendEquation( GL_FUNC_ADD );         LN_CHECK_GLERROR_D();
	            glBlendFunc( GL_SRC_ALPHA, GL_ONE );    LN_CHECK_GLERROR_D();
	            //glAlphaFunc( GL_GREATER, 0.0f );        LN_CHECK_GLERROR_D();
		        break;
	        case LN_BLEND_ADD_ALPHADISABLE:
	            glEnable( GL_BLEND );                   LN_CHECK_GLERROR_D();
	            glBlendEquation( GL_FUNC_ADD );         LN_CHECK_GLERROR_D();
	            glBlendFunc( GL_ONE, GL_ONE );          LN_CHECK_GLERROR_D();
	            //glAlphaFunc( GL_GREATER, 0.0f );        LN_CHECK_GLERROR_D();
		        break;
	        case LN_BLEND_SUB:
	            glEnable( GL_BLEND );                           LN_CHECK_GLERROR_D();
	            glBlendEquation( GL_FUNC_REVERSE_SUBTRACT );    LN_CHECK_GLERROR_D();
	            glBlendFunc( GL_SRC_ALPHA, GL_ONE );            LN_CHECK_GLERROR_D();
	            //glAlphaFunc( GL_GREATER, 0.0f );                LN_CHECK_GLERROR_D();
		        break;
	        case LN_BLEND_SUB_ALPHADISABLE:
	            glEnable( GL_BLEND );                           LN_CHECK_GLERROR_D();
	            glBlendEquation( GL_FUNC_REVERSE_SUBTRACT );    LN_CHECK_GLERROR_D();
	            glBlendFunc( GL_ONE, GL_ONE );                  LN_CHECK_GLERROR_D();
	            //glAlphaFunc( GL_GREATER, 0.0f );                LN_CHECK_GLERROR_D();
		        break;
	        case LN_BLEND_MUL:
	            glEnable( GL_BLEND );                           LN_CHECK_GLERROR_D();
	            glBlendEquation( GL_FUNC_ADD );                 LN_CHECK_GLERROR_D();
	            glBlendFunc( GL_BLEND_SRC_RGB, GL_ZERO );       LN_CHECK_GLERROR_D();
	            //glAlphaFunc( GL_GREATER, 0.0f );                LN_CHECK_GLERROR_D();
		        break;
	        case LN_BLEND_SCREEN:
	            glEnable( GL_BLEND );                           LN_CHECK_GLERROR_D();
	            glBlendEquation( GL_FUNC_ADD );                 LN_CHECK_GLERROR_D();
	            glBlendFunc( GL_ONE, GL_ONE_MINUS_DST_ALPHA );  LN_CHECK_GLERROR_D();
	            //glAlphaFunc( GL_GREATER, 0.0f );                LN_CHECK_GLERROR_D();
		        break;

            // http://memo.devjam.net/clip/538
            // http://d.hatena.ne.jp/melpon/20070824/p1
	    }

        // カリング
	    //if ( mRequestRenderState.Culling != mCurrentRenderState.Culling || reset_ )
	    {
            if (a->state->Culling == LN_CULL_DOUBLE)
            {
                glDisable(GL_CULL_FACE);
                LN_CHECK_GLERROR_D();
            }
            else
            {
                glEnable(GL_CULL_FACE);
                LN_CHECK_GLERROR_D();
		        const GLenum tb[] = { GL_BACK, GL_FRONT, GL_FRONT_AND_BACK };
                glCullFace( tb[ a->state->FillMode ] );
                LN_CHECK_GLERROR_D();
            }
	    }
#if !defined(LNOTE_GLES)
        // 塗りつぶし方法
	    //if ( mRequestRenderState.FillMode != mCurrentRenderState.FillMode || reset_ )
	    {
		    const GLenum tb[] = { GL_FILL, GL_LINE, GL_POINT };
	        glPolygonMode( GL_FRONT_AND_BACK, tb[ a->state->FillMode ] );
            LN_CHECK_GLERROR_D();
            //glPolygonMode(GL_FRONT, GL_FILL) // 表は塗りつぶす
            //glPolygonMode(GL_BACK, GL_LINE)  // 裏は線
	    }
	    
	    // アルファテスト
	    //if ( mRequestRenderState.AlphaTest != mCurrentRenderState.AlphaTest || reset_ )
	    {
	        if ( a->state->AlphaTest )
            {
	            glEnable( GL_ALPHA_TEST );
                LN_CHECK_GLERROR_D();
            }
	        else
            {
	            glDisable( GL_ALPHA_TEST );
                LN_CHECK_GLERROR_D();
            }
	    }
#endif
	    // 深度テスト
	    //if ( mRequestRenderState.DepthTest != mCurrentRenderState.DepthTest || reset_ )
	    {
	        if ( a->state->DepthTest )
            {
	            glEnable( GL_DEPTH_TEST );
                LN_CHECK_GLERROR_D();
            }
	        else
            {
	            glDisable( GL_DEPTH_TEST );
                LN_CHECK_GLERROR_D();
            }
	    }
	    // 深度書き込み
        //if ( mRequestRenderState.DepthWrite != mCurrentRenderState.DepthWrite || reset_ )
	    {
		    const GLboolean tb[] = { GL_FALSE, GL_TRUE };
	        glDepthMask( tb[ a->state->DepthWrite ] );
            LN_CHECK_GLERROR_D();
	    }
        // ポイントスプライト
        //if ( mRequestRenderState.PointSprite != mCurrentRenderState.PointSprite || reset_ )
	    {
            if ( a->state->PointSprite )
            {
#if defined(LNOTE_ANDROID)
                //glTexEnvi(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_TRUE);
                //LN_CHECK_GLERROR_D();
                
                //glEnable( GL_POINT_SPRITE_OES );
#else
	            glEnable( GL_POINT_SPRITE );
                LN_CHECK_GLERROR_D();
#endif
                
            }
	        else
            {
#if defined(LNOTE_ANDROID)
                /*
                glTexEnvi(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_TRUE);
                LN_CHECK_GLERROR_D();
                
                glEnable( GL_POINT_SPRITE_OES );
                LN_CHECK_GLERROR_D();

                glDisable( GL_POINT_SPRITE_OES );
                LN_CHECK_GLERROR_D();
                */
#else
	            glDisable( GL_POINT_SPRITE );
                LN_CHECK_GLERROR_D();
#endif
                
            }
	    }
        break;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    case LNGL_glGenTextures:
    {
        GET_ARGS(Args_glGenTextures);
        glGenTextures(a->n, a->textures);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glBindTexture:
    {
        GET_ARGS(Args_glBindTexture);
        glBindTexture(a->target, a->texture);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glTexImage2D:
    {
        GET_ARGS(Args_glTexImage2D);
        glTexImage2D(a->target, a->level, a->internalFormat, a->width, a->height, a->border, a->format, a->type, a->data);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glTexSubImage2D:
    {
        GET_ARGS(Args_glTexSubImage2D);
        glTexSubImage2D(a->target, a->level, a->xoffset, a->yoffset, a->width, a->height, a->format, a->type, a->data);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glPixelStorei:
    {
        GET_ARGS(Args_glPixelStorei);
        glPixelStorei(a->pname, a->param);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glDeleteTextures:
    {
        GET_ARGS(Args_glDeleteTextures);
        glDeleteTextures(a->n, a->textures);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glTexParameteri:
    {
        GET_ARGS(Args_glTexParameteri);
        glTexParameteri(a->target, a->pname, a->param);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glGenRenderbuffers:
    {
        GET_ARGS(Args_glGenRenderbuffers);
        glGenRenderbuffers(a->n, a->renderbuffers);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glBindRenderbuffer:
    {
        GET_ARGS(Args_glBindRenderbuffer);
        glBindRenderbuffer(a->target, a->renderbuffer);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glRenderbufferStorage:
    {
        GET_ARGS(Args_glRenderbufferStorage);
        glRenderbufferStorage(a->target, a->internalformat, a->width, a->height);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glDeleteRenderbuffers:
    {
        GET_ARGS(Args_glDeleteRenderbuffers);
        glDeleteRenderbuffers(a->n, a->renderbuffers);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glActiveTexture:
    {
        GET_ARGS(Args_glActiveTexture);
        glActiveTexture(a->texture);
        LN_CHECK_GLERROR_D();
        break;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    case LNGL_glDeleteProgram:
    {
        GET_ARGS(Args_glDeleteProgram);
        glDeleteProgram(a->program);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glGetProgramiv:
    {
        GET_ARGS(Args_glGetProgramiv);
        glGetProgramiv(a->program, a->pname, a->params);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_lnBuildShader:
    {
        GET_ARGS(Args_lnBuildShader);
        _buildShader(a->args, a->program, a->result, a->log);
        break;
    }
    case LNGL_glGetActiveUniform:
    {
        GET_ARGS(Args_glGetActiveUniform);
        glGetActiveUniform(a->program, a->index, a->bufSize, a->length, a->size, a->type, a->name);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glGetUniformLocation:
    {
        GET_ARGS(Args_glGetUniformLocation);
        //glUseProgram(a->program);
        //LN_CHECK_GLERROR_D();
        GLint v = glGetUniformLocation(a->program, a->name);
        LN_CHECK_GLERROR_D();
        PUSH_RETURN_VAL(GLint, v);
        break;
    }
    case LNGL_glUseProgram:
    {
        GET_ARGS(Args_glUseProgram);
        glUseProgram(a->program);
        LN_CHECK_GLERROR_D();
        break;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    case LNGL_glUniform1i:
    {
        GET_ARGS(Args_glUniform1i);
        glUniform1i(a->location, a->v0);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glUniform1f:
    {
        GET_ARGS(Args_glUniform1f);
        glUniform1f(a->location, a->v0);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glUniform2f:
    {
        GET_ARGS(Args_glUniform2f);
        glUniform2f(a->location, a->v0, a->v1);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glUniform3f:
    {
        GET_ARGS(Args_glUniform3f);
        glUniform3f(a->location, a->v0, a->v1, a->v2);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glUniform4f:
    {
        GET_ARGS(Args_glUniform4f);
        glUniform4f(a->location, a->v0, a->v1, a->v2, a->v3);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glUniform4fv:
    {
        GET_ARGS(Args_glUniform4fv);
        glUniform4fv(a->location, a->count, a->value);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glUniformMatrix4fv:
    {
        GET_ARGS(Args_glUniformMatrix4fv);
        glUniformMatrix4fv(a->location, a->count, a->transpose, a->value);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glGetActiveAttrib:
    {
        GET_ARGS(Args_glGetActiveAttrib);
        glGetActiveAttrib(a->program, a->index, a->bufSize, a->length, a->size, a->type, a->name);
        LN_CHECK_GLERROR_D();
        break;
    }
    case LNGL_glGetAttribLocation:
    {
        GET_ARGS(Args_glGetAttribLocation);
        GLint v = glGetAttribLocation(a->program, a->name);
        LN_CHECK_GLERROR_D();
        PUSH_RETURN_VAL(GLint, v);
        break;
    }
    
    //---------------------------------------------------------------------
    // end defines
    //---------------------------------------------------------------------
                    } // switch

                    LNGL::mReturnNotify.setTrue();
                }
            }
            catch ( const Base::Exception& e )
            {
                e.process();
          
#if 0      
                const Core::Base::OpenGLException* ge = dynamic_cast<const Core::Base::OpenGLException*>(&e);
                if (ge->getGLErrorCode() == GL_INVALID_FRAMEBUFFER_OPERATION_OES)
                {
                    GLenum id = glCheckFramebufferStatusOES();
                    switch (id)
                    {
                        case GL_FRAMEBUFFER_COMPLETE_OES: lnprintf("GL_FRAMEBUFFER_COMPLETE_OES\n"); break;
                        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES: lnprintf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES\n"); break;
                        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES: lnprintf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES\n"); break;
                        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES: lnprintf("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES\n"); break;
                        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES: lnprintf("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES\n"); break;
                        case GL_FRAMEBUFFER_UNSUPPORTED_OES: lnprintf("GL_FRAMEBUFFER_UNSUPPORTED_OES\n"); break;
                        case GL_FRAMEBUFFER_BINDING_OES: lnprintf("GL_FRAMEBUFFER_BINDING_OES\n"); break;
                        case GL_RENDERBUFFER_BINDING_OES: lnprintf("GL_RENDERBUFFER_BINDING_OES\n"); break;
                        case GL_MAX_RENDERBUFFER_SIZE_OES: lnprintf("GL_MAX_RENDERBUFFER_SIZE_OES\n"); break;
                        case GL_INVALID_FRAMEBUFFER_OPERATION_OES: lnprintf("GL_INVALID_FRAMEBUFFER_OPERATION_OES\n"); break;

                    }
                }
#endif
            }
        }

    private:

        void _buildShader(const LNGL::ShaderBuildArgs* args, GLenum* program, LNShaderCompileResult* result, LRefTString* log);
        bool _compileShader( GLuint shader_, int code_count_, const char** code_, GLint* size_, LRefTString* log );
    };

#define FUNC_BEGIN \
    mMutex.lock(); \
    mStandbyGLThread.wait(); \
    mStandbyGLThread.setFalse();

#define FUNC_END \
    mRunningNotify.setTrue(); \
    mReturnNotify.wait(); \
    mReturnNotify.setFalse(); \
    mMutex.unlock();

#define FUNC_END_RETURN(type) \
    mRunningNotify.setTrue(); \
    mReturnNotify.wait(); \
    mReturnNotify.setFalse(); \
    type ret = *((type*)LNGL::mMessageBuffer.getBuffer()); \
    mMutex.unlock(); \
    return ret;

#define SET_SEND_MSG(data_) \
    { \
        lnByte* buf = mMessageBuffer.request(sizeof(data_)); \
        memcpy(buf, &data_, sizeof(data_)); \
    }

// 引数のない関数呼び出し
#define POST_FUNC(func_) \
    { \
        lnByte* buf = mMessageBuffer.request(sizeof(lnByte)); \
        lnByte func = func_; \
        memcpy(buf, &func, sizeof(lnByte)); \
    }

#define DEF_ARGS_STRUCT(func) \
    Args_##func a; \
    a.Func = LNGL_##func;
    

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LNGL::initialize( const InitData& init_data_ )
    {
        mOpenGLThread = LN_NEW OpenGLThread();
        mOpenGLThread->start();

        FUNC_BEGIN
        
        LNGLInitializeArgs args;
        args.Func = LNGL_INITIALIZE;
        args.LogFile = init_data_.LogFile;
        args.SystemManager = init_data_.SystemManager;
        SET_SEND_MSG(args);

        FUNC_END
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LNGL::finalize()
    {
        FUNC_BEGIN
        POST_FUNC(LNGL_FINALIZE);
        FUNC_END

        mOpenGLThread->wait();
        SAFE_DELETE( mOpenGLThread );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LNGL::beginScene()
    {
        FUNC_BEGIN
        POST_FUNC(LNGL_beginScene);
        FUNC_END
    }
    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LNGL::endScene()
    {
        FUNC_BEGIN
        POST_FUNC(LNGL_endScene);
        FUNC_END
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LNGL::present()
    {
        FUNC_BEGIN
        POST_FUNC(LNGL_present);
        FUNC_END
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LNGL::glGenBuffers(GLsizei n, GLuint* buffers)
    {
        FUNC_BEGIN
        Args_glGenBuffers args;
        args.Func = LNGL_glGenBuffers;
        args.n = n;
        args.buffers = buffers;
        SET_SEND_MSG(args);
        FUNC_END
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LNGL::glBindBuffer(GLenum target, GLuint buffer)
    {
        FUNC_BEGIN
        Args_glBindBuffer a;
        a.Func = LNGL_glBindBuffer;
        a.target = target;
        a.buffer = buffer;
        SET_SEND_MSG(a);
        FUNC_END
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LNGL::glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
    {
        FUNC_BEGIN
        Args_glBufferData a;
        a.Func = LNGL_glBufferData;
        a.target = target;
        a.size = size;
        a.data = data;
        a.usage = usage;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data)
    {
        FUNC_BEGIN
        Args_glBufferSubData a;
        a.Func = LNGL_glBufferSubData;
        a.target = target;
        a.offset = offset;
        a.size = size;
        a.data = data;
        SET_SEND_MSG(a);
        FUNC_END
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void* LNGL::glMapBuffer(GLenum target, GLenum access)
    {
        FUNC_BEGIN
        Args_glMapBuffer a;
        a.Func = LNGL_glMapBuffer;
        a.target = target;
        a.access = access;
        SET_SEND_MSG(a);
        FUNC_END_RETURN(void*);
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    GLboolean LNGL::glUnmapBuffer(GLenum target)
    {
        FUNC_BEGIN
        Args_glUnmapBuffer a;
        a.Func = LNGL_glUnmapBuffer;
        a.target = target;
        SET_SEND_MSG(a);
        FUNC_END_RETURN(GLboolean);
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LNGL::glDeleteBuffers(GLsizei n, const GLuint* buffers)
    {
        FUNC_BEGIN
        Args_glDeleteBuffers a;
        a.Func = LNGL_glDeleteBuffers;
        a.n = n;
        a.buffers = buffers;
        SET_SEND_MSG(a);
        FUNC_END
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LNGL::glEnable(GLenum cap)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glEnable);
        a.cap = cap;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glDisable(GLenum cap)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glDisable);
        a.cap = cap;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glViewport);
        a.x = x;
        a.y = y;
        a.width = width;
        a.height = height;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glScissor);
        a.x = x;
        a.y = y;
        a.width = width;
        a.height = height;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::lnClear(bool target, bool depth, const LColor* color, lnfloat z)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(lnClear);
        a.target = target;
        a.depth = depth;
        a.color = color;
        a.z = z;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glVertexAttribPointer);
        a.index = index;
        a.size = size;
        a.type = type;
        a.normalized = normalized;
        a.stride = stride;
        a.pointer = pointer;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glEnableVertexAttribArray(GLuint index)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glEnableVertexAttribArray);
        a.index = index;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glDrawArrays(GLenum mode, GLint first, GLsizei count)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glDrawArrays);
        a.mode = mode;
        a.first = first;
        a.count = count;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glDrawElements);
        a.mode = mode;
        a.count = count;
        a.type = type;
        a.indices = indices;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid* indices)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glDrawRangeElements);
        a.mode = mode;
        a.start = start;
        a.end = end;
        a.count = count;
        a.type = type;
        a.indices = indices;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glGenFramebuffers(GLsizei n, GLuint* ids)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glGenFramebuffers);
        a.n = n;
        a.ids = ids;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glDeleteFramebuffers(GLsizei n, GLuint *framebuffers)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glDeleteFramebuffers);
        a.n = n;
        a.framebuffers = framebuffers;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glBindFramebuffer(GLenum target, GLuint framebuffer)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glBindFramebuffer);
        a.target = target;
        a.framebuffer = framebuffer;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glFramebufferTexture2D);
        a.target = target;
        a.attachment = attachment;
        a.textarget = textarget;
        a.texture = texture;
        a.level = level;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glFramebufferRenderbuffer);
        a.target = target;
        a.attachment = attachment;
        a.renderbuffertarget = renderbuffertarget;
        a.renderbuffer = renderbuffer;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glFrontFace(GLenum mode)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glFrontFace);
        a.mode = mode;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::lnSetRenderState(const LRenderState* state)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(lnSetRenderState);
        a.state = state;
        SET_SEND_MSG(a);
        FUNC_END
    }
   
    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LNGL::glGenTextures(GLsizei n, GLuint* textures)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glGenTextures);
        a.n = n;
        a.textures = textures;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glBindTexture(GLenum target, GLuint texture)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glBindTexture);
        a.target = target;
        a.texture = texture;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glTexImage2D);
        a.target = target;
        a.level = level;
        a.internalFormat = internalFormat;
        a.width = width;
        a.height = height;
        a.border = border;
        a.format = format;
        a.type = type;
        a.data = data;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glTexSubImage2D);
        a.target = target;
        a.level = level;
        a.xoffset = xoffset;
        a.yoffset = yoffset;
        a.width = width;
        a.height = height;
        a.format = format;
        a.type = type;
        a.data = data;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glPixelStorei(GLenum pname, GLint param)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glPixelStorei);
        a.pname = pname;
        a.param = param;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glDeleteTextures(GLsizei n, const GLuint* textures)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glDeleteTextures);
        a.n = n;
        a.textures = textures;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glTexParameteri(GLenum target, GLenum pname, GLint param)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glTexParameteri);
        a.target = target;
        a.pname = pname;
        a.param = param;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glGenRenderbuffers(GLsizei n, GLuint* renderbuffers)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glGenRenderbuffers);
        a.n = n;
        a.renderbuffers = renderbuffers;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glBindRenderbuffer(GLenum target, GLuint renderbuffer)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glBindRenderbuffer);
        a.target = target;
        a.renderbuffer = renderbuffer;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glRenderbufferStorage);
        a.target = target;
        a.internalformat = internalformat;
        a.width = width;
        a.height = height;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glDeleteRenderbuffers(GLsizei n, GLuint* renderbuffers)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glDeleteRenderbuffers);
        a.n = n;
        a.renderbuffers = renderbuffers;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glActiveTexture(GLenum texture)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glActiveTexture);
        a.texture = texture;
        SET_SEND_MSG(a);
        FUNC_END
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LNGL::glDeleteProgram(GLuint program)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glDeleteProgram);
        a.program = program;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glGetProgramiv(GLuint program, GLenum pname, GLint *params)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glGetProgramiv);
        a.program = program;
        a.pname = pname;
        a.params = params;
        SET_SEND_MSG(a);
        FUNC_END
    }

    void LNGL::lnBuildShader(const ShaderBuildArgs* args, GLenum* program, LNShaderCompileResult* result, LRefTString* log)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(lnBuildShader);
        a.args = args;
        a.program = program;
        a.result = result;
        a.log = log;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint * size, GLenum* type, GLchar* name)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glGetActiveUniform);
        a.program = program;
        a.index = index;
        a.bufSize = bufSize;
        a.length = length;
        a.size = size;
        a.type = type;
        a.name = name;
        SET_SEND_MSG(a);
        FUNC_END
    }
    GLint LNGL::glGetUniformLocation(GLuint program, const GLchar* name)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glGetUniformLocation);
        a.program = program;
        a.name = name;
        SET_SEND_MSG(a);
        FUNC_END_RETURN(GLint);
    }
    void LNGL::glUseProgram(GLuint program)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glUseProgram);
        a.program = program;
        SET_SEND_MSG(a);
        FUNC_END
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LNGL::glUniform1i(GLint location, GLint v0)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glUniform1i);
        a.location = location;
        a.v0 = v0;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glUniform1f(GLint location, GLfloat v0)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glUniform1f);
        a.location = location;
        a.v0 = v0;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glUniform2f(GLint location, GLfloat v0, GLfloat v1)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glUniform2f);
        a.location = location;
        a.v0 = v0;
        a.v1 = v1;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glUniform3f);
        a.location = location;
        a.v0 = v0;
        a.v1 = v1;
        a.v2 = v2;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glUniform4f);
        a.location = location;
        a.v0 = v0;
        a.v1 = v1;
        a.v2 = v2;
        a.v3 = v3;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glUniform4fv(GLint location, GLsizei count, const GLfloat* value)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glUniform4fv);
        a.location = location;
        a.count = count;
        a.value = value;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glUniformMatrix4fv);
        a.location = location;
        a.count = count;
        a.transpose = transpose;
        a.value = value;
        SET_SEND_MSG(a);
        FUNC_END
    }
    void LNGL::glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* 	size, GLenum* type, GLchar* name)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glGetActiveAttrib);
        a.program = program;
        a.index = index;
        a.bufSize = bufSize;
        a.length = length;
        a.size = size;
        a.type = type;
        a.name = name;
        SET_SEND_MSG(a);
        FUNC_END
    }
    GLint LNGL::glGetAttribLocation(GLuint program, const GLchar* name)
    {
        FUNC_BEGIN
        DEF_ARGS_STRUCT(glGetAttribLocation);
        a.program = program;
        a.name = name;
        SET_SEND_MSG(a);
        FUNC_END_RETURN(GLint);
    }


//=============================================================================
// ■ OpenGLThread
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void OpenGLThread::_buildShader(const LNGL::ShaderBuildArgs* args, GLenum* program, LNShaderCompileResult* result, LRefTString* log)
    {
        *result = LN_SHADERRESULT_ERROR; // エラーにしておく
        log->clear();

        GLuint mVertexShader   = glCreateShader( GL_VERTEX_SHADER );
        LN_CHECK_GLERROR_D();

	    GLuint mFragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
        LN_CHECK_GLERROR_D();

        if ( !_compileShader( mVertexShader, args->vs_count, args->vs_codes, args->vs_sizes, log ) ) { return; }

        if ( !_compileShader( mFragmentShader, args->fs_count, args->fs_codes, args->fs_sizes, log ) ) { return; }

        // プログラムオブジェクトの作成
        GLuint mProgram = glCreateProgram();
        LN_CHECK_GLERROR_D();

        // シェーダオブジェクトのシェーダプログラムへの登録
        glAttachShader( mProgram, mVertexShader );
        LN_CHECK_GLERROR_D();

        glAttachShader( mProgram, mFragmentShader );
        LN_CHECK_GLERROR_D();

        

        // シェーダプログラムのリンク
        GLint linked;
        glLinkProgram( mProgram );
        LN_CHECK_GLERROR_D();

        glGetProgramiv( mProgram, GL_LINK_STATUS, &linked );
        LN_CHECK_GLERROR_D();

        // ログがあるかチェック
        int log_size;
	    int length;
	    glGetProgramiv( mProgram, GL_INFO_LOG_LENGTH, &log_size );
        LN_CHECK_GLERROR_D();
	    if ( log_size > 1 )
	    {
            char* buf = LN_NEW char[log_size];
            glGetProgramInfoLog( mProgram, log_size, &length, buf );
            LN_CHECK_GLERROR_D();

            (*log) += "Program Info Log:\n";
            (*log) += buf;
            SAFE_DELETE_ARRAY( buf );
	    }


        // シェーダオブジェクトの削除
        glDeleteShader( mVertexShader );
        glDeleteShader( mFragmentShader );

        if ( linked == GL_FALSE ) { return; }

        // 返す
        *program = mProgram;
        
        

        //---------------------------------------------------------
        // ログがある場合はとりあえず警告にしておく

        if ( log->empty() )
        {
            *result = LN_SHADERRESULT_SUCCESS;
        }
        else
        {
            *result = LN_SHADERRESULT_WARNING;
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    bool OpenGLThread::_compileShader( GLuint shader_, int code_count_, const char** code_, GLint* size_, LRefTString* log )
    {
        // シェーダオブジェクトにソースプログラムをセット
        glShaderSource( shader_, code_count_, (const GLchar **)code_, size_ );
        LN_CHECK_GLERROR_D();
      
        // シェーダのコンパイル
        GLint compiled;
        glCompileShader( shader_ );
        LN_CHECK_GLERROR_D();

        glGetShaderiv( shader_, GL_COMPILE_STATUS, &compiled );
        LN_CHECK_GLERROR_D();
        
        // ログがあるかチェック (ログの長さは、最後のNULL文字も含む)
        int log_size;
        int length;
        glGetShaderiv( shader_, GL_INFO_LOG_LENGTH , &log_size );
        LN_CHECK_GLERROR_D();
        if ( log_size > 1 )
        {
            char* buf = LN_NEW char[log_size];
            glGetShaderInfoLog( shader_, log_size, &length, buf );
            LN_CHECK_GLERROR_D();

            (*log) += "Shader Info Log:\n";
            (*log) += buf;
            SAFE_DELETE_ARRAY( buf );
        }

        return ( compiled != GL_FALSE );
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