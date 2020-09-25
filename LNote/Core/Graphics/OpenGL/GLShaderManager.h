//=============================================================================
//�y ShaderManager �z
//-----------------------------------------------------------------------------
///**
//  @file       ShaderManager.h
//  @brief      ShaderManager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Base/Cache.h"
#include "GLGraphicsDevice.h"

struct lua_State;

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace OpenGL
{
class Shader;
typedef Base::CacheObjectManager< Shader >      ShaderCache;

//=============================================================================
// �� ShaderManager �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �V�F�[�_�v���O�����̊Ǘ��N���X
//*/
//=============================================================================
class ShaderManager
    : public Base::ReferenceObject
    , public ShaderCache
{
public:

    ///< initialize() �ɓn���\����
	struct InitData
	{
        Base::LogFile*          LogFile;
        OpenGL::GraphicsDevice* GraphicsDevice;
        lnU32                   ShaderCacheSize;
	};

public:

    ShaderManager();
    virtual ~ShaderManager();
    LN_REFOBJ_METHODS;

public:

    /// ������
    void initialize( const InitData& init_data_ );

    /// �I������
    void finalize();

    /// �V�F�[�_�̍쐬
    virtual IShader* createShader( const lnChar* filename_, LSharingKey key_ );

    /// �V�F�[�_�v���O�����̃L���b�V���̃N���A
    virtual void clearCache() { ShaderCache::clear(); }

public:

    /// �X�N���v�g���s
    void doScript( const char* script_, Shader* caller_ );

    /// GraphicsDevice �̎擾
    GraphicsDevice* getGraphicsDevice() { return mGraphicsDevice; }

    /// File::Manager �̎擾
    File::Manager* getFileManager() { return mGraphicsDevice->getFileManager(); }

    /// �ꎞ�o�b�t�@�̎擾
    Base::TempBuffer& getTempBuffer() { return mGraphicsDevice->getTempBuffer(); }

    /// Shader �̃f�X�g���N�^����Ă΂��
    void removeShader( Shader* shader_ ) { mShaderList.remove( shader_ ); }

    /// �f�o�C�X���Z�b�g���O�ɌĂ�
    void onLostDevice();

	/// �f�o�C�X���Z�b�g����ɌĂ�
    void onResetDevice();

    /// (GraphicsDevice::commitChangesGLResource() ����)
    void commitChangesGLResource();

private:

    static int _setVariableSemantics( lua_State* L );
    static int _addVariableAnnotationBool( lua_State* L );
    static int _addVariableAnnotationInt( lua_State* L );
    static int _addVariableAnnotationFloat( lua_State* L );
    static int _addVariableAnnotationVector( lua_State* L );
    static int _addVariableAnnotationString( lua_State* L );

    static int _addTechniqueAnnotationBool( lua_State* L );
    static int _addTechniqueAnnotationInt( lua_State* L );
    static int _addTechniqueAnnotationFloat( lua_State* L );
    static int _addTechniqueAnnotationVector( lua_State* L );
    static int _addTechniqueAnnotationString( lua_State* L );

    static int _addPassAnnotationBool( lua_State* L );
    static int _addPassAnnotationInt( lua_State* L );
    static int _addPassAnnotationFloat( lua_State* L );
    static int _addPassAnnotationVector( lua_State* L );
    static int _addPassAnnotationString( lua_State* L );

private:

    typedef std::list< Shader* >    ShaderList;

private:

    Base::LogFile*          mLogFile;
    OpenGL::GraphicsDevice* mGraphicsDevice;
    ShaderList              mShaderList;        ///< �L�[�������Ȃ����̂� onLostDevice() �̂��߂ɗp�ӁB�ꉞ�L�[�������̂����ׂĎ����Ă���
    lua_State*              mLuaState;
    
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace OpenGL
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================