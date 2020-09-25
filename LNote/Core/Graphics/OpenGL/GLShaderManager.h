//=============================================================================
//【 ShaderManager 】
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
// ■ ShaderManager クラス
//-----------------------------------------------------------------------------
///**
//  @brief      シェーダプログラムの管理クラス
//*/
//=============================================================================
class ShaderManager
    : public Base::ReferenceObject
    , public ShaderCache
{
public:

    ///< initialize() に渡す構造体
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

    /// 初期化
    void initialize( const InitData& init_data_ );

    /// 終了処理
    void finalize();

    /// シェーダの作成
    virtual IShader* createShader( const lnChar* filename_, LSharingKey key_ );

    /// シェーダプログラムのキャッシュのクリア
    virtual void clearCache() { ShaderCache::clear(); }

public:

    /// スクリプト実行
    void doScript( const char* script_, Shader* caller_ );

    /// GraphicsDevice の取得
    GraphicsDevice* getGraphicsDevice() { return mGraphicsDevice; }

    /// File::Manager の取得
    File::Manager* getFileManager() { return mGraphicsDevice->getFileManager(); }

    /// 一時バッファの取得
    Base::TempBuffer& getTempBuffer() { return mGraphicsDevice->getTempBuffer(); }

    /// Shader のデストラクタから呼ばれる
    void removeShader( Shader* shader_ ) { mShaderList.remove( shader_ ); }

    /// デバイスリセット直前に呼ぶ
    void onLostDevice();

	/// デバイスリセット直後に呼ぶ
    void onResetDevice();

    /// (GraphicsDevice::commitChangesGLResource() から)
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
    ShaderList              mShaderList;        ///< キーを持たないものの onLostDevice() のために用意。一応キーを持つものもすべて持っている
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