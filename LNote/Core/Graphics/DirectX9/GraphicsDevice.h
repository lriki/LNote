//=============================================================================
//�y GraphicsDevice �z
//-----------------------------------------------------------------------------
///**
//  @file       GraphicsDevice.h
//  @brief      GraphicsDevice
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <list>
#include "../../Base/Cache.h"
#include "../../Thread/EventFlag.h"
#include "../../Thread/Semaphore.h"
#include "../../System/Manager.h"
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
class Manager;

namespace DirectX9
{
class Renderer;
class CanvasBase;
class DefaultCanvas;
class Texture;
class RenderTargetTexture;
class DepthBuffer;
class Shader;
typedef Base::CacheObjectManager< Texture >     TextureCache;
typedef Base::CacheObjectManager< Shader >      ShaderCache;

//=============================================================================
// �� GraphicsDevice �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�O���t�B�b�N�X�f�o�C�X
//*/
//=============================================================================
class GraphicsDevice
    : public Base::ReferenceObject
    , public TextureCache
    , public ShaderCache
    , public IGraphicsDevice
{
public:

	/// �������f�[�^
	struct InitData
	{
        Base::LogFile*      LogFile;
        Manager*            Manager;
        //System::IWindow*    MainWindow;
        System::Manager*    SystemManager;
        File::Manager*      FileManager;
        Geometry::Size		CanvasSize;         ///< �L�����o�X (�o�b�N�o�b�t�@) �̃T�C�Y
        bool	            FullScreen;	        ///< �t���X�N���[���ŋN������ꍇ�� true
        bool	            EnableVSyncWait;    ///< VSync ��҂ꍇ�� true
        bool                EnableFPUPreserve;  ///< ���������_�̉��Z���x�𗎂Ƃ��Ȃ��悤�ɂ���ꍇ�� true
        u32                 TextureCacheSize;
        u32                 ShaderCacheSize;
    };

public:

    GraphicsDevice();

	virtual ~GraphicsDevice();

    LN_REFOBJ_METHODS;

public:

    /// ������
    LNRESULT initialize( const InitData& init_data_ );

    /// �I������
    virtual void finalize();

    /// �g�p���� GraphicsAPI �^�C�v�̎擾
    virtual LNGraphicsAPI getGraphicsAPI() { return LN_GRAPHICSAPI_DIRECTX9; }

    /// �`��N���X�̎擾
    virtual IRenderer* getRenderer();

    /// �f�t�H���g�̃o�b�N�o�b�t�@�������L�����o�X�̎擾
    virtual ICanvas* getDefaultCanvas();

    /// ���_�o�b�t�@�̍쐬
    virtual LNRESULT createVertexBuffer( IVertexBuffer** obj_, LNVertexElemenst* elements_, u32 vertex_num_, const void* data_, bool is_dynamic_ = false );

    /// �C���f�b�N�X�o�b�t�@�̍쐬
    virtual LNRESULT createIndexBuffer( IIndexBuffer** obj_, u32 index_num_, const u16* data_, bool is_dynamic_, bool is_16bit_ );

    /// �e�N�X�`���̍쐬
    virtual LNRESULT createTexture( ITexture** obj_, u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ );

    /// �e�N�X�`���̍쐬 ( �������ɓW�J���ꂽ�摜�f�[�^���� )
    virtual LNRESULT createTexture( ITexture** obj_, const void* data_, u32 size_, u32 color_key_ = 0, u32 levels_ = 1, LNSurfaceFormat format_ = LN_FMT_A8R8G8B8, LSharingKey key_ = lnNullKey );

    /// �e�N�X�`���̍쐬 (���̓X�g���[������)
    virtual LNRESULT createTexture( ITexture** obj_, File::IInStream* stream_, u32 color_key_ = 0, u32 levels_ = 1, LNSurfaceFormat format_ = LN_FMT_UNKNOWN, LSharingKey key_ = lnNullKey );

    /// �e�N�X�`���̍쐬 (�t�@�C������)
    virtual void createTexture( ITexture** obj_, const lnChar* filename_, u32 color_key_, u32 levels_, LNSurfaceFormat format_, LSharingKey key_ );

    /// �����_�[�^�[�Q�b�g�e�N�X�`���̍쐬
    virtual LNRESULT createRenderTarget( ITexture** obj_, u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ );

    /// �[�x�o�b�t�@�̍쐬
    virtual LNRESULT createDepthBuffer( ITexture** obj_, u32 width_, u32 height_, LNSurfaceFormat format_ );

    /// �V�F�[�_�̍쐬
    virtual LNRESULT createShader( IShader** obj_, const void* data_, u32 size_, LSharingKey key_ = lnNullKey );

    /// �V�F�[�_�̍쐬 ( ���̓X�g���[������ )
    virtual LNRESULT createShader( IShader** obj_, File::IInStream* stream_, LSharingKey key_ = lnNullKey );
	
    /// �V�F�[�_�̍쐬 (�t�@�C������)
    virtual void createShader( IShader** obj_, const lnChar* filename_, LSharingKey key_ );

    /// �L���b�V���̃N���A
    virtual void clearCache( bool texture_, bool shader_ );

    /// IDirect3DDevice9 �ւ̃|�C���^�̎擾
    virtual IDirect3DDevice9* getDX9Device() { return mDxDevice; }

    /// �f�o�C�X���Z�b�g�O����
    virtual void onLostDevice();

    /// �f�o�C�X���Z�b�g�㏈��
    virtual void onResetDevice();

    /// �f�o�C�X���Z�b�g
    virtual void resetDevice();

public:

    Manager* getManager() { return mManager; }

    /// �f�o�C�X�̃��Z�b�g (DefaultCanvas ����Ăяo�����B�t���X�N���[���ɂ���Ƃ��A���ƍ����͖���) (window_bb_size_ �̓E�B���h�E���[�h���̃o�b�N�o�b�t�@�T�C�Y)
    LNRESULT resetDevice( bool fullscreen_, const Geometry::Size& window_bb_size_ );

    /// �t���X�N���[���ɕύX�\���𔻒肷��
    bool canFullScreen() const { return ( mCanvasList.size() == 1 ); }

    /// �t���X�N���[�����𔻒肷��
    bool isFullscreen() const { return mIsFullScreen; }

    /// CanvasBase �̃R���X�g���N�^����Ă΂��
    void addCanvas( CanvasBase* canvas_ ) { mCanvasList.push_back( canvas_ ); }

    /// CanvasBase �̃f�X�g���N�^����Ă΂��
    void removeCanvas( CanvasBase* canvas_ ) { mCanvasList.remove( canvas_ ); }

    /// RenderTargetTexture �f�X�g���N�^����Ă΂��
    void removeRenderTarget( RenderTargetTexture* texture_ ) { mRenderTargetList.remove( texture_ ); }

    /// DepthBuffer �̊��� �f�X�g���N�^����Ă΂��
    void removeDepthBuffer( DepthBuffer* texture_ ) { mDepthBufferList.remove( texture_ ); }

    /// Shader �̃f�X�g���N�^����Ă΂��
    void removeShader( Shader* shader_ ) { mShaderList.remove( shader_ ); }

    /// �f�o�C�X���Z�b�g�̕K�v������ꍇ�� true
    bool isDeviceLost() { return mIsDeviceLost.isTrue(); }

    /// (Canvas::preset() ����Ă΂��B��x�Ă΂��� isDeviceLost() �� true ��Ԃ��悤�ɂȂ�)
    void postDeviceLostEvent();

    D3DPRESENT_PARAMETERS* getDXPresentParameters() { return &mPresentParameters; }

    /// (ShaderPass::begin() ����Ă΂��)
    void setSamplerState( int index_, const LSamplerState& state_, bool reset_ = false );

private:

    /// �f�o�C�X�̐��\�`�F�b�N
    LNRESULT _checkDeviceInformation();

    /// �X�N���[�����[�h ( mIsFullScreen ) �ɉ����� mPresentParameters �ݒ�
	void _setPresentParameters( const Geometry::Size& window_bb_size_ );

private:

    typedef std::list< CanvasBase* >            CanvasList;
    typedef std::list< RenderTargetTexture* >   RenderTargetList;
    typedef std::list< DepthBuffer* >           DepthBufferList;
    typedef std::list< Shader* >                ShaderList;

private:

    Base::LogFile*              mLogFile;
    Manager*                    mManager;
    System::Manager*            mSystemManager;
    System::IWindow*            mMainWindow;
    File::Manager*              mFileManager;
    Renderer*                   mRenderer;

    IDirect3D9*		            mDirect3D;
    IDirect3DDevice9*	        mDxDevice;
    D3DPRESENT_PARAMETERS       mPresentParameters;	///< �v���[���e�[�V�����p�����[�^

    // ��������
    D3DCAPS9                    mDxCaps;
    D3DDISPLAYMODE              mDxDisplayMode;
    D3DTEXTUREFILTERTYPE        mCurrentMinFilter;
	D3DTEXTUREFILTERTYPE        mCurrentMagFilter;
	D3DTEXTUREFILTERTYPE        mCurrentMipFilter;
	D3DTEXTUREFILTERTYPE        mBestMinFilter;
	D3DTEXTUREFILTERTYPE        mBestMagFilter;
	D3DTEXTUREFILTERTYPE        mBestMipFilter;
    DWORD                       mMSAAQualityWindowed;
    DWORD                       mMSAAQualityFullScreen;
    D3DDEVTYPE				    mDeviceType;        ///< �f�o�C�X�̎�ށB_checkDeviceInformation() �Ń`�F�b�N���� initialize() �ɓ`���邽�߂ɗp��

    CanvasList                  mCanvasList;
    DefaultCanvas*              mDefaultCanvas;
    RenderTargetList            mRenderTargetList;
    DepthBufferList             mDepthBufferList;
    ShaderList                  mShaderList;        ///< �L�[�������Ȃ����̂� onLostDevice() �̂��߂ɗp�ӁB�ꉞ�L�[�������̂����ׂĎ����Ă���

    Base::TempBuffer            mTempBuffer;
    Thread::EventFlag           mIsDeviceLost;
    Thread::Semaphore           mDeviceLostEventSemaphore;
    bool                        mIsFullScreen;
    bool                        mEnableFPUPreserve;

    LSamplerState               mSamplerStatus[LN_MAX_SAMPLER_STAGE_INDEX];
    
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