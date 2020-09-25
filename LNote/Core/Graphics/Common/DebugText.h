//=============================================================================
//�y DebugText �z
//-----------------------------------------------------------------------------
///**
//  @file       DebugText.h
//  @brief      DebugText
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

//=============================================================================
// �� DebugText �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�f�o�b�O�p�̕������`�悷�邽�߂̃N���X
//*/
//=============================================================================
class DebugText
    : public Base::ReferenceObject
{
public:
    DebugText();

protected:
    virtual ~DebugText();

public:

    /// ������
    void initialize( IGraphicsDevice* device_, SpriteRenderer* renderer_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      �\���ʒu�̐ݒ�
    //*/		
	//---------------------------------------------------------------------
	void setPosition( int x_, int y_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      �����F�̐ݒ�
    //*/		
	//---------------------------------------------------------------------
    void setColor( const LColor& color_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      ��������o�͂���
    //
    //  @par
    //              ������͓����Ńo�b�t�@�ɗ��߂��A����� draw() ��
    //              �S�ĕ`�悳��܂��B
    //*/		
	//---------------------------------------------------------------------
	LNRESULT printf( const char* format_, ... );

    DebugText& operator << ( const char* c_ );
	DebugText& operator << ( unsigned char v_ );
	DebugText& operator << ( short v_ );
	DebugText& operator << ( unsigned short v_ );
	DebugText& operator << ( int v_ );
	DebugText& operator << ( unsigned int v_ );
    DebugText& operator << ( long v_ );
	DebugText& operator << ( unsigned long v_ );
	DebugText& operator << ( float v_ );
	DebugText& operator << ( double v_ );
	DebugText& operator << ( bool v_ );
	DebugText& operator << ( void* p_ );

    /// (�}���`�X���b�h�΍�B�����t�F�[�Y�ŌĂԂ���)
    void commitText() { mDrawingText = mText; mText = _T(""); }

    //---------------------------------------------------------------------
	///**
    //  @brief      �������`�悷��
    //*/		
	//---------------------------------------------------------------------
	void draw();

public:

    static const int TEMP_BUFFER_SIZE = 128;    ///< �ꎞ�o�b�t�@�̑傫��

private:

    IGraphicsDevice*    mGraphicsDevice;
    SpriteRenderer*     mRenderer;
    ITexture*           mFontTexture;

    Geometry::Rect      mDrawArea;
    LColor              mColor;		                    ///< �����̐F

    char mTempBuffer[ TEMP_BUFFER_SIZE ];	    ///< ��Ɨp�̈ꎞ�o�b�t�@
    ln_std_tstring mText;					    ///< �\�����镶���� ( draw() ��ɃN���A )
    ln_std_tstring mDrawingText;			    ///< �}���`�X���b�h�΍�
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