//=============================================================================
//�y Pane �z
//-----------------------------------------------------------------------------
///**
//  @file       Pane.h
//  @brief      Pane
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Common/ScreenEffect.h"
#include "../../GUI/Controls/GUIDesktop.h"
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
class PanePostDrawContext;

//=============================================================================
// �� Pane �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		
//
//  @note
//              3D�A2D�AGUI ���܂Ƃ߂�B
//              [12/12/12] SceneGraph::makeDrawCommand() �Ƀ��[�g�����n����΂��낢��y�Ȃ̂� Parent - Children ���B
//*/
//=============================================================================
class Pane
    : public Base::ReferenceObject
    //: public SceneNode
{
public:

    /// �R���X�g���N�^
    Pane( SceneGraph* scene_ );

    /// �f�X�g���N�^
    virtual ~Pane();

public:

    /// ������
    void initialize( const Geometry::Size& size_ );//, bool is_3d_ );

	void setViewSize( const Geometry::Size& size );

	const LVector2& getViewSize() const { return mViewSize; }

    void setCamera( Camera* camera_3d_, Camera* camera_2d_ );

    void getCamera( Camera** camera_3d_, Camera** camera_2d_ ) { *camera_3d_ = mCamera3D; *camera_2d_ = mCamera2D; }

    void setRootNode( SceneNode* root_3d_, SceneNode* root_2d_ );

    void getRootNode( SceneNode** root_3d_, SceneNode** root_2d_ );

    /// (�Q�ƃJ�E���g++)
    void setGUIDesktop( GUI::GUIDesktop* desktop_ );

    /// ������ʂ̎g�p�ݒ�
    void setEffectMode( LNScreenEffectMode mode_ );

    void addChildPane( Pane* pane_ );

    void removeChildPane( Pane* pane_ );

	/// �w�i�F�̐ݒ� (������ʗL�����Ɏg�p����BLColor::TRANSPARENCY ��ݒ肷��ƃN���A���Ȃ�)
	void setBackgroundColor( const LColor& color_ ) { mBackgroundColor = color_; }

public:

    /// �F���̐ݒ�
    void setTone( const LTone& tone_, double duration_ ) { mScreenEffect->setTone( tone_, duration_ ); }

    /// �g�����W�V�����̏����Ƃ��Č��݂̉�ʂ��Œ肷��
    void freeze() { mScreenEffect->freeze(); }

    /// �g�����W�V�������s
    void transition( double duration_, Graphics::ITexture* mask_, int vague_ = 40 ) { mScreenEffect->transition( duration_, mask_, vague_ ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �u���[���s
    //
    //  @param[in]  duration_     : �K�p���鎞�� (�b)
    //  @param[in]  power_        : �u���[�̋��� (�u���[�C���[�W��`�悷�鎞�̃A���t�@�l�B0.0�`1.0 �Ŏw��)
    //  @param[in]  scale_        : �u���[�̑傫�� (����)
    //  @param[in]  center_       : �u���[�̒��S���W
    //  @param[in]  center_is_3d_ : center_ �̍��W��񂪎������W��� (true �̏ꍇ�A3D ��ԏ�̓_)
    // 
    //  @par
    //              
    //              duration_ �� 0 ���傫���ꍇ�A�l���̎��Ԃ�������<br>
    //              ���X�� power_ �� 0 �ɖ߂��܂��B
    //              <br>
    //              duration_ �� 0 �̏ꍇ�͋����������ŕω������܂���B<br>
    //              ���ɖ߂����߂ɂ́A������x���̊֐����Ă�� power_ �� 0 �ɂ���K�v������܂��B<br>
    //              <br>
    //              <br>
    //              scale_ �͌��̃X�N���[���C���[�W�ɑ΂��āA1�t���[��������ǂ̒��x�g�傷�邩���w�肵�܂��B<br>
    //              1.0 �ŃX�N���[���Ɠ��{ (�ω��Ȃ�)�A2.0 �ŃX�N���[����2�{�̑傫���ƂȂ�܂��B<br>
    //              scale_ �� 1.0 �̏ꍇ�A�u���[�͕��ˏ�Ɋg�U���܂���B<br>
    //              �ʏ�� 1.0�`1.1 �͈̔͂Ŏw�肷��̂�����ł��B<br>
    //              �܂��A1.0 ��菬�����l���w�肷��Ɠ��������̃u���[�ɂȂ�܂��B<br>
    //              <br>
    //              <br>
    //              center_is_3d_ �́Acenter_ �� 2D ��ԏ�̍��W���A3D ��ԏ�̍��W������ʂ��邽�߂Ɏw�肵�܂��B<br>
    //              <br>
    //              false ���w�肳��Ă���ꍇ�Acenter_ �̒l�����̂܂܃u���[�̒��S���W�ɂȂ�܂��B<br>
    //              ���S���W�̓��C���[�̍�������_(0, 0)�Ƃ��āA�s�N�Z���P�ʂŎw�肵�܂��B<br>
    //              <br>
    //              true ���w�肳��Ă���ꍇ�Acenter_ ��3D��ԏ�̓_�Ƃ��āA���̃��C���[�ɃZ�b�g����Ă���J�������猩��
    //              �X�N���[�����W��̓_�𒆐S�Ƃ��܂��B<br>
    //              <br>    
    //*/
    //---------------------------------------------------------------------
    void blur(
        double duration_,
        lnfloat power_,
        lnfloat scale_,
        const LVector3& center_,
        bool center_is_3d_ );

    /// �t���[���X�V (�g�����W�V�����A�u���\)
    void update( const Game::GameTime& game_time_ );

    /// �u���[�K�p�����𔻒肷��
    bool isBlurEffect() const { return mScreenEffect->isBlurEffect(); }

    /// �G�t�F�N�g�K�p�����𔻒肷��
    bool isEffect() const { return mScreenEffect->isEffect(); }

public:

    void makeDrawCommand( DrawCommandContainer* c_ );

    /// (SceneGraph::update() ����Ă΂��)
    void updateContext();

    PaneContext* getPaneContext() { return mPaneContext; }

    Graphics::ScreenEffect* getScreenEffectRenderer() { return mScreenEffect; }

	bool onEvent( const System::Event& e );

private:

	/// ������ʃ��\�[�X�쐬
    void _createEffectResource();

    /// ������ʃ��\�[�X���
    void _disposeEffectResource();

private:

    SceneGraph*				mSceneGraph;
    LVector2				mViewSize;
    Camera*					mCamera3D;
    Camera*					mCamera2D;
    SceneNode*				mRootNode3D;
    SceneNode*				mRootNode2D;
    GUI::GUIDesktop*		mGUIDesktop;

    PaneArray				mChildPaneArray;
    PaneContext*			mPaneContext;
	PanePostDrawContext*	mPanePostDrawContext;

    Graphics::ScreenEffect* mScreenEffect;
    LColor				    mBackgroundColor;
    LNScreenEffectMode      mEffectMode;

    friend class PaneContext;
	friend class PanePostDrawContext;
};

//=============================================================================
// �� PaneContext
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class PaneContext
    : public DrawingContext
{
public:

    PaneContext();
    ~PaneContext();
    
public:

    void initialize( Pane* pane_ );

    void updateContext();

    const LVector2& getViewSize() const { return mViewSize; }

    void getCameraContext( CameraContext** camera_context_3d_, CameraContext** camera_context_2d_ );

public:

    virtual void draw(
        Graphics::IRenderer* renderer_,
        Graphics::GeometryRenderer* geometry_renderer_ );

private:

    Pane*               mPane;
    LVector2            mViewSize;
    Camera*             mCamera3D;
    Camera*             mCamera2D;
    GUI::GUIDesktop*    mGUIDesktop;
    LNScreenEffectMode  mEffectMode;
};

//=============================================================================
// �� PanePostDrawContext
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class PanePostDrawContext
    : public DrawingContext
{
public:

    PanePostDrawContext();
    ~PanePostDrawContext();
    
public:

    void initialize( Pane* pane_ );

    void updateContext();

public:

    virtual void draw(
        Graphics::IRenderer* renderer_,
        Graphics::GeometryRenderer* geometry_renderer_ );

private:

    Pane*   mPane;

    // �u���\�̌�ɕ`���݂��Ȃ��ƁA�u��ʑS�̂ւ̌��ʁv�͂ł��Ȃ��B
    //  ���u���\�́u���O�̉�ʑS�́v���u�������v�ŕ`�悷�邽�߁B
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