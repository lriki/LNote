//=============================================================================
//�y SceneNode �z
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
// �� SceneNode �N���X
//=============================================================================

    
 

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
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
    // �� �f�X�g���N�^
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
    // �� ������
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
       
        // �f�t�H���g�̃V�F�[�_���Z�b�g
        // �w�肳�ꂽ���W��Ԃɒǉ�
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
            // ���܂̂Ƃ���Ȃɂ����Ȃ�
        }

		

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �m�[�h���̐ݒ�
    //---------------------------------------------------------------------
    LNRESULT SceneNode::setName( const lnChar* name_ )
    {
        mSceneGraph->onSetNodeName( this, mName.c_str(), name_ );
        mName = name_;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �q�m�[�h��ǉ�����
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
    // �� �q�m�[�h���O��
    //---------------------------------------------------------------------
    LNRESULT SceneNode::removeChild( SceneNode* node_ )
    {
        Base::STLUtil::remove( mChildNodeList, node_ );
        LN_SAFE_RELEASE( node_->mParentNode );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �K�p����V�F�[�_�̐ݒ�
    //---------------------------------------------------------------------
    void SceneNode::setShader( SceneShader* shader_ )
    {
        if ( shader_ )
        {
            // ���L�s�ōŏ��ɃZ�b�g���ꂽ���̂ł͂Ȃ��ꍇ�A���̒������O����������
            shader_->onSettingShader( this );
        }

        LN_SAFE_ADDREF( shader_ );
        LN_SAFE_RELEASE( mShader );

        mShader = shader_;
       
        // �V�F�[�_����K�v�ȓK�p���C�g�����擾���āA�z����������Ă���
        if ( mShader )
        {
            mApplyLights.resize( mShader->getRequiredLightNum() );
        }
    }

    //---------------------------------------------------------------------
    // �� �s����K�w�I�ɍX�V����
    //---------------------------------------------------------------------
    void SceneNode::updateTransformHierarchy( const LMatrix& parent_marix_, const NodeRenderParam* parent_params_ )
    {
        // ���[���h�s��̍X�V���K�v�ȏꍇ�͍Čv�Z
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

        // �T�u�Z�b�g�P�ʂ̃p�����[�^�������ōX�V
        mNodeRenderParam.updateSubsetRenderParam( parent_params_ );


        SceneNodeArray::iterator itr = mChildNodeList.begin();
	    SceneNodeArray::iterator end = mChildNodeList.end();
	    for ( ;itr != end; ++itr )
	    {
		    (*itr)->updateTransformHierarchy( mCombinedWorldMatrix, &mNodeRenderParam );
	    }
    }

    //---------------------------------------------------------------------
    // �� �J�����P�ʂ̕`����̍X�V
    //---------------------------------------------------------------------
    void SceneNode::updateParamFromCameraAndLight( Camera* camera_ )
    {
        //-----------------------------------------------------
        // Z �\�[�g�̊�l���v�Z

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
        // �q�m�[�h�Ăяo�� & Z �\�[�g

        if ( !mChildNodeList.empty() )
        {
            ln_foreach( SceneNode* node, mChildNodeList )
            {
                node->updateParamFromCameraAndLight( camera_ );
            }

            std::stable_sort( mChildNodeList.begin(), mChildNodeList.end(), _cmpZAndPrioritySort );
        }

        //-----------------------------------------------------
        // ������J�����O


        mEnableDraw = mIsVisible;
    }

    //---------------------------------------------------------------------
    // �� �`��R�}���h���K�w�I�ɍ쐬����
    //---------------------------------------------------------------------
    void SceneNode::makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ )
    {
        // �R�}���h�쐬
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
                // �V�F�[�_�o�R�ŃR�}���h�쐬
                shader->makeDrawCommand( container_, this, pass_ );
            }
            else
            {
                ////LN_SUB_COMMENT("�V�F�[�_���Ȃ��ꍇ�̏����B�Ƃ肠���� DrawGeometry �����B\n");
                container_->add_DrawBufferNoShader( this );
            }
        }

        // �q�Ăяo��
        SceneNodeArray::iterator itr = mChildNodeList.begin();
	    SceneNodeArray::iterator end = mChildNodeList.end();
	    for ( ;itr != end; ++itr )
	    {
		    (*itr)->makeDrawCommand( container_, pass_ );
	    }

        // �㏈��
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
    // �� Z �\�[�g�p�̔�r�֐�
    //---------------------------------------------------------------------
    bool SceneNode::_cmpZAndPrioritySort( const SceneNode* l_, const SceneNode* r_ )
    {
        if ( l_->mPriority == r_->mPriority )
        {
            // ����
            return l_->mZDistance < r_->mZDistance;
        }
        // ����
        return l_->mPriority < r_->mPriority;
    }

//=============================================================================
// �� SceneNodeContext
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

        // LMaterial ���ł̓��\�[�X�̎Q�ƃJ�E���g��ω������Ă���̂ŁAmemcpy �� NG
        for ( lnU32 i = 0; i < mSubsetNum; ++i )
        {
            mSubsetRenderParams[ i ] = mSceneNode->mNodeRenderParam.Subset[ i ];
        }

        // �e������ꍇ�� 0 �ԃT�u�Z�b�g�̃R�~�b�g�ςݏ����󂯌p��
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

        mPriorityParameter = mSceneNode->mPriorityParameter;    // NULL �ł��������c
        mCombinedWorldMatrix = mSceneNode->mCombinedWorldMatrix;
        mPriority = mSceneNode->mPriority;
        mBillboardType = mSceneNode->mBillboardType;
        mShader = mSceneNode->mShader;
        LN_SAFE_ADDREF( mShader );
        mIsVisible = mSceneNode->mIsVisible;

        // �K�v�ȃ��C�g�̐��Ŕz��� resize
        if ( mShader )
        {
            mAffectLightContextArray.resize( mShader->getRequiredLightNum() );
        }

        // �q�m�[�h�̃R���e�L�X�g
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
        // Z �\�[�g
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
        // �r���{�[�h�s��v�Z

        // ���ʂ������r���{�[�h�̏ꍇ
        if ( mBillboardType == LN_BILLBOARD_FRONT )
        {
            LMatrix mat;

            // ���݂̃��[���h�s�񂩂��]�������������o��
            mat.setRotateMatrix( mCombinedWorldMatrix );

            // ��]������������Z
            mat.setMul3x3( camera_context_->getViewMatrixInverse() );

            // �ړ���������Z���Ȃ���
            mat.translation( mCombinedWorldMatrix.m30, mCombinedWorldMatrix.m31, mCombinedWorldMatrix.m32 );

            mCombinedWorldMatrix = mat;

            /*
            // ���݂̃��[���h�s�񂩂��]�������������o��
            mBillboardMatrix.setRotateMatrix( mCombinedWorldMatrix );

            // ��]������������Z
            mBillboardMatrix.setMul3x3( camera_->getViewMatrixInverse() );

            // �ړ���������Z���Ȃ���
            mBillboardMatrix.translation( mCombinedWorldMatrix.m30, mCombinedWorldMatrix.m31, mCombinedWorldMatrix.m32 );
            */
       
        }
        // Y ����]�̂ݍs���r���{�[�h�̏ꍇ
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
        // ���C�g��

        // ���C�g���K�v�ȏꍇ (mApplyLights �� updateContext �� resize() �ς�)
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

            // �t�s��
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

            // �]�u�s��
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

            // �t�s��~�]�u�s��
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
            // ����
            return l_->mZDistance < r_->mZDistance;
        }
        // ����
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