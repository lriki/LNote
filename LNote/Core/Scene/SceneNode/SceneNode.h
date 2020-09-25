//=============================================================================
//�y SceneNode �z
//-----------------------------------------------------------------------------
///**
//  @file       SceneNode.h
//  @brief      SceneNode
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Game/GameTime.h"
#include "../../Graphics/Interface.h"
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

//=============================================================================
// �� SceneNode �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�V�[���m�[�h�̃N���X
//
//  @par
//              2D �I�u�W�F�N�g�̏ꍇ�A�ʒu�� z �l�͈̔͂� 0�`10000 �ł��B
//              �l���傫���قǎ�O�ɕ\������܂��B�͈͊O�̏ꍇ�͕\������܂���B
//
//              �D��x�́A�l�̑傫��������O�ɕ`�悳��܂��B(�ォ�珈������܂�)
//*/
//=============================================================================
class SceneNode
    : public Base::ReferenceObject
{
public:
    /// �R���X�g���N�^ (add_scene_ �� NullNode �p)
    SceneNode( SceneGraph* scene_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~SceneNode();

public:

    /// ������ (�`���Ԃ̐ݒ�̓f�t�H���g�V�F�[�_�̐ݒ�Ȃǂł����g���̂ŁA�p�����initialize() �ł͂ł��邾���擪�ɏ�������)
    //      context_ �ɉ�����n�����ꍇ�Asubset_num_ �͖����B
    LNRESULT initialize( lnU32 subset_num_, LNDrawCoordSpace cs_, SceneNodeContext* context_ = NULL );

    /// �m�[�h���̎擾
    const lnChar* getName() const { return mName.c_str(); }

    /// �m�[�h���̐ݒ�
    LNRESULT setName( const lnChar* name_ );

    /// ���[���h�s��̐ݒ�
    void setMatrix( const LMatrix& matrix_ )        { mMatrix = matrix_; mNeedUpdateMatrix = false; }

    /// ���[���h�s��̎擾
    const LMatrix& getMatrix() const                { return mMatrix; }

    /// �ʒu�̐ݒ�
    void setPosition( const LVector3& pos_ )        { mPosition = pos_; mNeedUpdateMatrix = true; }

    /// �ʒu�̎擾
    const LVector3& getPosition() const             { return mPosition; }

	/// ��]�p�x�̐ݒ�
	void setAngle( const LVector3& angle_ )         { mAngle = angle_; mNeedUpdateMatrix = true; }

    /// ��]�p�x�̎擾
	const LVector3& getAngle() const                { return mAngle; }

	/// �g�嗦�̐ݒ�
	void setScale( const LVector3& scale_ )         { mScale = scale_; mNeedUpdateMatrix = true; }

    /// �g�嗦�̎擾
	const LVector3& getScale() const                { return mScale; }

    /// ���_�̐ݒ�
    void setCenter( const LVector3& center_ )       { mCenter = center_; mNeedUpdateMatrix = true; }

    /// ���_�̎擾
    const LVector3& getCenter() const               { return mCenter; }

    /// ����Ԃ̐ݒ�
    void setVisible( bool flag_ )                   { mIsVisible = flag_; }

    /// ����Ԃ̔���
    bool isVisible() const                          { return mIsVisible; }

    /// �s�����x�̐ݒ�
    void setOpacity( lnfloat opacity_, lnU32 subset_idx_ = 0 )             { mNodeRenderParam.Subset[ subset_idx_ ].Opacity = opacity_; }//mOpacity = opacity_; }

    /// �s�����x�̎擾
    lnfloat getOpacity( lnU32 subset_idx_ = 0 ) const                      { return mNodeRenderParam.Subset[ subset_idx_ ].Opacity; }//return mOpacity; }

    /// ��Z�F�̐ݒ�
    void setColorScale( const LColor& color_, lnU32 subset_idx_ = 0 )      { mNodeRenderParam.Subset[ subset_idx_ ].ColorScale = color_; }          

    /// ��Z�F�̎擾
    const LColor& getColorScale( lnU32 subset_idx_ = 0 ) const             { return mNodeRenderParam.Subset[ subset_idx_ ].ColorScale; }

    /// �u�����h�J���[�̐ݒ�
    void setBlendColor( const LColor& color_, lnU32 subset_idx_ = 0 )      { mNodeRenderParam.Subset[ subset_idx_ ].BlendColor = color_; }

    /// �u�����h�J���[�̎擾
    const LColor& getBlendColor( lnU32 subset_idx_ = 0 ) const             { return mNodeRenderParam.Subset[ subset_idx_ ].BlendColor; }

    /// �F���̐ݒ�
    void setTone( const LTone& tone_, lnU32 subset_idx_ = 0 )              { mNodeRenderParam.Subset[ subset_idx_ ].Tone = tone_; }

    /// �F���̎擾
    const LTone& getTone( lnU32 subset_idx_ = 0 ) const                     { return mNodeRenderParam.Subset[ subset_idx_ ].Tone; }

    /// UV �ϊ��s��̐ݒ�
	void setUVTransform( const LMatrix& matrix_, lnU32 subset_idx_ = 0 )    { mNodeRenderParam.Subset[ subset_idx_ ].UVTransform = matrix_; }

    /// UV �ϊ��s��̐ݒ�
    const LMatrix& getUVTransform( lnU32 subset_idx_ = 0 )  const           { return mNodeRenderParam.Subset[ subset_idx_ ].UVTransform; }

    /// �������@�̐ݒ�
    void setBlendMode( LNBlendMode mode_ )          { mNodeRenderParam.RenderState.BlendMode = mode_; }

    /// �������@�̎擾
    LNBlendMode getBlendMode() const                { return mNodeRenderParam.RenderState.BlendMode; }

    /// �[�x�e�X�g�̗L���ݒ�
    void setEnableDepthTest( bool flag_ )           { mNodeRenderParam.RenderState.DepthTest = flag_; }

    /// �[�x�e�X�g�̗L������
    bool isEnableDepthTest() const                  { return mNodeRenderParam.RenderState.DepthTest; }

    /// �[�xWrite�̗L���ݒ�
    void setEnableDepthWrite( bool flag_ )          { mNodeRenderParam.RenderState.DepthWrite = flag_; }

    /// �[�x�������݂̗L������
    bool isEnableDepthWritet() const                { return mNodeRenderParam.RenderState.DepthWrite; }

    /// �J�����O���@�̐ݒ�
    void setCullingMode( LNCullingMode mode_ )      { mNodeRenderParam.RenderState.Culling = mode_; }

    /// �J�����O���@�̎擾
    LNCullingMode getCullingMode() const            { return mNodeRenderParam.RenderState.Culling; }

    /// ��]�����̐ݒ�
    void setRotateOrder( LNRotOrder order_ )        { mRotOrder = order_; }

    /// ��]�����̎擾
    LNRotOrder getRotateOrder() const               { return mRotOrder; }

    /// �D��x�̐ݒ�
    void setPriority( s32 priority_ )               { mPriority = priority_; }

    /// �D��x�̎擾
    s32 getPriority() const                         { return mPriority; }
  
    /// �r���{�[�h��Ԃ̐ݒ�
    void setBillboardType( LNBillboardType type_ )  { mBillboardType = type_; }

    /// �r���{�[�h��Ԃ̎擾
    LNBillboardType getBillboardType() const        { return mBillboardType; }

    /// �����X�V�̗L���ݒ�
    void setEnableAutoUpdate( bool flag_ )          { mIsAutoUpdate = flag_; }

    /// �����X�V�̗L������
    bool isEnableAutoUpdate()                       { return mIsAutoUpdate; }

    /// �V�F�[�_�̐ݒ�
    void setShader( SceneShader* shader_ );

    /// �V�F�[�_�̎擾
    SceneShader* getShader()                        { return mShader; }

public:

    /// �m�[�h�Ɋ֘A�t�����Ă������Ȓl���擾����
    //virtual LNRESULT getItemData( const lnChar* item_name_, LMatrix* out_ );

    /// �q�m�[�h��ǉ�����
    LNRESULT addChild( SceneNode* node_ );

    /// �q�m�[�h���O��
    LNRESULT removeChild( SceneNode* node_ );

    /// ���[���h�s����K�w�I�ɍX�V���� (SceneGraph::update() ����Ă΂��BContext �쐬���Ƃ肠����������)
    void updateTransformHierarchy( const LMatrix& parent_marix_, const NodeRenderParam* parent_params_ );

    /// �J�����P�ʂ̕`����̍X�V
    void updateParamFromCameraAndLight( Camera* camera_ );

    /// �~�D��p�����[�^�̐ݒ� (���͕̂K���`��I�����܂ő��݂��Ă��邱��)
    //      makeDrawCommand() �ɓn�������ł������Ǝv�������ǁA
    //      ���ꂾ�ƃI�t�X�N���[��RT�ł̕`��m�[�h�̎d�������K�w�`��
    //      �ɑΉ��ł��Ȃ��Ȃ�B
    void setPriorityParameter( LNPriorityParameter* param_ ) { mPriorityParameter = param_; }//mNodeRenderParam.setPriorityParameter( param_ ); }

    /// �`��R�}���h���K�w�I�ɍ쐬����
    virtual void makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ );

    /// �`�悷����W���
    LNDrawCoordSpace getDrawCoordSpace() const { return mDrawCoordSpace; }

    /// �`��p�����[�^�̎擾
    NodeRenderParam* getNodeRenderParam() { return &mNodeRenderParam; }

    /// �T�u�Z�b�g���̎擾 (0 �̃m�[�h�͕`�悵�Ȃ�)
    lnU32 getSubsetNum() const { return mSubsetNum; }

    SceneNodeContext* getContext() { return mContext; }

    void updateContext();

public: // �� �p����Ŏ�������

    /// �m�[�h�̎�ނ̎擾
    virtual LNSceneNodeType getSceneNodeType() const = 0;

    /// �t���[���X�V
    virtual void update( const Game::GameTime& time_ ) { }

    /// �T�u�Z�b�g��`�悷��
    virtual LNRESULT drawSubset( lnU32 index_ ) = 0;

    /// �f�o�b�O�p�̕`��
    virtual void drawDebug( lnU32 flags_ ) { };

private:

    /// Z �\�[�g�p�̔�r�֐�
    static bool _cmpZAndPrioritySort( const SceneNode* l_, const SceneNode* r_ );

protected:

    SceneGraph*         mSceneGraph;
    LNDrawCoordSpace    mDrawCoordSpace;
    lnU32               mSubsetNum;
    NodeRenderParam     mNodeRenderParam;

    SceneNode*          mParentNode;            ///< �e�m�[�h
    SceneNodeArray      mChildNodeList;         ///< �q�m�[�h�̃��X�g

    SceneNodeContext*   mContext;

    LRefTString         mName;
    LMatrix             mMatrix;                ///< ���[���h�ϊ��s��
    LMatrix             mCombinedWorldMatrix;   ///< �s��̊K�w�X�V�ō쐬����錋���ς݃��[���h�s��
    LVector3            mPosition;
    LVector3            mAngle;
    LVector3            mScale;
    LVector3            mCenter;
    LNRotOrder          mRotOrder;          ///< ��]����
    lnS32               mPriority;          ///< �D��x
    LNBillboardType     mBillboardType;
    SceneShader*        mShader;
    lnfloat             mZDistance;
    LightArray          mApplyLights;       ///< �`��Ɏg�����C�g (setShader() �ŕK�v�ȃ��C�g���� resize() ����Ă���)
    
    LNPriorityParameter*     mPriorityParameter;

    bool                mIsVisible;         ///< �����
    bool                mNeedUpdateMatrix;
    bool                mIsAutoUpdate;
    bool                mEnableDraw;        ///< mIsVisible �� false ��������A�J�����O�ŏ��O���ꂽ�ꍇ�� false

    friend class SceneNodeContext;
  
};

