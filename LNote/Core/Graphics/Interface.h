//=============================================================================
//�y Graphics �z
//-----------------------------------------------------------------------------
///**
//  @file       Graphics.h
//  @brief      Graphics
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Base/SharingKey.h"
#include "../Geometry/Vector2.h"
#include "../Geometry/Vector3.h"
#include "../Geometry/Vector4.h"
#include "../Geometry/Matrix.h"
#include "../Geometry/DrawingShapes.h"
#include "../System/Interface.h"
#include "../File/Interface.h"
#include "Common/Common.h"
#include "Common/RenderState.h"
#include "Common/VertexTypes.h"

#if defined(LNOTE_DIRECTX)
    struct IDirect3DDevice9;
#endif

#if defined(LNOTE_OPENGL)
    typedef unsigned int GLuint;
#endif

#define LN_DEFAULT_FONT_NAME    _T( "�l�r �o�S�V�b�N" )

/// �}���`�����_�����O�^�[�Q�b�g�̍ő吔
static const int LN_MAX_MULTI_RENDERTARGETS = 4;

/// �T���v���X�e�[�W�̍ő吔
static const int LN_MAX_SAMPLER_STAGE_INDEX = 8;

//-------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------

//#define LNEV_CANVAS_SIZE_CHANGED    1001    ///< �L�����o�X�T�C�Y�ύX�C�x���g

//-------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
class IGraphicsDevice;
class IResourceManager;
class IRenderer;
class ICanvas;
class IVertexBuffer;
class IIndexBuffer;
class ITexture;
class IFont;
class IShader;
class IShaderVariable;
class IShaderTechnique;
class IShaderPass;

class GeometryRenderer;
class SpriteRenderer;
class Image;

