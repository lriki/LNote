//=============================================================================
//【 NodeRenderParam 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Common.h"
#include "../SceneShader/SceneShader.h"
#include "Camera.h"
#include "Light.h"
#include "NodeRenderParam.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

    LNPriorityParameter LNPriorityParameter::HIDE_PARAM( true );

//=============================================================================
// ■ NodeRenderParam クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    NodeRenderParam::NodeRenderParam()
        : Subset        ( NULL )
        , mSceneNode    ( NULL )
        , mSubsetNum    ( 0 )
        , mSceneShader  ( NULL )
        , mCombinedSubsetRenderParams   ( NULL )
        , mAffectWorldMatrix    ( NULL )
        , mAffectCamera         ( NULL )
        , mAffectLightArray     ( NULL )
        , mIsVisible    ( false )
    {
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    NodeRenderParam::~NodeRenderParam()
    {
        LN_SAFE_RELEASE( mSceneShader );
        SAFE_DELETE_ARRAY( Subset );
        SAFE_DELETE_ARRAY( mCombinedSubsetRenderParams );
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT NodeRenderParam::initialize( SceneNode* node_, u32 subset_num_ )
    {
        mSceneNode = node_;

        if ( subset_num_ > 0 )
        {
            Subset = LN_NEW LNSubsetRenderParam[ subset_num_ ];
            mCombinedSubsetRenderParams = LN_NEW LNSubsetRenderParam[ subset_num_ ];
            mSubsetNum = subset_num_;
        }
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 実際に描画時に使う情報の更新
    //---------------------------------------------------------------------
    void NodeRenderParam::updateSubsetRenderParam( const NodeRenderParam* parent_params_ )
    {
        // まずは自分が持っているパラメータを丸ごとコミット
        mCommittedRenderState = RenderState;

        // LMaterial 内ではリソースの参照カウントを変化させているので、memcpu は NG
        for ( lnU32 i = 0; i < mSubsetNum; ++i )
        {
            mCombinedSubsetRenderParams[ i ] = Subset[ i ];
        }
        //memcpy( , , sizeof( LNSubsetRenderParam ) * mSubsetNum );

        // 親がある場合は 0 番サブセットのコミット済み情報を受け継ぐ
        if ( parent_params_ )
        {
            const LNSubsetRenderParam& pp = parent_params_->mCombinedSubsetRenderParams[ 0 ];

            for ( lnU32 i = 0; i < mSubsetNum; ++i )
            {
                LNSubsetRenderParam& pa = mCombinedSubsetRenderParams[ i ];

                ((LVector4&)pa.ColorScale) = ((LVector4&)pa.ColorScale) * ((LVector4&)pp.ColorScale);
                ((LVector4&)pa.BlendColor) = ((LVector4&)pa.BlendColor) + ((LVector4&)pp.BlendColor);
                ((LVector4&)pa.Tone)       = ((LVector4&)pa.Tone)       + ((LVector4&)pp.Tone);
            }
        }
    }

    //---------------------------------------------------------------------
    // ● 描画に使う行列情報等の設定
    //---------------------------------------------------------------------
    void NodeRenderParam::setAffectElements( const LMatrix* world_, const Camera* camera_, const LightArray* lights_, SceneShader* shader_, bool visible_ )
    {
        mAffectWorldMatrix = world_;
        mAffectCamera = camera_;
        mAffectLightArray = lights_;
        LN_SAFE_RELEASE( mSceneShader );
        mSceneShader = shader_;
        LN_SAFE_ADDREF( mSceneShader );
        mIsVisible = visible_;
    }

    //---------------------------------------------------------------------
    // ● シェーダ変数の要求に対応する行列(WORLD が含まれているもの)を返す
    //---------------------------------------------------------------------
    const LMatrix* NodeRenderParam::getObjectMatrix( MMEVariableRequest req_, int light_idx_ ) const
    {
        switch ( req_ )
        {
            case MME_VARREQ_MATRIX_World:
            {
                return mAffectWorldMatrix;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView:
            {
                mTempMatrix = *mAffectWorldMatrix * mAffectCamera->getViewMatrix();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj:
            {
                mTempMatrix = *mAffectWorldMatrix * mAffectCamera->getViewProjectionMatrix();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView:
            {
                mTempMatrix = *mAffectWorldMatrix * (*mAffectLightArray)[ light_idx_ ]->getViewMatrix();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj:
            {
                mTempMatrix = *mAffectWorldMatrix * (*mAffectLightArray)[ light_idx_ ]->getViewProjectionMatrix();
                return &mTempMatrix;
            }

            // 逆行列
            case MME_VARREQ_MATRIX_World_I:
            {
                LMatrix::inverse( &mTempMatrix, *mAffectWorldMatrix );
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView_I:
            {
                mTempMatrix = *mAffectWorldMatrix * mAffectCamera->getViewMatrix();
                mTempMatrix.inverse();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I:
            {
                mTempMatrix = *mAffectWorldMatrix * mAffectCamera->getViewProjectionMatrix();
                mTempMatrix.inverse();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView_I:
            {
                mTempMatrix = *mAffectWorldMatrix * (*mAffectLightArray)[ light_idx_ ]->getViewMatrix();
                mTempMatrix.inverse();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I:
            {
                mTempMatrix = *mAffectWorldMatrix * (*mAffectLightArray)[ light_idx_ ]->getViewProjectionMatrix();
                mTempMatrix.inverse();
                return &mTempMatrix;
            }

            // 転置行列
            case MME_VARREQ_MATRIX_World_T:
            {
                LMatrix::transpose( &mTempMatrix, *mAffectWorldMatrix );
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView_T:
            {
                mTempMatrix = *mAffectWorldMatrix * mAffectCamera->getViewMatrix();
                mTempMatrix.transpose();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T:
            {
                mTempMatrix = *mAffectWorldMatrix * mAffectCamera->getViewProjectionMatrix();
                mTempMatrix.transpose();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView_T:
            {
                mTempMatrix = *mAffectWorldMatrix * (*mAffectLightArray)[ light_idx_ ]->getViewMatrix();
                mTempMatrix.transpose();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T:
            {
                mTempMatrix = *mAffectWorldMatrix * (*mAffectLightArray)[ light_idx_ ]->getViewProjectionMatrix();
                mTempMatrix.transpose();
                return &mTempMatrix;
            }

            // 逆行列×転置行列
            case MME_VARREQ_MATRIX_World_IT:
            {
                LMatrix::inverse( &mTempMatrix, *mAffectWorldMatrix );
                mTempMatrix.transpose();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView_IT:
            {
                mTempMatrix = *mAffectWorldMatrix * mAffectCamera->getViewMatrix();
                mTempMatrix.inverse();
                mTempMatrix.transpose();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT:
            {
                mTempMatrix = *mAffectWorldMatrix * mAffectCamera->getViewProjectionMatrix();
                mTempMatrix.inverse();
                mTempMatrix.transpose();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView_IT:
            {
                mTempMatrix = *mAffectWorldMatrix * (*mAffectLightArray)[ light_idx_ ]->getViewMatrix();
                mTempMatrix.inverse();
                mTempMatrix.transpose();
                return &mTempMatrix;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT:
            {
                mTempMatrix = *mAffectWorldMatrix * (*mAffectLightArray)[ light_idx_ ]->getViewProjectionMatrix();
                mTempMatrix.inverse();
                mTempMatrix.transpose();
                return &mTempMatrix;
            }
        }

        return NULL;


        /*
        switch ( req_ )
        {
            case MME_VARREQ_MATRIX_World:
                return *m_w;
            case MME_VARREQ_MATRIX_CAMERA_WorldView:
                return m_wv_c;
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj:
                return m_wvp_c;
            case MME_VARREQ_MATRIX_LIGHT_WorldView:
            {
                if ( light_idx_ < 0 ) { return m_wv_l; }
                const Light* l = (*mAffectLightArray)[ light_idx_ ];
                if ( l->isDefaultLight() )
                {
                    return m_wv_l;
                }
                else
                {
                    mTempMatrix = (*m_w) * l->getViewMatrix();
                    return mTempMatrix;
                }
                break;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj:
            {
                if ( light_idx_ < 0 ) { return m_wvp_l; }
                const Light* l = (*mAffectLightArray)[ light_idx_ ];
                if ( l->isDefaultLight() )
                {
                    return m_wvp_l;
                }
                else
                {
                    mTempMatrix = (*m_w) * l->getViewProjectionMatrix();
                    return mTempMatrix;
                }
                break;
            }

            // 逆行列
            case MME_VARREQ_MATRIX_World_I:
                return m_w_i;
            case MME_VARREQ_MATRIX_CAMERA_WorldView_I:
                return m_wv_c_i;
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I:
                return m_wvp_c_i;
            case MME_VARREQ_MATRIX_LIGHT_WorldView_I:
            {
                if ( light_idx_ < 0 ) { return m_wv_l_i; }
                const Light* l = (*mAffectLightArray)[ light_idx_ ];
                if ( l->isDefaultLight() )
                {
                    return m_wv_l_i;
                }
                else
                {
                    mTempMatrix = (*m_w) * l->getViewMatrix();
                    mTempMatrix.inverse();
                    return mTempMatrix;
                }
                break;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I:
            {
                if ( light_idx_ < 0 ) { return m_wvp_l_i; }
                const Light* l = (*mAffectLightArray)[ light_idx_ ];
                if ( l->isDefaultLight() )
                {
                    return m_wvp_l_i;
                }
                else
                {
                    mTempMatrix = (*m_w) * l->getViewProjectionMatrix();
                    mTempMatrix.inverse();
                    return mTempMatrix;
                }
                break;
            }

            // 転置行列
            case MME_VARREQ_MATRIX_World_T:
                return m_w_t;
            case MME_VARREQ_MATRIX_CAMERA_WorldView_T:
                return m_wv_c_t;
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T:
                return m_wvp_c_t;
            case MME_VARREQ_MATRIX_LIGHT_WorldView_T:
            {
                if ( light_idx_ < 0 ) { return m_wv_l_t; }
                const Light* l = (*mAffectLightArray)[ light_idx_ ];
                if ( l->isDefaultLight() )
                {
                    return m_wv_l_t;
                }
                else
                {
                    mTempMatrix = (*m_w) * l->getViewMatrix();
                    mTempMatrix.transpose();
                    return mTempMatrix;
                }
                break;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T:
            {
                if ( light_idx_ < 0 ) { return m_wvp_l_t; }
                const Light* l = (*mAffectLightArray)[ light_idx_ ];
                if ( l->isDefaultLight() )
                {
                    return m_wvp_l_t;
                }
                else
                {
                    mTempMatrix = (*m_w) * l->getViewProjectionMatrix();
                    mTempMatrix.transpose();
                    return mTempMatrix;
                }
                break;
            }

            // 逆行列×転置行列
            case MME_VARREQ_MATRIX_World_IT:
                return m_w_it;
            case MME_VARREQ_MATRIX_CAMERA_WorldView_IT:
                return m_wv_c_it;
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT:
                return m_wvp_c_it;
            case MME_VARREQ_MATRIX_LIGHT_WorldView_IT:
            {
                if ( light_idx_ < 0 ) { return m_wv_l_it; }
                const Light* l = (*mAffectLightArray)[ light_idx_ ];
                if ( l->isDefaultLight() )
                {
                    return m_wv_l_it;
                }
                else
                {
                    mTempMatrix = (*m_w) * l->getViewMatrix();
                    mTempMatrix.inverse();
                    mTempMatrix.transpose();
                    return mTempMatrix;
                }
                break;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT:
            {
                if ( light_idx_ < 0 ) { return m_wvp_l_it; }
                const Light* l = (*mAffectLightArray)[ light_idx_ ];
                if ( l->isDefaultLight() )
                {
                    return m_wvp_l_it;
                }
                else
                {
                    mTempMatrix = (*m_w) * l->getViewProjectionMatrix();
                    mTempMatrix.inverse();
                    mTempMatrix.transpose();
                    return mTempMatrix;
                }
                break;
            }
        }

        return LMatrix::IDENTITY;
        */
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