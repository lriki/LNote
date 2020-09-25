//=============================================================================
//�y Viewport �z
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
// �� Viewport �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    Viewport::Viewport( SceneGraph* scene_ )
        : SceneNode                 ( scene_ )
        
    {
        scene_->addViewport( this );
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
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
    // �� ������
    //---------------------------------------------------------------------
    LNRESULT Viewport::initialize( const Geometry::Size& size_ )
    {
        SceneNode::initialize( 1, LN_DRAWCOORD_UNKNOWN );

        mViewSize.set( (lnfloat)size_.w, (lnfloat)size_.h );

        return LN_OK;
    }

    

    //---------------------------------------------------------------------
    // �� �v���v���Z�X�E�|�X�g�v���Z�X�p�̃V�F�[�_��ǉ�����
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
            //printf( "�V�F�[�_������A�r���[�{�[�g������O���悤�Ƀ`�F�b�N����Baddref �Ƃ��͂���Ȃ�\n" );
        }
        else
        {
            LN_THROW_InvalidCall( 0, Resource::String::ERR_SceneViewport );
        }
    }

    //---------------------------------------------------------------------
    // �� �v���v���Z�X�E�|�X�g�v���Z�X�p�̃V�F�[�_���O��
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
    // �� �`��R�}���h���K�w�I�ɍ쐬����
    //---------------------------------------------------------------------
    void Viewport::makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ )
    {
        // �r���[�|�[�g���Z�b�g����
        //commands_->add_SetViewport( &mViewportRect );

        

        if ( !mPriorityParameter || !mPriorityParameter->Hide )
        {
            // �|�X�g�G�t�F�N�g�� Script �́A�擪���� "ScriptExternal=Color" �܂ł̏���
            PPShaderArray::iterator itr = mPPShaderArray.begin();
            PPShaderArray::iterator end = mPPShaderArray.end();
            for ( ; itr != end; ++itr )
            {
                itr->Shader->makeDrawCommandPreExternal( container_, this, pass_ );
            }

            // �v���G�t�F�N�g�̏���
        }

        

        // �q�̏���
	    ln_foreach( SceneNode* node, mChildNodeList )
        {
		    node->makeDrawCommand( container_, pass_ );
	    }
        //SceneNode::makeDrawCommand( container_, pass_ );



        if ( !mPriorityParameter || !mPriorityParameter->Hide )
        {
            // �|�X�g�G�t�F�N�g�� Script �́A"ScriptExternal=Color" ����I�[�܂ł̏���
            // �|�X�g�G�t�F�N�g�� Script �́A�擪���� "ScriptExternal=Color" �܂ł̏���
            PPShaderArray::reverse_iterator ritr = mPPShaderArray.rbegin();
            PPShaderArray::reverse_iterator rend = mPPShaderArray.rend();
            for ( ; ritr != rend; ++ritr )
            {
                ritr->Shader->makeDrawCommandPostExternal( container_, this, pass_ );
            }
        }

        // �r���[�|�[�g�����ɖ߂�
        //commands_->add_RemoveViewport();
    }

    //---------------------------------------------------------------------
    // �� �t���[���X�V
    //---------------------------------------------------------------------
    void Viewport::update( const Game::GameTime& time_ )
    {
        //mTone.advanceTime( mTone.getTime() + 1 );//time_.getElapsedGameTime() );
        mTone.advanceTime( 1 );
        SceneNode::setTone( (const LTone&)mTone.getValue() );
    }

    //---------------------------------------------------------------------
    // �� �T�u�Z�b�g��`�悷��
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