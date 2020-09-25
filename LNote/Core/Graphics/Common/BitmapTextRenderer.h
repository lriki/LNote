//=============================================================================
//�y BitmapTextRenderer �z
//-----------------------------------------------------------------------------
///**
//  @file       BitmapTextRenderer.h
//  @brief      BitmapTextRenderer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Geometry/Vector4.h"
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

//=============================================================================
// �� BitmapTextRenderer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �e�N�X�`�����ɕ�����`�悷��N���X
//*/
//=============================================================================
class BitmapTextRenderer
    : public Base::NonCopyable
{
public:

    /// �R���X�g���N�^
    explicit BitmapTextRenderer( LNGraphicsAPI api_ );

    /// �f�X�g���N�^
    ~BitmapTextRenderer();

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �`���ƂȂ�r�b�g�}�b�v�o�b�t�@��ݒ肷��
    //
    //  @param[in]  buffer_ : �������݂��s���������o�b�t�@�A�h���X
    //  @param[in]  width_  : �C���[�W�̉���
    //  @param[in]  height_ : �C���[�W�̍���
    //
    //  @par
    //              1 �s�N�Z���� ARGB �� 32 �r�b�g�J���[�Ƃ��ď������݂܂��B
    //*/
    //---------------------------------------------------------------------
    void setTargetBuffer( void* buffer_, u32 width_, u32 height_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      ������̕`��Ɏg���t�H���g��ݒ肷��
    //*/
    //---------------------------------------------------------------------
    void setFont( IFont* font_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �������`�悷��
    //
    //  @param[in]  text_  : �`�悷�镶����
    //  @param[in]  rect_  : �`��̈�
    //  @param[in]  align_ : �e�L�X�g�A���C���̐ݒ�
    //  @param[in]  len_   : �`�悷�镶���� (-1 ���w�肷��ƏI�[�܂ŕ`�悷��)
    //
    //  @par
    //              �e�L�X�g�A���C���̓r�b�g�}�b�v�̕��ł͂Ȃ� rect_ ����Ƃ��čs���܂��B<br>
    //              �Ⴆ�Ε�������E�����Ƃ����ꍇ�Arect_.x + rect_.width ��
    //              �E�[�Ƃ��ĕ`����s���܂��B
    //*/
    //---------------------------------------------------------------------
    LNRESULT drawText( const lnChar* text_, const Geometry::Rect& rect_, LNTextAlign align_ = LN_TEXTALIGN_LEFT, int len_ = -1 );

private:

    /// 1�s���̕`��
    LNRESULT _drawLine( const lnChar* text_, int len_, int l_, int t_, int r_, int b_, u32 color_, int* write_len_ );
    
private:

    LNGraphicsAPI   mGraphicsAPI;       ///< �F�R�[�h�ϊ��Ɏg��
    u32*		    mBitmapBuffer;
    u32			    mWidth;
	u32			    mHeight;
    IFont*		    mFont;
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