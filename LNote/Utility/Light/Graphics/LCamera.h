//=============================================================================
//�y LCamera �z
//-----------------------------------------------------------------------------
///**
//  @file       LCamera.h
//  @brief      LCamera
//  @version    1.0
//  @date       2012/2/7
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Core/Scene/Manager.h"
#include "../../Core/Scene/SceneGraph.h"
#include "../../Core/Scene/SceneNode/Camera.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// �� LCamera �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �J�����̃N���X
//*/
//=============================================================================
class LCamera
    : public LRefClass< LCamera, Core::Scene::Camera >
{
public:

    /// �f�t�H���g�J�����̎擾
    static LCamera getDefaultCamera();

public:

    /// �ʒu�̎擾
    const LVector3& getPosition() const;

	/// �ʒu�̐ݒ�
    void setPosition( const LVector3& position_ );

	/// �����_�̎擾
    const LVector3& getLookAt() const;

	/// �����_�̐ݒ�
    void setLookAt( const LVector3& at_ );

	/// ������x�N�g���̎擾
    const LVector3& getUpDirection() const;

	/// ������x�N�g���̐ݒ�
    void setUpDirection( const LVector3& up_ );

    /// Y ��������p�̎擾
    lnfloat getFovY() const;

	/// Y ��������p�̐ݒ�
    void setFovY( lnfloat fov_y_ );

    /// �ł��߂��r���[�v���[���ʒu�̎擾
    lnfloat getNearClip() const;

	/// �ł��߂��r���[�v���[���ʒu�̐ݒ�
    void setNearClip( lnfloat near_ );

    /// �ł������r���[�v���[���ʒu�̎擾
    lnfloat getFarClip() const;

	/// �ł������r���[�v���[���ʒu�̐ݒ�
    void setFarClip( lnfloat far_ );

    /// �J�����^�C�v�̐ݒ�
    void setType( LNCameraType type_ );

    /// �J�������猩�� 3D ���W�� 2D ���W�ɕϊ�����
    void convertWorldPosToScreenPos( LVector3* out_, const LVector3& world_pos_ );

    /// �r���[�s��̎擾
    const LMatrix& getViewMatrix();

    /// �v���W�F�N�V�����s��̎擾
    const LMatrix& getProjectionMatrix();

    /// �r���[�s��ƃv���W�F�N�V�����s��̐ς̎擾
    const LMatrix& getViewProjectionMatrix();

    /// �r���[�s��̋t�s��̎擾
    const LMatrix& getViewMatrixInverse();

    /// ���ˉe�s��̎擾
    const LMatrix& getOrthoMatrix();


    LN_UTIL_REFCLASS_METHODS( LCamera, Core::Scene::Camera );
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================