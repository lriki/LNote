//=============================================================================
//�y GLGraphicsDevice �z
//-----------------------------------------------------------------------------
///**
//  @file       GLGraphicsDevice.h
//  @brief      GLGraphicsDevice
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <list>
#include "../../Base/Cache.h"
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
class Renderer;
class CanvasBase;
class DefaultCanvas;
class VertexBuffer;
class IndexBuffer;
class Texture;
class RenderTargetTexture;
class DepthBuffer;
class Shader;
class ShaderPass;
class ShaderManager;
typedef Base::CacheObjectManager< Texture >     TextureCache;

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
    , public IGraphicsDevice
{
public:

	/// �������f�[�^
	struct InitData
	{
        Base::LogFile*      LogFile;
        Graphics::Manager*  Manager;
        System::Manager*    SystemManager;
        File::Manager*      FileManager;
        Task::Manager*      TaskManager;        ///< OpenGL Shader �p�b��Ή�
        Geometry::Size		CanvasSize;         ///< �L�����o�X (�o�b�N�o�b�t�@) �̃T�C�Y
	    bool	            EnableVSyncWait;    ///< VSync ��҂ꍇ�� true (Android �ł͎��������ȋC�����邯�ǁc)
        lnU32               TextureCacheSize;
        lnU32               ShaderCacheSize;
    };

public:

    GraphicsDevice();

	virtual ~GraphicsDevice();

    LN_REFOBJ_METHODS;

public:

    /// ������
    void initialize( const InitData& init_data_ );

    /// �I������
    virtual void finalize();

    /// �g�p���� GraphicsAPI �^�C�v�̎擾
    virtual LNGraphicsAPI getGraphicsAPI() { return LN_GRAPHICSAPI_OPENGL; }

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
    virtual LNRESULT createRenderTarget( ITexture** obj_, u32 width_, u32 height_, u32 levels_ = 1, LNSurfaceFormat format_ = LN_FMT_A8R8G8B8 );

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

    /// �f�o�C�X���Z�b�g�̕K�v������ꍇ�� true
    virtual bool isDeviceLost() { return false; }

    /// �f�o�C�X���Z�b�g�O����
    virtual void onLostDevice();

    /// �f�o�C�X���Z�b�g�㏈��
    virtual void onResetDevice();

    /// �f�o�C�X���Z�b�g
    virtual void resetDevice() { LN_PRINT_NOT_IMPL_FUNCTION; } 

#if defined(LNOTE_DIRECTX)
    virtual IDirect3DDevice9* getDX9Device() { return NULL; }
#endif
    
public:

    Manager* getManager() { return mManager; }

    File::Manager* getFileManager() { return mFileManager; }

    /// �f�o�C�X�̃��Z�b�g (DefaultCanvas ����Ăяo�����B�t���X�N���[���ɂ���Ƃ��A���ƍ����͖���) (window_bb_size_ �̓E�B���h�E���[�h���̃o�b�N�o�b�t�@�T�C�Y)
    LNRESULT resetDevice( bool fullscreen_, const Geometry::Size& window_bb_size_ );

    /// �t���X�N���[���ɕύX�\���𔻒肷��
    bool canFullScreen() const { return false; }

    /// �t���X�N���[�����𔻒肷��
    bool isFullscreen() const { return false; }

    /// (VertexBuffer �f�X�g���N�^����Ă΂��)
    void removeVertexBuffer( VertexBuffer* vb_ ) { mVertexBufferList.remove( vb_ ); }

    /// (IndexBuffer �f�X�g���N�^����Ă΂��)
    void removeIndexBuffer( IndexBuffer* ib_ ) { mIndexBufferList.remove( ib_ ); }

    /// (Texture �f�X�g���N�^����Ă΂��)
    void removeTexture( Texture* texture_ ) { mTextureList.remove( texture_ ); }

    /// (RenderTargetTexture �f�X�g���N�^����Ă΂��)
    void removeRenderTarget( RenderTargetTexture* texture_ ) { mRenderTargetList.remove( texture_ ); }

    /// (DepthBuffer �̊��� �f�X�g���N�^����Ă΂��)
    void removeDepthBuffer( DepthBuffer* texture_ ) { mDepthBufferList.remove( texture_ ); }

    /// (Renderer::beginScene() ����)
    void commitChangesGLResource();

    


    /// �ꎞ�o�b�t�@�擾
    Base::TempBuffer& getTempBuffer() { return mTempBuffer; }


    void setCurrentShaderrPass( ShaderPass* pass_ );

    ShaderPass* getCurrentShaderrPass() { return mCurrentShaderrPass; }

    /// RenderTarget�ADepthBuffer �����Ƃ��ɌĂ΂��Btrue �̏ꍇ�A2�ׂ̂���ō��
    bool IsPowerOfTwoBufferTexture() { return true; }

    //bool isUseDrawThread() { return true; }

private:

    /// �f�o�C�X�̐��\�`�F�b�N
    LNRESULT _checkDeviceInformation();

    /// �f�o�C�X���Z�b�g�O����
    LNRESULT _onLostDevice();

    /// �f�o�C�X���Z�b�g�㏈��
    LNRESULT _onResetDevice();

private:

    typedef std::list< VertexBuffer* >          VertexBufferList;
    typedef std::list< IndexBuffer* >           IndexBufferList;
    typedef std::list< Texture* >               TextureList;
    typedef std::list< RenderTargetTexture* >   RenderTargetList;
    typedef std::list< DepthBuffer* >           DepthBufferList;

private:

    Base::LogFile*              mLogFile;
    Manager*                    mManager;
    System::Manager*            mSystemManager;
    File::Manager*              mFileManager;
    Renderer*                   mRenderer;
    ShaderManager*              mShaderManager;
    
    /*PlatformContext*            mPlatformContext;*/

    DefaultCanvas*              mDefaultCanvas;
    VertexBufferList            mVertexBufferList;
    IndexBufferList             mIndexBufferList;
    TextureList                 mTextureList;
    RenderTargetList            mRenderTargetList;
    DepthBufferList             mDepthBufferList;

    Base::TempBuffer            mTempBuffer;
    ShaderPass*                 mCurrentShaderrPass;

    bool                        mIsDeviceLost;

    friend class Renderer;
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