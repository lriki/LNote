//=============================================================================
//【 ShaderManager 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <lua/lua.hpp>
#include "../../File/Manager.h"
#include "GLShader.h"
#include "GLShaderManager.h"

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

//=============================================================================
// ■ ShaderManager クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    ShaderManager::ShaderManager()
        : mLogFile              ( NULL )
        , mGraphicsDevice       ( NULL )
        , mLuaState             ( NULL )
    {
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    ShaderManager::~ShaderManager()
    {
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    void ShaderManager::initialize( const InitData& init_data_ )
    {
        LN_LOG_INIT_BEGIN;

        mGraphicsDevice = init_data_.GraphicsDevice;

        ShaderCache::initialize( init_data_.ShaderCacheSize );

        mLuaState = luaL_newstate();
        luaL_openlibs( mLuaState );

        // Lua 関数登録
        lua_register( mLuaState, "setSemantics", _setVariableSemantics );
        lua_register( mLuaState, "addVariableAnnotationBool", _addVariableAnnotationBool );
        lua_register( mLuaState, "addVariableAnnotationInt", _addVariableAnnotationInt );
        lua_register( mLuaState, "addVariableAnnotationFloat", _addVariableAnnotationFloat );
        lua_register( mLuaState, "addVariableAnnotationVector", _addVariableAnnotationVector );
        lua_register( mLuaState, "addVariableAnnotationString", _addVariableAnnotationString );

        lua_register( mLuaState, "addTechniqueAnnotationBool", _addTechniqueAnnotationBool );
        lua_register( mLuaState, "addTechniqueAnnotationInt", _addTechniqueAnnotationInt );
        lua_register( mLuaState, "addTechniqueAnnotationFloat", _addTechniqueAnnotationFloat );
        lua_register( mLuaState, "addTechniqueAnnotationVector", _addTechniqueAnnotationVector );
        lua_register( mLuaState, "addTechniqueAnnotationString", _addTechniqueAnnotationString );

        lua_register( mLuaState, "addPassAnnotationBool", _addPassAnnotationBool );
        lua_register( mLuaState, "addPassAnnotationInt", _addPassAnnotationInt );
        lua_register( mLuaState, "addPassAnnotationFloat", _addPassAnnotationFloat );
        lua_register( mLuaState, "addPassAnnotationVector", _addPassAnnotationVector );
        lua_register( mLuaState, "addPassAnnotationString", _addPassAnnotationString );

        LN_LOG_INIT_END;
    }

    //---------------------------------------------------------------------
    // ● 終了処理
    //---------------------------------------------------------------------
    void ShaderManager::finalize()
    {
        LN_LOG_FIN_BEGIN;

        lua_close( mLuaState );

        ShaderCache::finalize();

        LN_LOG_FIN_END;
    }

    //---------------------------------------------------------------------
    // ● シェーダの作成 ( 入力ストリームから )
    //---------------------------------------------------------------------
    IShader* ShaderManager::createShader( const lnChar* filename_, LSharingKey key_ )
    {
        // キャッシュを検索
        Shader* shader = ShaderCache::findCacheAddRef( key_ );
        if ( shader )
        {
            return shader;
        }

        // Shader 作成
        Base::FinallyReleasePtr< Shader > shader_ptr( LN_NEW Shader( this ) );
        shader_ptr->initialize( filename_ );

        // キャッシュ管理に追加
        ShaderCache::registerCachedObject( key_, shader_ptr );

        mShaderList.push_back( shader_ptr );

        return shader_ptr.returnObject();
    }
 
    //---------------------------------------------------------------------
    // ● スクリプト実行
    //---------------------------------------------------------------------
    void ShaderManager::doScript( const char* script_, Shader* caller_ )
    {
        // caller_ を グローバル変数としてluaに格納
        lua_pushlightuserdata( mLuaState, (void*)caller_ );
        lua_setglobal( mLuaState, "caller_ptr" );

        // 実行
        int result = luaL_dostring( mLuaState, script_ );
        if ( result )
        {
            LN_THROW_InvalidCall( 0, "Lua result:%d\n%s", result, lua_tostring( mLuaState, lua_gettop( mLuaState ) ) );
        }

        // lua スタッククリア
        if ( lua_gettop( mLuaState ) )
        {
            lua_pop( mLuaState, lua_gettop( mLuaState ) );
        }
    }

    //---------------------------------------------------------------------
    // ● デバイスリセット直前に呼ぶ
    //---------------------------------------------------------------------
    void ShaderManager::onLostDevice()
    {
        ln_foreach( Shader* shader, mShaderList )
        {
            shader->onLostDevice();
        }
    }

	//---------------------------------------------------------------------
    // ● デバイスリセット直後に呼ぶ
    //---------------------------------------------------------------------
    void ShaderManager::onResetDevice()
    {
        ln_foreach( Shader* shader, mShaderList )
        {
            shader->onResetDevice();
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ShaderManager::commitChangesGLResource()
    {
        //ln_foreach( Shader* shader, mShaderList )
        //{
        //    shader->commitChanges();
        //}
    }


   
    struct SMDLuaArgs
    {
        Shader*             Caller;     ///< 対象のシェーダ
        const lnChar*       VarName;    ///< 対象の変数名
        const lnChar*       Arg1;       ///< セマンティクスまたはアノテーション名
        ShaderVariable*     Variable;   ///< 対象の変数 (見つからない場合は NULL)
        ShaderTechnique*    Technique;  ///< 対象のテクニック (見つからない場合は NULL)
        ShaderPass*         Pass;       ///< 対象のパス (見つからない場合は NULL)
    };
    static bool getSMDLuaArgs_Variable( lua_State* L, SMDLuaArgs* args_ )
    {
        lua_getglobal( L, "caller_ptr" );
        args_->Caller    = (Shader*)lua_touserdata( L, lua_gettop( L ) );
        args_->VarName   = lua_tostring( L, 1 );
        args_->Arg1      = lua_tostring( L, 2 );
        args_->Variable  = (ShaderVariable*)args_->Caller->getVariableByName( args_->VarName );
        args_->Technique = NULL;
        args_->Pass      = NULL;
        return ( args_->Variable != NULL );
    }
    static bool getSMDLuaArgs_Technique( lua_State* L, SMDLuaArgs* args_ )
    {
        lua_getglobal( L, "caller_ptr" );
        args_->Caller    = (Shader*)lua_touserdata( L, lua_gettop( L ) );
        args_->VarName   = lua_tostring( L, 1 );
        args_->Arg1      = lua_tostring( L, 2 );
        args_->Variable  = NULL;
        args_->Technique = (ShaderTechnique*)args_->Caller->getTechniqueByName( args_->VarName );
        args_->Pass      = NULL;
        return ( args_->Technique != NULL );
    }
    static bool getSMDLuaArgs_Pass( lua_State* L, SMDLuaArgs* args_ )
    {
        lua_getglobal( L, "caller_ptr" );
        args_->Caller    = (Shader*)lua_touserdata( L, lua_gettop( L ) );
        args_->VarName   = lua_tostring( L, 1 );
        args_->Arg1      = lua_tostring( L, 2 );
        args_->Variable  = NULL;
        args_->Technique = NULL;
        // TODO:テクニック名検索
        args_->Pass      = (ShaderPass*)args_->Caller->getTechniqueByIndex( 0 )->getPassByName( args_->VarName );
        return ( args_->Pass != NULL );
    }


    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    int ShaderManager::_setVariableSemantics( lua_State* L )
    {
        SMDLuaArgs args;
        if ( !getSMDLuaArgs_Variable( L, &args ) ) return 0;

        args.Variable->setSemanticName( args.Arg1 );
        return 0;
    }

    int ShaderManager::_addVariableAnnotationBool( lua_State* L )
    {
        SMDLuaArgs args;
        if ( !getSMDLuaArgs_Variable( L, &args ) ) return 0;

        int value = (int)lua_tonumber( L, 3 );
        args.Variable->addAnnotationBool( args.Arg1, value != 0 );
        return 0;
    }

    int ShaderManager::_addVariableAnnotationInt( lua_State* L )
    {
        SMDLuaArgs args;
        if ( !getSMDLuaArgs_Variable( L, &args ) ) return 0;

        int value = (int)lua_tonumber( L, 3 );
        args.Variable->addAnnotationInt( args.Arg1, value );
        return 0;
    }

    int ShaderManager::_addVariableAnnotationFloat( lua_State* L )
    {
        SMDLuaArgs args;
        if ( !getSMDLuaArgs_Variable( L, &args ) ) return 0;

        float value = (float)lua_tonumber( L, 3 );
        args.Variable->addAnnotationFloat( args.Arg1, value );
        return 0;
    }

    int ShaderManager::_addVariableAnnotationVector( lua_State* L )
    {
        SMDLuaArgs args;
        if ( !getSMDLuaArgs_Variable( L, &args ) ) return 0;

        float x = (float)lua_tonumber( L, 3 );
        float y = (float)lua_tonumber( L, 4 );
        float z = (float)lua_tonumber( L, 5 );
        float w = (float)lua_tonumber( L, 6 );
        args.Variable->addAnnotationVector( args.Arg1, x, y, z, w );
        return 0;
    }

    int ShaderManager::_addVariableAnnotationString( lua_State* L )
    {
        SMDLuaArgs args;
        if ( !getSMDLuaArgs_Variable( L, &args ) ) return 0;

        const char* value = lua_tostring( L, 3 );
        args.Variable->addAnnotationString( args.Arg1, value );
        return 0;
    }

    // Technique
    int ShaderManager::_addTechniqueAnnotationBool( lua_State* L )
    {
        SMDLuaArgs args;
        if ( !getSMDLuaArgs_Technique( L, &args ) ) return 0;

        int value = (int)lua_tonumber( L, 3 );
        args.Technique->addAnnotationBool( args.Arg1, value != 0 );
        return 0;
    }
    int ShaderManager::_addTechniqueAnnotationInt( lua_State* L )
    {
        SMDLuaArgs args;
        if ( !getSMDLuaArgs_Technique( L, &args ) ) return 0;

        int value = (int)lua_tonumber( L, 3 );
        args.Technique->addAnnotationInt( args.Arg1, value );
        return 0;
    }
    int ShaderManager::_addTechniqueAnnotationFloat( lua_State* L )
    {
        SMDLuaArgs args;
        if ( !getSMDLuaArgs_Technique( L, &args ) ) return 0;

        float value = (float)lua_tonumber( L, 3 );
        args.Technique->addAnnotationFloat( args.Arg1, value );
        return 0;
    }
    int ShaderManager::_addTechniqueAnnotationVector( lua_State* L )
    {
        SMDLuaArgs args;
        if ( !getSMDLuaArgs_Technique( L, &args ) ) return 0;

        float x = (float)lua_tonumber( L, 3 );
        float y = (float)lua_tonumber( L, 4 );
        float z = (float)lua_tonumber( L, 5 );
        float w = (float)lua_tonumber( L, 6 );
        args.Technique->addAnnotationVector( args.Arg1, x, y, z, w );
        return 0;
    }
    int ShaderManager::_addTechniqueAnnotationString( lua_State* L )
    {
        SMDLuaArgs args;
        if ( !getSMDLuaArgs_Technique( L, &args ) ) return 0;

        const char* value = lua_tostring( L, 3 );
        args.Technique->addAnnotationString( args.Arg1, value );
        return 0;
    }

    // Pass
    int ShaderManager::_addPassAnnotationBool( lua_State* L )
    {
        SMDLuaArgs args;
        if ( !getSMDLuaArgs_Pass( L, &args ) ) return 0;

        int value = (int)lua_tonumber( L, 3 );
        args.Pass->addAnnotationBool( args.Arg1, value != 0 );
        return 0;
    }
    int ShaderManager::_addPassAnnotationInt( lua_State* L )
    {
        SMDLuaArgs args;
        if ( !getSMDLuaArgs_Pass( L, &args ) ) return 0;

        int value = (int)lua_tonumber( L, 3 );
        args.Pass->addAnnotationInt( args.Arg1, value );
        return 0;
    }
    int ShaderManager::_addPassAnnotationFloat( lua_State* L )
    {
        SMDLuaArgs args;
        if ( !getSMDLuaArgs_Pass( L, &args ) ) return 0;

        float value = (float)lua_tonumber( L, 3 );
        args.Pass->addAnnotationFloat( args.Arg1, value );
        return 0;
    }
    int ShaderManager::_addPassAnnotationVector( lua_State* L )
    {
        SMDLuaArgs args;
        if ( !getSMDLuaArgs_Pass( L, &args ) ) return 0;

        float x = (float)lua_tonumber( L, 3 );
        float y = (float)lua_tonumber( L, 4 );
        float z = (float)lua_tonumber( L, 5 );
        float w = (float)lua_tonumber( L, 6 );
        args.Pass->addAnnotationVector( args.Arg1, x, y, z, w );
        return 0;
    }
    int ShaderManager::_addPassAnnotationString( lua_State* L )
    {
        SMDLuaArgs args;
        if ( !getSMDLuaArgs_Pass( L, &args ) ) return 0;

        const char* value = lua_tostring( L, 3 );
        args.Pass->addAnnotationString( args.Arg1, value );
        return 0;
    }

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