//=============================================================================
//�y Camera �z
//-----------------------------------------------------------------------------
///**
//  @file       Camera.h
//  @brief      Camera
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Common/Camera.h"
#include "../Common.h"

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
// �� CameraContext
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class CameraContext
{
public:

    CameraContext();
    virtual ~CameraContext();
    
public:

    void initialize( Camera* camera_ );

    virtual void updateContext();

public:

	const LVector4& getPosition() const { return mPosition; }

    const LVector3& getDirection() const { return mDirection; }

	lnfloat getFovY() const { return mFovY; }

	lnfloat getNearClip() const { return mNearClip; }

	lnfloat getFarClip() const { return mFarClip; }

    const LMatrix& getViewMatrix() const { return mViewMatrix; }

    // (�r���{�[�h�s��̌v�Z�Ŏg�p�BsetProjectionMatrix() ��łȂ��Ǝg���Ȃ��̂Œ���)
    const LMatrix& getViewMatrixInverse() const { return mCameraSceneParam.Mat_CameraView_Inverse; }    

    const LNCameraSceneParam& getCameraSceneParam() const { return mCameraSceneParam; }

    LNZSortFunc getZSortType() const { return mZSortFunc; }


    /// SceneGraphContext::OnSetActivePaneContext() ����ݒ肳���
    void setProjectionMatrix( const LMatrix& proj_ );

    const LMatrix& getProjectionMatrix() const { return mProjectionMatrix; }

    /// WORLDVIEWPROJECTION �Z�}���e�B�N�X���̂��߂Ɏg��
    const LMatrix& getViewProjectionMatrix() const { return mViewProjMatrix; }

private:

    Camera*             mCamera;

    LVector4	        mPosition;
    LVector3            mDirection;

    // fovY ���́APane �ɂ��v���W�F�N�V�����s��쐬�̂��߂ɕK�v
    lnfloat             mFovY;
    lnfloat             mNearClip;
    lnfloat             mFarClip;
    
    LMatrix             mViewMatrix;        ///< �r���[�s��
    LMatrix             mProjectionMatrix;  ///< �v���W�F�N�V�����s��
    LMatrix             mViewProjMatrix;    ///< �r���[�s��ƃv���W�F�N�V�����s��̐�

    LNZSortFunc         mZSortFunc;
    LNCameraSceneParam  mCameraSceneParam;  ///< �J�����P�ʂŃV�F�[�_�ɑ���f�[�^

    bool                mProjection2D;
    bool                mOrthographic;
};


