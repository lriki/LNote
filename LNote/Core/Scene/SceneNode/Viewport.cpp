//=============================================================================
//【 Viewport 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Base/STLUtil.h"
#include "../../Resource/LNResource.h"
#include "../SceneGraph.h"
#include "../SceneShader/SceneShader.h"
#include "Viewport.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

//=============================================================================
// ■ Viewport クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    Viewport::Viewport( SceneGraph* scene_ )
        : SceneNode                 ( scene_ )
        
    {
        scene_->addViewport( this );
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    Viewport::~Viewport()
    {
        

        if ( this->mSceneGraph )
        {
            this->mSceneGraph->removeViewport( this );
        }
        //ln_foreach( PPShader& pps, mPPShaderArray )
        //{
        //    LN_SAFE_RELEASE( pps.Shader );
        //}
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT Viewport::initialize( const Geometry::Size& size_ )
    {
        SceneNode::initialize( 1, LN_DRAWCOORD_UNKNOWN );

        mViewSize.set( (lnfloat)size_.w, (lnfloat)size_.h );

        return LN_OK;
    }

    

    //---------------------------------------------------------------------
    // ● プリプロセス・ポストプロセス用のシェーダを追加する
    //---------------------------------------------------------------------
    void Viewport::addPostEffectShader( SceneShader* shader_, int priority_ )
    {
        if ( ( shader_->getScriptClass() == MME_SCRCLS_scene || 
               shader_->getScriptClass() == MME_SCRCLS_sceneorobject ) &&
             ( shader_->getScriptOrder() == MME_SCRORDER_preprocess ||
               shader_->getScriptOrder() == MME_SCRORDER_postprocess ) )
        {
            PPShader pps;
            pps.Shader   = shader_;
            pps.Priority = priority_;
            mPPShaderArray.push_back( pps );
            std::stable_sort( mPPShaderArray.begin(), mPPShaderArray.end(), _cmpPPShader );

            //LN_SAFE_ADDREF( shader_ );
            //printf( "シェーダ解放時、ビューボートからも外すようにチェックする。addref とかはいらない\n" );
        }
        else
        {
            LN_THROW_InvalidCall( 0, Resource::String::ERR_SceneViewport );
        }
    }

    //---------------------------------------------------------------------
    // ● プリプロセス・ポストプロセス用のシェーダを外す
    //---------------------------------------------------------------------
    void Viewport::removePostEffectShader( SceneShader* shader_ )
    {
        if ( shader_ )
        {
            PPShader pps;
            pps.Shader = shader_;
            Base::STLUtil::remove_if( mPPShaderArray, pps );

            //LN_SAFE_RELEASE( shader_ );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Viewport::setTone( const LTone& tone_, double duration_ )
    {
        mTone.start( mTone.getValue(), (const LVector4&)tone_, duration_ );
        SceneNode::setTone( tone_ );
    }

    //---------------------------------------------------------------------
    // ● 描画コマンドを階層的に作成する
    //---------------------------------------------------------------------
    void Viewport::makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ )
    {
        // ビューポートをセットする
        //commands_->add_SetViewport( &mViewportRect );

        

        if ( !mPriorityParameter || !mPriorityParameter->Hide )
        {
            // ポストエフェクトの Script の、先頭から "ScriptExternal=Color" までの処理
            PPShaderArray::iterator itr = mPPShaderArray.begin();
            PPShaderArray::iterator end = mPPShaderArray.end();
            for ( ; itr != end; ++itr )
            {
                itr->Shader->makeDrawCommandPreExternal( container_, this, pass_ );
            }

            // プリエフェクトの処理
        }

        

        // 子の処理
	    ln_foreach( SceneNode* node, mChildNodeList )
        {
		    node->makeDrawCommand( container_, pass_ );
	    }
        //SceneNode::makeDrawCommand( container_, pass_ );



        if ( !mPriorityParameter || !mPriorityParameter->Hide )
        {
            // ポストエフェクトの Script の、"ScriptExternal=Color" から終端までの処理
            // ポストエフェクトの Script の、先頭から "ScriptExternal=Color" までの処理
            PPShaderArray::reverse_iterator ritr = mPPShaderArray.rbegin();
            PPShaderArray::reverse_iterator rend = mPPShaderArray.rend();
            for ( ; ritr != rend; ++ritr )
            {
                ritr->Shader->makeDrawCommandPostExternal( container_, this, pass_ );
            }
        }

        // ビューポートを元に戻す
        //commands_->add_RemoveViewport();
    }

    //---------------------------------------------------------------------
    // ● フレーム更新
    //---------------------------------------------------------------------
    void Viewport::update( const Game::GameTime& time_ )
    {
        //mTone.advanceTime( mTone.getTime() + 1 );//time_.getElapsedGameTime() );
        mTone.advanceTime( 1 );
        SceneNode::setTone( (const LTone&)mTone.getValue() );
    }

    //---------------------------------------------------------------------
    // ● サブセットを描画する
    //---------------------------------------------------------------------
    LNRESULT Viewport::drawSubset( lnU32 index_ )
    {
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