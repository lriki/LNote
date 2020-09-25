//=============================================================================
//�y Win32Font �z
//-----------------------------------------------------------------------------
///**
//  @file       Win32Font.h
//  @brief      Win32Font
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../Base/RefString.h"
#include "../../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace Win32
{

//=============================================================================
// �� Win32Font �N���X
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
class Win32Font
    : public Base::ReferenceObject
    , public IFont
{
public:

    /// �R���X�g���N�^
    Win32Font();

    /// �f�X�g���N�^
    virtual ~Win32Font();

    LN_REFOBJ_METHODS;

public:

    /// ������
    LNRESULT initialize(
        const lnChar* fontname_,
        u32 size_,
        const LColor& color_,
        bool bold_,
        bool italic_ );

    /// �t�H���g���̐ݒ�
	virtual LNRESULT setName( const lnChar* fontname_ );

    /// �t�H���g���̎擾
    virtual const lnChar* getName() const { return mFontName.c_str(); }

    /// �t�H���g�T�C�Y�̐ݒ�
    virtual void setSize( u32 size_ );

    /// �t�H���g�T�C�Y�̎擾
    virtual u32 getSize() const { return mFontSize; }

	/// �t�H���g�J���[�̐ݒ�
	virtual void setColor( const LColor& color_ );

    /// �t�H���g�J���[�̎擾
    virtual const LColor& getColor() const { return mColor; }

    /// �G�b�W�J���[�̐ݒ�
    virtual void setEdgeColor( const LColor& color_ ) { mEdgeColor = color_; mNeedUpdate = true; }

    /// �G�b�W�J���[�̎擾
    virtual const LColor& getEdgeColor() const { return mEdgeColor; }

    /// �G�b�W�̕��̐ݒ�
	virtual void setEdgeSize( u32 size_ ) { mEdgeSize = size_; mNeedUpdate = true; }

    /// �G�b�W�̕��̎擾
	virtual u32 getEdgeSize() const { return mEdgeSize; }

	/// �������̐ݒ�
	virtual void setBold( bool flag_ );

    /// �������̔���
    virtual bool isBold() const { return ( mLogFont.lfWeight == FW_BOLD ); }

	/// �C�^���b�N�̂̐ݒ�
	virtual void setItalic( bool flag_ );

    /// �C�^���b�N�̂̔���
    virtual bool isItalic() const { return ( mLogFont.lfItalic != FALSE ); }

    /// �A���`�G�C���A�X�̗L���ݒ�
    virtual void setAntiAlias( bool flag_ ) { mOutlineQuality = ( flag_ ) ? FONT_ANTI_QUALITY : 1; mNeedUpdate = true; }

    /// �A���`�G�C���A�X�̗L������
    virtual bool isAntiAlias() const { return ( mOutlineQuality != 1 ); }

    /// �������`�悵���Ƃ��̃T�C�Y (�s�N�Z���P��) �̎擾 (char)
    virtual LNRESULT getTextSize( const lnChar* text_, int len_, Geometry::Rect* rect_ );

    /// �t�H���g�����̃r�b�g�}�b�v���̎擾�J�n
    virtual LNRESULT beginGetGlyphOutline();

    /// �t�H���g�����̃r�b�g�}�b�v���̎擾�I��
    virtual LNRESULT endGetGlyphOutline();

    /// �t�H���g�����̃r�b�g�}�b�v�����擾���� (char)
    virtual LNRESULT getGlyphOutline( const char* code_, LNGlyphOutlineData** data_ );

    /// �t�H���g�����̃r�b�g�}�b�v�����擾���� (wchar_t)
    virtual LNRESULT getGlyphOutline( const wchar_t* code_, LNGlyphOutlineData** data_ );

    /// �t�H���g�̃R�s�[���쐬����
    virtual LNRESULT deepCopy( IFont** font_ );
   
public:

    /// �t�H���g�n���h���̎擾
    HFONT getFontHandle() const { return mFontHandle; }

    bool isOutlineFont() const { return ( mEdgeSize > 0 ); }

private:

    /// mDIBBitmapBuffer ���� mGlyphOutlineData.Bitmap �� ARGB �r�b�g�}�b�v���쐬����
    //      glyph_w_, glyph_h_ : ���������̃T�C�Y
    LNRESULT _makeGlyphFromDIBBitmap( u32 glyph_w_, u32 glyph_h_ );

    /// ���݂̏�Ԃ���t�H���g���X�V���� (�č\�z)
    LNRESULT _updateFont();

    /// ���\�[�X���
    void _dispose();

private:

    /// _updateFont() �ō쐬���ꂽ�t�H���g�̎��
    enum GlyphType
    {
        LN_GT_NOEDGE_NOANTI = 0,
        LN_GT_EDGE_NOANTI,
        LN_GT_NOEDGE_ANTI,
        LN_GT_EDGE_ANTI
    };

private:

    static const int FONT_ANTI_QUALITY = 4;

    HFONT	                mFontHandle;	    ///< �t�H���g�n���h��
    LOGFONT	                mLogFont;           ///< �t�H���g�̑���
    LRefTString             mFontName;          ///< �t�H���g��

    u32                     mFontSize;
    LColor                  mColor;             ///< �t�H���g�J���[
    
    GlyphType               mGlyphType;

    LColor                  mEdgeColor;         ///< �G�b�W�J���[
    u32                     mEdgeSize;
    u32                     mRealEdgeSize;
    u32                     mOutlineQuality;    ///< �Œ� 1
    HDC                     mMemDC;
    HBITMAP                 mDIBBitmap;
    u8*                     mDIBBitmapBuffer;
    u32                     mDIBBitmapWidth;
    u32                     mDIBBitmapHeight;
    HPEN                    mOutlinePen;
    HBRUSH                  mOutlineBrush;
    HBRUSH                  mOutlineBGBrush;    ///< �u���V

  

    LNGlyphOutlineData      mGlyphOutlineData;  ///< �O���t�f�[�^
    u32                     mBitmapBufferSize;
    u32                     mTMAscent;
    HDC                     mHDC;
    

    HFONT                   mOldFont;
    
    
    bool                    mNeedUpdate;
    bool                    mBeganGetGlyphOutline;
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