//=============================================================================
// �� SceneNodeContext
//-----------------------------------------------------------------------------
///**
//  @brief
//  @note
//              Context �̎g�p�菇�c
//                  1. SceneNodeContext ���p�������N���X���`
//                  2. updateContext() �ŕۑ����郁���o�ϐ����R�s�[����
//                  3. SceneNode::initialize() �ɁA�h���R���e�L�X�g�̃|�C���^��n��
//                  �� ��������͕K�v�Ȃ�
//*/
//=============================================================================
class SceneNodeContext
    //: public Base::ReferenceObject
{
public:

    SceneNodeContext();
    virtual ~SceneNodeContext();
    
public:

    void initialize( SceneNode* node_, lnU32 subset_num_ );

    /// [�����t�F�[�Y]
    virtual void updateContext( SceneNodeContext* parent_context_ );

    /// [�����t�F�[�Y] (�Ƃ肠���� updateContext() ����Ă�ł���)
    virtual void releaseAllResource();

    /// [�`��X���b�h]
    void setPriorityParameter( LNPriorityParameter* param_ ) { mPriorityParameter = param_; }

    /// [�`��X���b�h]
    virtual void OnSetActivePaneContext( SceneGraphContext* scene_context_, PaneContext* pane_context_, CameraContext* camera_context_ );

    /// [�`��X���b�h] Node �P�ʂ̃p�����[�^���V�F�[�_�ɐݒ肷�鎞�ɌĂяo�����
    virtual void onSetNodeParameter( SceneShader* shader_ ) {}

    /// [�`��X���b�h] (���� Model �݂̂Ŏg�p���B�ق��̂����ɁA����ŕ`�悷��悤�ɂ�����)
    virtual void drawSubset( lnU32 index_ ) {}

public:

    lnU32 getSubsetNum() const { return mSubsetNum; }

    SceneShader* getShader() { return mShader; }

    const LMatrix& getCombinedWorldMatrix() const { return mCombinedWorldMatrix; }

    // out_ �ɒl���i�[�����ꍇ�� true ��Ԃ�
    bool getObjectMatrix( LMatrix* out_, MMEVariableRequest req_, int light_idx_ ) const;

    // SceneShader::updateNodeParam() �p
    const LightContextArray&    getAffectLightArray() const { return mAffectLightContextArray; }

    // SceneShader::updateSubsetParam() �p
    const LNSubsetRenderParam&  getSubsetRenderParam( int index_ ) const { return mSubsetRenderParams[index_]; }

    const LRenderState&         getRenderState() const { return mRenderState; }

    CameraContext* getCameraContext() { return mAffectCameraContext; }

private:

    static bool _cmpZAndPrioritySort( const SceneNodeContext* l_, const SceneNodeContext* r_ );

protected:

    SceneNode*              mSceneNode;
    SceneNodeContextArray   mChildSceneNodeContextArray;
    lnU32                   mSubsetNum;
    LNSubsetRenderParam*    mSubsetRenderParams;
    LRenderState            mRenderState;
    LNPriorityParameter*    mPriorityParameter;
    LMatrix                 mCombinedWorldMatrix;   ///< �s��̊K�w�X�V�ō쐬����錋���ς݃��[���h�s��
    lnS32                   mPriority;
    LNBillboardType         mBillboardType;
    SceneShader*            mShader;
    lnfloat                 mZDistance;
    LightContextArray       mAffectLightContextArray;           ///< �`��Ɏg�����C�g
    CameraContext*          mAffectCameraContext;
    bool                    mIsVisible;

    
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