//=============================================================================
// �� Camera �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�J�����������N���X
//
//  @note
//              �ECamera = Pane �Ƃ��Ĉ����ׂ��H
//                  
//*/
//=============================================================================
class Camera
    : public Base::ReferenceObject
{
public:
    Camera( SceneGraph* scene_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~Camera();

public:

    // ������
    void initialize( bool projection_2d_ );

    /// �J�����^�C�v�̐ݒ�
	//void setType( LNCameraType type_ ) { mCoreCamera.setType( type_ ); }

	/// �ʒu�̐ݒ�
    void setPosition( const LVector3& pos ) { mCoreCamera.setPosition( pos ); }

    /// �ʒu�̎擾
	const LVector3& getPosition() const { return mCoreCamera.getPosition(); }

	/// �����_�̐ݒ�
	void setLookAt( const LVector3& at ) { mCoreCamera.setLookAt( at ); }

    /// �����_�̎擾
	const LVector3& getLookAt() const { return mCoreCamera.getPosition(); }

	/// ������x�N�g���̐ݒ�
	void setUpDirection( const LVector3& up ) { mCoreCamera.setUpDirection( up ); }

    /// ������x�N�g���̎擾
	const LVector3& getUpDirection() const { return mCoreCamera.getUpDirection(); }

    /// �����̎擾 (���K���ς�)
    const LVector3& getDirection() const { return mCoreCamera.getDirection(); }

    /// Y ��������p�̐ݒ�
	void setFovY( lnfloat fov_y ) { mCoreCamera.setFovY( fov_y ); }

    /// Y ��������p�̎擾
	lnfloat getFovY() const { return mCoreCamera.getFovY(); }

	/// �ł��߂��r���[�v���[���ʒu�̐ݒ�
	void setNearClip( lnfloat near_clip ) { mCoreCamera.setNearClip( near_clip ); }

    /// �ł��߂��r���[�v���[���ʒu�̎擾
	lnfloat getNearClip() const { return mCoreCamera.getNearClip(); }

	/// �ł������r���[�v���[���ʒu�̐ݒ�
	void setFarClip( lnfloat far_clip ) { mCoreCamera.setFarClip( far_clip ); }

    /// �ł������r���[�v���[���ʒu�̎擾
	lnfloat getFarClip() const { return mCoreCamera.getFarClip(); }

    /// �r���[�s��̎擾
    const LMatrix& getViewMatrix() const { return mCoreCamera.getViewMatrix(); }

    /// �v���W�F�N�V�����s��̎擾
	const LMatrix& getProjectionMatrix() const { return mCoreCamera.getProjectionMatrix(); }

    /// �r���[�s��ƃv���W�F�N�V�����s��̐ς̎擾
	const LMatrix& getViewProjectionMatrix() const { return mCoreCamera.getViewProjectionMatrix(); }

    /// �r���[�s��̋t�s��̎擾
	const LMatrix& getViewMatrixInverse() const { return mCoreCamera.getViewMatrixInverse(); }

    /// ������̎擾
    const LViewFrustum& getViewFrustum() const { return mCoreCamera.getViewFrustum(); }

public:

    /// �J��������N���X�̐ݒ� (�Q�ƃJ�E���g�̓C���N�������g���Ȃ����A�f�X�g���N�g���� release())
    void setCameraOperator( Graphics::CameraOperator* camera_operator ) { mCoreCamera.setCameraOperator( camera_operator ); }
    
    /// Z �\�[�g���@�̐ݒ�
    void setZSortType( LNZSortFunc type_ ) { mZSortFunc = type_; }

    /// Z �\�[�g���@�̎擾
    LNZSortFunc getZSortType() const { return mZSortFunc; }

    /// �J�������猩�� 3D ���W�� 2D ���W�ɕϊ�����
    void convertWorldPosToScreenPos( LVector3* out, const LVector3& world_pos ) const;


    LNDrawCoordSpace getCoordSpace() const { return ( mProjection2D ) ? LN_DRAWCOORD_2D : LN_DRAWCOORD_3D; }



    /// 
    void updateContext() { mContext->updateContext(); }

    CameraContext* getCameraContext() { return mContext; }

public:

	/// (Pane �ɃZ�b�g���ꂽ�Ƃ��ɌĂ΂��)
	void setHolderPane( Pane* pane );

    /// �J�����P�ʂŃV�F�[�_�ɑ���f�[�^�̎擾 (�s�񉉎Z�Ȃǂ��낢�����Ă�̂� 1 �t���[��������ĂԂ͔̂����邱��)
    LNCameraSceneParam* getCameraSceneParam();

    /// (called from SceneGraph::onEvent)
    LNRESULT onEvent( const System::Event& args );

protected:

    /*/// ��]����̏���
	void _doMouseMoveR( lnfloat dx_, lnfloat dy_, lnfloat width_, lnfloat height_ );

    /// ���s�ړ��̏��� (dx_ dy_ �ɂ͈ړ��ʂ�ݒ肷��)
	void _doMouseMoveM( lnfloat dx_, lnfloat dy_ );

    /// �}�E�X�z�C�[������̏���
	void _doMouseWheelEvent( int pos_ );*/

protected:

    ///// �}�E�X����֌W�̃f�[�^
    //struct
    //{
    //    Geometry::Point	PrevPos;
    //    bool			RDrag;
    //    bool			MDrag;

    //} mMouseData;

protected:

    SceneGraph*             mScene;
	Pane*					mHolderPane;
    Graphics::Camera        mCoreCamera;

    //LNCameraType	        mType;			    ///< �J�����̓���^�C�v
	//LVector3	            mPosition;		    ///< �J�������W
	//LVector3	            mLookAt;		    ///< �����_
	//LVector3	            mUpDirection;	    ///< �����
 //   LVector3                mDirection;

    u32                     mClearFlag;         ///< LNClearFlag �̑g�ݍ��킹
    /*lnfloat                 mFovY;
    lnfloat                 mNearClip;          ///< �J��������ł��߂��_
    lnfloat                 mFarClip;           ///< �J��������ł������_*/
    bool                    mOrthographic;      ///< ���ߊ��V�~�����[�g�t���O

    //mutable LMatrix         mViewMatrix;        ///< �r���[�s��
    //mutable LMatrix         mProjectionMatrix;  ///< �v���W�F�N�V�����s��
    //mutable LMatrix         mViewProjMatrix;    ///< �r���[�s��ƃv���W�F�N�V�����s��̐�
    //mutable LMatrix         mViewProjMatrixInv; ///< �r���[�s��̋t�s��
    //mutable LViewFrustum    mViewFrustum;

    LNZSortFunc             mZSortFunc;
    LNCameraSceneParam      mCameraSceneParam;  ///< �J�����P�ʂŃV�F�[�_�ɑ���f�[�^
    CameraContext*          mContext;

    mutable bool            mMemberChanged;
     
    bool                    mProjection2D;

    friend class CameraContext;
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================