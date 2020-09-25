//=============================================================================
//�y OffscreenScene �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../SceneGraph.h"
#include "../DrawCommand.h"
#include "SceneShader.h"
#include "SceneShaderManager.h"
#include "OffscreenScene.h"

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
// �� OffscreenScene �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    OffscreenScene::OffscreenScene( SceneShaderManager* manager_, SceneShader* shader_ )
        : mManager      ( manager_ )
        , mSceneShader  ( shader_ )
        , mClearColor   ( 0 )
        , mRTTexture    ( NULL )
        , mIsThereSelf  ( false )
        , mNeedRefreshAffectNodes   ( true )
    {
        LN_SAFE_ADDREF( mManager );
        mManager->addOffscreenScene( this );
    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    OffscreenScene::~OffscreenScene()
    {
        _releaseEffects();

        LN_SAFE_RELEASE( mRTTexture );
        
        if ( mManager )
        {
            mManager->removeOffscreenScene( this );
            LN_SAFE_RELEASE( mManager );
        }
    }

    //---------------------------------------------------------------------
	// �� ������
    //---------------------------------------------------------------------
    LNRESULT OffscreenScene::initialize( const InitData& init_data_ )
    {
        mErrorMessage.clear();
        setClearColor( init_data_.ClearColor );
        setClearDepth( init_data_.ClearDepth );
        setDescription( init_data_.Description );
        setDefaultEffectStrings( init_data_.DefaultEffectStrings );

        mRTTexture = init_data_.RTTexture;
        LN_SAFE_ADDREF( mRTTexture );

        _refreshEffects();

        mNeedRefreshAffectNodes = true;
        return LN_OK;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void OffscreenScene::setClearColor( const LVector4& color_ )
    {
        mClearColor = reinterpret_cast< const LColor& >( color_ ).to32Bit( this->mManager->getGraphicsManager()->getGraphicsAPI() );
    }

    //---------------------------------------------------------------------
	// �� �m�[�h��DefaultEffect �̎w��ƈ�v����ꍇ�ɒǉ�����
    //---------------------------------------------------------------------
    void OffscreenScene::addSceneNode( SceneNode* node_ )
    {
        // �r���[�|�[�g�̏ꍇ�͂͂���
        if ( node_->getSceneNodeType() == LN_SCENENODE_VIEWPORT ) return;

        ln_foreach( DefaultEffectEntry& e, mDefaultEffectEntryList )
        {
            // ���̂܂ܒǉ�
            if ( e.ObjectName == _T( "*" ) )
            {
                if ( e.Type != ENTRY_HIDE )
                {
                    SceneNodeEntry se;
                    se.Node = node_;
                    se.PriorityParameter.Shader = e.Shader;
                    se.PriorityParameter.Hide   = false;
                    mSceneNodeEntryArray.push_back( se );
                }
                return;
            }
            // ���̃I�t�X�N���[��RT�����V�F�[�_���ݒ肳��Ă���m�[�h���g
            else if ( e.ObjectName == _T( "self" ) )
            {
                if ( node_ == mSceneShader->getSelfSceneNode() )
                {
                    if ( e.Type != ENTRY_HIDE )
                    {
                        SceneNodeEntry se;
                        se.Node   = node_;
                        se.PriorityParameter.Shader = e.Shader;
                        se.PriorityParameter.Hide   = false;
                        mSceneNodeEntryArray.push_back( se );
                    }
                    return;
                }
            }
            // ���C���h�J�[�h�t���̔�r
            else if ( Base::StringUtil::match( node_->getName(), e.ObjectName.c_str() ) )
            {
                if ( e.Type != ENTRY_HIDE )
                {
                    SceneNodeEntry se;
                    se.Node   = node_;
                    se.PriorityParameter.Shader = e.Shader;
                    se.PriorityParameter.Hide   = false;
                    mSceneNodeEntryArray.push_back( se );
                }
                return;
            }
        }
    }

    //---------------------------------------------------------------------
	// �� �m�[�h���O��
    //---------------------------------------------------------------------
    void OffscreenScene::removeSceneNode( SceneNode* node_ )
    {
        SceneNodeEntryList::iterator itr = mSceneNodeEntryArray.begin();
        SceneNodeEntryList::iterator end = mSceneNodeEntryArray.end();
        while ( itr != end )
        {
            if ( itr->Node == node_ )
            {
                itr = mSceneNodeEntryArray.erase( itr );
                end = mSceneNodeEntryArray.end();
                continue;
            }
            ++itr;
        }
    }

    //---------------------------------------------------------------------
	// �� �V�[���ɓo�^����Ă��邷�ׂẴm�[�h�� addSceneNode() ���Ă�
    //---------------------------------------------------------------------
    void OffscreenScene::refreshAffectNodes()
    {
        /*
            ���̏����� initialize() �ɏ����̂�NG�B
            �Ȃ����Ƃ����ƁAModel::initalize() �̓����V�F�[�_�̎����ǂݍ��݂ł͂Ȃ��A
            ���[�U�[���g���V�F�[�_����� SceneNode::setShader() �ŃV�F�[�_��ݒ肷��ꍇ��
            �Ή����ł��Ȃ��Ȃ邩��B
            ��initialize() �ŏ�������ꍇ�A���̎��_�� self �Ƃ��Ĉ��� SceneNode ��
              �m��Ȃ��ƂȂ�Ȃ��B�Ή�����ɂ� createSceneshader �� SceneNode ��n���悤��
              ���Ȃ��ƂȂ�Ȃ����ǁAsetShader() ��ʓr�Ăяo���ꍇ��
              �ucreateShader �O�� SceneNode ������Ă����Ȃ��ƃ_���v�����A
              ����ς� createSceneshader �� SceneNode ��n���Ȃ��Ƃ����Ȃ����Ă����̂����_�B
        */

        SceneNodeArray& node_list = mManager->getScene()->getSceneNodeArray();
        ln_foreach( SceneNode* node, node_list )
        {
            addSceneNode( node );
        }
    }

    //---------------------------------------------------------------------
	// �� ���ׂẴI�t�X�N���[��RT�̕`��R�}���h���쐬����
    //---------------------------------------------------------------------
    void OffscreenScene::makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ )
    {
        // ������s�����A���ׂẴm�[�h�ɑ΂��Ă��̃I�t�X�N���[��RT�ŕ`�悷��ׂ������`�F�b�N���đI��
        if ( mNeedRefreshAffectNodes )
        {
            refreshAffectNodes();
            mNeedRefreshAffectNodes = false;
        }

        // �I�t�X�N���[���`�掞�̊�J�������f�t�H���g�̂��̈ȊO�ɂ���Ƃ���
        // ���̕ӂɍX�V�����������Ă���

        // �ȉ��̃m�[�h�z��̎擾���A���O�Ɏ�����J�����O�������
        // �ꗗ�ɂ��čs���΂����ƍœK���ł��邩��


        // ���̃I�t�X�N���[��RT�ŕ`�悷��m�[�h�ɂ͗D��p�����[�^��ݒ�B
        // �`�悵�Ȃ��ꍇ�� hide�B
        SceneNodeArray& node_list = mManager->getScene()->getSceneNodeArray();
        ln_foreach( SceneNode* node, node_list )
        {
            node->setPriorityParameter( &LNPriorityParameter::HIDE_PARAM );
        }
        ln_foreach( SceneNodeEntry& e, mSceneNodeEntryArray )
        {
            //LNPriorityParameter pp;
            //pp.Shader = e.Shader;
            //pp.Hide   = true;       // mSceneNodeEntryArray �ɓ����Ă���m�[�h�͂��ׂ� false ��OK
            e.Node->setPriorityParameter( &e.PriorityParameter );
        }


        // �O����
        container_->add_SetRT( 0, mRTTexture, true );
        container_->add_ClearColor( mClearColor );
        container_->add_ClearDepth( mClearDepth );
        
        // �R�}���h�쐬
        mManager->getScene()->makeSceneNodeDrawCommand(
            mManager->getScene()->getDefaultNode( LN_DEFAULT_SCENENODE_3D ), pass_ );

        // �㏈��
        container_->add_SetRT( 0, NULL, true );
    }

    //---------------------------------------------------------------------
	// �� �G�t�F�N�g�t�@�C���̊��蓖�čX�V
    //---------------------------------------------------------------------
    LNRESULT OffscreenScene::_refreshEffects()
    {
        LNRESULT lr;
        ln_std_tstring tms;
        mIsThereSelf = false;

        _releaseEffects();
        mDefaultEffectEntryList.clear();

        // ; �ŕ��� (�I�[�� ; �̏ꍇ�A����͖���)
        std::vector< ln_std_tstring > entrys = 
            Base::StringUtil::split( mDefaultEffectStrings.c_str(), _T( ";" ) );

        ln_foreach( ln_std_tstring& entry, entrys )
        {
            // ���� = �ŕ���
            std::vector< ln_std_tstring > items = 
                Base::StringUtil::split( entry, _T( "=" ) );

            // ������̗v�f���͕K�� 2�B����ȊO�̓G���[
            if ( items.size() == 2 )
            {
                // �O��̃X�y�[�X���폜
                Base::StringUtil::trimHFSpace( &items[0], items[0] );
                Base::StringUtil::trimHFSpace( &items[1], items[1] );

                DefaultEffectEntry e;
                e.ObjectName = items[0].c_str();

                if ( items[1] == _T( "none" ) )
                {
                    e.Type = ENTRY_NONE;
                    e.Shader = NULL;
                }
                else if ( items[1] == _T( "hide" ) )
                {
                    e.Type = ENTRY_HIDE;
                    e.Shader = NULL;
                }
                else
                {
                    // �K�v�ȃV�F�[�_���쐬
                    SceneShader* shader;
                    LN_CALL_R( mManager->createSceneShader( &shader, items[1].c_str() ) );
                    e.Type       = ENTRY_EFFECT;
                    e.EffectName = items[1].c_str();
                    e.Shader     = shader;
                }

                mDefaultEffectEntryList.push_back( e );

                // "self" ������H
                if ( items[1].c_str() == _T( "self" ) )
                {
                    mIsThereSelf = true;
                }
            }
            else
            {
                mErrorMessage = _T( "DefaultEffect item is disable." );
                return LN_ERR_INVALIDCALL;
            }
        }

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� �I�t�X�N���[�� RT ���ō쐬�����V�F�[�_���������
    //---------------------------------------------------------------------
    void OffscreenScene::_releaseEffects()
    {
        ln_foreach( DefaultEffectEntry& e, mDefaultEffectEntryList )
        {
            LN_SAFE_RELEASE( e.Shader );
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