//=============================================================================
// �� IGraphicsDevice �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �O���t�B�b�N�f�o�C�X�̃C���^�[�t�F�C�X
//
//  @par
//              �f�o�C�X���Z�b�g�͕K��
//*/
//=============================================================================
class IGraphicsDevice
    : public Base::Interface
{
public:

    /// �g�p���� GraphicsAPI �̎擾
    virtual LNGraphicsAPI getGraphicsAPI() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �`��N���X�̎擾
    //*/
    //---------------------------------------------------------------------
    virtual IRenderer* getRenderer() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �f�t�H���g�̃o�b�N�o�b�t�@�������L�����o�X�̎擾
    //*/
    //---------------------------------------------------------------------
    virtual ICanvas* getDefaultCanvas() = 0;

    /// ���_�o�b�t�@�̍쐬
    virtual LNRESULT createVertexBuffer( IVertexBuffer** obj_, LNVertexElemenst* elements_, u32 vertex_num_, const void* data_, bool is_dynamic_ = false ) = 0;

    /// �C���f�b�N�X�o�b�t�@�̍쐬
    virtual LNRESULT createIndexBuffer( IIndexBuffer** obj_, u32 index_num_, const u16* data_, bool is_dynamic_ = false, bool is_16bit_ = true ) = 0;

    // �e�N�X�`���̍쐬
    virtual LNRESULT createTexture( ITexture** obj_, u32 width_, u32 height_, u32 levels_, LNSurfaceFormat format_ ) = 0;

    ///// �e�N�X�`���̍쐬 (���̓X�g���[������)
    //virtual LNRESULT createTexture( ITexture** obj_, File::IInStream* stream_, u32 color_key_ = 0, u32 levels_ = 1, LNSurfaceFormat format_ = LN_FMT_UNKNOWN, LSharingKey key_ = lnNullKey ) = 0;

    /// �e�N�X�`���̍쐬 (�t�@�C������)
    virtual void createTexture( ITexture** obj_, const lnChar* filename_, u32 color_key_ = 0, u32 levels_ = 1, LNSurfaceFormat format_ = LN_FMT_UNKNOWN, LSharingKey key_ = lnNullKey ) = 0;

    /// �����_�[�^�[�Q�b�g�e�N�X�`���̍쐬
    virtual LNRESULT createRenderTarget( ITexture** obj_, u32 width_, u32 height_, u32 levels_ = 1, LNSurfaceFormat format_ = LN_FMT_A8R8G8B8 ) = 0;

    /// �[�x�o�b�t�@�̍쐬
    virtual LNRESULT createDepthBuffer( ITexture** obj_, u32 width_, u32 height_, LNSurfaceFormat format_ ) = 0;

    ///// �V�F�[�_�̍쐬 (���ߍ��݃��\�[�X����쐬����Ƃ��̓X�g���[��������������̕��������葁���̂ňꏏ�Ɍ��J)
    //virtual LNRESULT createShader( IShader** obj_, const void* data_, u32 size_, LSharingKey key_ = lnNullKey ) = 0;

    ///// �V�F�[�_�̍쐬 (���̓X�g���[������)
    //virtual LNRESULT createShader( IShader** obj_, File::IInStream* stream_, LSharingKey key_ = lnNullKey ) = 0;

    /// �V�F�[�_�̍쐬 (�t�@�C������)
    virtual void createShader( IShader** obj_, const lnChar* filename_, LSharingKey key_ = lnNullKey ) = 0;

    /// �L���b�V���̃N���A
    virtual void clearCache( bool texture_, bool shader_ ) = 0;

    /// �f�o�C�X���Z�b�g�̕K�v������ꍇ�� true
    virtual bool isDeviceLost() = 0;

    /// �f�o�C�X���Z�b�g�O����
    virtual void onLostDevice() = 0;

    /// �f�o�C�X���Z�b�g�㏈��
    virtual void onResetDevice() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �f�o�C�X���Z�b�g
    //
    //  @note
    //              ��{�I�ɓ����p�B
    //              �f�o�C�X���Z�b�g�͕K��
    //              onLostDevice() �� resetDevice() �� onResetDevice()
    //              �̏��ōs�����ƁB
    //              �֐��𕪂��Ă���̂́A���C�u�����O���Ń��Z�b�g���s���ꍇ�̑Ή��B
    //              ���̏ꍇ�� onLostDevice()�AonResetDevice() ��K�؂ȃ^�C�~���O�ŌĂԁB
    //              (��{�I�Ƀ��Z�b�g������ Graphics �ȊO�̃��W���[���ł��n���h�����O
    //              ���Ȃ��ƂȂ�Ȃ��̂ŁAFramework �� isDeviceLost() ����`�F�b�N���s���K�v������)
    //              ���C�u�����O���̃f�o�C�X���g�p���Ă���ꍇ�͏�� false ��Ԃ��B
    //
    //              �f�o�C�X���Z�b�g�� Canvas::present() ��
    //*/
    //---------------------------------------------------------------------
    virtual void resetDevice() = 0;

#if defined(LNOTE_DIRECTX)

    /// IDirect3DDevice9 �ւ̃|�C���^�̎擾
    virtual IDirect3DDevice9* getDX9Device() = 0;

#endif

protected:

    virtual ~IGraphicsDevice() {};
};

