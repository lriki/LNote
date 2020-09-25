//=============================================================================
//�y GLCanvas �z
//-----------------------------------------------------------------------------
///**
//  @file       GLCanvas.h
//  @brief      GLCanvas
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"
#include "GLPlatformContext.h"
#include "GLTexture.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System { class IWindow; }

namespace Graphics
{
namespace OpenGL
{
class GraphicsDevice;

//=============================================================================
// �� DefaultCanvas �N���X
//-----------------------------------------------------------------------------
///**
//  @brief
//
//  @note
//              OpenGL �̃L�����o�X�͏��1�B
//*/
//=============================================================================
class DefaultCanvas
    : public Base::ReferenceObject
    , public ICanvas
{
public:

    struct InitData
    {
        //OpenGL::PlatformContext*    PlatformContext;
        System::IWindow*            Window;
        Geometry::Size				CanvasSize;
    };

public:

    DefaultCanvas( GraphicsDevice* device_ );

    virtual ~DefaultCanvas();
    
    LN_REFOBJ_METHODS;

public:

    /// ������
    void initialize( const InitData& init_data_ );//System::Window* window_ );

    /// �L�����o�X�T�C�Y���擾����
    virtual const Geometry::Size& getSize() { return mCanvasSize; }

    /// �L�����o�X�T�C�Y��ύX����
    virtual LNRESULT resize( lnU32 width_, lnU32 height_ );

    /// ���z�o�b�N�o�b�t�@�̎擾
    virtual ITexture* getBackbuffer() { LN_PRINT_NOT_IMPL_FUNCTION; return NULL; }

    /// ���z�o�b�N�o�b�t�@�̃����_�����O�^�[�Q�b�g�A�[�x�o�b�t�@��ݒ肷��
    virtual LNRESULT activate();

    /// �o�b�N�o�b�t�@�̓��e���E�B���h�E�֓]������
    virtual LNRESULT present();

    /// �`���̃E�B���h�E�T�C�Y��ݒ肷��
    virtual void setWindowSize( const Geometry::Size& size_ );

    /// �L�����o�X���E�B���h�E�֓]������ۂ̍��W�ϊ��s��̎擾
    virtual const LMatrix& getScreenTransform() const { return mScreenTransformMatrix; }

#if defined(LNOTE_WINDOWS)
	/// present() �̓]����E�B���h�E�̃E�B���h�E�n���h���̐ݒ�
	virtual void setTargetWindowHandle(HWND hwnd) { }
#endif

public:

    const Geometry::Size& getWindowSize() { return mWindowSize; }

    /// �f�o�C�X���Z�b�g���O GraphicsDevice ����Ă΂��
    void onLostDevice();

    /// �f�o�C�X���Z�b�g��� GraphicsDevice ����Ă΂�� (EGLDisplay�AEGLConfig �������ς݂ł��邱��)
    void onResetDevice();

private:

    GraphicsDevice*         mGraphicsDevice;
    //PlatformContext*        mPlatformContext;
    System::IWindow*        mWindow;


    //EGLDisplay              mEGLDisplay;
    //EGLSurface              mEGLSurface;

    Geometry::Size			mWindowSize;
    Geometry::Size			mCanvasSize;
    LColor                  mBackgroundColor;
    u32                     mBackgroundColorCode;

    ITexture*               mCanvasBuffer;
    ITexture*               mCanvasDepthBuffer;

    //RECT                    mViewDestRect;           ///< �]�����`
    LMatrix                 mScreenTransformMatrix;

    LMatrix                 mTransformMatrix;
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