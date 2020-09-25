//=============================================================================
//�y Texture �z
//-----------------------------------------------------------------------------
///**
//  @file       Texture.h
//  @brief      Texture
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"
#include "GraphicsDevice.h"

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

//=============================================================================
// �� DX9TextureBase �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		DirectX9 �e�N�X�`���̊��
//*/
//=============================================================================
class DX9TextureBase
    : public Base::ReferenceObject
    , public ITexture
{
public:

    /// �R���X�g���N�^
    DX9TextureBase() {}

    /// �f�X�g���N�^
    virtual ~DX9TextureBase() {}

public:

    /// �t�@�C������쐬�����Ƃ��̃t�@�C����
    virtual const lnChar* getFilename() const { return NULL; }

    virtual void* getIDirect3DTexture9() { return getDxTexture(); }
    virtual void* getIDirect3DSurface9() { return getDxSurface(); }

public:

    /// IDirect3DTexture9 �|�C���^�̎擾
    virtual IDirect3DTexture9* getDxTexture() = 0;

	/// �e�N�X�`���T�[�t�F�C�X�ւ̃|�C���^�擾
	virtual IDirect3DSurface9* getDxSurface() = 0;

    /// �f�o�C�X���Z�b�g���O�̃R�[���o�b�N
    virtual LNRESULT onLostDevice() = 0;

	/// �f�o�C�X���Z�b�g����̃R�[���o�b�N
    virtual LNRESULT onResetDevice() = 0;
};

