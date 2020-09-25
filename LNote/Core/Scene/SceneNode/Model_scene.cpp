//=============================================================================
//【 Model 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Graphics/Common/Model/ModelManager.h"
#include "../SceneGraph.h"
#include "../SceneShader/SceneShaderManager.h"
#include "../SceneShader/SceneShader.h"
#include "Model.h"

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
// ■ Model クラス
//=============================================================================

    //---------------------------------------------------------------------
    // Context
    //---------------------------------------------------------------------
    class Model::DrawingContext
        : public SceneNodeContext
    {
    public:

        DrawingContext()
            : mSkinningBoneTexture      ( NULL )
        {}

        virtual ~DrawingContext()
        {
            releaseAllResource();
        }

        virtual void updateContext( SceneNodeContext* parent_context_ )
        {
            SceneNodeContext::updateContext( parent_context_ );

            // メンバコピー
            Model* n = (Model*)this->mSceneNode;
            mModel = n->mModel;
            LN_REFOBJ_SET( mSkinningBoneTexture, n->mSkinningBoneTexture );

            // スキニングテクスチャ更新
            if ( mSkinningBoneTexture )
            {
                mModel->updateSkinningMatrix();

                lnU32 frame_num = mModel->getFrameNum();
                LMatrix* t = (LMatrix*)mSkinningBoneTexture->lock();
                for ( lnU32 i = 0; i < frame_num; ++i )
                {
                    t[ i ] = mModel->getFrame( i )->getSkinningMatrix();
                }
                mSkinningBoneTexture->unlock();
            }
        }

        virtual void releaseAllResource()
        {
            SceneNodeContext::releaseAllResource();
            LN_SAFE_RELEASE( mSkinningBoneTexture );
        }

        virtual void onSetNodeParameter( SceneShader* shader_ )
        {
            if ( mSkinningBoneTexture )
            {
                Graphics::IShader* core = shader_->getCoreShader();
                Graphics::IShaderVariable* v;

                v = core->getVariableByName( _T("lnBoneTextureReciprocalSize") );
                if ( v ) v->setVector( LVector4( mSkinningBoneTexture->getRealSize( true ), 0, 0 ) );

                v = core->getVariableByName( _T("lnBoneTexture") );
                if ( v ) v->setTexture( mSkinningBoneTexture );
            }
        }

        virtual void drawSubset( lnU32 index_ )
        {
            mModel->drawSubset( index_ );
        }

    private:

        Graphics::Model*    mModel;
        Graphics::ITexture* mSkinningBoneTexture;
    };

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    Model::Model( SceneGraph* scene_ )
        : SceneNode     ( scene_ )
        , mModel        ( NULL )
        , mSkinningBoneTexture  ( NULL )
    {
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    Model::~Model()
    {
        LN_SAFE_RELEASE( mSkinningBoneTexture );
        LN_SAFE_RELEASE( mModel );
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT Model::initialize( const lnChar* filename_, lnU32 flags_ )
    {
        LNRESULT lr;

        

        LN_CALL_R( this->mSceneGraph->getGraphicsManager()->getModelManager()->createModel(
            &mModel, filename_, flags_, LSharingKey( filename_ ) ) );

        // コンテキストを指定して基底初期化
        DrawingContext* context = LN_NEW DrawingContext();
        context->initialize( this, mModel->getSubsetNum() );
        SceneNode::initialize( mModel->getSubsetNum(), LN_DRAWCOORD_3D, context );

        // 名前の設定は必ずオフスクリーンRT作成前に設定しておくこと！
        this->setName( File::getFileName( filename_ ).c_str() );

        // モデル本体のマテリアルを、このノードのデフォルトとして設定する
        lnU32 sn = mModel->getSubsetNum();
        for ( lnU32 i = 0; i < sn; ++i )
        {
            this->mNodeRenderParam.Subset[ i ].Material = *mModel->getMaterial( i );
        }

        // スキンメッシュの場合はスキニング用のシェーダを設定し、
        // ボーン行列を格納するテクスチャを作成する
        if ( mModel->isSkinMesh() )
        {
            SceneShader* shader;
            this->mSceneGraph->getSceneShaderManager()->createSceneShader(
                &shader, _T( "LNResource\\Shader\\SSBasic3DSkinning.lnfx" ) );
            this->setShader( shader );
            LN_SAFE_RELEASE( shader );

            this->mSceneGraph->getGraphicsManager()->getGraphicsDevice()->createTexture(
                &mSkinningBoneTexture, 4, mModel->getFrameNum(), 1, LN_FMT_A32B32G32R32F );
        }
        

        //-----------------------------------------------------
        // 同名の .fx がないかチェック
        //      MME の仕様対応。同名のシェーダファイルを読み込み、設定する。
#if 0
        LRefTString path = File::getExtName( filename_ );
        path += _T( ".fx" );

        SceneShader* shader = NULL;
        try
        {
            this->mSceneGraph->getSceneShaderManager()->createSceneShader( &shader, path.c_str() );
        
            if ( shader->isPostEffect() )
            {
                this->mSceneGraph->getDefaultNode( LN_DEFAULT_SCENENODE_3D )->addPostEffectShader( shader, 0 );
            }
            else
            {
                this->setShader( shader );
            }

            /*
            if ( shader )
            {
                if ( shader->isPostEffect() )
                {
                    printf( "setSelfSceneNode() は isPostEffect() じゃなくて hasOffscreenRT() での判定がいいと思う\n" );
                    //shader->setSelfSceneNode( this );
                    this->mSceneGraph->getDefaultNode( LN_DEFAULT_SCENENODE_3D )->addPostEffectShader( shader, 0 );
                }
                
                this->setShader( shader );
                LN_SAFE_RELEASE( shader );
            }
            */
        }
        catch ( Base::FileNotFoundException e )
        {
            _p(e.getOutputMessage());
        }
#endif
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● フレーム更新
    //---------------------------------------------------------------------
    void Model::update( const Game::GameTime& time_ )
    {
        mModel->updateFrameMatrix();

        ////// 任意のボーン更新をする場合はここに処理を書く必要がある…

        mModel->postUpdate( time_.getElapsedGameTime() );

        //mModel->skinning();
    }

    //---------------------------------------------------------------------
    // ● サブセットを描画する
    //---------------------------------------------------------------------
    LNRESULT Model::drawSubset( lnU32 index_ )
    {
        this->mContext->drawSubset( index_ );
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