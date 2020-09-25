//=============================================================================
//�y SceneGraph �z
//-----------------------------------------------------------------------------
///**
//  @file       SceneGraph.h
//  @brief      SceneGraph
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <algorithm>
#include "../Base/STLUtil.h"
#include "../Geometry/Vector2.h"
#include "../System/Interface.h"
#include "../Game/GameTime.h"
#include "../Graphics/Manager.h"
#include "Common.h"
#include "DrawCommand.h"
#include "SceneNode/DummyNode.h"
#include "SceneNode/Viewport.h"
#include "SceneNode/Pane.h"

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
// �� SceneGraph �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �V�[���O���t
//*/
//=============================================================================
class SceneGraph
    : public Base::ReferenceObject
{
public:

    struct InitData
    {
        Base::LogFile*      LogFile;
        File::Manager*      FileManager;
        Graphics::Manager*  GraphicsManager;
        Effect::Manager*    EffectManager;
        Physics::Manager*   PhysicsManager;
        LVector2            VirtualViewSize;
        lnU32               ShaderCacheSize;
    };

public:
    SceneGraph( Manager* manager_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~SceneGraph();

public:

    /// ������
    LNRESULT initialize( const InitData& init_data_ );

    /// �I������
    void finalize();

 //   /// ���z�X�N���[���T�C�Y�̐ݒ� (�E�B���h�E���T�C�Y�����ɌĂяo���B��{�I�Ɋe�G�f�B�^�p)
	////		�o�b�N�o�b�t�@�T�C�Y�܂ŕύX����ꍇ�A���̌�Ƀf�o�C�X���X�g�C�x���g�𔭐������邱�ƁB
	////		OnResetDevice() �ł��ׂẴr���[�|�[�g�Ȃǂ�RT����蒼���Ă���B
 //   void setVirtualScreenSize( const Geometry::Size& size_ );

 //   /// ���z�X�N���[���T�C�Y�̎擾
 //   const LVector2& getVirtualScreenSize() const { return mVirtualViewSize; }

	/// SceneNode ���������ɐݒ肳��郌���_�����O�X�e�[�g�̐ݒ�
	void setNodeDefaultRenderState( const LRenderState& state_ ) { mNodeDefaultRenderState = state_; }

	/// SceneNode ���������ɐݒ肳��郌���_�����O�X�e�[�g�̎擾
	const LRenderState& getNodeDefaultRenderState() { return mNodeDefaultRenderState; }

    /// �V�F�[�_�Ǘ��N���X�̎擾
    SceneShaderManager* getSceneShaderManager() const { return mSceneShaderManager; }

    /// �f�t�H���g�ō쐬�����m�[�h�̎擾
    Viewport* getDefaultNode( LNDefaultSceneNode node_idx_ ) { return mDefaultSceneNodes[ node_idx_ ]; }

    /// �f�t�H���g�ō쐬�����J�����̎擾 (0:3D 1:2D)
    Camera* getDefaultCamera( int type_ = 0 );

    /// �f�t�H���g�ō쐬����郉�C�g�̎擾
    Light* getDefaultLight() { return mDefaultLight; }
    
    /// �f�t�H���g�ō쐬�����V�F�[�_�v���O�����̎擾
    SceneShader* getDefaultShader( LNDefaultSceneShader shader_idx_ ) { return mDefaultShaderArray[ shader_idx_ ]; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �t���[���X�V (makeDrawCommand() �̑O�Ɉ�x�Ă�) 
    //
    //  @note       
    //              [�����t�F�[�Y]
    //              �V�[�����ŁA�`��m�[�h�A�J�����Ɉˑ����Ȃ�
    //              �O���[�o���ȃp�����[�^���ŐV�̏�ԂɍX�V����B<br>
    //              ��̓I�ɂ́A<br>
    //                  - �f�t�H���g���C�g�̏�Ԃ�S�V�F�[�_�ɃZ�b�g
    //                  - ���ԁA�}�E�X����S�V�F�[�_�ɃZ�b�g
    //                  - ���ׂẴm�[�h�̃��[���h�s��̊K�w�X�V
    //              <br>
    //              �����J�������g���Ƃ� (�����v���C���[�p�̃r���[�Ȃ�)�A
    //              1�t���[�����ɏd�����Ĕ������鏈�����܂Ƃ߂������B
    //              ���̊֐��� 1 �t���[���� 1 �x�����Ăяo���� OK�B
    //*/
    //---------------------------------------------------------------------
    void update( const Game::GameTime& game_time_ );

    /// [�����t�F�[�Y][�`�悪�K�v�Ȏ�]
    void updateAllContext();

    Pane* getDefaultPane() { return mDefaultRootPane; }

    /// [�����t�F�[�Y] ��A�̗���� update �� �K�v�ȕ����� makeDrawCommand() �� render();
    void makeDrawCommand( Pane* pane_ );







    /// �V�F�[�_�̃J�����n�p�����[�^�ɃZ�b�g����J�����̐ݒ� (NULL ��n���ƃf�t�H���g�J�����ōX�V)
    void setActiveCamera( Camera* camera_ ) { mActiveCamera = ( camera_ ) ? camera_ : mDefaultCamera; }

    /// ������ (�J�����܂��̓V���h�E�p���C�g) �X�V (NULL ��n����ActiveCamera�ōX�V)
    void updateViewFrustum( Camera* camera_ );

    /// ���ׂẴI�t�X�N���[��RT�̕`��R�}���h�쐬
    void makeOfscreenSceneDrawCommand();

    //---------------------------------------------------------------------
    ///**
    //  @brief      �`��R�}���h���쐬����
    //
    //  @param[in]  root_node_ : ���̃m�[�h�ȉ���`�悷�� (NULL �̏ꍇ�͂��ׂẴ��[�g)
    //  @param[in]  camera_    : �`��Ɏg���J���� (NULL �̏ꍇ�̓f�t�H���g�J����)
    //
    //  @note       
    //              �󂯎�����m�[�h�ȉ��� camera_ ���g�����`��R�}���h���쐬�E�ǉ�����B
    //              ������Ăяo���΂��̕��L���[�Ƀv�b�V������Ă����B
    //              �쐬�����R�}���h�� render() ���Ăяo�����Ƃ��ɂ��ׂĎ��s����A
    //              ��������ƃL���[�̓N���A�����B
    //*/
    //---------------------------------------------------------------------
    LNRESULT makeDrawCommand( SceneNode* root_node_ );//, Camera* camera_ );

    /// SceneNode �̕`��R�}���h���쐬���� (makeDrawCommand()�AOffscreenScene::makeDrawCommand() ����Ă΂��)
    void makeSceneNodeDrawCommand( SceneNode* root_node_, MMDPass pass_ );

    /// �S�m�[�h�� drawDebug() ���Ăяo��
    //      3D����B�K�w�`��ł͂Ȃ��_�ɒ��ӁBflags_ �� LNSceneDebugDrawFlag �̑g�ݍ��킹�B
    //      getGeometryRenderer() �Ŏ擾�ł��� GeometryRenderer �� setViewProjMatrix() �ɁA
    //      �A�N�e�B�u�J�����̍s�񂪐ݒ肳���B
    void makeSceneNodeDebugDrawCommand( SceneNode* root_node_, lnU32 flags_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �`��R�}���h�����s����
    //
    //  @note       
    //              �`��p�̃X���b�h�ŕ`�悷��ꍇ�A�R�}���h�쐬���
    //              ���̊֐����Ăяo�������� OK�B
    //*/
    //---------------------------------------------------------------------
    LNRESULT render();

public:

    /// ���b�Z�[�W���� (Manager ����Ă΂��)
    LNRESULT onEvent( const System::Event& args_ );

    /// FileManager �̎擾
    File::Manager* getFileManager() { return mFileManager; }

    /// GraphicsManager �̎擾
    Graphics::Manager* getGraphicsManager() { return mGraphicsManager; }

    /// GeometryRenderer �̎擾
    Graphics::GeometryRenderer* getGeometryRenderer() { return mGeometryRenderer; }

    /// EffectManager �̎擾
    Effect::Manager* getEffectManager() { return mEffectManager; }

    /// PhysicsManager �̎擾
    Physics::Manager* getPhysicsManager() { return mPhysicsManager; }

    /// (SceneNode �̃R���X�g���N�^����Ă΂��B�c���[�ł͂Ȃ��Ǘ����X�g�ɒǉ�����)
    void addNode( SceneNode* node_ );

    /// (SceneNode �̃f�X�g���N�^����Ă΂��)
    void onDeleteNode( SceneNode* node_ );//removeNode( SceneNode* node_ );

    /// (Camera �̃R���X�g���N�^����Ă΂��)
    void addCamera( Camera* camera_ ) { mCameraList.push_back( camera_ ); }

    /// (Camera �̃f�X�g���N�^����Ă΂��)
    void removeCamera( Camera* camera_ ) { Base::STLUtil::remove( mCameraList, camera_ ); }

    /// (Light �̃R���X�g���N�^����Ă΂��)
    void addLight( Light* light_ ) { mLightList.push_back( light_ ); }

    /// (Light �̃f�X�g���N�^����Ă΂��)
    void removeLight( Light* light_ ) { Base::STLUtil::remove( mLightList, light_ ); }

    /// (Viewport �̃R���X�g���N�^����Ă΂��)
    void addViewport( Viewport* vp_ ) { mViewportList.push_back( vp_ ); }

    /// (Viewport �̃f�X�g���N�^����Ă΂��)
    void removeViewport( Viewport* vp_ ) { mViewportList.remove( vp_ ); }

    /// (Pane �̃R���X�g���N�^����Ă΂��)
    void addPane( Pane* pane_ ) { mPaneList.push_back( pane_ ); }

    /// (Pane �̃f�X�g���N�^����Ă΂��)
    void removePane( Pane* pane_ ) { Base::STLUtil::remove( mPaneList, pane_ ); }

    /// SceneNode �̖��O��ύX���钼�O�ɌĂяo�� (SceneNode �� setName() ����Ă΂��)
    void onSetNodeName( SceneNode* node_, const lnChar* old_name_, const lnChar* new_name_ );

    /// �L�����C�g�̒����烉�C�g��񋓂��� 
    //      ref_pos_ : ��ʒu
    //      makeDrawCommand() ���� SceneNode::updateParamFromCameraAndLight() �o�R�ŌĂ΂��B
    //      ���C�g�� out_->size() �̐������i�[����B���炩���� resize ���Ă������ƁB
    void enumLights( LightArray* out_, const LVector3& ref_pos_ );

    /// SceneShader �̃f�X�g���N�^����Ă΂��
    ViewportList& getViewportList() { return mViewportList; }

    /// SceneNode �ꗗ�̎擾
    SceneNodeArray& getSceneNodeArray() { return mSceneNodeList; }

    BatchSpriteProxy* getBatchSpriteProxy( bool is_3d_ ) { return (is_3d_) ? mBatchSpriteProxy3D : mBatchSpriteProxy2D; }

private:

    struct ActiveLightData
    {
        Light*      ActiveLight;
        lnfloat     DistanceToCamera;   ///< �A�N�e�B�u�ȃJ��������̋�����2��
        lnfloat     Distance;           ///< �\�[�g�p�̊���� (�\�[�g���O�ɐݒ肷��B�A�N�e�B�u�ȃJ��������̋�����2��)
    };

    typedef std::vector< ActiveLightData > ActiveLightDataArray;
    

    /// ���C�g�񋓗p�̔�r�֐�
    static bool _cmpLights( const ActiveLightData& l_, const ActiveLightData& r_ );

private:

    Base::LogFile*          mLogFile;
    Manager*                mManager;
    File::Manager*          mFileManager;
    Graphics::Manager*      mGraphicsManager;
    Graphics::GeometryRenderer* mGeometryRenderer;
    Effect::Manager*        mEffectManager;
    Physics::Manager*       mPhysicsManager;
    LVector2                mVirtualViewSize;	// �폜�\��
	LRenderState			mNodeDefaultRenderState;
    SceneShaderManager*     mSceneShaderManager;

    //SceneNodeList           mNodeRegistOrderList;
    SceneNodeArray          mSceneNodeList;     ///< �쐬����Ă��� SceneObject �̃|�C���^���X�g
    SceneNodeNameMap        mNodeNameMap;       ///< �쐬����Ă��� SceneObject �̂����A���O�������̂̑Ή��\
    SceneNodeArray          mSceneNodeAdditionalList;
    CameraList              mCameraList;
    LightList               mLightList;
    ViewportList            mViewportList;
    PaneArray               mPaneList;

    Viewport*               mDefaultSceneNodes[ LN_MAX_DEFAULT_NODES ];
    Camera*                 mDefaultCamera;
    Camera*                 mDefault2DCamera;
    Light*                  mDefaultLight;
    SceneShader*            mDefaultShaderArray[ LN_MAX_DEFAULT_SHADERS ];
    Pane*                   mDefaultRootPane;
    //Pane*                   mDefault3DPane;
    //Pane*                   mDefault2DPane;
    Camera*                 mActiveCamera;
    Camera*                 mActiveViewFrustum;
    ActiveLightDataArray    mActiveLightDataArray;  ///< �L���ȃ��C�g�̈ꗗ (makeDrawCommand() �ō����)

    EffectBatchProxy*       mEffect3DBatchProxy;
    EffectBatchProxy*       mEffect2DBatchProxy;
    BatchSpriteProxy*       mBatchSpriteProxy3D;
    BatchSpriteProxy*       mBatchSpriteProxy2D;
    PhysicsDebugDrawProxy*  mPhysicsDebugDrawProxy;

    DrawCommandContainer    mDrawCommand;    ///< �`��R�}���h�̊i�[�ꏊ
    SceneGraphContext*      mSceneGraphContext;
    
    friend class SceneGraphContext;
};

//=============================================================================
// �� SceneGraphContext
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class SceneGraphContext
{
public:

    SceneGraphContext();

public:

    void initialize( SceneGraph* scene_ );

    void updateContext();

    void selectLights( LightContextArray* out_, const LVector3& ref_pos_ );

    void OnSetActivePaneContext( PaneContext* pane_context_, bool is_3d_ );

private:

    static bool _cmpLights( const LightContext* l_, const LightContext* r_ );

private:

    SceneGraph*             mSceneGraph;
    LVector2                mVirtualViewSize;
    CameraContextArray      mCameraContextArray;    // �J�����⃉�C�g�� AdditionalList �� Scene �ɍ��ΕK�v�Ȃ����ǁc
    LightContextArray       mLightContextArray;
    LightContextArray       mActiveLightContextArray;   ///< OnSetActivePaneContext() �ł̎�����J�����O�Ŋi�[
    LViewFrustum            mActiveViewFrustum;         ///< OnSetActivePaneContext() �ł̎�����J�����O�Ŋi�[
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================