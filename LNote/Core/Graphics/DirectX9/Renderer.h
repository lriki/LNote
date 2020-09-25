//=============================================================================
//�y Renderer �z
//-----------------------------------------------------------------------------
///**
//  @file       Renderer.h
//  @brief      Renderer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"

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
        Base::LogFile*      LogFile;
        GraphicsDevice*     GraphicsDevice;
        D3DCAPS9*           DxCaps;
    }; 

public:

    Renderer();

    virtual ~Renderer();

    LN_REFOBJ_METHODS;

public:

    /// ������
    LNRESULT initialize( const InitData& init_data_ );

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

    /// �f�o�C�X���Z�b�g���O GraphicsDevice ����Ă΂��
    LNRESULT onLostDevice();

    /// �f�o�C�X���Z�b�g��� GraphicsDevice ����Ă΂��
    LNRESULT onResetDevice();

private:

    /// ��ŕύX���Ȃ������_�����O�X�e�[�g���f�o�C�X�ɐݒ肷��
    LNRESULT _initRenderState();

    /// ���ۂɃ����_�����O�X�e�[�g���f�o�C�X�ɐݒ肷��
    LNRESULT _updateRenderState();

    /// ���݂̃����_�����O�X�e�[�g���f�o�C�X�ɍĐݒ肷��
    LNRESULT _resetRenderState();

    // �V�U�[��`�̐ݒ�
    LNRESULT _setScissorRect( const Geometry::Rect& rect_ );

private:

    Base::LogFile*      mLogFile;
    GraphicsDevice*	    mGraphicsDevice;    ///< �Q�ƃJ�E���g�͑��₳�Ȃ�

    IDirect3DDevice9*       mDxDevice;
    D3DCAPS9*               mDxCaps;

    LRenderState            mCurrentRenderState;
    LRenderState            mRequestRenderState;

    ITexture*               mCurrentRenderTargets[ LN_MAX_MULTI_RENDERTARGETS ];
    ITexture*               mCurrentDepthBuffer;

    Geometry::Rect			mCurrentViewportRect;

    VertexBuffer*           mCurrentVertexBuffer;
    IndexBuffer*            mCurrentIndexBuffer;

    Geometry::Rect			mCurrentScissorRect;
    bool                    mEnableScissorTest;
    bool                    mSceneBegan;


    // mCurrentRenderTargets �́AITexture* �ŕs��o��悤�Ȃ� IDirect3DSurface9* �ł� OK
    //  ���E�E�E�Ǝv�������ǁA�f�o�C�X���Z�b�g���ɍĐݒ肷��Ƃ��ɕK�v�����B
    //    �����A1�t���[���I�����ɂ��ׂẴ^�[�Q�b�g�� NULL �ɂ���Ƃ��Ȃ���Ȃ����ǁB
    
    //SamplerState        mCurrentSamplerState[ TEXTURE_STAGE_MAX_COUNT ];
    //SamplerState        mRequestSamplerState[ TEXTURE_STAGE_MAX_COUNT ];
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