//=============================================================================
// �� Texture �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		���ʂ̃e�N�X�`���̃N���X
//*/
//=============================================================================
class Texture
    : public DX9TextureBase
    , public TextureCache::CachedObject
{
public:
    
    LN_CACHED_REFOBJ_METHODS;

	/// �R���X�g���N�^
	Texture( GraphicsDevice* device_ );

	/// �f�X�g���N�^
	virtual ~Texture();

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief		�e�N�X�`���쐬
    //
    //  @param[in]  width_   : �e�N�X�`���̕�
    //  @param[in]  height_  : �e�N�X�`���̍���
    //  @param[in]  level_   : �~�b�v�}�b�v���x�� ( 0 �őS�č쐬 )
    //  @param[in]  format_  : �T�[�t�F�C�X�t�H�[�}�b�g
    //*/
    //---------------------------------------------------------------------
	LNRESULT initialize( u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief		�e�N�X�`���쐬 ( �������ɓW�J���ꂽ�摜�f�[�^���� )
    //  
    //  @param[in]  data_      : �e�N�X�`���f�[�^���i�[����Ă���o�b�t�@�ւ̃A�h���X
    //  @param[in]  size_      : �e�N�X�`���f�[�^�̃T�C�Y (�o�C�g�P��)
    //  @param[in]  color_key_ : ����������F
    //  @param[in]  level_     : �~�b�v�}�b�v���x�� ( 0 �őS�č쐬 )
    //  @param[in]  format_    : �T�[�t�F�C�X�t�H�[�}�b�g         
    //*/
    //---------------------------------------------------------------------
	LNRESULT initialize( const void* data_, u32 size_, u32 color_key_ = 0x00000000, u32 levels_ = 1, LNSurfaceFormat format_ = LN_FMT_A8R8G8B8 );

    

	/// ��ނ̎擾
	virtual LNTextureType getType() const { return LN_TEXTURETYPE_NORMAL; }

	/// �T�C�Y�̎擾
    virtual const LVector2& getSize() const { return mSize; }

	/// ���ۂ̃T�C�Y�̎擾
    virtual const LVector2& getRealSize( bool is_reciprocal_ ) const { return ( is_reciprocal_ ) ? mRealSizeInv : mRealSize; }

    /// �N���A
	virtual void clear( const LColor& color_ );

    /// ���b�N
	virtual u32* lock();

    /// �A�����b�N
	virtual void unlock();

    /// drawText() �ł̕�����`��Ɏg�p����t�H���g�̐ݒ�
    virtual void setFont( IFont* font_ ) { LN_SAFE_RELEASE( mFont ); mFont = font_; LN_SAFE_ADDREF( mFont ); }

    /// drawText() �ł̕�����`��Ɏg�p����t�H���g�̎擾
    virtual IFont* getFont() const { return mFont; }

    /// ������̕`��
    virtual LNRESULT drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_, int len_ );

    /// �e�N�X�`���̃u���b�N�]�� (���݁A���ʂ̃e�N�X�`�� (RT�A�[�x�o�b�t�@�ł͂Ȃ�) �ɂ̂ݗL��)
    virtual LNRESULT blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ );

    /// �t�@�C������쐬�����Ƃ��̃t�@�C����
    virtual const lnChar* getFilename() const { return mFilename.c_str(); }


	/// IDirect3DTexture9 �|�C���^�̎擾
	virtual IDirect3DTexture9* getDxTexture() { return mDxTexture; }

	/// �e�N�X�`���T�[�t�F�C�X�ւ̃|�C���^�擾
	virtual IDirect3DSurface9* getDxSurface() { return mDxSurface; }

    /// �f�o�C�X���Z�b�g���O�̃R�[���o�b�N
    virtual LNRESULT onLostDevice() { return LN_OK; }

	/// �f�o�C�X���Z�b�g����̃R�[���o�b�N
	virtual LNRESULT onResetDevice() { return LN_OK; }

private:

    GraphicsDevice*     mGraphicsDevice;    ///< �Ǘ��N���X
	IDirect3DTexture9*	mDxTexture;		    ///< Direct3DTexture �{��
	IDirect3DSurface9*	mDxSurface;		    ///< �e�N�X�`���̃T�[�t�F�C�X

    LVector2            mSize;                   ///< �쐬���ɗv�������T�C�Y
    LVector2            mRealSize;               ///< ���ۂ̃e�N�X�`���̃T�C�Y
    LVector2            mRealSizeInv;            ///< ���ۂ̃e�N�X�`���T�C�Y�̋t��

    LRefTString         mFilename;
	IFont*              mFont;

    LSharingKey mSharingKey;
};

//=============================================================================
// �� RenderTargetTexture �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�����_�[�^�[�Q�b�g�e�N�X�`���̃N���X
//*/
//=============================================================================
class RenderTargetTexture
    : public DX9TextureBase
{
public:

	/// �R���X�g���N�^
	RenderTargetTexture( GraphicsDevice* device_ );

	/// �f�X�g���N�^
	virtual ~RenderTargetTexture();

    LN_REFOBJ_METHODS;

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief		�����_�[�^�[�Q�b�g�e�N�X�`���̍쐬
    //
    //  @param[in]  width_   : �e�N�X�`���̕�
    //  @param[in]  height_  : �e�N�X�`���̍���
    //  @param[in]  level_   : �~�b�v�}�b�v���x�� ( 0 �őS�č쐬 )
    //  @param[in]  format_  : �T�[�t�F�C�X�t�H�[�}�b�g
    //*/
    //---------------------------------------------------------------------
	LNRESULT initialize( u32 width_, u32 height_, u32 level_, LNSurfaceFormat format_ );

	/// ��ނ̎擾
	virtual LNTextureType getType() const { return LN_TEXTURETYPE_RENDERTARGET; }

	/// �T�C�Y�̎擾
    virtual const LVector2& getSize() const { return mSize; }

	/// ���ۂ̃T�C�Y�̎擾
	virtual const LVector2& getRealSize( bool is_reciprocal_ ) const { return ( is_reciprocal_ ) ? mRealSizeInv : mRealSize; }

    /// �N���A ( �����_�����O�^�[�Q�b�g�ł̓T�|�[�g���Ă܂��� )
    void clear( const LColor& color_ ) { printf( "�����_�[�^�[�Q�b�g�e�N�X�`���� clear() �̓T�|�[�g���Ă��܂���B\n" ); }

    /// ���b�N
	u32* lock();

    /// �A�����b�N
	void unlock();

    /// drawText() �ł̕�����`��Ɏg�p����t�H���g�̐ݒ�
    virtual void setFont( IFont* font_ ) { }

    /// drawText() �ł̕�����`��Ɏg�p����t�H���g�̎擾
    virtual IFont* getFont() const { return NULL; }

    /// ������̕`��
    virtual LNRESULT drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_, int len_ ) { return LN_ERR_INVALIDCALL; }

    /// �e�N�X�`���̃u���b�N�]��
    virtual LNRESULT blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ ) { return LN_ERR_INVALIDCALL; }

public:

	/// IDirect3DTexture9 �|�C���^�̎擾
	virtual IDirect3DTexture9* getDxTexture() { return mDxTexture; }

	/// �e�N�X�`���T�[�t�F�C�X�ւ̃|�C���^�擾
	virtual IDirect3DSurface9* getDxSurface() { return mDxSurface; }

    /// �f�o�C�X���Z�b�g���O�̃R�[���o�b�N
	virtual LNRESULT onLostDevice();

	/// �f�o�C�X���Z�b�g����̃R�[���o�b�N
	virtual LNRESULT onResetDevice();

private:

    /// ���ۂɍ쐬����
    LNRESULT _create();

    /// �������
    void _dispose();

private:

    GraphicsDevice*     mGraphicsDevice;    ///< �Ǘ��N���X

	IDirect3DTexture9*	mDxTexture;			///< Direct3DTexture �{��
	IDirect3DSurface9*	mDxSurface;			///< �e�N�X�`���̃T�[�t�F�C�X
	IDirect3DSurface9*	mDxSystemSurface;	///< �����_�����O�^�[�Q�b�g�����b�N���鎞�Ɏg���A�V�X�e���������ɔz�u���ꂽ�T�[�t�F�C�X

    LVector2            mSize;              ///< �쐬���ɗv�������T�C�Y
    LVector2            mRealSize;          ///< ���ۂ̃e�N�X�`���̃T�C�Y
    LVector2            mRealSizeInv;            ///< ���ۂ̃e�N�X�`���T�C�Y�̋t��
    u32                 mMipLevel;			///< �~�b�v�}�b�v���x��
	LNSurfaceFormat	    mFormat;			///< �T�[�t�F�C�X�t�H�[�}�b�g

    //friend class ResourceManager;
};

//=============================================================================
// �� DepthBuffer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�[�x�o�b�t�@�̃N���X
//*/
//=============================================================================
class DepthBuffer
    : public DX9TextureBase
{
public:

	/// �R���X�g���N�^
	DepthBuffer( GraphicsDevice* device_ );

	/// �f�X�g���N�^
	virtual ~DepthBuffer();

    LN_REFOBJ_METHODS;

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief		�[�x�o�b�t�@�̍쐬
    //
    //  @param[in]  width_   : �e�N�X�`���̕�
    //  @param[in]  height_  : �e�N�X�`���̍���
    //  @param[in]  format_  : �T�[�t�F�C�X�t�H�[�}�b�g
    //*/
    //---------------------------------------------------------------------
	LNRESULT initialize( u32 width_, u32 height_, LNSurfaceFormat format_ );

	/// ��ނ̎擾
	virtual LNTextureType getType() const { return LN_TEXTURETYPE_DEPTH; }

	/// �T�C�Y�̎擾
    virtual const LVector2& getSize() const { return mSize; }

	/// ���ۂ̃T�C�Y�̎擾
	virtual const LVector2& getRealSize( bool is_reciprocal_ ) const { return ( is_reciprocal_ ) ? mRealSizeInv : mRealSize; }

    /// �N���A ( �[�x�o�b�t�@�ł̓T�|�[�g���Ă܂��� )
    void clear( const LColor& color_ ) { printf( "�[�x�o�b�t�@�� clear() �̓T�|�[�g���Ă��܂���B\n" ); }

    /// ���b�N
    u32* lock() { return NULL; }

    /// �A�����b�N
    void unlock() { }

    /// drawText() �ł̕�����`��Ɏg�p����t�H���g�̐ݒ�
    virtual void setFont( IFont* font_ ) { }

    /// drawText() �ł̕�����`��Ɏg�p����t�H���g�̎擾
    virtual IFont* getFont() const { return NULL; }

    /// ������̕`��
    virtual LNRESULT drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_, int len_ ) { return LN_ERR_INVALIDCALL; }

    /// �e�N�X�`���̃u���b�N�]��
    virtual LNRESULT blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ ) { return LN_ERR_INVALIDCALL; }


	/// IDirect3DTexture9 �|�C���^�̎擾
	virtual IDirect3DTexture9* getDxTexture() { return NULL; }

	/// �e�N�X�`���T�[�t�F�C�X�ւ̃|�C���^�擾
	virtual IDirect3DSurface9* getDxSurface() { return mDxSurface; }

    /// �f�o�C�X���Z�b�g���O�̃R�[���o�b�N
	virtual LNRESULT onLostDevice();

	/// �f�o�C�X���Z�b�g����̃R�[���o�b�N
	virtual LNRESULT onResetDevice();

private:

    /// ���ۂɍ쐬����
    LNRESULT _create();

    /// �������
    void _dispose();

private:

    GraphicsDevice*     mGraphicsDevice;    ///< �Ǘ��N���X
	IDirect3DSurface9*	mDxSurface;			///< �[�x�o�b�t�@�T�[�t�F�C�X
    LVector2            mSize;              ///< �쐬���ɗv�������T�C�Y
    LVector2            mRealSize;          ///< ���ۂ̃e�N�X�`���̃T�C�Y
    LVector2            mRealSizeInv;       ///< ���ۂ̃e�N�X�`���T�C�Y�̋t��
	LNSurfaceFormat	    mFormat;			///< �T�[�t�F�C�X�t�H�[�}�b�g

    friend class ResourceManager;
};

