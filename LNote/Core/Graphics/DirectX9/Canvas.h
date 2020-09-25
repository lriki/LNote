//=============================================================================
//�y Canvas �z
//-----------------------------------------------------------------------------
///**
//  @file       Canvas.h
//  @brief      Canvas
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"
#include "Texture.h"

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
class GraphicsDevice;

//=============================================================================
// �� CanvasBase �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class CanvasBase
    : public Base::ReferenceObject
    , public ICanvas
{
public:

    /// �R���X�g���N�^
    CanvasBase( GraphicsDevice* device_ );

    /// �f�X�g���N�^
    virtual ~CanvasBase();

    LN_REFOBJ_METHODS;

public:

    /// �f�o�C�X���Z�b�g���O GraphicsDevice ����Ă΂��
    virtual LNRESULT onLostDevice() = 0;

    /// �f�o�C�X���Z�b�g��� GraphicsDevice ����Ă΂��
    virtual LNRESULT onResetDevice() = 0;

protected:

    GraphicsDevice* mGraphicsDevice;
};

//=============================================================================
// �� DefaultCanvas �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		DX9 �̃f�t�H���g�ō쐬�����X���b�v�`�F�C�����g�����L�����o�X
//*/
//=============================================================================
class DefaultCanvas
    : public CanvasBase
{
public:

    DefaultCanvas( GraphicsDevice* device_ );

    virtual ~DefaultCanvas();

public:

    /// ������
    LNRESULT initialize( const Geometry::Size& size_ );

    /// �L�����o�X�T�C�Y���擾����
    virtual const Geometry::Size& getSize() { return mCanvasSize; }

    /// �L�����o�X�T�C�Y��ύX����
    virtual LNRESULT resize( u32 width_, u32 height_ );

    /// �o�b�N�o�b�t�@�̎擾
    virtual ITexture* getBackbuffer() { return mBackbuffer; }

    /// �o�b�N�o�b�t�@�̃����_�����O�^�[�Q�b�g�A�[�x�o�b�t�@��ݒ肷��
    virtual LNRESULT activate();

    /// �o�b�N�o�b�t�@�̓��e���E�B���h�E�֓]������
    virtual LNRESULT present();

    /// �`���̃E�B���h�E�T�C�Y��ݒ肷��
    virtual void setWindowSize( const Geometry::Size& size_ );

    /// �L�����o�X���E�B���h�E�֓]������ۂ̍��W�ϊ��s��̎擾
    virtual const LMatrix& getScreenTransform() const { return mScreenTransformMatrix; }

	/// present() �̓]����E�B���h�E�̃E�B���h�E�n���h���̐ݒ�
	virtual void setTargetWindowHandle(HWND hwnd) { mTargetWindowHandle = hwnd; }

public:

    /// �f�o�C�X���Z�b�g���O GraphicsDevice ����Ă΂��
    virtual LNRESULT onLostDevice();

    /// �f�o�C�X���Z�b�g��� GraphicsDevice ����Ă΂��
    virtual LNRESULT onResetDevice();

private:

    IDirect3DDevice9*       mDxDevice;
    SystemCreatedTexture*   mBackbuffer;
    ITexture*               mDepthBuffer;

    Geometry::Size			mWindowSize;
    Geometry::Size			mCanvasSize;
    LColor                  mBackgroundColor;
    u32                     mBackgroundColorCode;

    RECT                    mViewDestRect;           ///< �]�����`
    LMatrix                 mScreenTransformMatrix;

	HWND					mTargetWindowHandle;
};

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