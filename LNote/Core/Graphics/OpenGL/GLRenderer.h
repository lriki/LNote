//=============================================================================
//�y GLRenderer �z
//-----------------------------------------------------------------------------
///**
//  @file       GLRenderer.h
//  @brief      GLRenderer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"
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
class GraphicsDevice;
class VertexBuffer;
class IndexBuffer;

//=============================================================================
// �� Renderer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
class Renderer
    : public Base::ReferenceObject
    , public IRenderer
{
public:

    /// initialize() �ɓn���������f�[�^
    struct InitData
    {
        Base::LogFile*              LogFile;
        OpenGL::GraphicsDevice*     GraphicsDevice;
        //OpenGL::PlatformContext*    PlatformContext;
    }; 

public:

    Renderer();

    virtual ~Renderer();

    LN_REFOBJ_METHODS;

public:

    /// ������
    void initialize( const InitData& init_data_ );

    /// �����_�����O�X�e�[�g�̐ݒ�
    virtual LNRESULT setRenderState( const LRenderState& state_ );

    /// �����_�����O�^�[�Q�b�g�̎擾 (�Q�ƃJ�E���g�ɕω��̓i�V)
    virtual ITexture* getRenderTarget( u32 index_ ) { return mCurrentRenderTargets[ index_ ]; }

    // �����_�����O�^�[�Q�b�g�̐ݒ�
    virtual LNRESULT setRenderTarget( u32 index_, ITexture* texture_, bool reset_ = false );

    /// �[�x�o�b�t�@�̎擾 (�Q�ƃJ�E���g�ɕω��̓i�V)
    virtual ITexture* getDepthBuffer() { return mCurrentDepthBuffer; }

    /// �[�x�o�b�t�@�̐ݒ�
    virtual LNRESULT setDepthBuffer( ITexture* texture_ );

    /// �r���[�|�[�g��`�̎擾
    virtual const Geometry::Rect& getViewport() const;

	/// �r���[�|�[�g��`�̐ݒ�
	virtual LNRESULT setViewport( const Geometry::Rect& viewport_rect_ );

    /// �V�U�[��`�̎擾
    virtual const Geometry::Rect& getScissor() const;

	/// �V�U�[��`�̐ݒ�
	virtual LNRESULT setScissor( const Geometry::Rect& rect_ );

    /// ���_�o�b�t�@�̐ݒ�
	virtual LNRESULT setVertexBuffer( IVertexBuffer* vertex_buffer_, bool reset_ = false );

    /// �C���f�b�N�X�o�b�t�@�̐ݒ�
	virtual LNRESULT setIndexBuffer( IIndexBuffer* index_buffer_, bool reset_ = false );

    /// �ݒ肳��Ă���J���[�o�b�t�@�A�[�x�o�b�t�@���N���A����
	virtual LNRESULT clear( bool target_, bool depth_, u32 color_, lnfloat z_ = 1.0f );

    /// �v���~�e�B�u�`��
	virtual LNRESULT drawPrimitive( LNPrimitiveType primitive_, u32 start_vertex_, u32 primitive_count_ );

    /// �v���~�e�B�u�`�� ( �C���f�b�N�X�t�� )
	virtual LNRESULT drawPrimitiveIndexed( LNPrimitiveType primitive_, u32 start_index_, u32 primitive_count_ );

    /// �V�[���J�n
	virtual LNRESULT beginScene();

	/// �V�[���I��
	virtual LNRESULT endScene();

public: 

    /// GraphicsDevice::onLostDevice() ����Ă΂��
    void onLostDevice();

    /// GraphicsDevice::onResetDevice() ����Ă΂��
    void onResetDevice();

private:

    /// ��ŕύX���Ȃ������_�����O�X�e�[�g���f�o�C�X�ɐݒ肷��
    void _initRenderState();

    /// ���ۂɃ����_�����O�X�e�[�g���f�o�C�X�ɐݒ肷��
    LNRESULT _updateRenderState( bool reset_ );

    /// ���ۂɃJ���[�A�[�x�o�b�t�@��ݒ肷��
    void _refreshFramebuffer();

private:

    Base::LogFile*          mLogFile;
    GraphicsDevice*	        mGraphicsDevice;    ///< �Q�ƃJ�E���g�͑��₳�Ȃ�
    //PlatformContext*        mPlatformContext;

    GLuint                  mGLFramebuffer;

    LRenderState            mCurrentRenderState;
    LRenderState            mRequestRenderState;

    ITexture*               mCurrentRenderTargets[ LN_MAX_MULTI_RENDERTARGETS ];
    ITexture*               mCurrentDepthBuffer;
    Geometry::Size			mCurrentRenderTarget0Size;
    Geometry::Size			mCurrentRenderTarget0RealSize;


    Geometry::Rect			mCurrentViewportRect;
    Geometry::Rect			mCurrentViewportRealRect;

    VertexBuffer*           mCurrentVertexBuffer;
    IndexBuffer*            mCurrentIndexBuffer;

    Geometry::Rect			mCurrentScissorRect;
    bool                    mEnableScissorTest;
    bool                    mSceneBegan;
    bool                    mIsDeviceLost;

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