//=============================================================================
// �� SystemCreatedTexture �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		DirectX ���Ŏ����I�ɍ쐬���ꂽ�e�N�X�`���������N���X
//
//  @note
//              �Ƃ肠�����f�t�H���g�̃X���b�v�`�F�C���̏����Ŏg���Ă�B
//*/
//=============================================================================
class SystemCreatedTexture
    : public DX9TextureBase
{
public:

	/// �R���X�g���N�^
	SystemCreatedTexture();

	/// �f�X�g���N�^
	virtual ~SystemCreatedTexture();

    LN_REFOBJ_METHODS;

public:

    /// �e�N�X�`���A�T�[�t�F�C�X�̃Z�b�g (texture_ �� NULL �ł�OK�Bsurface_ �͕K���B�f�o�C�X���Z�b�g���͗��� NULL �ŎQ�ƃJ�E���g���f�N�������g)
	LNRESULT setTextureAndSurface( IDirect3DTexture9* texture_, IDirect3DSurface9* surface_ );

	/// ��ނ̎擾
	virtual LNTextureType getType() const { return LN_TEXTURETYPE_RENDERTARGET; }

    /// �T�C�Y�̎擾
    virtual const LVector2& getSize() const { return mSize; }

	/// ���ۂ̃T�C�Y�̎擾
	virtual const LVector2& getRealSize( bool is_reciprocal_ ) const { return ( is_reciprocal_ ) ? mRealSizeInv : mRealSize; }

    /// �N���A
    void clear( const LColor& color_ ) { printf( "�����_�[�^�[�Q�b�g�e�N�X�`���� clear() �̓T�|�[�g���Ă��܂���B\n" ); }

    /// drawText() �ł̕�����`��Ɏg�p����t�H���g�̐ݒ�
    virtual void setFont( IFont* font_ ) { }

    /// drawText() �ł̕�����`��Ɏg�p����t�H���g�̎擾
    virtual IFont* getFont() const { return NULL; }

    /// ������̕`��
    virtual LNRESULT drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_, int len_ ) { return LN_ERR_INVALIDCALL; }

    /// �e�N�X�`���̃u���b�N�]��
    virtual LNRESULT blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ ) { return LN_ERR_INVALIDCALL; }

    /// ���b�N
    virtual u32* lock() { return NULL; }

    /// �A�����b�N
    virtual void unlock() {}

public:

    /// IDirect3DTexture9 �|�C���^�̎擾
    virtual IDirect3DTexture9* getDxTexture() { return mDxTexture; }

	/// �e�N�X�`���T�[�t�F�C�X�ւ̃|�C���^�擾
	virtual IDirect3DSurface9* getDxSurface() { return mDxSurface; }

    /// �f�o�C�X���Z�b�g���O�̃R�[���o�b�N
    virtual LNRESULT onLostDevice() { return LN_OK; }

	/// �f�o�C�X���Z�b�g����̃R�[���o�b�N
    virtual LNRESULT onResetDevice() { return LN_OK; }

private:

    IDirect3DTexture9*  mDxTexture;
	IDirect3DSurface9*	mDxSurface;			///< �e�N�X�`���̃T�[�t�F�C�X

    LVector2            mSize;              ///< �쐬���ɗv�������T�C�Y
    LVector2            mRealSize;          ///< ���ۂ̃e�N�X�`���̃T�C�Y
    LVector2            mRealSizeInv;       ///< ���ۂ̃e�N�X�`���T�C�Y�̋t��
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