//=============================================================================
// �� IRenderer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief     �`��Ƃ��̃X�e�[�g�Ǘ����s���C���^�[�t�F�C�X
//*/
//=============================================================================
class IRenderer
    : public Base::Interface
{
public:

    /// �����_�����O�X�e�[�g�̐ݒ�
    virtual LNRESULT setRenderState( const LRenderState& state_ ) = 0;

    /// �����_�����O�^�[�Q�b�g�̎擾 (�Q�ƃJ�E���g�ɕω��̓i�V)
    virtual ITexture* getRenderTarget( u32 index_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �����_�����O�^�[�Q�b�g�̐ݒ�
    //
    //  @par
    //              index_ 0 �ɐݒ肵���ꍇ�A�ݒ肳�ꂽ�e�N�X�`���T�C�Y��
    //              �r���[�|�[�g�����Z�b�g���܂��B<br>
    //              index_ �� 0 �̏ꍇ�� texture_ �� NULL ���w�肷��ƁA
    //              �f�t�H���g�̃o�b�N�o�b�t�@�������_�����O�^�[�Q�b�g�Ƃ��Đݒ肵�܂��B<br>
    //              
    //  @attention
    //              0 �Ԃ� NULL ���ݒ肳�ꂽ�ꍇ�A�[�x�o�b�t�@���f�t�H���g�̂��̂��ݒ肳��܂��B
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT setRenderTarget( u32 index_, ITexture* texture_, bool reset_ = false ) = 0;

    /// �[�x�o�b�t�@�̎擾 (�Q�ƃJ�E���g�ɕω��̓i�V)
    virtual ITexture* getDepthBuffer() = 0;

    /// �[�x�o�b�t�@�̐ݒ�
    virtual LNRESULT setDepthBuffer( ITexture* texture_ ) = 0;

    /// �r���[�|�[�g��`�̎擾
    virtual const Geometry::Rect& getViewport() const = 0;

	/// �r���[�|�[�g��`�̐ݒ�
	virtual LNRESULT setViewport( const Geometry::Rect& viewport_rect_ ) = 0;

    /// �V�U�[��`�̎擾
    virtual const Geometry::Rect& getScissor() const = 0;

    /// �V�U�[��`�̐ݒ� (�g�p���Ȃ��ꍇ�A�v�f�����ׂ� 0 ���A�r���[�|�[�g�Ɠ����� rect_ ��n��)
	virtual LNRESULT setScissor( const Geometry::Rect& rect_ ) = 0;

    /// ���_�o�b�t�@�̐ݒ�
	virtual LNRESULT setVertexBuffer( IVertexBuffer* vertex_buffer_, bool reset_ = false ) = 0;

    /// �C���f�b�N�X�o�b�t�@�̐ݒ�
	virtual LNRESULT setIndexBuffer( IIndexBuffer* index_buffer_, bool reset_ = false ) = 0;

    /// �ݒ肳��Ă���J���[�o�b�t�@�A�[�x�o�b�t�@���N���A����
	virtual LNRESULT clear( bool target_, bool depth_, u32 color_, lnfloat z_ = 1.0f ) = 0;

    /// �v���~�e�B�u�`��
	virtual LNRESULT drawPrimitive( LNPrimitiveType primitive_, u32 start_vertex_, u32 primitive_count_ ) = 0;

    /// �v���~�e�B�u�`�� (�C���f�b�N�X�t���B���_�A�C���f�b�N�X�̗����̃o�b�t�@��dynamic�Astatic ����v���Ă���K�v������)
	virtual LNRESULT drawPrimitiveIndexed( LNPrimitiveType primitive_, u32 start_index_, u32 primitive_count_ ) = 0;

    /// �V�[���J�n
	virtual LNRESULT beginScene() = 0;

	/// �V�[���I��
	virtual LNRESULT endScene() = 0;
   
protected:

    virtual ~IRenderer() {}
};

//=============================================================================
// �� ICanvas �N���X
//-----------------------------------------------------------------------------
///**
//  @brief     �o�b�N�o�b�t�@�Ɖ��z�X�N���[���̃C���^�[�t�F�C�X
//*/
//=============================================================================
class ICanvas
    : public Base::Interface
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L�����o�X�̃T�C�Y���擾����
    //*/
    //---------------------------------------------------------------------
    virtual const Geometry::Size& getSize() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L�����o�X�T�C�Y��ύX���� (setWindowSize() �ɂ������ɐݒ肳���)
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT resize( u32 width_, u32 height_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���z�o�b�N�o�b�t�@�̎擾
    //
    //  @note
    //              WPF �A�v�����Ƃ̘A�g�̂��߂ɗp�ӂ������̂����ǁA
    //              DirectX9 �œ��삪�������̂Ō�Ń`�F�b�N���Ă����B
    //              [12/12/02] WPF ���̓t�H�[���z�X�g�ɂ�������R���͕K�v�Ȃ��B
    //*/
    //---------------------------------------------------------------------
    virtual ITexture* getBackbuffer() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���z�o�b�N�o�b�t�@�֕`��ł����Ԃɂ���
    //
    //  @par
    //              �Q�[����ʂ�`��ł����Ԃɂ��܂��B
    //
    //              �����I�ɂ́AgetBackbuffer() �Ŏ擾�ł��郌���_�����O�^�[�Q�b�g��
    //              �`���Ƃ��Đݒ肵�܂��B
    //              IRenderer::setRenderTarget( 0, NULL ); �Őݒ肳���
    //              �����_�����O�^�[�Q�b�g�Ƃ͈قȂ�܂��B
    //              (�����A����DirectX9��Present()�œ]������w�肵�Ă���̂œ���)
    //
    //              OpenGL �ł̓o�b�N�o�b�t�@��ݒ��A���z�X�N���[���̈��
    //              �r���[�|�[�g��ݒ肵�܂��B
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT activate() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �o�b�N�o�b�t�@�̓��e���E�B���h�E�֓]������
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT present() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �`���̃E�B���h�E�T�C�Y��ݒ肷��
	//
	//	@note
	//				���^�[�{�b�N�X�T�C�Y�v�Z�Ɏg�p����l�B
	//				�o�b�N�o�b�t�@�T�C�Y�͕ύX���Ȃ��̂Œ��ӁB
    //*/
    //---------------------------------------------------------------------
    virtual void setWindowSize( const Geometry::Size& size_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �L�����o�X���E�B���h�E�֓]������ۂ̍��W�ϊ��s��̎擾
    //
    //  @note
    //              GUI ���Ń}�E�X���W�̈ʒu�����킹�邽�߂ɗp�ӂ������́B
    //*/
    //---------------------------------------------------------------------
    virtual const LMatrix& getScreenTransform() const = 0;

#if defined(LNOTE_WINDOWS)
	/// present() �̓]����E�B���h�E�̃E�B���h�E�n���h���̐ݒ�
	virtual void setTargetWindowHandle(HWND hwnd) = 0;
#endif

protected:

    virtual ~ICanvas() {}
};

//=============================================================================
// �� IVertexBuffer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief     ���_�o�b�t�@�̃C���^�[�t�F�C�X
//*/
//=============================================================================
class IVertexBuffer
    : public Base::Interface
{
public:

    /// ���_���̎擾
	virtual u32 getVertexNum() const = 0;

	/// ���_�ЂƂ��̃T�C�Y�̎擾
	virtual u32 getVertexStride() const = 0;

	/// ���I�Ȓ��_���𔻒肷��	
	virtual bool isDynamic() const = 0;

	/// ���b�N
	virtual void* lock() = 0;

	/// �A�����b�N
	virtual void unlock() = 0;

protected:

    virtual ~IVertexBuffer() {};
};

//=============================================================================
// �� IIndexBuffer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief     �C���f�b�N�X�o�b�t�@�̃C���^�[�t�F�C�X
//*/
//=============================================================================
class IIndexBuffer
    : public Base::Interface
{
public:

    /// �C���f�b�N�X���̎擾
    virtual u32 getIndexNum() const = 0;

    /// �C���f�b�N�X�ЂƂ��̃T�C�Y�̎擾 (2 �܂��� 4)
	virtual lnU32 getIndexStride() const = 0;

	/// ���I�ȃC���f�b�N�X�o�b�t�@���𔻒肷��	
	virtual bool isDynamic() const = 0;

	/// ���b�N
	virtual void* lock() = 0;

	/// �A�����b�N
	virtual void unlock() = 0;

protected:

    virtual ~IIndexBuffer() {};
};

//=============================================================================
// �� ITexture �N���X
//-----------------------------------------------------------------------------
///**
//  @brief     �e�N�X�`���̃C���^�[�t�F�C�X
//*/
//=============================================================================
class ITexture
    : public Base::Interface
{
public:

	/// �e�N�X�`���̎�ނ̎擾
	virtual LNTextureType getType() const = 0;

	/// �T�C�Y�̎擾
    virtual const LVector2& getSize() const = 0;

	/// ���ۂ̃T�C�Y�̎擾
	virtual const LVector2& getRealSize( bool is_reciprocal_ = false ) const = 0;

	/// �N���A
	virtual void clear( const LColor& color_ = LColor::TRANSPARENCY ) = 0;

	/// ���b�N
	virtual u32* lock() = 0;

	/// �A�����b�N
	virtual void unlock() = 0;

    /// drawText() �ł̕�����`��Ɏg�p����t�H���g�̐ݒ� (�����_�����O�^�[�Q�b�g�A�[�x�o�b�t�@�̏ꍇ�͖���)
    virtual void setFont( IFont* font_ ) = 0;

    /// drawText() �ł̕�����`��Ɏg�p����t�H���g�̎擾
    virtual IFont* getFont() const = 0;

    /// ������̕`��
    virtual LNRESULT drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_ = LN_TEXTALIGN_LEFT, int len_ = -1 ) = 0;

    /// �e�N�X�`���̃u���b�N�]�� (���݁A���ʂ̃e�N�X�`�� (RT�A�[�x�o�b�t�@�ł͂Ȃ�) �ɂ̂ݗL��)
    virtual LNRESULT blt( int x_, int y_, ITexture* src_texture_, const Geometry::Rect& src_rect_ ) = 0;

    /// �t�@�C������쐬�����Ƃ��̃t�@�C����
    virtual const lnChar* getFilename() const = 0;

public:

#if defined(LNOTE_DIRECTX)
    virtual void* getIDirect3DTexture9() = 0;
    virtual void* getIDirect3DSurface9() = 0;
#endif

protected:

    virtual ~ITexture() {}
};

//=============================================================================
// �� IFont �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �t�H���g�̃C���^�[�t�F�C�X
//*/
//=============================================================================
class IFont
    : public Base::Interface
{
public:

    /// �t�H���g���̐ݒ�
	virtual LNRESULT setName( const lnChar* fontname_ ) = 0;

    /// �t�H���g���̎擾
	virtual const lnChar* getName() const = 0;

    /// �t�H���g�T�C�Y�̐ݒ�
	virtual void setSize( u32 size_ ) = 0;

    /// �t�H���g�T�C�Y�̎擾
	virtual u32 getSize() const = 0;

	/// �t�H���g�J���[�̐ݒ�
	virtual void setColor( const LColor& color_ ) = 0;

    /// �t�H���g�J���[�̎擾
	virtual const LColor& getColor() const = 0;

    /// �G�b�W�J���[�̐ݒ�
	virtual void setEdgeColor( const LColor& color_ ) = 0;

    /// �G�b�W�J���[�̎擾
	virtual const LColor& getEdgeColor() const = 0;

    /// �G�b�W�̕��̐ݒ� (0 �ŃG�b�W����)
	virtual void setEdgeSize( u32 size_ ) = 0;

    /// �G�b�W�̕��̎擾
	virtual u32 getEdgeSize() const = 0;

	/// �������̐ݒ�
	virtual void setBold( bool flag_ ) = 0;

    /// �������̔���
	virtual bool isBold() const = 0;

	/// �C�^���b�N�̂̐ݒ�
	virtual void setItalic( bool flag_ ) = 0;

    /// �C�^���b�N�̂̔���
	virtual bool isItalic() const = 0;

    /// �A���`�G�C���A�X�̗L���ݒ�
	virtual void setAntiAlias( bool flag_ ) = 0;

    /// �A���`�G�C���A�X�̗L������
	virtual bool isAntiAlias() const = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �������`�悵���Ƃ��̃T�C�Y (�s�N�Z���P��) �̎擾
    //
    //  @param[in]  text_ : �`�F�b�N���镶����
    //  @param[in]  len_  : ������ (-1 ���w�肷��ƏI�[�܂Ń`�F�b�N����)
    //  @param[out] rect_ : ���ʂ��i�[����ϐ��ւ̃|�C���^
    //
    //  @par
    //              ���ʂ̕��ƍ����� rect_ �� width�Aheiht �Ɋi�[����܂��B
    //              ���݁Arect_ �� x�Ay �v�f�ɂ͏�� 0 ���i�[����܂��B
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT getTextSize( const lnChar* text_, int len_, Geometry::Rect* rect_ ) = 0;


    //---------------------------------------------------------------------
    ///**
    //  @brief      �t�H���g�����̃r�b�g�}�b�v���̎擾�J�n
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT beginGetGlyphOutline() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �t�H���g�����̃r�b�g�}�b�v���̎擾�I��
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT endGetGlyphOutline() = 0;

  
    //---------------------------------------------------------------------
    ///**
    //  @brief      �t�H���g�����̃r�b�g�}�b�v�����擾���� (char)
    //
    //  @param[in]  code_ : �f�[�^���擾���镶���ւ̃|�C���^
    //  @param[out] data_ : �O���t�f�[�^�\���̂̃|�C���^���i�[����ϐ��ւ̃A�h���X
    //
    //  @retval     LN_ERR_INVALIDCALL : beginGetGlyphOutline() ���Ă΂�Ă��Ȃ�
    //
    //  @par
    //              code_ �Ƀ}���`�o�C�g������n���ꍇ�A�ȉ��̎��ŕϊ����Ă���
    //              �n���Ă��������B<br>
    //              code = (u8)c[ 0 ] << 8 | (u8)c[ 1 ];<br>
    //              <br>
    //              ���̊֐��� beginGetGlyphOutline() �` endGetGlyphOutline() �̊Ԃ�
    //              �Ăяo���K�v������܂��B
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT getGlyphOutline( const char* code_, LNGlyphOutlineData** data_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �t�H���g�����̃r�b�g�}�b�v�����擾���� (wchar_t)
    //
    //  @param[in]  code_ : �f�[�^���擾���镶���ւ̃|�C���^
    //  @param[out] data_ : �O���t�f�[�^�\���̂̃|�C���^���i�[����ϐ��ւ̃A�h���X
    //*/
    //---------------------------------------------------------------------
    virtual LNRESULT getGlyphOutline( const wchar_t* code_, LNGlyphOutlineData** data_ ) = 0;
  
    /// �t�H���g�̃R�s�[���쐬����
    virtual LNRESULT deepCopy( IFont** font_ ) = 0;

protected:

    virtual ~IFont() {}
};

//=============================================================================
// �� IShader �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �V�F�[�_�v���O�����̃C���^�[�t�F�C�X
//*/
//=============================================================================
class IShader
    : public Base::Interface
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���O�ɂ���ĕϐ����擾����
    //
    //  @param[in]  name_ : �ϐ��̖��O
    //
    //  @retval     NULL : ������Ȃ�����
    //*/
    //---------------------------------------------------------------------
	virtual IShaderVariable* getVariableByName( const char* name_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �C���f�b�N�X�ɂ���ĕϐ����擾����
    //
    //  @param[in]  name_ : �e�N�j�b�N�̖��O
    //
    //  @retval     NULL : ������Ȃ�����
    //*/
    //---------------------------------------------------------------------
	virtual IShaderVariable* getVariableByIndex( u32 index_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief     �Z�}���e�B�N�X�ɂ���ĕϐ����擾����
    //
    //  @param[in]  name_ : �ϐ��̖��O
    //
    //  @retval     NULL : ������Ȃ�����
    //*/
    //---------------------------------------------------------------------
	//virtual IShaderVariable* getVariableBySemantic( const char* name_ ) = 0;
    
    //---------------------------------------------------------------------
    ///**
    //  @brief      ���O�ɂ���ăe�N�j�b�N���擾����
    //
    //  @param[in]  name_ : �e�N�j�b�N�̖��O
    //
    //  @retval     NULL : ������Ȃ�����
    //
    //  @attention
    //              GLSL �ł̓e�N�j�b�N�͂ЂƂ����ŏ�ɓ������̂��Ԃ�܂��B
    //*/
    //---------------------------------------------------------------------
	virtual IShaderTechnique* getTechniqueByName( const char* name_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �C���f�b�N�X�ɂ���ăe�N�j�b�N���擾����
    //
    //  @param[in]  name_ : �e�N�j�b�N�̖��O
    //
    //  @retval     NULL : ������Ȃ�����
    //
    //  @attention
    //              GLSL �ł̓e�N�j�b�N�͂ЂƂ����ŏ�ɓ������̂��Ԃ�܂��B
    //*/
    //---------------------------------------------------------------------
	virtual IShaderTechnique* getTechniqueByIndex( lnU32 index_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �V�F�[�_�̏����擾����
    //*/
    //---------------------------------------------------------------------
	virtual const LNShaderDesc& getDesc() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���p�C�����ʂ��擾����
    //*/
    //---------------------------------------------------------------------
	virtual LNShaderCompileResult getCompileResult() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���p�C���G���[�E�x���̕������Ԃ�
    //*/
    //---------------------------------------------------------------------
	virtual const lnChar* getCompileErrors() = 0;

protected:

    virtual ~IShader() {}
};



//=============================================================================
// �� IShaderVariable �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �V�F�[�_�v���O�������̕ϐ��̃C���^�[�t�F�C�X
//
//  @attention
//              �s��z��̐ݒ�Ǝ擾�́A�V�F�[�_���̌^�� float4x4 �ȊO��
//              �傫���̏ꍇ(float4x3 float3x3 ��)�A��Ɨ̈悪�K�v�ɂȂ�܂��B
//              ���̗̈�m�ۂ͓����ŏ�������܂����A�����܂ŕ⏕�I�Ȃ��̂Ȃ̂�
//              ���܂�����͗ǂ�����܂���B<br>
//              (��̓I�ɂ́A���傫���̈悪�K�v�ɂȂ����Ƃ��ɍĊm�ۂ��܂��B
//              �ȍ~�̓V�F�[�_���̂���������܂ŁA���̗̈悪�c�����܂܂ɂȂ�܂��B)<br>
//              4x4 �ȊO�̑傫���̍s��^�́A�X�L�j���O�p�̎p���s�񂪑�ʂɕK�v�Ȏ��ȂǁA
//              �ǂ����Ă��K�v�Ȏ������g���悤�ɂ��Ă��������B<br>
//              <br>
//              �܂��A�A�m�e�[�V������ IShaderVariable �Ƃ��Ď擾���܂����A
//              �A�m�e�[�V�����ւ̒l�̐ݒ�A�l�X�g���ꂽ�A�m�e�[�V�����̎擾�͂ł��܂���B
//              <br>
//              GLSL �̃A�m�e�[�V�����T�|�[�g�� bool int float vec4 string �ł��B
//*/
//=============================================================================
class IShaderVariable
    : public Base::Interface
{
public:

    /// �u�[���l�̎擾
	virtual LNRESULT getBool( bool* b_ ) = 0;

    /// �u�[���l�̐ݒ�
	virtual LNRESULT setBool( bool b_ ) = 0;

    /// �����l�̎擾
	virtual LNRESULT getInt( int* value_ ) = 0;

    /// �����l�̐ݒ�
	virtual LNRESULT setInt( int value_ ) = 0;

    /// �����l�̎擾
    virtual LNRESULT getFloat( lnfloat* value_ ) = 0;

    /// �����l�̐ݒ�
    virtual LNRESULT setFloat( lnfloat value_ ) = 0;

    /// �x�N�g���̎擾
	virtual LNRESULT getVector( LVector4* vec_ ) = 0;

    /// �x�N�g���̐ݒ�
	virtual LNRESULT setVector( const LVector4& vec_ ) = 0;

    /// �x�N�g���z��̎擾
	virtual LNRESULT getVectorArray( LVector4* vec_, lnU32 count_ ) = 0;
    
    /// �x�N�g���z��̐ݒ�
	virtual LNRESULT setVectorArray( const LVector4* vec_, lnU32 count_ ) = 0;

    /// �s��̎擾
    virtual LNRESULT getMatrix( LMatrix* matrix_ ) = 0;

    /// �s��̐ݒ�
    virtual LNRESULT setMatrix( const LMatrix& matrix_ ) = 0;

    /// �s��̔z��̎擾
	virtual LNRESULT getMatrixArray( LMatrix* matrices_, u32 count_ ) = 0;

    /// �s��̔z��̐ݒ�
	virtual LNRESULT setMatrixArray( const LMatrix* matrices_, u32 count_ ) = 0;

    /// �e�N�X�`���̐ݒ� (�T���v���X�e�[�g�͍X�V���Ȃ�)
	virtual LNRESULT setTexture( ITexture* texture_ ) = 0;

    /// �e�N�X�`���̐ݒ�
	virtual LNRESULT setTexture( ITexture* texture_, const LSamplerState& state_ ) = 0;

    /// ������̎擾 (OpenGL ��Ή�)
	virtual LNRESULT getString( const char** str_ ) = 0;

    /// ������̐ݒ� (OpenGL ��Ή�)
	virtual LNRESULT setString( const char* str_ ) = 0;


    /// �ϐ����̎擾
	virtual const char* getName() const = 0;

    /// �Z�}���e�B�N�X���̎擾 (�Ȃ��ꍇ�� NULL)
	virtual const char* getSemanticName() const = 0;

    /// �^���̎擾
    virtual const LNShaderVariableTypeDesc& getTypeDesc() const = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �C���f�b�N�X�ɂ���ăA�m�e�[�V�������擾����
    //
    //  @param[in]  index_ : 0 ����n�܂�C���f�b�N�X
    //
    //*/
    //---------------------------------------------------------------------
	virtual IShaderVariable* getAnnotationByIndex( lnU32 index_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���O�ɂ���ăA�m�e�[�V�������擾����
    //
    //  @param[in]  name_ : �A�m�e�[�V�����̖��O
    //*/
    //---------------------------------------------------------------------
	virtual IShaderVariable* getAnnotationByName( const char* name_ ) = 0;

protected:

    virtual ~IShaderVariable() {}
};

//=============================================================================
// �� IShaderTechnique �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �V�F�[�_�v���O�������̃e�N�j�b�N�̃C���^�[�t�F�C�X
//*/
//=============================================================================
class IShaderTechnique
    : public Base::Interface
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���O�ɂ���ăA�m�e�[�V�������擾����
    //
    //  @param[in]  name_ : �A�m�e�[�V�����̖��O
    //*/
    //---------------------------------------------------------------------
	virtual IShaderVariable* getAnnotationByName( const char* name_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �C���f�b�N�X�ɂ���ăA�m�e�[�V�������擾����
    //
    //  @param[in]  index_ : 0 ����n�܂�C���f�b�N�X
    //*/
    //---------------------------------------------------------------------
	virtual IShaderVariable* getAnnotationByIndex( lnU32 index_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �e�N�j�b�N�̖��O���擾����
    //*/
    //---------------------------------------------------------------------
    virtual const char* getName() const = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �e�N�j�b�N���̃p�X�̐����擾����
    //*/
    //---------------------------------------------------------------------
	virtual u32 getPasses() const = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���O�ɂ���ăp�X���擾����
    //
    //  @param[in]  name_ : �p�X�̖��O
    //*/
    //---------------------------------------------------------------------
	virtual IShaderPass* getPassByName( const char* name_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �C���f�b�N�X�ɂ���ăp�X���擾����
    //
    //  @param[in]  index_ : 0 ����n�܂�C���f�b�N�X
    //*/
    //---------------------------------------------------------------------
	virtual IShaderPass* getPassByIndex( lnU32 index_ ) = 0;

protected:

    virtual ~IShaderTechnique() {}
};


//=============================================================================
// �� IShaderPass �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �V�F�[�_�v���O�������̃p�X�̃C���^�[�t�F�C�X
//*/
//=============================================================================
class IShaderPass
    : public Base::Interface
{
public:

    /// �p�X�̖��O���擾����
    virtual const lnChar* getName() const = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      ���O�ɂ���ăA�m�e�[�V�������擾����
    //
    //  @param[in]  name_ : �A�m�e�[�V�����̖��O
    //*/
    //---------------------------------------------------------------------
	virtual IShaderVariable* getAnnotationByName( const lnChar* name_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �C���f�b�N�X�ɂ���ăA�m�e�[�V�������擾����
    //
    //  @param[in]  index_ : 0 ����n�܂�C���f�b�N�X
    //*/
    //---------------------------------------------------------------------
	virtual IShaderVariable* getAnnotationByIndex( lnU32 index_ ) = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �p�X�̓K�p���J�n����
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT begin() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �p�X�̓K�p���I������
    //*/
    //---------------------------------------------------------------------
	virtual LNRESULT end() = 0;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �p�X�̎��s���ɕω������X�e�[�g���f�o�C�X�ɑ��M����
    //
    //  @pattention
    //              CgFX �̏ꍇ�A�e�N�X�`���̐ݒ�͑��M����Ȃ��̂Œ��ӂ��Ă��������B
    //*/
    //---------------------------------------------------------------------
	virtual void commit() = 0;

protected:

    virtual ~IShaderPass() {}
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================