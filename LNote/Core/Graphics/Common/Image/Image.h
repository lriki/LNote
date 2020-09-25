//=============================================================================
//�y Image �z
//-----------------------------------------------------------------------------
///**
//  @file       Image.h
//  @brief      Image
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../File/StreamObject.h"

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
// �� IImage �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		
//
//  @note
//              �擾�ł���C���[�W�f�[�^�͂��ׂ�32�r�b�g�B
//              �I�[�_�[�� AA BB GG RR
//*/
////=============================================================================
//class IImage
//{
//public:
//
//    /// �C���X�^���X�̍쐬 (swap_height_ = true �ŏ㉺�𔽓]����)
//    static IImage* createImage( void* data_, lnU32 size_, bool swap_height_ );
//
//public:
//
//    virtual ~IImage() { }
//
//    /// �����̎擾
//    virtual lnU32 getWidth() const = 0;
//
//    /// �����̎擾
//    virtual lnU32 getHeight() const = 0;
//
//    /// �C���[�W�f�[�^�̎擾
//    virtual void* getImageData() = 0;
//};



class Image
{
public:

    lnU32   Width;
    lnU32   Height;
    lnU32*  ImageData;
    lnU32   ImageDataSize;

public:

    /// Width�AHeight �ɒl���i�[����
    bool getImageInfo( File::IInStream* stream_ );

    /// �o�C�g�f�[�^����쐬 (��������)
    bool createFromBytes( void* data_, lnU32 size_, bool swap_height_ );

    /// png �f�[�^����쐬 (ImageData �Ƀo�b�t�@���m�ۂ���Ă��ăT�C�Y���\���ł���΂����ɏ�������)
    bool createPngFromBytes( void* data_, lnU32 size_, bool swap_height_ );

public:

    Image() : Width( 0 ), Height( 0 ), ImageData( NULL ), ImageDataSize( 0 ) {}
    ~Image() { SAFE_DELETE( ImageData ); }
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