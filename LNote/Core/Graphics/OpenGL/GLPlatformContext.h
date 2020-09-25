//=============================================================================
//�y GLPlatformContext �z
//-----------------------------------------------------------------------------
///**
//  @file       GLPlatformContext.h
//  @brief      GLPlatformContext
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#if defined(LNOTE_WINDOWS)
    //#include <gl/glew.h>
    //#include <gl/wglew.h>
#elif defined(LNOTE_ANDROID)
    #include <EGL/egl.h>
    #include <EGL/eglext.h>
    #include <GLES/gl.h>
    #include <GLES/glext.h> // for glGenFramebuffersOES
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif
#include "../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System { class Manager; }
    
namespace Graphics
{
namespace OpenGL
{

enum LNGLenum
{
#if defined(LNOTE_WINDOWS)
    LNGL_WRITE_ONLY = GL_WRITE_ONLY,
#elif defined(LNOTE_ANDROID)
    LNGL_WRITE_ONLY = GL_WRITE_ONLY_OES,
#endif
};

//=============================================================================
// �� PlatformContext �N���X
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
class PlatformContext
{
public:

    struct InitData
    {
        Base::LogFile*      LogFile;
        System::Manager*    SystemManager;
    };

public:

    PlatformContext();

    ~PlatformContext();

public:

    /// ������
    void initialize( const InitData& init_data_ );

    /// �V�[���J�n
    void beginScene();

    /// �V�[���I��
    void endScene();

    /// �o�b�N�o�b�t�@���E�B���h�E�ɓ]��
    void present();

    /// �f�o�C�X���Z�b�g���O GraphicsDevice ����Ă΂��
    void onLostDevice();

    /// �f�o�C�X���Z�b�g��� GraphicsDevice ����Ă΂��
    void onResetDevice();

public:

    static void glClearDepth( GLclampf depth_ );

    static void* glMapBuffer( GLenum target_, GLuint buffer_ );

    static GLboolean glUnmapBuffer( GLenum target_ );

public:

    /// LNDeclType -> GLenum, size(�v�f�� LN_DECL_FLOAT3 �Ȃ� 3), normalized(���K���̗L��)
    static void convertElementLNToGL( LNDeclType ln_type_, GLenum* gl_type_, GLint* size_, GLboolean* normalized_ );

private:

    Base::LogFile*      mLogFile;
    System::Manager*    mSystemManager;

#if defined(LNOTE_WINDOWS)
    HWND    mHWND;
    HDC     mHDC;
    HGLRC   mGLRC;
#elif defined(LNOTE_GLES)
    EGLDisplay  mEGLDisplay;
    EGLConfig   mEGLConfig;
    EGLContext  mEGLContext;
    EGLSurface  mEGLSurface;
    
    static PFNGLMAPBUFFEROESPROC   m_glMapBufferOES;
    static PFNGLUNMAPBUFFEROESPROC m_glUnmapBufferOES;
    
#endif

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