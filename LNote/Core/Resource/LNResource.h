//=============================================================================
//�y Resource �z
//=============================================================================

#pragma once

#include "../File/StreamObject.h"

namespace LNote
{
namespace Core
{
namespace Resource
{

/// �����񃊃\�[�X
namespace String
{
    static const lnChar* ERR_InvalidCallException   = _T( "���\�b�h�̌Ăяo���������ł��B�����ɕs���Ȓl���n���ꂽ�\��������܂��B" );
    static const lnChar* ERR_OutOfMemoryException   = _T( "�������������̂ɏ\���ȃ������̈悪����܂���B" );
    static const lnChar* ERR_FileNotFoundException  = _T( "�t�@�C�������݂��Ȃ����A�A�N�Z�X�ł��Ȃ��t�@�C���ł��B" );
    static const lnChar* ERR_SystemCallException    = _T( "�V�X�e���@�\�̌Ăяo���ŃG���[���������܂����B" );
    static const lnChar* ERR_FileFormatException    = _T( "�Ή����Ă��Ȃ��t�@�C���t�H�[�}�b�g�ł��B" );
    static const lnChar* ERR_NotImplementedException= _T( "�������A�܂��͖��Ή��̋@�\���Ăяo����܂����B" );
    static const lnChar* ERR_InitializedException   = _T( "�I�u�W�F�N�g�͊��ɏ���������Ă��܂��B" );
    static const lnChar* ERR_FatalException         = _T( "�v���I�ȃG���[���������܂����B" );
    static const lnChar* ERR_ComException           = _T( "HRESULT �ɃG���[���Ԃ���܂����B" );
    static const lnChar* ERR_OpenGLException        = _T( "OpenGL �ŃG���[���Ԃ���܂����B" );

    static const lnChar* ERR_FileArchive1           = _T( "#�A�[�J�C�u�t�@�C�����J�����Ƃ��ł��܂���ł����B" );
    static const lnChar* ERR_FileArchive2           = _T( "#�s���ȃA�[�J�C�u�t�@�C�����J�����Ƃ��܂����B" );
    static const lnChar* ERR_FileArchive3           = _T( "#�A�[�J�C�u�ɂ��J�����߂̃L�[���s���ł��B" );

    static const lnChar* ERR_SceneViewport          = _T( "�w�肳�ꂽ�V�F�[�_�̓v���v���Z�X�܂��̓|�X�g�v���Z�X�p�ł͂���܂���B" );
    static const lnChar* ERR_SceneShader_onSettingShader    = _T( "���L�ł��Ȃ��V�F�[�_���ݒ肳��悤�Ƃ��܂����B" );
    static const lnChar* ERR_SceneShader_build1     = _T( "�I�u�W�F�N�g��`�悷��G�t�F�N�g�� \"Draw=Buffer\" �R�}���h��������܂����B" );
    static const lnChar* ERR_SceneShader_build2     = _T( "�X�N���[���o�b�t�@��`�悷��G�t�F�N�g�� \"Draw=Geometry\" �R�}���h��������܂����B" );
    static const lnChar* ERR_SceneShader_build3     = _T( "�|�X�g�G�t�F�N�g�p�̃G�t�F�N�g�� \"ScriptExternal=Color\" �R�}���h�̂Ȃ��e�N�j�b�N��������܂����B" );
    static const lnChar* ERR_SceneShader_build4     = _T( "�|�X�g�G�t�F�N�g�p�ł͂Ȃ��G�t�F�N�g�� \"ScriptExternal=Color\" �R�}���h�̂���e�N�j�b�N��������܂����B" );
}

/// �V�F�[�_�v���O�������\�[�X
//namespace Shader
//{
//    /// ��{�`��`��p
//    extern const unsigned char GeometryRenderer[];
//    extern const unsigned int  GeometryRenderer_Size;
//
//    /// �X�v���C�g�`��p
//    extern const unsigned char SpriteRenderer[];
//    extern const unsigned int  SpriteRenderer_Size;
//
//    /// SceneShader 2D ��{
//    extern const unsigned char SSBasic2D[];
//    extern const unsigned int  SSBasic2D_Size;
//
//    /// SceneShader 3D ��{
//    extern const unsigned char SSBasic3D[];
//    extern const unsigned int  SSBasic3D_Size;
//
//    /// SceneShader ���C�e�B���O�Ȃ�
//    extern const unsigned char SSNoLighting[];
//    extern const unsigned int  SSNoLighting_Size;
//
//    
//}

///// OpenGL �V�F�[�_
//namespace GLSL
//{
//    /// ��{�`��`��p
//    extern const unsigned char GeometryRenderer[];
//    extern const unsigned int  GeometryRenderer_Size;
//}

/// �摜���\�[�X
namespace Image
{
   
}

/// ���ߍ��݃��\�[�X���X�g���[���Ƃ��ĕԂ��N���X
class HardCodeResourceArchive
    : public File::IArchive
{
public:

    HardCodeResourceArchive();

public:

    /// ���̓X�g���[���̍쐬
    virtual File::IInStream* createStream( const lnChar* filename_ );
};


} // namespace Resource
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================