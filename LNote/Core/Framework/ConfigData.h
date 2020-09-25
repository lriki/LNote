//=============================================================================
//�y Public �z
//-----------------------------------------------------------------------------
///**
//  @file       Public.h
//  @brief      Public
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Types.h"
#include "../Geometry/DrawingShapes.h"
#include "../Audio/Interface.h"
#include "../Graphics/Common/Types.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{

//=============================================================================
// �� ConfigData �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      ���C�u�����̏��������
//*/
//=============================================================================
class ConfigData
{
public:

    //-----------------------------------------------
    // �S��

    lnU32                   ClassInitializeFlag;    ///< ����������@�\�̎w�� (LNClassFlag �̑g�ݍ��킹)
    lnU32                   ClassUpdateFlag;        ///< �����X�V����@�\�̎w�� (LNClassFlag �̑g�ݍ��킹)
    bool                    EnableMakeLogFile;      ///< ���O�t�@�C�����o�͂��邩�������t���O
    const lnChar*           LogSaveDir;             ///< ���O�t�@�C���̕ۑ���f�B���N�g��
    LNUpdateMode            UpdateMode;             ///< �t���[�����[�N�̍X�V���@
    bool                    ForbidAutoFullscreen;   ///< Alt + Enter �ɂ��t���X�N���[���؂�ւ��̗L���t���O
    lnU32                   FrameRate;              ///< �t���[�����[�g
    bool                    AllInitialize;          ///< Framework::initialize() �ł��ׂĂ̋@�\������������
    bool                    EnableMultipleRunning;  ///< true �̏ꍇ�A���d�N����������
    bool                    EnableShowFPS;          ///< true �̏ꍇ�A�E�B���h�E�̃^�C�g���o�[�Ɍ��݂� FPS ��\������

    //-----------------------------------------------
    // System �֌W
    
    const lnChar*           WindowTitle;            ///< �E�B���h�E�^�C�g���̕�����
    lnU32                   WindowWidth;            ///< �E�B���h�E�̃N���C�A���g�̈�̕� (Android �ł͖���)
    lnU32                   WindowHeight;           ///< �E�B���h�E�̃N���C�A���g�̈�̍��� (Android �ł͖���)
    void*                   WindowHandle;           ///< ���[�U�[��`�̃E�B���h�E�n���h�� (Windows �̂�)
    bool                    EnableConsole;          ///< �R���\�[�������蓖�Ă�
    //bool                    EnableDummyWindow;      ///< (WindowHandle == NULL �ł��E�B���h�E���쐬���Ȃ�)

    //-----------------------------------------------
    // File �֌W

    LNArchiveOpenDataList   ArchiveList;            ///< (addArchive() ���g���Ēǉ����邱��)
    LNFileAccessMode        FileAccessMode;         ///< �t�@�C���A�N�Z�X�̗D�惂�[�h

    //-----------------------------------------------
    // Audio �֌W

    lnU32                   SoundCacheSize;         ///< �����t�@�C���̃L���b�V���T�C�Y (�t�@�C����)
    LNDirectMusicUsingMode  DMInitMode;             ///< DirectMusic �̏��������@

    //-----------------------------------------------
    // Graphics �֌W

    LNGraphicsAPI           GraphicsAPI;            ///< �`��Ɏg�p����O���t�B�b�N�X API
    LNFontNameList          FontFilenameList;       ///< �g�p����t�H���g�t�@�C�����̃��X�g (�V�X�e���ɃC���X�g�[������Ă��Ȃ��t�H���g���g���Ƃ��p)
    Geometry::Size			BackbufferSize;
    bool                    EnableFPUPreserve;      ///< true �̏ꍇ�ADirectX �̏������ŕ��������_���Z���j�b�g�̉��Z���x�𗎂Ƃ��Ȃ��悤�ɂ���
    bool                    EnableFullscreen;       ///< �N�����t���X�N���[���̗L���ݒ�

    //-----------------------------------------------
    // Android
#if defined(LNOTE_ANDROID)
    android_app*            AndroidApp;
#endif

public:

    //-------------------------------------------------------------
    /// �f�t�H���g�l
    ConfigData()
    {
        ClassInitializeFlag     = LN_CLASS_ALL;
        ClassUpdateFlag         = LN_CLASS_ALL;
        EnableMakeLogFile       = false;
        LogSaveDir              = NULL;
        UpdateMode              = LN_UPDATEMODE_THREADED;//LN_UPDATEMODE_ALWAYS;//
        ForbidAutoFullscreen    = true;
        FrameRate               = 60;
        AllInitialize           = true;
        EnableMultipleRunning   = false;
        EnableShowFPS           = false;

        WindowTitle             = LNOTE_NAME " " LNOTE_VERSION_STR;
        WindowWidth             = 640;
        WindowHeight            = 480;
        WindowHandle            = NULL;
        EnableConsole           = false;
        //EnableDummyWindow       = false;

        FileAccessMode          = LN_FILEACCESS_DIRECTORY_FIRST;

        SoundCacheSize          = 32;
        DMInitMode              = LN_DMUSINGMODE_NOTUSE;

        GraphicsAPI             = LN_GRAPHICSAPI_DIRECTX9;
        BackbufferSize          = Geometry::Size( 640, 480 );
        EnableFPUPreserve       = false;
        EnableFullscreen        = false;

#ifdef LNOTE_DEBUG
        EnableMakeLogFile       = true;
        EnableConsole           = true;
        EnableShowFPS           = true;
#endif  

#if defined(LNOTE_ANDROID)
        AndroidApp              = NULL;
#endif
    }

public:

    /// �A�[�J�C�u�̒ǉ�
    void addArchive( const lnChar* directory_, const lnChar* filename_, const lnChar* key_ )
    {
        LNArchiveOpenData data;
        data.Directory   = directory_;
        data.ArchiveName = filename_;
        data.ArchiveKey  = key_;
        ArchiveList.push_back( data );
    }

    /// �t�H���g�t�@�C���̒ǉ�
    void addFontFile( const lnChar* filename_ )
    {
        FontFilenameList.push_back( ln_std_tstring( filename_ ) );
    }

};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================