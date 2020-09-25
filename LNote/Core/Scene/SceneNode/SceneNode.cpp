//=============================================================================
//【 SceneNode 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <algorithm>
#include "../../Base/STLUtil.h"
#include "../SceneGraph.h"
#include "../SceneShader/SceneShader.h"
#include "Camera.h"
#include "Light.h"
#include "SceneNode.h"

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
// ■ SceneNode クラス
//=============================================================================

    
 

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    SceneNode::SceneNode( SceneGraph* scene_ )
        : mSceneGraph       ( scene_ )
        , mDrawCoordSpace   ( LN_DRAWCOORD_UNKNOWN )
        , mSubsetNum        ( 0 )
        , mParentNode       ( NULL )
        , mContext          ( NULL )
        , mScale            ( 1, 1, 1 )
        , mBillboardType    ( LN_BILLBOARD_NOT )
        , mShader           ( NULL )
        , mRotOrder         ( LN_ROTORDER_XYZ )
        , mPriority         ( 0 )
        , mZDistance        ( 0.0f )
        , mPriorityParameter    ( NULL )
        , mIsVisible            ( true )
        , mNeedUpdateMatrix     ( false )
        , mIsAutoUpdate         ( true )
        , mEnableDraw           ( true )

        //: mSceneGraph       ( scene_ )
        //, mParentNode       ( NULL )
        //, mZSortFunc        ( LN_ZSORTF_IMM_Z )
        //, mZDistance        ( 0.0f )
        //
        //
        //, mSubsetRenderParams   ( NULL )
        //
        //, mShader           ( NULL )
        //
        //, mUseMatrix        ( true )
        //
        //
    {
        LN_SAFE_ADDREF( mSceneGraph );
        
    }


    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    SceneNode::~SceneNode()
    {
        SAFE_DELETE( mContext );

        if ( mParentNode )
        {
            mParentNode->removeChild( this );
        }

        if ( mSceneGraph )
        {
            mSceneGraph->onDeleteNode( this );
        }

        LN_SAFE_RELEASE( mShader );
        LN_SAFE_RELEASE( mSceneGraph );
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT SceneNode::initialize( lnU32 subset_num_, LNDrawCoordSpace cs_, SceneNodeContext* context_ )
    {
        mDrawCoordSpace = cs_;

        mSceneGraph->addNode( this );

        if ( context_ )
        {
            mContext = context_;
            mSubsetNum = context_->getSubsetNum();
        }
        else
        {
            mSubsetNum = subset_num_;
            mContext = LN_NEW SceneNodeContext();
            mContext->initialize( this, mSubsetNum );
        }
        if ( mSubsetNum > 0 )
        {
            mNodeRenderParam.initialize( this, subset_num_ );
			mNodeRenderParam.RenderState = mSceneGraph->getNodeDefaultRenderState();
        }
       
        // デフォルトのシェーダをセット
        // 指定された座標空間に追加
        if ( getDrawCoordSpace() == LN_DRAWCOORD_3D )
        {
            mShader = mSceneGraph->getDefaultShader( LN_DEFSHADER_BASIC_3D );
            LN_SAFE_ADDREF( mShader );

            mSceneGraph->getDefaultNode( LN_DEFAULT_SCENENODE_3D )->addChild( this );
        }
        else if ( getDrawCoordSpace() == LN_DRAWCOORD_2D )
        {
            mShader = mSceneGraph->getDefaultShader( LN_DEFSHADER_BASIC_2D );
            LN_SAFE_ADDREF( mShader );

            mSceneGraph->getDefaultNode( LN_DEFAULT_SCENENODE_2D )->addChild( this );
        }
        else// if ( getDrawCoordSpace() == LN_DRAWCOORD_UNKNOWN )
        {
            // いまのところなにもしない
        }

		

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● ノード名の設定
    //---------------------------------------------------------------------
    LNRESULT SceneNode::setName( const lnChar* name_ )
    {
        mSceneGraph->onSetNodeName( this, mName.c_str(), name_ );
        mName = name_;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 子ノードを追加する
    //---------------------------------------------------------------------
    LNRESULT SceneNode::addChild( SceneNode* node_ )
    {
        if ( node_ )
        {
            if ( node_->mParentNode )
            {
                node_->mParentNode->removeChild( node_ );
            }

            mChildNodeList.push_back( node_ );
            node_->mParentNode = this;
            LN_SAFE_ADDREF( node_->mParentNode );
        }
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 子ノードを外す
    //---------------------------------------------------------------------
    LNRESULT SceneNode::removeChild( SceneNode* node_ )
    {
        Base::STLUtil::remove( mChildNodeList, node_ );
        LN_SAFE_RELEASE( node_->mParentNode );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 適用するシェーダの設定
    //---------------------------------------------------------------------
    void SceneNode::setShader( SceneShader* shader_ )
    {
        if ( shader_ )
        {
            // 共有不可で最初にセットされたものではない場合、この中から例外が投げられる
            shader_->onSettingShader( this );
        }

        LN_SAFE_ADDREF( shader_ );
        LN_SAFE_RELEASE( mShader );

        mShader = shader_;
       
        // シェーダから必要な適用ライト数を取得して、配列を準備しておく
        if ( mShader )
        {
            mApplyLights.resize( mShader->getRequiredLightNum() );
        }
    }

    //---------------------------------------------------------------------
    // ● 行列を階層的に更新する
    //---------------------------------------------------------------------
    void SceneNode::updateTransformHierarchy( const LMatrix& parent_marix_, const NodeRenderParam* parent_params_ )
    {
        // ワールド行列の更新が必要な場合は再計算
        if ( mNeedUpdateMatrix )
        {
            mMatrix.identity();
            mMatrix.translation( -mCenter.x, -mCenter.y, -mCenter.z );
            mMatrix.scaling( mScale );
            mMatrix.rotation( mAngle, mRotOrder );
            mMatrix.translation( mPosition );
            mNeedUpdateMatrix = false;   
        }

        mCombinedWorldMatrix = mMatrix * parent_marix_;

        // サブセット単位のパラメータもここで更新
        mNodeRenderParam.updateSubsetRenderParam( parent_params_ );


        SceneNodeArray::iterator itr = mChildNodeList.begin();
	    SceneNodeArray::iterator end = mChildNodeList.end();
	    for ( ;itr != end; ++itr )
	    {
		    (*itr)->updateTransformHierarchy( mCombinedWorldMatrix, &mNodeRenderParam );
	    }
    }

    //---------------------------------------------------------------------
    // ● カメラ単位の描画情報の更新
    //---------------------------------------------------------------------
    void SceneNode::updateParamFromCameraAndLight( Camera* camera_ )
    {
        //-----------------------------------------------------
        // Z ソートの基準値を計算

        switch ( camera_->getZSortType() )
        {
            default:
            case LN_ZSORTF_IMM_Z:
                mZDistance = mPosition.z;
                break;
            case LN_ZSORTF_CAMERA_DIST:
                mZDistance = LVector3::squareLength( mPosition - camera_->getPosition() );
                break;
            case LN_ZSORTF_CAMERA_SCREEN:
                mZDistance = LGeometry::PointPolyDistance(
                    mPosition, 
                    camera_->getPosition(),
                    camera_->getDirection() );
                break;
        }

        //-----------------------------------------------------
        // 子ノード呼び出し & Z ソート

        if ( !mChildNodeList.empty() )
        {
            ln_foreach( SceneNode* node, mChildNodeList )
            {
                node->updateParamFromCameraAndLight( camera_ );
            }

            std::stable_sort( mChildNodeList.begin(), mChildNodeList.end(), _cmpZAndPrioritySort );
        }

        //-----------------------------------------------------
        // 視錐台カリング


        mEnableDraw = mIsVisible;
    }

    //---------------------------------------------------------------------
    // ● 描画コマンドを階層的に作成する
    //---------------------------------------------------------------------
    void SceneNode::makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ )
    {
        // コマンド作成
        if ( mEnableDraw )
        {
            SceneShader* shader = getShader();
            if ( mPriorityParameter )
            {
                shader = mPriorityParameter->Shader;
            }
            

            container_->add_SetPriorityParameter( mPriorityParameter );

            if ( shader )
            {
                // シェーダ経由でコマンド作成
                shader->makeDrawCommand( container_, this, pass_ );
            }
            else
            {
                ////LN_SUB_COMMENT("シェーダがない場合の処理。とりあえず DrawGeometry だけ。\n");
                container_->add_DrawBufferNoShader( this );
            }
        }

        // 子呼び出し
        SceneNodeArray::iterator itr = mChildNodeList.begin();
	    SceneNodeArray::iterator end = mChildNodeList.end();
	    for ( ;itr != end; ++itr )
	    {
		    (*itr)->makeDrawCommand( container_, pass_ );
	    }

        // 後処理
        if ( mEnableDraw )
        {
            container_->add_SetPriorityParameter( NULL );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void SceneNode::updateContext()
    {
        mContext->updateContext( (mParentNode) ? mParentNode->mContext : NULL );
    }

    //---------------------------------------------------------------------
    // ● Z ソート用の比較関数
    //---------------------------------------------------------------------
    bool SceneNode::_cmpZAndPrioritySort( const SceneNode* l_, const SceneNode* r_ )
    {
        if ( l_->mPriority == r_->mPriority )
        {
            // 昇順
            return l_->mZDistance < r_->mZDistance;
        }
        // 昇順
        return l_->mPriority < r_->mPriority;
    }

//=============================================================================
// ■ SceneNodeContext
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    SceneNodeContext::SceneNodeContext()
        : mSceneNode            ( NULL )
        , mSubsetNum            ( 0 )
        , mSubsetRenderParams   ( NULL )
        , mPriorityParameter    ( NULL )
        , mPriority             ( 0 )
        , mBillboardType        ( LN_BILLBOARD_NOT )
        , mShader               ( NULL )
        , mZDistance            ( 0 )
        , mAffectCameraContext  ( NULL )
        , mIsVisible            ( true )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    SceneNodeContext::~SceneNodeContext()
    {
        SAFE_DELETE_ARRAY( mSubsetRenderParams );
        releaseAllResource();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void SceneNodeContext::initialize( SceneNode* node_, lnU32 subset_num_ )
    {
        mSceneNode = node_;

        if ( subset_num_ > 0 )
        {
            mSubsetRenderParams = LN_NEW LNSubsetRenderParam[subset_num_];
            mSubsetNum = subset_num_;
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void SceneNodeContext::updateContext( SceneNodeContext* parent_context_ )
    {
        releaseAllResource();

        // LMaterial 内ではリソースの参照カウントを変化させているので、memcpy は NG
        for ( lnU32 i = 0; i < mSubsetNum; ++i )
        {
            mSubsetRenderParams[ i ] = mSceneNode->mNodeRenderParam.Subset[ i ];
        }

        // 親がある場合は 0 番サブセットのコミット済み情報を受け継ぐ
        if ( parent_context_ )
        {
            const LNSubsetRenderParam& pp = parent_context_->mSubsetRenderParams[ 0 ];

            for ( lnU32 i = 0; i < mSubsetNum; ++i )
            {
                LNSubsetRenderParam& pa = mSubsetRenderParams[ i ];

                ((LVector4&)pa.ColorScale) = ((LVector4&)pa.ColorScale) * ((LVector4&)pp.ColorScale);
                ((LVector4&)pa.BlendColor) = ((LVector4&)pa.BlendColor) + ((LVector4&)pp.BlendColor);
                ((LVector4&)pa.Tone)       = ((LVector4&)pa.Tone)       + ((LVector4&)pp.Tone);
            }
        }

        mRenderState = mSceneNode->mNodeRenderParam.RenderState;

        mPriorityParameter = mSceneNode->mPriorityParameter;    // NULL でいいかも…
        mCombinedWorldMatrix = mSceneNode->mCombinedWorldMatrix;
        mPriority = mSceneNode->mPriority;
        mBillboardType = mSceneNode->mBillboardType;
        mShader = mSceneNode->mShader;
        LN_SAFE_ADDREF( mShader );
        mIsVisible = mSceneNode->mIsVisible;

        // 必要なライトの数で配列を resize
        if ( mShader )
        {
            mAffectLightContextArray.resize( mShader->getRequiredLightNum() );
        }

        // 子ノードのコンテキスト
        int n = mSceneNode->mChildNodeList.size();
        mChildSceneNodeContextArray.resize( n );
        for ( int i = 0; i < n; ++i )
        {
            mChildSceneNodeContextArray[i] = mSceneNode->mChildNodeList[i]->mContext;
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void SceneNodeContext::releaseAllResource()
    {
        LN_SAFE_RELEASE( mShader );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void SceneNodeContext::OnSetActivePaneContext( SceneGraphContext* scene_context_, PaneContext* pane_context_, CameraContext* camera_context_ )
    {
        mAffectCameraContext = camera_context_;

        //-----------------------------------------------------
        // Z ソート
/*
        switch ( camera_context_->getZSortType() )
        {
            default:
            case LN_ZSORTF_IMM_Z:
                mZDistance = mCombinedWorldMatrix.getPosition().z;
                break;
            case LN_ZSORTF_CAMERA_DIST:
                mZDistance = LVector3::squareLength( mCombinedWorldMatrix.getPosition() - camera_context_->getPosition() );
                break;
            case LN_ZSORTF_CAMERA_SCREEN:
                mZDistance = LGeometry::PointPolyDistance(
                    mCombinedWorldMatrix.getPosition(), 
                    camera_context_->getPosition(),
                    camera_context_->getDirection() );
                break;
        }

        std::stable_sort( mChildSceneNodeContextArray.begin(), mChildSceneNodeContextArray.end(), _cmpZAndPrioritySort );
*/
        //-----------------------------------------------------
        // ビルボード行列計算

        // 正面を向くビルボードの場合
        if ( mBillboardType == LN_BILLBOARD_FRONT )
        {
            LMatrix mat;

            // 現在のワールド行列から回転部分だけを取り出す
            mat.setRotateMatrix( mCombinedWorldMatrix );

            // 回転部分だけを乗算
            mat.setMul3x3( camera_context_->getViewMatrixInverse() );

            // 移動成分を乗算しなおす
            mat.translation( mCombinedWorldMatrix.m30, mCombinedWorldMatrix.m31, mCombinedWorldMatrix.m32 );

            mCombinedWorldMatrix = mat;

            /*
            // 現在のワールド行列から回転部分だけを取り出す
            mBillboardMatrix.setRotateMatrix( mCombinedWorldMatrix );

            // 回転部分だけを乗算
            mBillboardMatrix.setMul3x3( camera_->getViewMatrixInverse() );

            // 移動成分を乗算しなおす
            mBillboardMatrix.translation( mCombinedWorldMatrix.m30, mCombinedWorldMatrix.m31, mCombinedWorldMatrix.m32 );
            */
       
        }
        // Y 軸回転のみ行うビルボードの場合
        else if ( mBillboardType == LN_BILLBOARD_ROT_Y )
        {
            const LMatrix& view = camera_context_->getViewMatrix();

            LMatrix mat;
            mat.setRotateMatrix( mCombinedWorldMatrix );

            if ( view.m02 > 0.0f )
		    {
                mat.rotationY( -atanf( view.m22 / view.m02 ) + LMath::PI / 2 );
		    }   
		    else if ( view.m02 == 0.0f )
		    {
                mat.identity();
		    }
		    else
		    {
                mat.rotationY( -atanf( view.m22 / view.m02 ) - LMath::PI / 2 );
		    }

            mat.translation( mCombinedWorldMatrix.getPosition() );

            mCombinedWorldMatrix = mat;
        }

        //-----------------------------------------------------
        // ライト列挙

        // ライトが必要な場合 (mApplyLights は updateContext で resize() 済み)
        if ( mAffectLightContextArray.size() > 0 )
        {
            scene_context_->selectLights( &mAffectLightContextArray, mCombinedWorldMatrix.getPosition() );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    bool SceneNodeContext::getObjectMatrix( LMatrix* out_, MMEVariableRequest req_, int light_idx_ ) const
    {
        switch ( req_ )
        {
            case MME_VARREQ_MATRIX_World:
            {
                *out_ = mCombinedWorldMatrix;
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewMatrix();
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewProjectionMatrix();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewMatrix();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewProjectionMatrix();
                return true;
            }

            // 逆行列
            case MME_VARREQ_MATRIX_World_I:
            {
                LMatrix::inverse( out_, mCombinedWorldMatrix );
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView_I:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewMatrix();
                out_->inverse();
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewProjectionMatrix();
                out_->inverse();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView_I:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewMatrix();
                out_->inverse();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewProjectionMatrix();
                out_->inverse();
                return true;
            }

            // 転置行列
            case MME_VARREQ_MATRIX_World_T:
            {
                LMatrix::transpose( out_, mCombinedWorldMatrix );
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView_T:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewMatrix();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewProjectionMatrix();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView_T:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewMatrix();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewProjectionMatrix();
                out_->transpose();
                return true;
            }

            // 逆行列×転置行列
            case MME_VARREQ_MATRIX_World_IT:
            {
                LMatrix::inverse( out_, mCombinedWorldMatrix );
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView_IT:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewMatrix();
                out_->inverse();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewProjectionMatrix();
                out_->inverse();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView_IT:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewMatrix();
                out_->inverse();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewProjectionMatrix();
                out_->inverse();
                out_->transpose();
                return true;
            }
        }

        return false;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    bool SceneNodeContext::_cmpZAndPrioritySort( const SceneNodeContext* l_, const SceneNodeContext* r_ )
    {
        if ( l_->mPriority == r_->mPriority )
        {
            // 昇順
            return l_->mZDistance < r_->mZDistance;
        }
        // 昇順
        return l_->mPriority < r_->mPriority;
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