//=============================================================================
//【 LNGL 】
//-----------------------------------------------------------------------------
///**
//  @file       LNGL.h
//  @brief      LNGL
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Common/Common.h"
#include "../../Common/RenderState.h"
#include "../../../Thread/Thread.h"
#include "../../../Thread/EventFlag.h"
#include "../../../Thread/Mutex.h"

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
class OpenGLThread;

//=============================================================================
// ■ LNGL
//-----------------------------------------------------------------------------
///**
//  @brief      少し強引な OpenGL スレッドセーフ実装
//*/
//=============================================================================
class LNGL
{
public:

    struct InitData
    {
        Base::LogFile*      LogFile;
        System::Manager*    SystemManager;
    };

    struct ShaderBuildArgs
    {
        int vs_count;
        const char** vs_codes;
        int* vs_sizes;
        int fs_count;
        const char** fs_codes;
        int* fs_sizes;
    };

public:

    static void initialize( const InitData& init_data_ );
    static void finalize();
    static void beginScene();
    static void endScene();
    static void present();

    static void     glGenBuffers(GLsizei n, GLuint* buffers);
    static void     glBindBuffer(GLenum target, GLuint buffer);
    static void     glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
    static void     glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);

    static void*    glMapBuffer(GLenum target, GLenum access);
    static GLboolean glUnmapBuffer(GLenum target);
    static void     glDeleteBuffers(GLsizei n, const GLuint* buffers);

    static void glEnable(GLenum cap);
    static void glDisable(GLenum cap);
    static void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
    static void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
    //static void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    //static void glClearDepth(GLclampf depth);
    //static void glClear(GLbitfield mask);
    static void lnClear(bool target, bool depth, const LColor* color, lnfloat z);
    static void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
    static void glEnableVertexAttribArray(GLuint index);
    static void glDrawArrays(GLenum mode, GLint first, GLsizei count);
    static void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
    static void glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid* indices);  // glDrawRangeElements GLSL ES not supported
    

    static void glGenFramebuffers(GLsizei n, GLuint* ids);
    static void glDeleteFramebuffers(GLsizei n, GLuint *framebuffers);
    static void glBindFramebuffer(GLenum target, GLuint framebuffer);
    static void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    static void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
    static void glFrontFace(GLenum mode);
    //static void glBlendEquation(GLenum mode);
    //static void glBlendFunc(GLenum sfactor, GLenum dfactor);
    //static void glCullFace(GLenum mode);
    //static void glPolygonMode(GLenum face, GLenum mode);
    //static void glDepthMask(GLboolean flag);
    static void lnSetRenderState(const LRenderState* state);

    static void glGenTextures(GLsizei n, GLuint* textures);
    static void glBindTexture(GLenum target, GLuint texture);
    static void glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data);
    static void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data);
    static void glPixelStorei(GLenum pname, GLint param);
    static void glDeleteTextures(GLsizei n, const GLuint* textures);
    static void glTexParameteri(GLenum target, GLenum pname, GLint param);  // post
    static void glGenRenderbuffers(GLsizei n, GLuint* renderbuffers);
    static void glBindRenderbuffer(GLenum target, GLuint renderbuffer);
    static void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
    static void glDeleteRenderbuffers(GLsizei n, GLuint* renderbuffers);
    static void glActiveTexture(GLenum texture);    // post

    // シェーダ系は post 無し
    static void glDeleteProgram(GLuint program);
    static void glGetProgramiv(GLuint program, GLenum pname, GLint *params);
    /*
    static void glDeleteShader(GLuint shader);
    
    static GLuint glCreateShader(GLenum shaderType);
    static GLuint glCreateProgram(void);
    static void glAttachShader(GLuint program, GLuint shader);
    static void glLinkProgram(GLuint program);
    static void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
    static void glShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
    static void glCompileShader(GLuint shader);
    static void glGetShaderiv(GLuint shader, GLenum pname, GLint* params);
    static void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
    */
    static void lnBuildShader(const ShaderBuildArgs* args, GLenum* program, LNShaderCompileResult* result, LRefTString* log);

    static void glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint * size, GLenum* type, GLchar* name);
    static GLint glGetUniformLocation(GLuint program, const GLchar* name);
    static void glUseProgram(GLuint program);

    // Matrix とかはデータ量多くなるので、uniform 系もまとめて post 無しで統一
    static void glUniform1i(GLint location, GLint v0);
    static void glUniform1f(GLint location, GLfloat v0);
    static void glUniform2f(GLint location, GLfloat v0, GLfloat v1);
    static void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
    static void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    static void glUniform4fv(GLint location, GLsizei count, const GLfloat* value);
    static void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    static void glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* 	size, GLenum* type, GLchar* name);
    static GLint glGetAttribLocation(GLuint program, const GLchar* name);


private:

    static OpenGLThread*        mOpenGLThread;
    static Thread::EventFlag    mStandbyGLThread;
    static Thread::EventFlag    mRunningNotify;
    static Thread::EventFlag    mReturnNotify;
    static Thread::Mutex        mMutex;
    static Base::TempBuffer     mMessageBuffer;

    friend class OpenGLThread;
};

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