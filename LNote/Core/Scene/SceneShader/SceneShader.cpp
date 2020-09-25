//=============================================================================
//�y SceneShader �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Resource/LNResource.h"
#include "../../Graphics/Common/GraphicsUtil.h"
#include "../SceneGraph.h"
#include "../SceneNode/SceneNode.h"
#include "../SceneNode/Light.h"
#include "../SceneNode/Viewport.h"
#include "OffscreenScene.h"
#include "SceneShaderTechnique.h"
#include "SceneShader.h"

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
// �� SceneShader �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    SceneShader::SceneShader( SceneShaderManager* manager_ )
        : mManager          ( manager_ )
        , mShader           ( NULL )
        , mRequiredLightNum ( 0 )
        , mScriptOutput     ( MME_SCROUT_color )
        , mScriptClass      ( MME_SCRCLS_object )
        , mScriptOrder      ( MME_SCRORDER_standard )
        , mSelfSceneNode    ( NULL )
        , mIsError          ( false )
        , mShareable        ( true )
    {
        LN_SAFE_ADDREF( mManager );
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    SceneShader::~SceneShader()
    {
        if ( mManager && isPostEffect() )
        {
            ViewportList& list = mManager->getScene()->getViewportList();
            ln_foreach( Viewport* vp, list )
            {
                vp->removePostEffectShader( this );
            }
        }

        _release();
        LN_SAFE_RELEASE( mShader );
        LN_SAFE_RELEASE( mManager );
    }

    //---------------------------------------------------------------------
    // �� ������
    //---------------------------------------------------------------------
    LNRESULT SceneShader::initialize( const InitData& init_data_ )
    {
        LNRESULT lr;

        mName   = init_data_.ShaderName;
        //mSelfSceneNode = init_data_.SelfNode;
        mIsError = true;    // �ŏ��̓G���[�ɂ��Ă����B���������������� false

        // �V�F�[�_�v���O���������
        //  (���L�� Scene ���ł��Ă�̂ŁA�����ł� key_ �̎w��͖����� OK)
        try
        {
            mManager->getGraphicsDevice()->createShader( &mShader, mName.c_str() );
        }
        catch ( Base::Exception e )
        {
            if ( mShader )
            {
                mErrorMessage = mShader->getCompileErrors();
            }
            else
            {
                mErrorMessage = e.getErrorMessage();
            }
            throw e;
            return LN_ERR_ASSERT;
        }

        if ( mShader->getCompileResult() != LN_SHADERRESULT_SUCCESS )
        {
            mErrorMessage = mShader->getCompileErrors();
        }

        // �ϐ��A�e�N�j�b�N�����\�z
        LN_CALL_R( _build() );

        mIsError = false;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �ēǂݍ���
    //---------------------------------------------------------------------
    LNRESULT SceneShader::reload()
    {
        LN_PRINT_NOT_IMPL_FUNCTION;

        mIsError = true;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� Scene �P�ʂŕK�v�ȃp�����[�^��ݒ肷�� (�{�͕̂`�抮���܂ŉ�����Ȃ�����)
    //---------------------------------------------------------------------
    void SceneShader::updateSceneParam( const MMESceneParam& param_ )
    {
        MMEShaderVariable* sv;
        Graphics::IShaderVariable* var;
    
        ln_foreach( sv, mShaderVariableArray )
        {
            //sv = ( mShaderVarArray + i );

            // �ϐ����z��̃��C�g���͂��̊֐����ł͐ݒ肵�Ȃ��̂Ŏ���
            if ( sv->LightNum >= 1 )
            {
                continue;
            }

            var = sv->Variable;

            switch ( sv->Request )
            {
                // �s��
                case MME_VARREQ_MATRIX_LIGHT_View:
                    var->setMatrix( *param_.Mat_LightView );
                    break;
                case MME_VARREQ_MATRIX_LIGHT_Proj:
                    var->setMatrix( *param_.Mat_LightProj );
                    break;
                case MME_VARREQ_MATRIX_LIGHT_ViewProj:
                    var->setMatrix( *param_.Mat_LightViewProj );
                    break;

                // �t�s��
                case MME_VARREQ_MATRIX_LIGHT_View_I:
                    var->setMatrix( param_.Mat_LightView_Inverse );
                    break;
                case MME_VARREQ_MATRIX_LIGHT_Proj_I:
                    var->setMatrix( param_.Mat_LightView_Inverse );
                    break;
                case MME_VARREQ_MATRIX_LIGHT_ViewProj_I:
                    var->setMatrix( param_.Mat_LightView_Inverse );
                    break;

                // �]�u�s��
                case MME_VARREQ_MATRIX_LIGHT_View_T:
                    var->setMatrix( param_.Mat_LightView_Transpose );
                    break;
                case MME_VARREQ_MATRIX_LIGHT_Proj_T:
                    var->setMatrix( param_.Mat_LightView_Transpose );
                    break;
                case MME_VARREQ_MATRIX_LIGHT_ViewProj_T:
                    var->setMatrix( param_.Mat_LightView_Transpose );
                    break;

                // �t�s��~�]�u�s��
                case MME_VARREQ_MATRIX_LIGHT_View_IT:
                    var->setMatrix( param_.Mat_LightView_InverseTranspose );
                    break;
                case MME_VARREQ_MATRIX_LIGHT_Proj_IT:
                    var->setMatrix( param_.Mat_LightView_InverseTranspose );
                    break;
                case MME_VARREQ_MATRIX_LIGHT_ViewProj_IT:
                    var->setMatrix( param_.Mat_LightView_InverseTranspose );
                    break;

                // ���C�g�̐F
                case MME_VARREQ_LIGHT_DIFFUSE:
                    if ( sv->LightNum == 0 ) var->setVector( *param_.LightDiffuse );
                    break;
                case MME_VARREQ_LIGHT_AMBIENT:
                    if ( sv->LightNum == 0 ) var->setVector( *param_.LightAmbient );
                    break;
                case MME_VARREQ_LIGHT_SPECULAR:
                    if ( sv->LightNum == 0 ) var->setVector( *param_.LightSpecular );
                    break;

                // ���C�g�̈ʒu�E����
                case MME_VARREQ_LIGHT_POSITION:
                    if ( sv->LightNum == 0 ) var->setVector( param_.LightPosition );
                    break;
                case MME_VARREQ_LIGHT_DIRECTION:
                    if ( sv->LightNum == 0 ) var->setVector( param_.LightDirection );
                    break;

                // ���̑�
                case MME_VARREQ_TIME:
                    var->setFloat( param_.Time );
                    break;
                case MME_VARREQ_ELAPSEDTIME:
                    var->setFloat( param_.ElapsedTime );
                    break;
                case MME_VARREQ_VIEWPORTPIXELSIZE:
                    var->setVector( param_.ViewPixelSize );
                    break;
                case MME_VARREQ_MOUSEPOSITION:
                    var->setVector( param_.MousePosition );
                    break;
                case MME_VARREQ_LEFTMOUSEDOWN:
                    var->setVector( param_.LeftMouseDown );
                    break;
                case MME_VARREQ_MIDDLEMOUSEDOWN:
                    var->setVector( param_.MiddleMouseDown );
                    break;
                case MME_VARREQ_RIGHTMOUSEDOWN:
                    var->setVector( param_.RightMouseDown );
                    break;
            }
        }

    }

    //---------------------------------------------------------------------
    // �� Camera �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
    //---------------------------------------------------------------------
    void SceneShader::updateCameraParam( const LNCameraSceneParam& param_ )
    {
        MMEShaderVariable* sv;
        Graphics::IShaderVariable* var;
        
        ln_foreach( sv, mShaderVariableArray )
        {
            var = sv->Variable;

            switch ( sv->Request )
            {
                // �s��
                case MME_VARREQ_MATRIX_CAMERA_View:
                    var->setMatrix( *param_.Mat_CameraView );
                    break;
                case MME_VARREQ_MATRIX_CAMERA_Proj:
                    var->setMatrix( *param_.Mat_CameraProj );
                    break;
                case MME_VARREQ_MATRIX_CAMERA_ViewProj:
                    var->setMatrix( *param_.Mat_CameraViewProj );
                    break;

                // �t�s��
                case MME_VARREQ_MATRIX_CAMERA_View_I:
                    var->setMatrix( param_.Mat_CameraView_Inverse );
                    break;
                case MME_VARREQ_MATRIX_CAMERA_Proj_I:
                    var->setMatrix( param_.Mat_CameraProj_Inverse );
                    break;
                case MME_VARREQ_MATRIX_CAMERA_ViewProj_I:
                    var->setMatrix( param_.Mat_CameraViewProj_Inverse );
                    break;

                // �]�u�s��
                case MME_VARREQ_MATRIX_CAMERA_View_T:
                    var->setMatrix( param_.Mat_CameraView_Transpose );
                    break;
                case MME_VARREQ_MATRIX_CAMERA_Proj_T:
                    var->setMatrix( param_.Mat_CameraView_Transpose );
                    break;
                case MME_VARREQ_MATRIX_CAMERA_ViewProj_T:
                    var->setMatrix( param_.Mat_CameraView_Transpose );
                    break;
               
                // �t�s��~�]�u�s��
                case MME_VARREQ_MATRIX_CAMERA_View_IT:
                    var->setMatrix( param_.Mat_CameraView_InverseTranspose );
                    break;

                case MME_VARREQ_MATRIX_CAMERA_Proj_IT:
                    var->setMatrix( param_.Mat_CameraView_InverseTranspose );
                    break;

                case MME_VARREQ_MATRIX_CAMERA_ViewProj_IT:
                    var->setMatrix( param_.Mat_CameraView_InverseTranspose );
                    break;
                // �J�����̈ʒu�E����
                case MME_VARREQ_CAMERA_POSITION:
                    var->setVector( param_.CameraPosition );
                    break;
                case MME_VARREQ_CAMERA_DIRECTION:
                    var->setVector( param_.CameraDirection );
                    break;

                // ���ˉe�s��
                //case LN_VARREQ_MATRIX_CAMERA_ORTHOGRAPHIC:
                //    var->setMatrix( param_.Mat_CameraOrtho );
                //    break;

            }
        }
    }

    //---------------------------------------------------------------------
    // �� Node �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
    //---------------------------------------------------------------------
    LNRESULT SceneShader::updateNodeParam( const SceneNodeContext* param_ )// const NodeRenderParam& param_ )
    {
        MMEShaderVariable* sv;
        
        ln_foreach( sv, mShaderVariableArray )
        {
            // ���m�Ȕ͈͂���Ȃ����ǁA�����ł���������̉񐔂����炷���߂�
            if ( MME_VARREQ_GEOMETRY_MATRIX_BEGIN <= sv->Request && sv->Request <= MME_VARREQ_GEOMETRY_MATRIX_END )
            {
                // ���C�g�̏�񂪕K�v���A�Z�b�g����Ώۂ��z��̏ꍇ
                if ( sv->LightNum > 0 )
                {
                    // setMatrixArray() �Őݒ肷��K�v�����鍀��
                    if ( sv->LightParamIsMatrix )
                    {
                        Base::TempBuffer* buf = mManager->getTempBuffer();
                        LMatrix* matrices = reinterpret_cast< LMatrix* >( buf->lock( sizeof( LMatrix ) * sv->LightNum ) );
 
                        for ( lnU32 i = 0; i < sv->LightNum; ++i )
                            param_->getObjectMatrix( &matrices[ i ], sv->Request, i );

                        sv->Variable->setMatrixArray( matrices, sv->LightNum );

                        buf->unlock();
                    }
                    // setVectorArray() �Őݒ肷��K�v�����鍀��
                    else
                    {
                        const LightContextArray& lights = param_->getAffectLightArray();

                        Base::TempBuffer* buf = mManager->getTempBuffer();
                        LVector4* vectors = reinterpret_cast< LVector4* >( buf->lock( sizeof( LVector4 ) * sv->LightNum ) );
                        
                        switch ( sv->Request )
                        {
                            case MME_VARREQ_LIGHT_DIFFUSE:
                                for ( lnU32 i = 0; i < sv->LightNum; ++i )
                                    vectors[ i ] = static_cast< const LVector4& >( lights[ i ]->getDiffuseColor() );
                                break;
                            case MME_VARREQ_LIGHT_AMBIENT:
                                for ( lnU32 i = 0; i < sv->LightNum; ++i )
                                    vectors[ i ] = static_cast< const LVector4& >( lights[ i ]->getAmbientColor() );
                                break;
                            case MME_VARREQ_LIGHT_SPECULAR:
                                for ( lnU32 i = 0; i < sv->LightNum; ++i )
                                    vectors[ i ] = static_cast< const LVector4& >( lights[ i ]->getSpecularColor() );
                                break;
                            case MME_VARREQ_LIGHT_POSITION:
                                for ( lnU32 i = 0; i < sv->LightNum; ++i )
                                {
                                    const LVector3& v = lights[ i ]->getPosition();
                                    vectors[ i ].set( v.x, v.y, v.z, 0.0f );
                                }
                                break;
                            case MME_VARREQ_LIGHT_DIRECTION:
                                for ( lnU32 i = 0; i < sv->LightNum; ++i )
                                {
                                    const LVector3& v = lights[ i ]->getDirection();
                                    vectors[ i ].set( v.x, v.y, v.z, 0.0f );
                                }
                                break;
                        }
                        buf->unlock();
                    }

                }
                // ���C�g�ȊO
                else
                {
                    LMatrix mat;
                    if ( param_->getObjectMatrix( &mat, sv->Request, 0 ) )
                    {
                        sv->Variable->setMatrix( mat );
                    }
                }
            }
        }

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� Subset �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
    //---------------------------------------------------------------------
    void SceneShader::updateSubsetParam( const LNSubsetRenderParam& param_ )
    {
        MMEShaderVariable* sv;
        Graphics::IShaderVariable* var;

        LVector4 temp_vec;

        ln_foreach( sv, mShaderVariableArray )
        {
            var = sv->Variable;

            switch ( sv->Request )
            {
                // �f�B�t���[�Y�F�i�g�U���j
                case MME_VARREQ_OBJECT_DIFFUSE:
                    var->setVector( param_.Material.Diffuse );
                    break;

                // �A���r�G���g�F�i�����j
                case MME_VARREQ_OBJECT_AMBIENT:
                    var->setVector( param_.Material.Ambient );
                    break;

                // �G�~�b�V�����F�i���ˌ��j
                case MME_VARREQ_OBJECT_EMISSIVE:
                    var->setVector( param_.Material.Emissive );
                    break;

                // �X�y�L�����F�i���ʌ��j
                case MME_VARREQ_OBJECT_SPECULAR:
                    var->setVector( param_.Material.Specular );
                    break;

                // �X�y�L�����̋��x
                case MME_VARREQ_OBJECT_SPECULARPOWER:
                    var->setFloat( param_.Material.Power );
                    break;

                // �g�D�[���F
                case MME_VARREQ_OBJECT_TOONCOLOR:
                    var->setVector( param_.ToonColor );
                    break;

                // �֊s�F
                case MME_VARREQ_OBJECT_EDGECOLOR:
                    var->setVector( param_.EdgeColor );
                    break;

                // �}�e���A���ɐݒ肳��Ă���e�N�X�`��
                case MME_VARREQ_OBJECT_MATERIALTEXTURE:
                    if ( param_.Material.getTexture() )
                    {
                        var->setTexture( param_.Material.getTexture() );
                    }
                    // �e�N�X�`�����Ȃ���΃_�~�[��ݒ�
                    else
                    {
                        var->setTexture( mManager->getGraphicsManager()->getDummyTexture() );
                    }
                    break;

                // �}�e���A���ɐݒ肳��Ă���A�X�t�B�A�}�b�v�e�N�X�`��
                case MME_VARREQ_OBJECT_MATERIALSPHEREMAP:
                    if ( param_.Material.getSphereTexture() )
                    {
                        var->setTexture( param_.Material.getSphereTexture() );
                    }
                    // �e�N�X�`�����Ȃ���΃_�~�[��ݒ�
                    else
                    {
                        var->setTexture( mManager->getGraphicsManager()->getDummyTexture() );
                    }
                    break;

                // �s�����x
                case LN_VARREQ_OPACITY:
                {
                    var->setFloat( param_.Opacity );
                    break;
                }
                // ��Z����F
                case LN_VARREQ_COLOR_SCALE:
                {
                    var->setVector( param_.ColorScale );
                    break;
                }
                // �u�����h����F
                case LN_VARREQ_BLEND_COLOR:
                {
                    var->setVector( param_.BlendColor );
                    break;
                }
                // �F��
                case LN_VARREQ_TONE:
                {
                    var->setVector( param_.Tone );
                    break;
                }
                // �e�N�X�`�����W�ϊ��s��
                case LN_VARREQ_UVTRANSFORM:
                {
                    var->setMatrix( param_.UVTransform );
                    break;
                }
            }
        }
    }

    //---------------------------------------------------------------------
    // �� CONTROLOBJECT ��v�����Ă���ϐ��̒l�����ׂčX�V����
    //---------------------------------------------------------------------
    void SceneShader::updateControlObjectVariables()
    {
        //LN_PRINT_NOT_IMPL_FUNCTION;
    }

    //---------------------------------------------------------------------
    // �� �`��R�}���h�̍쐬
    //---------------------------------------------------------------------
    void SceneShader::makeDrawCommand( DrawCommandContainer* container_, SceneNode* object_, MMDPass pass_ )
    {
        // �I�u�W�F�N�g�P�ʂ̃p�����[�^�ݒ�R�}���h
        container_->add_SetObjectParamToShader( object_->getContext() );

        // �S�Ẵe�N�j�b�N�̂����A�p�X����v������̂����s����
        SceneShaderTechniqueArray::iterator itr = mTechniqueList.begin();
        SceneShaderTechniqueArray::iterator end = mTechniqueList.end();
        for ( ; itr!= end; ++itr )
        {
            if ( !(*itr)->isPostProcess() && (*itr)->getMMDPass() == pass_ )
            {
                (*itr)->makeDrawCommand( container_, object_ );
            }
        }
    }

    //---------------------------------------------------------------------
    // �� �`��R�}���h�̍쐬 ("ScriptExternal=Color" �̑O�܂�)
    //---------------------------------------------------------------------
    void SceneShader::makeDrawCommandPreExternal( DrawCommandContainer* container_, SceneNode* object_, MMDPass pass_ )
    {
        // �I�u�W�F�N�g�P�ʂ̃p�����[�^�ݒ�R�}���h
        container_->add_SetObjectParamToShader( object_->getContext() );

        // �S�Ẵe�N�j�b�N�̂����A�p�X����v������̂����s����
        SceneShaderTechniqueArray::iterator itr = mTechniqueList.begin();
        SceneShaderTechniqueArray::iterator end = mTechniqueList.end();
        for ( ; itr!= end; ++itr )
        {
            if ( (*itr)->isPostProcess() && (*itr)->getMMDPass() == pass_ )
            {
                (*itr)->makeDrawCommandPreExternal( container_, object_ );
            }
        }
    }

    //---------------------------------------------------------------------
    // �� �`��R�}���h�̍쐬 ("ScriptExternal=Color" ����I�[�܂�)
    //---------------------------------------------------------------------
    void SceneShader::makeDrawCommandPostExternal( DrawCommandContainer* container_, SceneNode* object_, MMDPass pass_ )
    {
        // �I�u�W�F�N�g�P�ʂ̃p�����[�^�ݒ�R�}���h
        container_->add_SetObjectParamToShader( object_->getContext() );

        // �S�Ẵe�N�j�b�N�̂����A�p�X����v������̂����s����
        SceneShaderTechniqueArray::iterator itr = mTechniqueList.begin();
        SceneShaderTechniqueArray::iterator end = mTechniqueList.end();
        for ( ; itr!= end; ++itr )
        {
            if ( (*itr)->isPostProcess() && (*itr)->getMMDPass() == pass_ )
            {
                (*itr)->makeDrawCommandPostExternal( container_, object_ );
            }
        }
    }

    //---------------------------------------------------------------------
    // �� SceneNode::setShader() �ł̐擪�ŌĂ΂��
    //---------------------------------------------------------------------
    void SceneShader::onSettingShader( SceneNode* node_ )
    {
        if ( isShareable() ) return;

        LN_THROW_InvalidCall( !mSelfSceneNode, Resource::String::ERR_SceneShader_onSettingShader );

        mSelfSceneNode = node_;
    }

    //---------------------------------------------------------------------
    // �� ���o��
    //---------------------------------------------------------------------
    void SceneShader::dumpInfo( FILE* stream_ )
    {
        FILE* fp = ( stream_ ) ? stream_ : stdout;

        ln_foreach( MMEShaderVariable* sv, mShaderVariableArray )
        {
            _ftprintf( fp, "[%s]\n", sv->Variable->getName() );
            _ftprintf( fp, "    Request : %d\n", sv->Request );
        }
    }

    //---------------------------------------------------------------------
    // �� �p�����[�^�Ȃǂ����ۂɐݒ肵�Ă���
    //---------------------------------------------------------------------
    LNRESULT SceneShader::_build()
    {
        LNRESULT lr;

        // SceneShader ���ō쐬�������\�[�X�̉��
        _release();

        //-----------------------------------------------------
        // �V�F�[�_�v���O�������̂��ׂĂ̕ϐ����`�F�b�N����

        u32 i = 0;
        Graphics::IShaderVariable* var;

        while ( true )
        {
            var = mShader->getVariableByIndex( i );
            if ( !var ) { 
            	break; }

            // �V�F�[�_�ϐ��B�Ƃ肠�����Ńt�H���g�l�����Ă���
            MMEShaderVariable* sv = LN_NEW MMEShaderVariable();
            sv->Variable            = var;
            sv->Request             = MME_VARREQ_NONE;
            sv->ControlObject       = NULL;
            sv->Texture             = NULL;
            sv->LightNum            = 0;
            sv->LightParamIsMatrix  = false;
            mShaderVariableArray.push_back( sv );

            // �Z�}���e�B�N�X�m�F�BCONTROLOBJECT �̓���Ȓl�������ꍇ�� sv �� ObjectName ���ɒl������
            MMEScriptOutput op1;
            MMEScriptClass  op2;
            MMEScriptOrder  op3;
            bool is_controllobject;
            SceneShaderUtil::checkVariableRequest(
                var,
                mManager->getSemanticsMap(),
                sv,
                &op1,
                &op2,
                &op3,
                &is_controllobject );
            if ( sv->Request == MME_VARREQ_STANDARDSGLOBAL )
            {
                mScriptOutput = op1;
                mScriptClass  = op2;
                mScriptOrder  = op3;
            }
            if ( is_controllobject )
            {
                mControlObjectVarArray.push_back( sv );
            }

            // ���׌y���̂��߁A�K�v�ȍs�񉉎Z���`�F�b�N����
            mWorldMatrixCalcMask |= SceneShaderUtil::checkMatrixMask( sv->Request );

            // �ϐ����Ƃ̑Ή��\�ɒǉ�
            mShaderVariableMap.insert( MMEShaderVariablePair( var->getName(), sv ) );

            // �K�v�ȃ��C�g�ő吔�`�F�b�N
            if ( sv->LightNum > mRequiredLightNum )
            {
                mRequiredLightNum = sv->LightNum;
            }

            ++i;
        }
                
        //-----------------------------------------------------
        // �e�N�j�b�N���쐬

        Graphics::IShaderTechnique* tech;
        i = 0;
        while ( true )
        {
            // STANDARDSGLOBAL �� Script �A�m�e�[�V�����Ńe�N�j�b�N�̌���������
            // �w�肳��Ă���ꍇ�͂����ŏ��Ԃ𑀍삷��

            tech = mShader->getTechniqueByIndex( i );
            if ( !tech ) { 
            	break; } 

            SceneShaderTechnique* sstech = LN_NEW SceneShaderTechnique();
            LN_CALL_R( sstech->initialize( this, tech ) );

            mTechniqueList.push_back( sstech );

            //-------------------------------------
            // �Ó����`�F�b�N
            // 
            if ( mScriptClass == MME_SCRCLS_object )
            {
                // �p�X�̃X�N���v�g�� Draw=Buffer �����s���Ă͂Ȃ�Ȃ�
                LN_THROW_InvalidCall(
                    !( sstech->getCommandExistFlags() & MME_SCEXIST_Draw_Buffer ),
                    Resource::String::ERR_SceneShader_build1 );
            }
            else if ( mScriptClass == MME_SCRCLS_scene )
            {
                // �p�X�̃X�N���v�g�� Draw=Geometry �����s���Ă͂Ȃ�Ȃ�
                LN_THROW_InvalidCall(
                    !( sstech->getCommandExistFlags() & MME_SCEXIST_Draw_Geometry ),
                    Resource::String::ERR_SceneShader_build2 );
            }

            if ( mScriptOrder == MME_SCRORDER_postprocess )
            {
                // ScriptExternal=Color ���K���K�v
                LN_THROW_InvalidCall(
                    sstech->getCommandExistFlags() & MME_SCEXIST_ScriptExternal_Color,
                    Resource::String::ERR_SceneShader_build3 );
            }
            else
            {
                // ScriptExternal=Color �������Ă͂Ȃ�Ȃ�
                LN_THROW_InvalidCall(
                    !( sstech->getCommandExistFlags() & MME_SCEXIST_ScriptExternal_Color ),
                    Resource::String::ERR_SceneShader_build4 );
            }

            

            ++i;
        }

        //-----------------------------------------------------
        

        


        //printf( "----------------------------------------\n" );
        //dumpInfo();
        //printf( "----------------------------------------\n" );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� ���\�[�X�̍č\�z
    //---------------------------------------------------------------------
    LNRESULT SceneShader::refreshResource( const LVector2& view_size_ )
    {
        LNRESULT lr = LN_OK;

        _releaseResource();

        mIsError = true;

        ln_foreach( MMEShaderVariable* sv, mShaderVariableArray )
        {
            // �e�N�X�`�����v������Ă��邩�`�F�b�N���č쐬����
            SceneShaderUtil::TextureResourceData resource_data;
            if ( SceneShaderUtil::checkTextureResourceData( sv, mManager->getAnnotationMap(), &resource_data ) )
            {
                // �쐬����ׂ��e�N�X�`���̃T�C�Y�𒲂ׂ�
                int width, height;
                SceneShaderUtil::checkTextureSize(
                    &width,
                    &height,
                    resource_data.Dimensions,
                    resource_data.ViewportRatio, 
					mManager->getScene()->getDefaultPane()->getViewSize() );

                //�e�N�X�`���쐬
                lr = mManager->createTexture(
                    &sv->Texture,
                    sv->Request,
                    resource_data.ResourceName.c_str(),
                    width,
                    height,
                    static_cast< lnU32 >( resource_data.Dimensions.z ),
                    resource_data.Miplevels,
                    resource_data.Format );
                if ( LN_FAILED( lr ) )
                {
                    mErrorMessage = _T( "Failed to create texture" );
                    return lr;
                }

                // ����ɍ쐬�ł����ꍇ�̓V�F�[�_�̕ϐ��ɐݒ肷�� (�[�x�o�b�t�@�̓e�N�X�`���Ƃ��č쐬����Ȃ��̂Őݒ肵�Ȃ�)
                if ( sv->Request != MME_VARREQ_RENDERDEPTHSTENCILTARGET )
                {
                    sv->Variable->setTexture( sv->Texture );
                }

                // �I�t�X�N���[��RT �̏ꍇ
                if ( sv->Request == MME_VARREQ_OFFSCREENRENDERTARGET )
                {
                    OffscreenScene::InitData data;
                    data.ClearColor             = resource_data.ClearColor;
                    data.ClearDepth             = resource_data.ClearDepth;
                    data.Description            = resource_data.Description;
                    data.DefaultEffectStrings   = resource_data.DefaultEffect;
                    data.RTTexture              = sv->Texture;
                    OffscreenScene* ofs = LN_NEW OffscreenScene( mManager, this );
                    LN_CALL_R( ofs->initialize( data ) );

                    mOffscreenSceneList.push_back( ofs );

                    // shared ���Ȃ��ꍇ�� SceneShader ���L�s��
                    if ( !sv->Variable->getTypeDesc().Shared )
                    {
                        mShareable = false;
                    }
                    // shared �����邪�A"self" ������ꍇ�����L�s��
                    else if ( sv->Variable->getTypeDesc().Shared && ofs->isThereSelf())
                    {
                        mShareable = false;
                    }
                }
            }
        }

        mIsError = false;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �ϐ����ɑΉ����� MMEShaderVariable ����������
    //---------------------------------------------------------------------
    MMEShaderVariable* SceneShader::findShaderVariable( const lnChar* name_ )
    {
        MMEShaderVariableMap::iterator it = mShaderVariableMap.find( name_ );
        if ( it != mShaderVariableMap.end() )
        {
            return it->second;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    // �� ���
    //---------------------------------------------------------------------
    void SceneShader::_release()
    {
        _releaseResource();

        // �e�N�j�b�N���
        ln_foreach( SceneShaderTechnique* tech, mTechniqueList )
        {
            SAFE_DELETE( tech );
        }
        mTechniqueList.clear();

        // �ϐ������
        ln_foreach( MMEShaderVariable* sv, mShaderVariableArray )
        {
            SAFE_DELETE( sv );
        }
        mShaderVariableArray.clear();

        mRequiredLightNum = 0;
    }

    //---------------------------------------------------------------------
    // �� ���\�[�X���
    //---------------------------------------------------------------------
    void SceneShader::_releaseResource()
    {
        ln_foreach( MMEShaderVariable* sv, mShaderVariableArray )
        {
            LN_SAFE_RELEASE( sv->Texture );
        }
        //mShaderVariableArray.clear(); // �����ł̓e�N�X�`���̉�������s���̂� clear �͂��Ȃ�

        ln_foreach( OffscreenScene* ofs, mOffscreenSceneList )
        {
            LN_SAFE_RELEASE( ofs );
        }
        mOffscreenSceneList.clear();
    }

//=============================================================================
// �� SceneShaderUtil
//=============================================================================

    //---------------------------------------------------------------------
    // �� �V�F�[�_�ϐ��̃Z�}���e�B�N�X�𒲂ׂđΉ�����v�����ڂ�Ԃ�
    //---------------------------------------------------------------------
    void SceneShaderUtil::checkVariableRequest(
        Graphics::IShaderVariable* var_,
        const MMESemanticsMap& semantics_map_,
        MMEShaderVariable* sv_,
        MMEScriptOutput* script_output_,
        MMEScriptClass* script_class_,
        MMEScriptOrder* script_order_,
        bool* is_controllobject_ )
    {
        const lnChar*       name;                           // �V�F�[�_���當������󂯎��ϐ�
        lnChar              temp[ MAX_SEMANTICS_NAME_LEN ]; // ������̃R�s�[�Ɏg���ꎞ�o�b�t�@
        MMEVariableRequest  req = MME_VARREQ_NONE;          // �߂�l

        MMESemanticsMap::const_iterator it; 
        MMESemanticsMap::const_iterator end = semantics_map_.end();
        const LNShaderVariableTypeDesc& var_desc = var_->getTypeDesc();  // �ϐ��̌^���

        *is_controllobject_ = false;

        name = var_->getSemanticName();

        // �Z�}���e�B�N�X������ꍇ
        if ( name )
        {
            // �啶�������āA�Ή��\����������
            _tcsncpy( temp, name, MAX_SEMANTICS_NAME_LEN );
            Base::StringUtil::toUpper( temp );
            it = semantics_map_.find( temp );
            if ( it != end )
            {
                int r;

                switch ( it->second )
                {
                    /////////////////////////////////////// �ϊ��s��
                    case MME_SEMANTIC_WORLD:
                        req = MME_VARREQ_MATRIX_World;
                        break;
                    case MME_SEMANTIC_VIEW:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_View : MME_VARREQ_MATRIX_LIGHT_View;
                        break;
                    case MME_SEMANTIC_PROJECTION:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_Proj : MME_VARREQ_MATRIX_LIGHT_Proj;
                        break;
                    case MME_SEMANTIC_WORLDVIEW:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_WorldView : MME_VARREQ_MATRIX_LIGHT_WorldView;
                        break;
                    case MME_SEMANTIC_VIEWPROJECTION:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_ViewProj : MME_VARREQ_MATRIX_LIGHT_ViewProj;
                        break;
                    case MME_SEMANTIC_WORLDVIEWPROJECTION:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_WorldViewProj : MME_VARREQ_MATRIX_LIGHT_WorldViewProj;
                        break;

                    ///////////////////////////////////////  �t�s��
                    case MME_SEMANTIC_WORLD_I:
                        req = MME_VARREQ_MATRIX_World_I;
                        break;
                    case MME_SEMANTIC_VIEW_I:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_View_I : MME_VARREQ_MATRIX_LIGHT_View_I;
                        break;
                    case MME_SEMANTIC_PROJECTION_I:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_Proj_I : MME_VARREQ_MATRIX_LIGHT_Proj_I;
                        break;
                    case MME_SEMANTIC_WORLDVIEW_I:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_WorldView_I : MME_VARREQ_MATRIX_LIGHT_WorldView_I;
                        break;
                    case MME_SEMANTIC_VIEWPROJECTION_I:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_ViewProj_I : MME_VARREQ_MATRIX_LIGHT_ViewProj_I;
                        break;
                    case MME_SEMANTIC_WORLDVIEWPROJECTION_I:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I : MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I;
                        break;

                    ///////////////////////////////////////  �]�u�s��
                    case MME_SEMANTIC_WORLD_T:
                        req = MME_VARREQ_MATRIX_World_T;
                        break;
                    case MME_SEMANTIC_VIEW_T:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_View_T : MME_VARREQ_MATRIX_LIGHT_View_T;
                        break;
                    case MME_SEMANTIC_PROJECTION_T:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_Proj_T : MME_VARREQ_MATRIX_LIGHT_Proj_T;
                        break;
                    case MME_SEMANTIC_WORLDVIEW_T:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_WorldView_T : MME_VARREQ_MATRIX_LIGHT_WorldView_T;
                        break;
                    case MME_SEMANTIC_VIEWPROJECTION_T:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_ViewProj_T : MME_VARREQ_MATRIX_LIGHT_ViewProj_T;
                        break;
                    case MME_SEMANTIC_WORLDVIEWPROJECTION_T:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T : MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T;
                        break;

                    ///////////////////////////////////////  �t�s��~�]�u�s��
                    case MME_SEMANTIC_WORLD_IT:
                        req = MME_VARREQ_MATRIX_World_IT;
                        break;
                    case MME_SEMANTIC_VIEW_IT:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_View_IT : MME_VARREQ_MATRIX_LIGHT_View_IT;
                        break;
                    case MME_SEMANTIC_PROJECTION_IT:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_Proj_IT : MME_VARREQ_MATRIX_LIGHT_Proj_IT;
                        break;
                    case MME_SEMANTIC_WORLDVIEW_IT:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_WorldView_IT : MME_VARREQ_MATRIX_LIGHT_WorldView_IT;
                        break;
                    case MME_SEMANTIC_VIEWPROJECTION_IT:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_ViewProj_IT : MME_VARREQ_MATRIX_LIGHT_ViewProj_IT;
                        break;
                    case MME_SEMANTIC_WORLDVIEWPROJECTION_IT:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT : MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT;
                        break;

                    /////////////////////////////////////// �F
                    case MME_SEMANTIC_DIFFUSE:
                        r = checkAnnotationGeometryOrLight( var_ );
                        if ( r != 0 )
                        {
                            req = ( r == 1 ) ? MME_VARREQ_OBJECT_DIFFUSE : MME_VARREQ_LIGHT_DIFFUSE;
                        }
                        break;

                    case MME_SEMANTIC_AMBIENT:
                        r = checkAnnotationGeometryOrLight( var_ );
                        if ( r != 0 )
                        {
                            req = ( r == 1 ) ? MME_VARREQ_OBJECT_AMBIENT : MME_VARREQ_LIGHT_AMBIENT;
                        }
                        break;

                    case MME_SEMANTIC_EMISSIVE:
                        req = MME_VARREQ_OBJECT_EMISSIVE;
                        break;

                    case MME_SEMANTIC_SPECULAR:
                        r = checkAnnotationGeometryOrLight( var_ );
                        if ( r != 0 )
                        {
                            req = ( r == 1 ) ? MME_VARREQ_OBJECT_SPECULAR : MME_VARREQ_LIGHT_SPECULAR;
                        }
                        break;

                    case MME_SEMANTIC_SPECULARPOWER:
                        req = MME_VARREQ_OBJECT_SPECULARPOWER;
                        break;

                    case MME_SEMANTIC_TOONCOLOR:
                        req = MME_VARREQ_OBJECT_TOONCOLOR;
                        break;

                    case MME_SEMANTIC_EDGECOLOR:
                        req = MME_VARREQ_OBJECT_EDGECOLOR;
                        break;

                    /////////////////////////////////////// �ʒu�E����
                    case MME_SEMANTIC_POSITION:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_CAMERA_POSITION : MME_VARREQ_LIGHT_POSITION;
                        break;

                    case MME_SEMANTIC_DIRECTION:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_CAMERA_DIRECTION : MME_VARREQ_LIGHT_DIRECTION;
                        break;
                    
                    /////////////////////////////////////// �}�e���A���̃e�N�X�`��
                    case MME_SEMANTIC_MATERIALTEXTURE:
                        req = MME_VARREQ_OBJECT_MATERIALTEXTURE;
                        break;

                    case MME_SEMANTIC_MATERIALSPHEREMAP:
                        req = MME_VARREQ_OBJECT_MATERIALSPHEREMAP;
                        break;

                    /////////////////////////////////////// misc
                    case MME_SEMANTIC_VIEWPORTPIXELSIZE:
                        req = MME_VARREQ_VIEWPORTPIXELSIZE;
                        break;

                    case MME_SEMANTIC_TIME:
                        req = MME_VARREQ_TIME;
                        break;

                    case MME_SEMANTIC_ELAPSEDTIME:
                        req = MME_VARREQ_ELAPSEDTIME;
                        break;

                    /////////////////////////////////////// �}�E�X
                    case MME_SEMANTIC_MOUSEPOSITION:
                        req = MME_VARREQ_MOUSEPOSITION;
                        break;
                    case MME_SEMANTIC_LEFTMOUSEDOWN:
                        req = MME_VARREQ_LEFTMOUSEDOWN;
                        break;
                    case MME_SEMANTIC_MIDDLEMOUSEDOWN:
                        req = MME_VARREQ_MIDDLEMOUSEDOWN;
                        break;
                    case MME_SEMANTIC_RIGHTMOUSEDOWN:
                        req = MME_VARREQ_RIGHTMOUSEDOWN;
                        break;

                    /////////////////////////////////////// CONTROLOBJECT
                    case MME_SEMANTIC_CONTROLOBJECT:
                    {
                        const char* name = NULL;
                        const char* item = NULL;

                        Graphics::IShaderVariable* name_anno = getAnnotationByName( var_, _T( "name" ) );
                        if ( !name_anno ) // name �͕K�{
                        {
                            break;
                        }
                        name_anno->getString( &name );

                        // ����Ώۂ̖��O�Ƃ��Ċo���Ă���
                        sv_->ObjectName = name;

                        // "item" ���`�F�b�N
                        Graphics::IShaderVariable* anno = getAnnotationByName( var_, _T( "item" ) );
                        if ( anno )
                        {
                            anno->getString( &item );
                            if ( item )
                            {
                                if ( _tcsicmp( item, _T( "X" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_X;
                                }
                                else if ( _tcsicmp( item, _T( "Y" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Y;
                                }
                                else if ( _tcsicmp( item, _T( "Y" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Z;
                                }
                                else if ( _tcsicmp( item, _T( "XYZ" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_XYZ;
                                }
                                else if ( _tcsicmp( item, _T( "Rx" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Rx;
                                }
                                else if ( _tcsicmp( item, _T( "Ry" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Ry;
                                }
                                else if ( _tcsicmp( item, _T( "Rz" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Rz;
                                }
                                else if ( _tcsicmp( item, _T( "Rxyz" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Rxyz;
                                }
                                else if ( _tcsicmp( item, _T( "Si" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Si;
                                }
                                else if ( _tcsicmp( item, _T( "Tr" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Tr;
                                }
                                else if ( var_desc.Type == LN_SVT_FLOAT )
                                {
                                    // �^�� float �̏ꍇ�͕\��
                                    if ( var_desc.Class == LN_SVC_SCALAR )
                                    {
                                        req = MME_VARREQ_CONTROLOBJECT_MorphBlend;
                                        // ����Ώۂ̖��O�Ƃ��Ċo���Ă���
                                        sv_->ItemName = item;
                                    }
                                    // float3 4 �̏ꍇ�̓{�[���ʒu
                                    else if ( var_desc.Class == LN_SVC_VECTOR && var_desc.Columns >= 3 )
                                    {
                                        req = MME_VARREQ_CONTROLOBJECT_BoneOffset;
                                        sv_->ItemName = item;
                                    }
                                    // matrix4x4 �̏ꍇ�̓{�[���s��
                                    //else if ( ( var_desc.Class == LN_SVC_MATRIX_ROWS || var_desc.Class == LN_SVC_MATRIX_COLUMNS )
                                    else if ( var_desc.Class == LN_SVC_MATRIX
                                        && var_desc.Columns == 4 && var_desc.Rows == 4 )
                                    {
                                        req = MME_VARREQ_CONTROLOBJECT_BoneMatrix;
                                        sv_->ItemName = item;
                                    }
                                }
                            } // if ( item )
                        } // if ( anno ) 

                        // �����܂łł܂� req �����܂��ĂȂ��ꍇ
                        if ( req == MME_VARREQ_NONE )
                        {
                            // �^�� bool �̏ꍇ
                            if ( var_desc.Class == LN_SVC_SCALAR && var_desc.Type == LN_SVT_BOOL )
                            {
                                req = MME_VARREQ_CONTROLOBJECT_Hide;
                            }
                            else if ( var_desc.Type == LN_SVT_FLOAT )
                            {
                                // float �̏ꍇ
                                if ( var_desc.Class == LN_SVC_SCALAR )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Scale;
                                }
                                // float3 or 4 �̏ꍇ
                                else if ( var_desc.Class == LN_SVC_VECTOR && var_desc.Columns >= 3 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Position;
                                }
                                // matrix4x4 �̏ꍇ
                                //else if ( ( var_desc.Class == LN_SVC_MATRIX_ROWS || var_desc.Class == LN_SVC_MATRIX_COLUMNS )
                                else if ( var_desc.Class == LN_SVC_MATRIX
                                    && var_desc.Columns == 4 && var_desc.Rows == 4 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_World;
                                }
                            }
                        }

                        // �����܂łŗv�����ڂ��������Ă�����A�L���� CONTROLOBJECT
                        if ( req != MME_VARREQ_NONE )
                        {
                            *is_controllobject_ = true;
                        }           

                        break;
                    }


                    /////////////////////////////////////// �e�N�X�`���֘A
                    case MME_SEMANTIC_RENDERCOLORTARGET:
                        req = MME_VARREQ_RENDERCOLORTARGET;
                        break;
                    case MME_SEMANTIC_RENDERDEPTHSTENCILTARGET:
                        req = MME_VARREQ_RENDERDEPTHSTENCILTARGET;
                        break;
                    case MME_SEMANTIC_ANIMATEDTEXTURE:
                        printf( "ANIMATEDTEXTURE �Z�}���e�B�N�X�͖��Ή��ł��B\n" );
                        req = MME_VARREQ_ANIMATEDTEXTURE;
                        break;
                    case MME_SEMANTIC_OFFSCREENRENDERTARGET:
                        req = MME_VARREQ_OFFSCREENRENDERTARGET;
                        break;
                    case MME_SEMANTIC_TEXTUREVALUE:
                        printf( "TEXTUREVALUE �Z�}���e�B�N�X�͖��Ή��ł��B\n" );
                        req = MME_VARREQ_TEXTUREVALUE;
                        break;

                    /////////////////////////////////////// STANDARDSGLOBAL
                    case MME_SEMANTIC_STANDARDSGLOBAL:
                    {
                        req = MME_VARREQ_STANDARDSGLOBAL;

                        const lnChar* script_class = NULL;
                        const lnChar* script_order = NULL;
                        const lnChar* script = NULL;

                        Graphics::IShaderVariable* anno = NULL;
                        anno = getAnnotationByName( var_, _T( "ScriptClass" ) );
                        if ( anno ) { 
                        	anno->getString( &script_class ); }

                        anno = getAnnotationByName( var_, _T( "ScriptOrder" ) );
                        if ( anno ) { 
                        	anno->getString( &script_order ); }

                        anno = getAnnotationByName( var_, _T( "Script" ) );
                        if ( anno ) { 
                        	anno->getString( &script ); }


                        *script_output_ = MME_SCROUT_color;
                        *script_class_ = MME_SCRCLS_object;
                        *script_order_ = MME_SCRORDER_standard;

                        // ���Ńf�t�H���g�l�Ƃ��đ�����Ă�̂ŃR�����g�A�E�g
                        //if ( stricmp( script_class, "object" ) ) { MME_SCRCLS_object; }
                        if      ( _tcsicmp( script_class, _T( "scene" ) ) == 0 )         { 
                        	*script_class_ = MME_SCRCLS_scene; }
                        else if ( _tcsicmp( script_class, _T("sceneorobject" ) ) == 0 ) { 
                        	*script_class_ = MME_SCRCLS_sceneorobject; }

                        //if ( stricmp( script_order, "object" ) ) { MME_SCRORDER_standard; }
                        if      ( _tcsicmp( script_order, _T("preprocess" ) ) == 0 )    { 
                        	*script_order_ = MME_SCRORDER_preprocess; }
                        else if ( _tcsicmp( script_order, _T("postprocess" ) ) == 0 )   { 
                        	*script_order_ = MME_SCRORDER_postprocess; }


                        if ( script )
                        {
                            printf( "STANDARDSGLOBAL �� Script �A�m�e�[�V�����͖��Ή��ł��B\n" );
                            // Script �ł͎g�p����e�N�j�b�N�̌����������w�肷��
                            
                            LN_PRINT_NOT_IMPL_FUNCTION;
                        }

                        break;
                    }

                    /////////////////////////////////////// �Ǝ��d�l����
                    /*
                    case LN_SEMANTIC_ORTHOGRAPHIC:
                    {
                        req = ( checkAnnotationCameraOrLight( var ) ) ? LN_VARREQ_MATRIX_CAMERA_ORTHOGRAPHIC : LN_VARREQ_MATRIX_LIGHT_ORTHOGRAPHIC;
                        break;
                    }
                    */
                    case LN_SEMANTIC_PROJECTION2D:
                    {
                        req = LN_VARREQ_MATRIX_PROJECTION2D;
                        break;
                    }
                    case LN_SEMANTIC_OPACITY:
                    {
                        req = LN_VARREQ_OPACITY;
                        break;
                    }
                    case LN_SEMANTIC_COLOR_SCALE:
                    {
                        req = LN_VARREQ_COLOR_SCALE;
                        break;
                    }
                    case LN_SEMANTIC_BLEND_COLOR:
                    {
                        req = LN_VARREQ_BLEND_COLOR;
                        break;
                    }
                    case LN_SEMANTIC_TONE:
                    {
                        req = LN_VARREQ_TONE;
                        break;
                    }
                    case LN_SEMANTIC_UVTRANSFORM:
                    {
                        req = LN_VARREQ_UVTRANSFORM;
                        break;
                    }

                } // switch ( it->second )
                
            } // if ( it != end ) // �Z�}���e�B�N�X���������đΉ�����l�����������ꍇ
        }

        // �܂����܂��Ă��Ȃ��ꍇ�̓Z�}���e�B�N�X�����e�N�X�`���̉\���𒲂ׂ�
        if ( req == MME_VARREQ_NONE )
        {
            if ( var_desc.Class == LN_SVC_OBJECT && var_desc.Type == LN_SVT_TEXTURE )
            {
                req = MME_VARREQ_TEXTURE;
            }
        }

        // ���C�g�֌W�̏ꍇ�͔z��̗v�f�� = �K�v�ȃ��C�g�̐����m�F����
        sv_->LightParamIsMatrix = false;
        switch ( req )
        {
            case MME_VARREQ_MATRIX_LIGHT_WorldView:
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj:
            case MME_VARREQ_MATRIX_LIGHT_View:
            case MME_VARREQ_MATRIX_LIGHT_Proj:
            case MME_VARREQ_MATRIX_LIGHT_ViewProj:
            case MME_VARREQ_MATRIX_LIGHT_WorldView_I:
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I:
            case MME_VARREQ_MATRIX_LIGHT_View_I:
            case MME_VARREQ_MATRIX_LIGHT_Proj_I:
            case MME_VARREQ_MATRIX_LIGHT_ViewProj_I:
            case MME_VARREQ_MATRIX_LIGHT_WorldView_T:
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T:
            case MME_VARREQ_MATRIX_LIGHT_View_T:
            case MME_VARREQ_MATRIX_LIGHT_Proj_T:
            case MME_VARREQ_MATRIX_LIGHT_ViewProj_T:
            case MME_VARREQ_MATRIX_LIGHT_WorldView_IT:
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT:
            case MME_VARREQ_MATRIX_LIGHT_View_IT:
            case MME_VARREQ_MATRIX_LIGHT_Proj_IT:
            case MME_VARREQ_MATRIX_LIGHT_ViewProj_IT:
                sv_->LightParamIsMatrix = true;
            case MME_VARREQ_LIGHT_DIFFUSE:
            case MME_VARREQ_LIGHT_AMBIENT:
            case MME_VARREQ_LIGHT_SPECULAR:
            case MME_VARREQ_LIGHT_POSITION:
            case MME_VARREQ_LIGHT_DIRECTION:
                sv_->LightNum = var_->getTypeDesc().Elements;   // �z��łȂ��ꍇ�� 0 
                break;
        }

        // �v�����ڂ�Ԃ�
        sv_->Request = req;
    }

    //---------------------------------------------------------------------
    // �� �ϐ��� "Object" �A�m�e�[�V�����𒲂ׂāA"Light" �̏ꍇ�� fales�A����ȊO�� true ��Ԃ�
    //---------------------------------------------------------------------
    bool SceneShaderUtil::checkAnnotationCameraOrLight( Graphics::IShaderVariable* var_ )
    {
        Graphics::IShaderVariable* anno = getAnnotationByName( var_, _T( "Object" ) );
        if ( !anno ) { 
        	return true; }

        const lnChar* name;
        anno->getString( &name );

        if ( !name ) { 
        	return true; }

        if ( _tcsicmp( name, _T( "Light" ) ) == 0 )
        {
            return false;
        }
        return true;
    }

    //---------------------------------------------------------------------
    // �� �ϐ��� "Object" �A�m�e�[�V�����𒲂ׂāA"Geometry" �̏ꍇ�� 1�A"Light" �̏ꍇ�� 2�A����ȊO�� 0 ��Ԃ�
    //---------------------------------------------------------------------
    int SceneShaderUtil::checkAnnotationGeometryOrLight( Graphics::IShaderVariable* var_ )
    {
        Graphics::IShaderVariable* anno = getAnnotationByName( var_, _T( "Object" ) );
        if ( !anno ) { 
        	return 0; }

        const lnChar* name;
        anno->getString( &name );

        if ( !name ) { 
        	return 0; }

        if ( _tcsicmp( name, _T( "Geometry" ) ) == 0 )
        {
            return 1;
        }
        if ( _tcsicmp( name, _T( "Light" ) ) == 0 )
        {
            return 2;
        }
        return 0;
    }

    //---------------------------------------------------------------------
    // �� ���O���w�肵�ăA�m�e�[�V�������������� (�啶���������̋�ʖ���)
    //---------------------------------------------------------------------
    Graphics::IShaderVariable* SceneShaderUtil::getAnnotationByName( Graphics::IShaderVariable* var_, const lnChar* name_ )
    {
        Graphics::IShaderVariable* anno;
        u32 idx = 0;
        while ( true )
        {
            anno = var_->getAnnotationByIndex( idx );
            if ( !anno ) { 
            	break; }

            if ( _tcsicmp( anno->getName(), name_ ) == 0 )
            {
                return anno;
            }
            ++idx;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    // �� �v�����ڂ���A�v�Z����K�v�̂���s��}�X�N�̃r�b�g����쐬���ĕԂ�
    //---------------------------------------------------------------------
    lnU32 SceneShaderUtil::checkMatrixMask( MMEVariableRequest req_ )
    {
        u32 flags = 0;

        switch ( req_ ) 
        {
            case MME_VARREQ_MATRIX_CAMERA_WorldView:        
            	flags = MME_WMCF_WORLD_VIEW_CAMERA; break;
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj:    
            	flags = MME_WMCF_WORLD_VIEW_PROJ_CAMERA; break;
            case MME_VARREQ_MATRIX_LIGHT_WorldView:         
            	flags = MME_WMCF_WORLD_VIEW_LIGHT; break;
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj:     
            	flags = MME_WMCF_WORLD_VIEW_PROJ_LIGHT; break;

            case MME_VARREQ_MATRIX_World_I:                 
            	flags = MME_WMCF_WORLD_I; break;
            case MME_VARREQ_MATRIX_CAMERA_WorldView_I:      
            	flags = MME_WMCF_WORLD_VIEW_CAMERA_I      | MME_WMCF_WORLD_VIEW_CAMERA; break;
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I:  
            	flags = MME_WMCF_WORLD_VIEW_PROJ_CAMERA_I | MME_WMCF_WORLD_VIEW_PROJ_CAMERA; break;
            case MME_VARREQ_MATRIX_LIGHT_WorldView_I:       
            	flags = MME_WMCF_WORLD_VIEW_LIGHT_I       | MME_WMCF_WORLD_VIEW_LIGHT; break;
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I:   
            	flags = MME_WMCF_WORLD_VIEW_PROJ_LIGHT_I  | MME_WMCF_WORLD_VIEW_PROJ_LIGHT; break;

            case MME_VARREQ_MATRIX_World_T:                 
            	flags = MME_WMCF_WORLD_T; break;
            case MME_VARREQ_MATRIX_CAMERA_WorldView_T:      
            	flags = MME_WMCF_WORLD_VIEW_CAMERA_T      | MME_WMCF_WORLD_VIEW_CAMERA; break;
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T:  
            	flags = MME_WMCF_WORLD_VIEW_PROJ_CAMERA_T | MME_WMCF_WORLD_VIEW_PROJ_CAMERA; break;
            case MME_VARREQ_MATRIX_LIGHT_WorldView_T:       
            	flags = MME_WMCF_WORLD_VIEW_LIGHT_T       | MME_WMCF_WORLD_VIEW_LIGHT; break;
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T:   
            	flags = MME_WMCF_WORLD_VIEW_PROJ_LIGHT_T  | MME_WMCF_WORLD_VIEW_PROJ_LIGHT; break;

            case MME_VARREQ_MATRIX_World_IT:                
            	flags = MME_WMCF_WORLD_I                  | MME_WMCF_WORLD_T                  | MME_WMCF_WORLD_IT; break;
            case MME_VARREQ_MATRIX_CAMERA_WorldView_IT:     
            	flags = MME_WMCF_WORLD_VIEW_CAMERA_I      | MME_WMCF_WORLD_VIEW_CAMERA_T      | MME_WMCF_WORLD_VIEW_CAMERA_IT; break;
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT: 
            	flags = MME_WMCF_WORLD_VIEW_PROJ_CAMERA_I | MME_WMCF_WORLD_VIEW_PROJ_CAMERA_T | MME_WMCF_WORLD_VIEW_PROJ_CAMERA_IT; break;
            case MME_VARREQ_MATRIX_LIGHT_WorldView_IT:      
            	flags = MME_WMCF_WORLD_VIEW_LIGHT_I       | MME_WMCF_WORLD_VIEW_LIGHT_T       | MME_WMCF_WORLD_VIEW_LIGHT_IT; break;
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT:  
            	flags = MME_WMCF_WORLD_VIEW_PROJ_LIGHT_I  | MME_WMCF_WORLD_VIEW_PROJ_LIGHT_T  | MME_WMCF_WORLD_VIEW_PROJ_LIGHT_IT; break;
        }
        return flags;
    }

    //---------------------------------------------------------------------
    // �� �e�N�X�`�����\�[�X�̏���Ԃ�
    //---------------------------------------------------------------------
    bool SceneShaderUtil::checkTextureResourceData(
        MMEShaderVariable* sv_,
        const MMEAnnotationMap& annotation_map_,
        TextureResourceData* data_ )
    {
        MMEVariableRequest req = sv_->Request;

        // �e�N�X�`�����쐬����K�v������ꍇ
        if ( needCreateTexture( req ) )
        {
            //-------------------------------------------------
            // �ϐ��̏����ƃf�t�H���g�l

            // ����Ȃ�ɋ���
            const char* resource_type = NULL;
            const char* resource_name = NULL;
            int width = 0;
            int height = 0;
            int depth = 0;
            LVector4 dimensions;
            LVector4 viewport_ratio( 1.0f, 1.0f, 0, 0 );
            const char* format = NULL;
            int miplevels = 0;
            LNSurfaceFormat lnformat = LN_FMT_A8R8G8B8;

            // ANIMATEDTEXTURE �̂�
            lnfloat offset = 0;     // �A�j���[�V�����̊J�n���Ԃ��炵��
            lnfloat speed = 1.0f;
            const char* seek_variable = NULL;

            // OFFSCREENRENDERTARGET �̂�
            LVector4 clear_color;
            lnfloat clear_depth = 1.0f;
            bool anti_alias = false;
            const char* description = NULL;
            const char* default_effect = NULL;

            //-------------------------------------------------
            // �A�m�e�[�V������ǂ�ł���
            Graphics::IShaderVariable* anno;
            MMEAnnotationMap::const_iterator it;
            MMEAnnotationMap::const_iterator end = annotation_map_.end();
            lnChar temp[ MAX_SEMANTICS_NAME_LEN ];
            u32 idx = 0;
            bool be_create = false;

            while ( true )
            {
                anno = sv_->Variable->getAnnotationByIndex( idx );
                if ( !anno ) break;

                // ��r�̂��߁A���O��啶����
                _tcsncpy( temp, anno->getName(), MAX_SEMANTICS_NAME_LEN );
                Base::StringUtil::toUpper( temp );

                it = annotation_map_.find( temp );
                if ( it != end )
                {
                    switch ( it->second )
                    {
                        // �ʏ�A�����_�[�^�[�Q�b�g�A�[�x�o�b�t�@
                        case MME_ANNOTATION_ResourceType:
                            anno->getString( &resource_type );
                            break;

                        case MME_ANNOTATION_ResourceName:
                            anno->getString( &resource_name );
                            be_create = true;
                            break;
                        case MME_ANNOTATION_Width:
                            anno->getInt( &width );
                            be_create = true;
                            break;
                        
                        case MME_ANNOTATION_Height:
                            anno->getInt( &height );
                            be_create = true;
                            break;
                        
                        case MME_ANNOTATION_Depth:
                            anno->getInt( &depth );
                            be_create = true;
                            break;
                        
                        case MME_ANNOTATION_Dimensions:
                            anno->getVector( &dimensions );
                            be_create = true;
                            break;
                        
                        case MME_ANNOTATION_ViewportRatio:
                            anno->getVector( &viewport_ratio );
                            be_create = true;
                            break;
                        
                        case MME_ANNOTATION_Format:
                            anno->getString( &format );
                            break;
                        
                        case MME_ANNOTATION_Miplevels:
                            anno->getInt( &miplevels );
                            break;
                        
                        case MME_ANNOTATION_Levels:
                            anno->getInt( &miplevels );
                            break;
                        
                        // �A�j���[�V�����e�N�X�`��
                        case MME_ANNOTATION_Offset:
                            anno->getFloat( &offset );
                            break;
                        
                        case MME_ANNOTATION_Speed:
                            anno->getFloat( &speed );
                            break;
                        
                        case MME_ANNOTATION_SeekVariable:
                            anno->getString( &seek_variable );
                            break;
                        
                        // �I�t�X�N���[�� RT
                        case MME_ANNOTATION_ClearColor:
                            anno->getVector( &clear_color );
                            break;
                        
                        case MME_ANNOTATION_ClearDepth:
                            anno->getFloat( &clear_depth );
                            break;
                        
                        case MME_ANNOTATION_AntiAlias:
                            anno->getBool( &anti_alias );
                            break;

                        case MME_ANNOTATION_Description:
                            anno->getString( &description );
                            break;

                        case MME_ANNOTATION_DefaultEffect:
                            anno->getString( &default_effect );
                            break;
                    }
                }

                ++idx;
            };

            //-------------------------------------------------
            // �L���`�F�b�N

            // �K�v�ȃA�m�e�[�V�������ЂƂ��w�肳��Ă��Ȃ�
            if ( !be_create ) return false;

            // ResourceType ���ȗ�����Ă�ꍇ�� 2D
            if ( !resource_type )
            {
                resource_type = _T( "2D" );
            }

            // �s�N�Z���t�H�[�}�b�g  �ȗ�����Ă���ꍇ�� A8R8G8B8
            if ( !format )
            {
                format = _T( "A8R8G8B8" );
            }
            lnformat = Graphics::Util::convertFMTStringToLNFMT( format );
            

            //-------------------------------------------------
            // �T�C�Y����

            // �T�C�Y���w�肳��Ă��Ȃ���� dimensions �̒l���g��
            if ( width == 0 || height == 0 )
            {
                width  = static_cast< int >( dimensions.x );
                height = static_cast< int >( dimensions.y );
            }
            if ( depth == 0 )
            {
                depth = static_cast< int >( dimensions.z );
            }

            //-------------------------------------------------
            // 
            
            data_->ResourceName = resource_name;
            data_->Dimensions.set( (lnfloat)width, (lnfloat)height, (lnfloat)depth );
            data_->ViewportRatio.set( viewport_ratio.x, viewport_ratio.y );
            data_->Miplevels = miplevels;
            data_->Format = lnformat;

            // �A�j���[�V�����e�N�X�`���p
            data_->Offset = offset;
            data_->Speed = speed;
            data_->SeekVariable = seek_variable;

            // �I�t�X�N���[��RT�p
            data_->ClearColor = clear_color;
            data_->ClearDepth = clear_depth;
            data_->AntiAlias = anti_alias;
            data_->Description = description;
            data_->DefaultEffect = default_effect;

           
            return true;
        } 

        return false;
    }

    //---------------------------------------------------------------------
    // �� �e�N�X�`�����\�[�X�̏�񂩂�A�e�N�X�`���T�C�Y��Ԃ�
    //---------------------------------------------------------------------
    void SceneShaderUtil::checkTextureSize(
        int* width_,
        int* height_,
        const LVector3& dimensions_,
        const LVector2& viewport_ratio_,
        const LVector2& viewport_size_ )
    {
        // dimensions_ ���L���Ȃ炻�̒l
        if ( dimensions_.x > 0 && dimensions_.y > 0 )
        {
            *width_  = static_cast< int >( dimensions_.x );
            *height_ = static_cast< int >( dimensions_.y );
        }
        // dimensions �ɒl���Ȃ��ꍇ�̓r���[�|�[�g�T�C�Y����
        else if ( viewport_ratio_.x >= 0.0f && viewport_ratio_.y >= 0.0f )
        {
            *width_  = static_cast< int >( viewport_size_.x * viewport_ratio_.x );
            *height_ = static_cast< int >( viewport_size_.y * viewport_ratio_.y );
        }
        // ����ł������������� 64 * 64
        else
        {
            *width_ = *height_ = 64;
        }
    }

    //---------------------------------------------------------------------
    // �� �`��T�u�Z�b�g�����������񂩂�A�T�u�Z�b�g�ԍ��̔z����쐬����
    //---------------------------------------------------------------------
    void SceneShaderUtil::makeSubsetIndexArray( SubsetIndexArray* list_, const lnChar* subset_text_ )
    {
        
        const lnChar* c;
        const lnChar* lc; // �Ō�Ɍ������� ',' �܂͂� '-' �̎��̕���
        int   num;
        int   last_num = 0;
        bool  is_range = false;     // - �ɂ���Ĕ͈͎w�肪����ꍇ true
        char  temp[ 64 ];
        u32   size = 0;             // ���ۂɓW�J�����z��̗v�f��
        SubsetIndexOrderList    subse_index_order_list;

        SubsetIndexOrder si;
        SubsetIndexOrder last_si;
        last_si.Index = 0;
        last_si.RangeState = 0;
        
     
        c = subset_text_;
        lc = c;

        // �R���}���I�[�܂ŒT�����[�v
        while ( true )
        {
            if ( *c == _T( ',' ) || *c == _T( '\0' ) )
            {
                strncpy( temp, lc, c - lc );
                temp[ c - lc ] = _T( '\0' );
                num = atoi( temp );
                lc = c + 1;

                si.Index = num;

                // �ЂƂO�̕����� - �̏ꍇ�̓T�u�Z�b�g�I�[�܂ł͈͎̔w��
                if ( *( c - 1 ) == _T( '-' ) )
                {
                    si.RangeState = 2;
                    size += 2;          // ���l�ƁA�I�[�܂ł�\���l 0xffffffff ���i�[���镪
                }
                else
                {
                    si.RangeState = 0;

                    // �ЂƂO����͈͎̔w��̏ꍇ
                    if ( last_si.RangeState == 1 )
                    {
                        size += ( si.Index - last_si.Index ) + 1;   // ���̐������܂ނ̂� + 1  ( i < num �ł͂Ȃ� i <= num )
                    }
                    // �ЂƂ̐��l�̏ꍇ
                    else
                    {
                        ++size;
                    }
                }
                
                subse_index_order_list.push_back( si );
                last_si = si;   // ����Ȃ�����
               
                // �I�[���A�T�u�Z�b�g�̏I�[�܂œǂނ��̂����������ꍇ�͂����ŏI��
                if ( *c == _T( '\0' ) || si.RangeState == 2 )
                {
                    break;
                }
            }
            // - �����������ꍇ�͂ЂƂO�� , (�܂��͐擪) ����𐔒l�ɂ��Ă���
            else if ( *c == _T( '-' ) )
            {
                strncpy( temp, lc, c - lc );
                temp[ c - lc ] = _T( '\0' );
                last_num = atoi( temp );
                is_range = true;
                lc = c + 1;

                SubsetIndexOrder si;
                si.Index = last_num;
                si.RangeState = true;
                subse_index_order_list.push_back( si );

                last_si = si;
            }
            ++c;
        }
       


        // �쐬�Ɏ��s�����ꍇ
        if ( subse_index_order_list.empty() )
        {
            // ��ɂ���
            list_->clear();
        }
        else
        {
            list_->resize( size );

            u32 idx = 0;
            SubsetIndexOrderList::iterator  it1 = subse_index_order_list.begin();
            SubsetIndexOrderList::iterator end1 = subse_index_order_list.end();
            for ( ; it1 != end1;  )
            {
                // ���̒l�܂ł͈͎̔w��
                if ( it1->RangeState == 1 )
                {
                    u32 i = it1->Index;
                    ++it1;

                    // �����T�u�Z�b�g���܂ł͈͎̔w��̏ꍇ
                    if ( it1->RangeState == 2 )
                    {
                        (*list_)[ idx ] = i;
                        ++idx;
                        
                        (*list_)[ idx ] = 0xffffffff;
                        // ��쓮�h�~�̂��߁A�����ŏI���Bit1->RangeState == 2 �̂Ƃ��A�I�[�͕K�� 0xffffffff �ɂȂ�B
                        break;
                    }
                    else
                    {
                        for ( ; ( i <= it1->Index ); ++i )
                        {
                            (*list_)[ idx ] = i;
                            ++idx;
                        }
                        ++it1;
                    }
                }
                // ���ʂ̒l
                else
                {
                    (*list_)[ idx ] = it1->Index;
                    ++idx;
                    ++it1;
                }
            }
        }
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