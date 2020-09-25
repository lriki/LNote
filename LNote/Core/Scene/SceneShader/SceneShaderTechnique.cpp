//=============================================================================
//【 SceneShaderTechnique 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "SceneShaderManager.h"
#include "SceneShader.h"
#include "SceneShaderTechnique.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

#define LN_SET_SHADER_ERR_MSG( fmt_, ... ) \
{ \
    LRefTString err; \
    err.format( fmt_, __VA_ARGS__ ); \
    mSceneShader->setErrorMessage( err ); \
}

//=============================================================================
// ■ SceneShaderTechnique クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    SceneShaderTechnique::SceneShaderTechnique()
        : mSceneShader              ( NULL )
        , mCoreShader               ( NULL )
        , mTechnique                ( NULL )
        , mMMDPass                  ( MMD_PASS_object )
        , mFlags                    ( MME_TECHSTATE_ALL )
        , mCommandExistFlags        ( 0 )
        , mNextClearColor           ( 0x00000000 )
        , mNextClearDepth           ( 1.0f )
    {
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    SceneShaderTechnique::~SceneShaderTechnique()
    {
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT SceneShaderTechnique::initialize( SceneShader* shader_, Graphics::IShaderTechnique* tech_ )
    {
        mSceneShader    = shader_;
        mCoreShader     = mSceneShader->getCoreShader();
        mTechnique      = tech_;
        mMMDPass        = MMD_PASS_object; // デフォルトは "object"

        //-------------------------------------------
        // MMD パスとアノテーションを調べる

        const lnChar* mmd_pass = NULL;
        bool use_texture = false;
        bool use_sphere_map = false;
        bool use_toon = false;
        Graphics::IShaderVariable* anno = NULL;

        anno = tech_->getAnnotationByName( _T( "MMDPass" ) );
        if ( anno )
        {
            anno->getString( &mmd_pass );
            if ( mmd_pass )
            {
                if      ( _tcsicmp( mmd_pass, "object" ) )    { 
	mMMDPass = MMD_PASS_object; }
                else if ( _tcsicmp( mmd_pass, "zplot" ) )     { 
                	mMMDPass = MMD_PASS_zplot; }
                else if ( _tcsicmp( mmd_pass, "object_ss" ) ) { 
                	mMMDPass = MMD_PASS_object_ss; }
                else if ( _tcsicmp( mmd_pass, "shadow" ) )    { 
                	mMMDPass = MMD_PASS_shadow; }
                else if ( _tcsicmp( mmd_pass, "edge" ) )      { 
                	mMMDPass = MMD_PASS_edge; }
            }
        }

        anno = tech_->getAnnotationByName( _T( "UseTexture" ) );
        if ( anno ) { 
        	anno->getBool( &use_texture ); } else { 
	mFlags |= MME_TECHSTATE_OMITTED_Texture; }

        anno = tech_->getAnnotationByName( _T( "UseSphereMap" ) );
        if ( anno ) { 
        	anno->getBool( &use_sphere_map ); } else { 
	mFlags |= MME_TECHSTATE_OMITTED_SphereMap; }

        anno = tech_->getAnnotationByName( _T( "UseToon" ) );
        if ( anno ) { 
        	anno->getBool( &use_toon ); } else { 
	mFlags |= MME_TECHSTATE_OMITTED_Toon; }

        mFlags |=
            ( use_texture )     ? MME_TECHSTATE_UseTexture : 0 ||
            ( use_sphere_map )  ? MME_TECHSTATE_UseSphereMap : 0 ||
            ( use_toon )        ? MME_TECHSTATE_UseToon : 0;

        //-------------------------------------------
        // 描画するSubset番号の指定を調べる

        const lnChar* subset_text = NULL;
        mSubsetIndexArray.clear();

        anno = tech_->getAnnotationByName( _T( "Subset" ) );
        if ( anno ) { 
        	anno->getString( &subset_text ); }

        if ( subset_text )
        {
            SceneShaderUtil::makeSubsetIndexArray( &mSubsetIndexArray, subset_text );
        }
        
        //-------------------------------------------
        // Script

        mScriptExternalIt = mScriptCommandList.end();

        // "Script" アノテーションがあるかチェック 
        Graphics::IShaderVariable* sc_var;
        sc_var = mTechnique->getAnnotationByName( _T( "Script" ) );
        if ( sc_var )
        {
            // 文字列型の場合はスクリプト解析に回す
            if ( sc_var->getTypeDesc().Type == LN_SVT_STRING )
            {
                _lexScriptCommandString( sc_var, false );

                // ScriptExternal=Color を探す
                //      _lexScriptCommandString() 内でコマンドを見つけたときにそのイテレータを記憶しても、
                //      その後のコマンド追加で配列のサイズが変わった瞬間にアウトになるので、
                //      コマンドリストを作り終わった後にチェックする。
                if ( mCommandExistFlags & MME_SCEXIST_ScriptExternal_Color )
                {
                    mScriptExternalIt = mScriptCommandList.begin();
                    MMEScriptCommandList::iterator end = mScriptCommandList.end();
                    for ( ; mScriptExternalIt != end; ++mScriptExternalIt )
                    {
                        if ( mScriptExternalIt->Type == MME_SCRIPTCOM_ScriptExternal_Color )
                        {
                            break;
                        }
                    }
                }
            }
        }
        // 省略されている場合はパスの分だけの処理
        else
        {
            u32 i = 0;
            Graphics::IShaderPass* pass;
            
            while ( true )
            {
                pass = mTechnique->getPassByIndex( i );
                if ( pass )
                {
                    // パス開始
                    MMEScriptCommand com;
                    com.Type = MME_SCRIPTCOM_BeginPass;
                    com.Pass = pass;
                    mScriptCommandList.push_back( com );

                    // パス内のスクリプト解析
                    sc_var = pass->getAnnotationByName( _T( "Script" ) );
                    _lexScriptCommandString( sc_var, pass );

                    // パス終了
                    com.Type = MME_SCRIPTCOM_EndPass;
                    com.Pass = pass;
                    mScriptCommandList.push_back( com );
                }
                else
                {
                    break;
                }
                ++i;
            }
        }

        //-------------------------------------------
        // 中身チェック
#if 0
        printf( "◆ Script\n" );
        MMEScriptCommandList::iterator it = mScriptCommandList.begin();
        for ( ; it != mScriptCommandList.end(); ++it )
        {
            MMEScriptCommand* c = &(*it);
            switch ( c->Type )
            {
                case MME_SCRIPTCOM_RenderColorTarget:           
                	printf( "RenderColorTarget index:%d var:%p\n", c->Index, c->Variable ); break;
                case MME_SCRIPTCOM_RenderDepthStencilTarget:    
                	printf( "RenderDepthStencilTarget\n" ); break;
                case MME_SCRIPTCOM_ClearSetColor:               
                	printf( "ClearSetColor\n" ); break;
                case MME_SCRIPTCOM_ClearSetDepth:               
                	printf( "ClearSetDepth\n" ); break;
                case MME_SCRIPTCOM_Clear_Color:                 
                	printf( "Clear_Color        %X\n", (it)->Color ); break;
                case MME_SCRIPTCOM_Clear_Depth:                 
                	printf( "Clear_Depth        %f\n", (it)->Depth ); break;
                case MME_SCRIPTCOM_ScriptExternal_Color:        
                	printf( "ScriptExternal_Color\n" ); break;
                case MME_SCRIPTCOM_Pass:                        
                	printf( "Pass\n" ); break;
                case MME_SCRIPTCOM_BeginPass:                   
                	printf( "BeginPass\n" ); break;
                case MME_SCRIPTCOM_EndPass:                     
                	printf( "EndPass\n" ); break;
                case MME_SCRIPTCOM_LoopByCount:                 
                	printf( "LoopByCount\n" ); break;
                case MME_SCRIPTCOM_LoopEnd:                     
                	printf( "LoopEnd\n" ); break;
                case MME_SCRIPTCOM_LoopGetIndex:                
                	printf( "LoopGetIndex\n" ); break;
                case MME_SCRIPTCOM_Draw_Geometry:               
                	printf( "Draw_Geometry\n" ); break;
                case MME_SCRIPTCOM_Draw_Buffer:                 
                	printf( "Draw_Buffer\n" ); break;
                case MME_SCRIPTCOM_RenderPort:                  
                	printf( "RenderPort\n" ); break;
            }  
        }
#endif
        
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 描画コマンドの作成
    //---------------------------------------------------------------------
    void SceneShaderTechnique::makeDrawCommand( DrawCommandContainer* container_, SceneNode* object_ )
    {
        // コマンド作成
        MMEScriptCommandList::iterator  it = mScriptCommandList.begin();
        MMEScriptCommandList::iterator end = mScriptCommandList.end();
        for ( ; it != end; ++it )
        {
            _makeDrawCommand( container_, &(*it), object_ );
        }
    }

    //---------------------------------------------------------------------
    // ● 描画コマンドの作成 ("ScriptExternal=Color" の前まで)
    //---------------------------------------------------------------------
    void SceneShaderTechnique::makeDrawCommandPreExternal( DrawCommandContainer* container_, SceneNode* object_ )
    {
        // "ScriptExternal=Color" 開始
        container_->add_PreExternalBegin();

        // コマンド作成
        MMEScriptCommandList::iterator  it = mScriptCommandList.begin();
        MMEScriptCommandList::iterator end = mScriptExternalIt;
        for ( ; it != end; ++it )
        {
            _makeDrawCommand( container_, &(*it), object_ );
        }

        // "ScriptExternal=Color" まで終了
        container_->add_PreExternalEnd();
    }

    //---------------------------------------------------------------------
    // ● 描画コマンドの作成 ("ScriptExternal=Color" から終端まで)
    //---------------------------------------------------------------------
    void SceneShaderTechnique::makeDrawCommandPostExternal( DrawCommandContainer* container_, SceneNode* object_ )
    {
        // "ScriptExternal=Color" から終端までのブロックの開始
        container_->add_PostExternalBegin();

        // コマンド作成
        MMEScriptCommandList::iterator  it = mScriptExternalIt;
        MMEScriptCommandList::iterator end = mScriptCommandList.end();
        for ( ; it != end; ++it )
        {
            _makeDrawCommand( container_, &(*it), object_ );
        }

        // 全体終了
        container_->add_PostExternalEnd();
    }

    //---------------------------------------------------------------------
    // ● スクリプト文字列の変数を受け取って mScriptCommandList にコマンドを追加していく
    //---------------------------------------------------------------------
    LNRESULT SceneShaderTechnique::_lexScriptCommandString( Graphics::IShaderVariable* var_, Graphics::IShaderPass* pass_ )
    {
        LNRESULT lr;
        const lnChar* script = NULL;
        lnChar* str = NULL;
        char* s;
        char* tp;
        Graphics::IShaderVariable* tv;

        if ( var_  )
        {
            var_->getString( &script );

            if ( script )
            {
                // 区切り文字(;)を終端文字に置換して処理をするため、一度全部コピーする
                str = Base::StringUtil::copyLength( script, -1 );

                s = tp = str;

                while ( tp != NULL )
                {
                    // ; を探して '\0' に置換。無ければ終端ということで終了
                    tp = strpbrk( s, _T( ";" ) );
                    if ( tp )
                    {
                        *tp = '\0';
                    }
                    else
                    {
                        break;
                    }

                    // 置換した '\0' まで、(文字列の部分一致でだけど) キーワードを探していく
                    switch ( *s )
                    {
                        /////////////////////////////
                        // RenderColorTarget_0～3
                        // RenderDepthStencilTarget
                        // RenderPort
                        case 'R':
                        {
                            ///////////////////////////// RenderColorTarget_0～3
                            if ( *( s + 6 ) == 'C' )
                            {
                                MMEScriptCommand com;
                                com.Type = MME_SCRIPTCOM_RenderColorTarget;

                                // 設定インデックスを調べる
                                char lc = *( s + 17 );
                                if ( '0' <= lc && lc <= '3' )
                                {
                                    com.Index = lc - '0';
                                }
                                else
                                {
                                    com.Index = 0;
                                }

                                // テクスチャ変数からテクスチャをとってくる
                                char* name = strpbrk( s, "=" ) + 1;
                                if ( *name == '\0' )
                                {
                                    com.Variable = NULL;
                                    mScriptCommandList.push_back( com );
                                }
                                else
                                {
                                    com.Variable = mSceneShader->findShaderVariable( name );

                                    // 変数が RT を要求しているかチェック
                                    if ( com.Variable && ( com.Variable->Request == MME_VARREQ_RENDERCOLORTARGET || com.Variable->Request == MME_VARREQ_OFFSCREENRENDERTARGET ) )
                                    {
                                        mScriptCommandList.push_back( com );
                                    }
                                    else
                                    {
                                        LN_SET_SHADER_ERR_MSG(
                                            _T( "RenderColorTarget コマンドのテクスチャ名が無効です。\n%s" ),
                                            name );
                                        return LN_ERR_INVALIDCALL;
                                    }
                                }
                            }
                            ///////////////////////////// RenderDepthStencilTarget
                            else if ( *( s + 6 ) == 'D' )
                            {
                                MMEScriptCommand com;
                                com.Type = MME_SCRIPTCOM_RenderDepthStencilTarget;

                                // テクスチャ変数からテクスチャをとってくる
                                char* name = strpbrk( s, "=" ) + 1;
                                if ( *name == '\0' )
                                {
                                    com.Variable = NULL;
                                    mScriptCommandList.push_back( com );
                                }
                                else
                                {
                                    com.Variable = mSceneShader->findShaderVariable( name );

                                    // 変数が深度バッファを要求しているかチェック
                                    if ( com.Variable->Request == MME_VARREQ_RENDERDEPTHSTENCILTARGET )
                                    {
                                        mScriptCommandList.push_back( com );
                                    }
                                    else
                                    {
                                        LN_SET_SHADER_ERR_MSG(
                                            _T( "RenderDepthStencilTarget コマンドのテクスチャ名が無効です。\n%s" ),
                                            name );
                                        return LN_ERR_INVALIDCALL;
                                    }
                                }
                            }
                            ///////////////////////////// RenderPort
                            else if ( *( s + 6 ) == 'P' )
                            {
                                // MME 自体も未実装のコマンド
                            }
                            break;
                        }

                        /////////////////////////////
                        // ClearSetColor
                        // ClearSetDepth
                        // Clear_Color
                        // Clear_Depth
                        case 'C':
                        {
                            MMEScriptCommand com;

                            ///////////////////////////// ClearSetColor
                            if ( *( s + 8 ) == 'C' )
                            {
                                com.Type = MME_SCRIPTCOM_ClearSetColor;

                                lnChar* name = strpbrk( s, "=" ) + 1;
                                com.CoreVariable = mCoreShader->getVariableByName( name );
                                
                                /*
                                LColor color;
                                tv = mCoreShader->getVariableByName( name );
                                if ( !tv )
                                {
                                    LN_SET_SHADER_ERR_MSG(
                                        _T( "ClearSetColor コマンドのパラメータ名が無効です。\n%s" ),
                                        name );
                                        return LN_ERR_INVALIDCALL;
                                }

                                tv->getVector( (LVector4*)&color );
                                com.Color = color.to32Bit();
                                */
                            }
                            ///////////////////////////// ClearSetDepth
                            else if ( *( s + 8 ) == 'D' )
                            {
                                com.Type = MME_SCRIPTCOM_ClearSetDepth;

                                lnChar* name = strpbrk( s, "=" ) + 1;
                                com.CoreVariable = mCoreShader->getVariableByName( name );
                                /*
                                lnfloat depth;
                                tv = mCoreShader->getVariableByName( name );
                                if ( !tv )
                                {
                                    LN_SET_SHADER_ERR_MSG(
                                        _T( "ClearSetDepth コマンドのパラメータ名が無効です。\n%s" ),
                                        name );
                                        return LN_ERR_INVALIDCALL;
                                }
                                    
                                tv->getFloat( &depth );
                                com.Depth = depth;
                                */
                            }
                            ///////////////////////////// Clear_Color
                            else if ( *( s + 6 ) == 'C' )
                            {
                                com.Type = MME_SCRIPTCOM_Clear_Color;
                            }
                            ///////////////////////////// Clear_Depth
                            else if ( *( s + 6 ) == 'D' )
                            {
                                com.Type = MME_SCRIPTCOM_Clear_Depth;
                            }

                            mScriptCommandList.push_back( com );
                            
                            break;
                        }

                        /////////////////////////////
                        // ScriptExternal_Color
                        case 'S':
                        {
                            MMEScriptCommand com;
                            com.Type = MME_SCRIPTCOM_ScriptExternal_Color;
                            mScriptCommandList.push_back( com );

                            mCommandExistFlags |= MME_SCEXIST_ScriptExternal_Color;
                            break;
                        }

                        /////////////////////////////
                        // Pass
                        case 'P':
                        {
                            char* name = strpbrk( s, "=" ) + 1;
                            Graphics::IShaderPass* pass = mTechnique->getPassByName( name );
                            if ( pass )
                            {
                                // パス開始
                                //MMEScriptCommand com;
                                //com.Type = MME_SCRIPTCOM_BeginPass;
                                //com.Pass = pass;
                                //mScriptCommandList.push_back( com );

                                // パス内にさらに Script がある場合は再起 (ない場合は Draw_Geometry が設定される)
                                Graphics::IShaderVariable* var = pass->getAnnotationByName( "Script" );
                                LN_CALL_R( _lexScriptCommandString( var, pass ) );

                                // パス終了
                                //com.Type = MME_SCRIPTCOM_EndPass;
                                //com.Pass = pass;
                                //mScriptCommandList.push_back( com );
                            }
                            else
                            {
                                LN_SET_SHADER_ERR_MSG(
                                    _T( "Pass コマンドのパス名が無効です。\n%s" ),
                                    name );
                                    return LN_ERR_INVALIDCALL;
                            }
                            break;
                        }

                        /////////////////////////////
                        // LoopByCount
                        // LoopEnd
                        // LoopGetIndex
                        case 'L':
                        {
                            MMEScriptCommand com;

                            ///////////////////////////// LoopByCount
                            if ( *( s + 4 ) == 'B' )
                            {
                                com.Type = MME_SCRIPTCOM_LoopByCount;

                                char* name = strpbrk( s, "=" ) + 1;
                                tv = mCoreShader->getVariableByName( name );
                                if ( !tv )
                                {
                                    LN_SET_SHADER_ERR_MSG(
                                        _T( "LoopByCount コマンドのパラメータ名が無効です。\n%s" ),
                                        name );
                                        return LN_ERR_INVALIDCALL;
                                }
                                tv->getInt( &com.LoopCount );
                            }
                            ///////////////////////////// LoopEnd
                            else if ( *( s + 4 ) == 'E' )
                            {
                                com.Type = MME_SCRIPTCOM_LoopEnd;
                            }
                            ///////////////////////////// LoopGetIndex
                            else if ( *( s + 4 ) == 'G' )
                            {
                                com.Type = MME_SCRIPTCOM_LoopGetIndex;

                                char* name = strpbrk( s, "=" ) + 1;
                                com.LoopVar = mCoreShader->getVariableByName( name );

                                if ( !com.LoopVar )
                                {
                                    LN_SET_SHADER_ERR_MSG(
                                        _T( "LoopGetIndex コマンドのパラメータ名が無効です。\n%s" ),
                                        name );
                                        return LN_ERR_INVALIDCALL;
                                }
                            }

                            mScriptCommandList.push_back( com );
                            break;
                        }

                        /////////////////////////////
                        // Draw_Geometry
                        // Draw_Buffer
                        case 'D':
                        {
                            MMEScriptCommand com;
                            ///////////////////////////// Draw_Geometry
                            if ( *( s + 5 ) == 'B' )
                            {
                                com.Type = MME_SCRIPTCOM_Draw_Buffer;
                                com.Pass = pass_;
                                mCommandExistFlags |= MME_SCEXIST_Draw_Buffer;
                            }
                            ///////////////////////////// Draw_Buffer
                            else
                            {
                                com.Type = MME_SCRIPTCOM_Draw_Geometry;
                                com.Pass = pass_;
                                mCommandExistFlags |= MME_SCEXIST_Draw_Geometry;
                            }
                            mScriptCommandList.push_back( com );
                            break;
                        }
                    }

                    s = tp + 1; // ; の次を指す
                }

                SAFE_DELETE_ARRAY( str );
            }
        }

        // パスの処理中で、スクリプトが無効だった場合は Draw_Geometry ひとつだけを詰める
        if ( !script && pass_ )
        {
            MMEScriptCommand com;
            com.Type = MME_SCRIPTCOM_Draw_Geometry;
            com.Pass = pass_;
            mScriptCommandList.push_back( com );
        }

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● ひとつ分の描画コマンドを作って container_ に追加
    //---------------------------------------------------------------------
    LNRESULT SceneShaderTechnique::_makeDrawCommand( DrawCommandContainer* container_, MMEScriptCommand* command_, SceneNode* object_ )
    {
        

        switch ( command_->Type )
        {
            case MME_SCRIPTCOM_RenderColorTarget:
                if ( command_->Variable && command_->Variable->Texture )
                {
                    container_->add_SetRT( command_->Index, command_->Variable->Texture, false );
                }
                else
                {
                    container_->add_SetRT( command_->Index, NULL, false );
                }
                break;
           
            case MME_SCRIPTCOM_RenderDepthStencilTarget:
                if ( command_->Variable && command_->Variable->Texture )
                {
                    container_->add_SetDepth( command_->Variable->Texture, false );
                }
                else
                {
                    container_->add_SetDepth( NULL, false );
                }
                break;

            case MME_SCRIPTCOM_ClearSetColor:
            {
                //mNextClearColor = command_->Color.to32Bit();
                LColor c;
                command_->CoreVariable->getVector( (LVector4*)&c );
                mNextClearColor = c.to32Bit( this->mSceneShader->getSceneShaderManager()->getGraphicsManager()->getGraphicsAPI() );
                break;
            }

            case MME_SCRIPTCOM_ClearSetDepth:
                //mNextClearDepth = command_->Depth;
                command_->CoreVariable->getFloat( &mNextClearDepth );
                break;

            case MME_SCRIPTCOM_Clear_Color:
                container_->add_ClearColor( mNextClearColor );
                break;

            case MME_SCRIPTCOM_Clear_Depth:
                container_->add_ClearDepth( mNextClearDepth );
                break;

            //case MME_SCRIPTCOM_ScriptExternal_Color:

            //case MME_SCRIPTCOM_Pass:

            //case MME_SCRIPTCOM_BeginPass:
            //    container_->add_BeginPass( command_->Pass );
            //    break;

            //case MME_SCRIPTCOM_EndPass:
            //    container_->add_EndPass( command_->Pass );
            //    break;

            case MME_SCRIPTCOM_LoopByCount:
                container_->add_LoopBegin( command_->LoopCount );
                break;
            case MME_SCRIPTCOM_LoopEnd:
                container_->add_LoopEnd();
                break;
            case MME_SCRIPTCOM_LoopGetIndex:
                container_->add_SetLoopCount( command_->LoopVar );
                break;

            /////////////////////////////////////////////////// Draw=Geometry
            case MME_SCRIPTCOM_Draw_Geometry:
            {
                container_->add_DrawGeometry( object_, &mSubsetIndexArray, command_->Pass );
                
                /*
                u32 pass_flags = object_->getExtraPasses();

                // 面を描画する
                if ( pass_flags & LN_EXPASS_BASIC )
                {
                    container_->add_SetRenderState( object_->getRenderState() );
                    _makeDrawSubsetCommand( container_, object_, LN_OBJDRAWPASS_SOLID );
                }

                // ワイヤーフレームを描画する
                if ( pass_flags & LN_EXPASS_WIRE )
                {
                    container_->add_SetRenderStateByPass( object_, LN_OBJDRAWPASS_WIRE );
                    _makeDrawSubsetCommand( container_, object_, LN_OBJDRAWPASS_WIRE );
                }

                // 頂点を描画する
                if ( pass_flags & LN_EXPASS_POINT )
                {
                    container_->add_SetRenderStateByPass( object_, LN_OBJDRAWPASS_POINT );
                    _makeDrawSubsetCommand( container_, object_, LN_OBJDRAWPASS_POINT );
                }
                */
                break;
            }

            /////////////////////////////////////////////////// Draw=Buffer
            case MME_SCRIPTCOM_Draw_Buffer:
                container_->add_DrawBuffer( command_->Pass );
                break;

            //case MME_SCRIPTCOM_RenderPort:
        }

        return LN_OK;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================