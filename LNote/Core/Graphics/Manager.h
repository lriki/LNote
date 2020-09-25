//=============================================================================
//�y Manager �z
//-----------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../File/Manager.h"
#include "../Physics/Manager.h"
#include "../System/Manager.h"
#include "Interface.h"
#include "Common/GeometryRenderer.h"
#include "Common/DebugText.h"
#include "Font/FontManagerBase.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace Win32 { class Win32FontManager; }

class SpriteRenderer;
class ModelManager;
class DebugText;

//=============================================================================
// �� Manager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�O���t�B�b�N�X�@�\�̊Ǘ��N���X
//*/
//=============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

	/// �������f�[�^
	struct InitData
	{
        Base::LogFile*      LogFile;
        File::Manager*      FileManager;
        Physics::Manager*   PhysicsManager;
        System::Manager*    SystemManager;
        Task::Manager*      TaskManager;        ///< OpenGL Shader �p�b��Ή�
        //System::IWindow*    MainWindow;
        LNGraphicsAPI       GraphicsAPI;
        Geometry::Size		CanvasSize;         ///< �L�����o�X (�o�b�N�o�b�t�@) �̃T�C�Y
        bool                EnableFPUPreserve;  ///< ���������_�̉��Z���x�𗎂Ƃ��Ȃ��悤�ɂ���ꍇ�� true
        LNFontNameList      FontFilenameList;
    };

public:

    Manager();

    LN_REFOBJ_METHODS;

public:

    /// ������
    LNRESULT initialize( const InitData& init_data_ );

    /// �I������
    void finalize();

    /// �g�p���� GraphicsAPI �^�C�v�̎擾
    LNGraphicsAPI getGraphicsAPI() { return mGraphicsAPI; }

    /// IGraphicsDevice �̎擾
    IGraphicsDevice* getGraphicsDevice();

    /// �t�H���g�̍쐬 (�t�H���g���� NULL �̏ꍇ�̓f�t�H���g�̃t�H���g���g����)
    LNRESULT createFont( IFont** obj_, const lnChar* fontname_ );

    /// �f�t�H���g�̃t�H���g���擾����
    IFont* getDefaultFont();

    /// �`��`��N���X�̎擾
    GeometryRenderer* getGeometryRenderer() { return mGeometryRenderer; }

    /// 3D�X�v���C�g�`��N���X�̎擾
    SpriteRenderer* getSprite3DRenderer() { return mSprite3DRenderer; }

    /// 2D�X�v���C�g�`��N���X�̎擾
    SpriteRenderer* getSprite2DRenderer() { return mSprite2DRenderer; }

    /// ���f���Ǘ��N���X�̎擾
    ModelManager* getModelManager() { return mModelManager; }

    /// �f�o�b�O������`��N���X�̎擾
    DebugText* getDebugText() { return mDebugText; }

    /// �_�~�[�p�e�N�X�`���̎擾 (32x32�A�s�����̔�)
    ITexture* getDummyTexture() { return mDummyTexture; }

    /// �e�N�X�`�����t�@�C������쐬����
    //LNRESULT createTextureFromFile( ITexture** tex_, const lnChar* filename_, u32 color_key_ = 0, u32 levels_ = 1, LNSurfaceFormat format_ = LN_FMT_UNKNOWN, LSharingKey key_ = lnNullKey );

    

    Task::Manager* getTaskManager() { return mTaskManager; }

protected:

	virtual ~Manager();

private:

    Base::LogFile*              mLogFile;
    File::Manager*              mFileManager;
    Task::Manager*              mTaskManager;        ///< OpenGL Shader �p�b��Ή�
    IGraphicsDevice*            mGraphicsDevice;
    LNGraphicsAPI               mGraphicsAPI;
    FontManagerBase*            mFontManager;
    //Win32::Win32FontManager*    mFontManager;

    GeometryRenderer*           mGeometryRenderer;
    SpriteRenderer*             mSprite3DRenderer;
    SpriteRenderer*             mSprite2DRenderer;
    ModelManager*               mModelManager;
    DebugText*                  mDebugText;
    ITexture*                   mDummyTexture;

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