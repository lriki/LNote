//=============================================================================
//�y SceneShaderManager �z
//-----------------------------------------------------------------------------
///**
//  @file       SceneShaderManager.h
//  @brief      SceneShaderManager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <algorithm>
#include "../../Base/STLUtil.h"
#include "../../Base/Cache.h"
#include "../../System/Interface.h"
#include "../../File/Manager.h"
#include "../../Graphics/Interface.h"
#include "../../Graphics/Manager.h"
#include "../../Game/GameTime.h"
#include "../Common.h"
#include "MMETypes.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
class SceneGraph;
class SceneShader;
class OffscreenScene;
class DrawCommandContainer;
typedef Base::CacheObjectManager< SceneShader >    SceneShaderCache;

//=============================================================================
// �� SceneShaderManager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �V�[���p�V�F�[�_�̃N���X
//*/
//=============================================================================
class SceneShaderManager
    : public Base::ReferenceObject
    , public SceneShaderCache
{
public:

    /// �������f�[�^
    struct InitData
    {
        Base::LogFile*      LogFile;
        File::Manager*      FileManager;
        Graphics::Manager*  GraphicsManager;
        lnU32               ShaderCacheSize;
        bool                UseSemantics;       ///< true �̏ꍇ�A�V�F�[�_�ϐ��̌����ɃZ�}���e�B�N�X�����g�� (false �̏ꍇ�͖��O)
    };

public:
    SceneShaderManager( SceneGraph* scene_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~SceneShaderManager();

public:

    /// ������
    LNRESULT initialize( const InitData& init_data_ );

    /// �I������
    void finalize();

    /// �V�F�[�_�̍쐬
    //LNRESULT createSceneShader( SceneShader** shader_, const void* data_, u32 size_, const lnChar* name_ );

    /// �V�F�[�_�̍쐬 (�t�@�C�����w��E�t�@�C�������L�[�Ƃ��Ď����o�^)
    LNRESULT createSceneShader( SceneShader** shader_, const lnChar* filename_ );

    /// ������K�v�ȃV�F�[�_���Ȃ����`�F�b�N����
    void updateShaderList();

    /// �S�V�F�[�_�ɃV�[���P�ʂŐݒ肷��ׂ��f�[�^��ݒ肷��
    void updateAllShaderGlobalParam( const Game::GameTime& game_time_ );

    /// �S�F�[�_�ɃJ�����P�ʂŐݒ肷��ׂ��f�[�^��ݒ肷��
    void updateAllShaderCameraParam( const LNCameraSceneParam& param_ );

    /// ���ׂẴI�t�X�N���[��RT�̕`��R�}���h���쐬����
    void makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ );

    /// (SceneGraph::addNode() ����Ă΂��)
    void addSceneNodeToOffscreenScene( SceneNode* node_ );

    /// (SceneGraph::onDeleteNode() ����Ă΂��)
    void removeSceneNodeFromOffscreenScene( SceneNode* node_ );

public:

    /// ���b�Z�[�W���� (SceneGraph ����Ă΂��)
    LNRESULT onEvent( const System::Event& args_ );

    SceneGraph* getScene() { return mScene; }

    /// �Z�}���e�B�N�X�̕������ID�̑Ή��\�̎擾
    const MMESemanticsMap& getSemanticsMap() { return mMMESemanticsMap; }

    /// �A�m�e�[�V�����̕������ID�̑Ή��\
    const MMEAnnotationMap& getAnnotationMap() { return mMMEAnnotationMap; }

    /// (SceneShader ������Ă΂��)
    LNRESULT createTexture(
        Graphics::ITexture** texture_,
        MMEVariableRequest req_,
        const lnChar* resource_name_,
        lnU32 width_,
        lnU32 height_,
        lnU32 depth_,
        lnU32 miplevels_,
        LNSurfaceFormat format_ );

    /// GraphicsManager �̎擾
    Graphics::Manager* getGraphicsManager() { return mGraphicsManager; }

    /// �O���t�B�b�N�X�f�o�C�X�̎擾
    Graphics::IGraphicsDevice* getGraphicsDevice() { return mGraphicsDevice; }

    /// (OffscreenScene �̃R���X�g���N�^����Ă΂��)
    void addOffscreenScene( OffscreenScene* ofs_ ) { mOffscreenSceneList.push_back( ofs_ ); }

    /// (OffscreenScene �̃f�X�g���N�^����Ă΂��)
    void removeOffscreenScene( OffscreenScene* ofs_ ) { Base::STLUtil::remove( mOffscreenSceneList, ofs_ ); }

    /// SceneShader::updateNodeParam() �ōs��E�x�N�g���z����Z�b�g����Ƃ��Ɏg���̂Ō��J
    Base::TempBuffer* getTempBuffer() { return &mTempBuffer; }

 
private:

    /// �V�F�[�_�̍쐬
    LNRESULT _createSceneShader( SceneShader** shader_, const void* data_, u32 size_, const lnChar* name_ );

    /// �V�F�[�_�̍쐬 (���̓X�g���[������)
    LNRESULT _createSceneShader( SceneShader** shader_, File::IInStream* stream_, const lnChar* name_ );  

private:

    typedef std::vector< SceneShader* >     SceneShaderList;
    typedef std::vector< OffscreenScene* >  OffscreenSceneList;

private:

    Base::LogFile*              mLogFile;
    SceneGraph*                 mScene;
    File::Manager*              mFileManager;
    Graphics::Manager*          mGraphicsManager;
    Graphics::IGraphicsDevice*  mGraphicsDevice;

    MMESemanticsMap             mMMESemanticsMap;   ///< �Z�}���e�B�N�X�̕������ID�̑Ή��\ (�Z�}���e�B�N�X���g��Ȃ��ꍇ�͕ϐ���) 
    MMEAnnotationMap            mMMEAnnotationMap;  ///< �A�m�e�[�V�����̕������ID�̑Ή��\

    SceneShaderList             mSceneShaderList;
    OffscreenSceneList          mOffscreenSceneList;

    double                      mPlayStartTime;
    MMESceneParam               mMMESceneParam;     ///< Scene �P�ʂ̃V�F�[�_�ݒ�p�����[�^ (updateAllShaderGlobalParam() �Őݒ�)
   
    Base::TempBuffer            mTempBuffer;        ///< 


};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================