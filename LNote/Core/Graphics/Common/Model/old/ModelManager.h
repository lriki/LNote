//=============================================================================
//�y ModelManager �z
//-----------------------------------------------------------------------------
///**
//  @file       ModelManager.h
//  @brief      ModelManager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../Base/Cache.h"
#include "../../../File/Interface.h"
#include "../../../File/Manager.h"
#include "../../../Physics/Manager.h"
#include "../../Interface.h"
#include "Model.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Graphics
{
class ModelCore;
class Motion;

typedef Base::CacheObjectManager< ModelCore >   ModelCoreCache;
typedef Base::CacheObjectManager< Motion >      MotionCache;

//=============================================================================
// �� ModelManager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		3D ���f���Ǘ��N���X
//*/
//=============================================================================
class ModelManager
     : public Base::ReferenceObject
     , public ModelCoreCache
     //, public MotionCache
{
public:

	/// �������f�[�^
	struct InitData
	{
        Base::LogFile*      LogFile;
        IGraphicsDevice*    GraphicsDevice;
        File::Manager*      FileManager;
        Physics::Manager*   PhysicsManager;
        lnU32               ModelCoreCacheSize;
	};

public:
    ModelManager();
    LN_REFOBJ_METHODS;

protected:
    virtual ~ModelManager();

public:

    /// ������
    LNRESULT initialize( const InitData& init_data_ );

    /// �I������
    void finalize();

    /// ���f���t�@�C����ǂݍ���
    LNRESULT createModel( Model** model_, void* data_, lnU32 size_, LSharingKey key_ );

    /// ���f���t�@�C����ǂݍ���
    LNRESULT createModel( Model** model_, File::IInStream* stream_, LSharingKey key_ );

    /// ���f���t�@�C����ǂݍ��� (�t�@�C�����w��)
    LNRESULT createModel( Model** model_, const lnChar* filename_, LSharingKey key_ );

    /// ���[�V�����t�@�C����ǂݍ���
    LNRESULT createMotion( Motion** motion_, void* data_, lnU32 size_, LSharingKey key_ );

    /// ���[�V�����t�@�C����ǂݍ���
    LNRESULT createMotion( Motion** motion_, File::IInStream* stream_, LSharingKey key_ );

    /// ���[�V�����t�@�C����ǂݍ��� (�t�@�C�����w��)
    LNRESULT createMotion( Motion** motion_, const lnChar* filename_, LSharingKey key_ );

    /// IGraphicsDevice �̎擾
    IGraphicsDevice* getGraphicsDevice() { return mGraphicsDevice; }

public:

    /// �����Ǘ��N���X�̎擾
    Physics::Manager* getPhysicsManager() { return mPhysicsManager; }

    /// �e�N�X�`���쐬 (ModelCore ����쐬���ɌĂ΂��BPMD�AX �t�@�C���̃e�N�X�`������ char �Ȃ̂� lnChar �ɂ͂��Ȃ�)
    LNRESULT createTexture( ITexture** texture_, const char* filename_ );

private:

    Base::LogFile*      mLogFile;
    IGraphicsDevice*    mGraphicsDevice;
    File::Manager*      mFileManager;
    Physics::Manager*   mPhysicsManager;
    Base::TempBuffer    mTempBuffer;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Lumine

//=============================================================================
//                              end of file
//=============================================================================